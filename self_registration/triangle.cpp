#include <iostream>
#include "shape.h"
#include "registrar.h"

struct triangle : auto_enrolment<shape,triangle>
{
   // concept registrable
   static auto name()   { return "triangle"; }
   static auto create() { return interface_ptr{new triangle()}; }
   // interface shape
   void do_draw() const override { std::cout << "my name is " << name() << std::endl; }
};
        