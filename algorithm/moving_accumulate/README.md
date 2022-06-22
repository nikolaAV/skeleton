# [`std::accumulate`](https://en.cppreference.com/w/cpp/algorithm/accumulate) with moving support of accumulated value(until C++20).
Implementation of accumulate(InputIt first, InputIt last, T init, BinaryOperation op) delegates calls to `op` which receives `init` by value,
which induces a copy whenever std::accumulate calls `op` and passes it the value it accumulated so far.
Always creating copies in this case is an unnecessary pessimization that has already been fixed in C++20.
For any reason if you are made to use a compiler supporting earlier language Standard, we can roll our own variant of std::accumulate,
which will trigger the C++ move semantics on the accumulated value instead of creating copies (just as std::accumulate does it in C++20). 
```cpp
template<class InputIt, class T, class BinaryOperation>
T accumulate(InputIt first, InputIt last, T init, BinaryOperation op)
{
    for (; first != last; ++first) {
        init = op(std::move(init), *first); // std::move since C++20
    }
    return init;
}
```
Algorithm `accumulate` is one of well-known technique to implement a powerful concept - Folding (or reduction) when we need to process
a collection of items one item at a time, in order to calculate something.
The result might be as simple as a sum or a product of all items in the collection,
or a number of all items that have a specific value or that satisfy a predefined predicate.
The result might also be something more complex, such as a new collection that contains only a part of the original (filtering),
or a new collection with the original items reordered (for sorting or partitioning).
In fact, algorithm `accumulate` is almost best choice because implementing Folding we can avoid more complex and having bad publicity approaches: loops and recursion. 

## Further informations
* [Fold Expressions](https://www.bfilipek.com/2017/01/cpp17features.html#fold-expressions) by Bartlomiej Filipek 
* [moving-accumulate](https://gitlab.com/manning-fpcpp-book/code-examples/-/blob/master/chapter-02/moving-accumulate/main.cpp) by Ivan Čukić
  
## Related links
* [Fold expressions. Examples.](https://github.com/nikolaAV/Modern-Cpp/tree/master/variadic/fold_expressions)
* [Implementing transform_if using std::accumulate](https://github.com/nikolaAV/Modern-Cpp/tree/master/lambda/lambda_currying2)
* [greatest common divisor](../gcd)

[back to algorithm](../)

## Compilers
* [GCC 7.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
