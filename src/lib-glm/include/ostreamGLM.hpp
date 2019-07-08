/// @author Benjamin Hampe <benjamin.hampe@gmx.de>

#ifndef DE_HAMPE_COMMON_OSTREAMGLM_HPP
#define DE_HAMPE_COMMON_OSTREAMGLM_HPP

#ifdef USE_LIB_GLM
#include <glm/glm.hpp>
#include <ostream>

// ------------------------------------------------------------
// --- Print GLM Types --- For my developer convenience
// ------------------------------------------------------------

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec1<T,P> const & v )
{
    out << v.x;
    return out;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec2<T,P> const & v )
{
    out << v.x << "," << v.y;
    return out;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec3<T,P> const & v )
{
    out << v.x << "," << v.y << "," << v.z;
    return out;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec4<T,P> const & v )
{
    out << v.x << "," << v.y << "," << v.z << "," << v.w;
    return out;
}

#endif // USE_LIB_GLM

#endif // DE_HAMPE_COMMON_OSTREAMGLM_HPP
