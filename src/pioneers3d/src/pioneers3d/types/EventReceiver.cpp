#include <pioneers3d/types/EventReceiver.hpp>
#include <pioneers3d/types/Game.hpp>

namespace pioneers3d {

GameEventReceiver::GameEventReceiver( Game_t* game )
    : irrExt::BaseEventReceiver( game->Device )
    , m_Game( game )
{
    std::cout << __FUNCTION__ << "()\n";
}

GameEventReceiver::~GameEventReceiver()
{
    std::cout << __FUNCTION__ << "()\n";
}

bool
GameEventReceiver::OnEvent( const irr::SEvent& event )
{
    if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
    {
        irr::SEvent::SKeyInput const & keyEvent = event.KeyInput;
#ifdef USE_IRRLICHT
        if ( keyEvent.Key == irr::KEY_ESCAPE && !keyEvent.PressedDown )
#else
        if ( keyEvent.Key == irr::IRR_KEY_ESCAPE && !keyEvent.PressedDown )
#endif
        {
            if ( m_Game->State == GameState::NOT_RUNNING || m_Game->State == GameState::IDLE )
            {
                m_Game->UI.Menu.Window->setVisible( !m_Game->UI.Menu.Window->isVisible() );
                Game_setCameraInput( m_Game, !m_Game->UI.Menu.Window->isVisible() );
            }
            else
            {
                if ( m_Game->State == GameState::PLACE_OBJECT )
                {
                    Action_Abort( m_Game );
                }
            }
            return true;
        }
    }


    if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
    {
        irr::SEvent::SMouseInput const & mouseEvent = event.MouseInput;

        if ( mouseEvent.Event == irr::EMIE_MOUSE_MOVED )
        {
            if ( m_Game->State == GameState::PLACE_OBJECT &&
                 m_Game->PlaceObject &&
                 m_Game->PlaceObjectType == ePlaceObjectType::ROAD &&
                 m_Game->SelectedWaypointR )
            {
                m_Game->PlaceObject->setRotation( irr::core::vector3df( 0, m_Game->SelectedWaypointR->Angle, 0 ) );
            }
        }

        if ( mouseEvent.Event == irr::EMIE_LMOUSE_LEFT_UP )
        {
            if ( m_Game->State == GameState::PLACE_OBJECT )
            {
                if ( m_Game->PlaceObjectType == ePlaceObjectType::ROBBER )
                {
//                    Waypoint_t * waypoint = m_Game->SelectedWaypointRoad;
//                    if ( waypoint )
//                    {
//                        m_Game->PlaceSceneNode->setPosition( toVec3df( waypoint->Pos ) );
//                        waypoint->Owner = getPlayer( m_Game, m_Game->Player );

//                        m_Game->State = GameState::IDLE;
//                    }
                }

                if ( m_Game->PlaceObjectType == ePlaceObjectType::ROAD )
                {
                    Waypoint_t * w = m_Game->SelectedWaypointR;
                    if ( w )
                    {
                        Player_addRoad( m_Game, w );
                        m_Game->State = GameState::IDLE;
                    }

                }

                if ( m_Game->PlaceObjectType == ePlaceObjectType::SETTLEMENT )
                {
                    Waypoint_t * w = m_Game->SelectedWaypointS;
                    if ( w )
                    {
                        Player_addSettlement( m_Game, w );
                        m_Game->State = GameState::IDLE;
                    }
                }
            }
        }
    }

    if ( m_Game->Camera &&
         m_Game->Camera->OnEvent( event ) )
    {
        return true;
    }

    if ( event.EventType == irr::EET_GUI_EVENT )
    {
        irr::SEvent::SGUIEvent const & guiEvent = event.GUIEvent;
        irr::gui::IGUIElement* caller = guiEvent.Caller;
        //irr::gui::IGUIEnvironment* env = m_Device->getGUIEnvironment();

        if ( guiEvent.EventType == irr::gui::EGET_BUTTON_CLICKED )
        {
            if ( caller && m_Game )
            {
                if ( caller == m_Game->UI.Menu.Exit )
                {
                    if ( m_Game->Device )
                    {
                        irr::scene::ICameraSceneNode* camera = m_Game->Device->getSceneManager()->getActiveCamera();
                        if ( camera )
                        {
                            camera->setInputReceiverEnabled( false );
                        }

                        m_Device->closeDevice();
                        //m_Device->drop();
                        m_Device = nullptr;

                        Game_destroy( m_Game );
                    }
                    return true;
                }

                if ( caller == m_Game->UI.Menu.Start )
                {
                    Game_start( m_Game );
                    return true;
                }
            }
        }
    }

    if ( UI_onEvent( m_Game, event ) )
    {
        return true;
    }

    return irrExt::BaseEventReceiver::OnEvent( event );
}

} // end namespace pioneers3d

