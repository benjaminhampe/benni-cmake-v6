#include <de/hampe/svg/TexturedMiterPolyLine.hpp>

namespace de {
namespace hampe {
namespace svg {

#if 0
/// @brief generate a mesh from lines
template <  typename FNO_ADD_TEXTURED_VERTEX_2D,
            typename FNO_ADD_INDEX,
            typename FNI_GET_POINT_2D,
            typename FNI_GET_POINTCOUNT>
bool
TexturedMiterPolyLine::asTriangleStrip(
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

/// @brief generate a mesh from lines
template <  bool CLOCK_WISE,
            typename FNO_ADD_TEXTURED_VERTEX_2D,
            typename FNO_ADD_INDEX,
            typename FNI_GET_POINT,
            typename FNI_GET_POINTCOUNT>
static bool
asTriangleStrip_Debug(
        FNO_ADD_TEXTURED_VERTEX_2D const &  addTexturedVertex2D,
        FNO_ADD_INDEX const &      addIndex,
        FNI_GET_POINT const &      getPoint,
        FNI_GET_POINTCOUNT const & getPointCount,
        float32_t const            lineWidth )
{
    size_t const n = getPointCount();

    TEXTURED_MITER_POLYLINE_LOG_DEBUG( "n(", n, "):\n" )

    //      ( n == 0 ) for empty input we simply abort
    //      ( n == 1 ) for a single point we atleast create LineCaps
    //      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
    //      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin

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

#ifdef USE_TEXTURED_MITER_POLYLINE_DEBUG_LOGGING
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
        addTexturedVertex2D( A - nAB, glm::vec2( 0.0f, 0.0f ) );    // AL - First vertex of first segment[0]
        addTexturedVertex2D( A + nAB, glm::vec2( 0.0f, 1.0f ) );    // AR - Second vertex of first segment[0]
    }
    else
    {
        addTexturedVertex2D( A + nAB, glm::vec2( 0.0f, 1.0f ) );    // AR
        addTexturedVertex2D( A - nAB, glm::vec2( 0.0f, 0.0f ) );    // AL
    }
    addIndex( 2*processedPoints );
    addIndex( 2*processedPoints + 1 );
    ++processedPoints;

#ifdef USE_TEXTURED_MITER_POLYLINE_DEBUG_LOGGING
    vAB = B - A;
    dAB = glm::normalize(vAB);
    lAB = glm::length(vAB);
    TEXTURED_MITER_POLYLINE_LOG_DEBUG( "Segment[0] A(", A, "), B(", B, "), n(", nAB, "), l(", lAB, "), d(", dAB, ")\n" )
#endif
    // </SEGMENT>

    //<SEGMENT id="Middle">
    for ( size_t i = 1; i < n - 1; ++i )
    {
        C = getPoint( i + 1 );                  // Fetch C
        glm::vec2 const AB = B - A;
        glm::vec2 const BC = C - B;
        phi = static_cast< float32_t >( atan2( BC.y, BC.x ) - atan2( AB.y, AB.x ) );

#ifdef USE_TEXTURED_MITER_POLYLINE_DEBUG_LOGGING
        vAB = B - A;
        dAB = glm::normalize(vAB);
        lAB = glm::length(vAB);
        TEXTURED_MITER_POLYLINE_LOG_DEBUG( "Segment[",i,"] A(",A,"),B(",B,"),C(",C,"), nAB(",nAB,"), phi(", Math::toDegrees(phi), ")\n" )
#endif

#ifdef USE_TEXTURED_MITER_POLYLINE_POINT_SKIPPING
        // if (parallel) then combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.1f )
        {
            B = C;                            // Skip current segment
            nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
            TEXTURED_MITER_POLYLINE_LOG_DEBUG( "skip parallel point ", i, "\n")
            continue;
        }
#endif
        // ... continue computations ...
        nBC = Math::getNormal2D( C - B ) * lineWidth;   // Compute normal nBC

        // [LEFT] intersection must exist, or we skip the segment
#ifndef USE_TEXTURED_MITER_POLYLINE_DEBUG_LOGGING
        bool const hasLeft = IntersectionTool2D::intersectLine32( SL, A - nAB, B - nAB, B - nBC, C - nBC );
#else
        bool const hasLeft = IntersectionTool2D::intersectLineEx64( SL, A - nAB, B - nAB, B - nBC, C - nBC, t, s );
        min_t = std::min( min_t, t );
        max_t = std::max( max_t, t );
        min_s = std::min( min_s, s );
        max_s = std::max( max_s, s );
#endif

#ifndef USE_TEXTURED_MITER_POLYLINE_DEBUG_LOGGING
        bool const hasRight = IntersectionTool2D::intersectLine32( SR, A + nAB, B + nAB, B + nBC, C + nBC );
#else
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
            TEXTURED_MITER_POLYLINE_LOG_ERROR( "Segment[", i, "/", n-1, "] has NO LEFT AND RIGHT intersection with ", ss.str(), ", skip point.\n" )
            assert( false );
        }
        if ( !hasLeft )
        {
            TEXTURED_MITER_POLYLINE_LOG_ERROR( "Segment[", i, "/", n-1, "] has NO LEFT intersection with ", ss.str(), ", skip point.\n" )
        }
        if ( !hasRight )
        {
            TEXTURED_MITER_POLYLINE_LOG_ERROR( "Segment[", i, "/", n-1, "] has NO RIGHT intersection with ", ss.str(), ", skip point.\n" )
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

        accumLength += glm::length(B - A);

        float64_t u = accumLength / lineWidth;

        // [ADD VERTICES]
        // ... now the last segment [i-1] has all 4 vertices and current segment has already 2 vertices ...
        if (CLOCK_WISE)
        {
            addTexturedVertex2D( SL, glm::vec2( u, 0.0f ) ); // SL
            addTexturedVertex2D( SR, glm::vec2( u, 1.0f ) ); // SR
        }
        else
        {
            addTexturedVertex2D( SR, glm::vec2( u, 1.0f ) ); // SR
            addTexturedVertex2D( SL, glm::vec2( u, 0.0f ) ); // SL
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

    accumLength += glm::length(B - A);

    float64_t u = accumLength / lineWidth;
    // <SEGMENT id="End-Segment">
    // [ADD VERTICES]
    // ... now add the last two missing vertices of the current [i] segment ...
    if (CLOCK_WISE)
    {
        addTexturedVertex2D( B - nAB, glm::vec2( u, 0.0f ) ); // DL
        addTexturedVertex2D( B + nAB, glm::vec2( u, 1.0f ) ); // DR
    }
    else
    {
        addTexturedVertex2D( B + nAB, glm::vec2( u, 1.0f ) ); // DR
        addTexturedVertex2D( B - nAB, glm::vec2( u, 0.0f ) ); // DL
    }

    // [ADD INDICES]
    addIndex( 2*processedPoints );
    addIndex( 2*processedPoints + 1 );
    ++processedPoints;

    // statistics
    #ifdef USE_TEXTURED_MITER_POLYLINE_DEBUG_LOGGING
    ss.str("");
    ss << "t(" << t << ", " << min_t << ", " << max_t << "), "
       << "s(" << s << ", " << min_s << ", " << max_s << ")";
    TEXTURED_MITER_POLYLINE_LOG_DEBUG( "[END] processedPoints(", processedPoints, ") of (", n, ") with ", ss.str(), "\n")
    #endif
    return true;
}

#endif

} // end namespace svg
} // end namespace hampe
} // end namespace de
