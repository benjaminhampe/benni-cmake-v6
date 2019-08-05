#include "MiterPolyLine.hpp"

#if 0

// BACKUP

namespace de {
namespace hampe {
namespace svg {

class MiterPolyLine
{
    CREATE_STATIC_CLASS_LOGGER( "de.hampe.svg.MiterPolyLine" )
    using LogLevel = de::hampe::logging::LogLevel;
    using IntersectionTool2D = de::hampe::math::IntersectionTool2D;
    using Math = de::hampe::math::Math;
public:
    /// @brief generate a mesh from lines
    template <  bool CLOCK_WISE,
                bool USE_POINT_SKIPPING,
                bool USE_VECTOR_ADDITION,
                typename FNO_ADD_VERTEX_2D,
                typename FNO_ADD_INDEX,
                typename FNI_GET_POINT,
                typename FNI_GET_POINTCOUNT>
    static bool
    asTriangleStripBest(
            FNO_ADD_VERTEX_2D const &  addVertex2D,
            FNO_ADD_INDEX const &      addIndex,
            FNI_GET_POINT const &      getPoint,
            FNI_GET_POINTCOUNT const & getPointCount,
            float32_t const            lineWidth )
    {
        size_t const n = getPointCount();

        STATIC_LOG_DEBUG("n(",n,"), cw(", CLOCK_WISE, "), pskip(", USE_POINT_SKIPPING, "), vadd(", USE_VECTOR_ADDITION, "), lwidth(", lineWidth, "):\n" )

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
        uint32_t processedPoints = 0; // num_discarded_points = getPointCount() - (processedSegments + 1)

#ifdef USE_MITER_DEBUG_LOGGING
        float32_t t,s;                                                  // for debugging and limitation of values
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
        if (CLOCK_WISE)
        {
            addVertex2D( A - nAB );    // AL - First vertex of first segment[0]
            addVertex2D( A + nAB );    // AR - Second vertex of first segment[0]
        }
        else
        {
            addVertex2D( A + nAB );    // AR
            addVertex2D( A - nAB );    // AL
        }
        addIndex( 2*processedPoints );
        addIndex( 2*processedPoints + 1 );
        ++processedPoints;

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
            C = getPoint( i + 1 );                  // Fetch C

            // Compute angle between vector AB and BC
            //phi = Math::getAngleABC( A, B, C );
            {
                glm::vec2 const AB = B - A;
                glm::vec2 const BC = C - B;
                phi = static_cast< float32_t >( atan2( BC.y, BC.x ) - atan2( AB.y, AB.x ) );
             }

#ifdef USE_MITER_DEBUG_LOGGING
            vAB = B - A;
            dAB = glm::normalize(vAB);
            lAB = glm::length(vAB);
            MITER_POLYLINE_LOG_DEBUG( "Segment[",i,"] A(",A,"),B(",B,"),C(",C,"), nAB(",nAB,"), phi(", Math::toDegrees(phi), ")\n" )
#endif
            if ( USE_POINT_SKIPPING )
            {
                // if (parallel) then combine both segments by skipping this one.
                if ( std::abs( phi ) < 0.1f )
                {
                    B = C;                            // Skip current segment
                    nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
                    MITER_POLYLINE_LOG_DEBUG( "skip parallel point ", i, "\n")
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
                #ifdef USE_MITER_DEBUG_LOGGING
                bool const hasLeft = IntersectionTool2D::intersectLineEx64( SL, A - nAB, B - nAB, B - nBC, C - nBC, t, s );
                min_t = std::min( min_t, t );
                max_t = std::max( max_t, t );
                min_s = std::min( min_s, s );
                max_s = std::max( max_s, s );
                #endif

                bool const hasRight = IntersectionTool2D::intersectLine32( SR, A + nAB, B + nAB, B + nBC, C + nBC );
                #ifdef USE_MITER_DEBUG_LOGGING
                bool const hasRight = IntersectionTool2D::intersectLineEx64( SR, A + nAB, B + nAB, B + nBC, C + nBC, t, s );
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
                    MITER_POLYLINE_LOG_ERROR( "Segment[", i, "/", n-1, "] has NO RIGHT intersection with ", ss.str(), ", skip point.\n" )
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
            }

            // [ADD VERTICES]
            // ... now the last segment [i-1] has all 4 vertices and current segment has already 2 vertices ...
            if (CLOCK_WISE)
            {
                addVertex2D( SL ); // SL
                addVertex2D( SR ); // SR
            }
            else
            {
                addVertex2D( SR ); // SR
                addVertex2D( SL ); // SL
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
            addIndex( 2*processedPoints );
            addIndex( 2*processedPoints + 1 );

            // prepare NEXT:
            A = B;
            B = C;
            nAB = nBC;
            ++processedPoints;
        }
        // </SEGMENT>

        // <SEGMENT id="End-Segment">
        // [ADD VERTICES]
        // ... now add the last two missing vertices of the current [i] segment ...
        if (CLOCK_WISE)
        {
            addVertex2D( B - nAB ); // DL
            addVertex2D( B + nAB ); // DR
        }
        else
        {
            addVertex2D( B + nAB ); // DR
            addVertex2D( B - nAB ); // DL
        }
        // [ADD INDICES]
        addIndex( 2*processedPoints );
        addIndex( 2*processedPoints + 1 );
        ++processedPoints;

        // statistics
        #ifdef USE_MITER_DEBUG_LOGGING
        ss.str("");
        ss << "t(" << t << ", " << min_t << ", " << max_t << "), "
           << "s(" << s << ", " << min_s << ", " << max_s << ")";
        MITER_POLYLINE_LOG_DEBUG( "[END] processedPoints(", processedPoints, ") of (", n, ") with ", ss.str(), "\n")
        #endif
        return true;
    }

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
};

} // end namespace svg
} // end namespace hampe
} // end namespace de

#endif
