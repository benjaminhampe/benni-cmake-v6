#include "PrimitiveRenderer.hpp"

PrimitiveRenderer2D::PrimitiveRenderer2D( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent )
     : AutoSceneNode( smgr, parent )
     , m_Current( nullptr )
     , m_zIndex( 0.0f )
{}
