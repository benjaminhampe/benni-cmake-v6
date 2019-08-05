#ifndef PIONEERS3D_GAME_TEXTURES_HPP
#define PIONEERS3D_GAME_TEXTURES_HPP

#include <pioneers3d/types/Common.hpp>
#include <pioneers3d/types/TextureType.hpp>
#include <pioneers3d/types/TileType.hpp>

namespace pioneers3d {

   class Game_t;

   const char* Texture_getName( eTexture type );

   irr::video::ITexture* Texture_get( Game_t* game, eTexture type );
   irr::video::ITexture* Game_getChipTexture( Game_t* game, TileType tileType, int diceValue );
   irr::video::ITexture* Game_getTileTexture( Game_t* game, TileType tileType );
   irr::video::ITexture* Game_getPlayerTexture( Game_t* game, int player );
   irr::video::ITexture* Game_getDiceTexture( Game_t* game, int diceValue );
   irr::video::ITexture* Game_getCardTexture( Game_t* game, TileType tileType ); // get composed texture
   irr::video::ITexture* GameBuilder_getRessourceCardTexture( Game_t* game, TileType tileType, bool fg );
   irr::video::ITexture* GameBuilder_createRessourceCardTexture( Game_t* game, TileType tileType, std::string const & fileName );
   // inline irr::video::ITexture* Dice_getTexture( Game_t* game, int diceValue ) { return Game_getDiceTexture( game, diceValue ); }

} // end namespace pioneers3d

#endif // PIONEERS3D_GAME_TEXTURES_HPP
