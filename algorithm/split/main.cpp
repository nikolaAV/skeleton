#include <algorithm>
#include <iterator>

template< typename InputIt, typename OutputIt, typename UnaryPredicate, typename BinaryOperation >
void split_if( InputIt first1, InputIt last1, OutputIt d_first, UnaryPredicate unary_op, BinaryOperation binary_op )
{
   while(first1!=last1){
      auto last_slice = std::find_if(first1,last1,unary_op);
      if(first1!=last_slice)
         *d_first++ = binary_op(first1,last_slice);
      if(last_slice==last1) return;
      first1 = std::next(last_slice);
   }
}

template< typename InputIt, typename OutputIt, typename T, typename BinaryOperation >
void split( InputIt first1, InputIt last1, OutputIt d_first, const T& value, BinaryOperation binary_op )
{
   split_if(
       first1
      ,last1
      ,d_first
      ,[&value](const auto v){ return value==v;}
      ,binary_op
   );
}

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <numeric>
#include <cassert>

using namespace std;

void test1()
{
   const vector<int> v {0,1,0,1,2,0,1,2,3,0,1,2,3,4,0,1,2,3,4,5};   
   //                     1     3       6        10          15
   list<int>   sums;
   split(begin(v),end(v),back_inserter(sums),0,[](auto slice_begin, auto slice_end){
      return accumulate(slice_begin,slice_end,0);      
   });
   assert(sums==(list<int>{1,3,6,10,15}));
}

void test2()
{
   const string qoute = "Don't comment bad code - rewrite it.\n --Brian Kernighan";
   const vector<string> expected = {"Don't","comment","bad","code","rewrite","it","Brian","Kernighan"};

   vector<string> words;
   split_if(begin(qoute),end(qoute),back_inserter(words)
      ,[](char ch) {
            return ch==' ' || ch=='-' || ch=='.'  || ch=='\n';
         }
      ,[](auto slice_begin, auto slice_end){
            return string(slice_begin,slice_end);      
         }
   );
   assert(words==expected);
}

int main()
{
   test1();
   test2();
   cin.get();
}
