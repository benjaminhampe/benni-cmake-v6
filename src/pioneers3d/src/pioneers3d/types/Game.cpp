#include <pioneers3d/types/Game.hpp>

namespace pioneers3d {

glm::ivec2
Game_getScreenSize( Game_t * game )
{
   if ( !game || !game->Device || !game->Device->getVideoDriver() ) return glm::ivec2(0,0);
   irr::core::dimension2du const uScreenSize = game->Device->getVideoDriver()->getScreenSize();
   return glm::ivec2( int32_t( uScreenSize.Width ), int32_t( uScreenSize.Height ) );
}

irr::IrrlichtDevice*
Game_getDevice( Game_t * game )
{
   if ( !game ) return nullptr;
   return game->Device;
}

irr::video::IVideoDriver*
Game_getVideoDriver( Game_t * game )
{
   if ( !game ) return nullptr;
   if ( !game->Device ) return nullptr;
   return game->Device->getVideoDriver();
}

irr::gui::IGUIEnvironment*
Game_getGUIEnvironment( Game_t * game )
{
   if ( !game ) return nullptr;
   if ( !game->Device ) return nullptr;
   return game->Device->getGUIEnvironment();
}

irr::scene::ISceneManager*
Game_getSceneManager( Game_t * game )
{
   if ( !game ) return nullptr;
   if ( !game->Device ) return nullptr;
   return game->Device->getSceneManager();
}

std::string
Game_getAssetFileName( Game_t * game, std::string const & relativeMediaFileName )
{
//   if ( !game ) return relativeMediaFileName; // Cant do anything
//   std::string fileName2 = game->MediaDir + relativeMediaFileName;
//   std::string fileName3 = de::hampe::os::FileSystemSTL::absoluteFileName( fileName2 );
//   std::cout << __FUNCTION__ << " :: [benni] << " << relativeMediaFileName << "\n";
//   std::cout << __FUNCTION__ << " :: [benni] == " << fileName2 << "\n";
//   std::cout << __FUNCTION__ << " :: [benni] >> " << fileName3 << "\n";
//   return fileName3;
   assert( game );
   return de::hampe::os::FileSystemSTL::absoluteFileName( game->MediaDir + relativeMediaFileName );
}

Font_t
Game_getFont( Game_t * game, eFontType fontType )
{
   assert( game );
   return game->FontManager.get( fontType );
}

void
Game_clear( Game_t* game )
{
   Board_clear( game );
   clearPlayers( game );
}

void
Game_destroy( Game_t* game )
{
   assert( game );
   Chat_destroy( game );
   game->FontManager.clear();
}

//    pioneers3d::Game_t game( device );
//    game.create();
//    game.save( "standard_test1.xml" );
//    game.load( "standard_test1.xml" );
//    game.save( "standard_test2.xml" );


void
Game_createStandard( Game_t* game, irr::IrrlichtDevice* device, int argc, char** argv )
{
   srand( static_cast< uint32_t >( time( nullptr ) ) );
   static_cast< void >( argc );

   std::cout << "// ==========================================================\n";
   std::cout << __FUNCTION__ << ":\n";
   std::cout << "// ==========================================================\n";

   assert( game );
   assert( device );
   game->Device = device;
   game->ExeFile = argv[0];
   game->ExeDir = de::hampe::os::FileSystemSTL::filePath( game->ExeFile );
   game->MediaDir = "../../media/";
   game->Type = GameType::STANDARD;
   game->State = GameState::IDLE;

   device->setResizable( true );
   device->setWindowCaption( L"Pioneers3D (c) 2018 by Benjamin Hampe <benjaminhampe@gmx.de>" );

   game->ClearColor = irr::video::SColor( 255, 225, 225, 255 );
   game->Receiver = new GameEventReceiver( game );
   game->Camera = new CylinderCamera( game->Device->getSceneManager(), game->Device->getSceneManager()->getRootSceneNode() );

   std::cout << "ExeFile = " << game->ExeFile << "\n";
   std::cout << "ExeDir = " << game->ExeDir << "\n";
   std::cout << "MediaDir = " << game->MediaDir << "\n";
   std::cout << "ClearColor = " << game->ClearColor << "\n";
   std::cout << "Type = " << uint32_t( game->Type ) << "\n";
   std::cout << "State = " << uint32_t( game->State ) << "\n";

   //Game_getFont( env, Game_getAssetFileName( game, "fonts/FontAwesome.ttf"), 8, true, true );
   irr::gui::IGUIEnvironment* env = device->getGUIEnvironment();
   game->FontManager.init( env );

   game->Text.Marquee.Font = Game_getFont( game, eFontType::Marquee );

   glm::ivec2 screenSize = Game_getScreenSize( game );
   glm::ivec2 screenPos = glm::ivec2( screenSize.x / 2, 50 );
   Font_t h1 = Game_getFont( game, eFontType::H1 );
   Font_t h2 = Game_getFont( game, eFontType::H2 );
   Font_t h3 = Game_getFont( game, eFontType::H3 );
   Font_t h4 = Game_getFont( game, eFontType::H4 );
   Font_t h5 = Game_getFont( game, eFontType::H5 );
   Font_t h6 = Game_getFont( game, eFontType::H6 );

   game->Text.H1.setData( h1, "H1 Line", screenPos, 0xFFFF0000 ); screenPos.y += h1.getTextSize( game->Text.H1.Text ).y;
   game->Text.H2.setData( h2, "H2 Line", screenPos, 0xFFFF0000 ); screenPos.y += h2.getTextSize( game->Text.H2.Text ).y;
   game->Text.H3.setData( h3, "H3 Line", screenPos, 0xFFFF0000 ); screenPos.y += h3.getTextSize( game->Text.H3.Text ).y;
   game->Text.H4.setData( h4, "H4 Line", screenPos, 0xFFFF0000 ); screenPos.y += h4.getTextSize( game->Text.H4.Text ).y;
   game->Text.H5.setData( h5, "H5 Line", screenPos, 0xFFFF0000 ); screenPos.y += h5.getTextSize( game->Text.H5.Text ).y;
   game->Text.H6.setData( h6, "H6 Line", screenPos, 0xFFFF0000 ); screenPos.y += h6.getTextSize( game->Text.H6.Text ).y;

   //GameBuilder_createRessourceCardTexture( game, eTileType::HOLZ, "card_holz.jpg" );
   //GameBuilder_createRessourceCardTexture( game, eTileType::LEHM, "card_lehm.jpg" );
   //GameBuilder_createRessourceCardTexture( game, eTileType::WEIZEN, "card_weizen.jpg" );
   //GameBuilder_createRessourceCardTexture( game, eTileType::WOLLE, "card_wolle.jpg" );
   //GameBuilder_createRessourceCardTexture( game, eTileType::ERZ, "card_erz.jpg" );
   Game_createCardObjectsDemo( game );

   //Board_create( game );
   Board_createStandard( game );
   Game_createRaeuber( game );

   //Players_createStandard( game );
   addStandardPlayer( game, 1, "Benni", 0xFF30ED30 );
   addStandardPlayer( game, 2, "Robot 1", 0xFFFF0000 );
   // addStandardPlayer( game, 3, "Robot 2", 0xFFFFFF00 );
   // addStandardPlayer( game, 4, "Robot 3", 0xFF0000FF );

   irr::video::ITexture* top = Texture_get( game, eTexture::SKYBOX_TOP );
   irr::video::ITexture* bottom = Texture_get( game, eTexture::SKYBOX_BOTTOM );
   irr::video::ITexture* left = Texture_get( game, eTexture::SKYBOX_LEFT );
   irr::video::ITexture* right = Texture_get( game, eTexture::SKYBOX_RIGHT );
   irr::video::ITexture* front = Texture_get( game, eTexture::SKYBOX_FRONT );
   irr::video::ITexture* back = Texture_get( game, eTexture::SKYBOX_BACK );
   irr::scene::ISceneManager* smgr = device->getSceneManager();
   irr::scene::ISceneNode* smgrRoot = smgr->getRootSceneNode();
   smgr->addSkyBoxSceneNode( top, bottom, left, right, front, back, smgrRoot, -1 );

   UI_create( game );
   Chat_create( game );

   std::cout << __FUNCTION__ << "=====================================================================\n";
   std::cout << __FUNCTION__ << "[Debug]:\n";
   std::cout << __FUNCTION__ << "=====================================================================\n";
   std::cout << __FUNCTION__ << "[Board]:\n" << game->Board << "\n";
   std::cout << __FUNCTION__ << "=====================================================================\n";
   std::cout << __FUNCTION__ << "\n[READY] Ready to play, just start game\n";
}


void
Game_start( Game_t * game )
{
    UI_setWindowVisible( game, eWindow::ALL, false );
    UI_setWindowVisible( game, eWindow::INIT_DICE, true );
    Board_hideWaypointsR( game );
    Board_hideWaypointsS( game );

    game->Round = 0;
    game->Player = 0;
    for ( int32_t i = 0; i < Player_getCount( game ); ++i )
    {
        Player_setAction( game, i, eAction::DICE );
    }

    UI_update( game );
    game->State = GameState::IDLE;
}


/// Start main loop:
int
Game_exec( Game_t * game )
{
    assert( game );
   assert( game->Device );

   // locals
   irr::IrrlichtDevice* device = game->Device;
    irr::video::IVideoDriver* driver = device->getVideoDriver();
    irr::scene::ISceneManager* smgr = device->getSceneManager();
    irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
    //irr::core::dimension2du screenSize = driver->getScreenSize();
    glm::ivec2 screenSize = Game_getScreenSize( game );

   // locals
    uint64_t startTime = device->getTimer()->getRealTime();
    uint64_t currTime = device->getTimer()->getRealTime();
    uint64_t lastScreenUpdateTime = device->getTimer()->getRealTime();
    uint64_t lastWindowTitleUpdateTime = device->getTimer()->getRealTime();
    uint64_t updateScreenMs = 10;
    uint64_t updateWindowTitleMs = 1000 / 10;

   // main loop:
    while (device && device->run())
    {
        currTime = device->getTimer()->getRealTime();

        if ( game->State == GameState::PLACE_OBJECT )
        {
            if ( game->EnableCollisionDetectionForWaypoints )
            {
                game->SelectedWaypointR = Board_isMouseOverWaypointR( game );
                game->SelectedWaypointS = Board_isMouseOverWaypointS( game );
            }

            if ( game->EnableCollisionDetectionForTiles )
            {
                game->SelectedTile = Board_isMouseOverTile( game );
            }

            if ( game->PlaceObject )
            {
                irr::IrrlichtDevice* device = game->Device;
                irr::scene::ISceneManager* smgr = device->getSceneManager();
                irr::core::position2di mousePos = device->getCursorControl()->getPosition();
                irr::core::line3df screenRay = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates( mousePos, smgr->getActiveCamera() );
                irr::core::plane3df xzPlane( irr::core::vector3df(0,0,0), irr::core::vector3df(0,1,0) );
                irr::core::vector3df hitPosition;
                if ( xzPlane.getIntersectionWithLine( screenRay.start, screenRay.getVector(), hitPosition ) )
                {
                    game->PlaceObject->setPosition( hitPosition );
                }

            } // endif PlaceObject
        } // endif PLACE_OBJECT

        if ( device->isWindowActive() )
        {
            if ( currTime - lastScreenUpdateTime >= updateScreenMs )
            {
                lastScreenUpdateTime = currTime;

                if (driver)
                {
                    driver->beginScene( true, true, game->ClearColor );

                    screenSize = Game_getScreenSize( game );

                    if ( smgr )
                    {
                        smgr->drawAll();
                    }

                    if ( guienv )
                    {
                        guienv->drawAll();

                    }


            // game
               game->Text.H1.draw();
               game->Text.H2.draw();
               game->Text.H3.draw();
               game->Text.H4.draw();
               game->Text.H5.draw();
               game->Text.H6.draw();

                    int y = screenSize.y/2;
                    int lineHeight = 50;

                    {
                        std::stringstream s; s << "FPS(" << std::to_string( driver->getFPS() ) << ")";
                        Font_draw( Game_getFont( game, eFontType::FPS_COUNTER ), s.str(), glm::ivec2( 10,10+y ), 0xFFFFFF00 );
                        y += lineHeight;
                    }
                    {
                        std::stringstream s; s << "Round = " << game->Round;
                        Font_draw( Game_getFont( game, eFontType::FPS_COUNTER ), s.str(), glm::ivec2( 10,10+y ), 0xFF00DF00 );
                        y += lineHeight;
                    }
                    {
                        std::stringstream s; s << "GameState = " << game->State;
                        Font_draw( Game_getFont( game, eFontType::FPS_COUNTER ), s.str(), glm::ivec2( 10,10+y ), 0xFFFFDF00 );
                        y += lineHeight;
                    }
                    if ( game->State == GameState::PLACE_OBJECT )
                    {
                        std::stringstream s; s << "ePlaceObjectType = " << game->PlaceObjectType;
                        Font_draw( Game_getFont( game, eFontType::FPS_COUNTER ), s.str(), glm::ivec2( 10,10+y ), 0xFFFFAF00 );
                        y += lineHeight;
                    }
                    {
                        std::stringstream s; s << "Player = " << ( game->Player+1 ) << ", " << Player_getName( game );
                        Font_draw( Game_getFont( game, eFontType::FPS_COUNTER ), s.str(), glm::ivec2( 10,10+y ), 0xFFFF9520 );
                        y += lineHeight;
                    }

                    Chat_draw( game, screenSize.x - 400, screenSize.y/2 - 200 );

                    driver->endScene();
                }

            }

            if ( currTime - lastWindowTitleUpdateTime >= updateWindowTitleMs )
            {
                if ( game->SelectedWaypointR )
                {
                  // std::cout << "[Waypoint_R] " << *game->SelectedWaypointR << "\n";
                }
                if ( game->SelectedWaypointS )
                {
                  // std::cout << "[Waypoint_S] " << *game->SelectedWaypointS << "\n";
                }
                if ( game->SelectedTile )
                {
                  // std::cout << "[Tile] " << *game->SelectedTile << "\n";
                }

                lastWindowTitleUpdateTime = currTime;

                std::stringstream s;
                s << "Pioneers3D - screen(" << screenSize << ")"
                  << ", fps(" << driver->getFPS() << ")"
                  << ", tris(" << device->getVideoDriver()->getPrimitiveCountDrawn( 0 ) << ")"
                  << ", tex(" << device->getVideoDriver()->getTextureCount() << ")";

                irr::scene::ICameraSceneNode* camera = smgr->getActiveCamera();
                if ( camera )
                {
                    s << "camPos(" << camera->getAbsolutePosition() << "), ";
                    s << "camEye(" << camera->getTarget() << "), ";
                    s << "camAng(" << camera->getRotation() << "), ";
                }



                device->setWindowCaption( irr::core::stringw( s.str().c_str() ).c_str() );
            }
        }
        else
        {
            device->yield();
        }

    }
    return 0;
}

void
Game_load( std::string const & fileName )
{

}

void
Game_save( std::string const & fileName )
{

}

} // end namespace pioneers3d
