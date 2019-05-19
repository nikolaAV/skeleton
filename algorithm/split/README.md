# Split
It can split any range of items at each occurrence of a specific value, and it copies the chunks that result from that into an output range.
The `split` algorithm works in a similar manner to [`std::transform`](http://en.cppreference.com/w/cpp/algorithm/transform) because
it accepts a pair of begin/end iterators of an input range and an output iterator.
It does something with the input range, which, in the end, results in
assignments to the output iterator. Apart from that, it accepts an item value
called 'value' and a binary function called 'binary_op'. 
```cpp
template< typename InputIt, typename OutputIt, typename T, typename BinaryOperation >
void split( InputIt first1, InputIt last1, OutputIt d_first, const T& value, BinaryOperation binary_op )
```
where 
- 'value' - value to compare the elements to, i.e. is a separator which cuts the input sequence of elements.
- 'binary_op' - accepts to iterators that mark the beginning and end of subrange that represents exactly one splice between two separators. The signature of the binary function should be equivalent to the following: `iterator_traits<OutputIt>::value_type binary_op(InputIt,InputIt)`

There is also version `split_if` where the separator is represented by unary function (predicate) which returns `true` for the required element if it meets the user-defined condition. The signature should be: `bool pred(const iterator_traits<InputIt>::value_type&)` 

Usage example for split:
```cpp
    // calculate sum for each splice separated by zero, i.e. 0
   const vector<int> v {0,1,0,1,2,0,1,2,3,0,1,2,3,4,0,1,2,3,4,5};   
   //                     1     3       6        10          15
   list<int>   sums;
   split(begin(v),end(v),back_inserter(sums),0,[](auto slice_begin, auto slice_end){
      return accumulate(slice_begin,slice_end,0);      
   });
   // sums: 1,3,6,10,15
```
Usage example for split_if:
```cpp
    // find all words in a sentence excluding punctuation symbols like ',',' ','<new line>',etc. 
   const string qoute = "Don't comment bad code - rewrite it.\n --Brian Kernighan";
   vector<string> words;
   split_if(begin(qoute),end(qoute),back_inserter(words)
      ,[](char ch) {
            return ch==' ' || ch=='-' || ch=='.'  || ch=='\n';
         }
      ,[](auto slice_begin, auto slice_end){
            return string(slice_begin,slice_end);      
         }
   );
   // words: "Don't","comment","bad","code","rewrite","it","Brian","Kernighan"
```

## Further informations
* [How to split a string in C++](https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/) by Jonathan Boccara
* [How to split a string in C++](http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html) by Martin Broadhurst

## Related links
* [based on std::string_view](../split2)  
[back to algorithm](../)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
