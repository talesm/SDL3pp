import { ApiEntryTransform, LockDefinition } from "./types";

export function getLockDefinition(
  targetEntry: ApiEntryTransform,
  sourceName: string
): LockDefinition {
  if (typeof targetEntry.lock === "object") {
    return {
      lockFunc: `SDL_Lock${sourceName}`,
      unlockFunc: `SDL_Unlock${sourceName}`,
      ...targetEntry.lock,
    };
  }
  if (!targetEntry.lock) return undefined;
  return {
    lockFunc: `SDL_Lock${sourceName}`,
    unlockFunc: `SDL_Unlock${sourceName}`,
  };
}
