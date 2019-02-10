#include <map>
#include <utility>
#include <iterator>
#include <cassert>

template<
    typename Key,
    typename T,
    typename Compare,
    typename Allocator
>
auto update_key( std::map<Key,T,Compare,Allocator>& m 
                ,typename std::map<Key,T,Compare,Allocator>::node_type n
                ,const Key& new_key
               )
{
   if(n.empty())
      return std::make_pair(m.end(),false);

   Key new_key_copy{new_key};
   std::swap(new_key_copy,n.key());
   auto [position,inserted,node] = m.insert(std::move(n));
   if(!inserted) {
      assert(position!=m.end());
      std::swap(new_key_copy,node.key());
      auto const res = m.insert(std::move(node));
      assert(res.inserted);
   }
   return std::make_pair(position,inserted);
}

/**
   Changes the keys of requested map items without re-insertion and memory relocation.

   param [in,out] 'm' is a map-source container
   param [in]     'old_key' is an old key value
   param [in]     'new_key' is a new key value
   retval  a std::pair<iterator,bool> consisting of an iterator to the element with updated key(or m.end()) and a bool denoting whether the updating took place.
            - if the updating failes then the source 'm' remains unchangeable.
            - if an element does not exist, pair::first is end(), pair::second is false
            - if a key duplication is detected, pair::first is a position to element with 'key_new', pair::second is false
*/

template<
    typename Key,
    typename T,
    typename Compare,
    typename Allocator
>
auto update_key( std::map<Key,T,Compare,Allocator>& m 
                ,const Key& old_key
                ,const Key& new_key
               )
{
   return update_key(m,m.extract(old_key),new_key); 
}

template<
    typename Key,
    typename T,
    typename Compare,
    typename Allocator
>
auto update_key( std::map<Key,T,Compare,Allocator>& m 
                ,typename std::map<Key,T,Compare,Allocator>::const_iterator pos
                ,const Key& new_key
               )
{
   return update_key(m,m.extract(pos),new_key);
}

////// Example of Usage //////

#include <iostream>
#include <string>
#include <functional>
#include <cassert>

using namespace std;

int main()
{
   map<int,string,greater<int>> student_grades= {
       {10, "Mario"}
      ,{9,  "Luigi"}
      ,{11, "Bowser"}
      ,{5,  "Yoshi"}
      ,{7,  "Toad"}
   };

   for(auto&& s:student_grades)
      cout << s.second << endl;

   auto r = update_key(student_grades,5,8);
   assert(r.second);
   assert(r.first->first==8 && r.first->second=="Yoshi");
   assert(r.first==student_grades.find(8));

   cout << "--- After update ---" << endl;
   for(auto&& s:student_grades)
      cout << s.second << endl;

   r = update_key(student_grades,11,9);
   assert(!r.second);
   assert(
      (map<int,string,greater<int>>{{10,"Mario"},{9,"Luigi"},{11,"Bowser"},{8,"Yoshi"},{7,"Toad"}}) 
      == student_grades
   );
   assert(r.first->first==9 && r.first->second=="Luigi");
}

