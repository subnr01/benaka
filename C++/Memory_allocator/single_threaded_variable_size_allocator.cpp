class MemoryChunk {
public:
MemoryChunk (MemoryChunk *nextChunk, size_t chunkSize);
~MemoryChunk() {delete mem; }
inline void *alloc (size_t size);
inline void free (void* someElement);
// Pointer to next memory chunk on the list.
MemoryChunk *nextMemChunk() {return next;}
// How much space do we have left on this memory chunk?
size_t spaceAvailable()
{ return chunkSize - bytesAlreadyAllocated; }
// this is the default size of a single memory chunk.
enum { DEFAULT_CHUNK_SIZE = 4096 };
private:
MemoryChunk *next;
void *mem;
// The size of a single memory chunk.
size_t chunkSize;
// This many bytes already allocated on the current memory chunk.
size_t bytesAlreadyAllocated;
};



MemoryChunk::MemoryChunk(MemoryChunk *nextChunk, size_t reqSize)
{
chunkSize = (reqSize > DEFAULT_CHUNK_SIZE) ?
reqSize : DEFAULT_CHUNK_SIZE;
next = nextChunk;
bytesAlreadyAllocated = 0;
mem = new char [chunkSize];
}


MemoryChunk :: ~MemoryChunk() { delete [] mem; }

void* MemoryChunk :: alloc (size_t requestSize)
{
void *addr = static_cast <void*>
(static_cast <size_t> mem + bytesAlreadyAllocated);
bytesAlreadyAllocated += requestSize;
return addr;
}

inline void MemoryChunk :: free (void *doomed) {}

class ByteMemoryPool {
public:
ByteMemoryPool (size_t initSize =
MemoryChunk::DEFAULT_CHUNK_SIZE);
~ByteMemoryPool ();
// Allocate memory from private pool.
inline void *alloc (size_t size);
// Free memory previously allocated from the pool
inline void free (void* someElement);
private:
// A list of memory chunks. This is our private storage.
MemoryChunk *listOfMemoryChunks;
// Add one memory chunk to our private storage
void expandStorage(size_t reqSize);
};

// Construct the ByteMemoryPool object. Build the private storage.
ByteMemoryPool :: ByteMemoryPool (size_t initSize)
{
expandStorage(initSize);
}

ByteMemoryPool :: ~ByteMemoryPool ()
{
MemoryChunk *memChunk = listOfMemoryChunks;
while (memChunk) {
listOfMemoryChunks = memChunk->nextMemChunk();
delete memChunk;
memChunk = listOfMemoryChunks;
}
}

void* ByteMemoryPool :: alloc (size_t requestSize)
{
size_t space = listOfMemoryChunks->spaceAvailable();
if ( space < requestSize ) {
expandStorage(requestSize);
}
return listOfMemoryChunks->alloc(requestSize);
}

inline
void ByteMemoryPool :: free (void *doomed)
{
listOfMemoryChunks->free(doomed);
}


void ByteMemoryPool :: expandStorage(size_t reqSize)
{
listOfMemoryChunks = new MemoryChunk(listOfMemoryChunks, reqSize);
}


class Rational {
public:
Rational (int a = 0, int b = 1 ) : n(a), d(b) {}
void *operator new(size_t size) {return memPool->alloc(size);}
void operator delete(void *doomed,size_t size)
{memPool->free(doomed);}
static void newMemPool() { memPool = new ByteMemoryPool;}
static void deleteMemPool() { delete memPool; }
private:
int n; // Numerator
int d; // Denominator
static ByteMemoryPool *memPool;
};

MemoryPool <Rational> *Rational::memPool = 0;
int main()
{
...
Rational *array[1000];
Rational::newMemPool();
// Start timing here
for (int j = 0; j < 500; j++) {
for (int i = 0; i < 1000; i++) {
array[i] = new Rational(i);
}
for (i = 0; i < 1000; i++) {
delete array[i];
}
}
// Stop timing here
Rational::deleteMemPool();
...
}


