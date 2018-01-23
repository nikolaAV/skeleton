#ifndef _STD_WARNING_SUPPRESS_ON_INCLUDED_
#define _STD_WARNING_SUPPRESS_ON_INCLUDED_

/**
   We have no chance to edit the original C++ Library sources.
   On the other hand, we want to compile our own translation units with the strongest level of static code analysis.
   This header file is aimed:
      1 - to suppress every well-known warning message comes from C++ std. 
      2 - any client code just needs to enclose <std_header> like that 
         #include "stdwarnings_suppress_on.h"
         #include <iostream>
         #include <vector>
         ...
         #include "stdwarnings_suppress_off.h"
*/


#ifdef _MSC_VER
   #pragma warning( push )
   #pragma warning( disable: 4365 ) // '<expression>': conversion from '<type>' to '<type>', signed/unsigned mismatch
   #pragma warning( disable: 4514 ) // '<name>': unreferenced inline function has been removed
   #pragma warning( disable: 4571 ) // Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
   #pragma warning( disable: 4625 ) // '<type>': copy constructor was implicitly defined as deleted
   #pragma warning( disable: 4626 ) // 'type': assignment operator was implicitly defined as deleted
   #pragma warning( disable: 4710 ) // '<name>': function not inlined
   #pragma warning( disable: 4774 ) // '<name>' : format string expected in argument 3 is not a string literal
   #pragma warning( disable: 4820 ) // '<name>': '<num>' bytes padding added after data member '<name>'
   #pragma warning( disable: 5026 ) // '<type>': move constructor was implicitly defined as deleted
   #pragma warning( disable: 5027 ) // '<type>': move assignment operator was implicitly defined as deleted
   #pragma warning( disable: 5031 ) // #pragma warning(pop): likely mismatch, popping warning state pushed in different file

   // ... TBD

      // C++ Code Analysis Warnings & CppCoreGuidelines cautions:
   #pragma warning( disable: 26400 ) // Do not dereference a invalid pointer (lifetimes rule 1)

   // ... TBD
#endif

#ifdef __GNUG__
   #pragma GCC diagnostic push
   #pragma GCC diagnostic ignored "-Wunused-function"
#endif


/**
   any standard headers

*/


/**
   #include "stdwarnings_suppress_off.h" is expected!!!
*/

#endif //_STD_WARNING_SUPPRESS_ON_INCLUDED_