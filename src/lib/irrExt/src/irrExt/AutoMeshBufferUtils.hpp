#ifndef DE_ALPHASONIC_AUTO_MESH_BUFFER_UTILS_HPP
#define DE_ALPHASONIC_AUTO_MESH_BUFFER_UTILS_HPP

#include <irrExt/AutoMeshBuffer.hpp>
#include <irrExt/sinCosTable.hpp>

namespace irrExt {

glm::vec3
getNormal( glm::vec3 A, glm::vec3 B, glm::vec3 C );

/// @brief Translate all vertices by a vector
void
translate( AutoMeshBuffer & p, glm::vec3 const & offset );

/// @brief Rotate all vertices by a vector
void
rotate( AutoMeshBuffer & p, glm::vec3 const & degrees );

AutoMeshBuffer*
createRect( glm::vec3 pos, glm::vec2 size, uint32_t color );

/// @brief Create a box with one repeating texture over all 6 planes
AutoMeshBuffer *
createBox( glm::vec3 const & pos, glm::vec3 const & size, uint32_t color );

/// @brief Create a box with one repeating texture over all 6 planes
AutoMeshBuffer *
createRotatedBox( glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );

#if 0

/// @brief Create a dice mesh ( box ) with 6 mesh-buffers and 6 textures
AutoMesh *
createDice( glm::vec3 const & size, uint32_t color );

#endif 

/// Hexagon ( ver_2018 ):
/**
                 D        triangles: ABF, BCE, BEF, CDE
                / \
              C/   \ E    M (x=0,y=0,z=0,u=0.5,v=0.5)
               |----|     F( 0.5, 0, -.25, 1, 0.25 )
               |  M |     E( 0.5, 0, 0.25, 1, 0.75 )
               |----|     D( 0.0, 0, 0.50, 0.5, 1 )
              B \  / F    C( -.5, 0, 0.25, 0, 0.75 )
                 \/       B( -.5, 0, -.25, 0, 0.25 )
                  A       A( 0, 0, -.5, .5, 0 )
*/

glm::vec3
getHexagonPoint( float w, float h, int i );

AutoMeshBuffer*
createHexagon( float w, float h );

AutoMeshBuffer*
createHafenXZ( float w, float h, int i );

AutoMeshBuffer*
createCircle( glm::vec3 const & pos, float r, uint32_t segments = 36, uint32_t color = 0xFFFFFFFF );

AutoMeshBuffer*
createCircleXZ( glm::vec3 const & pos, float r, uint32_t segments );

AutoMeshBuffer*
createCylinderHull(
    glm::vec3 pos,
    float radius,
    float height,
    uint32_t color,
    uint32_t tessCircle = 33,
    uint32_t tessHull = 1);

uint32_t
getVertexCount(
   AutoMeshBuffer & buffer );

uint32_t
getIndexCount(
   AutoMeshBuffer & buffer );

void
addVertex(
   AutoMeshBuffer & buffer,
   glm::vec3 const & p,
   glm::vec3 const & n,
   uint32_t color,
   glm::vec2 const & uv,
   bool flipV = true );

void
addIndex(
   AutoMeshBuffer & buffer,
   uint32_t index );

void
addIndexedTriangle(
   AutoMeshBuffer & buffer,
   uint32_t indexA,
   uint32_t indexB,
   uint32_t indexC );

void
addTriangle(
   AutoMeshBuffer & buffer,
   glm::vec3 const & A,
   glm::vec3 const & B,
   glm::vec3 const & C,
   uint32_t color,
   glm::vec2 const & uvA,
   glm::vec2 const & uvB,
   glm::vec2 const & uvC,
   bool flipV = true );

void
addTriangle(
   AutoMeshBuffer & buffer,
   glm::vec3 const & A,
   glm::vec3 const & B,
   glm::vec3 const & C,
   glm::vec3 const & n,
   uint32_t color,
   glm::vec2 const & uvA,
   glm::vec2 const & uvB,
   glm::vec2 const & uvC,
   bool flipV = true );

void
addQuad(
   AutoMeshBuffer & buffer,
   glm::vec3 const & A,
   glm::vec3 const & B,
   glm::vec3 const & C,
   glm::vec3 const & D,
   uint32_t color,
   bool flipV = true );

//  F  G
// /E /H
// B--C
// |/ |
// A--D

/// @brief Create a box with one repeating texture over all 6 planes
void
addBox(
   AutoMeshBuffer & p,
   glm::vec3 const & pos,
   glm::vec3 const & size,
   uint32_t color );

/// @brief Create a box with one repeating texture over all 6 planes
void
addBoxPlane( AutoMeshBuffer & p, int i, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color );

/// @brief Create a box
void
addBoxWithoutTop( AutoMeshBuffer & p, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color );

/// @brief Create a box
void
addBoxWithoutTopAndBottom( AutoMeshBuffer & p, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color );

} // end namespace irrExt

#endif // DE_ALPHASONIC_AUTO_MESH_BUFFER_HPP
