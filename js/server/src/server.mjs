import VolumeControl from 'volume-control';
import express from 'express';
import path from 'node:path';
import url from 'node:url';

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

console.log('devices:');
devices.forEach((_, d) => console.log(d));
console.log('sessions:');
devices.forEach((_, s) => console.log(s));

const app = express();
const staticDir = path.join(url.fileURLToPath(import.meta.url), '../../static');
app.use('/', express.static(staticDir));

// top level routes

app.get('/devices/input', (req, res) => {
    res.send([...inputDevices.keys()]);
});

app.get('/devices/output', (req, res) => {
    res.send([...outputDevices.keys()]);
});

app.get('/devices/output/default', (req, res) => {
    res.send(defaultOutput);
});

// device routes

const deviceCheck = (req, res, next) => {
    if (!devices.has(req.params.deviceId)){
        console.log(`invalid device request: ${req.params.deviceId}`)
        res.sendStatus(404);
    }
    else
        next();
}

app.get('/device/:deviceId/name', deviceCheck, (req, res) => {
    res.send(devices.get(req.params.deviceId).getName());
});

app.route('/device/:deviceId/volume')
    .get(deviceCheck, (req, res) => {
        res.send(devices.get(req.params.deviceId).getVolume().toString());
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
        res.send(devices.get(req.params.deviceId).getMute().toString());
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

// session routes

const sessionCheck = (req, res, next) => {
    if (!sessions.has(req.params.sessionId)){
        console.log(`invalid session request: ${req.params.sessionId}`)
        res.sendStatus(404);
    }
    else
        next();
}

app.get('/session/:sessionId/name', sessionCheck, (req, res) => {
    res.send(sessions.get(req.params.sessionId).getName());
});

app.route('/session/:sessionId/volume')
    .get(sessionCheck, (req, res) => {
        res.send(sessions.get(req.params.sessionId).getVolume().toString());
    })
    .put(sessionCheck, (req, res) => {
        const val = Number.parseFloat(req.body);
        if (Number.isNaN(val)) {
            res.sendStatus(406);
            return;
        }
        sessions.get(req.params.sessionId).setVolume(val);
        res.sendStatus(200);
    });

app.route('/session/:sessionId/mute')
    .get(deviceCheck, (req, res) => {
        res.send(sessions.get(req.params.sessionId).getMute().toString());
    })
    .put(deviceCheck, (req, res) => {
        const body = req.body.toLowerCase();
        const parsedAsTrue = (body === 'true') || (body === '1');
        const parsedAsFalse = (body === 'false') || (body === '0');
        if (!parsedAsTrue && !parsedAsFalse) {
            res.sendStatus(406);
            return;
        }
        sessions.get(req.params.sessionId).setVolume(parsedAsTrue);
        res.sendStatus(200);
    });

app.listen(8080)
