const { tokenize } = require("../src/tokenize.js");

/** @param {string} text  */
function tokenizeText(text) {
  return tokenize((text.trimEnd() + "\n#pragma region impl\n").split("\n"));
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
    value: "",
  }]);
});

test("tokenize alias", () => {
  expect(tokenizeText("using a = Templated<Ref<Value, OtherValue>>;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "alias",
    name: "a",
    type: "Templated<Ref<Value, OtherValue>>",
  }]);
});

test("tokenize alias defined next line", () => {
  expect(tokenizeText("using a =\n  Templated<Ref<Value, OtherValue>>;")).toEqual([{
    begin: 1,
    end: 3,
    spaces: 0,
    kind: "alias",
    name: "a",
    type: "Templated<Ref<Value, OtherValue>>",
  }]);
});
test("tokenize function", () => {
  expect(tokenizeText("int func(int a, int b);")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "function",
    name: "func",
    type: "int",
    parameters: "int a, int b",
  }]);
});

test("tokenize function with reference specifier", () => {
  expect(tokenizeText("int func(int a, int b) &;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "function",
    name: "func",
    type: "int",
    parameters: "int a, int b",
    reference: 1
  }]);
});
test("tokenize function with reference specifier", () => {
  expect(tokenizeText("int func(int a, int b) &&;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "function",
    name: "func",
    type: "int",
    parameters: "int a, int b",
    reference: 2
  }]);
});

test("tokenize function with default parameter", () => {
  expect(tokenizeText("int func(int a, int b = 12);")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "function",
    name: "func",
    type: "int",
    parameters: "int a, int b = 12",
  }]);
});

test("tokenize function returning a pointer", () => {
  expect(tokenizeText("const char *func(int a, int b);")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "function",
    name: "func",
    type: "const char *",
    parameters: "int a, int b",
  }]);
});

test("tokenize function with body", () => {
  expect(tokenizeText("int func(int a, int b)\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    name: "func",
    type: "int",
    parameters: "int a, int b",
  }]);
});

test("tokenize function with empty body", () => {
  expect(tokenizeText("int func(int a, int b)\n{}")).toEqual([{
    begin: 1,
    end: 3,
    spaces: 0,
    kind: "function",
    name: "func",
    type: "int",
    parameters: "int a, int b",
  }]);
});

test("tokenize function with body", () => {
  expect(tokenizeText("int func(int a, int b)\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    name: "func",
    type: "int",
    parameters: "int a, int b",
  }]);
});

test("tokenize function with templated return type", () => {
  expect(tokenizeText("std::optional<int> func(int a, int b) const\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    name: "func",
    type: "std::optional<int>",
    immutable: true,
    parameters: "int a, int b",
  }]);
});

test("tokenize function ctor with body", () => {
  expect(tokenizeText("Func(int a, int b)\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    name: "Func",
    type: "",
    parameters: "int a, int b",
  }]);
});

test("tokenize function operator() immutable with body", () => {
  expect(tokenizeText("void operator()(int a, int b) const\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    name: "operator()",
    type: "void",
    immutable: true,
    parameters: "int a, int b",
  }]);
});

test("tokenize function conversion operator with body", () => {
  expect(tokenizeText("operator Type() const\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    name: "operator Type",
    type: "",
    immutable: true,
    parameters: "",
  }]);
});

test("tokenize function conversion operator to const pointer with body", () => {
  expect(tokenizeText("operator const Type*() const = default;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "function",
    name: "operator const Type *",
    type: "",
    immutable: true,
    parameters: "",
  }]);
});

test("tokenize function operator<=> immutable with body", () => {
  expect(tokenizeText("auto operator<=>(const Type &other) const\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 6,
    spaces: 0,
    kind: "function",
    name: "operator<=>",
    type: "auto",
    immutable: true,
    parameters: "const Type &other",
  }]);
});

test("tokenize function ctor with initialization and body", () => {
  expect(tokenizeText("Func(int a, int b)\n  : a(a)\n  , b(b)\n{\n  int a;\n  ignored stuff\n}")).toEqual([{
    begin: 1,
    end: 8,
    spaces: 0,
    kind: "function",
    name: "Func",
    type: "",
    parameters: "int a, int b",
  }]);
});

test("tokenize var", () => {
  expect(tokenizeText("int aVar;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "var",
    name: "aVar",
    type: "int",
  }]);
});
test("tokenize var with assignment", () => {
  expect(tokenizeText("int aVar = 123;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "var",
    name: "aVar",
    type: "int",
  }]);
});
test("tokenize var with assignment in next line", () => {
  expect(tokenizeText("int aVar =\n   123;")).toEqual([{
    begin: 1,
    end: 3,
    spaces: 0,
    kind: "var",
    name: "aVar",
    type: "int",
  }]);
});

test("tokenize variables", () => {
  expect(tokenizeText("int a;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "var",
    name: "a",
    type: "int",
  }]);
  expect(tokenizeText("int *a;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "var",
    name: "a",
    type: "int *",
  }]);
  expect(tokenizeText("const int **a;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "var",
    name: "a",
    type: "const int **",
  }]);
});

test("tokenize struct", () => {
  expect(tokenizeText("struct Test{\n};")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "struct",
    name: "Test",
  }, {
    begin: 2,
    end: 3,
    spaces: 0,
    kind: "endStruct",
  },]);
  expect(tokenizeText("struct Test\n{\n};")).toEqual([{
    begin: 1,
    end: 3,
    spaces: 0,
    kind: "struct",
    name: "Test",
  }, {
    begin: 3,
    end: 4,
    spaces: 0,
    kind: "endStruct",
  },]);
});

test("tokenize struct with base type", () => {
  expect(tokenizeText("struct Test : TestBase {\n};")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "struct",
    name: "Test",
    type: "TestBase",
  }, {
    begin: 2,
    end: 3,
    spaces: 0,
    kind: "endStruct",
  },]);
  expect(tokenizeText("struct Test\n  : TestBase \n{\n};")).toEqual([{
    begin: 1,
    end: 4,
    spaces: 0,
    kind: "struct",
    name: "Test",
    type: "TestBase",
  }, {
    begin: 4,
    end: 5,
    spaces: 0,
    kind: "endStruct",
  },]);
});

test("grab spaces between tokens", () => {
  expect(tokenizeText("int a;\nint b;")).toEqual([{
    begin: 1,
    end: 2,
    spaces: 0,
    kind: "var",
    name: "a",
    type: "int",
  }, {
    begin: 2,
    end: 3,
    spaces: 0,
    kind: "var",
    name: "b",
    type: "int",
  }]);
  expect(tokenizeText("int a;\n\n\nint b;")).toEqual([{
    begin: 1,
    end: 4,
    spaces: 0,
    kind: "var",
    name: "a",
    type: "int",
  }, {
    begin: 4,
    end: 5,
    spaces: 0,
    kind: "var",
    name: "b",
    type: "int",
  }]);
  expect(tokenizeText("int a;\n\n\n#include <blah>\n\nint b;")).toEqual([{
    begin: 1,
    end: 4,
    spaces: 0,
    kind: "var",
    name: "a",
    type: "int",
  }, {
    begin: 6,
    end: 7,
    spaces: 0,
    kind: "var",
    name: "b",
    type: "int",
  }]);
});
