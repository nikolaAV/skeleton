#if !defined(_STATIC_KEY_VALUE_STORAGE_H__)
#define _STATIC_KEY_VALUE_STORAGE_H__

#include <type_traits>

/**

*/

namespace ct_storage // prefix 'ct_' stands for 'compile time'
{

template <typename T, T t>
struct value_type {
   using type = T;
   static constexpr type value{t};
};

template <typename T, typename U, T t, U u>
struct couple {
   using left_value_type  = value_type<T,t>;
   using right_value_type = value_type<U, u>;
   using left_type  = typename left_value_type::type;
   using right_type = typename right_value_type::type;
};

template <auto key, auto value>
struct kv : couple<decltype(key), decltype(value), key, value> {};

template <typename... Couples>
struct couple_list;

template <typename Couple>
struct couple_list<Couple> {
   using left_type = typename Couple::left_type;
   using right_type = typename Couple::right_type;
};

template <typename Head, typename... Tail>
struct couple_list<Head,Tail...> : couple_list <Tail...> {
   using base_type = couple_list <Tail...>;
   static_assert(std::is_same_v<typename Head::left_type, typename base_type::left_type>);
   static_assert(std::is_same_v<typename Head::right_type, typename base_type::right_type>);
};

template <typename Couple>
struct comparator_left{
   static constexpr bool test(typename Couple::left_type other) noexcept {
      return other==Couple::left_value_type::value;
   }
   static constexpr typename Couple::right_type opposite() noexcept {
      return Couple::right_value_type::value;
   }
};

template <typename Couple>
struct comparator_right {
   static constexpr bool test(typename Couple::right_type other) noexcept {
      return other == Couple::right_value_type::value;
   }
   static constexpr typename Couple::left_type opposite() noexcept {
      return Couple::left_value_type::value;
   }
};

template <template <typename Couple> class Comparator, typename CoupleList>
struct find_if;

template <template <typename> class Comparator, typename Couple>
struct find_if<Comparator, couple_list<Couple>> {
   using left_type = typename Couple::left_type;
   using right_type = typename Couple::right_type;
   using comparator_type = Comparator<Couple>;

   static constexpr right_type test(left_type key, right_type def_val = {}) noexcept {
      return comparator_type::test(key)? comparator_type::opposite() : def_val;
   }
};

template <template <typename> class Comparator, typename Head, typename... Tail>
struct find_if<Comparator, couple_list<Head,Tail...>> {
   using left_type = typename Head::left_type;
   using right_type = typename Head::right_type;
   using comparator_type = Comparator<Head>;

   static constexpr right_type test(left_type key, right_type def_val = {}) noexcept {
      return comparator_type::test(key)? comparator_type::opposite() : find_if<Comparator, couple_list<Tail...>>::test(key, def_val);
   } 
};

template <typename List>
constexpr
typename List::right_type 
lookup(typename List::left_type key, typename List::right_type def_val = {}) noexcept {
   return find_if<comparator_left, List>::test(key,def_val);
}

template <typename List>
constexpr
typename List::right_type 
lookup(typename List::right_type key, typename List::left_type def_val = {}) noexcept {
   return find_if<comparator_right, List>::test(key, def_val);
}

// Example of usage

namespace unit_test {

inline void ut_common() {
   
   using map_type = couple_list<
        kv<'1', 1>
      , kv<'2', 2>
      , kv<'3', 3>
      , kv<'4', 4>
      , kv<'5', 5>
      , kv<'6', 6>
   >;
   
   static_assert(3   == lookup<map_type>('3'));
   static_assert('5' == lookup<map_type>(5));
   static_assert('X' == lookup<map_type>(666,'X'));

}

} // namespace unit_test


} // namespace ct_storage

#endif //_STATIC_KEY_VALUE_STORAGE_H__
 