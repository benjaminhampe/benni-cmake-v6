//////////////////////////////////////////////////////////////////////
/// @file PerfTimer.hpp
/// @author Benjamin Hampe <benjaminhampe@gmx.de>
/// @brief Definition of the class PerformanceTimer.
///
/// Copyright (c) 2018 by Benjamin Hampe <benjaminhampe@gmx.de>
//////////////////////////////////////////////////////////////////////

#ifndef IRREXT_PERFTIMER_HPP
#define IRREXT_PERFTIMER_HPP

#include <irrExt/Types.hpp>
#include <chrono>

namespace irrExt {

/// @brief The PerfTimer class
/**
 * @code
 *    // Create
 *    PerfTimer m_PerformanceTimer;
 *
 *    // Start
 *    m_PerformanceTimer.start();
 *
 *    // ... do stuff ...
 *
 *    // Stop
 *    m_PerformanceTimer.stop();
 *    log() << "Test needed " << m_PerformanceTimer.toString() << ".\n"
 *
 * @endcode
 */
class PerfTimer
{
public:
   PerfTimer() {
      m_T1 = std::chrono::steady_clock::now();
      m_T0 = m_T1;
   }

   void start() { m_T0 = std::chrono::steady_clock::now(); }
   void stop() { m_T1 = std::chrono::steady_clock::now(); }
   float64_t asSeconds() const { std::chrono::duration< float64_t > const secs = m_T1 - m_T0; return secs.count(); }
   float64_t asMilliseconds() const { return 1.e3 * asSeconds(); }
   float64_t asMicroseconds() const { return 1.e6 * asSeconds(); }
   float64_t asNanoseconds() const { return 1.e9 * asSeconds(); }

   std::string toString() const
   {
      std::stringstream s;
      float64_t ms = asMilliseconds();
      if ( ms >= 0.9 )
      {
         s << ms << " ms";
      }
      else
      {
         s << asNanoseconds() << " ns";
      }

      return s.str();
   }
private:
   std::chrono::time_point< std::chrono::steady_clock > m_T0;
   std::chrono::time_point< std::chrono::steady_clock > m_T1;
};

} // end namespace irrExt

#endif // IRREXT_PERFTIMER_HPP
