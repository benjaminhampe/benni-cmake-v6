/// (c) 2017 - 2018 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_GAME_HPP
#define PIONEERS3D_TYPES_GAME_HPP

#include <pioneers3d/types/Common.hpp>
#include <pioneers3d/types/GameType.hpp>
#include <pioneers3d/types/GameState.hpp>
#include <pioneers3d/types/EventReceiver.hpp>
#include <pioneers3d/types/Texture.hpp>
#include <pioneers3d/types/Font.hpp>
#include <pioneers3d/types/Marquee.hpp>
#include <pioneers3d/types/CylinderCamera.hpp>
#include <pioneers3d/types/Dice.hpp>
#include <pioneers3d/types/Card.hpp>
#include <pioneers3d/types/PlaceObjectType.hpp>
#include <pioneers3d/types/Board.hpp>
#include <pioneers3d/types/Player.hpp>
#include <pioneers3d/types/Action.hpp>
#include <pioneers3d/types/Chat.hpp>
#include <pioneers3d/types/UI.hpp>

// ---------------------------------------------------------------------------------------

namespace pioneers3d {

class Game_t
{
public:
   std::string             Name;
   irr::IrrlichtDevice*    Device = nullptr;
   irr::video::IVideoDriver* Driver = nullptr;
   irr::IEventReceiver*    Receiver = nullptr;
   irr::video::SColor      ClearColor;
   std::string             ExeFile;
   std::string             ExeDir;
   std::string             MediaDir;
   std::string             FontFileName;
   FontManager_t           FontManager;

   struct TextCollection_t
   {
      Marquee_t            Marquee;
      Text_t               H1;
      Text_t               H2;
      Text_t               H3;
      Text_t               H4;
      Text_t               H5;
      Text_t               H6;
   };

   TextCollection_t        Text;
   void*                   Chat;
   GameType                Type;
   GameState               State;
   uint32_t                Round = 0;
   Dice_t                  Dice;
   std::vector< Player_t > Players;
   int32_t                 Player = 0;
   Board_t                 Board;
   Tile_t*                 SelectedTile = nullptr;
   bool                    EnableCollisionDetectionForTiles = true;
   Waypoint_t*             SelectedWaypointR = nullptr;
   Waypoint_t*             SelectedWaypointS = nullptr;
   bool                    EnableCollisionDetectionForWaypoints = true;
   Raueber_t               Raeuber;
   ePlaceObjectType        PlaceObjectType = ePlaceObjectType::ROBBER;
   AutoSceneNode*          PlaceObject = nullptr;
   CylinderCamera*         Camera;
   GameUI_t                UI;
public:
   Game_t() {}
   ~Game_t() {}
};

///
/// pioneers3d.MainApp
///

glm::ivec2
Game_getScreenSize( Game_t * game );

irr::IrrlichtDevice*
Game_getDevice( Game_t * game );

irr::video::IVideoDriver*
Game_getVideoDriver( Game_t * game );

irr::gui::IGUIEnvironment*
Game_getGUIEnvironment( Game_t * game );

irr::scene::ISceneManager*
Game_getSceneManager( Game_t * game );

std::string
Game_getAssetFileName( Game_t * game, std::string const & relativeMediaFileName );

void
Game_createStandard( Game_t* game, irr::IrrlichtDevice* device, int argc, char** argv );

void
Game_start( Game_t * game );

int
Game_exec( Game_t * game );

Font_t
Game_getFont( Game_t * game, eFontType fontType );

///
/// pioneers3d.MainApp.Extras
///
void Game_clear( Game_t* game );
void Game_destroy( Game_t* game );
void Game_load( std::string const & fileName );
void Game_save( std::string const & fileName );


} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_GAME_HPP
