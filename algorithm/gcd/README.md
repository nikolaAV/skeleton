# Computation of the greatest common divisor for ranges.  
Since C++17, the Standard Library provides algorithm [`std::gcd`](https://en.cppreference.com/w/cpp/numeric/gcd) which is defined to accept only two parameters. 
If we want to extend it accepting a range of values we can easily implement it by means [`std::accumulate`](https://en.cppreference.com/w/cpp/algorithm/accumulate) like this: 
```cpp
template< class InputIt>
auto gcd(InputIt first, InputIt last)
{
   return std::accumulate(first,last,*first,[](auto const& a, auto const&b){
      return std::gcd(a,b);
   });
}

template< class InputRange>
auto gcd(const InputRange& range)
{
   return gcd(std::begin(range), std::end(range));
}
```
where
* first, last - the range of elements to examine. They must meet the requirements of [_InputIterator_](https://en.cppreference.com/w/cpp/named_req/InputIterator)  
* range - any container for which [`std::begin`](https://en.cppreference.com/w/cpp/iterator/begin) and [`std::end`](https://en.cppreference.com/w/cpp/iterator/end) can be applicable. 

Usage:
```cpp
   assert(8==gcd(initializer_list<int>{48,16,24,96}));
   assert(1==gcd(vector<unsigned>{1,2,3}));
   assert(2==gcd(list<long>{8,6,4,2,10,12,100}));
```

## Further informations
TBD

## Related links
* [compile-time gcd](../../constexpr/greatest_common_divisor)

[back to algorithm](../)

## Compilers
* [GCC 7.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
