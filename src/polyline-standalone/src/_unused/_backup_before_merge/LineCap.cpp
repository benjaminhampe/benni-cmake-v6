#include <de/hampe/polyline/LineCap.hpp>

std::ostream &
operator<< ( std::ostream & out, LineCap const lineCap )
{
    switch( lineCap )
    {
        case LineCap::SQUARE: out << "SQUARE"; break;
        case LineCap::ROUND: out << "ROUND"; break;
        default: out << "NONE"; break;
    }

    return out;
}

void addLineCap(
        PrimitiveRenderer & dc,
        LineCap const lineCap,
        glm::vec2 const M,
        glm::vec2 const L,
        glm::vec2 const R,
        bool const isEndCap )
{
    if ( lineCap != LineCap::SQUARE &&
         lineCap != LineCap::ROUND)
    {
        return; // nothing todo ( none ) or unsupported LineCap
    }

    if ( lineCap == LineCap::SQUARE )
    {
        glm::vec2 const n = Utils::getNormal2D( L - M ) * glm::length( L - M );
        if ( !isEndCap )
        {
            glm::vec2 const P( L - n );
            glm::vec2 const Q( R - n );
            dc.addTriangle2D( Q, P, L, POLYLINE_COLOR_LINE_JOIN_BEVEL );
            dc.addTriangle2D( L, R, Q, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
        }
        else
        {
            glm::vec2 const P( L + n );
            glm::vec2 const Q( R + n );
            dc.addTriangle2D( R, L, P, POLYLINE_COLOR_LINE_JOIN_BEVEL );
            dc.addTriangle2D( P, Q, R, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
        }

        return;
    }

}
