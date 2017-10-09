/*
The kmemptr templated class is a simple smartptr
(similar to auto_ptr<T>, which uses kmem 
memory management for allocation/deallocations.

Unlike the autoptr, the _ref class is not implemented.
And there is a consutructor that takes the number of
elements desired to be allocated, and an allocate method
which allocates the correct amount of kernel memory
for what is desired.

*/

