#ifndef TEST_MITERPOLYLINE_HPP
#define TEST_MITERPOLYLINE_HPP

#include "TestCommon.hpp"
#include <de/hampe/svg/MiterPolyLine.hpp>

inline void
test_MiterPolyLine_asTriangleStrip(
        PrimitiveRenderer2D & dc,
        glm::vec2 const pos,
        float32_t const inlineWidth,
        float32_t const outlineWidth,
        uint32_t const bgColor,
        uint32_t const fgColor )
{
    using PolyLine = de::hampe::svg::MiterPolyLine;

    std::cout << "[TEST] " << __FUNCTION__ << "(inline=" << inlineWidth << ", outline=" << outlineWidth << "):\n";

    std::vector< glm::vec2 > points;
    sinc( points, POINT_COUNT, pos, glm::vec2(100, 100), SINC_MIN, SINC_MAX );

    if ( USE_POINT_LOGGING )
    {
        std::cout << toString( points );
    }

    auto getPointCount = [&points] () -> uint32_t { return points.size(); };
    auto getPoint = [&points] ( uint32_t const index ) -> glm::vec2 { return points[ index ]; };
    auto addInlineVertex2D = [&dc,fgColor] ( glm::vec2 const pos ) -> void { dc.addVertex2D( pos, fgColor, glm::vec2(0,0) ); };
    auto addOutlineVertex2D = [&dc,bgColor] ( glm::vec2 const pos ) -> void { dc.addVertex2D( pos, bgColor, glm::vec2(0,0) ); };
    auto addIndex = [&dc] ( uint32_t const index ) { dc.addIndex( index ); };

    /// [OUTLINE]
    dc.begin( irr::scene::EPT_TRIANGLE_STRIP, 0.01f );
    //PolyLine::asTriangleStrip<false,false,false>( addOutlineVertex2D, addIndex, getPoint, getPointCount, outlineWidth );
    PolyLine::drawTriangleStrip<false>( dc, getPoint, getPointCount, outlineWidth );
    dc.end();

    /// [INLINE]
    dc.begin( irr::scene::EPT_TRIANGLE_STRIP, 0.0f );
    dc.setWireframe( true );
    //PolyLine::asTriangleStrip<false,false,false>( addInlineVertex2D, addIndex, getPoint, getPointCount, inlineWidth );
    PolyLine::drawTriangleStrip<false>( dc, getPoint, getPointCount, inlineWidth );
    dc.end();

    /// [ORIGINAL POINTS]
    drawPointsAsBoxes( dc, points, glm::vec2(0.5f,0.5f), 0xFFFFFFFF );

    /// [POSTPROCESS]
    uint32_t vCount = dc.getVertexCount();
    uint32_t iCount = dc.getIndexCount();
    addTextBillboard( dc.getSceneManager(), 4.0f, "EPT_TRIANGLE_STRIP", dc.getBBoxCenter().x,dc.getBBoxMax().y + 10.0f, 0.0f, 0xFFFFFF00 );
    std::stringstream s; s << "p(" << POINT_COUNT << "), v(" << vCount << "), i(" << iCount << ")";
    addTextBillboard( dc.getSceneManager(), 4.0f, s.str(), dc.getBBoxCenter().x,dc.getBBoxMax().y + 5.0f, 0.0f, 0xFF9999FF );
}

inline void
test_MiterPolyLine_asIndexedTriangles( PrimitiveRenderer2D & dc, glm::vec2 const pos, float32_t const inlineWidth, float32_t const outlineWidth, uint32_t const bgColor, uint32_t const fgColor )
{
    using MiterPolyLine = de::hampe::svg::MiterPolyLine;

    std::cout << "[TEST] " << __FUNCTION__ << "(inline=" << inlineWidth << ", outline=" << outlineWidth << "):\n";

    std::vector< glm::vec2 > points;
    sinc( points, POINT_COUNT, pos, glm::vec2(100, 100), SINC_MIN, SINC_MAX );

    if ( USE_POINT_LOGGING )
    {
        std::cout << toString( points );
    }

    auto getPointCount = [&points] () -> uint32_t { return points.size(); };
    auto getPoint = [&points] ( uint32_t const index ) -> glm::vec2 { return points[ index ]; };
    auto addInlineVertex2D = [&dc,fgColor] ( glm::vec2 const pos ) -> void { dc.addVertex2D( pos, fgColor, glm::vec2(0,0) ); };
    auto addOutlineVertex2D = [&dc,bgColor] ( glm::vec2 const pos ) -> void { dc.addVertex2D( pos, bgColor, glm::vec2(0,0) ); };
    auto addIndexedTriangle = [&dc] ( uint32_t const indexA, uint32_t const indexB, uint32_t const indexC ) { dc.addIndexedTriangle( indexA, indexB, indexC ); };

    /// [OUTLINE]
    dc.begin( irr::scene::EPT_TRIANGLES, 0.01f );
    MiterPolyLine::drawTriangleStrip<false>( dc, getPoint, getPointCount, outlineWidth );
    dc.end();

    /// [INLINE]
    dc.begin( irr::scene::EPT_TRIANGLES, 0.0f );
    dc.setWireframe( true );
    MiterPolyLine::drawTriangleStrip<false>( dc, getPoint, getPointCount, inlineWidth );
    dc.end();

    /// [POSTPROCESS]
    dc.setDebugDataVisible( irr::scene::EDS_FULL );
    uint32_t vCount = dc.getVertexCount();
    uint32_t iCount = dc.getIndexCount();
    addTextBillboard( dc.getSceneManager(), 4.0f, "EPT_TRIANGLES", dc.getBBoxCenter().x,dc.getBBoxMax().y + 10.0f, 0.0f, 0xFFFFFF00 );
    std::stringstream s; s << "p(" << POINT_COUNT << "), v(" << vCount << "), i(" << iCount << ")";
    addTextBillboard( dc.getSceneManager(), 4.0f, s.str(), dc.getBBoxCenter().x,dc.getBBoxMax().y + 5.0f, 0.0f, 0xFF9999FF );
}

#if 0
void MiterPolyLineTest( PrimitiveRenderer2D & dc )
{
    float32_t outlineWidth = 2.5f;
    float32_t inlineWidth = 2.1f;
    testTexturedAsTriangleStrip<true> ( dc, glm::vec2( -100,-50 ), inlineWidth, outlineWidth, true, BG_COLOR, FG_COLOR );
    testTexturedAsTriangleStrip<false>( dc, glm::vec2(   0,-50 ),  inlineWidth, outlineWidth, true, BG_COLOR, FG_COLOR );

    testAsTriangles    ( dc, glm::vec2( 0,50 ),    inlineWidth, outlineWidth, false, BG_COLOR, FG_COLOR );
    testAsTriangleStrip( dc, glm::vec2( -100,50 ), inlineWidth, outlineWidth, false, BG_COLOR, FG_COLOR );
    addTextBillboard( dc.getSceneManager(), 8.0f, "MiterPolyLine", 0, dc.getBBoxMax().y + 10.0f, 0.0f, 0xFFFFFFFF );

    testAsTriangles    ( dc, glm::vec2( 0,0 ),    inlineWidth, outlineWidth, true, BG_COLOR, FG_COLOR );
    testAsTriangleStrip( dc, glm::vec2( -100,0 ), inlineWidth, outlineWidth, true, BG_COLOR, FG_COLOR );
}
#endif

#endif // TEST_MITERPOLYLINE_HPP
