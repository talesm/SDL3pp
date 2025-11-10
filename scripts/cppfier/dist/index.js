"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const generate_1 = require("./generate");
const parse_1 = require("./parse");
const transform_js_1 = require("./transform.js");
const utils_js_1 = require("./utils.js");
/**
 * Process the main
 * @param args the arguments
 */
function main(args) {
    utils_js_1.system.silent = false;
    utils_js_1.system.stopOnWarn = false;
    if (!args.length) {
        printErrorWithGuide("Missing verb");
        process.exit(1);
    }
    const command = args.shift();
    switch (command) {
        case "parse":
            parse(args);
            break;
        case "generate":
            generate(args);
            break;
        case "transform":
            transform(args);
            break;
        case "--help":
        case "help":
            help(args);
            break;
        default:
            printErrorWithGuide(`Invalid verb ${command}`);
            process.exit(1);
    }
}
const guideDoc = [
    wrapUsageText(`usage: node ${process.argv[1]} ` + "<verb> [OPTION]... [--] <file>..."),
    "\nValid verbs are:",
    "    parse      parse headers",
    "    transform  transform source C API into C++ API",
    "    generate   generate target headers",
    "    update     update target headers DEPRECATED",
    "    help       Show help",
];
/**
 * Prints error with a guide
 */
function printErrorWithGuide(message, ...parameters) {
    return printError(message, ...parameters, "", ...guideDoc);
}
/**
 * Prints error
 */
function printError(message, ...parameters) {
    return printLog(`Error: ${message}`, ...parameters);
}
/**
 * Scan files
 * @param args the arguments
 */
function parse(args) {
    if (args?.length == 0) {
        return help(["parse"]);
    }
    const config = {
        sources: [],
        outputFile: "",
        api: null,
        baseDir: [],
        storeLineNumbers: false,
    };
    let printConfig = false;
    for (let i = 0; i < args.length; i++) {
        const arg = args[i];
        if (arg == "--") {
            config.sources.push(...args.slice(i + 1).map(arg => arg.replaceAll("\\", '/')));
            break;
        }
        if (!arg.startsWith('-')) {
            if (arg.endsWith(".json")) {
                mergeInto(config, (0, utils_js_1.readJSONSync)(arg.replaceAll("\\", '/')));
            }
            else
                config.sources.push(arg.replaceAll("\\", '/'));
            continue;
        }
        switch (arg) {
            case '--outputFile':
            case '-o':
                config.outputFile = args[++i].replaceAll("\\", '/');
                break;
            case '--baseDir':
            case '-d':
                config.baseDir.push(args[++i].replaceAll("\\", '/'));
                break;
            case '--config':
            case '-c':
                mergeInto(config, (0, utils_js_1.readJSONSync)(args[++i].replaceAll("\\", '/')));
                break;
            case '--print-config':
                printConfig = true;
                break;
            default:
                throw new Error(`Invalid option ${arg}`);
        }
    }
    if (!config.baseDir?.length && config.sources.length && config.sources[0].includes('/')) {
        let baseDir = config.sources[0].slice(0, config.sources[0].lastIndexOf("/") + 1);
        for (let i = 1; i < config.sources.length; i++) {
            const file = config.sources[i];
            while (!file.startsWith(baseDir)) {
                const pos = baseDir.lastIndexOf('/');
                baseDir = baseDir.slice(0, pos + 1);
            }
        }
        if (baseDir) {
            config.baseDir = [baseDir];
            const baseDirLen = baseDir.length;
            config.sources = config.sources.map(file => file.startsWith(baseDir) ? file.slice(baseDirLen) : file);
        }
    }
    if (!config.outputFile && typeof config.api == "string")
        config.outputFile = config.api;
    if (printConfig)
        (0, utils_js_1.writeJSONSync)(config.outputFile ? 1 : 2, config);
    const api = (0, parse_1.parseApi)(config);
    (0, utils_js_1.writeJSONSync)(config.outputFile || 1, api);
}
function mergeInto(destiny, source) {
    for (const [key, value] of Object.entries(source)) {
        const prevValue = destiny[key];
        if (Array.isArray(prevValue)) {
            if (Array.isArray(value)) {
                prevValue.push(...value);
            }
            else
                prevValue.push(value);
        }
        else if (typeof prevValue != "object") {
            if (value == null) {
                delete destiny[key];
            }
            else
                destiny[key] = value;
        }
        else if (!prevValue) {
            destiny[key] = value;
        }
        else {
            mergeInto(prevValue, value);
        }
    }
}
/**
 * Generate
 */
