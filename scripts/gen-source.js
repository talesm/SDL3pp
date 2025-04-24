const { writeJSONSync, combineObject } = require("./cppfier/src/utils");
const { parseApi } = require("./cppfier/src/parse");
const sourceConfig = require("./config-source.json");
const sourceXml = require("./source_xml.json");

const api = parseApi({
  ...sourceConfig,
  sources: ["SDL_system.h"]
});
for (const entry of Object.values(api.files["SDL_system.h"].entries)) {
  if (Array.isArray(entry)) continue;
  delete entry.proto;
}

writeJSONSync("scripts/source.json", combineObject(sourceXml, api));
