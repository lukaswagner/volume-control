#pragma once

#include <vector>
#include <mmdeviceapi.h>

#include "device.hpp"

enum DeviceType {
    Output = eRender,
    Input = eCapture,
    Both = eAll,
};

class Volume
{
private:
    IMMDeviceEnumerator* m_devices;

public:
    Volume();
    Device getDefaultOutputDevice();
    std::vector<Device> getAllDevices(DeviceType type);
};
