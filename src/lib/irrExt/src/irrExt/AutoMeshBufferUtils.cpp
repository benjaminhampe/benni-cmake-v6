#include <irrExt/AutoMeshBufferUtils.hpp>

namespace irrExt {

glm::vec3
getNormal( glm::vec3 A, glm::vec3 B, glm::vec3 C )
{
    return glm::normalize( glm::cross( B-A, C-A ) );
}

void
translate( AutoMeshBuffer & p, glm::vec3 const & offset )
{
    irr::core::vector3df const irrOffset( offset.x, offset.y, offset.z );

    for ( uint32_t i = 0; i < p.Vertices.size(); ++i )
    {
        p.Vertices[ i ].Pos += irrOffset;
    }

    p.BoundingBox.MinEdge += irrOffset;
    p.BoundingBox.MaxEdge += irrOffset;
}

void
rotate( AutoMeshBuffer & p, glm::vec3 const & degrees )
{
    irr::core::matrix4 m;
    m.makeIdentity();
    m.setRotationDegrees( irr::core::vector3df( degrees.x, degrees.y, degrees.z ) );

    for ( uint32_t i = 0; i < p.Vertices.size(); ++i )
    {
        irr::core::vector3df & pos = p.Vertices[ i ].Pos;
        m.transformVect( pos );
    }

    p.recalculateBoundingBox();
}

AutoMeshBuffer*
createRect( glm::vec3 pos, glm::vec2 size, uint32_t color )
{
    AutoMeshBuffer* p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

    // bbox
    float32_t dx = 0.5f*size.x;
    float32_t dy = 0.5f*size.y;
    p->BoundingBox = irr::core::aabbox3df( pos.x-dx, pos.y-dy, pos.z, pos.x+dx, pos.y+dy, pos.z);

    // material
    // p->setDefaultMaterialType( color );

    // vertices
	 p->Vertices.reserve( 4 );
    //p->Vertices.reallocate( 4 );
    //p->Vertices.set_used( 0 );

    auto addVertex = [ p, &pos, color ] ( float32_t x, float32_t y, float32_t z, float32_t u, float32_t v )
    {
        p->Vertices.push_back( irr::video::S3DVertex( pos.x + x, pos.y + y, pos.z + z, 0, 0, -1, color, u, v ) );
    };

    addVertex( -dx, -dy, 0.0f, 0.0f, 1.0f ); // A=0
    addVertex( -dx,  dy, 0.0f, 0.0f, 0.0f ); // B=1
    addVertex(  dx,  dy, 0.0f, 1.0f, 0.0f ); // C=2
    addVertex(  dx, -dy, 0.0f, 1.0f, 1.0f ); // D=3

    // indices
    p->Indices.reserve( 6 );
    //p->Indices.set_used( 0 );

    auto addTriangle = [ p ] ( uint16_t a, uint16_t b, uint16_t c )
    {
        p->Indices.push_back( a );
        p->Indices.push_back( b );
        p->Indices.push_back( c );
    };

    addTriangle( 0, 1, 2 ); 	// ABC = 012
    addTriangle( 0, 2, 3 ); 	// ACD = 023

    return p;
}

/// @brief Create a box with one repeating texture over all 6 planes
AutoMeshBuffer *
createBox( glm::vec3 const & pos, glm::vec3 const & size, uint32_t color )
{
    AutoMeshBuffer * p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

    float const dx = 0.5f*size.x;
    float const dy = 0.5f*size.y;
    float const dz = 0.5f*size.z;

    glm::vec3 const A = pos + glm::vec3(-dx,-dy,-dz);
    glm::vec3 const B = pos + glm::vec3(-dx, dy,-dz);
    glm::vec3 const C = pos + glm::vec3( dx, dy,-dz);
    glm::vec3 const D = pos + glm::vec3( dx,-dy,-dz);
    glm::vec3 const E = pos + glm::vec3(-dx,-dy, dz);
    glm::vec3 const F = pos + glm::vec3(-dx, dy, dz);
    glm::vec3 const G = pos + glm::vec3( dx, dy, dz);
    glm::vec3 const H = pos + glm::vec3( dx,-dy, dz);

    addQuad( *p, E, F, B, A, color ); // n(-1, 0, 0);    // Neg X = EFBA
    addQuad( *p, E, A, D, H, color ); // n( 0,-1, 0);    // Neg Y = EADH
    addQuad( *p, A, B, C, D, color ); // n( 0, 0, -1);   // Neg Z = ABCD
    addQuad( *p, D, C, G, H, color ); // n( 1, 0, 0);    // Pos X = DCGH
    addQuad( *p, B, F, G, C, color ); // n( 0, 1, 0);    // Pos Y = BFGC
    addQuad( *p, H, G, F, E, color ); // n( 0, 0, 1);    // Pos Z = HGFE

    return p;
}

/// @brief Create a box with one repeating texture over all 6 planes
AutoMeshBuffer *
createRotatedBox( glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color )
{
    AutoMeshBuffer * p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

    float const dx = 0.5f*size.x;
    float const dy = 0.5f*size.y;
    float const dz = 0.5f*size.z;

    glm::mat4 m0;
    m0 = glm::rotate( m0, rot.x * irr::core::DEGTORAD, glm::vec3( 1,0,0 ) );
    m0 = glm::rotate( m0, rot.y * irr::core::DEGTORAD, glm::vec3( 0,1,0 ) );
    m0 = glm::rotate( m0, rot.z * irr::core::DEGTORAD, glm::vec3( 0,0,1 ) );
    glm::mat3 mat(m0);
    glm::vec3 const A = pos + mat * glm::vec3(-dx,-dy,-dz);
    glm::vec3 const B = pos + mat * glm::vec3(-dx, dy,-dz);
    glm::vec3 const C = pos + mat * glm::vec3( dx, dy,-dz);
    glm::vec3 const D = pos + mat * glm::vec3( dx,-dy,-dz);
    glm::vec3 const E = pos + mat * glm::vec3(-dx,-dy, dz);
    glm::vec3 const F = pos + mat * glm::vec3(-dx, dy, dz);
    glm::vec3 const G = pos + mat * glm::vec3( dx, dy, dz);
    glm::vec3 const H = pos + mat * glm::vec3( dx,-dy, dz);

    addQuad( *p, E, F, B, A, color ); // n(-1, 0, 0);    // Neg X = EFBA
    addQuad( *p, E, A, D, H, color ); // n( 0,-1, 0);    // Neg Y = EADH
    addQuad( *p, A, B, C, D, color ); // n( 0, 0, -1);   // Neg Z = ABCD
    addQuad( *p, D, C, G, H, color ); // n( 1, 0, 0);    // Pos X = DCGH
    addQuad( *p, B, F, G, C, color ); // n( 0, 1, 0);    // Pos Y = BFGC
    addQuad( *p, H, G, F, E, color ); // n( 0, 0, 1);    // Pos Z = HGFE

    return p;
}

#if 0 

AutoMesh *
createDice( glm::vec3 const & size, uint32_t color )
{
    float32_t const dx = 0.5f*size.x;
    float32_t const dy = 0.5f*size.y;
    float32_t const dz = 0.5f*size.z;
    glm::vec3 const A(-dx,-dy,-dz);
    glm::vec3 const B(-dx, dy,-dz);
    glm::vec3 const C( dx, dy,-dz);
    glm::vec3 const D( dx,-dy,-dz);
    glm::vec3 const E(-dx,-dy, dz);
    glm::vec3 const F(-dx, dy, dz);
    glm::vec3 const G( dx, dy, dz);
    glm::vec3 const H( dx,-dy, dz);

    AutoMesh * mesh = new AutoMesh();
    mesh->setBoundingBox( irr::core::aabbox3df(-dx,-dy,-dz, dx,dy,dz) );

    for ( int i = 0; i < 6; ++i )
    {
        AutoMeshBuffer* p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

             if (i==0) { addQuad( *p, E, F, B, A, color ); } // QUAD [1] neg dx
        else if (i==1) { addQuad( *p, E, A, D, H, color ); } // QUAD [2] neg dy
        else if (i==2) { addQuad( *p, A, B, C, D, color ); } // QUAD [3] neg dz
        else if (i==3) { addQuad( *p, D, C, G, H, color ); } // QUAD [4] pos dx
        else if (i==4) { addQuad( *p, B, F, G, C, color ); } // QUAD [5] pos dy
        else if (i==5) { addQuad( *p, H, G, F, E, color ); } // QUAD [6] pos dz

        // bbox
        p->recalculateBoundingBox();

        // add to mesh
        mesh->addAutoMeshBuffer( p, true );
    }

    mesh->setHardwareMappingHint( irr::scene::EHM_STATIC );
    return mesh;
}

#endif // 0

glm::vec3
getHexagonPoint( float w, float h, int i )
{
    switch ( i )
    {
        case 0: return glm::vec3( 0.00f*w, 0.0f, -.50f*h ); // A
        case 1: return glm::vec3( -.50f*w, 0.0f, -.25f*h ); // B
        case 2: return glm::vec3( -.50f*w, 0.0f, 0.25f*h ); // C
        case 3: return glm::vec3( 0.00f*w, 0.0f, 0.50f*h ); // D
        case 4: return glm::vec3( 0.50f*w, 0.0f, 0.25f*h ); // E
        case 5: return glm::vec3( 0.50f*w, 0.0f, -.25f*h ); // F
        default: return glm::vec3( 0,0,0 ); // M
    }
}

AutoMeshBuffer*
createHexagon( float w, float h )
{
    // MeshBuffer
    AutoMeshBuffer* p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

    // Vertices:
    auto addVertex = [ p ] ( float32_t x, float32_t y, float32_t z, float32_t u, float32_t v, bool flipV )
    {
        p->Vertices.push_back( irr::video::S3DVertex( x, y, z, 0, 1, 0, 0xFFFFFFEE, u, (flipV) ? 1.0f - v : v ) );
    };

    p->Vertices.reserve( 6 );
	 //p->Vertices.reallocate(6);
    //p->Vertices.set_used(0);
    addVertex( 0.00f*w, 0.0f, -.50f*h, 0.50f, 0.00f, true ); // A = 0
    addVertex( -.50f*w, 0.0f, -.25f*h, 0.00f, 0.25f, true ); // B = 1
    addVertex( -.50f*w, 0.0f, 0.25f*h, 0.00f, 0.75f, true ); // C = 2
    addVertex( 0.00f*w, 0.0f, 0.50f*h, 0.50f, 1.00f, true ); // D = 3
    addVertex( 0.50f*w, 0.0f, 0.25f*h, 1.00f, 0.75f, true ); // E = 4
    addVertex( 0.50f*w, 0.0f, -.25f*h, 1.00f, 0.25f, true ); // F = 5

    // Indices ( triangles: ABF, BCE, BEF, CDE ):
    auto addTriangle = [ p ] ( uint16_t a, uint16_t b, uint16_t c )
    {
        p->Indices.push_back( a );
        p->Indices.push_back( b );
        p->Indices.push_back( c );
    };

	 p->Indices.reserve( 12 );
    //p->Indices.reallocate( 12 );
    //p->Indices.set_used( 0 );
    addTriangle( 0,1,5 ); // ABF
    addTriangle( 1,2,4 ); // BCE
    addTriangle( 1,4,5 ); // BEF
    addTriangle( 2,3,4 ); // CDE

    // Return MeshBuffer
    return p;
}

AutoMeshBuffer*
createHafenXZ( float w, float h, int i )
{
    // MeshBuffer
    AutoMeshBuffer* p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

    // Vertices:
    auto addVertex = [ p ] ( float32_t x, float32_t y, float32_t z, float32_t u, float32_t v, bool flipV )
    {
        p->Vertices.push_back( irr::video::S3DVertex( x, y, z, 0, 1, 0, 0xFFFFFFEE, u, (flipV) ? 1.0f - v : v ) );
    };

	 p->Vertices.reserve( 6 );
    // p->Vertices.reallocate(6);
    // p->Vertices.set_used(0);
    addVertex( 0.00f*w, 0.0f, -.50f*h, 0.50f, 0.00f, true ); // A = 0
    addVertex( -.50f*w, 0.0f, -.25f*h, 0.00f, 0.25f, true ); // B = 1
    addVertex( -.50f*w, 0.0f, 0.25f*h, 0.00f, 0.75f, true ); // C = 2
    addVertex( 0.00f*w, 0.0f, 0.50f*h, 0.50f, 1.00f, true ); // D = 3
    addVertex( 0.50f*w, 0.0f, 0.25f*h, 1.00f, 0.75f, true ); // E = 4
    addVertex( 0.50f*w, 0.0f, -.25f*h, 1.00f, 0.25f, true ); // F = 5

    // Indices ( triangles: ABF, BCE, BEF, CDE ):
    auto addTriangle = [ p ] ( uint16_t a, uint16_t b, uint16_t c )
    {
        p->Indices.push_back( a );
        p->Indices.push_back( b );
        p->Indices.push_back( c );
    };

	 p->Indices.reserve( 12 );
    // p->Indices.reallocate( 12 );
    // p->Indices.set_used( 0 );
    addTriangle( 0,1,5 ); // ABF
    addTriangle( 1,2,4 ); // BCE
    addTriangle( 1,4,5 ); // BEF
    addTriangle( 2,3,4 ); // CDE

    // Return MeshBuffer
    return p;
}


AutoMeshBuffer*
createCircle( glm::vec3 const & pos, float r, uint32_t segments, uint32_t color )
{
    assert( r > 0.0f );

    assert( segments >= 3 );

    float const phi = float( 2.0 * M_PI ) / float(segments);

    AutoMeshBuffer* p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

	 p->Vertices.reserve( 1 + segments );
	 
    // push center of circle as first vertex
    p->Vertices.push_back( irr::video::S3DVertex( pos.x, pos.y, pos.z, 0, 0, -1, color, 0.5f, 0.5f ) );

    for ( uint32_t i = 0; i < segments; ++i )
    {
        float const w = phi * i;
        float const s = sinf( w );
        float const c = cosf( w );
        float const x = pos.x + (r * s);
        float const y = pos.y + (r * c);
        float const z = pos.z;
        float const u = 0.5f + ( 0.5f * s );
        float const v = 0.5f - ( 0.5f * c );
        p->Vertices.push_back( irr::video::S3DVertex( x, y, z, 0, 0, -1, color, u, v ) );
    }

    p->recalculateBoundingBox();

    // IndexList: Connect vertex-list as triangles ( center-point always reused and two neighbours )
    // so at this point an indexed based mesh is already better than a trianglelist for a circle.

    auto addTriangle = [ p ] ( uint16_t a, uint16_t b, uint16_t c )
    {
        p->Indices.push_back( a );
        p->Indices.push_back( b );
        p->Indices.push_back( c );
    };

	 p->Indices.reserve( 3 * ( 1 + segments ) );
	 
    for ( uint32_t i = 1; i <= segments; ++i )
    {
        if ( i == segments )
        {
            addTriangle( 0,i,1 ); // center point M, Ai, Ai+1
        }
        else
        {
            addTriangle( 0,i,i+1 ); // center point M, Ai, Ai+1
        }
    }

    return p;
}

AutoMeshBuffer*
createCircleXZ( glm::vec3 const & pos, float r, uint32_t segments )
{
    assert( r > 0.0f );
    assert( segments >= 3 );

    AutoMeshBuffer* p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );
    assert( p );

