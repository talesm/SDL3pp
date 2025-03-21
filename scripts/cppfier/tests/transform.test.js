const { transformApi } = require("../src/transform");
const emptyApi = require("./samples/empty.json");
const functionsApi = require("./samples/functions.json");
const structsApi = require("./samples/structs.json");
const structsAliasesApi = require("./samples/structs_aliases.json");
const structsExtendsApi = require("./samples/structs_extends.json");
const structsResourcesApi = require("./samples/structs_resources.json");
const enumsApi = require("./samples/enums.json");
const enumsAliasesApi = require("./samples/enums_aliases.json");

test("empty transform loopback", () => {
  const modifiedApi = transformApi({
    sourceApi: emptyApi,
    transform: {},
  });
  expect(modifiedApi).toEqual(emptyApi);
});

test("functions transform loopback", () => {
  const modifiedApi = transformApi({
    sourceApi: /** @type {Api} */(functionsApi),
    transform: {},
  });
  expect(modifiedApi).toMatchObject(functionsApi);
});

test("empty to functions transform", () => {
  const functionsFile = functionsApi.files["functions.h"];
  const newFunctions = Object.values(functionsFile.entries).reduce((/** @type {ApiEntry[]} */ acc, entry) => {
    if (Array.isArray(entry))
      acc.push(.../** @type {ApiEntry[]} */(entry));
    else acc.push(/** @type {ApiEntry} */(entry));
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
    sourceApi: /** @type {Api} */(structsApi),
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
    sourceApi: /** @type {Api} */(structsApi),
    transform: {
      prefixes: "My",
      renameRules: [
        {
          pattern: /^([^.]*)/,
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
    sourceApi: /** @type {Api} */(structsApi),
    transform: {
      prefixes: "My",
      renameRules: [
        {
          pattern: /^([^.]*)/,
          replacement: "$1_resources",
        }
      ],
      files: {
        "structs.h": {
          transform: {
            MyType1: {
              name: "Type1Base",
              type: "T",
              kind: "struct",
              template: [{ type: "class", name: "T" }],
              entries: {
                "T::T": { kind: "alias", name: "T::T" },
                MyCreateType1: "ctor",
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
  expect(modifiedApi).toMatchObject(structsResourcesApi);
});

test("enum transform basic", () => {
  const modifiedApi = transformApi({
    sourceApi: /** @type {Api} */(enumsApi),
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
  expect(modifiedApi).toMatchObject(enumsAliasesApi);
});
