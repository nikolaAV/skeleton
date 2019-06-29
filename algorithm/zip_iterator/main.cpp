#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <initializer_list>
#include <forward_list>
#include <set>
#include <deque>
#include <list>
#include <numeric>

#include "zip_iterator.h"

using namespace std;

int main()
{
   {
      vector v1 {1,2,3,4};
      vector v2 {.1,.2,.3};

      const zip::tuple p{ v1,v2 };

      auto aaa = zip::begin(p);
      auto bbb = std::move(aaa);
      zip::iterator u{};
      u==u;
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
         auto const[first,second,third] = element;
         cout << first << "," << second << "," << third << endl;
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

   {
      const vector c1   = { 1,2,3,4 };
      const deque  c2   = { .1,.2,.3 };
      const list   c3   = { 1,1,1 };
      const char   c4[] = {"hello,world!"};

      for(auto&& [_1,_2,_3,_4]:zip::tuple{c1,c2,c3,c4}) {
         cout << _1*_2*_3*_4 << endl;
      }
   }
   {
      double a[] {1.,2.,3.};
      double b[]{ 4.,5.,6. };
      auto sum = accumulate(zip::begin(a,b), zip::end(a,b),0.,[](auto s, auto v){
         return s + get<0>(v)*get<1>(v);
      });
      cout << sum << endl;
   }


   cin.get();
}
