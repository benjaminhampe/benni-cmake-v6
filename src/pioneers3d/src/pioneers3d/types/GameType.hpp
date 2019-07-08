/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_GAMETYPE_HPP
#define PIONEERS3D_TYPES_GAMETYPE_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

class GameType
{
public:
    enum eType : uint32_t
    {
        UNKNOWN = 0,
        STANDARD = 1,
        FISHER = 2,
        PIRATES = 4,
        THIEFS = 8,
        CARDS = 16,
        CHEATS = 128,
        ALL = STANDARD | FISHER | PIRATES | THIEFS | CARDS
    };

    GameType() : m_Value( STANDARD )
    {}

    GameType( eType type ) : m_Value( type )
    {}

    GameType( uint32_t type ) : m_Value( static_cast< eType >( type ) )
    {}

    operator eType() const { return m_Value; }

    bool
    hasAddon( eType type ) const { return m_Value & type; }

    eType
    value() const { return m_Value; }

    void
    setValue( eType value ) { m_Value = value; }

    GameType&
    operator|=( eType type )
    {
        m_Value = static_cast< eType >( uint32_t( m_Value ) | uint32_t( type ) );
        return *this;
    }

    GameType&
    operator|=( GameType const & type )
    {
        m_Value = static_cast< eType >( uint32_t( m_Value ) | uint32_t( type.value() ) );
        return *this;
    }

    /// @brief Parses the entire string for all values in enum GameType.
    /// The results are OR'ed together and returned as one value containing bitflags.
    static GameType
    parseString( std::string const & src )
    {
        GameType gameType;
        if ( src.find( "STANDARD" ) != std::string::npos ) { gameType |= GameType::STANDARD; }
        if ( src.find( "FISHER" ) != std::string::npos )   { gameType |= GameType::FISHER; }
        if ( src.find( "PIRATES" ) != std::string::npos )  { gameType |= GameType::PIRATES; }
        if ( src.find( "THIEFS" ) != std::string::npos )   { gameType |= GameType::THIEFS; }
        if ( src.find( "CARDS" ) != std::string::npos )    { gameType |= GameType::CARDS; }
        return gameType;
    }

    eType m_Value;
};

} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_GAMETYPE_HPP