    float const phi = float( 2.0 * M_PI ) / float(segments);
    assert( phi > 0.001f ); // If not, then fails because of too many vertices for one circle

    auto addVertex = [ p, &pos ] ( float32_t x, float32_t y, float32_t z, float32_t u, float32_t v )
    {
        p->Vertices.push_back( irr::video::S3DVertex( pos.x + x, pos.y + y, pos.z + z, 0, 1, 0, 0xFFFFFFEE, u, v ) );
    };

    // push 3d center point of circle as first vertex
    addVertex( 0.0f, 0.0f, 0.0f, 0.5f, 0.5f );

    for ( uint32_t i = 0; i < segments; ++i )
    {
        float const w = phi * i;
        float const s = sinf( w );
        float const c = cosf( w );
        float const u = 0.5f + ( 0.5f * s );
        float const v = 0.5f - ( 0.5f * c );
        addVertex( (r * s), 0.0f, (r * c), u, v );
    }

    p->recalculateBoundingBox();

    // IndexList: Connect vertex-list as triangles ( center-point always reused and two neighbours )
    // so at this point an indexed based mesh is already better than a trianglelist for a circle.

    auto addTriangle = [ p ] ( uint16_t a, uint16_t b, uint16_t c )
    {
        p->Indices.push_back( a );
        p->Indices.push_back( b );
        p->Indices.push_back( c );
    };

