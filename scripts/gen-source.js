import { writeJSONSync, combineObject } from "./cppfier/dist/utils.js";
import { parseApi } from "./cppfier/dist/parse.js";
import sourceConfig from "./config-source.json" with {type: "json"};
import sourceXml from "./source_xml.json" with {type: "json"};

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

writeJSONSync("scripts/source.json", sourceXml);
