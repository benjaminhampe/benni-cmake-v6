#include "SysInfo.hpp"
#include <iostream>

int main()
{
    std::cout << "Hello SysInfo!" << std::endl;

    RamInfo ramInfo;
    std::cout << ramInfo.toString() << std::endl;

    SysInfo sysInfo;
    std::cout << sysInfo.toString() << std::endl;

    return 0;
}
