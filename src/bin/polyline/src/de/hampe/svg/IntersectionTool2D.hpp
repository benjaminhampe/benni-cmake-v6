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

#include <de/hampe/svg/Math.hpp>

namespace de {
namespace hampe {
namespace svg {

///@brief A utility class carrying some helper functions used in data::tile::DtmGeometry, and a logger
class IntersectionTool2D
{
    CREATE_STATIC_LOGGER( "de.hampe.svg.IntersectionTool2D" )
    using LogLevel = de::hampe::logging::LogLevel;

    /// DEFINITIONS:
    ///
    /// A line: infinite long line that has a direction and contains 2 given points A,B or P,Q
    /// A ray: infinite long line that has a direction and a fixed start point -> NOT USED OR NEEDED
    /// A line-segment: finite line that has a direction, a start point P( t=0 ) and an end point Q( t=1 ) and only exists between these points.
    ///
    /// Thatswhy the most generic function is called intersectLineEx(),
    /// the 'Ex' stands for 'extended' and returns the internally computed params t,s back to the caller,
    /// This way the caller can make further decisions using t,s and has much more control.

public:
    ///@brief Most generic 2D intersection function for arbitrary
    ///@brief Intersects arbitrary 2d line PQ with arbitrary 2d line AB ( most general case )
    ///@param[out] s The intersection point returned by this function, if any ( contents are only valid if function returned true )
    ///@param[in] p The start point of arbitrary 2d line PQ
    ///@param[in] q The end point of arbitrary 2d line PQ
    ///@param[in] a The start point of arbitrary 2d line AB
    ///@param[in] b The end point of arbitrary 2d line AB
    ///@return True, if intersection was found between points P,Q ( param t of eq: P + t * PQ must be in range [0,1] ), else false.
    ///
    static bool
    intersectLine32( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B );

    ///@brief Most generic 2D intersection function for arbitrary
    ///@brief Intersects arbitrary 2d line PQ with arbitrary 2d line AB ( most general case )
    ///@param[out] s The intersection point returned by this function, if any ( contents are only valid if function returned true )
    ///@param[in] p The start point of arbitrary 2d line PQ
    ///@param[in] q The end point of arbitrary 2d line PQ
    ///@param[in] a The start point of arbitrary 2d line AB
    ///@param[in] b The end point of arbitrary 2d line AB
    ///@return True, if intersection was found between points P,Q ( param t of eq: P + t * PQ must be in range [0,1] ), else false.
    ///
    static bool
    intersectLine64( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B );

    ///@brief Most generic 2D intersection function for arbitrary
    ///@brief Intersects arbitrary 2d line PQ with arbitrary 2d line AB ( most general case )
    ///@param[out] s The intersection point returned by this function, if any ( contents are only valid if function returned true )
    ///@param[in] p The start point of arbitrary 2d line PQ
    ///@param[in] q The end point of arbitrary 2d line PQ
    ///@param[in] a The start point of arbitrary 2d line AB
    ///@param[in] b The end point of arbitrary 2d line AB
    ///@return True, if intersection was found between points P,Q ( param t of eq: P + t * PQ must be in range [0,1] ), else false.
    ///
    static bool
    intersectLineEx64( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B, float32_t & rT, float32_t & rS );

    ///@brief Most generic 2D intersection function for arbitrary
    ///@brief Intersects arbitrary 2d line PQ with arbitrary 2d line AB ( most general case )
    ///@param[out] s The intersection point returned by this function, if any ( contents are only valid if function returned true )
    ///@param[in] p The start point of arbitrary 2d line PQ
    ///@param[in] q The end point of arbitrary 2d line PQ
    ///@param[in] a The start point of arbitrary 2d line AB
    ///@param[in] b The end point of arbitrary 2d line AB
    ///@return True, if intersection was found between points P,Q ( param t of eq: P + t * PQ must be in range [0,1] ), else false.
    ///@attention Unlike the previous version, this does not compute and store the squared distance in S.z anymore. ( dist from start point P )
    ///
    static bool
    intersectLineEx( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B, float32_t & rT, float32_t & rS );

    static bool
    intersectLine( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B );

    static bool
    intersectLineSegment( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B );

    /// Used for DtmUtils::intersectPolyLine(), because the DTM grid can be seen as arbitrary line -> little speedup due to 1 if-statement instead of 2 per intersection
    static bool
    intersectLineSegmentAndLine( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B );

#if 0

   ///@brief Intersects arbitrary 2d line PQ with a horizontal 2d line AB ( specialized function for case: B.y - A.y == 0 )
   ///@param[out] s The intersection point returned by this function, if any ( contents are only valid if function returned true )
   ///@param[in] p The start point of arbitrary 2d line PQ
   ///@param[in] q The end point of arbitrary 2d line PQ
   ///@param[in] a The start point of horizontal 2d line AB
   ///@param[in] b The end point of horizontal 2d line AB ( B.y is discarded from computation )
   ///@return True, if intersection was found between points P,Q ( param t of eq: P + t * PQ must be in range [0,1] ), else false.
   ///@attention Unlike the previous version, this does not compute and store the squared distance in S.z anymore. ( dist from start point P )
   static bool
   intersectHLine2D( glm::vec2 & s,
                     glm::vec2 const & p, glm::vec2 const & q,
                     glm::vec2 const & a, glm::vec2 const & b );

   ///@brief Intersects arbitrary 2d line PQ with a vertical 2d line AB ( specialized function for case: B.x - A.x == 0 )
   ///@param[out] s The intersection point returned by this function, if any ( contents are only valid if function returned true )
   ///@param[in] p The start point of arbitrary 2d line PQ
   ///@param[in] q The end point of arbitrary 2d line PQ
   ///@param[in] a The start point of vertical 2d line AB
   ///@param[in] b The end point of vertical 2d line AB ( B.x is discarded from computation )
   ///@return True, if intersection was found between points P,Q ( param t of eq: P + t * PQ must be in range [0,1] ), else false.
   ///@attention Unlike the previous version, this does not compute and store the squared distance in S.z anymore. ( dist from start point P )
   static bool
   intersectVLine2D( glm::vec2 & s,
                     glm::vec2 const & p, glm::vec2 const & q,
                     glm::vec2 const & a, glm::vec2 const & b );

#endif

};

} // end namespace svg
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_SVG_INTERSECTIONTOOL2D_HPP
