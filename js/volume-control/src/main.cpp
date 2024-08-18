#include <napi.h>
#include "ivolume.hpp"

std::shared_ptr<VolumeControl::IVolumeControl> volumeControl;

Napi::String test(const Napi::CallbackInfo& info)
{
    auto deviceName = volumeControl->getDefaultOutputDevice()->getName();
    auto env = info.Env();
    return Napi::String::New(env, deviceName);
}

Napi::Value init(const Napi::CallbackInfo& info)
{
    if(!volumeControl)
        volumeControl = VolumeControl::init(".utf-8");

    auto env = info.Env();
    auto result = Napi::Object::New(env);
    result.Set("test", Napi::Function::New(env, test));

    return result;
}

Napi::Object setup(Napi::Env env, Napi::Object exports)
{
    exports.Set("init", Napi::Function::New(env, init));
    return exports;
}

NODE_API_MODULE(VolumeControl, setup)
