# cppfier
turn c into cpp

This is made from three components:

1. `parse` that reads a C or C++ header and turns into a json. This doesn't need to be perfect, just good enough for transformations
2. `update` that applies the json into a C or C++ header, adding, changing or removing entries to make it fit there.
3. `transform` applies a series of transformations to turn C's json into C++'s json. This applies mostly by turning naturally OO-like constructs into C++ classes. This relies on a transformation file to guide the transformation.

## Parse

Command:

```
$ node cppfier parse [[-c] <config-file.json>] [-o <output-file>] [-d <base-dir>] [--] input1 input2 ... inputN
```

If no base-dir is defined, we try to deduce the closest common ancestor from the inputs. If no output file is given or if it is a single dash ("-") it just outputs on stdout. If the first filename ends with ".json" it interprets it as a config file, making the "-c" optional. Multiple configurations can be added and their content is merged.

The config file is of the following format:

```ts
interface ParseConfig {
  files?: string[];
  outputFile?: string;
  baseDir?: string;
  storeLineNumbers?: boolean;
};
```
