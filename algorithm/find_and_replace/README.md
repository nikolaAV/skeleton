# Find and Replace All Occurrences in a C++ String
`std::string` has a set of overloaded [`replace`](https://en.cppreference.com/w/cpp/string/basic_string/replace) methods that replace only one string part at a time.
A proposed below implementation has no such limitation and provides the effective way to replace of a substring of any type [`std::basic_string<>`](https://en.cppreference.com/w/cpp/string/basic_string) with another one. 

```cpp
   string text1;
   text = "I have a blue house and a blue car";
   assert(find_and_replace(text1,"blue","red")=="I have a red house and a red car");

   wstring text2;
   text = L"   Remove all spaces.";
   assert(find_and_replace(text2,L" ",L"")==L"Removeallspaces.");
```

## Further informations
* [Find and Replace All Occurrences in a C++ String](https://bits.mdminhazulhaque.io/cpp/find-and-replace-all-occurrences-in-cpp-string.html) by Md. Minhazul Haque
* [How to replace all occurrences of a character in string?](https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string)

## Related links
[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
