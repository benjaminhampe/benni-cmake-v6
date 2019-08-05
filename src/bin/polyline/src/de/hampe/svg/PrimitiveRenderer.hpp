#ifndef PRIMITIVE_RENDERER_2D_SCENENODE_HPP
#define PRIMITIVE_RENDERER_2D_SCENENODE_HPP

#include <irrExt/AutoMeshBufferUtils.hpp>

class PrimitiveRenderer2D : public AutoSceneNode
{
    AutoMeshBuffer* m_Current;
    float32_t m_zIndex;
public:
    PrimitiveRenderer2D( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent );

    virtual ~PrimitiveRenderer2D() = default;

    void begin( irr::scene::E_PRIMITIVE_TYPE const primitiveType, float32_t zIndex = 0.0f )
    {
        m_Current = new AutoMeshBuffer( primitiveType );
        m_Current->MeshBuffer.Material.BackfaceCulling = true;
        m_Current->MeshBuffer.Material.AntiAliasing = irr::video::EAAM_QUALITY;
        m_Current->MeshBuffer.Material.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
        m_Current->MeshBuffer.Material.Lighting = false;
        m_Current->MeshBuffer.Material.FogEnable = false;
        m_Current->MeshBuffer.Material.Wireframe = false;
        m_Current->MeshBuffer.Material.NormalizeNormals = false;
        m_zIndex = zIndex;
    }

    void end()
    {
        add( m_Current, true );
    }

    uint32_t getVertexCount() const
    {
        if ( !m_Current ) return 0;
        return m_Current->getVertexCount();
    }

    uint32_t getIndexCount() const
    {
        if ( !m_Current ) return 0;
        return m_Current->getIndexCount();
    }

    glm::vec3 getBBoxMin() const
    {
        if ( !m_Current ) return glm::vec3();
        auto const v = m_Current->MeshBuffer.BoundingBox.MinEdge;
        return glm::vec3(v.X, v.Y, v.Z);
    }

    glm::vec3 getBBoxMax() const
    {
        if ( !m_Current ) return glm::vec3();
        auto const v = m_Current->MeshBuffer.BoundingBox.MaxEdge;
        return glm::vec3(v.X, v.Y, v.Z);
    }

    glm::vec3 getBBoxCenter() const
    {
        if ( !m_Current ) return glm::vec3();
        auto const v = m_Current->MeshBuffer.BoundingBox.getCenter();
        return glm::vec3(v.X, v.Y, v.Z);
    }

    irr::video::S3DVertex const &
    getVertex( uint32_t const i ) const
    {
        assert( m_Current );
        assert( m_Current->MeshBuffer.Vertices.size() > i );
        return m_Current->MeshBuffer.Vertices[ i ];
    }

    irr::video::S3DVertex &
    getVertex( uint32_t const i )
    {
        assert( m_Current );
        assert( m_Current->MeshBuffer.Vertices.size() > i );
        return m_Current->MeshBuffer.Vertices[ i ];
    }

    glm::vec2
    getVertexPos2D( uint32_t const i ) const
    {
        irr::video::S3DVertex const & v = getVertex( i );
        return glm::vec2( v.Pos.X, v.Pos.Y );
    }

    irr::video::IVideoDriver* getVideoDriver() const { return getSceneManager()->getVideoDriver(); }

    irr::video::ITexture* getTexture( std::string const & texName ) const { return getSceneManager()->getVideoDriver()->getTexture( texName.c_str() ); }

    void setWireframe( bool const enable )
    {
        if ( !m_Current ) return;
        m_Current->MeshBuffer.Material.setFlag( irr::video::EMF_WIREFRAME, enable );
    }

    void addVertex2D( glm::vec2 const pos, uint32_t const color = 0xFFFFFFFF, glm::vec2 const uv = glm::vec2() )
    {
        if ( !m_Current ) return;

        if ( m_Current->MeshBuffer.Vertices.size() == 0 )
        {
            m_Current->MeshBuffer.BoundingBox.reset( pos.x, pos.y, m_zIndex );
        }
        else
        {
            m_Current->MeshBuffer.BoundingBox.addInternalPoint( pos.x, pos.y, m_zIndex );
        }

        m_Current->MeshBuffer.Vertices.push_back( irr::video::S3DVertex( pos.x, pos.y, m_zIndex, 0.0f, 0.0f, 1.0f, color, uv.x, uv.y ) );
    }

    void addIndex( uint32_t const index )
    {
        if ( !m_Current ) return;
        assert( index < std::numeric_limits< int16_t >::max() );
        m_Current->MeshBuffer.Indices.push_back( index );
    }

    void addIndexedTriangle( uint32_t const indexA, uint32_t const indexB, uint32_t const indexC )
    {
        if ( !m_Current ) return;
        assert( indexA < std::numeric_limits< int16_t >::max() );
        assert( indexB < std::numeric_limits< int16_t >::max() );
        assert( indexC < std::numeric_limits< int16_t >::max() );

        m_Current->MeshBuffer.Indices.push_back( indexA );
        m_Current->MeshBuffer.Indices.push_back( indexB );
        m_Current->MeshBuffer.Indices.push_back( indexC );
    }

    /// This function is rather wasteful and should only be used for debugging or as last fallback for pure vertex mesh formats ( which are emulated by indexed irr mesh )
    void addTriangle2D( glm::vec2 const A, glm::vec2 const B, glm::vec2 const C,
                      uint32_t const debugColor = 0xFFFF00FF, // violett
                      glm::vec2 const uvA = glm::vec2(),
                      glm::vec2 const uvB = glm::vec2(),
                      glm::vec2 const uvC = glm::vec2() )
    {
        if ( !m_Current ) return;
        uint32_t const v = getVertexCount();
        addVertex2D( A, debugColor, uvA );
        addVertex2D( B, debugColor, uvB );
        addVertex2D( C, debugColor, uvC );
        addIndex( v );
        addIndex( v + 1 );
        addIndex( v + 2 );
    }

};

#endif // PRIMITIVE_RENDERER_2D_SCENENODE_HPP
