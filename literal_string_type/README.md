# String literals as non-type template parameters  
> A topic mentioned here is currently (Desember, 2019) under discussion to be approved to be in the Standard. But If you want to use this today, GCC and Clang have implemented it as an extension.
### Motivation
Compile-time strings are a sorely missed piece of functionality in C++. 
Indeed, while we can pass a string as a function argument, there is no way of getting a string as a compile-time entity from within a function. 
This prevents a function from creating an object whose type depends on the contents of the string being passed.  
One of the most desirable feature for many programmers now is to pass C-style string literal as template argument 
```cpp
template <const char* str>
struct Foo {
    const char* get() const {
         return str;
    }
};

int main()
{
    Foo<"String"> foo;  // <-- error
    cout<<foo.get();
}
```
### Solution (Clang, Gcc)
Encode the string as a type:
```cpp
template <char... chars>
struct char_sequence : std::integer_sequence<char, chars...> { 
    static const char* value() {
        static constexpr char str[sizeof...(chars) + 1] = { chars..., '\0' };
        return str;
    }
};
```
Create a user defined literal operator. That is what non-standard ([n3599](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3599.html)) functionality not in C++14/C++17 but that is supported by recent GCC and Clang builds, and hopefully will be reconsidered for C++20.
```cpp
template <typename T, T... chars>
constexpr char_sequence<chars...> operator""_cs() { return { }; }
```
And vu a la, this allows us to write
```cpp
Foo<decltype("string"_cs)>;
```
or we can even write it shorter if we are not unwilling to use a macro:
```cpp
#define LITERAL(s) decltype("" s ""_cs)

Foo<LITERAL("string")>; // <-- well done
```

## Further informations
* [String literals as non-type template parameters](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0424r2.pdf)
* [C-Style Strings as template arguments?](https://stackoverflow.com/a/28209546) 
* [Literal operator templates for strings](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3599.html)
* [Template literal operator for string literals](https://groups.google.com/a/isocpp.org/forum/#!topic/std-proposals/X4rnPAOB0as)

## Related links
* [Compile-time string concatenation](../static_string)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Not supported by Microsoft (R) C/C++ Compiler 19.14 
