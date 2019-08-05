#ifndef IRREXT_AUTOMESHBUFFER_HPP
#define IRREXT_AUTOMESHBUFFER_HPP

#include <irrExt/GlmUtils.hpp>

namespace irrExt {

void
enumerateMeshBuffer( irr::scene::SMeshBuffer & p );

//=======================================================================================
//
// CLASS: AutoMeshBuffer
//
//=======================================================================================

class AutoMeshBuffer : public irr::scene::IMeshBuffer
{
public:
   irr::scene::E_PRIMITIVE_TYPE PrimitiveType;	// We need this to enable auto rendering
   //irr::video::E_VERTEX_TYPE VertexType;		// We need this to enable auto rendering
   //irr::video::E_INDEX_TYPE IndexType;			// We need this to enable auto rendering
   //irr::scene::SMeshBuffer MeshBuffer;			// The traditional irrlicht meshbuffer ( aka non-auto, aka broken )
   irr::u32 ChangedID_Vertex;
   irr::u32 ChangedID_Index;
   //! hardware mapping hint
   irr::scene::E_HARDWARE_MAPPING MappingHint_Vertex;
   irr::scene::E_HARDWARE_MAPPING MappingHint_Index;
   //! Material for this meshbuffer.
   irr::video::SMaterial Material;
   //! Vertices of this buffer
   std::vector< irr::video::S3DVertex > Vertices;
   //! Indices into the vertices of this buffer.
   std::vector< uint32_t > Indices;
   //! Bounding box of this meshbuffer.
   irr::core::aabbox3df BoundingBox;

public:
   static irr::video::SMaterial
   createDefaultMaterial()
   {
      irr::video::SMaterial defaultMaterial;
      defaultMaterial.MaterialType = irr::video::EMT_SOLID;
      defaultMaterial.Lighting = false;
      defaultMaterial.FogEnable = false;
      return defaultMaterial;
   }

   /// @brief
   static uint32_t
   getPrimitiveCount( irr::scene::E_PRIMITIVE_TYPE primitiveType, uint32_t indexCount );

   /// @brief Default constructor
   AutoMeshBuffer();

   /// @brief Value constructor
   AutoMeshBuffer( irr::scene::E_PRIMITIVE_TYPE primType );

   /// @brief Destructor
   //~AutoMeshBuffer() override;

   /// @brief Is there some vertex.Color.getAlpha() below 255
   bool hasTransparentVertexColor() const;

   /// @brief We have all infos, but a material, to render this geometry/indices only buffer
   void render( irr::video::IVideoDriver* driver );

   /// @brief Add collision detection
   bool getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const;


   /// @brief
   irr::scene::E_PRIMITIVE_TYPE getPrimitiveType() const /* override */ { return PrimitiveType; }

   /// @brief
   uint32_t getPrimitiveCount() const /* override */ { return AutoMeshBuffer::getPrimitiveCount( PrimitiveType, getIndexCount() ); }

   //! Get the material of this meshbuffer
   /** \return Material of this buffer. */
   irr::video::SMaterial & getMaterial() override { return Material; }

   //! Get the material of this meshbuffer
   /** \return Material of this buffer. */
   irr::video::SMaterial const & getMaterial() const override { return Material; }

   /// @brief Get type of vertex data which is stored in this meshbuffer.
   /** \return Vertex type of this buffer. */
   irr::video::E_VERTEX_TYPE getVertexType() const override { return irr::video::EVT_STANDARD; }

   /// @brief Get amount of vertices in meshbuffer.
   /** \return Number of vertices in this buffer. */
   irr::u32 getVertexCount() const override { return irr::u32( Vertices.size() ); }

   /// @brief Get access to vertex data. The data is an array of vertices.
   /** Which vertex type is used can be determined by getVertexType().
   \return Pointer to array of vertices. */
   void const * getVertices() const override { return Vertices.data(); }

   /// @brief Get access to vertex data. The data is an array of vertices.
   /** Which vertex type is used can be determined by getVertexType().
   \return Pointer to array of vertices. */
   void * getVertices() override { return Vertices.data(); }

   /// @brief Get type of index data which is stored in this meshbuffer.
   /** \return Index type of this buffer. */
   irr::video::E_INDEX_TYPE getIndexType() const override { return irr::video::EIT_32BIT; }

   /// @brief Get number of indices
   /** \return Number of indices. */
   irr::u32 getIndexCount() const override { return irr::u32( Indices.size() ); }

   /// @brief Get access to Indices.
   /** \return Pointer to indices array. */
   irr::u16 const * getIndices() const override { return reinterpret_cast< irr::u16 const * >( Indices.data() ); }

   //! Get access to Indices.
   /** \return Pointer to indices array. */
   irr::u16 * getIndices() override { return reinterpret_cast< irr::u16 * >( Indices.data() ); }

   /// @brief Get the axis aligned bounding box of this meshbuffer.
   /** \return Axis aligned bounding box of this buffer. */
   irr::core::aabbox3df const & getBoundingBox() const override { return BoundingBox; }

