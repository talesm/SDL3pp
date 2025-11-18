#!/bin/bash
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
cd "$SCRIPT_DIR/cppfier"
npm run antlr
npm run compile
cd ../../
node scripts/cppfier parse scripts/config-source.json
