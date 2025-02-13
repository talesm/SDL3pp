const { transformApi } = require("../src/transform");
const emptyApi = require("./empty.json");

test("empty transform loopback", () => {
  const modifiedApi = transformApi({
    source: emptyApi,
    files: { "empty.h": true },
  });
  expect(modifiedApi).toEqual(emptyApi);
});
