#include <string_view>

/**
	TBD

	\see 
*/

template <typename UnaryOperation>
void split(std::string_view s, std::string_view delim, UnaryOperation op)
{
   using sv = std::string_view;
   auto op_not_empty = [&op](auto s) { if(s.size()) op(s); };
   size_t pos = 0;
   while ((pos = s.find(delim)) != sv::npos) {
      op_not_empty(sv{s.data(),pos});
      s.remove_prefix(pos+delim.size());
   }
   op_not_empty(s);
}

// Usage

#include "stdwarnings_suppress_on.h"
#include <string>
#include <cassert>
#include <vector>
#include <charconv>
#include <optional>
#include "stdwarnings_suppress_off.h"

using namespace std;

void test1()
{
   vector<string> out;
   const vector<string> expected {"scott","tiger","mushroom"} ;
   split("scott>=tiger>=mushroom>=",">=",[&out](string_view s){
      out.push_back(string{s});
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
   vector<int> out;
   split("1<delimeter>2<delimeter>3<delimeter>4<delimeter>5","<delimeter>",[&out](string_view s){
      int value;
      from_chars(s.data(),s.data()+s.size(),value);
      out.push_back(value);
   });
   assert((vector{1,2,3,4,5})==out);
}

void test4()
{
   optional<string> out;
   split("!@#!@#!@#!@#!@#","!@#",[&out](string_view s){
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
