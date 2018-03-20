#ifndef _EDITOR_INTRFACE_INCLUDED_
#define _EDITOR_INTRFACE_INCLUDED_

#include "shape.h"

class editor
{
   virtual void do_draw(const shape&) const = 0;
public:
   virtual ~editor() {}
   void draw(const shape& s) const
   {  // https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-Virtual_Interface
      do_draw(s);
   } 
};

#endif // _EDITOR_INTRFACE_INCLUDED_

