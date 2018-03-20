#include <iostream>
#include "registrar.h"
#include "shape.h"
#include "editor.h"

using shapes   = registrar<shape>;
using editors  = registrar<editor>;

int main()
{
   auto ab = editors::get("Acrobat");
   assert(ab);
   auto wp = editors::get("WordPad");
   assert(wp);

   if(auto s = shapes::get("circle"))
      ab->draw(*s);
   if(auto s = shapes::get("rectangle"))
      wp->draw(*s);
   if(auto s = shapes::get("triangle"))
      s->draw();
   if(auto s = shapes::get("unknown"))
      s->draw();

   std::cin.get();
} 

        