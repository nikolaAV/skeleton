#include <algorithm>
#include <iterator>

template<typename Range, typename UnaryPredicate>
auto trim_left_if(Range& range, UnaryPredicate pred)
{
   const auto first  = std::begin(range);
   const auto last   = std::end(range);
   const auto found  = std::find_if_not(first,last,pred);
   const auto dist   = std::distance(first,found);
   if(dist)
      range.erase(first,found);
   return dist;
}

template<typename Range, typename T>
auto trim_left(Range& range, const T& value)
{
   return trim_left_if(range,[&value](const auto& v) {
      return value==v;
   });
}

template<typename Range, typename UnaryPredicate>
auto trim_right_if(Range& range, UnaryPredicate pred)
{
   const auto first  = std::rbegin(range);
   const auto last   = std::rend(range);
   const auto found  = std::find_if_not(first,last,pred);
   const auto dist   = std::distance(found.base(),std::end(range));
   if(first!=found)
      range.erase(found.base(),std::end(range));
   return dist;
}

template<typename Range, typename T>
auto trim_right(Range& range, const T& value)
{
   return trim_right_if(range,[&value](const auto& v) {
      return value==v;
   });
}

template<typename Range, typename UnaryPredicate>
auto trim_if(Range& range, UnaryPredicate pred)
{
   return trim_left_if(range,pred) + trim_right_if(range,pred);
}

template<typename Range, typename T>
auto trim(Range& range, const T& value)
{
   return trim_if(range,[&value](const auto& v) {
      return value==v;
   });
}

#include <iostream>
#include <list>
#include <string>
#include <cassert>
#include <cctype>

using namespace std;

int main()
{
   wstring blank_padded_edit_box = L"  Hello, World!   ";
   const auto discarded = trim(blank_padded_edit_box,' ');
   assert(blank_padded_edit_box==L"Hello, World!");
   assert(discarded==5);

   string pin = "abce{2301X*&%-";   // https://en.wikipedia.org/wiki/Personal_identification_number
   const auto n = trim_if(pin,[](const auto& ch) {
      return !isdigit(ch);          // <-- decimal digits are only expected
   });
   assert(pin=="2301" && n==10);

   list<int> bounded_sequence = { 1,3,6,-9,6,-23, 0,1,2,3,4,5,6,7,8,9,0, -100,99,7,4,2,56 }; 
   trim_if(bounded_sequence,[](int v){
      return v!=0;
   });
   assert(bounded_sequence==(list<int>{0,1,2,3,4,5,6,7,8,9,0}));
}
