#include <pioneers3d/types/BoardObjects.hpp>
#include <pioneers3d/types/Game.hpp>

namespace pioneers3d {

/// Hexagon ( ver_2018 ):
///
///                 D        triangles: ABF, BCE, BEF, CDE
///                 /\
///              C /  \ E    M (x=0,y=0,z=0,u=0.5,v=0.5)
///               |----|     F( 0.5, 0, -.25, 1, 0.25 )
///               |  M |     E( 0.5, 0, 0.25, 1, 0.75 )
///               |----|     D( 0.0, 0, 0.50, 0.5, 1 )
///              B \  / F    C( -.5, 0, 0.25, 0, 0.75 )
///                 \/       B( -.5, 0, -.25, 0, 0.25 )
///                  A       A( 0, 0, -.5, .5, 0 )
///

void getHexagonTriangles( std::vector< irr::core::triangle3df > & container, glm::vec2 const & tile_size, glm::vec3 const & tile_pos )
{
    irr::core::vector3df const P(        tile_pos.x, tile_pos.y, tile_pos.z );
    irr::core::vector3df const A( 0.00f*tile_size.x, 0.0f, -.50f*tile_size.y );
    irr::core::vector3df const B( -.50f*tile_size.x, 0.0f, -.25f*tile_size.y );
    irr::core::vector3df const C( -.50f*tile_size.x, 0.0f, 0.25f*tile_size.y );
    irr::core::vector3df const D( 0.00f*tile_size.x, 0.0f, 0.50f*tile_size.y );
    irr::core::vector3df const E( 0.50f*tile_size.x, 0.0f, 0.25f*tile_size.y );
    irr::core::vector3df const F( 0.50f*tile_size.x, 0.0f, -.25f*tile_size.y );

    auto addTriangle = [ &container, P ] ( irr::core::vector3df const & a, irr::core::vector3df const & b, irr::core::vector3df const & c )
    {
        container.emplace_back( irr::core::triangle3df( P + a, P + b, P + c ) );
    };

    addTriangle( A,B,F ); // ABF
    addTriangle( B,C,E ); // BCE
    addTriangle( B,E,F ); // BEF
    addTriangle( C,D,E ); // CDE
}


glm::vec3 getHexagonCorner( int i, glm::vec2 const & tile_size, glm::vec3 const & tile_pos )
{
   assert( i >= 0 );
   assert( i < 6 );
   switch ( i )
   {
      case 0: return tile_pos + glm::vec3( 0.00f*tile_size.x, 0.0f, -.50f*tile_size.y ); // A
      case 1: return tile_pos + glm::vec3( -.50f*tile_size.x, 0.0f, -.25f*tile_size.y ); // B
      case 2: return tile_pos + glm::vec3( -.50f*tile_size.x, 0.0f, 0.25f*tile_size.y ); // C
      case 3: return tile_pos + glm::vec3( 0.00f*tile_size.x, 0.0f, 0.50f*tile_size.y ); // D
      case 4: return tile_pos + glm::vec3( 0.50f*tile_size.x, 0.0f, 0.25f*tile_size.y ); // E
      case 5: return tile_pos + glm::vec3( 0.50f*tile_size.x, 0.0f, -.25f*tile_size.y ); // F
      default: assert( false ); // not allowed to happen
   }
   return glm::vec3(0.f,0.f,0.f);
}

glm::vec3 getHexagonEdgeCenter( int i, glm::vec2 const & tile_size, glm::vec3 const & tile_pos )
{
   assert( i >= 0 );
   assert( i < 6 );
   int32_t a = i % 6;
   int32_t b = ( i + 1 ) % 6;
   glm::vec3 const A = getHexagonCorner( a, tile_size, tile_pos );
   glm::vec3 const B = getHexagonCorner( b, tile_size, tile_pos );
   return A + (B-A)*0.5f; // Center of egde (a,b)
}

float32_t getHexagonEdgeAngle( int i, glm::vec2 const & tile_size )
{
   assert( i >= 0 );
   assert( i < 6 );
   int32_t a = i % 6;
   int32_t b = ( i + 1 ) % 6;
   glm::vec3 const A = getHexagonCorner( a, tile_size );
   glm::vec3 const B = getHexagonCorner( b, tile_size );
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

/*
AutoSceneNode* createSettlement( Game_t* game );
AutoMeshBuffer* createRoad( glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );
AutoMeshBuffer* createHouse( glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );
void Game_createRoad( Game_t* game, glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );
void Game_createHouse( Game_t* game, glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );
void Game_createCity( Game_t* game );
*/

void Game_createRaeuber( Game_t* game )
{
    assert( game );
    assert( game->Device );
    assert( game->Device->getSceneManager() );
    irr::scene::ISceneManager * smgr = game->Device->getSceneManager();
    irr::scene::ISceneNode * root = smgr->getRootSceneNode();
    std::string const fileName = game->MediaDir + "raeuber/raeuber.x";
    irr::scene::IAnimatedMesh* mesh = smgr->getMesh( irr::io::path( fileName.c_str() ) );
    if ( !mesh )
    {
        std::cout << "[ERROR] " << __FUNCTION__ << " :: "
            << "Cannot get mesh from file(" << fileName << ")\n";
        return;
    }

    irr::scene::IAnimatedMeshSceneNode* node = game->Device->getSceneManager()->addAnimatedMeshSceneNode( mesh, root, -1 );
    mesh->drop();
    node->setScale( irr::core::vector3df(0.1f,0.1f,0.1f) );

    for ( uint32_t i = 0; i < node->getMaterialCount(); ++i )
    {
        node->getMaterial( i ).Wireframe = false;
        node->getMaterial( i ).Lighting = false;
        node->getMaterial( i ).FogEnable = false;
        node->getMaterial( i ).NormalizeNormals = true;
        node->getMaterial( i ).setTexture( 0, Texture_get( game, eTexture::TEX_ROBBER ) );
    }
    game->Raeuber.Pos = glm::vec3(0,0,0);
    game->Raeuber.Node = node;
}

} // end namespace pioneers3d
