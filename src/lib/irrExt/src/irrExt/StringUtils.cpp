#include "StringUtils.hpp"

#if defined(_WINDOWS)
   #include <windows.h>
#endif

#include <time.h>

namespace irrExt {

int64_t
getPerfTimer()
{
#ifdef _WINDOWS
    LARGE_INTEGER li;
    QueryPerformanceCounter( &li );
    return static_cast< int64_t >( li.QuadPart );
#else
    return 0;
#endif
}

int64_t
getPerfTimerFrequency()
{
#ifdef _WINDOWS
    LARGE_INTEGER li;
    QueryPerformanceFrequency( &li );
    return static_cast< int64_t >( li.QuadPart );
#else
    return 0;
#endif
}

std::string 
toString( std::vector< std::string > const & v, std::string const & prefix )
{
	std::stringstream s;
	size_t const n = v.size();
	if ( !prefix.empty() ) s << prefix;
	s << ".size(" << n << ")\n";
	if ( !v.empty() ) 
	{
		for ( size_t i = 0; i < v.size(); ++i )
		{
			if ( !prefix.empty() ) s << prefix;
			s << "[" << i << "] ";
			s << v.at( i ) << "\n";
		}
	}
	return s.str();
}

void
lowerCase( std::string & out
            #ifndef _MSC_VER
            , std::locale const & loc
            #endif
            )
{
   for ( size_t i = 0; i < out.size(); ++i )
   {
      #ifdef _MSC_VER
      out[ i ] = static_cast< char >( ::tolower( out[ i ] ) );
      #else
      out[ i ] = static_cast< char >( std::tolower< char >( out[ i ], loc ) );
      #endif
   }
}

void
upperCase(  std::string & out
            #ifndef _MSC_VER
            , std::locale const & loc
            #endif
        )
{
   for ( size_t i = 0; i < out.size(); ++i )
   {
      #ifdef _MSC_VER
      out[ i ] = static_cast< char >( ::toupper( out[ i ] ) );
      #else
      out[ i ] = static_cast< char >( std::toupper< char >( out[i], loc ) );
      #endif
   }
}

uint32_t
replace( std::string & s, std::string const & from, std::string const & to )
{
    uint32_t retVal = 0;
    size_t pos = 0;

    while( ( pos = s.find( from, pos ) ) != std::string::npos )
    {
        s.replace( pos, from.size(), to );
        pos += to.size(); // Handles case where 'to' is a substring of 'from'
        ++retVal;
    }
    return retVal;
}

bool
startsWith( std::string const & str, char c )
{
   if ( str.empty() ) return false;
   return str[ 0 ] == c;
}

bool
endsWith( std::string const & str, char c )
{
   if ( str.empty() ) return false;
   return str[ str.size() - 1 ] == c;
}

bool
startsWith( std::string const & str, std::string const & query, bool ignoreCase )
{
   if ( str.empty() ) return false;
   if ( query.empty() ) return false;
   if ( query.size() == str.size() ) {
      return query == str;
   }
   else if ( query.size() < str.size() ) {
      return query == str.substr( 0, query.size() );
   }
   else {
      return false;
   }
}

bool
endsWith( std::string const & str, std::string const & query, bool ignoreCase )
{
   if ( str.empty() ) return false;
   if ( query.empty() ) return false;
   if ( query.size() == str.size() ) {
      return query == str;
   }
   // abcde(5), cde(3), pos(2=c)
   else if ( query.size() < str.size() ) {
      return query == str.substr( str.size() - query.size(), query.size() );
   }
   else {
      return false;
   }
}

void
trimLeft( std::string & str )
{

}

void
trimRight( std::string & str )
{

}

void
trim( std::string & str )
{
   // e.g. "0  3 5 7 9 11  15   "; 3 + ( 6 + 6 + 1 ) + 4; n = 20
   // e.g. "   Hello World !    "; 3 + ( 6 + 6 + 1 ) + 4; n = 20
   size_t posLeft = str.find_first_not_of( ' ' ); 	// e.g. posLeft  = pos['H'] = 3;
   size_t posRight = str.find_last_not_of( ' ' );	// e.g. posRight = pos['!'] = 15 = 20 - 1 - 4;
   if( posRight > posLeft )
   {
      str = str.substr( posLeft, ( posRight - posLeft + 1 ) ); // p = 3, n = 13 = 15 - 3 + 1;
   }
}

} // end namespace irrExt





#if 0


std::string printDate()
{
    tm * t = localtime( nullptr );
    std::stringstream s;
    s   << t->tm_year << "-" << t->tm_mon << "-" << t->tm_mday << " "
        << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec;
    return s.str();
}

std::string printSystemTime()
{
#ifdef _WINDOWS
    SYSTEMTIME t;
    GetSystemTime( &t );
    std::stringstream s;
    s   << t.wYear << "-" << t.wMonth << "-" << t.wDay << " "
        << t.wHour << ":" << t.wMinute << ":" << t.wSecond << "." << t.wMilliseconds;
    return s.str();
#else
    return "";
#endif
}

std::string printLocalTime()
{
#ifdef _WINDOWS
    SYSTEMTIME t;
    GetLocalTime( &t );
    std::stringstream s;
    s   << t.wYear << "-" << t.wMonth << "-" << t.wDay << " "
        << t.wHour << ":" << t.wMinute << ":" << t.wSecond << "." << t.wMilliseconds;
    return s.str();
#else
    return "";
#endif
}

std::string printTimer()
{
    std::stringstream s;
    s   << getPerfTimer() << ", f=" << getPerfTimerFrequency() << " Hz\n";
    return s.str();
}


#endif
