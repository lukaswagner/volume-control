/**
 * @param {string} path
 */
async function get(path) {
    console.log(`request: ${path}`);
    const response = await fetch(path, { method: 'GET' });
    if(response.status !== 200)
        throw(`could not fetch ${path}`);
    const content = await response.text();
    console.log(`response: ${content}`);
    return content;
}

/**
 * @param {HTMLDivElement} container
 * @param {string} deviceId
 * @param {boolean} isDefault
 */
async function createDevice(container, deviceId, isDefault) {
    const title = document.createElement('div');
    const defaultTag = isDefault ? '[Default] ' : '';
    title.innerText = defaultTag + await get(`/device/${deviceId}/name`);

    const controls = await createControls(`/device/${deviceId}`);

    const sessionContainer = document.createElement('div');
    const sessions = JSON.parse(await get(`/device/${deviceId}/sessions`));
    sessions.forEach(async (s) => await createSession(sessionContainer, s));

    container.appendChild(title);
    container.appendChild(controls);
    container.appendChild(sessionContainer);
}

/**
 * @param {HTMLDivElement} container
 * @param {string} sessionId
 */
async function createSession(container, sessionId) {
    const title = document.createElement('div');
    title.innerText = await get(`/session/${sessionId}/name`);

    const controls = await createControls(`/session/${sessionId}`);

    container.appendChild(title);
    container.appendChild(controls);
}

/**
 * @param {string} path
 */
async function createControls(path) {
    const controls = document.createElement('div');

    const volume = document.createElement('input');
    volume.type = 'range';
    volume.min = '0';
    volume.max = '1';
    volume.step = '0.01';
    volume.value = await get(`${path}/volume`);
    controls.appendChild(volume);

    return controls;
}

async function createDeviceList(container) {
    const defaultDevice = await get('/devices/output/default');
    const devices = JSON.parse(await get('/devices/output'));
    devices.forEach(async (d) => await createDevice(container, d, d === defaultDevice));
}

const container = document.getElementById('vc');
createDeviceList(container);

