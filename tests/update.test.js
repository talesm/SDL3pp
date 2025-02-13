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
  const originalContent = readLinesSync("tests/empty.h");
  const api = parseContent("empty.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, api);
  expect(modifiedContent).toEqual(originalContent);
  expect(changes).toBe(0);
});

test("functions.h loopback", () => {
  const originalContent = readLinesSync("tests/functions.h");
  const api = parseContent("functions.h", originalContent);
  const modifiedContent = [...originalContent];
  const changes = updateContent(modifiedContent, api);
  expect(modifiedContent).toEqual(originalContent);
  expect(changes).toBe(0);
});

test("functions.h over empty file", () => {
  const originalContent = readLinesSync("tests/functions.h");
  const originalApi = parseContent("functions.h", originalContent);
  const modifiedContent = readLinesSync("tests/empty.h");
  const changes = updateContent(modifiedContent, originalApi);
  expect(changes).not.toBe(0);
  const modifiedApi = parseContent("functions.h", originalContent);
  expect(modifiedApi).toEqual(originalApi);
});
