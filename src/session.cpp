#include "session.hpp"

#include <regex>

#include "helper.hpp"

Session::Session(IAudioSessionControl* control)
    : m_control(control)
{
    m_control->QueryInterface(IID_PPV_ARGS(&m_volume));
    m_control->QueryInterface(IID_PPV_ARGS(&m_control2));
}

std::string Session::getName()
{
    // try reading display name
    LPWSTR displayName;
    auto result = m_control->GetDisplayName(&displayName);
    CHECK(result, "could not read name");
    auto name = toString(displayName);
    if (name.length() > 0) return name;

    LPWSTR sessionIdentifier;
    result = m_control2->GetSessionIdentifier(&sessionIdentifier);
    CHECK(result, "could not read session identifier");
    auto idStr = toString(sessionIdentifier);
    std::regex exe(R"(\\(\w+)\.)");
    std::smatch match;
    if (std::regex_search(idStr, match, exe)) name = match[1];

    return name;
}

float Session::getVolume()
{
    float volume;
    auto result = m_volume->GetMasterVolume(&volume);
    CHECK(result, "could not read volume");
    return volume;
}

void Session::setVolume(float volume)
{
    auto result = m_volume->SetMasterVolume(volume, NULL);
    CHECK(result, "could not write volume");
}

bool Session::getMute()
{
    BOOL mute;
    auto result = m_volume->GetMute(&mute);
    CHECK(result, "could not read mute");
    return mute != 0;
}

void Session::setMute(bool mute)
{
    auto result = m_volume->SetMute(mute, NULL);
    CHECK(result, "could not write mute");
}
