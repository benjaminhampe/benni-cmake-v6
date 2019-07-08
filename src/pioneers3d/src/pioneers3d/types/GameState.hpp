/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_GAMESTATE_HPP
#define PIONEERS3D_TYPES_GAMESTATE_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

enum class GameState : uint32_t
{
    NOT_RUNNING = 0,
    IDLE,
    PLACE_OBJECT, /// @see eGameObject
//    PLACE_ROBBER,
//    PLACE_ROAD,
//    PLACE_SETTLEMENT,
//    PLACE_CITY,
    COUNT
};

} // end namespace pioneers3d

inline std::ostream &
operator<< ( std::ostream & o, pioneers3d::GameState egs )
{
    switch( egs )
    {
       case pioneers3d::GameState::NOT_RUNNING: o << "NOT_RUNNING"; break;
       case pioneers3d::GameState::IDLE:        o << "IDLE"; break;
       case pioneers3d::GameState::PLACE_OBJECT: o << "PLACE_OBJECT"; break;
       default:                                 o << "UNKNOWN"; break;
    }
    return o;
}

#endif // PIONEERS3D_TYPES_GAMESTATE_HPP
