#include <iostream>
#include "editor.h"
#include "registrar.h"

struct wordpad : auto_enrolment<editor,wordpad>
{
   // concept registrable
   static auto name()   { return "WordPad"; }
   static auto create() { return interface_ptr{new wordpad()}; }
   // interface shape
   void do_draw(const shape& s) const override { std::cout << "[WordPad]: "; s.draw(); }
};
        