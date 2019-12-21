#include "typelist.h"


namespace unit_test
{
   using namespace tl;

   using integral_types = list<signed char, short, int, long, long long>;
   using empty_type = list<>;

   void tl_islist() {
      static_assert(is_list_v<integral_types>, "tl_islist assert 1");
      static_assert(is_list_v<empty_type>, "tl_islist assert 2");
      static_assert(!is_list_v<int>, "tl_islist assert 3");
   }

   void tl_front_back() {
      using first = front_t<integral_types>;
      static_assert(std::is_same_v<first, signed char>, "tl_front_back assert 1");
      static_assert(size_v<integral_types> == 5, "tl_front_back assert 2");

      using last = back_t<integral_types>;
      static_assert(std::is_same_v<last, long long>, "tl_front_back assert 3");
   }

   void tl_pop_front() {
      using another_list = pop_front_t<pop_front_t<integral_types>>;
      using first = front_t<another_list>;
      static_assert(std::is_same_v<first, int>, "tl_pop_front assert 1");
   }


   void tl_push_front() {
      using another_list = push_front_t<integral_types, float>;
      using first = front_t<another_list>;
      static_assert(std::is_same_v<first, float>, "tl_push_front assert 1");

      using another_list2 =
         push_back_t<
         push_back_t<empty_type, float
         >, int
         >;
      static_assert(std::is_same_v<another_list2, list<float, int>>, "tl_push_front assert 2");

      using l1 = list<int, short, char>;
      using l2 = list<const char*, void*>;
      using another_list3 = push_front_t<l1, l2>;
      static_assert(std::is_same_v<another_list3, list<const char*, void*, int, short, char>>, "tl_push_front assert 3");

      using another_list4 = push_back_t<l1, l2>;
      static_assert(std::is_same_v<another_list4, list<int, short, char, const char*, void*>>, "tl_push_front assert 4");

   }

   void tl_push_unique() {
      using another_list1 = push_front_unique_t<integral_types, long>;
      static_assert(std::is_same_v<another_list1, integral_types>, "tl_push_unique 1");
      using another_list2 = push_back_unique_t<integral_types, short>;
      static_assert(std::is_same_v<another_list2, integral_types>, "tl_push_unique 2");
   }

   void tl_replace_front() {
      using another_list = replace_front_t<integral_types, double>;
      using first = front_t<another_list>;
      static_assert(std::is_same_v<first, double>, "replace_front assert 1");
   }

   void tl_nth_element() {
      using second = nth_element_t<integral_types, 1>;
      static_assert(std::is_same_v<second, short>, "tl_nth_element assert 1");
      using first = nth_element_t<integral_types, 0>;
      static_assert(std::is_same_v<first, signed char>, "tl_nth_element assert 2");
      using last = nth_element_t<integral_types, 4>;
      static_assert(std::is_same_v<last, long long>, "tl_nth_element assert 3");
   }

   void tl_empty_size() {
      static_assert(!is_empty_v<integral_types>, "tl_empty_size assert 1");
      static_assert(size_v<integral_types> == 5, "tl_empty_size assert 2");
      using another_list =
         pop_front_t<
         pop_front_t<
         pop_front_t<
         pop_front_t<
         pop_front_t<integral_types>
         >
         >
         >
         >;
      static_assert(is_empty_v<another_list>, "tl_empty_size assert 3");
      static_assert(size_v<another_list> == 0, "tl_empty_size assert 4");

      using another_list2 = push_back_t<push_back_t<another_list, int>, void*>;
      using last = nth_element_t<another_list2, 1>;
      static_assert(std::is_same_v<last, void*>, "tl_empty_size assert 5");
   }

   void tl_reverse() {
      using another_list = reverse_t<integral_types>;
      using first = nth_element_t<another_list, 0>;
      static_assert(std::is_same_v<first, long long>, "tl_reverse assert 1");
      using last = nth_element_t<another_list, 4>;
      static_assert(std::is_same_v<last, signed char>, "tl_reverse assert 2");
   }

   void tl_any_none_all() {
      static_assert(any_of_v<integral_types, short>, "tl_any_none_all assert 1");
      static_assert(!any_of_v<integral_types, void*>, "tl_any_none_all assert 2");
      static_assert(!any_of_v<empty_type, void*>, "tl_any_none_all assert empty");

      static_assert(!none_of_v<integral_types, short>, "tl_any_none_all assert 3");
      static_assert(none_of_v<integral_types, void*>, "tl_any_none_all assert 4");
      static_assert(none_of_v<empty_type, void*>, "tl_any_none_all assert empty");

      static_assert(!all_of_v<integral_types, int>, "tl_any_none_all assert 5");
      //        static_assert(!all_of_v<empty_type,void>,"tl_any_none_all assert empty"); <-- Fails, fix it
      using many_ints = list<int, int, int, int, int, int, int, int, int, int>;
      static_assert(all_of_v<many_ints, int>, "tl_any_none_all assert 6");
      static_assert(is_same_v<many_ints>, "tl_any_none_all assert 7");
   }

