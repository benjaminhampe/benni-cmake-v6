#include <de/hampe/polyline/LineJoin.hpp>

std::ostream &
operator<< ( std::ostream & out, LineJoin const lineJoin )
{
    switch( lineJoin )
    {
        case LineJoin::BEVEL: out << "BEVEL"; break;
        case LineJoin::ROUND: out << "ROUND"; break;
        case LineJoin::MITER: out << "MITER"; break;
        default: out << "NONE"; break;
    }

    return out;
}

void
addBeveledLineJoin(
        PrimitiveRenderer & dc,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        uint32_t const colorA,
        uint32_t const colorB );

void
addMiterLineJoin(
        PrimitiveRenderer & dc,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        float32_t const miterLimit,
        uint32_t const colorA,
        uint32_t const colorB );

void
addRoundLineJoin(
        PrimitiveRenderer & dc,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        float32_t const phi,
        uint32_t const colorA,
        uint32_t const colorB );

void
addLineJoin(
        PrimitiveRenderer & dc,
        LineJoin const lineJoin,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        float32_t const phi )
{
    if ( lineJoin == LineJoin::BEVEL )
    {
        addBeveledLineJoin( dc, C, A, B, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
    else if ( lineJoin == LineJoin::ROUND )
    {
        addRoundLineJoin( dc, C, A, B, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
    else if ( lineJoin == LineJoin::MITER )
    {
        // addMiterLineJoin( dc, C, A, B, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
        assert( false && "Miter Line Join is not supported by now." );
    }
    else
    {
        assert( false && "Unknown Line Join not supported." );
    }
}


void
addBeveledLineJoin(
        PrimitiveRenderer & dc,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        uint32_t const colorA,
        uint32_t const colorB )
{
    // Add always the bevel triangle (B,CR,BR)
    dc.addTriangle2D( C, A, B, colorA );

    // Add Double Bevel:
    // compute and check if another triangle fits
    glm::vec2 const AB  { B - A         };
    glm::vec2 const M   { A + AB * 0.5f };
    glm::vec2 const CM  { M - C         };

    // Add Double Bevel triangle only if triangle is small enough
    if ( 2.0f * glm::length( CM ) < glm::length( AB ) )
    {
        dc.addTriangle2D( A, M + CM, B, colorB );
    }
}

void
addMiterLineJoin(
        PrimitiveRenderer & dc,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        float32_t const miterLimit,
        uint32_t const colorA,
        uint32_t const colorB )
{
    // Add always the bevel triangle (B,CR,BR)
    dc.addTriangle2D( C, A, B, 0xFFFFBF40 );

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
        addBeveledLineJoin( dc, M, L, R, colorA, colorB );
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
