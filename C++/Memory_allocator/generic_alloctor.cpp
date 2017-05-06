template < class T >
class MemoryPool {
public:
MemoryPool (size_t size = EXPANSION_SIZE);
~MemoryPool ();
// Allocate a T element from the free list.
inline void* alloc (size_t size);
// Return a T element to the free list.
inline void free (void *someElement);
private:
// next element on the free list.
MemoryPool<T> *next;
// If the freeList is empty, expand it by this amount.enum { EXPANSION_SIZE = 32};
// Add free elements to the free list
void expandTheFreeList(int howMany = EXPANSION_SIZE);
};

template < class T >
MemoryPool < T > :: MemoryPool (size_t size)
{
expandTheFreeList(size);
}

template < class T >
MemoryPool < T > :: ~MemoryPool ()
{
MemoryPool<T> *nextPtr = next;
for (nextPtr = next; nextPtr != NULL; nextPtr = next) {
next = next->next;
delete [] nextPtr;
}
}

template < class T >
inline
void* MemoryPool < T > :: alloc (size_t)
{
if (!next) {
expandTheFreeList();
}
MemoryPool<T> *head = next;
next = head->next;
return head;
}

template < class T >
inline
void MemoryPool < T > :: free (void *doomed)
{
MemoryPool<T> *head = static_cast <MemoryPool<T> *> doomed;
head->next = next;
next = head;
}

template < class T >
void MemoryPool < T > :: expandTheFreeList(int howMany)
{
// We must allocate an object large enough to contain the
// next pointer.
size_t size = (sizeof(T) > sizeof(MemoryPool<T> *)) ?
sizeof(T) : sizeof(MemoryPool<T> *);
MemoryPool<T> *runner = static_cast <MemoryPool<T> *> new char
[size];
next = runner;
for (int i = 0; i < howMany ; i++) {
runner->next =
static_cast <MemoryPool<T> *> new char [size];
runner = runner->next;
}
runner->next = 0;
}

class Rational {
public:
Rational (int a = 0, int b = 1 ) : n(a), d(b) {}
void *operator new(size_t size) { return memPool->alloc(size); }
void operator delete(void *doomed,size_t size)
{ memPool->free(doomed); }
static void newMemPool() { memPool = new MemoryPool <Rational>; }
static void deleteMemPool() { delete memPool; }
private:
int n; // Numerator
int d; // Denominator
static MemoryPool <Rational> *memPool;
};
