

//https://cs.stackexchange.com/questions/19020/why-should-one-not-use-a-2p-size-hash-table-when-using-the-division-method-as-a


namespace util
{
  template<typename DATA, int BUCKETCOUNT, endolist_node DATA::* NODENAME> class endohash;
}


/*
This class provides a no-allocation hash table for objects.
Example:
  
  struct Data 
  {
   Data (int k, int v = 0): key(k), value(v) {}
   int key;
   int value;
   
   virtual uint32_t hash() const { return key;}
   
   virtual bool operator==(const Data &ref) const {
      return ref.key == key;
   }
   endolist_node node;
 }
 
 typedef util::endohash<Data, 10001> DataTable;
 
 Data d_a(1, 1000);
 Data d_b(2, 2000);
 DataTable table;
 table.insert(d_a);
 table.insert(d_b);
 
 Data search(1);
 ASSERT( table.find(search) == &d.a);
 
 table.erase(d_a);
 table.erase(d_b);
 
 ASSERT(table.size()) == 0);
*/

template<typename DATA,
         int BUCKETCOUNT,
         endolist_node DATA::*NODENAME = &DATA::node>
class endohash : private Noncopyable {
{
protected:
    typedef endolist<DATA,NODENAME> BUCKET;
    
public:
    endohash() {
      _count=0;
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
    
    void touch (DATA &obj)
    {
       BUCKET& bucket = _buckets[ obj.hash() % BUCKETCOUNT ];
       if ( &(bucket.front() != &obj) {
           bucket.erase(obj);
           bucket.push_front(obj);
       }
    }
    
    DATA* find (const DATA &ref)
    {
       BUCKET& bucket = _buckets[ ref.hash() % BUCKETCOUNT ];
       for (typename BUCKET::iterator ii = bucket.begin(); ii != bucket.end(); ii++) 
       {
           if (*ii == ref)
           {
              return &*ii;
           }
       }
       return NULL;
    }


    const DATA* find (const DATA &ref) const
    {
       const BUCKET& bucket = _buckets[ ref.hash() % BUCKETCOUNT ];
       for (typename BUCKET::const_iterator ii = bucket.begin(); ii != bucket.end(); ii++) 
       {
           if (*ii == ref)
           {
              return &*ii;
           }
       }
       return NULL;
    }
    
protected:
    BUCKET _buckets[BUCKETCOUNT];    
    size_t _count;
};

    
      
    
    
    
    
    
    






