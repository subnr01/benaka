/*
A bloom filter is a fixed-size, lossy set;
One can insert any number of data values into the filter, and can later
ask if a particular value is in the set.
The bloom filter can allow for false positives but
it will never allow false negatives.



*/

namespace util 
{
  template<size_t BYTES, class INTTYPE> class bloomfilter;
  template<size_t BYTES, class INTTYPE> class bloomarray;
  enum bloomtype { BLOOMTYPE_ARRAY, BLOOMTYPE_FILTER };
}

template <size_t BYTES, class INTTYPE>
class bloomfilter 
