#include <type_traits>
#include <iterator>
#include <cassert>
#include <algorithm>
#include <utility>

namespace concepts
{
   template< typename T, typename = std::void_t<>>
   struct is_sequence_container : std::false_type { };

   template< typename T >
   struct is_sequence_container<T, std::void_t
                                 < decltype(std::begin(std::declval<T>()))
                                  ,decltype(std::end(std::declval<T>()))
                                  ,decltype(std::declval<T>().insert(std::declval<typename T::const_iterator>(),std::declval<typename T::value_type>()))
                                 > 
                            > : std::true_type { };

   template< typename T >
   inline constexpr bool is_sequence_container_v = is_sequence_container<T>::value;

}  // end of namespace concepts


/**
   \brief Inserts elements at a location to preserve the order in the container.
   \note  the container must be sorted before. Otherwise, this would not work.
   \param [in,out] seq - a sequence container (vector,list,deque,string) in which elements are sorted 
   \param [in]     v   - element value to insert
   \retval  an iterator pointing to the inserted value
   \complexity  logarithmic
   \exception bad_alloc may be caused by memory reallocation if the existing container capacity is not enough the new element to be inserted
*/

template <typename Sequence>
auto insert_sorted(Sequence& seq, typename Sequence::value_type&& v)
{
   static_assert(concepts::is_sequence_container_v<Sequence>);
   assert(std::is_sorted(std::begin(seq),std::end(seq)));
   const auto pos = std::lower_bound(std::begin(seq),std::end(seq),v);
   return seq.insert(pos,std::forward<typename Sequence::value_type>(v));
}

///////// Example of usage

#include <vector>
#include <list>
#include <array>
#include <deque>
#include <forward_list>
#include <set>
#include <string>

using namespace std;

int main()
{
   static_assert(concepts::is_sequence_container_v<vector<int>>);
   static_assert(concepts::is_sequence_container_v<list<double>>);
   static_assert(!concepts::is_sequence_container_v<array<char,1>>);
   static_assert(concepts::is_sequence_container_v<deque<unsigned>>);
   static_assert(!concepts::is_sequence_container_v<forward_list<long>>);
   static_assert(concepts::is_sequence_container_v<set<const char*>>);
   static_assert(concepts::is_sequence_container_v<wstring>);

   vector<string> vec {"some","random","words","without","order","aaa","zzz"};
   sort(begin(vec),end(vec));
   insert_sorted(vec,"bbb");
   assert(vec==(vector<string>{"aaa","bbb","order","random","some","without","words","zzz"}));

   list<size_t> lst;
   for(auto i:{3,6,0,4,1,5,8,2,9,7})
      insert_sorted(lst,i);
   lst.pop_front();
   assert(lst==(list<size_t>{1,2,3,4,5,6,7,8,9}));

   set<string> st{"some","random","words","without","order","aaa","zzz"};
   insert_sorted(st,"bbb");
   assert(st==(set<string>{"aaa","bbb","order","random","some","without","words","zzz"}));
}
