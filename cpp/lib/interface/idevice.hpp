#pragma once

#include <string>
#include <vector>
#include <memory>

#include "vc_export.hpp"
#include "isession.hpp"

namespace VolumeControl
{
class VC_EXPORT IDevice
{
public:
    virtual ~IDevice() {};
    virtual std::string getId() = 0;
    virtual std::string getName() = 0;
    virtual float getVolume() = 0;
    virtual void setVolume(float volume) = 0;
    virtual bool getMute() = 0;
    virtual void setMute(bool mute) = 0;
    virtual int getSessionCount() = 0;
    virtual std::vector<std::string> getSessionIds() = 0;
    virtual std::shared_ptr<ISession> getSession(std::string id) = 0;
    virtual std::vector<std::shared_ptr<ISession>> getSessions() = 0;
    virtual void dumpInfo(std::ostream& stream) = 0;
};
}
