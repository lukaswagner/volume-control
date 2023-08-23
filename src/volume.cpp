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

Device Volume::getDefaultOutputDevice()
{
    IMMDevice* device;
    auto result =
        m_devices->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    CHECK(result, "could not acquire default device");
    return Device(device);
}

std::vector<Device> Volume::getAllDevices(DeviceType type)
{
    IMMDeviceCollection* devices;
    auto result = m_devices->EnumAudioEndpoints((EDataFlow)type, DEVICE_STATE_ACTIVE, &devices);
    CHECK(result, "could not enumerate devices");
    unsigned int count;
    result = devices->GetCount(&count);
    CHECK(result, "could not get number of devices");
    std::vector<Device> deviceList;
    for(auto i = 0u; i < count; ++i)
    {
        IMMDevice* device;
        result = devices->Item(i, &device);
        CHECK(result, "could not acquire device");
        deviceList.emplace_back(device);
    }
    return deviceList;
}
