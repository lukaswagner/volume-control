#pragma once

#include <string>

#include <mmdeviceapi.h>

class Volume
{
private:
    IMMDeviceEnumerator* m_devices;
    IMMDevice* m_defaultDevice;
    std::string m_defaultDeviceId;

public:
    Volume();
    std::string getDefaultDeviceId() { return m_defaultDeviceId; }
};
