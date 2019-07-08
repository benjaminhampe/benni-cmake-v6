#include <cstdint>
#include <irrlicht.h>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <time.h>

template < typename T >
std::ostream& operator<< ( std::ostream& out, irr::core::vector2d< T > const & v )
{
   out << v.X << "," << v.Y;
   return out;
}

template < typename T >
std::ostream& operator<< ( std::ostream& out, irr::core::vector3d< T > const & v )
{
   out << v.X << "," << v.Y << "," << v.Z;
   return out;
}

inline bool 
dbFileExist( irr::IrrlichtDevice* device, std::string fileName )
{
	return device->getFileSystem()->existFile( fileName.c_str() );
}
	
//template < typename T >
//std::ostream& operator<< ( std::ostream& out, irr::core::vector4d< T > const & v )
//{
//   out << v.X << "," << v.Y << "," << v.Z << "," << v.W;
//   return out;
//}

class MyEventReceiver : public irr::IEventReceiver
{
public:
    explicit MyEventReceiver( irr::IrrlichtDevice* device = nullptr );
    bool OnEvent( const irr::SEvent& event ) override;
    void OnResize( irr::core::dimension2du const & winSize );
    void setDevice( irr::IrrlichtDevice* device );

public:
    irr::IrrlichtDevice* m_Device = nullptr;
    irr::core::dimension2du m_WindowSize;
};

MyEventReceiver::MyEventReceiver( irr::IrrlichtDevice* device )
    : m_Device( device )
{}

bool
MyEventReceiver::OnEvent( const irr::SEvent& event )
{
    if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
    {
        irr::SEvent::SKeyInput const & keyEvent = event.KeyInput;

#ifdef USE_IRRLICHT		
        if ( keyEvent.Key == irr::KEY_ESCAPE && !keyEvent.PressedDown )
#else // USE_IRRLICHT_GLES
        if ( keyEvent.Key == irr::IRR_KEY_ESCAPE && !keyEvent.PressedDown )
#endif
        {
            if ( m_Device )
            {
                m_Device->closeDevice();
            }
            return true;
        }

#ifdef USE_IRRLICHT
        if ( keyEvent.Key == irr::KEY_SPACE && !keyEvent.PressedDown )
#else // USE_IRRLICHT_GLES
        if ( keyEvent.Key == irr::IRR_KEY_SPACE && !keyEvent.PressedDown )
#endif
		{
            if ( m_Device )
            {
                irr::scene::ICameraSceneNode* camera = m_Device->getSceneManager()->getActiveCamera();
                if ( camera )
                {
                    camera->setInputReceiverEnabled( !camera->isInputReceiverEnabled() );
                }
            }
            return true;
        }
    }

    return false;
}

void
MyEventReceiver::OnResize( irr::core::dimension2du const & winSize )
{
    if ( m_Device )
    {
        m_Device->getVideoDriver()->OnResize( winSize );
    }
}

void
MyEventReceiver::setDevice( irr::IrrlichtDevice* device )
{
    if ( device == nullptr )
    {
        m_Device = device;
    }
}

int main( int argc, char * argv[] )
{
   const char* appName = "test.irrlicht.fpscube (c) 2019 by Benjamin Hampe <benjaminhampe@gmx.de>";
	
   std::string fileName = "tux.png";
	
   std::cout << "// ==========================================================\n";
   std::cout << "// prgm: " << argv[0] << ":\n";
   std::cout << "// ==========================================================\n";

   srand( (unsigned int)time( nullptr ) );

   irr::SIrrlichtCreationParameters cfg;
#ifdef USE_IRRLICHT		
   cfg.DriverType = irr::video::EDT_OPENGL;
#else
   cfg.DriverType = irr::video::EDT_OGLES2;
#endif	
   cfg.WindowSize = irr::core::dimension2du( 1280, 800 );
   cfg.AntiAlias = irr::video::EAAM_OFF;
   cfg.Bits = 32;
   cfg.Doublebuffer = false;
   cfg.Vsync = false;
   cfg.EventReceiver = nullptr;
   cfg.Fullscreen = false;
   cfg.Stencilbuffer = true;
   irr::IrrlichtDevice* device = irr::createDeviceEx( cfg );
   assert( device );
   device->setResizable( true );
   device->setWindowCaption( irr::core::stringw( appName ).c_str() );

   MyEventReceiver myEventReceiver( device );
   device->setEventReceiver( &myEventReceiver );

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

   irr::SKeyMap keyMap[6];
	keyMap[0].Action = irr::EKA_MOVE_FORWARD;
	keyMap[1].Action = irr::EKA_MOVE_BACKWARD;
	keyMap[2].Action = irr::EKA_STRAFE_LEFT;
	keyMap[3].Action = irr::EKA_STRAFE_RIGHT;
	keyMap[4].Action = irr::EKA_JUMP_UP;
	keyMap[5].Action = irr::EKA_CROUCH;
#ifdef USE_IRRLICHT
	keyMap[0].KeyCode = irr::KEY_KEY_W;
	keyMap[1].KeyCode = irr::KEY_KEY_S;
	keyMap[2].KeyCode = irr::KEY_KEY_A;
	keyMap[3].KeyCode = irr::KEY_KEY_D;
	keyMap[4].KeyCode = irr::KEY_SPACE;
	keyMap[5].KeyCode = irr::KEY_KEY_C;
#else
	keyMap[0].KeyCode = irr::IRR_KEY_W;
	keyMap[1].KeyCode = irr::IRR_KEY_S;
	keyMap[2].KeyCode = irr::IRR_KEY_A;
	keyMap[3].KeyCode = irr::IRR_KEY_D;
	keyMap[4].KeyCode = irr::IRR_KEY_SPACE;
	keyMap[5].KeyCode = irr::IRR_KEY_C;	
#endif
   irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS( rootNode, 65.f,0.5f,-1,keyMap,6,false,1.0f, false, true );
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