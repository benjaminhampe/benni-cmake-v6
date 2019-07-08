//////////////////////////////////////////////////////////////////////
/// @file 	LogLevel.hpp
/// @brief 	Definition of Enum-Class LogLevel
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

#ifndef DE_HAMPE_COMMON_LOGGING_LOGLEVEL_HPP
#define DE_HAMPE_COMMON_LOGGING_LOGLEVEL_HPP

#include <de/hampe/Types.hpp>
#include <string>

namespace de { 
namespace hampe { 
namespace logging {

    class LogLevel
    {
    public:
        enum eLogLevel
        {
            None,
            Trace,
            Debug,
            Ok,
            Benni,
            Info,
            Warn,
            Error,
            Fatal,
            Count
        };

    private:
        eLogLevel m_Level = None;
    public:
        //inline LogLevel() : m_Level(None) {}
        inline LogLevel( eLogLevel logLevel ) : m_Level(logLevel) {}
        inline operator eLogLevel() const { return m_Level; }
        inline void setLevel( eLogLevel logLevel ) { m_Level = logLevel; }
        inline eLogLevel getLevel() const { return m_Level; }

        static LogLevel trace() { return LogLevel( Trace ); }
        static LogLevel debug() { return LogLevel( Debug ); }
        static LogLevel ok() { return LogLevel( Ok ); }
        static LogLevel benni() { return LogLevel( Benni ); }
        static LogLevel info() { return LogLevel( Info ); }
        static LogLevel warn() { return LogLevel( Warn ); }
        static LogLevel error() { return LogLevel( Error ); }
        static LogLevel fatal() { return LogLevel( Fatal ); }

        /// @brief Convert enum value to string.
        std::string toString() const
        {
            switch ( m_Level )
            {
                case None: return "None";
                case Trace: return "Trace";
                case Debug: return "Debug";
                case Ok: return "OK";
                case Benni: return "Benni";
                case Info: return "Info";
                case Warn: return "Warn";
                case Error: return "Error";
                case Fatal: return "Fatal";
                default: return "Unknown";
            }
        }

        /// @brief Convert enum value to string.
        uint32_t getColor() const
        {
            switch ( m_Level )
            {
                case Trace: return 0xFFFF80FF;
                case Debug: return 0xFFFF00FF;
                case Warn: return 0xFFFFFF00;
                case Error: return 0xFFFF0000;
                case Fatal: return 0xFFFF6020;
                case Info: return 0xFFFFFFFF;
                default: return 0xFFFFFFFF;
            }
        }
    };

} // end namespace logging
} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_LOGGING_LOGLEVEL_HPP
