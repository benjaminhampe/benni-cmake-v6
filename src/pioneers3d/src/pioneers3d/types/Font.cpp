#include <pioneers3d/types/Font.hpp>
#include <pioneers3d/types/Game.hpp>
#include <irrExt/CGUITTFont.hpp>
#include <algorithm>

std::ostream &
operator<< ( std::ostream & o, pioneers3d::eFontType const fontType )
{
   switch( fontType )
   {
      //case pioneers3d::eFontType::Default: o << "Default"; break;
      case pioneers3d::eFontType::Marquee: o << "Marquee"; break;
      case pioneers3d::eFontType::H1: o << "H1"; break;
      case pioneers3d::eFontType::H2: o << "H2"; break;
      case pioneers3d::eFontType::H3: o << "H3"; break;
      case pioneers3d::eFontType::H4: o << "H4"; break;
      case pioneers3d::eFontType::H5: o << "H5"; break;
      case pioneers3d::eFontType::H6: o << "H6"; break;
      case pioneers3d::eFontType::AWESOME: o << "AWESOME"; break;
      case pioneers3d::eFontType::User: o << "User"; break;
      case pioneers3d::eFontType::CHAT_BG: o << "CHAT_BG"; break;
      case pioneers3d::eFontType::CHAT: o << "CHAT"; break;
      case pioneers3d::eFontType::FPS_COUNTER: o << "FPS_COUNTER"; break;
      case pioneers3d::eFontType::SMALL: o << "SMALL"; break;
      default: o << "Default"; break;
   }
   return o;
}

std::ostream &
operator<< ( std::ostream & o, pioneers3d::Font_t const & font )
{
   o << font.Type << "," << font.Font << "," << font.FileName << "," << font.Size << "," << font.Style << "," << font.AntiAlias << "," << font.Transparent;
   return o;
}

