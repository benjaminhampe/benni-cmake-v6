#include <pioneers3d/types/Action.hpp>
#include <pioneers3d/types/Game.hpp>
#include <pioneers3d/types/Chat.hpp>
#include <pioneers3d/types/UI.hpp>

#define PRINT_FUNCTION_NAME { std::cout << __FUNCTION__ << ":" << __LINE__ << "()" << std::endl; }
#define PRINT_FUNCTION { Chat_log( game, de::hampe::logging::LogLevel::Debug, __FUNCTION__ ); }

namespace pioneers3d {


//void Action_Dice( Game_t* game );
//void Action_Bank( Game_t* game );
//void Action_Trade( Game_t* game );
//void Action_BuyEventCard( Game_t* game );
//void Action_BuyRoad( Game_t* game );
//void Action_BuySettlement( Game_t* game );
//void Action_BuyCity( Game_t* game );
//void Action_EndTurn( Game_t* game );
//void Action_PlayEventCard( Game_t* game );
//void Action_PlaceRobber( Game_t* game );
//void Action_PlaceRoad( Game_t* game );
//void Action_PlaceSettlement( Game_t* game );
//void Action_PlaceCity( Game_t* game );
//void Action_Win( Game_t* game );


void Action_Abort( Game_t* game )
{
    if ( game->State == GameState::NOT_RUNNING )
    {
        return;
    }

    if ( game->State == GameState::PLACE_OBJECT )
    {

        if (game->PlaceObject )
        {
            game->PlaceObject->drop();
            game->PlaceObject = nullptr;
        }

        if ( game->PlaceObjectType == ePlaceObjectType::ROBBER )
        {
            Player_setActionEnabled( game, eAction::PLACE_ROBBER, true );
        }
        if ( game->PlaceObjectType == ePlaceObjectType::ROAD )
        {
            Player_setActionEnabled( game, eAction::BUY_ROAD, true );
            Board_hideWaypointsR( game );
        }
        if ( game->PlaceObjectType == ePlaceObjectType::SETTLEMENT )
        {
            Player_setActionEnabled( game, eAction::BUY_SETTLEMENT, true );
            Board_hideWaypointsS( game );
        }
        if ( game->PlaceObjectType == ePlaceObjectType::CITY )
        {
            Player_setActionEnabled( game, eAction::BUY_CITY, true );
            Board_hideWaypointsS( game );
        }
    }

    game->State = GameState::IDLE;
    UI_update( game );
}

void Action_EndTurn( Game_t* game )
{
    // BEGIN CURRENT PLAYER
    uint32_t playerColor = Player_getColor( game, game->Player );
    std::string playerName = Player_getName( game, game->Player );
    Chat_print( game, de::hampe::sprintf("(", game->Player + 1, ") ", playerName.c_str(), " ends turn."), playerColor );
    // END CURRENT PLAYER

    // BEGIN NEXT PLAYER
    game->Player++;

    if ( game->Player >= int32_t( game->Players.size() ) )
    {
        game->Player = 0;
        game->Round++;
    }

    playerColor = Player_getColor( game, game->Player );
    playerName = Player_getName( game, game->Player );
    Chat_print( game, de::hampe::sprintf("(", game->Player + 1, ") ", playerName.c_str(), " begins turn..."), playerColor );

    Player_setActionEnabled( game, game->Player, eAction::ENDTURN, false );

    // DECIDE STATE
    if ( game->Round == 0 )
    {
        Player_setAction( game, eAction::DICE );
    }
    else if ( game->Round == 1 )
    {
        Player_setAction( game, eAction::BUY_ROAD | eAction::BUY_SETTLEMENT );
    }
    else if ( game->Round == 2 )
    {
        Player_setAction( game, eAction::BUY_ROAD | eAction::BUY_SETTLEMENT );
    }
    else if ( game->Round == 3 )
    {
        Player_setAction( game, eAction::DICE );
    }
    else
    {
        Player_setActionEnabled( game, eAction::DICE, true );
    }

    UI_setWindowVisible( game, eWindow::DICE, false );

    UI_update( game );
}

void Action_Dice( Game_t* game )
{
    UI_setWindowVisible( game, eWindow::DICE, true );
    game->Dice.A = rand() % 6 + 1;
    game->Dice.B = rand() % 6 + 1;
    game->UI.Dice.A->setImage( Game_getDiceTexture( game, game->Dice.A ) );
    game->UI.Dice.B->setImage( Game_getDiceTexture( game, game->Dice.B ) );

    int32_t playerIndex = game->Player;
    int32_t playerCount = Player_getCount( game );
    uint32_t playerColor = Player_getColor( game, playerIndex );
    std::string playerName = Player_getName( game, playerIndex );

    int dice = game->Dice.sum();

    Chat_print( game, de::hampe::sprintf(playerName.c_str(), " (", playerIndex + 1, ") got value (", dice, ") with dice (", game->Dice.A, " + ", game->Dice.B, ")" ), playerColor );

    Player_setActionEnabled( game, game->Player, eAction::ENDTURN, true );
    Player_setActionEnabled( game, game->Player, eAction::DICE, false );

    UI_update( game );

    if ( game->State != GameState::IDLE )
    {
        return;
    }

    // GAME PHASE, only after 2 set rounds and 1 dice round at the start of each game


    if ( dice == 7 )
    {
        Action_PlaceRobber( game );
        return;
    }
    else
    {
       // Get tiles with eyes of dice
       Board_t * board = Board_get( game );
      std::vector< Tile_t const* > diceTiles = board->getTilesByValue( dice );
      Chat_print( game, de::hampe::sprintf("tiles(", diceTiles.size(), ") with diceValue(", dice, ")" ), 0xFF000000 );

      // Give players new ressource cards:
      // Loop over all tiles
         // Loop over all player
            // Compute sum victory-points on tile for each player
            // Add sum victory-points of ressource as cards of tile-type to bank

      for ( size_t k = 0; k < diceTiles.size(); ++k )
      {
         Tile_t const * const tile = diceTiles[ k ];
         if ( !tile ) continue;

         for ( int32_t p = 0; p < Player_getCount( game ); ++p )
         {
            int32_t victoryPoints = tile->getPlayerPoints( p );
            if ( victoryPoints > 0 )
            {
               int32_t oldResCount = Player_getNumRessource( game, tile->m_Type );
               Player_setNumRessource( game, tile->m_Type, oldResCount + victoryPoints );

               Chat_print( game, de::hampe::sprintf("Player(", p, ") new cards(", victoryPoints, ") of type(", tile->m_Type.toString(), ")" ), 0xFF0000FF );
            }
         }
      }

    }
    // Collect ressource cards
    Bank_t bank;

    for ( uint32_t i = 0; i < game->Board.Tiles.size(); ++i )
    {
        Tile_t & tile = game->Board.Tiles[ i ];
        if ( tile.m_DiceValue == dice )
        {
            std::cout << __FUNCTION__ << " :: [Ok] Found Tile with diceValue(" << dice << ") that has type(" << tile.m_Type.toString() << ")\n";
            bank.add( tile.m_Type, 1 );
        }
    }

    std::cout << __FUNCTION__ << " :: [Ok] " << bank.toString() << "\n";
}

void Action_Bank( Game_t* game )
{
    Chat_printPlayerMessage( game, game->Player, "bank" );

    game->UI.Bank.Window->setVisible( true );
    //game->UI.Bank.Window->getEnvironment()->getRootGUIElement()->bringToFront( game->UI.Bank.Window );
}

void Action_Trade( Game_t* game )
{
    Chat_printPlayerMessage( game, game->Player, "trade" );
}

void Action_BuyEventCard( Game_t* game )
{
    Chat_printPlayerMessage( game, game->Player, "buy event card" );
}

void Action_BuyRoad( Game_t* game )
{
//    if ( game->State == GameState::PLACE_OBJECT )
//    {
//        return;
//    }
    game->State = GameState::PLACE_OBJECT;
    game->PlaceObjectType = ePlaceObjectType::ROAD;

    Chat_printPlayerMessage( game, game->Player, "buy road" );

    // create Road ( a box )
    irr::scene::ISceneManager* smgr = game->Device->getSceneManager();
    AutoSceneNode* node = new AutoSceneNode( smgr, smgr->getRootSceneNode(), -1 );
    node->add( createRotatedBox( glm::vec3( 0,2.5f,0 ), glm::vec3(30,5,5), glm::vec3(0,0,0), Player_getColor( game ) ), true );
    game->PlaceObject = node;

    Board_hideWaypointsS( game );
    Board_showWaypointsR( game );

    if ( game->Round == 0 )
    {
        // Dice Round, nothing todo...
    }
    else if ( game->Round == 1 )
    {
        Player_setActionEnabled( game, eAction::BUY_ROAD, false );
        if ( Player_getNumSettlements( game ) >= 1 )
        {
            Player_setActionEnabled( game, eAction::ENDTURN, true );
        }
    }
    else if ( game->Round == 2 )
    {
        Player_setActionEnabled( game, eAction::BUY_ROAD, false );





        if ( Player_getNumSettlements( game ) >= 2 )
        {
            Player_setActionEnabled( game, eAction::ENDTURN, true );
        }
    }
    else
    {
        // Normal game mode
    }

    UI_update( game );
}

void Action_BuySettlement( Game_t* game )
{
    game->State = GameState::PLACE_OBJECT;
    game->PlaceObjectType = ePlaceObjectType::SETTLEMENT;

    Chat_printPlayerMessage( game, game->Player, "buy settlement" );

    irr::scene::ISceneManager* smgr = game->Device->getSceneManager();
    AutoSceneNode* node = new AutoSceneNode( smgr, smgr->getRootSceneNode(), -1 );
    node->add( createRotatedBox( glm::vec3( 0,4,0 ), glm::vec3(10,8,10), glm::vec3(0,0,0), Player_getColor( game ) ), true );
    node->add( createRotatedBox( glm::vec3( 0,9,0 ), glm::vec3(8,2,8), glm::vec3(0,0,0), Player_getColor( game ) ), true );

    game->PlaceObject = node;

    Board_hideWaypointsR( game );
    Board_showWaypointsS( game );

    if ( game->Round == 0 )
    {
        // Dice Round, nothing todo...
    }
    else if ( game->Round == 1 )
    {
        Player_setActionEnabled( game, eAction::BUY_SETTLEMENT, false );
        if ( Player_getNumRoads( game ) >= 1 )
        {
            Player_setActionEnabled( game, eAction::ENDTURN, true );
        }
    }
    else if ( game->Round == 2 )
    {
        Player_setActionEnabled( game, eAction::BUY_SETTLEMENT, false );
        if ( Player_getNumRoads( game ) >= 2 )
        {
            Player_setActionEnabled( game, eAction::ENDTURN, true );
        }
    }
    else
    {
        // Normal game mode
    }

    UI_update( game );
}

void Action_BuyCity( Game_t* game )
{
    PRINT_FUNCTION
    game->State = GameState::PLACE_OBJECT;
}

void Action_PlaceRobber( Game_t * game )
{
    PRINT_FUNCTION
    game->State = GameState::PLACE_OBJECT;
}

void Action_PlaceRoad( Game_t* game )
{
    PRINT_FUNCTION
    game->State = GameState::PLACE_OBJECT;
}

void Action_PlaceSettlement( Game_t* game )
{
    PRINT_FUNCTION
    game->State = GameState::PLACE_OBJECT;
}

void Action_PlaceCity( Game_t* game )
{
    PRINT_FUNCTION
    game->State = GameState::PLACE_OBJECT;
}

} // end namespace pioneers3d
