/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_HEXAGON_TILE_HPP
#define PIONEERS3D_TYPES_HEXAGON_TILE_HPP

#include <pioneers3d/types/Common.hpp>
#include <pioneers3d/types/TileType.hpp>
#include <pioneers3d/types/Bank.hpp>
#include <pioneers3d/types/Waypoint.hpp>
#include <array>

namespace pioneers3d {

/// Hexagon ( ver_2018 ):
/**
        D        triangles: ABF, BCE, BEF, CDE
        /\
     C /  \ E    M (x=0,y=0,z=0,u=0.5,v=0.5)
      |----|     F(  .5, 0, -.25,  1, 0.25 )
      | +M |     E(  .5, 0,  .25,  1, 0.75 )
      |----|     D(   0, 0,  .50, .5, 1.00 )
     B \  / F    C( -.5, 0,  .25,  0, 0.75 )
        \/       B( -.5, 0, -.25,  0, 0.25 )
        |        A(   0, 0, -.50, .5, 0.00 )
        |  +
        |  = 0.75 h || 0 h
         \
          \
*/
///@brief A geometry hexagonal tile class for basic hexagonal tile stuff
class Hexagon_t
{
public:
//   float const x = (0.50f * dx) * ( std::abs(j) % 2 ) + dx * i;
//   float const y = 0.0f;
//   float const z = (0.75f * dz) * boardIndex.y;
   static glm::vec3
   computePositionFromBoardIndex( glm::vec3 const & tileSize,
      glm::ivec2 const & index, float32_t const y = 0.0f )
   {
      float32_t x = (0.50f * tileSize.x) * ( std::abs(index.y) % 2 ) + tileSize.x * index.x;
      float32_t z = (0.75f * tileSize.z) * index.y;
      return glm::vec3( x, y, z );
   }

   /// @brief Get texcoords for all corners to render
   static std::array< glm::vec2, 6 > const &
   getTexCoords()
   {
      static std::array< glm::vec2, 6 > const s_TexCoords = {
         glm::vec2( .5f, 0.f ), // A
         glm::vec2( 0.f, .25f ), // B
         glm::vec2( 0.f, .75f ), // C
         glm::vec2( .5f, 1.f ), // D
         glm::vec2( 1.f, .75f ), // E
         glm::vec2( 1.f, .25f ) // F
      };
      return s_TexCoords;
   }

   /// @brief Get raw corner (a scale vector) of a hexagonal tile
   // Use: cornerPos[i] = scale[i] * tile_size + tile_pos to
   static std::array< glm::vec3, 6 > const &
   getCorners()
   {
      static std::array< glm::vec3, 6 > const s_RawCorners = {
         glm::vec3(  .0f, 0.0f, -.50f ), // A
         glm::vec3( -.5f, 0.0f, -.25f ), // B
         glm::vec3( -.5f, 0.0f,  .25f ), // C
         glm::vec3(  .0f, 0.0f,  .50f ), // D
         glm::vec3(  .5f, 0.0f,  .25f ), // E
         glm::vec3(  .5f, 0.0f, -.25f )  // F
      };
      return s_RawCorners;
   }

   /// @brief Get raw corner (a scale vector) for a specific point of a hexagonal tile
   // Use: cornerPos[i] = scale[i] * tile_size + tile_pos to
   static glm::vec2 const &
   getTexCoord( size_t i )
   {
      assert( i < 6 );
      return getTexCoords()[ i ];
   }


   /// @brief Get raw corner (a scale vector) for a specific point of a hexagonal tile
   // Use: cornerPos[i] = scale[i] * tile_size + tile_pos to
   static glm::vec3 const &
   getCorner( size_t i )
   {
      assert( i < 6 );
      return getCorners()[ i ];
   }

   /// @brief Get raw corner (a scale vector) for a specific point of a hexagonal tile
   // Use: cornerPos[i] = scale[i] * tile_size + tile_pos to
   static glm::vec3
   getCorner( size_t i, glm::vec3 const & size, glm::vec3 const & pos = glm::vec3( 0.f, 0.f, 0.f ) )
   {
      assert( i < 6 );
      return getCorner( i ) * size + pos;
   }

