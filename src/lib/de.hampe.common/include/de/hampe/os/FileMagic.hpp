#ifndef DE_HAMPE_OS_FILEMAGIC_HPP
#define DE_HAMPE_OS_FILEMAGIC_HPP

#include <de/hampe/os/FileType.hpp>

#ifdef USE_TAGLIB
#include <tbytevector.h>
#endif

namespace de { 
namespace hampe { 
namespace os {


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

} // end namespace os
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_OS_FILEMAGIC_HPP
