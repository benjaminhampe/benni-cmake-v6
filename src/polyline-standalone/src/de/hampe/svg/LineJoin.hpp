//////////////////////////////////////////////////////////////////////
/// @file   LineJoin.hpp
/// @brief  SVG LineJoin
/// @author Copyright (c) Benjamin Hampe <benjaminhampe@gmx.de>
///
///	The author grants permission of free use, free distribution and
/// free usage for commercial applications even for modified sources.
///
///	What is not allowed is to remove the original authors name or claim
/// any intellectual rights. This original code was written in my spare time
/// and is distributed publicly as free software to anyone over GitLab and GitHub.
///
//////////////////////////////////////////////////////////////////////

#ifndef DE_HAMPE_SVG_LINEJOIN_HPP
#define DE_HAMPE_SVG_LINEJOIN_HPP

#include <ostream>

namespace de {
namespace hampe {
namespace svg {

/// @brief LineJoins between two segments of a polyline by SVG standard
struct LineJoin
{
    enum ELineJoin
    {
        NONE = 0,   // No visible line-join
        BEVEL,      // Add one or two triangles == AUTO_DOUBLE_BEVEL
        ROUND,      // For now a circular, not elliptic shape, because lineWidth is const for all segments ( for now )
        MITER,      // Not used, produces too large and ugly triangles. TODO: Miter should reuse already computed intersection point
        ENUM_END    // Indicates invalid state, also counts the enum-items before
    };

    LineJoin( ELineJoin value ) : Value( value ) {}
    operator ELineJoin () const { return Value; }
    ELineJoin Value = ROUND; ///< Member Field
};

} // end namespace svg
} // end namespace hampe
} // end namespace de

/// @brief Print class LineJoin
std::ostream &
operator<< ( std::ostream & out, de::hampe::svg::LineJoin const lineJoin );

#endif // DE_HAMPE_SVG_LINEJOIN_HPP
