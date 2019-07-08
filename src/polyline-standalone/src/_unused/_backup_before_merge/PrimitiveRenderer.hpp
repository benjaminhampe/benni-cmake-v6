#ifndef PRIMITIVE_RENDERER_SCENENODE_HPP
#define PRIMITIVE_RENDERER_SCENENODE_HPP

#include <AlphaSonic/AutoMeshBufferUtils.hpp>

class PrimitiveRenderer : public AutoSceneNode
{
public:
    struct Line3D_t
    {
        glm::vec3 A;
        glm::vec3 B;
        irr::video::SColor ColorA;
        irr::video::SColor ColorB;
    };

    struct Triangle3D_t
    {
        glm::vec3 A;
        glm::vec3 B;
        glm::vec3 C;
        irr::video::SColor ColorA;
        irr::video::SColor ColorB;
        irr::video::SColor ColorC;
        glm::vec2 UVA;
        glm::vec2 UVB;
        glm::vec2 UVC;
    };

    bool m_IsDirty;

    std::vector< Line3D_t > m_InputLines;
    std::vector< Triangle3D_t > m_InputTriangles;

    AutoMeshBuffer m_Lines;
    AutoMeshBuffer m_Triangles;
    AutoMeshBuffer m_Custom;
public:

    PrimitiveRenderer( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent );

    void render() override;

    void updateGeometry();

protected:

    void updateLineGeometry();
    void updateTriangleGeometry();
    void updateCustomGeometry();
public:

    // 2D
    void addLine2D( glm::vec2 const A, glm::vec2 const B, uint32_t const color );
    void addLine2D( glm::vec2 const A, glm::vec2 const B, uint32_t const colorA, uint32_t const colorB );
    void addTriangle2D( glm::vec2 const A, glm::vec2 const B, glm::vec2 const C, uint32_t const color );
    void addTriangle2D( glm::vec2 const A, glm::vec2 const B, glm::vec2 const C,
                        uint32_t const colorA, uint32_t const colorB, uint32_t const colorC );
    // 3D
    void addLine3D( glm::vec3 A, glm::vec3 B, uint32_t color );
    void addLine3D( glm::vec3 A, glm::vec3 B, uint32_t colorA, uint32_t colorB );
    void addTriangle3D( glm::vec3 A, glm::vec3 B, glm::vec3 C, uint32_t color );
    void addTriangle3D( glm::vec3 A, glm::vec3 B, glm::vec3 C, uint32_t colorA, uint32_t colorB, uint32_t colorC );


    uint32_t getVertexCount() const
    {
        return m_Custom.MeshBuffer.Vertices.size();
    }

    void addVertex2D( glm::vec2 const pos, uint32_t const color, glm::vec2 const uv )
    {
        m_Custom.MeshBuffer.Vertices.push_back( irr::video::S3DVertex( pos.x, 0.0f, pos.y, 0.0f, 1.0f, 0.0f, color, uv.x, uv.y ) );
        m_IsDirty = true;
    }

    void addIndex( uint16_t index )
    {
        m_Custom.MeshBuffer.Indices.push_back( index );
        m_IsDirty = true;
    }

    void addIndexedTriangle( uint16_t indexA, uint16_t indexB, uint16_t indexC )
    {
        m_Custom.MeshBuffer.Indices.push_back( indexA );
        m_Custom.MeshBuffer.Indices.push_back( indexB );
        m_Custom.MeshBuffer.Indices.push_back( indexC );
        m_IsDirty = true;
    }

    std::string toString() const
    {
        std::stringstream s;
        s << "Lines(v:" << m_Lines.MeshBuffer.Vertices.size() << ", i:" << m_Lines.MeshBuffer.Indices.size() << "), ";
        s << "Triangles(v:" << m_Triangles.MeshBuffer.Vertices.size() << ", i:" << m_Triangles.MeshBuffer.Indices.size() << "), ";
        s << "Custom(v:" << m_Custom.MeshBuffer.Vertices.size() << ", i:" << m_Custom.MeshBuffer.Indices.size() << ")";
        return s.str();
    }
};

#endif // POLYLINESCENENODE_HPP
