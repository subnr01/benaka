/*
To create a copy of an object or a new object without knowing its concrete type.
*/


/*
C++ natively supports polymorphic destruction of objects using a virtual destructor. An equivalent support for creation and copying of objects is missing. In C++, creation of object(s) always requires its type to be known at compile-time. The Virtual Constructor idiom allows polymorphic creation of and copying of objects in C++.

*/


class Employee 
{
  public:
    virtual ~Employee () {}                 // Native support for polymorphic destruction.
    virtual Employee * create () const = 0; // Virtual constructor (creation) 
    virtual Employee * clone () const = 0;  // Virtual constructor (copying) 
};
class Manager : public Employee     // "is-a" relationship
{
  public:
    Manager ();                     // Default constructor
    Manager (Manager const &);      // Copy constructor
    virtual ~Manager () {}                  // Destructor
    Manager * create () const       // Virtual constructor (creation) 
    {
      return new Manager();
    }
    Manager * clone () const        // Virtual constructor (copying) 
    {
      return new Manager (*this);
    }
};
class Programmer : public Employee { /* Very similar to the Manager class */ };
Employee * duplicate (Employee const & e)
{
   return e.clone();  // Using virtual constructor idiom.
}


//what is this
class Employee
{
  public:
    typedef std::tr1::shared_ptr<Employee> Ptr;
    virtual ~Employee () {}                    // Native support for polymorphic destruction.
    virtual Ptr create () const = 0; // Virtual constructor (creation)
    virtual Ptr clone () const = 0;  // Virtual constructor (copying)
};
class Manager : public Employee     // "is-a" relationship
{
  public:
    Manager () {}                     // Default constructor
    Manager (Manager const &) {}      // Copy constructor
    virtual ~Manager () {}
    Ptr create () const       // Virtual constructor (creation)
    {
      return Ptr(new Manager());
    }
    Ptr clone () const        // Virtual constructor (copying)
    {
      return Ptr(new Manager (*this));
    }
};




