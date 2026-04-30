import { ApiContext } from "./transform";
import {
  Dict,
  ApiEntry,
  ApiFileTransform,
  ApiEntryTransform,
  ParsedDoc,
} from "./types";
import { combineObject } from "./utils";

let lastFirstWord = "";

export function expandNamespaces(
  sourceEntries: Dict<ApiEntry>,
  file: ApiFileTransform,
  context: ApiContext,
) {
  const namespacesMap = file.namespacesMap ?? {};
  for (const [prefix, nsName] of Object.entries(namespacesMap)) {
    const nsEntries: Dict<ApiEntryTransform> = {};
    const sourceEntriesListed = Object.entries(sourceEntries).filter(
      ([key]) =>
        key.startsWith(prefix) && !file.transform[key]?.name?.includes("."),
    );
    for (const [key, entry] of sourceEntriesListed) {
      const entryDelta = file.transform[key] || {};
      const localName = entryDelta.name || entry.name.slice(prefix.length);
      file.transform[key] = entryDelta;
      entryDelta.name = nsName + "." + localName;

      fixDefToConstexpr(context, entry, entryDelta, localName);
      context.addName(key, `${nsName}.${localName}`);
    }
    if (sourceEntriesListed.length) {
      const nsEntry: ApiEntryTransform = {
        kind: "ns",
        name: nsName,
        ...file.transform[nsName],
      };
      if (nsEntry.entries) {
        combineObject(nsEntry.entries, nsEntries);
      } else {
        nsEntry.entries = nsEntries;
      }
      context.includeBefore(nsEntry, sourceEntriesListed[0][0]);
    }
  }
  lastFirstWord = "";
}

function fixDefToConstexpr(
  context: ApiContext,
  entry: ApiEntry,
  entryDelta: ApiEntryTransform,
  localName: string,
) {
  if (entry.kind === "def" && !entryDelta.kind) {
    entryDelta.kind = "var";
    entryDelta.type = "auto";
    entryDelta.constexpr = true;
    entryDelta.doc ??= entry.doc || makeDefDoc(context, entry.name, localName);
  }
}

function makeDefDoc(
  context: ApiContext,
  srcName: string,
  localName: string,
): ParsedDoc {
  const nameParts = srcName.split("_").slice(1);
  const localNameParts = localName.split("_");
  if (nameParts[0] === "PROP") {
    const isCreate = srcName.includes("CREATE") || srcName.includes("METADATA");
    const type = localNameParts.pop();
    const lowercaseParts = localNameParts.map((part) =>
      part.toLocaleLowerCase(),
    );
    switch (type) {
      case "BOOLEAN":
        if (isCreate) {
          lowercaseParts.unshift("enable");
        } else {
          lowercaseParts.push("enabled");
        }
        break;
      case "FLOAT":
        lowercaseParts.unshift("float for");
        break;
      case "NUMBER":
        if (lowercaseParts[0] === `n${lastFirstWord}`) {
          lowercaseParts.shift();
          lowercaseParts.unshift(`number of ${lastFirstWord}`);
        } else if (lowercaseParts.at(-1) !== "count") {
          lowercaseParts.unshift("number for");
        }
        break;
      case "POINTER":
        lowercaseParts.unshift("pointer to");
        break;
      case "STRING":
        lowercaseParts.unshift("string for");
        break;
      default:
        lowercaseParts.push(type.toLocaleLowerCase());
        break;
    }
    const lowercaseName = lowercaseParts.join(" ");
    lastFirstWord = localNameParts[0].toLowerCase();
    return [`${lowercaseName[0].toUpperCase()}${lowercaseName.slice(1)}.`];
  }
  return [localNameParts.join(" ")];
}
