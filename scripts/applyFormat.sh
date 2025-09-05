#!/bin/bash

mkdir -p src/patches
for file in include/SDL3pp/*; do
  if [ -f "$file" ]; then # Check if it's a regular file (not a directory)
    filename=$(basename "$file")
    echo "formatting $filename"
    clang-format -i $file
  fi
done
