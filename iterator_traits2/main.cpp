#include <type_traits>
#include <iterator>

namespace iterators
{
   using namespace std;

   template <typename T>
   using category = typename iterator_traits<T>::iterator_category;

   template <typename ItCat, typename ItTag>
   using equal = is_same<ItCat,ItTag>;

   template <typename, typename>
   using always_true = true_type;

   template< typename T, typename ItTag, template <typename,typename> typename Eq = equal ,typename = void_t<>>
   struct is_category : false_type {};

   template< typename T, typename ItTag, template <typename,typename> typename Eq>
   struct is_category<T,ItTag,Eq,void_t<category<T>>> 
                      : conditional_t<
                            Eq<category<T>,ItTag>::value
                           ,true_type
                           ,false_type> {};

   template< typename T>
   using is_iterator = is_category<T,T,always_true>; 
   template< typename T> inline constexpr bool   is_iterator_v = is_iterator<T>::value;

   #define DEFINE_TRAITS_FOR_(name)   \
      template< typename T>   \
      using is_##name = is_category<T,name##_tag>; \
      template< typename T> inline constexpr bool   is_##name##_v = is_##name<T>::value

   DEFINE_TRAITS_FOR_(random_access_iterator);
   DEFINE_TRAITS_FOR_(input_iterator);
   DEFINE_TRAITS_FOR_(output_iterator);
   DEFINE_TRAITS_FOR_(forward_iterator);
   DEFINE_TRAITS_FOR_(bidirectional_iterator);

   #undef DEFINE_TRAITS_FOR_

} // end namespace iterators 

#include <utility>
#include <vector>
#include <set>
#include <list>
#include <forward_list>
#include <deque>
#include <iostream>

int main()
{
   using namespace iterators;

   {  vector<int> v = {1,2,3,4,5};
      auto it = v.begin();

      using container_type = decltype(v);
      using iterator_type =  decltype(it);
      static_assert(!is_iterator_v<container_type>);
      static_assert(is_iterator_v<iterator_type>);
      static_assert(is_random_access_iterator_v<iterator_type>);
   }

   {  deque<int> v = {1,2,3,4,5};
      auto it = v.begin();

      using container_type = decltype(v);
      using iterator_type =  decltype(it);
      static_assert(!is_iterator_v<container_type>);
      static_assert(is_iterator_v<iterator_type>);
      static_assert(is_random_access_iterator_v<iterator_type>);
   }

   {  list<int> v = {1,2,3,4,5};
      auto it = v.begin();

      using container_type = decltype(v);
      using iterator_type =  decltype(it);
      static_assert(!is_iterator_v<container_type>);
      static_assert(is_iterator_v<iterator_type>);
      static_assert(is_bidirectional_iterator_v<iterator_type>);
   }

   {  forward_list<int> v = {1,2,3,4,5};
      auto it = v.begin();

      using container_type = decltype(v);
      using iterator_type =  decltype(it);
      static_assert(!is_iterator_v<container_type>);
      static_assert(is_iterator_v<iterator_type>);
      static_assert(is_forward_iterator_v<iterator_type>);
   }

   {  set<int> v = {1,2,3,4,5};
      auto it = v.begin();

      using container_type = decltype(v);
      using iterator_type =  decltype(it);
      static_assert(!is_iterator_v<container_type>);
      static_assert(is_iterator_v<iterator_type>);
      static_assert(is_bidirectional_iterator_v<iterator_type>);
   }
}
