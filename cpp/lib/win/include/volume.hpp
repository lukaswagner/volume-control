#pragma once

#include <map>
#include <string>
#include <vector>
#include <set>
#include <mmdeviceapi.h>

#include "ivolume.hpp"
#include "device.hpp"

namespace VolumeControl
{
using DevicePtr = std::shared_ptr<IDevice>;

class Volume : public IVolumeControl, IMMNotificationClient
{
private:
    IMMDeviceEnumerator* m_devices;
    std::map<std::string, DevicePtr> m_deviceMap;
    long int m_refCount;
    std::set<EventListener*> m_listeners;

public:
    Volume();
    ~Volume();

    std::string initDevice(IMMDevice*);

    std::string getDefaultOutputDeviceId() override;
    std::vector<std::string> getDeviceIds(DeviceType type) override;
    DevicePtr getDevice(std::string id) override;

    DevicePtr getDefaultOutputDevice() override;
    std::vector<DevicePtr> getDevices(DeviceType type) override;

    void registerListener(EventListener* listener) override;
    void unregisterListener(EventListener* listener) override;

    HRESULT OnDeviceStateChanged(LPCWSTR id, DWORD newState) override;
    HRESULT OnDeviceAdded(LPCWSTR id) override;
    HRESULT OnDeviceRemoved(LPCWSTR id) override;
    HRESULT OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR id) override;
    HRESULT OnPropertyValueChanged(LPCWSTR id, PROPERTYKEY key) override { return S_OK; };
    HRESULT QueryInterface(REFIID ref, void** obj) override;
    ULONG AddRef() override;
    ULONG Release() override;
};
}
