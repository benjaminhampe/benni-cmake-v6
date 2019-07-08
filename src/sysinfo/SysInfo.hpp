//////////////////////////////////////////////////////////////////////
/// @file SysInfo.hpp
/// @brief Definition of class SysInfo
/// @author Benjamin Hampe <benjaminhampe@gmx.de>
//////////////////////////////////////////////////////////////////////

#ifndef DE_HAMPE_OS_SYSINFO_HPP
#define DE_HAMPE_OS_SYSINFO_HPP

#include <sstream>

#if defined(__WIN32) || defined(_WINDOWS_)
    #define MD_OS_WINDOWS
    #include <windows.h>
    #include <sysinfoapi.h>
    #include <shellapi.h>
    #include <io.h>
    //#include <windowsx.h>
    typedef float float32_t;
    typedef double float64_t;
#endif

#if defined(__linux__) || defined(__POSIX)
    #define MD_OS_POSIX
    #include <sys/sysinfo.h>
#endif

template < typename T >
T percent( T z, T n )
{
    static T const zero( 0 );

    if ( std::abs( n ) < zero )
    {
        return zero;
    }

    return T( 100 ) * z / n;
}

namespace de {
namespace hampe {
namespace os {

class RamInfo
{
    float64_t m_RamTotal = 0;
    float64_t m_RamFree = 0;
    float64_t m_SwapTotal = 0;
    float64_t m_SwapFree = 0;
public:
    RamInfo() { update(); }
    ~RamInfo() = default;

    void update();

    std::string
    toString() const
    {
        std::stringstream s;
        s << "RAM( total:" << m_RamTotal << ", free:" << m_RamFree << " " << percent( m_RamFree, m_RamTotal ) << " ), "
          << "Swap( total:" << m_SwapTotal << ", free:" << m_SwapFree << " " << percent( m_SwapFree, m_SwapTotal )<< " )";
        return s.str();
    }
};

class SysInfo
{
public:
    ~SysInfo() = default;
    SysInfo() { update(); }

    void updateHostName()
    {
        #ifdef MD_OS_WINDOWS
        char buffer[ 256 ];
        memset( buffer, 0, 256 );
        DWORD dummy = 0;
        ::GetComputerNameA( buffer, &dummy );
        m_HostName = buffer;
        m_HostNameLen = dummy;
        m_CurrentDir = getcwd( nullptr, 0 );
        #endif
    }

    void updateCPU()
    {
        #ifdef MD_OS_WINDOWS
        SYSTEM_INFO sysInfo;
        ::GetSystemInfo ( &sysInfo );
        m_CpuNumCores = sysInfo.dwNumberOfProcessors;
        m_CpuNumThreads = m_CpuNumCores;
        if ( sysInfo.dwActiveProcessorMask )
        {
            DWORD cpuMask = sysInfo.dwActiveProcessorMask;
            uint32_t numThreads = 0;
            size_t const maxIndex = sizeof( uint32_t ) << 3;
            for ( size_t i = 0; i < maxIndex; ++i )
            {
                numThreads += ( ( cpuMask & 0x01 ) == 0x01 ); // consume right most bit 0
                cpuMask >>= 1; // shift right until all bits are consumed
            }
            m_CpuNumThreads = numThreads;
        }
        #endif
    }

    void update()
    {
        updateHostName();
        updateCPU();
    }

    std::string
    toString() const
    {
        std::stringstream s;
        s << "[SysInfo] "
          // << "Hostname( "<< m_HostName << ":" << m_HostNameLen << "), "
          << "CPU( cores:" << m_CpuNumCores << ", threads:"<< m_CpuNumThreads << " ), "
          << "PWD( " << m_CurrentDir << ")"
          << "";
        return s.str();
    }

private:
    uint64_t m_CpuNumSockets = 1;
    uint64_t m_CpuNumCores = 2;
    uint64_t m_CpuNumThreads = 4;
    std::string m_CurrentDir;
    uint32_t m_HostNameLen = 0;
    std::string m_HostName;
};

} // end namespace os
} // end namespace hampe
} // end namespace de

inline std::ostream &
operator<< ( std::ostream & out, de::hampe::os::SysInfo const & sysInfo )
{
    out << sysInfo.toString();
    return out;
}

#endif // DE_HAMPE_OS_SYSINFO_HPP
