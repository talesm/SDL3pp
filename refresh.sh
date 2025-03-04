#! /usr/bin/bash
node scripts/ parse scripts/config.json -d /usr/local/include/SDL3/
node scripts/ transform scripts/config.json
node scripts/ update scripts/config.json
