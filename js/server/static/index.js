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
 */
async function createDevice(container, deviceId) {
    const title = document.createElement('div');
    title.innerText = await get(`/device/${deviceId}/name`);

    const controls = await createControls(`/device/${deviceId}`);

    container.appendChild(title);
    container.appendChild(controls)
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
    const devices = JSON.parse(await get('/devices/output'));
    devices.forEach(async (d) => await createDevice(container, d));
}

const container = document.getElementById('vc');
createDeviceList(container);

