void mutex_lock (int *mutex) 
{
  int v;
  
  /* Bit 31 was clear, we got the mutex (this is the fastpath) */ 
  if (atomic_bit_test_set(mutex, 31) == 0) {
    return;
  }

  atomic_increment (mutex);

  while(1) {
    if (atomic_bit_test_set (mutex, 31) == 0) {
      atomic_decrement(mutex);
    }

    /*
      We have to wait now. First make sure the futex value
      we are monitoring is truly negative (i.e. locked).
    */

    v = *mutex;

    if (v >= 0) {
      continue;
    }

    futex_wait(mutex, v);
  }

}


void mutex_unlock (int *mutex) 
{
  /*
    Adding 0x80000000 to the counter results
    in 0 if and only if there are no other interested
    threads
  */
  if (atomic_add_zero (mutex, 0x80000000)) {
    return;
  }

  /*
    There are other threads waiting for the mutex,
    make one of them up.
  */
  futex_wake (mutex);
}
