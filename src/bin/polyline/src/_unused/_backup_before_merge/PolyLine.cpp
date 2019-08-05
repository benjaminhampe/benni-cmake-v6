#include <de/hampe/polyline/PolyLine.hpp>

#include <de/hampe/polyline/LineJoin.hpp>

#ifdef USE_POLYLINE_DEBUG_LOGGING
#   include <sstream>
#   include <iostream>
#endif

#define USE_POLYLINE_POINT_SKIPPING

//
// Supports different line-joins and end-caps to comply with SVG standard.
//
// We can process all 'polyline' shapes,
//      0.) for empty input we simply abort
//      1.) for a single point we create 2 LineCaps
//      2.) for a single line-segment we create 2 LineCaps and 1 Segment, TODO: build rect instead of hexagon
//      3.) a normal polyline ABC with 2 LineCaps, 2 Segments and 1 LineJoin
//
//      4.) Points that lead to too small segments will be discarded/jumped over. DANGEROUS AFTER INTERSECT-POLYLINE
//
// ... With (n >= 3) we have normal polyline ( e.g. SVG Path ) with:
//          + 1x StartSegment
//          + 1x EndSegment
//          + (n-3) x MiddleSegments
//          + (n-2) x LineJoins ( supports SVG joins like miter, beveled ( enhanced ) or rounded )
//          + 2x EndCaps ( optional )

void addPolyLine(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth,
        LineJoin const lineJoin,
        LineCap const lineCap )
{
    // -----AL--------BL-----------     Dreiecke:   1.) A, AL, BL
    //     /       ---- \                           2.) A, BL, B
    //    /    ----      \    -n         CW         3.) A, B, BR
    //   / ----           \    |                    4.) A, BR, AR
    //  A------------------B-------
    //   \ ----           /    |
    //    \    ----      /    +n
    //     \       ---- /
    // -----AR--------BR-----------

    auto addHexagon = [&dc] (   glm::vec2 const A, glm::vec2 const AL, glm::vec2 const AR,
                                glm::vec2 const B, glm::vec2 const BL, glm::vec2 const BR,
                                uint32_t const debugColor )
    {
        dc.addTriangle2D( A, AL, BL, debugColor );
        dc.addTriangle2D( A, BL, B, Utils::varyColor( debugColor, 50 ) );
        dc.addTriangle2D( A, B, BR, Utils::varyColor( debugColor, 50 ) );
        dc.addTriangle2D( A, BR, AR, Utils::varyColor( debugColor, 50 ) );
    };

    // We can process all 'polyline' shapes,

    size_t const n = points.size();

    //      ( n == 0 ) for empty input we simply abort
    //      ( n == 1 ) for a single point we atleast create LineCaps
    //      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
    //      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin

    if ( n == 0 )
    {
        return;// nothing todo, empty input
    }
    else if ( n == 1 ) // A single point ( invisible ) but with possible end-caps ( optional ).
    {
        glm::vec2 const A = points[ 0 ];
        glm::vec2 const nA( 0, -1 );
        addLineCap( dc, lineCap, A, A - nA, A + nA, false );
        addLineCap( dc, lineCap, A, A - nA, A + nA, true );
        return;
    }

    //
    // Local variables:
    //
    glm::vec2 A,B,C;            // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 KL,KR,QL,QR;      // Fully describes current hexagon segment A,KL,KR,B,QL,QR
    glm::vec2 nAB, nBC;         // We also need normals for both line-segments AB and BC
    float32_t phi;              // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
    uint32_t color = 0xFFFFFFFF;// Only for debugging, TODO: remove in release mode

    size_t discardPoints = 0;

#ifdef USE_POLYLINE_VECTOR_ADDITION
        glm::vec2 nSL;
#else
        glm::vec2 S;
#endif

    // handle START segment:
    color = Utils::varyColor( 0xFF2020AF, 50 );        // blue debug color
    A = points[ 0 ];                            // Fetch A
    B = points[ 1 ];                            // Fetch B
    nAB = Utils::getNormal2D( B - A ) * lineWidth;      // Compute normal nAB
    KL = A - nAB;                               // specific for StartSegment
    KR = A + nAB;                               // specific for StartSegment
    addLineCap( dc, lineCap, A, KL, KR, false );// Construct an EndCap for StartSegment

#ifdef USE_POLYLINE_DEBUG_LOGGING
    std::cout << "StartSegment[" << 0 << "/" << n-1 << "]: "
              << "A(" << A << "), B(" << B << "), C(" << C << "), "
              << "KL(" << KL << "), KR(" << KR << "), QL(" << QL << "), QR(" << QR << ")\n";
#endif

    // handle MIDDLE segment(s):
    for ( size_t i = 1; i < n - 1; ++i )
    {
        C = points[ i+1 ];                      // Fetch C
        phi = Utils::getAngleABC( A, B, C );           // Compute angle between vector AB and BC

#ifdef USE_POLYLINE_DEBUG_LOGGING
        std::cout << "MiddleSegment[" << i << "/" << n-1 << "]: ";
#endif

#ifdef USE_POLYLINE_POINT_SKIPPING
        // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.00001f || std::abs( phi - Utils::TWO_PI ) < 0.00001f )
        {
            B = C;                                      // Skip current segment by replacing current point B with next point
            nAB = Utils::getNormal2D( B - A ) * lineWidth;      // Compute normal nAB
            ++discardPoints;
            continue;
        }
#endif

        // ... continue computations ...

        nBC = Utils::getNormal2D( C - B ) * lineWidth;  // Compute normal nBC

        // turn left ( math positive, ccw )
        //
        //     DL---C----DR
        //      \    \    \
        //       \    \    \
        //        \    \    \
        //         \    \    \
        // AL-------SL   \    \
        // |    ----  -   \    \
        // |----        -  \    \
        // A----------------B----CR
        // |--------        |    /
        // |        --------|  /
        // AR---------------BR/
        //
        // SL = B - 0.5 ( nAB + nBC );
        // CR = B + nBC;
        // BR = B + nAB;
        //

        if ( phi <= Utils::PI )     // turn left ( math positive, ccw )
        {
#ifdef USE_POLYLINE_DEBUG_LOGGING
            std::cout << "LEFT turn with phi(" << Utils::toDegrees( phi ) << "), ";
#endif
#ifdef USE_POLYLINE_VECTOR_ADDITION
            QL = B - ( nAB + nBC ) * .5f; // left intersection point
#else
            if ( IntersectionTool::intersectRay2D( S, A - nAB, B - nAB, B - nBC, C - nBC ) )
            {
                QL = S;
            }
#endif
            glm::vec2 const BR = B + nAB;
            glm::vec2 const CR = B + nBC;
            addHexagon( A, KL, KR, B, QL, BR, color );
            addLineJoin( dc, lineJoin, B, CR, BR, phi );
            QR = CR;
        }

        // turn right ( math negative, cw )
        //
        // AL---------------BL\
        // |        --------|  \
        // |--------        |    \
        // A----------------B----CL
        // |----        -  /    /
        // |    ----  -   /    /
        // AR-------SR   /    /
        //         /    /    /
        //        /    /    /
        //       /    /    /
        //      /    /    /
        //     DR---C----DL
        //
        // SR = B + 0.5 ( nAB + nBC );
        // BL = B - nBC;
        // CL = B - nAB;
        //
        else
        {
#ifdef USE_POLYLINE_DEBUG_LOGGING
            std::cout << "RIGHT turn with phi(" << Utils::toDegrees( phi ) << "), ";
#endif
#ifdef USE_POLYLINE_VECTOR_ADDITION
            QR = B  + ( nAB + nBC ) * .5f; // right intersection point
#else
            // Compute RIGHT intersection ( TODO: Only compute this when using miter join )
            if ( IntersectionTool::intersectRay2D( S, A + nAB, B + nAB, B + nBC, C + nBC ) )
            {
                QR = S;
            }
#endif
            glm::vec2 const BL = B - nAB;
            glm::vec2 const CL = B - nBC;
            addHexagon( A, KL, KR, B, BL, QR, color );
            addLineJoin( dc, lineJoin, B, BL, CL, phi );
            QL = CL;
        }

#ifdef USE_POLYLINE_DEBUG_LOGGING
        std::cout << "A(" << A << "), B(" << B << "), C(" << C << "), "
                  << "KL(" << KL << "), KR(" << KR << "), QL(" << QL << "), QR(" << QR << ")\n";
#endif
        A = B;
        B = C;
        nAB = nBC;
        KL = QL;
        KR = QR;
        color = randomColor();
    }

    // handle END segment:
    QL = B - nAB;
    QR = B + nAB;
    addHexagon( A, KL, KR, B, QL, QR, Utils::varyColor( 0xFFFF4000, 50 ) ); // red end segment
    addLineCap( dc, lineCap, B, QL, QR, true );

#ifdef USE_POLYLINE_DEBUG_LOGGING
    std::cout << "EndSegment[" << n-1 - 1 << "/" << n-1 << "] "
              << "A(" << A << "), B(" << B << "), C(" << C << "), "
              << "KL(" << KL << "), KR(" << KR << "), QL(" << QL << "), QR(" << QR << ")\n";

    std::cout << "Discarded points = " << discardPoints << "\n";
#endif

}

