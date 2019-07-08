#include "StringUtils.hpp"

#if defined(_WINDOWS)
   #include <windows.h>
#endif

#include <time.h>

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