    for ( uint32_t i = 1; i <= segments; ++i )
    {
        if ( i == segments )
        {
            addTriangle( 0,i,1 ); // center point M, Ai, Ai+1
        }
        else
        {
            addTriangle( 0,i,i+1 ); // center point M, Ai, Ai+1
        }
    }

    return p;
}


/// @brief Create a cylinder hull meshbuffer.

AutoMeshBuffer*
createCylinderHull(
    glm::vec3 pos,
    float radius,
    float height,
    uint32_t color,
    uint32_t tessCircle,
    uint32_t tessHull)
{
    AutoMeshBuffer* p = new AutoMeshBuffer();

    assert( p );

    p->PrimitiveType = irr::scene::EPT_TRIANGLES;

    // common vars and constants
    float const y0 = 0.5f*height;				// top
    float const y1 = -0.5f*height;			// bottom
    float const sx = 1.f / (float)tessCircle;   // value step for circle
    float const sy = 1.f / (float)tessHull;		// value step for hull

    // bbox
    p->BoundingBox.reset( irr::core::aabbox3df(-radius,y1,-radius, radius,y0,radius) );

    // points

    SinCosTablef sincosTable( tessCircle );

    std::vector< glm::vec3 > vertexData;

    for (uint32_t y=0; y<=tessHull; y++)
    {
        for (uint32_t x=0; x<=tessCircle; x++)
        {
            float const s = sincosTable[x].s;
            float const c = sincosTable[x].c;
            vertexData.emplace_back( pos + glm::vec3( radius*s, y1 + height*sy*y, radius*c ) );
        }
    }

    // vertices

    uint32_t const pitch = tessCircle + 1; // vertices per row to skip

    for (uint32_t j = 0; j < tessHull; ++j )
    {
        for (uint32_t i = 0; i < tessCircle; ++i )
        {
            uint32_t const iA = i + j*pitch;
            uint32_t const iB = i + (j+1)*pitch;
            uint32_t const iC = (i+1) + (j+1)*pitch;
            uint32_t const iD = (i+1) + j*pitch;

            glm::vec3 const & D = vertexData[ iA ];
            glm::vec3 const & C = vertexData[ iB ];
            glm::vec3 const & B = vertexData[ iC ];
            glm::vec3 const & A = vertexData[ iD ];

            addQuad( *p, A, B, C, D, color );
        }
    }

    return p;
}