function generate(args) {
    if (args?.length == 0) {
        return help(["generate"]);
    }
    const config = {
        /** @type {string[]} */
        targets: [],
        /** @type {Api} */
        api: null,
        baseDir: "",
    };
    for (let i = 0; i < args.length; i++) {
        const arg = args[i];
        if (arg == "--") {
            config.targets.push(...args.slice(i + 1).map(arg => arg.replaceAll("\\", '/')));
            break;
        }
        if (!arg.startsWith('-')) {
            if (arg.endsWith(".json")) {
                mergeInto(config, (0, utils_js_1.readJSONSync)(arg.replaceAll("\\", '/')));
            }
            else
                config.targets.push(arg.replaceAll("\\", '/'));
            continue;
        }
        switch (arg) {
            case '-a':
                config.api = (0, utils_js_1.readJSONSync)(args[++i].replaceAll("\\", '/'));
                break;
            case '-d':
                config.baseDir = args[++i].replaceAll("\\", '/');
                break;
            case '-c':
                mergeInto(config, (0, utils_js_1.readJSONSync)(args[++i].replaceAll("\\", '/')));
                break;
            default:
                throw new Error(`Invalid option ${arg}`);
        }
    }
    if (!config.api)
        throw new Error("Missing target");
    if (typeof config.api == "string")
        config.api = (0, utils_js_1.readJSONSync)(config.api);
    const files = config.targets;
    delete config.targets;
    if (files?.length) {
        if (!config.baseDir && files[0].includes('/')) {
            config.baseDir = files[0].slice(0, files[0].lastIndexOf("/") + 1);
            for (let i = 1; i < files?.length; i++) {
                const file = files[i];
                while (!file.startsWith(config.baseDir)) {
                    const pos = config.baseDir.lastIndexOf('/');
                    config.baseDir = config.baseDir.slice(0, pos + 1);
                }
            }
            if (!config.baseDir) {
                throw new Error("Could not deduce baseDir");
            }
        }
        const baseDirLen = config.baseDir?.length ?? 0;
        const localFiles = new Set(files.map(file => file.startsWith(config.baseDir) ? file.slice(baseDirLen) : file));
        for (const file of Object.keys(config.api?.files ?? {})) {
            if (!localFiles.has(file))
                delete config.api.files[file];
        }
    }
    (0, generate_1.generateApi)(config);
}
/**
 * Transform files
 * @param args the arguments
 */
