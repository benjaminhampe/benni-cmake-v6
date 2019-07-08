//////////////////////////////////////////////////////////////////////
/// @file Math.hpp
/// @brief Definition of class Math
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

#ifndef DE_HAMPE_SVG_MATH_HPP
#define DE_HAMPE_SVG_MATH_HPP

#include <de/hampe/logging/Logger.hpp>

namespace de {
namespace hampe {
namespace svg {

class Math
{
    CREATE_STATIC_LOGGER("de.hampe.svg.Math" )

public:
    #ifdef PI32 // make sure we don't collide with a define
    #undef PI32
    #endif
	
    /// @brief 32-bit constant for Pi ( aka 180 degrees )
    constexpr static float32_t const PI32 = 3.14159265359f;

    /// @brief 32-bit constant for two Pi ( aka 360 degrees, aka the full circumference of the unit circle ).
    constexpr static float32_t const TWO_PI32 = float32_t( 2.0 * 3.1415926535897932384626433832795028841971693993751 );

    /// @brief 32-bit constant for converting from degrees to radians
    constexpr static float32_t const DEGTORAD32 = float32_t( 3.1415926535897932384626433832795028841971693993751 / 180.0 );

    /// @brief 32-bit constant for converting from radians to degrees (formally known as GRAD_PI)
    constexpr static float32_t const RADTODEG32 = float32_t( 180.0 / 3.1415926535897932384626433832795028841971693993751 );

    /// @brief 32-bit constant for reciprocal (1 / Pi).
    constexpr static float32_t const RECIPROCAL_PI32 = float32_t( 1.0 / 3.1415926535897932384626433832795028841971693993751 );

    /// @brief 32-bit constant for half pi (Pi / 2).
    constexpr static float32_t const HALF_PI32 = float32_t( 3.1415926535897932384626433832795028841971693993751 / 2.0 );

    /// @brief Compile time helper
    constexpr static float32_t toDegrees ( float32_t const radians ) { return radians * RADTODEG32; }

    /// @brief Compile time helper
    constexpr static float32_t toRadians ( float32_t const degrees ) { return degrees * DEGTORAD32; }

    /// @brief Core Math Function I. used for PolyLines
    /// version 2.1.0, working fine,
    /// @remarks Rotates always 90 deg clock-wise (cw), e.g. +x(1,0) -> -y(0,-1) -> -x(-1,0) -> +y(0,1) -> +x again.
    static glm::vec2
    getNormal2D( glm::vec2 const AB );

    /// @brief Core Math Function II. used for PolyLines
    /// version 6.0, working fine
    /// @remarks Returns the difference angle of two vectors AB and BC ( both share point B and intersect at B )
    /// by computing difference: atan2( BC ) - atan2( AB ).
    /// Symmetric to zero and in radians
    /// Left turn when phi is lower or equal zero (phi <= 0) == left, (phi > 0) == right
    /// Right turn when phi is greater than zero
    /// @param[in] A The start point of vector AB
    /// @param[in] B The end point of vector AB and the start point of vector BC
    /// @param[in] C The end point of vector BC
    /// @return Result angle phi in range [-PI,+PI]
    static float32_t
    getAngleABC( glm::vec2 const A, glm::vec2 const B, glm::vec2 const C );

    /// @brief Core Math Function III. used for PolyLines
    /// @version 1.0, testing
    /// @remarks Accumulates all segment lengths of the given point-list
    static float64_t
    getAccumLength( std::vector< glm::vec2 > const & points );

#if 0
    /// @brief Computes the full 4-quadrant angle of vector AB and x-axis (1,0) using atan2( AB ).
    /// @details Since we want to determine the direction of the polyline we cannot rely on the simpler cos(AB) = dotP(AB,{1,0})/abs(AB).
    /// @return Result will be always in range [-0,359.999], beware of the negative zero!
    static float32_t
    getAngle360( glm::vec2 const P );

    /// @brief Computes the full 4-quadrant angle of vector AB and vector CD using atan2.
    /// @details Since we want to determine the direction of the polyline we cannot rely on the simpler cos(AB,CD) = dotP(AB,CD)/( abs(AB)*abs(CD) ).
    /// @return Result will be always in range [-0,359.999], beware of the negative zero!
    static float32_t
    getAngle360( glm::vec2 const AB, glm::vec2 const CD );

    /// @brief Rotate
    static glm::vec2
    rotate( glm::vec2 const AB, float32_t const phi );
#endif
    
};

} // end namespace svg
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_MATH_SVG_HPP
