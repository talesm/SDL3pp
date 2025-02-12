const { updateChanges } = require("../src/update");

test("updateChanges", () => {
  const baseline = ["A", "B", "C", "D"];
  const target1 = [...baseline];
  updateChanges(target1, [{ begin: 2, end: 4, replacement: "NEW!" }]);
  expect(target1).toEqual(["A", "NEW!", "D"]);
});
