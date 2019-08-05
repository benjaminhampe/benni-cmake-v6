/// (c) 2017 - 20190 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef PIONEERS3D_TYPES_TILETYPE_HPP
#define PIONEERS3D_TYPES_TILETYPE_HPP

#include <pioneers3d/types/Common.hpp>

namespace pioneers3d {

class TileType
{
public:
    enum eType : uint32_t
    {
        UNKNOWN = 0,
        WASSER = 1,  // TileType
        LAND = 1<<1, // TileType
        //NONE = 1<<2, // RessourceType
        HAFEN = 1<<3, // RessourceType
        HOLZ = 1<<4, // RessourceType
        LEHM = 1<<5, // RessourceType
        WEIZEN = 1<<6, // RessourceType
        WOLLE = 1<<7, // RessourceType
        ERZ = 1<<8, // RessourceType
        RESSOURCE_MASK = HOLZ | LEHM | WEIZEN | WOLLE | ERZ,
        RESSOURCE_COUNT = 5,
        LAND_WUESTE = LAND,
        LAND_HOLZ = LAND | HOLZ,
        LAND_LEHM = LAND | LEHM,
        LAND_WEIZEN = LAND | WEIZEN,
        LAND_WOLLE = LAND | WOLLE,
        LAND_ERZ = LAND | ERZ,
        HAFEN_3zu1 = HAFEN,
        HAFEN_HOLZ = HAFEN | HOLZ,
        HAFEN_LEHM = HAFEN | LEHM,
        HAFEN_WEIZEN = HAFEN | WEIZEN,
        HAFEN_WOLLE = HAFEN | WOLLE,
        HAFEN_ERZ = HAFEN | ERZ,
        FORCE32BIT = 0x7FFFFFFF
    };
    static TileType holz() { return TileType::HOLZ; }
    static TileType lehm() { return TileType::LEHM; }
    static TileType weizen() { return TileType::WEIZEN; }
    static TileType wolle() { return TileType::WOLLE; }
    static TileType erz() { return TileType::ERZ; }

    TileType() : m_TileType( UNKNOWN ) {}
    TileType( eType type ) : m_TileType( type ) {}
    TileType( uint32_t type ) : m_TileType( type ) {}
    bool isUnknown() const { return m_TileType == UNKNOWN; }
    bool isWasser() const { return contains( WASSER ); }
    bool isLand() const { return contains( LAND ); }
    bool isRessource() const { return contains( RESSOURCE_MASK ); }
    bool isHafen() const { return contains( HAFEN ); }

    bool contains( uint32_t flags ) const
    {
        return (m_TileType & flags) != 0;
    }

    void set( uint32_t flags ) { m_TileType = flags; }
    uint32_t get() const { return m_TileType; }
    operator uint32_t () const { return m_TileType; }

//    bool isRessource( TileType tileType ) const
//    {
//        return ((m_TileType & RESSOURCE_MASK) & tileType) != 0;
//    }

    uint32_t getRessourceColor() const
    {
             if ( contains( HOLZ ) ) return 0xFF00C000;
        else if ( contains( LEHM ) ) return 0xFF0000FF;
        else if ( contains( WEIZEN ) ) return 0xFFFFFF00;
        else if ( contains( WOLLE ) ) return 0xFF60FF60;
        else if ( contains( ERZ ) ) return 0xFFFF0000;
        else return 0xFFFF00FF;
    }

    std::string getRessourceString() const
    {
             if ( contains( HOLZ ) ) return "holz";
        else if ( contains( LEHM ) ) return "lehm";
        else if ( contains( WEIZEN ) ) return "weizen";
        else if ( contains( WOLLE ) ) return "wolle";
        else if ( contains( ERZ ) ) return "erz";
        else return "";
    }

    std::string
    toString() const
    {
        if ( m_TileType == UNKNOWN ) return "UNKNOWN";
        if ( m_TileType == WASSER ) return "WASSER";

        std::stringstream s;

        if ( isLand() )
        {
            s << "LAND";
                 if ( contains( HOLZ ) ) s << "_HOLZ";
            else if ( contains( LEHM ) ) s << "_LEHM";
            else if ( contains( WEIZEN ) ) s << "_WEIZEN";
            else if ( contains( WOLLE ) ) s << "_WOLLE";
            else if ( contains( ERZ ) ) s << "_ERZ";
            else s << "_WUESTE";
        }
        else if ( isHafen() )
        {
            s << "HAFEN";
                 if ( contains( HOLZ ) ) s << "_HOLZ";
            else if ( contains( LEHM ) ) s << "_LEHM";
            else if ( contains( WEIZEN ) ) s << "_WEIZEN";
            else if ( contains( WOLLE ) ) s << "_WOLLE";
            else if ( contains( ERZ ) ) s << "_ERZ";
            else s << "_3zu1";
        }

        return s.str();
    }

    static TileType
    fromString( std::string const & txt )
    {
        uint32_t tileType = TileType::UNKNOWN;
             if ( txt == "WASSER" )        { tileType = TileType::WASSER; }
        else if ( txt == "LAND" )          { tileType = TileType::LAND; }
        else if ( txt == "LAND_HOLZ" )     { tileType = TileType::LAND_HOLZ; }
        else if ( txt == "LAND_LEHM" )     { tileType = TileType::LAND_LEHM; }
        else if ( txt == "LAND_WEIZEN" )   { tileType = TileType::LAND_WEIZEN; }
        else if ( txt == "LAND_ERZ" )      { tileType = TileType::LAND_ERZ; }
        else if ( txt == "LAND_WOLLE")     { tileType = TileType::LAND_WOLLE; }
        else if ( txt == "HAFEN_3zu1")     { tileType = TileType::HAFEN_3zu1; }
        else if ( txt == "HAFEN_HOLZ")     { tileType = TileType::HAFEN_HOLZ; }
        else if ( txt == "HAFEN_LEHM")     { tileType = TileType::HAFEN_LEHM; }
        else if ( txt == "HAFEN_WEIZEN")   { tileType = TileType::HAFEN_WEIZEN; }
        else if ( txt == "HAFEN_WOLLE")    { tileType = TileType::HAFEN_WOLLE; }
        else if ( txt == "HAFEN_ERZ")      { tileType = TileType::HAFEN_ERZ; }

        return tileType;
    }

    uint32_t m_TileType;
};

} // end namespace pioneers3d

#endif // PIONEERS3D_TYPES_TILETYPE_HPP
