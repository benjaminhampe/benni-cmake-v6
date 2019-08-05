#ifndef GAMEEVENTRECEIVER_HPP
#define GAMEEVENTRECEIVER_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

class Game_t;

class GameEventReceiver : public irrExt::BaseEventReceiver
{
public:
    explicit GameEventReceiver( Game_t* game = nullptr );
    virtual ~GameEventReceiver();
    virtual bool OnEvent( irr::SEvent const & event ) override;
public:
    Game_t* m_Game = nullptr;
};

} // end namespace pioneers3d


#endif // GAMEEVENTRECEIVER_HPP
