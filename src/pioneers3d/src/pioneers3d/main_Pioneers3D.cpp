#include <pioneers3d/StandardGame.hpp>

using namespace pioneers3d;

irr::IrrlichtDevice* createOpenGLDevice()
{
   srand( (unsigned int)time( nullptr ) );

   std::cout << "// ==========================================================\n";
   std::cout << __FUNCTION__ << "()\n";
   std::cout << "// ==========================================================\n";
   glm::ivec3 const desktopSize = getDesktopSize();
   std::cout << "DesktopSize.x = " << desktopSize.x << " px\n";
   std::cout << "DesktopSize.y = " << desktopSize.y << " px\n";
   std::cout << "DesktopSize.z = " << desktopSize.z << " bits\n";

   irr::SIrrlichtCreationParameters cfg;
   cfg.DriverType = irr::video::EDT_OPENGL;
   cfg.WindowSize.Width = 1280; //1440; //desktopSize.x;
   cfg.WindowSize.Height = 800; //900; //desktopSize.y;
   cfg.AntiAlias = irr::video::EAAM_QUALITY;
   cfg.Bits = 32;
   cfg.Doublebuffer = false;
   cfg.Vsync = false;
   cfg.EventReceiver = nullptr;
   cfg.Fullscreen = false;
   cfg.ZBufferBits = 24;
   cfg.Stencilbuffer = true;
   irr::IrrlichtDevice* device = irr::createDeviceEx( cfg );
   return device;
}

int main( int argc, char** argv )
{
   irr::IrrlichtDevice* device = createOpenGLDevice();
   assert( device );
   Game_t game;
   Game_createStandard( &game, device, argc, argv );
   Game_start( &game );
   return Game_exec( &game );
}
