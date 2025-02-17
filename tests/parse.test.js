const { parseApi } = require("../src/parse.js");
const emptyApi = require("./empty.json");
const functionsApi = require("./functions.json");
const structsApi = require("./structs.json");
const structsAliasesApi = require("./structs_aliases.json");

test("parse empty.h", () => {
  expect(parseApi({
    baseDir: "tests/",
    sources: ["empty.h"]
  })).toEqual(emptyApi);
});

test("parse functions.h", () => {
  expect(parseApi({
    baseDir: "tests/",
    sources: ["functions.h"]
  })).toEqual(functionsApi);
});

test("parse structs.h", () => {
  expect(parseApi({
    baseDir: "tests/",
    sources: ["structs.h"]
  })).toEqual(structsApi);
});

test("parse structs_aliases.h", () => {
  expect(parseApi({
    baseDir: "tests/",
    sources: ["structs_aliases.h"]
  })).toMatchObject(structsAliasesApi);
});
