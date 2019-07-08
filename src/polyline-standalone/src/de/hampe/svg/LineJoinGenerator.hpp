//////////////////////////////////////////////////////////////////////
/// @file   LineJoinGenerator.hpp
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

#ifndef DE_HAMPE_SVG_LINEJOIN_GENERATOR_HPP
#define DE_HAMPE_SVG_LINEJOIN_GENERATOR_HPP

#include <de/hampe/svg/LineJoin.hpp>
#include <de/hampe/svg/PrimitiveRenderer.hpp>

namespace de {
namespace hampe {
namespace svg {

// [Old]: Line-joins for pure triangle based meshes ( only for debugging )

void addBeveledLineJoin( PrimitiveRenderer2D & dc, glm::vec2 const C, glm::vec2 const A, glm::vec2 const B );
void addRoundLineJoin( PrimitiveRenderer2D & dc, glm::vec2 const C, glm::vec2 const A, glm::vec2 const B, float32_t const phi );
//void addMiterLineJoin( PrimitiveRenderer2D & dc, glm::vec2 const C, glm::vec2 const A, glm::vec2 const B, float32_t const miterLimit );

inline void
addLineJoin(
        PrimitiveRenderer2D & dc,
        LineJoin const lineJoin,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        float32_t const phiACB )
{
    // LineJoin::NONE : nothing todo.

//    if ( lineJoin == LineJoin::BEVEL )
//    {
//        addBeveledLineJoin( dc, C, A, B );
//    }
//    else
    if ( lineJoin == LineJoin::ROUND )
    {
        addRoundLineJoin( dc, C, A, B, phiACB );
    }
    // Miter is integrated in PolyLine algorithm, and this is not called by other classes ( so far )
//    else if ( lineJoin == LineJoin::MITER )
//    {
//        // addMiterLineJoin( dc, C, A, B, phiACB );
//        //assert( false && "Miter Line Join is not supported by now." );
//    }
}

} // end namespace svg
} // end namespace hampe
} // end namespace de

#if 0

// [New]: Line-joins for indexed triangles based meshes ( unfinished )

void addBeveledLineJoin( PrimitiveRenderer2D & dc, uint32_t const M, uint32_t const L, uint32_t const R );
void addRoundLineJoin( PrimitiveRenderer2D & dc, uint32_t const M, uint32_t const L, uint32_t const R, float32_t const phi );

inline void
addLineJoin( PrimitiveRenderer2D & dc, LineJoin const lineJoin, uint32_t const M, uint32_t const L, uint32_t const R, float32_t const angleLMR ) // angleABC only used for round line-joins ( so far )
{
    // LineJoin::NONE : nothing todo.

    if ( lineJoin == LineJoin::BEVEL )
    {
        addBeveledLineJoin( dc, M, L, R );
    }
    else if ( lineJoin == LineJoin::ROUND )
    {
        // delta_phi tells to produce every x degrees (e.g. 10.0f) a new triangle up to phi_ABC ( e.g. 55.0f deg ) -> produces 5 triangles that are 11.0f deg apart.
        addRoundLineJoin( dc, M, L, R, angleLMR );
    }
    // Miter is integrated in PolyLine algorithm, and this func is not called by other classes ( so far )
//    else if ( lineJoin == LineJoin::MITER )
//    {
//        addMiterLineJoin( dc, M, L, R, phi );
//        //assert( false && "Miter Line Join is not supported by now." );
//    }
}
#endif

#endif // DE_HAMPE_SVG_LINEJOIN_GENERATOR_HPP
