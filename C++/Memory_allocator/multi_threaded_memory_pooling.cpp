template <class POOLTYPE, class LOCK>
class MTMemoryPool {
public:
// Allocate an element from the freeList.
inline void* alloc (size_t size);
// Return an element to the freeList.
inline void free (void* someElement);
private:
POOLTYPE stPool; // Single-threaded pool.
LOCK theLock;
};


template <class M, class L>
inline
void* MTMemoryPool<M,L>::alloc (size_t size)
{
void * mem;
theLock.lock();
mem = stPool.alloc(size);
theLock.unlock();
return mem;
}

template <class M, class L>inline
void MTMemoryPool<M,L>::free (void* doomed)
{
theLock.lock();
stPool.free(doomed);
theLock.unlock();
}


class ABCLock {// Abstract base class
public:
virtual ~ABCLock() {}
virtual void lock() = 0;
virtual void unlock() = 0;
};
class MutexLock : public ABCLock {
public:
MutexLock() {pthread_mutex_init(&lock, NULL);}
~MutexLock() {pthread_mutex_destroy(&lock);}
inline void lock() {pthread_mutex_lock(&lock);}
inline void unlock() {pthread_mutex_unlock(&lock);}
private:
pthread_mutex_t lock;
};


class Rational {
public:
...
static void newMemPool() {
memPool = new MTMemoryPool <MemoryPool <Rational>,
MutexLock>;
}
private:
...
static MTMemoryPool <MemoryPool<Rational>, MutexLock> *memPool;
};


//Faster locking

class PrimitiveLock : public ABCLock {
public:
PrimitiveLock() {
_clear_lock((static_cast<atomic_p>) &_lock,
LOCK_FREE);}
~PrimitiveLock() {}
inline void lock() { // Spin lock
while (!_check_lock((static_cast<atomic_p>)&_lock,
LOCK_FREE, LOCK_BUSY));
}
inline void unlock() {
_clear_lock((static_cast<atomic_p>) &_lock, LOCK_FREE);
}
private:
int _lock;
enum {LOCK_FREE = 0, LOCK_BUSY = 1};
};


class Rational {
public:
...
static void newMemPool() {
memPool = new MTMempryPool <MemoryPool <Rational>,
PrimitiveLock>;
}
private:
...
static MTMemoryPool < MemoryPool<Rational>, PrimitiveLock>
*memPool;
};


class DummyLock : public ABCLock {
public:
inline void lock() {}
inline void unlock() {}
};

MTMemoryPool <MemoryPool<Rational>, DummyLock> myRationalPool;
class Rational {
public:
...
static void newMemPool() {
memPool = new MTMemoryPool <ByteMemoryPool, MutexLock>;
}
private:
...
static MTMemoryPool <ByteMemoryPool, MutexLock> *memPool;
};

