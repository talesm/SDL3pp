#!/bin/bash

mkdir -p src/generated
mkdir -p src/patches
mkdir -p include/SDL3pp/
cmake --build  build/ -t SDL3pp_generate
for file in src/generated/*.h; do
  if [ -f "$file" ]; then # Check if it's a regular file (not a directory)
    filename=$(basename "$file")
    echo "updating patch $filename"
    clang-format -i $file
    diff "$file" "include/SDL3pp/$filename" > "src/patches/$filename.patch"
  fi
done
