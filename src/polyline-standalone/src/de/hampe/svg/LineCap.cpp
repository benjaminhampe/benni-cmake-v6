//////////////////////////////////////////////////////////////////////
/// @file   LineCap.cpp
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

#include "LineCap.hpp"

using de::hampe::svg::LineCap;

std::ostream &
operator<< ( std::ostream & out, LineCap const lineCap )
{
    switch( lineCap )
    {
        case LineCap::SQUARE: out << "SQUARE"; break;
        case LineCap::ROUND: out << "ROUND"; break;
        //case LineCap::MITER: out << "MITER"; break;
        default: out << "NONE"; break;
    }

    return out;
}
