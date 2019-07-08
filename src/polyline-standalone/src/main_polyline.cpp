#include "EventReceiver.hpp"
#include "PolyLineTest.hpp"
#include <time.h>

int main(int argc, char *argv[])
{
    srand( uint32_t( time( nullptr ) ) );

    EventReceiver receiver;
    irr::SIrrlichtCreationParameters params;
    params.WindowSize = irr::core::dimension2du( 1200,800 );
    params.Bits = 32;
    params.AntiAlias = irr::video::EAAM_QUALITY;
    params.Doublebuffer = true;
    params.Vsync = true;
    params.DriverType = irr::video::EDT_OPENGL;
    params.HighPrecisionFPU = true;
    params.Fullscreen = false;
    params.Stencilbuffer = true;
    params.ZBufferBits = 16;
    params.EventReceiver = &receiver;
    irr::IrrlichtDevice* device = irr::createDeviceEx( params );
    if (!device)
    {
        std::cout << "ERROR: Cannot create device\n";
        return 0;
    }
    device->setResizable();
    receiver.setDevice( device );

    irr::video::IVideoDriver* driver = device->getVideoDriver();
    irr::video::SColor clearColor( 255, 20, 20, 20 );
    irr::scene::ISceneManager* smgr = device->getSceneManager();
    irr::scene::ISceneNode* root = smgr->getRootSceneNode();

    irr::SKeyMap keyMap[6];
    keyMap[0].Action = irr::EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = irr::KEY_KEY_W;
    keyMap[1].Action = irr::EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = irr::KEY_KEY_S;
    keyMap[2].Action = irr::EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = irr::KEY_KEY_A;
    keyMap[3].Action = irr::EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = irr::KEY_KEY_D;
    keyMap[4].Action = irr::EKA_CROUCH;
    keyMap[4].KeyCode = irr::KEY_KEY_C;
    keyMap[5].Action = irr::EKA_JUMP_UP;
    keyMap[5].KeyCode = irr::KEY_KEY_Q;

    irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS( root, 100.0f, 0.025f, -1, keyMap, 6 );
    camera->setPosition( irr::core::vector3df( 0, 0.0f,100.0f) );
    camera->setTarget( irr::core::vector3df(0,0,0) );
//    camera->setNearValue( 1.f );
//    camera->setFarValue( 1000000.0f );

    // =========================================================================

    PrimitiveRenderer2D* painter = new PrimitiveRenderer2D( smgr, smgr->getRootSceneNode() );
    PrimitiveRenderer2D & dc = *painter;

    test_AllPolyLine( dc );

    // =========================================================================

    irr::ITimer* pTimer = device->getTimer();
    uint32_t t0 = pTimer->getRealTime();

    irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

    while (device->run())
    {
        if (device->isWindowActive())
        {
            driver->beginScene( true, true, clearColor );
            smgr->drawAll();
            if (guienv) guienv->drawAll();
            driver->endScene();

            if ( pTimer->getRealTime() - t0 > 250 )
            {
               irr::scene::ICameraSceneNode* activeCamera = smgr->getActiveCamera();

               std::wstringstream s;
               s << "FPS(" << driver->getFPS() << "), ";
                if (activeCamera)
                {
                  irr::core::vector3df camPos = activeCamera->getAbsolutePosition();
                  irr::core::vector3df camRot = activeCamera->getAbsoluteTransformation().getRotationDegrees();
                  s << "CamPos(" << int32_t(camPos.X)  << "," << int32_t(camPos.Y)  << "," << int32_t(camPos.Z) << "), "
                    << "CamRot(" << int32_t(camRot.X)  << "," << int32_t(camRot.Y)  << "," << int32_t(camRot.Z) << ")";
                }

                //s << painter->toString().c_str();
               device->setWindowCaption( s.str().c_str() );
               t0 = pTimer->getRealTime();
            }
        }
        else
        {
            device->yield();
        }
    }
    device->closeDevice();
    device->drop();
    return 0;
}
