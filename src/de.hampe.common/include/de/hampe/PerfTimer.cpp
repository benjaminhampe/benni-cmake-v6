//////////////////////////////////////////////////////////////////////
/// @file PerfTimer.cpp
/// @author Benjamin Hampe <benjaminhampe@gmx.de>
/// @brief Implementation of the class PerformanceTimer.
///
/// Copyright (c) 2018 by Benjamin Hampe <benjaminhampe@gmx.de>
//////////////////////////////////////////////////////////////////////

#include "PerfTimer.hpp"

#include <assert.h>

namespace de {
namespace hampe {
namespace perf {

#if 0
PerfTimer::PerfTimer()
{
   m_CurrTime = std::chrono::steady_clock::now();
   m_LastTime = m_CurrTime;
}

PerfTimer::~PerfTimer()
{

}

void
PerfTimer::start()
{
   m_LastTime = std::chrono::steady_clock::now();
}

void
PerfTimer::stop()
{
   m_CurrTime = std::chrono::steady_clock::now();
}

float64_t
PerfTimer::asNanoseconds() const
{
   std::chrono::duration<std::chrono::nanoseconds> diff = m_CurrTime - m_LastTime;
   return diff.count();
}

float64_t
PerfTimer::getTimeAsMilliseconds() const
{
   std::chrono::duration< float64_t > diff = m_CurrTime - m_LastTime;
   return 1000.0 * diff.count();
}

std::string
PerfTimer::toString() const
{
   std::stringstream s;
   s << getTimeAsMilliseconds() << " ms";
   return s.str();
}

#endif
} // end namespace perf
} // end namespace hampe
} // end namespace de
