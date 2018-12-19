# On a way towards C++ string constants instead of using native C string literals  
This example will focus on our goal: initialize a possibly concatenated string __statically__
```cpp
   constexpr auto std      = literal("std::");
   constexpr auto out      = literal(" << ");
   constexpr auto quote    = literal("\"");
   constexpr auto phrase   = literal("Hello") + ", " + "World" + "!";
   constexpr auto phrase2  = quote+phrase+quote;
   constexpr auto expr     = std+"cout" +out+ phrase2 +out+ std+"endl";

   // std::cout << "Hello, World!" << std::endl
   // 012       ^         ^         ^         ^
   static_assert(41==expr.size());
   cout << expr << endl;
```

## Further informations
* [Compile-time string concatenation](https://akrzemi1.wordpress.com/2017/06/28/compile-time-string-concatenation/) by Andrzej Krzemienski

## Related links
* [other examples compile-time computing](https://github.com/nikolaAV/Modern-Cpp/tree/master/constexpr)
* [examples of pack expansions come with variadic templates](https://github.com/nikolaAV/Modern-Cpp/tree/master/variadic)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
