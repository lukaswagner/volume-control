#include "volume.hpp"

#include <iostream>
#include <stdexcept>

#include "helper.hpp"

#define CHECK(status, message)                                                 \
    if (status != S_OK) throw std::runtime_error(message);

std::string Volume::readProperty(REFPROPERTYKEY key)
{
    PROPVARIANT prop;
    PropVariantInit(&prop);
    auto result = m_deviceProperties->GetValue(key, &prop);
    if (result != S_OK) throw std::runtime_error("could not read property");
    auto ptr = prop.pwszVal;
    if (ptr == 0) throw std::runtime_error("could not read property as string");
    return toString(ptr);
}

Volume::Volume()
{
    auto result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (result != S_OK && result != S_FALSE)
        throw std::runtime_error("could not initialize COM");

    result = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        NULL,
        CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        (void**)&m_devices);
    CHECK(result, "could not acquire device enumerator");

    result =
        m_devices->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_device);
    CHECK(result, "could not acquire default device")

    result = m_device->OpenPropertyStore(STGM_READ, &m_deviceProperties);
    CHECK(result, "could not acquire device properties");

    result = m_device->Activate(
        __uuidof(IAudioEndpointVolume),
        CLSCTX_ALL,
        NULL,
        (void**)&m_deviceVolume);
    CHECK(result, "could not acquire device volume");
}

std::string Volume::getDeviceId()
{
    LPWSTR* id;
    auto result = m_device->GetId(id);
    CHECK(result, "could not acquire device id");
    return toString(*id);
}

float Volume::getDeviceVolume()
{
    float volume;
    auto result = m_deviceVolume->GetMasterVolumeLevelScalar(&volume);
    CHECK(result, "could not read volume");
    return volume;
}

void Volume::setDeviceVolume(float volume)
{
    auto result = m_deviceVolume->SetMasterVolumeLevelScalar(volume, NULL);
    CHECK(result, "could not write volume");
}
