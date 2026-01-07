export type Dict<T> = { [key: string]: T };

export interface Api {
  files: Dict<ApiFile>;
  paramReplacements?: Dict<string>;
  delegatedReplacements?: Dict<string>;
}

export interface ApiFile {
  name: string;
  doc?: ParsedDoc;
  entries?: ApiEntries;
  namespace?: string;
  includes?: string[];
  localIncludes?: string[];
  docBegin?: number;
  docEnd?: number;
  entriesBegin?: number;
  entriesEnd?: number;
}

export type ApiEntryKind =
  | "alias"
  | "concept"
  | "callback"
  | "def"
  | "enum"
  | "forward"
  | "function"
  | "struct"
  | "union"
  | "var"
  | "ns"
  | "plc";

export interface ApiEntryBase {
  name?: string;
  kind?: ApiEntryKind;
  doc?: ParsedDoc;
  type?: string;
  parameters?: ApiParameters;
  template?: ApiParameters;
  sourceName?: string;
  constexpr?: boolean;
  immutable?: boolean;
  reference?: number;
  proto?: boolean;
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
  mayFail?: boolean | string;
  removeParamThis?: boolean;
  private?: boolean;
  wrapSelf?: boolean;
  changeAccess?: "public" | "private" | "protected";
  delegate?: string;
  methodName?: string;
  noexcept?: boolean;
  copyDoc?: string;
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
  link?: ApiEntry;
  overload?: ApiEntry;
}

export interface ApiType extends ApiEntry {
  kind: "struct" | "alias" | "ns";
}

export type ApiEntries = Dict<ApiEntry>;

export type ApiParameters = ApiParameter[];

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
  baseVersions?: Dict<VersionTag>;
  signatureRules?: SignatureTransform[];

  /// The default namespace
  namespace: string;

  /// Replacements
  paramReplacements?: Dict<string>;
  delegatedReplacements?: Dict<string>;
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

export type ApiEntryTransformMap = Dict<ApiEntryTransform | QuickTransform>;

export interface ApiEntryTransform extends ApiEntryBase {
  entries?: ApiEntryTransformMap;
  link?: ApiEntryTransform;
  callback?: boolean | FunctorSupport | CallbackDefinition;
  enum?: boolean | string | EnumerationDefinition;
  wrapper?: boolean | WrapperDefinition;
  resource?: boolean | string | ResourceDefinition;
  lock?: boolean | LockDefinition;
  before?: string;
  after?: string;
}

export type FunctorSupport = "std" | "lightweight";

export interface CallbackDefinition {
  functorSupport?: boolean | FunctorSupport;

  userdataIndex?: number;

  /// The return type. By default the raw callback's return type
  type?: string;

  /// The parameters. By default the raw callback's return type
  parameters?: ApiParameters;

  /// Wrapped name
  wrapper?: string;
}

export interface ResourceDefinition {
  /**
   * The source name of constructors
   *
   * Anything marked as "ctor" is automatically added here
   */
  ctors?: string[];

  /**
   * The shared field name
   */
  shared?: boolean | string;

  /**
   * Name of free function. By default it uses the first subentry with
   * containing "Destroy", "Close" or "free" substring, in that order.
   */
  free?: string;

  /**
   * Name of raw resource type
   */
  rawName?: string;

  /**
   * Enable automatic method detection. Defaults to true
   */
  enableAutoMethods?: boolean;

  /**
   * If true allow member access with `->` arrow.
   *
   * Default true for non-opaque structs
   */
  enableMemberAccess?: boolean;

  /**
   * Enable const parameters, defaults true if enableMemberAccess is true
   */
  enableConstParam?: boolean;

  /**
   * Enable ref type. Default to false
   */
  owning?: boolean;

  /**
   * Enable ref type. Default to true if non shared and owning are both false
   */
  ref?: boolean;
}

export interface LockDefinition {
  lockFunc?: string;
  unlockFunc?: string;

  /**
   * If false, does not generate control variable
   */
  controlVar?: boolean;

  controlType?: string;

  paramType?: string;
}

export interface WrapperDefinition {
  /** Defaults to `value` */
  attribute?: string;

  /** Defaults to true */
  invalidState?: boolean;

  /** Defaults to {} */
  defaultValue?: string;

  /** Defaults to true if alias to pointer, false otherwise */
  nullable?: boolean;

  /** Defaults to true */
  genCtor?: boolean;

  /** Defaults to true, relevant only to struct */
  genMembers?: boolean;

  /**
   * Defaults to true if alias to anything but `void *`, false otherwise.
   */
  ordered?: boolean;

  /**
   * Defaults to true if ordered is false and not a struct
   */
  comparable?: boolean;

  /**
   * Param type for generated methods
   */
  paramType?: string;

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
  proto?: boolean;
  static?: boolean;
  explicit?: boolean;
  begin: number;
  end: number;
  spaces: number;
  doc?: string;
  since?: VersionTag;
}

export type ParsedDoc = ParsedDocContent[];

export type ParsedDocContent =
  | string
  | ListContent
  | StaticContent
  | TaggedContent;

export type ListContent = TaggedContent[];
export interface StaticContent {
  tag?: undefined;
  content: string;
}

export interface TaggedContent {
  tag: string;
  content: string;
}
