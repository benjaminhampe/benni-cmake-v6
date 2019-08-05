#include "PlanCAL.hpp"

namespace irrExt {

void
addGround( AutoMeshBuffer & buffer, glm::vec3 const & pos, glm::vec3 const & size )
{
   addBox( buffer, pos, size, 0xFFCC00CC );
}

void
addWall( AutoMeshBuffer & buffer, glm::vec3 const & pos, glm::vec3 const & size )
{
   addBox( buffer, pos, size, 0xFF10BB20 );
}

void
addTestObject( irr::scene::ISceneManager* smgr )
{
   /// GROUND
   /// TOP view XY, Z introduces height
   AutoMeshBuffer* pmb = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );
   AutoMeshBuffer& mb = *pmb;

   // Ground plane touches 0 ( zero ) z plane, everything above ground is above zero in [m]
   glm::vec3 groundSize = glm::vec3( 1000.0f, 1.0f, 1000.0f );
   addGround( mb, glm::vec3( 0.0f, -0.5f * groundSize.y, 0.0f ), groundSize );

   float32_t wallHeight = 3.0f;
   float32_t wallSize = 0.3f;
   addWall( mb, glm::vec3( 0, 0, 0.5f * groundSize.z ), glm::vec3( groundSize.x, wallHeight, wallSize ) ); // back wall ( top )
   addWall( mb, glm::vec3( 0, 0, -0.5f * groundSize.z ), glm::vec3( groundSize.x, wallHeight, wallSize ) ); // front wall ( bottom )
   addWall( mb, glm::vec3( -0.5f * groundSize.x, 0, 0 ), glm::vec3( wallSize, wallHeight, groundSize.z ) ); // back wall ( top )
   addWall( mb, glm::vec3( 0.5f * groundSize.x, 0, 0 ), glm::vec3( wallSize, wallHeight, groundSize.z ) ); // front wall ( bottom )

   AutoSceneNode* calnode = new AutoSceneNode( smgr, smgr->getRootSceneNode(), -1 );
   calnode->add( pmb, true );
}   

} // end namespace irrExt