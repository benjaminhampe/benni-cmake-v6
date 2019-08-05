/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_BANK_HPP
#define PIONEERS3D_TYPES_BANK_HPP

#include <pioneers3d/types/Common.hpp>
#include <pioneers3d/types/TileType.hpp>

namespace pioneers3d {

class Bank_t
{
public:
    void clear()
    {
        Holz = Lehm = Weizen = Wolle = Erz = 0;
    }

    int32_t getTotal() const
    {
        return Holz + Lehm + Weizen + Wolle + Erz;
    }

    bool canRobberSteal() const
    {
        return getTotal() >= 7;
    }

    std::string toString() const
    {
        std::stringstream s;
        s << "Bank: "<< Name << "; "
        << "Holz:" << Holz << "; "
        << "Lehm:" << Lehm << "; "
        << "Weizen:" << Weizen << "; "
        << "Wolle:" << Wolle << "; "
        << "Erz:" << Erz << "; "
        << ";";
        return s.str();
    }

    int32_t get( TileType type ) const
    {
        int32_t n = 0;
        if ( type.contains( TileType::HOLZ ) ) n += Holz;
        if ( type.contains( TileType::LEHM ) ) n += Lehm;
        if ( type.contains( TileType::WEIZEN ) ) n += Weizen;
        if ( type.contains( TileType::WOLLE ) ) n += Wolle;
        if ( type.contains( TileType::ERZ ) ) n += Erz;
        return n;
    }

    void set( TileType type, int32_t value )
    {
        if ( type.contains( TileType::HOLZ ) ) Holz = glm::clamp( value, 0, 16000 );
        if ( type.contains( TileType::LEHM ) ) Lehm = glm::clamp( value, 0, 16000 );
        if ( type.contains( TileType::WEIZEN ) ) Weizen = glm::clamp( value, 0, 16000 );
        if ( type.contains( TileType::WOLLE ) ) Wolle = glm::clamp( value, 0, 16000 );
        if ( type.contains( TileType::ERZ ) ) Erz = glm::clamp( value, 0, 16000 );
    }

    void add( TileType type, int32_t value )
    {
        if ( type.contains( TileType::HOLZ ) ) Holz = glm::clamp( int32_t( Holz ) + value, 0, 16000 );
        if ( type.contains( TileType::LEHM ) ) Lehm = glm::clamp( int32_t( Lehm ) + value, 0, 16000 );
        if ( type.contains( TileType::WEIZEN ) ) Weizen = glm::clamp( int32_t( Weizen ) + value, 0, 16000 );
        if ( type.contains( TileType::WOLLE ) ) Wolle = glm::clamp( int32_t( Wolle ) + value, 0, 16000 );
        if ( type.contains( TileType::ERZ ) ) Erz = glm::clamp( int32_t( Erz ) + value, 0, 16000 );
    }

public:
    int32_t Holz = 0;           // Anzahl Holz
    int32_t Lehm = 0;           // Anzahl Lehm
    int32_t Weizen = 0;         // Anzahl Weizen
    int32_t Wolle = 0;          // Anzahl Wolle
    int32_t Erz = 0;            // Anzahl Erz
    std::string Name = "Bank";
};

} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_BANK_HPP
