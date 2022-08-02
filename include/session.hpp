#pragma once

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
    std::string getName();
    float getVolume();
    void setVolume(float volume);
    bool getMute();
    void setMute(bool mute);
};
