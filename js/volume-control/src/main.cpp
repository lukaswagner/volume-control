#include <napi.h>
#include "ivolume.hpp"

Napi::String Method(const Napi::CallbackInfo& info)
{
    auto volume = VolumeControl::init(".utf-8");
    auto deviceName = volume->getDefaultOutputDevice()->getName();
    Napi::Env env = info.Env();
    return Napi::String::New(env, deviceName);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "hello"),
        Napi::Function::New(env, Method));
    return exports;
}

NODE_API_MODULE(hello, Init)
