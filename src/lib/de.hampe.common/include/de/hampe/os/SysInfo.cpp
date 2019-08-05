//////////////////////////////////////////////////////////////////////
/// @file SysInfo.cpp
/// @brief Implementation of class SysInfo
/// @author Benjamin Hampe <benjaminhampe@gmx.de>
//////////////////////////////////////////////////////////////////////

#include <de/hampe/os/SysInfo.hpp>

namespace de {
namespace hampe {
namespace os {

void
RamInfo::update()
{
// #ifdef MD_OS_WINDOWS
#if defined(_WINDOWS) || defined(_WIN32) || defined(WIN32)
    SYSTEM_INFO sysInfo;
    ::GetSystemInfo( &sysInfo );

    MEMORYSTATUSEX memInfo;
    //memset( &memInfo, 0, sizeof(memInfo) );
    ::GlobalMemoryStatusEx( &memInfo );
//        if ( ::GlobalMemoryStatusEx( &memInfo ) )
//        {
    m_RamTotal = memInfo.ullTotalPhys;
    m_RamFree = memInfo.ullAvailPhys;
    m_SwapTotal = memInfo.ullTotalPageFile;
    m_SwapFree = memInfo.ullAvailPageFile;
//        }
#endif
// #endif
}

} // end namespace os
} // end namespace hampe
} // end namespace de
