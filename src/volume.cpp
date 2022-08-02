#include "volume.hpp"

#include <iostream>
#include <stdexcept>

#include "helper.hpp"

Volume::Volume()
{
    auto result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (result != S_OK && result != S_FALSE)
    {
        throw std::runtime_error("could not initialize COM");
    }

    const auto clsid = __uuidof(MMDeviceEnumerator);
    const auto iid = __uuidof(IMMDeviceEnumerator);
    result = CoCreateInstance(clsid, NULL, CLSCTX_ALL, iid, (void**)&m_devices);
    if (result != S_OK)
    {
        throw std::runtime_error("could not acquire device enumerator");
    }

    result = m_devices->GetDefaultAudioEndpoint(
        eRender, eMultimedia, &m_defaultDevice);
    if (result != S_OK)
        throw std::runtime_error("could not acquire default device");

    LPWSTR* id;
    result = m_defaultDevice->GetId(id);
    if (result != S_OK)
        throw std::runtime_error("could not acquire default device id");
    m_defaultDeviceId = toString(*id);
}
