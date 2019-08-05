//////////////////////////////////////////////////////////////////////
/// @file Math.cpp
/// @brief Implementation of class Math
/// @author Copyright (c) Benjamin Hampe <benjaminhampe@gmx.de>
///
///	The author grants permission of free use, free distribution and
/// free usage for commercial applications even for modified sources.
///
///	What is not allowed is to remove the original authors name or claim
/// any intellectual rights. This original code was written in my spare time
/// and is distributed publicly as free software to anyone over GitLab and GitHub.
///
//////////////////////////////////////////////////////////////////////

#include <de/hampe/svg/Math.hpp>

namespace de {
namespace hampe {
namespace svg {

glm::vec2
Math::getNormal2D( glm::vec2 const m )
{
    return glm::normalize( glm::vec2( m.y, -m.x ) );
}

float32_t
Math::getAngleABC( glm::vec2 const A, glm::vec2 const B, glm::vec2 const C )
{
    glm::vec2 const AB = B - A;
    glm::vec2 const BC = C - B;
    float64_t const phiAB = atan2( float64_t( AB.y ), float64_t( AB.x ) );
    float64_t const phiBC = atan2( float64_t( BC.y ), float64_t( BC.x ) );
    return static_cast< float32_t >( phiBC - phiAB );
}

float64_t
Math::getAccumLength( std::vector< glm::vec2 > const & points )
{
    float64_t accumLength = 0.0;

    if ( points.size() < 2 ) return accumLength;

    glm::vec2 A = points[ 0 ];
    glm::vec2 B;

    for ( size_t i = 1; i < points.size(); ++i )
    {
        B = points[ i ];
        accumLength += glm::length( B - A );
        A = B;
    }

    return accumLength;
}

#if 0

float32_t
Math::getAngle360( glm::vec2 const P )
{
    float32_t phi = static_cast< float32_t >( atan2( P.y, P.x ) );
    if ( phi < 0.0f )
    {
        phi += Math::TWO_PI;
    }
    return phi;
}

float32_t
Math::getAngle360( glm::vec2 const AB, glm::vec2 const CD )
{
    float32_t const phiAB = static_cast< float32_t >( atan2( AB.y, AB.x ) );
    float32_t const phiCD = static_cast< float32_t >( atan2( CD.y, CD.x ) );
    float32_t phi = phiCD - phiAB;
    if ( phi < 0.0f )
    {
        phi += Math::TWO_PI;
    }
    return phi;
}

glm::vec2
Math::rotate( glm::vec2 const AB, float32_t const phi )
{
    float32_t const s = static_cast< float32_t >( sin( phi ) );
    float32_t const c = static_cast< float32_t >( cos( phi ) );
    float32_t const x = AB.x * c - AB.y * s;
    float32_t const y = AB.x * s + AB.y * c;
    return { x, y };
}

#endif

} // end namespace svg
} // end namespace hampe
} // end namespace de
