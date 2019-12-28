#include "typelist.h"


namespace unit_test
{
   using namespace tl;

   using integral_types = list<signed char, short, int, long, long long>;
   using empty_list = list<>;

   void tl_negation() {
      static_assert(negation_v<std::true_type> == false, "tl_negation 1");
      static_assert(negation_v<std::false_type> == std::true_type::value, "tl_negation 2");
   }

   void tl_islist() {
      static_assert(is_list_v<integral_types>, "tl_islist assert 1");
      static_assert(is_list_v<empty_list>, "tl_islist assert 2");
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
         push_back_t<empty_list, float
         >, int
         >;
      static_assert(std::is_same_v<another_list2, list<float, int>>, "tl_push_front assert 2");

      using l1 = list<int, short, char>;
      using l2 = list<const char*, void*>;
      using another_list3 = push_front_t<l1, l2>;
      static_assert(std::is_same_v<another_list3, list<list<const char*, void*>, int, short, char>>, "tl_push_front assert 3");

      using another_list4 = push_back_t<l1, l2>;
      static_assert(std::is_same_v<another_list4, list<int, short, char, list<const char*, void*>>>, "tl_push_front assert 4");
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
      static_assert(!any_of_v<empty_list, void*>, "tl_any_none_all assert empty");

      static_assert(!none_of_v<integral_types, short>, "tl_any_none_all assert 3");
      static_assert(none_of_v<integral_types, void*>, "tl_any_none_all assert 4");
      static_assert(none_of_v<empty_list, void*>, "tl_any_none_all assert empty");

      static_assert(!all_of_v<integral_types, int>, "tl_any_none_all assert 5");
      static_assert(!all_of_v<empty_list, void>, "tl_any_none_all assert empty");
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
   }

   void tl_unique() {
      using input = list<int, int, char, void, float, void*, void>;
      using output1 = unique_t<input>;
      using output2 = unique_reverse_t<input>;
      static_assert(std::is_same_v<output1, list<int, char, void, float, void*>>, "tl_unique assert 1");
      static_assert(std::is_same_v<output2, list<void*, float, void, char, int>>, "tl_unique assert 2");
   }

   void tl_has_nested_list() {
      static_assert(!has_nested_list_v<empty_list>, "tl_has_nested_list assert 1");
      static_assert(!has_nested_list_v<integral_types>, "tl_has_nested_list assert 2");
      static_assert(has_nested_list_v<list<integral_types>>, "tl_has_nested_list assert 3");
      static_assert(has_nested_list_v<list<int, bool, list<char, void>>>, "tl_has_nested_list assert 4");
   }

   void tl_push_back_termwise() {
      using l1 = push_back_termwise_t<list<int, int>, list<bool, bool>>;
      static_assert(std::is_same_v<l1, list<int, int, bool, bool>>, "tl_push_back_termwise assert 1");

      using l2 = push_back_termwise_t<list<int, int>, list<>>;
      static_assert(std::is_same_v<l2, list<int, int>>, "tl_push_back_termwise assert 2");

      using l3 = push_back_termwise_t<list<int, int>, bool>;
      static_assert(std::is_same_v<l3, list<int, int, bool>>, "tl_push_back_termwise assert 3");

      using l4 = push_back_termwise_t<list<>, list<void, list<bool, int>>>;
      static_assert(std::is_same_v<l4, list<void, bool, int>>, "tl_push_back_termwise assert 4");

      using l5 = push_back_termwise_t<list<char>, list<void, list<bool, int>, void>>;
      static_assert(std::is_same_v<l5, list<char, void, bool, int, void>>, "tl_push_back_termwise assert 5");

      using l6 = push_back_termwise_t<list<>, list<list<list<void*>>>>;
      static_assert(!has_nested_list_v<l6>, "tl_push_back_termwise assert 6");
      static_assert(std::is_same_v<l6, list<void*>>, "tl_push_back_termwise assert 7");
   }

   void tl_linear_list() {
      using l1 = linear_list_t<list<list<list<void>>>>;
      static_assert(std::is_same_v<l1, list<void>>, "tl_linear_list assert 1");

      using l2 = linear_list_t<list<empty_list, empty_list, empty_list, empty_list>>;
      static_assert(std::is_same_v<l2, list<>>, "tl_linear_list assert 2");
   }

