# for_each algorithm in context of the regular expression
It iterates over all matches of a regular search and applies the given function object 'f' to each macth in the target sequence of symbols, in order.
```cpp
const string data =	
   "<book>\n"
      "<title>THE C++STANDARD LIBRARY. A Tutorial and Reference</title>\n"
      "<author>Nicoli M. Josuttis</author>\n"
      "<publisher>Addison-Wesley</publisher>\n"
      "<year>2012</year>\n"
   "</book>\n";
for_each(data,regex{"<(.*)>(.*)</(\\1)>"},[](auto const& m){
   cout << left << setw(12) << m[1] << m[2] << endl; 
});														

---
output: 
title       THE C++STANDARD LIBRARY. A Tutorial and Reference
author      Nicoli M. Josuttis
publisher   Addison-Wesley
year        2012
```
where _for_each_ defined as
```cpp
template<class CharT, class UnaryFunction>
UnaryFunction for_each(const basic_string<CharT>& str, const basic_regex<CharT>& rx, UnaryFunction f);
template<class InputIt, class UnaryFunction>
UnaryFunction for_each(InputIt first, InputIt last, const basic_regex<CharT>& rx, UnaryFunction f);
```
* first, last - the range of a character sequence to apply the function to. They are [_InputIterator_](https://en.cppreference.com/w/cpp/named_req/InputIterator)  
* rx - object representing the regular expression to check whether a character sequence matches a specific pattern
* f  - function object of Unary function type, to be applied to the match-subset in the range of a character sequence [first, last).  The signature of Unary function must be `void fun(const std::match_results<CharT>&)`.
* _for_each_ returns 'f'
* exceptons: regex_error and any other which can be thrown by 'f'

## Further informations
* [Regular expressions library](https://en.cppreference.com/w/cpp/regex)

## Related links
* [Tokenizing input with the regular expression](https://github.com/nikolaAV/Modern-Cpp/tree/master/regex)

[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
