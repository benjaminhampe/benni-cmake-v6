#include "PolyLineTest.hpp"
#include <de/hampe/svg/PolyLine.hpp>
#include <de/hampe/svg/MiterPolyLine.hpp>
#include <de/hampe/svg/TexturedMiterPolyLine.hpp>

using de::hampe::svg::LineCap;
using de::hampe::svg::LineJoin;
using de::hampe::svg::PolyLine;
using de::hampe::svg::MiterPolyLine;

void
test_AllPolyLine( PrimitiveRenderer2D & dc )
{
    glm::vec2 size;

    float32_t outlineWidth = 2.5f;
    float32_t inlineWidth = 2.1f;

    addTextBillboard( dc.getSceneManager(), 8.0f, "AllPolyLineTest", 0, 60.0f, 0.0f, 0xFFFFFFFF );

    test_PolyLine( dc, glm::vec2(0,0) );
    test_PolyLine( dc, glm::vec2(-50,0) );

//    test_TexturedMiterPolyLine_asTriangleStrip( dc, glm::vec2( -100,-50 ), inlineWidth, outlineWidth, true );
//    test_TexturedMiterPolyLine_asTriangleStrip( dc, glm::vec2(   0,-50 ),  inlineWidth, outlineWidth, false );

//    test_MiterPolyLine_asIndexedTriangles( dc, glm::vec2( 0,50 ), inlineWidth, outlineWidth, BG_COLOR, FG_COLOR );
//    test_MiterPolyLine_asTriangleStrip( dc, glm::vec2( -100,50 ), inlineWidth, outlineWidth, BG_COLOR, FG_COLOR );

//    test_MiterPolyLine_asIndexedTriangles( dc, glm::vec2( 0,0 ),    inlineWidth, outlineWidth, BG_COLOR, FG_COLOR );
//    test_MiterPolyLine_asTriangleStrip( dc, glm::vec2( -100,0 ), inlineWidth, outlineWidth, BG_COLOR, FG_COLOR );

}

class PolyLineTest
{
    CREATE_STATIC_LOGGER("de.hampe.svg.PolyLineTest")
public:
    template < bool CW >
    static void
    testPolyLineImpl(
        PrimitiveRenderer2D & dc,
        std::vector< glm::vec2 > const & points,
        glm::vec2 const pos,
        float32_t lineWidth = 2.5f,
        LineCap lineCap = LineCap::SQUARE,
        LineJoin lineJoin = LineJoin::BEVEL )
    {
        auto getPointCount = [&points] () -> uint32_t { return points.size(); };
        auto getPoint2D = [&points,&pos] ( uint32_t const index ) -> glm::vec2 { return pos + points[ index ]; };

        /// [OUTLINE]
        dc.begin( irr::scene::EPT_TRIANGLE_STRIP, 0.01f );
        PolyLine::asIndexedTriangles< CW >( dc, getPoint2D, getPointCount, lineWidth, lineCap, lineJoin );
        dc.end();
    }

    template < bool CW >
    static void
    test_TurnLeft( PrimitiveRenderer2D & dc, glm::vec2 const pos, float32_t const lineWidth, LineCap const lineCap, LineJoin const lineJoin )
    {
        std::vector< glm::vec2 > points;
        points.push_back( pos + glm::vec2( 10,0 ) );
        points.push_back( pos + glm::vec2( 10,10 ) );
        points.push_back( pos + glm::vec2( 0,10 ) );
        points.push_back( pos + glm::vec2( 0,0 ) );

        bool cw = CW;
        STATIC_LOG_DEBUG( "[TEST] TurnLeft :: cw(",cw,"), n(",points.size(),"), pos(",pos,"), lw(",lineWidth,"), cap(",lineCap,"), join(",lineJoin,")\n" )

        testPolyLineImpl< CW >( dc, points, pos, lineWidth, lineCap, lineJoin );
    }

    template < bool CW >
    static void
    test_TurnRight( PrimitiveRenderer2D & dc, glm::vec2 const pos, float32_t const lineWidth, LineCap const lineCap, LineJoin const lineJoin )
    {
        std::vector< glm::vec2 > points;
        points.push_back( pos + glm::vec2( 0,0 ) );
        points.push_back( pos + glm::vec2( 0,10 ) );
        points.push_back( pos + glm::vec2( 10,10 ) );
        points.push_back( pos + glm::vec2( 10,0 ) );

        bool cw = CW;
        STATIC_LOG_DEBUG("[TEST] TurnRight :: cw(",cw,"), n(",points.size(),"), pos(",pos,"), lw(",lineWidth,"), cap(",lineCap,"), join(",lineJoin,")\n" )

        testPolyLineImpl< CW >( dc, points, pos, lineWidth, lineCap, lineJoin );
    }

