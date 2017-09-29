/*

Memory Allocator



*/


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

inline size_t
Allocator::size()
{
  return _count;
}
  
