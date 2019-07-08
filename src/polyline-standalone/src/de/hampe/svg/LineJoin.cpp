//////////////////////////////////////////////////////////////////////
/// @file   LineJoin.cpp
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

#include "LineJoin.hpp"

using de::hampe::svg::LineJoin;

std::ostream &
operator<< ( std::ostream & out, LineJoin const lineJoin )
{
    switch( lineJoin )
    {
        case LineJoin::BEVEL: out << "BEVEL"; break;
        case LineJoin::ROUND: out << "ROUND"; break;
        case LineJoin::MITER: out << "MITER"; break;
        default: out << "NONE"; break;
    }

    return out;
}
