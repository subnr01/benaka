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
  
  
  
