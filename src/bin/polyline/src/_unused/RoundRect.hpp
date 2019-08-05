// Copyright (C) 2002-2018 Benjamin Hampe

#ifndef DE_HAMPE_COMMON_MATH_CREATEROUNDRECT_HPP
#define DE_HAMPE_COMMON_MATH_CREATEROUNDRECT_HPP

#include <de/hampe/irrExt/AutoMeshBufferUtils.hpp>

#if 0

	/// @brief Create a round-rect mesh-buffer with one material texture and multiple color vertices
	AutoMeshBuffer*
		createRoundRect(
		glm::vec3 pos,
		glm::vec2 size,
		glm::vec2 radius,
		uint32_t tesselation,
		uint32_t color );

	/// @brief Create a round-rect mesh-buffer with one material texture and multiple color vertices
	irr::scene::SMeshBuffer*
		createRoundRectMeshBuffer(
		glm::vec2 size,
		glm::vec2 radius,
		uint32_t tesselation,
		bool bFullyTextured,
		irr::video::SColor const & outerColor,
		irr::video::SColor const & innerColor );

#endif
	
#endif // DE_HAMPE_COMMON_MATH_CREATEROUNDRECT_HPP

