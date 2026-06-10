#ifndef HEADER_GUARD_UARGSVTCCPWYZKVU
#define HEADER_GUARD_UARGSVTCCPWYZKVU

#include <variant>
#include <utility>

namespace std_ext {

template <typename E>
struct unexpected {
    E value;
};

template<typename T, typename E>
class expected {
private:
    // unexpected wraps E so variant<T, unexpected> stays unambiguous even when T == E.
    using unexpected_t = unexpected<E>;
    std::variant<T, unexpected_t> storage_;

public:
    expected(T val) : storage_(std::move(val)) {}
    expected(unexpected_t err) : storage_(std::move(err)) {}

    bool has_value() const { return storage_.index() == 0; }
    explicit operator bool() const { return has_value(); }

    T&       value()       { return std::get<0>(storage_); }
    T const& value() const { return std::get<0>(storage_); }

    E&       error()       { return std::get<1>(storage_).value; }
    E const& error() const { return std::get<1>(storage_).value; }
};

// Monadic bind — "railway-oriented" pipeline.
// Found via ADL on std_ext::expected; propagates the error unchanged.
template<typename T, typename E, typename F>
auto operator|(expected<T, E> res, F&& f)
    -> decltype(std::forward<F>(f)(std::declval<T>()))
{
    if (res.has_value())
        return std::forward<F>(f)(std::move(res.value()));
    return unexpected<E>{res.error()};
}

} // namespace std_ext

#endif // HEADER_GUARD_UARGSVTCCPWYZKVU
