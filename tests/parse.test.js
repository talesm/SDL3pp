const { parseApi } = require("../src/parse.js");
const emptyApi = require("./samples/empty.json");
const functionsApi = require("./samples/functions.json");
const structsApi = require("./samples/structs.json");
const structsAliasesApi = require("./samples/structs_aliases.json");
const structsExtendsApi = require("./samples/structs_extends.json");
const structsResourcesApi = require("./samples/structs_resources.json");

test("parse empty.h", () => {
  expect(parseApi({
    baseDir: "tests/samples/",
    sources: ["empty.h"]
  })).toEqual(emptyApi);
});

test("parse functions.h", () => {
  expect(parseApi({
    baseDir: "tests/samples/",
    sources: ["functions.h"]
  })).toEqual(functionsApi);
});

test("parse structs.h", () => {
  expect(parseApi({
    baseDir: "tests/samples/",
    sources: ["structs.h"]
  })).toEqual(structsApi);
});

test("parse structs_aliases.h", () => {
  expect(parseApi({
    baseDir: "tests/samples/",
    sources: ["structs_aliases.h"]
  })).toEqual(structsAliasesApi);
});

test("parse structs_extends.h", () => {
  expect(parseApi({
    baseDir: "tests/samples/",
    sources: ["structs_extends.h"]
  })).toEqual(structsExtendsApi);
});

test("parse structs_resources.h", () => {
  expect(parseApi({
    baseDir: "tests/samples/",
    sources: ["structs_resources.h"]
  })).toEqual(structsResourcesApi);
});
