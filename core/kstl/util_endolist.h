
/*
This class provides a list container for objects that
contain their own iterator information.
Semanics similar to the STL list container.
*/
namespace util {
   struct endolist node;
   template< class _Tp, endolist_node _Tp::*_nodename> class endolist;
   template< class _Tp, endolist_node _Tp::*_nodename, class _Ref, class _Ptr> struct _endolist_iterator;
}


/*
This must exist as a public data member
in any class used to comprise a list
*/
struct endolist_node : private Noncopyable
{
    endolist_node* _M_prev;
    endolist_node* _M_next;
    
    endolist_node() :_M_prev(NULL), _M_next(NULL) {}
    ~endolist_node() { ASSERT(!is_linked()); }
    
    void reset()
    {
        _M_prev = _M_next = NULL;
    }
    
    bool is_linked() const 
    {
        return _M_next != NULL || _M_prev != NULL;
    }
    
};


/*
   Endolist iterator base class
*/
struct _endolist_iterator
{

};

/*
   Endolist template class
*/
class endolist : private Noncopyable
{

};
   
