#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <functional>

#include "vc_export.hpp"
#include "idevice.hpp"

namespace VolumeControl
{
enum DeviceType
{
    Output = 0b01,
    Input = 0b10,
    Both = 0b11,
};

enum EventType
{
    DeviceAdded,
    DeviceRemoved,
    DeviceStateChanged,
    DefaultDeviceChanged,
};

enum DeviceState
{
    Active,
    Disabled,
    NotPresent,
    Unplugged,
};

union EventData
{
    DeviceState state;
};

struct Event
{
    EventType type;
    std::string id;
    std::optional<EventData> data;
};

typedef std::function<void(Event)> EventListener;

class VC_EXPORT IVolumeControl
{
public:
    virtual ~IVolumeControl() {};

    virtual std::string getDefaultOutputDeviceId() = 0;
    virtual std::vector<std::string> getDeviceIds(DeviceType type) = 0;
    virtual std::shared_ptr<IDevice> getDevice(std::string id) = 0;

    virtual std::shared_ptr<IDevice> getDefaultOutputDevice() = 0;
    virtual std::vector<std::shared_ptr<IDevice>> getDevices(DeviceType type) = 0;

    virtual void registerListener(EventListener* listener) = 0;
    virtual void unregisterListener(EventListener* listener) = 0;
};

std::shared_ptr<IVolumeControl> VC_EXPORT init(std::string locale = "");
}
