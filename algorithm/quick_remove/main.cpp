#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <utility>

/**
   Removes all elements satisfying specific criteria from the std::vector.   
   Removing is done without shifting the elements in the vector.
   After removing the relative order of the elements is not preserved.
*/

template<typename T>
void unstable_remove_at(std::vector<T>& vec, std::size_t idx) {
   assert(idx<vec.size());
   vec[idx]=std::move(vec.back());
   vec.pop_back();
}

template<typename T>
void unstable_remove_at(std::vector<T>& vec, typename std::vector<T>::iterator it) {
   assert(0<=std::distance(vec.begin(),it));
   assert(0< std::distance(it,vec.end()));
   *it=std::move(vec.back());
   vec.pop_back();
}

template<typename T, typename UnaryPredicate>
auto unstable_remove_if(std::vector<T>& vec, UnaryPredicate p)
{
   auto first = vec.begin();
   auto last  = vec.end();

   for (;;++first) {
	   // Find the first instance of "p"...
	   for (;;++first)
		   if (first == last)
			   return first;
		   else if (p(*first))
			   break;
	   // ...and the last instance of "not p"...
	   for (--last;;--last)
		   if (first == last)
			   return first;
		   else if (!p(*last))
			   break;
	   // ...and move the latter over top of the former.
	   *first = std::move(*last);
   }
}

template<typename T, typename UnaryPredicate>
auto unstable_erase_if(std::vector<T>& vec, UnaryPredicate p) {
   return vec.erase(unstable_remove_if(vec,p),vec.end());
}

template<typename T>
auto unstable_remove(std::vector<T>& vec, const T& value) {
   return unstable_remove_if(vec,[&value](auto const& v){return v==value;});
}

template<typename T>
auto unstable_erase(std::vector<T>& vec, const T& value) {
   return unstable_erase_if(vec,[&value](auto const& v){return v==value;});
}

///////// Example of Usage

using std::vector;

void test1()
{
   vector<int> v {};
   unstable_remove(v,3);
   assert(v.empty());
   assert((vector<int>{})==v);
}

void test2()
{
   vector<int> v {3,3,3,3,3,3,3,3,3,3};
   unstable_erase(v,3);
   assert(v.empty());
   assert((vector<int>{})==v);
}

void test3()
{
   vector<int> v {1,2,3,3,3,3,3,3};
   unstable_erase(v,3);
   assert(2==v.size());
   assert((vector<int>{1,2})==v);
}

void test4()
{
   vector<int> v {1,2,3};
   unstable_erase(v,3);
   assert(2==v.size());
   assert((vector<int>{1,2})==v);
}

void test5()
{
   vector<int> v {1,2,4,5,6};
   unstable_erase(v,3);
   assert(5==v.size());
   assert((vector<int>{1,2,4,5,6})==v);
}

void test6()
{
   vector<int> v {1,2,3};
   unstable_erase(v,1);
   assert(2==v.size());
   assert((vector<int>{3,2})==v);
}

void test11()
{
   vector<int> v {1,2,3,4,5};
   unstable_erase(v,3);
   assert(4==v.size());
   assert((vector<int>{1,2,5,4})==v);
}

void test12()
{
   vector<int> v {1,1,1,1,1};
   unstable_erase(v,1);
   assert(v.empty());
}

void test13()
{
   vector<int> v {1,8,2,8,3,8,4,8,5,8};
   unstable_erase(v,8);
   assert(5==v.size());
   assert((vector<int>{1,5,2,4,3})==v);
}

void test14()
{
   vector<int> v {1,8,2,8,3,8,4,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,5,8};
   unstable_erase(v,8);
   assert(5==v.size());
   assert((vector<int>{1,5,2,4,3})==v);
}

void test15()
{
   vector<int> v {1,2,3,4,5,6,7,8,9,0};
   unstable_erase_if(v, [](int val) {return val%2;} );
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