#if 0

// Supports different line-joins and end-caps to comply with SVG standard.

void addPolyLine( PrimitiveRenderer & dc, std::vector< glm::vec2 > const & points, float32_t const lineWidth, LineJoin const lineJoin )
{
    // -----AL--------BL-----------     Dreiecke:   1.) A, AL, BL
    //     /       ---- \                           2.) A, BL, B
    //    /    ----      \    -n         CW         3.) A, B, BR
    //   / ----           \    |                    4.) A, BR, AR
    //  A------------------B-------
    //   \ ----           /    |
    //    \    ----      /    +n
    //     \       ---- /
    // -----AR--------BR-----------

    auto addHexagon = [&dc] (   glm::vec2 const A, glm::vec2 const AL, glm::vec2 const AR,
                                glm::vec2 const B, glm::vec2 const BL, glm::vec2 const BR,
                                uint32_t const color1, uint32_t const color2,
                                uint32_t const color3, uint32_t const color4 )
    {
        dc.addTriangle2D( A, AL, BL, color1 );
        dc.addTriangle2D( A, BL, B, color2 );
        dc.addTriangle2D( A, B, BR, color3 );
        dc.addTriangle2D( A, BR, AR, color4 );
    };

    //
    //
    //
    //
    //  A------------------B
    //
    //
    //
    //
    size_t const n = points.size();

    if ( n == 0 ) // Invalid polyline, nothing todo
    {
        // nothing todo, empty input
        // but processing 'it' was still ok.
        return;
    }
    else if ( n == 1 ) // A point ( invisible ) but with possible end-caps ( optional ).
    {
        // addEndCap();
        return;
    }
    else if ( n == 2 ) // Polyline with 1 segment ( visible ) with possible end-caps ( optional ).
    {
        // processPolyLineSegment( dc, prev, next, segmentType, lineWidth, lineJoin );
        // addEndCap();
        return;
    }

    // ... With (n >= 3) we have normal SVG Path compliant polyline with:
    //          + 1x StartSegment
    //          + 1x EndSegment
    //          + (n-3) x MiddleSegments
    //          + (n-2) x LineJoins ( supports SVG joins like miter, beveled ( enhanced ) or rounded )
    //          + 2x EndCaps ( optional )
    size_t const n-1 = n-1;

    //
    // Local variables:
    //
    glm::vec2 A,B,C;            // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 KL,KR,QL,QR;      // Fully describes current hexagon segment A,KL,KR,B,QL,QR
    glm::vec2 nAB, nBC;         // We also need normals for both line-segments AB and BC
    float32_t phi;              // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
    uint32_t color = 0xFFFFFFFF;// Only for debugging, TODO: remove in release mode

//    std::cout << "StartSegment[" << 0 << "/" << n-1 << "]: ";
//    color = varyColor( 0xFF2020AF, 50 ); // blue debug color

    // Loop over all MiddleSegments ( == n-3 = (n-1)-Start-End )  = pointCount - 1
    for ( size_t i = 0; i < n; ++i )
    {
#ifdef USE_POLYLINE_DEBUG_LOGGING
        if ( i == 0 )
        {
            std::cout << "StartSegment[" << 0 << "/" << n-1 << "]: ";
            color = varyColor( 0xFF2020AF, 50 ); // blue debug color
        }
        else if ( i >= n - 2 )
        {
            color = varyColor( 0xFFFF4000, 50 ); // red
            std::cout << "EndSegment[" << n-1 - 1 << "/" << n-1 << "] ";
        }
        else
        {
            //color = varyColor( 0xFF208F20, 50 ); // green
            color = randomColor();
            std::cout << "MiddleSegment[" << i << "/" << n-1 << "]: ";
        }

        uint32_t color2 = varyColor( color, 50 );
        uint32_t color3 = varyColor( color, 50 );
        uint32_t color4 = varyColor( color, 50 );
#endif
        A = points[ i ];                          // Fetch A
        B = points[ i+1 ];                          // Fetch B
        nAB = getNormal2D( A, B ) * lineWidth;  // Compute normal nAB

#if USE_LINEGEOMETRY_PROCESSING
        addHexagon( A, A - nAB, A + nAB, B, B - nAB, B + nAB, color, color2, color3, color4 );
#else
        if ( i == 0 )
        {
            KL = A - nAB;                            // specific for StartSegment
            KR = A + nAB;                            // specific for StartSegment
        }
        else if ( i == n - 2 )
        {
            QL = B - nAB;
            QR = B + nAB;
            // TODO: addLineEndCap();

            std::cout << "A(" << ::toString( A ) << "), "
                      << "B(" << ::toString( B ) << "), "
                      << "C(" << ::toString( C ) << "), "
                      << "KL(" << ::toString( KL ) << "), "
                      << "KR(" << ::toString( KR ) << "), "
                      << "QL(" << ::toString( QL ) << "), "
                      << "QR(" << ::toString( QR ) << ")\n";

            return;
        }


        // Big Step II: Add line-join, depends only on angle ( aka turn direction of the polyline )
        // Compute ANGLE ABC which is the angle between rays AB and BC
        // This is used to determine the SIDE OF the polyline's LINE-JOIN ( opposite direction to turn )
        C = points[ i+2 ];                          // Fetch C
        nBC = getNormal2D( B, C ) * lineWidth;  // Compute normal nBC
        phi = getAngleABC( A, B, C );

#ifdef USE_POLYLINE_VECTOR_ADDITION
        glm::vec2 const nSL = ( nAB + nBC ) * .5f;
#else
        glm::vec2 S;
#endif

        // turn left ( math positive, ccw )
        //
        //     DL---C----DR
        //      \    \    \
        //       \    \    \
        //        \    \    \
        //         \    \    \
        // AL-------SL   \    \
        // |    ----  -   \    \
        // |----        -  \    \
        // A----------------B----CR
        // |--------        |    /
        // |        --------|  /
        // AR---------------BR/
        //
        // SL = B - 0.5 ( nAB + nBC );
        // CR = B + nBC;
        // BR = B + nAB;
        //

        if ( phi < 180.0f )     // turn left ( math positive, ccw )
        {
            std::cout << "LEFT turn with phi(" << phi << "), ";
#ifdef USE_POLYLINE_VECTOR_ADDITION
            QL = B + nSL; // left intersection point
#else
            // Compute LEFT intersection ( TODO: Only compute this when using miter join )
            if ( IntersectionTool::intersectRay2D( S,
                                                   A - nAB,       // vector AB - n
                                                   B - nAB,       // vector AB - n
                                                   B - nBC,       // vector BC - n
                                                   C - nBC ) )    // vector BC - n
            {
                QL = S;
            }
#endif
            QR = B + nAB;

            addLineJoin( dc, lineJoin, B, B + nBC /* CR */, B + nAB /* BR */, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );

            addHexagon( A, KL, KR, B, QL, QR, color, color2, color3, color4 );

            QR = B + nBC;
        }

        //
        // turn right ( math negative, cw )
        //
        // AL---------------BL\
        // |        --------|  \
        // |--------        |    \
        // A----------------B----CL
        // |----        -  /    /
        // |    ----  -   /    /
        // AR-------SR   /    /
        //         /    /    /
        //        /    /    /
        //       /    /    /
        //      /    /    /
        //     DR---C----DL
        //
        // SR = B + 0.5 ( nAB + nBC );
        // BL = B - nBC;
        // CL = B - nAB;
        //
        else        // turn right ( math negative, cw )
        {
            std::cout << "RIGHT turn with phi(" << phi << "), ";
#ifdef USE_POLYLINE_VECTOR_ADDITION
            QR = B + nSL; // right intersection point
#else
            // Compute RIGHT intersection ( TODO: Only compute this when using miter join )
            if ( IntersectionTool::intersectRay2D( S,
                                                   A + nAB,       // vector AB - n
                                                   B + nAB,       // vector AB - n
                                                   B + nBC,       // vector BC - n
                                                   C + nBC ) )    // vector BC - n
            {
                QR = S;
            }
#endif
            QL = B - nAB;
            addLineJoin( dc, lineJoin, B, B - nAB /* BL */, B - nBC /* CL */, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
            addHexagon( A, KL, KR, B, QL, QR, color, color2, color3, color4 );
            QL = B - nBC;
        }

        std::cout << "A(" << ::toString( A ) << "), "
                  << "B(" << ::toString( B ) << "), "
                  << "C(" << ::toString( C ) << "), "
                  << "KL(" << ::toString( KL ) << "), "
                  << "KR(" << ::toString( KR ) << "), "
                  << "QL(" << ::toString( QL ) << "), "
                  << "QR(" << ::toString( QR ) << ")\n";
#endif
        A = B;
        B = C;
        nAB = nBC;
        KL = QL;
        KR = QR;
    }

}












void addPolyLine( PrimitiveRenderer & dc, std::vector< glm::vec2 > const & points, float32_t const lineWidth, LineJoin const lineJoin )
{
    //
    // -----AL--------BL-----------     Dreiecke:   1.) A, AL, BL
    //     /       ---- \                           2.) A, BL, B
    //    /    ----      \    +n         CW         3.) A, B, BR
    //   / ----           \    |                    4.) A, BR, AR
    //  A------------------B--------
    //   \ ----           /    |
    //    \    ----      /    -n
    //     \       ---- /
    // -----AR--------BR-----------

    auto addHexagon = [&dc] (   glm::vec2 const A, glm::vec2 const AL, glm::vec2 const AR,
                                glm::vec2 const B, glm::vec2 const BL, glm::vec2 const BR,
                                uint32_t const color1, uint32_t const color2,
                                uint32_t const color3, uint32_t const color4 )
    {
        dc.addTriangle2D( A, AL, BL, color1 );
        dc.addTriangle2D( A, BL, B, color2 );
        dc.addTriangle2D( A, B, BR, color3 );
        dc.addTriangle2D( A, BR, AR, color4 );
    };

{
    glm::vec2 p0 = points[ 0 ];
    glm::vec2 p1 = points[ 1 ];
    glm::vec2 p2 = points[ 2 ];
    glm::vec2 n = getNormal2D( p1-p0, p2- p1 ) * lineWidth;
    float32_t phi = getAngleABC( p0, p1, p2 );

    std::cout << "p0(" << toString( p0 ) << "), "
                 << "p1(" << toString( p1 ) << "),  "
                    << "p2(" << toString( p2 ) << "), "
                       << "n(" << toString( n ) << "), "
                          << "phi(" << phi << ")\n";

}

    struct Element_t
    {
        glm::vec2 M;    // Original center point A,B,C,...,N-1
        glm::vec2 n;    // Normal of vector (M,M+1)
        glm::vec2 L;    // stores point L = (M - n) to prevent numerical issues when recomputing for next segment
        glm::vec2 R;    // stores point R = (M + n) to prevent numerical issues when recomputing for next segment

        std::string toString() const {
            std::stringstream s;
            s << "M(" << ::toString( M ) << "), ";
            s << "n(" << ::toString( n ) << "), ";
            s << "L(" << ::toString( L ) << "), ";
            s << "R(" << ::toString( R ) << ")";
            return s.str();
        }
    };

    auto addPolyLineJoin = [&dc, lineJoin] ( Element_t const A, Element_t const B, Element_t const C )
    {

        // Big Step II: Add line-join, depends only on angle ( aka turn direction of the polyline )
        // Compute ANGLE ABC which is the angle between rays AB and BC
        // This is used to determine the SIDE OF the polyline's LINE-JOIN ( opposite direction to turn )
        float32_t const phi = getAngleABC( A.M, B.M, C.M );

        if ( phi < 180.0f )     // turn left ( math positive, ccw )
        {
            std::cout << "LEFT turn with phi(" << phi << "), ";
            addLineJoin( dc, lineJoin, B.M, B.M + B.n /* CR */, B.M + A.n /* BR */, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
        }
        else        // turn right ( math negative, cw )
        {
            std::cout << "RIGHT turn with phi(" << phi << "), ";
            addLineJoin( dc, lineJoin, B.M, B.M - A.n, B.M - B.n, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
        }
    };


    auto intersectBorders = [&dc, lineJoin] ( Element_t const A, Element_t & B, Element_t const C )
    {
    #if 0
        glm::vec2 S;

        // Compute LEFT intersection ( TODO: Only compute this when using miter join )
        if ( IntersectionTool::intersectRay2D( S,
                                               A.M - A.n,       // vector AB - n
                                               B.M - A.n,       // vector AB - n
                                               B.M - B.n,       // vector BC - n
                                               C.M - B.n ) )    // vector BC - n
        {
            B.L = S;
        }

        // Compute RIGHT intersection ( TODO: Only compute this when using miter join )
        if ( IntersectionTool::intersectRay2D( S,
                                               A.M + A.n,       // vector AB - n
                                               B.M + A.n,       // vector AB - n
                                               B.M + B.n,       // vector BC - n
                                               C.M + B.n ) )    // vector BC - n
        {
            B.R = S;
        }
    #else

    #endif
    };

    // constexpr static float32_t const CONST_POLYLINE_ABCD_ANGLE_STEP = 10.0f; // degrees

    size_t const n = points.size();

    // Supports different line-joins and end-caps to comply with SVG standard.

    if ( n == 0 ) // Invalid polyline, nothing todo
    {
        // nothing todo, empty input
        // but processing 'it' was still ok.
        return;
    }
    else if ( n == 1 ) // A point ( invisible ) but with possible end-caps ( optional ).
    {
        // addEndCap();
        return;
    }
    else if ( n == 2 ) // Polyline with 1 segment ( visible ) with possible end-caps ( optional ).
    {
        // processPolyLineSegment( dc, prev, next, segmentType, lineWidth, lineJoin );
        // addEndCap();
        return;
    }

    // ... With (n >= 3) we have normal SVG Path compliant polyline with:
    //          + 1x StartSegment
    //          + 1x EndSegment
    //          + (n-3) x MiddleSegments
    //          + (n-2) x LineJoins ( supports SVG joins like miter, beveled ( enhanced ) or rounded )
    //          + 2x EndCaps ( optional )

    size_t const n-1 = n-1;

    Element_t A; // one element is sufficient for a point
    Element_t B; // two elements are sufficient for a simple line-segment
    Element_t C; // three elements are sufficient for any real polyline ( even for non-const lineWidth )

    uint32_t color = 0xFFFFFFFF; // for debug purpose

    //**********************
    //  process START Segment
    //**********************
    std::cout << "StartSegment[" << 0 << "/" << n-1 << "]:\n";

    color = varyColor( 0xFF20208F, 50 ); // blue debug color

    A.M = points[ 0 ];                          // Fetch A
    B.M = points[ 1 ];                          // Fetch B
    C.M = points[ 2 ];                          // Fetch C

    A.n = getNormal2D( A.M, B.M ) * lineWidth;  // Compute normal nAB
    B.n = getNormal2D( B.M, C.M ) * lineWidth;  // Compute normal nBC

    A.L = A.M - A.n;                            // specific for StartSegment
    A.R = A.M + A.n;                            // specific for StartSegment

    // Big Step II: Add line-join, depends only on angle ( aka turn direction of the polyline )
    // Compute ANGLE ABC which is the angle between rays AB and BC
    // This is used to determine the SIDE OF the polyline's LINE-JOIN ( opposite direction to turn )
    float32_t const phi = getAngleABC( A.M, B.M, C.M );
    glm::vec2 const nAB = ( A.n + B.n ) * .5f;

    if ( phi < 180.0f )     // turn left ( math positive, ccw )
    {
        std::cout << "LEFT turn with phi(" << phi << "), ";
        B.L = B.M - nAB; // left intersection point
        addLineJoin( dc, lineJoin, B.M, B.M + B.n /* CR */, B.M + A.n /* BR */, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
    else        // turn right ( math negative, cw )
    {
        std::cout << "RIGHT turn with phi(" << phi << "), ";
        B.R = B.M + nAB; // right intersection point
        addLineJoin( dc, lineJoin, B.M, B.M - A.n, B.M - B.n, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }

    addHexagon( A.M, A.L, A.R, B.M, B.L, B.R, color, varyColor( color, 20 ), varyColor( color, 20 ), varyColor( color, 20 ) ); // Doing this is the same as the complete LineGeometryProcessor does ( and its nothing )

    std::cout << "A={ " << A.toString() << " }\n"
              << "B={ " << B.toString() << " }\n"
              << "C={ " << C.toString() << " }\n";


    // Loop over all MiddleSegments ( == n-3 = (n-1)-Start-End )  = pointCount - 1
    for ( size_t i = 2; i < n-1-1; ++i )
    {
        A = B;
        B = C;

        color = varyColor( 0xFF208F20, 100 ); // green
        std::cout << "MiddleSegment[" << i << "/" << n-1 << "]:\n";

        //B.M = points[ i ];                      // Fetch C
y<        C.M = points[ i ];                      // Fetch C
        B.n = getNormal2D( B.M, C.M ) * lineWidth;  // Compute normal nBC

        // Big Step II: Add line-join, depends only on angle ( aka turn direction of the polyline )
        // Compute ANGLE ABC which is the angle between rays AB and BC
        // This is used to determine the SIDE OF the polyline's LINE-JOIN ( opposite direction to turn )
        float32_t const phi = getAngleABC( A.M, B.M, C.M );
        glm::vec2 const nAB = ( A.n + B.n ) * .5f;

        if ( phi < 180.0f )     // turn left ( math positive, ccw )
        {
            std::cout << "LEFT turn with phi(" << phi << "), ";
            B.L = B.M - nAB; // left intersection point
            addLineJoin( dc, lineJoin, B.M, B.M + B.n /* CR */, B.M + A.n /* BR */, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
        }
        else        // turn right ( math negative, cw )
        {
            std::cout << "RIGHT turn with phi(" << phi << "), ";
            B.R = B.M + nAB; // right intersection point
            addLineJoin( dc, lineJoin, B.M, B.M - A.n, B.M - B.n, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
        }

        // Doing this is the same as the complete LineGeometryProcessor does ( and its nothing )
        addHexagon( A.M, A.L, A.R, B.M, B.L, B.R, color, varyColor( color, 20 ), varyColor( color, 20 ), varyColor( color, 20 ) );
        addPolyLineJoin( A, B, C );

        std::cout << "A={ " << A.toString() << " }\n"
                  << "B={ " << B.toString() << " }\n"
                  << "C={ " << C.toString() << " }\n";
    }

    color = varyColor( 0xFFFF0000, 50 ); // red
    std::cout << "EndSegment[" << n-1 - 1 << "/" << n-1 << "] ";

    C.L = C.M - B.n;
    C.R = C.M + B.n;
    addHexagon( B.M, B.L, B.R, C.M, C.L, C.R, color, varyColor( color, 20 ), varyColor( color, 20 ), varyColor( color, 20 ) );
    // TODO: addLineEndCap();

}

glm::vec2 S;

        // Compute LEFT intersection ( TODO: Only compute this when using miter join )
        if ( IntersectionTool::intersectRay2D( S,
                                               A.M - A.n,       // vector AB - n
                                               B.M - A.n,       // vector AB - n
                                               B.M - B.n,       // vector BC - n
                                               C.M - B.n ) )    // vector BC - n
        {
            B.L = S;
        }

        // Compute RIGHT intersection ( TODO: Only compute this when using miter join )
        if ( IntersectionTool::intersectRay2D( S,
                                               A.M + A.n,       // vector AB - n
                                               B.M + A.n,       // vector AB - n
                                               B.M + B.n,       // vector BC - n
                                               C.M + B.n ) )    // vector BC - n
        {
            B.R = S;
        }


        // Big Step I: Add the current hexagon segment AB as 4 triangles
        addHexagon( A.M, A.L, A.R, B.M, B.L, B.R, color, varyColor( color, 20 ), varyColor( color, 20 ), varyColor( color, 20 ) );


// turn left ( math positive, ccw )
//
//     DL---C----DR
//      \    \    \
//       \    \    \
//        \    \    \
//         \    \    \
// AL-------SL   \    \
// |    ----  -   \    \
// |----        -  \    \
// A----------------B----CR
// |--------        |    /
// |        --------|  /
// AR---------------BR/
//

// turn right ( math negative, cw )
//
// AL---------------BL\
// |        --------|  \
// |--------        |    \
// A----------------B----CL
// |----        -  /    /
// |    ----  -   /    /
// AR-------SR   /    /
//         /    /    /
//        /    /    /
//       /    /    /
//      /    /    /
//     DR---C----DL
//


void addPolyLineSegment(    PrimitiveRenderer & dc,
                            PolyLineSegmentMetaData_t & curr,
                            PolyLineSegmentMetaData_t & next,
                            uint8_t segmentType,
                            float32_t const lineWidth,
                            LineJoin const lineJoin,
                            int idx )
{
    glm::vec2 const A( curr.A );
    glm::vec2 const B( curr.B );
    glm::vec2 const C( next.B );
    float32_t const phi = getAngleABC( A, B, C );

    curr.n = getNormal2D( A, B ) * lineWidth;
    curr.AL = curr.A - curr.n;
    curr.AR = curr.A + curr.n;
    curr.BL = curr.B - curr.n;
    curr.BR = curr.B + curr.n;

    if ( segmentType == 2 /* handle end segment and exit asap. */ )
    {
        // turn left ( math positive, ccw )
        if ( phi < 180.0f )
        {
            dc.addTriangle2D( curr.A, curr.AL, curr.SL, 0xFF208F20 );
            dc.addTriangle2D( curr.A, curr.SL, curr.B, 0xFF408F40 );
            dc.addTriangle2D( curr.A, curr.B, curr.BR, 0xFF608F60 );
            dc.addTriangle2D( curr.A, curr.BR, curr.AR, 0xFF808F80 );

            // End Segment
            //dc.addTriangle2D( curr.B, curr.SL, next.BL, 0xFFFF2020 );
            //dc.addTriangle2D( curr.B, curr.BL, next.B, 0xFFFF4040 );
            //dc.addTriangle2D( curr.B, next.A, next.BR, 0xFFFF6060 );
            //dc.addTriangle2D( curr.B, next.BR, next.AR, 0xFFFF8080 );

            addLineJoin( dc, lineJoin, curr.B, next.AR, curr.BR, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
        }
        // turn right ( math negative, cw )
        else
        {
            dc.addTriangle2D( curr.A, curr.AL, curr.BL, 0xFF208F20 );
            dc.addTriangle2D( curr.A, curr.BL, curr.B, 0xFF408F40 );
            dc.addTriangle2D( curr.A, curr.B, curr.AR, 0xFF608F60 );
            dc.addTriangle2D( curr.AR, curr.B, curr.SR, 0xFF808F80 );

            // End Segment
            //dc.addTriangle2D( curr.B, curr.AL, next.BL, 0xFFFF2020 );
            //dc.addTriangle2D( curr.B, next.BL, next.A, 0xFFFF4040 );
            //dc.addTriangle2D( curr.B, next.A, next.BR, 0xFFFF6060 );
            //dc.addTriangle2D( curr.B, next.BR, curr.SR, 0xFFFF8080 );

            addLineJoin( dc, lineJoin, curr.B, curr.BL, next.AL, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
        }

    }

    next.n = getNormal2D( B, C ) * lineWidth;
    next.AL = next.A - next.n;
    next.AR = next.A + next.n;
    next.BL = next.B - next.n;
    next.BR = next.B + next.n;

    curr.has_SL = IntersectionTool::intersectRay2D( curr.SL, curr.AL, curr.BL, next.AL, next.BL );
    curr.has_SR = IntersectionTool::intersectRay2D( curr.SR, curr.AR, curr.BR, next.AR, next.BR );

    #ifdef USE_POLYLINE_ABC_DEBUG_LOGGING
    std::cout << __FUNCTION__ << "(" << toString( A ) << "; " << toString( B ) << "; " << toString( C ) << ")\n";
    std::cout << " phi(" << phi << ")\n";
    std::cout << " n1(" << toString( n1 ) << ")\n";
    std::cout << " n2(" << toString( n2 ) << ")\n";
    std::cout << " AL(" << toString( AL ) << "), BL(" << toString( BL ) << "), CL(" << toString( CL ) << "), DL(" << toString( DL ) << ")\n";
    std::cout << " AR(" << toString( AR ) << "), BR(" << toString( BR ) << "), CR(" << toString( CR ) << "), DR(" << toString( DR ) << ")\n";
    std::cout << " SL(" << toString( SL ) << ")\n";
    std::cout << " SR(" << toString( SR ) << ")\n";
    #endif

    if ( curr.has_SL && curr.has_SR )
    {
        // std::cout << "[Debug] " << __FUNCTION__ << "() :: [benni] Segment[" << idx << "] has 2 intersections with next L(" << curr.SL << "), R(" << curr.SR << ")\n";
    }
    else
    {
        if ( !curr.has_SL )
        {
            std::cout << "[Error] " << __FUNCTION__ << "() :: [benni] Segment[" << idx << "] missing left intersection\n";
        }
        if ( !curr.has_SR )
        {
            std::cout << "[Error] " << __FUNCTION__ << "() :: [benni] Segment[" << idx << "] missing right intersection\n";
        }
        return;
    }

    float32_t const phi = getAngleABC( A, B, C );

    // turn left ( math positive, ccw )
    if ( phi < 180.0f )
    {
        dc.addTriangle2D( curr.A, curr.AL, curr.SL, 0xFF208F20 );
        dc.addTriangle2D( curr.A, curr.SL, curr.B, 0xFF408F40 );
        dc.addTriangle2D( curr.A, curr.B, curr.BR, 0xFF608F60 );
        dc.addTriangle2D( curr.A, curr.BR, curr.AR, 0xFF808F80 );

        // End Segment
        //dc.addTriangle2D( curr.B, curr.SL, next.BL, 0xFFFF2020 );
        //dc.addTriangle2D( curr.B, curr.BL, next.B, 0xFFFF4040 );
        //dc.addTriangle2D( curr.B, next.A, next.BR, 0xFFFF6060 );
        //dc.addTriangle2D( curr.B, next.BR, next.AR, 0xFFFF8080 );

        addLineJoin( dc, lineJoin, curr.B, next.AR, curr.BR, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
    // turn right ( math negative, cw )
    else
    {
        dc.addTriangle2D( curr.A, curr.AL, curr.BL, 0xFF208F20 );
        dc.addTriangle2D( curr.A, curr.BL, curr.B, 0xFF408F40 );
        dc.addTriangle2D( curr.A, curr.B, curr.AR, 0xFF608F60 );
        dc.addTriangle2D( curr.AR, curr.B, curr.SR, 0xFF808F80 );

        // End Segment
        //dc.addTriangle2D( curr.B, curr.AL, next.BL, 0xFFFF2020 );
        //dc.addTriangle2D( curr.B, next.BL, next.A, 0xFFFF4040 );
        //dc.addTriangle2D( curr.B, next.A, next.BR, 0xFFFF6060 );
        //dc.addTriangle2D( curr.B, next.BR, curr.SR, 0xFFFF8080 );

        addLineJoin( dc, lineJoin, curr.B, curr.BL, next.AL, phi, POLYLINE_COLOR_LINE_JOIN_BEVEL, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
}


///
/// Create PolyLine ABCD
//
//  L0---------------L1-------S1   Triangles: CW
//  |        --------|  \    /         Start-Segment:
//  |--------        |    \ /                  1.) A, AL,B2    P0, P0L, P1E1   P(k)[0], P(k)[1], P(k+1)E1
//  P0---------------P1----K1                  2.) A, B2,B     P0, P1E1, P1    P(k)[0], P(k+1)[2],
//  |       ----- - /|    /                    3.) A, B, AR    P0, P1, P0R
//  | -----    -   / |   /                     4.) AR,B, B1    P0R, P1, P1SR
//  R0------R1    /  |  /
//         /|    /   | /               Middle-Segment:
//        / |   /    |/                        1.) B, B3, C1   P1, P1E2, P2SL
//       /  |  /     L2----------DL            2.) B, C1, C    P1, P2SL, P2
//      /   | /   -       ------ |             3.) B, C, B1    P1, P2, P1SR
//     /    |/  -  ------        |             4.) B1, C, C2   P1SR, P2, P2E1
//    R2----P2-------------------P3
//   /  \   |----------          |     End-Segment:
//  /     \ |          ----------|             1.) C, C1, DL   P2, P2SR, P3L
// C4-------C3-------------------DR            2.) C, DL, D    P2, P3L, P3
//                                             3.) C, D, DR    P2, P3, P3R
//                                             4.) C, DR, C3   P2, P3R, P2E2
//

//

#define USE_POLYLINE_ABCD_DEBUG_LOGGING

inline bool
    addPolyLine(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const points,
        float32_t const lineWidth,
        LineJoin const lineJoin = LineJoin::BEVEL )
{
    constexpr static float32_t const CONST_POLYLINE_ABCD_ANGLE_STEP = 10.0f; // degrees

    int segmentType = 1; // StartSegment == 1
                        // MiddleSegment == 0 ( default )
                        // EndSegment == 2

    /// Loop over all segments S = N - 1
    for ( size_t seg = 0; seg < points.size() - 1; ++seg )
    {


    }

    glm::vec2 const A( points[ 0 ] );
    glm::vec2 const B( points[ 1 ] );
    glm::vec2 const C( points[ 2 ] );
    glm::vec2 const D( points[ 3 ] );

    float32_t const phi = getAngleABC( A, B, C );
    glm::vec2 const n1 = getNormal2D( A, B ) * lineWidth;
    glm::vec2 const n2 = getNormal2D( B, C ) * lineWidth;
    glm::vec2 const AL( A.x - n1.x, A.y - n1.y );
    glm::vec2 const BL( B.x - n1.x, B.y - n1.y );
    glm::vec2 const CL( B.x - n2.x, B.y - n2.y );
    glm::vec2 const DL( C.x - n2.x, C.y - n2.y );
    glm::vec2 const AR( A.x + n1.x, A.y + n1.y );
    glm::vec2 const BR( B.x + n1.x, B.y + n1.y );
    glm::vec2 const CR( B.x + n2.x, B.y + n2.y );
    glm::vec2 const DR( C.x + n2.x, C.y + n2.y );
    glm::vec2 SL;
    glm::vec2 SR;

    bool const hasL = IntersectionTool::intersectRay2D( SL, AL, BL, CL, DL );
    bool const hasR = IntersectionTool::intersectRay2D( SR, AR, BR, CR, DR );

#ifdef USE_POLYLINE_ABC_DEBUG_LOGGING
    std::cout << __FUNCTION__ << "(" << toString( A ) << "; " << toString( B ) << "; " << toString( C ) << ")\n";
    std::cout << " phi(" << phi << ")\n";
    std::cout << " n1(" << toString( n1 ) << ")\n";
    std::cout << " n2(" << toString( n2 ) << ")\n";
    std::cout << " AL(" << toString( AL ) << "), BL(" << toString( BL ) << "), CL(" << toString( CL ) << "), DL(" << toString( DL ) << ")\n";
    std::cout << " AR(" << toString( AR ) << "), BR(" << toString( BR ) << "), CR(" << toString( CR ) << "), DR(" << toString( DR ) << ")\n";
    std::cout << " SL(" << toString( SL ) << ")\n";
    std::cout << " SR(" << toString( SR ) << ")\n";
#endif

    if ( hasL && hasR )
    {
        std::cout << __FUNCTION__ << "() :: [benni] Segment exists SL,SR\n";
    }
    else
    {
        if ( !hasL )
        {
            std::cout << __FUNCTION__ << "() :: [benni] Segment SL does not exist\n";
        }
        if ( !hasR )
        {
            std::cout << __FUNCTION__ << "() :: [benni] Segment SR does not exist\n";
        }
        return false;
    }

    if ( phi < 180.0f )
    {
        // turn left ( math positive, ccw )
        //
        //     DL---C----DR
        //      \    \    \
        //       \    \    \
        //        \    \    \
        //         \    \    \
        // AL-------SL   \    \
        // |    ----  -   \    \
        // |----        -  \    \
        // A----------------B----CR
        // |--------        |    /
        // |        --------|  /
        // AR---------------BR/
        //

        dc.addTriangle2D( A, AL, SL, 0xFF208F20 );
        dc.addTriangle2D( A, SL, B, 0xFF408F40 );
        dc.addTriangle2D( A, B, BR, 0xFF608F60 );
        dc.addTriangle2D( A, BR, AR, 0xFF808F80 );

        dc.addTriangle2D( B, SL, DL, 0xFFFF2020 );
        dc.addTriangle2D( B, DL, C, 0xFFFF4040 );
        dc.addTriangle2D( B, C, DR, 0xFFFF6060 );
        dc.addTriangle2D( B, DR, CR, 0xFFFF8080 );

        //
        // Create Line Join
        //

        if ( lineJoin == LineJoin::BEVEL || lineJoin == LineJoin::ROUND )
        {
            if ( lineJoin == LineJoin::BEVEL )
            {
                // Add always the bevel triangle (B,CR,BR)
                dc.addTriangle2D( B, CR, BR, 0xFFFFBF40 );

                // Double Bevel: compute and check if another triangle fits
                glm::vec2 const BRCR = ( CR - BR );
                glm::vec2 const M = ( BR + ( CR - BR ) * 0.5f );
                glm::vec2 const MB = ( M-B );

                if ( 2.0f * glm::length( MB ) < glm::length( BRCR ) )
                {
                    // Add Double Bevel triangle
                    dc.addTriangle2D( CR, B + ( M-B ) * 2.0f, BR, 0xFFFFDF40 );
                }
            }
            else // if ( lineJoin == LineJoin::ROUND )
            {
                // addRoundJoin( dc, B, CR, BR, lineWidth );
                if ( phi >= 1.5f * CONST_POLYLINE_ABCD_ANGLE_STEP )
                {
                    const uint32_t steps = uint32_t( std::round( phi / CONST_POLYLINE_ANGLE_STEP ) );

                    for ( uint32_t i = 0; i < steps; ++i )
                    {

                    }
                }
                else
                {
                    // Add always the bevel triangle
                    dc.addTriangle2D( B, CR, BR, 0xFFFFBF40 );
                }

            }
        }
    }
    else // if ( phi >= 180.0f )
    {
        // turn right ( math negative, cw )
        //
        // AL---------------BL\
        // |        --------|  \
        // |--------        |    \
        // A----------------B----CL
        // |----        -  /    /
        // |    ----  -   /    /
        // AR-------SR   /    /
        //         /    /    /
        //        /    /    /
        //       /    /    /
        //      /    /    /
        //     DR---C----DL
        //

        dc.addTriangle2D( A, AL, BL, 0xFF208F20 );
        dc.addTriangle2D( A, BL, B, 0xFF408F40 );
        dc.addTriangle2D( A, B, AR, 0xFF608F60 );
        dc.addTriangle2D( AR, B, SR, 0xFF808F80 );

        dc.addTriangle2D( B, CL, DL, 0xFFFF2020 );
        dc.addTriangle2D( B, DL, C, 0xFFFF4040 );
        dc.addTriangle2D( B, C, DR, 0xFFFF6060 );
        dc.addTriangle2D( B, DR, SR, 0xFFFF8080 );

        if ( lineJoin == LineJoin::BEVEL || lineJoin == LineJoin::ROUND )
        {
            if ( lineJoin == LineJoin::BEVEL )
            {
                // Add always the bevel triangle (B,CR,BR)
                dc.addTriangle2D( B, BL, CL, 0xFFFFBF40 );

                // Double Bevel: compute and check if another triangle fits
                glm::vec2 const BLCL = ( CL - BL );
                glm::vec2 const M = ( BL + ( CL - BL ) * 0.5f );
                glm::vec2 const MB = ( M-B );

                if ( 2.0f * glm::length( MB ) < glm::length( BLCL ) )
                {
                    dc.addTriangle2D( BL, B + MB * 2.0f, CL, 0xFFFFDF40 );
                }
            }
            else // if ( lineJoin == LineJoin::ROUND )
            {
                // addRoundJoin( dc, B, BL, CL, lineWidth );
            }
        } // endif LineJoin
    }

    return true;
}


AutoMeshBuffer* createPolyLineMB( PolyLine in )
{
    AutoMeshBuffer* amb = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

    addPolyLine( amb->MeshBuffer, in.Points, in.LineWidth );
    return amb;
}


///
/// Create PolyLine
///
/// AL---------------BL\-----SBL
/// |        --------|  \   /
/// |--------        |    \/
/// A----------------B----BL
/// |----        -  /    /
/// |    ----  -   /    /
/// AR-------SBR  /    /
///         /    /    /
///        /    /    /
///       /    /    /
///      /    /    /
///     CR---C----CL
///

inline bool
mkPolyLine( PrimitiveRenderer & out, float32_t const lineWidth, std::vector< glm::vec2 > const points )
{
    std::cout << __FUNCTION__ << "():\n";

    if ( points.size() < 4 )
    {
        std::cout << __FUNCTION__ << "(" << points.size() << ") - Not enough points\n";
        return false;
    }

    /// PolyLine ABCD: StartSegment AB
    glm::vec2 A = points[ 0 ];
    glm::vec2 B = points[ 1 ];
    glm::vec2 C = points[ 2 ];
    glm::vec2 D = points[ 3 ];


    glm::vec2 AL = points[ 0 ];
    glm::vec2 B = points[ 1 ];


    size_t n = points.size();

    float32_t phi = irr::core::RADTODEG * ( atan2( C.y - B.y, C.x - B.x ) - atan2( B.y - A.y, B.x - A.x ) );

    if ( phi < 0.0f ) phi += 360.0f;
    std::cout << "phi = " << phi << "\n";

    glm::vec2 const nAB = getNormal2D( A, B ) * lineWidth;
    glm::vec2 const nBC = getNormal2D( B, C ) * lineWidth;

    glm::vec2 const A1 ( A.x - nAB.x, A.y - nAB.y );
    glm::vec2 const B1 ( B.x - nAB.x, B.y - nAB.y );
    glm::vec2 const B2 ( B.x - nBC.x, B.y - nBC.y );
    glm::vec2 const C1 ( C.x - nBC.x, C.y - nBC.y );

    glm::vec2 const A2 ( A.x + nAB.x, A.y + nAB.y );
    glm::vec2 const B3 ( B.x + nAB.x, B.y + nAB.y );
    glm::vec2 const B4 ( B.x + nBC.x, B.y + nBC.y );
    glm::vec2 const C2 ( C.x + nBC.x, C.y + nBC.y );

    glm::vec2 S_ABC_1;
    glm::vec2 S_ABC_2;

    if ( IntersectionTool::intersectRay2D( S_ABC_1, A1, B1, B2, C1 )
         && IntersectionTool::intersectRay2D( S_ABC_2, A2, B3, B4, C2 ) )
    {
    }
    else
    {
        std::cout << __FUNCTION__ << "() :: [benni] No intersection S_ABC found\n";
    }

    out.addTriangle2D( A, A1, B1, randomColor() );
    out.addTriangle2D( A, B1, B, randomColor() );
    out.addTriangle2D( A, B, B2, randomColor() );
    out.addTriangle2D( A, B2, A2, randomColor() );

    out.addLine2D( points[ 0 ], points[ 1 ], randomColor() ); // AB

    /// PolyLine ABCD: MiddleSegments BC
    for ( size_t i = 0; i < n - 3; ++i )
    {
        glm::vec2 A = points[ i ];
        glm::vec2 B = points[ i + 1 ];
        glm::vec2 C = points[ i + 2 ];
        glm::vec2 D = points[ i + 3 ];

        if ( i == 0 )
        {
            out.addLine2D( A, B, randomColor() );
        }
        if ( i == points.size() - 1 )
        {
            out.addLine2D( C, D, randomColor() );
        }
        else
        {
            out.addLine2D( A, B, randomColor() );
        }

        out.addLine2D( A, B, randomColor() );
        //out.addLine2D( B, C, randomColor() );

        //        float32_t phiAB = irr::core::RADTODEG * atan2( B.y - A.y, B.x - A.x );
//        float32_t phiBC = irr::core::RADTODEG * atan2( C.y - B.y, C.x - B.x );
//        float32_t phiABC = phiBC - phiAB;
//        if ( phiAB < 0.0f ) phiAB += 360.0f;
//        if ( phiBC < 0.0f ) phiBC += 360.0f;
//        if ( phiABC < 0.0f ) phiABC += 360.0f;
//        std::cout << "phiAB = " << phiAB << "\n";
//        std::cout << "phiBC = " << phiBC << "\n";
//        std::cout << "phiABC = " << phiABC << "\n";

        float32_t phi = irr::core::RADTODEG * ( atan2( C.y - B.y, C.x - B.x ) - atan2( B.y - A.y, B.x - A.x ) );

        if ( phi < 0.0f ) phi += 360.0f;
        std::cout << "phi = " << phi << "\n";

        glm::vec2 const nAB = getNormal2D( A, B ) * lineWidth;
        glm::vec2 const nBC = getNormal2D( B, C ) * lineWidth;

        glm::vec2 const A1 ( A.x - nAB.x, A.y - nAB.y );
        glm::vec2 const B1 ( B.x - nAB.x, B.y - nAB.y );
        glm::vec2 const B2 ( B.x - nBC.x, B.y - nBC.y );
        glm::vec2 const C1 ( C.x - nBC.x, C.y - nBC.y );

        glm::vec2 const A2 ( A.x + nAB.x, A.y + nAB.y );
        glm::vec2 const B3 ( B.x + nAB.x, B.y + nAB.y );
        glm::vec2 const B4 ( B.x + nBC.x, B.y + nBC.y );
        glm::vec2 const C2 ( C.x + nBC.x, C.y + nBC.y );

        glm::vec2 S_ABC_1;
        glm::vec2 S_ABC_2;

        if ( IntersectionTool::intersectRay2D( S_ABC_1, A1, B1, B2, C1 )
             && IntersectionTool::intersectRay2D( S_ABC_2, A2, B3, B4, C2 ) )
        {
        }
        else
        {
            std::cout << __FUNCTION__ << "() :: [benni] No intersection S_ABC found\n";
        }

        out.addTriangle2D( A, A1, B1, randomColor() );
        out.addTriangle2D( A, B1, B, randomColor() );
        out.addTriangle2D( A, B, B2, randomColor() );
        out.addTriangle2D( A, B2, A2, randomColor() );
    }

    /// PolyLine ABCD: EndSegments CD

    out.addLine2D( points[ n-2 ], points[ n-1 ], randomColor() ); // AB

    return true;
}
#endif
