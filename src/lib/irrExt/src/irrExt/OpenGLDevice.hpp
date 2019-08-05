#ifndef IRREXT_OPENGLDEVICE_HPP
#define IRREXT_OPENGLDEVICE_HPP

#include <irrExt/NullDevice.hpp>

namespace irrExt {

class OpenGLDevice
{
public:
   static irr::IrrlichtDevice*
   createDevice( int32_t w = -1, int32_t h = -1, int32_t b = 32 );

   explicit OpenGLDevice( int32_t w = -1, int32_t h = -1, int32_t b = 32 );
   ~OpenGLDevice();

   irr::IrrlichtDevice* getDevice() const;
   bool is_open() const;
   bool openDevice();
   void closeDevice();
   irr::io::IXMLReaderUTF8* loadXml( std::string const & fileName );
private:
   irr::IrrlichtDevice* m_Device;
   int32_t m_ScreenWidth;
   int32_t m_ScreenHeight;
   int32_t m_ScreenDepth;
};

} // end namespace irrExt

#endif // IRREXT_OPENGLDEVICE_HPP
