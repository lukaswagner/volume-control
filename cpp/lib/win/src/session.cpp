#include "session.hpp"

#include <iostream>
#include <regex>

#include "helper.hpp"
#include "mainWindow.hpp"

namespace VolumeControl
{
std::map<std::string, std::string> Session::s_idMap;

std::string Session::getId(IAudioSessionControl* control)
{
    IAudioSessionControl2* control2;
    control->QueryInterface(IID_PPV_ARGS(&control2));

    LPWSTR idPtr;
    auto result = control2->GetSessionInstanceIdentifier(&idPtr);
    CHECK(result, "could not acquire session id");
    SAFE_RELEASE(control2);
    auto winId = toString(idPtr);

    std::string id;
    auto idIter = s_idMap.find(winId);
    if(idIter != s_idMap.end())
        id = idIter->second;
    else
    {
        id = createId();
        s_idMap[winId] = id;
    }

    return id;
}


Session::Session(IAudioSessionControl* control)
    : m_control(control)
{
    m_id = getId(control);

    m_control->QueryInterface(IID_PPV_ARGS(&m_volume));
    m_control->QueryInterface(IID_PPV_ARGS(&m_control2));
}

Session::~Session()
{
    SAFE_RELEASE(m_control);
    SAFE_RELEASE(m_control2);
    SAFE_RELEASE(m_volume);
}

std::string Session::getId()
{
    return m_id;
}

std::string Session::getName()
{
    // check for system session
    if (isSystem()) return "System";

    // try getting the session name
    LPWSTR displayNameTemp;
    CHECK(m_control->GetDisplayName(&displayNameTemp), "could not read display name");
    auto displayName = toString(displayNameTemp);
    if(displayName.length() > 0) return displayName;

    // fallback based on https://stackoverflow.com/a/27088482
    DWORD procId;
    CHECK(m_control2->GetProcessId(&procId), "GetProcessId failed");
    auto mainWindow = FindMainWindow(procId);
    const auto length = 256u;
    wchar_t title[length];
    auto read = GetWindowTextW(mainWindow, title, length);
    if(read > 0)
    {
        return toString(title, false);
    }

    // another fallback based on session identifier (for applications)
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
        return VolumeControl::getPath(path);
    }

    // try reading session identifier (for applications)
    LPWSTR sessionIdentifier;
    CHECK(
        m_control2->GetSessionIdentifier(&sessionIdentifier),
        "could not read session identifier");
    auto idStr = toString(sessionIdentifier);
    return VolumeControl::getPath(idStr);
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

    DUMP("session id") << getId() << std::endl;

    DUMP("getName") << getName() << std::endl;
    DUMP("getPath") << getPath() << std::endl;
    DUMP("getVolume") << getVolume() << std::endl;
    DUMP("getMute") << (getMute() ? "true" : "false") << std::endl;
    stream << std::endl;
}
}
