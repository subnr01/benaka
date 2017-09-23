/*
 Referenceable provides a simple increment/decrement reference semantics with
 an atomic count. When the count reaches zero, the pure virtual _delete()
 method is called. Classes which wish to be accessed via smart pointers 
 can be Referenceable and implement an appropriate _delete method 
 */


namespace util
{
  class Referenceable;
};



class Referenceable : virtual protected Noncopyable
{
