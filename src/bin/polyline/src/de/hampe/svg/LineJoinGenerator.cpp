//////////////////////////////////////////////////////////////////////
/// @file   LineJoinGenerator.cpp
/// @brief  SVG LineJoinGenerator
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

#include "LineJoinGenerator.hpp"

#include <de/hampe/svg/Math.hpp>

#define CONST_POLYLINEJOIN_ANGLE_MIN 10.0f
#define CONST_POLYLINEJOIN_ANGLE_STEP 10.0f
#define CONST_POLYLINEJOIN_COLOR_1 0xFFFFC810
#define CONST_POLYLINEJOIN_COLOR_2 0xFFFF8010

namespace de {
namespace hampe {
namespace svg {

// [Old]: Line-joins for pure triangle based meshes ( only for debugging )

/// BEVEL
void addBeveledLineJoin( PrimitiveRenderer2D & dc, glm::vec2 const C, glm::vec2 const A, glm::vec2 const B )
{
    // Add always the bevel triangle (B,CR,BR)
    dc.addTriangle2D( C, A, B, CONST_POLYLINEJOIN_COLOR_1 );

    // Add Double Bevel:
    // compute and check if another triangle fits
    glm::vec2 const AB  { B - A         };
    glm::vec2 const MAB { A + AB * 0.5f };
    glm::vec2 const CM  { MAB - C       };

    // Add Double Bevel triangle only if triangle is small enough
    if ( 2.0f * glm::length( CM ) < glm::length( AB ) )
    {
        dc.addTriangle2D( A, MAB + CM, B, CONST_POLYLINEJOIN_COLOR_2 );
    }
}

/// ROUND
void addRoundLineJoin( PrimitiveRenderer2D & dc, glm::vec2 const M, glm::vec2 const L, glm::vec2 const R, float32_t const angleLMR )
{
    float32_t const join_step = CONST_POLYLINEJOIN_ANGLE_STEP;
    uint32_t const steps = uint32_t( std::abs( angleLMR / join_step ) ); // dirty floor cast
    float32_t const phi = angleLMR / float32_t( steps );
    std::cout << __FUNCTION__ << " :: M(" << M << "), L(" << L << "), R(" << R << "), angleLMR(" << angleLMR << ")"
              << ", join-step(" << join_step << "), steps(" << steps << ") phi(" << phi << ")\n";

    if ( phi <= 1.5f * CONST_POLYLINEJOIN_ANGLE_STEP )
    {
        std::cout << __FUNCTION__ << " :: Computed phi(" << phi << ") is too small for round-join, fallback to bevel-join.\n";
        addBeveledLineJoin( dc, M, L, R );
        return;
    }

    if ( steps < 3 )
    {
        std::cout << __FUNCTION__ << " :: Too few steps(" << steps << ") for round-join, fallback to bevel-join.\n";
        addBeveledLineJoin( dc, M, L, R );
        return;
    }

    dc.addTriangle2D( M, L, R, 0xFFFF00FF ); // debug triangle

    // construct LookUpTable
#if 0
    float32_t rhoL = Utils::getAngle360( L - M );
    float32_t rhoR = Utils::getAngle360( R - M );
    glm::vec2 n = L - M;

    std::cout << "n(" << n << "), rhoL(" << Utils::toDegrees(rhoL) << "), rhoR(" << Utils::toDegrees(rhoR) << ")\n";

    dc.addLine2D( M, L, 0xFFFFFF00 );
    dc.addLine2D( M, R, 0xFFFF8F00 );

    glm::vec2 u = L;
    glm::vec2 v = R;


    // turn left
    if ( rhoR < rhoL )
    {
        float32_t angle = rhoL - angle_step;

        for ( uint32_t i = 1; i < steps-1; ++i )
        {
            v = M + Utils::rotate( n, angle );

            dc.addLine2D( M, v, randomColor(), randomColor() );

            std::cout << "Step[" << i << "] angle(" << Utils::toDegrees(angle) << ") v = " << v << "\n";

            angle -= angle_step;
        }
    }
    else
    {
        float32_t angle = rhoL + angle_step;

        for ( uint32_t i = 1; i < steps-1; ++i )
        {
            v = M + Utils::rotate( n, angle );

            dc.addLine2D( M, v, randomColor(), randomColor() );

            std::cout << "Step[" << i << "] angle(" << Utils::toDegrees(angle) << ") v = " << v << "\n";

            angle += angle_step;
        }

    }
#endif
    /*

        float32_t sin_angle = sin( angle );
        float32_t cos_angle = cos( angle );
        P2.x = n.x * cos_angle - n.y * sin_angle;
        P2.y = n.x * sin_angle + n.y * cos_angle;
        std::cout << "M(" << M << "), L(" << L << "), R(" << R << "), n(" << n << ")\n";
        std::cout << "angle_start(" << angle_start << " = " << ( irr::core::RADTODEG * angle_start ) << ")\n";
        std::cout << "angle_step(" << angle_step << " = " << ( irr::core::RADTODEG * angle_step ) << ")\n";
        std::cout << "angle(" << angle << " = " << ( irr::core::RADTODEG * angle ) << ")\n";
        std::cout << "P1(" << P1 << "), P2(" << P2 << ")\n";

        for ( uint32_t i = 0; i < steps - 2; ++i )
        {
            dc.addTriangle2D( M, P1, P2, colorA );

            P1 = P2;
            angle += angle_step;

            sin_angle = sin( angle );
            cos_angle = sin( angle );
            P2.x = n.x * cos_angle - n.y * sin_angle;
            P2.y = n.x * sin_angle + n.y * cos_angle;
        }

        P1 = P2;
        P2 = R;
        dc.addTriangle2D( M, P1, P2, colorB );
    */
}

} // end namespace svg
} // end namespace hampe
} // end namespace de



