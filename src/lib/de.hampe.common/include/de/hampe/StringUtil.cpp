#include <de/hampe/StringUtil.hpp>

BEGIN_NAMESPACE_DE_HAMPE_COMMON

// static
std::string
StringUtil::toLower( std::string const & out, std::locale const & loc )
{
   std::string retVal = out;
   makeLower( retVal, loc );
   return retVal;
}

// static
std::string
StringUtil::toUpper( std::string const & out, std::locale const & loc )
{
   std::string retVal = out;
   makeUpper( retVal, loc );
   return retVal;
}

void
StringUtil::makeLower( std::string & out
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
StringUtil::makeUpper(  std::string & out
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

END_NAMESPACE_DE_HAMPE_COMMON
