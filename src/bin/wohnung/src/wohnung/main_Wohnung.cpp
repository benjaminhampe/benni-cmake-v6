#include <wohnung/Wohnung.hpp>

int main( int argc, char * argv[] )
{
   const char* appName = "Wohnung (c) 2019 by Benjamin Hampe <benjaminhampe@gmx.de>";

   std::string fileName = "tux.png";

   std::cout << "// ==========================================================\n";
   std::cout << "// prgm: " << argv[0] << ":\n";
   std::cout << "// ==========================================================\n";

   srand( (unsigned int)time( nullptr ) );

	irrExt::OpenGLDevice oglDevice( 1280, 800, 32 );
   irr::IrrlichtDevice* device = oglDevice.getDevice();
	
   assert( device );
	
   device->setResizable( true );
   device->setWindowCaption( irr::core::stringw( appName ).c_str() );

   BaseEventReceiver receiver( device );

   // addSkyBox( AutoSceneNode* )
//   {
//     irr::video::ITexture* top = Game_getTexture( game, eTexture::SKYBOX_TOP );
//     irr::video::ITexture* bottom = Game_getTexture( game, eTexture::SKYBOX_BOTTOM );
//     irr::video::ITexture* left = Game_getTexture( game, eTexture::SKYBOX_LEFT );
//     irr::video::ITexture* right = Game_getTexture( game, eTexture::SKYBOX_RIGHT );
//     irr::video::ITexture* front = Game_getTexture( game, eTexture::SKYBOX_FRONT );
//     irr::video::ITexture* back = Game_getTexture( game, eTexture::SKYBOX_BACK );
//     //irr::scene::ISceneNode* skyBox =
//     smgr->addSkyBoxSceneNode( top, bottom, left, right, front, back, smgr->getRootSceneNode(), -1 );
//   }

   irr::video::IVideoDriver* driver = device->getVideoDriver();
   irr::scene::ISceneManager* smgr = device->getSceneManager();
   irr::scene::ISceneNode* rootNode = smgr->getRootSceneNode();
   irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
   irr::core::dimension2du screenSize = driver->getScreenSize();
   irr::video::SColor clearColor( 255,80,80,200 );

	
   irr::scene::ICameraSceneNode* cam = FpsCamera::create( smgr );
   cam->setNearValue( 1.0f );
   cam->setFarValue( 10000.0f );
   cam->setPosition( irr::core::vector3df(0,200,-300) );
   cam->setTarget( irr::core::vector3df(0,0,0) );

   irr::core::vector3df cubePos( 0, 0, 0);
   irr::core::vector3df cubeAng( 0, 0, 0);
   irr::core::vector3df cubeScale( 1, 1, 1 );
   irr::scene::IMeshSceneNode* cube = smgr->addCubeSceneNode( 100.0f, rootNode, -1, cubePos, cubeAng, cubeScale );
   cube->setMaterialType( irr::video::EMT_SOLID );
   cube->setMaterialFlag( irr::video::EMF_LIGHTING, false );
   cube->setMaterialFlag( irr::video::EMF_FOG_ENABLE, false );

   irr::video::ITexture* tex = driver->getTexture( fileName.c_str() );

   //irr::video::ITexture* tex = driver->getTexture("pcc_logo.png");

   std::cout << "dbFileExist() = " << dbFileExist( device, fileName ) << "\n";
   std::cout << "ITexture = " << tex << "\n";

   cube->setMaterialTexture( 0, tex );

   irr::core::vector3df cubeAngSpeed( 2.f, 3.f, 0.1f );
   irr::scene::ISceneNodeAnimator* cubeAnim = smgr->createRotationAnimator( cubeAngSpeed );
   cube->addAnimator( cubeAnim );
   cubeAnim->drop();

   addTestObject( smgr );

   /// MAIN LOOP
   uint64_t timeNow = device->getTimer()->getRealTime();
   uint64_t timeStart = timeNow;
   uint64_t timeLastScreenUpdate = device->getTimer()->getRealTime();
   uint64_t timeLastWindowTitleUpdate = device->getTimer()->getRealTime();
   uint64_t waitUpdateScreen = 0;
   uint64_t waitUpdateWindowTitle = 1000 / 10;

   irr::core::dimension2du lastWindowSize = irr::core::dimension2du( 0,0 );

   while (device && device->run())
   {
      timeNow = device->getTimer()->getRealTime();

      if ( driver )
      {
         screenSize = driver->getScreenSize();
         if ( lastWindowSize != irr::core::dimension2du( 0,0 )
            && screenSize != lastWindowSize )
         {
            float aspect = 1.0f;
            if ( screenSize.Height > 0 )
            {
               aspect = float( screenSize.Width ) / float( screenSize.Height );
            }

            irr::scene::ICameraSceneNode* camera = smgr->getActiveCamera();
            if ( camera )
            {
               camera->setAspectRatio( aspect );
            }
         }
      }

      if ( device->isWindowActive() &&
           timeNow - timeLastScreenUpdate >= waitUpdateScreen )
      {

         timeLastScreenUpdate = timeNow;

         if ( driver )
         {
            driver->beginScene( true, true, clearColor );

            if ( smgr )
            {
               smgr->drawAll();
            }

            if ( guienv )
            {
               guienv->drawAll();
            }

            driver->endScene();

            if ( timeNow - timeLastWindowTitleUpdate >= waitUpdateWindowTitle )
            {
               timeLastWindowTitleUpdate = timeNow;

               std::stringstream s;
               s << appName;
               s << "- FPS(" << driver->getFPS() << ")";
               s << ", Screen(" << screenSize.Width << "," << screenSize.Height << ")";

               irr::scene::ICameraSceneNode* camera = smgr->getActiveCamera();
               if ( camera )
               {

                 s << ", CamPos(" << camera->getAbsolutePosition() << ")";
                 s << ", CamLookAt(" << camera->getTarget() << ")";
                 s << ", CamPhi(" << camera->getRotation() << ")";
               }

               s << ", POLY(" << device->getVideoDriver()->getPrimitiveCountDrawn() << ")";
               s << ", TEX(" << device->getVideoDriver()->getTextureCount() << ")";

               device->setWindowCaption( irr::core::stringw( s.str().c_str() ).c_str() );
            }
         }

      }
      else
      {
         device->yield();
      }

   }
   device->closeDevice();
   device->drop();
   device = nullptr;
   return 0;

}

