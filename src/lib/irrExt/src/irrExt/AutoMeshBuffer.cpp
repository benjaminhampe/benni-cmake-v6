#include <irrExt/AutoMeshBuffer.hpp>

namespace irrExt {

/*
void
enumerateMesh( irr::scene::SMesh & p )
{
    std::cout << "VertexCount = " << p.Vertices.size() << "\n";
    for ( uint32_t i = 0; i < p.Vertices.size(); ++i )
    {
        std::cout << "Vertex[" << i << "]" << toString( p.Vertices[ i ] ) << "\n";
    }

    std::cout << "IndexCount = " << p.Indices.size() << "\n";

    for ( uint32_t i = 0; i < p.Indices.size(); ++i )
    {
        std::cout << "Index[" << i << "]" << p.Indices[ i ] << "\n";
    }

    std::cout << "Material = " << p.Vertices.size() << "\n";
    std::cout << "BoundingBox = " << p.Vertices.size() << "\n";
}
*/

void
enumerateMeshBuffer( irr::scene::SMeshBuffer & p )
{
    std::cout << "VertexCount = " << p.Vertices.size() << "\n";
    for ( uint32_t i = 0; i < p.Vertices.size(); ++i )
    {
        std::cout << "Vertex[" << i << "]" << p.Vertices[ i ] << "\n";
    }

    std::cout << "IndexCount = " << p.Indices.size() << "\n";

    for ( uint32_t i = 0; i < p.Indices.size(); ++i )
    {
        std::cout << "Index[" << i << "]" << p.Indices[ i ] << "\n";
    }

    std::cout << "Material = " << p.Vertices.size() << "\n";
    std::cout << "BoundingBox = " << p.Vertices.size() << "\n";
}

//=======================================================================================
//
// CLASS: AutoMeshBuffer
//
//=======================================================================================

// static
uint32_t
AutoMeshBuffer::getPrimitiveCount( irr::scene::E_PRIMITIVE_TYPE primitiveType, uint32_t indexCount )
{
   switch( primitiveType )
   {
      case irr::scene::EPT_POINTS: return indexCount;
      case irr::scene::EPT_LINES: return indexCount / 2;
      case irr::scene::EPT_LINE_LOOP: return indexCount - 1;
      case irr::scene::EPT_TRIANGLES: return indexCount / 3;
      case irr::scene::EPT_POLYGON: return indexCount;
      case irr::scene::EPT_QUADS: return indexCount / 4;
      default: return 0;
   }
}

AutoMeshBuffer::AutoMeshBuffer()
    : PrimitiveType( irr::scene::EPT_POINTS ) // This works for every mesh type ( aka fallback )
//    , VertexType( irr::video::EVT_STANDARD )	// Standard Vertex: FVF_POSITION | FVF_NORMAL | FVF_COLOR32 | FVF_TEXCOORD0
//    , IndexType( irr::video::EIT_32BIT )
//    , MeshBuffer( VertexType, IndexType )
{
   Material.MaterialType = irr::video::EMT_SOLID;
   Material.Lighting = false;
   Material.FogEnable = false;
}

AutoMeshBuffer::AutoMeshBuffer( irr::scene::E_PRIMITIVE_TYPE primType )
    : PrimitiveType( primType )
//    , VertexType( vertexType )
//    , IndexType( indexType )
//    , MeshBuffer( VertexType, IndexType )
{
   Material.MaterialType = irr::video::EMT_SOLID;
   Material.Lighting = false;
   Material.FogEnable = false;
}

// AutoMeshBuffer::~AutoMeshBuffer() {}

void
AutoMeshBuffer::render( irr::video::IVideoDriver* driver )
{
   if ( driver )
   {
      driver->setMaterial( this->getMaterial() );
      driver->drawVertexPrimitiveList(
         this->getVertices(),
         this->getVertexCount(),
         this->getIndices(),
         this->getPrimitiveCount(),
         this->getVertexType(),
         this->getPrimitiveType(),
         this->getIndexType()
      );
   }
}

bool
AutoMeshBuffer::hasTransparentVertexColor() const
{
   for ( uint32_t i = 0; i < getVertexCount(); ++i )
   {
      if ( Vertices[ i ].Color.getAlpha() < 255 )
      {
         return true;
      }
   }
   return false;
}

bool
AutoMeshBuffer::getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const
{
   if ( PrimitiveType == irr::scene::EPT_TRIANGLES )
   {
      uint32_t const vertexCount = getVertexCount();
      uint32_t const indexCount = getIndexCount();

      //            std::cout << __FUNCTION__ << "(" << toString( ray.start ) << ", " << toString( ray.getVector() ) << ")"
      //                << " v:" << vertexCount << ", i:" << indexCount << "\n";

      for ( uint32_t i = 0; i < indexCount/3; ++i )
      {
         uint32_t const iA = Indices[ 3 * i ];
         uint32_t const iB = Indices[ 3 * i + 1 ];
         uint32_t const iC = Indices[ 3 * i + 2 ];
         if ( ( iA < vertexCount ) && ( iB < vertexCount ) && ( iC < vertexCount ) )
         {
            irr::core::vector3df const & A = Vertices[ iA ].Pos;
            irr::core::vector3df const & B = Vertices[ iB ].Pos;
            irr::core::vector3df const & C = Vertices[ iC ].Pos;
            irr::core::triangle3df const tri( A, B, C );
            if ( tri.getIntersectionWithLine( ray.start, ray.getVector(), hitPosition ) )
            {
               return true;
            }
         }
         else
         {
            std::cout << "[Warn] " << __FUNCTION__ << "(" << " v:" << vertexCount << ", i:" << indexCount << ")"
            << "iA:" << iA << ", iB:" << iB << ", iC:" << iC << " -> Broken mesh\n";
         }
      }
   }
   return false;
}

} // end namespace irrExt
















