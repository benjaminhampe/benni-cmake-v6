//////////////////////////////////////////////////////////////////////
/// @file IntersectionTool2D.hpp
/// @brief Definition of class IntersectionTool2D
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

#ifndef DE_HAMPE_MATH_INTERSECTIONTOOL2D_HPP
#define DE_HAMPE_MATH_INTERSECTIONTOOL2D_HPP

#include <de/hampe/math/Math.hpp>

namespace de {
namespace hampe {
namespace math {

///@brief A utility class carrying some helper functions used in data::tile::DtmGeometry, and a logger
class IntersectionTool2DTest
{
    CREATE_STATIC_CLASS_LOGGER( "de.hampe.math.IntersectionTool2DTest" )
    using LogLevel = de::hampe::logging::LogLevel;
};

} // end namespace math
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_MATH_INTERSECTIONTOOL2D_HPP
