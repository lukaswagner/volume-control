export function init(): VolumeControl;

export enum DeviceType {
    Output = 0b01,
    Input = 0b10,
    Both = 0b11,
}

export class VolumeControl {
    getDefaultOutputDevice(): Device;
    getDevices(type: DeviceType): Device[];
}

export class Device {
    getName(): string;
    getId(): string;
    getVolume(): number;
    setVolume(volume: number): void;
    getMute(): boolean;
    setMute(mute?: boolean): void;
    getSessions(): Session[];
}

export class Session {
    getName(): string;
    getId(): string;
    getVolume(): number;
    setVolume(volume: number): void;
    getMute(): boolean;
    setMute(mute?: boolean): void;
}
