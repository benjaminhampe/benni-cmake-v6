/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_PLACEOBJECTTYPE_HPP
#define PIONEERS3D_TYPES_PLACEOBJECTTYPE_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

///@brief Differentiate between PLACE_OBJECT objects.
enum class ePlaceObjectType : unsigned char
{
    ROBBER = 0,
    ROAD,
    SETTLEMENT,
    CITY,
    COUNT
};

} // end namespace pioneers3d


inline std::ostream &
operator<< ( std::ostream & o, pioneers3d::ePlaceObjectType const pot )
{
    switch( pot )
    {
       case pioneers3d::ePlaceObjectType::ROBBER: o << "ROBBER"; break;
       case pioneers3d::ePlaceObjectType::ROAD: o << "ROAD"; break;
       case pioneers3d::ePlaceObjectType::SETTLEMENT: o << "SETTLEMENT"; break;
       case pioneers3d::ePlaceObjectType::CITY: o << "CITY"; break;
       default: o << "UNKNOWN"; break;
    }
    return o;
}

#endif // PIONEERS3D_TYPES_PLACEOBJECTTYPE_HPP
