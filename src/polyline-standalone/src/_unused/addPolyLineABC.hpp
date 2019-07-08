#ifndef ADDPOLYLINEABC_HPP
#define ADDPOLYLINEABC_HPP

#include <de/hampe/polyline/PolyLineCommon.hpp>
#include <de/hampe/polyline/LineJoin.hpp>
#include <de/hampe/polyline/LineCap.hpp>
#include <de/hampe/polyline/PrimitiveRenderer.hpp>

//
// Create PolyLine ABC
//
// AL---------------BL\-----SBL
// |        --------|  \   /
// |--------        |    \/
// A----------------B----BL
// |----        -  /    /
// |    ----  -   /    /
// AR-------SBR  /    /
//         /    /    /
//        /    /    /
//       /    /    /
//      /    /    /
//     CR---C----CL
//
bool addPolyLineABC(    PrimitiveRenderer & dc,
                        glm::vec2 const A,
                        glm::vec2 const B,
                        glm::vec2 const C,
                        float32_t const lineWidth,
                        LineJoin const lineJoin = LineJoin::BEVEL );

#endif // ADDPOLYLINEABC_HPP
