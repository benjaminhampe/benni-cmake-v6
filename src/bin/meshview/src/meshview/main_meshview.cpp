#include <meshview/meshview.hpp>

int main( int argc, char * argv[] )
{
   srand( (unsigned int)time( nullptr ) );

   const char* appName = "MeshViewer (c) 2019 by <BenjaminHampe@gmx.de>";

   if ( argc < 2 )
   {
      std::cout << "[Error] " << __FUNCTION__ << " Must give atleast one filename as argument(s)\n";
      return 0;
   }

   std::string fileName = argv[ 1 ];

   if ( fileName.size() < 3 )
   {
      std::cout << "[Error] " << __FUNCTION__ << " Invalid fileName(" << fileName << ")\n";
      return 0;
   }

   if ( !irrExt::existFile( fileName ) )
   {
      std::cout << "[Error] " << __FUNCTION__ << " File not found(" << fileName << ")\n";
      return 0;
   }

   irrExt::OpenGLDevice oglDevice;
   irr::IrrlichtDevice* device = oglDevice.getDevice();
   if ( !device )
   {
      std::cout << "[Error] " << __FUNCTION__ << " Cannot create Irrlicht device\n";
      return 0;
   }

   device->setWindowCaption( irr::core::stringw( appName ).c_str() );
   irrExt::BaseEventReceiver receiver( device );

   irr::video::IVideoDriver* driver = device->getVideoDriver();
   irr::scene::ISceneManager* smgr = device->getSceneManager();
   irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
   irr::core::dimension2du screenSize = driver->getScreenSize();
   irr::video::SColor clearColor( 255,80,80,200 );

   irr::scene::ICameraSceneNode* camera = irrExt::FpsCamera::create( smgr );

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

// MESH-FILE:

   std::cout << "[Ok] " << __FUNCTION__ << " Loading file(" << fileName << ")...\n";

   uint32_t const globalTexCount = smgr->getVideoDriver()->getTextureCount();

   // mesh
   irr::scene::IAnimatedMesh* mesh = smgr->getMesh( fileName.c_str() );
   if ( !mesh )
   {
      std::cout << "[Error] " << __FUNCTION__ << " Cannot read file(" << fileName << ")\n";
   }
   else
   {
      uint32_t texCount = smgr->getVideoDriver()->getTextureCount() - globalTexCount; // Delta loaded textures
      uint32_t vertexCount = irrExt::Mesh_getVertexCount( mesh );
      uint32_t indexCount = irrExt::Mesh_getIndexCount( mesh );
      glm::vec3 meshSize = irrExt::Mesh_getSize( mesh );
      int32_t objId = irrExt::SceneNode_getFreeId( smgr );

      std::cout << "[Ok] " << __FUNCTION__ << " Loaded mesh: "
            << "id(" << objId << "), "
            << "file(" << fileName << "), "
            << "size(" << meshSize << "), "
            << "tex(" << texCount << "), "
            << "vertex(" << vertexCount << "), "
            << "index(" << indexCount << "), "
            << "\n";

      irr::scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh, smgr->getRootSceneNode(), objId );
      mesh->drop();

      if ( !node )
      {
         std::cout << "[Error] " << __FUNCTION__ << " Cannot create scene node(" << fileName << ")\n";
         device->closeDevice();
         return 0;
      }
   }

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
   return 0;
}

