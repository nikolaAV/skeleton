#ifndef _EXTERNAL_TUT_INCLUDED_
#define _EXTERNAL_TUT_INCLUDED_

/**
   During compilation original TUT (http://mrzechonek.github.io/tut-framework/) with 'pedantic' warning level
   some issues appeare
   This header was just aimed to suppress them and to integrate TUT and your your domain app in the simplest way
   there are only two things what you need to do
   - 1) [project global setting], set a path to TUT includes, like ...<tut_root>/include  
   - 2) include this header in your translation units 
*/

#ifdef _MSC_VER
   #pragma warning( push )
   #pragma warning( disable: 4571 ) // semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
   #pragma warning( disable: 4626 ) // 'tut::no_such_group': assignment operator was implicitly defined as deleted
   #pragma warning( disable: 4061 ) // enumerator 'tut::test_result::skipped' in switch of enum 'tut::test_result::result_type' is not explicitly handled by a case label
   #pragma warning( disable: 4625 ) // '<class_name>' : copy constructor was implicitly defined as deleted
#endif

#ifdef __GNUG__
   #pragma GCC diagnostic push
   #pragma GCC diagnostic ignored "-Wunused-function"
#endif


#include <tut/tut.hpp>
#include <tut/tut_reporter.hpp>

#ifdef __GNUG__
   #pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
   #pragma warning( pop )
#endif

#endif //_EXTERNAL_TUT_INCLUDED_
