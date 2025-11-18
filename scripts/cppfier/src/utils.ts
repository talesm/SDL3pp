import {
  readFileSync,
  writeFileSync,
  type PathOrFileDescriptor,
} from "node:fs";

/**
 * Read a file as an array of lines
 * @param path
 */

export function readLinesSync(path: PathOrFileDescriptor) {
  return readFileSync(path, "utf8").split(/\r?\n/);
}

/**
 * Write an array of lines to a file
 * @param path
 * @param data
 */
export function writeLinesSync(path: PathOrFileDescriptor, data: string[]) {
  return writeFileSync(path, data.join("\n").trim() + "\n");
}

/**
 * Read a file as a JSON
 * @param path
 */
export function readJSONSync(path: PathOrFileDescriptor) {
  return JSON.parse(readFileSync(path, "utf8"));
}

/**
 * Write an array of lines to a file
 * @param path
 * @param data
 */
export function writeJSONSync(path: PathOrFileDescriptor, data: any) {
  return writeFileSync(path, JSON.stringify(data, null, 2) + "\n");
}

/**
 * Combine source into target
 * @param target
 * @param source
 * @returns
 */
export function combineArray(target: any[], source: any) {
  if (Array.isArray(source)) {
    if (source.length < target.length) {
      target.length = source.length;
    }
    for (let i = 0; i < source.length; i++) {
      const targetValue = target[i];
      if (targetValue === null || typeof targetValue !== "object") {
        target[i] = source[i];
      } else if (Array.isArray(targetValue)) {
        combineArray(targetValue, source[i]);
      } else combineObject(targetValue, source[i]);
    }
  } else {
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
export function combineObject(target: { [key: string]: any }, source: any) {
  if (!source || typeof source !== "object") return target;
  for (const [k, v] of Object.entries(source)) {
    const targetValue = target[k];
    if (
      !Object.hasOwn(target, k) ||
      targetValue === null ||
      typeof targetValue !== "object"
    ) {
      target[k] = v;
    } else if (Array.isArray(targetValue)) {
      combineArray(targetValue, v);
    } else {
      combineObject(targetValue, v);
    }
  }
  return target;
}

/**
 * Returns true if this seems like a free() function
 * @param name
 */
export function looksLikeFreeFunction(name: string) {
  return (
    /^[A-Z]+_([Dd]estroy|[Cc]lose|[Ff]ree)[A-Z]/.test(name) ||
    /_([Dd]estroy|[Cc]lose|[Ff]ree)$/.test(name)
  );
}

export const system = {
  silent: true,
  stopOnWarn: true,

  log(...data) {
    if (!system.silent) console.log(...data);
  },
  warn(...data) {
    if (!system.silent) console.warn(...data);
    if (this.stopOnWarn) {
      let message = "Stopped on warning";
      if (this.silent && data[0]) message += ": " + (data[0] ?? "");
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
export function deepClone<T>(obj: T): T {
  if (typeof obj !== "object" || obj === null) return obj;
  if (Array.isArray(obj)) return obj.map((el) => deepClone(el)) as T;
  // @ts-ignore
  if (typeof obj?.clone === "function") return obj.clone() as T;
  const result = {};
  for (const [key, value] of Object.entries(obj)) {
    result[key] = deepClone(value);
  }
  return result as T;
}
