/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_eTexture_HPP
#define PIONEERS3D_TYPES_eTexture_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

enum class eTexture : uint32_t
{
    UNKNOWN = 0,
    SKYBOX_TOP,
    SKYBOX_BOTTOM,
    SKYBOX_LEFT,
    SKYBOX_RIGHT,
    SKYBOX_FRONT,
    SKYBOX_BACK,
    DICE_1,
    DICE_2,
    DICE_3,
    DICE_4,
    DICE_5,
    DICE_6,
    CHIP_W,
    CHIP_S,
    CHIP_HOLZ,
    CHIP_LEHM,
    CHIP_WEIZEN,
    CHIP_WOLLE,
    CHIP_ERZ,
    CHIP_2,
    CHIP_3,
    CHIP_4,
    CHIP_5,
    CHIP_6,
    //CHIP_7,
    CHIP_8,
    CHIP_9,
    CHIP_10,
    CHIP_11,
    CHIP_12,
    TILE_WASSER,
    TILE_WUESTE,
    TILE_HOLZ,
    TILE_LEHM,
    TILE_WEIZEN,
    TILE_WOLLE,
    TILE_ERZ,
    CARD_HOLZ, // Composed ressource card texture
    CARD_LEHM,  // Composed ressource card texture
    CARD_WEIZEN,    // Composed ressource card texture
    CARD_WOLLE, // Composed ressource card texture
    CARD_ERZ,   // Composed ressource card texture
    CARD_EVENT,  // Composed event card texture
    CARD_KNIGHT,  // Composed event card texture
    CARD_POINT,  // Composed event card texture
    CARD_INVENTION,  // Composed event card texture
    CARD_BONUS,  // Composed event card texture
    CARD_BONUS_ARMY,  // Composed event card texture
    CARD_BONUS_ROAD,  // Composed event card texture
    STAT_ROAD,  // Composed icon texture
    STAT_SETTLEMENT,  // Composed icon texture
    STAT_CITY,  // Composed icon texture

    ACTION_DICE,
    ACTION_PLACE_ROBBER,
    ACTION_ENDTURN,
    ACTION_TRADE,
    ACTION_BANK,
    ACTION_BUY_EVENT_CARD,
    ACTION_PLAY_EVENT_CARD,
    ACTION_BUY_ROAD,
    ACTION_BUY_SETTLEMENT,
    ACTION_BUY_CITY,
    PLAYER_ICON,
    PLAYER_ICON_1,
    PLAYER_ICON_2,
    PLAYER_ICON_3,
    PLAYER_ICON_4,
    TEX_ROBBER,
    TEX_TILE_BG,
    TEX_ROAD,
    TEX_SETTLEMENT = TEX_ROAD,
    TEX_CITY = TEX_ROAD,
    BUY_EVENT_CARD,
    PLAN_BG,
    PLAN_STANDARD,
    COUNT
};

} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_eTexture_HPP
