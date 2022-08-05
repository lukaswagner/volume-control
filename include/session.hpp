#pragma once

#include <iostream>
#include <string>

#include <audiopolicy.h>

class Session
{
private:
    IAudioSessionControl* m_control;
    IAudioSessionControl2* m_control2;
    ISimpleAudioVolume* m_volume;

public:
    Session(IAudioSessionControl* control);
    bool isSystem() { return m_control2->IsSystemSoundsSession() == S_OK; };
    std::string getName();
    std::string getPath();
    float getVolume();
    void setVolume(float volume);
    bool getMute();
    void setMute(bool mute);
    void dumpInfo(std::ostream& stream);
};
