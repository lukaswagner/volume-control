#pragma once

#include <map>
#include <string>
#include <vector>
#include <mmdeviceapi.h>

#include "ivolume.hpp"
#include "device.hpp"

namespace VolumeControl
{
using DevicePtr = std::shared_ptr<IDevice>;

class Volume : public IVolumeControl
{
private:
    IMMDeviceEnumerator* m_devices;
    std::map<std::string, DevicePtr> m_deviceMap;

public:
    Volume();
    ~Volume();

    std::string getDefaultOutputDeviceId() override;
    std::vector<std::string> getDeviceIds(DeviceType type) override;
    DevicePtr getDevice(std::string id) override;

    DevicePtr getDefaultOutputDevice() override;
    std::vector<DevicePtr> getDevices(DeviceType type) override;
};
}
