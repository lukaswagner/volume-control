import VolumeControl from 'volume-control';
import express from 'express';

const volume = VolumeControl.init();

let defaultOutput = volume.getDefaultOutputDevice().getId();

/** @type {Map<string, VolumeControl.Device>} */
let inputDevices = new Map();
volume.getDevices(VolumeControl.DeviceType.Input).forEach(
    (d) => inputDevices.set(d.getId(), d));

/** @type {Map<string, VolumeControl.Device>} */
let outputDevices = new Map();
volume.getDevices(VolumeControl.DeviceType.Output).forEach(
    (d) => outputDevices.set(d.getId(), d));

const app = express();

app.get('/input', function (req, res) {
    res.send([...inputDevices.keys()]);
});

app.get('/output', function (req, res) {
    res.send([...outputDevices.keys()]);
});

app.get('/defaultOutput', function (req, res) {
    res.send(defaultOutput);
});

app.listen(8080)
