# Keeping sequence instances sorted
Arrays and vectors do not sort their payload objects themselves.
But if we need that, this does not mean that we always have to switch to data structures,
which were designed to do that automatically.
For example `std::vector` is perfect for our use case, it is still very simple and practical to add items to it in a sorting manner.

```cpp
vector<string> vec {"some","random","words","without","order","aaa","zzz"};
std::sort(begin(vec),end(vec));
insert_sorted(vec,"bbb");
// output: "aaa","bbb","order","random","some","without","words","zzz" 
```
> Note: 'insert_sorted' works properly if the input/output 'vec' is ordered with respect to value that should be inserted.  

Definition:
```cpp
template <typename Sequence>
typename Sequence::iterator 
insert_sorted(Sequence& seq, typename Sequence::value_type&& v);
```
where
* seq - a sequence container (vector,list,deque,string) in which elements are sorted
* v   - element value to insert
* it returns an iterator pointing to the inserted value
* may throw exception [`std::bad_alloc`](https://en.cppreference.com/w/cpp/memory/new/bad_alloc)
* complexity is logarithmic

The 'insert_sorted' function is pretty generic. It can also work on other container payload types, and even on other containers such [`std::set`](https://en.cppreference.com/w/cpp/container/set). 
`std::set` is a sorted collection in which the position of an element depends on its value and, by definition, its own insertion is more efficient.  

## Further informations
* [C++17 STL Cookbook](https://books.google.com.ua/books?id=-nc5DwAAQBAJ&pg=PA53&dq=Keeping+std::vector+instances+sorted&hl=en&sa=X&ved=0ahUKEwjjvMae_fvfAhWshKYKHSn1AvkQ6AEIMDAB#v=onepage&q&f=false) by Jacek Galowicz

## Related links
[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
