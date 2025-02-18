const { transformApi } = require("../src/transform");
const emptyApi = require("./empty.json");
const functionsApi = require("./functions.json");
const structsApi = require("./structs.json");
const structsAliasesApi = require("./structs_aliases.json");
const structsExtendsApi = require("./structs_extends.json");
const structsResourcesApi = require("./structs_resources.json");

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

test("structs transform extends", () => {
  const modifiedApi = transformApi({
    sourceApi: structsApi,
    transform: {
      prefixes: "My",
      renameRules: [
        {
          pattern: "^([^.]*)",
          replacement: "$1_extends",
        }
      ],
      files: {
        "structs.h": {
          transform: {
            MyType1: {
              type: "MyType1",
              kind: "struct",
              entries: {
                MyType1Check: "function",
                MyGetType1Var1: "function",
                MySetType1Var1: "function",
                MyType1GetVar2: "function",
                MyType1SetVar2: "function",
              },
            },
            MyType2: {
              type: "MyType2",
              kind: "struct",
              entries: {},
            },
          },
        },
      },
    },
  });
  expect(modifiedApi).toMatchObject(structsExtendsApi);
});

test("structs transform resources", () => {
  const modifiedApi = transformApi({
    sourceApi: structsApi,
    transform: {
      prefixes: "My",
      renameRules: [
        {
          pattern: "^([^.]*)",
          replacement: "$1_extends",
        }
      ],
      files: {
        "structs.h": {
          transform: {
            MyType1: {
              type: "MyType1",
              kind: "struct",
              entries: {
                MyType1Check: "function",
                MyGetType1Var1: "function",
                MySetType1Var1: "function",
                MyType1GetVar2: "function",
                MyType1SetVar2: "function",
              },
            },
            MyType2: {
              type: "MyType2",
              kind: "struct",
              entries: {},
            },
          },
        },
      },
    },
  });
  expect(modifiedApi).toMatchObject(structsExtendsApi);
});
