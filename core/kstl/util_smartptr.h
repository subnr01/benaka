/*

The SmartPtr templated class is used to hold a refCounted pointer.

The template parameter should have the incRef() and the decRef() defined
for incrementing and decrementing the reference count.
The Referenceable class can be used a simple implementation of this behavior.

*/


namespace util
{
    template<class T> class SmartPtr;
}


template<class T>
class SmartPtr : private NonCopyable
{
   public:
      SmartPtr() : _ptr(0);
      SmartPtr(T *p) : _ptr(0) { _assign(p); }
      
      SmartPtr(const SmartPtr& sp) : _ptr(0) { _assign(sp.get());
      
