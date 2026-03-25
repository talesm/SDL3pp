#!/bin/bash
mkdir -p build/generated/
npm run generate
scripts/applyFormat.sh
