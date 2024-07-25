#pragma once

#include <vector>
#include <memory>

#include "vc_export.hpp"
#include "idevice.hpp"

namespace VolumeControl
{
enum DeviceType {
    Output,
    Input,
    Both,
};

class VC_EXPORT IVolumeControl
{
public:
    virtual std::shared_ptr<IDevice> getDefaultOutputDevice() = 0;
    virtual std::vector<std::shared_ptr<IDevice>> getAllDevices(DeviceType type) = 0;
};

std::shared_ptr<IVolumeControl> VC_EXPORT init();
}
