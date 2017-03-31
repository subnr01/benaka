#include "StdAfx.h"
#include "MultiThreadSingleQueue.h"

CMultiThreadSingleQueue::CMultiThreadSingleQueue(void)
{
   // node = new node() # Allocate a free node
   // node next = NULL # Make it the only node in the linked list
   CNode* pFirstNode = new CNode;
   // The queue
   // QHead = QTail = node # Both Head and Tail point to it
   pHeadNode = pFirstNode;
   pTailNode = pFirstNode;
   // Queue size, dummy node counted off
   m_Size = 0;
   // QHlock = QTlock = FREE # Locks are initially free
}

CMultiThreadSingleQueue::~CMultiThreadSingleQueue(void)
{
   void* pDummyValue = NULL;
   while ( Pop(pDummyValue) ) ;
   if ( pHeadNode ) { try { delete pHeadNode; } catch(...) { NULL; } }
}

bool CMultiThreadSingleQueue::Push(void* pNewValue)
{
   // node = new node() # Allocate a new node from the free list
   // node->next = NULL # Set next pointer of node to NULL
   CNode* pNewNode = new CNode;
   // node->value = value # Copy enqueued value into node
   pNewNode->pValuePointer = pNewValue;
   // lock(&QTlock) # Acquire T lock in order to access Tail
   CSingleLock singleLock(&m_TailCriticalSection);
   singleLock.Lock();
   // QTail->next = node # Link node at the end of the linked list
   pTailNode->pNextNode = pNewNode;
   // QTail = node # Swing Tail to node
   pTailNode = pNewNode;
   // Increment size - use InterlockedIncrement for accurate sizes
   // ::InterlockedIncrement(&m_Size);
   m_Size++;
   // unlock(&QTlock) # Release T lock
   singleLock.Unlock();
   return true;
}

bool CMultiThreadSingleQueue::Pop(void*& pValue)
{
   // lock(&QH lock) # Acquire H lock in order to access Head
   CSingleLock singleLock(&m_HeadCriticalSection);
   singleLock.Lock();
   // node = Q->Head # Read Head
   CNode* pCurrentNode = pHeadNode;
   // new_head = node->next # Read next pointer
   CNode* pNewHeadNode = pHeadNode->pNextNode;
   // if new_head == NULL # Is queue empty?
   if ( NULL == pNewHeadNode ) // # Queue was empty
   {
      //    unlock(&QH lock) # Release H lock before return
      singleLock.Unlock();
      //    return FALSE
      return false;
   }
   // endif
   // *pvalue = new_head->value # Queue not empty. Read value before release
   pValue = pNewHeadNode->pValuePointer;
   // QHead = new_head # Swing Head to next node
   pHeadNode = pNewHeadNode;
   // decrement size - use InterlockedDecrement for accurate sizes
   // ::InterlockedDecrement(&m_Size);
   m_Size--;
   // unlock(&QH lock) # Release H lock
   singleLock.Unlock();
   // free(node) # Free node
   try { delete pCurrentNode; } catch(...) { NULL; }
   // return TRUE # Queue was not empty, dequeue succeeded
   return true;
}
