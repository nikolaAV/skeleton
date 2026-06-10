# Migration Guide: C-style → Functional Pipeline (C++23)
For people who usually read from left to the right in  their native language, might find it difficult reading sequential function call in C++ language.
```cpp
h(g(f()))
```
but it is quite natural to see that intention of the code above is a sequence of calls
```cpp
f -> g -> h
```
Situation is getting more complicated when functions to be invoked have different number of parameters of different types and moreover may have different return code (or exceptions) to show how execution ends, successfully ot not.

Let's suppose we are having set of third-party functions which are not allowed to modify (a fictitious example for demonstration)
```cpp
namespace api {

struct Code01 {...};
struct Code02 {...};
struct Code03 {...};

Code01 get_current_login(std::string& login_out);
Code02 user_full_name(std::string const& login_in, std::pair<std::string, std::string>& full_name_out);
Code03 to_html(std::pair<std::string, std::string> const& full_name_шт, std::string& html_out);

}
```
and let the naive solution be called ***spaghetti***
```cpp
std::string spagetti_variant()
{
    using namespace api;

    std::string login;
    Code01 login_code = get_current_login(login);
    if (login_code.code == Code01::Success)
    {
        std::pair<std::string, std::string> full_name;
        Code02 full_name_code = user_full_name(login, full_name);
        if (full_name_code.value == Code02::Yes)
        {
            std::string http;
            Code03 html_code = to_html(full_name, http);
            if (html_code.result == Code03::True)
            {
                return http;
            }
            else
            {
                std::cerr << "Error: " << html_code.what << ", with code "<< html_code.result << std::endl;
            }
        }
        else 
        {
            std::cerr << "Error: " << full_name_code.message << ", with code "<< full_name_code.value << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: " << login_code.reason << ", with code "<< login_code.code << std::endl;
    }
    return {};
}

int main()
{
    std::cout << spagetti_variant() << std::endl;
}
```
our purpose is to make code particularly organized to be able to write the code like that following a functional programming style
```cpp
std::string pipeline_variant()
{
    using namespace pipeline;
    return start | current_login | full_name | to_html | stop;
}

int main()
{
    std::cout << pipeline_variant() << std::endl;
}
```
The goal is to replace deeply nested if-checks ("arrow anti-pattern") with a linear chain of composable steps, each of which either produces a value or carries an error forward — without the caller having to inspect the result at every stage.
The mechanism is [`std::expected<T,E>`](https://en.cppreference.com/cpp/utility/expected) combined with a custom operator`|` that acts as a monadic bind [("railway-oriented programming")](https://fsharpforfunandprofit.com/rop).

## Step 1. Define a single error carrier
The C-style API uses three different code structs (Code01 / Code02 / Code03), each with its own specific content to describe operation failure

Create one uniform error type inside your pipeline namespace:
```cpp
// BEFORE (spread across three structs, three field names)
struct Code01 { enum ECode { Success, Error } code;   std::string reason; };
struct Code02 { enum Value { Yes, No }        value;  std::string message; };
struct Code03 { enum Result { True, False }   result; std::string what; };

// AFTER (one type, used by every step)
struct ErrorMessage { std::string value; };
```
This single type `ErrorMessage` becomes the E in `std::expected<T, E>` for the whole pipeline.

## Step 2. Introduce type aliases to make notation shorter
Inside your pipeline namespace add convenience aliases so other signatures stay readable:
```cpp
template<typename T>
using expected  = std::expected<T, ErrorMessage>;
using unexpected = std::unexpected<ErrorMessage>;
```

Every step in pipeline chain (enclosed between `|`) now returns `expected<SomeType>` instead of a raw code struct.

## Step 3. Wrap each api call
For every C-style function, write a thin wrapper that:
1. calls the original API function,
2. returns the output value on success,
3. returns unexpected{error_message} on failure.

Pattern:
```cpp
auto STEP_NAME(INPUT_TYPE input) -> expected<OUTPUT_TYPE>
{
    OUTPUT_TYPE out;
    auto const c = api::ORIGINAL_FUNCTION(input, out);
    if (SUCCESS_CONDITION)
        return out;
    return unexpected{{ERROR_FIELD}};
}
```
Concrete example of migration

| Before | After |
| --- | --- |
| expected<pair<string,string>> n;<br>Code02 c = user_full_name(l,n);<br>if (c.value == Code02::Yes)<br>&ensp;{ /* use n */ }<br>else<br>&ensp;cerr << c.message; | auto full_name(string const& login) -> expected<pair<string,string>><br>{<br>&ensp;pair<string,string> out;<br>&ensp;auto c = api::user_full_name(l,out)<br>&ensp;if (c.value == Сode02::Yes)<br>&ensp;&ensp;return out;<br>&ensp;return unexpected{{c.message}};<br>}   |

## Step 4. add the monadic bind operator
Place this operator `|` once inside the pipeline namespace. It is found via ADL (Argument Dependent Lookup) whenever the left operand is `expected<T, ErrorMessage>`.

```cpp
template<typename T, typename F>
auto operator|(expected<T> res, F&& f)
    -> decltype(std::forward<F>(f)(std::declval<T>()))
{
    if (res.has_value())
        return std::forward<F>(f)(std::move(res.value()));
    return unexpected{res.error()};   // short-circuit: skip f
}
```
How it works:
* Success path:  unwraps the value, passes it to f, returns f's result.
* Error path:    ignores f entirely, wraps the existing error in the
                   return type of f (preserving the E type).

## Step 5. add `start` and `stop` bookends
To make the chain self-contained add:

`start` — a sentinel `expected<void>{}` that kicks off the pipeline without requiring an initial value. Its operator`|` overload simply calls the first step with no argument:
```cpp
auto start {expected<void>{}};

template<typename E, typename F>
auto operator|(std::expected<void, E>, F&& f) -> decltype(f())
{
    return std::forward<F>(f)();
}
```

`stop` — converts the final `expected<string>` back to a plain `std::string` and handles the error-reporting in one place:
```cpp
auto stop(expected<string> const& res) -> string
{
    if (res.has_value()) return res.value();
    std::cerr << "Error: " << res.error().value << "\n";
    return {};
}

auto operator|(expected<string> res,
                string(*f)(expected<string> const&)) -> string
{
    return f(res);
}
```
## Step 6. compose the pipeline
```cpp
string pipeline_variant()
{
    using namespace pipeline;
    return start | current_login | full_name | to_html | stop;
}
```

## CHECKLIST
*  [✓] Compiler set to C++23 (or C++17 + expected.hpp polyfill)
*  [✓] One ErrorMessage struct covers all API error strings
*  [✓] expected<T> / unexpected type aliases defined in pipeline namespace
*  [✓] Each API call wrapped in its own expected<T>-returning function
*  [✓] operator|(expected<T>, F) defined once in pipeline namespace
*  [✓] start sentinel and stop sink added
*  [✓] Call site is a single | chain with no if-checks


## TRADE-OFFS
* Advantages of the pipeline style:
  * Linear, top-to-bottom reading order — no indentation pyramid.
  * Error propagation is automatic; new steps cannot accidentally ignore it.
  * Each wrapper is independently unit-testable.
  * Adding or reordering a step requires changing only one line.

* Costs to be aware of:
  * Requires C++23 (or a polyfill) — not usable with older toolchains as-is.
  * The operator| trick is non-obvious to developers unfamiliar with monads.
  * expected<void> as start sentinel is a convention, not a standard idiom.
  * Stack traces through operator| are slightly less direct than plain ifs.


## Further informations
* [C++23 std::expected — Mastering Monadic Error Handling Pipelines](https://towardsdev.com/cpp23-std-expected-monadic-operation-2c62a2eedbaf) by Sagar
* [Monad (functional programming)](https://en.wikipedia.org/wiki/Monad_(functional_programming)) on wiki
* [Railway Oriented Programming](https://fsharpforfunandprofit.com/rop) on F# for Fun and Profit
* [Monads in pictures](https://www.adit.io/posts/2013-04-17-functors,_applicatives,_and_monads_in_pictures.html) by Aditya Y Bhargava, [Grokking Algorithms](https://www.amazon.com/Grokking-Algorithms-Second-Aditya-Bhargava-dp-1633438538/dp/1633438538)

## Related links
* [std::optional as a monad](https://github.com/nikolaAV/Modern-Cpp/tree/master/stl/optional#example-2-chain-building-for-function-calls)
* [another example of pipeline](../algorithm/pipe/)

## Compilers
* [GCC 14.3.0](https://wandbox.org/)
