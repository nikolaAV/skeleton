# 'match' and the Power of Pattern Matching
From a list of provided alternatives __match__ applies the selected one to the requested variant.
```cpp
template <typename Variant, typename... Matcher>
auto match(Variant&& v, Matcher&&... ms) {
   return std::visit(
       details_::overloaded{std::forward<Matcher>(ms)...} 
      ,std::forward<Variant>(v)
   );
}
```
[`details_::overloaded`(aka The overload Pattern)](https://www.bfilipek.com/2019/02/2lines3featuresoverload.html) allows us to write a lambda as a handler for each type which can be stored in [std::variant](https://en.cppreference.com/w/cpp/utility/variant), 
almost like [pattern matching](https://en.wikipedia.org/wiki/Pattern_matching) - a nice language feature which already exists in various other languages like [Scala](https://docs.scala-lang.org/tour/pattern-matching.html) or [Rust](https://doc.rust-lang.org/1.8.0/book/patterns.html), and is a core building block in most functional languages (e.g. [Haskell](https://www.haskell.org/tutorial/patterns.html)). 
As of today, we can only emulate pattern matching in C++ using [libraries](https://github.com/mpark/patterns), but there are already proposals on the way to add this as a native language feature in the future ([P1371](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1371r0.pdf), [P1260](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1260r0.pdf)). 
The function __match__ is a little wrapper around the `overloaded` helper mentioned above, which doesn’t do much except for saving a little bit efforts of typing, and putting the variant argument first instead of last. 
So that we are able to write code like that
```cpp
std::variant<T1,T2,T3> v {...};
match(v,
    [](T1 t) {...}
   ,[](T2 t) {...}
   ,[](T3 t) {...}
);
```
### Example
Let's suppose there is a [heterogeneous collection](https://github.com/nikolaAV/Modern-Cpp/tree/master/variant/heterogeneous_container) of geometrical figures we are asked to program total area computation  of them. 
We are restricted that the figures design come from outside and is not allowed to make any modification into it like adding 'area' function-member. 
Thanks to __match__ a solution may be very easy.  
```cpp
const auto total = 
accumulate(begin(c),end(c),0.,	    // <--- 'c' is a shape collection
    [](auto sum, const shape& s){   // <--- 'shape' is a std::variant<...>
            return sum + match(s,
                [](const rectangle& r)   { return r.width_*r.height_; }
               ,[](const semi_circle& c) { return 3.14159*c.radius_*c.radius_*.5; }
               ,[](const square& s)      { return r.side_*r.side_; }
               ...
		   );
    }
);
```cpp
![pattern_matching](https://www.walletfox.com/course/cheatsheetsSource/pattern_matching_classes_cheatsheet-compressor.png)

## Further informations
* [Space Game: A std::variant-Based State Machine by Example](https://www.bfilipek.com/2019/06/fsm-variant-game.html)

## Related links
* [Overloaded Lambdas](https://github.com/nikolaAV/Modern-Cpp/tree/master/lambda/lambda_overloaded)
* [Heterogeneous Container](https://github.com/nikolaAV/Modern-Cpp/tree/master/variant/heterogeneous_container)
* [C++17 Pattern matching on type](https://github.com/nikolaAV/Modern-Cpp/tree/master/variant/pattern_matching)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.1](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.16 
