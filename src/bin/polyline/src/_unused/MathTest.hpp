//////////////////////////////////////////////////////////////////////
/// @file MathTest.hpp
/// @brief Definition of class MathTest
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

#ifndef DE_HAMPE_MATH_MATHTEST_HPP
#define DE_HAMPE_MATH_MATHTEST_HPP

#include <de/hampe/math/Math.hpp>

namespace de {
namespace hampe {
namespace math {

class MathTest
{
public:
    static void test_getNormal2D()
    {
        glm::vec2 A( 1, 0 );
        glm::vec2 B( 0, 1 );
        glm::vec2 C( -1, 0 );
        glm::vec2 nA = Math::getNormal2D( A );
        glm::vec2 nB = Math::getNormal2D( B );
        glm::vec2 nC = Math::getNormal2D( C );


    }

    static void test_getAngleABC_1()
    {
        glm::vec2 A( 0, 0 );
        glm::vec2 B( 1, 0 );
        glm::vec2 C( 1, 1 );
        float32_t angle = Math::getAngleABC( A, B, C );
    }

    static void test_getAngleABC_2()
    {
        glm::vec2 A( 0, 0 );
        glm::vec2 B( -1, 0 );
        glm::vec2 C( -1, 1 );
        float32_t angle = Math::getAngleABC( A, B, C );
    }

    static void test_ATAN2()
    {
        std::cout << "\n[" << __FUNCTION__ << "]:\n\n";

        float32_t phiT1 = -90.0f + Math::RADTODEG32 * atan2( 1, 0 );
        float32_t phiT2 = -90.0f + Math::RADTODEG32 * atan2( 1, 1 );
        float32_t phiT3 = -90.0f + Math::RADTODEG32 * atan2( 0, 1 );
        float32_t phiT4 = -90.0f + Math::RADTODEG32 * atan2( -1, 1 );
        float32_t phiT5 = -90.0f + Math::RADTODEG32 * atan2( -1, 0 );
        float32_t phiT6 = -90.0f + Math::RADTODEG32 * atan2( -1, -1 );
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

} // end namespace math
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_MATH_MATHTEST_HPP
