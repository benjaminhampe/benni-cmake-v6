#ifndef LINECAP_HPP
#define LINECAP_HPP

#include "PolyLineCommon.hpp"
#include "PrimitiveRenderer.hpp"

/// @brief LineCaps for Start- and EndSegment of a polyline by SVG standard
struct LineCap
{
    enum ELineCap
    {
        NONE = 0,       // SVG lineCap = "butt"
        SQUARE,         // SVG lineCap = "square"
        ROUND,          // SVG lineCap = "round"
        ENUM_END
    };

    LineCap( ELineCap value ) : Value( value ) {}
    operator ELineCap () const { return Value; }
    ELineCap Value = NONE; ///< Member Field
};

/// @brief Print LineCaps
std::ostream &
operator<< ( std::ostream & out, LineCap const lineCap );

/// @brief Generates different types of line-caps by the SVG standard
/// @param[in,out] dc Geometry generator that can create triangles -> main output
/// @param[in] lineCap The type of shape we construct ( none, square, round ).
/// @param[in] M The already computed center-point A ( reused to prevent artifacts )
/// @param[in] L The already computed left point L = A - nAB ( reused to prevent artifacts )
/// @param[in] R The already computed reight point R = A + nAB ( reused to prevent artifacts )
/// @param[in] isEndCap If <false> we have a StartCap and if <true> when we construct an EndCap.
/// @param[in] color Only used for visual debugging, TODO: remove in release
void
addLineCap(
    PrimitiveRenderer & dc,
    LineCap const lineCap,
    glm::vec2 const M,
    glm::vec2 const L,
    glm::vec2 const R,
    bool const isEndCap );

#endif // LINECAP_HPP
