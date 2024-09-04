#pragma once

#include <map>
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

namespace VolumeControl
{
using SessionPtr = std::shared_ptr<ISession>;

class Device : public IDevice
{
private:
    std::string m_id;
    IMMDevice* m_device;
    IPropertyStore* m_deviceProperties;
    IAudioEndpointVolume* m_deviceVolume;
    IAudioSessionManager2* m_sessionManager;
    IAudioSessionEnumerator* m_sessionList;
    std::map<std::string, SessionPtr> m_sessionMap;
    std::string readProperty(REFPROPERTYKEY key);

public:
    static std::string getId(IMMDevice* device);

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
    std::vector<std::string> getSessionIds() override;
    SessionPtr getSession(std::string id) override;

    std::vector<SessionPtr> getSessions() override;

    void dumpInfo(std::ostream& stream) override;
};
}
