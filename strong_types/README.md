# Strong Types
 Often, native types don’t tell much about the meaning of their instances.
```cpp
void setPosition(int row, int column);
```
As srated above, we could use `int`s to represent concept of rows and columns, but doing this doesn’t carry any information about what those `int` represents, and that can even get confusing in an interface.
```cpp
setPosition(12, 14);
```
At the place of **setPosition** invocation someone reading it, he or she can’t know whether **12** represents the row, the column, or even something completely unrelated. And what is worse, being aware about input parameters there is a risk of mixing up the row and the column. 
```cpp
int row {12}, column {14};
setPosition(column, row);
```
The compiler keeps a silence and you are falsely convinced that everything is Ok.  
There is an incredibly useful technique for introducing a new integer type that is almost an exact copy, yet distinct type in modern C++11 programs: an enum class with an explicitly specified underlying type. 
```cpp
enum class row  : int {};
enum class column  : int {};

void setPosition(row r, column c);
```
And it can be used like that:
```cpp
setPosition(row{12}, column{14}); // Ok
setPosition(column{12}, row{14}); // Error
```
Since C++17 the initialization is mitigated by allowing for direct list initialization of scoped enums (braces mandatory, no narrowing conversions). Empty scoped enumerations can be extraordinarily useful as strong type aliases for integral types, and the new list initialization adds the missing piece for their mainstream usage as such.  
Scoped enumerators are restricted with respect to the wrapped type: it must be an integral type (`bool`, `int`, `unsigned short`, etc.) When a `double` or a `std::string` are involved, you are out of luck, but you are able to support concept 'strong types' by means [NamedType](https://github.com/joboccara/NamedType) header-only library as an example. 

## Further informations
* [Strong types for strong interfaces](https://www.fluentcpp.com/2016/12/08/strong-types-for-strong-interfaces/)
* [Construction Rules for enum class Values, P0138R2](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0138r2.pdf)
*  

## Related links
* [Named parameters for functions in C++14](https://github.com/nikolaAV/Modern-Cpp/tree/master/tuple/named_params)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.1](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.16 
