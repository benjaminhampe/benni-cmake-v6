#ifndef PIONEERS3D_TYPES_CARD_HPP
#define PIONEERS3D_TYPES_CARD_HPP

#include <pioneers3d/types/Common.hpp>
#include <pioneers3d/types/TileType.hpp>

namespace pioneers3d {

class Card_t
{
public:
   Card_t();
};

class CardSceneNode : public AutoSceneNode
{
public:
   CardSceneNode( glm::vec2 size,
                  glm::vec2 radius,
                  glm::vec3 pos,
                  TileType tileType,
                  irr::video::ITexture* tex, // given by game or we need to pass a pointer to game
                  irr::scene::ISceneManager* smgr,
                  irr::scene::ISceneNode* parent,
                  int id = -1 );

public:
   TileType m_TileType;
};

class Game_t;
void Game_createCardObjectsDemo( Game_t * game );
} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_CARD_HPP
