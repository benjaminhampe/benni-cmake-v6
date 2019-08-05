#include "PolyLineTest.hpp"

#include <de/hampe/polyline/PolyLine.hpp>
#include <de/hampe/polyline/FastPolyLine.hpp>
#include <de/hampe/polyline/addPolyLineABC.hpp>

#include <de/hampe/polyline/LineGeometryProcessor.hpp>

void testPolyLine(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth,
        LineJoin const lineJoin,
        LineCap const lineCap )
{
    std::stringstream s;

    s << __FUNCTION__ << "( lineWidth=" << lineWidth << ", lineJoin=" << lineJoin << ", lineCap=" << lineCap << " )\n";

    s << "points.size() = " << points.size() << "\n";

    for ( size_t i = 0; i < points.size(); ++i )
    {
        s << "points[" << i << "] = " << points[ i ] << "\n";
    }

    addPolyLine( dc, points, lineWidth, lineJoin, lineCap );

    dc.updateGeometry();

    std::cout << s.str();
}

void test_I_turn_left( PrimitiveRenderer & dc, glm::vec2 const pos )
{
    std::cout << "\n[" << __FUNCTION__ << "]:\n\n";

    std::vector< glm::vec2 > points;
    points.push_back( pos + glm::vec2( 0,0 ) );
    points.push_back( pos + glm::vec2( 0,10 ) );
    points.push_back( pos + glm::vec2( -10,10 ) );
    points.push_back( pos + glm::vec2( -10,0 ) );

    testPolyLine( dc, points, 2.5f, LineJoin::ROUND, LineCap::SQUARE );
}

void test_I_turn_right( PrimitiveRenderer & dc, glm::vec2 const pos )
{
    std::cout << "\n[" << __FUNCTION__ << "]:\n\n";

    std::vector< glm::vec2 > points;
    points.push_back( pos + glm::vec2( 0,0 ) );
    points.push_back( pos + glm::vec2( 0,10 ) );
    points.push_back( pos + glm::vec2( 10,10 ) );
    points.push_back( pos + glm::vec2( 10,0 ) );

    testPolyLine( dc, points, 2.5f, LineJoin::ROUND, LineCap::SQUARE );
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



void PolyLineTest( PrimitiveRenderer & dc )
{
    UtilsTest::testAll();

    test_I_turn_left( dc, glm::vec2( -50, 0 ) );
    test_I_turn_right( dc, glm::vec2( -50, 50 ) );
    //test_II( dc );
    //test_III( dc, glm::vec2( 0, -50 ) );
    //test_IV( dc, glm::vec2( 0, -50 ) );
    //test_V( dc, glm::vec2( 0, 0 ) );
    test_LineGeometryProcessor( dc, glm::vec2( 50, -100 ) );
}

void PolyLineABCTest( PrimitiveRenderer & dc )
{
    float32_t lineWidth = 12.5f;

    for ( uint32_t k = 0; k < 10; ++k )
    {
        glm::vec2 A( 100.f + 0.01f * ( rand() % 10000 ), 0.01f * ( rand() % 10000 ) );
        glm::vec2 B( 100.f + 0.01f * ( rand() % 10000 ), 0.01f * ( rand() % 10000 ) );
        glm::vec2 C( 100.f + 0.01f * ( rand() % 10000 ), 0.01f * ( rand() % 10000 ) );
        lineWidth = 1.0f + 0.01f * ( rand() % 100 );
        addPolyLineABC( dc, A, B, C, lineWidth );
    }

    dc.updateGeometry();

}


