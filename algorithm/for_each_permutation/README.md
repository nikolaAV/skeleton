# Generating permutations of input sequences
When testing code that must deal with sequences of inputs where the order of the arguments is not important, it is beneficial to test whether it results in the same output for __all possible permutations__ of that input.
Such a test could, for example, check whether a self-implemented sort algorithm sorts correctly. 
No matter for what reason we need all permutations of some value range, 'for_each_permutation' can conveniently do it for us.
```cpp
   const auto seq = {'a','b','c'};
   for_each_permutation(begin(seq), end(seq), [](auto first, auto last) {
      copy(first,last,ostream_iterator<char>{cout,", "});
      cout << endl;
   });
   ---
   output: 
    a, b, c, 
    a, c, b, 
    b, a, c, 
    b, c, a, 
    c, a, b, 
    c, b, a, 
```
where for_each_permutation defined as
```cpp
   template< class InputIt, class RangeFunction >
   RangeFunction for_each( InputIt first, InputIt last, RangeFunction f);
```
* first, last - the range to apply the function to. They are [_InputIterator_](https://en.cppreference.com/w/cpp/named_req/InputIterator)  
* f - function object, to be applied to a permutation based on the range [first,last). The signature of the function should be equivalent to the following: void fun(InputIt, InputIt).

## Further informations
TBD

## Related links
[back to algorithm](../)

## Compilers
* [GCC 7.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
