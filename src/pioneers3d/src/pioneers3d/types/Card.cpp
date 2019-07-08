#include <pioneers3d/types/Card.hpp>
#include <pioneers3d/types/Game.hpp>

namespace pioneers3d {

Card_t::Card_t()
{

}

CardSceneNode::CardSceneNode(
                  glm::vec2 size,
                  glm::vec2 radius,
                  glm::vec3 pos,
                  TileType tileType,
                  irr::video::ITexture* tex,
                  irr::scene::ISceneManager* smgr,
                  irr::scene::ISceneNode* parent,
                  int id )
    : AutoSceneNode( smgr, parent, id )
    , m_TileType( tileType )
{
   std::cout << __FUNCTION__ << "()" << std::endl;
   uint32_t tesselation = 13; // per 90 degrees
   AutoMeshBuffer* meshBuffer = createRoundRect( glm::vec3(0,0,0), size, radius, tesselation, irr::video::SColor( 255, 255, 255, 255 ).color );
   meshBuffer->setTexture( 0, tex );
   m_Mesh.addAutoMeshBuffer( meshBuffer, true );
   SceneNode_setPosition( this, pos );
}

void
Game_createCardObjectsDemo( Game_t * game )
{
   assert( game );
   irr::video::IVideoDriver* driver = Game_getVideoDriver( game );
   if ( !driver ) return;
   irr::scene::ISceneManager* smgr = Game_getSceneManager( game );
   if ( !smgr ) return;

   glm::vec2 size( 50.0f, 100.0f );
   glm::vec2 radius( 10.0f, 10.0f );

   new CardSceneNode( size, radius, glm::vec3( -350.0f, 100.0f, 200.0f ), TileType::HOLZ, Game_getCardTexture( game, TileType::HOLZ ), smgr, smgr->getRootSceneNode(), -1 );
   new CardSceneNode( size, radius, glm::vec3( -200.0f, 100.0f, 200.0f ), TileType::LEHM, Game_getCardTexture( game, TileType::LEHM ), smgr, smgr->getRootSceneNode(), -1 );
   new CardSceneNode( size, radius, glm::vec3(  -50.0f, 100.0f, 200.0f ), TileType::WEIZEN, Game_getCardTexture( game, TileType::WEIZEN ), smgr, smgr->getRootSceneNode(), -1 );
   new CardSceneNode( size, radius, glm::vec3(  200.0f, 100.0f, 200.0f ), TileType::WOLLE, Game_getCardTexture( game, TileType::WOLLE ), smgr, smgr->getRootSceneNode(), -1 );
   new CardSceneNode( size, radius, glm::vec3(  350.0f, 100.0f, 200.0f ), TileType::ERZ, Game_getCardTexture( game, TileType::ERZ ), smgr, smgr->getRootSceneNode(), -1 );

   auto left0 = new CardSceneNode( size, radius, glm::vec3( -200.0f, 100.0f, -350.0f ), TileType::HOLZ, Game_getCardTexture( game, TileType::HOLZ ), smgr, smgr->getRootSceneNode(), -1 );
   auto left1 = new CardSceneNode( size, radius, glm::vec3( -200.0f, 100.0f, -200.0f ), TileType::LEHM, Game_getCardTexture( game, TileType::LEHM ), smgr, smgr->getRootSceneNode(), -1 );
   auto left2 = new CardSceneNode( size, radius, glm::vec3( -200.0f, 100.0f,  -50.0f ), TileType::WEIZEN, Game_getCardTexture( game, TileType::WEIZEN ), smgr, smgr->getRootSceneNode(), -1 );
   auto left3 = new CardSceneNode( size, radius, glm::vec3( -200.0f, 100.0f,  200.0f ), TileType::WOLLE, Game_getCardTexture( game, TileType::WOLLE ), smgr, smgr->getRootSceneNode(), -1 );
   auto left4 = new CardSceneNode( size, radius, glm::vec3( -200.0f, 100.0f,  350.0f ), TileType::ERZ, Game_getCardTexture( game, TileType::ERZ ), smgr, smgr->getRootSceneNode(), -1 );

   glm::vec3 degrees = glm::vec3( 0, -90, 0 );
   SceneNode_setRotation( left0, degrees );
   SceneNode_setRotation( left1, degrees );
   SceneNode_setRotation( left2, degrees );
   SceneNode_setRotation( left3, degrees );
   SceneNode_setRotation( left4, degrees );

   auto r0 = new CardSceneNode( size, radius, glm::vec3( 200.0f, 100.0f, -350.0f ), TileType::HOLZ, Game_getCardTexture( game, TileType::HOLZ ), smgr, smgr->getRootSceneNode(), -1 );
   auto r1 = new CardSceneNode( size, radius, glm::vec3( 200.0f, 100.0f, -200.0f ), TileType::LEHM, Game_getCardTexture( game, TileType::LEHM ), smgr, smgr->getRootSceneNode(), -1 );
   auto r2 = new CardSceneNode( size, radius, glm::vec3( 200.0f, 100.0f,  -50.0f ), TileType::WEIZEN, Game_getCardTexture( game, TileType::WEIZEN ), smgr, smgr->getRootSceneNode(), -1 );
   auto r3 = new CardSceneNode( size, radius, glm::vec3( 200.0f, 100.0f,  200.0f ), TileType::WOLLE, Game_getCardTexture( game, TileType::WOLLE ), smgr, smgr->getRootSceneNode(), -1 );
   auto r4 = new CardSceneNode( size, radius, glm::vec3( 200.0f, 100.0f,  350.0f ), TileType::ERZ, Game_getCardTexture( game, TileType::ERZ ), smgr, smgr->getRootSceneNode(), -1 );

   degrees = glm::vec3( 0, 90, 0 );
   SceneNode_setRotation( r0, degrees );
   SceneNode_setRotation( r1, degrees );
   SceneNode_setRotation( r2, degrees );
   SceneNode_setRotation( r3, degrees );
   SceneNode_setRotation( r4, degrees );

}

} // end namespace pioneers3d
