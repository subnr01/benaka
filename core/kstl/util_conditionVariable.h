/*
This is a wrapper class around the 
sk_cv condition variable. Condition variables
work in conjunction with spinlocks. If threads need
to suspend in critical regions, then use semaphores instead.

*/

class ConditionVariable
{
  public:
    ConditionVariable(const char *cvName, int flags)
    {
      pthread_cond_init(&_cv, NULL);
    }
  
    ~ConditionVariable()
    {
      pthread_cond_destroy(&_cv);
    }
  
    //Atomically unlock the spinlock
    //and wait till signalled
    inline void wait(Spinlock *lockp)
    {
      lockp->assertLocked();
      lockp->_isLocked = false;
      
      pthread_cond_wait(&_cv, &lockp->_lock);
      lockp->_isLocked = true;
    }
  
    //wake up one of the waiters
    //on the condition variable
    inline void signal()
    {
      pthread_cond_signal(&_cv);
    }
  
    //Wake up all the waiters on the
    //condition variable
    inline void broadcast()
    {
      pthread_cond_broadcast(&_cv);
    }
  
private:
   #ifdef TEST_SUITE
    pthread_cond_t _cv;
  #else
    sk_Cv cv;
  #endif
};  
    
