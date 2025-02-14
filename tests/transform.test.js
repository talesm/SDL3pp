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

test("empty to functions transform", () => {
  const functionsFile = functionsApi.files["functions.h"];
  const newFunctions = Object.values(functionsFile.entries).reduce((acc, entry) => {
    if (Array.isArray(entry))
      acc.push(...entry);
    else acc.push(entry);
    return acc;
  }, []);
  const modifiedApi = transformApi({
    source: emptyApi,
    files: {
      "empty.h": {
        name: "functions.h",
        doc: functionsFile.doc,
        includeAfter: {
          __begin: newFunctions
        }
      }
    },
  });
  expect(modifiedApi).toMatchObject(functionsApi);
});
