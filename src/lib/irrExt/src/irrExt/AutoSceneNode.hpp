#ifndef IRREXT_AUTOSCENENODE_HPP
#define IRREXT_AUTOSCENENODE_HPP

#include <irrExt/AutoMeshBuffer.hpp>

namespace irrExt {

	
	
void SceneNode_setPosition( irr::scene::ISceneNode* node, glm::vec3 const & pos );
void SceneNode_setRotation( irr::scene::ISceneNode* node, glm::vec3 const & degrees );

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

} // end namespace irrExt

#endif // IRREXT_AUTOSCENENODE_HPP
