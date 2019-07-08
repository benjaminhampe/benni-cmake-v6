#include "addPolyLineABC.hpp"

#define USE_POLYLINE_ABC_DEBUG_LOGGING

//
// Create PolyLine ABC
//
// AL---------------BL\-----SBL
// |        --------|  \   /
// |--------        |    \/
// A----------------B----BL
// |----        -  /    /
// |    ----  -   /    /
// AR-------SBR  /    /
//         /    /    /
//        /    /    /
//       /    /    /
//      /    /    /
//     CR---C----CL
//
bool
    addPolyLineABC(
        PrimitiveRenderer & dc,
        glm::vec2 const A,
        glm::vec2 const B,
        glm::vec2 const C,
        float32_t const lineWidth,
        LineJoin const lineJoin )
{
    constexpr static float32_t const CONST_POLYLINE_ANGLE_STEP = 10.0f; // degrees

    float32_t const phi = Utils::getAngleABC( A, B, C );
    glm::vec2 const n1 = Utils::getNormal2D( B - A ) * lineWidth;
    glm::vec2 const n2 = Utils::getNormal2D( C - B ) * lineWidth;
    glm::vec2 const AL( A.x - n1.x, A.y - n1.y );
    glm::vec2 const AR( A.x + n1.x, A.y + n1.y );

    glm::vec2 const BL( B.x - n1.x, B.y - n1.y );
    glm::vec2 const BR( B.x + n1.x, B.y + n1.y );

    glm::vec2 const CL( B.x - n2.x, B.y - n2.y );
    glm::vec2 const CR( B.x + n2.x, B.y + n2.y );

    glm::vec2 const DL( C.x - n2.x, C.y - n2.y );
    glm::vec2 const DR( C.x + n2.x, C.y + n2.y );

    glm::vec2 SL;
    glm::vec2 SR;

    bool const hasL = IntersectionTool::intersectRay2D( SL, AL, BL, CL, DL );
    bool const hasR = IntersectionTool::intersectRay2D( SR, AR, BR, CR, DR );

#ifdef USE_POLYLINE_ABC_DEBUG_LOGGING
    std::cout << __FUNCTION__ << "(" << A << "; " << B << "; " << C << ")\n";
    std::cout << " phi(" << phi << ")\n";
    std::cout << " n1(" << n1 << ")\n";
    std::cout << " n2(" << n2 << ")\n";
    std::cout << " AL(" << AL << "), BL(" << BL << "), CL(" << CL << "), DL(" << DL << ")\n";
    std::cout << " AR(" << AR << "), BR(" << BR << "), CR(" << CR << "), DR(" << DR << ")\n";
    std::cout << " SL(" << SL << ")\n";
    std::cout << " SR(" << SR << ")\n";
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
                if ( phi >= 1.5f * CONST_POLYLINE_ANGLE_STEP )
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
