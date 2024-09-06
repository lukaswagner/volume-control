export function check({ map }, req, res, next) {
    if (!map.has(req.params.id)){
        console.log(`invalid request: ${req.params.id}`);
        res.sendStatus(404);
    }
    else
        next();
}

export function getName({ map }, req, res, next) {
    res.send(map.get(req.params.id).getName());
}

export function getVolume({ map }, req, res, next) {
    res.send(map.get(req.params.id).getVolume().toString());
}

export function setVolume({ map }, req, res, next) {
    const val = Number.parseFloat(req.body);
    if (Number.isNaN(val)) {
        console.log(`invalid volume request: ${req.body}`);
        res.sendStatus(406);
        return;
    }
    map.get(req.params.id).setVolume(val);
    res.sendStatus(200);
}

export function getMute({ map }, req, res, next) {
    res.send(map.get(req.params.id).getMute().toString());
}

export function setMute({ map }, req, res, next) {
    const body = req.body.toLowerCase();
    const parsedAsTrue = (body === 'true') || (body === '1');
    const parsedAsFalse = (body === 'false') || (body === '0');
    if (!parsedAsTrue && !parsedAsFalse) {
        console.log(`device - invalid mute request: ${req.body}`);
        res.sendStatus(406);
        return;
    }
    map.get(req.params.id).setMute(parsedAsTrue);
    res.sendStatus(200);
}
