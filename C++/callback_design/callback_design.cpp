
#include <vector.h>

class QueueListener {
   public:
       virtual void DataReady(class MyQueue *q) = 0;
       virtual ~QueueListener() {}
};

class MyQueue {
   public:
       void Add (int x) {
          theQueue.push_back(x);
          for (int i = 0; i < theCallBacks.size(); i++) {
             theCallBacks[i]->DataReady(this);
          }
       }

       void Register (QueueListener *ql) {
            theCallBacks.push_back(ql);
       }


   private:
       vector <QueueListener *> theCallBacks;
       vector <int> theQueue;
};



class MyListener : public QueueListener {
   public:
       virtual ~MyListener () {
          printf("MyListener destructor!");
       }
       MyListener(MyQueue *q);
       virtual void DataReady(class MyQueue *p);
};



#include "File1.h"


MyListener::MyListener(MyQueue *q)
{
   q->Register(this);
}

void MyListener::DataReady(class MyQueue *p)
{
   Sleep(500);
}


void __fastcall TForm1::Button1Click(TObject *Sender)
{
    MyQueue *q = new MyQueue();
    MyListener ml(q);

    q->Add(1);

}