   /// @brief Set axis aligned bounding box
   /** \param box User defined axis aligned bounding box to use
   for this buffer. */
   void setBoundingBox( irr::core::aabbox3df const & bbox ) override { BoundingBox = bbox; }

   /// @brief Recalculates the bounding box. Should be called if the mesh changed.
   void recalculateBoundingBox() override
   {
      if ( !getVertexCount() )
      {
         BoundingBox.reset( 0.f, 0.f, 0.f );
      }
      else
      {
         BoundingBox.reset( Vertices[ 0 ].Pos );
         for ( uint32_t i = 1; i < getVertexCount(); ++i )
         {
            BoundingBox.addInternalPoint( Vertices[ i ].Pos );
         }
      }
   }

   /// @brief Get the currently used ID for identification of changes.
   /** This shouldn't be used for anything outside the VideoDriver. */
   uint32_t getChangedID_Vertex() const override { return ChangedID_Vertex; }

   /// @brief Get the currently used ID for identification of changes.
   /** This shouldn't be used for anything outside the VideoDriver. */
   uint32_t getChangedID_Index() const override { return ChangedID_Index; }

   /// @brief Get the current hardware mapping hint
   irr::scene::E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const override { return MappingHint_Vertex; }

   /// @brief Get the current hardware mapping hint
   irr::scene::E_HARDWARE_MAPPING getHardwareMappingHint_Index() const override { return MappingHint_Index; }

   /// @brief Set the hardware mapping hint, for driver
   void setHardwareMappingHint( irr::scene::E_HARDWARE_MAPPING newMappingHint,
                                irr::scene::E_BUFFER_TYPE buffer = irr::scene::EBT_VERTEX_AND_INDEX ) override
   {}

   /// @brief Flags the meshbuffer as changed, reloads hardware buffers
   void setDirty( irr::scene::E_BUFFER_TYPE buffer = irr::scene::EBT_VERTEX_AND_INDEX ) override {}

   /// @brief Returns position of vertex i
   irr::core::vector3df const & getPosition( uint32_t i ) const override { return Vertices[ i ].Pos; }

   /// @brief Returns position of vertex i
   irr::core::vector3df & getPosition( uint32_t i ) override { return Vertices[ i ].Pos; }

   /// @brief Returns normal of vertex i
   irr::core::vector3df const & getNormal( uint32_t i ) const override { return Vertices[ i ].Normal; }

   /// @brief Returns normal of vertex i
   irr::core::vector3df & getNormal( uint32_t i ) override { return Vertices[ i ].Normal; }

   /// @brief Returns texture coord of vertex i
   irr::core::vector2df const & getTCoords( uint32_t i ) const override { return Vertices[ i ].TCoords; }

   /// @brief Returns texture coord of vertex i
   irr::core::vector2df & getTCoords( uint32_t i ) override { return Vertices[ i ].TCoords; }

   /// @brief Append the meshbuffer to the current buffer
   /** Only works for compatible vertex types
   \param other Buffer to append to this one. */
   void append( irr::scene::IMeshBuffer const * const other ) override
   {}

   /// @brief Append the vertices and indices to the current buffer
   /** Only works for compatible vertex types.
   \param vertices Pointer to a vertex array.
   \param numVertices Number of vertices in the array.
   \param indices Pointer to index array.
   \param numIndices Number of indices in array. */
   void append(   void const * const vertices,
                  irr::u32 numVertices,
                  irr::u16 const * const indices,
                  irr::u32 numIndices ) override
   {}


};

} // end namespace irrExt

#endif // IRREXT_AUTOMESHBUFFER_HPP

#if 0

//=======================================================================================
//
// CLASS: AutoMesh
//
//=======================================================================================

class AutoMesh : public irr::scene::IMesh
{
public:
    //! constructor
    AutoMesh();

    //! destructor
    ~AutoMesh() override;

    //! clean mesh
    void clear();

    virtual irr::u32 getMaterialCount() const;

    virtual irr::video::SMaterial & getMaterial( irr::u32 i );

    //! returns amount of mesh buffers.
    virtual irr::u32 getMeshBufferCount() const override;

    //! returns pointer to a mesh buffer
    virtual irr::scene::IMeshBuffer* getMeshBuffer( irr::u32 i ) const override;

    //! returns a meshbuffer which fits a material
    /** reverse search */
    virtual irr::scene::IMeshBuffer* getMeshBuffer( irr::video::SMaterial const & material ) const override;

    //! returns an axis aligned bounding box
    virtual irr::core::aabbox3d<irr::f32> const & getBoundingBox() const override;

    //! set user axis aligned bounding box
    virtual void setBoundingBox( irr::core::aabbox3df const & box ) override;

    //! recalculates the bounding box
    virtual void recalculateBoundingBox(); // override

