///////////////////////////////////////////////////////////////////////////////
/// @file   ColorFormat.hpp
/// @brief  Definition of class ColorFormat
/// @author Benjamin Hampe <benjaminhampe@gmx.de>
/// @date   2018
///
/// Copyright (c) 2018 Benjamin Hampe
///////////////////////////////////////////////////////////////////////////////

#ifndef DE_HAMPE_IMAGE_COLORFORMAT_HPP
#define DE_HAMPE_IMAGE_COLORFORMAT_HPP

#include <stdint.h>

namespace de {
namespace hampe {
namespace image {

/// @brief
class ColorFormat
{
public:
   enum eColorFormat : uint32_t
   {
      A8R8G8B8 = 0,
      R8G8B8,
      R8,
      R16, // e.g. heightmap
      R32, // e.g. heightmap
      R5G6B5,
      A1R5G5B5,
      ENUM_END
   };

   ~ColorFormat() = default;
   ColorFormat() = default;
   ColorFormat( ColorFormat const & ) = default;
   ColorFormat( ColorFormat && ) = default;
   ColorFormat& operator=( ColorFormat const & ) = default;
   ColorFormat& operator=( ColorFormat && ) = default;

   static uint32_t getBytesPerPixel( uint32_t const colorFormat )
   {
      switch( colorFormat )
      {
         case A8R8G8B8: return 4;
         case R8G8B8: return 3;
         case R8: return 1;
         case R16: return 2;
         case R32: return 4;
         case R5G6B5: return 2;
         case A1R5G5B5: return 2;
         default: return 0;
      }
   }

   uint32_t getBytesPerPixel() const { return getBytesPerPixel( m_ColorFormat ); }
   uint32_t getBitsPerPixel() const { return getBytesPerPixel() << 3; }

   ColorFormat( uint32_t const colorFormat ) : m_ColorFormat( colorFormat ) {}
   operator uint32_t() const { return m_ColorFormat; }
   uint32_t getValue() const { return m_ColorFormat; }
   void setValue( uint32_t colorFormat ) { m_ColorFormat = colorFormat; }

private:
   uint32_t m_ColorFormat = A8R8G8B8;
};

} // end namespace image
} // end namespace hampe
} // end namespace de

#endif   // DE_HAMPE_IMAGE_COLORFORMAT_HPP
