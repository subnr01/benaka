/*
A bloom filter is a fixed-size, lossy set;
One can insert any number of data values into the filter, and can later
ask if a particular value is in the set.
The bloom filter can allow for false positives but
it will never allow false negatives.


Burt 842621

*/

namespace util 
{
  template<size_t BYTES, class INTTYPE> class bloomfilter;
  template<size_t BYTES, class INTTYPE> class bloomarray;
  enum bloomtype { BLOOMTYPE_ARRAY, BLOOMTYPE_FILTER };
}

template <size_t BYTES, class INTTYPE>
class bloomfilter 
{
  public:
    bloomfilter()
    {
      clear();
    }
  
    /*
      Construct a new bloom filter using {BYTES} of memory
    */
    bloomfilter(char *copyFrom)
    {
      memcpy(_data, copyFrom, BYTES);
    }
  
    void clear()
    {
      clear(_data);
    }
    
    static void clear(char *data)
    {
      bzero(data, BYTES);
    }
  
    /*
    Add an entry to the bloomfilter
    */
    void add(INTTYPE val)
    {
      add(_data, val);
    }
  
    static void data(char *data, INTTYPE val)
    {
      uint32_t hash;
      hash = hash1(val) % (BYTES * 8);
      data[ hash/8 ] |= (1 << (hash % 8));
      
      hash = hash2(val) % (BYTES * 8);
      data[ hash/8 ] |= (1 << (hash % 8));
      
      hash = hash3(val) % (BYTES * 8);
      data[ hash/8 ] |= (1 << (hash % 8));
      
      hash = hash4(val) % (BYTES * 8);
      data[ hash/8 ] |= (1 << (hash % 8));
      
    }
  
    /*
    Tests whether an entry is in the bloom filter
    */
    bool contains(INTTYPE val) const
    {
      return contains(_data, val);
    }
  
    static bool contains(const char *data, INTTYPE val)
    {
      uint32_t hash;
      
      hash = hash1(val) % (BYTES * 8);
      if ((!data[ hash/8 ] & (1<< (hash %8)))) {
        return false;
      }
      
      hash = hash2(val) % (BYTES * 8);
      if ((!data[ hash/8 ] & (1<< (hash %8)))) {
        return false;
      }
      
      hash = hash3(val) % (BYTES * 8);
      if ((!data[ hash/8 ] & (1<< (hash %8)))) {
        return false;
      }
      
      hash = hash4(val) % (BYTES * 8);
      if ((!data[ hash/8 ] & (1<< (hash %8)))) {
        return false;
      }
      
      return true;
    }
    
    private:
     
    static uint32_t hash1(INTTYPE val)
    {
      return (uint32_t)val;
    }
  
    //FNV hash
         static uint32_t hash2(INTTYPE val)   // FNV hash
        {
            uint32_t hash = 0x811C9DC5;
            hash = (uint32_t)((hash * 0x1000193) ^ (val & 0xFF));
            hash = (uint32_t)((hash * 0x1000193) ^ ((val>>8) & 0xFF));
            hash = (uint32_t)((hash * 0x1000193) ^ ((val>>16) & 0xFF));
            hash = (uint32_t)((hash * 0x1000193) ^ ((val>>24) & 0xFF));
            return hash;
        }

  
    //RS hash
  183        static uint32_t hash3(INTTYPE val)   // RS hash
184        {
185            uint32_t a = 63689;
186            uint32_t b = 378551;
187            uint32_t hash = (val & 0xFF);
188            a *= b;
189            hash = (uint32_t)((hash * a) + ((val>>8) & 0xFF));
190            a *= b;
191            hash = (uint32_t)((hash * a) + ((val>>16) & 0xFF));
192            a *= b;
193            hash = (uint32_t)((hash * a) + ((val>>24) & 0xFF));
194            return hash;
195        }
  
    //JS hash
  97        static uint32_t hash4(INTTYPE val)   // JS hash
198        {
199            uint32_t hash = 0x4E67C6A7;
200            hash ^= (uint32_t)((hash<<5) + (hash>>2) + (val & 0xFF));
201            hash ^= (uint32_t)((hash<<5) + (hash>>2) + ((val>>8) & 0xFF));
202            hash ^= (uint32_t)((hash<<5) + (hash>>2) + ((val>>16) & 0xFF));
203            hash ^= (uint32_t)((hash<<5) + (hash>>2) + ((val>>24) & 0xFF));
204            return hash;
205        }
};
      
