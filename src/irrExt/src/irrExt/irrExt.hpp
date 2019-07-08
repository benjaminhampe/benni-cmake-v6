#ifndef DE_ALPHASONIC_IRRLICHT_BACKEND_HPP
#define DE_ALPHASONIC_IRRLICHT_BACKEND_HPP

#include <cstdlib>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <map>

#include <irrlicht.h>

#include <assert.h>

/// ext-lib: TinyXML2 ( header only )
//#include <tinyxml2.h>

/// ext-lib: GLM - OpenGL Math ( header only )
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include <irrExt/ustring.hpp>
#include <irrExt/CXMLWriterUTF8.hpp>
#include <irrExt/CGUITTFont.hpp>
#include <irrExt/CGUIWindow.hpp>

/// ext-lib: CLang format
typedef float float32_t;
typedef double float64_t;

template < typename T >
std::ostream &
operator<< ( std::ostream & o, irr::core::vector2d< T > const & v )
{
    o << v.X << "," << v.Y;
    return o;
}

template < typename T >
std::ostream &
operator<< ( std::ostream & o, irr::core::vector3d< T > const & v )
{
    o << v.X << "," << v.Y << "," << v.Z;
    return o;
}

inline std::ostream &
operator<< ( std::ostream & o, irr::video::SColor const & color )
{
    o << color.getRed() << "," << color.getGreen() << "," << color.getBlue() << "," << color.getAlpha();
    return o;
}

inline std::ostream &
operator<< ( std::ostream & o, irr::core::dimension2du const & d )
{
    o << d.Width << "," << d.Height;
    return o;
}

inline std::ostream &
operator<< ( std::ostream & o, irr::core::recti const & r )
{
    o << r.UpperLeftCorner.X << "," << r.UpperLeftCorner.Y << "," << r.getWidth() << "," << r.getHeight();
    return o;
}

inline std::ostream &
operator<< ( std::ostream & o, irr::video::S3DVertex const & v )
{
    o << v.Pos << "," << v.Normal << "," << v.Color << "," << v.TCoords;
    return o;
}

inline bool
equals( float32_t a, float32_t b, float32_t eps = 0.0001f )
{
    return irr::core::equals( a, b, eps );
}

inline float32_t
curveValue( float_t a, float_t b, float_t speed = 1.5f )
{
   if ( speed < 1.0f ) // a = 1,[11,13.8],   <- this is the computed value from last iteration and fed into here, or the start value at first call
        speed = 1.0f;  // b = 15,[15,15]     <- destination value is const for all calls
   float32_t ba = a - b;      // ba = -14,[-4,-1.2] ba/s = -4,[-1.2,-0.3]
   return b + ( ba / speed ); // b = 11,[13.8,14.7]
}

inline glm::vec3
curveVector( glm::vec3 a, glm::vec3 b, float_t speed = 1.5f )
{
   if ( speed < 1.0f ) // a = 1,[11,13.8],   <- this is the computed value from last iteration and fed into here, or the start value at first call
        speed = 1.0f;  // b = 15,[15,15]     <- destination value is const for all calls
   return b + ( ( a - b ) / speed ); // b = 11,[13.8,14.7] // ba = -14,[-4,-1.2] ba/s = -4,[-1.2,-0.3]
}

inline float32_t
convertToNormalized( int16_t value )
{
    // ( make all symmetric around +0.5f )
    if ( value == 0 )
        return 0.5f;
    else if ( value > 0 )
        return 0.5f + static_cast< float32_t >(value)/65534.0f;
    else
        return 0.5f + static_cast< float32_t >(value)/65536.0f;
}

inline irr::core::recti
mkRect( int32_t x, int32_t y, irr::core::dimension2du size )
{
    return irr::core::recti( irr::core::position2di( x, y ), size );
}

inline irr::core::recti
mkRect( int32_t x, int32_t y, int32_t w, int32_t h )
{
   w = glm::clamp( w, 0, 100000 );
   h = glm::clamp( h, 0, 100000 );
   return irr::core::recti( irr::core::position2di( x, y ), irr::core::dimension2du( uint32_t( w ), uint32_t( h ) ) );
}

inline irr::core::dimension2du
getTexSize( irr::video::ITexture* tex )
{
    if ( !tex ) return irr::core::dimension2du(0,0);
    return tex->getOriginalSize();
}

inline float32_t
getTexAspectRatio( irr::video::ITexture* tex )
{
    if ( !tex || tex->getOriginalSize().Height == 0 ) return 0.0f;
    return float32_t(tex->getOriginalSize().Width) / float32_t(tex->getOriginalSize().Height);
}

inline std::string
getTexNameA( irr::video::ITexture* tex )
{
    if ( !tex ) return "";
    return std::string( irr::core::stringc( tex->getName() ).c_str() );
}

inline std::wstring
getTexNameW( irr::video::ITexture* tex )
{
    if ( !tex ) return L"";
    return std::wstring( irr::core::stringw( tex->getName() ).c_str() );
}

inline uint32_t
getScreenWidth( irr::video::IVideoDriver* driver )
{
    if ( !driver ) return 0;
    return driver->getScreenSize().Width;
}

inline uint32_t
getScreenHeight( irr::video::IVideoDriver* driver )
{
    if ( !driver ) return 0;
    return driver->getScreenSize().Height;
}

inline void
saveTexture( irr::video::IVideoDriver* driver, irr::video::ITexture* tex, std::string const & fileName )
{
    if ( !driver ) return;
    irr::video::IImage* img = driver->createImage( tex, irr::core::position2di(0,0), tex->getOriginalSize() );
    if ( img )
    {
        driver->writeImageToFile( img, fileName.c_str() );
        img->drop();
    }
}

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

#include <glm/glm.hpp>

#ifdef GLM_COMPILER

inline irr::core::vector3df toIRR( glm::vec3 const & v )
{
    return irr::core::vector3df( v.x, v.y, v.z );
}

inline glm::vec3 toGLM( irr::core::vector3df const & v )
{
    return glm::vec3( v.X, v.Y, v.Z );
}

inline bool
equals( glm::vec2 a, glm::vec2 b, float32_t eps = 0.0001f )
{
    if ( equals( a.x, b.x, eps ) &&
         equals( a.y, b.y, eps ) ) return true;
    return false;
}

inline bool
equals( glm::vec3 a, glm::vec3 b, float32_t eps = 0.0001f )
{
    if ( equals( a.x, b.x, eps ) &&
         equals( a.y, b.y, eps ) &&
         equals( a.z, b.z, eps ) ) return true;
    return false;
}

inline glm::ivec2
getScreenSize( irr::video::IVideoDriver* driver )
{
    if ( !driver ) return glm::ivec2(0,0);
    return glm::ivec2( driver->getScreenSize().Width, driver->getScreenSize().Height );
}

inline glm::ivec3
getDesktopSize( irr::IrrlichtDevice * device )
{
    glm::ivec3 desktop( 0,0,0 );
    assert( device );
    if ( !device ) return desktop;
    desktop.x = device->getVideoModeList()->getDesktopResolution().Width;
    desktop.y = device->getVideoModeList()->getDesktopResolution().Height;
    desktop.z = device->getVideoModeList()->getDesktopDepth();
    return desktop;
}

inline glm::ivec3
getDesktopSize()
{
    glm::ivec3 desktop( 0,0,0 );
    irr::IrrlichtDevice * device = irr::createDevice( irr::video::EDT_NULL );
    desktop = getDesktopSize( device );
    device->drop();
    return desktop;
}

#endif

#endif // DE_ALPHASONIC_IRRLICHT_BACKEND_HPP
