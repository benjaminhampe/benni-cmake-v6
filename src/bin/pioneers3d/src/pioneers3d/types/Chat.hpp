#ifndef GAME_CHAT_HPP
#define GAME_CHAT_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

   class Game_t;
   void Chat_create( Game_t * game );
   void Chat_destroy( Game_t * game );
   void Chat_clear( Game_t * game );
   void Chat_draw( Game_t * game, int32_t x, int32_t y );
   void Chat_print( Game_t * game, std::string txt, uint32_t color = 0xFF000000 );
   void Chat_printPlayerMessage( Game_t * game, int32_t player, std::string txt );
   void Chat_log( Game_t * game, de::hampe::logging::LogLevel const & level, std::string const & txt );
   std::string Chat_toString( Game_t * game );

} // end namespace pioneers3d


#endif // GAME_CHAT_HPP
