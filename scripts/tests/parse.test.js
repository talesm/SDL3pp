const { parseApi, parseContent } = require("../cppfier/parse");
const emptyApi = require("./samples/empty.json");
const functionsApi = require("./samples/functions.json");
const structsApi = require("./samples/structs.json");
const enumsApi = require("./samples/enums.json");

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

test("parse functions.h", () => {
  expect(parseApi({
    baseDir: ["scripts/tests/samples/"],
    sources: ["functions.h"]
  })).toEqual(functionsApi);
});

test.failing("parse structs.h", () => {
  expect(parseApi({
    baseDir: ["scripts/tests/samples/"],
    sources: ["structs.h"]
  })).toEqual(structsApi);
});


test.failing("parse enums.h", () => {
  expect(parseApi({
    baseDir: ["scripts/tests/samples/"],
    sources: ["enums.h"]
  })).toEqual(enumsApi);
});

