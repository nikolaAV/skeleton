#include <variant>
#include <utility>

/**
   Composes a visitor from separate alternatives (matchers) and applies it to the requested variant.
   
   \param v - variant
   \param ms - list of Callable objects
   \reval - the value returned by the selected matcher of the visitor.
   \see https://www.bfilipek.com/2019/06/fsm-variant-game.html
        https://github.com/lethal-guitar/VariantTalk/blob/308401a978d36a32f27797b6361a0f2143d71035/shared/match.hpp#L46
*/

namespace details_
{ 
   // https://github.com/nikolaAV/Modern-Cpp/tree/master/lambda/lambda_overloaded
   template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
   template<typename... Ts> overloaded(Ts...)->overloaded<Ts...>;
}  // end of namespace details_

template <typename Variant, typename... Matcher>
auto match(Variant&& v, Matcher&&... ms) {
   return std::visit(
       details_::overloaded{std::forward<Matcher>(ms)...}
      ,std::forward<Variant>(v)
   );
}

/**
   Example of usage
   \see https://www.walletfox.com/course/cheatsheetsSource/pattern_matching_classes_cheatsheet-compressor.png
*/

#include <numeric>
#include <vector>
#include <iterator>
#include <iostream>

using namespace std;

struct rectangle {
   double width_  = 0;
   double height_ = 0;
};

struct semi_circle {
   double radius_ = 0;
};

using shape       = variant<rectangle,semi_circle>;
using container   = vector<shape>;

int main()
{
   const container c = {
       rectangle{7,5}
      ,semi_circle{2.5}
      ,semi_circle{3.5}
   };

   const auto total = accumulate(begin(c),end(c),0.,
      [](auto sum, const shape& s){
            return sum + match(s,
                [](const rectangle& r)   { return r.width_*r.height_; }
               ,[](const semi_circle& c) { return 3.14159*c.radius_*c.radius_*.5; }
            );
      }
   );
   cout << "area total: " << total << endl;
   cin.get();
}
