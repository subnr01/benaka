/*
Achieve lazy copy optimization. Like lazy initialization, do the work just
when you need because of efficiency.

*/


/*
Copying an object can sometimes cause a performance penalty. If objects are frequently copied but infrequently modified later, copy-on-write can provide significant optimization. To implement copy-on-write, a smart pointer to the real content is used to encapsulate the object's value, and on each modification an object reference count is checked; if the object is referenced more than once, a copy of the content is created before modification.
*/


template <class T>
class CowPtr
{
    public:
        typedef boost::shared_ptr<T> RefPtr;

    private:
        RefPtr m_sp;

        void detach()
        {
            T* tmp = m_sp.get();
            if( !( tmp == 0 || m_sp.unique() ) ) {
                m_sp = RefPtr( new T( *tmp ) );
            }
        }

    public:
        CowPtr(T* t)
            :   m_sp(t)
        {}
        CowPtr(const RefPtr& refptr)
            :   m_sp(refptr)
        {}
        const T& operator*() const
        {
            return *m_sp;
        }
        T& operator*()
        {
            detach();
            return *m_sp;
        }
        const T* operator->() const
        {
            return m_sp.operator->();
        }
        T* operator->()
        {
            detach();
            return m_sp.operator->();
        }
};


/*

CowPtr<String> s1 = "Hello";
char &c = s1->operator[](4); // Non-const detachment does nothing here
CowPtr<String> s2(s1); // Lazy-copy, shared state
c = '!'; // Uh-oh

what is the abvoe problem
*/
