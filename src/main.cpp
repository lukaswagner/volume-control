#include <iostream>

#include "volume.hpp"

int main(int argc, char const* argv[])
{
    std::cerr << "hi" << std::endl;
    try
    {
        Volume volume;
        std::cerr << volume.getDefaultDeviceId() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::cerr << "bye" << std::endl;
    return 0;
}
