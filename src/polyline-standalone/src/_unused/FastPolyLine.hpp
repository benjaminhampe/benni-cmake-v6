#ifndef FASTPOLYLINE_HPP
#define FASTPOLYLINE_HPP

#include <de/hampe/polyline/PrimitiveRenderer.hpp>

void addFastBeveledPolyLineAsTriangleList(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth,
        uint32_t const color, // Only for debugging, TODO: remove in release mode
        uint32_t const colorL, // Only for debugging, TODO: remove in release mode
        uint32_t const colorR // Only for debugging, TODO: remove in release mode
);

void addFastBeveledPolyLineAsTriangles(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth,
        uint32_t const color // Only for debugging, TODO: remove in release mode
        );

#endif // FASTPOLYLINE_HPP
