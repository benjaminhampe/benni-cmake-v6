#ifndef PLANCAL_HPP
#define PLANCAL_HPP

#include <irrExt/AutoMeshBufferUtils.hpp>

namespace irrExt {
	
void
addGround( AutoMeshBuffer & buffer, glm::vec3 const & pos, glm::vec3 const & size );

void
addWall( AutoMeshBuffer & buffer, glm::vec3 const & pos, glm::vec3 const & size );

void
addTestObject( irr::scene::ISceneManager* smgr );

} // end namespace irrExt

#endif // WOHNUNG_TYPES_HPP
