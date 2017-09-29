/*

Memory Allocator



*/

static const size_t MIN_ALLOCATION  = 4096;
static const size_t MAX_ALLOCATION  = 1048576;

class Allocator
{
  public:
    Allocator(size_t elementSize, size_t elementCount);
    size_t size();
    void* pop();
    
  protected:
    struct FreeListElement {
      FreeListElement *next;
    } *_freeList;
  
  size_t count;
};

Allocator::
Allocator(size_t elementSize, size_t elementCount)
{
  unintptr_t const pc = (uintptr_t) RETURN_ADDRESS;
  
  VERIFY(elementSize >= sizeof(FreeListElement));
  
  _freelist = NULL;
  _count = 0;
  
  while (_count < elementCount)
  {
    size_t allocBytes = elementSize * (elementCount - _count);
    
    if (allocBytes < MIN_ALLOCATION) {
      allocBytes = MIN_ALLOCATION;
    }
    
    if (allocBytes > MAX_ALLOCATION) {
      allocBytes = MAX_ALLOCATION;
    }
    
    if (allocBytes < elementSize) {
      allocBytes = elementSize;
    }
    
    
    size_t allocCount = allocBytes / elementSize;
    allocBytes = allocCount * elementSize;
    
    char *blockP = (char*) sk_allocate_memory_nopanic_pc(allocBytes, M_SK_UTIL, pc, 0);
    
    while (blockP == NULL &&
           allocCount >= 2 &&
           allocBytes >= MIN_ALLOCATION * 2)
    {
      /*
      Unable to allocate, so retry with smaller
      */
      allocCount /= 2;
      allocBytes = allocCount * elementSize;
      blockP = (char*) sk_allocate_memory_nopanic_pc(allocBytes, M_SK_UTIL, pc, 0);
    }
    
    if (blockP == NULL)
    { 
      //out of memory
      break;
    }
    
    char *pp = blockP;
    
    for (size_t ii = 0; ii < allocCount; ++ii; pp == elementSize)
    {
      FreeListElement *fle = reinterpret_cast<FreeListElement *> (pp);
      fle->next = _freeList;
      _freeList = fle;
      _count++;
    }
  }
}

//The allocation routine that we use SK_ALLOC, is intended 
//for allocations that are never released and there is 
// in face no SK_FREE. But since Factories data structures are 
//indeed global and only destroyed during shutdown
//it is largely irrevelant.

inline size_t
Allocator::size()
{
  return _count;
}

inline void* 
Allocator::pop()
{
  FreeListElement *fle = _freeList;
  if (fle != NULL) {
    _freeList = fle->next;
    fle->next = NULL;
    _count--;
  }
  return reinterpret_cast<void*>(fle);
}

  



