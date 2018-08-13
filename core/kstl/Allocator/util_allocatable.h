/*

Util::Allocatable provides a base class for use by any kernel-resident
class that the user wishes to dynamically allocate. In Ontap, the default
implementation of new can block indefinitely. Allocatable provides a
non blocking implementation via kmem_alloc with KMA_NOSLEEP.

In addition, it follows the nothrow exception semantics, which means users
of Allocatable objects can expect calls to new to return NULL instead
of throwing.

Being kmem_alloc(), we still cannot new() while holding 
any locks at all, thanks to the over-generalisation 
in lib/kma.c

*/
#define RETURN_ADDRESS  __builtin_return_address(0u)

template <class TAG, util::InjectModule INJ_MOD, util::InjectInstance INJ_INST>
class Allocatable {
   public:
    void* operator new(size_t size) throw();
    void* operator new[](size_t size) throw();
    void* operator new(size_t size, uintptr_t pc, uint32_t km_flags) throw();
    void* operator new[](size_t size, uintptr_t pc, uint32_t km_flags) throw();
    void operator delete(void *pp);
    void operator delete[](void *pp);
  
  private:
    static void* _new_common(size_t size, uintptr_t pc, uint32_t km_flags = SKMA_NOWAIT) throw();
};


inline void* Allocatable::
operator new(size_t size) throw()
{
  uintptr_t const pc = (uintptr_t) RETURN_ADDRESS;
  return _new_common(size, pc);
}

inline void* Allocatable::
operator new[](size_t size) throw()
{
  uintptr_t const pc = (uintptr_t) RETURN_ADDRESS;
  return _new_common(size, pc);
}


inline void* Allocatable::
operator new(size_t size, uintptr_t pc, uint32_t km_flags) throw()
{
  return _new_common(size, pc);
}

inline void* Allocatable::
operator new[](size_t size, uintptr_t pc, uint32_t km_flags) throw()
{
  return _new_common(size, pc);
}

inline void* Allocatable::
_new_common(size_t size, uintptr_t pc, uint32_t km_flags) throw()
{
  return sk_kmem_alloc_pc(size, km_flags, pc);
}

void Allocatable::
operator delete(void *pp)
{
  uintptr_t const pc = (uintptr_t) RETURN_ADDRESS;
  sk_kmem_free_pc(pp, oc);
}

void Allocatable::
operator delete[](void *pp)
{
  uintptr_t const pc = (uintptr_t) RETURN_ADDRESS;
  sk_kmem_free_pc(pp, oc);
}










