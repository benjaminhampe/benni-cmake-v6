#ifndef POLYLINE_HPP
#define POLYLINE_HPP

#include <de/hampe/logging/Logger.hpp>
#include <de/hampe/svg/IntersectionTool2D.hpp>
#include <de/hampe/svg/PrimitiveRenderer.hpp>
#include <de/hampe/svg/LineCapGenerator.hpp>
#include <de/hampe/svg/LineJoinGenerator.hpp>
#include <de/hampe/color/Color.hpp>

#define USE_POLYLINE_POINT_SKIPPING

// Unit contants in [degrees]

namespace de {
namespace hampe {
namespace svg {

// ===========================================================================================================
/// @brief Create SVG PolyLine - Supports different line-joins and end-caps to comply with SVG standard.
// ===========================================================================================================
//
// We can process all polyline shapes:
//
//      ( n == 0 ) for empty input we simply abort
//      ( n == 1 ) for a single point we atleast create LineCaps
//      ( n == 2 ) for a single line-segment we create 2 LineCaps and a hexagon: TODO: only build rect, but need specialized handling outside the main loop
//      ( n >= 3 ) a normal polyline ABC with 2 LineCaps, 2 hexagons and 1 LineJoin
//
// ===========================================================================================================

class PolyLine
{
    // Logger
    CREATE_STATIC_LOGGER("de.hampe.svg.PolyLine")

    // Usings
    using LogLevel = de::hampe::logging::LogLevel;

public:

