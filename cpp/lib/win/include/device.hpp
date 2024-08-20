#pragma once

#include <string>
#include <vector>

#include <audiopolicy.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>

#include "idevice.hpp"
#include "session.hpp"

// clang-format off
// must come last
#include <functiondiscoverykeys.h>
// clang-format on

/// https://www.reddit.com/r/cpp_questions/comments/rzv10h/whats_the_most_elegant_way_of_handling/
// Option 1: Normal polymorphism with an abstract base class and concrete implementation derived classes with a factory method for creating the correct derived objects.

namespace VolumeControl
{
class Device : public IDevice
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
    ~Device();
    std::string getId() override;
    std::string getName() override
    {
        return this->readProperty(PKEY_Device_FriendlyName);
    }
    float getVolume() override;
    void setVolume(float volume) override;
    bool getMute() override;
    void setMute(bool mute) override;
    int getSessionCount() override;
    std::shared_ptr<ISession> getSession(int id) override;
    std::vector<std::shared_ptr<ISession>> getSessions() override;
    void dumpInfo(std::ostream& stream) override;
};
}
