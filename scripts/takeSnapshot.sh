#!/bin/bash

mkdir -p build/snapshot/
cp include/SDL3pp/* build/snapshot/
mkdir -p src/patches
cmake --build  build/ -t SDL3pp_generate
for file in include/SDL3pp/*; do
  if [ -f "$file" ]; then # Check if it's a regular file (not a directory)
    filename=$(basename "$file")
    echo "updating patch $filename"
    clang-format -i $file
    diff -u "$file" "build/snapshot/$filename" > "src/patches/$filename.patch"
  fi
done
cmake --build  build/ -t SDL3pp_refresh_new
