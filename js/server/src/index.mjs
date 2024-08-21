import VolumeControl from 'volume-control';
import express from 'express';

const volume = VolumeControl.init();

let defaultOutput = volume.getDefaultOutputDevice().getId();

/** @type {Map<string, VolumeControl.Device>} */
let devices = new Map();

/** @type {Map<string, VolumeControl.Device>} */
let inputDevices = new Map();
volume.getDevices(VolumeControl.DeviceType.Input).forEach(
    (d) => {
        devices.set(d.getId(), d);
        inputDevices.set(d.getId(), d);
    });

/** @type {Map<string, VolumeControl.Device>} */
let outputDevices = new Map();
volume.getDevices(VolumeControl.DeviceType.Output).forEach(
    (d) => {
        devices.set(d.getId(), d);
        outputDevices.set(d.getId(), d);
    });

/** @type {Map<string, VolumeControl.Session>} */
let sessions = new Map();
/** @type {Map<string, Set<string>>} */
let deviceSessions = new Map();
[...inputDevices, ...outputDevices].forEach((d) => {
    const s = d[1].getSessions();
    s.forEach((s) => sessions.set(s.getId(), s));
    deviceSessions.set(d[0], new Set(s.map((s) => s.getId())));
});

const app = express();

// top level routes

app.get('/input', (req, res) => {
    res.send([...inputDevices.keys()]);
});

app.get('/output', (req, res) => {
    res.send([...outputDevices.keys()]);
});

app.get('/defaultOutput', (req, res) => {
    res.send(defaultOutput);
});

// device routes

const deviceCheck = (req, res) => {
    if (!devices.has(req.params.deviceId))
        res.sendStatus(404);
    else
        next();
}

app.get('/device/:deviceId/name', deviceCheck, (req, res) => {
    res.send(devices.get(req.params.deviceId).getName());
});

app.route('/device/:deviceId/volume')
    .get(deviceCheck, (req, res) => {
        res.send(devices.get(req.params.deviceId).getVolume());
    })
    .put(deviceCheck, (req, res) => {
        const val = Number.parseFloat(req.body);
        if (Number.isNaN(val)) {
            res.sendStatus(406);
            return;
        }
        devices.get(req.params.deviceId).setVolume(val);
        res.sendStatus(200);
    });

app.route('/device/:deviceId/mute')
    .get(deviceCheck, (req, res) => {
        res.send(devices.get(req.params.deviceId).getMute());
    })
    .put(deviceCheck, (req, res) => {
        const body = req.body.toLowerCase();
        const parsedAsTrue = (body === 'true') || (body === '1');
        const parsedAsFalse = (body === 'false') || (body === '0');
        if (!parsedAsTrue && !parsedAsFalse) {
            res.sendStatus(406);
            return;
        }
        devices.get(req.params.deviceId).setVolume(parsedAsTrue);
        res.sendStatus(200);
    });

app.get('/device/:deviceId/sessions', deviceCheck, (req, res) => {
    res.send([...deviceSessions.get(req.params.deviceId)]);
});

app.listen(8080)
