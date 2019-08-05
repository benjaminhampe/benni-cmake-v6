#ifndef IRREXT_IMAGEUTILS_HPP
#define IRREXT_IMAGEUTILS_HPP

#include <irrExt/Types.hpp>

namespace irrExt {

void
printTextures( irr::video::IVideoDriver* driver );

irr::video::IImage*
loadImage( irr::video::IVideoDriver* driver, std::string const & fileName );

irr::video::IImage*
createImage( irr::video::IVideoDriver* driver, int w, int h );

irr::video::ITexture*
createTextureFromImage(
    irr::video::IVideoDriver* driver,
    irr::video::IImage* img,
    std::string const & imgName = "" );

} // end namespace irrExt

#endif // IRREXT_IMAGEUTILS_HPP
