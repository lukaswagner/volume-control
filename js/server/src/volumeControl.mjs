import VolumeControl from 'volume-control';

const volume = VolumeControl.init();

export const defaultOutput = volume.getDefaultOutputDevice().getId();

/** @type {Map<string, VolumeControl.Device>} */
export const devices = new Map();

/** @type {Map<string, VolumeControl.Device>} */
export const inputDevices = new Map();

volume.getDevices(VolumeControl.DeviceType.Input).forEach(
    (d) => {
        devices.set(d.getId(), d);
        inputDevices.set(d.getId(), d);
    });

/** @type {Map<string, VolumeControl.Device>} */
export const outputDevices = new Map();

volume.getDevices(VolumeControl.DeviceType.Output).forEach(
    (d) => {
        devices.set(d.getId(), d);
        outputDevices.set(d.getId(), d);
    });

/** @type {Map<string, VolumeControl.Session>} */
export const sessions = new Map();
/** @type {Map<string, Set<string>>} */
export const deviceSessions = new Map();

[...inputDevices, ...outputDevices].forEach((d) => {
    const s = d[1].getSessions();
    s.forEach((s) => sessions.set(s.getId(), s));
    deviceSessions.set(d[0], new Set(s.map((s) => s.getId())));
});
