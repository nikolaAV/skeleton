#include "ascii_string.h"

#include <iostream>
#include <cassert>
#include <sstream>

using namespace std; 

void test_01() {
   assert(ascii::string { "Hello, World!"}== "Hello, World!");
   assert(ascii::wstring{L"Hello, World!"}==L"Hello, World!");
   assert( "Hello, World!"==ascii::string { "Hello, World!"});
   assert(L"Hello, World!"==ascii::wstring{L"Hello, World!"});

   assert(ascii::string{"characters: '\xFF' and '\xAA'"} == "characters: '?' and '?'" );
   assert(ascii::string(10,char(0xFF))=="??????????");
}

void test_01a() {
   using namespace ascii_literals;

   assert( "Hello, World!"_ascii  == "Hello, World!");
   assert(L"Hello, World!"_ascii  ==L"Hello, World!");
   assert( "Hello, World!"== "Hello, World!"_ascii);
   assert(L"Hello, World!"==L"Hello, World!"_ascii);
}

void test_02() {

   try {
      using ascii_string = ascii::basic_string<char,ascii::sieve_exception<char>>;
      const ascii_string s = R"(characters: 'ÿ' and 'ª')";
   }
   catch(const std::invalid_argument& e) {
      assert(std::string{"no ascii character"} == e.what());
      return;
   }
   assert(false);
}

void test_02a() {

   try {
      using ascii_string = ascii::basic_string<char,ascii::sieve_exception<char,std::domain_error>>;
      const ascii_string s = "characters: '\xFF' and '\xAA'";
   }
   catch(const std::domain_error& e) {
      assert(std::string{"no ascii character"} == e.what());
      return;
   }
   assert(false);
}

void test_02b() {
   using ascii_string = ascii::basic_string<char,ascii::sieve_replacement<char,'#'>>;
   assert(ascii_string{"characters: '\xFF' and '\xAA'"} == "characters: '#' and '#'" );
   assert(ascii_string(10,char(0xFF))=="##########");
}

void test_03() {
   using namespace ascii_literals;

   const auto s1 = "Hello"_ascii;
   const auto s2 = "World!"_ascii;
   const auto s3 = s1 + ", " + s2;
   assert(s3 =="Hello, World!");
   static_assert(is_same_v<decltype(s1),decltype(s3)>);
}

void test_04() {
   using ascii::cast;
   assert(cast<ascii::string> ( "Hello, World!"s)  ==  "Hello, World!");
   assert(cast<ascii::wstring>( "Hello, World!"s)  == L"Hello, World!");
   assert(cast<ascii::wstring>(L"Hello, World!"s)  == L"Hello, World!");
   assert(cast<ascii::string> (L"Hello, World!"s)  ==  "Hello, World!");

   assert(cast<ascii::string> ( "Copyright symbol: '\xB8'"s)         == "Copyright symbol: '?'");
   assert(cast<ascii::string> (L"Copyright symbol: '\xA9', Utf-16"s) == "Copyright symbol: '?', Utf-16");
}

void test_05() {
   using namespace ascii_literals;
   {
      stringstream ss;
      ss << "Hello, World!"_ascii;
      assert(ss.str() =="Hello, World!");
   }
   {
      wstringstream ss;
      ss << L"Hello, World!"_ascii;
      assert(ss.str() == L"Hello, World!");
   }
   {
      stringstream ss;
      ss << "characters: '\xFF' and '\xAA'"_ascii;
      assert(ss.str() == "characters: '?' and '?'");
   }
}

void test_06() {
   ascii::string  s1 {"C++ protects against accident, not against fraud"};
   std::string    s2 {"Bjarne Stroustrup"};

   swap(s1,s2);
   assert("Bjarne Stroustrup"==s1);
   assert("C++ protects against accident, not against fraud"==s2);

   swap(s2,s1);
   assert("Bjarne Stroustrup"==s2);
   assert("C++ protects against accident, not against fraud"==s1);
}

void test_06a() {
   ascii::string  s1;
   std::string    s2 {"characters: '\xFF' and '\xAA'"};
   swap(s1,s2);
   assert(s1=="characters: '?' and '?'");
   assert(s2.empty());
}

void test_07() {
   const std::string    original_message {"The C++ Programming Language"};
                                       //   0   1     2          3
   std::istringstream   is {original_message};
   std::ostringstream   os;
   ascii::string        s0, s1, s2, s3;

   is >> s0 >> s1 >> s2 >> s3;
   assert(s0 +" "+ s1 +" "+ s2+ " " +s3 ==original_message.c_str());
   os << s0 << " " << s1 << " " << s2 << " " << s3;
   assert(original_message == os.str());
}

void test_07a() {
   ascii::string        s;
//   std::istringstream   is {R"(German-umlaut:'ä'-and-euro-symbol:'¤')"};
   std::istringstream   is {"German-umlaut:'\xE4'-and-euro-symbol:'\xA4'"};  // ISO Latin-15 encoding
   is >> s;
   assert(s=="German-umlaut:'?'-and-euro-symbol:'?'");
}

int main()
{
   test_01();
   test_01a();
   test_02();
   test_02a();
   test_02b();
   test_03();
   test_04();
   test_05();
   test_06();
   test_06a();
   test_07();
   test_07a();
}
