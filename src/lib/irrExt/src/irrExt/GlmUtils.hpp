#ifndef IRREXT_GLMUTILS_HPP
#define IRREXT_GLMUTILS_HPP

#include <irrExt/Types.hpp> // uses equals, headers, etc...

/// ext-lib: GLM - OpenGL Math ( header only )
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#ifdef GLM_COMPILER

#include <ostream>

// ------------------------------------------------------------
// --- Print GLM Types --- For my developer convenience
// ------------------------------------------------------------

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec1<T,P> const & v )
{
    out << v.x;
    return out;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec2<T,P> const & v )
{
    out << v.x << "," << v.y;
    return out;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec3<T,P> const & v )
{
    out << v.x << "," << v.y << "," << v.z;
    return out;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec4<T,P> const & v )
{
    out << v.x << "," << v.y << "," << v.z << "," << v.w;
    return out;
}

namespace irrExt {

inline irr::core::recti
mkRect( int32_t x, int32_t y, int32_t w, int32_t h )
{
   w = glm::clamp( w, 0, 100000 );
   h = glm::clamp( h, 0, 100000 );
   return irr::core::recti( irr::core::position2di( x, y ), irr::core::dimension2du( uint32_t( w ), uint32_t( h ) ) );
}


inline glm::vec3
curveVector( glm::vec3 a, glm::vec3 b, float_t speed = 1.5f )
{
   if ( speed < 1.0f ) // a = 1,[11,13.8],   <- this is the computed value from last iteration and fed into here, or the start value at first call
        speed = 1.0f;  // b = 15,[15,15]     <- destination value is const for all calls
   return b + ( ( a - b ) / speed ); // b = 11,[13.8,14.7] // ba = -14,[-4,-1.2] ba/s = -4,[-1.2,-0.3]
}

inline irr::core::vector3df toIRR( glm::vec3 const & v )
{
    return irr::core::vector3df( v.x, v.y, v.z );
}

inline glm::vec3 toGLM( irr::core::vector3df const & v )
{
    return glm::vec3( v.X, v.Y, v.Z );
}

inline bool
equals( glm::vec2 const & a, glm::vec2 const & b, float32_t eps = 0.0001f )
{
    if ( equals( a.x, b.x, eps ) &&
         equals( a.y, b.y, eps ) ) return true;
    return false;
}

inline bool
equals( glm::vec3 const & a, glm::vec3 const & b, float32_t eps = 0.0001f )
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

} // end namespace irrExt

#endif // IRREXT_GLMUTILS_HPP
