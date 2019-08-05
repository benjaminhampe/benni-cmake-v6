/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_OBJECTS_HEXAGON_ROBBER_SETTLEMENTS_HPP
#define PIONEERS3D_TYPES_OBJECTS_HEXAGON_ROBBER_SETTLEMENTS_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

/// Hexagon ( ver_2018 ):
/**
                 D        triangles: ABF, BCE, BEF, CDE
                 /\
              C /  \ E    M (x=0,y=0,z=0,u=0.5,v=0.5)
               |----|     F(  .5, 0, -.25,  1, 0.25 )
               |  M |     E(  .5, 0,  .25,  1, 0.75 )
               |----|     D(   0, 0,  .50, .5, 1.00 )
              B \  / F    C( -.5, 0,  .25,  0, 0.75 )
                 \/       B( -.5, 0, -.25,  0, 0.25 )
                 A        A(   0, 0, -.50, .5, 0.00 )
*/

void getHexagonTriangles( std::vector< irr::core::triangle3df > & container, glm::vec2 const & tile_size, glm::vec3 const & tile_pos = glm::vec3(0,0,0) );
glm::vec3 getHexagonCorner( int i, glm::vec2 const & tile_size, glm::vec3 const & tile_pos = glm::vec3(0,0,0) );
glm::vec3 getHexagonEdgeCenter( int i, glm::vec2 const & tile_size, glm::vec3 const & tile_pos = glm::vec3(0,0,0) );
float32_t getHexagonEdgeAngle( int i, glm::vec2 const & tile_size );

/*
AutoSceneNode* createSettlement( Game_t* game )
{
   irr::scene::ISceneManager* smgr = game->Device->getSceneManager();
   AutoSceneNode* node = new AutoSceneNode( smgr, smgr->getRootSceneNode(), -1 );
   node->add( createRotatedBox( glm::vec3( 0,4,0 ), glm::vec3(10,8,10), glm::vec3(0,0,0), Player_getColor( game ) ), true );
   node->add( createRotatedBox( glm::vec3( 0,9,0 ), glm::vec3(8,2,8), glm::vec3(0,0,0), Player_getColor( game ) ), true );
   return node;
}
/// @brief Create a box with one repeating texture over all 6 planes
AutoMeshBuffer* createRoad( glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );
AutoMeshBuffer* createHouse( glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );
*/

class Game_t;

struct Raueber_t
{
    glm::ivec2                  BoardIndex;
    glm::vec3                   Pos;
    irr::scene::ISceneNode*     Node = nullptr;
    //~Raueber_t() { if (Node) Node->drop(); }
};

void Game_createRaeuber( Game_t* game );
//void Game_createRoad( Game_t* game, glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );
//void Game_createHouse( Game_t* game, glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );
//void Game_createCity( Game_t* game );


} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_ROBBER_HPP
