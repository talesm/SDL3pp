import { writeJSONSync, combineObject } from "./cppfier/dist/utils.js";
import { parseApi } from "./cppfier/dist/parse.js";
import sourceConfig from "./config-source.json" with {type: "json"};
import sourceConfig2 from "./config-source2.json" with {type: "json"};
import sourceXml from "./source_xml.json" with {type: "json"};
import { parseApi as parseApiNew } from "./cppfier/dist/parse-grammar.js";

const api = parseApi({
  ...sourceConfig,
  sources: ["SDL_system.h", "SDL_hidapi.h", "SDL_pixels.h", "SDL_messagebox.h"]
});
for (const [key, file] of Object.entries(api.files)) {
  for (const entry of Object.values(file.entries)) {
    if (Array.isArray(entry)) continue;
    delete entry.proto;
  }
  combineObject(file, sourceXml.files[key]);
  sourceXml.files[key] = file;
}

const newApi = parseApiNew(sourceConfig2);
for (const [key, file] of Object.entries(newApi.files)) {
  sourceXml.files[key] = file;
}

writeJSONSync("scripts/source.json", sourceXml);
