//////////////////////////////////////////////////////////////////////
/// @file   LineCapGenerator.cpp
/// @brief  SVG LineCapGenerator
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

#include <de/hampe/svg/LineCapGenerator.hpp>
#include <de/hampe/svg/Math.hpp>

#define CONST_POLYLINECAP_COLOR_1 0xFFFFC810
#define CONST_POLYLINECAP_COLOR_2 0xFFFF8010

namespace de {
namespace hampe {
namespace svg {

void addLineCap(
        PrimitiveRenderer2D & dc,
        LineCap const lineCap,
        glm::vec2 const A,
        glm::vec2 const B,
        float32_t const lineWidth,
        bool const isEndCap )
{
    if ( lineCap == LineCap::NONE)
    {
        return; // nothing todo ( none )
    }

    if ( lineCap == LineCap::SQUARE )
    {
        glm::vec2 const m = glm::normalize(B - A) * lineWidth;
        glm::vec2 const n = Math::getNormal2D( B - A ) * lineWidth;
        if ( !isEndCap )
        {
            glm::vec2 const a = A - n - m;
            glm::vec2 const b = A - n;
            glm::vec2 const c = A + n;
            glm::vec2 const d = A + n - m;
            dc.addTriangle2D( b, a, d, CONST_POLYLINECAP_COLOR_1 );
            dc.addTriangle2D( b, d, c, CONST_POLYLINECAP_COLOR_2 );
        }
        else
        {
            glm::vec2 const a = B - n;
            glm::vec2 const b = B - n + m;
            glm::vec2 const c = B + n + m;
            glm::vec2 const d = B + n;
            dc.addTriangle2D( b, a, d, CONST_POLYLINECAP_COLOR_1 );
            dc.addTriangle2D( b, d, c, CONST_POLYLINECAP_COLOR_2 );
        }
    }
}


} // end namespace svg
} // end namespace hampe
} // end namespace de
