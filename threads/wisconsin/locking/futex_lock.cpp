/*
Linux Futex

Linux 2 phase lock
A two-phase lock realizes that spinning can be useful, particularly 
if the lock is about to be released. So in the first phase, the lock spins for a while, 
hoping that it can acquire the lock.

However, if the lock is not acquired during the first spin phase, a sec- ond phase is entered, 
where the caller is put to sleep, and only woken up when the lock becomes free later.

The Linux lock above is a form of such a lock, but it only spins once; a generalization of this 
could spin in a loop for a fixed amount of time before using futex support to sleep.

Two-phase locks are yet another instance of a hybrid approach, 
where combining two good ideas may indeed yield a better one.

*/


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
      return;
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
  
