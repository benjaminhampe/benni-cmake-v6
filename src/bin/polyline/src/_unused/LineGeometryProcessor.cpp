#include "LineGeometryProcessor.hpp"

#include <de/hampe/polyline/PolyLineCommon.hpp>


void
LineGeometryProcessor::asTriangles(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth )
{
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
    glm::vec2 nAB, nBC;         // We also need normals for both line-segments AB and BC
    float32_t phi;              // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
    uint32_t processedSegments = 0;

    // handle START segment
    A = points[ 0 ];    // Fetch A
    B = points[ 1 ];    // Fetch B
    nAB = Utils::getNormal2D( B - A ) * lineWidth;      // Compute normal nAB

    // Add first 2 vertices of start segment [0]
    dc.addVertex2D( A - nAB, randomColor(), glm::vec2( 0.f, 0.f ) );    // AL
    dc.addVertex2D( A + nAB, randomColor(), glm::vec2( 0.f, 1.f ) );    // AR

    // handle MIDDLE segment(s):
    for ( size_t i = 1; i < n - 1; ++i )
    {
        C = points[ i + 1 ];                    // Fetch C
        phi = Utils::getAngleABC( A, B, C );    // Compute angle between vector AB and BC

        // if (parallel) then combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.00001f || std::abs( phi - Utils::TWO_PI ) < 0.00001f )
        {
            B = C;                                          // Skip current segment
            nAB = Utils::getNormal2D( B - A ) * lineWidth;  // Update normal nAB
            continue;
        }

        // ... continue computations ...
        nBC = Utils::getNormal2D( C - B ) * lineWidth;  // Compute normal nBC

        dc.addVertex2D( B - nAB, randomColor(), glm::vec2( 0.f, 0.f ) );    // BL
        dc.addVertex2D( B + nAB, randomColor(), glm::vec2( 0.f, 1.f ) );    // BR
        // ... now the last segment [i-1] has all 4 vertices and
        // then we also push the first 2 vertices of the current segment [i] ...
        dc.addVertex2D( B - nBC, randomColor(), glm::vec2( 0.f, 0.f ) );    // CL
        dc.addVertex2D( B + nBC, randomColor(), glm::vec2( 0.f, 1.f ) );    // CR

        uint32_t const iAL = 4 * processedSegments;
        dc.addIndexedTriangle( iAL, iAL + 3, iAL + 1 );      // AL, BR, AR
        dc.addIndexedTriangle( iAL, iAL + 2, iAL + 3 );      // AL, BL, BR

        // turn left: ( math positive, ccw )
        //
        // -AL----BL                A,B are actually not stored as vertices
        //  |\    |\
        //  | \   | \
        //  A  \  B  \
        //  |   \ |   \
        //  |    \|    \
        // -AR----BR----CR
        //
        if ( phi <= Utils::PI )
        {
            std::cout << "Segment[" << i << "] turns left with phi(" << Utils::toDegrees( phi ) << ")\n";
            dc.addIndexedTriangle( iAL + 2, iAL + 5, iAL + 3 );  // BL, CR, BR
        }

        // turn right: ( math negative, cw )
        //
        // -AL----BL----CL
        //  |\    |    /
        //  | \   |   /
        //  A  \  B  /
        //  |   \ | /
        //  |    \|/
        // -AR----BR
        //
        else
        {
            std::cout << "Segment[" << i << "] turns right with phi(" << Utils::toDegrees( phi ) << ")\n";
            dc.addIndexedTriangle( iAL + 2, iAL + 4, iAL + 3 );  // BL, CL, BR
        }

        // prepare NEXT:
        A = B;
        B = C;
        nAB = nBC;
        ++processedSegments;
    }

    // ... now add the last two missing vertices of the current [i] segment ...
    dc.addVertex2D( B - nAB, randomColor(), glm::vec2( 0.5f, 0.0f ) ); // DL
    dc.addVertex2D( B + nAB, randomColor(), glm::vec2( 0.5f, 1.0f ) ); // DR
    uint32_t const iAL = 4 * processedSegments;
    dc.addIndexedTriangle( iAL, iAL + 3, iAL + 1 );      // AL, BR, AR
    dc.addIndexedTriangle( iAL, iAL + 2, iAL + 3 );      // AL, BL, BR

#if 0
    //addLineCap( dc, LineCap::SQUARE, B, QL, QR, true );
    {
        glm::vec2 const n = glm::normalize( B - A ) * lineWidth;
        glm::vec2 const EL( QL + n );
        glm::vec2 const ER( QR + n );
        dc.addTriangle2D( QR, QL, EL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
        dc.addTriangle2D( EL, ER, QR, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
#endif
}



void
LineGeometryProcessor::asTriangleList(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth )
{
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
    glm::vec2 nAB, nBC;         // We also need normals for both line-segments AB and BC
    float32_t phi;              // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute

    // handle START segment
    A = points[ 0 ];    // Fetch A
    B = points[ 1 ];    // Fetch B
    nAB = Utils::getNormal2D( B - A ) * lineWidth;      // Compute normal nAB
    dc.addVertex2D( A - nAB, randomColor(), glm::vec2( 0.f, 0.f ) );    // AL
    dc.addVertex2D( A + nAB, randomColor(), glm::vec2( 0.f, 1.f ) );    // AR

    // handle MIDDLE segment(s):
    for ( size_t i = 1; i < n - 1; ++i )
    {
        C = points[ i + 1 ];      // Fetch C
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

        dc.addVertex2D( B - nAB, randomColor(), glm::vec2( 0.f, 0.f ) );    // BL
        dc.addVertex2D( B + nAB, randomColor(), glm::vec2( 0.f, 1.f ) );    // BR
        dc.addVertex2D( B - nBC, randomColor(), glm::vec2( 0.f, 0.f ) );    // CL
        dc.addVertex2D( B + nBC, randomColor(), glm::vec2( 0.f, 1.f ) );    // CR

        // turn left: ( math positive, ccw )
        //
        // -AL----BL     Dreiecke: AR, AL, BR, BL, CR
        //  |\    |\
        //  | \   | \               AR, AL, BR
        //  A  \  B  \              AL, BR, BL
        //  |   \ |   \             BR, BL, CR -> CR is start of next segment
        //  |    \|    \
        // -AR----BR----CR
        //
        if ( phi < 180.0f )
        {
            dc.addIndex( 2 * i - 1  );      // AR = BL - 1
            dc.addIndex( 2 * i - 2  );      // AL = BL - 2
            dc.addIndex( 2 * i + 1  );      // BR = BL + 1
            dc.addIndex( 2 * i );           // BL at current i
            dc.addIndex( 2 * i + 3  );      // CR = BL + 3
        }

        // turn right: ( math negative, cw )
        //
        // -AL----BL----CL   Dreiecke: AR, AL, BR, BL, CL, CL, CR
        //  |\    |    /
        //  | \   |   /
        //  A  \  B  /
        //  |   \ | /
        //  |    \|/
        // -AR----BR
        //
        else
        {
            dc.addIndex( 2 * i - 1  );      // AR = BL - 1
            dc.addIndex( 2 * i - 2  );      // AL = BL - 2
            dc.addIndex( 2 * i + 1  );      // BR = BL + 1
            dc.addIndex( 2 * i );           // BL at current i
            dc.addIndex( 2 * i + 2  );      // CL = BL + 2
//            dc.addIndex( 2 * i + 2  );      // CL = BL + 2
//            dc.addIndex( 2 * i + 3  );      // CR = BL + 3
        }

        // prepare NEXT:
        A = B;
        B = C;
        nAB = nBC;
    }

    dc.addVertex2D( C - nBC, randomColor(), glm::vec2( 0.5f, 0.0f ) ); // DL
    dc.addVertex2D( C + nBC, randomColor(), glm::vec2( 0.5f, 1.0f ) ); // DR

    dc.addIndex( 2 * n - 3  );      // CR
    dc.addIndex( 2 * n - 4  );      // CL
    dc.addIndex( 2 * n - 1  );      // DR
    dc.addIndex( 2 * n - 2  );      // DL

#if 0
    //addLineCap( dc, LineCap::SQUARE, B, QL, QR, true );
    {
        glm::vec2 const n = glm::normalize( B - A ) * lineWidth;
        glm::vec2 const EL( QL + n );
        glm::vec2 const ER( QR + n );
        dc.addTriangle2D( QR, QL, EL, POLYLINE_COLOR_LINE_JOIN_BEVEL );
        dc.addTriangle2D( EL, ER, QR, POLYLINE_COLOR_LINE_JOIN_DOUBLE_BEVEL );
    }
#endif
}

