# skeleton
contains set of outlines (complete projects to be used) based on well known design patterns or C++ idioms  

* [algorithms](./algorithm) In some situations, the existing STL algorithms are not enough. But nothing hinders us from implementing our own.
* [Sequence of ASCII characters](./ascii_string) C++ string type for managing characters with codes in the range [0,128) called 7-bit [ASCII Chart Table](https://en.cppreference.com/w/cpp/language/ascii).
* [charsets conversions](./cast_charset) It provides transformation std::strings (C-strings i.e. zero terminated) of __fixed__ charsets (US-ASCII, ISO-LAtin-1, ISO-LAtin-9) into wstrings (UCS-2, UCS-4) and vice versa.
* [pointer_cast](./cast_pointer) Safe & Portable with compile-time support pointer converting to any type into integer representation.
* [string representation of integral types](./cast_string) `std::stringstream` based technique to convert strings to numbers and vice versa.
* [`std::iterator_traits<>` extensions](./iterator_traits2) to detect [_iterator categoty_](https://en.cppreference.com/w/cpp/iterator) at compile time.
* [parallel::cout](./parallel_cout) is a equivalent to `std::cout` to be used in multithreading programming.
* [parallel::dag](./parallel_dag) An easy way how to transform sequential tasks described by [Directed acyclic graph](https://en.wikipedia.org/wiki/Directed_acyclic_graph) into a concurrent analog to be used on multiple CPU cores. 
* [parallel::weak_this_idiom](./weak_this_idiom) The technique gives you a guarantee that a target object still exists or has been already destroyed when an asynchronous callback is invoked.
* [passkey idiom](./passkey_idiom) How to get restricted friendship.
* [Auto generation of !=,>,<=,>=](./rel_ops_auto) for any object of type __T__ with given a user-defined `operator==` and `operator<`.
* [self_registration](./self_registration) Polymorphic types with self registration ability just to get rid of Factory modification.
* [string constants](./static_string) String literal manipulation at compile time.
* [compiler warning suppress](./std_warning_suppress) Enabling/Disabling particular warnings come from std. 
* [string_switch](./switch_string) Language extension: how to make `switch` statement accepting string literal constants.
* [strong_types](./strong_types) By carrying meaning, these types improve the readability and safety of the code.
* [type safe prinf](./type%20safe%20printf) At compile time variable type deduction to be printed by __'printf("% % %", 1, "2", 3.)'__
* [Unit Test Project Template](./unit_test_substrate) A ready to be used Unit Test Machine based on [TUT framework](http://mrzechonek.github.io/tut-framework/).
* [User Defined & system errors](.user_extensible_error) An easy way to integrate own errors into [the standard system errors](https://en.cppreference.com/w/cpp/header/system_error). 
* [URL Reader](./url_reader) The very easy way to get a content a web page by specified URL. (Windows only)
