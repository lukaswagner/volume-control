#include <iostream>

#include "helper.hpp"
#include "volume.hpp"

int main(int argc, char const* argv[])
{
    // required to fix wide character conversion
    setlocale(LC_ALL, ".OCP");

    auto verbose = false;
    for(auto i = 0; i < argc; ++i)
    {
        if(std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--verbose") == 0)
            verbose = true;
    }

    try
    {
        Volume volume;
        auto devices = volume.getAllDevices(Output);
        DUMPTO(std::cerr, "num of devices") << devices.size() << std::endl;
        for (auto &&device : devices)
        {
            if(verbose) device.dumpInfo(std::cerr);
            else std::cerr << device.getName() << std::endl;
            auto sessions = device.getSessions();
            for (int i = 0; i < sessions.size(); ++i)
            {
                auto session = sessions[i];
                if(verbose) session.dumpInfo(std::cerr);
                else std::cerr << session.getName() << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::cerr << "done" << std::endl;
    return 0;
}
