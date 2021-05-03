#ifndef STD_EXT_OUTPUT_TRANSFORM_ITERATOR_INCLUDED_
#define STD_EXT_OUTPUT_TRANSFORM_ITERATOR_INCLUDED_

#include <utility>
#include <iterator>
#include "assignable.h"

namespace stdext {

   /**
      \brief  shifts the transform function into the output iterator.

      \param Iterator   - a type of the output iterator for which into the transform function has to be mixed
      \param TransformFunction    - any functional object with signature:
                                       Iterator::value_type func(T)
      \see  https://www.fluentcpp.com/2017/11/28/output-iterator-adaptors-symmetry-range-adaptors/
   */

   template<typename Iterator, typename TransformFunction>
   class output_transform_iterator
   {
      Iterator iterator_;
      assignable<TransformFunction> function_;

   public:
      explicit output_transform_iterator(Iterator iterator, TransformFunction function)
         : iterator_(std::move(iterator))
         , function_(std::move(function))
      {}

      output_transform_iterator& operator++() { ++iterator_; return *this; }
      output_transform_iterator& operator*() { return *this; }
      template<typename T>
      output_transform_iterator& operator=(T const& value)
      {
         *iterator_ = function_.get()(value);
         return *this;
      }
      Iterator get_underlying() const { return iterator_; }
   };

   template<typename TransformFunction>
   class output_transformer
   {
      TransformFunction function_;

   public:
      explicit output_transformer(TransformFunction function) : function_(function) {}
      template<typename Iterator>
      output_transform_iterator<Iterator, TransformFunction> operator()(Iterator iterator) const
      {
         return output_transform_iterator<Iterator, TransformFunction>(iterator, function_);
      }
   };

   template<typename TransformFunction>
   output_transformer<TransformFunction> make_output_transformer(TransformFunction function)
   {
      return output_transformer<TransformFunction>(function);
   }

}  // namespace stdext

namespace std
{
   template<typename Iterator, typename TransformFunction>
   struct iterator_traits<stdext::output_transform_iterator<Iterator, TransformFunction>>
   {
      using iterator_category = typename iterator_traits<Iterator>::iterator_category;
      using value_type = typename iterator_traits<Iterator>::value_type;
   };

} // namespace std

#endif // STD_EXT_OUTPUT_TRANSFORM_ITERATOR_INCLUDED_
