
/*
 *
 * Copyright (c) 2006-2013 NetApp, Inc.
 * All rights reserved.
 *
 */

#ifndef UTIL_ENDOLIST_H
#define UTIL_ENDOLIST_H

// SYSTEM INCLUDES
//
#if defined(UNIT_TEST) || defined(__linux__)
#include <stddef.h>
#else /* UNIT_TEST || __linux__ */
#include <sys/stddef.h> // for NULL
#endif /* UNIT_TEST || __linux__ */

// PROJECT INCLUDES
//
#ifdef SCSI_BLADE
#include <ics/ics_assert.h>
#endif

#ifdef UNIT_TEST
#include <unit_test.h>
#endif /* UNIT_TEST */

#ifndef ASSERT
#include <foundation/sk_debug.h>
#endif
#include <util_noncopyable.h>


namespace util
{
    struct endolist_node;
    template<class _Tp, endolist_node _Tp::*_nodename> class endolist;
    template<class _Tp, endolist_node _Tp::*_nodename, class _Ref, class _Ptr>
        struct _endolist_iterator;
}

/**
 * util::endolist
 *
 * This class provides a no-allocation list container for objects which
 * contain their own iterator information.  The semantics of an endolist
 * closely match those of the STL's std::list container.
 */

// --------------------------------------------------------------------------
// The embedded list_node class which must exist as a public data member in
// any class used to comprise a list.
struct util::endolist_node : private util::Noncopyable
{
    endolist_node();
    ~endolist_node();

    void reset();
    bool is_linked() const;

    endolist_node* _M_next;
    endolist_node* _M_prev;
};

inline util::endolist_node::
endolist_node()
    : _M_next(NULL),
      _M_prev(NULL)
{
}

inline util::endolist_node::
~endolist_node()
{
    ASSERT(!is_linked());
}

inline void util::endolist_node::
reset()
{
    _M_next = _M_prev = NULL;
}

inline bool util::endolist_node::
is_linked() const
{
    return _M_next != NULL || _M_prev != NULL;
}

#define GNUC_PREREQ(maj,min) \
    ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))


// --------------------------------------------------------------------------
// The endolist iterator base class.
template<class _Tp,
         util::endolist_node _Tp::*_nodename,
         class _Ref,
         class _Ptr>
struct util::_endolist_iterator
{
    typedef _endolist_iterator<_Tp,
                               _nodename,
                               _Tp&,
                               _Tp*>             iterator;
    typedef _endolist_iterator<_Tp,
                               _nodename,
                               const _Tp&,
                               const _Tp*>       const_iterator;
    typedef _endolist_iterator<_Tp,
                               _nodename,
                               _Ref,
                               _Ptr>             _Self;

    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef endolist_node _Node;
    typedef size_t size_type;

    _Node* _M_node;

    _endolist_iterator(_Node* __x) : _M_node(__x) { ASSERT(_M_node != NULL); }
    _endolist_iterator(const _Node* __x) : _M_node(const_cast<_Node*>(__x)) { ASSERT(_M_node != NULL); }
    _endolist_iterator() : _M_node(0) {}
    _endolist_iterator(const iterator& __x) : _M_node(__x._M_node) { ASSERT(_M_node != NULL); }

    bool operator==(const _Self& __x) const { return _M_node == __x._M_node; }
    bool operator!=(const _Self& __x) const { return _M_node != __x._M_node; }

    // ----------------------------------------------------------------------
    // From FreeBSD.org:
    //    GNU/76069: FreeBSD's definition of offsetof isn't good for C++
    //    (filed against FreeBSD 5.3-STABLE i386)
    //
    // In C++ code, the normal offsetof() macro provided with freebsd's
    // default headers cannot be used without compile-time errors.  This
    // is the revised offsetof macro suggested in the gnu bug report.
    reference operator*() const {
        _Tp& foo(*((_Tp*)1024));
        size_t offset(reinterpret_cast<size_t>(&(foo.*_nodename)) - 1024);
        return *((_Tp*)((char*)_M_node - offset));
    }

    pointer operator->() const { return &(operator*()); }
    operator pointer() const { return &(operator*()); }

    _Self& operator++() {
        _M_node = (_Node*)(_M_node->_M_next);
        ASSERT(_M_node != NULL);
        return *this;
    }
    _Self operator++(int) {
        _Self __tmp = *this;
        ++*this;
        return __tmp;
    }
    _Self& operator--() {
        _M_node = (_Node*)(_M_node->_M_prev);
        ASSERT(_M_node != NULL);
        return *this;
    }
    _Self operator--(int) {
        _Self __tmp = *this;
        --*this;
        return __tmp;
    }
};


// --------------------------------------------------------------------------
// The endolist template class.
template<class _Tp, util::endolist_node _Tp::*_nodename = &_Tp::node>
class util::endolist : private util::Noncopyable
{
  public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef endolist_node _Node;
    typedef size_t size_type;
    typedef util::_endolist_iterator<_Tp,
                                     _nodename,
                                     _Tp&,
                                     _Tp*>       iterator;
    typedef util::_endolist_iterator<_Tp,
                                     _nodename,
                                     const _Tp&,
                                     const _Tp*> const_iterator;

