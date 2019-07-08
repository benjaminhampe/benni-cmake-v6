#ifndef LINEJOIN_HPP
#define LINEJOIN_HPP

#include "PolyLineCommon.hpp"
#include "PrimitiveRenderer.hpp"

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

/// @brief Print class LineJoin
std::ostream &
operator<< ( std::ostream & out, LineJoin const lineJoin );

/// @brief Adds a line-join using a given geometry generator
/// using 3 points to construct each LineJoin and using precomputed angle phiABC to enable some smarter abort conditions
void
addLineJoin(
        PrimitiveRenderer & dc,
        LineJoin const lineJoin,
        glm::vec2 const C,
        glm::vec2 const A,
        glm::vec2 const B,
        float32_t const phi );

#endif // LINEJOIN_HPP
