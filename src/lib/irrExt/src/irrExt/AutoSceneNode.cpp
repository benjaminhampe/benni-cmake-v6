#include <irrExt/AutoSceneNode.hpp>

namespace irrExt {

void SceneNode_setPosition( irr::scene::ISceneNode* node, glm::vec3 const & pos )
{
   if ( !node ) return;
   node->setPosition( irr::core::vector3df( pos.x, pos.y, pos.z ) );
}

void SceneNode_setRotation( irr::scene::ISceneNode* node, glm::vec3 const & degrees )
{
   if ( !node ) return;
   node->setRotation( irr::core::vector3df( degrees.x, degrees.y, degrees.z ) );
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
   // std::cout << __FUNCTION__ << "()\n";
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

} // end namespace irrExt
