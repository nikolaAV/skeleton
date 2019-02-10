# Efficiently swapping the keys of `std::map` items
TBD
```cpp
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
* [update_map_key](../.update_map_key)  
[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 7.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