uint32_t
getVertexCount(
   AutoMeshBuffer & buffer )
{
   return buffer.Vertices.size();
}

uint32_t
getIndexCount(
   AutoMeshBuffer & buffer )
{
   return buffer.Indices.size();
}

void
addVertex(
   AutoMeshBuffer & buffer,
   glm::vec3 const & p,
   glm::vec3 const & n,
   uint32_t color,
   glm::vec2 const & uv, bool flipV )
{
   buffer.Vertices.push_back( irr::video::S3DVertex( p.x, p.y, p.z, n.x, n.y, n.z, irr::video::SColor( color ), uv.x, (flipV) ? 1.0f - uv.y : uv.y ) );
   //buffer.BoundingBox.addInternalPoint( p.x, p.y, p.z );
}

void
addIndex(
   AutoMeshBuffer & buffer,
   uint32_t index )
{
   // assert( index < 65536 );
   buffer.Indices.push_back( index );
}

void
addIndexedTriangle(
   AutoMeshBuffer & buffer,
   uint32_t indexA, uint32_t indexB, uint32_t indexC )
{
   // assert( indexA < 65536 );
   // assert( indexB < 65536 );
   // assert( indexC < 65536 );
   buffer.Indices.push_back( indexA );
   buffer.Indices.push_back( indexB );
   buffer.Indices.push_back( indexC );
}

