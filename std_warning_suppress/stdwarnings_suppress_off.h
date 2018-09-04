#ifndef STD_WARNING_SUPPRESS_OFF_INCLUDED_
#define STD_WARNING_SUPPRESS_OFF_INCLUDED_

/**
   This header is a mandatory ending of the construct

   #include "stdwarnings_suppress_on.h"   // <-- begin
      #include <iostream>
      #include <vector>
            ...
   #include "stdwarnings_suppress_off.h"  // <-- end
*/

#ifndef  STD_WARNING_SUPPRESS_ON_INCLUDED_ 
   #error stdwarnings_suppress_on.h must be included first
#endif

#undef STD_WARNING_SUPPRESS_ON_INCLUDED_

#ifdef __GNUG__
   #pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
   #pragma warning( pop )
#endif

#endif //STD_WARNING_SUPPRESS_OFF_INCLUDED_
