///////////////////////////////////////////////////////////////////////////////
/// @file   Color.hpp
/// @brief  Definition of class Color
/// @author Benjamin Hampe <benjaminhampe@gmx.de>
/// @date   2018
///
/// Copyright (c) 2018 Benjamin Hampe
///////////////////////////////////////////////////////////////////////////////

#ifndef DE_HAMPE_COLOR_HPP
#define DE_HAMPE_COLOR_HPP

#include <de/hampe/Types.hpp>

inline uint8_t ARGB_A( uint32_t const color ) { return ( color >> 24 ) & 0xff; }
inline uint8_t ARGB_R( uint32_t const color ) { return ( color >> 16 ) & 0xff; }
inline uint8_t ARGB_G( uint32_t const color ) { return ( color >> 8 ) & 0xff; }
inline uint8_t ARGB_B( uint32_t const color ) { return ( color ) & 0xff; }

inline uint32_t
ARGB( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 )
{
	return ( uint32_t( a ) << 24 ) | ( uint32_t( r ) << 16 ) | ( uint32_t( g ) << 8 ) | uint32_t( b );
}

inline uint8_t
randomByte()
{
	return uint8_t( rand() % 255 );
}

inline float32_t
randomFloat()
{
	return glm::clamp( float32_t( rand() % 65536 ) * ( 1.0f / 65535.0f ), 0.0f, 1.0f );
}

inline uint32_t
randomRGB()
{
	return ARGB( randomByte(), randomByte(), randomByte(), 255 );
}

inline glm::vec4
randomRGBf()
{
	return glm::vec4( randomFloat(), randomFloat(), randomFloat(), 1.0f ) ;
}

inline uint32_t
randomColor()
{
	return ARGB( rand() % 255, rand() % 255, rand() % 255, 255 );
}

/// @brief Creates a pseudo random color relative to given color that add an offset to each component in range [-variance/2,+variance/2].
inline uint32_t
varyColor( uint32_t color, int variance )
{
	int32_t r = ARGB_R( color );
	int32_t g = ARGB_G( color );
	int32_t b = ARGB_B( color );
	int32_t const a = ARGB_A( color );

	r = glm::clamp( r - variance / 2 + ( rand() % variance ), 0, 255 );
	g = glm::clamp( g - variance / 2 + ( rand() % variance ), 0, 255 );
	b = glm::clamp( b - variance / 2 + ( rand() % variance ), 0, 255 );
	return ARGB( r, g, b, a );
}

#if 0
// ============================================================================
static QColor blendColors( const QColor& src, const QColor& dst )
// ============================================================================
{
   const float s = (float)src.alpha() / 255.0f;
   const float d = (float)dst.alpha() / 255.0f;

   const int r = math::min32( 255, math::round32( s * src.red() + d * dst.red() ) );
   const int g = math::min32( 255, math::round32( s * src.green() + d * dst.green() ) );
   const int b = math::min32( 255, math::round32( s * src.blue() + d * dst.blue() ) );
   const int a = math::min32( 255, math::round32( s * src.alpha() + d * dst.alpha() ) );

   return QColor( r,g,b,a );
}
#endif
#endif // DE_HAMPE_COMMON_IMAGE_COLOR_HPP
