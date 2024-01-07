#include <type_traits>

/**
   \brief expressive conditional statements (C++17 version)
   \see  https://github.com/nikolaAV/skeleton/tree/master/algorithm/expressive_condition
*/

namespace ecs { // <--- expressive conditional statements

namespace aux {

struct logical_or {};
struct logical_and {};

template <typename Op, auto... Ts>   // -std=c++17
struct variety {
   using logical_type = Op;

   template <typename U>
   static bool contains(U value) noexcept {
      if constexpr (std::is_same_v<logical_type, logical_or>)
         return ((Ts == value) || ...);
      else 
      if constexpr (std::is_same_v<logical_type, logical_and>)
         return ((Ts == value) && ...);
   }
};

template <auto... Ts>
using variety_or = variety<logical_or, Ts...>;
template <auto... Ts>
using variety_and = variety<logical_and, Ts...>;

template <typename T, typename Enabled = void> 
struct is_variety : std::false_type {}; 

template <typename T> 
struct is_variety<T, std::void_t<decltype(std::decay_t<T>::contains(typename std::decay_t<T>::logical_type{}))>> : std::true_type {}; 

template <typename T> 
using is_variety_t = typename is_variety<T>::type;

template <typename T> 
inline constexpr bool is_variety_v = is_variety<T>::value;

}  // namespace aux

template <auto... Ts> 
struct any_of : aux::variety_or<Ts...> {
    template <typename U>
    auto operator()(U value) const noexcept {
        return aux::variety_or<Ts...>::contains(value);
    } 
};

template <auto... Ts> 
struct all_of : aux::variety_and<Ts...> {
    template <typename U>
    auto operator()(U value) const noexcept {
        return aux::variety_and<Ts...>::contains(value);
    } 
};

template <auto... Ts> 
struct none_of : aux::variety_or<Ts...> {
    template <typename U>
    auto operator()(U value) const noexcept {
        return !aux::variety_or<Ts...>::contains(value);
    } 
};

template <typename U, typename V>
auto
operator==(V const& left, U right) noexcept -> std::enable_if_t<aux::is_variety_v<decltype(left)>, bool>
{
   return left(right);
}

template <typename U, typename V>
auto
operator!=(V const& left, U right) noexcept -> std::enable_if_t<aux::is_variety_v<decltype(left)>, bool>
{
   return !(left == right);
}

template <typename U, typename V>
auto
operator==(U left, V const& right) noexcept -> std::enable_if_t<aux::is_variety_v<decltype(right)>, bool>
{
   return right(left);
}

template <typename U, typename V>
auto
operator!=(U left, V const& right) noexcept -> std::enable_if_t<aux::is_variety_v<decltype(right)>, bool>
{
   return !(right == left);
}

}  // namespace ecs

#include <iostream>
#include <cassert>

int main()
{
   using namespace std;

   auto const v = int{10};
   auto const any_of = ecs::any_of<1, 2, 3, 4, 5, 6>{};
   auto const none_of = ecs::none_of<1, 2, 3, 4, 5, 6>{};
   auto const all_of = ecs::all_of<1, 2, 3, 4, 5, 6>{};

   cout << (any_of(v)) << "," << (any_of == v) << "," << (v == any_of) << endl;
   cout << (any_of(5)) << "," << (any_of == 5) << "," << (5 == any_of) << endl;
   cout << (none_of(v)) << "," << (none_of == v) << "," << (v == none_of) << endl;
   cout << (none_of(5)) << "," << (none_of == 5) << "," << (5 == none_of) << endl;

   cout << (all_of(v)) << endl;
   cout << (all_of(5)) << endl;
   cout << (ecs::all_of<5, 5, 5, 5, 5, 5>{}(5)) << endl;
   cout << (ecs::all_of<5, 5, 5, 5, 5, 5>::contains(5)) << endl;

   assert(v != any_of);
   assert(v == none_of);
   assert(v != all_of);

   assert(5 == any_of);
   assert(5 != none_of);
   assert(5 != all_of);
   assert((5 == ecs::all_of<5, 5, 5, 5, 5, 5>{}));

   assert(any_of  == 5);
   assert(none_of != 5);
   assert(all_of  != 5);
   assert((ecs::all_of<5, 5, 5, 5, 5, 5>{} == 5));

   cin.get();
}