   void tl_is_unique() {
      static_assert(is_unique_v<integral_types>, "tl_unique assert 1");
      using another_list = push_back_t<integral_types, long>;
      static_assert(!is_unique_v<another_list>, "tl_unique assert 2");
   }

   template <typename T>
   struct make_pointer {
      using type = T * ;
   };

   void tl_transform() {
      using other_list = transform_t<integral_types, make_pointer>;
      static_assert(std::is_same_v<other_list, list<signed char*, short*, int*, long*, long long*>>, "tl_transform assert 1");
   }

   template <typename T1, typename T2>
   struct larger_by_size {
      using type = std::conditional_t<sizeof(T1) >= sizeof(T2), T1, T2>;
   };

   template <typename T1, typename T2>
   struct int_filter {
      using type = std::conditional_t<std::is_integral_v<T2>, push_back_t<T1, T2>, T1>;
   };

   void tl_accumulate() {
      using largest = accumulate_t<
         pop_front_t<integral_types>
         , larger_by_size
         , front_t<integral_types>
      >;
      static_assert(std::is_same_v<largest, long long>, "tl_accumulate assert 1");

      // With `accumulate` in hand, we can reverse a typelist using
      using other1 = accumulate_t<integral_types, push_front, list<>>;
      using other2 = reverse_t<integral_types>;
      static_assert(std::is_same_v<other1, other2>, "tl_accumulate assert 3");

      // and select only integral types from input list of arbitrary types
      using input = list<unsigned, double, long, float, void, char*, short>;
      using output = accumulate_t<input, int_filter, list<>>;
      static_assert(std::is_same_v<output, list<unsigned, long, short>>, "tl_accumulate assert 4");
   }

   void tl_accumulate_nested() {
      using l1 = list<int, char>;
      using l2 = list<double, float>;
      using l1l2 = accumulate_t<l2, push_back, l1>;
      static_assert(std::is_same_v<l1l2, list<int, char, double, float>>, "tl_accumulate_nested assert 1");

      using l2_nested = list<l2>;
      using l1l2_nested = accumulate_t<l2_nested, push_back, l1>;
      static_assert(std::is_same_v<l1l2, l1l2_nested>, "tl_accumulate_nested assert 1");
   }

   void tl_unique() {
      using input = list<int, int, char, void, float, void*, void>;
      using output1 = unique_t<input>;
      using output2 = unique_reverse_t<input>;
      static_assert(std::is_same_v<output1, list<int, char, void, float, void*>>, "tl_unique assert 1");
      static_assert(std::is_same_v<output2, list<void*, float, void, char, int>>, "tl_unique assert 2");
   }

   void tl_has_nested_list() {
      static_assert(!has_nested_list_v<empty_type>, "tl_has_nested_list assert 1");
      static_assert(!has_nested_list_v<integral_types>, "tl_has_nested_list assert 2");
      static_assert(has_nested_list_v<list<integral_types>>, "tl_has_nested_list assert 3");
      static_assert(has_nested_list_v<list<int,bool,list<char,void>>>, "tl_has_nested_list assert 4");
   }


} // end of unit_test


// make linear

using namespace tl;


template<typename TList1, typename T> 
struct copy : push_back<TList1,T> {
};

template <typename TList1, typename... Ts>
struct copy<TList1, list<Ts...>> : accumulate<list<Ts...>, push_back, TList1> {
};

template <typename TList1>
struct copy<TList1, list<>> {
   using type = TList1;
};

template<typename TList1, typename TList2>
using copy_t = typename copy<TList1, TList2>::type;


template<typename TList>
struct nlr_traverse : copy<list<>,TList> {
};

template<typename TList>
using  nlr_traverse_t = typename  nlr_traverse<TList>::type;

///


int main()
{

   static_assert(!has_nested_list_v<list<int,bool,void>>);
   static_assert(has_nested_list_v<list<int,bool,list<>,void>>);

{
   using l1 = copy_t<list<int,int>,list<bool,bool>>;
   static_assert(std::is_same_v<l1,list<int,int,bool,bool>>);

   using l2 = copy_t<list<int, int>, list<>>;
   static_assert(std::is_same_v<l2, list<int, int>>);

   using l3 = copy_t<list<int, int>, bool>;
   static_assert(std::is_same_v<l3, list<int, int, bool>>);

   using l4 = copy_t<list<>, list<void,list<bool,int>>>;
   static_assert(std::is_same_v<l4, list<void,bool,int>>);

   using l5 = copy_t<list<char>, list<void, list<bool, int>, void>>;
   static_assert(std::is_same_v<l5, list<char,void, bool, int, void>>);
}
}
