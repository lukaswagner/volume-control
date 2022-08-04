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

class Device
{
private:
    IMMDevice* m_device;
    IPropertyStore* m_deviceProperties;
    IAudioEndpointVolume* m_deviceVolume;
    IAudioSessionManager2* m_sessionManager;
    IAudioSessionEnumerator* m_sessionList;
    std::string readProperty(REFPROPERTYKEY key);

public:
    Device(IMMDevice* device);
    std::string getId();
    std::string getName()
    {
        return this->readProperty(PKEY_Device_FriendlyName);
    }
    float getVolume();
    void setVolume(float volume);
    bool getMute();
    void setMute(bool mute);
    int getSessionCount();
    Session getSession(int id);
    std::vector<Session> getSessions();
};
