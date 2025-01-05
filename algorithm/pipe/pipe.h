#ifndef PIPE_HEADER_GUARD_LNQTEACVKBKENVTA
#define PIPE_HEADER_GUARD_LNQTEACVKBKENVTA

#include <utility>
#include <algorithm>
#include <iterator>
#include <vector>
#include <tuple>

///
/// @brief 
///
namespace pipe
{

namespace std_ext
{

///
/// @brief A modified implementation of std::accumulate which avoids unnecessary copies
/// @see std::accumulate, https://en.cppreference.com/w/cpp/algorithm/accumulate
/// 

template <typename InputIt, typename U, typename F>
U moving_accumulate(InputIt first, InputIt last, U init, F&& f)
{
   // the standard implementation has an annoying bug 
   // by passing 'init' by value for each element in the collection

    for (; first != last; ++first) {
        init = f(std::move(init), *first);   // <--- !!!
    }
    return init;
}

}  // namespace std_ext

///
/// @brief adapt std::algorithm signatures to use 'range' concept instead of annoyed begin/end iterators 
///
namespace range
{

///
/// @see std::copy_if, https://en.cppreference.com/w/cpp/algorithm/copy
/// 
template <typename C, typename OutputIt, typename UnaryPred>
auto copy_if(C&& in, OutputIt d_first, UnaryPred&& pred)
{
   return std::copy_if(std::begin(in), std::end(in), d_first, std::forward<UnaryPred>(pred));
}

///
/// @see std::transform, https://en.cppreference.com/w/cpp/algorithm/transform
/// 
template <typename C, typename OutputIt, typename UnaryOp>
auto transform (C&& in, OutputIt d_first, UnaryOp&& unary_op)
{
   return std::transform(std::begin(in), std::end(in), d_first, std::forward<UnaryOp>(unary_op));
}

///
/// @see std::accumulate, https://en.cppreference.com/w/cpp/algorithm/accumulate
/// 
template <typename C, typename U, typename BinaryOp>
auto accumulate (C&& in, U&& init, BinaryOp&& op)
{
   return std_ext::moving_accumulate(std::begin(in), std::end(in), std::forward<U>(init), std::forward<BinaryOp>(op));
}

} // namespace range

namespace impl
{

template<typename C>
using collection_element_type_t = typename std::remove_reference_t<decltype(*std::begin(std::declval<C&>()))>;

///
/// @brief filter: (collection<T>, (T -> bool)) -> collection<T>
///
template <typename C, typename F>
auto filter(C&& in, F&& f)
{
   using T = collection_element_type_t<C>;
   std::vector<T> out;
   std::ignore = range::copy_if(in, std::back_inserter(out), std::forward<F>(f));
   return out;
}

///
/// @brief transform: (collection<T>, (T -> U)) -> collection<U>
///
template <typename C, typename F>
auto transform(C&& in, F&& f)
{
   using T = collection_element_type_t<C>;
   using U = decltype(f(std::declval<T>()));
   std::vector<U> out;
   std::ignore = range::transform(in, std::back_inserter(out), std::forward<F>(f));
   return out;
}

///
/// @brief fold: (collection<T>, U, ((U,T) -> U)) -> U
///
template <typename C, typename U, typename F>
auto fold(C&& in, U&& init, F&& f)
{
   return range::accumulate(in, std::forward<U>(init), std::forward<F>(f));
}

} // namespace impl

template <typename F>
auto filter(F&& f)
{
   return [&](auto&& in)
   {
      return impl::filter(std::forward<decltype(in)>(in), std::forward<decltype(f)>(f));
   };
}

template <typename F>
auto transform(F&& f)
{
   return [&](auto&& in)
   {
      return impl::transform(std::forward<decltype(in)>(in), std::forward<decltype(f)>(f));
   };
}

template <typename F, typename U>
auto fold(F&& f, U init)
{
   return [&](auto&& in)
   {
      return impl::fold(std::forward<decltype(in)>(in), std::move(init), std::forward<decltype(f)>(f));
   };
}

//
// @brief pipe builder/compositor
//    that allows to compose functions in a more readable way like 
//    auto result = input | filter | transform | do_something_else;
// 
template <typename C, typename F>
auto operator|(C&& in, F&& f)
{
   return f(std::forward<C>(in));
}

}  // namespace pipe

 
#endif // PIPE_HEADER_GUARD_LNQTEACVKBKENVTA
