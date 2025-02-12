const { parseApi } = require("../src/parse.js");
const emptyApi = require("./empty.json");
const functionsApi = require("./functions.json");

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
