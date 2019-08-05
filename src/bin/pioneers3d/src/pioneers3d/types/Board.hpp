#ifndef PIONEERS3D_GAME_BOARD_HPP
#define PIONEERS3D_GAME_BOARD_HPP

#include <pioneers3d/types/Common.hpp>
#include <pioneers3d/types/Tile.hpp>
#include <pioneers3d/types/BoardObjects.hpp>

namespace pioneers3d {

typedef uint32_t BoardType;

class Game_t;

class Board_t
{
public:
   Game_t*                 Game;
   std::string             Name;
   BoardType               Type;
   Bank_t                  Bank;
   glm::ivec2              TileCount;
   glm::vec3               TileSize;
   std::vector< Tile_t >   Tiles;
   std::vector< Waypoint_t > Waypoints;

   //    Tile_t*                 SelectedTile = nullptr;
   //    bool                    EnableCollisionDetectionForTiles = true;
   //    Waypoint_t*             SelectedWaypointR = nullptr;
   //    Waypoint_t*             SelectedWaypointS = nullptr;
   //    bool                    EnableCollisionDetectionForWaypoints = true;

   Raueber_t               Raeuber;
   AutoSceneNode*          Object = nullptr;

   Board_t();
   ~Board_t();

   void
   addWaypoint( float32_t r,float32_t h, uint32_t tesselation, glm::vec3 pos, float32_t angle, bool isRoad );

   std::vector< Tile_t const * >
   getTilesByValue( int32_t diceValue ) const
   {
       std::vector< Tile_t const * > container;
       for ( size_t i = 0; i < Tiles.size(); ++i )
       {
         Tile_t const & tile = Tiles[ i ];
         if ( tile.m_DiceValue == diceValue )
         {
            container.emplace_back( &tile );
         }
       }
       return container;
   }

   void clear()
   {
      clearWaypoints();
      clearTiles();
   }

   void clearTiles()
   {
      for ( size_t i = 0; i < Tiles.size(); ++i )
      {
         Tile_t & tile = Tiles[ i ];
         if ( tile.m_Node )
         {
            tile.m_Node->drop();
            tile.m_Node = nullptr;
         }
      }
      Tiles.clear();
   }

   void clearWaypoints()
   {
      for ( size_t i = 0; i < Waypoints.size(); ++i )
      {
         Waypoint_t & waypoint = Waypoints[ i ];
         if ( waypoint.Node )
         {
            waypoint.Node->drop();
            waypoint.Node = nullptr;
         }
      }
      Waypoints.clear();
   }

   Waypoint_t*
   findWaypoint( glm::vec3 pos )
   {
      for ( uint32_t i = 0; i < Waypoints.size(); ++i )
      {
         Waypoint_t & w = Waypoints[ i ];
         if ( equals( w.Pos, pos ) )
         {
            //std::cout << __FUNCTION__ << " [Found] :: Waypoint(" << i << ") at pos(" << toString(pos) << ")\n";
            return &w;
         }
      }
      return nullptr;
   }

   std::vector< Tile_t* >
   findTiles( glm::vec3 const & pos )
   {
      std::vector< Tile_t* > container;

      for ( size_t i = 0; i < Tiles.size(); ++i )
      {
         Tile_t & t = Tiles[ i ];
         if ( equals( t.m_Pos, pos ) )
         {
            container.emplace_back( &t );
         }
      }

      return container;
   }

   void forEachTile( std::function<void(Tile_t * t)> const & lambda )
   {
      for ( size_t i = 0; i < Tiles.size(); ++i )
      {
         lambda( &Tiles[i] );
      }
   }

   void forEachWaypoint( std::function<void(Waypoint_t * w)> const & lambda )
   {
      for ( size_t i = 0; i < Waypoints.size(); ++i )
      {
         lambda( &Waypoints[i] );
      }
   }

   void forEachWaypointR( std::function<void(Waypoint_t * w)> const & lambda )
   {
      for ( size_t i = 0; i < Waypoints.size(); ++i )
      {
         if ( Waypoints[ i ].IsRoad )
         {
            lambda( &Waypoints[i] );
         }
      }
   }

