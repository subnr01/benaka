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
    
    explicit AutoPtr(T* p = 0) : _ptr(p) {}
  
    AutoPtr (AutoPtr &ap) : _ptr(ap.release())  {}
  
    template<class D >
    AutoPtr (AutoPtr<D> &ap) : _ptr(ap.release())  {}
      
    AutoPtr (AutoPtrRef<T> ref) : _ptr(ref._ptr)  {}
    
    ~AutoPtr() {reset(); }
  
    //OPERATORS
    AutoPtr&  operator=(AutoPtr &rhs)
    {
      reset(rhs.release());
      return *this;
    }
  
    template< class D >
    AutoPtr&  operator=(AutoPtr<D> &rhs)
    {
      reset(rhs.release());
      return *this;
    }
    
    AutoPtr&  operator=(AutoPtrRef<T> &rhs)
    {
      reset(rhs._ptr);
      return *this;
    }
  
    T* operator->() const
    {
      ASSERT(_ptr);
      return _ptr;
    }
  
    T& operator*() const
    {
      ASSERT(_ptr);
      return *_ptr;
    }
  
    template<class D>
    operator AutoPtr<D>() { return AutoPtr<D>(release()); }
  
    template<class D>
    operator AutoPtrRef<D>() { return AutoPtrRef<D>(release()); }
    
    //OPERATIONS
    void reset(T *p =0)
    {
      if (_ptr != p) {
        if (_ptr) {
          delete _ptr;
        }
        _ptr = p;
      }
    }
  
    T* release ()
    {
      T *p = _ptr;
      _ptr = 0;
      return p;
    }
  
    //ACCESS
    T *get() const { return _ptr; }
 
  private:
    //DATA MEMBERS
    T *ptr;
  
};

/*
Class that allows implicit conversions for copying and
assignment of auto_ptr's between each other
*/

template<class T>
class AutoPtrRef
{
  public:
    explicit AutoPtrRef(T *p) : _ptr(p) {}
  
    T *ptr;
};

  
    
     
   
    
