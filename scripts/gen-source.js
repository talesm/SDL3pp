import { writeJSONSync, system } from "./cppfier/dist/utils.js";
import sourceConfig2 from "./config-source2.json" with {type: "json"};
import { parseApi } from "./cppfier/dist/parse-grammar.js";

system.silent = false;

const newApi = parseApi(sourceConfig2);
writeJSONSync("scripts/source.json", newApi);