    template < bool CW >
    static void
    test_II( PrimitiveRenderer2D & dc, glm::vec2 const pos, float32_t const lineWidth, LineCap const lineCap, LineJoin const lineJoin )
    {
        std::vector< glm::vec2 > points;
        points.push_back( glm::vec2( 0,0 ) );
        points.push_back( glm::vec2( 10,0 ) );
        points.push_back( glm::vec2( 1,1 ) );
        points.push_back( glm::vec2( 9,2 ) );
        points.push_back( glm::vec2( 9,4 ) );
        points.push_back( glm::vec2( 2,3 ) );
        points.push_back( glm::vec2( 2,6 ) );

        std::cout << "[TEST] " << __FUNCTION__ << "(n:" << points.size() << ", pos:" << pos << ",lw:" << lineWidth << ",lc:" << lineCap << ",lj:" << lineJoin << ") ...\n";

        testPolyLineImpl< CW >( dc, points, pos, lineWidth, lineCap, lineJoin );
    }

    template < bool CW >
    static void
    test_Random( PrimitiveRenderer2D & dc, glm::vec2 const pos, float32_t const lineWidth, LineCap const lineCap, LineJoin const lineJoin )
    {
        glm::vec2 size( 20, 20 );
        std::vector< glm::vec2 > points;
        for ( uint32_t i = 0; i < 10; ++i )
        {
            float32_t const x = size.x * float32_t(rand()%65536) / 65536.0f;
            float32_t const y = size.y * float32_t(rand()%65536) / 65536.0f;
            points.push_back( pos + glm::vec2( x,y ) );
        }

        std::cout << "[TEST] " << __FUNCTION__ << "(n:" << points.size() << ", pos:" << pos << ",lw:" << lineWidth << ",lc:" << lineCap << ",lj:" << lineJoin << ") ...\n";

        testPolyLineImpl< CW >( dc, points, pos, lineWidth, lineCap, lineJoin );
    }

    template < bool CW >
    static void
    test_PolyLine( PrimitiveRenderer2D & dc, glm::vec2 const pos )
    {
        test_TurnLeft< CW >( dc, pos + glm::vec2( -40, 0 ), 2.5f, LineCap::SQUARE, LineJoin::NONE );
//        test_TurnLeft< CW >( dc, pos + glm::vec2( -20, 0 ), 2.5f, LineCap::SQUARE, LineJoin::BEVEL );
//        test_TurnLeft< CW >( dc, pos + glm::vec2(   0, 0 ), 2.5f, LineCap::SQUARE, LineJoin::BEVEL );
//        test_TurnLeft< CW >( dc, pos + glm::vec2(  20, 0 ), 2.5f, LineCap::SQUARE, LineJoin::BEVEL );

//        test_TurnRight< CW >( dc, pos + glm::vec2(-30, 0 ), 2.5f, LineCap::NONE, LineJoin::BEVEL );
//        test_TurnRight< CW >( dc, pos + glm::vec2(-10, 0 ), 2.5f, LineCap::SQUARE, LineJoin::BEVEL );
//        test_TurnRight< CW >( dc, pos + glm::vec2( 10, 0 ), 2.5f, LineCap::NONE, LineJoin::BEVEL);
//        test_TurnRight< CW >( dc, pos + glm::vec2( 30, 0 ), 2.5f, LineCap::SQUARE, LineJoin::BEVEL );
        test_TurnRight< CW >( dc, pos + glm::vec2( 30, 0 ), 2.5f, LineCap::SQUARE, LineJoin::BEVEL );

//        test_Random< CW >( dc, pos + glm::vec2( 40, 0 ), 2.5f, LineCap::NONE, LineJoin::NONE );

//        test_II< CW >( dc, pos + glm::vec2( -40, 10 ), 2.5f, LineCap::NONE, LineJoin::NONE );
//        test_II< CW >( dc, pos + glm::vec2(-30, 10 ), 2.5f, LineCap::NONE, LineJoin::NONE );
//        test_II< CW >( dc, pos + glm::vec2( -20, 10 ), 2.5f, LineCap::SQUARE, LineJoin::BEVEL );
//        test_II< CW >( dc, pos + glm::vec2(-10, 10 ), 2.5f, LineCap::SQUARE, LineJoin::BEVEL );
    }

};

