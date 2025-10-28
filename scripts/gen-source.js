const { writeJSONSync, combineObject } = require("./cppfier/src/utils");
const { parseApi } = require("./cppfier/src/parse");
const sourceConfig = require("./config-source.json");
const sourceXml = require("./source_xml.json");

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
