const { transformApi } = require("../src/transform");
const emptyApi = require("./empty.json");
const functionsApi = require("./functions.json");

test("empty transform loopback", () => {
  const modifiedApi = transformApi({
    source: emptyApi,
    files: { "empty.h": true },
  });
  expect(modifiedApi).toEqual(emptyApi);
});

test("functions transform loopback", () => {
  const modifiedApi = transformApi({
    source: functionsApi,
    files: { "functions.h": true },
  });
  expect(modifiedApi).toMatchObject(functionsApi);
});
