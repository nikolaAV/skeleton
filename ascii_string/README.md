# Sequence of ASCII characters

TBD

```cpp
const ascii::string  s1  {"Hello, World!"};  // Ok
const ascii::wstring s2  {L"Hello, World!"}; // Ok
const ascii::string  s3  {"Hello, World\0xFF"};  // Exception, std::invalid_argument "no ascii character"


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
