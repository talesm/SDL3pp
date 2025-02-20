const { tokenize } = require("../src/tokenize.js");

/** @param {string} text  */
function tokenizeText(text) {
  return tokenize((text.trimEnd() + "\n").split("\n"));
}

test("tokenize doc", () => {
  const tokens = tokenizeText("/**\n * text\n *\n*text\n*/\n//Forward decl");
  expect(tokens).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "doc",
    value: "text\n\ntext",
  }, {
    begin: 6,
    end: 7,
    spaces: 0,
    kind: "doc",
  }]);
});

test("tokenize function", () => {
  expect(tokenizeText("int func(int a, int b);")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "function",
    value: "func",
    type: "int",
    parameters: "int a, int b",
  }]);

  // with body
  expect(tokenizeText("int func(int a, int b)\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    value: "func",
    type: "int",
    parameters: "int a, int b",
  }]);

  // with empty body
  expect(tokenizeText("int func(int a, int b)\n{}")).toEqual([{
    begin: 1,
    end: 3,
    spaces: 0,
    kind: "function",
    value: "func",
    type: "int",
    parameters: "int a, int b",
  }]);

  // with body
  expect(tokenizeText("int func(int a, int b)\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    value: "func",
    type: "int",
    parameters: "int a, int b",
  }]);

  // ctor with body
  expect(tokenizeText("Func(int a, int b)\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    value: "Func",
    type: "",
    parameters: "int a, int b",
  }]);

  // operator() immutable with body
  expect(tokenizeText("void operator()(int a, int b) const\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    value: "operator()",
    type: "void",
    immutable: true,
    parameters: "int a, int b",
  }]);

  // conversion operator with body
  expect(tokenizeText("operator Type() const\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    value: "operator Type",
    type: "",
    immutable: true,
    parameters: "",
  }]);

  // conversion operator to const pointer with body
  expect(tokenizeText("operator const Type*() const\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    value: "operator const Type *",
    type: "",
    immutable: true,
    parameters: "",
  }]);

  // ctor with initialization and body
  expect(tokenizeText("Func(int a, int b)\n: a(a)\n  , b(b)\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 8,
    spaces: 0,
    kind: "function",
    value: "Func",
    type: "",
    parameters: "int a, int b",
  }]);
});

test("grab spaces between tokens", () => {
  expect(tokenizeText("int a;\nint b;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "var",
    value: "a",
    type: "int",
  }, {
    begin: 2,
    end: 3,
    spaces: 0,
    kind: "var",
    value: "b",
    type: "int",
  }]);
  expect(tokenizeText("int a;\n\n\nint b;")).toEqual([{
    begin: 1,
    end: 4,
    spaces: 0,
    kind: "var",
    value: "a",
    type: "int",
  }, {
    begin: 4,
    end: 5,
    spaces: 0,
    kind: "var",
    value: "b",
    type: "int",
  }]);
  expect(tokenizeText("int a;\n\n\n#include <blah>\n\nint b;")).toEqual([{
    begin: 1,
    end: 4,
    spaces: 0,
    kind: "var",
    value: "a",
    type: "int",
  }, {
    begin: 6,
    end: 7,
    spaces: 0,
    kind: "var",
    value: "b",
    type: "int",
  }]);
});
