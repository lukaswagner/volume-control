#include <napi.h>

#include "ivolume.hpp"

using Info = const Napi::CallbackInfo&;
using Volume = std::shared_ptr<VolumeControl::IVolumeControl>;
using Device = std::shared_ptr<VolumeControl::IDevice>;
using Session = std::shared_ptr<VolumeControl::ISession>;

Napi::Value createSession(Info info, Session session)
{
    auto env = info.Env();
    auto nodeSession = Napi::Object::New(env);

    nodeSession.Set(
        "getName",
        Napi::Function::New(
            env,
            [session](Info info)
            {
                return Napi::String::New(info.Env(), session->getName());
            }));

    nodeSession.Set(
        "getVolume",
        Napi::Function::New(
            env,
            [session](Info info)
            {
                return Napi::Number::New(info.Env(), session->getVolume());
            }));

    nodeSession.Set(
        "setVolume",
        Napi::Function::New(
            env,
            [session](Info info)
            {
                session->setVolume(info[0].ToNumber().FloatValue());
            }));

    nodeSession.Set(
        "getMute",
        Napi::Function::New(
            env,
            [session](Info info)
            {
                return Napi::Boolean::New(info.Env(), session->getMute());
            }));

    nodeSession.Set(
        "setMute",
        Napi::Function::New(
            env,
            [session](Info info)
            {
                session->setMute(info[0].ToBoolean().Value());
            }));

    return nodeSession;
}

Napi::Value createDevice(Info info, Device device)
{
    auto env = info.Env();
    auto nodeDevice = Napi::Object::New(env);

    nodeDevice.Set(
        "getName",
        Napi::Function::New(
            env,
            [device](Info info)
            {
                return Napi::String::New(info.Env(), device->getName());
            }));

    nodeDevice.Set(
        "getVolume",
        Napi::Function::New(
            env,
            [device](Info info)
            {
                return Napi::Number::New(info.Env(), device->getVolume());
            }));

    nodeDevice.Set(
        "setVolume",
        Napi::Function::New(
            env,
            [device](Info info)
            {
                device->setVolume(info[0].ToNumber().FloatValue());
            }));

    nodeDevice.Set(
        "getMute",
        Napi::Function::New(
            env,
            [device](Info info)
            {
                return Napi::Boolean::New(info.Env(), device->getMute());
            }));

    nodeDevice.Set(
        "setMute",
        Napi::Function::New(
            env,
            [device](Info info)
            {
                device->setMute(info[0].ToBoolean().Value());
            }));

    nodeDevice.Set(
        "getSessions",
        Napi::Function::New(
            env,
            [device](Info info)
            {
                auto sessions = device->getSessions();
                auto result = Napi::Array::New(info.Env());
                for (auto i = 0; i < sessions.size(); ++i)
                    result[i] = createSession(info, sessions[i]);
                return result;
            }));

    return nodeDevice;
}

Napi::Value init(Info info)
{
    auto env = info.Env();
    auto volumeControl = VolumeControl::init(".utf-8");

    auto nodeVolumeControl = Napi::Object::New(env);

    nodeVolumeControl.Set(
        "getDefaultOutputDevice",
        Napi::Function::New(
            env,
            [volumeControl](Info info)
            {
                return createDevice(info, volumeControl->getDefaultOutputDevice());
            }));

    nodeVolumeControl.Set(
        "getDevices",
        Napi::Function::New(
            env,
            [volumeControl](Info info)
            {
                auto type =
                    static_cast<VolumeControl::DeviceType>(info[0].ToNumber().Uint32Value());
                auto devices = volumeControl->getDevices(type);
                auto result = Napi::Array::New(info.Env());
                for (auto i = 0; i < devices.size(); ++i)
                    result[i] = createDevice(info, devices[i]);
                return result;
            }));

    return nodeVolumeControl;
}

Napi::Object setup(Napi::Env env, Napi::Object exports)
{
    exports.Set("init", Napi::Function::New(env, init));

    auto deviceType = Napi::Object::New(env);
    deviceType.Set("Output", 0b01);
    deviceType.Set("Input", 0b10);
    deviceType.Set("Both", 0b11);
    exports.Set("DeviceType", deviceType);

    return exports;
}

NODE_API_MODULE(VolumeControl, setup)
