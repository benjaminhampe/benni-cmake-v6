#ifndef FONTTOOLS_HPP
#define FONTTOOLS_HPP

#include <irrExt/Types.hpp>
#include <irrExt/GlmUtils.hpp>

namespace irrExt {

/*
inline irr::gui::CGUITTFont*
createTTFont( irr::gui::IGUIEnvironment* env, std::string const & fileName, uint32_t pxSize, bool aa = true, bool transparency = true )
{
    irr::gui::CGUITTFont* font = irr::gui::CGUITTFont::create( env, fileName.c_str(), pxSize, aa, transparency );
    return font;
}

inline void
addFont( irr::gui::IGUIEnvironment* env, std::string const & fileName, uint32_t pxSize, bool aa = true, bool transparency = true )
{
    irr::gui::CGUITTFont* font = irr::gui::CGUITTFont::create( env, fileName.c_str(), pxSize, aa, transparency );
    if ( !font )
    {
        std::cout << "[Error] " << __FUNCTION__ << " :: Cannot create and set font from file (" << fileName << ")\n";
        return;
    }
    env->getSkin()->setFont( font, irr::gui::EGDF_DEFAULT );
}


*/
inline irr::gui::IGUIImage*
createGUIImage( irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent, irr::core::recti const & pos, irr::video::ITexture* tex  )
{
    std::cout << __FUNCTION__ << "(" << pos << ")\n";
    assert( env );
//    if ( !env )
//    {
//        std::cout << __FUNCTION__ << " :: Invalid pointer to IGUIEnvironment\n";
//        return nullptr;
//    }
    if ( !parent )
    {
        parent = env->getRootGUIElement();
    }

    //bool bPreserveAspect = true;
    int32_t x = pos.UpperLeftCorner.X;
    int32_t y = pos.UpperLeftCorner.Y;
    int32_t w = pos.getWidth();
    int32_t h = pos.getHeight();

    float32_t aspect = getTexAspectRatio( tex );
    if ( aspect > 0.0f )
    {
        irr::core::dimension2du size = getTexSize( tex );

        if ( aspect > 1.0f ) // w > h
        {
            float32_t fScale = float32_t(pos.getWidth()) / float32_t(size.Width);
            h = irr::core::round32( fScale * size.Height );
            y = pos.UpperLeftCorner.Y + (pos.getHeight() - h) / 2;
        }
        else
        {

        }

    }

    irr::gui::IGUIImage* element = env->addImage( mkRect( x, y, w, h ), parent, -1, getTexNameW(tex).c_str(), true );
    element->setScaleImage( true );
    element->setImage( tex );
    return element;
}

irr::core::dimension2du
Font_getPixelSize( irr::gui::IGUIFont* font, std::string txt );

irr::gui::CGUITTFont*
Font_create(
        irr::gui::IGUIEnvironment* env,
        std::string fileName,
        int32_t pxSize,
        bool aa,
        bool transparent = true );

void
Font_draw(
        irr::gui::IGUIFont* font,
        std::string txt,
        int32_t x,
        int32_t y,
        uint32_t color );

void
Font_draw(
        irr::gui::IGUIFont* font,
        std::string txt,
        irr::core::recti const & pos,
        uint32_t color );

struct Text_t
{
    std::string Text;
    uint32_t Color;
    irr::gui::IGUIFont * Font;
    Text_t() : Text( "X" ), Color( 0xFFFFFFFF ), Font( nullptr ) {}
    Text_t( std::string txt, uint32_t color, irr::gui::IGUIFont* font ) : Text( txt ), Color( color ), Font( font ) {}
    bool canDraw() const
    {
        if ( !Font ) return false;
        if ( Text.empty() ) return false;
        return true;
    }
};

void Text_draw( Text_t * txt, int32_t x, int32_t y );
void Text_draw( Text_t * txt, irr::core::recti const & pos );

} // end namespace irrExt

#endif // FONTTOOLS_HPP
