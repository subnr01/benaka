

----------
Queues:
----------
 
-------------------
How to intialise:
-------------------
1. First
std::queue<int> v { 34,23 };

2. Second
queue<int> v (2);
v = { 34,23 };

3. Third
queue <int> v;
int vv[2] = { 12,43 };
v.assign(&vv[0], &vv[0]+2);

4, Fourth
int vv[] = { 12,43 };
std::queue<int> v(begin(vv), end(vv));

5. Fifth, fill with zeros
std::queue<int> queue1(length, 0);


--------------------
Important methods:
--------------------

void push: Inserts a new element at the end of the queue, after its current last element. 

void pop: Removes the next element in the queue, effectively reducing its size by one.

value_type &front(): Returns a reference to the next element in the queue.

value_type &back(): Returns a reference to the last element in the queue.


void emplace(arg): similar to push, but constructs the object.
emplace creates another instance of the class in the container, that's already appended to the container. 
The arguments to emplace are forwarded as arguments to the container's class's constructor.

void swap(queue& x) : Exchanges the contents of the container adaptor (*this) by those of x.
  
---------------------
Tricks for queues:
-----------------------
sort: 
std::sort(myqueue.begin(), myqueue.end());

find: 
std::find(queue.begin(), queue.end(), item) != queue.end()

itoa:
// Fill with 0, 1, ..., 99.
std::iota (std::begin(v), std::end(v), 0); 

merge two queues: 
queue1.insert( queue1.end(), queue2.begin(), queue2.end() );

Compare two queues:
The overload of operator == that works on two std::queues will compare the queue sizes 
and return false if those are different; if not, it will compare the contents of the queue 
element-by-element.
