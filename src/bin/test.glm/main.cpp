#include <glm/glm.hpp>
#include <iostream>

// ------------------------------------------------------------
// --- Print GLM Types --- For my developer convenience
// ------------------------------------------------------------

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec1<T,P> const & v )
{
    out << v.x;
    return out;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec2<T,P> const & v )
{
    out << v.x << "," << v.y;
    return out;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec3<T,P> const & v )
{
    out << v.x << "," << v.y << "," << v.z;
    return out;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& out, glm::tvec4<T,P> const & v )
{
    out << v.x << "," << v.y << "," << v.z << "," << v.w;
    return out;
}

int main( int argc, char** argv )
{
	glm::vec3 a( 1, 2, 3);
	glm::vec3 b( 5, -6, 9);
	glm::vec3 c = glm::cross( a, b );
	
	std::cout << "a = " << a << "\n";
	std::cout << "b = " << b << "\n";
	std::cout << "c = a x b = " << c << "\n";
    return 0;
}
