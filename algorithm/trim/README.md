# Trim

Example 1. Input sequence is wide string with leading and trailing white-space characters.
```cpp
   wstring blank_padded_edit_box = L"  Hello, World!   ";
   trim(blank_padded_edit_box,' ');
   // output: "Hello, World!"
```
Example 2. Input sequence is byte string. Leading and trailing characters that do not represent decimal digits must be removed. 
```cpp
   string pin = "abce{2301X*&%-";
   trim_if(pin,[](const auto& ch){return !isdigit(ch);});
   // output: "2301"
```
Example 3. Input sequence is a list with elements of `int`. Leading and trailing characters before requested marker (digit:0) must be rejected. 
```cpp
   list<int> bounded_sequence = { 1,3,6,-9,6,-23, 0,1,2,3,4,5,6,7,8,9,0, -100,99,7,4,2,56 }; 
   trim_if(bounded_sequence,[](int v){return v!=0;});
   // output: 0,1,2,3,4,5,6,7,8,9,0
```

## Further informations
* [String trim](https://www.bfilipek.com/2014/12/top-5-beautiful-c-std-algorithms.html#trim) by Bartlomiej Filipek

## Related links
[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
