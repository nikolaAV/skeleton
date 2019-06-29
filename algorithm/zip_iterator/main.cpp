#include "stdwarnings_suppress_on.h"
#include <iterator>
#include <tuple>
#include <utility>
#include "stdwarnings_suppress_off.h"

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

   template <size_t... Idx>
   void increment(std::index_sequence<Idx...>) {
      (std::get<Idx>(ts_)++, ...);
   }
   template <size_t... Idx>
   auto values(std::index_sequence<Idx...>) const {
      return std::tuple<decltype(*std::get<Idx>(ts_))...>{ *std::get<Idx>(ts_)... };
   }

public:
   static constexpr auto indices = std::make_index_sequence<sizeof...(Ts)>();

   explicit iterator(Ts... ts) noexcept : ts_(ts...) {}
   iterator& operator++() {
      increment(indices);
      return *this;
   }
   auto operator*() const {
      return values(indices);
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
}  // end of namespace zip


// Example of usage

#include "stdwarnings_suppress_on.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <initializer_list>
#include <forward_list>
#include <set>
#include "stdwarnings_suppress_off.h"

using namespace std;


int main()
{
   {
      vector v1 {1,2,3,4};
      vector v2 {.1,.2,.3};

      const zip::tuple p{ v1,v2 };

      zip::begin(p);
      zip::end(v1, v2);
      zip::end(p);


      for (auto i = begin(p); i != end(p); ++i) {
         cout << get<0>(*i) << " "<< get<1>(*i) << endl;
      }

      for_each(zip::begin(p), zip::end(p),[](const auto& pair){
         cout << get<0>(pair) << " " << get<1>(pair) << endl;
      });
   }

   {
      const auto l1 = initializer_list{ 'a','b','c','d','e','f' };
      const auto l2 = forward_list    { 'A','B','C','D','E','F' };
      const auto l3 = set             {  0,1,2,3,4,5 };

      for_each(zip::begin(l1,l2,l3), zip::end(l1,l2,l3), [](const auto& element) {
         cout << get<0>(element) << "," << get<1>(element) << "," << get<2>(element) << endl;
      });
   }

   {
      vector v1{ 0,1,2,3,4 };
      vector v2{ 9,8,7,6,5 };

      int* p_one = &v1[1]; 
      cout << v1[1] << " <-> " << *p_one << endl;

//      swap(v1,v2);
//      cout << v1[1] << " <-> " << *p_one << endl;

      for_each(zip::begin(v1, v2), zip::end(v1, v2), [](const auto& element) {
         std::swap(get<0>(element), get<1>(element));
      });

      cout << v1[1] << " <-> " << *p_one << endl;
   }

   cin.get();
}
