#include <iostream>

#include "volume.hpp"

int main(int argc, char const* argv[])
{
    try
    {
        Volume volume;
        std::cerr << "id       : " << volume.getDeviceId() << std::endl
                  << "name     : " << volume.getDeviceName() << std::endl
                  << "volume   : " << volume.getDeviceVolume() << std::endl
                  << "mute     : " << volume.getDeviceMute() << std::endl
                  << "sessions : " << volume.getSessionCount() << std::endl;
        // volume.setDeviceVolume(0.2);
        // volume.setDeviceMute(!volume.getDeviceMute());
        auto sessions = volume.getSessions();
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
