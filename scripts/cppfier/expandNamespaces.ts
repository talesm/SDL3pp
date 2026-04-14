import { ApiContext } from "./transform";
import { Dict, ApiEntry, ApiFileTransform, ApiEntryTransform } from "./types";
import { combineObject } from "./utils";

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
      const localName = entry.name.slice(prefix.length);
      file.transform[key] = entryDelta;
      entryDelta.name = nsName + "." + localName;

      fixDefToConstexpr(entry, entryDelta);
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
}

function fixDefToConstexpr(entry: ApiEntry, entryDelta: ApiEntryTransform) {
  if (entry.kind === "def" && !entryDelta.kind) {
    entryDelta.kind = "var";
    entryDelta.type = "auto";
    entryDelta.constexpr = true;
  }
}