   void forEachWaypointS( std::function<void(Waypoint_t * w)> const & lambda )
   {
      for ( size_t i = 0; i < Waypoints.size(); ++i )
      {
         if ( !Waypoints[ i ].IsRoad )
         {
            lambda( &Waypoints[i] );
         }
      }
   }

};

Board_t*    Board_get( Game_t * game );
inline std::string Board_toString( Game_t * game );
inline void Board_clear( Game_t * game ) { Board_get( game )->clear(); }
inline void Board_clearTiles( Game_t* game ) { Board_get( game )->clearTiles(); }
inline void Board_clearWaypoints( Game_t* game ) { Board_get( game )->clearWaypoints(); }
inline Waypoint_t* Board_findWaypoint( Game_t * game, glm::vec3 const & pos ) { return Board_get( game )->findWaypoint( pos ); }
inline std::vector< Tile_t* > Board_findTiles( Game_t * game, glm::vec3 const & pos ) { return Board_get( game )->findTiles( pos ); }
inline void forEachTile( Game_t * game, std::function<void(Tile_t * t)> const & lambda ) { Board_get( game )->forEachTile( lambda ); }
inline void forEachWaypoint(  Game_t * game, std::function<void(Waypoint_t * w)> const & lambda ) { Board_get( game )->forEachWaypoint( lambda ); }
inline void forEachWaypointR( Game_t * game, std::function<void(Waypoint_t * w)> const & lambda ) { Board_get( game )->forEachWaypointR( lambda ); }
inline void forEachWaypointS( Game_t * game, std::function<void(Waypoint_t * w)> const & lambda ) { Board_get( game )->forEachWaypointS( lambda ); }

// ---------------------------------------------------------------------------------------
void        Board_createStandard( Game_t * game );
void        Board_addTile( Game_t * game, TileType tileType, int i, int j, int diceValue, int angle60 );
void        Board_addWaypoint( Game_t * game, float32_t r, float32_t h, uint32_t tesselation, glm::vec3 pos, float32_t angle, bool isRoad = false );
void        Board_createWaypoints( Game_t * game, float32_t radius, float32_t height, uint32_t tesselation, bool isRoad = false );
Tile_t *    Board_isMouseOverTile( Game_t * game );
Waypoint_t* Board_isMouseOverWaypoint( Game_t * game, bool isRoad );
//          Board_collectTilesByPosition( Game_t * game, glm::vec3 pos );
//void      Board_setVisible_Tiles( Game_t* game, bool visible );
//void      Board_setVisible_Tiles( Game_t* game, bool visible, TileType filter );
void        Board_setVisible_Waypoints( Game_t* game, bool visible );
void        Board_setVisible_WaypointsR( Game_t* game, bool visible );
void        Board_setVisible_WaypointsS( Game_t* game, bool visible );
inline Waypoint_t* Board_isMouseOverWaypointR( Game_t * game ) { return Board_isMouseOverWaypoint( game, true ); }
inline Waypoint_t* Board_isMouseOverWaypointS( Game_t * game ) { return Board_isMouseOverWaypoint( game, false ); }
inline void Board_showWaypoints( Game_t* game ) { Board_setVisible_Waypoints( game, true ); }
inline void Board_showWaypointsR( Game_t* game ) { Board_setVisible_WaypointsR( game, true ); }
inline void Board_showWaypointsS( Game_t* game ) { Board_setVisible_WaypointsS( game, true ); }
inline void Board_hideWaypoints( Game_t* game ) { Board_setVisible_Waypoints( game, false ); }
inline void Board_hideWaypointsR( Game_t* game ) { Board_setVisible_WaypointsR( game, false ); }
inline void Board_hideWaypointsS( Game_t* game ) { Board_setVisible_WaypointsS( game, false ); }

} // end namespace pioneers3d

std::ostream &
operator<< ( std::ostream & o, pioneers3d::Board_t const & b );


#endif // PIONEERS3D_STANDARD_GAME_HPP
