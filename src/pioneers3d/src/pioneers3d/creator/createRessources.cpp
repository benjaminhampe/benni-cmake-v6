#include "createRessources.hpp"

void createRessourceCard( irr::video::IVideoDriver * driver, std::string const & bgFile, std::string const & fgFile, std::string const & dstFile );

void createRessourceCards( irr::video::IVideoDriver * driver, std::string const & basePath )
{
    std::string dir = basePath + "/tiles/";

    createRessourceCard( driver, dir + "holz_bg.jpg", dir + "holz.jpg", dir + "card_holz.jpg" );
    //createRessourceCard( driver, dir + "holz_bg.jpg", dir + "holz.jpg", dir + "card_holz.jpg" );
    //createRessourceCard( driver, dir + "holz_bg.jpg", dir + "holz.jpg", dir + "card_holz.jpg" );
    //createRessourceCard( driver, dir + "holz_bg.jpg", dir + "holz.jpg", dir + "card_holz.jpg" );
}

void createRessourceCard( irr::video::IVideoDriver * driver, std::string const & bgFile, std::string const & fgFile, std::string const & dstFile )
{
    assert( driver );
    irr::video::ITexture * bgTex = driver->getTexture( bgFile.c_str() );
    irr::video::ITexture * fgTex = driver->getTexture( fgFile.c_str() );

    uint32_t w = 512;
    uint32_t h = w;

    irr::video::ITexture * rtt0 = driver->addRenderTargetTexture( irr::core::dimension2du( w,h ), "card_rtt", irr::video::ECF_A8R8G8B8 );

    assert( rtt0 );

    driver->setRenderTarget( rtt0, true, true, irr::video::SColor(0,0,0,0) );

    driver->beginScene( false, false );



    driver->endScene();

    driver->setRenderTarget(0);

    //irr::video::ITexture * dstTex = driver->getTexture( dstFile.c_str() );
}
