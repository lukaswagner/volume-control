#include "volume.hpp"

#include <stdexcept>

#include "helper.hpp"

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
}

Device Volume::getDevice()
{
    IMMDevice* device;
    auto result =
        m_devices->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    CHECK(result, "could not acquire default device");
    return Device(device);
}
