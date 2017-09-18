

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






