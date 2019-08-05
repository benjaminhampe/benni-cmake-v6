#include "OpenGLDevice.hpp"
#include <irrlicht.h>

namespace irrExt {

irr::IrrlichtDevice*
OpenGLDevice::createDevice( int32_t w, int32_t h, int32_t b )
{
   irr::SIrrlichtCreationParameters cfg;
   #ifdef USE_IRRLICHT
   cfg.DriverType = irr::video::EDT_OPENGL;
   #else
   cfg.DriverType = irr::video::EDT_OGLES2;
   #endif
   if ( w < 0 || h < 0 )
   {
      irr::IrrlichtDevice* nullDevice = irr::createDevice( irr::video::EDT_NULL );
      if ( !nullDevice )
      {
         std::cout << "[Error] " << __FUNCTION__ << " Cannot create null device for desktop resolution, fallback to 640x480x16\n";
         w = 640;
         h = 480;
         b = 16;
      }
      else
      {
         w = nullDevice->getVideoModeList()->getDesktopResolution().Width;
         h = nullDevice->getVideoModeList()->getDesktopResolution().Height;
         b = nullDevice->getVideoModeList()->getDesktopDepth();
         nullDevice->drop();
      }
   }
   cfg.WindowSize = irr::core::dimension2du( w, h );
   cfg.AntiAlias = irr::video::EAAM_FULL_BASIC;
   cfg.Bits = b;
   cfg.Doublebuffer = false;
   cfg.Vsync = false;
   cfg.EventReceiver = nullptr;
   cfg.Fullscreen = false;
   cfg.Stencilbuffer = true;
   cfg.ZBufferBits = 24;
   cfg.HighPrecisionFPU = true;
   cfg.Stereobuffer = false;
   cfg.WithAlphaChannel = false;

   irr::IrrlichtDevice* device = irr::createDeviceEx( cfg );
   //irr::IrrlichtDevice* device = irr::createDevice( irr::video::EDT_OPENGL, irr::core::dimension2du( w, h ), 32, false, true, false, nullptr );
   if( device );
   {
      device->setResizable( true );
   }
   return device;
}

OpenGLDevice::OpenGLDevice( int32_t w, int32_t h, int32_t b )
   : m_Device( nullptr )
   , m_ScreenWidth( w )
   , m_ScreenHeight( h )
   , m_ScreenDepth( b )
{
   std::cout << "[Info] " << __FUNCTION__ << "\n";
   openDevice();
}

OpenGLDevice::~OpenGLDevice()
{
   closeDevice();
   std::cout << "[Info] " << __FUNCTION__ << "\n";
}

irr::IrrlichtDevice*
OpenGLDevice::getDevice() const
{
   return m_Device;
}

bool
OpenGLDevice::is_open() const
{
   return ( m_Device != nullptr );
}

bool
OpenGLDevice::openDevice()
{
   if ( m_Device )
   {
      return true; // Already open
   }

   m_Device = OpenGLDevice::createDevice( m_ScreenWidth, m_ScreenHeight, m_ScreenDepth );

   return is_open();
}

void
OpenGLDevice::closeDevice()
{
   if ( !is_open() )
   {
      return; // Already closed
   }

   if ( m_Device->run() )
   {
      m_Device->closeDevice();
   }

   m_Device->drop();
   m_Device = nullptr;
}

irr::io::IXMLReaderUTF8*
OpenGLDevice::loadXml( std::string const & fileName )
{
   irr::io::IXMLReaderUTF8* xml = nullptr;

   if ( !is_open() )
   {
      return xml;
   }

   xml = m_Device->getFileSystem()->createXMLReaderUTF8( fileName.c_str() );

   return xml;
}




} // end namespace irrExt
