#ifndef EVENTRECEIVER_HPP
#define EVENTRECEIVER_HPP

#include <irrlicht.h>

// ============================================================================
class EventReceiver : public irr::IEventReceiver
// ============================================================================
{
    irr::IrrlichtDevice* m_Device = nullptr;
public:
    EventReceiver() : m_Device( nullptr ) {}

    virtual ~EventReceiver() {}

    void setDevice( irr::IrrlichtDevice* p ) { m_Device = p; }

    virtual bool OnEvent( const irr::SEvent& event )
    {
        if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
        {
            irr::SEvent::SKeyInput const & key = event.KeyInput;
            if (key.Key == irr::KEY_ESCAPE)
            {
                //std::cout << "KEY_ESCAPE\n";
                if (m_Device)
                {
                    m_Device->closeDevice();
                    m_Device = nullptr;
                }
                return true;
            }
            if (key.Key == irr::KEY_SPACE && key.PressedDown == false)
            {
                if (m_Device)
                {
                     irr::scene::ICameraSceneNode* camera = m_Device->getSceneManager()->getActiveCamera();
                     if (camera)
                     {
                        camera->setInputReceiverEnabled( !camera->isInputReceiverEnabled() );
                     }
                }
                return true;
            }
        }
//       else if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
//        {
//            irr::SEvent::SMouseInput const & m = event.MouseInput;
//            if (m.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
//            {
//                return true;
//            }
//        }
        return false;
    }
};


#endif // EVENTRECEIVER_HPP
