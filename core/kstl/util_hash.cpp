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
  
  static const uint32_t 
  
  
