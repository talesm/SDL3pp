export { _transformApi as transformApi };
export type TransformConfig = {
    sourceApi: Api;
    transform?: ApiTransform | undefined;
};
/**
 * @import { Api, ApiEntries, ApiEntry, ApiEntryKind, ApiEntryTransform, ApiFile, ApiParameters, ApiTransform, Dict, ApiFileTransform, ReplacementRule, StringMap, ApiParameter, ApiType, VersionTag, ApiEntryBase, EntryHint, QuickTransform, ApiEntryTransformMap } from "./types"
 */
/**
 * @typedef {object} TransformConfig
 * @prop {Api}            sourceApi
 * @prop {ApiTransform=}  transform
 */
/**
 * Transform the Api
 *
 * @param {TransformConfig} config the rules to apply into source
 */
declare function _transformApi(config: TransformConfig): Api;
import type { Api } from "./types";
import type { ApiTransform } from "./types";
