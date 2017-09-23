/*
This class creates a singleton of the client

*/


namespace util {
  template<typename T> class Singleton;
};


template<typename T>
class Singleton : virtual protected Noncopyable
{
    public:
      //STATEFUNCTION
      static T* instance() { return &_t; }
      
    protected:
      //LIFECYCLE
      Singleton() throw() {}
      
      virtual ~Singleton() throw() {}
      
      private:
        //STATIC MEMBER
        static T _t;
};

//STATIC MEMBER
template<typename T> T Singleton<T>::_t;

