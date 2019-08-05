// Copyright (C) 2002-2018 Benjamin Hampe

#ifndef IRREXT_CIRCLE_HPP
#define IRREXT_CIRCLE_HPP

#include <irrExt/AutoMeshBuffer.hpp>

namespace irrExt {

/// @brief Create a round-rect mesh-buffer with one material texture and multiple color vertices
AutoMeshBuffer*
    createCircle(
    glm::vec3 pos,
    glm::vec2 size,
    glm::vec2 radius,
    uint32_t tesselation,
    uint32_t color );

/// @brief Create a round-rect mesh-buffer with one material texture and multiple color vertices
irr::scene::SMeshBuffer*
    createCircleMeshBuffer(
    glm::vec2 size,
    glm::vec2 radius,
    uint32_t tesselation,
    bool bFullyTextured,
    irr::video::SColor const & outerColor,
    irr::video::SColor const & innerColor );

} // end namespace irrExt

#endif // IRREXT_CIRCLE_HPP

