#!/bin/bash
node scripts/gen-transform scripts/target.json
node scripts/cppfier transform scripts/config-target.json
