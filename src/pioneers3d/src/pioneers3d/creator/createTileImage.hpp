#ifndef PIONEERS3D_TEXTURE_DATA_HPP
#define PIONEERS3D_TEXTURE_DATA_HPP

#include <pioneers3d/Types.hpp>

namespace pioneers3d {


    irr::video::IImage*
    createImageWasser( irr::video::IVideoDriver* driver, int w, int h );

    irr::video::IImage*
    createImageLandWueste( irr::video::IVideoDriver* driver, int w, int h );

    irr::video::IImage*
    createImageLandHolz( irr::video::IVideoDriver* driver, int w, int h );

    irr::video::IImage*
    createImageLandLehm( irr::video::IVideoDriver* driver, int w, int h );

    irr::video::IImage*
    createImageLandWolle( irr::video::IVideoDriver* driver, int w, int h );

    irr::video::IImage*
    createImageLandWeizen( irr::video::IVideoDriver* driver, int w, int h );

    irr::video::IImage*
    createImageLandErz( irr::video::IVideoDriver* driver, int w, int h );

    /// @brief Create tile images manully from random patterns as low quality fallback

/*
    inline irr::video::ITexture*
    createTileTexture( eTileType tileType, irr::video::IVideoDriver* driver, int w = -1, int h = -1 )
    {
        std::cout << __FUNCTION__ << "()\n";
        assert( driver );

        irr::video::IImage * img = nullptr;
        switch( tileType )
        {
            case eTileType::WASSER: img = createImageWasser( driver, w, h ); break;
            case eTileType::LAND_WUESTE: img = createImageLandWueste( driver, w, h ); break;
            case eTileType::LAND_HOLZ: img = createImageLandHolz( driver, w, h ); break;
            case eTileType::LAND_LEHM: img = createImageLandLehm( driver, w, h ); break;
            case eTileType::LAND_ERZ: img = createImageLandErz( driver, w, h ); break;
            case eTileType::LAND_WOLLE: img = createImageLandWolle( driver, w, h ); break;
            case eTileType::LAND_WEIZEN: img = createImageLandWeizen( driver, w, h ); break;
            default: break;
        }

        if ( !img )
        {
            std::cout << "Cannot create image.\n";
            return nullptr;
        }

        irr::video::ITexture* tex = driver->addTexture( "", img );

        img->drop();

        if ( !tex )
        {
            std::cout << "Cannot create texture.\n";
            return nullptr;
        }

        return tex;
    }
*/

#if 0
    void
    createLowQualityTextures( TextureManager & data, irr::video::IVideoDriver* driver, int w, int h )
    {
        std::cout << __FUNCTION__ << "()\n";
        assert( driver );

        std::string key;

        key = eTileTypeToString( eTileType::WASSER ); data.addTextureFromImage( key, createImageWasser( driver, w, h ), key + ".png", true );
        key = eTileTypeToString( eTileType::LAND_WUESTE ); data.addTextureFromImage( key, createImageLandWueste( driver, w, h ), key + ".png", true );
        key = eTileTypeToString( eTileType::LAND_HOLZ ); data.addTextureFromImage( key, createImageLandHolz( driver, w, h ), key + ".png", true );
        key = eTileTypeToString( eTileType::LAND_LEHM ); data.addTextureFromImage( key, createImageLandLehm( driver, w, h ), key + ".png", true );
        key = eTileTypeToString( eTileType::LAND_ERZ ); data.addTextureFromImage( key, createImageLandErz( driver, w, h ), key + ".png", true );
        key = eTileTypeToString( eTileType::LAND_WOLLE ); data.addTextureFromImage( key, createImageLandWolle( driver, w, h ), key + ".png", true );
        key = eTileTypeToString( eTileType::LAND_WEIZEN ); data.addTextureFromImage( key, createImageLandWeizen( driver, w, h ), key + ".png", true );
    }

    void
    createStandardTextures( TextureManager & data, irr::video::IVideoDriver* driver, int w, int h )
    {
        std::cout << __FUNCTION__ << "()\n";
        assert( driver );

        std::string key;
        key = eTileTypeToString( eTileType::WASSER ); data.addTextureFromImageFile( key, key + ".png" );
        key = eTileTypeToString( eTileType::LAND_WUESTE ); data.addTextureFromImageFile( key, key + ".png" );
        key = eTileTypeToString( eTileType::LAND_HOLZ ); data.addTextureFromImageFile( key, key + ".png" );
        key = eTileTypeToString( eTileType::LAND_LEHM ); data.addTextureFromImageFile( key, key + ".png" );
        key = eTileTypeToString( eTileType::LAND_ERZ ); data.addTextureFromImageFile( key, key + ".png" );
        key = eTileTypeToString( eTileType::LAND_WOLLE ); data.addTextureFromImageFile( key, key + ".png" );
        key = eTileTypeToString( eTileType::LAND_WEIZEN ); data.addTextureFromImageFile( key, key + ".png" );
    }
#endif


} // end namespace pioneers3d

#endif // PIONEERS3D_TEXTURE_DATA_HPP
