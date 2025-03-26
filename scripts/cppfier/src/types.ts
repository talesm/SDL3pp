export type Dict<T> = { [file: string]: T };

export interface Api {
  files: Dict<ApiFile>;
}

export interface ApiFile {
  name: string;
  doc?: string;
  entries?: ApiEntries;
  docBegin?: number;
  docEnd?: number;
  entriesBegin?: number;
  entriesEnd?: number;
}

export type ApiEntryKind = "alias" | "callback" | "def" | "enum" | "forward" | "function" | "struct" | "union" | "var";

interface ApiEntryBase {
  name?: string;
  kind?: ApiEntryKind;
  doc?: string;
  type?: string;
  parameters?: ApiParameters;
  template?: ApiParameters;
  sourceName?: string;
  constexpr?: boolean;
  immutable?: boolean;
  reference?: number;
  static?: boolean;
}

export interface ApiEntry extends ApiEntryBase {
  name: string;
  kind: ApiEntryKind;
  begin?: number;
  decl?: number;
  end?: number;
  entries?: ApiEntries;
}

export type ApiEntries = Dict<ApiEntry | ApiEntry[]>;

export type ApiParameters = (string | ApiParameter)[];

export interface ApiParameter {
  name: string;
  type?: string;
  default?: string;
}

export type StringMap = Dict<string>;

export interface ApiTransform {
  files?: Dict<FileTransform>;
  prefixes?: string | string[];
  renameRules?: ReplacementRule[];
  docRules?: ReplacementRule[];
  typeMap?: StringMap;
  paramTypeMap?: StringMap;
  returnTypeMap?: StringMap;
}

export interface FileTransform {
  name?: string;
  doc?: string;
  includeDefs?: string[];
  ignoreEntries?: string[];
  includeAfter?: ApiEntryTransformMap;
  transform?: Dict<ApiEntryTransform>;
  resources?: Dict<ApiResource>;
}

export type ApiEntryTransformMap = Dict<ApiEntryTransform | ApiEntryTransform[]>;

export interface ApiEntryTransform extends ApiEntryBase {
  entries?: ApiSubEntryTransformMap;
}

export interface ApiResource extends ApiEntryTransform {
  kind?: "struct";

  /**
   * Name of free function. By default it uses the first subentry with
   * containing "Destroy", "Close" or "free" substring, in that order.
   */
  free?: string

  /**
   * The name of base resource class, defaults to _uniqueName_`Base`
   */
  name?: string;

  /**
   * The Reference name. Defaults to _uniqueName_`Ref`
   */
  refName?: string;

  /**
   * The Unique name. Defaults to the converted name from the original type
   */
  uniqueName?: string;

  /**
   * The wrapped type, defaults to the original type
   */
  type?: string;

  /**
   * The type of pointer. Defaults to _type_`*`
   */
  pointerType?: string;

  /**
   * If not false it prepends the aliases for refType and uniqueType at the beginning
   */
  prependAliases?: boolean;

  /**
   * The type to replace on parameters, defaults to "ref"
   * 
   */
  paramType?: "ref" | "unique" | "none";

  /**
   * The type to replace on return types, defaults to "ref"
   * 
   */
  returnType?: "ref" | "unique" | "none";
}

export type QuickTransform = "placeholder" | "immutable" | "ctor" | ApiEntryKind;

export type ApiSubEntryTransformMap = Dict<ApiEntryTransform | ApiEntry[] | QuickTransform>;

export interface ReplacementRule {
  pattern: RegExp;
  replacement: string;
}

export type FileTokenKind = ApiEntryKind | "doc" | "namespace" | "template" | "endStruct";

export interface FileToken {
  value: string;
  kind: FileTokenKind;
  parameters?: string;
  type?: string;
  constexpr?: boolean;
  immutable?: boolean;
  reference?: number;
  static?: boolean;
  begin: number;
  end: number;
  spaces: number;
  doc?: string;
}
