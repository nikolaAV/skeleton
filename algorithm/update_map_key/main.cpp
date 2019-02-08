#include <map>
#include <utility>
#include <iterator>

/**
   Changes the keys of requested map items without re-insertion and memory relocation.

   param [in,out] 'm' is a map-source container
   param [in]     'pos' iterator to the position in which the element key will be updated
   param [in]     'new_key' is a new key value
   retval  a pair consisting of an iterator to the element with updated key(or m.end()) and a bool denoting whether the updating took place. 
*/

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
   auto nh = m.extract(pos);
   if(!nh.empty())
      nh.key() = new_key;
   auto [position, inserted, node] = m.insert(std::move(nh));
   return std::make_pair(position, inserted);
}

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
   return update_key(m,m.find(old_key),new_key); 
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

   cout << "--- After update ---" << endl;
   for(auto&& s:student_grades)
      cout << s.second << endl;

   r = update_key(student_grades,11,9);
   assert(!r.second);
}

