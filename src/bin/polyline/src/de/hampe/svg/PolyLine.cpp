#include <de/hampe/svg/PolyLine.hpp>

#if 0

// ===========================================================================

template <  bool CLOCK_WISE,
            bool USE_POINT_SKIPPING,
            bool USE_VECTOR_ADD,
        typename FNI_GET_POINT_2D,
        typename FNI_GET_POINTCOUNT>
static bool
asIndexedTriangles(
    PrimitiveRenderer2D & dc,
    FNI_GET_POINT_2D    getPoint2D,
    FNI_GET_POINTCOUNT  getPointCount,
    float32_t const     lineWidth,
    LineCap const       lineCap = LineCap::SQUARE,
    LineJoin const      lineJoin = LineJoin::BEVEL )
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
        if ( CLOCK_WISE )
        {
            dc.addTriangle( A, AL, BL, debugColor );
            dc.addTriangle( A, BL, B, varyColor( debugColor, 100 ) );
            dc.addTriangle( A, B, BR, varyColor( debugColor, 100 ) );
            dc.addTriangle( A, BR, AR, varyColor( debugColor, 100 ) );
        }
        else // better ( right turn of triangles )
        {
            dc.addTriangle( A, AR, BR, debugColor );
            dc.addTriangle( A, BR, B, varyColor( debugColor, 100 ) );
            dc.addTriangle( A, B, BL, varyColor( debugColor, 100 ) );
            dc.addTriangle( A, BL, AL, varyColor( debugColor, 100 ) );
        }
    };

    size_t const pointCount = getPointCount();

    // nothing todo, empty input ( d.k. if i should return 'true' here )
    if ( pointCount == 0 )
    {
        return false;
    }
    // Contruct A single point ( invisible ) but with possible end-caps ( optional, can be made visible / turned on ).
    // This is most probably not what the caller wanted, so we return false here.
    // It would be better to collect all single points and recall this function.
    // The 'false' says 'collect single points' to the caller
    else if ( pointCount == 1 )
    {
        glm::vec2 const A = getPoint2D( 0 );
        glm::vec2 const nA( 0, -1 );
        addStartLineCap( dc, lineCap, A, A - nA, A + nA );
        addEndLineCap( dc, lineCap, A, A - nA, A + nA );
        return false;
    }
    // Two points make up a single segment AB ( visible ) with possible end-caps ( optional ).
