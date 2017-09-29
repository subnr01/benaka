/*
This is a simple factory method that
can be used to pre-allocated objects which might be needed
in the event they are needed.

This is great when the work which will need the object 
can only determine whether the object is needed while holding
a lock. This is a percpu factory so pre-population
must be done by the same CPU which will be performing the
allocation.

*/


template <class T> class PerCpuFactory : Noncopyable
{
public:
  PerCpuFactory() {}
  ~PerCpuFactory() { reset(); }
  
  bool pre_fill(const size_t n = 0) 
  {
    int my_proc = sk_my_processor_id();
    return _arrays[my_proc].populate(n);
  }
  
  T* allocate()
  {
    int my_proc = sk_my_processor_id();
    return _arrays[my_proc].allocate(n);
  }
  
  void recycle(T *t)
  {
    int my_proc = sk_my_processor_id();
    _arrays[my_proc].recycle();
  }
  
  void reset()
  {
    for (size_t i = 0; i < PSM_PROCESSORS_MAX; i++)
    {
      _arrays[i].reset();
    }
  }
 private:
  ArrayFactory<T> _arrays[PSM_PROCESSORS_MAX];
};

      
    
    
  
  
  
