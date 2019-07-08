#ifndef PIONEERS3D_SCENE_TileSceneNode_HPP
#define PIONEERS3D_SCENE_TileSceneNode_HPP

#include <pioneers3d/types/Tile.hpp>

namespace pioneers3d {

class TileSceneNode : public irr::scene::ISceneNode
{
    Tile_t m_Data;
    irr::scene::SMesh m_Mesh;

public:
   TileSceneNode( irr::scene::ISceneManager* smgr,
                  irr::scene::ISceneNode* parent,
                  int id = -1 );

   ~TileSceneNode() override;

   void render() override
   {
      if ( m_Mesh.getMeshBufferCount() < 1 ) return;

      irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

      for ( irr::u32 i = 0; i < m_Mesh.getMeshBufferCount(); ++i )
      {
      irr::scene::IMeshBuffer* b = m_Mesh.getMeshBuffer( i );

      assert( b );

      driver->drawVertexPrimitiveList(
                  b->getVertices(),
                  b->getVertexCount(),
                  b->getIndices(),
                  b->getIndexCount() / 3,
                  irr::video::EVT_STANDARD,
                  irr::scene::EPT_TRIANGLES,
                  irr::video::EIT_16BIT );
      }
   }

   irr::u32 getMaterialCount() const override
   {
      return m_Mesh.getMeshBufferCount();
   }

   irr::video::SMaterial& getMaterial(irr::u32 num) override
   {
      return m_Mesh.getMeshBuffer(num)->getMaterial();
   }

   irr::core::aabbox3d<irr::f32> const & getBoundingBox() const override
   {
      return m_Mesh.getBoundingBox();
   }

protected:

};

} // end namespace pioneers3d

#endif // PIONEERS3D_SCENE_TileSceneNode_HPP
