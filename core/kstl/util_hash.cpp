  /*
  
  We support the 32 and 64 bit versions of the FNV-1a hash algorithm,
  in two variations.
  
  The standard version does a bytewise hash of the memory region.
  
  Example:
  wafl_Vol *vol;
  uint64_t hash = util_hash_fnv_1a_64(vol, sizeof(wafl_Vol));
  
  However if you want more control over which members of a struct 
  get hashed, then use the _inc versions. These allow you to incrementally
  build up the hash by passing in whatever variables you did like.
  
  If you are using the inc methods, you must set hashp to NULL the first time.
  This directs the hashing function to do the FNV setup. Pass the returned value
  to _inc as the hashp for subsequent calls.
  
  Example:
  wafl_Vol *vol;
  //set the vol from somewhere
  unint64_t hash = util_hash_fnv_la_64_inc(&vol->wv_name, sizeof(vol->wv_name), NULL);
  util_hash_fnv_la_64_inc(&vol-> wv_aggr, sizeof(vol->wv_aggr), &hash);
  ...
  return hash;
  */
  
  
  
  /* 
  The FNV algorithm comes from "http://isthe.com/chongo/tech/comp/fnv/".
  Though they provide sample code, an independent implementation has been done
  here .
  */
  
static const uint32_t fnv_const_offset_basis_32 = 2166136261ULL;
static const uint64_t fnv_const_offset_basis_64 = 14695981039346656037ULL;
  
static const uint32_t fnv_const_prime_32 = 16777619ULL;
static const uint64_t fnv_const_prime_64 = 1099511628211ULL;

uint32_t util_hash_fnv_la_32(const uint8_t *data, uint64_t len)
{
  return util_hash_fnv_la_32_inc(data, len, 0);
}

uint64_t util_hash_fnv_la_64(const uint8_t *data, uint64_t len)
{
  return util_hash_fnv_la_32_inc(data, len, 0);
}

uint32_t util_hash_fnv_la_32_inc(const uint8_t *data, const uint64_t len, uint32_t *hashp)
{
  uint32_t hash = hashp? *hashp: fnv_const_offset_basis_32;
  
  for( uint64_t i = 0; i < len; i++)
  {
    hash ^= (uint32_t) data[i];
    hash *= fnv_const_prime_32;
  }
  
  if (hashp)
  {
    *hashp = hash;
  }
  return hash;
}


extern "C"
uint64_t util_hash_fnv_la_64_inc(const uint8_t *data, const uint64_t len, uint64_t *hashp)
{
  uint64_t hash = hashp? *hashp: fnv_const_offset_basis_64;
  
  for( uint64_t i = 0; i < len; i++)
  {
    hash ^= (uint64_t) data[i];
    hash *= fnv_const_prime_64;
  }
  
  if (hashp)
  {
    *hashp = hash;
  }
  return hash;
}
