const process = require('node:process');
const { spawnSync } = require("node:child_process");
const config = require("./scripts/config.json");

/**
 * Process the main
 * @param {string[]} args the arguments
 */
async function main(args) {
  process.chdir(__dirname);
  var { status } = spawnSync("node", ["scripts/", "parse", "scripts/config.json", "-d", "/usr/local/include/SDL3/"], { stdio: 'inherit' });
  if (status === null) process.exit(1);
  var { status } = spawnSync("node", ["scripts/", "transform", "scripts/config.json"], { stdio: 'inherit' });
  if (status === null) process.exit(2);

  const targets = [];
  if (args?.length) {
    const filter = new Set(args.map(getRadix));
    targets.push(...config.sources.map(getRadix).filter(s => !filter.has(s)).map(s => `SDL3pp_${s}.h`));
  }
  var { status } = spawnSync("node", ["scripts/", "update", "scripts/config.json", ...targets], { stdio: 'inherit' });
  if (status === null) process.exit(3);
}

/** @param {string} name  */
function getRadix(name) {
  return /^(?:SDL(?:3pp)?_)?([^.]+)(?:\.h)?$/.exec(name)?.[1] ?? "";
}

main(process.argv.slice(2));
