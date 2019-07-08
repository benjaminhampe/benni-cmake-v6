#include "PolyLineCommon.hpp"

// TODO: add logger

uint32_t
Utils::varyColor( uint32_t color, int variance )
{
    int32_t r = ARGB_R( color );
    int32_t g = ARGB_G( color );
    int32_t b = ARGB_B( color );
    int32_t const a = ARGB_A( color );

    r = glm::clamp( r - variance / 2 + ( rand() % variance ), 0, 255 );
    g = glm::clamp( g - variance / 2 + ( rand() % variance ), 0, 255 );
    b = glm::clamp( b - variance / 2 + ( rand() % variance ), 0, 255 );
    return ARGB( r, g, b, a );
}

glm::vec2
Utils::rotate( glm::vec2 const AB, float32_t const phi )
{
    float32_t const s = sin( phi );
    float32_t const c = cos( phi );
    float32_t const x = AB.x * c - AB.y * s;
    float32_t const y = AB.x * s + AB.y * c;
    return { x, y };
}

glm::vec2
Utils::getNormal2D( glm::vec2 const m )
{
    glm::vec2 const n = glm::normalize( glm::vec2( m.y, -m.x ) );
#if 0
    std::cout << __FUNCTION__ << "(" << m << ") :: "
        << "phi_m(" << getVectorAngle360( m ) * irr::core::RADTODEG << "), "
        << "n(" << n << "), phi_n(" << getVectorAngle360( n ) * irr::core::RADTODEG << ")\n";
#endif
    return n;
}


float32_t
Utils::getAngle360( glm::vec2 const P )
{
    float32_t phi = atan2( P.y, P.x );
    if ( phi < 0.0f ) phi += ( 2.0f * irr::core::PI );
#if 0
    std::cout << __FUNCTION__ << "(" << P << ") = " << (phi * irr::core::RADTODEG) << " [deg]\n";
#endif
    assert( phi >= 0.0f && phi < irr::core::PI * 2.0f );
    return phi;
}

float32_t
Utils::getAngle360( glm::vec2 const AB, glm::vec2 const CD )
{
    float32_t const phiAB = atan2( AB.y, AB.x );
    float32_t const phiCD = atan2( CD.y, CD.x );
    float32_t phi = phiCD - phiAB;

#if 0
    std::cout << __FUNCTION__ << "{ AB(" << AB << "), CD(" << CD << ") } :: ";
    std::cout << "phiAB(" << Utils::toDegrees( phiAB ) << "), ";
    std::cout << "phiCD(" << Utils::toDegrees( phiCD ) << "), ";
    std::cout << "phi = " << Utils::toDegrees( phi ) << ")";
#endif

    if ( phi < 0.0f )
    {
        phi += Utils::TWO_PI;
#if 0
        std::cout << " -> phi2(" << phi << ")";
#endif
    }
 #if 0
    std::cout << "\n";
#endif
    return phi;
}

float32_t
Utils::getAngleABC( glm::vec2 const A, glm::vec2 const B, glm::vec2 const C )
{
#if 0
    std::cout << __FUNCTION__ << "{ A(" << A << "), B(" << B << "), C(" << C << ") } :: ";
#endif
    return getAngle360( B - A, C - B );
}
