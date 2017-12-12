/*
make_heap

A separate heap container class would be redundant since a heap is 
just a vector in which the values have a certain arrangement. 

make_heap(v.begin(), v.end());
    This algorithm converts the entire vector v into a heap.
    
push_heap(v.begin(), v.end());
    This function will push an element into an existing heap.
    
pop_heap(v.begin(), v.end());
    This function will remove the first element from the heap.
    
sort_heap(v.begin(), v.end());
    This algorithm sorts the elements in v, and should be used in preference to the "regular" STL 
    sort algorithm when we know v to be a heap.
*/


/*
 * CONSTRUCTS A MAX HEAP
 * This version uses the operator< for comparison 
 */
template< class RandomIt >
void make_heap( RandomIt first, RandomIt last );

/*
 * In this version, the user must provide the comparison
 * function. The comparison function is of the form
 * bool cmp(const Type1 &a, const Type2 &b);
 */
template< class RandomIt, class Compare >
void make_heap( RandomIt first, RandomIt last,
                Compare comp );

/*
Complexity
At most 3*std::distance(first, last) comparisons.
*/

#include <iostream>
#include <algorithm>
#include <vector>
 
int main()
{
    std::vector<int> v { 3, 1, 4, 1, 5, 9 };
 
    std::cout << "initially, v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
 
    std::make_heap(v.begin(), v.end());
 
    std::cout << "after make_heap, v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
 
    std::pop_heap(v.begin(), v.end());
    auto largest = v.back();
    v.pop_back();
    std::cout << "largest element: " << largest << '\n';
 
    std::cout << "after removing the largest element, v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
}

/*
Output
initially, v: 3 1 4 1 5 9 
after make_heap, v: 9 5 4 1 1 3 
largest element: 9
after removing the largest element, v: 5 3 4 1 1
*/


/*
sort_heap
Converts the max heap [first, last) into a sorted range in ascending order. 
The resulting range no longer has the heap property. 

Complexity
At most 2×N×log(N) comparisons where N=std::distance(first, last)
*/

#include <algorithm>
#include <vector>
#include <iostream>
 
int main()
{
    std::vector<int> v = {3, 1, 4, 1, 5, 9}; 
 
    std::make_heap(v.begin(), v.end());
 
    std::cout << "heap:\t";
    for (const auto &i : v) {
        std::cout << i << ' ';
    }   
 
    std::sort_heap(v.begin(), v.end());
 
    std::cout << "\nsorted:\t";
    for (const auto &i : v) {                                                   
        std::cout << i << ' ';
    }   
    std::cout << '\n';
}

/*
Output
heap:   9 4 5 1 1 3 
sorted: 1 1 3 4 5 9
*/


//Possible implementation
template< class RandomIt >
void sort_heap( RandomIt first, RandomIt last );
{
    while (first != last)
        std::pop_heap(first, last--);
}

/*

pop_heap
Swaps the value in the position first and the value in the position 
last-1 and makes the subrange [first, last-1) into a max heap.

Complexity
At most 2×log(N) comparisons where N=std::distance(first, last)

*/
#include <iostream>
#include <algorithm>
#include <vector>
 
int main()
{
    std::vector<int> v { 3, 1, 4, 1, 5, 9 };
 
    std::make_heap(v.begin(), v.end());
 
    std::cout << "v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
 
    std::pop_heap(v.begin(), v.end()); // moves the largest to the end
 
    std::cout << "after pop_heap: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
 
    int largest = v.back();
    v.pop_back();  // actually removes the largest element
    std::cout << "largest element: " << largest << '\n';
 
    std::cout << "heap without largest: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
}

/*
Output:

v: 9 5 4 1 1 3 
after pop_heap: 5 3 4 1 1 9 
largest element: 9
heap without largest: 5 3 4 1 1
*/  
  

/*

Push_heap
Inserts the element at the position last-1 into the max heap 
defined by the range [first, last-1)


Complexity
At most log(N) comparisons where N=std::distance(first, last). 
*/

#include <iostream>
#include <algorithm>
#include <vector>
 
int main()
{
    std::vector<int> v { 3, 1, 4, 1, 5, 9 };
 
    std::make_heap(v.begin(), v.end());
 
    std::cout << "v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
 
    v.push_back(6);
 
    std::cout << "before push_heap: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
 
    std::push_heap(v.begin(), v.end());
 
    std::cout << "after push_heap: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
}

/*
Output:

v: 9 5 4 1 1 3 
before push_heap: 9 5 4 1 1 3 6 
after push_heap:  9 5 6 1 1 3 4
*/
