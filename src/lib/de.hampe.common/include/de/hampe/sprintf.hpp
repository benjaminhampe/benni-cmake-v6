/// @author Benjamin Hampe <benjamin.hampe@gmx.de>

#ifndef DE_HAMPE_COMMON_SPRINTF_HPP
#define DE_HAMPE_COMMON_SPRINTF_HPP

#include <de/hampe/Types.hpp>
#include <sstream>
#include <vector>

// ----------------------------------------------------
// --- sprintf/vsprintf ---
// ----------------------------------------------------

BEGIN_NAMESPACE_DE_HAMPE_COMMON

   /// @brief Magic I.
   template< typename ... T >
   std::string
   sprintf( T const & ... t )
   {
       std::stringstream ss;
       (void)std::initializer_list< int >{ ( ss << t, 0)... };
       return ss.str();
   }

   /// @brief Magic II.
   template< typename ... T >
   std::vector< std::string >
   svprintf( T const & ... t )
   {
       std::vector< std::string > retVal;
       std::stringstream ss;
       (void)std::initializer_list<int>{ (ss.str(""), ss<<t, retVal.push_back(ss.str()), 0)... };
       return retVal;
   }

END_NAMESPACE_DE_HAMPE_COMMON

#endif // DE_HAMPE_COMMON_SPRINTF_HPP
