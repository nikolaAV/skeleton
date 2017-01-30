# type-safe printf("% % %", 1, "2", 3.)
an example (from ``A brief introduction to Variadic templates'' (see references)) implementing a general, type-safe, printf()

It would probably be better to use boost::format, but consider:
```cpp
// C run time function int printf( const char* format, ... ); http://en.cppreference.com/w/cpp/io/c/fprintf
const string pi = "pi";
const char* m = "The value of %s is about %g (unless you live in %s).\n";
printf(m,pi,3.14159,"Indiana");

// type-safe version, focus on the format string below: converion specifiers are absent
const string pi = "pi";
const char* m = "The value of % is about % (unless you live in %).\n";
type_safe::printf(m,pi,3.14159,"Indiana");
```

# Building test
To build the tests, you will require the following:

* [TUT framework](https://github.com/mrzechonek/tut-framework), to be cloned under the 'my_root_tut' directory in your file system structure 
* specify a path 'my_root_tut/include' in your project global settings
* include 'tut.h' header in every translation unit you want to test

# Related links
* [C++11FAQ](http://www.stroustrup.com/C++11FAQ.html#variadic-templates) by Bjarne Stroustrup
* [Modern-Cpp,variadic templates] (https://github.com/nikolaAV/Modern-Cpp/tree/master/variadic) 
