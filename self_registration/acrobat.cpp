#include <iostream>
#include "editor.h"
#include "registrar.h"

struct acrobat : auto_enrolment<editor,acrobat>
{
   // concept registrable
   static auto name()   { return "Acrobat"; }
   static auto create() { return interface_ptr{new acrobat()}; }
   // interface shape
   void do_draw(const shape& s) const override { std::cout << "[Acrobat]: "; s.draw(); }
};
        