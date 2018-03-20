#include <iostream>
#include "shape.h"
#include "registrar.h"

struct rectangle : auto_enrolment<shape,rectangle>
{
   // concept registrable
   static auto name()   { return "rectangle"; }
   static auto create() { return interface_ptr{new rectangle()}; }
   // interface shape
   void do_draw() const override { std::cout << "my name is " << name() << std::endl; }
};
        