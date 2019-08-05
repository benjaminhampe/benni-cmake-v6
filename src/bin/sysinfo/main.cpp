#include <de/hampe/os/SysInfo.hpp>
#include <iostream>

using de::hampe::os::RamInfo;
using de::hampe::os::SysInfo;

int main()
{
    std::cout << "Hello SysInfo!" << std::endl;

    RamInfo ramInfo;
    std::cout << ramInfo.toString() << std::endl;

    SysInfo sysInfo;
    std::cout << sysInfo.toString() << std::endl;

    return 0;
}
