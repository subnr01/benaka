

Priority queue
----------------

A priority queue is a container adaptor that provides constant time lookup of the largest (by default) element, at the expense of logarithmic insertion and extraction.

A user-provided Compare can be supplied to change the ordering, e.g. using std::greater<T> would cause the smallest element to appear as the top().

template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;


Member functions
-------------------

Top:  accesses the top element

Empty:  checks whether the underlying container is empty

size: returns the size of the elements

push:  inserts element and sorts the underlying container 

emplace (C++11) : 
constructs element in-place and sorts the underlying container 
Pushes new element to the priority queue. The element is constructed in-place, i.e. no copy or move operations are performed. The constructor of the element is called with exactly the same arguments as supplied to the function.
This new element is constructed in place passing args as the arguments for its constructor.
Emplace calls push_heap.

pop:  removes the top element

swap:  swaps the contents.



Non-Member functions
-----------------------
std:swap(std:priority_queue) : uses the std:swap algorithm for the priority queue.


Examples
------------

 std::priority_queue<int> q;
 for(int n : {1,8,5,6,3,4,0,9,7,2}) {
        q.push(n);
 }
 
 std::priority_queue<int, std::vector<int>, std::greater<int> > q2;
 for(int n : {1,8,5,6,3,4,0,9,7,2}) {
     q2.2.push(n);
 }
 
 
 //Using lambda to compare elements
 auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1);};
 std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);
 
 for(int n : {1,8,5,6,3,4,0,9,7,2}) {
    q3.push(n);
 }
    
 
 std::priority_queue<std::string> mypq;
 mypq.emplace("orange");
 mypq.emplace("strawberry");
 mypq.emplace("apple");
 
 while (!mypq.empty())
 {
    std::cout << ' ' << mypq.top();
    mypq.pop();
 }
 
 
 Questions:

1. When to use priority queue vs push_heap?
        









