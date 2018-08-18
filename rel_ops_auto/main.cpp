
namespace comparable
{
   /**
      A class that implements the equality operator (==) will likely also implement the
      inequality operator (!=), and a class that implements the less-than operator (<) will
      likely implement the other relational operators as well (>, <=, >=). In many cases,
      the definition of only one of these operators is actually interesting, while the others
      can simply be defined in terms of that one operator.
      The C++ standard library contains similar such definitions as part of the <utility> header. 
      However, these definitions (for !=, >, <=, and >=) were relegated to the namespace std::rel_ops during standardization,
      when it was determined that they caused problems when made available in namespace std.
      The formulation of these operator templates based on CRTP allows classes to opt in to the general operator definitions,
      providing the benefits of increased code reuse without the side effects of an overly general operator.

      \see  https://en.wikipedia.org/wiki/Barton%E2%80%93Nackman_trick
            https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
            https://en.cppreference.com/w/cpp/utility/rel_ops/operator_cmp
   */

   template <typename Derived>
   struct type {
      friend bool operator!= (const Derived& left, const Derived& right) noexcept(noexcept(left==right)) {
         return !(left == right);
      }
      friend bool operator> (const Derived& left, const Derived& right) noexcept(noexcept(right < left)) {
         return right < left; 
      }
      friend bool operator<= (const Derived& left, const Derived& right) noexcept(noexcept(right < left)) {
         return !(right < left); 
      }
      friend bool operator>= (const Derived& left, const Derived& right) noexcept(noexcept(left < right)) {
         return !(left < right); 
      }
   };
} // namespace comparable 


#include <iostream>

struct Foo : comparable::type<Foo> {
    int n;
    Foo(int v) : n(v) {}
};
 
bool operator==(const Foo& lhs, const Foo& rhs)
{
    return lhs.n == rhs.n;
}
 
bool operator<(const Foo& lhs, const Foo& rhs)
{
    return lhs.n < rhs.n;
}
 
int main()
{
    const Foo f1{1}, f2{2};

    using namespace std;
    cout << boolalpha;
    cout << "not equal?     : " << (f1 != f2) << '\n';
    cout << "greater?       : " << (f1 > f2) << '\n';
    cout << "less equal?    : " << (f1 <= f2) << '\n';
    cout << "greater equal? : " << (f1 >= f2) << '\n';
}
