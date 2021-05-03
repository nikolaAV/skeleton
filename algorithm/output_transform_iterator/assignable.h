#ifndef STD_EXT_ASSIGNABLE_INCLUDED_
#define STD_EXT_ASSIGNABLE_INCLUDED_

#include <optional>
#include <functional>
#include <utility>

namespace stdext {
   /**
      Motivation: 
      If an object which doesn't have an operator= is a member of another class, 
      then it doesn't allow this class-owner to have its own operator=.
      The compiler is not going to write it for you.
      That is a typical case when you want to have a lambda as a data-member.
      Also that is a problem for all the types that have a copy constructor and no operator=.
   */

   /**
      \brief makes a copyable object assignable
      \param T usually it's a lambda type, because lambdas are copyable but they do not have 'operator='
      \see  https://www.fluentcpp.com/2020/11/06/how-to-make-a-copyable-object-assignable-in-cpp/
   */

   template<typename T>
   class assignable
   {
      std::optional<T> value_;

   public:
      assignable(T const& value) : value_(value) {}
      assignable(T&& value) : value_(std::move(value)) {}
      assignable(assignable&& other) = default;
      assignable(assignable const& other) = default;
      assignable& operator=(assignable&& other) = default;

      assignable& operator=(assignable const& other)
      {
         value_.emplace(*other.value_);
         return *this;
      }

      T const& get() const { return *value_; }
      T& get() { return *value_; }
   };

   template<typename T>
   class assignable<T&>
   {
      std::reference_wrapper<T> value_;

   public:
      explicit assignable(T& value) : value_(value) {}

      T const& get() const { return value_; }
      T& get() { return value_; }
   };

} // namespace stdext 

#endif // STD_EXT_ASSIGNABLE_INCLUDED_
