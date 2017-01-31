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

    { // test 2
            
    }

}