#if 0

// [Old]: Line-joins for pure triangle based meshes ( only for debugging )

/// BEVEL
void addBeveledLineJoin( PrimitiveRenderer2D & dc, glm::vec2 const C, glm::vec2 const A, glm::vec2 const B )
{
    // Add always the bevel triangle (B,CR,BR)
    dc.addTriangle( C, A, B, CONST_POLYLINEJOIN_COLOR_1 );

    // Add Double Bevel:
    // compute and check if another triangle fits
    glm::vec2 const AB  { B - A         };
    glm::vec2 const MAB { A + AB * 0.5f };
    glm::vec2 const CM  { MAB - C       };

    // Add Double Bevel triangle only if triangle is small enough
    if ( 2.0f * glm::length( CM ) < glm::length( AB ) )
    {
        dc.addTriangle( A, MAB + CM, B, CONST_POLYLINEJOIN_COLOR_2 );
    }
}

/// ROUND
void addRoundLineJoin( PrimitiveRenderer2D & dc, glm::vec2 const M, glm::vec2 const L, glm::vec2 const R, float32_t const angleLMR )
{
    float32_t const join_step = CONST_POLYLINEJOIN_ANGLE_STEP;
    uint32_t const steps = uint32_t( std::abs( angleLMR / join_step ) ); // dirty floor cast
    float32_t const phi = angleLMR / float32_t( steps );
    std::cout << __FUNCTION__ << " :: M(" << M << "), L(" << L << "), R(" << R << "), angleLMR(" << angleLMR << ")"
              << ", join-step(" << join_step << "), steps(" << steps << ") phi(" << phi << ")\n";

    if ( phi <= 1.5f * CONST_POLYLINEJOIN_ANGLE_STEP )
    {
        std::cout << __FUNCTION__ << " :: Computed phi(" << phi << ") is too small for round-join, fallback to bevel-join.\n";
        addBeveledLineJoin( dc, M, L, R );
        return;
    }

    if ( steps < 3 )
    {
        std::cout << __FUNCTION__ << " :: Too few steps(" << steps << ") for round-join, fallback to bevel-join.\n";
        addBeveledLineJoin( dc, M, L, R );
        return;
    }

    dc.addTriangle( M, L, R, 0xFFFF00FF ); // debug triangle

    // construct LookUpTable
#if 0
    float32_t rhoL = Utils::getAngle360( L - M );
    float32_t rhoR = Utils::getAngle360( R - M );
    glm::vec2 n = L - M;

    std::cout << "n(" << n << "), rhoL(" << Utils::toDegrees(rhoL) << "), rhoR(" << Utils::toDegrees(rhoR) << ")\n";

    dc.addLine2D( M, L, 0xFFFFFF00 );
    dc.addLine2D( M, R, 0xFFFF8F00 );

    glm::vec2 u = L;
    glm::vec2 v = R;


    // turn left
    if ( rhoR < rhoL )
    {
        float32_t angle = rhoL - angle_step;

        for ( uint32_t i = 1; i < steps-1; ++i )
        {
            v = M + Utils::rotate( n, angle );

            dc.addLine2D( M, v, randomColor(), randomColor() );

            std::cout << "Step[" << i << "] angle(" << Utils::toDegrees(angle) << ") v = " << v << "\n";

            angle -= angle_step;
        }
    }
    else
    {
        float32_t angle = rhoL + angle_step;

        for ( uint32_t i = 1; i < steps-1; ++i )
        {
            v = M + Utils::rotate( n, angle );

            dc.addLine2D( M, v, randomColor(), randomColor() );

            std::cout << "Step[" << i << "] angle(" << Utils::toDegrees(angle) << ") v = " << v << "\n";

            angle += angle_step;
        }

    }
#endif
    /*

        float32_t sin_angle = sin( angle );
        float32_t cos_angle = cos( angle );
        P2.x = n.x * cos_angle - n.y * sin_angle;
        P2.y = n.x * sin_angle + n.y * cos_angle;
        std::cout << "M(" << M << "), L(" << L << "), R(" << R << "), n(" << n << ")\n";
        std::cout << "angle_start(" << angle_start << " = " << ( irr::core::RADTODEG * angle_start ) << ")\n";
        std::cout << "angle_step(" << angle_step << " = " << ( irr::core::RADTODEG * angle_step ) << ")\n";
        std::cout << "angle(" << angle << " = " << ( irr::core::RADTODEG * angle ) << ")\n";
        std::cout << "P1(" << P1 << "), P2(" << P2 << ")\n";

        for ( uint32_t i = 0; i < steps - 2; ++i )
        {
            dc.addTriangle2D( M, P1, P2, colorA );

            P1 = P2;
            angle += angle_step;

            sin_angle = sin( angle );
            cos_angle = sin( angle );
            P2.x = n.x * cos_angle - n.y * sin_angle;
            P2.y = n.x * sin_angle + n.y * cos_angle;
        }

        P1 = P2;
        P2 = R;
        dc.addTriangle2D( M, P1, P2, colorB );
    */
}

