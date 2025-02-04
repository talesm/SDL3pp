const { readFileSync, writeFileSync } = require("node:fs");
const { parseApi } = require("./read-api.js")
const targetConfig = require('./target.json');

const baseDir = 'include/SDL3pp/'
const filename = 'stdinc.hpp'


if (require.main == module) {
  updateHeaders([filename])
}
