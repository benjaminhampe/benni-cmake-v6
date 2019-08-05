#ifndef IRREXT_ALL_HPP
#define IRREXT_ALL_HPP

#include <irrExt/Types.hpp>
#include <irrExt/AutoMeshBuffer.hpp>
#include <irrExt/AutoMeshBufferUtils.hpp>
#include <irrExt/AutoSceneNode.hpp>
#include <irrExt/BaseEventReceiver.hpp>
#include <irrExt/BaseWindow.hpp>
#include <irrExt/Camera.hpp>
#include <irrExt/CylinderCamera.hpp>
#include <irrExt/CGUITTFont.hpp>
#include <irrExt/CGUIWindow.hpp>
#include <irrExt/NullDevice.hpp>
#include <irrExt/OpenGLDevice.hpp>
#include <irrExt/createCircle.hpp>
#include <irrExt/createRoundRect.hpp>
#include <irrExt/CXMLWriterUTF8.hpp>
#include <irrExt/FileSystem.hpp>
#include <irrExt/FontUtils.hpp>
#include <irrExt/GlmUtils.hpp>
#include <irrExt/ImageUtils.hpp>
#include <irrExt/irrExt.hpp>
#include <irrExt/libFreeType.hpp>
#include <irrExt/sinCosTable.hpp>
#include <irrExt/StringUtils.hpp>
#include <irrExt/ustring.hpp>
#include <irrExt/MeshLoader.hpp>
#include <irrExt/PerfTimer.hpp>

// =============================================================================================

class CGUIEditorSceneNode : public irr::gui::IGUIElement
{
public:
   CGUIEditorSceneNode(
      irr::gui::IGUIEnvironment* env,
      irr::gui::IGUIElement* parent,
      int id,
      irr::core::recti const & pos );

   ~CGUIEditorSceneNode() override;

   void draw() override;

   //! Called if an event happened.
   bool OnEvent( irr::SEvent const & event ) override;

public:
};