void
addTriangle(
   AutoMeshBuffer & buffer,
   glm::vec3 const & A,
   glm::vec3 const & B,
   glm::vec3 const & C,
   uint32_t color,
   glm::vec2 const & uvA,
   glm::vec2 const & uvB,
   glm::vec2 const & uvC,
   bool flipV )
{
   uint32_t const v = getVertexCount( buffer );
   glm::vec3 const n = glm::normalize( glm::cross( B - A, C - A ) );
   addVertex( buffer, A, n, color, uvA, flipV );
   addVertex( buffer, B, n, color, uvB, flipV );
   addVertex( buffer, C, n, color, uvC, flipV );
   addIndexedTriangle( buffer, v, v + 1, v + 2 );
}

void
addTriangle(
   AutoMeshBuffer & buffer,
   glm::vec3 const & A,
   glm::vec3 const & B,
   glm::vec3 const & C,
   glm::vec3 const & n,
   uint32_t color,
   glm::vec2 const & uvA,
   glm::vec2 const & uvB,
   glm::vec2 const & uvC,
   bool flipV )
{
   uint32_t const v = getVertexCount( buffer );
   addVertex( buffer, A, n, color, uvA, flipV );
   addVertex( buffer, B, n, color, uvB, flipV );
   addVertex( buffer, C, n, color, uvC, flipV );
   addIndexedTriangle( buffer, v, v + 1, v + 2 );
}

