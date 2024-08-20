import VolumeControl from 'volume-control';

const vc = VolumeControl.init();
const devices = vc.getDevices(VolumeControl.DeviceType.Output);
devices.forEach((device, i) =>
{
    if(i > 0) console.log('-----');
    console.log(
        device.getVolume().toFixed(2),
        device.getName(),
    );
    const sessions = device.getSessions();
    sessions.forEach(
        (session) => console.log(
            session.getVolume().toFixed(2),
            session.getName(),
        )
    );
});
