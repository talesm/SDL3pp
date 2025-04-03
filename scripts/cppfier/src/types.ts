export type Dict<T> = { [key: string]: T };

export interface Api {
  files: Dict<ApiFile>;
}

export interface ApiFile {
  name: string;
  doc?: string;
  entries?: ApiEntries;
  docBegin?: number;
  docEnd?: number;
  namespace?: string;
  entriesBegin?: number;
  entriesEnd?: number;
}

export type ApiEntryKind = "alias" | "callback" | "def" | "enum" | "forward" | "function" | "struct" | "union" | "var" | "ns";

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
  explicit?: boolean;
}

export interface ApiEntry extends ApiEntryBase {
  name: string;
  kind: ApiEntryKind;
  value?: string;
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
  files?: Dict<ApiFileTransform>;
  prefixes?: string | string[];
  definitionPrefix?: string;
  renameRules?: ReplacementRule[];
  docRules?: ReplacementRule[];
  typeMap?: StringMap;
  paramTypeMap?: StringMap;
  returnTypeMap?: StringMap;
}

export interface ApiFileTransform {
  name?: string;
  doc?: string;
  ignoreEntries?: string[];
  includeAfter?: ApiEntryTransformMap;
  transform?: Dict<ApiEntryTransform>;
  resources?: Dict<ApiResource>;
  enumerations?: Dict<ApiEnumeration>;
  wrappers?: Dict<ApiWrapper>;
  namespacesMap?: StringMap;
  definitionPrefix?: string;
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

export interface ApiWrapper extends ApiEntryTransform {
  kind?: "struct";

  includeAfter?: string;

  /** Defaults to `value` */
  attribute?: string;

  /** Defaults to true */
  invalidState?: boolean;

  /** Defaults to {} */
  defaultValue?: string

  /** Defaults to true if alias to pointer, false otherwise */
  nullable?: boolean

  /** 
   * Defaults to true if alias to anything but `void *`, false otherwise.
   */
  ordered?: boolean
}

export interface ApiEnumeration extends ApiEntryTransform {
  kind?: "struct" | "alias" | "enum";
  prefix?: string;
  newPrefix?: string;
  values?: string[];
  includeAfter?: string;
}

export type QuickTransform = "placeholder" | "immutable" | "ctor" | ApiEntryKind;

export type ApiSubEntryTransformMap = Dict<ApiEntryTransform | ApiEntry[] | QuickTransform>;

export interface ReplacementRule {
  pattern: RegExp;
  replacement: string;
}

export type FileTokenKind = ApiEntryKind | "doc" | "template" | "endStruct";

export interface FileToken {
  name?: string;
  value?: string;
  kind: FileTokenKind;
  parameters?: string;
  type?: string;
  constexpr?: boolean;
  immutable?: boolean;
  reference?: number;
  static?: boolean;
  explicit?: boolean;
  begin: number;
  end: number;
  spaces: number;
  doc?: string;
}
