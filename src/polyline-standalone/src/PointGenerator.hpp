#ifndef POINTGENERATOR_HPP
#define POINTGENERATOR_HPP

#include <de/hampe/logging/Logger.hpp>
#include <de/hampe/svg/PrimitiveRenderer.hpp>
#include <de/hampe/svg/addTextBillboard.hpp>

constexpr bool USE_POINT_LOGGING = false;

inline std::string
toString( std::vector< glm::vec2 > const & points )
{
    std::stringstream s;
    s << "points.size() = " << points.size() << "\n";
    for ( size_t i = 0; i < points.size(); ++i )
    {
        s << "points[" << i << "] = " << points[ i ] << "\n";
    }

    return s.str();
}

inline void
sinc( 	std::vector< glm::vec2 > & points,
        size_t n,
        glm::vec2 pos,
        glm::vec2 size,
        float32_t phi_start,
        float32_t phi_end )
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

        // prevent div by zero
        if ( std::fabs( phi ) <= 0.0001f )
            phi = 0.0001f;

        float32_t y = 0.2f * dy * sin( phi ) / phi;

        points.emplace_back( pos + glm::vec2( x, y ) );

        phi += phi_step;
    }
}

inline void
randomCloud( 	std::vector< glm::vec2 > & points,
                size_t n,
                glm::vec2 pos,
                glm::vec2 size )
{
    size_t const s0 = points.size();
    points.reserve( s0 + n );
    points.resize( s0 );

    auto randomTileLocalPos = [] () -> glm::vec2
    {
        float32_t const x = -1.0 + 2.0 * float64_t(rand() % 65536) / 65536.0; // random tile local pos in range [-1,1], to test numeric stability
        float32_t const y = -1.0 + 2.0 * float64_t(rand() % 65536) / 65536.0; // random tile local pos in range [-1,1], to test numeric stability
        return glm::vec2( x,y );
    };

    glm::vec2 const translation = glm::vec2( 1.0f, 1.0f );
    glm::vec2 const scale = glm::vec2( 0.5f * size.x, 0.5f * size.y );

    for ( size_t i = 0; i < n; ++i )
    {
        auto const p1 = randomTileLocalPos();

        points.emplace_back( pos + ( p1 + translation ) * scale );
    }
}

inline void
drawPointsAsDots( PrimitiveRenderer2D & dc, std::vector< glm::vec2 > const & points )
{
    /// [ORIGINAL POINTS]
    dc.begin( irr::scene::EPT_POINTS, 0.1f );
    dc.setMaterialFlag( irr::video::EMF_BACK_FACE_CULLING, false );
    dc.getMaterial(0).Thickness = 13.5f;
    for ( size_t i = 0; i < points.size(); ++i )
    {
        dc.addVertex2D( points[i], 0xFFFFFFFF, glm::vec2() );
        dc.addIndex( i );
    }
    dc.end();
}

inline void
drawPointsAsBoxes( PrimitiveRenderer2D & dc, std::vector< glm::vec2 > const & points,
                    glm::vec2 const size /* = glm::vec2(0.5f, 0.5f) */,
                    uint32_t colorA /* = 0xFFFFFFFF */,
                    uint32_t colorB /* = 0xFFFFFF00 */,
                    uint32_t colorC /* = 0xFFFF0000 */,
                    uint32_t colorD /* = 0xFF00FF00 */ )
{
    auto addQuad = [&dc,size,colorA,colorB,colorC,colorD] ( glm::vec2 pos )
    {
        size_t const v0 = dc.getVertexCount();
        float32_t const dx = 0.5f * size.x;
        float32_t const dy = 0.5f * size.y;
        dc.addVertex2D( pos + glm::vec2(-dx,-dy), colorA, glm::vec2( 0,1 ) ); // A
        dc.addVertex2D( pos + glm::vec2(-dx, dy), colorB, glm::vec2( 0,0 ) ); // B
        dc.addVertex2D( pos + glm::vec2( dx, dy), colorC, glm::vec2( 1,0 ) ); // C
        dc.addVertex2D( pos + glm::vec2( dx,-dy), colorD, glm::vec2( 1,1 ) ); // D
        dc.addIndexedTriangle( v0, v0 + 1, v0 + 2 );
        dc.addIndexedTriangle( v0, v0 + 2, v0 + 3 );
    };

    dc.begin( irr::scene::EPT_TRIANGLES, 0.1f );
    dc.setMaterialTexture( 0, nullptr );
    for ( size_t i = 0; i < points.size(); ++i )
    {
        addQuad( points[ i ] );
    }
    //dc.setMaterialFlag( irr::video::EMF_BACK_FACE_CULLING, false );
    //dc.getMaterial(0).Thickness = 13.5f;
    dc.end();
}

inline void
drawPointsAsBoxes( PrimitiveRenderer2D & dc, std::vector< glm::vec2 > const & points,
                    glm::vec2 const size,
                    uint32_t color )
{
    drawPointsAsBoxes( dc, points, size, color, color, color, color );
}


#endif // POINTGENERATOR_HPP
