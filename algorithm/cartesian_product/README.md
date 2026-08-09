# Cartesian product
This code implements the Cartesian product algorithm for an arbitrary number of tuples (`std::tuple`) using a functional approach, recursion, and variadic templates.
The Cartesian product of data sets is a fundamental operation that allows generating all possible combinations of elements from multiple containers (represented as `std::tuple`).

```cpp
#include "cartesian_product.hpp"

int main()
{
   cartesian_product::exec(
         [](int v1, char const* v2, char const* v3) { std::cout << v1 << " " << v2 << " " << v3 << std::endl; }
        ,std::make_tuple(1, 2, 3)
        ,std::make_tuple("up", "down")
        ,std::make_tuple("blue", "red")
   );
   // output:
   // ----------------- 
   // 1 up blue
   // 1 up red
   // 1 down blue
   // 1 down red
   // 2 up blue
   // 2 up red
   // 2 down blue
   // 2 down red
   // 3 up blue
   // 3 up red
   // 3 down blue
   // 3 down red
}    
```

## Further informations
* [std::apply`](https://en.cppreference.com/cpp/utility/apply)
* [How to Generate All the Combinations from Several Collections](https://www.fluentcpp.com/2022/03/18/how-to-generate-all-the-combinations-from-several-collections/) by Jonathan Boccara

## Related links
* [Unpacking & enumerating Tuples in C++14](https://github.com/nikolaAV/Modern-Cpp/tree/master/tuple/unpacking_tuples)
* [Generating cartesian product pairs of any input at compile time](https://github.com/nikolaAV/Modern-Cpp/tree/master/lambda/lambda_cartesian)
* [back to algorithm section](../)

## Compilers
* [GCC 14.3.0](https://wandbox.org/) with `-std=c++17` option
