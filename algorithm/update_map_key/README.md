# Efficiently modifying the keys of `std::map` items
Since the `std::map` data structure maps from keys to values in a way that the keys are always unique and sorted,
it is of crucial value that users cannot modify the keys of map nodes that are already inserted.
In order to prevent the user from modifying the key items of perfectly sorted map nodes, the `const` qualifier is added to the key type.  
This kind of restriction is perfectly sane because it makes it harder for the user to use `std::map` the wrong way.
But what shall we do if we really need to change the keys of some map items?  
Prior to `C++17`, we had to remove the items of which we need to change the key value from the tree, in order to reinsert them.
The downside of this approach is that this always needlessly reallocates some memory, which sounds bad in terms of performance.  
Since `C++17`, we can remove and reinsert map nodes __without__ any reallocation of memory.
[Quick C++ Benchmark](http://quick-bench.com/BPG02nX9ZuvENMWye0pevx9gKlQ) shows the following benefit in the performance which [modern facilities](https://en.cppreference.com/w/cpp/container/map/extract) of the Library can give us: 
![result](./performance.png)

```cpp
template<typename Key, typename T>
std::pair<iterator,bool> update_key(
                 std::map<Key,T>& m 
                ,const Key& old_key
                ,const Key& new_key);
```

where
* 'm' is a map-source container
* 'old_key' is a the key value which to be changed
* 'new_key' is a new key value
* it returns a pair consisting of an iterator to the element with updated key(or m.end()) and a `bool` denoting whether the updating took place. If the updating failes then the source 'm' remains unchangeable.
    - if an element does not exist, `pair::first` is end(), `pair::second` is false
    - if a key duplication is detected, `pair::first` is a position to element with 'key_new', `pair::second` is false
* complexity is logarithmic

## Further informations
* [C++17 STL Cookbook](https://books.google.com.ua/books?id=-nc5DwAAQBAJ&pg=PA63&lpg=PA63&dq=Efficiently+modifying+the+keys+of+%60std::map%60+items+%D0%A1%2B%2B&source=bl&ots=hQCR5jWEWv&sig=ACfU3U2tyvpxJTVTQvbaWUnBWW6PmTQOjA&hl=en&sa=X&ved=2ahUKEwjq9fe_jazgAhXzCTQIHekiDjsQ6AEwAXoECAMQAQ#v=onepage&q&f=false) by Jacek Galowicz
* [`std::map::extract`](https://en.cppreference.com/w/cpp/container/map/extract)

## Related links
* [swap_map_key](../swap_map_key) 
[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 7.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
