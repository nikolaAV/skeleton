#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

#include "output_transform_iterator.h"

namespace alt {

template< typename InputIt, typename OutputIt, typename UnaryOperation >
OutputIt transform(InputIt first, InputIt last, OutputIt out, UnaryOperation op)
{
   auto const converter = stdext::make_output_transformer(op);
   return std::copy(first, last, converter(out)).get_underlying();
}

template< typename InputIt, typename OutputIt, typename UnaryPredicate, typename UnaryOperation>
OutputIt transform_if(InputIt first, InputIt last, OutputIt out, UnaryPredicate pred, UnaryOperation op)
{
   auto const converter = stdext::make_output_transformer(op);
   return std::copy_if(first, last, converter(out), pred).get_underlying();
}

} // namespace alt


using namespace std;

void test1()
{
   vector<int> const in = { 0,1,2,3,4,5,6,7,8,9 };
   vector<double> expected = { 0.,2.,4.,6.,8.,10.,12.,14.,16.,18. };
   auto const op = [](auto v) {return v * 2.;};
   vector<double> result1, result2;

   std::transform(in.begin(), in.end(), back_inserter(result1), op);
   assert(result1==expected);

   alt::transform(in.begin(), in.end(), back_inserter(result2), op);
   assert(result2 == expected);
}

void test2()
{
   vector<int> const in = { 0,1,2,3,4,5,6,7,8,9 };
   vector<double> expected = { 0.,4.,8.,12.,16. };
   auto const op = [](auto v) {return v * 2.; };
   auto const even = [](auto v) {return v % 2 == 0; };
   vector<double> result;

   alt::transform_if(in.begin(), in.end(), back_inserter(result), even, op);
   assert(result == expected);
   copy(begin(result), end(result), ostream_iterator<double>(cout, ","));
}

int main()
{
   test1();
   test2();
}

