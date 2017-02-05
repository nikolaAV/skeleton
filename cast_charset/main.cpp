#include <iostream>
#include "charset_cast.h"

using namespace std;

int main()
{
    { // test 1
        const char* s { "'The Standard C++ Library' by Nicolai M.Josuttis. 16.4.4 Character Classification and Conversion"}; 
        const wstring ws { charset_cast<wstring>(s) };
        assert(charset_cast<string>(ws)==s);
        cout    << s << endl;
        wcout   << ws << endl;
    }

    try
    { // test 2
#ifdef _MSC_VER
      // string with German umlaut and euro symbol (in Windows encoding):
      // http://www.science.co.il/Language/Character-code.asp?s=1252

       const auto& loc = locale("deu_DEU.1252");
       const string original = "nj: \xE4 + \x80 1";
#else
      // string with German umlaut and euro symbol (in ISO Latin-15 encoding):
      // http://www.science.co.il/Language/Character-sets.asp

       const auto& loc = locale("deu_DEU.ISO-8859-15");
       const string original = "nj: \xE4 + \xA4 1";
#endif

       cout << endl << "default locale: " << original;
       cout.imbue(loc);
       cout << endl << "german locale: " << original;

       const auto w = to_wstring(original, loc);
       wcout << endl <<  w << endl;

       assert(original==to_string(w,loc));
    }
    catch(const std::exception& e)
    {
       cout << e.what() << endl;
    }
}
