/*
Simulate a virtual friend function.

Friend functions are often needed in C++. A canonical example is that of types that can be printed to output streams (e.g., std::cout). An overloaded left-shift operator function, which is often a friend, is needed to achieve seamless streaming capabilities. Friend functions are really an extension of the class's interface. However, friend functions in C++ can not be declared virtual and therefore no dynamic binding of friend functions is possible.

Applying a friend function to an entire hierarchy of classes becomes awkward if an overloaded friend function is needed for every class in the hierarchy. This lack of support for dynamic binding makes it hard to justify that friend functions are in fact an extension of the class's interface. Virtual friend function idiom addresses this concern elegantly.
*/


class Base {
  public:
    friend ostream& operator << (ostream& o, const Base& b);
    // ...
  protected:
    virtual void print(ostream& o) const
    { ... }
};
/* make sure to put this function into the header file */
inline std::ostream& operator<< (std::ostream& o, const Base& b)
{
  b.print(o); // delegate the work to a polymorphic member function.
  return o;
}

class Derived : public Base {
  protected:
    virtual void print(ostream& o) const
    { ... }
};
