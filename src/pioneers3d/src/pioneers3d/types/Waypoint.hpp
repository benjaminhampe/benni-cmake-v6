/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_WAYPOINT_HPP
#define PIONEERS3D_TYPES_WAYPOINT_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

class Game_t;
class Board_t;
class Player_t;
class Tile_t;

class Waypoint_t
{
public:
   Board_t*                Board = nullptr;
   int32_t                 Player = -1;   // Owner of the waypoint, use Player_t* Player_getByIndex( game, Player ) to get pointer to data.
   int                     VictoryPoints = 0; // default: none = 0, settlement = 1, city = 2, bespin sky city = 3
   bool                    IsRoad = false; // Default: false == can settle, true == only a road
   bool                    IsEnabled = true; // Can generate ressource cards ( no thief/robber )
   glm::vec3               Pos;
   AutoSceneNode*          Node = nullptr;

   std::vector< Tile_t* >  Tiles; // collect neighbouring tiles 1-3
   float32_t               Angle = 0;

   AutoSceneNode*          OwnerNode = nullptr;
public:
   //Waypoint_t() {}
   //~Waypoint_t() { if (Node) Node->drop(); }

   int32_t addTile( Tile_t * tile );
};

int32_t Waypoint_findIndex( Game_t* game, Waypoint_t* w );

} // end namespace pioneers3d

std::ostream &
operator<< ( std::ostream & o, pioneers3d::Waypoint_t const & w );

inline std::ostream &
operator<< ( std::ostream & o, std::vector< pioneers3d::Waypoint_t > const & waypoints )
{
   o << "Waypoints.size() = " << waypoints.size() << "\n";
   for ( size_t i = 0; i < waypoints.size(); ++i )
   {
      o << "Waypoint[" << i << "] " << waypoints[ i ] << "\n";
   }
   return o;
}

#endif // PIONEERS3D_TYPES_WAYPOINT_HPP
