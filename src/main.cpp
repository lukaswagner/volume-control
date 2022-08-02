#include <iostream>

#include "volume.hpp"

int main(int argc, char const* argv[])
{
    try
    {
        Volume volume;
        std::cerr << volume.getDeviceId() << std::endl
                  << volume.getDeviceName() << std::endl
                  << volume.getDeviceVolume() << std::endl;
        // volume.setDeviceVolume(0.2);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::cerr << "done" << std::endl;
    return 0;
}
