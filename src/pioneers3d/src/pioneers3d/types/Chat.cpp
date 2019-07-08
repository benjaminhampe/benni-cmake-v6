#include <pioneers3d/types/Chat.hpp>
#include <pioneers3d/types/Game.hpp>
#include <pioneers3d/types/Font.hpp>

//#define PRINT_FUNCTION_NAME { std::cout << __FUNCTION__ << ":" << __LINE__ << "()" << std::endl; }
//#define PRINT_FUNCTION { Chat_log( game, LogLevel::Debug, __FUNCTION__ ); }

namespace pioneers3d {

struct ChatItem_t
{
    uint32_t Color;
    std::string Text;
    ChatItem_t() : Color(0xFFFFFFFF), Text("Hello World!") {}
    ChatItem_t( std::string txt, uint32_t color ) : Color(color), Text(txt) {}
};

struct Chat_t
{
    std::vector< ChatItem_t > Items;
};

void Chat_create( Game_t * game )
{
    if (!game) return;
    game->Chat = new Chat_t;
}

void Chat_destroy( Game_t * game )
{
    if (!game) return;
    Chat_t* chat = (Chat_t*)game->Chat;
    if (chat)
    {
        delete chat;
        game->Chat = nullptr;
    }
}

void Chat_clear( Game_t * game )
{
    if (!game) return;
    Chat_t* chat = (Chat_t*)game->Chat;
    if (!chat) return;
    chat->Items.clear();
}

void Chat_print( Game_t * game, std::string txt, uint32_t color )
{
    if (!game) return;
    Chat_t* chat = (Chat_t*)game->Chat;
    if (!chat) return;
    chat->Items.emplace_back( ChatItem_t( txt, color ) );
}

void Chat_log( Game_t * game, de::hampe::logging::LogLevel const & level, std::string const & txt )
{
    //std::stringstream s;
    //s << level.toString() << " :: " << txt;
    Chat_print( game, txt, level.getColor() );
}

void Chat_printPlayerMessage( Game_t * game, int32_t player, std::string txt )
{
    if (!game) return;
    Chat_t* chat = (Chat_t*)game->Chat;
    if (!chat) return;
    uint32_t color = Player_getColor( game, player );
    std::string name = Player_getName( game, player );
    chat->Items.emplace_back( ChatItem_t( de::hampe::sprintf("(", player + 1, ") ", name.c_str(), " ", txt.c_str()), color ) );
}

std::string Chat_toString( Game_t * game )
{
    std::stringstream s; s << "Chat: ";
    if (!game) { s << "Error"; return s.str(); }
    Chat_t* chat = (Chat_t*)game->Chat;
    if (!game) { s << "Error"; return s.str(); }
    s << chat->Items.size() << "\n";
    for ( size_t i = 0; i < chat->Items.size(); ++i )
    {
        ChatItem_t const & item = chat->Items[ i ];
        s << item.Text << "\n";
    }

    return s.str();
}

void Chat_draw( Game_t * game, int32_t x, int32_t y )
{
    if (!game) return;
    Chat_t* chat = (Chat_t*)game->Chat;
    if (!chat) return;
    Font_t font = Game_getFont( game, eFontType::CHAT );
    if (!font.Font) return;
    int32_t tx = x;
    int32_t ty = y;
    size_t maxItems = chat->Items.size();
    size_t drawnItems = 15;
    size_t iStart = 0;
    if ( maxItems >= drawnItems )
    {
        iStart = maxItems - drawnItems;
    }

    irr::video::IVideoDriver* driver = game->Device->getVideoDriver();
    irr::core::dimension2du screen = driver->getScreenSize();
    //irr::core::recti viewport = driver->getViewPort();

    game->Device->getVideoDriver()->draw2DRectangle( 0x60FFFFFF, mkRect( tx-10, ty-10, screen.Width, screen.Height ) );
    for ( size_t i = iStart; i < maxItems; ++i )
    {
        ChatItem_t const & item = chat->Items[ i ];
        irr::core::dimension2du textSize = Font_getTextDimension( font, item.Text );

        uint32_t bgColor = 0xFF202020;
        if ( irr::video::SColor( item.Color ).getAverage() < 90 )
        {
            bgColor = 0xFFFFFFFF;
        }

        Font_draw( font, item.Text, glm::ivec2( tx-1, ty ), bgColor );
        Font_draw( font, item.Text, glm::ivec2( tx-1, ty+1 ), bgColor );
        Font_draw( font, item.Text, glm::ivec2( tx+2, ty ), bgColor );
        Font_draw( font, item.Text, glm::ivec2( tx+2, ty+1 ), bgColor );

        Font_draw( font, item.Text, glm::ivec2( tx, ty-1 ), bgColor );
        Font_draw( font, item.Text, glm::ivec2( tx+1, ty-1 ), bgColor );
        Font_draw( font, item.Text, glm::ivec2( tx, ty+2 ), bgColor );
        Font_draw( font, item.Text, glm::ivec2( tx+1, ty+2 ), bgColor );

        Font_draw( font, item.Text, glm::ivec2( tx,   ty ), item.Color );
        Font_draw( font, item.Text, glm::ivec2( tx+1, ty+1 ), item.Color );
        Font_draw( font, item.Text, glm::ivec2( tx+1, ty ), item.Color );
        Font_draw( font, item.Text, glm::ivec2( tx,   ty+1 ), item.Color );

        ty += textSize.Height;
    }
}

} // end namespace pioneers3d

/*
void GameUI_createChat( Game_t * game, irr::core::recti const & pos )
{
    irr::gui::IGUIEnvironment* env = game->Device->getGUIEnvironment();
    BaseWindow* win = addWindow( game, "Chat & Logs:", pos, env, env->getRootGUIElement() );
    irr::core::recti r_client = win->getClientRect();
    int x = r_client.UpperLeftCorner.X;
    int y = r_client.UpperLeftCorner.Y;
    int w = r_client.getWidth();
    int h = r_client.getHeight();
    int b = 5;
    game->UI.Chat.Window = win;
    game->UI.Chat.Input = env->addEditBox( L"Chat box", mkRect( x+b, y+b, w - 64 - 3*b, 32 ), true, win, -1 );
    game->UI.Chat.Send = env->addButton( mkRect( x+ w - 64 - 2*b, y+b, 64, 32 ), win, -1, L"Bank", L"Bank" );
    game->UI.Chat.LogBox = env->addListBox( mkRect( x+b, y+32+2*b, w-2*b, h-32-3*b ), win, -1, true );
}
*/
