#include <iostream>
#include "shape.h"
#include "registrar.h"

struct circle : auto_enrolment<shape,circle>
{
   // concept registrable
   static auto name()   { return "circle"; }
   static auto create() { return interface_ptr{new circle()}; }
   // interface shape
   void do_draw() const override { std::cout << "my name is " << name() << std::endl; }
};
        