
/**
   \brief applies the given function 'unary_op' to every element from input range that meets the requirement specified by predicate 'unary_pred'. 

   \param [first,lst)- a pair of iterators specifying the source range of elements to transform
   \param d_first    - an iterator to the beginning of the destination range, may be equal to 'first' or 'last'
   \param unary_pred - unary predicate which returns 'true' for the required elements.
                       The signature should be 'bool func(T v)', where T  is the value type of 'InputIt'
   \param unary_op   - unary operation function object that will be applied
                       The signature of the function should be equivalent to the following: 
                        'Ret func(const T& v)'
                       The type T must be such that an object of type 'InputIt' can be dereferenced.
   \retval - Output iterator to the element past the last element transformed.
*/

template< typename InputIt, typename OutputIt, typename UnaryPredicate, typename UnaryOperation >
OutputIt transform_if( InputIt first, InputIt last, OutputIt d_first, UnaryPredicate unary_pred, UnaryOperation unary_op )
{
   for(;first!=last;++first)
      if(unary_pred(*first))
         *d_first++=unary_op(*first);
   return d_first;
}

///////////////////////////////////////////////////

#include <iterator>
#include <iostream>
#include <initializer_list>

using namespace std;

int main()
{
   const auto seq = {1,2,3,4,5,6};
   transform_if(begin(seq), end(seq), ostream_iterator<int>(cout,",")
      ,[](int v) { return 0==v%2; } // even
      ,[](int v) { return v*2; } // twice
   );
}
