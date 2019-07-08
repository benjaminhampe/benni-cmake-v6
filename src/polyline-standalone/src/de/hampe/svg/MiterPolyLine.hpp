//////////////////////////////////////////////////////////////////////
/// @file   MiterPolyLine.hpp
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

#ifndef DE_HAMPE_SVG_MITERPOLYLINE_HPP
#define DE_HAMPE_SVG_MITERPOLYLINE_HPP

#include <de/hampe/svg/Math.hpp>
#include <de/hampe/svg/IntersectionTool2D.hpp>
#include <de/hampe/svg/LineCap.hpp>
#include <de/hampe/svg/LineJoin.hpp>
#include <de/hampe/svg/PrimitiveRenderer.hpp>
#include <de/hampe/color/Color.hpp> // From de.hampe.common

//#define USE_MITER_DEBUG_LOGGING
//#define USE_MITER_ERROR_LOGGING
//#define USE_MITER_POLYLINE_POINT_SKIPPING

#ifdef USE_MITER_DEBUG_LOGGING
    #ifndef MITER_POLYLINE_LOG_DEBUG
    #define MITER_POLYLINE_LOG_DEBUG(...) STATIC_LOG_DEBUG(__VA_ARGS__)
    #endif
#else
    #ifndef MITER_POLYLINE_LOG_DEBUG
    #define MITER_POLYLINE_LOG_DEBUG(...)
    #endif
#endif

#ifdef USE_MITER_ERROR_LOGGING
    #ifndef MITER_POLYLINE_LOG_ERROR
    #define MITER_POLYLINE_LOG_ERROR(...) STATIC_LOG_ERROR(__VA_ARGS__)
    #endif
#else
    #ifndef MITER_POLYLINE_LOG_ERROR
    #define MITER_POLYLINE_LOG_ERROR(...)
    #endif
#endif

