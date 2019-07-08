#ifndef POLYLINE_nHPP
#define POLYLINE_nHPP

#include "PolyLineCommon.hpp"
#include "LineJoin.hpp"
#include "LineCap.hpp"
#include "PrimitiveRenderer.hpp"

///
/// Create PolyLine ABCD
//
//  L0---------------L1-------S1   Triangles: CW
//  |        --------|  \    /         Start-Segment:
//  |--------        |    \ /                  1.) A, AL,B2    P0, P0L, P1E1   P(k)[0], P(k)[1], P(k+1)E1
//  P0---------------P1----K1                  2.) A, B2,B     P0, P1E1, P1    P(k)[0], P(k+1)[2],
//  |       ----- - /|    /                    3.) A, B, AR    P0, P1, P0R
//  | -----    -   / |   /                     4.) AR,B, B1    P0R, P1, P1SR
//  R0------R1    /  |  /
//         /|    /   | /               Middle-Segment:
//        / |   /    |/                        1.) B, B3, C1   P1, P1E2, P2SL
//       /  |  /     L2----------DL            2.) B, C1, C    P1, P2SL, P2
//      /   | /   -       ------ |             3.) B, C, B1    P1, P2, P1SR
//     /    |/  -  ------        |             4.) B1, C, C2   P1SR, P2, P2E1
//    R2----P2-------------------P3
//   /  \   |----------          |     End-Segment:
//  /     \ |          ----------|             1.) C, C1, DL   P2, P2SR, P3L
// C4-------C3-------------------DR            2.) C, DL, D    P2, P3L, P3
//                                             3.) C, D, DR    P2, P3, P3R
//                                             4.) C, DR, C3   P2, P3R, P2E2
void addPolyLine(
        PrimitiveRenderer & dc,
        std::vector< glm::vec2 > const & points,
        float32_t const lineWidth,
        LineJoin const lineJoin,
        LineCap const lineCap );

#endif // POLYLINE_HPP
