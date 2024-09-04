#include "volume.hpp"

#include <stdexcept>
#include <unordered_map>

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

Volume::~Volume()
{
    SAFE_RELEASE(m_devices);
}

std::string Volume::getDefaultOutputDeviceId()
{
    IMMDevice* device;
    auto result =
        m_devices->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    CHECK(result, "could not acquire default device");

    auto id = Device::getId(device);
    if(m_deviceMap.find(id) == m_deviceMap.end())
        m_deviceMap[id] = std::make_shared<Device>(device);
    else
        SAFE_RELEASE(device);

    return id;
}

std::unordered_map<DeviceType, EDataFlow> DeviceTypeMap = {
    {Output, EDataFlow::eRender},
    {Input, EDataFlow::eCapture},
    {Both, EDataFlow::eAll},
};

std::vector<std::string> Volume::getDeviceIds(DeviceType type)
{
    IMMDeviceCollection* devices;
    auto result = m_devices->EnumAudioEndpoints(DeviceTypeMap[type], DEVICE_STATE_ACTIVE, &devices);
    CHECK(result, "could not enumerate devices");
    unsigned int count;
    result = devices->GetCount(&count);
    CHECK(result, "could not get number of devices");
    std::vector<std::string> deviceList;
    for(auto i = 0u; i < count; ++i)
    {
        IMMDevice* device;
        result = devices->Item(i, &device);
        CHECK(result, "could not acquire device");

        auto id = Device::getId(device);
        if(m_deviceMap.find(id) == m_deviceMap.end())
            m_deviceMap[id] = std::make_shared<Device>(device);
        else
            SAFE_RELEASE(device);

        deviceList.push_back(id);
    }
    return deviceList;
}

DevicePtr Volume::getDevice(std::string id)
{
    auto device = m_deviceMap.find(id);
    if(device != m_deviceMap.end())
        return device->second;
    throw std::runtime_error("unknown device id");
}

DevicePtr Volume::getDefaultOutputDevice()
{
    return getDevice(getDefaultOutputDeviceId());
}

std::vector<DevicePtr> Volume::getDevices(DeviceType type)
{
    auto ids = getDeviceIds(type);
    std::vector<DevicePtr> result;
    for(const auto& id : ids)
        result.push_back(getDevice(id));
    return result;
}

std::shared_ptr<Volume> instance;
std::shared_ptr<IVolumeControl> init(std::string locale)
{
    if(!instance)
    {
        setlocale(LC_ALL, locale.c_str());
        instance = std::make_shared<Volume>();
    }
    return instance;
}
}
