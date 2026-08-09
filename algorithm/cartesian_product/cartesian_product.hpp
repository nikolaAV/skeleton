
#include <tuple>

#pragma once

/// The Cartesian product of data sets is a fundamental operation that allows generating all possible combinations of elements from multiple containers.
/// Below is a complete and functional implementation of the algorithm based on variadic templates and std::tuple.
/// The choice of std::tuple and std::apply is justified by Rule F.21 (https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines.html), 
/// which states that structures and tuples are preferable for returning and passing sets of heterogeneous data.
/// The std::apply interface acts as a "bridge" between the world of tuples (where data is packed into a single object) and variadic functions.
/// It unpacks a tuple, passing its elements as individual arguments to a function object.
/// This ensures strong static typing and allows the algorithm to work with elements of any type without performance loss.

namespace cartesian_product {

namespace detail {

/// @brief Base case: all tuples consumed; invokes @p f with the fully accumulated argument tuple.
/// @tparam F    Callable type.
/// @tparam ArgTuple Tuple holding the arguments accumulated so far.
/// @param f    The callable to invoke.
/// @param args The complete argument tuple to forward to @p f.
template <typename F, typename ArgTuple>
void cartesian_impl(F& f, ArgTuple const& args)
{
    std::apply(f, args);
}

/// @brief Recursive case: iterates each element of @p t, appends it to @p args, and recurses.
/// @tparam F        Callable type.
/// @tparam ArgTuple Tuple holding the arguments accumulated so far.
/// @tparam Tuple    The current tuple whose elements are iterated.
/// @tparam Rest     Remaining tuples to process.
/// @param f    The callable to invoke at the base case.
/// @param args Arguments accumulated from outer recursion levels.
/// @param t    The current tuple to iterate over.
/// @param rest Remaining tuples passed unchanged to the next recursion level.
template <typename F, typename ArgTuple, typename Tuple, typename... Rest>
void cartesian_impl(F& f, ArgTuple const& args, Tuple const& t, Rest const&... rest)
{
    std::apply([&](auto const&... elems) {
        (cartesian_impl(f, std::tuple_cat(args, std::tie(elems)), rest...), ...);
    }, t);
}

} // namespace detail

/// @brief Calls @p f once for every element in the cartesian product of @p ts....
/// @tparam F   Callable type; must accept one argument per tuple in @p ts.
/// @tparam Ts  Pack of tuple types whose cartesian product is iterated.
/// @param f   The callable to invoke for each combination.
/// @param ts  Tuples whose elements form the cartesian product.
template <typename F, typename... Ts>
void exec(F f, Ts const&... ts)
{
    detail::cartesian_impl(f, std::tuple<>{}, ts...);
}

}   // namespace cartesian_product


/**
 * @par Example
 * @code
 * auto const t1 = std::make_tuple(_1{}, _2{}, _3{});
 * auto const t2 = std::make_tuple(up{}, down{});
 * auto const t3 = std::make_tuple(blue{}, red{});
 *
 * cartesian_product::exec(
 *     [](auto&&... args) { ((std::cout << args << " "), ...) << '\n'; },
 *     t1, t2, t3
 * );
 * 
 *     // expected output
 *     // ----------------- 
 *     // 1 up blue
 *     // 1 up red
 *     // 1 down blue
 *     // 1 down red
 *     // 2 up blue
 *     // 2 up red
 *     // 2 down blue
 *     // 2 down red
 *     // 3 up blue
 *     // 3 up red
 *     // 3 down blue
 *     // 3 down red
 * @endcode
 *
 * @par Execution flow explanation
 * @verbatim
 * exec(f, t1, t2, t3)
 * │
 * └─── cartesian_impl(f, {}, t1, t2, t3)
 *      │   // std::apply unpacks t1 -> elems: {_1, _2, _3}
 *      │   // fold expression invokes calls for each element of t1:
 *      │
 *      ├─── [_1] -> cartesian_impl(f, {_1}, t2, t3)
 *      │    │   // std::apply unpacks t2 -> elems: {up, down}
 *      │    ├─── [up]   -> cartesian_impl(f, {_1, up}, t3)
 *      │    │    │   // std::apply unpacks t3 -> elems: {blue, red}
 *      │    │    ├─── [blue] -> cartesian_impl(f, {_1, up, blue})  // BASE CASE -> "1 up blue"
 *      │    │    └─── [red]  -> cartesian_impl(f, {_1, up, red})   // BASE CASE -> "1 up red"
 *      │    │
 *      │    └─── [down] -> cartesian_impl(f, {_1, down}, t3)
 *      │         ├─── [blue] -> "1 down blue"
 *      │         └─── [red]  -> "1 down red"
 *      │
 *      ├─── [_2] -> cartesian_impl(f, {_2}, t2, t3)
 *      │    ├─── [up]   -> ... (similar to branch _1)
 *      │    └─── [down] -> ...
 *      │
 *      └─── [_3] -> cartesian_impl(f, {_3}, t2, t3)
 *           ├─── [up]   -> ... (similar to branch _1)
 *           └─── [down] -> ...
 * @endverbatim
 */

