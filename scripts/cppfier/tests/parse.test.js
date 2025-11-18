const { parseApi, parseContent } = require("../parse");
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
    baseDir: ["scripts/cppfier/tests/samples/"],
    sources: ["empty.h"]
  })).toEqual(emptyApi);
});

test("parse vars.h", () => {
  expect(parseApi({
    baseDir: ["scripts/cppfier/tests/samples/"],
    sources: ["vars.h"]
  })).toEqual(varsApi);
});

test("parse functions.h", () => {
  expect(parseApi({
    baseDir: ["scripts/cppfier/tests/samples/"],
    sources: ["functions.h"]
  })).toEqual(functionsApi);
});

test("parse structs.h", () => {
  expect(parseApi({
    baseDir: ["scripts/cppfier/tests/samples/"],
    sources: ["structs.h"]
  })).toEqual(structsApi);
});

test("parse structs_aliases.h", () => {
  expect(parseApi({
    baseDir: ["scripts/cppfier/tests/samples/"],
    sources: ["structs_aliases.h"]
  })).toEqual(structsAliasesApi);
});

test("parse structs_extends.h", () => {
  expect(parseApi({
    baseDir: ["scripts/cppfier/tests/samples/"],
    sources: ["structs_extends.h"]
  })).toEqual(structsExtendsApi);
});

test("parse structs_resources.h", () => {
  expect(parseApi({
    baseDir: ["scripts/cppfier/tests/samples/"],
    sources: ["structs_resources.h"]
  })).toEqual(structsResourcesApi);
});

test("parse ends on trailing }", () => {
  const content = ["namespace {", "", "int a;", "}", "", "int b;", ""];
  expect(parseContent("file.h", content, { storeLineNumbers: true })).toEqual({
    name: "file.h",
    doc: "",
    entries: {
      a: {
        doc: "",
        kind: "var",
        name: "a",
        type: "int",
        begin: 3,
        decl: 3,
        end: 4
      },
    },
    docBegin: 1,
    docEnd: 1,
    entriesBegin: 3,
    entriesEnd: 4,
  });
});

test("parse enums.h", () => {
  expect(parseApi({
    baseDir: ["scripts/cppfier/tests/samples/"],
    sources: ["enums.h"]
  })).toEqual(enumsApi);
});

test("parse enums_aliases.h", () => {
  expect(parseApi({
    baseDir: ["scripts/cppfier/tests/samples/"],
    sources: ["enums_aliases.h"]
  })).toEqual(enumsAliasesApi);
});
