#ifndef DE_HAMPE_STRINGUTIL_HPP
#define DE_HAMPE_STRINGUTIL_HPP

#include <de/hampe/Types.hpp>
#include <de/hampe/logging/Logger.hpp>
#include <string>

#ifdef QT
#include <QString>
#endif

BEGIN_NAMESPACE_DE_HAMPE_COMMON

class StringUtil
{
   CREATE_STATIC_LOGGER( "de.hampe.StringUtil" )
public:

   static std::string
   toLower( std::string const & out, std::locale const & loc = std::locale() );

   static std::string
   toUpper( std::string const & out, std::locale const & loc = std::locale() );

   static void
   makeLower( std::string & out
               #ifndef _MSC_VER
               , std::locale const & loc = std::locale()
               #endif
   );

   static void
   makeUpper( std::string & out
               #ifndef _MSC_VER
               , std::locale const & loc = std::locale()
               #endif
   );


};

END_NAMESPACE_DE_HAMPE_COMMON

#endif // DE_HAMPE_STRINGUTIL_HPP
