/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_ACTIONTYPE_HPP
#define PIONEERS3D_TYPES_ACTIONTYPE_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

class Game_t; // never include header for this here

// ---------------------------------------------------------------------------------------
// GameLogic
// ---------------------------------------------------------------------------------------
void Action_Abort( Game_t* game );
void Action_Dice( Game_t* game );
void Action_Bank( Game_t* game );
void Action_Trade( Game_t* game );
void Action_BuyEventCard( Game_t* game );
void Action_BuyRoad( Game_t* game );
void Action_BuySettlement( Game_t* game );
void Action_BuyCity( Game_t* game );
void Action_EndTurn( Game_t* game );
void Action_PlayEventCard( Game_t* game );
void Action_PlaceRobber( Game_t* game );
void Action_PlaceRoad( Game_t* game );
void Action_PlaceSettlement( Game_t* game );
void Action_PlaceCity( Game_t* game );
void Action_Win( Game_t* game );

class eAction
{
    uint32_t m_Action;
public:
    enum eType : uint32_t
    {
        UNKNOWN         = 0,
        ENDTURN         = 1,
        DICE            = 1<<1,
        BANK            = 1<<2,
        TRADE           = 1<<3,
        BUY_EVENT_CARD  = 1<<4,
        BUY_ROAD        = 1<<5,
        BUY_SETTLEMENT  = 1<<6,
        BUY_CITY        = 1<<7,
        PLAY_EVENT_CARD = 1<<8,
        PLACE_ROAD      = 1<<9,
        PLACE_SETTLEMENT= 1<<10,
        PLACE_CITY      = 1<<11,
        PLACE_ROBBER    = 1<<12,
        LOOSE           = 1<<13,
        WIN             = 1<<14,
        COUNT           = 15,
        DEFAULT_MASK = ENDTURN | DICE | BANK | TRADE | WIN | LOOSE,
        INIT_PHASE_DICE_MASK = ENDTURN | DICE,
        INIT_PHASE_SET_MASK = ENDTURN | DICE | BUY_ROAD | BUY_SETTLEMENT,
        BUY_MASK = BUY_EVENT_CARD | BUY_ROAD | BUY_SETTLEMENT | BUY_CITY,
        PLAY_MASK = PLAY_EVENT_CARD,
        PLACE_MASK = PLACE_ROBBER | PLACE_ROAD | PLACE_SETTLEMENT | PLACE_CITY,
        ACTION_MASK = DEFAULT_MASK | BUY_MASK | PLAY_MASK | PLACE_MASK
    };

    eAction() : m_Action( UNKNOWN ) {}
    eAction( eType action ) : m_Action( static_cast< uint32_t >( action ) ) {}
    eAction( uint32_t action ) : m_Action( action ) {}
    operator uint32_t() const { return m_Action; }
    uint32_t getValue() const { return m_Action; }
    void setValue( uint32_t value ) { m_Action = value; }
    bool isEnabled( uint32_t action ) const { return ( m_Action & action ) == action; }
    void enable( uint32_t action ) { m_Action |= action; }
    void disable( uint32_t action ) { m_Action &= ~action; }
    inline bool contains( uint32_t flags ) const { return isEnabled( flags ); }

    void setEnabled( uint32_t action, bool enabled )
    {
        if ( enabled )
        {
            m_Action |= uint32_t( action );
        }
        else
        {
            m_Action &= ~uint32_t( action );
        }
    }

    void toggle( uint32_t action )
    {
        if ( isEnabled( action ) )
        {
            disable( action );
        }
        else
        {
            enable( action );
        }
    }
};

} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_ACTIONTYPE_HPP
