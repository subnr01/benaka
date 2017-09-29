
/*
This class exists to provide a fountain of individually usable and 
freeable objects where a high cycling rate is unlikely. Objects
allocated through it can be freed through the standard delete
operator. Objects allocated through the system new operator
may be freed through it.

*/


template <class T> class ArrayFactory: private Noncopyable {
  public:
    ArrayFactory(); _pointers(NULL), _capacity(0), _count(0) {}
    ~ArrayFactory();
    bool resize(const size_t n);
    bool populate(const size_t n);
    void recycle(T *element);
    T *allocate();
    void reset();
 private:
    T **_pointers;
    unsigned int _capacity;
    unsigned int _count;
};


    
template <class T>
bool ArrayFactory::resize(const size_t n)
{
  if (_capacity >= n) {
    return true;
  }
  
  T **temp = NULL;
  
  const size_t bytes_needed = (n * sizeof(T*));
  
  if (IN_WAFL) {
    DECLARE_CAN_SUSPEND();
    temp = (T**)wafl_kmem_malloc_common(bytes_needed, NULL, M_UTIL, (uintptr_t)RETURN_ADDRESS);
  } else {
    temp = (T**)sk_kmem_malloc(bytes_needed, M_UTIL, SKMA_NOWAIT);
  }
  
  if (temp == NULL) {
    return false;
  }
  
  if (_pointers != NULL) {
    for (size_t i = 0; i < count; i++) {
      temp[i] = _pointers[i];
      _pointers[i] = NULL;
    }
    sk_kmem_free(_pointers);
  }
  
  _pointers = temp;
  _capacity = n;
  
  return true;
}

template <class T>
bool ArrayFactory::populate(const size_t n)
{
  if (n <= count)
  {
    return true;
  }
  
  if (resize(n) == false) {
    return false;
  }
  
  while(_count < n) {
    T *temp;
    temp = new T;
    if (temp == NULL) {
      return false;
    }
    recycle(temp);
  }
  return true;
}

template<class T>
void ArrayFactory<T>::recycle(T *element)
{
  ASSERT(element != NULL);
  
  if(_count >= _capacity) {
    delete element;
    return;
  }
  
  _pointers[_count] = element;
  ++_count;
}

template<class T>
T* ArrayFactory<T>::allocate()
{
  if(_count == 0) {
    return NULL;
  }
  
  --_count;
  
  T *temp = _pointers[_count];
  _pointers[_count] = NULL;
  return temp;
}

template<class T>
void ArrayFactory<T>::reset()
{
  if (_pointers == NULL)
  {
    return;
  }
  
  for(size_t i = 0; i < count; i++)
  {
    delete _pointers[i];
  }
  
  _count = 0;
  _capacity = 0;
  
  sk_mem_free(_pointers);
  _pointers = NULL;
}

template<class T>
ArrayFactory<T>::~ArrayFactory()
{
  reset();
}



