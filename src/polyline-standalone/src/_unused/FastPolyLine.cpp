#include "FastPolyLine.hpp"

#include <de/hampe/polyline/LineJoin.hpp>
#include <de/hampe/polyline/LineCap.hpp>

#ifdef USE_POLYLINE_DEBUG_LOGGING
#   include <sstream>
#   include <iostream>
#endif

#define USE_POLYLINE_POINT_SKIPPING

void addFastBeveledPolyLineAsTriangleList(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth,
        uint32_t const color, // Only for debugging, TODO: remove in release mode
        uint32_t const colorL, // Only for debugging, TODO: remove in release mode
        uint32_t const colorR // Only for debugging, TODO: remove in release mode
        )
{
    // -----AL--------BL---CL-------     Dreiecke:   1.) turn left: AL, BL, A, B, AR, BR [, B, CR, CR] CW
    //     /       ---- \                           oder
    //    /    ----      \    -n                     2.) turn right: AL, AL, BL, A, B, AR, BR, BR, B, B, BL, CL, CL
    //   / ----           \    |
    //  A------------------B-------
    //   \ ----           /    |
    //    \    ----      /    +n
    //     \       ---- /
    // -----AR--------BR---CR-------

    auto addHexagon = [&dc] (   glm::vec2 const A, glm::vec2 const AL, glm::vec2 const AR,
                                glm::vec2 const B, glm::vec2 const BL, glm::vec2 const BR,
                                uint32_t const color, uint32_t const colorL, uint32_t const colorR )
    {
        uint32_t const n = dc.getVertexCount();
        dc.addVertex2D( A,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
        dc.addVertex2D( AL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
        dc.addVertex2D( AR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5

        dc.addVertex2D( B,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
        dc.addVertex2D( BL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
        dc.addVertex2D( BR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5

        dc.addIndex( n+1  ); // AL, BL, A, B, AR, BR
        dc.addIndex( n+4  );
        dc.addIndex( n  );
        dc.addIndex( n+3  );
        dc.addIndex( n+2  );
        dc.addIndex( n+5  );
    };

    // We can process all 'polyline' shapes,

    size_t const n = points.size();

    //      ( n == 0 ) for empty input we simply abort
    //      ( n == 1 ) for a single point we atleast create LineCaps
    //      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
    //      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin

//    if ( n == 0 )
//    {
//        return;// nothing todo, empty input
//    }
//    else if ( n == 1 ) // A single point ( invisible ) but with possible end-caps ( optional ).
//    {
//        glm::vec2 const A = points[ 0 ];
//        glm::vec2 const nA( 0, -1 );
//        addLineCap( dc, lineCap, A, A - nA, A + nA, false );
//        addLineCap( dc, lineCap, A, A - nA, A + nA, true );
//        return;
//    }

    //
    // Local variables:
    //
    glm::vec2 A,B,C;            // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 KL,KR,QL,QR;      // Fully describes current hexagon segment A,KL,KR,B,QL,QR
    glm::vec2 nAB, nBC;         // We also need normals for both line-segments AB and BC
    float32_t phi;              // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
    glm::vec2 S;

    // handle START segment:
    A = points[ 0 ];                            // Fetch A
    B = points[ 1 ];                            // Fetch B
    nAB = Utils::getNormal2D( B - A ) * lineWidth;      // Compute normal nAB
    KL = A - nAB;                               // specific for StartSegment
    KR = A + nAB;                               // specific for StartSegment
    QL = B - nAB;
    QR = B + nAB;

    //addLineCap( dc, LineCap::SQUARE, A, KL, KR, false );// Construct an EndCap for StartSegment
    {
        glm::vec2 const n = glm::normalize( B - A ) * lineWidth;
        glm::vec2 const EL( KL - n );
        glm::vec2 const ER( KR - n );
        dc.addTriangle2D( ER, EL, KL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
        dc.addTriangle2D( KL, KR, ER, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }

    // build START segment:
    size_t vertexCount = dc.getVertexCount();
    dc.addVertex2D( A,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
    dc.addVertex2D( KL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
    dc.addVertex2D( KR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5

    // handle MIDDLE segment(s):
    for ( size_t i = 1; i < n - 1; ++i )
    {
        vertexCount = dc.getVertexCount();

        C = points[ i+1 ];                      // Fetch C
        phi = Utils::getAngleABC( A, B, C );    // Compute angle between vector AB and BC

        // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.01f || std::abs( phi - 360.0f ) < 0.01f )
        {
            B = C;                                          // Skip current segment
            nAB = Utils::getNormal2D( B - A ) * lineWidth;  // Update normal nAB
            continue;
        }

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

        if ( phi < 180.0f )     // turn left ( math positive, ccw )
        {
            if ( IntersectionTool::intersectRay2D( S, A - nAB, B - nAB, B - nBC, C - nBC ) )
            {
                QL = S;
            }
            glm::vec2 const BR = B + nAB;
            glm::vec2 const CR = B + nBC;
            //addHexagon( A, KL, KR, B, QL, BR, color, colorL, colorR );
            dc.addVertex2D( B,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
            dc.addVertex2D( QL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
            dc.addVertex2D( QR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5
            //addLineJoin( dc, LineJoin::BEVEL, B, CR, BR, phi );
            dc.addTriangle2D( B, CR, BR, POLYLINE_COLOR_LINE_JOIN_BEVEL );
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
            // Compute RIGHT intersection ( TODO: Only compute this when using miter join )
            if ( IntersectionTool::intersectRay2D( S, A + nAB, B + nAB, B + nBC, C + nBC ) )
            {
                QR = S;
            }
            glm::vec2 const BL = B - nAB;
            glm::vec2 const CL = B - nBC;
            //addHexagon( A, KL, KR, B, BL, QR, color, colorL, colorR );
            dc.addVertex2D( B,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
            dc.addVertex2D( QL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
            dc.addVertex2D( QR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5
            //addLineJoin( dc, LineJoin::BEVEL, B, BL, CL, phi );
            dc.addTriangle2D( B, BL, CL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
            QL = CL;
        }
        // build hexagon // AL, BL, A, B, AR, BR
//        dc.addIndex( vertexCount+1-3  ); // AL, BL, A, B, AR, BR
//        dc.addIndex( vertexCount+1-3  ); // AL, BL, A, B, AR, BR

//        dc.addIndex( vertexCount+5-3  );
        dc.addIndex( vertexCount+1-3  );
        dc.addIndex( vertexCount+4-3  );
        dc.addIndex( vertexCount-3  );
        dc.addIndex( vertexCount+3-3  );
        dc.addIndex( vertexCount+2-3  );
        dc.addIndex( vertexCount+5-3  );
        dc.addIndex( vertexCount+5-3  );
        dc.addIndex( vertexCount+7-3  );

        // prepare NEXT:
        A = B;
        B = C;
        nAB = nBC;
        KL = QL;
        KR = QR;
    }

    // handle END segment:
    QL = B - nAB;
    QR = B + nAB;
    //addHexagon( A, KL, KR, B, QL, QR, color, colorL, colorR ); // red end segment
    dc.addVertex2D( B,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
    dc.addVertex2D( QL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
    dc.addVertex2D( QR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5
    dc.addIndex( vertexCount+1-3  );
    dc.addIndex( vertexCount+1-3  );
    dc.addIndex( vertexCount+4-3  );
    dc.addIndex( vertexCount-3  );
    dc.addIndex( vertexCount+3-3  );
    dc.addIndex( vertexCount+2-3  );
    dc.addIndex( vertexCount+5-3  );

    //addLineCap( dc, LineCap::SQUARE, B, QL, QR, true );
    {
        glm::vec2 const n = glm::normalize( B - A ) * lineWidth;
        glm::vec2 const EL( QL + n );
        glm::vec2 const ER( QR + n );
        dc.addTriangle2D( QR, QL, EL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
        dc.addTriangle2D( EL, ER, QR, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
}

void addFastBeveledPolyLineAsTriangles(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth,
        uint32_t const color // Only for debugging, TODO: remove in release mode
        )
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

//    if ( n == 0 )
//    {
//        return;// nothing todo, empty input
//    }
//    else if ( n == 1 ) // A single point ( invisible ) but with possible end-caps ( optional ).
//    {
//        glm::vec2 const A = points[ 0 ];
//        glm::vec2 const nA( 0, -1 );
//        addLineCap( dc, lineCap, A, A - nA, A + nA, false );
//        addLineCap( dc, lineCap, A, A - nA, A + nA, true );
//        return;
//    }

    //
    // Local variables:
    //
    glm::vec2 A,B,C;            // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 KL,KR,QL,QR;      // Fully describes current hexagon segment A,KL,KR,B,QL,QR
    glm::vec2 nAB, nBC;         // We also need normals for both line-segments AB and BC
    float32_t phi;              // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
    glm::vec2 S;

    // handle START segment:
    A = points[ 0 ];                            // Fetch A
    B = points[ 1 ];                            // Fetch B
    nAB = Utils::getNormal2D( B - A ) * lineWidth;      // Compute normal nAB
    KL = A - nAB;                               // specific for StartSegment
    KR = A + nAB;                               // specific for StartSegment

    //addLineCap( dc, LineCap::SQUARE, A, KL, KR, false );// Construct an EndCap for StartSegment
    {
        glm::vec2 const n = glm::normalize( B - A ) * lineWidth;
        glm::vec2 const EL( KL - n );
        glm::vec2 const ER( KR - n );
        dc.addTriangle2D( ER, EL, KL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
        dc.addTriangle2D( KL, KR, ER, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }

    // handle MIDDLE segment(s):
    for ( size_t i = 1; i < n - 1; ++i )
    {
        C = points[ i+1 ];                      // Fetch C
        phi = Utils::getAngleABC( A, B, C );    // Compute angle between vector AB and BC

        // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.01f || std::abs( phi - 360.0f ) < 0.01f )
        {
            B = C;                                          // Skip current segment
            nAB = Utils::getNormal2D( B - A ) * lineWidth;  // Update normal nAB
            continue;
        }

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

        if ( phi < 180.0f )     // turn left ( math positive, ccw )
        {
            if ( IntersectionTool::intersectRay2D( S, A - nAB, B - nAB, B - nBC, C - nBC ) )
            {
                QL = S;
            }
            glm::vec2 const BR = B + nAB;
            glm::vec2 const CR = B + nBC;
            addHexagon( A, KL, KR, B, QL, BR, color );
            //addLineJoin( dc, LineJoin::BEVEL, B, CR, BR, phi );
            dc.addTriangle2D( B, CR, BR, POLYLINE_COLOR_LINE_JOIN_BEVEL );
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
            // Compute RIGHT intersection ( TODO: Only compute this when using miter join )
            if ( IntersectionTool::intersectRay2D( S, A + nAB, B + nAB, B + nBC, C + nBC ) )
            {
                QR = S;
            }
            glm::vec2 const BL = B - nAB;
            glm::vec2 const CL = B - nBC;
            addHexagon( A, KL, KR, B, BL, QR, color );
            //addLineJoin( dc, LineJoin::BEVEL, B, BL, CL, phi );
            dc.addTriangle2D( B, BL, CL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
            QL = CL;
        }

        // prepare NEXT:
        A = B;
        B = C;
        nAB = nBC;
        KL = QL;
        KR = QR;
    }

    // handle END segment:
    QL = B - nAB;
    QR = B + nAB;
    addHexagon( A, KL, KR, B, QL, QR, color ); // red end segment

    //addLineCap( dc, LineCap::SQUARE, B, QL, QR, true );
    {
        glm::vec2 const n = glm::normalize( B - A ) * lineWidth;
        glm::vec2 const EL( QL + n );
        glm::vec2 const ER( QR + n );
        dc.addTriangle2D( QR, QL, EL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
        dc.addTriangle2D( EL, ER, QR, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
}

void addFastBeveledPolyLineAsIndexedTriangles(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth,
        uint32_t const color,   // Only for debugging, TODO: remove in release mode
        uint32_t const colorL,  // Only for debugging, TODO: remove in release mode
        uint32_t const colorR   // Only for debugging, TODO: remove in release mode
    )
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
                                uint32_t const color, uint32_t const colorL, uint32_t const colorR )
    {
        //        dc.addTriangle2D( A, AL, BL, debugColor );
        //        dc.addTriangle2D( A, BL, B, Utils::varyColor( debugColor, 50 ) );
        //        dc.addTriangle2D( A, B, BR, Utils::varyColor( debugColor, 50 ) );
        //        dc.addTriangle2D( A, BR, AR, Utils::varyColor( debugColor, 50 ) )
    };

    // We can process all 'polyline' shapes,

    size_t const n = points.size();

    //      ( n == 0 ) for empty input we simply abort
    //      ( n == 1 ) for a single point we atleast create LineCaps
    //      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
    //      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin

//    if ( n == 0 )
//    {
//        return;// nothing todo, empty input
//    }
//    else if ( n == 1 ) // A single point ( invisible ) but with possible end-caps ( optional ).
//    {
//        glm::vec2 const A = points[ 0 ];
//        glm::vec2 const nA( 0, -1 );
//        addLineCap( dc, lineCap, A, A - nA, A + nA, false );
//        addLineCap( dc, lineCap, A, A - nA, A + nA, true );
//        return;
//    }

    //
    // Local variables:
    //
    glm::vec2 A,B,C;            // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 KL,KR,QL,QR;      // Fully describes current hexagon segment A,KL,KR,B,QL,QR
    glm::vec2 nAB, nBC;         // We also need normals for both line-segments AB and BC
    float32_t phi;              // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
    glm::vec2 S;

    // handle START segment:
    A = points[ 0 ];                            // Fetch A
    B = points[ 1 ];                            // Fetch B
    nAB = Utils::getNormal2D( B - A ) * lineWidth;      // Compute normal nAB
    KL = A - nAB;                               // specific for StartSegment
    KR = A + nAB;                               // specific for StartSegment

    //addLineCap( dc, LineCap::SQUARE, A, KL, KR, false );// Construct an EndCap for StartSegment
    {
        glm::vec2 const n = glm::normalize( B - A ) * lineWidth;
        glm::vec2 const EL( KL - n );
        glm::vec2 const ER( KR - n );
        dc.addTriangle2D( ER, EL, KL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
        dc.addTriangle2D( KL, KR, ER, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }

    dc.addVertex2D( A,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0
    dc.addVertex2D( KL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 1
    dc.addVertex2D( KR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 2

    // handle MIDDLE segment(s):
    for ( size_t i = 1; i < n - 1; ++i )
    {
        uint32_t const v0 = dc.getVertexCount();

        C = points[ i+1 ];                      // Fetch C
        phi = Utils::getAngleABC( A, B, C );    // Compute angle between vector AB and BC

        // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.01f || std::abs( phi - 360.0f ) < 0.01f )
        {
            B = C;                                          // Skip current segment
            nAB = Utils::getNormal2D( B - A ) * lineWidth;  // Update normal nAB
            continue;
        }

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

        if ( phi < 180.0f )     // turn left ( math positive, ccw )
        {
            if ( IntersectionTool::intersectRay2D( S, A - nAB, B - nAB, B - nBC, C - nBC ) )
            {
                QL = S;
            }
////            glm::vec2 const BR = B + nAB;
////            glm::vec2 const CR = B + nBC;
//            //addHexagon( A, KL, KR, B, QL, BR, color, colorL, colorR );
//            dc.addVertex2D( A,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
//            dc.addVertex2D( AL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
//            dc.addVertex2D( AR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5
//            dc.addIndexedTriangle( v0 - 3, v0 + 1 - 3, v0 + 4 - 3  ); // Triangle A, AL, BL
//            dc.addIndexedTriangle( v0 - 3, v0 + 4 - 3, v0 + 3 - 3  ); // Triangle A, BL, B
//            dc.addIndexedTriangle( v0 - 3, v0 + 3 - 3, v0 + 5 - 3  ); // Triangle A, B, BR
//            dc.addIndexedTriangle( v0 - 3, v0 + 5 - 3, v0 + 2 - 3  ); // Triangle A, BR, AR

            //            glm::vec2 const BR = B + nAB;
            //            glm::vec2 const CR = B + nBC;
                        //addHexagon( A, KL, KR, B, QL, BR, color, colorL, colorR );
            dc.addVertex2D( A,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
            dc.addVertex2D( KL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
            dc.addVertex2D( KR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5
            dc.addIndexedTriangle( v0 - 3, v0 + 1 - 3, v0 + 4 - 3  ); // Triangle A, AL, BL
            dc.addIndexedTriangle( v0 - 3, v0 + 4 - 3, v0 + 3 - 3  ); // Triangle A, BL, B
            dc.addIndexedTriangle( v0 - 3, v0 + 3 - 3, v0 + 5 - 3  ); // Triangle A, B, BR
            dc.addIndexedTriangle( v0 - 3, v0 + 5 - 3, v0 + 2 - 3  ); // Triangle A, BR, AR

            //addLineJoin( dc, LineJoin::BEVEL, B, CR, BR, phi );
            //dc.addTriangle2D( B, CR, BR, color, colorR, colorR );
            //dc.addIndexedTriangle( v0 + 3 - 3, v0 + 8 - 3, v0 + 5 - 3  ); // Triangle B, CR = BR + 6, BR
            QR = B + nBC; // CR
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
            // Compute RIGHT intersection ( TODO: Only compute this when using miter join )
            if ( IntersectionTool::intersectRay2D( S, A + nAB, B + nAB, B + nBC, C + nBC ) )
            {
                QR = S;
            }
//            glm::vec2 const BL = B - nAB;
//            glm::vec2 const CL = B - nBC;
            //addHexagon( A, KL, KR, B, BL, QR, color, colorL, colorR );
            dc.addVertex2D( A,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
            dc.addVertex2D( KL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
            dc.addVertex2D( KR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5
            dc.addIndexedTriangle( v0 - 3, v0 + 1 - 3, v0 + 4 - 3  ); // Triangle A, AL, BL
            dc.addIndexedTriangle( v0 - 3, v0 + 4 - 3, v0 + 3 - 3  ); // Triangle A, BL, B
            dc.addIndexedTriangle( v0 - 3, v0 + 3 - 3, v0 + 5 - 3  ); // Triangle A, B, BR
            dc.addIndexedTriangle( v0 - 3, v0 + 5 - 3, v0 + 2 - 3  ); // Triangle A, BR, AR

            //addLineJoin( dc, LineJoin::BEVEL, B, BL, CL, phi );
            //dc.addTriangle2D( B, BL, CL, color, colorL, colorL );
            //dc.addIndexedTriangle( v0 + 3, v0 + 1, v0 + 7  ); // Triangle B, BL, CL = BL + 6
            QL = B - nBC; // BL
        }

        // prepare NEXT:
        A = B;
        B = C;
        nAB = nBC;
        KL = QL;
        KR = QR;
    }

    // handle END segment:
    QL = B - nAB;
    QR = B + nAB;
    addHexagon( A, KL, KR, B, QL, QR, color, colorL, colorR ); // red end segment

    //addLineCap( dc, LineCap::SQUARE, B, QL, QR, true );
    {
        glm::vec2 const n = glm::normalize( B - A ) * lineWidth;
        glm::vec2 const EL( QL + n );
        glm::vec2 const ER( QR + n );
        dc.addTriangle2D( QR, QL, EL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
        dc.addTriangle2D( EL, ER, QR, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
}
