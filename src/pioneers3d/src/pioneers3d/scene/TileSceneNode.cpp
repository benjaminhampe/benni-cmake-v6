#include <pioneers3d/scene/TileSceneNode.hpp>

namespace pioneers3d {

TileSceneNode::TileSceneNode(
   irr::scene::ISceneManager* smgr,
   irr::scene::ISceneNode* parent,
   int id )
    : irr::scene::ISceneNode( parent, smgr, id )
{
    std::cout << __FUNCTION__ << "()" << std::endl;


}

TileSceneNode::~TileSceneNode()
{
    std::cout << "~" << __FUNCTION__ << "()" << std::endl;
}


} // end namespace pioneers3d

/*
 *
 *
void
addHexagon( irr::scene::SMeshBuffer * o, glm::vec3 size )
{
   // Append:
   //   o->Vertices.reallocate(6);
   //   o->Vertices.set_used(0);
   //   o->Indices.reallocate( 12 );
   //   o->Indices.set_used( 0 );

   auto addVertex = [ o ] ( float32_t x, float32_t y, float32_t z, float32_t u, float32_t v, bool flipV )
   {
      o->Vertices.push_back( irr::video::S3DVertex( x, y, z, 0, 1, 0, 0xFFFFFFEE, u, (flipV) ? 1.0f - v : v ) );
   };

   auto addTriangle = [ o ] ( uint16_t a, uint16_t b, uint16_t c )
   {
     o->Indices.push_back( a );
     o->Indices.push_back( b );
     o->Indices.push_back( c );
   };

   addVertex( 0.00f*size.x, size.y, -.50f*size.z, 0.50f, 0.00f, true ); // A = 0
   addVertex( -.50f*size.x, size.y, -.25f*size.z, 0.00f, 0.25f, true ); // B = 1
   addVertex( -.50f*size.x, size.y, 0.25f*size.z, 0.00f, 0.75f, true ); // C = 2
   addVertex( 0.00f*size.x, size.y, 0.50f*size.z, 0.50f, 1.00f, true ); // D = 3
   addVertex( 0.50f*size.x, size.y, 0.25f*size.z, 1.00f, 0.75f, true ); // E = 4
   addVertex( 0.50f*size.x, size.y, -.25f*size.z, 1.00f, 0.25f, true ); // F = 5

   // Indices ( triangles: ABF, BCE, BEF, CDE ):
   addTriangle( 0,1,5 ); // ABF
   addTriangle( 1,2,4 ); // BCE
   addTriangle( 1,4,5 ); // BEF
   addTriangle( 2,3,4 ); // CDE
}


*/
