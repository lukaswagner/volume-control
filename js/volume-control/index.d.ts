export function init(): VolumeControl;

export enum DeviceType {
    Output = 0b01,
    Input = 0b10,
    Both = 0b11,
}

export class VolumeControl {
    getDefaultOutputDevice(): Device;
    getAllDevices(type: DeviceType): Device[];
}

export class Device {
    getName(): string;
    getVolume(): number;
    setVolume(volume: number): void;
    getMute(): boolean;
    setMute(mute?: boolean): void;
    getSessionCount(): number;
    getSession(id: number): Session;
    getSessions(): Session[];
}

export class Session {
    getName(): string;
    getVolume(): number;
    setVolume(volume: number): void;
    getMute(): boolean;
    setMute(mute?: boolean): void;
}