#if 0

//=======================================================================================
//
// CLASS: AutoMesh
//
//=======================================================================================

AutoMesh::AutoMesh()
{
    #ifdef _DEBUG
    setDebugName("AutoMesh");
    #endif
    DefaultMaterial.Wireframe = false;
    DefaultMaterial.Lighting = false;
    DefaultMaterial.MaterialType = irr::video::EMT_SOLID;
}

AutoMesh::~AutoMesh()
{
    for ( irr::u32 i = 0; i < MeshBuffers.size(); ++i )
    {
        AutoMeshBuffer* p = MeshBuffers[ i ];
        if ( p )
        {
            p->drop();
        }
    }
}

void
AutoMesh::clear()
{
    for ( irr::u32 i = 0; i < MeshBuffers.size(); ++i )
    {
        AutoMeshBuffer* p = MeshBuffers[ i ];
        if ( p )
        {
            p->drop();
        }
    }
    MeshBuffers.clear();
    BoundingBox.reset ( 0.f, 0.f, 0.f );
}

irr::u32
AutoMesh::getMaterialCount() const
{
    return getMeshBufferCount();
}

irr::video::SMaterial &
AutoMesh::getMaterial( irr::u32 i )
{
    if ( i >= getMaterialCount() )
    {
        std::cout << __FUNCTION__ << "(" << i << ") :: [Error] Invalid material index.\n";
        return DefaultMaterial;
    }

    AutoMeshBuffer* p = MeshBuffers[ i ];
    if ( !p )
    {
        std::cout << __FUNCTION__ << "(" << i << ") :: [Error] Invalid pointer to AutoMeshBuffer\n";
        return DefaultMaterial;
    }

    return p->MeshBuffer.Material;
}

irr::u32
AutoMesh::getMeshBufferCount() const
{
    return static_cast< irr::u32 >( MeshBuffers.size() );
}

irr::scene::IMeshBuffer*
AutoMesh::getMeshBuffer( irr::u32 i ) const
{
    AutoMeshBuffer* p = MeshBuffers[ i ];
    if ( !p )
    {
        std::cout << __FUNCTION__ << "(" << i << ") :: [Error] Invalid pointer to AutoMeshBuffer\n";
        return nullptr;
    }
    return &(p->MeshBuffer);
}

irr::scene::IMeshBuffer*
AutoMesh::getMeshBuffer( irr::video::SMaterial const & material ) const
{
    for ( int32_t i = int32_t( MeshBuffers.size() ) - 1; i >= 0; --i )
    {
        AutoMeshBuffer* p = MeshBuffers[ i ];

        if ( p && p->MeshBuffer.getMaterial() == material )
        {
            return &(p->MeshBuffer);
        }
    }

    return nullptr;
}

irr::core::aabbox3d<irr::f32> const &
AutoMesh::getBoundingBox() const
{
    return BoundingBox;
}

void
AutoMesh::setBoundingBox( irr::core::aabbox3df const & box )
{
    BoundingBox = box;
}

void
AutoMesh::recalculateBoundingBox()
{
    if (MeshBuffers.size())
    {
        BoundingBox = MeshBuffers[ 0 ]->getBoundingBox();
        for ( irr::u32 i = 1; i < MeshBuffers.size(); ++i )
        {
            BoundingBox.addInternalBox( MeshBuffers[i]->getBoundingBox() );
        }
    }
    else
        BoundingBox.reset(0.0f, 0.0f, 0.0f);
}

