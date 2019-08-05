#ifndef DIRTYLINEGEOMETRYPROCESSOR_HPP
#define DIRTYLINEGEOMETRYPROCESSOR_HPP

#include <de/hampe/polyline/PrimitiveRenderer.hpp>

class LineGeometryProcessor
{
public:
    static void
    asTriangles(
            PrimitiveRenderer & dc,
            std::vector< glm::vec2 > const & points,
            float32_t const lineWidth );

    static void
    asTriangleList(
            PrimitiveRenderer & dc,
            std::vector< glm::vec2 > const & points,
            float32_t const lineWidth );
};


#endif // DIRTYLINEGEOMETRYPROCESSOR_HPP
