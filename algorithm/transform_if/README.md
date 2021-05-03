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
### alternative 1. std::accumulate + lambdas = transform_if
`transform_if` can be implemented on a way which makes it "more functional" by means `std::accumulate`. `std::accumulate` i just ageneral _folding_ function. Folding a range means applying a binary operation to an accumulator variable and stepwise every item contained in the range (the result of each operation is then the accumulator value for the next one). As this function is so general, we can do all kinds of things with it, just like implementing `transform_if`. 
```cpp
template< typename InputIt, typename OutputIt, typename UnaryPredicate, typename UnaryOperation>
OutputIt transform_if( InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred, UnaryOperation op)
{
    return std::accumulate(first, last, d_first, for_selected(pred, map_reduce(op)));
}
```
where the outcome of expression `for_selected(pred, map_reduce(op))` is then also called the _[reduce|https://github.com/nikolaAV/Modern-Cpp/tree/master/lambda/lambda_currying2/main2.cpp]_ function.
```cpp
template <typename UnaryOperation>
auto map_reduce(UnaryOperation op) {
   return [op=std::move(op)](auto out, auto const& value){
      return *out = op(value), ++out;
   };
}

template <typename UnaryPredicate, typename ReduceOperation>
auto for_selected(UnaryPredicate pred, ReduceOperation op) {
   return[pred = std::move(pred), map_op = std::move(op)](auto out, auto const& value) {
      return pred(value)? map_op(out, value) : out;
   };
}
```
### alternative 2. std::copy_if + smart_outpu_iterator = transform_if
This is an approach when transfer function is moved to the output iterator 
```cpp
template< typename InputIt, typename OutputIt, typename UnaryPredicate, typename UnaryOperation>
OutputIt transform_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred, UnaryOperation op)
{
   auto const converter = make_output_transformer(op);
   return std::copy_if(first, last, converter(d_first), pred).get_underlying();
}
```


## Further informations

## Related links
* [find_all](../find_all)
* [transform_if=std::accumulate+lambdas](https://github.com/nikolaAV/Modern-Cpp/tree/master/lambda/lambda_currying2)

[back to algorithm](../)

## Compilers
* [GCC 5.5.0](https://wandbox.org/)
* [clang 5.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
