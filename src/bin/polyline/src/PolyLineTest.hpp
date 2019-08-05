#ifndef POLYLINETEST_HPP
#define POLYLINETEST_HPP

#include <de/hampe/logging/Logger.hpp>
#include <de/hampe/svg/PrimitiveRenderer.hpp>
#include <de/hampe/svg/addTextBillboard.hpp>
#include "PointGenerator.hpp"

constexpr uint32_t POINT_COUNT = 32*32;
constexpr float32_t SINC_MIN = -65.0f;
constexpr float32_t SINC_MAX = 65.0f;
constexpr uint32_t BG_COLOR = 0xFFFFFF00;
constexpr uint32_t FG_COLOR = 0xFFFF0000;
constexpr float32_t BG_WIDTH = 2.5f;
constexpr float32_t FG_WIDTH = 2.1f;

inline irr::video::ITexture* 
createRoadTexture( irr::video::IVideoDriver* driver, float32_t outlineWidth, float32_t inlineWidth, uint32_t outlineColor, uint32_t inlineColor)
{
    CREATE_LOGGER(__FUNCTION__)

    uint32_t const w = 512;
    uint32_t const h = 1;
    irr::video::IImage* img = driver->createImage( irr::video::ECF_A8R8G8B8, irr::core::dimension2du( w, h ) );
    if ( !img )
    {
        LOG_ERROR("Cannot create image\n")
        return nullptr;
    }
    img->fill( outlineColor );

    int32_t const x1 = std::floor( float32_t(w) * ( 1.0f - inlineWidth / outlineWidth ) );
    int32_t const x2 = int32_t(w - 1) - x1;

    for ( int32_t x = x1; x <= x2; ++x )
    {
        img->setPixel( x, 0, inlineColor, false );
    }

    std::stringstream ss; ss << "texOUT.jpg"; // ss << std::hex << outlineColor << "IN" << inlineColor << ".jpg";

    if ( driver->writeImageToFile( img, ss.str().c_str() ) )
    {
        LOG_ERROR("Cannot write image to file(", ss.str(), ")\n" )
    }
    else
    {
        LOG_ERROR("Saved image file(", ss.str(), ")\n" )
    }

    irr::video::ITexture* tex = driver->addTexture( ss.str().c_str(), img, 0 );
    if ( !tex )
    {
        LOG_ERROR("Cannot create texture\n")
        return nullptr;
    }
    return tex;
}

void
test_AllPolyLine( PrimitiveRenderer2D & dc );


void
test_PolyLine(
        PrimitiveRenderer2D & dc,
        glm::vec2 const pos );


#endif // POLYLINETEST_HPP
