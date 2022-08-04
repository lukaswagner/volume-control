#pragma once

#include <mmdeviceapi.h>

#include "device.hpp"

class Volume
{
private:
    IMMDeviceEnumerator* m_devices;

public:
    Volume();
    Device getDevice();
};
