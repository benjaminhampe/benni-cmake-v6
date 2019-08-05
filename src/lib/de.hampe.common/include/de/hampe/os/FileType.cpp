#include "FileType.hpp"

namespace de { 
namespace hampe { 
namespace os {

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