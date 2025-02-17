const { transformApi } = require("../src/transform");
const emptyApi = require("./empty.json");
const functionsApi = require("./functions.json");
const structsApi = require("./structs.json");
const structsAliasesApi = require("./structs_aliases.json");

test("empty transform loopback", () => {
  const modifiedApi = transformApi({
    sourceApi: emptyApi,
    transform: {},
  });
  expect(modifiedApi).toEqual(emptyApi);
});

test("functions transform loopback", () => {
  const modifiedApi = transformApi({
    sourceApi: functionsApi,
    transform: {},
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
    sourceApi: emptyApi,
    transform: {
      files: {
        "empty.h": {
          name: "functions.h",
          doc: functionsFile.doc,
          includeAfter: {
            __begin: newFunctions
          }
        }
      },
    },
  });
  expect(modifiedApi).toMatchObject(functionsApi);
});

test("structs transform basic", () => {
  const modifiedApi = transformApi({
    sourceApi: structsApi,
    transform: {
      prefixes: "My",
      renameRules: [
        {
          pattern: /^([^.]*)/,
          replacement: "$1_aliases"
        }
      ]
    },
  });
  expect(modifiedApi).toMatchObject(structsAliasesApi);
});
