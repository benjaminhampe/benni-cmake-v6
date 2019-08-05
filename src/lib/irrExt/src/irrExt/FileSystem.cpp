#include <irrExt/FileSystem.hpp>
#include <irrExt/StringUtils.hpp>

#include <iostream>

/// I use GCC 7.3.0 and link extra with -lstdc++fs
#include <experimental/filesystem>

#ifdef WIN32
   #include <windows.h>
#endif
namespace irrExt {

namespace {
   namespace fs = std::experimental::filesystem;
}

/// or STL C++17
// #include <filesystem>
// namespace fs = std::filesystem;

/// or Boost
// #include <boost/filesystem.hpp>
// namespace fs = boost::filesystem;

std::string
stringifyFilePermsissions( fs::perms const perm )
{
   std::stringstream s;
   s << ((perm & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
     << ((perm & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
     << ((perm & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
     << ((perm & fs::perms::group_read) != fs::perms::none ? "r" : "-")
     << ((perm & fs::perms::group_write) != fs::perms::none ? "w" : "-")
     << ((perm & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
     << ((perm & fs::perms::others_read) != fs::perms::none ? "r" : "-")
     << ((perm & fs::perms::others_write) != fs::perms::none ? "w" : "-")
     << ((perm & fs::perms::others_exec) != fs::perms::none ? "x" : "-");
   return s.str();
}

std::string
getCurrentDirectory()
{
   return fs::absolute( "." ).string();
}

bool
existFile( std::string const & fileName )
{
   if ( fileName.empty() ) return false;
   fs::file_status s = fs::status( fileName );
   bool ok = fs::exists( s ) && fs::is_regular_file( s );
   return ok;
}

bool
existDirectory( std::string const & fileName )
{
   if ( fileName.empty() ) return false;
   fs::file_status s = fs::status( fileName );
   bool ok = fs::exists( s ) && fs::is_directory( s );
   return ok;
}

bool
existParentDirectory( std::string const & fileName )
{
   if ( fileName.empty() ) return false;
   fs::file_status s = fs::status( fileName );
   bool ok = fs::exists( s ) && fs::is_directory( s );
   return ok;
}

std::string
getAbsoluteFileName( std::string const & fileName )
{
   if ( fileName.empty() ) return "";
   return fs::canonical( fileName ).string();
}

void
makePosixPath( std::string & fileName )
{
   //replace( fileName, '\\', '/' );
   //replace( fileName, '\\', '/' ); // CR, LF
}

std::vector< std::string >
getAbsoluteFileNames( std::vector< std::string > fileNames, std::string baseDir )
{
   // make filenames absolute using dir from given filename
   //std::string baseDir = irrExt::getAbsoluteFilePath( fileName );

   makePosixPath( baseDir );

   if ( baseDir.empty() )
   {
      baseDir = getCurrentDirectory();
   }

   if ( !endsWith( baseDir, "/" ) )
   {
      baseDir += "/";
   }

   for ( size_t i = 0; i < fileNames.size(); ++i )
   {
      std::string & fileName = fileNames[ i ];
      fileName = irrExt::getAbsoluteFileName( baseDir + fileName );
   }
}

std::string
getAbsoluteFilePath( std::string const & fileName )
{
   if ( fileName.empty() ) return "";
   return fs::canonical( fileName ).parent_path().string();
}

std::string
getFileName( std::string const & fileName )
{
   if ( fileName.empty() ) return "";
   return fs::path( fileName ).filename().string();
}

std::string
getFileExtension( std::string const & fileName )
{
   if ( fileName.empty() ) return "";
   std::string ext = fs::path( fileName ).extension().string();
   lowerCase( ext );
   return ext;
}

uintmax_t
getFileSize( std::string const & fileName )
{
   if ( fileName.empty() ) return 0;
   return fs::file_size( fileName );
}


std::string
choseDirectory()
{

}





   /*
   inline std::string
   execShellCommand( std::string command )
   {
      FILE* p = ::popen( command.c_str(), "rb" );
      if (!p)
      {
         return de::hampe::common::sprintf( "Pipe failed for cmd(", command, ")" );
      }

      const uint32_t numChars = 2048;
      char buffer[ numChars ];

      std::stringstream s;

      while( !::feof( p ) )
      {
         memset( buffer, 0, numChars );
         if( ::fgets( buffer, numChars, p ) != NULL )
         {
            s << buffer;
         }
      }

      ::pclose( p );
      return s.str();
   }
   */

   bool
   loadTextFile( std::string & content, std::string const & fileName, bool debugPrint )
   {
      std::ifstream fin( fileName.c_str() );
      if ( !fin.is_open() )
      {
         return false;
      }

      std::string line;
      std::stringstream s;
      while ( std::getline( fin, line ) )
      {
         s << line << "\n";
      }
      content = std::move( s.str() );

      if ( debugPrint )
      {
         std::cout << "==========================================\n";
         std::cout << __FUNCTION__ << "(" << fileName << ")\n";
         std::cout << "==========================================\n";
         std::cout << content;
      }
      return true;
   }

   bool
   saveTextFile( std::string content, std::string const & saveName )
   {
      FILE * fout = ::fopen( saveName.c_str(), "wb" );
      if ( !fout )
      {
         return false;
      }

      size_t bytesWritten = ::fwrite( content.c_str(), content.size(), content.size(), fout );
      ::fclose ( fout );
      return true;
   }

   std::vector< std::string >
   loadStringVector( std::string const & fileName )
   {
      std::vector< std::string > lines;

      std::ifstream fin( fileName.c_str() );
      if ( !fin.is_open() )
      {
         std::cout << "[Error] " << __FUNCTION__ << " :: Cannot read file(" << fileName << ")\n";
         return lines;
      }

      std::cout << "[Info] " << __FUNCTION__ << " :: Read text file(" << fileName << ")\n";
      {
         std::string line;
         while ( std::getline( fin, line ) )
         {
            lines.push_back( line );
         }
      }
      fin.close();

      std::cout << toString( lines, "[Info] rawLines" );

      // printVector
      //printVector( lines, std::cout, "[Info] fileNames" );

      return lines;
   }

} // end namespace irrExt














































#if 0

dbFileType
dbGetFileType( std::string const & fileName )
{
   if ( fileName.empty() ) return "";
   return FileType::fromFileExtension( fileExtension( fileName ) );
}

dbFileType
dbGetFileMagic( std::string const & fileName )
{
   return os::getFileMagic( fileName );
}


#ifdef USE_TAGLIB
#include <tbytevector.h>
#endif


namespace de {
namespace hampe {
namespace os {


// ============================================================================
/// ImageMagic
// ============================================================================

const unsigned int 		IMAGE_MAGIC_JPG 		= 0xFFD8FF00;
const unsigned int 		IMAGE_MAGIC_JPG_JFIF 	= 0x4A464946;
const unsigned int 		IMAGE_MAGIC_JPG_Exif 	= 0x45786966;
const unsigned int 		IMAGE_MAGIC_JPG_EXIF 	= 0x45584946;
const unsigned int 		IMAGE_MAGIC_PNG_1 		= 0x89504E47;
const unsigned int 		IMAGE_MAGIC_PNG_2 		= 0x0D0A1A0A;
const unsigned int 		IMAGE_MAGIC_TIFF 		= 0x49204900;
const unsigned int 		IMAGE_MAGIC_TIFF_LE 	= 0x49492A00;
const unsigned int 		IMAGE_MAGIC_TIFF_BE 	= 0x4D4D002A;
const unsigned int 		IMAGE_MAGIC_GIF_1 		= 0x47494638;
const unsigned short 	IMAGE_MAGIC_GIF_2 		= 0x3761;
const unsigned short 	IMAGE_MAGIC_GIF_3 		= 0x3961;
const unsigned short 	IMAGE_MAGIC_BMP 		= 0x424D;

bool
isFileMagic_JPG( FILE* in )
{
   rewind( in );
   unsigned int magic;
   size_t n = ::fread( (void*)&magic, 4, 1, in );
   (void)n;
   if ( (magic & 0xFFFFFF00) == IMAGE_MAGIC_JPG ) return true; else return false;
}
bool
isFileMagic_PNG( FILE* in )
{
    rewind( in );
    unsigned int magic_a;
    size_t n = ::fread( (void*)&magic_a, 4, 1, in );
    unsigned int magic_b;
    n = ::fread( (void*)&magic_b, 4, 1, in );
    (void)n;
    if (( magic_a == IMAGE_MAGIC_PNG_1 ) && ( magic_b == IMAGE_MAGIC_PNG_2 )) return true; else return false;
}
bool
isFileMagic_BMP( FILE* in )
{
    rewind( in );
    unsigned short magic;
    size_t n = ::fread( (void*)&magic, 2, 1, in);
    (void)n;
    if ( magic == IMAGE_MAGIC_BMP ) return true; else return false;
}
bool
isFileMagic_GIF( FILE* in )
{
    rewind( in );
    unsigned int magic_a;
    size_t n = ::fread( (void*)&magic_a, 4, 1, in);
    (void)n;
    unsigned short magic_b;
    n = ::fread( (void*)&magic_b, 2, 1, in);
    if (( magic_a == IMAGE_MAGIC_GIF_1 ) && (( magic_b == IMAGE_MAGIC_GIF_2 ) || ( magic_b == IMAGE_MAGIC_GIF_3 ))) return true; else return false;
}
bool
isFileMagic_TIF( FILE* in )
{
    rewind( in );
    unsigned int magic;
    size_t n = ::fread( (void*)&magic, 4, 1, in);
    (void)n;
    if (( magic == IMAGE_MAGIC_TIFF_LE ) || ( magic == IMAGE_MAGIC_TIFF_BE ) || ((magic & 0xFFFFFF00) == IMAGE_MAGIC_TIFF )) return true; else return false;
}
// ============================================================================
// Mime: image/x-tga
// TGA	- Truevision Targa Graphic file
// Trailer:
// 54 52 55 45 56 49 53 49   TRUEVISI
// 4F 4E 2D 58 46 49 4C 45   ON-XFILE
// 2E 00                     ..
bool
isFileMagic_TGA( FILE* in )
{
    return false;
}
bool
isFileMagic_PCX( FILE* in )
{
    return false;
}

// ============================================================================
/// ImageMagic
// ============================================================================

FileType
getImageFileMagic( FILE* in )
{
    if      (isFileMagic_JPG(in)) return FileType::eFileTypeJPG;
    else if (isFileMagic_PNG(in)) return FileType::eFileTypePNG;
    else if (isFileMagic_BMP(in)) return FileType::eFileTypeBMP;
    else if (isFileMagic_GIF(in)) return FileType::eFileTypeGIF;
    else if (isFileMagic_TIF(in)) return FileType::eFileTypeTIF;
    else if (isFileMagic_TGA(in)) return FileType::eFileTypeTGA;
    else if (isFileMagic_PCX(in)) return FileType::eFileTypePCX;
    else                          return FileType::eFileTypeCount;
}

FileType
getImageFileMagic( std::string const & fileName )
{
   FileType fileType = FileType::eFileTypeCount;
   FILE* f = fopen( fileName.c_str(), "rb" );
   if ( f )
   {
      fileType = getImageFileMagic( f );
      fclose( f );
   }
   return fileType;
}

// ============================================================================
//	TagLib
// ============================================================================

#ifdef USE_TAGLIB

bool isFileMagic_JPG( const TagLib::ByteVector& in )
{
    if (in.size() < 4) return false;
    unsigned int magic = in.mid(0,4).toUInt();
    if ( (magic & 0xFFFFFF00) == IMAGE_MAGIC_JPG ) return true; else return false;
}
bool isFileMagic_PNG( const TagLib::ByteVector& in )
{
    if (in.size() < 8) return false;
    unsigned int magic_a = in.mid(0,4).toUInt();
    unsigned int magic_b = in.mid(5,4).toUInt();
    if (( magic_a == IMAGE_MAGIC_PNG_1 ) && ( magic_b == IMAGE_MAGIC_PNG_2 )) return true; else return false;
}
bool isFileMagic_BMP( const TagLib::ByteVector& in )
{
    if (in.size() < 2) return false;
    unsigned short magic = in.mid(0,2).toUShort();
    if ( magic == IMAGE_MAGIC_BMP ) return true; else return false;
}
bool isFileMagic_GIF( const TagLib::ByteVector& in )
{
    if (in.size() < 6) return false;
    unsigned int magic_a = in.mid(0,4).toUInt();
    unsigned short magic_ = in.mid(5,2).toUShort();
    if (( magic_a == IMAGE_MAGIC_GIF_1 ) && (( magic_ == IMAGE_MAGIC_GIF_2 ) || ( magic_ == IMAGE_MAGIC_GIF_3 ))) return true; else return false;
}
bool isFileMagic_TIF( const TagLib::ByteVector& in )
{
    if (in.size() < 6) return false;
    unsigned int magic = in.mid(0,4).toUInt();
    if (( magic == IMAGE_MAGIC_TIFF_LE ) || ( magic == IMAGE_MAGIC_TIFF_BE ) || ((magic & 0xFFFFFF00) == IMAGE_MAGIC_TIFF )) return true; else return false;
}
bool isFileMagic_TGA( const TagLib::ByteVector& in )
{
    if (in.size() < 4) return false;
    return false;
}
bool isFileMagic_PCX( const TagLib::ByteVector& in )
{
    if (in.size() < 4) return false;
    return false;
}

FileType
getImageFileMagic( TagLib::ByteVector const & byteVector )
{
    if      (isFileMagic_JPG( byteVector )) return FileType::eFileTypeJPG;
    else if (isFileMagic_PNG( byteVector )) return FileType::eFileTypePNG;
    else if (isFileMagic_BMP( byteVector )) return FileType::eFileTypeBMP;
    else if (isFileMagic_GIF( byteVector )) return FileType::eFileTypeGIF;
    else if (isFileMagic_TIF( byteVector )) return FileType::eFileTypeTIF;
    else if (isFileMagic_TGA( byteVector )) return FileType::eFileTypeTGA;
    else if (isFileMagic_PCX( byteVector )) return FileType::eFileTypePCX;
    else return eFileTypeCount;
}

#endif // USE_TAGLIB


// ============================================================================
// Audio File Magic
// ============================================================================

// CDA
// 52 49 46 46 xx xx xx xx == RIFF....
// 43 44 44 41 66 6D 74 20 == CDDAfmt

// WAV
// 52 49 46 46 xx xx xx xx == RIFF....
// 57 41 56 45 66 6D 74 20 == WAVEfmt

const unsigned int AUDIO_MAGIC_RIFF = 0x52494646; // 'R' 'I' 'F' 'F' - Bytes 0..3
const unsigned int AUDIO_MAGIC_CDDA = 0x43444441; // 'C' 'D' 'D' 'A' - Bytes 8..11
const unsigned int AUDIO_MAGIC_WAVE = 0x57415645; // 'W' 'A' 'V' 'E' - Bytes 8..11
const unsigned int AUDIO_MAGIC_fmts = 0x666D7420; // 'f' 'm' 't' ' ' - Bytes 12..15
const unsigned int AUDIO_MAGIC_data = 0x64617461; // 'd' 'a' 't' 'a' - Bytes 12..15

// FLAC
// 66 4C 61 43 00 00 00 22 == fLaC..."

const unsigned int AUDIO_MAGIC_FLAC_1 = 0x664C6143; // 'f' 'L' 'a' 'C'
const unsigned int AUDIO_MAGIC_FLAC_2 = 0x00000022; // '\0' '\0' '\0' '"'

// OGA, OGG, OGV, OGX - Ogg Vorbis Codec compressed Multimedia file
// 4F 67 67 53 00 02 00 00  OggS....
// 00 00 00 00 00 00	 	......

const unsigned int AUDIO_MAGIC_OGG_1 = 0x4F676753; // 'O' 'g' 'g' 'S'
const unsigned int AUDIO_MAGIC_OGG_2 = 0x00020000; // '\0' '\STX' '\0' '\0'
const unsigned int AUDIO_MAGIC_OGG_3 = 0x00000000; // '\0' '\0' '\0' '\0'
const unsigned short AUDIO_MAGIC_OGG_4 = 0x0000; // '\0' '\0'

// MP4 - MPEG-4 video files
// xx xx xx xx 66 74 79 70 33 67 70 35 = [4-Byte-Offset] ftyp3gp5

// MP4 - MPEG-4 video files
// xx xx xx xx 66 74 79 70 4D 53 4E 56 = [4-Byte-Offset] ftypMSNV

// M4V - MPEG-4 video|QuickTime file
// xx xx xx xx 66 74 79 70 6D 70 34 32 = [4-Byte-Offset] ftypmp42

// M4A - Apple Lossless Audio Codec file
// xx xx xx xx 66 74 79 70 4D 34 41 20 = [4-Byte-Offset] ftypM4A

// MPEG, MPG, MP3 - MPEG audio file frame  synch pattern
// FF Ex	 	ÿ.
// FF Fx	 	ÿ.
// 0xFF FB = 0b1111 1111 1111 1011
// 0xFF FA = 0b1111 1111 1111 1010

// AAAAAAAA AAABBCCD
// A	11	(31-21)	Frame sync (all bits must be set)
// B	2	(20,19)	MPEG Audio version ID
// 00 - MPEG Version 2.5 (later extension of MPEG 2)
// 01 - reserved
// 10 - MPEG Version 2 (ISO/IEC 13818-3)
// 11 - MPEG Version 1 (ISO/IEC 11172-3)
// Note: MPEG Version 2.5 was added lately to the MPEG 2 standard. It is an extension used for very low bitrate files, allowing the use of lower sampling frequencies. If your decoder does not support this extension, it is recommended for you to use 12 bits for synchronization instead of 11 bits.
//
// C	2	(18,17)	Layer description
// 00 - reserved
// 01 - Layer III
// 10 - Layer II
// 11 - Layer I

// AAC	 	MPEG-4 Advanced Audio Coding (AAC) Low Complexity (LC) audio file
// FF F1    0b1111 1111 1111 0001

// AAC	 	MPEG-2 Advanced Audio Coding (AAC) Low Complexity (LC) audio file
// FF F9	0b1111 1111 1111 1001

// REG	 	Windows Registry file ( scheiss Microsoft )
// FF FE    0b1111 1111 1111 1110

bool isFileMagic_AAC( FILE* in )
{
    return false;
}

bool isFileMagic_WAV( FILE* in )
{
    return false;
}

bool isFileMagic_MP3( FILE* in )
{
    return false;
}

bool isFileMagic_MP4( FILE* in )
{
    return false;
}

bool isFileMagic_FLAC( FILE* in )
{
    return false;
}

bool isFileMagic_OGG( FILE* in )
{
    return false;
}

bool isFileMagic_WMA( FILE* in )
{
    return false;
}

bool isFileMagic_M4A( FILE* in )
{
    return false;
}

bool isFileMagic_FLV( FILE* in )
{
    return false;
}

FileType
getAudioFileMagic( FILE* in )
{
         if (isFileMagic_WAV(in)) return FileType::eFileTypeWAV;
    else if (isFileMagic_AAC(in)) return FileType::eFileTypeAAC;
    else if (isFileMagic_MP3(in)) return FileType::eFileTypeMP3;
    else if (isFileMagic_MP4(in)) return FileType::eFileTypeMP4;
    else if (isFileMagic_M4A(in)) return FileType::eFileTypeM4A;
    else if (isFileMagic_FLAC(in))return FileType::eFileTypeFLAC;
    else if (isFileMagic_OGG(in)) return FileType::eFileTypeOGG;
    else if (isFileMagic_WMA(in)) return FileType::eFileTypeWMA;
    else if (isFileMagic_FLV(in)) return FileType::eFileTypeFLV;
    else                          return FileType::eFileTypeCount;
}

FileType
getAudioFileMagic( std::string const & fileName )
{
   FileType fileType = FileType::eFileTypeCount;
   FILE* f = fopen( fileName.c_str(), "rb" );
   if ( f )
   {
      fileType = getAudioFileMagic( f );
      fclose( f );
   }
   return fileType;
}

// ============================================================================
// File Magic
// ============================================================================

FileType
getFileMagic( FILE* in )
{
   FileType fileType = getImageFileMagic( in );
   if ( fileType == FileType::eFileTypeCount )
   {
      fileType = getAudioFileMagic( in );
   }
   return fileType;
}

FileType
getFileMagic( std::string const & fileName )
{
   FileType fileType = FileType::eFileTypeCount;
   FILE* f = fopen( fileName.c_str(), "rb" );
   if ( f )
   {
      fileType = getImageFileMagic( f );
      if ( fileType == FileType::eFileTypeCount )
      {
         fileType = getAudioFileMagic( f );
      }
      fclose( f );
   }
   return fileType;
}


// static
FileType
FileType::fromFileExtension( std::string const & ext )
{
   // std::string const ext = fileInfo.suffix().toLower().toStdString();
   if ( ext == "jpeg" ) return eFileTypeWAV;
   else if ( ext == "jpg" ) return eFileTypeJPG;
   else if ( ext == "png" ) return eFileTypePNG;
   else if ( ext == "bmp" ) return eFileTypeBMP;
   else if ( ext == "gif" ) return eFileTypeGIF;
   else if ( ext == "tif" ) return eFileTypeTIF;
   else if ( ext == "tga" ) return eFileTypeTGA;
   else if ( ext == "pcx" ) return eFileTypePCX;
   else if ( ext == "mp3" ) return eFileTypeMP3;
   else if ( ext == "wav" ) return eFileTypeWAV;
   else if ( ext == "aac" ) return eFileTypeAAC;
   else if ( ext == "mp2" ) return eFileTypeMP2;
   else if ( ext == "mp1" ) return eFileTypeMP1;
   else if ( ext == "mp4" ) return eFileTypeMP4;
   else if ( ext == "m4a" ) return eFileTypeM4A;
   else if ( ext == "flac" ) return eFileTypeFLAC;
   else if ( ext == "ogg" ) return eFileTypeOGG;
   else if ( ext == "wma" ) return eFileTypeWMA;
   else if ( ext == "3gp" ) return eFileType3GP;
   else if ( ext == "flv" ) return eFileTypeFLV;
   else return eFileTypeCount;
}

// static
const char*
FileType::getFileExtension( eFileType const fileType )
{
   switch ( fileType )
   {
      // image types ( e.g. to decode the real image-type of CoverArts, if given mime-type is wrong and cannot be trusted )
      case eFileTypeJPG: return "jpg";
      case eFileTypePNG: return "png";
      case eFileTypeBMP: return "bmp";
      case eFileTypeGIF: return "gif";
      case eFileTypeTIF: return "tif";
      case eFileTypeTGA: return "tga";
      case eFileTypePCX: return "pcx";
      // audio types
      case eFileTypeMP3: return "mp3";
      case eFileTypeWAV: return "wav";
      case eFileTypeAAC: return "aac";
      case eFileTypeMP4: return "mp4";
      case eFileTypeM4A: return "m4a";
      case eFileTypeFLAC:return "flac";
      case eFileTypeOGG: return "ogg";
      case eFileTypeWMA: return "wma";
      case eFileTypeFLV: return "flv";
      default: return "unknown";
   }
}

// static
FileType
FileType::fromMimeType( std::string mimeType )
{
    StringUtil::makeLower( mimeType );
    static std::string::size_type const NO = std::string::npos;
    if ((mimeType.find("image/jpeg")!=NO) || (mimeType.find("image/pjpeg")!=NO))  		return eFileTypeJPG;
    if (mimeType.find("image/png")!=NO)                  								return eFileTypePNG;
    if ((mimeType.find("image/bmp")!=NO) || (mimeType.find("image/x-windows-bmp")!=NO))	return eFileTypeBMP;
    if (mimeType.find("image/gif")!=NO)                  								return eFileTypeGIF;
    if ((mimeType.find("image/tiff")!=NO) || (mimeType.find("image/x-tiff")!=NO)) return eFileTypeTIF;
    if ((mimeType.find("image/x-tga")!=NO) || (mimeType.find("image/x-targa")!=NO)) return eFileTypeTGA;
    if (mimeType.find("image/x-pcx")!=NO)                    return eFileTypePCX;
   // if ((tmp.find("image/jpeg")!=NO) || (tmp.find("image/pjpeg")!=NO)) return ftJPG;
    // if (tmp.find("image/png")!=NO) return ftPNG;
    // if ((tmp.find("image/bmp")!=NO) || (tmp.find("image/x-windows-bmp")!=NO)) return ftBMP;
    // if (tmp.find("image/gif")!=NO) return ftGIF;
    // if ((tmp.find("image/tiff")!=NO) || (tmp.find("image/x-tiff")!=NO)) return ftTIF;
    // if ((tmp.find("image/x-tga")!=NO) || (tmp.find("image/x-targa")!=NO)) return ftTGA;
    // if (tmp.find("image/x-pcx")!=NO) return ftPCX;
    else return eFileTypeCount;
}

// static
const char*
FileType::getMimeType( eFileType const fileType )
{
    switch ( fileType )
    {
        case eFileTypeJPG: return "image/jpeg";
        case eFileTypePNG: return "image/png";
        case eFileTypeBMP: return "image/bmp";
        case eFileTypeGIF: return "image/gif";
        case eFileTypeTIF: return "image/tif";
        case eFileTypeTGA: return "image/x-tga";
        case eFileTypePCX: return "image/x-pcx";
        case eFileTypeWAV: return "audio/wav";
        case eFileTypeAAC: return "audio/aac";
        case eFileTypeMP3: return "audio/mp3";
        case eFileTypeMP4: return "audio/mp4";
        case eFileTypeM4A: return "audio/m4a";
        case eFileTypeFLAC:return "audio/flac";
        case eFileTypeOGG: return "audio/ogg";
        case eFileTypeWMA: return "audio/wma";
        case eFileTypeFLV: return "audio/flv";
        default: return "unknown";
    }
}

// static
bool
FileType::isImage( eFileType const fileType )
{
    switch ( fileType )
    {
        case eFileTypeJPG: return true;
        case eFileTypePNG: return true;
        case eFileTypeBMP: return true;
        case eFileTypeGIF: return true;
        case eFileTypeTIF: return true;
        case eFileTypeTGA: return true;
        case eFileTypePCX: return true;
        default: return false;
    }
}

// static
bool
FileType::isAudio( eFileType const fileType )
{
    switch ( fileType )
    {
        case eFileTypeMP3: return true;
        case eFileTypeWAV: return true;
        case eFileTypeAAC: return true;
        case eFileTypeMP4: return true;
        case eFileTypeM4A: return true;
        case eFileTypeFLAC:return true;
        case eFileTypeOGG: return true;
        case eFileTypeWMA: return true;
        case eFileTypeFLV: return true;
        default: return false;
    }
}

// static
bool
FileType::isVideo( eFileType const fileType )
{
    switch ( fileType )
    {
        case eFileTypeMP4: return true;
        case eFileTypeFLV: return true;
        default: return false;
    }
}


} // end namespace os
} // end namespace hampe
} // end namespace de

#endif

//.3dm  x-world/x-3dmf
//.3dmf x-world/x-3dmf
//.a    application/octet-stream
//.aab  application/x-authorware-bin
//.aam  application/x-authorware-map
//.aas  application/x-authorware-seg
//.abc  text/vnd.abc
//.acgi text/html
//.afl  video/animaflex
//.ai   application/postscript
//.aif  audio/aiff
//.aif  audio/x-aiff
//.aifc audio/aiff
//.aifc audio/x-aiff
//.aiff audio/aiff
//.aiff audio/x-aiff
//.aim  application/x-aim
//.aip  text/x-audiosoft-intra
//.ani  application/x-navi-animation
//.aos  application/x-nokia-9000-communicator-add-on-software
//.aps  application/mime
//.arc  application/octet-stream
//.arj  application/arj
//.arj  application/octet-stream
//.art  image/x-jg
//.asf  video/x-ms-asf
//.asm  text/x-asm
//.asp  text/asp
//.asx  application/x-mplayer2
//.asx  video/x-ms-asf
//.asx  video/x-ms-asf-plugin
//.au   audio/basic
//.au   audio/x-au
//.avi  application/x-troff-msvideo
//.avi  video/avi
//.avi  video/msvideo
//.avi  video/x-msvideo
//.avs  video/avs-video
//.bcpio    application/x-bcpio
//.bin  application/mac-binary
//.bin  application/macbinary
//.bin  application/octet-stream
//.bin  application/x-binary
//.bin  application/x-macbinary
//.bm   image/bmp
//.bmp  image/bmp
//.bmp  image/x-windows-bmp
//.boo  application/book
//.book application/book
//.boz  application/x-bzip2
//.bsh  application/x-bsh
//.bz   application/x-bzip
//.bz2  application/x-bzip2
//.c    text/plain
//.c    text/x-c
//.c++  text/plain
//.cat  application/vnd.ms-pki.seccat
//.cc   text/plain
//.cc   text/x-c
//.ccad application/clariscad
//.cco  application/x-cocoa
//.cdf  application/cdf
//.cdf  application/x-cdf
//.cdf  application/x-netcdf
//.cer  application/pkix-cert
//.cer  application/x-x509-ca-cert
//.cha  application/x-chat
//.chat application/x-chat
//.class    application/java
//.class    application/java-byte-code
//.class    application/x-java-class
//.com  application/octet-stream
//.com  text/plain
//.conf text/plain
//.cpio application/x-cpio
//.cpp  text/x-c
//.cpt  application/mac-compactpro
//.cpt  application/x-compactpro
//.cpt  application/x-cpt
//.crl  application/pkcs-crl
//.crl  application/pkix-crl
//.crt  application/pkix-cert
//.crt  application/x-x509-ca-cert
//.crt  application/x-x509-user-cert
//.csh  application/x-csh
//.csh  text/x-script.csh
//.css  application/x-pointplus
//.css  text/css
//.cxx  text/plain
//.dcr  application/x-director
//.deepv    application/x-deepv
//.def  text/plain
//.der  application/x-x509-ca-cert
//.dif  video/x-dv
//.dir  application/x-director
//.dl   video/dl
//.dl   video/x-dl
//.doc  application/msword
//.dot  application/msword
//.dp   application/commonground
//.drw  application/drafting
//.dump application/octet-stream
//.dv   video/x-dv
//.dvi  application/x-dvi
//.dwf  drawing/x-dwf (old)
//.dwf  model/vnd.dwf
//.dwg  application/acad
//.dwg  image/vnd.dwg
//.dwg  image/x-dwg
//.dxf  application/dxf
//.dxf  image/vnd.dwg
//.dxf  image/x-dwg
//.dxr  application/x-director
//.el   text/x-script.elisp
//.elc  application/x-bytecode.elisp (compiled elisp)
//.elc  application/x-elc
//.env  application/x-envoy
//.eps  application/postscript
//.es   application/x-esrehber
//.etx  text/x-setext
//.evy  application/envoy
//.evy  application/x-envoy
//.exe  application/octet-stream
//.f    text/plain
//.f    text/x-fortran
//.f77  text/x-fortran
//.f90  text/plain
//.f90  text/x-fortran
//.fdf  application/vnd.fdf
//.fif  application/fractals
//.fif  image/fif
//.fli  video/fli
//.fli  video/x-fli
//.flo  image/florian
//.flx  text/vnd.fmi.flexstor
//.fmf  video/x-atomic3d-feature
//.for  text/plain
//.for  text/x-fortran
//.fpx  image/vnd.fpx
//.fpx  image/vnd.net-fpx
//.frl  application/freeloader
//.funk audio/make
//.g    text/plain
//.g3   image/g3fax
//.gif  image/gif
//.gl   video/gl
//.gl   video/x-gl
//.gsd  audio/x-gsm
//.gsm  audio/x-gsm
//.gsp  application/x-gsp
//.gss  application/x-gss
//.gtar application/x-gtar
//.gz   application/x-compressed
//.gz   application/x-gzip
//.gzip application/x-gzip
//.gzip multipart/x-gzip
//.h    text/plain
//.h    text/x-h
//.hdf  application/x-hdf
//.help application/x-helpfile
//.hgl  application/vnd.hp-hpgl
//.hh   text/plain
//.hh   text/x-h
//.hlb  text/x-script
//.hlp  application/hlp
//.hlp  application/x-helpfile
//.hlp  application/x-winhelp
//.hpg  application/vnd.hp-hpgl
//.hpgl application/vnd.hp-hpgl
//.hqx  application/binhex
//.hqx  application/binhex4
//.hqx  application/mac-binhex
//.hqx  application/mac-binhex40
//.hqx  application/x-binhex40
//.hqx  application/x-mac-binhex40
//.hta  application/hta
//.htc  text/x-component
//.htm  text/html
//.html text/html
//.htmls    text/html
//.htt  text/webviewhtml
//.htx  text/html
//.ice  x-conference/x-cooltalk
//.ico  image/x-icon
//.idc  text/plain
//.ief  image/ief
//.iefs image/ief
//.iges application/iges
//.iges model/iges
//.igs  application/iges
//.igs  model/iges
//.ima  application/x-ima
//.imap application/x-httpd-imap
//.inf  application/inf
//.ins  application/x-internett-signup
//.ip   application/x-ip2
//.isu  video/x-isvideo
//.it   audio/it
//.iv   application/x-inventor
//.ivr  i-world/i-vrml
//.ivy  application/x-livescreen
//.jam  audio/x-jam
//.jav  text/plain
//.jav  text/x-java-source
//.java text/plain
//.java text/x-java-source
//.jcm  application/x-java-commerce
//.jfif image/jpeg
//.jfif image/pjpeg
//.jfif-tbnl    image/jpeg
//.jpe  image/jpeg
//.jpe  image/pjpeg
//.jpeg image/jpeg
//.jpeg image/pjpeg
//.jpg  image/jpeg
//.jpg  image/pjpeg
//.jps  image/x-jps
//.js   application/x-javascript
//.js   application/javascript
//.js   application/ecmascript
//.js   text/javascript
//.js   text/ecmascript
//.jut  image/jutvision
//.kar  audio/midi
//.kar  music/x-karaoke
//.ksh  application/x-ksh
//.ksh  text/x-script.ksh
//.la   audio/nspaudio
//.la   audio/x-nspaudio
//.lam  audio/x-liveaudio
//.latex    application/x-latex
//.lha  application/lha
//.lha  application/octet-stream
//.lha  application/x-lha
//.lhx  application/octet-stream
//.list text/plain
//.lma  audio/nspaudio
//.lma  audio/x-nspaudio
//.log  text/plain
//.lsp  application/x-lisp
//.lsp  text/x-script.lisp
//.lst  text/plain
//.lsx  text/x-la-asf
//.ltx  application/x-latex
//.lzh  application/octet-stream
//.lzh  application/x-lzh
//.lzx  application/lzx
//.lzx  application/octet-stream
//.lzx  application/x-lzx
//.m    text/plain
//.m    text/x-m
//.m1v  video/mpeg
//.m2a  audio/mpeg
//.m2v  video/mpeg
//.m3u  audio/x-mpequrl
//.man  application/x-troff-man
//.map  application/x-navimap
//.mar  text/plain
//.mbd  application/mbedlet
//.mc$  application/x-magic-cap-package-1.0
//.mcd  application/mcad
//.mcd  application/x-mathcad
//.mcf  image/vasa
//.mcf  text/mcf
//.mcp  application/netmc
//.me   application/x-troff-me
//.mht  message/rfc822
//.mhtml    message/rfc822
//.mid  application/x-midi
//.mid  audio/midi
//.mid  audio/x-mid
//.mid  audio/x-midi
//.mid  music/crescendo
//.mid  x-music/x-midi
//.midi application/x-midi
//.midi audio/midi
//.midi audio/x-mid
//.midi audio/x-midi
//.midi music/crescendo
//.midi x-music/x-midi
//.mif  application/x-frame
//.mif  application/x-mif
//.mime message/rfc822
//.mime www/mime
//.mjf  audio/x-vnd.audioexplosion.mjuicemediafile
//.mjpg video/x-motion-jpeg
//.mm   application/base64
//.mm   application/x-meme
//.mme  application/base64
//.mod  audio/mod
//.mod  audio/x-mod
//.moov video/quicktime
//.mov  video/quicktime
//.movie    video/x-sgi-movie
//.mp2  audio/mpeg
//.mp2  audio/x-mpeg
//.mp2  video/mpeg
//.mp2  video/x-mpeg
//.mp2  video/x-mpeq2a
//.mp3  audio/mpeg3
//.mp3  audio/x-mpeg-3
//.mp3  video/mpeg
//.mp3  video/x-mpeg
//.mpa  audio/mpeg
//.mpa  video/mpeg
//.mpc  application/x-project
//.mpe  video/mpeg
//.mpeg video/mpeg
//.mpg  audio/mpeg
//.mpg  video/mpeg
//.mpga audio/mpeg
//.mpp  application/vnd.ms-project
//.mpt  application/x-project
//.mpv  application/x-project
//.mpx  application/x-project
//.mrc  application/marc
//.ms   application/x-troff-ms
//.mv   video/x-sgi-movie
//.my   audio/make
//.mzz  application/x-vnd.audioexplosion.mzz
//.nap  image/naplps
//.naplps   image/naplps
//.nc   application/x-netcdf
//.ncm  application/vnd.nokia.configuration-message
//.nif  image/x-niff
//.niff image/x-niff
//.nix  application/x-mix-transfer
//.nsc  application/x-conference
//.nvd  application/x-navidoc
//.o    application/octet-stream
//.oda  application/oda
//.omc  application/x-omc
//.omcd application/x-omcdatamaker
//.omcr application/x-omcregerator
//.p    text/x-pascal
//.p10  application/pkcs10
//.p10  application/x-pkcs10
//.p12  application/pkcs-12
//.p12  application/x-pkcs12
//.p7a  application/x-pkcs7-signature
//.p7c  application/pkcs7-mime
//.p7c  application/x-pkcs7-mime
//.p7m  application/pkcs7-mime
//.p7m  application/x-pkcs7-mime
//.p7r  application/x-pkcs7-certreqresp
//.p7s  application/pkcs7-signature
//.part application/pro_eng
//.pas  text/pascal
//.pbm  image/x-portable-bitmap
//.pcl  application/vnd.hp-pcl
//.pcl  application/x-pcl
//.pct  image/x-pict
//.pcx  image/x-pcx
//.pdb  chemical/x-pdb
//.pdf  application/pdf
//.pfunk    audio/make
//.pfunk    audio/make.my.funk
//.pgm  image/x-portable-graymap
//.pgm  image/x-portable-greymap
//.pic  image/pict
//.pict image/pict
//.pkg  application/x-newton-compatible-pkg
//.pko  application/vnd.ms-pki.pko
//.pl   text/plain
//.pl   text/x-script.perl
//.plx  application/x-pixclscript
//.pm   image/x-xpixmap
//.pm   text/x-script.perl-module
//.pm4  application/x-pagemaker
//.pm5  application/x-pagemaker
//.png  image/png
//.pnm  application/x-portable-anymap
//.pnm  image/x-portable-anymap
//.pot  application/mspowerpoint
//.pot  application/vnd.ms-powerpoint
//.pov  model/x-pov
//.ppa  application/vnd.ms-powerpoint
//.ppm  image/x-portable-pixmap
//.pps  application/mspowerpoint
//.pps  application/vnd.ms-powerpoint
//.ppt  application/mspowerpoint
//.ppt  application/powerpoint
//.ppt  application/vnd.ms-powerpoint
//.ppt  application/x-mspowerpoint
//.ppz  application/mspowerpoint
//.pre  application/x-freelance
//.prt  application/pro_eng
//.ps   application/postscript
//.psd  application/octet-stream
//.pvu  paleovu/x-pv
//.pwz  application/vnd.ms-powerpoint
//.py   text/x-script.phyton
//.pyc  application/x-bytecode.python
//.qcp  audio/vnd.qcelp
//.qd3  x-world/x-3dmf
//.qd3d x-world/x-3dmf
//.qif  image/x-quicktime
//.qt   video/quicktime
//.qtc  video/x-qtc
//.qti  image/x-quicktime
//.qtif image/x-quicktime
//.ra   audio/x-pn-realaudio
//.ra   audio/x-pn-realaudio-plugin
//.ra   audio/x-realaudio
//.ram  audio/x-pn-realaudio
//.ras  application/x-cmu-raster
//.ras  image/cmu-raster
//.ras  image/x-cmu-raster
//.rast image/cmu-raster
//.rexx text/x-script.rexx
//.rf   image/vnd.rn-realflash
//.rgb  image/x-rgb
//.rm   application/vnd.rn-realmedia
//.rm   audio/x-pn-realaudio
//.rmi  audio/mid
//.rmm  audio/x-pn-realaudio
//.rmp  audio/x-pn-realaudio
//.rmp  audio/x-pn-realaudio-plugin
//.rng  application/ringing-tones
//.rng  application/vnd.nokia.ringing-tone
//.rnx  application/vnd.rn-realplayer
//.roff application/x-troff
//.rp   image/vnd.rn-realpix
//.rpm  audio/x-pn-realaudio-plugin
//.rt   text/richtext
//.rt   text/vnd.rn-realtext
//.rtf  application/rtf
//.rtf  application/x-rtf
//.rtf  text/richtext
//.rtx  application/rtf
//.rtx  text/richtext
//.rv   video/vnd.rn-realvideo
//.s    text/x-asm
//.s3m  audio/s3m
//.saveme   application/octet-stream
//.sbk  application/x-tbook
//.scm  application/x-lotusscreencam
//.scm  text/x-script.guile
//.scm  text/x-script.scheme
//.scm  video/x-scm
//.sdml text/plain
//.sdp  application/sdp
//.sdp  application/x-sdp
//.sdr  application/sounder
//.sea  application/sea
//.sea  application/x-sea
//.set  application/set
//.sgm  text/sgml
//.sgm  text/x-sgml
//.sgml text/sgml
//.sgml text/x-sgml
//.sh   application/x-bsh
//.sh   application/x-sh
//.sh   application/x-shar
//.sh   text/x-script.sh
//.shar application/x-bsh
//.shar application/x-shar
//.shtml    text/html
//.shtml    text/x-server-parsed-html
//.sid  audio/x-psid
//.sit  application/x-sit
//.sit  application/x-stuffit
//.skd  application/x-koan
//.skm  application/x-koan
//.skp  application/x-koan
//.skt  application/x-koan
//.sl   application/x-seelogo
//.smi  application/smil
//.smil application/smil
//.snd  audio/basic
//.snd  audio/x-adpcm
//.sol  application/solids
//.spc  application/x-pkcs7-certificates
//.spc  text/x-speech
//.spl  application/futuresplash
//.spr  application/x-sprite
//.sprite   application/x-sprite
//.src  application/x-wais-source
//.ssi  text/x-server-parsed-html
//.ssm  application/streamingmedia
//.sst  application/vnd.ms-pki.certstore
//.step application/step
//.stl  application/sla
//.stl  application/vnd.ms-pki.stl
//.stl  application/x-navistyle
//.stp  application/step
//.sv4cpio  application/x-sv4cpio
//.sv4crc   application/x-sv4crc
//.svf  image/vnd.dwg
//.svf  image/x-dwg
//.svr  application/x-world
//.svr  x-world/x-svr
//.swf  application/x-shockwave-flash
//.t    application/x-troff
//.talk text/x-speech
//.tar  application/x-tar
//.tbk  application/toolbook
//.tbk  application/x-tbook
//.tcl  application/x-tcl
//.tcl  text/x-script.tcl
//.tcsh text/x-script.tcsh
//.tex  application/x-tex
//.texi application/x-texinfo
//.texinfo  application/x-texinfo
//.text application/plain
//.text text/plain
//.tgz  application/gnutar
//.tgz  application/x-compressed
//.tif  image/tiff
//.tif  image/x-tiff
//.tiff image/tiff
//.tiff image/x-tiff
//.tr   application/x-troff
//.tsi  audio/tsp-audio
//.tsp  application/dsptype
//.tsp  audio/tsplayer
//.tsv  text/tab-separated-values
//.turbot   image/florian
//.txt  text/plain
//.uil  text/x-uil
//.uni  text/uri-list
//.unis text/uri-list
//.unv  application/i-deas
//.uri  text/uri-list
//.uris text/uri-list
//.ustar    application/x-ustar
//.ustar    multipart/x-ustar
//.uu   application/octet-stream
//.uu   text/x-uuencode
//.uue  text/x-uuencode
//.vcd  application/x-cdlink
//.vcs  text/x-vcalendar
//.vda  application/vda
//.vdo  video/vdo
//.vew  application/groupwise
//.viv  video/vivo
//.viv  video/vnd.vivo
//.vivo video/vivo
//.vivo video/vnd.vivo
//.vmd  application/vocaltec-media-desc
//.vmf  application/vocaltec-media-file
//.voc  audio/voc
//.voc  audio/x-voc
//.vos  video/vosaic
//.vox  audio/voxware
//.vqe  audio/x-twinvq-plugin
//.vqf  audio/x-twinvq
//.vql  audio/x-twinvq-plugin
//.vrml application/x-vrml
//.vrml model/vrml
//.vrml x-world/x-vrml
//.vrt  x-world/x-vrt
//.vsd  application/x-visio
//.vst  application/x-visio
//.vsw  application/x-visio
//.w60  application/wordperfect6.0
//.w61  application/wordperfect6.1
//.w6w  application/msword
//.wav  audio/wav
//.wav  audio/x-wav
//.wb1  application/x-qpro
//.wbmp image/vnd.wap.wbmp
//.web  application/vnd.xara
//.wiz  application/msword
//.wk1  application/x-123
//.wmf  windows/metafile
//.wml  text/vnd.wap.wml
//.wmlc application/vnd.wap.wmlc
//.wmls text/vnd.wap.wmlscript
//.wmlsc    application/vnd.wap.wmlscriptc
//.word application/msword
//.wp   application/wordperfect
//.wp5  application/wordperfect
//.wp5  application/wordperfect6.0
//.wp6  application/wordperfect
//.wpd  application/wordperfect
//.wpd  application/x-wpwin
//.wq1  application/x-lotus
//.wri  application/mswrite
//.wri  application/x-wri
//.wrl  application/x-world
//.wrl  model/vrml
//.wrl  x-world/x-vrml
//.wrz  model/vrml
//.wrz  x-world/x-vrml
//.wsc  text/scriplet
//.wsrc application/x-wais-source
//.wtk  application/x-wintalk
//.xbm  image/x-xbitmap
//.xbm  image/x-xbm
//.xbm  image/xbm
//.xdr  video/x-amt-demorun
//.xgz  xgl/drawing
//.xif  image/vnd.xiff
//.xl   application/excel
//.xla  application/excel
//.xla  application/x-excel
//.xla  application/x-msexcel
//.xlb  application/excel
//.xlb  application/vnd.ms-excel
//.xlb  application/x-excel
//.xlc  application/excel
//.xlc  application/vnd.ms-excel
//.xlc  application/x-excel
//.xld  application/excel
//.xld  application/x-excel
//.xlk  application/excel
//.xlk  application/x-excel
//.xll  application/excel
//.xll  application/vnd.ms-excel
//.xll  application/x-excel
//.xlm  application/excel
//.xlm  application/vnd.ms-excel
//.xlm  application/x-excel
//.xls  application/excel
//.xls  application/vnd.ms-excel
//.xls  application/x-excel
//.xls  application/x-msexcel
//.xlt  application/excel
//.xlt  application/x-excel
//.xlv  application/excel
//.xlv  application/x-excel
//.xlw  application/excel
//.xlw  application/vnd.ms-excel
//.xlw  application/x-excel
//.xlw  application/x-msexcel
//.xm   audio/xm
//.xml  application/xml
//.xml  text/xml
//.xmz  xgl/movie
//.xpix application/x-vnd.ls-xpix
//.xpm  image/x-xpixmap
//.xpm  image/xpm
//.x-png    image/png
//.xsr  video/x-amt-showrun
//.xwd  image/x-xwd
//.xwd  image/x-xwindowdump
//.xyz  chemical/x-pdb
//.z    application/x-compress
//.z    application/x-compressed
//.zip  application/x-compressed
//.zip  application/x-zip-compressed
//.zip  application/zip
//.zip  multipart/x-zip
//.zoo  application/octet-stream
//.zsh  text/x-script.zsh



