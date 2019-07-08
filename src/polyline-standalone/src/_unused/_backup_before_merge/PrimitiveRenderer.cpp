#include "PrimitiveRenderer.hpp"

PrimitiveRenderer::PrimitiveRenderer( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent )
     : AutoSceneNode( smgr, parent )
     , m_IsDirty( true )
     , m_Lines( irr::scene::EPT_LINES )
     , m_Triangles( irr::scene::EPT_TRIANGLES )
{}

void
PrimitiveRenderer::addLine2D( glm::vec2 const A, glm::vec2 const B, uint32_t const color )
{
    //float32_t const z = float32_t( m_InputLines.size() );
    Line3D_t line;
    line.A = glm::vec3( A.x, 0.1f, A.y );
    line.B = glm::vec3( B.x, 0.1f, B.y );
    line.ColorA = color;
    line.ColorB = color;
    m_InputLines.emplace_back( std::move( line ) );
    m_IsDirty = true;
}

void
PrimitiveRenderer::addLine2D( glm::vec2 const A, glm::vec2 const B, uint32_t const colorA, uint32_t const colorB )
{
    //float32_t const z = float32_t( m_InputLines.size() );
    Line3D_t line;
    line.A = glm::vec3( A.x, 0.1f, A.y );
    line.B = glm::vec3( B.x, 0.1f, B.y );
    line.ColorA = colorA;
    line.ColorB = colorB;
    m_InputLines.emplace_back( std::move( line ) );
    m_IsDirty = true;
}

void
PrimitiveRenderer::addLine3D( glm::vec3 A, glm::vec3 B, uint32_t color )
{
    Line3D_t line;
    line.A = A;
    line.B = B;
    line.ColorA = color;
    line.ColorB = color;
    m_InputLines.emplace_back( std::move( line ) );
    m_IsDirty = true;
}

void
PrimitiveRenderer::addLine3D( glm::vec3 A, glm::vec3 B, uint32_t colorA, uint32_t colorB )
{
    Line3D_t line;
    line.A = A;
    line.B = B;
    line.ColorA = colorA;
    line.ColorB = colorB;
    m_InputLines.emplace_back( std::move( line ) );
    m_IsDirty = true;
}

void
PrimitiveRenderer::addTriangle2D( glm::vec2 const A, glm::vec2 const B, glm::vec2 const C, uint32_t const color )
{
    Triangle3D_t triangle;
    //float32_t const z = 0.001f * float32_t( m_InputTriangles.size() );
    triangle.A = glm::vec3( A.x, 0.0f, A.y );
    triangle.B = glm::vec3( B.x, 0.0f, B.y );
    triangle.C = glm::vec3( C.x, 0.0f, C.y );
    triangle.ColorA = color;
    triangle.ColorB = color;
    triangle.ColorC = color;
    m_InputTriangles.emplace_back( std::move( triangle ) );
    m_IsDirty = true;
}

void
PrimitiveRenderer::addTriangle2D( glm::vec2 const A, glm::vec2 const B, glm::vec2 const C,
                                  uint32_t const colorA, uint32_t const colorB, uint32_t const colorC )
{
    Triangle3D_t triangle;
    //float32_t const z = 0.001f * float32_t( m_InputTriangles.size() );
    triangle.A = glm::vec3( A.x, 0.0f, A.y );
    triangle.B = glm::vec3( B.x, 0.0f, B.y );
    triangle.C = glm::vec3( C.x, 0.0f, C.y );
    triangle.ColorA = colorA;
    triangle.ColorB = colorB;
    triangle.ColorC = colorC;
    m_InputTriangles.emplace_back( std::move( triangle ) );
    m_IsDirty = true;
}

void
PrimitiveRenderer::addTriangle3D( glm::vec3 A, glm::vec3 B, glm::vec3 C, uint32_t color )
{
    Triangle3D_t triangle;
    triangle.A = A;
    triangle.B = B;
    triangle.C = C;
    triangle.ColorA = color;
    triangle.ColorB = color;
    triangle.ColorC = color;
    m_InputTriangles.emplace_back( std::move( triangle ) );
    m_IsDirty = true;
}

void
PrimitiveRenderer::addTriangle3D( glm::vec3 A, glm::vec3 B, glm::vec3 C, uint32_t colorA, uint32_t colorB, uint32_t colorC )
{
    Triangle3D_t triangle;
    triangle.A = A;
    triangle.B = B;
    triangle.C = C;
    triangle.ColorA = colorA;
    triangle.ColorB = colorB;
    triangle.ColorC = colorC;
    m_InputTriangles.emplace_back( std::move( triangle ) );
    m_IsDirty = true;
}


void
PrimitiveRenderer::render() // override
{
    m_Triangles.render( SceneManager->getVideoDriver() );
    m_Lines.render( SceneManager->getVideoDriver() );
    m_Custom.render( SceneManager->getVideoDriver() );

    AutoSceneNode::render();
}

void
PrimitiveRenderer::updateGeometry()
{
    updateLineGeometry();
    updateTriangleGeometry();
    updateCustomGeometry();
}

