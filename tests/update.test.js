const { updateContent, updateChanges } = require("../src/update.js");
const { parseContent } = require("../src/parse.js");
const { readLinesSync } = require("../src/utils.js");

test("updateChanges", () => {
  const baseline = ["A", "B", "C", "D"];
  const target1 = [...baseline];
  updateChanges(target1, [{ begin: 2, end: 4, replacement: "NEW!" }]);
  expect(target1).toEqual(["A", "NEW!", "D"]);
});

test("empty.h loopback", () => {
  const originalContent = readLinesSync("tests/samples/empty.h");
  const originalApi = parseContent("empty.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, originalApi);
  expect(modifiedContent).toEqual(originalContent);
  expect(changes).toBe(0);
});

test("vars.h loopback", () => {
  const originalContent = readLinesSync("tests/samples/vars.h");
  const originalApi = parseContent("vars.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, originalApi);
  expect(modifiedContent).toEqual(originalContent);
  expect(changes).toBe(0);
});

test("vars.h over empty file", () => {
  const originalContent = readLinesSync("tests/samples/vars.h");
  const originalApi = parseContent("vars.h", originalContent);
  const modifiedContent = readLinesSync("tests/samples/empty.h");
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("vars.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
});

test("functions.h loopback", () => {
  const originalContent = readLinesSync("tests/samples/functions.h");
  const originalApi = parseContent("functions.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, originalApi);
  expect(modifiedContent).toEqual(originalContent);
  expect(changes).toBe(0);
});

test("functions.h over empty file", () => {
  const originalContent = readLinesSync("tests/samples/functions.h");
  const originalApi = parseContent("functions.h", originalContent);
  const modifiedContent = readLinesSync("tests/samples/empty.h");
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("functions.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
});

test("structs.h loopback", () => {
  const originalContent = readLinesSync("tests/samples/structs.h");
  const originalApi = parseContent("structs.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, originalApi);
  expect(modifiedContent).toEqual(originalContent);
  expect(changes).toBe(0);
});

test("structs.h over empty file", () => {
  const originalContent = readLinesSync("tests/samples/structs.h");
  const originalApi = parseContent("structs.h", originalContent);
  const modifiedContent = readLinesSync("tests/samples/empty.h");
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
});

test("structs_aliases.h loopback", () => {
  const originalContent = readLinesSync("tests/samples/structs_aliases.h");
  const originalApi = parseContent("structs_aliases.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, originalApi);
  expect(modifiedContent).toEqual(originalContent);
  expect(changes).toBe(0);
});

test("structs_aliases.h over empty file", () => {
  const originalContent = readLinesSync("tests/samples/structs_aliases.h");
  const originalApi = parseContent("structs_aliases.h", originalContent);
  const modifiedContent = readLinesSync("tests/samples/empty.h");
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs_aliases.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
});

test("structs_extends.h loopback", () => {
  const originalContent = readLinesSync("tests/samples/structs_extends.h");
  const originalApi = parseContent("structs_extends.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, originalApi);
  expect(modifiedContent).toEqual(originalContent);
  expect(changes).toBe(0);
});

test("structs_extends.h over empty file", () => {
  const originalContent = readLinesSync("tests/samples/structs_extends.h");
  const originalApi = parseContent("structs_extends.h", originalContent);
  const modifiedContent = readLinesSync("tests/samples/empty.h");
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs_extends.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
});

test("structs_resources.h loopback", () => {
  const originalContent = readLinesSync("tests/samples/structs_resources.h");
  const originalApi = parseContent("structs_resources.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, originalApi);
  expect(modifiedContent).toEqual(originalContent);
  expect(changes).toBe(0);
});

test("structs_resources.h over empty file", () => {
  const originalContent = readLinesSync("tests/samples/structs_resources.h");
  const originalApi = parseContent("structs_resources.h", originalContent);
  const modifiedContent = readLinesSync("tests/samples/empty.h");
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs_resources.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
});

test("function is added correctly", () => {
  const originalContent = ["inline int a();", "", "inline int b(float b)", "{", '  static_assert(false, "Not implemented");', "}", ""];
  const originalApi = parseContent("functions.h", originalContent);
  const modifiedContent = ["inline int a();", ""];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0); const modifiedApi = parseContent("functions.h", reSplit(originalContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});

test("function template is added correctly", () => {
  const originalContent = ["template<class T>", "inline int a()", "{", '  static_assert(false, "Not implemented");', "}", ""];
  const originalApi = parseContent("functions.h", originalContent);
  const modifiedContent = ["inline int a();", ""];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0); const modifiedApi = parseContent("functions.h", reSplit(originalContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});

test("overload is added correctly", () => {
  const originalContent = ["inline int a();", "", "inline int a(float b)", "{", '  static_assert(false, "Not implemented");', "}", ""];
  const originalApi = parseContent("functions.h", originalContent);
  const modifiedContent = ["inline int a();", ""];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0); const modifiedApi = parseContent("functions.h", reSplit(originalContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});

test("overload is removed correctly from the end", () => {
  const originalContent = ["inline int a();", ""];
  const originalApi = parseContent("functions.h", originalContent);
  const modifiedContent = ["inline int a();", "inline int a(float b);", ""];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0); const modifiedApi = parseContent("functions.h", reSplit(originalContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});

test("struct with fields is ignored correctly", () => {
  const originalContent = ["struct S", "{", "  int a;", "};"];
  const originalApi = parseContent("structs.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).toBe(0);
});

test("struct with fields is created correctly", () => {
  const originalContent = ["struct S", "{", "  int a;", "", "};"];
  const originalApi = parseContent("structs.h", originalContent);
  const modifiedContent = [];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs.h", reSplit(originalContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});

test("struct's fields is added correctly", () => {
  const originalContent = ["struct S", "{", "  int a;", "", "};", ""];
  const originalApi = parseContent("structs.h", originalContent);
  const modifiedContent = ["struct S", "{", "};"];;
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});
test("struct's fields is added correctly on empty struct", () => {
  const originalContent = ["struct S", "{", "  int a;", "", "};", ""];
  const originalApi = parseContent("structs.h", originalContent);
  const modifiedContent = ["struct S", "{};", "", ""];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});
test("struct's fields is added correctly on non-empty struct", () => {
  const originalContent = ["struct S", "{", "", "  int a;", "", "  float b;", "};", ""];
  const originalApi = parseContent("structs.h", originalContent);
  const modifiedContent = ["struct S", "{", "  float b;", "};", ""];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});

test("struct's fields is replaced correctly on non-empty struct", () => {
  const originalContent = ["struct S", "{", "", "  float a;", "", "  float b;", "};", ""];
  const originalApi = parseContent("structs.h", originalContent);
  const modifiedContent = ["struct S", "{", "", "  int a;", "  float b;", "};", ""];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});

test("struct's template is added correctly", () => {
  const originalContent = ["template<class T>", "struct S", "{", "  int a;", "", "};", ""];
  const originalApi = parseContent("structs.h", originalContent);
  const modifiedContent = ["struct S", "{", "  int a;", "", "};", ""];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs.h", reSplit(modifiedContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});

test("using T::T ignored correctly", () => {
  const originalContent = ["using T::T;"];
  const originalApi = parseContent("structs_resources.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).toBe(0);
});

test("using T::T updates correctly", () => {
  const originalContent = ["using T::T;"];
  const originalApi = parseContent("structs_resources.h", originalContent);
  const modifiedContent = [];
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("structs_resources.h", reSplit(originalContent));
  expect(modifiedApi).toEqual(originalApi);
  expect(modifiedContent.join("\n").trim()).toEqual(originalContent.join("\n").trim());
});

/** @param {string[]} lines  */
function reSplit(lines) {
  return lines.join("\n").split("\n");
}
