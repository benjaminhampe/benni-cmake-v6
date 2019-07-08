//////////////////////////////////////////////////////////////////////
/// @file   TexturedMiterPolyLine.hpp
/// @brief  SVG Miter PolyLine (miter line joins, fastest polyline)
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

#ifndef DE_HAMPE_SVG_TEXTUREDMITERPOLYLINE_HPP
#define DE_HAMPE_SVG_TEXTUREDMITERPOLYLINE_HPP

#include <de/hampe/svg/Math.hpp>
#include <de/hampe/svg/IntersectionTool2D.hpp>
#include <de/hampe/svg/LineCap.hpp>
#include <de/hampe/svg/LineJoin.hpp>

//#define USE_TEXTURED_MITER_POLYLINE_DEBUG_LOGGING
//#define USE_TEXTURED_MITER_POLYLINE_ERROR_LOGGING

#ifdef USE_TEXTURED_MITER_POLYLINE_DEBUG_LOGGING
    #ifndef TEXTURED_MITER_POLYLINE_LOG_DEBUG
    #define TEXTURED_MITER_POLYLINE_LOG_DEBUG(...) STATIC_LOG_DEBUG(__VA_ARGS__)
    #endif
#else
    #ifndef TEXTURED_MITER_POLYLINE_LOG_DEBUG
    #define TEXTURED_MITER_POLYLINE_LOG_DEBUG(...)
    #endif
#endif

#ifdef USE_TEXTURED_MITER_POLYLINE_ERROR_LOGGING
    #ifndef TEXTURED_MITER_POLYLINE_LOG_ERROR
    #define TEXTURED_MITER_POLYLINE_LOG_ERROR(...) STATIC_LOG_ERROR(__VA_ARGS__)
    #endif
#else
    #ifndef TEXTURED_MITER_POLYLINE_LOG_ERROR
    #define TEXTURED_MITER_POLYLINE_LOG_ERROR(...)
    #endif
#endif

