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
  includes?: string[];
  localIncludes?: string[];
}

export type ApiEntryKind = "alias" | "callback" | "def" | "enum" | "forward" | "function" | "struct" | "union" | "var" | "ns" | 'plc';

export interface ApiEntryBase {
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
  proto?: boolean
  static?: boolean;
  since?: VersionTag;
  explicit?: boolean;
  hints?: EntryHint;
  forceUpdate?: true;
}

export interface EntryHint {
  init?: string[];
  delete?: boolean;
  default?: boolean;
  body?: string;
  self?: string;
  super?: string;
  static?: boolean;
  mayFail?: boolean;
  removeParamThis?: boolean;
  private?: boolean;
  wrapSelf?: boolean;
  changeAccess?: 'public' | 'private';
  delegate?: string;
  methodName?: string;
}

export interface VersionTag {
  tag: string;
  major: number;
  minor: number;
  patch: number;
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

export interface ApiType extends ApiEntry {
  kind: "struct" | "alias" | "ns"
}

export type ApiEntries = Dict<ApiEntry | ApiEntry[]>;

export type ApiParameters = (string | ApiParameter)[];

export interface ApiParameter {
  name?: string;
  type?: string;
  default?: string;
}

export type StringMap = Dict<string>;

export interface ApiTransform {
  files?: Dict<ApiFileTransform>;
  prefixes?: string | string[];
  definitionPrefix?: string;
  sourceIncludePrefix?: string;
  renameRules?: ReplacementRule[];
  docRules?: ReplacementRule[];
  paramTypeMap?: StringMap;
  returnTypeMap?: StringMap;
  minVersions?: Dict<VersionTag>;
  signatureRules?: SignatureTransform[];

  /// The default namespace
  namespace: string;
}

export interface ApiFileTransform {
  name?: string;
  doc?: string;
  ignoreEntries?: string[];
  includes?: string[];
  localIncludes?: string[];
  sourceIncludePrefix?: string;
  transform?: Dict<ApiEntryTransform>;
  namespacesMap?: StringMap;
  definitionPrefix?: string;
  enableException?: boolean;
}

export type ApiEntryTransformMap = Dict<ApiEntryTransform | ApiEntryTransform[]>;

export interface ApiEntryTransform extends ApiEntryBase {
  entries?: ApiSubEntryTransformLegacyMap;
  link?: ApiEntryTransform;
  enum?: true | string | EnumerationDefinition;
  wrapper?: boolean | WrapperDefinition;
  resource?: boolean | string | ResourceDefinition;
  before?: string;
  after?: string;
}

export interface ResourceDefinition {

  /**
   * The source name of constructors
   * 
   * Anything marked as "ctor" is automatically added here
   */
  ctors?: string[]

  /**
   * The shared field name
   */
  shared?: boolean | string

  /**
   * Name of free function. By default it uses the first subentry with
   * containing "Destroy", "Close" or "free" substring, in that order.
   */
  free?: string

  /**
   * Name of raw resource type
   */
  rawName?: string;

  /**
   * Enable automatic method detection. Defaults to true
   */
  enableAutoMethods?: boolean;

  /**
   * Enable const parameters
   */
  enableConstParam?: boolean

  /**
   * Enable ref type. Default to false
   */
  owning?: boolean

  /**
   * Enable ref type. Default to true if non shared and owning are both false
   */
  ref?: boolean
}

export interface ApiLock extends ApiEntryTransform {
  kind?: "struct";
}

export interface WrapperDefinition {
  /** Defaults to `value` */
  attribute?: string;

  /** Defaults to true */
  invalidState?: boolean;

  /** Defaults to {} */
  defaultValue?: string

  /** Defaults to true if alias to pointer, false otherwise */
  nullable?: boolean

  /** Defaults to true */
  genCtor?: boolean

  /** Defaults to true, relevant only to struct */
  genMembers?: boolean

  /** 
   * Defaults to true if alias to anything but `void *`, false otherwise.
   */
  ordered?: boolean

  /** 
   * Defaults to true if ordered is false and not a struct
   */
  comparable?: boolean

  /**
   * Param type for generated methods
   */
  paramType?: string

  /**
   * Name of raw resource type
   */
  rawName?: string;
}

export interface EnumerationDefinition {
  prefix?: string;
  newPrefix?: string;
  values?: string[];
  valueType?: string;
}

export type QuickTransform = "immutable" | "ctor" | ApiEntryKind;

export type ApiSubEntryTransformLegacyMap = Dict<ApiEntryTransform | ApiEntryBase[] | QuickTransform>;

export interface SignatureTransform {
  pattern: ApiParameter[];
  replaceParams: ApiParameter[];
}

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
  proto?: boolean
  static?: boolean;
  explicit?: boolean;
  begin: number;
  end: number;
  spaces: number;
  doc?: string;
  since?: VersionTag;
}
