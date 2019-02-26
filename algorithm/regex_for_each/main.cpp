#include <regex>
#include <string>
#include <utility>

/**
   Iterates over all matches of a regular search and applies the given function object 'f' to each macth in the target sequence of symbols, in order.
   
   \param [in] first, last - the range of a character sequence to apply the function to
   \param [in] rx - object representing the regular expression to check whether a character sequence matches a specific pattern
   \param [in] f  - function object of Unary function type, to be applied to the match-subset in the range of a character sequence [first, last) 
                    The signature of the function should be equivalent to the following:
                        void fun(const std::match_results<CharT>&)
                    UnaryFunction must meet the requirements of MoveConstructible. Does not have to be CopyConstructible.
   \return		f
   \exception	regex_error and any exception which can be thrown by f

	Example usage:
	\code
	   const string data =	
            "<person>\n"
					   "<first>Bjarne</first>\n"
					   "<last>Stroustrup</last>\n"
				"</person>\n";
	   regex reg("<(.*)>(.*)</(\\1)>");	// <-- ECMAScript
	   for_each(data,reg,[](const std::smatch& m){
		   cout << "match:" << m.str(0) << ", tag: " << m.str(1) << ", value: " << m.str(2) << endl; 
	   });														
	\endcode
*/

template< class CharT, class UnaryFunction >
UnaryFunction for_each(
    typename std::basic_string<CharT,std::char_traits<CharT>,std::allocator<CharT>>::const_iterator first
   ,typename std::basic_string<CharT,std::char_traits<CharT>,std::allocator<CharT>>::const_iterator last 
   ,const typename std::basic_regex<CharT,std::regex_traits<CharT>>& rx
   ,UnaryFunction f
)
{
   using regex_iterator = std::regex_iterator<decltype(first),CharT,std::regex_traits<CharT>>;
	for (regex_iterator it{first,last,rx}; it != regex_iterator{}; ++it)
	   f(*it);
	return f;
}

template< class CharT, class UnaryFunction >
UnaryFunction for_each(
    const std::basic_string<CharT,std::char_traits<CharT>,std::allocator<CharT>>& str
   ,const typename std::basic_regex<CharT,std::regex_traits<CharT>>& rx
   ,UnaryFunction f
)
{
   return for_each(str.begin(),str.end(),rx,std::move(f));
}

////// Example of Usage:

#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
	const string data =	
      "<book>\n"
		   "<title>The Design and Evolution of C++</title>\n"
		   "<author>Bjarne Stroustrup</author>\n"
			"<publisher>Addison-Wesley</publisher>\n"
			"<year>1994</year>\n"
		"</book>\n"
      "<book>\n"
		   "<title>THE C++STANDARD LIBRARY. A Tutorial and Reference</title>\n"
		   "<author>Nicoli M. Josuttis</author>\n"
			"<publisher>Addison-Wesley</publisher>\n"
			"<year>2012</year>\n"
		"</book>\n"
      "<book>\n"
		   "<title>Effective Modern C++</title>\n"
		   "<author>Scott Meyers</author>\n"
			"<publisher>O'Reilly</publisher>\n"
			"<year>2014</year>\n"
		"</book>\n";

      // groups:          [1]  [2]     \see https://github.com/nikolaAV/Modern-Cpp/tree/master/regex
	for_each(data,regex{"<(.*)>(.*)</(\\1)>"},[](auto const& m){
		cout << left << setw(12) << m[1] << m[2] << endl; 
	});														
}

