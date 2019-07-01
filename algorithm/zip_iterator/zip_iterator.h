#if !defined( _ZIP_ITERATOR_H__)
#define _ZIP_ITERATOR_H__

#include <iterator>
#include <tuple>
#include <utility>

/**
   The idea was taken from boost::zip_iterator
   The zip iterator provides the ability to parallel-iterate over several controlled sequences simultaneously.
   A zip iterator is constructed from (1) a tuple of iterators or from (2) a variadic list of singular iterators. 
   Moving the zip iterator moves all the iterators in parallel.
   Dereferencing the zip iterator returns a tuple that contains the results of dereferencing the individual iterators.

   Example:
   \code
      const vector c1   = { 1,2,3,4 };
      const deque  c2   = { .1,.2,.3 };
      const list   c3   = { 1,1,1 };
      const char   c4[] = {"hello,world!"};

      // implicitly zip::begin(), zip::end(), zip::iterator::operator* are invoked
      for(const auto&& [_1,_2,_3,_4] : zip::tuple{c1,c2,c3,c4}) {
         cout << _1 << _2 << _3 << _4 << endl;
      }
   \endcode

   \see https://www.boost.org/doc/libs/1_41_0/libs/iterator/doc/zip_iterator.html
        https://github.com/nikolaAV/skeleton/tree/master/algorithm/zip_iterator
*/

namespace zip
{
/**
   tuple of references to the specified containers
*/
template <typename... Ts>
struct tuple : std::tuple<Ts&...> {
   tuple(Ts&... ts) noexcept : std::tuple<Ts&...>(ts...) {}
};

/**
   tuple of iterators to the specified containers in order
*/

template <typename... Ts>
class iterator {
   std::tuple<Ts...> ts_; // tuple of iterators
public:
   static constexpr auto indices = std::make_index_sequence<sizeof...(Ts)>();

   explicit iterator(Ts... ts) noexcept : ts_(ts...) {}
   iterator& operator++() {
      std::apply([](auto&... ts){ (ts++,...); }, ts_);
      return *this;
   }
   auto operator*() const {
      // https://github.com/nikolaAV/Modern-Cpp/tree/master/tuple/variadic_parameter_list
      return std::apply([](auto&... ts){ return std::tuple<decltype(*ts)...>{*ts...}; },ts_);
   }
   auto tuple() const noexcept {
      return ts_;
   }
};

template <typename... Ts, size_t... Idx>
bool is_different(const iterator<Ts...>& left, const iterator<Ts...>& right, std::index_sequence<Idx...>) noexcept {
   return ((std::get<Idx>(left.tuple()) != std::get<Idx>(right.tuple())) && ...);
}

template <typename... Ts>
bool operator!=(const iterator<Ts...>& left, const iterator<Ts...>& right) noexcept {
   static_assert(left.indices.size== right.indices.size);
   return is_different(left,right,left.indices);
}

template <typename... Ts>
bool operator==(const iterator<Ts...>& left, const iterator<Ts...>& right) noexcept {
   return !(left!=right);
}         

template <typename... Ts>
auto begin(Ts&... ts) noexcept {
   return iterator{ std::begin(ts)... };
}

template <typename... Ts, size_t... Idx>
auto begin(tuple<Ts...> p, std::index_sequence<Idx...>) {
   return begin(std::get<Idx>(p)...);
}

template <typename... Ts>
auto begin(tuple<Ts...> p) {
   return begin(p,std::make_index_sequence<sizeof...(Ts)>());
}

template <typename... Ts>
auto end(Ts&... ts) noexcept {
   return iterator{ std::end(ts)... };
}

template <typename... Ts, size_t... Idx>
auto end(tuple<Ts...> p, std::index_sequence<Idx...>) {
   return end(std::get<Idx>(p)...);
}

template <typename... Ts>
auto end(tuple<Ts...> p) {
   return end(p, std::make_index_sequence<sizeof...(Ts)>());
}

}  // end of namespace zip

namespace std
{
   template <typename... Ts>
   struct iterator_traits<zip::iterator<Ts...>> {
      using iterator_category = std::forward_iterator_tag;
      using difference_type = long;
      using value_type = std::tuple<Ts...>;
   };
}  // end of namespace std


#endif // _ZIP_ITERATOR_H__
