#!/bin/bash
mkdir -p src/generated/
npm run generate
scripts/applyFormat.sh