/// BEVEL
void
addBeveledLineJoin(
        PrimitiveRenderer2D & dc,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        uint32_t const colorA,
        uint32_t const colorB )
{
    // Add always the bevel triangle (B,CR,BR)
    dc.addTriangle( C, A, B, POLYLINE_COLOR_LINE_JOIN_BEVEL );

    // Add Double Bevel:
    // compute and check if another triangle fits
    glm::vec2 const AB  { B - A         };
    glm::vec2 const M   { A + AB * 0.5f };
    glm::vec2 const CM  { M - C         };

    // Add Double Bevel triangle only if triangle is small enough
    if ( 2.0f * glm::length( CM ) < glm::length( AB ) )
    {
        dc.addTriangle( A, M + CM, B, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
}

void
addRoundLineJoin(
        PrimitiveRenderer & dc,
        glm::vec2 const M,
        glm::vec2 const L,
        glm::vec2 const R,
        float32_t const phi,
        uint32_t const colorA,
        uint32_t const colorB )
{
    std::cout << __FUNCTION__ << "( M{" << M << "}, L{" << L << "}, R{" << R << "}, phi{" << phi << "} ):\n";

    if ( phi <= 1.5f * CONST_POLYLINE_JOIN_ANGLE_STEP )
    {
        std::cout << "Phi(" << phi << ") is too small for round-join, fallback to bevel-join.\n";
        addBeveledLineJoin( dc, M, L, R, colorA, colorB );
        return;
    }

    // construct LookUpTable

    float32_t const angle_step = CONST_POLYLINE_JOIN_ANGLE_STEP * Utils::DEGTORAD;
    const uint32_t steps = uint32_t( std::round( phi / CONST_POLYLINE_JOIN_ANGLE_STEP ) );

    std::cout << "steps(" << steps << ") for phi(" << phi << ")\n";

    if ( steps < 3 )
    {
        std::cout << "Too few steps(" << steps << ") for round-join, fallback to bevel-join.\n";
        addBeveledLineJoin( dc, M, L, R, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
        return;
    }


    dc.addTriangle2D( M, L, R, 0xFFFF00FF );

    float32_t rhoL = Utils::getAngle360( L - M );
    float32_t rhoR = Utils::getAngle360( R - M );
    glm::vec2 n = L - M;

    std::cout << "n(" << n << "), rhoL(" << Utils::toDegrees(rhoL) << "), rhoR(" << Utils::toDegrees(rhoR) << ")\n";

    dc.addLine2D( M, L, 0xFFFFFF00 );
    dc.addLine2D( M, R, 0xFFFF8F00 );

    glm::vec2 u = L;
    glm::vec2 v = R;


    // turn left
    if ( rhoR < rhoL )
    {
        float32_t angle = rhoL - angle_step;

        for ( uint32_t i = 1; i < steps-1; ++i )
        {
            v = M + Utils::rotate( n, angle );

            dc.addLine2D( M, v, randomColor(), randomColor() );

            std::cout << "Step[" << i << "] angle(" << Utils::toDegrees(angle) << ") v = " << v << "\n";

            angle -= angle_step;
        }
    }
    else
    {
        float32_t angle = rhoL + angle_step;

        for ( uint32_t i = 1; i < steps-1; ++i )
        {
            v = M + Utils::rotate( n, angle );

            dc.addLine2D( M, v, randomColor(), randomColor() );

            std::cout << "Step[" << i << "] angle(" << Utils::toDegrees(angle) << ") v = " << v << "\n";

            angle += angle_step;
        }

    }
    /*

        float32_t sin_angle = sin( angle );
        float32_t cos_angle = cos( angle );
        P2.x = n.x * cos_angle - n.y * sin_angle;
        P2.y = n.x * sin_angle + n.y * cos_angle;
        std::cout << "M(" << M << "), L(" << L << "), R(" << R << "), n(" << n << ")\n";
        std::cout << "angle_start(" << angle_start << " = " << ( irr::core::RADTODEG * angle_start ) << ")\n";
        std::cout << "angle_step(" << angle_step << " = " << ( irr::core::RADTODEG * angle_step ) << ")\n";
        std::cout << "angle(" << angle << " = " << ( irr::core::RADTODEG * angle ) << ")\n";
        std::cout << "P1(" << P1 << "), P2(" << P2 << ")\n";

        for ( uint32_t i = 0; i < steps - 2; ++i )
        {
            dc.addTriangle2D( M, P1, P2, colorA );

            P1 = P2;
            angle += angle_step;

            sin_angle = sin( angle );
            cos_angle = sin( angle );
            P2.x = n.x * cos_angle - n.y * sin_angle;
            P2.y = n.x * sin_angle + n.y * cos_angle;
        }

        P1 = P2;
        P2 = R;
        dc.addTriangle2D( M, P1, P2, colorB );
    */
}

/*
void
addMiterLineJoin(
        PrimitiveRenderer2D & dc,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        float32_t const miterLimit,
        uint32_t const colorA,
        uint32_t const colorB )
{
    // Add always the bevel triangle (B,CR,BR)
    dc.addTriangle( C, A, B, 0xFFFFBF40 );

    // Add Double Bevel:
    // compute and check if another triangle fits
    glm::vec2 const AB  { B - A         };
    glm::vec2 const M   { A + AB * 0.5f };
    glm::vec2 const CM  { M - C         };

    // Add Double Bevel triangle only if triangle is small enough
    if ( 2.0f * glm::length( CM ) < glm::length( AB ) )
    {
        dc.addTriangle2D( A, M + CM, B, 0xFFFFDF40 );
    }
}
*/

#endif
