#include <pioneers3d/scene/BoardSceneNode.hpp>

#if 0

namespace pioneers3d {

BoardSceneNode::BoardSceneNode( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent, int id )
    : irr::scene::ISceneNode( parent, smgr, id )
{
    std::cout << __FUNCTION__ << "()" << std::endl;

    //setPosition( irr::core::vector3df(x,y,z) );
    setAutomaticCulling( irr::scene::EAC_OFF );

    m_Mesh = new irr::scene::SMesh();

    /// create default board
    for ( int i = -4; i < 5; ++i )
    {
        for ( int j = -4; j < 5; ++j )
        {
            eTileType tileType = static_cast< eTileType >( rand() % int(eTileType::COUNT) );

            irr::scene::SMeshBuffer * mb = createTileMeshBuffer( tileType, i, j );

            m_Mesh->addMeshBuffer( mb );

            mb->drop();

            m_Mesh->recalculateBoundingBox();
        }
    }
}

BoardSceneNode::~BoardSceneNode()
{
    std::cout << "~" << __FUNCTION__ << "()" << std::endl;

    if ( m_Mesh )
    {
        m_Mesh->drop();
    }
}

} // end namespace pioneers3d

#endif
