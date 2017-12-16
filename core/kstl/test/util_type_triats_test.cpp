/*
 * Copyright (c) 2015, NetApp, Inc.
 * All rights reserved.
 */

#include <inttypes.h>

#include <cxxtest/TestSuite.h>
#include <util_type_traits.h>

using namespace util;

class TypeTraitsTest : public CxxTest::TestSuite
{
public:
    void test_remove_cv();
    void test_integral_constant();
    void test_is_integral();
    void test_is_pod();
    void test_is_pointer();
    void test_is_trivially_copyable();
    void test_void_type_sfinae();
};

namespace {

// Test classes for POD checking
// is pod
// is trivially_copyable
struct Pod {
    int x;
    int y;
};

// not pod
// is trivially_copyable
class WithTrivialInheritance : public Pod {
public:
    uint64_t a;
};


// not pod
// is trivially_copyable
class WithConstructor {
public:
    WithConstructor(int x = 0, int y = 0) :
        x_(x), y_(y)
    {}

    int x() const { return x_; }
    int y() const { return y_; }

private:
    int x_;
    int y_;
};

// not pod
// not trivially copyable
class WithCopyConstructor {
public:
    WithCopyConstructor(int x = 0, int y = 0) :
        x_(x), y_(y)
    {}
    WithCopyConstructor(const WithCopyConstructor& o) :
        x_(o.x_), y_(o.y_)
    {}

    int x() const { return x_; }
    int y() const { return y_; }

private:
    int x_;
    int y_;
};

// not pod
// not trivially_copyable
class WithCopyAssignment {
public:
    WithCopyAssignment(int x = 0, int y = 0) :
        x_(x), y_(y)
    {}

    WithCopyAssignment& operator=(const WithCopyAssignment& o)
    {
        x_ = o.x_;
        y_ = o.y_;
        return *this;
    }

    int x() const { return x_; }
    int y() const { return y_; }

private:
    int x_;
    int y_;
};

class WithVirtual {
public:
    virtual void foo() = 0;
    virtual ~WithVirtual() {};
};

// not pod
// not trivially_copyable
class WithPolymorphicInheritance : public WithVirtual {
public:
    virtual void foo() { std::cout << "Hello World!\n"; }
};

// not pod
// not trivially_copyable
class WithDestructor {
public:
    ~WithDestructor() { std::cout << "Goodbye World\n"; }
};

// not pod
// not trivially_copyable
class WithNonTrivialMember {
public:
    WithCopyConstructor& member() { return member_; }
private:
    WithCopyConstructor member_;
};

// Test classes for void_type SFINAE testing
// This is an example class that the default will match anything and
// have a false value member...
template <class T, class U = void>
struct HasTypeTypedef : public util::false_type
{
};

// While the specialization matches anything where T::type is defined,
// making the value member be true.
template <class T>
struct HasTypeTypedef<T, typename util::void_type<typename T::type>::type>
    : public util::true_type
{
};

// Test class that has a type member typedef.
struct HasType {
    typedef HasType type;
};

} // namespace

void TypeTraitsTest::test_remove_cv()
{
    bool result = !is_same<const volatile int, int>::value;
    TS_ASSERT(result);

    TS_ASSERT((is_same<remove_const<int>::type, int>::value));
    TS_ASSERT((is_same<remove_const<const int>::type, int>::value));

    TS_ASSERT((is_same<remove_volatile<char>::type, char>::value));
    TS_ASSERT((is_same<remove_volatile<volatile char>::type, char>::value));

    TS_ASSERT((is_same<remove_cv<unsigned int>::type, unsigned int>::value));
    TS_ASSERT((is_same<remove_cv<const unsigned int>::type, unsigned int>::value));
    TS_ASSERT(
        (is_same<remove_cv<volatile unsigned int>::type, unsigned int>::value));
    TS_ASSERT((
        is_same<remove_cv<const volatile unsigned int>::type, unsigned int>::value));
}

// Verify the true_type and false_type base instances.
void TypeTraitsTest::test_integral_constant()
{
    TS_ASSERT(true_type::value);
    TS_ASSERT(!false_type::value);
}

// Verify the various is_integral tests, including
// the cv-qualifications.
void TypeTraitsTest::test_is_integral()
{
    TS_ASSERT(is_integral<char>::value);
    TS_ASSERT(is_integral<unsigned>::value);
    TS_ASSERT(is_integral<const int>::value);
    TS_ASSERT(is_integral<volatile uint64_t>::value);
    TS_ASSERT(is_integral<const volatile int64_t>::value);

    // Reals aren't integral
    TS_ASSERT(!is_integral<float>::value);
    TS_ASSERT(!is_integral<double>::value);

    // Nor are structs/classes.
    TS_ASSERT(!is_integral<Pod>::value);
    TS_ASSERT(!is_integral<WithConstructor>::value);
}

