/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_UI_HPP
#define PIONEERS3D_TYPES_UI_HPP

#include <pioneers3d/types/Common.hpp>
#include <pioneers3d/types/Font.hpp>

namespace pioneers3d {

class Game_t;

// ---------------------------------------------------------------------------------------
void UI_create( Game_t * game );
void UI_update( Game_t * game );
bool UI_onEvent( Game_t * game, irr::SEvent const & event );
void UI_createActionWindow( Game_t * game );
void UI_updateActionWindow( Game_t * game );
void UI_createPlayerWindow( Game_t * game );
void UI_updatePlayerWindow( Game_t * game );
void UI_createDiceWindow( Game_t * game );
void UI_createChatWindow( Game_t * game );
void UI_createBankWindow( Game_t * game );
void UI_createStartWindow( Game_t * game );
void UI_createHelpWindow( Game_t * game );

void UI_createCameraWindow( Game_t * game );
bool UI_handleCameraWindow( Game_t * game, irr::SEvent const & event );

BaseWindow*
UI_addWindow( Game_t * game, std::string const & title, irr::core::recti const & pos, irr::gui::IGUIElement* parent, int id = -1 );

class eWindow
{
public:
    enum Enum : uint32_t
    {
        UNKNOWN = 0,
        MAINMENU = 1,
        DICE = 1<<1,
        BANK = 1<<2,
        ACTION = 1<<3,
        PLAYER = 1<<4,
        CHAT = 1<<5,
        INIT_DICE = DICE | PLAYER | ACTION,
        INIT_PLACE = DICE | PLAYER | ACTION,
        ALL = MAINMENU | BANK | DICE | ACTION | PLAYER
    };

    eWindow( uint32_t flags ) : Flags( flags ) {}

    // eWindow( Enum flags ) : Flags( flags ) {}

    operator uint32_t() const { return Flags; }

    uint32_t Flags;
};

void
UI_setWindowVisible( Game_t * game, eWindow window, bool visible );

irr::gui::IGUIButton*
UI_createImageButton(
        irr::gui::IGUIEnvironment* env,
        irr::gui::IGUIElement* parent,
        irr::core::recti const & pos,
        std::string name,
        irr::video::ITexture* tex );

struct GUI_Menu_t
{
    BaseWindow* Window = nullptr;
    irr::gui::IGUIButton* Start = nullptr;
    irr::gui::IGUIButton* Options = nullptr;
    irr::gui::IGUIButton* Exit = nullptr;
};

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

struct GUI_Bank_t
{
    BaseWindow * Window = nullptr;
    GUI_Card_t * Holz = nullptr;
    GUI_Card_t * Lehm = nullptr;
    GUI_Card_t * Weizen = nullptr;
    GUI_Card_t * Wolle = nullptr;
    GUI_Card_t * Erz = nullptr;
};

struct GUI_Action_t
{
    BaseWindow* Window = nullptr;
    irr::gui::IGUIButton* Dice = nullptr;
    irr::gui::IGUIButton* Bank = nullptr;
    irr::gui::IGUIButton* Trade = nullptr;
    irr::gui::IGUIButton* PlayCard = nullptr;
    irr::gui::IGUIButton* BuyCard = nullptr;
    irr::gui::IGUIButton* BuyRoad = nullptr;
    irr::gui::IGUIButton* BuySett = nullptr;
    irr::gui::IGUIButton* BuyCity = nullptr;
    irr::gui::IGUIButton* PlaceRobber = nullptr;
    irr::gui::IGUIButton* PlaceRoad = nullptr;
    irr::gui::IGUIButton* PlaceSett = nullptr;
    irr::gui::IGUIButton* PlaceCity = nullptr;
    irr::gui::IGUIButton* EndRound = nullptr;
};

struct GUI_Player_t
{
    BaseWindow* Window = nullptr;
    irr::gui::IGUIStaticText* Name = nullptr;
    irr::gui::IGUIImage* Avatar = nullptr;
    GUI_Card_t * Roads = nullptr;
    GUI_Card_t * Settlements = nullptr;
    GUI_Card_t * Cities = nullptr;
    // Bank for ressource cards
    GUI_Card_t * Holz = nullptr;
    GUI_Card_t * Lehm = nullptr;
    GUI_Card_t * Weizen = nullptr;
    GUI_Card_t * Wolle = nullptr;
    GUI_Card_t * Erz = nullptr;
    // Bonus and Addons
    GUI_Card_t * EventCards = nullptr;
    GUI_Card_t * KnightCards = nullptr;
    GUI_Card_t * PointCards = nullptr;
    GUI_Card_t * BonusRoad = nullptr;
    GUI_Card_t * BonusArmy = nullptr;
    // A lotbox
    irr::gui::IGUIStaticText* LogBox = nullptr;
};
struct GUI_ChatBox_t
{
    BaseWindow* Window = nullptr;
    irr::gui::IGUIEditBox* Input = nullptr;
    irr::gui::IGUIButton* Send = nullptr;
    irr::gui::IGUIListBox* LogBox = nullptr;
};
struct GUI_Dice_t
{
    BaseWindow* Window = nullptr;
    irr::gui::IGUIButton* A = nullptr;
    irr::gui::IGUIButton* B = nullptr;
};

struct GUI_Camera_t
{
    BaseWindow* Window = nullptr;
    irr::gui::IGUIButton* Default = nullptr;
    irr::gui::IGUIButton* Left = nullptr;
    irr::gui::IGUIButton* Right = nullptr;
    irr::gui::IGUIButton* Front = nullptr;
    irr::gui::IGUIButton* Back = nullptr;
    irr::gui::IGUIButton* Top = nullptr;
    irr::gui::IGUIButton* Bottom = nullptr;

    irr::gui::IGUIButton* MoveLeft = nullptr;
    irr::gui::IGUIButton* MoveRight = nullptr;
    irr::gui::IGUIButton* MoveUp = nullptr;
    irr::gui::IGUIButton* MoveDown = nullptr;
    irr::gui::IGUIButton* MoveForward = nullptr;
    irr::gui::IGUIButton* MoveBack = nullptr;
};

struct GameUI_t
{
    GUI_Menu_t          Menu;
    GUI_Bank_t          Bank;
    GUI_Action_t        Action;
    GUI_Player_t        Player;
    GUI_ChatBox_t       Chat;
    GUI_Dice_t          Dice;
    GUI_Camera_t        Camera;
    std::vector< irr::gui::IGUIWindow* > Windows;
};


} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_UI_HPP
