#include "createTileImage.hpp"

namespace pioneers3d {

irr::video::IImage*
createImageWasser( irr::video::IVideoDriver* driver, int w, int h )
{
    irr::video::IImage* img = createImage( driver, w, h );

    srand( (unsigned int)time(NULL) );

    for ( irr::u32 y = 0; y < img->getDimension().Height; ++y )
    {
        for ( irr::u32 x = 0; x < img->getDimension().Width; ++x )
        {
            const int r = (rand() % 20) + 10;
            const int g = (rand() % 30) + 20;
            const int b = (rand() % 115) + 140;
            img->setPixel( x, y, irr::video::SColor( 255, r, g, b ) );
        }
    }

    return img;
}

irr::video::IImage*
createImageLandWueste( irr::video::IVideoDriver* driver, int w, int h )
{
    irr::video::IImage* img = createImage( driver, w, h );

    srand( (unsigned int)time(NULL) );

    for ( irr::u32 y = 0; y < img->getDimension().Height; ++y )
    {
        for ( irr::u32 x = 0; x < img->getDimension().Width; ++x )
        {
            const int r = 255-(rand() % 65);
            const int g = 255-(rand() % 115);
            const int b = (rand() % 30) + 210;
            img->setPixel( x, y, irr::video::SColor( 255, r, g, b ) );
        }
    }

    return img;
}

irr::video::IImage*
createImageLandHolz( irr::video::IVideoDriver* driver, int w, int h )
{
    irr::video::IImage* img = createImage( driver, w, h );

    srand( (unsigned int)time(NULL) );

    for ( irr::u32 y = 0; y < img->getDimension().Height; ++y )
    {
        for ( irr::u32 x = 0; x < img->getDimension().Width; ++x )
        {
            const int r = (rand() % 20) + 10;
            const int g = (rand() % 115) + 140;
            const int b = (rand() % 30) + 20;
            img->setPixel( x, y, irr::video::SColor( 255, r, g, b ) );
        }
    }

    return img;
}

irr::video::IImage*
createImageLandLehm( irr::video::IVideoDriver* driver, int w, int h )
{
    irr::video::IImage* img = createImage( driver, w, h );

    srand( (unsigned int)time(NULL) );

    for ( irr::u32 y = 0; y < img->getDimension().Height; ++y )
    {
        for ( irr::u32 x = 0; x < img->getDimension().Width; ++x )
        {
            const int r = (rand() % 115) + 140;
            const int g = (rand() % 115) + 140;
            const int b = (rand() % 30) + 20;
            img->setPixel( x, y, irr::video::SColor( 255, r, g, b ) );
        }
    }

    return img;
}


irr::video::IImage*
createImageLandWolle( irr::video::IVideoDriver* driver, int w, int h )
{
    irr::video::IImage* img = createImage( driver, w, h );

    srand( (unsigned int)time(NULL) );

    for ( irr::u32 y = 0; y < img->getDimension().Height; ++y )
    {
        for ( irr::u32 x = 0; x < img->getDimension().Width; ++x )
        {
            const int r = (rand() % 30) + 60;
            const int g = 255-(rand() % 100);
            const int b = (rand() % 30) + 60;
            img->setPixel( x, y, irr::video::SColor( 255, r, g, b ) );
        }
    }

    return img;
}


irr::video::IImage*
createImageLandWeizen( irr::video::IVideoDriver* driver, int w, int h )
{
    irr::video::IImage* img = createImage( driver, w, h );

    srand( (unsigned int)time(NULL) );

    for ( irr::u32 y = 0; y < img->getDimension().Height; ++y )
    {
        for ( irr::u32 x = 0; x < img->getDimension().Width; ++x )
        {
            const int r = 255-(rand() % 65);
            const int g = 255-(rand() % 115);
            const int b = (rand() % 30) + 20;
            img->setPixel( x, y, irr::video::SColor( 255, r, g, b ) );
        }
    }

    return img;
}

irr::video::IImage*
createImageLandErz( irr::video::IVideoDriver* driver, int w, int h )
{
    irr::video::IImage* img = createImage( driver, w, h );

    srand( (unsigned int)time(NULL) );

    for ( irr::u32 y = 0; y < img->getDimension().Height; ++y )
    {
        for ( irr::u32 x = 0; x < img->getDimension().Width; ++x )
        {
            //const int r = (rand() % 30) + 60;
            const int g = 255-(rand() % 190);
            //const int b = (rand() % 30) + 60;
            img->setPixel( x, y, irr::video::SColor( 255, g, g, g ) );
        }
    }

    return img;
}


} // end namespace pioneers3d

