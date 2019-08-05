//////////////////////////////////////////////////////////////////////
///
/// @file 	Logger.hpp
/// @brief 	Macros for static and normal class logging
/// @author Benjamin Hampe <benjaminhampe@gmx.de>
///
//////////////////////////////////////////////////////////////////////

#ifndef DE_HAMPE_LOGGING_LOGGER_HPP
#define DE_HAMPE_LOGGING_LOGGER_HPP

#include <de/hampe/Types.hpp>

// ----------------------------------------------------
// --- Logging Macros ---
// ----------------------------------------------------

#define ENABLE_DE_HAMPE_LOGGING

#ifdef ENABLE_DE_HAMPE_LOGGING

   #include <de/hampe/sprintf.hpp>
   #include <de/hampe/logging/LogLevel.hpp>
   #include <de/hampe/logging/StdOutLogger.hpp>

   #ifndef CREATE_STATIC_LOGGER
   #define CREATE_STATIC_LOGGER(loggerName) \
       static de::hampe::logging::StdOutLogger & \
       getLogger() \
       { \
           static de::hampe::logging::StdOutLogger s_Logger( (loggerName) ); \
           return s_Logger; \
       }
   #endif
   #ifndef CREATE_LOGGER
   #define CREATE_LOGGER(loggerName) \
         de::hampe::logging::StdOutLogger m_Logger = de::hampe::logging::StdOutLogger( (loggerName) );
   #endif
   #ifndef CREATE_FN_LOGGER
   #define CREATE_FN_LOGGER CREATE_LOGGER(__func__)
   #endif
   #ifndef STATIC_LOG_FN
   #define STATIC_LOG_FN getLogger() << de::hampe::logging::LogLevel::Info << __FUNCTION__;
   #endif
   #ifndef STATIC_LOG_TRACE
   #define STATIC_LOG_TRACE(...) getLogger() << de::hampe::logging::LogLevel::trace() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef STATIC_LOG_DEBUG
   #define STATIC_LOG_DEBUG(...) getLogger() << de::hampe::logging::LogLevel::debug() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef STATIC_LOG_INFO
   #define STATIC_LOG_INFO(...) getLogger() << de::hampe::logging::LogLevel::info() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef STATIC_LOG_WARN
   #define STATIC_LOG_WARN(...) getLogger() << de::hampe::logging::LogLevel::warn() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef STATIC_LOG_ERROR
   #define STATIC_LOG_ERROR(...) getLogger() << de::hampe::logging::LogLevel::error() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef STATIC_LOG_FATAL
   #define STATIC_LOG_FATAL(...) getLogger() << de::hampe::logging::LogLevel::fatal() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef STATIC_LOG_FLUSH
   #define STATIC_LOG_FLUSH getLogger() << std::endl;
   #endif
   #ifndef LOG_FLUSH
   #define LOG_FLUSH m_Logger << std::endl;
   #endif
   #ifndef LOG_FN
   #define LOG_FN m_Logger << de::hampe::logging::LogLevel::Info << __FUNCTION__;
   #endif
   #ifndef LOG_TRACE
   #define LOG_TRACE( ... ) m_Logger << de::hampe::logging::LogLevel::trace() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef LOG_DEBUG
   #define LOG_DEBUG( ... ) m_Logger << de::hampe::logging::LogLevel::debug() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef LOG_INFO
   #define LOG_INFO( ... ) m_Logger << de::hampe::logging::LogLevel::info() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef LOG_WARN
   #define LOG_WARN( ... ) m_Logger << de::hampe::logging::LogLevel::warn() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef LOG_ERROR
   #define LOG_ERROR( ... ) m_Logger << de::hampe::logging::LogLevel::error() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif
   #ifndef LOG_FATAL
   #define LOG_FATAL( ... ) m_Logger << de::hampe::logging::LogLevel::fatal() << de::hampe::sprintf( __FUNCTION__, __VA_ARGS__ );
   #endif

#else

   // All logging reduce to nothing ( no-op )

   #ifndef CREATE_STATIC_LOGGER
   #define CREATE_STATIC_LOGGER(loggerName)
   #endif
   #ifndef CREATE_LOGGER
   #define CREATE_LOGGER(loggerName)
   #endif
   #ifndef CREATE_FN_LOGGER
   #define CREATE_FN_LOGGER
   #endif
   #ifndef STATIC_LOG_FN
   #define STATIC_LOG_FN( ... )
   #endif
   #ifndef STATIC_LOG_TRACE
   #define STATIC_LOG_TRACE( ... )
   #endif
   #ifndef STATIC_LOG_DEBUG
   #define STATIC_LOG_DEBUG( ... )
   #endif
   #ifndef STATIC_LOG_INFO
   #define STATIC_LOG_INFO( ... )
   #endif
   #ifndef STATIC_LOG_WARN
   #define STATIC_LOG_WARN( ... )
   #endif
   #ifndef STATIC_LOG_ERROR
   #define STATIC_LOG_ERROR( ... )
   #endif
   #ifndef STATIC_LOG_FATAL
   #define STATIC_LOG_FATAL( ... )
   #endif
   #ifndef STATIC_LOG_FLUSH
   #define STATIC_LOG_FLUSH
   #endif
   #ifndef LOG_FLUSH
   #define LOG_FLUSH
   #endif
   #ifndef LOG_FN
   #define LOG_FN( ... )
   #endif
   #ifndef LOG_TRACE
   #define LOG_TRACE( ... )
   #endif
   #ifndef LOG_DEBUG
   #define LOG_DEBUG( ... )
   #endif
   #ifndef LOG_INFO
   #define LOG_INFO( ... )
   #endif
   #ifndef LOG_WARN
   #define LOG_WARN( ... )
   #endif
   #ifndef LOG_ERROR
   #define LOG_ERROR( ... )
   #endif
   #ifndef LOG_FATAL
   #define LOG_FATAL( ... )
   #endif

#endif

#endif // ENABLE_DE_HAMPE_LOGGING