   /// @brief Get 3d pos (x,y,z) of a corner, index must be in range [0,5], or dinosaurs eat you
   /// The edge center is used for Waypoints R ( Road ) and finding neighbour tiles aswell ( even better )
   static glm::vec3
   getEdgeCenter( size_t const i )
   {
      size_t const a = i % 6; // go round
      size_t const b = ( i + 1 ) % 6; // and round
      glm::vec3 const & A = getCorner( a );
      glm::vec3 const & B = getCorner( b );
      return A + ( B - A ) * 0.5f; // Center of egde (a,b)
   }

   /// @brief Get 1d angle of an egde, starting with south/south-west edge AB=0, index must be in range [0,5]
   /// The edge angle is used for placing Roads on Waypoints R ( Road )
   /// TODO just precompute this stuff for 6 edges
   static float32_t
   getEdgeAngle( size_t const i )
   {
      size_t const a = i % 6; // go round
      size_t const b = ( i + 1 ) % 6; // and round
      glm::vec3 const & A = getCorner( a );
      glm::vec3 const & B = getCorner( b );
      glm::vec3 eAB( B.x-A.x, 0.0f, B.z-A.z); // projected onto xz plane
      glm::vec3 eX(1,0,0); // we want angle between +x axis
      // dot(a,b) = |a||b|cos(a,b);
      // cos(a,b) = dot(a,b) / |a||b|;
      // angle(a,b) = acos( dot(a,b) / |a||b| );
      float32_t angle = irr::core::RADTODEG * acos( glm::dot(eAB,eX) /* / glm::length(eAB) */ );
      while ( angle < 0.0f ) angle += 360.0f;
      while ( angle >= 360.0f ) angle -= 360.0f;
      std::cout << __FUNCTION__ << "(" << i << ") -> a60(" << angle << ")\n";
      return angle;
   }

   /// @brief Create irrlicht triangles for collision detection and engine stuff like rendering
   static std::vector< irr::core::triangle3df >
   getCollisionTriangles( glm::vec3 const & size, glm::vec3 const & pos = glm::vec3( 0.f, 0.f, 0.f ) )
   {
      glm::vec3 const A = getCorner( 0, size, pos );
      glm::vec3 const B = getCorner( 1, size, pos );
      glm::vec3 const C = getCorner( 2, size, pos );
      glm::vec3 const D = getCorner( 3, size, pos );
      glm::vec3 const E = getCorner( 4, size, pos );
      glm::vec3 const F = getCorner( 5, size, pos );

      std::vector< irr::core::triangle3df > out;
      out.reserve( 4 );

      auto addTriangle = [ &out ] ( glm::vec3 const & a, glm::vec3 const & b, glm::vec3 const & c )
      {
         out.emplace_back( irr::core::triangle3df( irr::core::vector3df( a.x, a.y, a.z), irr::core::vector3df( b.x, b.y, b.z ), irr::core::vector3df( c.x, c.y, c.z ) ) );
      };

      addTriangle( A, B, F ); // ABF - 015
      addTriangle( B, C, E ); // BCE - 124
      addTriangle( B, E, F ); // BEF - 145
      addTriangle( C, D, E ); // CDE - 234

      return out;
   }

   static void
   addIndexedTriangles( irr::scene::SMeshBuffer * o, glm::vec3 size, glm::vec3 pos = glm::vec3( 0.f, 0.f, 0.f ) )
   {
      // Append:
      auto addVertex = [ o ] ( glm::vec3 const & p, glm::vec2 tex, bool flipV )
      {
         o->Vertices.push_back( irr::video::S3DVertex( p.x, p.y, p.z,
            0, 0, -1, 0xFFFFFFEE, tex.x, (flipV) ? 1.0f - tex.y : tex.y ) );
      };

      auto addTriangle = [ o ] ( uint16_t a, uint16_t b, uint16_t c )
      {
        o->Indices.push_back( a );
        o->Indices.push_back( b );
        o->Indices.push_back( c );
      };

      for ( size_t i = 0; i < 6; ++i )
      {
         addVertex( getCorner( i ) * size + pos, getTexCoord( i ), true ); // A..F = 0..5
      }

      // Indices ( triangles: ABF, BCE, BEF, CDE ):
      addTriangle( 0,1,5 ); // ABF
      addTriangle( 1,2,4 ); // BCE
      addTriangle( 1,4,5 ); // BEF
      addTriangle( 2,3,4 ); // CDE
   }

