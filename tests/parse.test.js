const { parseApi } = require("../src/parse.js");
const emptyApi = require("./empty.json");
const functionsApi = require("./functions.json");
const structsApi = require("./structs.json");
const structsAliasesApi = require("./structs_aliases.json");

test("parse empty.h", () => {
  expect(parseApi({
    baseDir: "tests/",
    files: ["empty.h"]
  })).toEqual(emptyApi);
});

test("parse functions.h", () => {
  expect(parseApi({
    baseDir: "tests/",
    files: ["functions.h"]
  })).toEqual(functionsApi);
});

test("parse structs.h", () => {
  expect(parseApi({
    baseDir: "tests/",
    files: ["structs.h"]
  })).toEqual(structsApi);
});

test("parse structs_aliases.h", () => {
  expect(parseApi({
    baseDir: "tests/",
    sources: ["structs_aliases.h"]
  })).toEqual(structsAliasesApi);
});