/*
 *
void
UI_setWindowVisible( Game_t * game, eWindow window, bool visible );

irr::gui::IGUIButton*
UI_createImageButton(
        irr::gui::IGUIEnvironment* env,
        irr::gui::IGUIElement* parent,
        irr::core::recti const & pos,
        std::string name,
        irr::video::ITexture* tex );

irr::gui::IGUIButton*
UI_createImageButton(  irr::gui::IGUIEnvironment* env,
               irr::gui::IGUIElement* parent,
               irr::core::recti const & pos,
               std::string name,
               irr::video::ITexture* tex  )
{
   assert( env );
   std::cout << __FUNCTION__ << "(" << pos << ")\n";
   if ( !parent ) parent = env->getRootGUIElement();

   irr::gui::IGUIButton* button = env->addButton( pos, parent, -1, L"", irr::core::stringw( name.c_str() ).c_str() );
   button->setImage( tex );
   //button->setScaleImage( true );
   return button;
}


void UI_createActionWindow( Game_t * game )
{
   irr::gui::IGUIEnvironment* env = Game_getGUIEnvironment( game );

   glm::ivec2 const screen = Game_getScreenSize( game );
   BaseWindow* win = UI_addWindow( game, "Chat & Logs:", mkRect( screen.x/2+100, screen.y/4, screen.x/2 - 150, screen.y/2 ), env->getRootGUIElement() );
   irr::core::recti r_client = win->getClientRect();
   int x = r_client.UpperLeftCorner.X;
   int y = r_client.UpperLeftCorner.Y;
   int w = r_client.getWidth();
   int h = r_client.getHeight();
   int b = 5;

   irr::gui::IGUIEnvironment* env = Game_getGUIEnvironment( game );

   BaseWindow* win = UI_addWindow( game, "Action Menu", mkRect( 100, 10, 900, 150 ), env->getRootGUIElement() );
   irr::core::recti rc = win->getClientRect();
   int x = rc.UpperLeftCorner.X;
   int y = rc.UpperLeftCorner.Y;
   game->UI.Action.Window = win;

   auto addButton = [game,env,&x,y,win] ( irr::gui::IGUIButton* & button, std::string name, eTexture tex ) -> void
   {
      button = env->addButton( mkRect( x+2, y+2, 80, 80 ), win, -1, L"", irr::core::stringw( name.c_str() ).c_str() );
      button->setImage( Texture_get( game, tex ) );
      x += 85;
   };

   addButton( game->UI.Action.EndRound, "End Round", eTexture::ACTION_ENDTURN );
   addButton( game->UI.Action.Dice, "Dice", eTexture::ACTION_DICE );
   addButton( game->UI.Action.Bank, "Bank", eTexture::ACTION_BANK );
   addButton( game->UI.Action.Trade, "Trade", eTexture::ACTION_TRADE );
   addButton( game->UI.Action.PlayCard, "Play City", eTexture::CARD_EVENT );
   addButton( game->UI.Action.BuyCard, "Buy Event Card", eTexture::ACTION_BUY_EVENT_CARD );
   addButton( game->UI.Action.BuyRoad, "Buy Road", eTexture::ACTION_BUY_ROAD );
   addButton( game->UI.Action.BuySett, "Buy Settlement", eTexture::ACTION_BUY_SETTLEMENT );
   addButton( game->UI.Action.BuyCity, "Buy City", eTexture::ACTION_BUY_CITY );
   addButton( game->UI.Action.PlaceRobber, "Place Robber", eTexture::ACTION_PLACE_ROBBER );
   addButton( game->UI.Action.PlaceRoad, "Place Road", eTexture::ACTION_BUY_ROAD );
   addButton( game->UI.Action.PlaceSett, "Place Settlement", eTexture::ACTION_BUY_SETTLEMENT );
   addButton( game->UI.Action.PlaceCity, "Place City", eTexture::ACTION_BUY_CITY );
}


bool UI_handleCameraWindow( Game_t * game, irr::SEvent const & event )
{
   assert( game );
   if ( event.EventType != irr::EET_GUI_EVENT ) { return false; }
   irr::SEvent::SGUIEvent const & e = event.GUIEvent;
   irr::gui::IGUIElement* caller = e.Caller;
   if ( !caller ) { return false; }
   if ( e.EventType == irr::gui::EGET_BUTTON_CLICKED )
   {
      if ( caller == game->UI.Camera.Default ) { Camera_moveToDefault( game ); return true; }
      if ( caller == game->UI.Camera.Top ) { Camera_moveToTop( game ); return true; }
      if ( caller == game->UI.Camera.Bottom ) { Camera_moveToBottom( game ); return true; }
      if ( caller == game->UI.Camera.Left ) { Camera_moveToLeft( game ); return true; }
      if ( caller == game->UI.Camera.Right ) { Camera_moveToRight( game ); return true; }
      if ( caller == game->UI.Camera.Front ) { Camera_moveToFront( game ); return true; }
      if ( caller == game->UI.Camera.Back ) { Camera_moveToBack( game ); return true; }
      if ( caller == game->UI.Camera.MoveLeft ) { return true; }
   }
   return false;
}

bool UI_onEvent( Game_t * game, irr::SEvent const & event )
{
    if ( !game )
    {
        return false;
    }

    if ( event.EventType != irr::EET_GUI_EVENT )
    {
        return false;
    }

   if ( UI_handleCameraWindow( game, event ) )
   {
      return true;
   }

    irr::SEvent::SGUIEvent const & e = event.GUIEvent;
    irr::gui::IGUIElement* caller = e.Caller;

    if ( !caller )
    {
        return false;
    }

    if ( e.EventType == irr::gui::EGET_BUTTON_CLICKED )
    {
//        if ( caller == game->UI.Menu.Exit )
//        {
//            irr::IrrlichtDevice* device = game->Device;
//            if ( device )
//            {
//                irr::scene::ICameraSceneNode* camera = device->getSceneManager()->getActiveCamera();
//                if ( camera )
//                {
//                    camera->setInputReceiverEnabled( false );
//                }

//                device->closeDevice();
//                device->drop();
//                device = nullptr;

//                Game_destroy( game );
//            }
//            return true;
//        }
//        if ( caller == game->UI.Menu.Start )
//        {
//            Game_start( game );
//            return true;
//        }

        /// -------------------------------------------------
        ///
        /// ACTIONS:
        ///
        ///

        if ( caller == game->UI.Action.Dice )
        {
            Action_Dice( game );
            return true;
        }
        if ( caller == game->UI.Action.EndRound )
        {
            Action_EndTurn( game );
            return true;
        }
        if ( caller == game->UI.Action.BuyCard )
        {
            Action_BuyEventCard( game );
            return true;
        }
        if ( caller == game->UI.Action.BuyRoad )
        {
            Action_BuyRoad( game );
            return true;
        }
        if ( caller == game->UI.Action.BuySett )
        {
            Action_BuySettlement( game );
            return true;
        }
        if ( caller == game->UI.Action.BuyCity )
        {
            Action_BuyCity( game );
            return true;
        }
        if ( caller == game->UI.Action.Trade )
        {
            Action_Trade( game );
            return true;
        }
        if ( caller == game->UI.Action.Bank )
        {
            Action_Bank( game );
            return true;
        }
    }

    return false;
}


// =============================================================================================

class GUI_Card_t : public irr::gui::IGUIElement
{
public:
    GUI_Card_t( irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent, int id, irr::core::recti const & pos );
    ~GUI_Card_t() override;
    void draw() override;
    void setTitle( Text_t title ) { m_Title = title; }
    void setValue( Text_t value ) { m_Value = value; }
    void setTexture( irr::video::ITexture* tex ) { m_Tex = tex; }
    Text_t const & getTitle() const { return m_Title; }
    Text_t const & getValue() const { return m_Value; }
    Text_t & getTitle() { return m_Title; }
    Text_t & getValue() { return m_Value; }

public:
    Text_t m_Title;
    Text_t m_Value;
    irr::video::ITexture* m_Tex;
};

GUI_Card_t*
createGUICard( Game_t * game, irr::gui::IGUIElement* parent, eFontType fontType, irr::core::recti const & rect, std::string name, std::string value, TileType tt )
{
   assert( game );
   assert( game->Device );
   assert( game->Device->getGUIEnvironment() );
   Font_t font = Game_getFont( game, fontType );
   GUI_Card_t* card = new GUI_Card_t( game->Device->getGUIEnvironment(), parent, -1, rect );
   card->setTexture( Game_getCardTexture( game, tt ) );
   Text_t t_title{ font, name, glm::ivec2(0,0), tt.getRessourceColor() };
   Text_t t_value{ font, value, glm::ivec2(0,0), 0xFFFFFFFF };
   card->setTitle( std::move( t_title ) );
   card->setValue( std::move( t_value ) );
   //x += dx + b;
   return card;
}

GUI_Card_t::GUI_Card_t(
        irr::gui::IGUIEnvironment* env,
        irr::gui::IGUIElement* parent,
        int id,
        irr::core::recti const & pos )
    : irr::gui::IGUIElement( irr::gui::EGUIET_ELEMENT, env, parent, id, pos )
    , m_Tex( nullptr )
{}

GUI_Card_t::~GUI_Card_t()
{}

void
GUI_Card_t::draw()
{
    // std::cout << "GUI_Card_t." << __FUNCTION__ << "(" << m_Title << "," << m_Value << "," << getTexNameA( m_Tex ) << "," << toString( AbsoluteRect ) << ")\n";

    if ( Environment && isVisible() )
    {
        irr::video::IVideoDriver* driver = Environment->getVideoDriver();
        if ( driver )
        {
            int b = 5;
            int x = AbsoluteRect.UpperLeftCorner.X;
            int y = AbsoluteRect.UpperLeftCorner.Y;
            int w = AbsoluteRect.getWidth();
            int h = AbsoluteRect.getHeight();
            int lineHeight = 15;
            irr::core::recti r_name  = mkRect( x+b, y+b, w-2*b, lineHeight);
            irr::core::recti r_image = mkRect( x+b, y+b+lineHeight, w-2*b, h - 3*b - 2*lineHeight );
            irr::core::recti r_value = mkRect( x+b, y+h-b-lineHeight, w-2*b, lineHeight );

            Font_draw( m_Title.Font, m_Title.Text, r_name, m_Title.Color );
            if ( m_Tex )
            {
                driver->draw2DImage( m_Tex, r_image, mkRect(0,0,m_Tex->getOriginalSize()), nullptr, nullptr, true );
            }
            Font_draw( m_Value.Font, m_Value.Text, r_name, m_Value.Color );

        }

    }

    irr::gui::IGUIElement::draw();
}
*/

#endif // IRREXT_ALL_HPP
