#include <algorithm>
#include <utility>
#include <array>
#include <type_traits>

/**
   \brief expressive conditional statements (C++14 version)
   \see  https://github.com/nikolaAV/skeleton/tree/master/algorithm/expressive_condition
*/

namespace algo_ext {

namespace details {

struct any {
   template<typename... Args>
   constexpr static decltype(auto) test(Args&&... args) {
      return std::any_of(std::forward<Args>(args)...);
   }
};

struct none {
   template<typename... Args>
   constexpr static decltype(auto) test(Args&&... args) {
      return std::none_of(std::forward<Args>(args)...);
   }
};

struct all {
   template<typename... Args>
   constexpr static decltype(auto) test(Args&&... args) {
      return std::all_of(std::forward<Args>(args)...);
   }
};

template <typename Algo, typename... Ts>
struct array_of {
   std::array<std::common_type_t<Ts...>, sizeof...(Ts)> value;

   template <typename U>
   constexpr bool test(U v) const {
      return Algo::template test(value.begin(), value.end(), [v](auto const a) {
         return a == v;
      });
   }
};

} // namespace details

template <typename... Ts>
constexpr auto any_of(Ts&&... ts) {
   return details::array_of<details::any, Ts...>{{std::forward<Ts>(ts)...}};
}

template <typename... Ts>
constexpr auto none_of(Ts&&... ts) {
   return details::array_of<details::none, Ts...>{{std::forward<Ts>(ts)...}};
}

template <typename... Ts>
constexpr auto all_of(Ts&&... ts) {
   return details::array_of<details::all, Ts...>{{std::forward<Ts>(ts)...}};
}

template <typename U, typename A, typename... Ts>
constexpr bool operator==(U value, details::array_of<A,Ts...> const& arr)
{
   return arr.test(value);
}

template <typename U, typename A, typename... Ts>
constexpr bool operator==(details::array_of<A, Ts...> const& arr, U value)
{
   return value == arr;
}

template <typename U, typename A, typename... Ts>
constexpr bool operator!=(U value, details::array_of<A, Ts...> const& arr)
{
   return !(value == arr);
}

template <typename U, typename A, typename... Ts>
constexpr bool operator!=(details::array_of<A, Ts...> const& arr, U value)
{
   return !(value == arr);
}

} // namespace algo_ext

#include <iostream>
#include <cassert>

int main()
{
   using namespace std;
   using namespace algo_ext;

   int v = 10;

   cout << (v == any_of(1, 2, 3, 4, 5, 6)) << endl;
   cout << (5 == any_of(1, 2, 3, 4, 5, 6)) << endl;

   cout << (v == none_of(1, 2, 3, 4, 5, 6)) << endl;
   cout << (5 == none_of(1, 2, 3, 4, 5, 6)) << endl;

   cout << (v == all_of(1, 2, 3, 4, 5, 6)) << endl;
   cout << (5 == all_of(1, 2, 3, 4, 5, 6)) << endl;
   cout << (5 == all_of(5, 5, 5, 5, 5, 5)) << endl;

   assert(v != any_of(1, 2, 3, 4, 5, 6));
   assert(v == none_of(1, 2, 3, 4, 5, 6));
   assert(v != all_of(1, 2, 3, 4, 5, 6));

   assert(5 == any_of(1, 2, 3, 4, 5, 6));
   assert(5 != none_of(1, 2, 3, 4, 5, 6));
   assert(5 != all_of(1, 2, 3, 4, 5, 6));
   assert(5 == all_of(5, 5, 5, 5, 5, 5));

   assert(any_of(1, 2, 3, 4, 5, 6)  == 5);
   assert(none_of(1, 2, 3, 4, 5, 6) != 5);
   assert(all_of(1, 2, 3, 4, 5, 6)  != 5);
   assert(all_of(5, 5, 5, 5, 5, 5)  == 5);

   cin.get();
}
