#include "stdwarnings_suppress_on.h"   // <-- begin
   #include <iostream>
   #include <string>
   #include <locale>
#include "stdwarnings_suppress_off.h"  // <-- end

using namespace std;

class widget
{
   std::string locale_;
public:
   std::string& get_locale() { return locale_; }
};

int main()
{
   const char* p;
   {
      widget w;
      std::string loc = w.get_locale();
      p = w.get_locale().c_str();
      w.get_locale() = "what we want not to expect!";
   }
   /**
      Always try to use the compiler in the pedantic mode 
      to be informed about potential problem in your code

      The line below is a cause raising the message (VS2017+CppCoreCheck enabled):
      "warning C26412: Do not dereference an invalid pointer (lifetimes rule 1). 'p' was invalidated at line 24 by 'end of scope for loc, w'.   "
   */
   std::cout << "I'm a dangle pointer: " << p; // <--!!!
}