void
AutoMesh::addMeshBuffer( irr::scene::IMeshBuffer* buf )
{
    std::cout << "[Warn] " << __FUNCTION__ << "() :: Should not be called\n";
}

void
AutoMesh::addAutoMeshBuffer( AutoMeshBuffer* buf, bool dropAfter )
{
    if ( !buf )
    {
        std::cout << "[Warn] " << __FUNCTION__ << " :: Cannot add nullptr\n";
        return;
    }

    buf->grab();

    if ( MeshBuffers.size() == 0 )
    {
        BoundingBox.reset( buf->getBoundingBox() );
    }
    else
    {
        BoundingBox.addInternalBox( buf->getBoundingBox() );
    }

    MeshBuffers.emplace_back( buf );

    if ( dropAfter )
    {
        buf->drop(); // remove old owner from ref count
    }
}

void AutoMesh::setMaterialFlag( irr::video::E_MATERIAL_FLAG flag, bool newvalue)
{
    for ( irr::u32 i=0; i<MeshBuffers.size(); ++i )
    {
        getMeshBuffer( i )->getMaterial().setFlag( flag, newvalue );
    }
}

void AutoMesh::setHardwareMappingHint( irr::scene::E_HARDWARE_MAPPING newMappingHint, irr::scene::E_BUFFER_TYPE buffer )
{
    for ( irr::u32 i=0; i<MeshBuffers.size(); ++i )
    {
        getMeshBuffer( i )->setHardwareMappingHint( newMappingHint, buffer);
    }
}

void AutoMesh::setDirty( irr::scene::E_BUFFER_TYPE buffer )
{
    for (irr::u32 i=0; i<MeshBuffers.size(); ++i)
    {
        getMeshBuffer( i )->setDirty(buffer);
    }
}

/// @brief Add collision detection
bool
AutoMesh::getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const
{
    for ( uint32_t i = 0; i < MeshBuffers.size(); ++i )
    {
        AutoMeshBuffer* p = MeshBuffers[ i ];
        if ( p && p->getIntersectionWithLine( ray, hitPosition ) )
        {
            return true;
        }
    }

    return false;
}


//=======================================================================================
//
// CLASS: AutoSceneNode
//
//=======================================================================================

AutoSceneNode::AutoSceneNode( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent, int id )
   : irr::scene::ISceneNode( parent, smgr, id )
{
   std::cout << __FUNCTION__ << "()\n";
   //setAutomaticCulling( irr::scene::EAC_OFF );
}

AutoSceneNode::~AutoSceneNode()
{
   std::cout << __FUNCTION__ << "()\n";
   clear();
}

//! This method is called just before the rendering process of the whole scene.
void
AutoSceneNode::OnRegisterSceneNode()
{
   std::cout << __FUNCTION__ << "()\n";
   if ( IsVisible )
   {
      SceneManager->registerNodeForRendering(this);
   }
   ISceneNode::OnRegisterSceneNode();
}

void
AutoSceneNode::render()
{
   assert( SceneManager );
   irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();
   if ( !driver )
   {
      std::cout << __FUNCTION__ << "() :: Invalid IVideoDriver\n";
      return;
   }

   driver->setTransform( irr::video::ETS_WORLD, getAbsoluteTransformation() );

   for ( size_t i = 0; i < m_MeshBuffer.size(); ++i )
   {
      AutoMeshBuffer * p = m_MeshBuffer[ i ];
      assert( p );
      p->render( driver );
   }

   if ( DebugDataVisible & irr::scene::EDS_BBOX )
   {
      driver->draw3DBox( m_BoundingBox );
   }

   if ( DebugDataVisible & irr::scene::EDS_BBOX_BUFFERS )
   {
      for ( size_t i = 0; i < m_MeshBuffer.size(); ++i )
      {
         AutoMeshBuffer * p = m_MeshBuffer[ i ];
         assert( p );
         driver->draw3DBox( p->getBoundingBox(), 0xFFEFEFEF );
      }
   }

   // irr::scene::ISceneNode::render();
}

bool
AutoSceneNode::getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const
{
   // irr::core::matrix4 mat = this->getAbsoluteTransformation();
   // double minDistance = std::numeric_limits< double >::max();

   for ( size_t i = 0; i < m_MeshBuffer.size(); ++i )
   {
      AutoMeshBuffer * p = m_MeshBuffer[ i ];
      assert( p );
      if ( p->getIntersectionWithLine( ray, hitPosition ) )
      {
         return true;
      }
   }
   return false;
}

//void
//AutoSceneNode::setPosition( float32_t x, float32_t y, float32_t z )
//{
//    irr::scene::ISceneNode::setPosition( irr::core::vector3df( x, y, z ) );
//}
#endif

