#include "stdwarnings_suppress_on.h"
#include <type_traits>
#include <iterator>
#include <utility>
#include "stdwarnings_suppress_off.h"

/**

*/

template <typename T, T t>
struct value_type {
   using type = T;
   static constexpr type value{t};
};

template <typename T, typename U, T t, U u>
struct couple {
   using first_type = T;
   using second_type = U;
   static constexpr first_type first_value {t};
   static constexpr second_type second_value {u};
};

template <auto key, auto value>
struct kv : couple<decltype(key), decltype(value), key, value> {};

template <typename... Couples>
struct couple_list;

template <typename Couple>
struct couple_list<Couple> {
   using first_type = typename Couple::first_type;
   using second_type = typename Couple::second_type;
};

template <typename Head, typename... Tail>
struct couple_list<Head,Tail...> : couple_list <Tail...> {
   using base_type = couple_list <Tail...>;
   static_assert(std::is_same_v<typename Head::first_type, typename base_type::first_type>);
   static_assert(std::is_same_v<typename Head::second_type, typename base_type::second_type>);
};

template <typename Couple>
struct comparator_first{
   static constexpr bool test(typename Couple::first_type other) noexcept {
      return other==Couple::first_value;
   }
   static constexpr typename Couple::second_type opposite() noexcept {
      return Couple::second_value;
   }
};

template <typename Couple>
struct comparator_second {
   static constexpr bool test(typename Couple::second_type other) noexcept {
      return other == Couple::first_value;
   }
   static constexpr typename Couple::first_type opposite() noexcept {
      return Couple::first_value;
   }
};

template <template <typename Couple> class Comparator, typename CoupleList>
struct find_if;

template <template <typename> class Comparator, typename Couple>
struct find_if<Comparator, couple_list<Couple>> {
   using first_type = typename Couple::first_type;
   using second_type = typename Couple::second_type;
   using comparator_type = Comparator<Couple>;

   static constexpr second_type test(first_type key, second_type def_val = {}) noexcept {
      return comparator_type::test(key)? comparator_type::opposite() : def_val;
   }
};

template <template <typename> class Comparator, typename Head, typename... Tail>
struct find_if<Comparator, couple_list<Head,Tail...>> {
   using first_type = typename Head::first_type;
   using second_type = typename Head::second_type;
   using comparator_type = Comparator<Head>;

   static constexpr second_type test(first_type key, second_type def_val = {}) noexcept {
      return comparator_type::test(key)? comparator_type::opposite() : find_if<Comparator, couple_list<Tail...>>::test(key, def_val);
   } 
};

template <typename List>
typename List::second_type match(typename List::first_type key, typename List::second_type def_val = {}) noexcept {
   return find_if<comparator_first, List>::test(key,def_val);
}

template <typename List>
typename List::second_type match(typename List::second_type key, typename List::first_type def_val = {}) noexcept {
   return find_if<comparator_second, List>::test(key, def_val);
}


// Example of usage

#include "stdwarnings_suppress_on.h"
#include <string>
#include <initializer_list>
#include <iostream>
#include "stdwarnings_suppress_off.h"

using namespace std;

using cmap = couple_list<
    kv<'1',1>
   ,kv<'2',2>
   ,kv<'3',3>
   ,kv<'4',4>
   ,kv<'5',5>
   ,kv<'6',6>
>;

int main()
{
//   cout << find_if<cmap>::test('1') << endl;
   cout << match<cmap>('3') << endl;
   cout << match<cmap>('7') << endl;
   cout << match<cmap>('8',8) << endl;
   cin.get();
}
