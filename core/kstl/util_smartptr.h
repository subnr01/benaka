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
      
      SmartPtr(const SmartPtr& sp) : _ptr(0) { _assign(sp.get());}
      template<typename D>
      SmartPtr(const SmartPtr<D>& sp) : _ptr(0) { _assign(sp.get());}
      
      ~SmartPtr() { release(); }
 
      
      //OPERATORS
      SmartPtr& operator=(T *p) { return _assign(p);}
      SmartPtr& operator=(const Smartptr& rhs) { return _assign(rhs.get()); }
      template<typename D>
      SmartPtr& operator=(const Smartptr<D>& rhs) { return _assign(rhs.get()); }
      
      operator T*() const { return _ptr; }
      T* operator->() const { return _ptr; }
      T& operator*() const { return *_ptr; }
      
      void release() { _assign(0); }
      
      T *get() const { return _ptr; }
      
   private:
      SmartPtr & _assign(T* p)
      {
            if (_ptr != p) {
                if (p) {
                    p->incRef();
                }
                
                T* tmp = _ptr;
                _ptr = p;
                
                /*
                Now that we reassigned ptr value,
                release our hold on the old value.
                The decRef() needs to be after the 
                reassignment of _ptr so that the old 
                value's delete() does not interact 
                with the operation of this function
                */
                
                if (tmp)
                {
                    temp->decRef();
                }
             }
             return *this;
       }
        
        
       //DATA MEMBERS
       T *_ptr;
 };


            
                
      
      
      
      
  
      
