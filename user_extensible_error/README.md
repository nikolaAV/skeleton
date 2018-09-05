# System error support in C++0x
The little header-only library [`<system_error>`](https://en.cppreference.com/w/cpp/header/system_error) provides ability users to create their own errors with full integration into the standard system errors.  
As a basis for a worked example, let's assume we're writing an HTTP library and need errors that correspond to the HTTP response codes.
#### Step 1: define the error values/constants
```cpp
   enum class http_errc
   {
     continue_request      = 100,
     switching_protocols   = 101,
     ok                    = 200,
     // ...
     forbidden             = 403,
     gateway_timeout       = 504,
     version_not_supported = 505
   };
```
#### Step 2: add a source of the errors
```cpp
   class http_error_category : public std::error_category
   {
      const char*          name() const override {...}
      std::string          message(int code) const override {...}
```
#### Step 3: give the category a human-readable name
This name does not need to be universally unique, as it is really only used when writing an error code to a `std::ostream`. However, it would certainly be desirable to make it unique within a given program.
```cpp
      const char* http_error_category::name() const {
         return "http"; 
      }
```
#### Step 4: Error values/constants should be described i.e. can be represented as human-readable strings 
```cpp
      std::string http_error_category::message(int code) const {
         switch(code) 
         {
            case errc::continue_request:
               return "Continue";
            case errc::switching_protocols:
               return "Switching protocols";
            case errc::ok:
               return "OK";
            //...
```
#### Step 5: uniquely identify the category
The identity of an error_category-derived object is determined by its address.
```cpp
      std::error_category& http_category() {
         static http_error_category instance{};
         return instance;
      }
```
#### Step 6: construct an `std::error_code` from the http_errc
The `<system_error>` implementation requires a function called __make_error_code()__ to associate an error value with a category. 
```cpp
   std::error_code make_error_code(http_errc e)
   {
      return std::error_code(
         static_cast<int>(e),
         http_category());
   }
```
#### Step 7: register for implicit conversion to `std::error_code`
For the __http_errc__ enumerators to be usable as `std::error_code` constants, enable the conversion constructor using the [`is_error_code_enum`](https://en.cppreference.com/w/cpp/error/error_code/is_error_code_enum) type trait
```cpp
namespace std
{
   template <>
   struct is_error_code_enum<http::errc>
      : public true_type {};

}
```
that allows you to write the following in the client code
```cpp
    std::error code = http_myfunction(...);
    if(code==http_errc::ok)
    {...}
```

#### Step 8 (optional): assign default error conditions
Some of the errors you define may have a similar meaning to the standard's [errc](https://en.cppreference.com/w/cpp/error/errc) error conditions.
For example, the HTTP response code 403 Forbidden (__http_errc::forbidden__, see above) means basically the same thing as __std::errc::permission_denied__.  
If you want to compare your specific http value/constant with the Standard condition value like 
```cpp
    std::error code = http_myfunction(...);
    if(code==std::errc::permission_denied)
    {...}
```
then you have to override virtual function __default_error_condition__ in your __http_error_category__ derived from [`std::error_category`](https://en.cppreference.com/w/cpp/error/error_category).
```cpp
   class http_category : public std::error_category
   {
      std::error_condition default_error_condition(int code) const override {
         switch (code)
         {
            case errc::forbidden:
               return std::errc::permission_denied;
            default:
               return std::error_condition(code, *this);
         }
      }
```
- - -  
  
You can also use this technique when wrapping the errors produced by an existing library.

## Further informations
* [System error support in C++0x](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-1.html)
* [<system_error>](https://en.cppreference.com/w/cpp/header/system_error)
## Related links
* TBD

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Visual C++ 19.14 
