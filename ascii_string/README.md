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
#### initialization (continued 2)
```cpp
using my_ascii_string = ascii::basic_string<char,sieve_replacement<char,'#'>>;
my_ascii_string s = {"characters: '\xFF' and '\xAA'"};
// s == "characters: '#' and '#'"
```
In order to prohibit instantiation if the input data contain any non-ascii character just write the following:  
#### initialization (continued 3)
```cpp
using my_ascii_string = ascii::basic_string<char,sieve_exception<char>>;
my_ascii_string s = "characters: '\xFF' and '\xAA'"; // Error
// std::invalid_argument will be thrown

using my_ascii_string2 = ascii::basic_string<char,sieve_exception<char,std::domain_error>>;
my_ascii_string2 s = "characters: '\xFF' and '\xAA'"; // Error
// std::domain_error will be thrown
```
`std::string` and `ascii::string` are incompatible, but they can be convertible if you specify it explicitly
#### type casting
```cpp
auto a  = cast<ascii::string> ("Hello, World!"s);
auto w  = cast<std::wstring>  (a);
a = cast<ascii::string> (w);
auto s  = cast<std::string>  (a);

a = cast<ascii::string> ("Copyright symbol: '\xB8'"s);
// a == Copyright symbol: '?'
```
Content of two strings different types can be swapped. This operation is not required memory re-allocation.
#### swapping
```cpp
ascii::string  s1 {"C++ protects against accident, not against fraud"};
std::string    s2 {"Bjarne Stroustrup"};
swap(s1,s2);
// s1: "Bjarne Stroustrup"
// s2: "C++ protects against accident, not against fraud"
swap(s2,s1);
// s2: "Bjarne Stroustrup"
// s1: "C++ protects against accident, not against fraud"
```
Charactres can be read and written from/to streams.
#### stream operations support
```cpp
ascii::string        s0, s1, s2, s3;
// let's suppose an user entered: The C++ Programming Language 
std::cin  >> s0 >> s1 >> s2 >> s3;
std::cout << s0 << s1 << s2 << s3;
// out: TheC++ProgrammingLanguage

ascii::string a;
// input: German-umlaut:'ä'-and-euro-symbol:'€'
std::cin  >> a;
std::cout << a;
// out: German-umlaut:'?-and-euro-symbol:'?'
```

### How it works ...
Fortunately, `std::string` is only a specializing typedef of the template class, [`std::basic_string`](https://en.cppreference.com/w/cpp/string/basic_string).
This class contains all the complicated memory handling stuff, but it does not impose any policy on how strings are copied, compared, and so on. 
This is something that is imported into `basic_string` by accepting a template parameter that contains a traits class.
Building own traits class will help us create `ascii::string` without reimplementing anything. 
The `char_traits<>` class contains algorithms that `basic_string` needs for its operation. 
The `char_traits<>` knows how to compare, find, and copy characters and strings. 
If we want a string class to behave differently, we can try to reuse as much as possible from what `basic_string` and `char_traits` already provide.
```cpp
template <typename CharT>
struct ascii::char_traits : std::char_traits<CharT> {
   static constexpr void assign( CharT& left, const CharT& right );
   static constexpr CharT* copy( CharT* dest, const CharT* src, std::size_t count ); {
   ...
```
`ascii::string` is configured as a completely new string type by using `ascii::char_traits` as  substitute for the standard `char_traits`
```cpp
template <
   typename CharT, 
   typename Traits = ascii::char_traits<CharT>, 
   typename Alloc = std::allocator<CharT>>
using basic_string = std::basic_string<CharT,Traits,Alloc>;

using string  = basic_string<char>;
using wstring = basic_string<wchar_t>;
```

## Further informations
* [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string) 
* 

## Related links
* TBD 

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 7.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.16 
