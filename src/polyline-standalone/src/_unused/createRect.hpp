#ifndef DE_HAMPE_SVG_CREATE_RECT_HPP
#define DE_HAMPE_SVG_CREATE_RECT_HPP

#include <de/hampe/logging/Logger.hpp>
#include <de/hampe/scene/PrimitiveRenderer.hpp>

//////////////////////////////////////////////////////////////////////
/// @file   createRect.hpp
/// @brief  SVG Rect/Box
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

namespace de {
namespace hampe {
namespace svg {

class Rect2D
{
public:
    CREATE_STATIC_CLASS_LOGGER( "de.hampe.svg.Rect" )
    using LogLevel = de::hampe::logging::LogLevel;

public:
    /*
        Here we can see that every triangle can be cw or ccw just by flipping B,C
        thatswhy such easy tranformation should be solved by the engine itself
        and the user can think in cw ( clock-wise ) terms only.
     *
     * B +-----+ C  cw:     ABCD, ABC + ACD ( y up, z up )
         |    /|
         |  /  |    ccw:    BACD, ACB + ADC ( y up, z down )
         |/    |
       A +-----+ D
    */

    /// @brief generate
    template <  bool CLOCK_WISE,
                typename FNI_GET_VERTEXCOUNT,
                typename FNO_ADD_VERTEX_2D,
                typename FNO_ADD_INDEXED_TRIANGLE >
    static bool
    generateIndexedTriangles(
        FNI_GET_VERTEXCOUNT const & getVertexCount,
        FNO_ADD_VERTEX_2D const & addVertex2D,
        FNO_ADD_INDEXED_TRIANGLE const &     addIndexedTriangle,
        glm::vec2 const &         siz = glm::vec2(1,1),
        glm::vec2 const &         pos = glm::vec2(0,0) )
    {
        STATIC_LOG_DEBUG( "w(",siz.x,"), h(",siz.y,"), pos(",pos,")\n" )
        glm::vec2 const half = siz * 0.5f;
        glm::vec2 const A = pos + glm::vec2( -half.x, -half.y );
        glm::vec2 const B = pos + glm::vec2( -half.x,  half.y );
        glm::vec2 const C = pos + glm::vec2(  half.x,  half.y );
        glm::vec2 const D = pos + glm::vec2(  half.x, -half.y );
        uint32_t const v = getVertexCount();
        addVertex2D( A );
        addVertex2D( B );
        addVertex2D( C );
        addVertex2D( D );
        addIndexedTriangle( v, v + 1, v + 2 ); // ABC
        addIndexedTriangle( v, v + 2, v + 3 ); // ACD
        return true;
    }

    /// @brief generate
    template <  bool CLOCK_WISE >
    static bool
    draw(
        PrimitiveRenderer2D & dc,
        glm::vec2 const &     siz = glm::vec2(1,1),
        glm::vec2 const &     pos = glm::vec2(0,0) )
    {
        STATIC_LOG_DEBUG( "w(",siz.x,"), h(",siz.y,"), pos(",pos,")\n" )
        auto getVertexCount = [&dc] () -> uint32_t { return dc.getVertexCount(); };
        auto addVertex2D = [&dc,&pos] ( glm::vec2 const p, uint32_t color, glm::vec2 const uv ) -> void { dc.addVertex2D( pos + p, color, uv); };
        auto addIndexedTriangle = [&dc] ( uint32_t A, uint32_t B, uint32_t C ) ->void { dc.addIndexedTriangle( A,B,C ); };
        generateIndexedTriangles( getVertexCount, addVertex2D, addIndexedTriangle, siz, pos );
        return true;
    }

};

} // end namespace svg
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_SVG_CREATE_RECT_HPP
