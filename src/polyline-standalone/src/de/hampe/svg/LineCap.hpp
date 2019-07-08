//////////////////////////////////////////////////////////////////////
/// @file 	LineCap.hpp
/// @brief  SVG LineCap
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

#ifndef DE_HAMPE_SVG_LINECAP_HPP
#define DE_HAMPE_SVG_LINECAP_HPP

#include <ostream>

namespace de {
namespace hampe {
namespace svg {

/// @brief LineCaps for Start- and EndSegment of a polyline by SVG standard
struct LineCap
{
    enum eLineCap
    {
        NONE = 0,       // SVG lineCap = "butt"
        SQUARE,         // SVG lineCap = "square"
        ROUND,          // SVG lineCap = "round"
        ENUM_END
    };

    LineCap( eLineCap value ) : Value( value ) {}
    operator eLineCap () const { return Value; }

    eLineCap Value = NONE; ///< Member Field
};

} // end namespace svg
} // end namespace hampe
} // end namespace de

/// @brief Print LineCaps
std::ostream &
operator<< ( std::ostream & out, de::hampe::svg::LineCap const lineCap );

#endif // DE_HAMPE_SVG_LINECAP_HPP
