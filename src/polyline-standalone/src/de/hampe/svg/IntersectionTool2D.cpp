//////////////////////////////////////////////////////////////////////
/// @file IntersectionTool2D.cpp
/// @brief Implements class IntersectionTool2D
/// @author Benjamin Hampe <benjaminhampe@gmx.de>
//////////////////////////////////////////////////////////////////////

#include <de/hampe/svg/IntersectionTool2D.hpp>

// #define ENABLE_ERROR_LOGGING
// #define ENABLE_DEBUG_LOGGING

namespace de {
namespace hampe {
namespace svg {
	
constexpr float32_t const EPSILON = std::numeric_limits< float32_t >::epsilon();
constexpr float64_t const EPSILON64 = std::numeric_limits< float64_t >::epsilon();

/// Final Benni version so far to be tested...
bool
IntersectionTool2D::intersectLine32(    glm::vec2 & S,
                                        glm::vec2 const P, glm::vec2 const Q,
                                        glm::vec2 const A, glm::vec2 const B )
{
    // <body id="default">
    //
    //    P + t * PQ:
    //
    //        t0 = [( P.y - A.y )( ABdx ) - ( P.x - A.x )( ABdy )]
    //        -------------------------------------------------------------
    //            [( ABdy )( PQdx ) - ( ABdx )( PQdy )]
    //
    //    A + s * AB:
    //
    //        s0 = [( P.y - A.y )( PQdx ) + ( P.x - A.x )( PQdy )]
    //        -------------------------------------------------------------
    //            [( ABdy )( PQdx ) - ( ABdx )( PQdy )]
    //
    float32_t const C1 = B.x - A.x; // C1 == ABdx
    float32_t const C2 = B.y - A.y; // C2 == ABdy
    float32_t const C3 = Q.x - P.x; // C3 == PQdx
    float32_t const C4 = Q.y - P.y; // C4 == PQdy
    float32_t const C5 = P.x - A.x; // C5 == APdx
    float32_t const C6 = P.y - A.y; // C6 == APdy
    float32_t const n = ( C2 * C3 - C1 * C4 );
    float32_t const t = ( C6 * C1 - C5 * C2 ) / n;

    if ( !std::isinf(t) )
    {
        // compute intersection point S
        S.x = P.x + t * C3;
        S.y = P.y + t * C4;
#ifdef ENABLE_DEBUG_LOGGING
        LOG_DEBUG( "Found ray intersection S(", S, ") with t(", t, ")\n" )
#endif
        return true;
    }
    // </body>

    // <body id="fallback" optional="true">
    // Fallback path, to be tested if ever needed...
    float32_t const s = ( C6 * C3 + C5 * C4 ) / n;
    if ( !std::isinf(s) )
    {
        // compute intersection point S
        S.x = A.x + s * C1;
        S.y = A.y + s * C2;
#ifdef ENABLE_DEBUG_LOGGING
        LOG_DEBUG( "Found fallback ray intersection S(", S, ") with s(", s, ")\n" )
#endif
        return true;
    }
    // </body>

    // <return>
    // Caller has to skip point or something
#ifdef ENABLE_ERROR_LOGGING
    LOG_ERROR("No intersection found s(", s, "), t(", t, ")\n")
#endif
    return false;
    // </return>
}

bool
IntersectionTool2D::intersectLine64(    glm::vec2 & S,
                                        glm::vec2 const P, glm::vec2 const Q,
                                        glm::vec2 const A, glm::vec2 const B )
{
    //
    //    P + t * PQ:
    //
    //        t0 = [( P.y - A.y )( ABdx ) - ( P.x - A.x )( ABdy )]
    //        -------------------------------------------------------------
    //            [( ABdy )( PQdx ) - ( ABdx )( PQdy )]
    //
    //    A + s * AB:
    //
    //        s0 = [( P.y - A.y )( PQdx ) + ( P.x - A.x )( PQdy )]
    //        -------------------------------------------------------------
    //            [( ABdy )( PQdx ) - ( ABdx )( PQdy )]
    //

    float64_t const C1 = B.x - A.x; // C1 == ABdx
    float64_t const C2 = B.y - A.y; // C2 == ABdy
    float64_t const C3 = Q.x - P.x; // C3 == PQdx
    float64_t const C4 = Q.y - P.y; // C4 == PQdy
    float64_t const C5 = P.x - A.x; // C5 == APdx
    float64_t const C6 = P.y - A.y; // C6 == APdy
    float64_t const n = ( C2 * C3 - C1 * C4 );
    float64_t const t = ( C6 * C1 - C5 * C2 ) / n;

    if ( !std::isinf(t) )
    {
        /// compute intersection point S
        S.x = float32_t( float64_t(P.x) + t * C3 );
        S.y = float32_t( float64_t(P.y) + t * C4 );
#ifdef ENABLE_DEBUG_LOGGING
        LOG_DEBUG( "Found ray intersection S(", S, ") with t(", t, ")\n" )
#endif
        return true;
    }

    // Fallback

    float64_t const s = ( C6 * C3 + C5 * C4 ) / n;
    if ( !std::isinf(s) )
    {
        /// compute intersection point S
        S.x = float32_t( float64_t(A.x) + s * C1 );
        S.y = float32_t( float64_t(A.y) + s * C2 );
#ifdef ENABLE_DEBUG_LOGGING
        LOG_DEBUG( "Found fallback ray intersection S(", S, ") with s(", s, ")\n" )
#endif
        return true;
    }

    // Caller has to skip point or something
#ifdef ENABLE_ERROR_LOGGING
    LOG_ERROR("No intersection found s(", s, "), t(", t, ")\n")
#endif
    return false;
}

bool
IntersectionTool2D::intersectLineEx64(  glm::vec2 & S,
                                        glm::vec2 const P, glm::vec2 const Q,
                                        glm::vec2 const A, glm::vec2 const B,
                                        float32_t & rT, float32_t & rS )
{
    //
    //    P + t * PQ:
    //
    //        t0 = [( P.y - A.y )( ABdx ) - ( P.x - A.x )( ABdy )]
    //        -------------------------------------------------------------
    //            [( ABdy )( PQdx ) - ( ABdx )( PQdy )]
    //
    //
    //    A + s * AB:
    //
    //        s0 = [( P.y - A.y )( PQdx ) + ( P.x - A.x )( PQdy )]
    //        -------------------------------------------------------------
    //            [( ABdy )( PQdx ) - ( ABdx )( PQdy )]
    //

    /// 1.) Both rays must not be vertical ( worst form of parallel ) at the same time...
    float64_t const C1 = B.x - A.x; // C1 - ABdx
    float64_t const C3 = Q.x - P.x; // C3 - PQdx
    if ( (std::fabs( C1 ) <= EPSILON64) && (std::fabs( C3 ) <= EPSILON64) )
    {
        #ifdef ENABLE_ERROR_LOGGING
        getLogger() << LogLevel::Error << __FUNCTION__ << " :: [benni] Both lines may not be parallel (vertical) together\n";
        #endif
        return false;
    }

    /// 2.) Both rays must not be parallel at all ...
    float64_t const C2 = B.y - A.y; // C2 - ABdy
    float64_t const C4 = Q.y - P.y; // C4 - PQdy
    float64_t const mAB = C2 / C1;
    float64_t const mPQ = C4 / C3;
    if ( std::fabs( mPQ - mAB ) <= EPSILON64 )
    {
        #ifdef ENABLE_ERROR_LOGGING
        getLogger() << LogLevel::Error << __FUNCTION__ << " :: [benni] Both lines may not be parallel together\n";
        #endif
        return false;
    }

    /// 3.) Prevent div by zero
    float64_t const n = ( C2 * C3 - C1 * C4 );
    if ( std::fabs( n ) <= EPSILON64 )
    {
        #ifdef ENABLE_ERROR_LOGGING
        getLogger() << LogLevel::Error << __FUNCTION__ << " :: [benni] Denominator (" << n << ") too small\n";
        #endif
        return false;
    }

    float64_t const C5 = P.x - A.x; // C5 - APdx
    float64_t const C6 = P.y - A.y; // C6 - APdy
    float64_t const invN = 1.0 / float64_t( n );
    float64_t const t = ( C6 * C1 - C5 * C2 ) * invN;
    float64_t const s = ( C6 * C3 + C5 * C4 ) * invN;

    /// compute intersection point S
    S.x = float32_t( float64_t(P.x) + float64_t(t) * C3 );
    S.y = float32_t( float64_t(P.y) + float64_t(t) * C4 );
    //S.z = P.z + t * ( Q.z - P.z );

    // this is the only difference to the 'non Ex' version
    rT = float32_t(t);
    rS = float32_t(s);

    #ifdef ENABLE_DEBUG_LOGGING
    getLogger() << LogLevel::Debug << __FUNCTION__ << " :: [benni] Found ray intersection S(" << S.x << "," << S.y << ") with t(" << t << "), s(" << s << ")\n";
    #endif
    return true;
}


bool
IntersectionTool2D::intersectLineEx(    glm::vec2 & S,
                                        glm::vec2 const P, glm::vec2 const Q,
                                        glm::vec2 const A, glm::vec2 const B,
                                        float32_t & rT, float32_t & rS )
{
    //
    //    P + t * PQ:
    //
    //        t0 = [( P.y - A.y )( ABdx ) - ( P.x - A.x )( ABdy )]
    //        -------------------------------------------------------------
    //            [( ABdy )( PQdx ) - ( ABdx )( PQdy )]
    //
    //
    //    A + s * AB:
    //
    //        s0 = [( P.y - A.y )( PQdx ) + ( P.x - A.x )( PQdy )]
    //        -------------------------------------------------------------
    //            [( ABdy )( PQdx ) - ( ABdx )( PQdy )]
    //

    /// 1.) Both rays must not be vertical ( worst form of parallel ) at the same time...
    float32_t const C1 = B.x - A.x; // C1 - ABdx
    float32_t const C3 = Q.x - P.x; // C3 - PQdx
    if ( (std::fabs( C1 ) <= EPSILON) && (std::fabs( C3 ) <= EPSILON) )
    {
        #ifdef ENABLE_ERROR_LOGGING
        getLogger() << LogLevel::Error << __FUNCTION__ << " :: [benni] Both lines may not be parallel (vertical) together\n";
        #endif
        return false;
    }

    /// 2.) Both rays must not be parallel at all ...
    float32_t const C2 = B.y - A.y; // C2 - ABdy
    float32_t const C4 = Q.y - P.y; // C4 - PQdy
    float32_t const mAB = C2 / C1;
    float32_t const mPQ = C4 / C3;
    if ( std::fabs( mPQ - mAB ) <= EPSILON )
    {
        #ifdef ENABLE_ERROR_LOGGING
        getLogger() << LogLevel::Error << __FUNCTION__ << " :: [benni] Both lines may not be parallel together\n";
        #endif
        return false;
    }

    /// 3.) Prevent div by zero
    float32_t const n = ( C2 * C3 - C1 * C4 );
    if ( std::fabs( n ) <= EPSILON )
    {
        #ifdef ENABLE_ERROR_LOGGING
        getLogger() << LogLevel::Error << __FUNCTION__ << " :: [benni] Denominator (" << n << ") too small\n";
        #endif
        return false;
    }

    float32_t const C5 = P.x - A.x; // C5 - APdx
    float32_t const C6 = P.y - A.y; // C6 - APdy
    float32_t const invN = 1.0F / n;
    float32_t const t = ( C6 * C1 - C5 * C2 ) * invN;
    float32_t const s = ( C6 * C3 + C5 * C4 ) * invN;

    /// compute intersection point S
    S.x = P.x + t * C3;
    S.y = P.y + t * C4;
    //S.z = P.z + t * ( Q.z - P.z );

    // this is the only difference to the 'non Ex' version
    rT = t;
    rS = s;

    #ifdef ENABLE_DEBUG_LOGGING
    getLogger() << LogLevel::Debug << __FUNCTION__ << " :: [benni] Found ray intersection S(" << S.x << "," << S.y << ") with t(" << t << "), s(" << s << ")\n";
    #endif
    return true;
}

bool
IntersectionTool2D::intersectLine( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B )
{
    float32_t t;
    float32_t s;
    return intersectLineEx( S, P, Q, A, B, t, s );
}

bool
IntersectionTool2D::intersectLineSegment( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B )
{
    float32_t t;
    float32_t s;
    if ( !intersectLineEx( S, P, Q, A, B, t, s ) )
    {
        return false;
    }

    // Check computed param t ( between P and Q )
    if ( ( t < -EPSILON ) || ( t > 1.0f + EPSILON ) )
    {
        #ifdef ENABLE_ERROR_LOGGING
        getLogger() << LogLevel::Error << __FUNCTION__ << " :: [benni] Param t(" << t << ") is out of bounds [0,1]\n";
        #endif
        return false; // No intersection point between points P and Q
    }

    // Check computed param s ( between A and B )
    if ( ( s < -EPSILON ) || ( s > 1.0f + EPSILON ) )
    {
        #ifdef ENABLE_ERROR_LOGGING
        getLogger() << LogLevel::Error << __FUNCTION__ << " :: [benni] Param s(" << s << ") is out of bounds [0,1]\n";
        #endif
        return false; // No intersection point between points P and Q
    }

    /// We got a valid line-line-intersection and both params (t,s) are in range [0,1], which means the line segments also intersect.
    return true;
}

bool
IntersectionTool2D::intersectLineSegmentAndLine( glm::vec2 & S, glm::vec2 const P, glm::vec2 const Q, glm::vec2 const A, glm::vec2 const B )
{
    float32_t t;
    float32_t s;
    if ( !intersectLineEx( S, P, Q, A, B, t, s ) )
    {
        return false;
    }

    // Check computed param t ( between P and Q )
    if ( ( t < -EPSILON ) || ( t > 1.0f + EPSILON ) )
    {
        #ifdef ENABLE_ERROR_LOGGING
        getLogger() << LogLevel::Error << __FUNCTION__ << " :: [benni] Param t(" << t << ") is out of bounds [0,1]\n";
        #endif
        return false; // No intersection point between points P and Q
    }

    /// We got a valid line-line-intersection and param t is in range [0,1], which means that line-segment PQ intersects ray AB.
    return true;
}

#if 0
bool
IntersectionTool::intersectHLine2D( glm::vec2 & S,
                                    glm::vec2 const & P, glm::vec2 const & Q,
                                    glm::vec2 const & A, glm::vec2 const & B )
{
#ifdef DTM_UTILS_DEBUG_LOGGING
   getLogger() << LogLevel::Debug << __FUNCTION__ << " :: "
               << "P(" << P.x << "," << P.y << "), Q(" << Q.x << "," << Q.y << "), "
               << "A(" << A.x << "," << A.y << "), B(" << B.x << "," << B.y << ")\n";
#endif

   // Compute denominator:
   // HLine means line AB is horizontal ( PQ is still arbitrary )
   // B.y - A.y = 0
   float32_t const n = ( A.x - B.x ) * ( Q.y - P.y );

   // Validate denominator:
   if ( std::abs( n ) <= std::numeric_limits< float32_t >::epsilon() )
   {
   #ifdef DTM_UTILS_DEBUG_LOGGING
      getLogger() << LogLevel::Debug << __FUNCTION__ << " :: n = " << n << " is too small\n";
   #endif
      return false; // No meaningful intersection point possible
   }

   // Compute numerator if denominator wasn't zero:
   float32_t const t = ( ( P.y - A.y ) * ( B.x - A.x ) ) / n;

   // Check computed param t ( between P and Q )
   if ( ( t < -std::numeric_limits< float32_t >::epsilon() ) ||
        ( t > 1.0f + std::numeric_limits< float32_t >::epsilon() ) )
   {
   #ifdef DTM_UTILS_DEBUG_LOGGING
      getLogger() << LogLevel::Debug << __FUNCTION__ << " :: param t = " << t << " out of bounds [0,1]\n";
   #endif
      return false; // No intersection point between points P and Q
   }

   // Return intersection point
   S.x = P.x + t * ( Q.x - P.x );
   S.y = P.y + t * ( Q.y - P.y );
   return true;
}


bool
IntersectionTool::intersectVLine2D( glm::vec2 & S,
                                    glm::vec2 const & P, glm::vec2 const & Q,
                                    glm::vec2 const & A, glm::vec2 const & B )
{
#ifdef DTM_UTILS_DEBUG_LOGGING
   getLogger() << LogLevel::Debug << __FUNCTION__ << " :: "
               << "P(" << P.x << "," << P.y << "), Q(" << Q.x << "," << Q.y << "), "
               << "A(" << A.x << "," << A.y << "), B(" << B.x << "," << B.y << ")\n";
#endif

   // Compute denominator:
   // VLine means line AB is vertical ( PQ is still arbitrary )
   // B.x - A.x = 0
   float32_t const n = ( B.y - A.y ) * ( Q.x - P.x );

   // Validate denominator:
   if ( std::abs( n ) <= std::numeric_limits< float32_t >::epsilon() )
   {
   #ifdef DTM_UTILS_DEBUG_LOGGING
      getLogger() << LogLevel::Debug << __FUNCTION__ << " :: n = " << n << " is too small\n";
   #endif
      return false; // No meaningful intersection point possible
   }

   // Compute numerator if denominator wasn't zero:
   float32_t const t = ( ( A.x - P.x ) * ( B.y - A.y ) ) / n;

   // Check computed param t ( between P and Q )
   if ( ( t < -std::numeric_limits< float32_t >::epsilon() ) ||
        ( t > 1.0f + std::numeric_limits< float32_t >::epsilon() ) )
   {
   #ifdef DTM_UTILS_DEBUG_LOGGING
      getLogger() << LogLevel::Debug << __FUNCTION__ << " :: param t = " << t << " out of bounds [0,1]\n";
   #endif
      return false; // No intersection point between points P and Q
   }

   // Return intersection point
   S.x = P.x + t * ( Q.x - P.x );
   S.y = P.y + t * ( Q.y - P.y );
   return true;
}

#endif


} // end namespace svg
} // end namespace hampe
} // end namespace de
