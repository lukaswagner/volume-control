#pragma once

#include <vector>
#include <mmdeviceapi.h>

#include "ivolume.hpp"
#include "device.hpp"

namespace VolumeControl
{

class Volume : public IVolumeControl
{
private:
    IMMDeviceEnumerator* m_devices;
public:
    Volume();
    ~Volume();
    std::shared_ptr<IDevice> getDefaultOutputDevice() override;
    std::vector<std::shared_ptr<IDevice>> getDevices(DeviceType type) override;
};
}
