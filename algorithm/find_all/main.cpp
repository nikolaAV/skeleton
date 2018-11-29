
/**
   \brief find all the elements matching a predicate in a given range. The result is iterators referring to requested elements in the range.. 

   \param [first,lst)- a pair of iterators specifying the source range of elements to be found
   \param d_first    - an iterator to the beginning of the destination range. Element type is Ret, see 'unary_op' below
   \param unary_pred - unary predicate (function object) which returns true for the required elements,  a.k.a. search criteria
                       The signature should be 'bool func(InputIt)'
   \param unary_op   - unary operation function object that will be applied for each element (referenced by an iterator) which meets the search criteria
                       The signature should be 'Ret func(InputIt)'
   \retval - Output iterator to the element in the destination range, one past the last element copied.
*/

template< typename InputIt, typename OutputIt, typename UnaryPredicate, typename UnaryOperation >
OutputIt find_all_if( InputIt first, InputIt last, OutputIt d_first, UnaryPredicate unary_pred, UnaryOperation unary_op )
{
   for(;first!=last;++first) {
      if(unary_pred(first)) {
         *d_first++=unary_op(first);
      }
   }
   return d_first;
}

/**
   \param [first,lst)- a pair of iterators specifying the source range of elements to be found
   \param d_first    - an iterator to the beginning of the destination range. Element type is Inputit
   \param unary_pred - unary predicate (function object) which returns true for the required elements,  a.k.a. search criteria
                       The signature should be 'bool func(InputIt)'
   \retval - Output iterator to the element in the destination range, one past the last element copied.
*/

template< typename InputIt, typename OutputIt, typename UnaryPredicate>
OutputIt find_all_if( InputIt first, InputIt last, OutputIt d_first, UnaryPredicate unary_pred)
{
   return find_all_if(first,last,d_first,unary_pred,[](InputIt it){
      return it;   
   });   
}

/**
   \param [first,lst)- a pair of iterators specifying the source range of elements to be found
   \param d_first    - an iterator to the beginning of the destination range. Element type is Inputit
   \param value      - value to compare the elements to
   \retval - Output iterator to the element in the destination range, one past the last element copied.
*/

template< typename InputIt, typename OutputIt, typename T>
OutputIt find_all( InputIt first, InputIt last, OutputIt d_first, const T& value)
{
   return find_all_if(first,last,d_first,[&value](InputIt it){
      return value==*it;
   });   
}

///////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <iterator>
#include <map>
#include <cassert>

void test1()
{
   using namespace std;
                  //  0  1  2  3  4  5  6  7
   const auto seq = { 3, 4, 5, 4, 5, 6, 5, 8 };
   vector<decltype(seq)::const_iterator> matches;

   find_all(begin(seq),end(seq),back_inserter(matches),5);
   cout << "Found " << matches.size() << " matching elements: " << endl;
   for (const auto& it : matches)
      cout << *it << " at position " << (it - cbegin(seq)) << endl;;
}

void test2()
{
   using namespace std;
   const vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   map<int, int> matches;  // {value,index}

   find_all_if(begin(vec),end(vec),inserter(matches,end(matches))
      ,[](auto it){ return *it%3? false:true; }
      ,[vec_begin=begin(vec)](auto it){ return make_pair(*it,it-vec_begin); }
   );

   for(const auto& [value,index] : matches)
      assert(value==index);
}

int main()
{
   test1();
   test2();
}
