#!/bin/bash

mkdir -p src/patches
for file in include/SDL3pp/*; do
  if [ -f "$file" ]; then # Check if it's a regular file (not a directory)
    filename=$(basename "$file")
    patchFile="src/patches/$filename.patch"
    if [ -e $patchFile ]; then
      echo "patching $filename"
      patch -f $file $patchFile #Apply patch
    else
      echo "no patches for $filename"
    fi
  fi
done
