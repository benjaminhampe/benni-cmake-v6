#ifndef IRREXT_FILESYSTEM_HPP
#define IRREXT_FILESYSTEM_HPP

#include <irrExt/StringUtils.hpp>

#include <sstream>

namespace irrExt {
	
	void
	makePosixPath( std::string & fileName );
	
	bool 
	loadTextFile( std::string & content, std::string const & fileName, bool debugPrint = false );
	
	bool 
	saveTextFile( std::string content, std::string const & saveName );
   
	std::vector< std::string > 
	loadStringVector( std::string const & fileName );
	
	std::string
	getCurrentDirectory();

	/// @brief Ask filesystem layer if given fileName leads to a regular file.
	bool
	existFile( std::string const & fileName );

	/// @brief Ask filesystem layer if given directoryName leads to a directory.
	bool
	existDirectory( std::string const & directoryName );

	/// @brief Ask filesystem layer if given fileName leads to a directory.
	bool
	existParentDirectory( std::string const & fileName );

	std::string
	getAbsoluteFilePath( std::string const & fileName );

	std::string
	getAbsoluteFileName( std::string const & fileName );

	std::vector< std::string > 
	getAbsoluteFileNames( std::vector< std::string > fileNames, std::string baseDir );
		
	std::string
	getFilePath( std::string const & fileName );

	std::string
	getFileName( std::string const & fileName );

	std::string
	getFileExtension( std::string const & fileName );

	uintmax_t
	getFileSize( std::string const & fileName );

	std::string
	choseDirectory();

#if 0
dbFileType
dbGetFileType( std::string const & fileName );

os::FileType
dbGetFileMagic( std::string const & fileName );

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

// ============================================================================
// File Magic
// ============================================================================

// scans FileMagic for all supported types
FileType
getFileMagic( FILE* in );

FileType
getFileMagic( std::string const & fileName );

// ============================================================================
// Image Magic
// ============================================================================

// scans FileMagic only for all supported image file types
FileType
getImageFileMagic( FILE* in );

FileType
getImageFileMagic( std::string const & fileName );

#ifdef USE_TAGLIB

FileType
getImageFileMagic( TagLib::ByteVector const & byteVector );

#endif

// ============================================================================
// Audio Magic
// ============================================================================

// scans FileMagic only for all supported audio file types
FileType
getAudioFileMagic( FILE* in );

FileType
getAudioFileMagic( std::string const & fileName );

#endif

} // end namespace irrExt

#endif // IRREXT_FILESYSTEM_HPP
