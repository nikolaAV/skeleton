# Deleting items from an unsorted std::vector in O(1) time
Deleting items from somewhere in the middle of an `std::vector` takes _O(n)_ time. This is because the resulting gap from removing item must be filled by moving all items which come after the gap one slot to the left.
While moving items around like this, we preserve their order. If preserving the order is not important we can optimize this, as this code shows: 
```cpp
template<typename T>
void unstable_remove_at(std::vector<T>& vec, std::size_t idx)
{
   vec[idx]=std::move(vec.back());
   vec.pop_back();
}
```
The 'unstable_remove_at' function removes items pretty quickly without touching too many other items. 
The _actual_ item (pointed out by 'idx') is removed with the _last_ item in the vector. Take into account that the _last_ item is __moved__ to the place of the _actial_ one.  

Usage example:
```cpp
vector<int> v {1,2,3,4,5};
unstable_remove_at(v,3);
assert(4==v.size());
assert((vector<int>{1,2,5,4})==v);

vector<int> v {1,2,3,4,5,6,7,8,9,0};
unstable_erase_if(v, [](int val) {return val%2;} );
assert(5==v.size());
assert((vector<int>{0,2,8,4,6})==v);
```
Of course the best advantage `unstable_erase` gives over `std::remove` provided that the following both conditions are true: a vector owns a big amount of elements and one of the beginning elements is removed. [Quick C++ Benchmark](http://quick-bench.com/qlGUGjE4QQDQnziliMALViwJ6s8) shows a result for a vector having 100 elements and when the first one has to be removed. 
![result](./result.png)

## Further informations
* [O(1) deletion in an unordered vector](https://stackoverflow.com/questions/31115718/o1-deletion-in-an-unordered-vector?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa) on stackoverflow
* [C++17 STL Cookbook](https://books.google.com.ua/books?id=-nc5DwAAQBAJ&lpg=PA46&ots=hQAQ1hPxUx&dq=Deleting%20items%20from%20an%20unsorted%20std%3A%3Avector%20in%20O(1)%20time&pg=PA47#v=onepage&q&f=false) by Jacek Galowicz
* [Erase–remove idiom](https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom)
* [Unstable remove algorithms](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0041r0.html) proposal by Brent Friedman

## Related links
[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