function transform(args) {
    if (args?.length == 0) {
        return help(["transform"]);
    }
    const config = {
        sourceApi: null,
        transform: null,
        api: "",
        baseDir: "",
        sources: [],
        storeLineNumbers: false,
    };
    let printConfig = false;
    for (let i = 0; i < args.length; i++) {
        const arg = args[i];
        if (arg == "--") {
            config.sources.push(...args.slice(i + 1).map(arg => arg.replaceAll("\\", '/')));
            break;
        }
        if (!arg.startsWith('-')) {
            if (arg.endsWith(".json")) {
                mergeTransformInto(config, (0, utils_js_1.readJSONSync)(arg.replaceAll("\\", '/')));
            }
            else
                config.sources.push(arg.replaceAll("\\", '/'));
            continue;
        }
        switch (arg) {
            case '--sourceApi':
            case '-s':
                config.sourceApi = (0, utils_js_1.readJSONSync)(args[++i].replaceAll("\\", '/'));
                break;
            case '--transform':
            case '-t':
                config.transform = (0, utils_js_1.readJSONSync)(args[++i].replaceAll("\\", '/'));
                break;
            case '--outputFile':
            case '-o':
                config.api = args[++i].replaceAll("\\", '/');
                break;
            case '--baseDir':
            case '-d':
                config.baseDir = args[++i].replaceAll("\\", '/');
                break;
            case '--config':
            case '-c':
                mergeTransformInto(config, (0, utils_js_1.readJSONSync)(args[++i].replaceAll("\\", '/')));
                break;
            case '--print-config':
            case '-p':
                printConfig = true;
                break;
            case '--store-line-numbers':
            case '-l':
                config.storeLineNumbers = true;
                break;
            default:
                throw new Error(`Invalid option ${arg}`);
        }
    }
    if (config.sources?.length) {
        const files = config.sources;
        if (!config.baseDir && files[0].includes('/')) {
            config.baseDir = files[0].slice(0, files[0].lastIndexOf("/") + 1);
            for (let i = 1; i < files?.length; i++) {
                const file = files[i];
                while (!file.startsWith(config.baseDir)) {
                    const pos = config.baseDir.lastIndexOf('/');
                    config.baseDir = config.baseDir.slice(0, pos + 1);
                }
            }
            if (!config.baseDir) {
                throw new Error("Could not deduce baseDir");
            }
        }
        const baseDirLen = config.baseDir?.length ?? 0;
        const localFiles = new Set(files.map(file => file.startsWith(config.baseDir) ? file.slice(baseDirLen) : file));
        for (const file of Object.keys(config.sourceApi?.files ?? {})) {
            if (!localFiles.has(file))
                delete config.sourceApi.files[file];
        }
    }
    if (printConfig) {
        (0, utils_js_1.writeJSONSync)(config.api ? 1 : 2, config);
    }
    const api = (0, transform_js_1.transformApi)(config);
    (0, utils_js_1.writeJSONSync)(config.api || 1, api);
}
function mergeTransformInto(destiny, source) {
    if (typeof source.sourceApi == "string") {
        source.sourceApi = (0, utils_js_1.readJSONSync)(source.sourceApi);
    }
    if (typeof source.transform == "string") {
        source.transform = (0, utils_js_1.readJSONSync)(source.transform);
    }
    mergeInto(destiny, source);
}
function help(args = []) {
    switch (args[0]) {
        case "parse":
            printLog("Parse API from header files", "", wrapUsageText(`usage: node ${process.argv[1]} `, "parse [ [-c] <config-file>] [-o <output-file>] [-d <base-dir>] [--] <input>..."), "", wrapText("If no base-dir is defined, we try to deduce the closest common ancestor from the inputs. If no output file is given or if it is a single dash (\" - \") it just outputs on stdout. If the first filename ends with \".json\" it interprets it as a config file, making the \" - c\" optional. Multiple configurations can be added and their content is merged."));
            break;
        default:
            printLog("Transform OO-like C APIs into actual C++ OO API\n", ...guideDoc);
            break;
    }
}
function wrapUsageText(prefix, parameters = "") {
    const wordRegex = /([^\[<\s]|\[[^\]]+\]|<[^>]+>)+/;
    let margin = prefix.length;
    if (margin > 40) {
        margin = 8;
    }
    return wrapText(prefix + parameters, { margin, wordRegex });
}
/**
 * Format text int 80 columns
 * @param text   the text to wrap
 * @param config
 */
function wrapText(text, config = {}) {
    const margin = config.margin ?? 0;
    const wordRegex = config.wordRegex ?? /[^\s]+/;
    const columns = config.columns ?? 80;
    const result = [];
    let currentLine = "";
    const marginText = " ".repeat(margin);
    while (text.length) {
        const m = text.match(wordRegex);
        if (!m)
            break;
        const word = m[0];
        const wordStart = m.index;
        const wordEnd = wordStart + word.length;
        if (currentLine.length + wordEnd < columns || !currentLine) {
            currentLine += " ".repeat(wordStart) + word;
        }
        else {
            result.push(currentLine);
            currentLine = marginText + word;
        }
        text = text.slice(wordEnd);
    }
    if (currentLine)
        result.push(currentLine);
    return result.join('\n');
}
/**
 * Print into error/log out
 *
 * @param message the message line
 * @param parameters optionally more message lines
 */
function printLog(message, ...parameters) {
    (0, utils_js_1.writeLinesSync)(2, [message, ...parameters]);
}
main(process.argv.slice(2));
