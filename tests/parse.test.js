const { parseApi } = require("../src/parse-api.js");
const functionsApi = require("./functions.json");

test("parse functions.h", () => {
  expect(parseApi({
    baseDir: "tests/",
    files: ["functions.h"]
  })).toEqual(functionsApi);
});
