#include "volume.hpp"

#include <stdexcept>

#include "helper.hpp"

namespace VolumeControl
{
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

std::shared_ptr<IDevice> Volume::getDefaultOutputDevice()
{
    IMMDevice* device;
    auto result =
        m_devices->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    CHECK(result, "could not acquire default device");
    return std::make_shared<Device>(device);
}

EDataFlow DeviceTypeMap[3] = {
    EDataFlow::eRender,
    EDataFlow::eCapture,
    EDataFlow::eAll,
};

std::vector<std::shared_ptr<IDevice>> Volume::getAllDevices(DeviceType type)
{
    IMMDeviceCollection* devices;
    auto result = m_devices->EnumAudioEndpoints(DeviceTypeMap[type], DEVICE_STATE_ACTIVE, &devices);
    CHECK(result, "could not enumerate devices");
    unsigned int count;
    result = devices->GetCount(&count);
    CHECK(result, "could not get number of devices");
    std::vector<std::shared_ptr<IDevice>> deviceList;
    for(auto i = 0u; i < count; ++i)
    {
        IMMDevice* device;
        result = devices->Item(i, &device);
        CHECK(result, "could not acquire device");
        deviceList.push_back(std::make_shared<Device>(device));
    }
    return deviceList;
}

std::shared_ptr<IVolumeControl> init() {
    return std::make_shared<Volume>();
}
}
