/*
 Referenceable provides a simple increment/decrement reference semantics with
 an atomic count. When the count reaches zero, the pure virtual _delete()
 method is called. Classes which wish to be accessed via smart pointers 
 can be Referenceable and implement an appropriate _delete method 
 */


namespace util
{
  class Referenceable;
};



class Referenceable : virtual protected Noncopyable
{
 public:
    
   Referenceable() :_referenceCount(0) {};
   
   virtual ~Referenceable() : { ASSERT(_referenceCount.get == 0)};
   
   virtual void incRef() 
   {
     _referenceCount.atomicIncrement();
   }
   
   virtual void decRef() 
   {
     ASSERT(_referenceCount.get() != 0);
     if(_referenceCount.atomicDecrementAndLessThanOne())
     {
       _delete();
     }
   }
   
   virtual uint32_t getRefCount() const
   {
     return _referenceCount.get());
   }
   
 protected:
   //Notice this method is protected, while others are public
   virtual void _delete()
   {
     delete this;
   }
  
 private:
   Atomic<uint32_t> _referenceCount;
 };
   
   
   