    endolist();
    ~endolist();

    iterator begin()             { return (_M_node._M_next); }
    const_iterator begin() const { return (_M_node._M_next); }

    iterator end()             { return &_M_node; }
    const_iterator end() const { return &_M_node; }

    bool empty() const { return _M_size == 0; }
    size_type size() const { return _M_size; }
    size_type max_size() const { return size_type(-1); }

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(--end()); }
    const_reference back() const { return *(--end()); }

    pointer next(_Tp& __x) {
        iterator __tmp(&(__x.*_nodename));
        __tmp++;
        if (__tmp == end()) {
            return NULL;
        }
        return &(*__tmp);
    }
    pointer prev(_Tp& __x) {
        iterator __tmp(&(__x.*_nodename));
        if (__tmp == begin()) {
            return NULL;
        }
        __tmp--;
        return &(*__tmp);
    }

    iterator insert(iterator __position, _Tp& __x) {
        _Node* __tmp = &(__x.*_nodename);
        ASSERT(!__tmp->is_linked());
        __tmp->_M_next = __position._M_node;
        __tmp->_M_prev = __position._M_node->_M_prev;
        ((_Node*) (__position._M_node->_M_prev))->_M_next = __tmp;
        __position._M_node->_M_prev = __tmp;
        ++_M_size;
        return __tmp;
    }

    void insert(iterator __position, const _Tp* __first, const _Tp* __last);
    void insert(iterator __position,
                const_iterator __first, const_iterator __last);

    void push_front(_Tp& __x) { insert(begin(), __x); }
    void push_front() {insert(begin());}
    void push_back(_Tp& __x) { insert(end(), __x); }
    void push_back() {insert(end());}

    iterator erase(iterator __position) {
        ASSERT(__position != end());
        ASSERT(_M_size);
        _Node* __next_node = (_Node*) (__position._M_node->_M_next);
        _Node* __prev_node = (_Node*) (__position._M_node->_M_prev);
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        __position._M_node->reset();
        --_M_size;
        return iterator(__next_node);
    }
    iterator erase(iterator __first, iterator __last) {
        while (__first != __last) {
            erase(__first++);
        }

        return __last;
    }
    iterator erase(_Tp& __x) {
        _Node* __tmp = &(__x.*_nodename);
        ASSERT(__tmp->is_linked());
        return erase(__tmp);
    }
    void clear();
    void free();

    void pop_front() { erase(begin()); }
    void pop_back() {
        iterator __tmp = end();
        erase(--__tmp);
    }

    void splice(iterator __position, endolist& other, iterator __first, iterator __last) {
        if (this == &other) {
            _Node* __prev_node;
            _Node* __last_node;
            // detach from old location
            __prev_node = (_Node*)(__first._M_node->_M_prev);
            __prev_node->_M_next = __last._M_node;
            __last_node = (_Node*)(__last._M_node->_M_prev);
            __last._M_node->_M_prev = __prev_node;
            // attach to new position
            __prev_node = (_Node*)__position._M_node->_M_prev;
            __prev_node->_M_next = __first._M_node;
            __first._M_node->_M_prev = __prev_node;
            __position._M_node->_M_prev = __last_node;
            __last_node->_M_next = __position._M_node;
        } else {
            for (iterator __i = __first; __i != __last; ) {
                iterator __next = other.erase(__i);
                insert(__position, *__i);
                __i = __next;
            }
        }
    }

  private:
    util::endolist_node _M_node;
    size_type           _M_size;
};

template<class _Tp, util::endolist_node _Tp::*_nodename>
util::endolist<_Tp, _nodename>::
endolist()
{

    _M_node._M_next = _M_node._M_prev = &_M_node;
    _M_size = 0;
}

template<class _Tp, util::endolist_node _Tp::*_nodename>
util::endolist<_Tp, _nodename>::
~endolist()
{
    clear();
    _M_node._M_next = _M_node._M_prev = NULL;
}

template<class _Tp, util::endolist_node _Tp::*_nodename>
void util::endolist<_Tp, _nodename>::
clear()
{
    while (size()) {
        pop_front();
    }
}

template<class _Tp, util::endolist_node _Tp::*_nodename>
void util::endolist<_Tp, _nodename>::
free()
{
    while (size()) {
        _Tp *__front = &(front());
        pop_front();

        delete __front;
    }
}

template<class _Tp, util::endolist_node _Tp::*_nodename>
void util::endolist<_Tp, _nodename>::
insert(iterator __position, const _Tp* __first, const _Tp* __last)
{
    for ( ; __first != __last; ++__first) {
        insert(__position, *__first);
    }
}

template<class _Tp, util::endolist_node _Tp::*_nodename>
void util::endolist<_Tp, _nodename>::
insert(iterator __position, const_iterator __first, const_iterator __last)
{
    for ( ; __first != __last; ++__first) {
        insert(__position, *__first);
    }
}

#endif /* UTIL_ENDOLIST_H */
   