namespace de {
namespace hampe {
namespace svg {

class MiterPolyLine
{
    CREATE_STATIC_LOGGER( "de.hampe.svg.MiterPolyLine" )
    using LogLevel = de::hampe::logging::LogLevel;
public:
    /// @brief generate a mesh from lines
    template <  bool CLOCK_WISE,
                typename FNI_GET_POINT,
                typename FNI_GET_POINTCOUNT>
    static bool
    drawTriangleStrip(
            PrimitiveRenderer2D & dc,
            FNI_GET_POINT const &      getPoint,
            FNI_GET_POINTCOUNT const & getPointCount,
            float32_t const            lineWidth )
    {
        uint32_t const n = getPointCount();
        uint32_t v = dc.getVertexCount();
        STATIC_LOG_DEBUG("n(",n,"), cw(", CLOCK_WISE, "), lwidth(", lineWidth, "):\n" )

        //      ( n == 0 ) for empty input we simply abort
        //      ( n == 1 ) for a single point we atleast create LineCaps
        //      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
        //      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin

        if ( n == 0 )
        {
            MITER_POLYLINE_LOG_ERROR( "getPointCount() == 0, abort.\n" )
            return false; // nothing todo, empty input
        }

        else if ( n == 1 )
        {
            MITER_POLYLINE_LOG_ERROR( "n(1), NOT IMPLEMENTED, INFORMATION-LOSS, should create 2 LineCaps for a single point.\n" )
            return false; // nothing todo, empty input
        }

        //
        // Local variables:
        //
        glm::vec2 A,B,C,nAB,nBC,SL,SR,nABC; // We also need normals for both line-segments AB and BC
        float32_t phi;                   // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
        uint32_t processedPoints = 0; // num_discarded_points = getPointCount() - (processedSegments + 1)

        //--------------------------------------------------------------------
        //<START-SEGMENT>
        A = getPoint( 0 );                              // Fetch A
        B = getPoint( 1 );                              // Fetch B
        nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
        if (CLOCK_WISE)
        {
            dc.addVertex2D( A - nAB, randomColor() );                     // AL
            dc.addVertex2D( A + nAB, randomColor() );                     // AR
        }
        else
        {
            dc.addVertex2D( A + nAB, randomColor() );                     // AR
            dc.addVertex2D( A - nAB, randomColor() );                     // AL
        }
        dc.addIndex( v );                  // Irrlicht seem to need indices even for triangle-strip ?#*+.!
        dc.addIndex( v + 1 );              // Irrlicht seem to need indices even for triangle-strip ?#*+.!
        ++processedPoints;                              // statistics
        //</START-SEGMENT>
        //--------------------------------------------------------------------
        //<MIDDLE-SEGMENTS>
        for ( size_t i = 1; i < n - 1; ++i )
        {
            C = getPoint( i + 1 );                  // Fetch C
            phi = Math::getAngleABC( A,B,C );       // Compute angle between vector AB and BC

#if 1 // USE_POINT_SKIPPING == 1

            if ( std::abs( phi ) < 0.1f )           // if (parallel) then combine both segments by skipping this one.
            {
                B = C;                              // Skip current segment
                nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
                MITER_POLYLINE_LOG_DEBUG( "skip parallel point ", i, "\n")
                continue;
            }
#endif
            nBC = Math::getNormal2D( C - B ) * lineWidth;   // Compute normal nBC
            nABC = nAB + nBC;
            SL = B - nABC;
            SR = B + nABC;
            v = dc.getVertexCount();
            // [ADD VERTICES]
            // ... now the last segment [i-1] has all 4 vertices and current segment has already 2 vertices ...
            if (CLOCK_WISE)
            {
                dc.addVertex2D( SL ); // SL
                dc.addVertex2D( SR ); // SR
            }
            else
            {
                dc.addVertex2D( SR ); // SR
                dc.addVertex2D( SL ); // SL
            }

            // [ADD INDICES]
            //
            //    -AL----BL                A,B are actually not stored as vertices
            //    |\    |\
            //    | \   | \
            //    A  \  B  \
            //    |   \ |   \
            //    |    \|    \
            //    -AR----BR----CR
            dc.addIndex( v ); // 2*processedPoints );
            dc.addIndex( v + 1 ); // 2*processedPoints + 1 );

            // prepare NEXT:
            A = B;
            B = C;
            nAB = nBC;
            ++processedPoints;
        }
        // </MIDDLE-SEGMENTS>
        //--------------------------------------------------------------------
        // <END-SEGMENT>
        // ... [VERTICES] ... Add the last two missing vertices of the current [i] segment ...
        v = dc.getVertexCount();

        if (CLOCK_WISE)
        {
            dc.addVertex2D( B - nAB ); // DL
            dc.addVertex2D( B + nAB ); // DR
        }
        else
        {
            dc.addVertex2D( B + nAB ); // DR
            dc.addVertex2D( B - nAB ); // DL
        }

        // ... [INDICES] ...
        dc.addIndex( v ); // 2*processedPoints );
        dc.addIndex( v + 1 ); // 2*processedPoints + 1 );
        ++processedPoints;
        // </END-SEGMENT>
        //--------------------------------------------------------------------
        return true;
    }

#if 0
    /// @brief generate a mesh from lines
    template <  typename FNO_ADD_VERTEX_2D,
                typename FNO_ADD_INDEXED_TRIANGLE,
                typename FNI_GET_POINT,
                typename FNI_GET_POINTCOUNT >
    static bool
    asIndexedTriangles(
            FNO_ADD_VERTEX_2D const &          addVertex2D,
            FNO_ADD_INDEXED_TRIANGLE const &   addIndexedTriangle,
            FNI_GET_POINT const &              getPoint,
            FNI_GET_POINTCOUNT const &         getPointCount,
            float32_t const                    lineWidth )
    {
        size_t const n = getPointCount();

        MITER_POLYLINE_LOG_DEBUG( "n(", n, "):\n" )

        //      ( n == 0 ) for empty input we simply abort
        //      ( n == 1 ) for a single point we atleast create LineCaps
        //      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
        //      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin

        if ( n == 0 )
        {
            MITER_POLYLINE_LOG_ERROR( "getPointCount() == 0, abort.\n" )
            return false; // nothing todo, empty input
        }

        else if ( n == 1 )
        {
            MITER_POLYLINE_LOG_ERROR( "n(1), NOT IMPLEMENTED, INFORMATION-LOSS, should create 2 LineCaps for a single point.\n" )
            return false; // nothing todo, empty input
        }

        //
        // Local variables:
        //
        glm::vec2 A,B,C,nAB,nBC, SL, SR; // We also need normals for both line-segments AB and BC
        float32_t phi;                   // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
        uint32_t processedSegments = 0; // num_discarded_points = getPointCount() - (processedSegments + 1)

        float32_t t,s;        // for debugging and limitation of values
#ifdef USE_MITER_DEBUG_LOGGING
        float32_t min_t = std::numeric_limits< float32_t >::max();      // for debugging intersection functions
        float32_t max_t = std::numeric_limits< float32_t >::lowest();   // for debugging intersection functions
        float32_t min_s = std::numeric_limits< float32_t >::max();      // for debugging intersection functions
        float32_t max_s = std::numeric_limits< float32_t >::lowest();   // for debugging intersection functions
        std::stringstream ss;
        glm::vec2 vAB, vBC; // DEBUG vectors
        glm::vec2 dAB, dBC; // DEBUG directions
        float32_t lAB, lBC; // DEBUG lengths
#endif

        //<SEGMENT id="Start">
        A = getPoint( 0 );    // Fetch A
        B = getPoint( 1 );    // Fetch B
        nAB = Math::getNormal2D( B - A ) * lineWidth;      // Compute normal nAB
        addVertex2D( A - nAB );    // AL - First vertex of first segment[0]
        addVertex2D( A + nAB );    // AR - Second vertex of first segment[0]

#ifdef USE_MITER_DEBUG_LOGGING
        vAB = B - A;
        dAB = glm::normalize(vAB);
        lAB = glm::length(vAB);
        MITER_POLYLINE_LOG_DEBUG( "Segment[0] A(", A, "), B(", B, "), n(", nAB, "), l(", lAB, "), d(", dAB, ")\n" )
#endif
        // </SEGMENT>

        //<SEGMENT id="Middle">
        for ( size_t i = 1; i < n - 1; ++i )
        {
            C = getPoint( i + 1 );                    // Fetch C
            phi = Math::getAngleABC( A, B, C );    // Compute angle between vector AB and BC

#ifdef USE_MITER_DEBUG_LOGGING
            vAB = B - A;
            dAB = glm::normalize(vAB);
            lAB = glm::length(vAB);
            MITER_POLYLINE_LOG_DEBUG( "Segment[",i,"] A(",A,"),B(",B,"),C(",C,"), nAB(",nAB,"), phi(", Math::toDegrees(phi), ")\n" )
#endif

#ifdef USE_POINT_SKIPPING
            // if (parallel) then combine both segments by skipping this one.
            if ( std::abs( phi ) < 0.00001f || std::abs( phi - Math::TWO_PI ) < 0.00001f )
            {
                B = C;                            // Skip current segment
                nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
                MITER_POLYLINE_DEBUG( "Skip parallel point(", i, ")\n" )
                continue;
            }
#endif
            // ... continue computations ...
            nBC = Math::getNormal2D( C - B ) * lineWidth;   // Compute normal nBC

            // [LEFT] intersection must exist, or we skip the segment
            bool const hasLeft = IntersectionTool2D::intersectLineEx64( SL, A - nAB, B - nAB, B - nBC, C - nBC, t, s );
#ifdef USE_MITER_DEBUG_LOGGING
            min_t = std::min( min_t, t );
            max_t = std::max( max_t, t );
            min_s = std::min( min_s, s );
            max_s = std::max( max_s, s );
#endif

            bool const hasRight = IntersectionTool2D::intersectLineEx( SR, A + nAB, B + nAB, B + nBC, C + nBC, t, s );
#ifdef USE_MITER_DEBUG_LOGGING
            min_t = std::min( min_t, t );
            max_t = std::max( max_t, t );
            min_s = std::min( min_s, s );
            max_s = std::max( max_s, s );

            ss.str("");
            ss << "t(" << t << ", " << min_t << ", " << max_t << "), "
               << "s(" << s << ", " << min_s << ", " << max_s << ")";

            if ( !hasLeft && !hasRight)
            {
                MITER_POLYLINE_LOG_ERROR( "Segment[", i, "/", n-1, "] has NO LEFT AND RIGHT intersection with ", ss.str(), ", skip point.\n" )
                assert( false );
            }
            if ( !hasLeft )
            {
                MITER_POLYLINE_LOG_ERROR( "Segment[", i, "/", n-1, "] has NO LEFT intersection with ", ss.str(), ", skip point.\n" )
            }
            if ( !hasRight )
            {
                LOG_ERROR( "Segment[", i, "/", n-1, "] has NO RIGHT intersection with ", ss.str(), ", skip point.\n" )
            }
#endif
            if ( !hasLeft )
            {
                B = C;                            // Skip current segment
                nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
                continue;
            }
            if ( !hasRight )
            {
                B = C;                            // Skip current segment
                nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
                continue;
            }

            // [ADD VERTICES]
            // ... now the last segment [i-1] has all 4 vertices and current segment has already 2 vertices ...
            addVertex2D( SL ); // BL
            addVertex2D( SR ); // BL

            // [ADD INDICES]
            //
            //    -AL----BL                A,B are actually not stored as vertices
            //    |\    |\
            //    | \   | \
            //    A  \  B  \
            //    |   \ |   \
            //    |    \|    \
            //    -AR----BR----CR

            uint32_t const iAL = 2 * processedSegments;
            addIndexedTriangle( iAL, iAL + 3, iAL + 1 ); // AL, BR, AR
            addIndexedTriangle( iAL, iAL + 2, iAL + 3 ); // AL, BL, BR

            // prepare NEXT:
            A = B;
            B = C;
            nAB = nBC;
            ++processedSegments; // this leads to +1 than actual segments
        }
        // </SEGMENT>

        // <SEGMENT id="End-Segment">
        // ... now add the last two missing vertices of the current [i] segment ...
        addVertex2D( B - nAB ); // DL
        addVertex2D( B + nAB ); // DR
        uint32_t const iAL = 2 * processedSegments;
        addIndexedTriangle( iAL, iAL + 3, iAL + 1 );      // AL, BR, AR
        addIndexedTriangle( iAL, iAL + 2, iAL + 3 );      // AL, BL, BR
        ++processedSegments;

        // statistics
        #ifdef USE_MITER_DEBUG_LOGGING
        ss.str("");
        ss << "t(" << t << ", " << min_t << ", " << max_t << "), "
           << "s(" << s << ", " << min_s << ", " << max_s << ")";
        MITER_POLYLINE_LOG_DEBUG("processedSegments(", processedSegments,") of (", n - 1, ") segments.\n" )
        #endif
        return true;
    }
#endif
};

} // end namespace svg
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_SVG_MITERPOLYLINE_HPP





