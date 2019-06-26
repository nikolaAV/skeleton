# [Fowler-Noll-Vo Hash (FNV1a)](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
Let's start from the end and have a look at several examples of the usage:
```cpp
// an input data is a sequence of characters as a C-string literals or std::string
constexpr char    exp[] = { "C++ language" };
static_assert(hash("C++ language") == hash(exp)); // compile time
assert(hash(std::string{exp}) == hash(exp));      // run time
```
```cpp
// an input data is a sequence of characters as std::array
constexpr std::array arr1 = { 'F','N','V','1','a' };
static_assert(hash("FNV1a") == hash(arr1));     // compile time
const std::array arr2 {arr1};
assert(hash(arr1) == hash(arr2));               // run time
```
```cpp
// an input data is a stream of characters
std::istringstream s("Hello, World!");
using iterator = std::istream_iterator<char>;
std::cout << hash(iterator(s),iterator());
```
```cpp
// an input data is a vector containing elements of floating type 
const std::vector v = { .1,.2,.3,.4,.5,.6,.7,.8,.9 };
std::cout << hash(v);
```
```cpp
// an input data is a C array containing elements of integral type 
const int a[] = { 0,1,2,3,4,5,6,7,8,9 };
std::cout << hash(a);
```
```cpp
// an input data of user-defined type. 
// may be a singular or a range of elements
struct aggregate {
   size_t m1;
   float  m2;
};
const auto agg  = aggregate{0,.9};
std::cout << hash(agg);
const auto list = { agg,agg,agg }; // std::initializer_list<aggregate>
std::cout << hash(list);
```
### Implementation
The heart of the hash function can be expressed like this ([FNV1a](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)):
```cpp
constexpr uint32_t _prime_ = 0x01000193; //   16777619
constexpr uint32_t _seed_  = 0x811C9DC5; // 2166136261

inline constexpr uint32_t hash(uint8_t _1byte, uint32_t prev) {
   return (_1byte^prev)*_prime_;
}
// for signed char the overload is given
inline constexpr uint32_t hash(char _1byte, uint32_t prev) {
   return hash((uint8_t)_1byte,prev);
}
```
To iterate through an input sequence of bytes we've created the analog of [`std::accumulate`](https://en.cppreference.com/w/cpp/algorithm/accumulate) algorithm  because the standard version cannot be used at compile time (keyword `constexpr` is missing).  
```cpp
template<typename InputIt, typename T, typename BinaryOperation>
constexpr T accumulate(InputIt first, InputIt last, T init, BinaryOperation op) {
   while (first != last)
      init = op(init, *first++);
   return init;
}
```
The iterator-based __hash__ function can be implemented as
```cpp
template<typename InputIt>
inline constexpr uint32_t hash(InputIt first, InputIt last) {
   return accumulate(first, last, _seed_, [](const uint32_t& prev, const auto& v) {
      return hash(v, prev);
   });
}
// to simplify usage of it, the range-based hash overload is provided too
template<typename Range>
inline constexpr uint32_t hash(const Range& r) {
   return hash(std::begin(r),std::end(r));
}
```
At this point our __hash__ function works only for containers with characters, because of the terminal __hash__ function which is called from the iterable/range __hash__ can only accept `unsigned char` (`uint8_t`) type. 
So how can we work with types size of of which is greater than 8 bits?  
First of all, it's needed to create a lightweight type cast that can  transform a value of any type T to sequence of bytes. 
```cpp
template <std::size_t N> struct bytes_t {
   uint8_t _[N];
};
const union { 
    T value; 
    bytes_t<sizeof(T)> bytes;
} cast_t;

cast_t cast{1234};  // initialization by integer
cast.bytes._[0];cast.bytes._[1]; ... // access to each byte of integer representation
```
and we have to make __hash__ function learn to work with __bytes_t<>__ class template.  
```cpp
template <std::size_t N>
inline constexpr uint32_t hash(bytes_t<N> bytes, uint32_t prev) {
   return accumulate(std::begin(bytes._), std::end(bytes._),prev,[](uint32_t p, uint8_t v){
      return hash(v,p); // <--- the key function accepting 8-bit value
   });
}

template <typename T>
inline uint32_t hash(T value, uint32_t prev) {
   const union { T value; bytes_t<sizeof(T)> bytes; } cast {value};
   return hash(cast.bytes, prev);
}
```
### Specification
```cpp
#1 iterator-based 
template<typename InputIt>
constexpr uint32_t hash(InputIt first, InputIt last) noexcept;

#2 range-based
template<typename Range>
constexpr uint32_t hash(const Range& range) noexcept;

#3 value-based
template<typename T>
constexpr uint32_t hash(const T& value) noexcept;
```
* first, last	-	the range [first,last) of elements to compute the hash. InputIt must meet the requirements of [LegacyInputIterator](https://en.cppreference.com/w/cpp/named_req/InputIterator).
* range	-	shorter term of [first,last). Range must meet the requirements of [`std::ranges::Range`](https://en.cppreference.com/w/cpp/ranges/Range)
* value	-	an instance of any type for which a hash is supposed to compute. Please note T should be [POD](https://en.cppreference.com/w/cpp/named_req/PODType) type. Otherwise you can get a hash you don't want to expect.  

Although variants #2, #3 have the same signature they are included/excluded from [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution) by means a condition provided by type traits technique [`std::enable_if`](https://en.cppreference.com/w/cpp/types/enable_if).
## Further informations
* [Fowler-Noll-Vo Hash (FNV1a)](https://create.stephan-brumme.com/fnv-hash/) by Stephan Brumme
* [Fowler–Noll–Vo hash function](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function) on wikipedia 
* [FNV Hash](http://www.isthe.com/chongo/tech/comp/fnv/) official site

## Related links
* [`constexpr` Lambda Expressions](https://github.com/nikolaAV/Modern-Cpp/tree/master/lambda/lambda_constexpr)
* [`switch` statement accepting strings](../../switch_string)
* [simple XOR](../simple_xor) another example of function template overload resolution that have the same signatures 
* [back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 7.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
