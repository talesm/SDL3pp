#!/bin/bash

mkdir -p src/patches
mkdir -p build/patched
rm build/patched/*
mkdir -p include/SDL3pp
cp src/generated/*.h build/patched/
for file in build/patched/*; do
  if [ -f "$file" ]; then # Check if it's a regular file (not a directory)
    filename=$(basename "$file")
    patchFile="src/patches/$filename.patch"
    if [ -e $patchFile ]; then
      echo "patching $filename"
      patch --merge -f $file $patchFile #Apply patch
    else
      echo "no patches for $filename"
    fi
    finalFile=include/SDL3pp/$filename
    if ! diff -q $file $finalFile >/dev/null; then
      echo "Updating $filename"
      cp $file $finalFile
    fi
  fi
done
