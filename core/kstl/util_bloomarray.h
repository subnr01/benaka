/*
bloomarray
*/

namespace util
{
  template<size_t BYTES, class INTTYPE> class bloomarray;
  enum bloomtype {BLOOMTYPE_ARRAY, BLOOMTYPE_FITLER};
}


class bloomarray
{
  public:
    /*
    Construct a new bloom array using {BYTES} bytes of memory;
    you will be able to store (BYTES/sizeof(INTTYPE)) elements
    as an array, before overflowing and using it as an bloom filter
    instead
    */
  
  
    bloomarray()
    {
      _type = BLOOMTYPE_ARRAY;
      clear();
    }
    
    bloomarray(char* copyFrom, bloomtype type)
    {
      _type = type;
      memcpy(_data, copyFrom, BYTES);
    }
  
    /*
    Reset the structure to contain no entries
    */
    void clear()
    {
      clear(_data, &_type);
    }
    
    static void clear(char *data, bloomtype* type)
    {
      *type = BLOOMTYPE_ARRAY;
      memset(data, 0xFF, BYTES);
    }
  
    /*
     Add an entry to the structure
     */
    void add(INTTYPE val)
    {
      add(_data, &_type, val);
    }
    
    static void add(char *data, bloomtype *type, INTTYPE val)
    {
      if (*type == BLOOMTYPE_ARRAY)
      {
        uint32_t max_count = BYTES/sizeof(INTTYPE);
        for(uint32_t index = 0; index < max_count; index++)
        {
          if (get(data, *type, index) == (INTTYPE)~0) {
            set(data, *type, index, val);
            return;
          }
        }
        
        char array_data[BYTES];
        memcpy(array_data, data, BYTES);
        
        *type = BLOOMTYPE_FILTER;
        bloomfilter<BYTES, INTTYPE>::clear(data);
        for(uint32_t index = 0; index < max_count; index++)
        {
          INTTYPE array_val = get(array_data, BLOOMTYPE_ARRAY, index);
          if (array_val != (INTTYPE)~0) {
            bloomfilter<BYTES, INTTYPE>::add(data, array_val);
            }
        }
        bloomfilter<BYTES, INTTYPE>::add(data, val);
      }
      
    private:
      char _data[BYTES];
      bloomtype _type;
 };
  
      
    
      
      
      
        
        
        
    
  
    
      
   
