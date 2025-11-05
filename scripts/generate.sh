#!/bin/bash
mkdir -p src/generated/
node scripts/cppfier generate scripts/config-target.json
scripts/applyFormat.sh
