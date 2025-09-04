#!/bin/bash

mkdir -p src/patches
for file in src/templates/*; do
  if [ -f "$file" ]; then # Check if it's a regular file (not a directory)
    filename=$(basename "$file")
    targetFile="include/SDL3pp/$filename"
    patchFile="src/patches/$filename.patch"
    if [ -e $patchFile ]; then
      echo "patching $filename"
      patch -f $targetFile $patchFile #Apply patch
    else
      echo "no patches for $filename"
    fi
  fi
done
