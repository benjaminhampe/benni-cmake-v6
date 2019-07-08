#include "FileSystemSTL.hpp"

#include <iostream>

// I <still> use GCC 7.3.0
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
// #include <filesystem>
// namespace fs = std::filesystem;

namespace de { 
namespace hampe { 
namespace os {

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

bool
FileSystemSTL::existFile( std::string const & fileName )
{
   fs::file_status fileStat = fs::status( fileName );
   bool ok = fs::exists( fileStat ) && fs::is_regular_file( fileStat );
   return ok;
}

bool
FileSystemSTL::existDirectory( std::string const & fileName )
{
   fs::file_status fileStat = fs::status( fileName );
   bool ok = fs::exists( fileStat ) && fs::is_directory( fileStat );
   return ok;
}

std::string
FileSystemSTL::absoluteFileName( std::string const & fileName )
{
   return fs::canonical( fileName ).string();
}

std::string
FileSystemSTL::filePath( std::string const & fileName )
{
   return fs::canonical( fileName ).root_path().string();
}

std::string
FileSystemSTL::fileName( std::string const & fileName )
{
   return fs::path( fileName ).filename().string();
}

std::string
FileSystemSTL::fileExtension( std::string const & fileName )
{
   return fs::path( fileName ).extension().string();
}

uintmax_t
FileSystemSTL::fileSize( std::string const & fileName )
{
   return fs::file_size( fileName );
}

os::FileType
FileSystemSTL::fileType( std::string const & fileName )
{
   return os::FileType::fromFileExtension( fileExtension( fileName ) );
}

os::FileType
FileSystemSTL::fileMagic( std::string const & fileName )
{
   return os::getFileMagic( fileName );
}

} // end namespace os
} // end namespace hampe
} // end namespace de