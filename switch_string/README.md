# switch statement accepting strings
How often have you written code like this:
```cpp
   if     ("value X"==v)
      do_this();
   else if("value Y"==v)
      do_that();
   else if("value Z"==v)
      do_something_else();
   else
      dont_know_what_to_do();
```
Too many! And those of you who had the chance to take a look at C# might think 'Why can't I code something similar in C++ [using `switch` statement"](https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/switch) because `case` clause accepting a string is a legal instruction there. 
It seems in C++ there is no possibility to do exactly what can be done in C#, but something very close to can be achieved.  
```cpp
   switch(hash(v)) {
      case "value X"_hash : do_this();             break;
      case "value Y"_hash : do_that();             break;
      case "value Z"_hash : do_something_else();   break;
      default :             dont_know_what_to_do();
   }
```
An idea is based on C++11 feature `constexpr`. This feature flags function as being able to evaluate at compile time. That is, if the compiler can evaluate it to a constant expression, that the compiler will replace the function call with the constant value. 
Function _hash_ (and [user-defined literal](https://en.cppreference.com/w/cpp/language/user_literal): __hash_) provides compile time transformation a string value into integer value so that  the ordinary `switch` statement can be used.
_hash_ - is a compile time string hashing function that satisfies [__Hash requirement__](https://en.cppreference.com/w/cpp/named_req/Hash) i.e. it depends only on the input and has a very low probability of yielding the same output given different input values.

### Limitation
The main problem with this approach is the possibility of [hash collisions](https://crypto.stackexchange.com/questions/8765/is-there-a-hash-function-which-has-no-collisions).
Any of implementation of hash function isn’t very strong, very obviously, but can be dealt with by the compiler.
If a collision occurs, it will be detected at compile-time and the compiler will issue a warning or an error about “duplicate case value”. In this case, the only correct workaround is to eliminate collisions by changing the hash function.

## Further informations
* [Fowler–Noll–Vo hash function](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
* [Evaluate a string with a switch in C++](https://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c/16388610#16388610)
* [Strings in C++ Switch/Case statements](https://hbfs.wordpress.com/2017/01/10/strings-in-c-switchcase-statements/)

## Related links
* [Other examples of compile time computing](https://github.com/nikolaAV/Modern-Cpp/blob/master/constexpr)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 7.1.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.16 
