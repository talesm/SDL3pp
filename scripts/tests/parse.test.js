const { parseApi, parseContent } = require("../cppfier/parse");
const emptyApi = require("./samples/empty.json");
const varsApi = require("./samples/vars.json");
const functionsApi = require("./samples/functions.json");
const structsApi = require("./samples/structs.json");
const structsAliasesApi = require("./samples/structs_aliases.json");
const structsExtendsApi = require("./samples/structs_extends.json");
const structsResourcesApi = require("./samples/structs_resources.json");
const enumsApi = require("./samples/enums.json");
const enumsAliasesApi = require("./samples/enums_aliases.json");

test("parse empty.h", () => {
  expect(parseApi({
    baseDir: ["scripts/tests/samples/"],
    sources: ["empty.h"]
  })).toEqual(emptyApi);
});

test("parse non-empty file", () => {
  const content = "\nint a;\n";
  expect(parseContent("file.h", content)).toEqual({
    name: "file.h",
    entries: {
      a: {
        kind: "var",
        name: "a",
        type: "int",
      },
    },
  });
});

test("parse vars.h", () => {
  expect(parseApi({
    baseDir: ["scripts/tests/samples/"],
    sources: ["vars.h"]
  })).toEqual(varsApi);
});

test("parse functions.h", () => {
  expect(parseApi({
    baseDir: ["scripts/tests/samples/"],
    sources: ["functions.h"]
  })).toEqual(functionsApi);
});

test("parse structs.h", () => {
  expect(parseApi({
    baseDir: ["scripts/tests/samples/"],
    sources: ["structs.h"]
  })).toEqual(structsApi);
});


test("parse enums.h", () => {
  expect(parseApi({
    baseDir: ["scripts/tests/samples/"],
    sources: ["enums.h"]
  })).toEqual(enumsApi);
});

