#include <pioneers3d/types/Tile.hpp>
#include <pioneers3d/types/Game.hpp>

std::ostream &
operator<< ( std::ostream & o, pioneers3d::Tile_t const & t )
{
    o << "type: " << t.m_Type.toString() << "; "
      << "i: "<< t.m_Index.x << "; "
      << "j: "<< t.m_Index.y << "; "
      << "x: "<< t.m_Pos.x << "; "
      << "y: "<< t.m_Pos.y << "; "
      << "z: "<< t.m_Pos.z << "; "
      << "dice: "<< t.m_DiceValue << "; "
      << "ang60: "<< t.m_Angle60 << "; "
      << "waypoints: "<< t.m_Waypoints.size() << "; "
      << "";
    return o;
}

namespace pioneers3d {

Tile_t::Tile_t()
   : m_Board( nullptr )
   , m_Type( TileType::WASSER )
   , m_DiceValue( 0 )
   , m_Angle60( 0 ) // in range [0...5]
   , m_Waypoints{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }  // Corners of the graph
   , m_Tiles{ nullptr, nullptr, nullptr } // Neighbours
   //, m_Roads;      // (Full) Edges of the graph
   , m_Triangles()
   , m_Node( nullptr )
   , m_TriangleSelector( nullptr )
{

}

Tile_t::~Tile_t()
{
   // if (Node) Node->drop();
}


int32_t
Tile_t::getPlayerPoints( int playerIndex ) const
{
   int32_t victoryPoints = 0;

   for ( size_t i = 0; i < m_Waypoints.size(); ++i )
   {
      Waypoint_t const * const w = m_Waypoints[ i ];
      if ( w && w->Player == playerIndex )
      {
         victoryPoints += w->VictoryPoints;
      }
   }

   return victoryPoints;
}

} // end namespace pioneers3d
