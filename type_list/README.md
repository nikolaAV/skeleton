# Type Lists
A typelist is a type that represents a list of types and can be manipulated by a template metaprogram. 
It provides the operations typically associated with a list: iterating over the elements (types) in the list, adding elements, or removing elements. 
However, typelists differ from most run-time data structures, such as `std::list`, in that they don’t allow mutation. 
Adding an element to a typelist, on the other hand, does not change the original typelist: Rather, adding an element to an existing typelist creates a new typelist without modifying the original. 

## Examples of usage
### How to determine whether the given typelist has no elements? Use `is_empty` trait.
```cpp
static_assert(is_empty_v<list<int,double,void*>>==false);
static_assert(is_empty_v<list<>>==true);
```
### `size` returns number of elements in the list.
```cpp
static_assert(size_v<list<int,double,void*>>==3);
static_assert(size_v<list<>>==0);
```
### `any_of` checks if specified type T exists in a list of types
```cpp
static_assert(any_of_v<list<int,char,double>,char>);
static_assert(none_of_v<list<int,char,double>,void*>);
```
### `front`/`back` metafunctions extract the first/last element from the typelist
```cpp
using my_list = list<double,float,long>;
using first = front_t<my_list>; // double
using last = back_t<my_list>;   // long
```
### the `pop_front` removes the first element from the typelis
```cpp
using original_list = list<A,B,C>;
using other_list = pop_front_t<original_list>;    // list<B,C>
```
### the `push_front` inserts an element onto the front of the typelist
```cpp
using original_list = list<A,B,C>;
using other_list1 = push_front_t<original_list,X>;    // list<X,A,B,C>
using other_list2 = push_front_t<original_list,original_list>;    // list<A,B,C,A,B,C>
```
### `nth_element` extracts a specific element from the list (zero-based indexing)
```cpp
using my_list = list<int,char,long,unsigned>;
using first = nth_element_t<my_list,0>; // int
using last = nth_element_t<my_list,3>; // unsigned
```
### `unique` eliminates all duplications from the typelist
```cpp
using input = list<A,B,C,A,D,B>;
using output = unique_t<input>; // list <A,B,C,D>
```
### how to get a typelist with element in reverse order? it's easy just use `reverse`
```cpp
using input = list<A,B,C,D>;
using output = reverse_t<input>; // list <D,C,B,A>
```
### rest of other typelist algorithms can be foud [here](./typelist.h) 

## Further informations
* [TypeLists and a TypeList Toolbox via Variadic Templates](https://www.codeproject.com/Articles/1077852/TypeLists-and-a-TypeList-Toolbox-via-Variadic-Temp) on codeproject
* [Type Lists](https://blog.galowicz.de/2016/05/08/compile_time_type_lists/) by Jacek Galowicz
* [C++ Templates - The Complete Guide, 2nd Edition](http://www.tmplbook.com/), Chapter 24
* [Generic Programming:Typelists and Applications](https://www.drdobbs.com/generic-programmingtypelists-and-applica/184403813) by Andrei Alexandrescu

## Related links
* [Variatic Templates. A few useful examples](https://github.com/nikolaAV/Modern-Cpp/tree/master/variadic)
* [Compile time configuration](https://github.com/nikolaAV/compile_time_configuration)
* [using C++20 concept to find types in the list by requested traits](https://github.com/nikolaAV/Modern-Cpp/tree/master/concept%20(C++20)/for_each)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.1](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.16 
