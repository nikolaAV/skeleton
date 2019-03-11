# Sequence of ASCII characters
`std::string` is supposed to hold a set of characters from the range [0,288), that can be split into two separate subsets: [ACSII Chart Table](https://en.cppreference.com/w/cpp/language/ascii) with fixed visual symbols and [The extended ASCII codes](https://en.wikipedia.org/wiki/Extended_ASCII) where symbol representation depends on the current locale on the given computer. 
So the question is quite simple. Is there a way that can guarantee that in a particular context we are only working with characters limited by ASCII Table?  
```cpp
const ascii::string  s1  {"Hello, World!"};  // Ok, ASCII characters are only allowed
const ascii::wstring s2  {L"Hello, World!"}; // Ok
const ascii::string  s3 = "Hello, World!"s   // Error, that are incompatible types, `std::string` may contain any extended code.    
const std::string    s4 = "Hello, World!"_ascii; // Ok, 7-bit ASCII code is in a subset of [0,258)  
```
Thus, by mistake the following code will not be possible. The compiler will be our guard who will inspect our code and say if something wrong by throwing an error. 
```cpp
std::string read_file(const filesystem::path& p);

const ascii::string content = read_file("main.cpp"); // <-- At this line we expect ASCII only, but our expectation is violated.
```

TBD

```cpp

const ascii::string  s3  {"Hello, World\0xFF"};  // Exception, std::invalid_argument "no ascii character"

### How it works ...


```


## Further informations
* TBD 
* 

## Related links
* [Computing with constexpr](https://github.com/nikolaAV/Modern-Cpp/tree/master/constexpr/invocation_context)
* 

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 7.1.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.16 