// Verify the POD rules tests
void TypeTraitsTest::test_is_pod()
{
    // Tests normally run under clang, but gcov still doesn't, so
    // guard this test or it'll fail building the gcov variant
#ifdef __clang__
    // Scalars are POD
    TS_ASSERT(is_pod<unsigned>::value);
    // So are POD structs
    TS_ASSERT(is_pod<Pod>::value);

    // Classes with a constructor are not POD.
    TS_ASSERT(!is_pod<WithConstructor>::value);
    // Classes with a copy constructor are not TC
    TS_ASSERT(!is_pod<WithCopyConstructor>::value);
    // Classes with a copy assignment are not TC
    TS_ASSERT(!is_pod<WithCopyAssignment>::value);
    // Classes with a destructor are not POD.
    TS_ASSERT(!is_pod<WithDestructor>::value);
    // Classes with members in base and super are not POD
    TS_ASSERT(!is_pod<WithTrivialInheritance>::value);
    // Classes with virtual methods are not POD
    TS_ASSERT(!is_pod<WithVirtual>::value);
    // Classes with a virtual base are not POD
    TS_ASSERT(!is_pod<WithPolymorphicInheritance>::value);
    // Classes with a non-pod member are not POD
    TS_ASSERT(!is_pod<WithNonTrivialMember>::value);
#endif
}

void TypeTraitsTest::test_is_pointer()
{
    TS_ASSERT(!is_pointer<int>::value);
    TS_ASSERT(!is_pointer<const int>::value);
    TS_ASSERT(!is_pointer<const volatile int>::value);

    TS_ASSERT(!is_pointer<int&>::value);
    TS_ASSERT(!is_pointer<const int&>::value);
    TS_ASSERT(!is_pointer<const volatile int&>::value);

    TS_ASSERT(is_pointer<int*>::value);
    TS_ASSERT(is_pointer<const int*>::value);
    TS_ASSERT(is_pointer<volatile int*>::value);

    TS_ASSERT(!is_pointer<Pod>::value);
    TS_ASSERT(!is_pointer<const Pod>::value);
    TS_ASSERT(!is_pointer<const volatile Pod>::value);

    TS_ASSERT(is_pointer<Pod*>::value);
    TS_ASSERT(is_pointer<const Pod*>::value);
    TS_ASSERT(is_pointer<volatile Pod*>::value);
}

// Verify the TC rules
void TypeTraitsTest::test_is_trivially_copyable()
{
#ifdef __clang__
    // Scalars are TC
    TS_ASSERT(is_trivially_copyable<unsigned>::value);
    // So are POD structs
    TS_ASSERT(is_trivially_copyable<Pod>::value);
    // So are simple classes with just a constructor
    TS_ASSERT(is_trivially_copyable<WithConstructor>::value);
    // So are trivial inheritance hierarchies
    TS_ASSERT(is_trivially_copyable<WithTrivialInheritance>::value);

    // Classes with a copy constructor are not TC
    TS_ASSERT(!is_trivially_copyable<WithCopyConstructor>::value);
    // Classes with a copy assignment are not TC
    TS_ASSERT(!is_trivially_copyable<WithCopyAssignment>::value);
    // Classes with a destructor are not TC.
    TS_ASSERT(!is_trivially_copyable<WithDestructor>::value);
    // Classes with virtual methods are not TC
    TS_ASSERT(!is_trivially_copyable<WithVirtual>::value);
    // Classes with a virtual base are not TC
    TS_ASSERT(!is_trivially_copyable<WithPolymorphicInheritance>::value);
    // Classes with a non-TC member are not TC
    TS_ASSERT(!is_trivially_copyable<WithNonTrivialMember>::value);
#endif
}

// Verify the void_type SFINAE magic.
void TypeTraitsTest::test_void_type_sfinae()
{
    // These have a type member typedef.
    TS_ASSERT(HasTypeTypedef<HasType>::value);
    TS_ASSERT(HasTypeTypedef<void_type<int> >::value);

    // These do not.
    TS_ASSERT(!HasTypeTypedef<int>::value);
    TS_ASSERT(!HasTypeTypedef<Pod>::value);
}
