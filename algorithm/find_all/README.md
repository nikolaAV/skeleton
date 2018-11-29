# find_all, algorithm that works with Standard Library iterators just like the standard algorithms.  
Suppose that you want to find all the elements matching a predicate in a given range.
The `find()` and [`find_if()`](https://en.cppreference.com/w/cpp/algorithm/find) algorithms are the most likely candidates, but each returns an iterator referring to only one element.
You can use [`copy_if()`](https://en.cppreference.com/w/cpp/algorithm/copy) to find all elements matching a given predicate, but it fills the output with copies of the found elements.
If you want to avoid copies, you can use `copy_if()` with a [`back_insert_iterator`](https://en.cppreference.com/w/cpp/iterator/back_insert_iterator) into a `vector<reference_wrapper<T>>`, but this does not give you the position of the found elements.
In fact, there is no standard algorithm to get iterators to all the elements matching a predicate.
The proposed functionality here gives you what you need is called _find_all_. 

```cpp
   const auto seq = { 3, 4, 5, 4, 5, 6, 5, 8 };
                  //  0  1  2  3  4  5  6  7
   vector<decltype(seq)::const_iterator> matches;

   find_all(begin(seq),end(seq),back_inserter(matches),5);

   cout << "Found " << matches.size() << " matching elements: " << endl;
   for (const auto& it : matches)
      cout << *it << " at position " << (it - cbegin(seq)) << endl;;
   ---
   output: 
    Found 3 matching elements: 
    5 at position 2
    5 at position 4
    5 at position 6
```


where _find_all_ defined as
```cpp
    template< typename InputIt, typename OutputIt, typename T>
    OutputIt find_all( InputIt first, InputIt last, OutputIt d_first, const T& value)
```
* first, last - the range of elements to examine. They must meet the requirements of [_InputIterator_](https://en.cppreference.com/w/cpp/named_req/InputIterator)  
* d_first - the beginning of the destination range. It must meet the requirements of [_OutputIterator_](https://en.cppreference.com/w/cpp/named_req/OutputIterator)
* value - value to compare the elements to

## Further informations
TBD

## Related links
[back to algorithm](../)

## Compilers
* [GCC 7.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
