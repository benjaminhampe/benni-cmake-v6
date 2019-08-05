#ifndef IRREXT_TYPES_HPP
#define IRREXT_TYPES_HPP

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

#include <irrExt/ustring.hpp>
#include <irrExt/CXMLWriterUTF8.hpp>
#include <irrExt/CGUITTFont.hpp>
#include <irrExt/CGUIWindow.hpp>

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

/// ext-lib: CLang format
typedef float float32_t;
typedef double float64_t;

namespace irrExt {
	
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

} // end namespace irrExt





#endif // DE_ALPHASONIC_IRRLICHT_BACKEND_HPP