    //! adds a MeshBuffer
    /** The bounding box is not updated automatically. */
    virtual void addMeshBuffer( irr::scene::IMeshBuffer* buf ); // override

    //! adds a MeshBuffer
    /** The bounding box is not updated automatically. */
    virtual void addAutoMeshBuffer( AutoMeshBuffer* buf, bool dropAfter = false ); // override

    //! sets a flag of all contained materials to a new value
    virtual void setMaterialFlag( irr::video::E_MATERIAL_FLAG flag, bool newvalue) override;

    //! set the hardware mapping hint, for driver
    virtual void setHardwareMappingHint( irr::scene::E_HARDWARE_MAPPING newMappingHint, irr::scene::E_BUFFER_TYPE buffer = irr::scene::EBT_VERTEX_AND_INDEX ) override;

    //! flags the meshbuffer as changed, reloads hardware buffers
    virtual void setDirty( irr::scene::E_BUFFER_TYPE buffer = irr::scene::EBT_VERTEX_AND_INDEX ) override;

    //! The meshbuffers of this mesh
    std::vector< AutoMeshBuffer* > MeshBuffers;

    //! The bounding box of this mesh
    irr::core::aabbox3df BoundingBox;

    //! What we return on invalid index
    irr::video::SMaterial DefaultMaterial;

    /// @brief Add collision detection
    bool getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const;
};


//=======================================================================================
//
// CLASS: AutoSceneNode
//
//=======================================================================================

class AutoSceneNode : public irr::scene::ISceneNode
{
public:
   /// @brief Constructor
   AutoSceneNode( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent, int id = -1 );

   /// @brief Destructor
   ~AutoSceneNode() override;

// interface: ISceneNode

   /// @brief This method is called just before the rendering process of the whole scene.
   /** Nodes may register themselves in the render pipeline during this call,
   precalculate the geometry which should be renderered, and prevent their
   children from being able to register themselves if they are clipped by simply
   not calling their OnRegisterSceneNode method.
   If you are implementing your own scene node, you should overwrite this method
   with an implementation code looking like this:
   \code
   if (IsVisible)
      SceneManager->registerNodeForRendering(this);

   ISceneNode::OnRegisterSceneNode();
   \endcode
   */
   void OnRegisterSceneNode() override;

   /// @brief Renders the node.
   void render() override;

   /// @brief Get the axis aligned, not transformed bounding box of this node.
   /** This means that if this node is an animated 3d character,
   moving in a room, the bounding box will always be around the
   origin. To get the box in real world coordinates, just
   transform it with the matrix you receive with
   getAbsoluteTransformation() or simply use
   getTransformedBoundingBox(), which does the same.
   \return The non-transformed bounding box. */
   irr::core::aabbox3df const & getBoundingBox() const override
   {
      return m_BoundingBox;
   }

   /// @brief Get amount of materials used by this scene node.
   /** \return Current amount of materials of this scene node. */
   irr::u32 getMaterialCount() const override
   {
      return getMeshBufferCount();
   }

   /// @brief Returns the material based on the zero based index i.
   /** To get the amount of materials used by this scene node, use
   getMaterialCount(). This function is needed for inserting the
   node into the scene hierarchy at an optimal position for
   minimizing renderstate changes, but can also be used to
   directly modify the material of a scene node.
   \param num Zero based index. The maximal value is getMaterialCount() - 1.
   \return The material at that index. */
   irr::video::SMaterial & getMaterial( irr::u32 i ) override
   {
      assert( i < getMeshBufferCount() );
      return m_MeshBuffer[ i ]->Material;
   }

// interface: AutoSceneNode

   uint32_t getMeshBufferCount() const
   {
      return static_cast< uint32_t >( m_MeshBuffer.size() );
   }

   AutoMeshBuffer* getMeshBuffer( uint32_t i )
   {
      assert( i < getMeshBufferCount() );
      return m_MeshBuffer[ i ];
   }

   void clear()
   {
      std::cout << __FUNCTION__ << "()\n";
      for ( uint32_t i = 0; i < getMeshBufferCount(); ++i )
      {
         AutoMeshBuffer* p = m_MeshBuffer[ i ];
         if ( p ) p->drop();
      }
      m_MeshBuffer.clear();
      m_BoundingBox.reset ( 0.f, 0.f, 0.f );
   }

   void addMeshBuffer( AutoMeshBuffer * meshBuffer, bool dropAfterAdd = false )
   {
      std::cout << __FUNCTION__ << "()\n";
      if ( meshBuffer )
      {
         // tell we like to use it
         meshBuffer->grab();

         // use it
         m_MeshBuffer.emplace_back( meshBuffer );

         // take ownership
         if ( dropAfterAdd )
         {
            meshBuffer->drop();
         }
      }
   }

   /// @brief Add collision detection
   bool getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const;

protected:
   irr::core::aabbox3df m_BoundingBox;

   std::vector< AutoMeshBuffer* > m_MeshBuffer;
};

#endif // 0
