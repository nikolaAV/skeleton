# Efficiently swapping the keys of `std::map` items
`std::map` is an _associative_ container i.e. it's a _sorted_ collection in which the position of an element depends on its key due to a certain sorting criterion. 
In order to prevent the user from modifying the key items of perfectly sorted map nodes, the `const` qualifier is added to the key type.
This kind of restriction does not allow users to change keys directly to swap them. When such type of key swap may have a sense? 
The most frequent case is when a type of value stored in the map cannot be moved and copying of it requires "high-weigth" operation in the performance terms. 
Since `C++17`, we can operate with map nodes directly (see ['std::extract'](https://en.cppreference.com/w/cpp/container/map/extract)) and there is a possibility to change/swap its keys.

```cpp
// swaps values of the specified elements by its keys in the requested map-container
template<typename Key,typename T>
bool swap_value( std::map<Key,T>& m 
                ,typename std::map<Key,T>::iterator first
                ,typename std::map<Key,T>::iterator second
               );
template<typename Key,typename T>
bool swap_value( std::map<Key,T>& m, const Key& first, const Key& second); 

// swaps keys of the specified elements in the requested map-container
template<typename Key,typename T>
bool swap_key( std::map<Key,T>& m 
                ,typename std::map<Key,T>::iterator first
                ,typename std::map<Key,T>::iterator second
               );
template<typename Key,typename T>
bool swap_key( std::map<Key,T>& m, const Key& first, const Key& second); 
```
where
* 'm' is a target map-container
* first, second - keys or iterators which points out map items whose contents to swap
* returns `true` if the swapping took place, `false` otherwise

### Example
Let's suppose we implement a little application that orders the placement of drivers in a fictional race in an `std::map` structure. 
While drivers pass each other during the race, we need to change their placement keys.
```cpp
   map<int,string> race_placement = {
       {1, "Mario"}
      ,{2, "Luigi"}
      ,{3, "Bowser"}
      ,{4, "Yoshi"}
      ,{5, "Toad"}
      ,{6, "Peach"}
      ,{7, "Koopa"}
      ,{8, "Donkey Kong Jr."}
   };
```
Let's say that during one race lap, Bowser had a little accident and dropped to the last place and Donkey Kong Jr. took the chance to jump from the last place to the third place.
There are two ways how it can be done
```cpp
   swap_value(race_placement,3,8);
```
or
```cpp
   swap_key(race_placement,3,8);
```


## Further informations
* [C++17 STL Cookbook](https://books.google.com.ua/books?id=-nc5DwAAQBAJ&pg=PA63&lpg=PA63&dq=Efficiently+modifying+the+keys+of+%60std::map%60+items+%D0%A1%2B%2B&source=bl&ots=hQCR5jWEWv&sig=ACfU3U2tyvpxJTVTQvbaWUnBWW6PmTQOjA&hl=en&sa=X&ved=2ahUKEwjq9fe_jazgAhXzCTQIHekiDjsQ6AEwAXoECAMQAQ#v=onepage&q&f=false) by Jacek Galowicz
* [`std::map::extract`](https://en.cppreference.com/w/cpp/container/map/extract)

## Related links
* [update_map_key](../update_map_key)  
[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 7.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
