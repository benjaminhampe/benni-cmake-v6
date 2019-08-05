#include <pioneers3d/types/Marquee.hpp>


namespace pioneers3d {



Marquee_t::Marquee_t()
   : Font()
   , Pos( 0, 0 )
   , Color( 0xFFFFFFFF )
   , Align( TextAlign::Center | TextAlign::Middle )
   , Text( "Hello Marquee !\nHello World !!!" )
{

}

Marquee_t::~Marquee_t()
{

}

void
Marquee_t::draw()
{
   Font_draw( Font, Text, Pos, Color, Align );
}


} // end namespace pioneers3d
