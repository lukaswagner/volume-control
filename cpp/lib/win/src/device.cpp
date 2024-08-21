#include "device.hpp"

#include <stdexcept>

#include "helper.hpp"

namespace VolumeControl
{
std::string Device::readProperty(REFPROPERTYKEY key)
{
    PROPVARIANT prop;
    PropVariantInit(&prop);
    auto result = m_deviceProperties->GetValue(key, &prop);
    if (result != S_OK) throw std::runtime_error("could not read property");
    auto ptr = prop.pwszVal;
    if (ptr == 0) throw std::runtime_error("could not read property as string");
    return toString(ptr);
}

Device::Device(IMMDevice* device)
    : m_device(device)
{
    auto result = CoCreateGuid(&m_guid);
    CHECK(result, "could not generate id");

    m_id = guidToString(m_guid);

    result = m_device->OpenPropertyStore(STGM_READ, &m_deviceProperties);
    CHECK(result, "could not acquire device properties");

    result = m_device->Activate(
        __uuidof(IAudioEndpointVolume),
        CLSCTX_ALL,
        NULL,
        (void**)&m_deviceVolume);
    CHECK(result, "could not acquire device volume");

    result = m_device->Activate(
        __uuidof(IAudioSessionManager2),
        CLSCTX_ALL,
        NULL,
        (void**)&m_sessionManager);
    CHECK(result, "could not acquire session manager");

    result = m_sessionManager->GetSessionEnumerator(&m_sessionList);
    CHECK(result, "could not acquire session list");
}

Device::~Device()
{
    SAFE_RELEASE(m_device);
    SAFE_RELEASE(m_deviceProperties);
    SAFE_RELEASE(m_deviceVolume);
    SAFE_RELEASE(m_sessionManager);
    SAFE_RELEASE(m_sessionList);
}

std::string Device::getId()
{
    return m_id;
}

float Device::getVolume()
{
    float volume;
    auto result = m_deviceVolume->GetMasterVolumeLevelScalar(&volume);
    CHECK(result, "could not read volume");
    return volume;
}

void Device::setVolume(float volume)
{
    auto result = m_deviceVolume->SetMasterVolumeLevelScalar(volume, NULL);
    CHECK(result, "could not write volume");
}

bool Device::getMute()
{
    BOOL mute;
    auto result = m_deviceVolume->GetMute(&mute);
    CHECK(result, "could not read mute");
    return mute != 0;
}

void Device::setMute(bool mute)
{
    auto result = m_deviceVolume->SetMute(mute, NULL);
    CHECK(result, "could not write mute");
}

int Device::getSessionCount()
{
    int count;
    auto result = m_sessionList->GetCount(&count);
    CHECK(result, "could not read count");
    return count;
}

std::shared_ptr<ISession> Device::getSession(int id)
{
    IAudioSessionControl* controls;
    auto result = m_sessionList->GetSession(id, &controls);
    CHECK(result, "could not acquire session");
    return std::make_shared<Session>(controls);
}

std::vector<std::shared_ptr<ISession>> Device::getSessions()
{
    std::vector<std::shared_ptr<ISession>> result;
    auto count = getSessionCount();
    for (int i = 0; i < count; ++i)
        result.push_back(getSession(i));
    return result;
}

void Device::dumpInfo(std::ostream& stream) {
    stream << "### device info ###" << std::endl;
    DUMP("getId") << getId() << std::endl;
    DUMP("getName") << getName() << std::endl;
    DUMP("getVolume") << getVolume() << std::endl;
    DUMP("getMute") << getMute() << std::endl;
    DUMP("getSessionCount") << getSessionCount() << std::endl;
    stream << std::endl;
}
}
