#include <pioneers3d/types/UI.hpp>
#include <pioneers3d/types/Game.hpp>

namespace pioneers3d {

BaseWindow*
UI_addWindow( Game_t * game, std::string const & title, irr::core::recti const & pos, irr::gui::IGUIElement* parent, int id )
{
   if ( !game ) return nullptr;
   irr::gui::IGUIEnvironment* env = Game_getGUIEnvironment( game );
   if ( !env ) return nullptr;
   BaseWindow* win = createBaseWindow( title, pos, env, parent, id );
   game->UI.Windows.emplace_back( win );
   return win;
}

void
UI_setWindowVisible( Game_t * game, eWindow window, bool visible )
{
   assert( game );
   BaseWindow * p = nullptr;

   if ( window & eWindow::MAINMENU ) { p = game->UI.Menu.Window; }
   if ( window & eWindow::ACTION ) { p = game->UI.Action.Window; }
   if ( window & eWindow::BANK ) { p = game->UI.Bank.Window; }
   if ( window & eWindow::DICE ) { p = game->UI.Dice.Window; }
   if ( window & eWindow::PLAYER ) { p = game->UI.Player.Window; }
   if ( window & eWindow::CHAT ) { p = game->UI.Chat.Window; }

   if ( p )
   {
      p->setVisible( visible );
   }
}

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


void UI_create( Game_t * game )
{
    std::cout << __FUNCTION__ << "\n";
    assert( game );
    assert( game->Device );
    //irr::gui::IGUIEnvironment* env = game->Device->getGUIEnvironment();
    //assert( env );

    glm::ivec2 const screen = Game_getScreenSize( game );
    UI_createStartWindow( game );
    UI_createChatWindow( game );
    UI_createActionWindow( game );
    UI_createPlayerWindow( game );
    UI_createDiceWindow( game );
    UI_createBankWindow( game );
    UI_createHelpWindow( game );
    UI_createCameraWindow( game );

    //UI_setWindowVisible( game, eWindow::ALL, false );
    //UI_setWindowVisible( game, eWindow::ACTION, true );
    //UI_setWindowVisible( game, eWindow::PLAYER, true );
}

void UI_update( Game_t * game )
{
    UI_updateActionWindow( game );
    UI_updatePlayerWindow( game );
}

void UI_createStartWindow( Game_t * game )
{
    irr::gui::IGUIEnvironment* env = Game_getGUIEnvironment( game );

    glm::ivec2 const screen = Game_getScreenSize( game );
    BaseWindow* win = UI_addWindow( game, "GAME MENU", mkRect( screen.x/2 - 200, 50, 400, 450 ), env->getRootGUIElement() );

    irr::core::recti r_client = win->getClientRect();
    int bW = 200;
    int bH = 100;
    int x1 = (r_client.getWidth() - bW) / 2;
    int x2 = (r_client.getWidth() + bW) / 2;
    int y = 50;
    game->UI.Menu.Window = win;
    game->UI.Menu.Start = env->addButton( irr::core::recti( x1, y, x2, y+bH ), win, -1, L"Start Game", L"Starts a new game..." ); y += bH + 25;
    game->UI.Menu.Options = env->addButton( irr::core::recti( x1, y, x2, y+bH ), win, -1, L"Options", L"Enter game settings..." ); y += bH + 25;
    game->UI.Menu.Exit = env->addButton( irr::core::recti( x1, y, x2, y+bH ), win, -1, L"Exit Game", L"Exit game and program..." ); y += bH + 25;

}

void UI_createDiceWindow( Game_t * game )
{
    std::cout << __FUNCTION__ << "\n";
    irr::gui::IGUIEnvironment* env = Game_getGUIEnvironment( game );

    glm::ivec2 const screen = Game_getScreenSize( game );
    BaseWindow* win = UI_addWindow( game, "D I C E", mkRect( screen.x - 300, 10, 250, 200 ), env->getRootGUIElement() );
    irr::core::recti r_client = win->getClientRect();
    int w = r_client.getWidth();
    int h = r_client.getHeight();
    int x = r_client.UpperLeftCorner.X;
    int y = r_client.UpperLeftCorner.Y;
    int b = 10;
    int dx = (w-3*b)/2;
    int dy = h-2*b;
    game->UI.Dice.Window = win;
    game->UI.Dice.A = UI_createImageButton( env, win, mkRect(x+b,y+b,dx,dy), "Dice 1", Game_getDiceTexture( game, 1+(rand()%6) ) );
    game->UI.Dice.B = UI_createImageButton( env, win, mkRect(x+w-1-b-dx,y+b,dx,dy), "Dice 2", Game_getDiceTexture( game, 1+(rand()%6) ) );
}

void UI_createActionWindow( Game_t * game )
{
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

void UI_updateActionWindow( Game_t * game )
{
    auto setButton = [] ( irr::gui::IGUIButton* & button, bool visibleAndEnabled ) -> void
    {
        button->setVisible( visibleAndEnabled );
        button->setEnabled( visibleAndEnabled );
    };

    int32_t playerIndex = game->Player;
    Player_t* player = Player_getByIndex( game, playerIndex );

    setButton( game->UI.Action.EndRound, player->Action.isEnabled( eAction::ENDTURN ) );
    setButton( game->UI.Action.Dice, player->Action.isEnabled( eAction::DICE ) );
    setButton( game->UI.Action.Bank, player->Action.isEnabled( eAction::BANK ) );
    setButton( game->UI.Action.Trade, player->Action.isEnabled( eAction::TRADE ) );
    setButton( game->UI.Action.PlayCard, player->Action.isEnabled( eAction::PLAY_EVENT_CARD ) );
    setButton( game->UI.Action.BuyCard, player->Action.isEnabled( eAction::BUY_EVENT_CARD ) );
    setButton( game->UI.Action.BuyRoad, player->Action.isEnabled( eAction::BUY_ROAD ) );
    setButton( game->UI.Action.BuySett, player->Action.isEnabled( eAction::BUY_SETTLEMENT ) );
    setButton( game->UI.Action.BuyCity, player->Action.isEnabled( eAction::BUY_CITY ) );
    setButton( game->UI.Action.PlaceRobber, player->Action.isEnabled( eAction::PLACE_ROBBER ) );
    setButton( game->UI.Action.PlaceRoad, player->Action.isEnabled( eAction::PLACE_ROAD ) );
    setButton( game->UI.Action.PlaceSett, player->Action.isEnabled( eAction::PLACE_SETTLEMENT ) );
    setButton( game->UI.Action.PlaceCity, player->Action.isEnabled( eAction::PLACE_CITY ) );
}

void UI_createPlayerWindow( Game_t * game )
{
   std::cout << __FUNCTION__ << "\n";
   irr::gui::IGUIEnvironment* env = Game_getGUIEnvironment( game );

   glm::ivec2 const screen = Game_getScreenSize( game );

   BaseWindow* win = UI_addWindow( game, "Player", mkRect( 10, screen.y - 210, screen.x - 100, 200 ), env->getRootGUIElement() );
   irr::core::recti r_client = win->getClientRect();
   Font_t font = Game_getFont( game, eFontType::SMALL );
   int x = r_client.UpperLeftCorner.X;
   int y = r_client.UpperLeftCorner.Y;
   int dx = 64;
   int dy = 128;
   int b = 5;
   game->UI.Player.Window = win;
   createGUIImage( env, win, mkRect(x+b,   y+b,96,96), Game_getPlayerTexture( game, 1 ) );
   createGUIImage( env, win, mkRect(x+b,   y+b+96+b,32,32), Texture_get( game, eTexture::STAT_ROAD ) );
   createGUIImage( env, win, mkRect(x+b+32,y+b+96+b,32,32), Texture_get( game, eTexture::STAT_SETTLEMENT ) );
   createGUIImage( env, win, mkRect(x+b+64,y+b+96+b,32,32), Texture_get( game, eTexture::STAT_CITY ) );
   x += 96 + b;

   auto addCard = [game,env,win,font,&x,y,dx,dy,b] ( GUI_Card_t* & card, std::string name, std::string value, eTexture tex, uint32_t color )
   {
      card = new GUI_Card_t( env, win, -1, mkRect(x,y,dx,dy) );
      card->setTexture( Texture_get( game, tex ) );
      Text_t t_title{ font, name, glm::ivec2(0,0), color };
      Text_t t_value{ font, value, glm::ivec2(0,0), 0xFFFFFFFF };
      card->setTitle( std::move( t_title ) );
      card->setValue( std::move( t_value ) );
      x += dx + b;
   };

   addCard( game->UI.Player.Holz, "Holz", "0", eTexture::CARD_HOLZ,      TileType::holz().getRessourceColor() );
   addCard( game->UI.Player.Lehm, "Lehm", "0", eTexture::CARD_LEHM,      TileType::lehm().getRessourceColor() );
   addCard( game->UI.Player.Weizen, "Weizen", "0", eTexture::CARD_WEIZEN, TileType::weizen().getRessourceColor() );
   addCard( game->UI.Player.Wolle, "Wolle", "0", eTexture::CARD_WOLLE,   TileType::wolle().getRessourceColor() );
   addCard( game->UI.Player.Erz, "Erz", "0", eTexture::CARD_ERZ,         TileType::erz().getRessourceColor() );

   //    addCard( game->UI.Player.Roads, "Roads", "0", eTexture::STAT_ROAD,  0xFFCCCCCC );
   //    addCard( game->UI.Player.Settlements, "Settlements", "0", eTexture::STAT_SETTLEMENT, 0xFFDDDDDD );
   //    addCard( game->UI.Player.Cities, "Cities", "0", eTexture::STAT_CITY, 0xFFEEEEEE );
   addCard( game->UI.Player.EventCards, "Event Cards", "0", eTexture::CARD_EVENT, 0xFFFFFFFF );
   addCard( game->UI.Player.KnightCards, "Knight Cards", "0", eTexture::CARD_KNIGHT, 0xFFFFFFFF );
   addCard( game->UI.Player.PointCards, "Point Cards", "0", eTexture::CARD_POINT, 0xFFFFFFFF );
   addCard( game->UI.Player.BonusRoad, "Bonus", "Longest Road", eTexture::CARD_BONUS_ROAD, 0xFFFFFFFF );
   addCard( game->UI.Player.BonusArmy, "Bonus", "Biggest Army", eTexture::CARD_BONUS_ARMY, 0xFFFFFFFF );
}

void UI_updatePlayerWindow( Game_t * game )
{
   int32_t playerIndex = game->Player;

   Player_t* player = Player_getByIndex( game, playerIndex );

   auto setButton = [] ( irr::gui::IGUIButton* & button, bool visibleAndEnabled ) -> void
   {
      button->setVisible( visibleAndEnabled );
      button->setEnabled( visibleAndEnabled );
   };

   //game->UI.Player.Erz->getValue().setValue( Player_getNumErz( game ) );
   setButton( game->UI.Action.Dice, player->Action.isEnabled( eAction::DICE ) );
   setButton( game->UI.Action.Bank, player->Action.isEnabled( eAction::BANK ) );
   setButton( game->UI.Action.Trade, player->Action.isEnabled( eAction::TRADE ) );
   setButton( game->UI.Action.PlayCard, player->Action.isEnabled( eAction::PLAY_EVENT_CARD ) );
   setButton( game->UI.Action.BuyCard, player->Action.isEnabled( eAction::BUY_EVENT_CARD ) );
   setButton( game->UI.Action.BuyRoad, player->Action.isEnabled( eAction::BUY_ROAD ) );
   setButton( game->UI.Action.BuySett, player->Action.isEnabled( eAction::BUY_SETTLEMENT ) );
   setButton( game->UI.Action.BuyCity, player->Action.isEnabled( eAction::BUY_CITY ) );
   setButton( game->UI.Action.PlaceRobber, player->Action.isEnabled( eAction::PLACE_ROBBER ) );
   setButton( game->UI.Action.PlaceRoad, player->Action.isEnabled( eAction::PLACE_ROAD ) );
   setButton( game->UI.Action.PlaceSett, player->Action.isEnabled( eAction::PLACE_SETTLEMENT ) );
   setButton( game->UI.Action.PlaceCity, player->Action.isEnabled( eAction::PLACE_CITY ) );
}

void UI_createChatWindow( Game_t * game )
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
   game->UI.Chat.Window = win;
   game->UI.Chat.Input = nullptr;
   game->UI.Chat.Send = nullptr;
   game->UI.Chat.LogBox = nullptr;
   //    game->UI.Chat.Input = env->addEditBox( L"Chat box", mkRect( x+b, y+b, w - 64 - 3*b, 32 ), true, win, -1 );
   //    game->UI.Chat.Send = env->addButton( mkRect( x+ w - 64 - 2*b, y+b, 64, 32 ), win, -1, L"Bank", L"Bank" );
   //    game->UI.Chat.LogBox = env->addListBox( mkRect( x+b, y+32+2*b, w-2*b, h-32-3*b ), win, -1, true );
}

void UI_createBankWindow( Game_t * game )
{
   std::cout << __FUNCTION__ << "\n";
   irr::gui::IGUIEnvironment* env = Game_getGUIEnvironment( game );

   glm::ivec2 const screen = Game_getScreenSize( game );
   BaseWindow* win = UI_addWindow( game, "Bank", mkRect( 10, (screen.y - 200)/2, 400, 160 ), env->getRootGUIElement() );
   irr::core::recti r_client = win->getClientRect();
   game->UI.Bank.Window = win;
   int x = r_client.UpperLeftCorner.X;
   int y = r_client.UpperLeftCorner.Y;
   int dx = 64;
   int dy = 128;
   int b = 0;

   Font_t font = Game_getFont( game, eFontType::Default );

   auto addCard = [game,env,win,font,&x,y,dx,dy,b] ( GUI_Card_t* & card, std::string name, std::string value, TileType tt )
   {
      card = new GUI_Card_t( env, win, -1, mkRect(x,y,dx,dy) );
      card->setTexture( Game_getCardTexture( game, tt ) );
      Text_t t_title{ font, name, glm::ivec2(0,0), tt.getRessourceColor() };
      Text_t t_value{ font, value, glm::ivec2(0,0), 0xFFFFFFFF };
      card->setTitle( std::move( t_title ) );
      card->setValue( std::move( t_value ) );
      x += dx + b;
   };

   addCard( game->UI.Bank.Holz, "Holz", "0", TileType::HOLZ );
   addCard( game->UI.Bank.Lehm, "Lehm", "0", TileType::LEHM );
   addCard( game->UI.Bank.Weizen, "Weizen", "0", TileType::WEIZEN );
   addCard( game->UI.Bank.Wolle, "Wolle", "0", TileType::WOLLE );
   addCard( game->UI.Bank.Erz, "Erz", "0", TileType::ERZ );
}

void UI_createHelpWindow( Game_t * game )
{

}


void UI_createCameraWindow( Game_t * game )
{
   irr::gui::IGUIEnvironment* env = Game_getGUIEnvironment( game );

   glm::ivec2 const screen = Game_getScreenSize( game );
   BaseWindow* win = UI_addWindow( game, "Action Menu", mkRect( 10, 300, 400, 400 ), env->getRootGUIElement() );
   irr::core::recti rc = win->getClientRect();
   int x = rc.UpperLeftCorner.X;
   int y = rc.UpperLeftCorner.Y;
   int w = rc.getWidth();
   int h = rc.getHeight();
   int p = 10; // padding

   auto addButton = [&] ( irr::gui::IGUIButton* & button, std::string name, irr::core::recti const & pos ) -> void
   {
      irr::core::stringw buttonText( name.c_str() );
      button = env->addButton( pos, win, -1, buttonText.c_str(), buttonText.c_str() );
   };

   game->UI.Camera.Window = win;

   int dx = 64;
   int dy = 64;
   int bx = x + p;
   int by = y + p;

   addButton( game->UI.Camera.Default, "Default", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.Front, "Front", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.Back, "Back", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.Left, "Left", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.Right, "Right", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.Top, "Top", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.Bottom, "Bottom", mkRect( bx, by, dx, dy ) ); bx += dx + p;

   bx = x + p;
   by += dy + p;
   addButton( game->UI.Camera.MoveDown, "Move Down", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.MoveForward, "Move Forward", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.MoveUp, "Move Up", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.MoveLeft, "Move Left", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.MoveBack, "Move Back", mkRect( bx, by, dx, dy ) ); bx += dx + p;
   addButton( game->UI.Camera.MoveRight, "Move Right", mkRect( bx, by, dx, dy ) ); bx += dx + p;
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



} // end namespace pioneers3d
