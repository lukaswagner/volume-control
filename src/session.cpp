#include "session.hpp"

#include <iostream>
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
    // check for system session
    if (isSystem()) return "System";

    // try reading session identifier (for applications)
    LPWSTR sessionIdentifier;
    std::string name = "";
    auto result = m_control2->GetSessionIdentifier(&sessionIdentifier);
    CHECK(result, "could not read session identifier");
    auto idStr = toString(sessionIdentifier);
    std::regex exe(R"(\\(\w+)\.)");
    std::smatch match;
    if (std::regex_search(idStr, match, exe)) name = match[1];

    return name;
}

std::string Session::getPath()
{
    // system session: stored in display name
    if (isSystem())
    {
        LPWSTR displayName;
        auto result = m_control->GetDisplayName(&displayName);
        CHECK(result, "could not read name");
        auto path = toString(displayName);
        return ::getPath(path);
    }

    // try reading session identifier (for applications)
    LPWSTR sessionIdentifier;
    CHECK(
        m_control2->GetSessionIdentifier(&sessionIdentifier),
        "could not read session identifier");
    auto idStr = toString(sessionIdentifier);
    return ::getPath(idStr);
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

void Session::dumpInfo(std::ostream& stream)
{
    LPWSTR p;

    stream << "### session info ###" << std::endl;

    CHECK(m_control->GetDisplayName(&p), "GetDisplayName failed");
    DUMP("display name") << toString(p) << std::endl;

    CHECK(m_control->GetIconPath(&p), "GetIconPath failed");
    DUMP("icon path") << toString(p) << std::endl;

    DUMP("is system") << (isSystem() ? "true" : "false") << std::endl;

    if (!isSystem())
    {
        DWORD w;
        CHECK(m_control2->GetProcessId(&w), "GetProcessId failed");
        DUMP("process id") << w << std::endl;
    }

    CHECK(m_control2->GetSessionIdentifier(&p), "GetSessionIdentifier failed");
    DUMP("session id") << toString(p) << std::endl;

    DUMP("getName") << getName() << std::endl;
    DUMP("getPath") << getPath() << std::endl;
    DUMP("getVolume") << getVolume() << std::endl;
    DUMP("getMute") << (getMute() ? "true" : "false") << std::endl;
}
