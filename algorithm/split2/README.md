# Split2, "string_view" slider
This function moves a [`std::string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view) slider between delimiter markers __without__ any unnecessary copies or data modifications. 
The interface is very similar to [`std::for_each`](https://en.cppreference.com/w/cpp/algorithm/for_each) where an user-defined function object is to apply to every sequence of characters which are enclosed within specified delimeters - another sequence of characters.
```cpp
template <typename StringView, typename UnaryOperation>
UnaryOperation split(StringView s, StringView delim, UnaryOperation op)
```
where 
* s - the range `std::string_view` sequential characters to apply the function to
* delim - the range `std::string_view` sequential characters specifying boundary marker
* op - 	function object, to be applied to each occurrence separated by the boundary marker

Usage example for split:
```cpp
   struct callback {
      vector<int> out;
      void operator()(string_view s) {
         int value{0};
         from_chars(s.data(),s.data()+s.size(),value);
         out.push_back(value);
      }
   };
   const auto result = split("1<$>2<$>3<$>4<$>5","<$>",callback{});
   assert((vector{1,2,3,4,5})==result.out);
```

## Further informations
* [How to split a string in C++](https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/) by Jonathan Boccara
* [How to split a string in C++](http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html) by Martin Broadhurst

## Related links
* [std::transform variation](../split)

[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
