

What is ABA?



- Process P_1 reads value A from shared memory,
- P_1 is preempted, allowing process P_2 to run,
- P_2 modifies the shared memory value A to value B and back to A before preemption,
- P_1 begins execution again, sees that the shared memory value has not changed and continues.


Although P_1 can continue executing, it is possible that the behavior will not be 
correct due to the "hidden" modification in shared memory.


Consider a software example of ABA using a lock-free stack:

  /* Naive lock-free stack which suffers from ABA problem.*/
  class Stack {
    std::atomic<Obj*> top_ptr;
    //
    // Pops the top object and returns a pointer to it.
    //
    Obj* Pop() {
      while(1) {
        Obj* ret_ptr = top_ptr;
        if (!ret_ptr) return std::nullptr;
        // For simplicity, suppose that we can ensure that this dereference is safe
        // (i.e., that no other thread has popped the stack in the meantime).
        Obj* next_ptr = ret_ptr->next;
        // If the top node is still ret, then assume no one has changed the stack.
        // (That statement is not always true because of the ABA problem)
        // Atomically replace top with next.
        if (top_ptr.compare_exchange_weak(ret_ptr, next_ptr)) {
          return ret_ptr;
        }
        // The stack has changed, start over.
      }
    }
    //
    // Pushes the object specified by obj_ptr to stack.
    //
    void Push(Obj* obj_ptr) {
      while(1) {
        Obj* next_ptr = top_ptr;
        obj_ptr->next = next_ptr;
        // If the top node is still next, then assume no one has changed the stack.
        // (That statement is not always true because of the ABA problem)
        // Atomically replace top with obj.
        if (top_ptr.compare_exchange_weak(next_ptr, obj_ptr)) {
          return;
        }
        // The stack has changed, start over.
      }
    }
  };

This code can normally prevent problems from concurrent access, but suffers from ABA problems. Consider the following sequence:

Stack initially contains top → A → B → C

Thread 1 starts running pop:

 ret = A;
 next = B;
Thread 1 gets interrupted just before the compare_exchange_weak...

  { // Thread 2 runs pop:
    ret = A;
    next = B;
    compare_exchange_weak(A, B)  // Success, top = B
    return A;
  } // Now the stack is top → B → C
  { // Thread 2 runs pop again:
    ret = B;
    next = C;
    compare_exchange_weak(B, C)  // Success, top = C
    return B;
  } // Now the stack is top → C
  delete B;
  { // Thread 2 now pushes A back onto the stack:
    A->next = C;
    compare_exchange_weak(C, A)  // Success, top = A
  }
  
Now the stack is top → A → C

When Thread 1 resumes:

 compare_exchange_weak(A, B)
This instruction succeeds because it finds top == ret (both are A), so it sets top to next (which is B). As B has been deleted the program will access freed memory when it tries to look the first element on the stack. In C++, as shown here, accessing freed memory is undefined behavior: this may result in crashes, data corruption or even just silently appear to work correctly. ABA bugs, such as this, can be difficult to debug.