//    else if ( pointCount == 2 )
//    {
//        glm::vec2 A,B,AL,AR,BL,BR;            // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
//        glm::vec2 nAB;         // We also need normals for both line-segments AB and BC
//        A = getPoint2D( 0 );                            // Fetch A
//        B = getPoint2D( 1 );                            // Fetch B
//        nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
//        AL = A - nAB;
//        AR = A + nAB;
//        BL = B - nAB;
//        BR = B + nAB;
//        addStartLineCap( dc, lineCap, A, AL, AR );      // Build LineCap for start-segment
//        addHexagon( A, AL, AR, B, BL, BR, randomColor() );
//        addEndLineCap( dc, lineCap, B, BL, BR ); // Build LineCap for Start-Segment
//        return true;
//    }

    //
    // Local variables:
    //
    float64_t accumLength,accumLengthL,accumLengthR;
    glm::vec2 A,B,C;
    glm::vec2 AL,AR,BL,BR,CL,CR,S;         // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 nAB,nBC,nABC;         // We also need normals for both line-segments AB and BC
    float32_t phi;      // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
    uint32_t color = randomColor();

    //
    // Start segment:
    //
    A = getPoint2D( 0 );                            // Fetch A
    B = getPoint2D( 1 );                            // Fetch B
    accumLength= glm::length( B - A );
    accumLengthL= accumLength;
    accumLengthR= accumLength;
    nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
    AL = A - nAB;
    AR = A + nAB;
    BL = B - nAB;
    BR = B + nAB;
    addStartLineCap( dc, lineCap, A, AL, AR );      // Build LineCap for start-segment
    color = 0xFF0000FF; // start segment ( blue )

    //
    // Middle segments:
    //

    for ( size_t i = 1; i < pointCount - 1; ++i )
    {

        // Fetch C
        C = getPoint2D( i + 1 );

        // Used to determine direction (left/right) of polyline
        phi = Math::getAngleABC( A, B, C );

        // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.01f )
        {
            B = C;                                         // Skip current point B
            nAB = Math::getNormal2D( B - A ) * lineWidth;  // Update normal nAB with new B
            continue;
        }

        // ... continue computations ...
        nBC = Math::getNormal2D( C - B ) * lineWidth;  // Compute normal nBC
        nABC = ( nAB + nBC ) * 0.5f; // nAB and nBC are already scaled by lineWidth

        if ( i > 1 )
        {
            color = randomColor();
        }
        else
        {
            CL = C - nBC;
            CR = C + nBC;
        }

#if 0
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
        // turn left ( math positive, ccw )
        if ( phi > 0.0f )
        {
            // intersect on the left side
//            if ( !USE_VECTOR_ADD )
//            {
                if ( IntersectionTool2D::intersectLine( S, A - nAB, B - nAB, B - nBC, C - nBC ) )
                {
                    BL = S;
                }
                else
                {
                    BL = B - nABC;
                }
//            }
//            else
//            {
//                BL = B - nABC;
//            }
            //BR = B + nAB;
            //CR = B + nBC;
            addHexagon( A, AL, AR, B, BL, BR, color );
            addLineJoin( dc, lineJoin, B, CR, BR, phi );
            accumLengthL += glm::length( CR - BR );
            BR = CR; // jump over line-join to build next hexagon
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
            // intersect on the right side
//            if ( !USE_VECTOR_ADD )
//            {
                if ( IntersectionTool2D::intersectLine( S, A + nAB, B + nAB, B + nBC, C + nBC ) )
                {
                    BR = S;
                }
                else
                {
                    BR = B + nABC;
                }
//            }
//            else
//            {
//                BR = B + nABC;
//            }
            //BL = B - nAB;
            //CL = B - nBC;
            addHexagon( A, AL, AR, B, BL, BR, color );
            addLineJoin( dc, lineJoin, B, BL, CL, phi );
            accumLengthR += glm::length( CL - BL );
            BL = CL;
        }
#endif
        // prepare NEXT:
        A = B;
        B = C;
        nAB = nBC;
        AL = BL;
        AR = BR;
//        BL = CL;
//        BR = CR;
    }
    //
    // End segment:
    //
    color = 0xFFFF0000; // end segment ( red )
    BL = B - nAB;
    BR = B + nAB;
    addHexagon( A, AL, AR, B, BL, BR, color );
    addEndLineCap( dc, lineCap, B, BL, BR );
    return true;
}



    /// @brief Generate SVG Path compliant 'const-width-extruded' polyline with line-joins and line-caps and even more...
    /// @version 2.0 Indexed triangles computed with intersectLine32/64(), because we know it works ( aka stable version ).
    /// @remarks There will be no support for computing intersections with plain vector addition in this (stable) version.
    template <  bool CLOCK_WISE,
                bool USE_POINT_SKIPPING,
                typename FNI_GET_POINT_2D,
                typename FNI_GET_POINTCOUNT>
    static bool
    asIndexedTriangles(
            PrimitiveRenderer2D & dc,
            FNI_GET_POINT_2D    getPoint2D,
            FNI_GET_POINTCOUNT  getPointCount,
            float32_t const     lineWidth,
            LineCap const       lineCap = LineCap::SQUARE,
            LineJoin const      lineJoin = LineJoin::BEVEL )
{
    // Generic Benni SVG PolyLine Primitive for LineJoins and LineCaps by SVG Standard
    /**
    // -----AL--------BL---CL-------     Dreiecke:   1.) turn left: AL, BL, A, B, AR, BR [, B, CR, CR] CW
    //     /       ---- \                           oder
    //    /    ----      \    -n                     2.) turn right: AL, AL, BL, A, B, AR, BR, BR, B, B, BL, CL, CL
    //   / ----           \    |
    //  A------------------B-------
    //   \ ----           /    |
    //    \    ----      /    +n
    //     \       ---- /
    // -----AR--------BR---CR-------
    */
    // Builds 3 vertices and 12 indices ( 4 indexed triangles ) per iteration / segment (i)
    // Each segment (i) will reuse the vertices pushed by its successor segment (i+1)
    // This happens as often as there are segments: segmentCount = pointCount - 1.
    // Then, at the end, the missing last 3 vertices have to be pushed and the geometry will be complete.
    // ( Because the indices from the last segment already address these missing vertices - and it would crash badly otherwise )
    auto addHexagon = [&dc] (   glm::vec2 const A, glm::vec2 const AL, glm::vec2 const AR,
                                glm::vec2 const B, glm::vec2 const BL, glm::vec2 const BR,
                                uint32_t const color = 0xFFFF0000, uint32_t const colorL = 0xFF202020, uint32_t const colorR = 0xFFF8DF18,
                                glm::vec2 const uvA = glm::vec2(0,0.5f), glm::vec2 const uvAL = glm::vec2(0,0), glm::vec2 const uvAR = glm::vec2(0,1),
                                glm::vec2 const uvB = glm::vec2(1,0.5f), glm::vec2 const uvBL = glm::vec2(1,0), glm::vec2 const uvBR = glm::vec2(1,1))
    {
        uint32_t const v = dc.getVertexCount();
        dc.addVertex2D( A,  color,  uvA ); // v0 + 3
        dc.addVertex2D( AL, colorL, uvAL ); // v0 + 4
        dc.addVertex2D( AR, colorR, uvAR ); // v0 + 5
        dc.addVertex2D( B,  color,  uvB ); // v0 + 3
        dc.addVertex2D( BL, colorL, uvBL ); // v0 + 4
        dc.addVertex2D( BR, colorR, uvBR ); // v0 + 5
        dc.addIndex( v + 1  ); // AL, BL, A, B, AR, BR
        dc.addIndex( v + 4  );
        dc.addIndex( v      );
        dc.addIndex( v + 3  );
        dc.addIndex( v + 2  );
        dc.addIndex( v + 5  );
    };

    // nothing todo, empty input ( d.k. if i should return 'true' here )
    size_t const pointCount = getPointCount();
    if ( pointCount == 0 )
    {
        return false;
    }
    // Contruct A single point ( invisible ) but with possible end-caps ( optional, can be made visible / turned on ).
    // This is most probably not what the caller wanted, so we return false here.
    // It would be better to collect all single points and recall this function.
    // The 'false' says 'collect single points' to the caller
    else if ( pointCount == 1 )
    {
        glm::vec2 const A = getPoint2D( 0 );
        glm::vec2 const nA( 0, -1 );
        addStartLineCap( dc, lineCap, A, A - nA, A + nA );
        addEndLineCap( dc, lineCap, A, A - nA, A + nA );
        return false;
    }
    // Two points make up a single segment AB ( visible ) with possible end-caps ( optional ).
    else if ( pointCount == 2 )
    {
        glm::vec2 A,B, AL,AR,BL,BR;            // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
        glm::vec2 nAB;         // We also need normals for both line-segments AB and BC
        A = getPoint2D( 0 );                            // Fetch A
        B = getPoint2D( 1 );                            // Fetch B
        nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
        AL = A - nAB;
        AR = A + nAB;
        BL = B - nAB;
        BR = B + nAB;
        addStartLineCap( dc, lineCap, A, AL, AR );      // Build LineCap for start-segment
        addHexagon( A, AL, AR, B, BL, BR );
        addEndLineCap( dc, lineCap, B, BL, BR ); // Build LineCap for Start-Segment
        return true;
    }
    //      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
    //      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin

    // <locals>
    float64_t accumLength,accumLengthL,accumLenghtR;
    glm::vec2 A,B,C,SL,SR;         // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 nAB,nBC,nABC;         // We also need normals for both line-segments AB and BC
    float32_t phi;      // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute

    // <start-segment>
    A = getPoint2D( 0 );                            // Fetch A
    B = getPoint2D( 1 );                            // Fetch B
    accumLength= glm::length( B - A );
    accumLengthL= accumLength;
    accumLengthR= accumLength;
    nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
    addStartLineCap( dc, lineCap, A, A - nAB, A + nAB );      // Build LineCap for start-segment
    // </start-segment>

    // <join-segments>
    for ( size_t i = 1; i < pointCount - 1; ++i )
    {
        C = getPoint2D( i + 1 );            // Fetch C

        // Used to determine direction (left/right) of polyline
        phi = Math::getAngleABC( A, B, C );

        // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.01f )
        {
            B = C;                                         // Skip current point B
            nAB = Math::getNormal2D( B - A ) * lineWidth;  // Update normal nAB with new B
            continue;
        }

        // ... continue computations ...
        nBC = Math::getNormal2D( C - B ) * lineWidth;  // Compute normal nBC

        // <line-join type="miter">
        if ( lineJoin == LineJoin::MITER )
        {
#if 0
            bool const hasLeft = IntersectionTool2D::intersectLine32( SL, A - nAB, B - nAB, B - nBC, C - nBC );
            bool const hasRight = IntersectionTool2D::intersectLine32( SR, A + nAB, B + nAB, B + nBC, C + nBC );
            if ( !hasLeft || !hasRight)
            {
                B = C;                            // Skip current segment
                nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
                //TEXTURED_MITER_POLYLINE_LOG_ERROR( "Segment[", i, "/", n-1, "] has NO intersection, skip point.\n" )
                continue;
            }
#endif
            nABC = ( nAB + nBC ) * 0.5f; // nAB and nBC are already scaled by lineWidth
            //addMiterPrimtitve( B, B - nABC, B + nABC, accumLength, randomColor() );
        }
        // <line-join type="bevel|round">
        else
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
            // SL = B - 0.5 ( nAB + nBC );
            // CR = B + nBC;
            // BR = B + nAB;
            //
            // <turn-left cw="false">
            // 1.) intersect on left side ( always with vadd fallback )
            // 2.) add right line-join B,CR,BR
#if 0

        if ( phi < 0.0f )     // turn left ( math positive, ccw )
        {
            // if ( IntersectionTool2D::intersectLine32( SL, A - nAB, B - nAB, B - nBC, C - nBC ) )

            glm::vec2 const BR = B + nAB;
            glm::vec2 const CR = B + nBC;
            //addHexagon( A, KL, KR, B, QL, BR, color, colorL, colorR );
            dc.addVertex( B,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
            dc.addVertex( QL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
            dc.addVertex( QR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5
            //addLineJoin( dc, LineJoin::BEVEL, B, CR, BR, phi );
            dc.addTriangle2D( B, CR, BR, POLYLINE_COLOR_LINE_JOIN_BEVEL );
            QR = CR;
        }
#endif
            if ( phi <= 0.0f )
            {
                glm::vec2 const BR = B + nAB;
                if ( IntersectionTool2D::intersectLine32( SL, A - nAB, B - nAB, B - nBC, C - nBC ) )
                {

                }
                addHexagon( A, KL, KR, B, QL, BR, color, colorL, colorR );
                addHexagon( B, SL, BR, accumLength, randomColor() );
                addLineJoin( dc, lineJoin, B, B + nBC, BR, phi );
            } // </turn-left>
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            // <turn-right cw="true">
            // 1.) intersect on right side ( always with vadd fallback )
            // 2.) add left line-join
#if 0
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
    #endif

            else
            {
                glm::vec2 const BL = B - nAB;
                if ( IntersectionTool2D::intersectLine( SR, A + nAB, B + nAB, B + nBC, C + nBC ) )
                {
                    addPrimitive( B, BL, SR, accumLength, randomColor() );
                }
                addLineJoin( dc, lineJoin, B, BL, B + nBC, phi );
            } // </turn-right>
        } // </line-join>

        // prepare NEXT:
        accumLength += glm::length( C - B );
        A = B;
        B = C;
        nAB = nBC;
    }

    //<end-segment>
    //A = getPoint2D( getPointCount() - 2 );
    //B = getPoint2D( getPointCount() - 1 );
    nAB = Math::getNormal2D( B - A ) * lineWidth;
    dc.addVertex( B,                         0xFFFF0000, glm::vec2( accumLength, 0.5f ) );
    dc.addVertex( B - nAB, varyColor( 0xFFFF0000, 100 ), glm::vec2( accumLength, 0.0f ) );
    dc.addVertex( B + nAB, varyColor( 0xFFFF0000, 100 ), glm::vec2( accumLength, 1.0f ) );
    addEndLineCap( dc, lineCap, B, B - nAB, B + nAB );
    //</end-segment>
    return true;
}

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

    /// @brief Generate SVG Path compliant 'const-width-extruded' polyline with line-joins and line-caps and even more...
    /// @version 2.0 Indexed triangles computed with intersectLine32/64(), because we know it works ( aka stable version ).
    /// @remarks There will be no support for computing intersections with plain vector addition in this (stable) version.
    template <  bool CLOCK_WISE,
                bool USE_POINT_SKIPPING,
                typename FNI_GET_POINT_2D,
                typename FNI_GET_POINTCOUNT>
    static bool
    asIndexedTriangles(
            PrimitiveRenderer2D & dc,
            FNI_GET_POINT_2D    getPoint2D,
            FNI_GET_POINTCOUNT  getPointCount,
            float32_t const     lineWidth,
            LineCap const       lineCap = LineCap::SQUARE,
            LineJoin const      lineJoin = LineJoin::BEVEL )
{
    // Generic Benni SVG PolyLine Primitive for LineJoins and LineCaps by SVG Standard
    /**
    //  AL = A - nAB
    //  +---+
    //  |  /|
    //  |/  |
    //  A---B
    //  |\  |
    //  |  \|
    //  +---+
    //  AR = A + nAB
    */
    // Builds 3 vertices and 12 indices ( 4 indexed triangles ) per iteration / segment (i)
    // Each segment (i) will reuse the vertices pushed by its successor segment (i+1)
    // This happens as often as there are segments: segmentCount = pointCount - 1.
    // Then, at the end, the missing last 3 vertices have to be pushed and the geometry will be complete.
    // ( Because the indices from the last segment already address these missing vertices - and it would crash badly otherwise )
    auto addPrimitive = [&dc] ( glm::vec2 const A, glm::vec2 const AL, glm::vec2 const AR, float32_t u, uint32_t const debugColor )
    {
        uint32_t const v = dc.getVertexCount();
        dc.addVertex( A,                    debugColor, glm::vec2( u, 0.5f ) ); // A  = v + 0, B  = v + 3
        dc.addVertex( AL, varyColor( debugColor, 100 ), glm::vec2( u, 0.0f ) ); // AL = v + 1, BL = v + 4
        dc.addVertex( AR, varyColor( debugColor, 100 ), glm::vec2( u, 1.0f ) ); // AR = v + 2, BR = v + 5
        if ( CLOCK_WISE )
        {
            dc.addIndexedTriangle( v, v + 1, v + 4 ); // Triangle A,AL,BL
            dc.addIndexedTriangle( v, v + 4, v + 3 ); // Triangle A,BL,B
            dc.addIndexedTriangle( v, v + 3, v + 5 ); // Triangle A,B,BR
            dc.addIndexedTriangle( v, v + 5, v + 2 ); // Triangle A,BR,AR
        }
        else
        {
            dc.addIndexedTriangle( v, v + 2, v + 5 ); // Triangle A,AR,BR
            dc.addIndexedTriangle( v, v + 5, v + 3 ); // Triangle A,BR,B
            dc.addIndexedTriangle( v, v + 3, v + 4 ); // Triangle A,B,BL
            dc.addIndexedTriangle( v, v + 4, v + 1 ); // Triangle A,BL,AL
        }
    };

    // nothing todo, empty input ( d.k. if i should return 'true' here )
    size_t const pointCount = getPointCount();
    if ( pointCount == 0 )
    {
        return false;
    }
    // Contruct A single point ( invisible ) but with possible end-caps ( optional, can be made visible / turned on ).
    // This is most probably not what the caller wanted, so we return false here.
    // It would be better to collect all single points and recall this function.
    // The 'false' says 'collect single points' to the caller
    else if ( pointCount == 1 )
    {
        glm::vec2 const A = getPoint2D( 0 );
        glm::vec2 const nA( 0, -1 );
        addStartLineCap( dc, lineCap, A, A - nA, A + nA );
        addEndLineCap( dc, lineCap, A, A - nA, A + nA );
        return false;
    }
    // Two points make up a single segment AB ( visible ) with possible end-caps ( optional ).
    else if ( pointCount == 2 )
    {
        glm::vec2 A,B, AL,AR,BL,BR;            // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
        glm::vec2 nAB;         // We also need normals for both line-segments AB and BC
        A = getPoint2D( 0 );                            // Fetch A
        B = getPoint2D( 1 );                            // Fetch B
        nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
        AL = A - nAB;
        AR = A + nAB;
        BL = B - nAB;
        BR = B + nAB;
        addStartLineCap( dc, lineCap, A, AL, AR );      // Build LineCap for start-segment
        addPrimitive( A, AL, AR, 0.0f, 0xFF0000FF );    // Build start-segment ( blue )
        // push the last three missing vertices and the geometry is complete
        dc.addVertex( B,                    0xFFFF0000, glm::vec2( 1.0f, 0.5f ) ); // ( red )
        dc.addVertex( BL, varyColor( 0xFFFF0000, 100 ), glm::vec2( 1.0f, 0.0f ) ); // ( red )
        dc.addVertex( BR, varyColor( 0xFFFF0000, 100 ), glm::vec2( 1.0f, 1.0f ) ); // ( red )
        addEndLineCap( dc, lineCap, B, BL, BR ); // Build LineCap for Start-Segment
        return true;
    }
    //      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
    //      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin

    // <locals>
    float64_t accumLength,accumLengthL,accumLenghtR;
    glm::vec2 A,B,C,SL,SR;         // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 nAB,nBC,nABC;         // We also need normals for both line-segments AB and BC
    float32_t phi;      // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute

    // <start-segment>
    A = getPoint2D( 0 );                            // Fetch A
    B = getPoint2D( 1 );                            // Fetch B
    accumLength= glm::length( B - A );
    accumLengthL= accumLength;
    accumLengthR= accumLength;
    nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
    addStartLineCap( dc, lineCap, A, A - nAB, A + nAB );      // Build LineCap for start-segment
    // addPrimitive( A, A - nAB, A + nAB, 0.0f, 0xFF0000FF );    // Build start-segment ( blue )
    uint32_t v = dc.getVertexCount();
    dc.addVertex( A,                         debugColor, glm::vec2( u, 0.5f ) ); // A  = v + 0, B  = v + 3
    dc.addVertex( A - nAB, varyColor( debugColor, 100 ), glm::vec2( u, 0.0f ) ); // AL = v + 1, BL = v + 4
    dc.addVertex( A + nAB, varyColor( debugColor, 100 ), glm::vec2( u, 1.0f ) ); // AR = v + 2, BR = v + 5
    dc.addIndexedTriangle( v, v + 2, v + 5 ); // Triangle A,AR,BR
    dc.addIndexedTriangle( v, v + 5, v + 3 ); // Triangle A,BR,B
    dc.addIndexedTriangle( v, v + 3, v + 4 ); // Triangle A,B,BL
    dc.addIndexedTriangle( v, v + 4, v + 1 ); // Triangle A,BL,AL

    // </start-segment>

    // <join-segments>
    for ( size_t i = 1; i < pointCount - 1; ++i )
    {
        C = getPoint2D( i + 1 );            // Fetch C

        // Used to determine direction (left/right) of polyline
        phi = Math::getAngleABC( A, B, C );

        // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.01f )
        {
            B = C;                                         // Skip current point B
            nAB = Math::getNormal2D( B - A ) * lineWidth;  // Update normal nAB with new B
            continue;
        }

        // ... continue computations ...
        nBC = Math::getNormal2D( C - B ) * lineWidth;  // Compute normal nBC

        // <line-join type="miter">
        if ( lineJoin == LineJoin::MITER )
        {
#if 0
            bool const hasLeft = IntersectionTool2D::intersectLine32( SL, A - nAB, B - nAB, B - nBC, C - nBC );
            bool const hasRight = IntersectionTool2D::intersectLine32( SR, A + nAB, B + nAB, B + nBC, C + nBC );
            if ( !hasLeft || !hasRight)
            {
                B = C;                            // Skip current segment
                nAB = Math::getNormal2D( B - A ) * lineWidth; // Update normal nAB
                //TEXTURED_MITER_POLYLINE_LOG_ERROR( "Segment[", i, "/", n-1, "] has NO intersection, skip point.\n" )
                continue;
            }
#endif
            nABC = ( nAB + nBC ) * 0.5f; // nAB and nBC are already scaled by lineWidth
            addPrimitive( B, B - nABC, B + nABC, accumLength, randomColor() );
        }
        // <line-join type="bevel|round">
        else
        {
            // <turn-left cw="false">
            // 1.) intersect on left side ( always with vadd fallback )
            // 2.) add right line-join
            if ( phi <= 0.0f )
            {
                glm::vec2 const BR = B + nAB;
                if ( IntersectionTool2D::intersectLine32( SL, A - nAB, B - nAB, B - nBC, C - nBC ) )
                {
                    addPrimitive( B, SL, BR, accumLength, randomColor() );
                }
                addLineJoin( dc, lineJoin, B, B + nBC, BR, phi );
            } // </turn-left>
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            // <turn-right cw="true">
            // 1.) intersect on right side ( always with vadd fallback )
            // 2.) add left line-join
            else
            {
                glm::vec2 const BL = B - nAB;
                if ( IntersectionTool2D::intersectLine( SR, A + nAB, B + nAB, B + nBC, C + nBC ) )
                {
                    addPrimitive( B, BL, SR, accumLength, randomColor() );
                }
                addLineJoin( dc, lineJoin, B, BL, B + nBC, phi );
            } // </turn-right>
        } // </line-join>

        // prepare NEXT:
        accumLength += glm::length( C - B );
        A = B;
        B = C;
        nAB = nBC;
    }

    //<end-segment>
    //A = getPoint2D( getPointCount() - 2 );
    //B = getPoint2D( getPointCount() - 1 );
    nAB = Math::getNormal2D( B - A ) * lineWidth;
    dc.addVertex( B,                         0xFFFF0000, glm::vec2( accumLength, 0.5f ) );
    dc.addVertex( B - nAB, varyColor( 0xFFFF0000, 100 ), glm::vec2( accumLength, 0.0f ) );
    dc.addVertex( B + nAB, varyColor( 0xFFFF0000, 100 ), glm::vec2( accumLength, 1.0f ) );
    addEndLineCap( dc, lineCap, B, B - nAB, B + nAB );
    //</end-segment>
    return true;
}

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

    /// @deprecated Replaced by intersectLine() correct version 2.0b
    /// @brief generate a mesh from lines
    /// @version 1.0 Indexed triangles with vector-addition, but thats not sufficient without computing correct length for scaling nABC
    template <  bool CLOCK_WISE,
                bool USE_POINT_SKIPPING,
                bool USE_VECTOR_ADDITION,
                typename FNI_GET_POINT_2D,
                typename FNI_GET_POINTCOUNT>
    static bool
    asIndexedTriangles(
            PrimitiveRenderer2D & dc,
            FNI_GET_POINT_2D    getPoint2D,
            FNI_GET_POINTCOUNT  getPointCount,
            float32_t const     lineWidth,
            LineCap const       lineCap = LineCap::SQUARE,
            LineJoin const      lineJoin = LineJoin::BEVEL )
{

    // Generic Benni SVG PolyLine Primitive for LineJoins and LineCaps by SVG Standard
    /**
    //  AL = A - nAB
    //  +---+
    //  |  /|
    //  |/  |
    //  A---B
    //  |\  |
    //  |  \|
    //  +---+
    //  AR = A + nAB
    */
    // Builds 3 vertices and 12 indices ( 4 indexed triangles ) per iteration / segment (i)
    // Each segment (i) will reuse the vertices pushed by its successor segment (i+1)
    // This happens as often as there are segments: segmentCount = pointCount - 1.
    // Then, at the end, the missing last 3 vertices have to be pushed and the geometry will be complete.
    // ( Because the indices from the last segment already address these missing vertices - and it would crash badly otherwise )
    auto addPrimitive = [&dc] ( glm::vec2 const A, glm::vec2 const AL, glm::vec2 const AR, float32_t u, uint32_t const debugColor )
    {
        uint32_t const v = dc.getVertexCount();
        dc.addVertex( A,                    debugColor, glm::vec2( u, 0.5f ) ); // A  = v + 0, B  = v + 3
        dc.addVertex( AL, varyColor( debugColor, 100 ), glm::vec2( u, 0.0f ) ); // AL = v + 1, BL = v + 4
        dc.addVertex( AR, varyColor( debugColor, 100 ), glm::vec2( u, 1.0f ) ); // AR = v + 2, BR = v + 5
        if ( CLOCK_WISE )
        {
            dc.addIndexedTriangle( v, v + 1, v + 4 ); // Triangle A,AL,BL
            dc.addIndexedTriangle( v, v + 4, v + 3 ); // Triangle A,BL,B
            dc.addIndexedTriangle( v, v + 3, v + 5 ); // Triangle A,B,BR
            dc.addIndexedTriangle( v, v + 5, v + 2 ); // Triangle A,BR,AR
        }
        else
        {
            dc.addIndexedTriangle( v, v + 2, v + 5 ); // Triangle A,AR,BR
            dc.addIndexedTriangle( v, v + 5, v + 3 ); // Triangle A,BR,B
            dc.addIndexedTriangle( v, v + 3, v + 4 ); // Triangle A,B,BL
            dc.addIndexedTriangle( v, v + 4, v + 1 ); // Triangle A,BL,AL
        }
    };

    // nothing todo, empty input ( d.k. if i should return 'true' here )
    size_t const pointCount = getPointCount();
    if ( pointCount == 0 )
    {
        return false;
    }
    // Contruct A single point ( invisible ) but with possible end-caps ( optional, can be made visible / turned on ).
    // This is most probably not what the caller wanted, so we return false here.
    // It would be better to collect all single points and recall this function.
    // The 'false' says 'collect single points' to the caller
    else if ( pointCount == 1 )
    {
        glm::vec2 const A = getPoint2D( 0 );
        glm::vec2 const nA( 0, -1 );
        addStartLineCap( dc, lineCap, A, A - nA, A + nA );
        addEndLineCap( dc, lineCap, A, A - nA, A + nA );
        return false;
    }
    // Two points make up a single segment AB ( visible ) with possible end-caps ( optional ).
    else if ( pointCount == 2 )
    {
        glm::vec2 A,B, AL,AR,BL,BR;            // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
        glm::vec2 nAB;         // We also need normals for both line-segments AB and BC
        A = getPoint2D( 0 );                            // Fetch A
        B = getPoint2D( 1 );                            // Fetch B
        nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
        AL = A - nAB;
        AR = A + nAB;
        BL = B - nAB;
        BR = B + nAB;
        addStartLineCap( dc, lineCap, A, AL, AR );      // Build LineCap for start-segment
        addPrimitive( A, AL, AR, 0.0f, 0xFF0000FF );    // Build start-segment ( blue )
        // push the last three missing vertices and the geometry is complete
        dc.addVertex( B,                    0xFFFF0000, glm::vec2( 1.0f, 0.5f ) ); // ( red )
        dc.addVertex( BL, varyColor( 0xFFFF0000, 100 ), glm::vec2( 1.0f, 0.0f ) ); // ( red )
        dc.addVertex( BR, varyColor( 0xFFFF0000, 100 ), glm::vec2( 1.0f, 1.0f ) ); // ( red )
        addEndLineCap( dc, lineCap, B, BL, BR ); // Build LineCap for Start-Segment
        return true;
    }
    //      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
    //      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin

    // <locals>
    float64_t accumLength;
    glm::vec2 A,B,C;         // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 nAB,nBC,nABC;         // We also need normals for both line-segments AB and BC
    float32_t phi;      // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute

    // <start-segment>
    A = getPoint2D( 0 );                            // Fetch A
    B = getPoint2D( 1 );                            // Fetch B
    accumLength= glm::length( B - A );
    nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
    addStartLineCap( dc, lineCap, A, A - nAB, A + nAB );      // Build LineCap for start-segment
    addPrimitive( A, A - nAB, A + nAB, 0.0f, 0xFF0000FF );    // Build start-segment ( blue )
    // </start-segment>

    // <join-segments>
    for ( size_t i = 1; i < pointCount - 1; ++i )
    {
        C = getPoint2D( i + 1 );            // Fetch C
        phi = Math::getAngleABC( A, B, C ); // Compute angle between vector AB and BC

        // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
        if ( std::abs( phi ) < 0.01f )
        {
            B = C;                                         // Skip current point B
            nAB = Math::getNormal2D( B - A ) * lineWidth;  // Update normal nAB with new B
            continue;
        }

        // ... continue computations ...
        nBC = Math::getNormal2D( C - B ) * lineWidth;  // Compute normal nBC
        nABC = glm::normalize( nAB + nBC );
        addPrimitive( B, B - nABC * 2.0f, B + nABC * 2.0f, accumLength, randomColor() );
#if 0
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

        if ( phi < 0.0f )     // turn left ( math positive, ccw )
        {
            // if ( IntersectionTool2D::intersectLine32( SL, A - nAB, B - nAB, B - nBC, C - nBC ) )

            glm::vec2 const BR = B + nAB;
            glm::vec2 const CR = B + nBC;
            //addHexagon( A, KL, KR, B, QL, BR, color, colorL, colorR );
            dc.addVertex( B,  color,  glm::vec2( 0.5f, 0.5f ) ); // v0 + 3
            dc.addVertex( QL, colorL, glm::vec2( 0.5f, 0.0f ) ); // v0 + 4
            dc.addVertex( QR, colorR, glm::vec2( 0.5f, 1.0f ) ); // v0 + 5
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
#endif
        // prepare NEXT:
        accumLength += glm::length( C - B );
        A = B;
        B = C;
        nAB = nBC;
    }

    //<end-segment>
    //A = getPoint2D( getPointCount() - 2 );
    //B = getPoint2D( getPointCount() - 1 );
    nAB = Math::getNormal2D( B - A ) * lineWidth;
    dc.addVertex( B,                         0xFFFF0000, glm::vec2( accumLength, 0.5f ) );
    dc.addVertex( B - nAB, varyColor( 0xFFFF0000, 100 ), glm::vec2( accumLength, 0.0f ) );
    dc.addVertex( B + nAB, varyColor( 0xFFFF0000, 100 ), glm::vec2( accumLength, 1.0f ) );
    addEndLineCap( dc, lineCap, B, B - nAB, B + nAB );
    //</end-segment>
    return true;
}
#endif
