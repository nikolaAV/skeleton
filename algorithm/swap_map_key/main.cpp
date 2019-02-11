#include <map>
#include <utility>
#include <cassert>

/**
   'swap_value' swaps values of the specified elements by its keys in the requested map-container    
   'swap_key'   swaps keys of the specified elements in the requested map-container    

   \param[in,out] m - a target map-container
   \param[in] first, second - keys or iterators which points out map items whose contents to swap
   \retval 'true' if the swapping took place, 'false' otherwise
*/

template<
    typename Key,
    typename T,
    typename Compare,
    typename Allocator
>
bool swap_value( std::map<Key,T,Compare,Allocator>& m 
                ,typename std::map<Key,T,Compare,Allocator>::iterator first
                ,typename std::map<Key,T,Compare,Allocator>::iterator second
               )
{
   using std::swap;
   if(first!=m.end() && second!=m.end()) {
      swap(first->second,second->second);
      return true;
   }
   return false;
}

template<
    typename Key,
    typename T,
    typename Compare,
    typename Allocator
>
bool swap_value( std::map<Key,T,Compare,Allocator>& m 
                ,const Key& first
                ,const Key& second
               )
{
   return swap_value(m,m.find(first),m.find(second));
}

template<
    typename Key,
    typename T,
    typename Compare,
    typename Allocator
>
bool swap_key( std::map<Key,T,Compare,Allocator>& m 
                ,typename std::map<Key,T,Compare,Allocator>::node_type first
                ,typename std::map<Key,T,Compare,Allocator>::node_type second
               )
{
   if(first.empty() || second.empty())
      return false;

   using std::swap;
   swap(first.key(),second.key());
   auto const [f_pos,f_inserted,f_node] = m.insert(std::move(first));
   assert(f_pos!=m.end() && f_inserted);
   auto const [s_pos,s_inserted,s_node] = m.insert(std::move(second));
   assert(s_pos!=m.end() && s_inserted);
   return true;
}

template<
    typename Key,
    typename T,
    typename Compare,
    typename Allocator
>
bool swap_key( std::map<Key,T,Compare,Allocator>& m 
                ,const Key& first
                ,const Key& second
               )
{
   return swap_key(m,m.extract(first),m.extract(second));
}

template<
    typename Key,
    typename T,
    typename Compare,
    typename Allocator
>
bool swap_key( std::map<Key,T,Compare,Allocator>& m 
                ,typename std::map<Key,T,Compare,Allocator>::iterator first
                ,typename std::map<Key,T,Compare,Allocator>::iterator second
               )
{
   return swap_key(m,m.extract(first),m.extract(second));
}

///////// Example of usage

using namespace std;

int main()
{
   map<int,string> race_placement = {
       {1, "Mario"}
      ,{2, "Luigi"}
      ,{3, "Bowser"}
      ,{4, "Yoshi"}
      ,{5, "Toad"}
      ,{6, "Peach"}
      ,{7, "Koopa"}
      ,{8, "Donkey Kong Jr."}
   };
   assert(race_placement[3]=="Bowser");
   assert(race_placement[8]=="Donkey Kong Jr.");

   swap_value(race_placement,3,8);
   assert(race_placement[3]=="Donkey Kong Jr.");
   assert(race_placement[8]=="Bowser");

   swap_key(race_placement,3,8);
   assert(race_placement[3]=="Bowser");
   assert(race_placement[8]=="Donkey Kong Jr.");
}
