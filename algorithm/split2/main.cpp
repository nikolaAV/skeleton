#include <string_view>
#include <utility>

/**
	This function moves a std::string_view slider between delimiter markers without any unnecessary copies or data modifications.
	The interface is very similar to std::for_each where an user-defined function object
	is to apply to every sequence of characters which are enclosed within specified delimeters - another sequence of characters.

	\see https://github.com/nikolaAV/skeleton/tree/master/algorithm/split2
*/

namespace details
{

template <typename UnaryOperation, typename CharT, typename Traits = std::char_traits<CharT> >
UnaryOperation split(std::basic_string_view<CharT,Traits> s, std::basic_string_view<CharT,Traits> delim, UnaryOperation op)
      noexcept( noexcept(op(std::basic_string_view<CharT,Traits>{})) )
{
   using sv = std::basic_string_view<CharT,Traits>;
   const auto op_not_empty = [&op](auto s) { if(s.size()) op(s); };
   size_t pos = 0;
   while ((pos = s.find(delim)) != sv::npos) {
      op_not_empty(sv{s.data(),pos});
      s.remove_prefix(pos+delim.size());
   }
   op_not_empty(s);
   return op;
}

template <typename UnaryOperation, typename StringView>
UnaryOperation split(StringView s, StringView delim, UnaryOperation op)
{
   using CharT  = typename StringView::value_type;
   using Traits = typename StringView::traits_type;
   return split<UnaryOperation,CharT,Traits>(s,delim,op);
}

}  // end of namespace details

template <typename UnaryOperation>
UnaryOperation split(std::string_view s, std::string_view delim, UnaryOperation op)
{
   return details::split<UnaryOperation,decltype(s)>(s,delim,op);
}

template <typename UnaryOperation>
UnaryOperation split(std::wstring_view s, std::wstring_view delim, UnaryOperation op)
{
   return details::split<UnaryOperation,decltype(s)>(s,delim,op);
}

template <typename UnaryOperation>
UnaryOperation split(std::u16string_view s, std::u16string_view delim, UnaryOperation op)
{
   return details::split<UnaryOperation,decltype(s)>(s,delim,op);
}

template <typename UnaryOperation>
UnaryOperation split(std::u32string_view s, std::u32string_view delim, UnaryOperation op)
{
   return details::split<UnaryOperation,decltype(s)>(s,delim,op);
}

// Usage

#include <string>
#include <cassert>
#include <vector>
#include <charconv>
#include <optional>

using namespace std;

void test1()
{
   vector<string>       out;
   const vector<string> expected {"scott","tiger","mushroom"} ;
   split("scott--->tiger--->mushroom--->","--->",[&out](string_view s){
      out.emplace_back(s.data(),s.size());
   });
   assert(expected==out);
}

void test2()
{
   string out;
   split("key:value",":",[&out](string_view s){
      out += "+" + string{s};
   });
   assert("+key+value"==out);
}

void test3()
{
   struct callback {
      vector<int> out;
      void operator()(string_view s) {
         int value{0};
         from_chars(s.data(),s.data()+s.size(),value);
         out.push_back(value);
      }
   };
   const auto result = split("1<$>2<$>3<$>4<$>5","<$>",callback{});
   assert((vector{1,2,3,4,5})==result.out);
}

void test4()
{
   optional<wstring> out;
   split(L"!@#!@#!@#!@#!@#",L"!@#",[&out](auto const s){
      out = s;
   });
   assert(!out.has_value());
}

int main()
{
   test1();
   test2();
   test3();
   test4();
}
