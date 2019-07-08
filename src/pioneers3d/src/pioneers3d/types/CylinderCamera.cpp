#include <pioneers3d/types/CylinderCamera.hpp>
#include <pioneers3d/types/Game.hpp>

namespace pioneers3d {

// ==============================================================
// Camera controls
// ==============================================================

irr::scene::ICameraSceneNode*
Game_getCamera( Game_t * game )
{
   if ( !game ) return nullptr;
   if ( !game->Device ) return nullptr;
   irr::scene::ICameraSceneNode* camera = game->Device->getSceneManager()->getActiveCamera();
   return camera;
}

void Camera_update( Game_t * game )
{

}


void
Camera_moveToDefault( Game_t * game )
{
   irr::scene::ICameraSceneNode* camera = Game_getCamera( game );
   if ( !camera ) return;
   glm::vec3 A = Camera_getPosition( camera );
   glm::vec3 B = glm::vec3( -50.0f, 100.0f, -200.0f );
   Camera_setPosition( camera, curveVector( A, B ) );
   Camera_setLookAt( camera, glm::vec3( 0, 1, 0 ) );
}

void
Camera_moveToTop( Game_t * game )
{
   irr::scene::ICameraSceneNode* camera = Game_getCamera( game );
   if ( !camera ) return;
   glm::vec3 A = Camera_getPosition( camera );
   glm::vec3 B = glm::vec3( 0.0f, 400.0f, 0.0f );
   Camera_setPosition( camera, curveVector( A, B ) );
   Camera_setLookAt( camera, glm::vec3( 0, 1, 0 ) );
}

void
Camera_moveToBottom( Game_t * game )
{
   irr::scene::ICameraSceneNode* camera = Game_getCamera( game );
   if ( !camera ) return;
   glm::vec3 A = Camera_getPosition( camera );
   glm::vec3 B = glm::vec3( -100.0f, 100.0f, -100.0f );
   Camera_setPosition( camera, curveVector( A, B ) );
   Camera_setLookAt( camera, glm::vec3( 0, 1, 0 ) );
}

void
Camera_moveToFront( Game_t * game )
{
   irr::scene::ICameraSceneNode* camera = Game_getCamera( game );
   if ( !camera ) return;
   glm::vec3 A = Camera_getPosition( camera );
   glm::vec3 B = glm::vec3( 0.0f, 100.0f, -400.0f );
   Camera_setPosition( camera, curveVector( A, B ) );
   Camera_setLookAt( camera, glm::vec3( 0, 1, 0 ) );
}

void
Camera_moveToBack( Game_t * game )
{
   irr::scene::ICameraSceneNode* camera = Game_getCamera( game );
   if ( !camera ) return;
   glm::vec3 A = Camera_getPosition( camera );
   glm::vec3 B = glm::vec3( 0.0f, 100.0f, 400.0f );
   Camera_setPosition( camera, curveVector( A, B ) );
   Camera_setLookAt( camera, glm::vec3( 0, 1, 0 ) );
}

void
Camera_moveToLeft( Game_t * game )
{
   irr::scene::ICameraSceneNode* camera = Game_getCamera( game );
   if ( !camera ) return;
   glm::vec3 A = Camera_getPosition( camera );
   glm::vec3 B = glm::vec3( -400.0f, 100.0f, 0.0f );
   Camera_setPosition( camera, curveVector( A, B ) );
   Camera_setLookAt( camera, glm::vec3( 0, 1, 0 ) );
}

void
Camera_moveToRight( Game_t * game )
{
   irr::scene::ICameraSceneNode* camera = Game_getCamera( game );
   if ( !camera ) return;
   glm::vec3 A = Camera_getPosition( camera );
   glm::vec3 B = glm::vec3( 400.0f, 100.0f, 0.0f );
   Camera_setPosition( camera, curveVector( A, B ) );
   Camera_setLookAt( camera, glm::vec3( 0, 1, 0 ) );
}

void
Game_setCameraInput( Game_t * game, bool enable )
{
   irr::scene::ICameraSceneNode* camera = game->Device->getSceneManager()->getActiveCamera();
   if ( !camera ) return;
   camera->setInputReceiverEnabled( enable );
}

// ==============================================================
// Camera helpers
// ==============================================================

glm::vec3
Camera_getPosition( irr::scene::ICameraSceneNode* camera )
{
   if ( !camera ) return glm::vec3(0,0,0);
   auto p = camera->getAbsolutePosition();
   return glm::vec3( p.X, p.Y, p.Z );
}

glm::vec3
Camera_getLookAt( irr::scene::ICameraSceneNode* camera )
{
   if ( !camera ) return glm::vec3(0,0,0);
   auto p = camera->getTarget();
   return glm::vec3( p.X, p.Y, p.Z );
}

void
Camera_setPosition( irr::scene::ICameraSceneNode* camera, glm::vec3 pos )
{
   if ( !camera ) return;
   camera->setPosition( irr::core::vector3df( pos.x, pos.y, pos.z ) );
}
void
Camera_setLookAt( irr::scene::ICameraSceneNode* camera, glm::vec3 eye )
{
   if ( !camera ) return;
   camera->setTarget( irr::core::vector3df( eye.x, eye.y, eye.z ) );
}

#if 0
void Game_resetCamera( Game_t * game )
{

}

void Game_setCameraTopView( Game_t * game )
{

}

void Game_moveCameraLeft( Game_t * game )
{

}

void Game_moveCameraRight( Game_t * game )
{

}

void Game_moveCameraUp( Game_t * game )
{

}

void Game_moveCameraDown( Game_t * game )
{

}

void Game_moveCameraForward( Game_t * game )
{

}

void Game_moveCameraBack( Game_t * game )
{

}

bool
Game_isCameraInput( Game_t * game )
{
    irr::IrrlichtDevice* device = game->Device;
    irr::scene::ICameraSceneNode* camera = device->getSceneManager()->getActiveCamera();
    if ( camera )
    {
        return camera->isInputReceiverEnabled();
    }
    else
    {
        return false;
    }
}

void
Game_toggleCameraInput( Game_t * game )
{
    irr::IrrlichtDevice* device = game->Device;
    irr::scene::ICameraSceneNode* camera = device->getSceneManager()->getActiveCamera();
    if ( camera )
    {
        camera->setInputReceiverEnabled( !camera->isInputReceiverEnabled() );
    }
}



#endif

} // end namespace pioneers3d