namespace de {
namespace hampe {
namespace svg {

class TexturedMiterPolyLine
{
    CREATE_STATIC_LOGGER( "de.hampe.svg.TexturedMiterPolyLine" )
    using LogLevel = de::hampe::logging::LogLevel;

public:
    /// @brief generate a mesh from lines
    template <  typename FNO_ADD_TEXTURED_VERTEX_2D,
                typename FNO_ADD_INDEX,
                typename FNI_GET_POINT_2D,
                typename FNI_GET_POINTCOUNT>
    static bool
    asTriangleStrip(
        bool CLOCK_WISE,
        bool USE_POINT_SKIPPING,
        bool USE_VECTOR_ADDITION,
        FNO_ADD_TEXTURED_VERTEX_2D const &  addTexturedVertex2D,
        FNO_ADD_INDEX const &               addIndex,
        FNI_GET_POINT_2D const &            getPoint2D,
        FNI_GET_POINTCOUNT const &          getPointCount,
        float32_t const                     lineWidth )
    {
        size_t const n = getPointCount();

        STATIC_LOG_DEBUG("n(",n,"), cw(", CLOCK_WISE, "), pskip(", USE_POINT_SKIPPING, "), vadd(", USE_VECTOR_ADDITION, "), lwidth(", lineWidth, ")\n" )

        if ( n == 0 )
        {
            TEXTURED_MITER_POLYLINE_LOG_ERROR( "getPointCount() == 0, abort.\n" )
            return false; // nothing todo, empty input
        }

        else if ( n == 1 )
        {
            TEXTURED_MITER_POLYLINE_LOG_ERROR( "n(1), NOT IMPLEMENTED, INFORMATION-LOSS, should create 2 LineCaps for a single point.\n" )
            return false; // nothing todo, empty input
        }

        //
        // Local variables:
        //
        glm::vec2 A,B,C,nAB,nBC, SL, SR; // We also need normals for both line-segments AB and BC
        float32_t phi;                   // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
        uint32_t processedPoints = 0; // num_discarded_points = getPointCount() - (processedSegments + 1)
        float64_t accumLength = 0.0;

        //<SEGMENT id="Start">
        A = getPoint2D( 0 );    // Fetch A
        B = getPoint2D( 1 );    // Fetch B
        nAB = Math::getNormal2D( B - A ) * lineWidth;      // Compute normal nAB
        if (CLOCK_WISE)
        {
            addTexturedVertex2D( A - nAB, glm::vec2( 0.0f, 0.0f ) );    // AL - First vertex of first segment[0]
            addTexturedVertex2D( A + nAB, glm::vec2( 1.0f, 0.0f ) );    // AR - Second vertex of first segment[0]
        }
        else
        {
            addTexturedVertex2D( A + nAB, glm::vec2( 1.0f, 0.0f ) );    // AR
            addTexturedVertex2D( A - nAB, glm::vec2( 0.0f, 0.0f ) );    // AL
        }
        addIndex( 2*processedPoints );
        addIndex( 2*processedPoints + 1 );
        ++processedPoints;
        // </SEGMENT>

        //<SEGMENT id="Middle">
        for ( size_t i = 1; i < n - 1; ++i )
        {
            C = getPoint2D( i + 1 );                  // Fetch C
            glm::vec2 const AB = B - A;
            glm::vec2 const BC = C - B;
            phi = static_cast< float32_t >( atan2( BC.y, BC.x ) - atan2( AB.y, AB.x ) );

            if ( USE_POINT_SKIPPING )
            {
               // if (parallel) then combine both segments by skipping this one.
               if ( std::abs( phi ) < 0.1f )
               {
                   B = C;                            // Skip current segment
                   nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
                   TEXTURED_MITER_POLYLINE_LOG_DEBUG( "skip parallel point ", i, "\n")
                   continue;
               }
            }

            // ... continue computations ...
            nBC = Math::getNormal2D( C - B ) * lineWidth;   // Compute normal nBC

            if ( USE_VECTOR_ADDITION )
            {
                glm::vec2 const nABC = glm::normalize( nAB + nBC );
                SL = B - nABC;
                SR = B + nABC;
            }
            else
            {
                // [LEFT] intersection must exist, or we skip the segment
                bool const hasLeft = IntersectionTool2D::intersectLine32( SL, A - nAB, B - nAB, B - nBC, C - nBC );
                bool const hasRight = IntersectionTool2D::intersectLine32( SR, A + nAB, B + nAB, B + nBC, C + nBC );

                if ( !hasLeft || !hasRight)
                {
                    B = C;                            // Skip current segment
                    nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
                    TEXTURED_MITER_POLYLINE_LOG_ERROR( "Segment[", i, "/", n-1, "] has NO intersection, skip point.\n" )
                    continue;
                }
            }

            accumLength += glm::length(B - A);
            float64_t v = accumLength / lineWidth;

            // [ADD VERTICES]
            // ... now the last segment [i-1] has all 4 vertices and current segment has already 2 vertices ...
            if (CLOCK_WISE)
            {
                addTexturedVertex2D( SL, glm::vec2( 0.0f, v ) ); // SL
                addTexturedVertex2D( SR, glm::vec2( 1.0f, v ) ); // SR
            }
            else
            {
                addTexturedVertex2D( SR, glm::vec2( 1.0f, v ) ); // SR
                addTexturedVertex2D( SL, glm::vec2( 0.0f, v ) ); // SL
            }

            // [ADD INDICES]
            addIndex( 2*processedPoints );
            addIndex( 2*processedPoints + 1 );

            // prepare NEXT:
            A = B;
            B = C;
            nAB = nBC;
            ++processedPoints;
        }
        // </SEGMENT>

        accumLength += glm::length(B - A);
        float64_t v = accumLength / lineWidth;

        // <SEGMENT id="End-Segment">
        // [ADD VERTICES]
        // ... now add the last two missing vertices of the current [i] segment ...
        if (CLOCK_WISE)
        {
            addTexturedVertex2D( B - nAB, glm::vec2( 0.0f, v ) ); // DL
            addTexturedVertex2D( B + nAB, glm::vec2( 1.0f, v ) ); // DR
        }
        else
        {
            addTexturedVertex2D( B + nAB, glm::vec2( 1.0f, v ) ); // DR
            addTexturedVertex2D( B - nAB, glm::vec2( 0.0f, v ) ); // DL
        }

        // [ADD INDICES]
        addIndex( 2*processedPoints );
        addIndex( 2*processedPoints + 1 );
        ++processedPoints;
        return true;
    }

};

} // end namespace svg
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_SVG_TEXTUREDMITERPOLYLINE_HPP
