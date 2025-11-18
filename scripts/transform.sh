#!/bin/bash
node scripts/gen-transform scripts/transform.json
node scripts/cppfier transform scripts/config-target.json
