#pragma once

#include <iostream>
#include <string>

#include <audiopolicy.h>

#include "isession.hpp"

namespace VolumeControl
{
class Session : public ISession
{
private:
    IAudioSessionControl* m_control;
    IAudioSessionControl2* m_control2;
    ISimpleAudioVolume* m_volume;

public:
    Session(IAudioSessionControl* control);
    ~Session();
    bool isSystem() override { return m_control2->IsSystemSoundsSession() == S_OK; };
    std::string getName() override;
    std::string getPath() override;
    float getVolume() override;
    void setVolume(float volume) override;
    bool getMute() override;
    void setMute(bool mute) override;
    void dumpInfo(std::ostream& stream) override;
};
}
