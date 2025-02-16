#include "volume.hpp"

#include <stdexcept>
#include <unordered_map>

#include "helper.hpp"

namespace VolumeControl
{
Volume::Volume()
{
    m_refCount = 1;
    m_listeners = std::set<EventListener*>();

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

    getDeviceIds(DeviceType::Both);

    result = m_devices->RegisterEndpointNotificationCallback(this);
    CHECK(result, "could not register notification callback");
    std::cout << "registered" << std::endl;
}

Volume::~Volume()
{
    m_devices->UnregisterEndpointNotificationCallback(this);

    SAFE_RELEASE(m_devices);
}

std::string Volume::initDevice(IMMDevice* device)
{
    auto id = Device::getId(device);
    if(m_deviceMap.find(id) == m_deviceMap.end())
        m_deviceMap[id] = std::make_shared<Device>(device);
    else
        SAFE_RELEASE(device);
    return id;
}

std::string Volume::getDefaultOutputDeviceId()
{
    IMMDevice* device;
    auto result =
        m_devices->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    CHECK(result, "could not acquire default device");

    return initDevice(device);
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

        auto id = initDevice(device);
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

void Volume::registerListener(EventListener* listener)
{
    m_listeners.insert(listener);
}

void Volume::unregisterListener(EventListener* listener)
{
    m_listeners.erase(listener);
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

std::unordered_map<DWORD, DeviceState> DeviceStateMap = {
    {DEVICE_STATE_ACTIVE, Active},
    {DEVICE_STATE_DISABLED, Disabled},
    {DEVICE_STATE_NOTPRESENT, NotPresent},
    {DEVICE_STATE_UNPLUGGED, Unplugged},
};

#pragma region IMMNotificationClient
HRESULT Volume::OnDeviceStateChanged(LPCWSTR id, DWORD newState)
{
    IMMDevice* device;
    m_devices->GetDevice(id, &device);
    DeviceState state = DeviceStateMap[newState];
    Event event = { DeviceStateChanged, initDevice(device), std::make_optional<EventData>({ state }) };
    for(auto it = m_listeners.begin(); it != m_listeners.end(); it++)
    {
        (**it)(event);
    }

    return S_OK;
};

HRESULT Volume::OnDeviceAdded(LPCWSTR id)
{
    IMMDevice* device;
    m_devices->GetDevice(id, &device);
    Event event = { DeviceAdded, initDevice(device), std::make_optional<EventData>() };
    for(auto it = m_listeners.begin(); it != m_listeners.end(); it++)
    {
        (**it)(event);
    }

    return S_OK;
};

HRESULT Volume::OnDeviceRemoved(LPCWSTR id)
{
    IMMDevice* device;
    m_devices->GetDevice(id, &device);
    Event event = { DeviceRemoved, initDevice(device), std::make_optional<EventData>() };
    for(auto it = m_listeners.begin(); it != m_listeners.end(); it++)
    {
        (**it)(event);
    }

    return S_OK;
};

HRESULT Volume::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR id)
{
    if(flow != eRender || role != eMultimedia) return S_OK;

    IMMDevice* device;
    m_devices->GetDevice(id, &device);
    Event event = { DefaultDeviceChanged, initDevice(device), std::make_optional<EventData>() };
    for(auto it = m_listeners.begin(); it != m_listeners.end(); it++)
    {
        (**it)(event);
    }

    return S_OK;
};
#pragma endregion IMMNotificationClient

#pragma region IUnknown
// implementations from https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/multimedia/audio/osd/endpointMonitor.cpp
HRESULT Volume::QueryInterface(REFIID iid, void** ppUnk)
{
    if ((iid == __uuidof(IUnknown)) ||
        (iid == __uuidof(IMMNotificationClient)))
    {
        *ppUnk = static_cast<IMMNotificationClient*>(this);
    }
    else
    {
        *ppUnk = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

ULONG Volume::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

ULONG Volume::Release()
{
    long lRef = InterlockedDecrement(&m_refCount);
    if (lRef == 0)
    {
        delete this;
    }
    return lRef;
}
#pragma endregion IUnknown
}
