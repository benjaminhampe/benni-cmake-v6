#ifndef IRREXT_STRINGUTILS_HPP
#define IRREXT_STRINGUTILS_HPP

#include <irrExt/Types.hpp>

namespace irrExt {

	int64_t 
	getPerfTimer();

	int64_t 
	getPerfTimerFrequency();

	uint32_t 
	replace( std::string & s, std::string const & from, std::string const & to );

	std::string 
	toString( std::vector< std::string > const & v, std::string const & prefix = "" );
	
#ifdef _MSC_VER
	void
	lowerCase( std::string & out );
	void
	upperCase( std::string & out );
#else
	void
	lowerCase( std::string & out, std::locale const & loc = std::locale() );
	void
	upperCase( std::string & out, std::locale const & loc = std::locale() );
#endif
	bool
	startsWith( std::string const & str, char c );
	
	bool
	endsWith( std::string const & str, char c );
	
	bool
	startsWith( std::string const & str, std::string const & query, bool ignoreCase = false );
	
	bool
	endsWith( std::string const & str, std::string const & query, bool ignoreCase = false );
	
	void
	trimLeft( std::string & str );
	
	void
	trimRight( std::string & str );
	
	void
	trim( std::string & str );
	
} // end namespace irrExt


#if 0

struct GregorianDate {
    int32_t year;
    int32_t month;
    int32_t day;
};

struct GregorianTime {
    int32_t hour;
    int32_t minute;
    int32_t second;
    int32_t microsecond;
};

struct JulianDays {
    float64_t jd;
};

std::string printDate();

std::string printSystemTime();

std::string printLocalTime();

std::string printTimer();

inline void
XML_writeHeader( std::stringstream & out )
{
    out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
}

inline std::string
XML_ATTRIBUTE( std::string key, std::string value )
{
    std::stringstream s;
    s << ' ' << key << '=' << '"' << value << '"';
    return s.str();
}

inline int
XML_findAttribute( irr::io::IXMLReaderUTF8 * xml, const char* attribName )
{
    int const count = xml->getAttributeCount();

    for ( int i = 0; i < count; ++i )
    {
        std::string const tmp = xml->getAttributeName( i );

        if ( tmp == attribName )
        {
            std::cout << __FUNCTION__ << "(" << attribName << ") :: Found attribute[" << i << "]\n";
            return i;
        }
    }

    std::cout << __FUNCTION__ << "(" << attribName << ") :: Cannot find attribute.\n";

    return -1;
}


inline bool
XML_hasAttribute( irr::io::IXMLReaderUTF8 * xml, const char* attribName )
{
    if ( XML_findAttribute( xml, attribName ) != -1 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif

#endif // IRREXT_STRINGUTILS_HPP
