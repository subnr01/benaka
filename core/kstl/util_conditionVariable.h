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
