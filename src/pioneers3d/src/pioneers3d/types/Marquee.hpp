#ifndef MARQUEE_HPP
#define MARQUEE_HPP

#include <pioneers3d/types/Font.hpp>

namespace pioneers3d {


class Marquee_t
{
public:
   Marquee_t();
   ~Marquee_t();

   void
   draw();

   Font_t Font;
   glm::ivec2 Pos;
   uint32_t Color;
   TextAlign Align;
   std::string Text;

   uint32_t AnimType;     // current anim type
   uint32_t AnimDirection; // current anim move direction
   uint64_t AnimTime;     // current time in [ms]
   uint64_t AnimDuration; // total time in [ms]
   uint64_t AnimStart;    // time of animation start in [ms]
};

} // end namespace pioneers3d

#endif // MARQUEE_HPP
