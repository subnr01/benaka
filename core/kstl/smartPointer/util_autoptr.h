/*
This class does the same thing as the traditional C++ autoptr.
Destruction of the object manipulates delete operator, while
assignment from one autoptr to another, causes a transfer
of ownership.

Example:
  AutoPtr<int> int_ap(new int(3));
  // do not need explicit call delete on int_ap since
  // delete will be called by the AutoPtr reference.

*/


namespace util 
{
  template<class T> class AutoPtr;
  template<class T> class AutoPtrRef;
};


template <class T>
class AutoPtr
{
  public:
    
