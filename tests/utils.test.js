const { combineArray, combineObject } = require("../src/utils");

test("combine into array", () => {
  expect(combineArray([1], [1])).toEqual([1]);
  expect(combineArray([1], [])).toEqual([]);
  expect(combineArray([1], [1, 2])).toEqual([1, 2]);
  expect(combineArray([{ a: 1 }], [{ b: 2 }])).toEqual([{ a: 1, b: 2 }]);
});

test("combine objects", () => {
  expect(combineObject({ a: 1 }, { b: 2 })).toEqual({ a: 1, b: 2 });
  expect(combineObject({ a: 1 }, { b: 2 })).not.toBe({ a: 1, b: 2 });
  expect(combineObject({ a: 1 }, undefined)).toEqual({ a: 1 });
  expect(combineObject({ a: 1 }, undefined)).not.toBe({ a: 1 });
  expect(combineObject({ a: { b: 1 } }, { a: { c: 2 } })).toEqual({ a: { b: 1, c: 2 } });
});
