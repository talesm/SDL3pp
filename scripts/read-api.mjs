import { readFileSync } from "node:fs";

const baseDir = '/usr/local/include/SDL3/'
const filename = 'SDL_stdinc.h'

console.log(`Reading file ${filename}`)

const content = readFileSync(baseDir + filename, 'utf-8')

console.log(JSON.stringify(parseContent(filename, content), null, 2));

/**
 * 
 * @param {string} name 
 * @param {string} content 
 */
function parseContent(name, content) {
  // Get file doc
  const docStart = content.indexOf('/**') + 4;
  const docEnd = content.indexOf(' */', docStart);
  const pos = docEnd + 3;


  return {
    name,
    doc: parseDoc(content.slice(docStart, docEnd)),
    entries: parseEntries(content.slice(pos).split('\n')),
  }
}

/**
 * 
 * @param {string} content 
 */
function parseDoc(content) {
  return content.replace(/^ \*\s?/gm, '')
}

/**
 * 
 * @param {string[]} content 
 */
function parseEntries(content) {
  const entries = {};
  /** @type {RegExpExecArray} */
  let m = null;
  for (let i = 0; i < content.length; i++) {
    const line = content[i]
    let entry = {}
    if (m = /^#define (SDL_\w+)(\([\w ,]*\))/.exec(line)) {
      entry = {
        name: m[1],
        kind: 'macro',
      }
      if (m[2]) {
        entry.parameters = m[2].slice(1, m[2].length - 1).split(',').map(p => p.trim())
      }
    } else if (m = /^extern (SDL_\w+\s+)*(.*)SDLCALL (SDL_\w+)\(/.exec(line)) {
      entry = {
        name: m[3],
        kind: 'function',
      }
      if (entries[entry.name]?.kind == 'macro') {
        entries[entry.name] = entry
      }
    }

    if (entry.name && !entries[entry.name]) {
      entries[entry.name] = entry
    }
  }
  return entries
}
