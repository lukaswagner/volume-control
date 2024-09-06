export function sortKeys(map, keys = [...map.keys()], defaultKey) {
    return keys.sort((a, b) => {
        if(defaultKey !== undefined) {
            if(a === b) return 0;
            if(a === defaultKey) return -1;
            if(b === defaultKey) return 1;
        }
        const nameA = map.get(a).getName();
        const nameB = map.get(b).getName();
        return nameA.localeCompare(nameB);
    });
}