void
addQuad(
   AutoMeshBuffer & buffer,
   glm::vec3 const & A,
   glm::vec3 const & B,
   glm::vec3 const & C,
   glm::vec3 const & D,
   uint32_t color,
   bool flipV )
{
   uint32_t const v = getVertexCount( buffer );
   glm::vec3 const n = ( getNormal( A, B, C ) + getNormal( A, C, D ) ) * 0.5f;
   addVertex( buffer, A, n, color, glm::vec2( 0.0f, 1.0f ), flipV );
   addVertex( buffer, B, n, color, glm::vec2( 0.0f, 0.0f ), flipV );
   addVertex( buffer, C, n, color, glm::vec2( 1.0f, 0.0f ), flipV );
   addVertex( buffer, D, n, color, glm::vec2( 1.0f, 1.0f ), flipV );
   addIndexedTriangle( buffer, v, v + 1, v + 2 ); // ABC
   addIndexedTriangle( buffer, v, v + 2, v + 3 ); // ACD
}

void addRect( AutoMeshBuffer* p, glm::vec3 pos, glm::vec2 size, uint32_t color)
{
float32_t dx = 0.5f*size.x;   // bbox
float32_t dy = 0.5f*size.y;   // bbox
uint32_t const n = p->Vertices.size();
p->Vertices.push_back( irr::video::S3DVertex( pos.x - dx, pos.y - dy, pos.z, 0, 0, -1, color, 0.0f, 1.0f ) ); // A=0
p->Vertices.push_back( irr::video::S3DVertex( pos.x - dx, pos.y + dy, pos.z, 0, 0, -1, color, 0.0f, 0.0f ) ); // B=1
p->Vertices.push_back( irr::video::S3DVertex( pos.x + dx, pos.y + dy, pos.z, 0, 0, -1, color, 1.0f, 0.0f ) ); // C=2
p->Vertices.push_back( irr::video::S3DVertex( pos.x + dx, pos.y - dy, pos.z, 0, 0, -1, color, 1.0f, 1.0f ) ); // D=3
p->Indices.push_back( n + 0 );
p->Indices.push_back( n + 1 );
p->Indices.push_back( n + 2 );
p->Indices.push_back( n + 0 );
p->Indices.push_back( n + 2 );
p->Indices.push_back( n + 3 );
}

