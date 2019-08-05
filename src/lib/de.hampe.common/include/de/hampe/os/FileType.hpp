#ifndef DE_HAMPE_OS_FILETYPE_HPP
#define DE_HAMPE_OS_FILETYPE_HPP

#include <de/hampe/Types.hpp>
#include <de/hampe/StringUtil.hpp>
#include <de/hampe/logging/Logger.hpp>

namespace de { 
namespace hampe { 
namespace os {

class FileType
{
public:
   enum eFileType
   {
      Directory = 0,
      // Supported sub file types for images
      eFileTypeJPG,      // image/jpeg, image/pjpeg
      eFileTypePNG,          // image/png
      eFileTypeBMP,          // image/bmp, image/x-windows-bmp
      eFileTypeGIF,          // image/gif
      eFileTypeTIF,          // image/tiff, image/x-tiff
      eFileTypeTGA,          // image/x-tga, image/x-targa
      eFileTypePCX,          // image/x-pcx

      eFileTypeMP3,
      eFileTypeWAV,
      eFileTypeAAC,
      eFileTypeMP2,
      eFileTypeMP1,
      eFileTypeMP4,
      eFileTypeM4A,
      eFileTypeFLAC,
      eFileTypeOGG,
      eFileTypeWMA,
      eFileType3GP,
      eFileTypeFLV,

      eFileTypeCount
   };

   static FileType
   fromFileExtension( std::string const & ext );

   static FileType
   fromMimeType( std::string mimeType );

   static const char*
   getFileExtension( eFileType const fileType );

   static const char*
   getMimeType( eFileType const fileType );

   static bool
   isImage( eFileType const fileType );

   static bool
   isAudio( eFileType const fileType );

   static bool
   isVideo( eFileType const fileType );

   FileType()
      : m_FileType( eFileTypeCount )
   {}

   FileType( uint32_t const value )
      : m_FileType( eFileTypeCount )
   {
      if ( value < uint32_t( eFileTypeCount ) )
      {
         m_FileType = static_cast< eFileType >( value );
      }
   }

   FileType( FileType const & other )
      : m_FileType( other.m_FileType )
   {}

   operator eFileType() const
   {
      return m_FileType;
   }

   bool
   isUnknown() const
   {
      return ( m_FileType >= eFileTypeCount );
   }

   bool
   isImage() const
   {
      return isImage( m_FileType );
   }

   bool
   isAudio() const
   {
      return isAudio( m_FileType );
   }

   bool
   isVideo() const
   {
      return isVideo( m_FileType );
   }

   const char*
   getFileExtension() const
   {
      return getFileExtension( m_FileType );
   }

protected:
   eFileType m_FileType;

};

} // end namespace os
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_OS_FILETYPE_HPP