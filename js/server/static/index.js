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
 * @param {string} path
 * @param {string} value
 */
async function put(path, value) {
    console.log(`request: ${path} ${value}`);
    const response = await fetch(path, { method: 'PUT', body: value });
    if(response.status !== 200)
        throw(`could not fetch ${path}`);
    const content = await response.text();
    console.log(`response: ${content}`);
    return content;
}

/**
 * @param {string} deviceId
 * @param {boolean} isDefault
 */
async function createDevice(deviceId, isDefault) {
    const container = document.createElement('div');
    container.classList.add('container');

    const title = document.createElement('div');
    const defaultTag = isDefault ? '[Default] ' : '';
    title.innerText = defaultTag + await get(`/device/${deviceId}/name`);

    const controls = await createControls(`/device/${deviceId}`);

    const sessionContainer = document.createElement('div');
    const sessions = JSON.parse(await get(`/device/${deviceId}/sessions`));
    sessions.forEach(async (s) => sessionContainer.appendChild(await createSession(s)));

    container.appendChild(title);
    container.appendChild(controls);
    container.appendChild(sessionContainer);
    return container;
}

/**
 * @param {string} sessionId
 */
async function createSession(sessionId) {
    const container = document.createElement('div');
    container.classList.add('container');

    const title = document.createElement('div');
    title.innerText = await get(`/session/${sessionId}/name`);

    const controls = await createControls(`/session/${sessionId}`);

    container.appendChild(title);
    container.appendChild(controls);
    return container;
}

/**
 * @param {string} path
 */
async function createControls(path) {
    const controls = document.createElement('div');
    controls.classList.add('controls');

    let isMute = await get(`${path}/mute`) === 'true';
    const mute = document.createElement('button');
    mute.innerHTML = '&#128264;';
    mute.classList.add('mute');
    if(!isMute) mute.classList.add('unmute');
    mute.onclick = () => {
        isMute = !isMute;
        if(isMute)
            mute.classList.remove('unmute');
        else
            mute.classList.add('unmute');
        put(`${path}/mute`, isMute.toString());
    };
    controls.appendChild(mute);

    const volume = document.createElement('input');
    volume.type = 'range';
    volume.min = '0';
    volume.max = '1';
    volume.step = '0.01';
    volume.value = await get(`${path}/volume`);
    volume.onchange = () => put(`${path}/volume`, volume.value);
    controls.appendChild(volume);

    return controls;
}

async function createDeviceList(container) {
    const defaultDevice = await get('/devices/output/default');
    const devices = JSON.parse(await get('/devices/output'));
    devices.forEach(async (d) =>
        container.appendChild(await createDevice(d, d === defaultDevice)));
}

const container = document.getElementById('vc');
createDeviceList(container);

