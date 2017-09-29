
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


    