    // -----AL--------BL-----------     Dreiecke:   1.) A, AL, BL
    //     /       ---- \                           2.) A, BL, B
    //    /    ----      \    -n         CW         3.) A, B, BR
    //   / ----           \    |                    4.) A, BR, AR
    //  A------------------B-------
    //   \ ----           /    |
    //    \    ----      /    +n
    //     \       ---- /
    // -----AR--------BR-----------
template < bool CW >
static void
addHexagon( PrimitiveRenderer2D & dc,
            glm::vec2 const A, glm::vec2 const AL, glm::vec2 const AR,
            glm::vec2 const B, glm::vec2 const BL, glm::vec2 const BR,
            uint32_t const debugColor )
{
    if ( CW )
    {
        dc.addTriangle2D( A, AL, BL, debugColor );
        dc.addTriangle2D( A, BL, B, varyColor( debugColor, 100 ) );
        dc.addTriangle2D( A, B, BR, varyColor( debugColor, 100 ) );
        dc.addTriangle2D( A, BR, AR, varyColor( debugColor, 100 ) );
    }
    else // better ( right turn of triangles )
    {
        dc.addTriangle2D( A, AR, BR, debugColor );
        dc.addTriangle2D( A, BR, B, varyColor( debugColor, 100 ) );
        dc.addTriangle2D( A, B, BL, varyColor( debugColor, 100 ) );
        dc.addTriangle2D( A, BL, AL, varyColor( debugColor, 100 ) );
    }
}

template <  bool CW,
        typename FNI_GET_POINT_2D,
        typename FNI_GET_POINTCOUNT >
static bool
asIndexedTriangles(
    PrimitiveRenderer2D & dc,
    FNI_GET_POINT_2D    getPoint2D,
    FNI_GET_POINTCOUNT  getPointCount,
//    FNI_GET_VERTEXCOUNT getVertexCount,
//    FNO_ADD_VERTEX_2D   getVertex2D,
//    FNO_ADD_INDEXED_TRIANGLE addIndexedTriangle,
    float32_t const     lineWidth,
    LineCap const       lineCap,
    LineJoin const      lineJoin )
{
    //     AL----------BL      -n    Dreiecke:   1.) A, AL, BL
    //     /       ---- \       |                2.) A, BL, B
    //    A-   ----     -B      +     CW         3.) A, B, BR
    //   / ----           \     |                4.) A, BR, AR
    //  AR-----------------BR  +n
    // In the begin 2 start vertices AL,AR are pushed ( not A )
    // Then each iteration adds 2 vertices BL,BR ( not B, computed with C = B[i+1] ) and connects them with the previous vertices AL,AR from last iteration.
    // When there are no C then BL,BR are simply B-nAB,B+nAB reusing last normal nAB.
    // Each current segment (i+1) connects the vertices pushed by previous segment (i)
    // This happens as often as there are segments: segmentCount = pointCount - 1.
    auto addMiterPrimitive = [&dc] ( glm::vec2 const AL, glm::vec2 const AR, uint32_t const debugColor )
    {
        uint32_t const v = dc.getVertexCount();
        uint32_t const off = 2;
        dc.addVertex2D( AL, debugColor,                   glm::vec2( 0.0f, 0.0f ) ); // AL = v + 0, BL = v + 2
        dc.addVertex2D( AR, varyColor( debugColor, 100 ), glm::vec2( 0.0f, 1.0f ) ); // AR = v + 1, BR = v + 3
        if ( CW )
        {
            dc.addIndexedTriangle( v - off, v + 2 - off, v + 3 - off ); // Triangle AL,BL,BR
            dc.addIndexedTriangle( v - off, v + 3 - off, v + 1 - off ); // Triangle AL,BR,AR
        }
        else
        {
            dc.addIndexedTriangle( v - off, v + 1 - off, v + 3 - off ); // Triangle AL,AR,BR
            dc.addIndexedTriangle( v - off, v + 3 - off, v + 2 - off ); // Triangle AL,BR,BL
        }
    };


    size_t const n = getPointCount();
    uint32_t const v = dc.getVertexCount();

    // nothing todo, empty input ( d.k. if i should return 'true' here )
    if ( n == 0 )
    {
        return false;
    }
    // Contruct A single point ( invisible ) but with possible end-caps ( optional, can be made visible / turned on ).
    // This is most probably not what the caller wanted, so we return false here.
    // It would be better to collect all single points and recall this function.
    // The 'false' says 'collect single points' to the caller
    else if ( n == 1 )
    {
        glm::vec2 const A = getPoint2D( 0 );
        glm::vec2 const nA( 1, 0 );
        addStartLineCap( dc, lineCap, A, A + nA, lineWidth );
        addEndLineCap( dc, lineCap, A - nA, A, lineWidth );
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
    glm::vec2 AL,AR,BL,BR;  // We need exactly 3 points A,B,C to construct a line-join between 2 segments AB and BC
    glm::vec2 nAB,nBC,nABC; // We also need normals for both line-segments AB and BC
    float32_t phi;          // The angle between AB and BC in degrees, used to determine side of line-join and intersection to compute
    uint32_t color = randomColor();
    uint32_t processedPoints = 0;
    //
    // Start segment:
    //
    A = getPoint2D( 0 );                            // Fetch A
    B = getPoint2D( 1 );                            // Fetch B
    //    accumLength= glm::length( B - A );
    //    accumLengthL= accumLength;
    //    accumLengthR= accumLength;
    nAB = Math::getNormal2D( B - A ) * lineWidth;   // Compute normal nAB
    AL = A - nAB;
    AR = A + nAB;
    BL = B - nAB;
    BR = B + nAB;
    color = 0xFF0000FF; // start segment ( blue )

    //addStartLineCap( dc, lineCap, A, B, lineWidth );      // Build LineCap for start-segment
    // Miter Polyline ( deactivated line-join )
    if ( lineJoin == LineJoin::NONE || lineJoin == LineJoin::MITER )
    {
        dc.addVertex2D( A, color, glm::vec2( 0.0f, 0.5f) );
        dc.addVertex2D( AL, color, glm::vec2( 0.0f, 0.0f) );
        dc.addVertex2D( AR, color, glm::vec2( 0.0f, 1.0f) );
        ++processedPoints;
        //
        // Middle segments:
        //
        for ( size_t i = 1; i < n; ++i )
        {
            // Fetch C
            if ( i == n - 1 )
            {
                color = 0xFFFF0000;
                BL = B - nAB;
                BR = B + nAB;
            }
            else
            {
#ifdef USE_POINT_SKIPPING
                float32_t const phi = Math::getAngleABC( A, B, C );             // Used to determine direction (left/right) of polyline
                if ( std::abs( phi ) < 0.01f )                  // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
                {
                    B = C;                                         // Skip current point B
                    nAB = Math::getNormal2D( B - A ) * lineWidth;  // Update normal nAB with new B
                    continue;
                }
#endif
                color = randomColor();
                C = getPoint2D( i + 1 );
                nBC = Math::getNormal2D( C - B ) * lineWidth;  // Compute normal nBC
                nABC = ( nAB + nBC ); // nAB and nBC are already scaled by lineWidth
                BL = B - nABC;
                BR = B + nABC;
            }
            addMiterPrimitive( BL, BR, color );
            A = B;
            B = C;
            nAB = nBC;
            AL = BL;
            AR = BR;
            ++processedPoints;
        }
    }
    // Beveled and round line-join Polylines.
    else
    {
        //
        // Middle segments:
        //
        for ( size_t i = 1; i < n - 1; ++i )
        {
            if ( i > 1 ) { color = randomColor(); }

            // Fetch C
            C = getPoint2D( i + 1 );

#ifdef USE_POINT_SKIPPING
            phi = Math::getAngleABC( A, B, C );             // Used to determine direction (left/right) of polyline
            if ( std::abs( phi ) < 0.01f )                  // Skip segment if next segment is parallel to us, and combine both segments by skipping this one.
            {
                B = C;                                         // Skip current point B
                nAB = Math::getNormal2D( B - A ) * lineWidth;  // Update normal nAB with new B
                continue;
            }
#endif

            // ... continue computations ...
            nBC = Math::getNormal2D( C - B ) * lineWidth;   // Compute normal nBC
            nABC = nAB + nBC;                               // nAB and nBC are already scaled by lineWidth
            //BL = B - nABC; // SL
            //BR = B + nABC; // SR

            // turn left ( math positive, ccw )
            /*
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
            */
            if ( phi <= 0.0f )
            {
                //STATIC_LOG_DEBUG("[TURN-LEFT] :: A(",A,"), AL(",AL,"), AR(",AR,"), B(",B,"), BL(",BL,"), BR(",BR,"), C(",C,"), CL(",C-nBC,"), CR(",C+nBC,"), nAB(",nAB,"), nBC(",nBC,"), nABC(",nABC,")\n" )
                STATIC_LOG_DEBUG("[TURN-LEFT] :: A(",A,"), B(",B,"), C(",C,"), phi(",phi,"), nAB(",nAB,"), nBC(",nBC,"), nABC(",nABC,")\n" )
                //BR = B + nAB;
                BL = B - nABC; // SL
                addHexagon< CW >( dc, A, AL, AR, B, BL, BR, color );
                glm::vec2 const CR = B + nBC;
                addLineJoin( dc, lineJoin, B, CR, BR, phi );
                BR = CR;
            }
            // turn right ( math negative, cw )
            /*
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
            */
            else
            {
                STATIC_LOG_DEBUG("[TURN-RIGHT] :: A(",A,"), B(",B,"), C(",C,"), phi(",phi,"), nAB(",nAB,"), nBC(",nBC,"), nABC(",nABC,")\n" )
                //BL = B - nAB;
                BR = B + nABC; // SR
                addHexagon< CW >( dc, A, AL, AR, B, BL, BR, color );
                glm::vec2 const CL = B - nBC;
                addLineJoin( dc, lineJoin, B, BL, CL, phi );
                BL = CL;
            }

            // prepare NEXT:
            A = B;
            B = C;
            nAB = nBC;
            AL = BL;
            AR = BR;
        }
        //
        // End segment:
        //
        color = 0xFFFF0000; // end segment ( red )
        BL = B - nAB;
        BR = B + nAB;
        addHexagon< CW >( dc, A, AL, AR, B, BL, BR, color );
        addEndLineCap( dc, lineCap, A, B, lineWidth );
    }

    return true;
}

// ===========================================================================
// [BACKUP]:
// ===========================================================================

};

} // end namespace svg
} // end namespace hampe
} // end namespace de

#endif // POLYLINE_HPP