   void tl_push_front_if() {
      using output1 = push_front_if_t<integral_types, std::is_integral, bool>;
      static_assert(std::is_same_v<output1, push_front_t<integral_types, bool>>, "tl_push_front_if 1");

      using output2 = push_front_if_t<integral_types, std::is_integral, float>;
      static_assert(!std::is_same_v<output2, push_front_t<integral_types, float>>, "tl_push_front_if 2");

      using output3 = push_back_if_t<list<int>, is_list, float>;
      static_assert(std::is_same_v<output3, list<int>>, "tl_push_front_if 3");

      using output4 = push_back_if_t<list<int>, is_list, list<float>>;
      static_assert(std::is_same_v<output4, list<int, list<float>>>, "tl_push_front_if 4");
   }

   void tl_find_if() {
      using output = find_if_t<integral_types, std::is_integral>;
      static_assert(std::is_same_v<output, integral_types>, "tl_find_if 1");
      static_assert(tl::size_v<integral_types> == tl::size_v<output>, "tl_find_if 2");

      using input2 = list<list<int, int>, void, list<short, short>, bool, char>;
      using output2 = find_if_t<input2, is_list>;
      static_assert(std::is_same_v<output2, list<list<int, int>, list<short, short>>>, "tl_find_if 3");
   }


   void tl_copy() {
      using l1 = list<int, char>;
      using l2 = list<void, bool>;
      static_assert(std::is_same_v<concatenate_t<l1, l2>, list<int, char, void, bool>>, "tl_copy assert 1");
      static_assert(std::is_same_v<copy_t<l1, l2>, list<int, char, void, bool>>, "tl_copy assert 2");
      static_assert(std::is_same_v<copy_t<l1, l2, l2, l1>, list<int, char, void, bool, void, bool, int, char>>, "tl_copy assert 3");
   }

   void tl_any_none_all_of_if() {
      using my_list = list<double*, float*, const char*, void*>;
      static_assert(any_of_if_v<my_list, std::is_integral> == false, "tl_any_none_all_of_if assert 1");
      static_assert(any_of_if_v<integral_types, std::is_integral> == true, "tl_any_none_all_of_if assert 2");
      static_assert(any_of_if_v<empty_list, std::is_integral> == false, "tl_any_none_all_of_if assert 3");
      static_assert(none_of_if_v<my_list, std::is_integral> == true, "tl_any_none_all_of_if assert 4");
      static_assert(none_of_if_v<integral_types, std::is_integral> == false, "tl_any_none_all_of_if assert 5");
      static_assert(none_of_if_v<empty_list, std::is_integral> == true, "tl_any_none_all_of_if assert 6");
      static_assert(all_of_if_v<integral_types, std::is_integral> == true, "tl_all_none_all_of_if assert 7");
      static_assert(all_of_if_v<empty_list, std::is_integral> == false, "tl_any_none_all_of_if assert 8");
      static_assert(all_of_if_v<my_list, std::is_pointer> == true, "tl_any_none_all_of_if assert 9");
   }


   void tl_any_none_all_of_from() {
      static_assert(any_of_from_v<integral_types, list<void*, const char*, long, void>>, "tl_any_none_all_of_from 1");
      static_assert(none_of_from_v<integral_types, list<void*, const char*, long, void>> == false, "tl_any_none_all_of_from 2");
      static_assert(any_of_from_v<empty_list, list<void*, const char*, long, void>> == false, "tl_any_none_all_of_from 3");
      static_assert(none_of_from_v<empty_list, list<void*, const char*, long, void>> == true, "tl_any_none_all_of_from 4");

      using list1 = list<double, float>;
      using list2 = list<char, double, void, float, int>;
      static_assert(all_of_from_v<list1, list2> == true, "tl_any_none_all_of_from 5");
   }

}   // end of unit_test namespace




using namespace tl;
using namespace rt;


// Example of usage

#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct visitor
{
   void operator()(int*, int a1, double a2, const char* a3 ) { 
      collector.push_back(string{"I'm `int`: "} + to_string(a1));
   }
   void operator()(double*, int a1, double a2, const char* a3) { 
      collector.push_back(string{ "I'm `double`: " } +to_string(a2));
   }
   void operator()(const char**, int a1, double a2, const char* a3) {
      collector.push_back(string{ "I'm `string`: " } + a3);
   }

   vector<string> collector;
};


int main()
{
   using composition = list<int,double,const char*>;
   auto out = for_each<composition>(visitor{}, 1, .1, "Hello, World!").collector;
   for(auto s:out)
      cout << s << endl;   
   cin.get();
}
