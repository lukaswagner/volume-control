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
sessions.forEach((_, s) => console.log(s));

const app = express();
app.use(express.text());

const staticDir = path.join(url.fileURLToPath(import.meta.url), '../../static');
app.use('/', express.static(staticDir));

// top level routes

function sortKeys(map, checkDefault) {
    return [...map.keys()].sort((a, b) => {
        if(checkDefault) {
            if(a === b) return 0;
            if(a === defaultOutput) return -1;
            if(b === defaultOutput) return 1;
        }
        const nameA = map.get(a).getName();
        const nameB = map.get(b).getName();
        return nameA.localeCompare(nameB);
    })
}

app.get('/devices/input', (req, res) => {
    res.send(sortKeys(inputDevices, false));
});

app.get('/devices/output', (req, res) => {
    res.send(sortKeys(outputDevices, true));
});

app.get('/devices/output/default', (req, res) => {
    res.send(defaultOutput);
});

// device routes

const deviceCheck = (req, res, next) => {
    if (!devices.has(req.params.deviceId)){
        console.log(`invalid device request: ${req.params.deviceId}`);
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
            console.log(`device - invalid volume request: ${req.body}`);
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
            console.log(`device - invalid mute request: ${req.body}`);
            res.sendStatus(406);
            return;
        }
        devices.get(req.params.deviceId).setMute(parsedAsTrue);
        res.sendStatus(200);
    });

app.get('/device/:deviceId/sessions', deviceCheck, (req, res) => {
    res.send([...deviceSessions.get(req.params.deviceId)]
        .sort((a, b) => {
            const nameA = sessions.get(a).getName();
            const nameB = sessions.get(b).getName();
            return nameA.localeCompare(nameB);
        }));
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
            console.log(`session - invalid volume request: ${req.body}`);
            res.sendStatus(406);
            return;
        }
        sessions.get(req.params.sessionId).setVolume(val);
        res.sendStatus(200);
    });

app.route('/session/:sessionId/mute')
    .get(sessionCheck, (req, res) => {
        res.send(sessions.get(req.params.sessionId).getMute().toString());
    })
    .put(sessionCheck, (req, res) => {
        const body = req.body.toLowerCase();
        const parsedAsTrue = (body === 'true') || (body === '1');
        const parsedAsFalse = (body === 'false') || (body === '0');
        if (!parsedAsTrue && !parsedAsFalse) {
            console.log(`session - invalid mute request: ${req.body}`);
            res.sendStatus(406);
            return;
        }
        sessions.get(req.params.sessionId).setMute(parsedAsTrue);
        res.sendStatus(200);
    });

app.listen(8080)
