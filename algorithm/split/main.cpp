#include <algorithm>
#include <iterator>

/**
   Splits any range of items at each occurrence of a specific value, 
   and it copies the chunks that result from that into an output range.

   \see https://github.com/nikolaAV/skeleton/tree/master/algorithm/split
*/

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

void test3()
{
   const string no_delim = "Don't comment bad code - rewrite it.\n --Brian Kernighan";
   vector<string> out;
   split(begin(no_delim),end(no_delim),back_inserter(out),'#',[](auto slice_begin, auto slice_end){
      return string(slice_begin,slice_end);      
   });
   assert(1==out.size());  
   assert(no_delim==out[0]);  
}

void test4()
{
   list<string> out;
   const string right   = "#right";
   split(begin(right),end(right),back_inserter(out),'#',[](auto x0, auto x1){
      return string{x0,x1};
   });
   assert(1==out.size());  
   assert("right"==out.front());  

   out.clear();
   const string left    = "left#";
   split(begin(left),end(left),back_inserter(out),'#',[](auto x0, auto x1){
      return string{x0,x1};
   });
   assert(1==out.size());  
   assert("left"==out.front());  

   out.clear();
   const string nothing = "#";
   split(begin(nothing),end(nothing),back_inserter(out),'#',[](auto x0, auto x1){
      return string{x0,x1};
   });
   assert(0==out.size());  
}

void test5()
{
   const string double_nothing = "##";
   string out;
   size_t count{0};
   split(begin(double_nothing),end(double_nothing),&out,'#',[&count](auto x0, auto x1){
      ++count;
      return string{x0,x1};
   });
   assert(0==count);  
}

int main()
{
   test1();
   test2();
   test3();
   test4();
   test5();
}
