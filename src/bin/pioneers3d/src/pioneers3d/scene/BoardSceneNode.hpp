#ifndef PIONEERS3D_SCENE_BOARDSCENENODE_HPP
#define PIONEERS3D_SCENE_BOARDSCENENODE_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

#if 0
class BoardSceneNode : public irr::scene::ISceneNode
{
public:
    BoardSceneNode( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent, int id = -1 );

    virtual ~BoardSceneNode();

    void render() override
    {
        if ( !m_Mesh ) return;

        irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

        for ( irr::u32 i = 0; i < m_Mesh->getMeshBufferCount(); ++i )
        {
            irr::scene::IMeshBuffer* b = m_Mesh->getMeshBuffer( i );

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
        return m_Mesh->getMeshBufferCount();
    }

    irr::video::SMaterial& getMaterial(irr::u32 num) override
    {
        return m_Mesh->getMeshBuffer(num)->getMaterial();
    }

    irr::core::aabbox3d<irr::f32> const & getBoundingBox() const override
    {
        return m_Mesh->getBoundingBox();
    }

protected:

    irr::scene::SMeshBuffer*
    createTileMeshBuffer( eTileType const & tileType, int i, int j )
    {
        float const x = (0.50f * m_TileSize.x) * ( std::abs(j) % 2 ) + m_TileSize.x * i;
        float const y = 0.0f; // m_TileSize.y * sqrtf( (float)( i*i + j*j ) );
        float const z = (0.75f * m_TileSize.z) * j;

        irr::scene::SMeshBuffer * meshBuffer = createHexagonMeshBufferXZ( m_TileSize.x, m_TileSize.z, x, y, z );

        //meshBuffer->Material.setTexture( 0, getTileTexture( tileType ) );

        return meshBuffer;
    }

protected:
    irr::scene::SMesh * m_Mesh = nullptr;

    Board_t * m_Board = nullptr;

    glm::vec3 m_TileSize = glm::vec3( 10.0f, 4.0f, 10.0f );

    eTileType m_DefaultTileType = eTileType::WASSER;
};

#endif

} // end namespace pioneers3d

#endif // PIONEERS3D_SCENE_BOARDSCENENODE_HPP
