#ifndef IRREXT_NULLDEVICE_HPP
#define IRREXT_NULLDEVICE_HPP

#include <irrExt/Types.hpp>

namespace irrExt {

class NullDevice
{
   irr::IrrlichtDevice* m_Device;
public:

   NullDevice()
      : m_Device( nullptr )
   {
      openDevice();
   }

   ~NullDevice()
   {
      closeDevice();
   }

   irr::IrrlichtDevice* getDevice() const { return m_Device; }

   bool is_open() const
   {
      return ( m_Device != nullptr );
   }

   bool openDevice()
   {
      if ( m_Device )
      {
         return true; // Already open
      }

      m_Device = irr::createDevice( irr::video::EDT_NULL );

      return is_open();
   }

   void closeDevice()
   {
      if ( !is_open() )
      {
         return; // Already closed
      }

      m_Device->closeDevice();
      m_Device->drop();
      m_Device = nullptr;
   }

   irr::io::IXMLReaderUTF8* loadXml( std::string const & fileName )
   {
      irr::io::IXMLReaderUTF8* xml = nullptr;

      if ( !is_open() )
      {
         return xml;
      }

      xml = m_Device->getFileSystem()->createXMLReaderUTF8( fileName.c_str() );

      return xml;
   }
};

} // end namespace irrExt

#endif // IRREXT_NULLDEVICE_HPP
