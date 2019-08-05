#ifndef TEST_TEXTUREDMITERPOLYLINE_HPP
#define TEST_TEXTUREDMITERPOLYLINE_HPP

#include "TestCommon.hpp"
#include <de/hampe/svg/TexturedMiterPolyLine.hpp>

inline void test_TexturedMiterPolyLine_asTriangleStrip(
        PrimitiveRenderer2D & dc,
        glm::vec2 const pos,
        float32_t const inlineWidth,
        float32_t const outlineWidth,
        bool usePointSkipping )
{
    glm::vec2 size(100, 100);
    using PolyLine = de::hampe::svg::TexturedMiterPolyLine;

    std::cout << "[TEST] " << __FUNCTION__ << "(inline=" << inlineWidth << ", outline=" << outlineWidth << "):\n";

    std::vector< glm::vec2 > points;
    sinc( points, POINT_COUNT, pos, size, SINC_MIN, SINC_MAX );

    auto getPointCount = [&points] () -> uint32_t { return points.size(); };
    auto getPoint = [&points] ( uint32_t const index ) -> glm::vec2 { return points[ index ]; };
    auto addOutlineVertex2D = [&dc] ( glm::vec2 const pos, glm::vec2 const uv ) -> void { dc.addVertex2D( pos, BG_COLOR, uv ); };
    auto addInlineVertex2D = [&dc] ( glm::vec2 const pos, glm::vec2 const uv ) -> void { dc.addVertex2D( pos, FG_COLOR, uv ); };
    auto addIndex = [&dc] ( uint32_t const index ) { dc.addIndex( index ); };

    addTextBillboard( dc.getSceneManager(), 4.0f, "EPT_TRIANGLE_STRIP", pos.x + size.x*0.5f, pos.y + size.y - 10.0f, 0.0f, 0xFFFFFF00 );

    irr::video::ITexture* tex = dc.getTexture( "../media/benni_road.png" );

    /// [OUTLINE]
    dc.begin( irr::scene::EPT_TRIANGLE_STRIP, 0.01f );
    dc.setMaterialTexture( 0, tex );
    dc.setTextureRepeatMode( 0, irr::video::ETC_CLAMP, irr::video::ETC_REPEAT );
    PolyLine::asTriangleStrip( false,usePointSkipping,true, addOutlineVertex2D, addIndex, getPoint, getPointCount, outlineWidth );
    dc.end();
    std::stringstream s; s << "p(" << points.size() << "), v(" << dc.getVertexCount() << "), i(" << dc.getIndexCount() << ")";
    addTextBillboard( dc.getSceneManager(), 4.0f, s.str(), dc.getBBoxCenter().x,dc.getBBoxMax().y + 10.0f, 0.0f, 0xFF9999FF );

    /// [INLINE]
    dc.begin( irr::scene::EPT_TRIANGLE_STRIP, 0.0f );
    dc.setWireframe( true );
    dc.setMaterialTexture( 0, nullptr );
    PolyLine::asTriangleStrip( false,usePointSkipping,true, addInlineVertex2D, addIndex, getPoint, getPointCount, inlineWidth );
    dc.end();

    /// [ORIGINAL POINTS]
    drawPointsAsBoxes( dc, points, glm::vec2(0.5f,0.5f), 0xFFFFFFFF );

    if ( usePointSkipping )
    {
        addTextBillboard( dc.getSceneManager(), 4.0f, "usePointSkipping = true", dc.getBBoxCenter().x,dc.getBBoxMax().y + 5.0f, 0.0f, 0xFFFFFFFF );
    }
    else
    {
        addTextBillboard( dc.getSceneManager(), 4.0f, "usePointSkipping = false", dc.getBBoxCenter().x,dc.getBBoxMax().y + 5.0f, 0.0f, 0xFFFFFFFF );
    }
}


#endif // TEXTUREDMITERPOLYLINETEST_HPP
