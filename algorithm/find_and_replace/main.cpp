#include <string>
#include <string_view>

/**
   \brief Find and Replace All Occurrences

   \param [in,out] source
   \param [in]     substr, string to search for
   \param [in]     scount, length of 'substr' to search for
   \param [in]     replacement, 	string to use for replacement
   \param [in]     rcount, length of the replacement

   \see  https://bits.mdminhazulhaque.io/cpp/find-and-replace-all-occurrences-in-cpp-string.html
         https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
*/

template <typename CharT, typename Traits=std::char_traits<CharT>, typename Allocator=std::allocator<CharT>>
std::basic_string<CharT,Traits,Allocator>& 
find_and_replace(
       std::basic_string<CharT,Traits,Allocator>& source
      ,const CharT* substr, typename std::basic_string<CharT,Traits,Allocator>::size_type scount
      ,const CharT* replacement, typename std::basic_string<CharT,Traits,Allocator>::size_type rcount)
{
    using string_type = std::basic_string<CharT,Traits,Allocator>;
    using size_type   = typename string_type::size_type;
    auto i = size_type{0};
    while((i = source.find(substr, i, scount)) != string_type::npos) {
        source.replace(i, scount, replacement, rcount);
        i += rcount;
    }
    return source;
}

template <typename CharT, typename Traits=std::char_traits<CharT>, typename Allocator=std::allocator<CharT>>
std::basic_string<CharT,Traits,Allocator>& 
find_and_replace(
       std::basic_string<CharT,Traits,Allocator>&  source
      ,std::basic_string_view<CharT,Traits>        substr
      ,std::basic_string_view<CharT,Traits>        replacement)
{
   return find_and_replace(source
                           ,substr.data(),substr.length()
                           ,replacement.data(),replacement.length()
   );
}

inline
std::string&
find_and_replace(std::string& source, std::string_view substr, std::string_view replacement)
{
   return find_and_replace<char>(source,substr,replacement);
}

inline
std::wstring&
find_and_replace(std::wstring& source, std::wstring_view substr, std::wstring_view replacement)
{
   return find_and_replace<wchar_t>(source,substr,replacement);
}

inline
std::u16string&
find_and_replace(std::u16string& source, std::u16string_view substr, std::u16string_view replacement)
{
   return find_and_replace<char16_t>(source,substr,replacement);
}

inline
std::u32string&
find_and_replace(std::u32string& source, std::u32string_view substr, std::u32string_view replacement)
{
   return find_and_replace<char32_t>(source,substr,replacement);
}


#include <iostream>
#include <cassert>

using namespace std;

void test_string()
{
   string text;

   text = "I have a blue house and a blue car";
   assert(find_and_replace(text,"blue","red")=="I have a red house and a red car");

   text = "";
   assert(find_and_replace(text,"blue","red")=="");

   text = "\r\nnew_line\r\nnew_line\r\nnew_line";
   assert(find_and_replace(text,"\r\n","\n")=="\nnew_line\nnew_line\nnew_line");
}

void test_wstring()
{
   wstring text;

   text = L"   Remove all spaces.";
   assert(find_and_replace(text,L" ",L"")==L"Removeallspaces.");
}


int main()
{
   test_string();
   test_wstring();
}