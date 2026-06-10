// g++ main.cpp -std=c++23 -Wextra -Wall -pedantic-errors -o exe

#include <cassert>
#include <iostream>
#include <utility>
#include <string>
#include <expected>

// https://towardsdev.com/cpp23-std-expected-monadic-operation-2c62a2eedbaf

namespace api {
    // original third-party C-style API with own error codes and input/output parameters.

struct Code01 { enum ECode { Success, Error } code; std::string reason; };
struct Code02 { enum Value { Yes, No }       value; std::string message; };
struct Code03 { enum Result { True, False }   result; std::string what; };

Code01 get_current_login(std::string& login_out)
{
    login_out = "anonymous_user";
    return {Code01::Success, "OK"};
}

Code02 user_full_name(std::string const& login, std::pair<std::string, std::string>& full_name_out)
{
    std::ignore = login; // to avoid warning: unused parameter ‘login’
    full_name_out = {"John", "Smith"};
    return {Code02::Yes, "OK"};
}

Code03 to_html(std::pair<std::string, std::string> const& full_name, std::string& html_out)
{
    html_out = "<html><body>" + full_name.first + " " + full_name.second + "</body></html>";
    return {Code03::True, "OK"};
}

} // namespace api

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

// Thin wrappers that adapt the C-style API into the std_ext::expected monad.
namespace pipeline {

// Error carrier: holds the reason/message/what string from a failed API call.
struct ErrorMessage { std::string value; };

template<typename T>
using expected = std::expected<T, ErrorMessage>;
using unexpected = std::unexpected<ErrorMessage>;

auto current_login() -> expected<std::string>
{
    expected<std::string> output{""};    
    auto const c = api::get_current_login(output.value());
    if(c.code != api::Code01::Success)
        output.error() = {c.reason};
    return output;
}

auto full_name(std::string const& login) -> expected<std::pair<std::string, std::string>>
{
    std::pair<std::string, std::string> name;
    auto const c = api::user_full_name(login, name);
    if (c.value == api::Code02::Yes)
        return name;
    return unexpected{{c.message}};
}

auto to_html(std::pair<std::string, std::string> const& name) -> expected<std::string>
{
    std::string html;
    auto const c = api::to_html(name, html);
    if (c.result == api::Code03::True)
        return html;
    return unexpected{{c.what}};
}

auto start {expected<void>{}};

auto stop(expected<std::string> const & res) -> std::string
{
    if (res.has_value())
        return res.value();
    else 
        std::cerr << "Error: " << res.error().value << std::endl;
    return {};
}

///
/// @brief Monadic bind (">>=") for std::expected — "railway-oriented" pipeline.
///
/// Chains a sequence of fallible steps without nested if-checks.
/// If @p res holds a value, it is moved into @p f and the result is returned.
/// If @p res already holds an error, @p f is never called and the error is
/// propagated unchanged to the next stage ("short-circuit").
///
/// Found via ADL because the left-hand operand is std::expected<T,E>.
///
/// @tparam T   Value type carried by the left-hand expected.
/// @tparam E   Error type shared across the entire pipeline.
/// @tparam F   Callable type; must accept T and return expected<U,E> for some U.
///
/// @param  res Left-hand operand: the current pipeline state.
/// @param  f   Next step to execute when @p res is not an error.
/// @return     expected<U,E> — either the result of f(*res) or the forwarded error.
///
/// @see https://en.wikipedia.org/wiki/Monad_(functional_programming)
/// @see https://fsharpforfunandprofit.com/rop   (Railway Oriented Programming)
///
template<typename T, typename F>
auto operator|(expected<T> res, F&& f) -> decltype(std::forward<F>(f)(std::declval<T>()))
{
    if (res.has_value())
        return std::forward<F>(f)(std::move(res.value()));
    return unexpected{res.error()};
}

template<typename E, typename F>
auto operator|(std::expected<void, E> res, F&& f) -> decltype(std::forward<F>(f)())
{
    assert(res.has_value() && "this overload for std::expected<void, E> should only be used as the first step in the pipeline");
    return std::forward<F>(f)();
}

/// for the last step (stop), which returns std::string instead of expected<std::string>
auto operator|(expected<std::string> res, std::string(*f)(expected<std::string> const&)) -> std::string
{
    return f(res);
}

} // namespace pipeline

std::string pipeline_variant()
{
    using namespace pipeline;
    return start | current_login | full_name | to_html | stop;
}

int main()
{
    std::string html = pipeline_variant();  // instead of spagetti_variant()
    std::cout << html << std::endl;
}






