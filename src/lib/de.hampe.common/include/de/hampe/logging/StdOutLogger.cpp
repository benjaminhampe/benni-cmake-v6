//////////////////////////////////////////////////////////////////////
/// @file 	StdOutLogger.cpp
/// @brief 	Implementation of class Logger
/// @author Copyright (c) Benjamin Hampe <benjaminhampe@gmx.de>
///
///	The author grants permission of free use, free distribution and
/// free usage for commercial applications even for modified sources.
///
///	What is not allowed is to remove the original authors name or claim
/// any intellectual rights. This original code was written in my spare time
/// and is distributed publicly as free software to anyone over GitLab and GitHub.
///
//////////////////////////////////////////////////////////////////////

#include "StdOutLogger.hpp"

namespace de { 
namespace hampe { 
namespace logging {

StdOutLogger::~StdOutLogger()
{
   flush();
}

StdOutLogger::StdOutLogger()
   : m_Name( "StdOutLogger" )
{}

StdOutLogger::StdOutLogger( std::string const & name )
   : m_Name( std::move( name ) )
{}

StdOutLogger::StdOutLogger( StdOutLogger const & other )
   : m_Name( std::move( other.m_Name ) )
{}

void
StdOutLogger::flush()
{
   std::cout << std::flush;
}

std::string const &
StdOutLogger::getName() const
{
   return m_Name;
}

void
StdOutLogger::setName( std::string const & name )
{
   m_Name = std::move( name );
}

StdOutLogger&
StdOutLogger::operator<< ( LogLevel const & logLevel )
{
   std::cout << "[" << logLevel.toString() << "] " << getName() << ".";
   return *this;
}

StdOutLogger&
StdOutLogger::operator<< ( std::ostream& (*func)( std::ostream& ) )
{
   func( std::cout );
   return *this;
}

} // end namespace logging
} // end namespace hampe
} // end namespace de
