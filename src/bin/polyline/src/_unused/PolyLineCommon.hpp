#ifndef POLYLINECOMPILECONFIG_HPP
#define POLYLINECOMPILECONFIG_HPP

#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef float float32_t;
typedef double float64_t;

#include <iostream>
#include <AlphaSonic/IntersectionTool.hpp>

// Unit contants in [degrees]
#define CONST_POLYLINE_JOIN_ANGLE_MIN 10.0f
#define CONST_POLYLINE_JOIN_ANGLE_STEP 10.0f
#define POLYLINE_COLOR_LINE_JOIN_BEVEL 0xFFFFC810
#define POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL 0xFFFF8010

//#ifndef USE_POLYLINE_DEBUG_LOGGING
//#define USE_POLYLINE_DEBUG_LOGGING
//#endif
//#define USE_POLYLINE_VECTOR_ADDITION
// #define USE_GET_ANGLE_ABC_DEBUG_LOGGING

//! Rounding error constant often used when comparing f32 values.

class Utils
{
public:
    /// @brief RoundingError
    constexpr static float32_t const ROUNDING_ERROR_f32 = 0.000001f;

    #ifdef PI // make sure we don't collide with a define
    #undef PI
    #endif

    /// @brief 32-bit constant for Pi ( aka 180 degrees )
    constexpr static float32_t const PI = 3.14159265359f;

    /// @brief 32-bit constant for two Pi ( aka 360 degrees, aka the full circumference of the unit circle ).
    constexpr static float32_t const TWO_PI = float32_t( 2.0 * 3.1415926535897932384626433832795028841971693993751 );

    /// @brief 32-bit constant for converting from degrees to radians
    constexpr static float32_t const DEGTORAD = float32_t( 3.1415926535897932384626433832795028841971693993751 / 180.0 );

    /// @brief 32-bit constant for converting from radians to degrees (formally known as GRAD_PI)
    constexpr static float32_t const RADTODEG = float32_t( 180.0 / 3.1415926535897932384626433832795028841971693993751 );

    /// @brief 32-bit constant for reciprocal (1 / Pi).
    constexpr static float32_t const RECIPROCAL_PI = float32_t( 1.0 / 3.1415926535897932384626433832795028841971693993751 );

    /// @brief 32-bit constant for half pi (Pi / 2).
    constexpr static float32_t const HALF_PI = float32_t( 3.1415926535897932384626433832795028841971693993751 / 2.0 );

    /// @brief Compile time helper
    constexpr static float32_t toDegrees ( float32_t radians ) { return radians * RADTODEG; }

    /// @brief Compile time helper
    constexpr static float32_t toRadians ( float32_t degrees ) { return degrees * DEGTORAD; }

    /// @brief Creates a pseudo random color relative to given color that add an offset to each component in range [-variance/2,+variance/2].
    static uint32_t
    varyColor( uint32_t color, int variance );

    /// @brief Core Math Function I. used for PolyLines,
    /// version 2.1.0, working fine,
    /// @remarks Still, the direction of the rotation should be reversed,
    /// but that changes everything that was called L (left) to R (right) and vice versa,
    /// and would change the winding order of the generated triangles for sure,
    /// which would make them invisible without adaption of everything else, beware of any change!
    static glm::vec2
    getNormal2D( glm::vec2 const AB );

    /// @brief Computes the full 4-quadrant angle of vector AB and x-axis (1,0) using atan2( AB ).
    /// @details Since we want to determine the direction of the polyline we cannot rely on the simpler cos(AB) = dotP(AB,{1,0})/abs(AB).
    /// @return Result will be always in range [-0,359.999], beware of the negative zero!
    static float32_t
    getAngle360( glm::vec2 const P );


    static float32_t
    getAngle360( glm::vec2 const AB, glm::vec2 const CD );

    /// @brief Returns the difference angle of two rays AB and BC ( both rays share point B and intersect at B )
    /// by computing difference: atan2( BC ) - atan2( AB ).
    /// @param[in] A The start point of ray AB
    /// @param[in] B The end point of ray AB and the start point of ray BC
    /// @param[in] C The end point of ray BC
    /// @return Result will be always in range [-0,359.999], beware of the negative zero!
    static float32_t
    getAngleABC( glm::vec2 const A, glm::vec2 const B, glm::vec2 const C );

    /// @brief Rotate
    static glm::vec2
    rotate( glm::vec2 const AB, float32_t const phi );
};

class UtilsTest
{
public:
    static void test_getAngleABC_1()
    {
        glm::vec2 A( 0, 0 );
        glm::vec2 B( 1, 0 );
        glm::vec2 C( 1, 1 );
        float32_t angle = Utils::getAngleABC( A, B, C );
    }

    static void test_getAngleABC_2()
    {
        glm::vec2 A( 0, 0 );
        glm::vec2 B( -1, 0 );
        glm::vec2 C( -1, 1 );
        float32_t angle = Utils::getAngleABC( A, B, C );
    }

    static void test_ATAN2()
    {
        std::cout << "\n[" << __FUNCTION__ << "]:\n\n";

        float32_t phiT1 = -90.0f + irr::core::RADTODEG * atan2( 1, 0 );
        float32_t phiT2 = -90.0f + irr::core::RADTODEG * atan2( 1, 1 );
        float32_t phiT3 = -90.0f + irr::core::RADTODEG * atan2( 0, 1 );
        float32_t phiT4 = -90.0f + irr::core::RADTODEG * atan2( -1, 1 );
        float32_t phiT5 = -90.0f + irr::core::RADTODEG * atan2( -1, 0 );
        float32_t phiT6 = -90.0f + irr::core::RADTODEG * atan2( -1, -1 );
        std::cout << "phiT1 = " << phiT1 << "\n";
        std::cout << "phiT2 = " << phiT2 << "\n";
        std::cout << "phiT3 = " << phiT3 << "\n";
        std::cout << "phiT4 = " << phiT4 << "\n";
        std::cout << "phiT5 = " << phiT5 << "\n";
        std::cout << "phiT6 = " << phiT6 << "\n";
    }

    static void testAll()
    {
        test_ATAN2();
        test_getAngleABC_1();
        test_getAngleABC_2();
    }

};

#endif // POLYLINECOMPILECONFIG_HPP
