#include <cstdint>
#include <cstddef>
#include <string_view>

namespace s3 // sss, string switch support
{

/**
   \brief Fowler–Noll–Vo hash function
   \see https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
*/
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count) {
   return ((count ? fnv1a_32(s, count - 1) : 2166136261ull) ^ s[count]) * 16777619ull;
}

constexpr std::uint32_t operator"" _hash(char const* s, std::size_t count) {
    return fnv1a_32(s, count);
}

constexpr std::uint32_t hash(std::string_view sv) {
    return fnv1a_32(sv.data(), sv.size());
}

}  //end of namespace s3

///////////////////////////////////////////////////

#include <iostream>

using namespace std;

void do_this()                { cout << "do_this" << endl; }
void do_that()                { cout << "do_that" << endl; }
void do_something_else()      { cout << "do_something_else" << endl; }
void dont_know_what_to_do()   { cout << "dont_know_what_to_do" << endl; }


void test_run_time_if(const string& v)
{
   if     ("value X"==v)
      do_this();
   else if("value Y"==v)
      do_that();
   else if("value Z"==v)
      do_something_else();
   else
      dont_know_what_to_do();
}

void test_string_switch(const string& v)
{
   using namespace s3;
   switch(s3::hash(v)) {
      case "value X"_hash : do_this();             break;
      case "value Y"_hash : do_that();             break;
      case "value Z"_hash : do_something_else();   break;
      default :             dont_know_what_to_do();
   }
}

int main()
{
   test_string_switch("value X");
   test_string_switch("value Z");
   test_string_switch("value #");
}
