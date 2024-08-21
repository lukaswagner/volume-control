#pragma once

#include <iostream>
#include <string>

#include "vc_export.hpp"

namespace VolumeControl
{
class VC_EXPORT ISession
{
public:
    virtual ~ISession() {};
    virtual bool isSystem() = 0;
    virtual std::string getId() = 0;
    virtual std::string getName() = 0;
    virtual std::string getPath() = 0;
    virtual float getVolume() = 0;
    virtual void setVolume(float volume) = 0;
    virtual bool getMute() = 0;
    virtual void setMute(bool mute) = 0;
    virtual void dumpInfo(std::ostream& stream) = 0;
};
}
