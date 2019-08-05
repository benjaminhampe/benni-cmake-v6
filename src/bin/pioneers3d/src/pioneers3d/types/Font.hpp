/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_FONT_HPP
#define PIONEERS3D_TYPES_FONT_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

// Game Fonts
enum class eFontType : uint32_t
{
   Default = 0,
   Marquee,
   H1,
   H2,
   H3,
   H4,
   H5,
   H6,
   User,
   CHAT_BG,
   CHAT,
   AWESOME,
   FPS_COUNTER,
   SMALL,
};

struct Font_t
{
   eFontType Type;
   std::string FileName;
   uint32_t Size;
   uint32_t Style;
   irr::gui::CGUITTFont* Font;
   bool AntiAlias;
   bool Transparent;

   Font_t()
      : Type( eFontType::Default )
      , FileName( "fonts/Garton.ttf" )
      , Size( 16 )
      , Style( 0 )
      , Font( nullptr )
      , AntiAlias( true )
      , Transparent( true )
   {}

   glm::ivec2
   getTextSize( std::string const & txt ) const
   {
      if ( !Font ) { return glm::ivec2(0,0); }
      irr::core::dimension2du const irrWitz = Font->getDimension( irr::core::stringw( txt.c_str() ).c_str() );
      return glm::ivec2( int32_t( irrWitz.Width ), int32_t( irrWitz.Height ) );
   }
};

typedef std::vector< Font_t > Fonts_t;

class TextAlign
{
public:
   enum eTextAlign : uint32_t {
      Default = 0,
      Left = 1,
      Center = 1<<1,
      Right = 1<<2,
      Top = 1<<3,
      Middle = 1<<4,
      Bottom = 1<<5,
      Baseline = 1<<6,
   };

   TextAlign() : m_Align( Default ) {}
   TextAlign( uint32_t flags ) : m_Align( flags ) {}

   operator uint32_t() const { return m_Align; }

private:
   uint32_t m_Align;
};

Font_t
Font_create( irr::gui::IGUIEnvironment* env,
            std::string fileName,
            uint32_t fontSizePx,
            uint32_t fontStyle,
            eFontType fontType,
            bool aa,
            bool transparent );

irr::core::dimension2du
Font_getTextDimension( Font_t const & font, std::string const & txt );

glm::ivec2
Font_getTextSize( Font_t const & font, std::string const & txt );

irr::video::IVideoDriver*
Font_getVideoDriver( Font_t const & font );

void
Font_draw( Font_t const & font,
            std::string const & txt,
            glm::ivec2 const & pos,
            uint32_t color,
            uint32_t textAlign = 0 );


inline void
Font_draw( Font_t const & font,
            std::string const & txt,
            irr::core::recti const & r,
            uint32_t color )
{
   int32_t x = r.UpperLeftCorner.X;
   int32_t y = r.UpperLeftCorner.Y;
   Font_draw( font, txt, glm::ivec2( x, y ), color, TextAlign::Center | TextAlign::Middle );
}

// void Font_draw( Font_t const & font, std::string const & txt, int32_t x, int32_t y, uint32_t color );
// void Font_draw( Font_t const & font, std::string const & txt, irr::core::recti const & pos, uint32_t color );

// Game logic
class Game_t;

struct FontManager_t
{
   Fonts_t Fonts;

   FontManager_t();
   ~FontManager_t();

   void init( irr::gui::IGUIEnvironment* env );
   void clear();
   void add( irr::gui::IGUIEnvironment* env, eFontType type, std::string fileName, uint32_t sizePx, uint32_t style, bool aa, bool transparent );

   Font_t get( eFontType type ) const;

};


class Text_t
{
public:
   Text_t()
      : Font()
      , Pos( 0, 0 )
      , Color( 0xFFFFFFFF )
      , Align( TextAlign::Center | TextAlign::Middle )
      , Text( "Hello Text !\nHello World !!!" )
   {}

   Text_t( Font_t font, std::string txt, glm::ivec2 pos, uint32_t color, TextAlign align = TextAlign::Center | TextAlign::Middle)
   {
      Font = font;
      Pos = pos;
      Color = color;
      Align = align;
      Text = txt;
   }

   void
   draw()
   {
      Font_draw( Font, Text, Pos, Color, Align );
   }

   void
   setData( Font_t font, std::string txt, glm::ivec2 pos, uint32_t color, TextAlign align = TextAlign::Center | TextAlign::Middle)
   {
      Font = font;
      Pos = pos;
      Color = color;
      Align = align;
      Text = txt;
   }

   Font_t Font;
   glm::ivec2 Pos;
   uint32_t Color;
   TextAlign Align;
   std::string Text;
};

} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_FONT_HPP
