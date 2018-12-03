# transform_if
Most developers who have used [`std::copy_if`](https://en.cppreference.com/w/cpp/algorithm/copy) and [`std::transform`](https://en.cppreference.com/w/cpp/algorithm/transform) may have asked themselves already, why there is no std::transform_if.
The `std::copy_if` function copies items from a source range to a destination range, but skips the items that are not selected by a user-defined predicate function.
The `std::transform` unconditionally copies all items from a source range to a destination range but transforms them in between.
The transformation is provided by a user-defined function, which might do simple things, such as multiplying numbers or transforming items to completely different types.
Such functions have been there for a long time now, but there is still no `std::transform_if` function.
The possible implementation can be:
```cpp
    template< typename InputIt, typename OutputIt, typename UnaryPredicate, typename UnaryOperation>
    OutputIt transform_if( InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred, UnaryOperation op)
    {
        for(;first!=last;++first)
            if(unary_pred(*first))
                *d_first++=unary_op(*first);
        return d_first;
    }
```
* first, last - the input range of elements to examine. They must meet the requirements of [_InputIterator_](https://en.cppreference.com/w/cpp/named_req/InputIterator)  
* d_first - the beginning of the destination range. It must meet the requirements of [_OutputIterator_](https://en.cppreference.com/w/cpp/named_req/OutputIterator)
* pred - unary predicate which returns `true` for the required elements
* op - unary operation function object that will be applied.  

Example of usage:
```cpp
   const auto seq = {1,2,3,4,5,6};
   transform_if(begin(seq), end(seq), ostream_iterator<int>(cout,",")
      ,[](int v) { return 0==v%2; } // even
      ,[](int v) { return v*2; } // twice
   );
   ---
   output: 4,8,12,
```

## Further informations
TBD

## Related links
* [find_all](../find_all)

[back to algorithm](../)


## Compilers
* [GCC 5.5.0](https://wandbox.org/)
* [clang 5.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
