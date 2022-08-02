#pragma once

#include <string>

#include <endpointvolume.h>
#include <mmdeviceapi.h>

// clang-format off
// must come last
#include <functiondiscoverykeys.h>
// clang-format on

class Volume
{
private:
    IMMDeviceEnumerator* m_devices;
    IMMDevice* m_device;
    IPropertyStore* m_deviceProperties;
    IAudioEndpointVolume* m_deviceVolume;
    std::string readProperty(REFPROPERTYKEY key);

public:
    Volume();
    std::string getDeviceId();
    std::string getDeviceName()
    {
        return this->readProperty(PKEY_Device_FriendlyName);
    }
    float getDeviceVolume();
    void setDeviceVolume(float volume);
};
