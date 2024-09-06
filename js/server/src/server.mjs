import express from 'express';
import path from 'node:path';
import url from 'node:url';
import { devices, sessions, inputDevices, outputDevices, defaultOutput, deviceSessions } from './volumeControl.mjs';
import { check, getMute, getName, getVolume, setMute, setVolume } from './handler.mjs';
import { sortKeys } from './helper.mjs';

console.log('devices:');
devices.forEach((_, d) => console.log(d));
console.log('sessions:');
sessions.forEach((_, s) => console.log(s));

const app = express();
app.use(express.text());

const staticDir = path.join(url.fileURLToPath(import.meta.url), '../../static');
app.use('/', express.static(staticDir));

// top level routes

const getInputs = (req, res) => res.send(sortKeys(inputDevices));
app.get('/devices/input', getInputs);
const getOutputs = (req, res) => res.send(sortKeys(outputDevices, undefined, defaultOutput));
app.get('/devices/output', getOutputs);

app.get('/devices/output/default', (req, res) => res.send(defaultOutput));

// device routes

const deviceCheck = check.bind(undefined, { map: devices });

app.get('/device/:id/name', deviceCheck, getName.bind(undefined, { map: devices }));

app.route('/device/:id/volume')
    .get(deviceCheck, getVolume.bind(undefined, { map: devices }))
    .put(deviceCheck, setVolume.bind(undefined, { map: devices }));

app.route('/device/:id/mute')
    .get(deviceCheck, getMute.bind(undefined, { map: devices }))
    .put(deviceCheck, setMute.bind(undefined, { map: devices }));

const getSessions = (req, res) => res.send(sortKeys(sessions, [...deviceSessions.get(req.params.id)]));
app.get('/device/:id/sessions', deviceCheck, getSessions);

// session routes

const sessionCheck = check.bind(undefined, { map: sessions });

app.get('/session/:id/name', sessionCheck, getName.bind(undefined, { map: sessions }));

app.route('/session/:id/volume')
    .get(sessionCheck, getVolume.bind(undefined, { map: sessions }))
    .put(sessionCheck, setVolume.bind(undefined, { map: sessions }));

app.route('/session/:id/mute')
    .get(sessionCheck, getMute.bind(undefined, { map: sessions }))
    .put(sessionCheck, setMute.bind(undefined, { map: sessions }));

app.listen(8080);
