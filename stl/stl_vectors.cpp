
----------
Vectors:
----------
Vectors are sequence containers representing arrays that can change in size.
Some key points:
-- random access
-- dynamic size
-- addition or removal at the end or start-- good performance
-- addition/removal in the middle-- poor performance
  
-------------------
How to intialise:
-------------------
1. First
std::vector<int> v { 34,23 };

2. Second
vector<int> v (2);
v = { 34,23 };

3. Third
vector <int> v;
int vv[2] = { 12,43 };
v.assign(&vv[0], &vv[0]+2);

4, Fourth
int vv[] = { 12,43 };
std::vector<int> v(begin(vv), end(vv));

5. Fifth, fill with zeros
std::vector<int> vector1(length, 0);


--------------------
Important methods:
--------------------
push_back: adds an element after the current last element.

pop_back: removes last element, reducing size by 1.

resize: Resizes the container so that it contains n elements.
resize(5); 
resize(8,100); 

reserve (size_type n): Requests that the vector capacity be at least enough to contain n elements.

capacity: Returns the size of the storage space currently allocated for the vector.

size: Returns the number of elements in the vector.
  
emplace: The container is extended by inserting a new element at position.
std::vector<int> myvector = {10,20,30};
auto it = myvector.emplace ( myvector.begin()+1, 100 );
myvector.emplace ( it, 200 );
myvector.emplace ( myvector.end(), 300 );
Output: 10 200 100 20 30 300 

emplace_back: Inserts a new element at the end of the vector, right after its current last element. 
std::vector<int> myvector = {10,20,30};  
myvector.emplace_back (100);
myvector.emplace_back (200);  
Output: 10 20 30 100 200

front: return first element.

back:   returns last element.

clear: Remove all elements from the vector, leaving the size to 0.

erase: Removes from vector a single element or a range of elements.
iterator erase (iterator position);
iterator erase (iterator first, iterator last);  
// erase the 6th element
myvector.erase (myvector.begin()+5);

rbegin/rend: reverse iterator begin to reverse end.

cbegin/cend: const iterator to begin and end of the vector.

  
---------------------
Tricks for vectors:
-----------------------
sort: 
std::sort(myvector.begin(), myvector.end());

find: 
std::find(vector.begin(), vector.end(), item) != vector.end()

itoa:
// Fill with 0, 1, ..., 99.
std::iota (std::begin(v), std::end(v), 0); 

merge two vectors: 
vector1.insert( vector1.end(), vector2.begin(), vector2.end() );

Compare two vectors:
The overload of operator == that works on two std::vectors will compare the vector sizes 
and return false if those are different; if not, it will compare the contents of the vector 
element-by-element.
