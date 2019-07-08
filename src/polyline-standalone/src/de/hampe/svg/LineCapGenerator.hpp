//////////////////////////////////////////////////////////////////////
/// @file 	LineCapGenerator.hpp
/// @brief  SVG LineCap
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

#ifndef DE_HAMPE_SVG_LINECAP_GENERATOR_HPP
#define DE_HAMPE_SVG_LINECAP_GENERATOR_HPP

#include <de/hampe/svg/LineCap.hpp>
#include <de/hampe/svg/PrimitiveRenderer.hpp>

namespace de {
namespace hampe {
namespace svg {

/// @brief Generates different types of line-caps by the SVG standard
/// @param[in,out] dc Geometry generator that can create triangles -> main output
/// @param[in] lineCap The type of shape we construct ( none, square, round ).
/// @param[in] M The already computed center-point A ( reused to prevent artifacts )
/// @param[in] L The already computed left point L = A - nAB ( reused to prevent artifacts )
/// @param[in] R The already computed reight point R = A + nAB ( reused to prevent artifacts )
/// @param[in] isEndCap If <false> we have a StartCap and if <true> when we construct an EndCap.
/// @param[in] color Only used for visual debugging, TODO: remove in release
void
addLineCap(
    PrimitiveRenderer2D & dc,
    LineCap const lineCap,
    glm::vec2 const A,
    glm::vec2 const B,
    float32_t const lineWidth,
    bool const isEndCap );

inline void
addStartLineCap( PrimitiveRenderer2D & dc, LineCap const lineCap, glm::vec2 const A, glm::vec2 const B, float32_t const lineWidth )
{
    addLineCap( dc, lineCap, A, B, lineWidth, false );
}

inline void
addEndLineCap( PrimitiveRenderer2D & dc, LineCap const lineCap, glm::vec2 const A, glm::vec2 const B, float32_t const lineWidth )
{
    addLineCap( dc, lineCap, A, B, lineWidth, true );
}

} // end namespace svg
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_SVG_LINECAP_GENERATOR_HPP