void
PrimitiveRenderer::updateLineGeometry()
{
    AutoMeshBuffer & p = m_Lines;
    p.PrimitiveType = irr::scene::EPT_LINES;
    p.MeshBuffer.Material.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
    p.MeshBuffer.Material.Thickness = 4.35f;
    p.MeshBuffer.Material.Wireframe = false;
    p.MeshBuffer.Material.FogEnable = false;
    p.MeshBuffer.Material.Lighting = false;

    p.MeshBuffer.Vertices.reallocate( m_InputLines.size() * 2 );
    p.MeshBuffer.Vertices.set_used( 0 );
    p.MeshBuffer.Indices.reallocate( m_InputLines.size() * 2 );
    p.MeshBuffer.Indices.set_used( 0 );
    p.MeshBuffer.BoundingBox.reset( 0,0,0 );

    if ( m_InputLines.size() < 1 )
    {
        return;
    }

    Line3D_t const & line = m_InputLines[ 0 ];
    p.MeshBuffer.BoundingBox.reset( line.A.x, line.A.y, line.A.z );

    for ( size_t i = 0; i < m_InputLines.size(); ++i )
    {
        Line3D_t const & line = m_InputLines[ i ];

        irr::core::vector3df const n( 0,1,0 );
        irr::video::S3DVertex const vA( line.A.x, line.A.y, line.A.z, n.X, n.Y, n.Z, line.ColorA, 0.f, 1.f );
        irr::video::S3DVertex const vB( line.B.x, line.B.y, line.B.z, n.X, n.Y, n.Z, line.ColorB, 0.f, 1.f );

        p.MeshBuffer.BoundingBox.addInternalPoint( vA.Pos );
        p.MeshBuffer.BoundingBox.addInternalPoint( vB.Pos );

        uint32_t const vtxCount = p.MeshBuffer.Vertices.size();
        p.MeshBuffer.Vertices.push_back( vA );
        p.MeshBuffer.Vertices.push_back( vB );
        p.MeshBuffer.Indices.push_back( vtxCount );
        p.MeshBuffer.Indices.push_back( vtxCount + 1 );

    }
}

void
PrimitiveRenderer::updateTriangleGeometry()
{
    AutoMeshBuffer & p = m_Triangles;
    p.PrimitiveType = irr::scene::EPT_TRIANGLES;
    p.MeshBuffer.Material.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
    p.MeshBuffer.Material.Wireframe = false;
    p.MeshBuffer.Material.FogEnable = false;
    p.MeshBuffer.Material.Lighting = false;
    p.MeshBuffer.Vertices.reallocate( m_InputTriangles.size() * 3 );
    p.MeshBuffer.Vertices.set_used( 0 );
    p.MeshBuffer.Indices.reallocate( m_InputTriangles.size() * 3 );
    p.MeshBuffer.Indices.set_used( 0 );
    p.MeshBuffer.BoundingBox.reset( 0,0,0 );

    if ( m_InputTriangles.size() < 1 )
    {
        return;
    }

    Triangle3D_t const & triangle = m_InputTriangles[ 0 ];
    p.MeshBuffer.BoundingBox.reset( triangle.A.x, triangle.A.y, triangle.A.z );

    for ( size_t i = 0; i < m_InputTriangles.size(); ++i )
    {
        Triangle3D_t const & triangle = m_InputTriangles[ i ];
        irr::core::vector3df const n( 0,1,0 );
        irr::video::S3DVertex const vA( triangle.A.x, triangle.A.y, triangle.A.z, n.X, n.Y, n.Z, triangle.ColorA, triangle.UVA.x, triangle.UVA.y );
        irr::video::S3DVertex const vB( triangle.B.x, triangle.B.y, triangle.B.z, n.X, n.Y, n.Z, triangle.ColorB, triangle.UVB.x, triangle.UVB.y );
        irr::video::S3DVertex const vC( triangle.C.x, triangle.C.y, triangle.C.z, n.X, n.Y, n.Z, triangle.ColorC, triangle.UVC.x, triangle.UVC.y );
        uint32_t const vtxCount = p.MeshBuffer.Vertices.size();
        p.MeshBuffer.Vertices.push_back( vA );
        p.MeshBuffer.Vertices.push_back( vB );
        p.MeshBuffer.Vertices.push_back( vC );
        p.MeshBuffer.Indices.push_back( vtxCount );
        p.MeshBuffer.Indices.push_back( vtxCount + 1 );
        p.MeshBuffer.Indices.push_back( vtxCount + 2 );
        p.MeshBuffer.BoundingBox.addInternalPoint( vA.Pos );
        p.MeshBuffer.BoundingBox.addInternalPoint( vB.Pos );
        p.MeshBuffer.BoundingBox.addInternalPoint( vC.Pos );
    }
}

void
PrimitiveRenderer::updateCustomGeometry()
{
    AutoMeshBuffer & p = m_Custom;
    p.PrimitiveType = irr::scene::EPT_TRIANGLE_STRIP;
    p.MeshBuffer.Material.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
    p.MeshBuffer.Material.Wireframe = true;
    p.MeshBuffer.Material.FogEnable = false;
    p.MeshBuffer.Material.Lighting = false;
    p.MeshBuffer.recalculateBoundingBox();
}

