#ifndef _SHAPE_INTRFACE_INCLUDED_
#define _SHAPE_INTRFACE_INCLUDED_

class shape
{
   virtual void do_draw() const = 0;
public:
   virtual ~shape() {}
   void draw() const
   {  // https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-Virtual_Interface
      do_draw();
   } 
};

#endif // _SHAPE_INTRFACE_INCLUDED_

