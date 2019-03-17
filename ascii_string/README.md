# Sequence of ASCII characters
`std::string` is supposed to hold a set of characters from the range [0,288), that can be split into two separate subsets: [ACSII Chart Table](https://en.cppreference.com/w/cpp/language/ascii) with fixed visual symbols and [The extended ASCII codes](https://en.wikipedia.org/wiki/Extended_ASCII) where symbol representation depends on the current locale on the given computer. 
So the question is quite simple. Is there a way that can guarantee that in a particular context we are only working with characters limited by ASCII Chart Table?  
#### initialization
```cpp
const ascii::string  s1  {"Hello, World!"};      // Ok, ASCII characters are only allowed
const ascii::wstring s2  {L"Hello, World!"};     // Ok
const ascii::string  s3 = "Hello, World!"s       // Error, thay are incompatible types, `std::string` may contain any extended code.    
const auto           s4 = "Hello, World!"_ascii; // Ok
```
Thus, as stated below the following code is not be possible (not compilable) if you write it by mistake. The compiler will be our guard who will inspect our code and say if something wrong by throwing an error. 
```cpp
std::string read_file(const filesystem::path& p);

const ascii::string content = read_file("main.cpp"); // <-- At this line we expect ASCII only
// but our expectation is violated because returned value may contain extended characters as well.
```
`ascii::string` __guarantees__ that it holds __only__ characters limited by codes from [ASCII Chart Table](https://en.cppreference.com/w/cpp/language/ascii).
It means that an `ascii::string` exemplar whether cannot be created if non-ascii character encountered or contains special symbol-replacement instead of it. 
#### initialization (continued)
```cpp
const ascii::string  s1  {"Hello, World\0xFF"};  // Ok, s1 = "Hello, World?"
const auto           s2 = L"characters: '\xFF' and '\xAA'"_ascii_; // Ok, s2 = "characters: '?' and '?'"
```
By default, `ascii::string` is defined as a sequence of ASCII characters with replacement policy where the special mark is `'?'`. 
If you want you are free to customize the replacement mark    


### How it works ...




## Further informations
* TBD 
* 

## Related links
* [Computing with constexpr](https://github.com/nikolaAV/Modern-Cpp/tree/master/constexpr/invocation_context)
* 

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 7.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.16 
