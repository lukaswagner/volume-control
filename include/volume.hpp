#pragma once

#include <string>
#include <vector>

#include <audiopolicy.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>

#include "session.hpp"

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
    IAudioSessionManager2* m_sessionManager;
    IAudioSessionEnumerator* m_sessionList;
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
    bool getDeviceMute();
    void setDeviceMute(bool mute);
    int getSessionCount();
    Session getSession(int id);
    std::vector<Session> getSessions();
};