namespace pioneers3d {

irr::video::IVideoDriver*
Font_getVideoDriver( Font_t const & font )
{
    if ( !font.Font ) { return nullptr; }
    return font.Font->getVideoDriver();
}

irr::core::dimension2du
Font_getTextDimension( Font_t const & font, std::string const & txt )
{
    if ( !font.Font ) { return irr::core::dimension2du(0,0); }
    return font.Font->getDimension( irr::core::stringw( txt.c_str() ).c_str() );
}

glm::ivec2
Font_getTextSize( Font_t const & font, std::string const & txt )
{
   if ( !font.Font ) { return glm::ivec2(0,0); }
   irr::core::dimension2du const irrSize = font.Font->getDimension( irr::core::stringw( txt.c_str() ).c_str() );
   return glm::ivec2( int32_t( irrSize.Width ), int32_t( irrSize.Height ) );
}

Font_t
Font_create(
        irr::gui::IGUIEnvironment* env,
        std::string fileName,
        uint32_t fontSizePx,
        uint32_t fontStyle,
        eFontType fontType,
        bool aa,
        bool transparent )
{
   // Default: invalid font

   // assert( env );
   // assert( !fileName.empty() );
   // assert( de::hampe::common::os::FileSystemSTL::existFile( fileName ) == true && fileName.c_str() );

   if( !env )
   {
      std::cout << __FUNCTION__ << " Invalid pointer to irrlicht GUI env\n";
      return Font_t();
   }

   if( fileName.empty() )
   {
      std::cout << __FUNCTION__ << " Empty filename\n";
      return Font_t();
   }

   if( !de::hampe::os::FileSystemSTL::existFile( fileName ) )
   {
      std::cout << __FUNCTION__ << " Cannot open filename(" << fileName << ")\n";
      return Font_t();
   }

   try
   {
      Font_t font;
      font.Font = irr::gui::CGUITTFont::create( env, fileName.c_str(), fontSizePx, aa, transparent );
      font.Type = fontType;
      font.Size = fontSizePx;
      font.Style = fontStyle;
      font.AntiAlias = aa;
      font.Transparent = transparent;
      std::cout << __FUNCTION__ << "(" << fileName << ") :: Loaded font(" << font << ")\n";
      return font;
   }
   catch( ... )
   {
      std::cout << __FUNCTION__ << "(" << fileName << ") :: Got exception\n";
      return Font_t();
   }
}

void
Font_draw( Font_t const & font,
            std::string const & txt,
            glm::ivec2 const & pos,
            uint32_t color,
            uint32_t textAlign )
{
   //if ( !font ) { return; }
   if ( !font.Font ) { return; }
   glm::ivec2 txtSize = Font_getTextSize( font, txt );
   if ( txtSize.x < 1 || txtSize.y < 1 ) return;
   int32_t tx = pos.x; // top
   int32_t ty = pos.y; // left corner
   if ( textAlign & TextAlign::Center )
   {
      tx -= txtSize.x / 2;
   }
   else if ( textAlign & TextAlign::Right )
   {
      tx -= txtSize.x;
   }

   if ( textAlign & TextAlign::Middle )
   {
      ty += txtSize.y / 2;
   }
   else if ( textAlign & TextAlign::Baseline ) {
      // way more compilicated stuff -> approxxx
      ty += ( 4 * txtSize.y ) / 5;
   }
   else if ( textAlign & TextAlign::Bottom ) {
      ty += txtSize.y;
   }

   font.Font->draw( irr::core::stringw( txt.c_str() ), irr::core::recti( tx, ty, txtSize.x, txtSize.y ), color, false, false, nullptr );
}

#if 0
void Font_drawBig( irr::gui::IGUIFont* font, std::string txt, int32_t x, int32_t y, uint32_t color )
{
    Font_draw( font, txt, x-1, y-1, color );
    Font_draw( font, txt, x,   y-1, color );
    Font_draw( font, txt, x+1, y-1, color );
    Font_draw( font, txt, x-1, y,   color );
    Font_draw( font, txt, x,   y,   color );
    Font_draw( font, txt, x+1, y,   color );
    Font_draw( font, txt, x-1, y+1, color );
    Font_draw( font, txt, x,   y+1, color );
    Font_draw( font, txt, x+1, y+1, color );
}

void Font_drawShadow( irr::gui::IGUIFont* font, std::string txt, int32_t x, int32_t y, uint32_t color )
{
    if (!font) return;
    uint32_t shadowColor = 0xFF202020;
    uint32_t highlightColor = 0xFFFFFFFF;
    Font_draw( font, txt, x,   y-1, shadowColor );
    Font_draw( font, txt, x+1, y-1, shadowColor );
    Font_draw( font, txt, x,   y+1, highlightColor );
    Font_draw( font, txt, x+1, y+1, highlightColor );
    Font_draw( font, txt, x,   y,   color );
    Font_draw( font, txt, x+1, y,   color );

//    Font_draw( font, item.Text, tx-1, ty, color );
//    Font_draw( font, item.Text, tx+2, ty, bgColor );
//    Font_draw( font, item.Text, tx-1, ty+1, bgColor );
//    Font_draw( font, item.Text, tx,   ty+1, item.Color );
//    Font_draw( font, item.Text, tx+1, ty+1, item.Color );
//    Font_draw( font, item.Text, tx+2, ty+1, bgColor );
//    Font_draw( font, item.Text, tx, ty+2, bgColor );
//    Font_draw( font, item.Text, tx+1, ty+2, bgColor );
}
#endif

FontManager_t::FontManager_t()
{
   std::cout << "FontManager." << __FUNCTION__ << "\n";
}

FontManager_t::~FontManager_t()
{
   std::cout << "FontManager." << __FUNCTION__ << "\n";

   clear();
}

void
FontManager_t::init( irr::gui::IGUIEnvironment* env )
{
   std::cout << "FontManager." << __FUNCTION__ << "\n";
   if ( !env )
   {
      std::cout << "FontManager." << __FUNCTION__ << " :: Invalid pointer to irrlicht gui\n";
      return;
   }

   std::string const fontDir = "../../media/fonts/";
   add( env, eFontType::Default, fontDir + "DejaVuSansMono.ttf", 12, 0, true, true );
   add( env, eFontType::H1, fontDir + "Garton.ttf", 128, 0, true, true );
   add( env, eFontType::H2, fontDir + "DejaVuSansMono.ttf", 96, 0, true, true );
   add( env, eFontType::H3, fontDir + "DejaVuSansMono.ttf", 64, 0, true, true );
   add( env, eFontType::H4, fontDir + "FontAwesome.ttf", 48, 0, true, true );
   add( env, eFontType::H5, fontDir + "FontAwesome.ttf", 32, 0, true, true );
   add( env, eFontType::H6, fontDir + "FontAwesome.ttf", 24, 0, true, true );
   add( env, eFontType::User, fontDir + "DejaVuSansMono.ttf", 12, 0, true, true );
   add( env, eFontType::CHAT, fontDir + "Garton.ttf", 24, 0, true, false );
   add( env, eFontType::FPS_COUNTER, fontDir + "DejaVuSansMono.ttf", 32, 0, true, true );

   add( env, eFontType::AWESOME, fontDir + "FontAwesome.ttf", 64, 0, true, true );
}

void
FontManager_t::clear()
{
   std::cout << "FontManager." << __FUNCTION__ << "\n";

   for ( size_t i = 0; i < Fonts.size(); ++i )
   {
      Font_t & font = Fonts[ i ];
      if ( font.Font )
      {
         font.Font->drop(); // We owned it
      }
   }
   Fonts.clear();
}

void
FontManager_t::add( irr::gui::IGUIEnvironment* env, eFontType type, std::string const fileName, uint32_t sizePx, uint32_t style, bool aa, bool transparent )
{
   std::cout << "FontManager." << __FUNCTION__ << "(" << fileName << "):\n";
   Font_t font = Font_create( env, fileName, sizePx, style, type, aa, transparent );
   Fonts.emplace_back( std::move( font ) );
}

Font_t
FontManager_t::get( eFontType type ) const
{
   Font_t font;

   for ( size_t i = 0; i < Fonts.size(); ++i )
   {
      if ( Fonts[ i ].Type == type )
      {
         font = Fonts[ i ];
         break;
      }
   }
   return font;
}

} // end namespace pioneers3d
