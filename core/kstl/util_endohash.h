

//https://cs.stackexchange.com/questions/19020/why-should-one-not-use-a-2p-size-hash-table-when-using-the-division-method-as-a


/*
 * Copyright (c) 2015 Network Appliance, Inc.
 * All rights reserved.
 *
 */

#ifndef UTIL_ENDOHASH_H
#define UTIL_ENDOHASH_H

#ifdef UNIT_TEST
#include <unit_test.h>
#endif /* UNIT_TEST */
#include <util_functional.h>
#include <util_noncopyable.h>
#include <util_endolist.h>

namespace util
{
    template<typename DATA, int BUCKETCOUNT, endolist_node DATA::* NODENAME> class endohash;
}


/*
 * This class provides a no-allocation hash table for objects.
 *
 * Example:
 *   struct Data
 *   {
 *      Data(int k, int v = 0) : key(k), value(v) {}
 *      int key;
 *      int value;
 *
 *      virtual uint32_t hash() const { return key; }
 *      virtual bool operator== (const Data& ref) const { return ref.key == key; }
 *      endolist_node node;
 *   };
 *
 *   typdef util::endohash<Data,10001> DataTable;
 *
 *   Data d_a(1,1000);
 *   Data d_b(2,2000);
 *   DataTable table;
 *   table.insert(d_a);
 *   table.insert(d_b);
 *
 *   Data search(1);
 *   ASSERT(table.find(search) == &d_a);
 *
 *   table.erase(d_a);
 *   table.erase(d_b);
 *   ASSERT(table.size() == 0);
 */

template<typename DATA,
         int BUCKETCOUNT,
         util::endolist_node DATA::* NODENAME = &DATA::node>
class util::endohash : private util::Noncopyable
{
    protected:
        typedef util::endolist<DATA,NODENAME> BUCKET;

    public:
        endohash()
        {
            _count = 0;
        }

        size_t size() const
        {
            return _count;
        }

        void insert (DATA& obj)
        {
            BUCKET& bucket = _buckets[ obj.hash() % BUCKETCOUNT ];
            bucket.push_front(obj);
            _count++;
        }

        void erase (DATA& obj)
        {
            BUCKET& bucket = _buckets[ obj.hash() % BUCKETCOUNT ];
            bucket.erase(obj);
            _count--;
        }

        void touch (DATA& obj)
        {
            BUCKET& bucket = _buckets[ obj.hash() % BUCKETCOUNT ];
            if (&(bucket.front()) != &obj) {
                bucket.erase(obj);
                bucket.push_front(obj);
            }
        }

        DATA* find (const DATA& ref)
        {
            BUCKET& bucket = _buckets[ ref.hash() % BUCKETCOUNT ];
            for (typename BUCKET::iterator ii = bucket.begin(); ii != bucket.end(); ++ii) {
                if (*ii == ref) {
                    return &*ii;
                }
            }
            return NULL;
        }

        const DATA* find (const DATA& ref) const
        {
            const BUCKET& bucket = _buckets[ ref.hash() % BUCKETCOUNT ];
            for (typename BUCKET::const_iterator ii = bucket.begin(); ii != bucket.end(); ++ii) {
                if (*ii == ref) {
                    return &*ii;
                }
            }
            return NULL;
        }

        DATA* next_match (const DATA* walk)
        {
            BUCKET& bucket = _buckets[ walk->hash() % BUCKETCOUNT ];
            ASSERT(!bucket.empty() && ((*walk).*NODENAME).is_linked());

            typename BUCKET::iterator ii (&((*walk).*NODENAME));
            for (++ii; ii != bucket.end(); ++ii) {
                if (*ii == *walk) {
                    return &*ii;
                }
            }
            return NULL;
        }

        const DATA* next_match (const DATA* walk) const
        {
            const BUCKET& bucket = _buckets[ walk->hash() % BUCKETCOUNT ];
            ASSERT(!bucket.empty() && ((*walk).*NODENAME).is_linked());

            typename BUCKET::const_iterator ii (&((*walk).*NODENAME));
            for (++ii; ii != bucket.end(); ++ii) {
                if (*ii == *walk) {
                    return &*ii;
                }
            }
            return NULL;
        }

    protected:
        BUCKET _buckets[ BUCKETCOUNT ];
        size_t _count;
};

#endif // UTIL_ENDOHASH_H
    
    
    
    
    
    






