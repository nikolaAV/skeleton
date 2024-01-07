# Expressive expressions in conditional statements
> If you want to go fast, if you want to get done quickly, if you want your code to be easy
to write, make it easy to read  
_Robert C. Martin_

A few adapters for [std::`all_of\any_of\none_off`](https://en.cppreference.com/w/cpp/algorithm/all_any_none_of) algorithms to make them more expressive in conditional expression on equality. 
Examples below demonstrates how verbose expressions in condition statements can be written shorter without losing of clarity.  
Instead of combination several logical `||` to describe the `true` condition     
```cpp
   if(   product == fruit::orange 
      || product == fruit::apple
      || product == fruit::cherry
      || product == fruit::pear
      || product == fruit::pineapple
      || product == fruit::banana)
   { // ...
```
it can be done alternatively like this 
```cpp
   if (product == any_of(fruit::orange
                        ,fruit::apple
                        ,fruit::cherry
                        ,fruit::pear
                        ,fruit::pineapple
                        ,fruit::banana))
   { // ...
```

## Further informations
* [std::`all_of\any_of\none_off`](https://en.cppreference.com/w/cpp/algorithm/all_any_none_of)

## Related links
* [detection_idiom](https://github.com/nikolaAV/Modern-Cpp/tree/master/concept%20(pceudo)/detection_idiom)
* [disable_templates_enable_if](https://github.com/nikolaAV/Modern-Cpp/tree/master/concept%20(pceudo)/enable_if_ctor)
* [fold_expressions](https://github.com/nikolaAV/Modern-Cpp/tree/master/variadic/fold_expressions)
* [back to algorithm section](../)

## Compilers
* [GCC 5.4.0](https://wandbox.org/) with `-std=c++14` option
* [clang 6.0.1](https://wandbox.org/) with `-std=c++14` option
* Microsoft (R) C/C++ Compiler 19.14 