   static irr::scene::SMeshBuffer*
   createMeshBuffer( glm::vec3 size, glm::vec3 pos = glm::vec3( 0.f, 0.f, 0.f ) )
   {
      irr::scene::SMeshBuffer * o = new irr::scene::SMeshBuffer();
      o->Vertices.reallocate( 6 );  // 6 corners
      o->Vertices.set_used( 0 );
      o->Indices.reallocate( 12 );  // 4 triangles
      o->Indices.set_used( 0 );
      addIndexedTriangles( o, size, pos );
      o->recalculateBoundingBox();
      // o->Material.MaterialType = irr::video::EMT_SOLID;
      o->Material.Wireframe = false;
      o->Material.Lighting = false;
      o->Material.FogEnable = false;
      //o->Material.setTexture( 0, stage0 );
      return o;
   }

   ///@brief
   static AutoMeshBuffer*
   createAutoMeshBuffer( glm::vec3 size, glm::vec3 pos = glm::vec3( 0.f, 0.f, 0.f ) )
   {
      // MeshBuffer
      AutoMeshBuffer* p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );
      addIndexedTriangles( &p->MeshBuffer, size, pos );
      return p;
   }

public:
};

///@brief
class Tile_t : public Hexagon_t
{
public:
   ///@brief
   Tile_t();
   ///@brief
   ~Tile_t();

   void updatePosition()
   {
      m_Pos = computePositionFromBoardIndex( m_Size, m_Index, 0.0f );
   }

   void
   setSize( glm::vec3 const & size )
   {
      m_Size = size;
   }

   void
   setBoardIndex( glm::ivec2 const & index )
   {
      m_Index = index;
   }

   ///@brief
   int32_t
   getPlayerPoints( int playerIndex ) const;

//   void
//   setPosition( glm::vec3 const & size, glm::ivec2 const & index )
//   {
//      m_Size = size;
//      m_BoardIndex = index;
//      updatePosition();
//   }


//   /// @brief Get 2d index (i,j) on a hexagonal board
//   glm::ivec2 const &
//   getIndex() const { return m_Index; }

//   /// @brief Get 3d size (w,d,h) on a hexagonal board laying on XZ plane
//   glm::vec3 const &
//   getSize() const { return m_Size; }

//   /// @brief Get 3d pos (x,y,z) in space, depends on size and board-index
//   glm::vec3 const &
//   getPos() const { return m_Pos; }

//   /// @brief Get 3d pos (x,y,z) of a corner, index must be in range [0,5], or dinosaurs eat you
//   /// The position is used for Waypoints S ( Settlement ) and finding neighbour tiles
//   glm::vec3 const &
//   getCorner( size_t const i ) const
//   {
//      assert( i < 6 ); // replace dinosaur with a bomb
//      return m_Corners[ i ]; // the cat or the bomb
//   }

public:
   ///@brief The board parent
   Board_t* m_Board = nullptr;
   ///@brief The 2d board index on the parent
   glm::ivec2 m_Index;
   ///@brief The 3d size of the tile ( const for board )
   glm::vec3 m_Size;
   ///@brief The 3d pos of the tile ( computed by size and board index )
   glm::vec3 m_Pos;
   ///@brief
   TileType m_Type = TileType::WASSER;
   ///@brief
   int32_t m_DiceValue = 0;
   ///@brief
   int32_t m_Angle60 = 0;    // in range [0...5]
//   ///@brief The 3d positions of the tile corners, beginning with A=0 in the south to F=5 east-south, @see Pioneers Hexagon
//   std::array< glm::vec3, 6 > m_Corners;
   ///@brief The 3d position of the tile center, computed by tile-size and board-index
   //std::array< glm::vec3, 6 > m_EdgeCenters;
   ///@brief
   std::array< Waypoint_t*, 6 > m_Waypoints;  // Corners of the graph
   ///@brief
   std::array< Tile_t*, 6 > m_Tiles;      // Neighbours
   ///@brief
   //std::array< Road_t*, 6 > m_Roads;      // (Full) Edges of the graph
   ///@brief Irrlicht stuff
   std::vector< irr::core::triangle3df > m_Triangles;
   ///@brief
   AutoSceneNode* m_Node = nullptr;
   ///@brief
   irr::scene::ITriangleSelector* m_TriangleSelector = nullptr;

};

} // end namespace pioneers3d

///@brief
std::ostream &
operator<< ( std::ostream & o, pioneers3d::Tile_t const & t );

#endif // PIONEERS3D_TYPES_ROBBER_HPP
