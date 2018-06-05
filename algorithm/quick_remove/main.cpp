#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>


namespace quick
{

template<typename T>
void remove_at(std::vector<T>& vec, std::size_t idx)
{
   assert(idx<vec.size());

   vec[idx]=std::move(vec.back());
   vec.pop_back();
}

template<typename T>
void remove_at(std::vector<T>& vec, typename std::vector<T>::iterator it)
{
   assert(0<=std::distance(vec.begin(),it));
   assert(0< std::distance(it,vec.end()));

   *it=std::move(vec.back());
   vec.pop_back();
}

namespace details_
{
   template<typename T, typename UnaryPredicate>
   void trim_back(std::vector<T>& vec, UnaryPredicate p)
   {
      auto last = std::find_if_not(std::rbegin(vec),std::rend(vec),p);
      vec.erase(last.base(),vec.end());
   }

   template<typename T, typename UnaryPredicate>
   void remove_if(std::vector<T>& vec, UnaryPredicate p, size_t idx)
   {
      trim_back(vec,p);
      for(size_t i=idx; i < vec.size(); ++i)
         if(p(vec[i])) {
            quick::remove_at(vec,i);
            details_::remove_if(vec,p,++i);
            return;
         }
   }

}  // details

template<typename T, typename UnaryPredicate>
void remove_if(std::vector<T>& vec, UnaryPredicate p)
{
   details_::remove_if(vec,p,0);
}

template<typename T>
void remove(std::vector<T>& vec, const T& value)
{
   quick::remove_if(vec,[&value](auto const& v){return v==value;});
}


}  // end of namespace quick

using std::vector;
using std::cin;
using namespace quick;

void test1()
{
   vector<int> v {};
   remove(v,3);
   assert(0==v.size());
   assert((vector<int>{})==v);
}

void test2()
{
   vector<int> v {3,3,3,3,3,3,3,3,3,3};
   remove(v,3);
   assert(0==v.size());
   assert((vector<int>{})==v);
}

void test3()
{
   vector<int> v {1,2,3,3,3,3,3,3};
   remove(v,3);
   assert(2==v.size());
   assert((vector<int>{1,2})==v);
}

void test4()
{
   vector<int> v {1,2,3};
   remove(v,3);
   assert(2==v.size());
   assert((vector<int>{1,2})==v);
}

void test5()
{
   vector<int> v {1,2,4,5,6};
   remove(v,3);
   assert(5==v.size());
   assert((vector<int>{1,2,4,5,6})==v);
}

void test6()
{
   vector<int> v {1,2,3};
   remove(v,1);
   assert(2==v.size());
   assert((vector<int>{3,2})==v);
}

void test11()
{
   vector<int> v {1,2,3,4,5};
   remove(v,3);
   assert(4==v.size());
   assert((vector<int>{1,2,5,4})==v);
}

void test12()
{
   vector<int> v {1,1,1,1,1};
   remove(v,1);
   assert(0==v.size());
}

void test13()
{
   vector<int> v {1,8,2,8,3,8,4,8,5,8};
   remove(v,8);
   assert(5==v.size());
   assert((vector<int>{1,5,2,4,3})==v);
}

void test14()
{
   vector<int> v {1,8,2,8,3,8,4,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,5,8};
   remove(v,8);
   assert(5==v.size());
   assert((vector<int>{1,5,2,4,3})==v);
}

void test15()
{
   vector<int> v {1,2,3,4,5,6,7,8,9,0};
   remove_if(v, [](int val) {return val%2;} );
   assert(5==v.size());
   assert((vector<int>{0,2,8,4,6})==v);
}

int main()
{
   test1();
   test2();
   test3();
   test4();
   test5();
   test6();
   test11();
   test12();
   test13();
   test14();
   test15();
}