void
test_PolyLine( PrimitiveRenderer2D & dc, glm::vec2 const pos )
{
    PolyLineTest::test_PolyLine<true>( dc, pos );
}

#if MATH_TEST
template < bool CW >
void test_II( PrimitiveRenderer2D & dc, glm::vec2 const pos, float32_t const lineWidth, LineCap const lineCap, LineJoin const lineJoin )
{
    std::vector< glm::vec2 > points;
    points.push_back( glm::vec2( 0,0 ) );
    points.push_back( glm::vec2( 100,0 ) );
    points.push_back( glm::vec2( 10,10 ) );
    points.push_back( glm::vec2( 90,20 ) );
    points.push_back( glm::vec2( 90,40 ) );
    points.push_back( glm::vec2( 20,30 ) );
    points.push_back( glm::vec2( 20,60 ) );

    float32_t const lineWidth = 2.5f;

    std::cout << "[TEST] " << __FUNCTION__ << "(n:" << points.size() << ", pos:" << pos << ",lw:" << lineWidth << ",lc:" << lineCap << ",lj:" << lineJoin << ") ...\n";

    testPolyLineImpl< CW >( points, dc, pos, lineWidth, lineCap, lineJoin );


    glm::vec2 A ( points[ 0 ] );
    glm::vec2 B ( points[ 1 ] );
    glm::vec2 C ( points[ 2 ] );
    float32_t phi = Utils::getAngleABC( A,B,C );
    std::cout << "getAngle(A,B,C) = " << phi << "\n";
    glm::vec2 nAB = Utils::getNormal2D( B - A );
    glm::vec2 nBC = Utils::getNormal2D( C - B );
    std::cout << "getNormal2D(A,B) = " << nAB << "\n";
    std::cout << "getNormal2D(B,C) = " << nBC << "\n";

}
#endif

#if 0
void testPolyLine(
        PrimitiveRenderer2D & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth,
        LineJoin const lineJoin,
        LineCap const lineCap )
{
    std::stringstream s;

    s << __FUNCTION__ << "( lineWidth=" << lineWidth eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee<< ", lineJoin=" << lineJoin << ", lineCap=" << lineCap << " )\n";

    s << "points.size() = " << points.size() << "\n";

    for ( size_t i = 0; i < points.size(); ++i )
    {
        s << "points[" << i << "] = " << points[ i ] << "\n";
    }

    addPolyLine( dc, points, lineWidth, lineJoin, lineCap );

    dc.updateGeometry();

    std::cout << s.str();
}

void test_II( PrimitiveRenderer & dc )
{
    std::cout << "\n[" << __FUNCTION__ << "]:\n\n";

    std::vector< glm::vec2 > points;
    points.push_back( glm::vec2( 0,0 ) );
    points.push_back( glm::vec2( 100,0 ) );
    points.push_back( glm::vec2( 10,10 ) );
    points.push_back( glm::vec2( 90,20 ) );
    points.push_back( glm::vec2( 90,40 ) );
    points.push_back( glm::vec2( 20,30 ) );
    points.push_back( glm::vec2( 20,60 ) );

    float32_t const lineWidth = 2.5f;

    testPolyLine( dc, points, lineWidth, LineJoin::ROUND, LineCap::SQUARE );

    glm::vec2 A ( points[ 0 ] );
    glm::vec2 B ( points[ 1 ] );
    glm::vec2 C ( points[ 2 ] );
    float32_t phi = Utils::getAngleABC( A,B,C );
    std::cout << "getAngle(A,B,C) = " << phi << "\n";
    glm::vec2 nAB = Utils::getNormal2D( B - A );
    glm::vec2 nBC = Utils::getNormal2D( C - B );
    std::cout << "getNormal2D(A,B) = " << nAB << "\n";
    std::cout << "getNormal2D(B,C) = " << nBC << "\n";

}

