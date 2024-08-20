#include <iostream>
#include <clocale>
#include <memory>

#include "ivolume.hpp"

template <typename T>
std::string getVolume(T deviceOrSession)
{
    if(deviceOrSession->getMute())
        return "mute";

    auto vol = static_cast<int>(deviceOrSession->getVolume() * 100);
    auto str = std::to_string(vol);
    if (vol < 100) str.insert(0, 1, ' ');
    str.append("%");
    return str;

}

void logDevice(std::shared_ptr<VolumeControl::IDevice> device)
{
    std::cerr << "[" << getVolume(device) << "] " << device->getName() << std::endl;
    for (const auto& session : device->getSessions())
        std::cerr << "[" << getVolume(session) << "] " << session->getName() << std::endl;
    std::cerr << std::endl;
}

void logDeviceVerbose(std::shared_ptr<VolumeControl::IDevice> device)
{
    device->dumpInfo(std::cerr);
    for (const auto& session : device->getSessions())
        session->dumpInfo(std::cerr);
}

void logDevices(
    std::vector<std::shared_ptr<VolumeControl::IDevice>> devices,
    bool verbose
)
{
    for (const auto &device : devices)
    {
        if(verbose) logDeviceVerbose(device);
        else logDevice(device);
    }
}

int main(int argc, char const* argv[])
{
    auto verbose = false;
    for(auto i = 0; i < argc; ++i)
    {
        if(std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--verbose") == 0)
            verbose = true;
    }

    try
    {
        auto volume = VolumeControl::init();

        auto input = volume->getDevices(VolumeControl::Input);
        std::cerr << "INPUT DEVICES" << std::endl << std::endl;
        logDevices(input, verbose);

        auto output = volume->getDevices(VolumeControl::Output);
        std::cerr << "OUTPUT DEVICES" << std::endl << std::endl;
        logDevices(output, verbose);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