//  F  G
// /E /H
// B--C
// |/ |
// A--D

/// @brief Create a box with one repeating texture over all 6 planes
void
addBox( AutoMeshBuffer & p, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color )
{
 glm::vec3 const d = size * 0.5f;
 glm::vec3 const A = pos + glm::vec3( -d.x, -d.y, -d.z );
 glm::vec3 const B = pos + glm::vec3( -d.x,  d.y, -d.z );
 glm::vec3 const C = pos + glm::vec3(  d.x,  d.y, -d.z );
 glm::vec3 const D = pos + glm::vec3(  d.x, -d.y, -d.z );
 glm::vec3 const E = pos + glm::vec3( -d.x, -d.y,  d.z );
 glm::vec3 const F = pos + glm::vec3( -d.x,  d.y,  d.z );
 glm::vec3 const G = pos + glm::vec3(  d.x,  d.y,  d.z );
 glm::vec3 const H = pos + glm::vec3(  d.x, -d.y,  d.z );
 addQuad( p, E, F, B, A, color ); // n(-1, 0, 0);    // Neg X = EFBA
 addQuad( p, E, A, D, H, color ); // n( 0,-1, 0);    // Neg Y = EADH
 addQuad( p, A, B, C, D, color ); // n( 0, 0, -1);   // Neg Z = ABCD
 addQuad( p, D, C, G, H, color ); // n( 1, 0, 0);    // Pos X = DCGH
 addQuad( p, B, F, G, C, color ); // n( 0, 1, 0);    // Pos Y = BFGC
 addQuad( p, H, G, F, E, color ); // n( 0, 0, 1);    // Pos Z = HGFE
}

/// @brief Create a box with one repeating texture over all 6 planes
void
addBoxPlane( AutoMeshBuffer & p, int i, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color )
{
 float const dx = 0.5f*size.x;
 float const dy = 0.5f*size.y;
 float const dz = 0.5f*size.z;
 glm::vec3 const A = pos + glm::vec3(-dx,-dy,-dz);
 glm::vec3 const B = pos + glm::vec3(-dx, dy,-dz);
 glm::vec3 const C = pos + glm::vec3( dx, dy,-dz);
 glm::vec3 const D = pos + glm::vec3( dx,-dy,-dz);
 glm::vec3 const E = pos + glm::vec3(-dx,-dy, dz);
 glm::vec3 const F = pos + glm::vec3(-dx, dy, dz);
 glm::vec3 const G = pos + glm::vec3( dx, dy, dz);
 glm::vec3 const H = pos + glm::vec3( dx,-dy, dz);

      if ( i == 0 ) addQuad( p, E, F, B, A, color ); // n(-1, 0, 0); // Neg X = EFBA
 else if ( i == 1 ) addQuad( p, E, A, D, H, color ); // n( 0,-1, 0); // Neg Y = EADH
 else if ( i == 2 ) addQuad( p, A, B, C, D, color ); // n( 0, 0,-1); // Neg Z = ABCD
 else if ( i == 3 ) addQuad( p, D, C, G, H, color ); // n( 1, 0, 0); // Pos X = DCGH
 else if ( i == 4 ) addQuad( p, B, F, G, C, color ); // n( 0, 1, 0); // Pos Y = BFGC
 else if ( i == 5 ) addQuad( p, H, G, F, E, color ); // n( 0, 0, 1); // Pos Z = HGFE
}

