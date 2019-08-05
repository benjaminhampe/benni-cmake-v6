#ifndef GEOPOSITION_HPP
#define GEOPOSITION_HPP

#include <de/hampe/Types.hpp>

// Dont use this class for computations, only for storage,
// when compute, get nds or wgs coords out of it,
// and make a new GeoPosition out of the result, that is safe because it checks before storing again.
// The checks include fmod around the globe and clamping to border
// x - Nullmeridian Greenwhich Laengengrad ( East/Ost ist positiv )
// y - Aequator Breitengrad ( Nord ist positiv )
class GeoPosition
{
    glm::ivec2 m_NDS;
public:
    constexpr static float64_t const UMFANG_EINHEITSKREIS_RAD = 2.0 * M_PI;
    constexpr static float64_t const UMFANG_ERDE_X_METER = 44000000.0;
    constexpr static float64_t const UMFANG_ERDE_Y_METER = 42000000.0;
    constexpr static glm::dvec2 const NDS_TO_WGS = glm::dvec2(  UMFANG_EINHEITSKREIS_RAD / UMFANG_ERDE_X_METER, UMFANG_EINHEITSKREIS_RAD / UMFANG_ERDE_Y_METER );
    constexpr static glm::dvec2 const WGS_TO_NDS = glm::dvec2(  UMFANG_ERDE_X_METER / UMFANG_EINHEITSKREIS_RAD, UMFANG_ERDE_Y_METER / UMFANG_EINHEITSKREIS_RAD );
    constexpr static int32_t const NDS_TOTALRANGE = 0x40000000; // 4 . 0100 0000 0000 0000 . 0000 0000 0000 0000
    constexpr static int32_t const MIN_LONGITUDE_NDS = std::numeric_limits< int32_t >::lowest();
    constexpr static int32_t const MAX_LONGITUDE_NDS = std::numeric_limits< int32_t >::max();
    constexpr static int32_t const MIN_LATITUDE_NDS = std::numeric_limits< int32_t >::lowest();
    constexpr static int32_t const MAX_LATITUDE_NDS = std::numeric_limits< int32_t >::max();
    constexpr static float64_t const MIN_LONGITUDE_WGS = -179.999;
    constexpr static float64_t const MAX_LONGITUDE_WGS = 179.999;
    constexpr static float64_t const MIN_LATITUDE_WGS = -89.999;
    constexpr static float64_t const MAX_LATITUDE_WGS = 89.999;

    static GeoPosition fromNDS( glm::ivec2 const nds ) { return GeoPosition( nds ); } // uses nds constructor
    static GeoPosition fromWGS( glm::dvec2 const wgs ) { return GeoPosition( wgs ); } // uses wgs constructor

    ~GeoPosition() = default;
    GeoPosition() = default;
    GeoPosition( glm::ivec2 const nds ) : m_NDS() { setNDS( nds ); }
    GeoPosition( glm::dvec2 const wgs ) : m_NDS() { setWGS( wgs ); }
    GeoPosition( GeoPosition const & pos ) : m_NDS() { setNDS( pos.toNDS() ); }
    GeoPosition& operator=( GeoPosition const & pos ) { setNDS( pos.toNDS() ); return *this; }

    glm::ivec2 toNDS() const { return m_NDS; }
    glm::dvec2 toWGS() const { return NDS_TO_WGS * glm::dvec2( m_NDS.x, m_NDS.y ); } // SIMD ?
    float64_t x() const { return m_NDS.x * NDS_TO_WGS.x; }
    float64_t y() const { return m_NDS.y * NDS_TO_WGS.y; }
    int32_t getLatitude() const { return m_NDS.x; }
    int32_t getLongitude() const { return m_NDS.y; }

    void setNDS( glm::ivec2 const nds ) { m_NDS = clampNDS( nds ); }
    void setWGS( glm::dvec2 const wgs ) { m_NDS = clampWGS( wgs ) * WGS_TO_NDS; }

    // use SIMD
    static glm::ivec2 clampNDS( glm::ivec2 const nds ) {
        return glm::clamp( nds, glm::ivec2( MIN_LONGITUDE_NDS, MIN_LATITUDE_NDS ), glm::ivec2( MAX_LONGITUDE_NDS, MAX_LATITUDE_NDS ) );
    }

    // use SIMD
    static glm::dvec2 clampWGS( glm::dvec2 const & wgs ) {
        return glm::clamp( wgs, glm::dvec2( MIN_LONGITUDE_WGS, MIN_LATITUDE_WGS ), glm::dvec2( MAX_LONGITUDE_WGS, MAX_LATITUDE_WGS ) );
    }

    // not so SIMD
    static float64_t limitLongitudeWGS( float64_t x ) { return glm::clamp( x, MIN_LONGITUDE_WGS, MAX_LONGITUDE_WGS ); }
    static float64_t limitLatitudeWGS( float64_t y ) { return glm::clamp( y, MIN_LATITUDE_WGS, MAX_LATITUDE_WGS ); }
};

#endif // GEOPOSITION_HPP
