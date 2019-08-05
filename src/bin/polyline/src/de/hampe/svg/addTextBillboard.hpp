#ifndef DE_HAMPE_SCENE_ADD_TEXT_BILLBOARD_HPP
#define DE_HAMPE_SCENE_ADD_TEXT_BILLBOARD_HPP

#include <irrExt/AutoMeshBufferUtils.hpp>

void
addTextBillboard( irr::scene::ISceneManager* smgr,
                  float32_t const lineHeight,
                  std::string const & textMessage,
                  float32_t const x, float32_t const y, float32_t const z,
                  uint32_t const colorTop, uint32_t const colorBottom );

inline void
addTextBillboard( irr::scene::ISceneManager* smgr,
                  float32_t const lineHeight,
                  std::string const & textMessage,
                  float32_t const x, float32_t const y, float32_t const z,
                  uint32_t const color )
{
    addTextBillboard( smgr, lineHeight, textMessage, x, y, z, color, color );
}

#endif // DE_HAMPE_SCENE_ADD_TEXT_BILLBOARD_HPP
