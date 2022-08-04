#include <iostream>

#include "volume.hpp"

int main(int argc, char const* argv[])
{
    try
    {
        Volume volume;
        auto device = volume.getDevice();
        std::cerr << "id       : " << device.getId() << std::endl
                  << "name     : " << device.getName() << std::endl
                  << "volume   : " << device.getVolume() << std::endl
                  << "mute     : " << device.getMute() << std::endl
                  << "sessions : " << device.getSessionCount() << std::endl;
        // device.setVolume(0.2);
        // device.setMute(!device.getMute());
        auto sessions = device.getSessions();
        for (int i = 0; i < sessions.size(); ++i)
        {
            auto session = sessions[i];
            std::cerr << "name     : " << session.getName() << std::endl
                      << "volume   : " << session.getVolume() << std::endl
                      << "mute     : " << session.getMute() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::cerr << "done" << std::endl;
    return 0;
}
