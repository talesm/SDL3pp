"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.system = void 0;
exports.readLinesSync = readLinesSync;
exports.writeLinesSync = writeLinesSync;
exports.readJSONSync = readJSONSync;
exports.writeJSONSync = writeJSONSync;
exports.combineArray = combineArray;
exports.combineObject = combineObject;
exports.looksLikeFreeFunction = looksLikeFreeFunction;
exports.deepClone = deepClone;
const node_fs_1 = require("node:fs");
/**
 * Read a file as an array of lines
 * @param path
 */
function readLinesSync(path) {
    return (0, node_fs_1.readFileSync)(path, "utf8").split(/\r?\n/);
}
/**
 * Write an array of lines to a file
 * @param path
 * @param data
 */
function writeLinesSync(path, data) {
    return (0, node_fs_1.writeFileSync)(path, data.join("\n").trim() + "\n");
}
/**
 * Read a file as a JSON
 * @param path
 */
function readJSONSync(path) {
    return JSON.parse((0, node_fs_1.readFileSync)(path, "utf8"));
}
/**
 * Write an array of lines to a file
 * @param path
 * @param data
 */
function writeJSONSync(path, data) {
    return (0, node_fs_1.writeFileSync)(path, JSON.stringify(data, null, 2) + "\n");
}
/**
 * Combine source into target
 * @param target
 * @param source
 * @returns
 */
function combineArray(target, source) {
    if (Array.isArray(source)) {
        if (source.length < target.length) {
            target.length = source.length;
        }
        for (let i = 0; i < source.length; i++) {
            const targetValue = target[i];
            if (targetValue === null || typeof targetValue !== "object") {
                target[i] = source[i];
            }
            else if (Array.isArray(targetValue)) {
                combineArray(targetValue, source[i]);
            }
            else
                combineObject(targetValue, source[i]);
        }
    }
    else {
        target.push(source);
    }
    return target;
}
/**
 *
 * @param target
 * @param source
 * @returns
 */
function combineObject(target, source) {
    if (!source || typeof source !== "object")
        return target;
    for (const [k, v] of Object.entries(source)) {
        const targetValue = target[k];
        if (!Object.hasOwn(target, k) ||
            targetValue === null ||
            typeof targetValue !== "object") {
            target[k] = v;
        }
        else if (Array.isArray(targetValue)) {
            combineArray(targetValue, v);
        }
        else {
            combineObject(targetValue, v);
        }
    }
    return target;
}
/**
 * Returns true if this seems like a free() function
 * @param name
 */
function looksLikeFreeFunction(name) {
    return (/^[A-Z]+_([Dd]estroy|[Cc]lose|[Ff]ree)[A-Z]/.test(name) ||
        /_([Dd]estroy|[Cc]lose|[Ff]ree)$/.test(name));
}
exports.system = {
    silent: true,
    stopOnWarn: true,
    log(...data) {
        if (!exports.system.silent)
            console.log(...data);
    },
    warn(...data) {
        if (!exports.system.silent)
            console.warn(...data);
        if (this.stopOnWarn) {
            let message = "Stopped on warning";
            if (this.silent && data[0])
                message += ": " + (data[0] ?? "");
            throw new Error(message);
        }
    },
};
/**
 * Clone object, using .clone() if provided
 * @template {any} T the type
 * @param {T} obj the object to bee cloned
 * @returns {T}
 */
function deepClone(obj) {
    if (typeof obj !== "object" || obj === null)
        return obj;
    if (Array.isArray(obj))
        return obj.map((el) => deepClone(el));
    // @ts-ignore
    if (typeof obj?.clone === "function")
        return obj.clone();
    const result = {};
    for (const [key, value] of Object.entries(obj)) {
        result[key] = deepClone(value);
    }
    return result;
}