void test_III( PrimitiveRenderer & dc, glm::vec2 const pos )
{
    std::cout << "\n[" << __FUNCTION__ << "]:\n\n";

    std::vector< glm::vec2 > points;

    size_t const pointCount = 100;

    float32_t const phi_step = 5.0f * irr::core::PI / float32_t( pointCount ); // 10 Umdrehungen in 1000 Schritten
    float32_t phi = 0.0f;

    for ( size_t i = 0; i < pointCount; ++i )
    {
        float32_t x = 1.5f * float32_t( i );
        float32_t y = 20.0f * sin( 45.0f * phi );

        points.emplace_back( pos + glm::vec2( x, y ) );

        phi += phi_step;
    }

    float32_t const lineWidth = 1.5f;

    testPolyLine( dc, points, lineWidth, LineJoin::ROUND, LineCap::SQUARE );
}

void si( std::vector< glm::vec2 > & points, size_t n, glm::vec2 pos, glm::vec2 size, float32_t phi_start, float32_t phi_end )
{
    size_t const s0 = points.size();
    points.reserve( s0 + n );
    points.resize( s0 );

    float32_t const dx = float32_t( size.x ); // 10 Umdrehungen in 1000 Schritten
    float32_t const dy = float32_t( size.y ); // 10 Umdrehungen in 1000 Schritten
    float32_t const phi_delta = phi_end - phi_start; // 10 Umdrehungen in 1000 Schritten
    float32_t const phi_step = phi_delta / float32_t( n ); // 10 Umdrehungen in 1000 Schritten

    float32_t phi;
    for ( size_t i = 0; i < n; ++i )
    {
        phi = phi_start + phi_step * i;
        float32_t x = dx * ( phi - phi_start ) / phi_delta;
        if ( phi >= -0.0001f && phi <= 0.0001f )
            phi = 0.0001f;
        float32_t y = 0.1f * dy * sin( phi ) / phi;

        points.emplace_back( pos + glm::vec2( x, y ) );

        phi += phi_step;
    }
}

void test_IV( PrimitiveRenderer & dc, glm::vec2 const pos )
{
    std::cout << "\n[" << __FUNCTION__ << "]:\n\n";

    std::vector< glm::vec2 > points;
    si( points, 30, pos, glm::vec2(50, 50), -15.f, 15.f );
    testPolyLine( dc, points, 0.5f, LineJoin::BEVEL, LineCap::SQUARE );
}


void test_V( PrimitiveRenderer & dc, glm::vec2 const pos )
{
    std::cout << "\n[" << __FUNCTION__ << "]:\n\n";

    std::vector< glm::vec2 > points;
    si( points, 80, pos, glm::vec2(100, 100), -25.f, 25.f );
    float32_t const lineWidth = 0.5f;
    float32_t const lineColor = 0xFFFFB0FF;

    std::stringstream s;
    s << "lineWidth = " << lineWidth << "\n"
      << "lineColor = " << lineColor << "\n"
      << "points.size() = " << points.size() << "\n";

    for ( size_t i = 0; i < points.size(); ++i )
    {
        s << "points[" << i << "] = " << points[ i ] << "\n";
    }

    //addFastBeveledPolyLineAsTriangleList( dc, points, lineWidth, 0xFFFF0000, 0xFF505050, 0xFFFFFF30 );
    addFastBeveledPolyLineAsTriangles( dc, points, lineWidth, 0xFFFF6020 );

    dc.updateGeometry();

    std::cout << s.str();
}

void test_LineGeometryProcessor( PrimitiveRenderer & dc, glm::vec2 const pos )
{
    std::cout << "\n[" << __FUNCTION__ << "]:\n\n";

    std::vector< glm::vec2 > points;
    si( points, 80, pos, glm::vec2(100, 100), -25.f, 25.f );
    float32_t const lineWidth = 0.5f;
    float32_t const lineColor = 0xFFFFB0FF;

    std::stringstream s;
    s << "lineWidth = " << lineWidth << "\n"
      << "lineColor = " << lineColor << "\n"
      << "points.size() = " << points.size() << "\n";

    for ( size_t i = 0; i < points.size(); ++i )
    {
        s << "points[" << i << "] = " << points[ i ] << "\n";
    }

    LineGeometryProcessor::asTriangles( dc, points, lineWidth );

    //LineGeometryProcessor::asTriangleList( dc, points, lineWidth );

    dc.updateGeometry();

    std::cout << s.str();
}



#endif
