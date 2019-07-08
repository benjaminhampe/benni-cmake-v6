#include <pioneers3d/types/Texture.hpp>
#include <pioneers3d/types/Game.hpp>

namespace pioneers3d {

const char*
Texture_getName( eTexture const type )
{
   switch( type )
   {
      case eTexture::UNKNOWN: return "unknown.jpg";
      case eTexture::SKYBOX_TOP: return "skybox/ct/top.jpg";
      case eTexture::SKYBOX_BOTTOM: return "skybox/ct/bottom.jpg";
      case eTexture::SKYBOX_LEFT: return "skybox/ct/left.jpg";
      case eTexture::SKYBOX_RIGHT: return "skybox/ct/right.jpg";
      case eTexture::SKYBOX_FRONT: return "skybox/ct/front.jpg";
      case eTexture::SKYBOX_BACK: return "skybox/ct/back.jpg";

      case eTexture::DICE_1: return "dice/dice1.png";
      case eTexture::DICE_2: return "dice/dice2.png";
      case eTexture::DICE_3: return "dice/dice3.png";
      case eTexture::DICE_4: return "dice/dice4.png";
      case eTexture::DICE_5: return "dice/dice5.png";
      case eTexture::DICE_6: return "dice/dice6.png";

      case eTexture::CHIP_W: return "chips/waypoint_w.png";
      case eTexture::CHIP_S: return "chips/waypoint_s.png";
      case eTexture::CHIP_HOLZ: return "chips/chip_holz.jpg";
      case eTexture::CHIP_LEHM: return "chips/chip_lehm.png";
      case eTexture::CHIP_WEIZEN: return "chips/chip_weizen.jpg";
      case eTexture::CHIP_WOLLE: return "chips/chip_wolle.jpg";
      case eTexture::CHIP_ERZ: return "chips/chip_erz.jpg";
      case eTexture::CHIP_2: return "chips/chip02.png";
      case eTexture::CHIP_3: return "chips/chip03.png";
      case eTexture::CHIP_4: return "chips/chip04.png";
      case eTexture::CHIP_5: return "chips/chip05.png";
      case eTexture::CHIP_6: return "chips/chip06.png";
      case eTexture::CHIP_8: return "chips/chip08.png";
      case eTexture::CHIP_9: return "chips/chip09.png";
      case eTexture::CHIP_10: return "chips/chip10.png";
      case eTexture::CHIP_11: return "chips/chip11.png";
      case eTexture::CHIP_12: return "chips/chip12.png";

      case eTexture::TILE_WASSER: return "tiles/wasser.png";
      case eTexture::TILE_WUESTE: return "tiles/wueste.jpg";
      case eTexture::TILE_HOLZ: return "tiles/land_holz.png";
      case eTexture::TILE_LEHM: return "tiles/land_lehm.png";
      case eTexture::TILE_WEIZEN: return "tiles/land_weizen.jpg";
      case eTexture::TILE_WOLLE: return "tiles/land_wolle.jpg";
      case eTexture::TILE_ERZ: return "tiles/land_erz.jpg";

      // Composed textures
      case eTexture::CARD_HOLZ:       return "cards/card_holz.jpg";
      case eTexture::CARD_LEHM:       return "cards/card_lehm.jpg";
      case eTexture::CARD_WEIZEN:     return "cards/card_weizen.jpg";
      case eTexture::CARD_WOLLE:      return "cards/card_wolle.jpg";
      case eTexture::CARD_ERZ:        return "cards/card_erz.jpg";
      case eTexture::CARD_EVENT:      return "cards/event_card.png"; // The back of event cards
      case eTexture::CARD_KNIGHT:     return "cards/event_card_knight.png";
      case eTexture::CARD_POINT:      return "cards/event_card_point.png";
      case eTexture::CARD_BONUS:      return "cards/bonus_card.png";
      case eTexture::CARD_BONUS_ARMY: return "cards/bonus_card_army.png";
      case eTexture::CARD_BONUS_ROAD: return "cards/bonus_card_road.png";
      case eTexture::STAT_ROAD:       return "cards/stat_roads.png";
      case eTexture::STAT_SETTLEMENT: return "cards/stat_settlements.png";
      case eTexture::STAT_CITY:       return "cards/stat_cities.png";

      case eTexture::ACTION_DICE:             return "action/dice.png";
      case eTexture::ACTION_PLACE_ROBBER:     return "action/place_robber.png";
      case eTexture::ACTION_ENDTURN:          return "action/endturn.png";
      case eTexture::ACTION_TRADE:            return "action/trade.png";
      case eTexture::ACTION_BANK:             return "action/bank.png";
      case eTexture::ACTION_PLAY_EVENT_CARD:  return "action/play_event_card.png";
      case eTexture::ACTION_BUY_EVENT_CARD:   return "action/buy_event_card.png";
      case eTexture::ACTION_BUY_ROAD:         return "action/buy_road.png";
      case eTexture::ACTION_BUY_SETTLEMENT:   return "action/buy_settlement.png";
      case eTexture::ACTION_BUY_CITY:         return "action/buy_city.png";

      case eTexture::TEX_ROAD:    return "tex_road.jpg";
      case eTexture::TEX_ROBBER:  return "raeuber/raeuber_tex.jpg";

      case eTexture::PLAYER_ICON: return "player/unknown.png";
      case eTexture::PLAYER_ICON_1: return "player_avatars/genghis.png";
      case eTexture::PLAYER_ICON_2: return "player_avatars/gandhi.png";
      case eTexture::PLAYER_ICON_3: return "player_avatars/catherine.png";
      case eTexture::PLAYER_ICON_4: return "player_avatars/hatshepsut.png";

      default: return ""; // Always return empty string, not a nullptr
    }
}


irr::video::ITexture*
Texture_get( Game_t * game, eTexture const type )
{
    char const * const texName = Texture_getName( type );

    if ( strlen( texName ) < 3 )
    {
        std::cout << __FUNCTION__ << " [Error] :: Invalid texName(" << texName << ")\n";
        return nullptr;
    }

    assert( game );
    assert( game->Device );
    assert( game->Device->getVideoDriver() );
    std::string const fileName = Game_getAssetFileName( game, texName );
    irr::video::IVideoDriver * driver = game->Device->getVideoDriver();
    driver->setTextureCreationFlag( irr::video::ETCF_ALWAYS_32_BIT, true );
    //driver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, false );

    irr::video::ITexture* tex = driver->getTexture( fileName.c_str() );
    if ( !tex )
    {
        std::cout << __FUNCTION__ << " [Error] :: Cannot get texture (" << fileName << ")\n";
        return nullptr;
    }

    return tex;

}

irr::video::ITexture*
Game_getDiceTexture( Game_t* game, int diceValue )
{
    switch( diceValue )
    {
        case 1: return Texture_get( game, eTexture::DICE_1 );
        case 2: return Texture_get( game, eTexture::DICE_2 );
        case 3: return Texture_get( game, eTexture::DICE_3 );
        case 4: return Texture_get( game, eTexture::DICE_4 );
        case 5: return Texture_get( game, eTexture::DICE_5 );
        case 6: return Texture_get( game, eTexture::DICE_6 );
        default: return Texture_get( game, eTexture::UNKNOWN );
    }
}

irr::video::ITexture*
Game_getPlayerTexture( Game_t* game, int player )
{
    switch( player )
    {
        case 1: return Texture_get( game, eTexture::PLAYER_ICON_1 );
        case 2: return Texture_get( game, eTexture::PLAYER_ICON_2 );
        case 3: return Texture_get( game, eTexture::PLAYER_ICON_3 );
        case 4: return Texture_get( game, eTexture::PLAYER_ICON_4 );
        default: return Texture_get( game, eTexture::PLAYER_ICON );
    }
}

irr::video::ITexture*
Game_getTileTexture( Game_t* game, TileType tileType )
{
    eTexture texType = eTexture::TILE_WASSER;
    switch( tileType )
    {
        case TileType::LAND_WUESTE: texType = eTexture::TILE_WUESTE; break;
        case TileType::LAND_HOLZ: texType = eTexture::TILE_HOLZ; break;
        case TileType::LAND_LEHM: texType = eTexture::TILE_LEHM; break;
        case TileType::LAND_WEIZEN: texType = eTexture::TILE_WEIZEN; break;
        case TileType::LAND_WOLLE: texType = eTexture::TILE_WOLLE; break;
        case TileType::LAND_ERZ: texType = eTexture::TILE_ERZ; break;
        default: break;
    }

    return Texture_get( game, texType );
}

irr::video::ITexture*
Game_getCardTexture( Game_t* game, TileType tileType )
{
    eTexture texType = eTexture::UNKNOWN;
    if ( tileType.contains( TileType::HOLZ ) ) texType = eTexture::CARD_HOLZ;
    if ( tileType.contains( TileType::LEHM ) ) texType = eTexture::CARD_LEHM;
    if ( tileType.contains( TileType::WEIZEN ) ) texType = eTexture::CARD_WEIZEN;
    if ( tileType.contains( TileType::WOLLE ) ) texType = eTexture::CARD_WOLLE;
    if ( tileType.contains( TileType::ERZ ) ) texType = eTexture::CARD_ERZ;

    return Texture_get( game, texType );
}

irr::video::ITexture*
Game_getChipTexture( Game_t* game, TileType tileType, int diceValue )
{
    eTexture texType = eTexture::UNKNOWN;
    if ( tileType.isHafen() )
    {
        switch( tileType )
        {
            //case TileType::HAFEN_3zu1: texType = eTexture::UNKNOWN; break;
            case TileType::HAFEN_HOLZ: texType = eTexture::CHIP_HOLZ; break;
            case TileType::HAFEN_LEHM: texType = eTexture::CHIP_LEHM; break;
            case TileType::HAFEN_WEIZEN: texType = eTexture::CHIP_WEIZEN; break;
            case TileType::HAFEN_WOLLE: texType = eTexture::CHIP_WOLLE; break;
            case TileType::HAFEN_ERZ: texType = eTexture::CHIP_ERZ; break;
            default: break;
        }
    }
    else if ( tileType.isLand() )
    {
        switch( diceValue )
        {
            case 2: texType = eTexture::CHIP_2; break;
            case 3: texType = eTexture::CHIP_3; break;
            case 4: texType = eTexture::CHIP_4; break;
            case 5: texType = eTexture::CHIP_5; break;
            case 6: texType = eTexture::CHIP_6; break;
            //case 7: texType = eTexture::CHIP_7; break;
            case 8: texType = eTexture::CHIP_8; break;
            case 9: texType = eTexture::CHIP_9; break;
            case 10: texType = eTexture::CHIP_10; break;
            case 11: texType = eTexture::CHIP_11; break;
            case 12: texType = eTexture::CHIP_12; break;
            default: break;
        }
    }

    return Texture_get( game, texType );
}

irr::video::ITexture*
GameBuilder_getRessourceCardTexture( Game_t* game, TileType tt, bool fg )
{
    eTexture texType = eTexture::UNKNOWN;
    if ( fg )
    {
             if (tt.contains( TileType::HOLZ) ) { texType = eTexture::CHIP_HOLZ; }
        else if (tt.contains( TileType::LEHM) ) { texType = eTexture::CHIP_LEHM; }
        else if (tt.contains( TileType::WEIZEN) ) { texType = eTexture::CHIP_WEIZEN; }
        else if (tt.contains( TileType::WOLLE) ) { texType = eTexture::CHIP_WOLLE; }
        else if (tt.contains( TileType::ERZ) ) { texType = eTexture::CHIP_ERZ; }
    }
    else
    {
             if (tt.contains( TileType::HOLZ) ) { texType = eTexture::TILE_HOLZ; }
        else if (tt.contains( TileType::LEHM) ) { texType = eTexture::TILE_LEHM; }
        else if (tt.contains( TileType::WEIZEN) ) { texType = eTexture::TILE_WEIZEN; }
        else if (tt.contains( TileType::WOLLE) ) { texType = eTexture::TILE_WOLLE; }
        else if (tt.contains( TileType::ERZ) ) { texType = eTexture::TILE_ERZ; }
    }

    return Texture_get( game, texType );
}

irr::video::ITexture*
GameBuilder_createRessourceCardTexture( Game_t* game, TileType tileType, std::string const & fileName )
{
    assert( game );
    assert( game->Device );

    irr::IrrlichtDevice* device = game->Device;
    irr::video::IVideoDriver* driver = device->getVideoDriver();
    irr::scene::ISceneManager* smgr = device->getSceneManager();
    smgr->clear();

    AutoSceneNode* node = new AutoSceneNode( smgr, smgr->getRootSceneNode(), -1 );
    node->add( createRoundRect( glm::vec3(0,0,    0), glm::vec2(600,1000), glm::vec2(40,40), 9, tileType.getRessourceColor() ), true );
    node->add( createRoundRect( glm::vec3(0,0,-0.05f), glm::vec2(580,980), glm::vec2(40,40), 9, 0xFFFFFFFF ), true );
    AutoMeshBuffer* bg0 = createRoundRect( glm::vec3(0,0,-0.15f), glm::vec2(540,940), glm::vec2(40,40), 9, 0xFFFFFFFF );
    bg0->setTexture( 0, GameBuilder_getRessourceCardTexture( game, tileType, false ) );
    node->add( bg0, true );
    node->add( createCircle( glm::vec3(0,0,-0.2f), 190.f, 36, tileType.getRessourceColor() ), true );
    AutoMeshBuffer* bg1 = createCircle( glm::vec3(0,0,-0.25f), 180.f, 36, 0xFFFFFFFF );
    bg1->setTexture( 0, GameBuilder_getRessourceCardTexture( game, tileType, true ) );
    node->add( bg1, true );

    irr::video::ITexture* tex = driver->getTexture( "offscreen_0" );
    if ( !tex )
    {
        tex = driver->addRenderTargetTexture( irr::core::dimension2du(1024,1024), "offscreen_0", irr::video::ECF_A8R8G8B8 );
    }

    driver->setRenderTarget( tex, true, true, 0xFFFFFFFF );
    //driver->beginScene( true, true, 0xFFFFFFFF ); // 0x00000000 );

    irr::core::matrix4 m_WorldMatrix; m_WorldMatrix.makeIdentity();
    irr::core::matrix4 m_ViewMatrix; m_ViewMatrix.makeIdentity();
    irr::core::matrix4 m_ProjMatrix; m_ProjMatrix.buildProjectionMatrixOrthoLH( tex->getSize().Width, tex->getSize().Height, 1.0f, 10000.0f );
    driver->setTransform( irr::video::ETS_WORLD, m_WorldMatrix );
    driver->setTransform( irr::video::ETS_VIEW, m_ViewMatrix );
    driver->setTransform( irr::video::ETS_PROJECTION, m_ProjMatrix );
    node->render();
    //driver->endScene();
    driver->setRenderTarget( 0 );
    node->drop();
    smgr->clear();

    // std::stringstream s; s << "card_"<< tileType.getRessourceString() << ".jpg";

    irr::core::dimension2du texSize = tex->getOriginalSize();
    irr::video::IImage* img = driver->createImage( tex, irr::core::position2di(200,0), irr::core::dimension2du( texSize.Width-400, texSize.Height ) );
    if ( img )
    {
        driver->writeImageToFile( img, (game->MediaDir + fileName).c_str() );
        img->drop();
    }


    //rtt0->drop();

    return tex;
}

} // end namespace pioneers3d
