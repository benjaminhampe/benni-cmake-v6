#include <pioneers3d/types/Waypoint.hpp>
#include <pioneers3d/types/Game.hpp>

std::ostream &
operator<< ( std::ostream & o, pioneers3d::Waypoint_t const & w )
{
    o << "Enabled(" << w.IsEnabled << "), "
      << "Road(" << w.IsRoad << "), "
      << "Player: "<< w.Player << "; "
      << "Pos(" << w.Pos << "), "
      << "Tiles(" << w.Tiles.size() << "), "
      << "VictoryPoints(" << w.VictoryPoints << "), "
      << "Angle(" << w.Angle << ")";
    return o;
}

namespace pioneers3d {

int32_t
Waypoint_t::addTile( Tile_t * tile )
{
   if ( !tile )
   {
      return -1;
   }

   for ( size_t i = 0; i < Tiles.size(); ++i )
   {
      Tile_t* t = Tiles[ i ];
      if ( t == tile )
      {
         return int32_t( i ); // break; // Already there
      }
   }

   Tiles.emplace_back( tile );
   return int32_t( Tiles.size() ) - 1;
}

int32_t Waypoint_findIndex( Game_t* game, Waypoint_t* w )
{
   for ( size_t i = 0; i < game->Board.Waypoints.size(); ++i )
   {
      Waypoint_t const & store = game->Board.Waypoints[ i ];
      if ( &store == w )
      {
         return int32_t( i );
      }
   }
   return -1;
}

} // end namespace pioneers3d