/// @brief Create a box
void
addBoxWithoutTop( AutoMeshBuffer & p, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color )
{
 float const dx = 0.5f*size.x;
 float const dy = 0.5f*size.y;
 float const dz = 0.5f*size.z;
 glm::vec3 const A = pos + glm::vec3(-dx,-dy,-dz);
 glm::vec3 const B = pos + glm::vec3(-dx, dy,-dz);
 glm::vec3 const C = pos + glm::vec3( dx, dy,-dz);
 glm::vec3 const D = pos + glm::vec3( dx,-dy,-dz);
 glm::vec3 const E = pos + glm::vec3(-dx,-dy, dz);
 glm::vec3 const F = pos + glm::vec3(-dx, dy, dz);
 glm::vec3 const G = pos + glm::vec3( dx, dy, dz);
 glm::vec3 const H = pos + glm::vec3( dx,-dy, dz);

 addQuad( p, E, F, B, A, color ); // n(-1, 0, 0);    // Neg X = EFBA
 addQuad( p, E, A, D, H, color ); // n( 0,-1, 0);    // Neg Y = EADH
 addQuad( p, A, B, C, D, color ); // n( 0, 0, -1);   // Neg Z = ABCD
 addQuad( p, D, C, G, H, color ); // n( 1, 0, 0);    // Pos X = DCGH
 //addQuad( p, B, F, G, C, color ); // n( 0, 1, 0);    // Pos Y = BFGC
 addQuad( p, H, G, F, E, color ); // n( 0, 0, 1);    // Pos Z = HGFE
}

/// @brief Create a box
void
addBoxWithoutTopAndBottom( AutoMeshBuffer & p, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color )
{
 float const dx = 0.5f*size.x;
 float const dy = 0.5f*size.y;
 float const dz = 0.5f*size.z;
 glm::vec3 const A = pos + glm::vec3(-dx,-dy,-dz);
 glm::vec3 const B = pos + glm::vec3(-dx, dy,-dz);
 glm::vec3 const C = pos + glm::vec3( dx, dy,-dz);
 glm::vec3 const D = pos + glm::vec3( dx,-dy,-dz);
 glm::vec3 const E = pos + glm::vec3(-dx,-dy, dz);
 glm::vec3 const F = pos + glm::vec3(-dx, dy, dz);
 glm::vec3 const G = pos + glm::vec3( dx, dy, dz);
 glm::vec3 const H = pos + glm::vec3( dx,-dy, dz);

 addQuad( p, E, F, B, A, color ); // n(-1, 0, 0);    // Neg X = EFBA
 //addQuad( p, E, A, D, H, color ); // n( 0,-1, 0);    // Neg Y = EADH
 addQuad( p, A, B, C, D, color ); // n( 0, 0, -1);   // Neg Z = ABCD
 addQuad( p, D, C, G, H, color ); // n( 1, 0, 0);    // Pos X = DCGH
 //addQuad( p, B, F, G, C, color ); // n( 0, 1, 0);    // Pos Y = BFGC
 addQuad( p, H, G, F, E, color ); // n( 0, 0, 1);    // Pos Z = HGFE
}

} // end namespace irrExt