import { writeJSONSync } from "./cppfier/utils.ts";
import baseVersions from "./base.json" with { type: 'json' };

/**
 * @import {ApiTransform} from "./cppfier/types"
 */

const currentVersion = ["0", "5", "6"];

/** @type {ApiTransform} */
const transform = {
  prefixes: ["SDL_", "IMG_", "TTF_"],
  definitionPrefix: "SDL_",
  sourceIncludePrefix: 'SDL3/',
  namespace: "SDL",
  renameRules: [{
    pattern: String.raw`^SDL_(.*)\.h$`,
    replacement: "SDL3pp_$1.h"
  }],
  minVersions: {
    SDL: { tag: "SDL", major: 3, minor: 2, patch: 0 },
    SDL_TTF: { tag: "SDL_TTF", major: 3, minor: 2, patch: 0 },
    SDL_IMAGE: { tag: "SDL_IMAGE", major: 3, minor: 2, patch: 0 },
  },
  baseVersions,
  paramTypeMap: {
    "const char *": "StringParam",
    "TTF_TextEngine *": "TextEngineParam",
  },
  returnTypeMap: {
    "TTF_TextEngine *": "TextEngine"
  },
  signatureRules: [{
    pattern: [
      { type: "Uint8", name: "r" },
      { type: "Uint8", name: "g" },
      { type: "Uint8", name: "b" },
      { type: "Uint8", name: "a" }
    ],
    replaceParams: [{ type: "ColorRaw", name: "c" }]
  }, {
    pattern: [
      { type: "float", name: "r" },
      { type: "float", name: "g" },
      { type: "float", name: "b" },
      { type: "float", name: "a" }
    ],
    replaceParams: [{ type: "const FColorRaw &", name: "c" }]
  }, {
    pattern: [{ type: "int", name: "x" }, { type: "int", name: "y" }],
    replaceParams: [{ type: "const PointRaw &", name: "p" }]
  }, {
    pattern: [{ type: "int", name: "x1" }, { type: "int", name: "y1" }],
    replaceParams: [{ type: "const PointRaw &", name: "p1" }]
  }, {
    pattern: [{ type: "int", name: "x2" }, { type: "int", name: "y2" }],
    replaceParams: [{ type: "const PointRaw &", name: "p2" }]
  }, {
    pattern: [{ type: "int", name: "w" }, { type: "int", name: "h" }],
    replaceParams: [{ type: "const PointRaw &", name: "size" }]
  }, {
    pattern: [{ type: "int", name: "width" }, { type: "int", name: "height" }],
    replaceParams: [{ type: "const PointRaw &", name: "size" }]
  }, {
    pattern: [{ type: "int", name: "offset_x" }, { type: "int", name: "offset_y" }],
    replaceParams: [{ type: "const PointRaw &", name: "offset" }]
  }, {
    pattern: [{ type: "float", name: "x" }, { type: "float", name: "y" }],
    replaceParams: [{ type: "const FPointRaw &", name: "p" }]
  }, {
    pattern: [{ type: "float", name: "x1" }, { type: "float", name: "y1" }],
    replaceParams: [{ type: "const FPointRaw &", name: "p1" }]
  }, {
    pattern: [{ type: "float", name: "x2" }, { type: "float", name: "y2" }],
    replaceParams: [{ type: "const FPointRaw &", name: "p2" }]
  }, {
    pattern: [{ type: "float", name: "w" }, { type: "float", name: "h" }],
    replaceParams: [{ type: "const FPointRaw &", name: "size" }]
  }, {
    pattern: [{ type: "float", name: "width" }, { type: "float", name: "height" }],
    replaceParams: [{ type: "const FPointRaw &", name: "size" }]
  }, {
    pattern: [{ type: "float", name: "offset_x" }, { type: "float", name: "offset_y" }],
    replaceParams: [{ type: "const FPointRaw &", name: "offset" }]
  }],
  paramReplacements: {
    "SourceBytes": "$.data(), $.size_bytes()",
    "TargetBytes": "$.data(), $.size_bytes()",
  },
  delegatedReplacements: {
    "StringParam": "std::move($)",
    "SourceBytes": "std::move($)",
    "TargetBytes": "std::move($)",
  },
  files: {
    "SDL_assert.h": {
      localIncludes: ['SDL3pp_callbackWrapper.h', 'SDL3pp_strings.h'],
      ignoreEntries: [
        "__debugbreak"
      ],
      transform: {
        "SDL_ReportAssertion": {
          parameters: [
            {},
            {},
            {},
            {
              "type": "int",
              "name": "line"
            }
          ]
        },
        "SDL_AssertionHandler": {
          callback: "lightweight"
        },
        "SetAssertionHandler": {
          kind: "function",
          after: "SDL_SetAssertionHandler",
          type: "void",
          parameters: [{ type: "AssertionHandlerCB", name: "handler" }]
        },
      }
    },
    "SDL_asyncio.h": {
      localIncludes: ["SDL3pp_log.h", "SDL3pp_stdinc.h"],
      transform: {
        "SDL_AsyncIO": {
          resource: { free: "SDL_CloseAsyncIO" },
          entries: {
            "SDL_AsyncIOFromFile": "ctor",
            "SDL_CloseAsyncIO": "function"
          },
        },
        "SDL_AsyncIOQueue": {
          entries: {
            "SDL_CreateAsyncIOQueue": "ctor",
          }
        },
        "SDL_GetAsyncIOResult": {
          type: "std::optional<AsyncIOOutcome>",
          parameters: [{}],
          hints: { methodName: "GetResult" }
        },
        "SDL_WaitAsyncIOResult": {
          type: "std::optional<AsyncIOOutcome>",
          parameters: [{}, { type: "Milliseconds", name: "timeout" }],
          hints: { methodName: "WaitResult" },
        },
        "WaitAsyncIOResult": {
          kind: "function",
          type: "std::optional<AsyncIOOutcome>",
          parameters: [{ type: "AsyncIOQueueParam", name: "queue" }],
          hints: { methodName: "WaitResult" },
        },
      }
    },
    "SDL_atomic.h": {
      localIncludes: ["SDL3pp_stdinc.h"],
      ignoreEntries: [
        "SDL_SpinLock",
        "SDL_TryLockSpinlock",
        "SDL_LockSpinlock",
        "SDL_UnlockSpinlock",
        "SDL_KernelMemoryBarrierFunc",
        "SDL_MEMORY_BARRIER_USES_FUNCTION",
      ],
      transform: {
        "SDL_MemoryBarrierReleaseFunction": { name: "MemoryBarrierRelease", after: "__begin" },
        "SDL_MemoryBarrierAcquireFunction": { name: "MemoryBarrierAcquire", after: "__begin" },
        "SDL_AtomicInt": {
          kind: "struct",
          wrapper: {
            genMembers: false,
            invalidState: false,
            genCtor: false,
          },
          entries: {
            "AtomicInt": {
              kind: "function",
              type: "",
              constexpr: true,
              parameters: [{
                type: "int",
                name: "value"
              }],
              hints: {
                init: ["AtomicIntRaw(value)"],
              }
            },
            "AtomicInt#2": {
              kind: "function",
              type: "",
              proto: true,
              parameters: [{
                type: "const AtomicInt &",
                name: "value"
              }],
              hints: {
                delete: true
              }
            },
            "operator=": {
              kind: "function",
              type: "AtomicInt &",
              proto: true,
              parameters: [{
                type: "const AtomicInt &",
                name: "value"
              }],
              hints: {
                delete: true
              }
            },
            "SDL_CompareAndSwapAtomicInt": "function",
            "SDL_SetAtomicInt": "function",
            "SDL_GetAtomicInt": "function",
            "SDL_AddAtomicInt": "function",
            "SDL_AtomicIncRef": {
              kind: "function",
              type: "bool",
              parameters: [],
            },
            "SDL_AtomicDecRef": {
              kind: "function",
              type: "bool",
              parameters: [],
            }
          }
        },
        "SDL_AtomicU32": {
          kind: "struct",
          wrapper: {
            genMembers: false,
            invalidState: false,
            genCtor: false,
          },
          entries: {
            "AtomicU32": {
              kind: "function",
              type: "",
              constexpr: true,
              parameters: [{
                type: "Uint32",
                name: "value"
              }],
              hints: {
                init: ["m_value(value)"],
                changeAccess: "public",
              }
            },
            "AtomicU32#2": {
              kind: "function",
              type: "",
              proto: true,
              parameters: [{
                type: "const AtomicU32 &",
                name: "value"
              }],
              hints: {
                delete: true
              }
            },
            "operator=": {
              kind: "function",
              type: "AtomicU32 &",
              proto: true,
              parameters: [{
                type: "const AtomicU32 &",
                name: "value"
              }],
              hints: {
                delete: true
              }
            },
            "SDL_CompareAndSwapAtomicU32": "function",
            "SDL_SetAtomicU32": "function",
            "SDL_GetAtomicU32": "function",
          }
        },
        "AtomicPointer": {
          kind: "struct",
          template: [{
            type: "class",
            name: "T"
          }],
          hints: {
            self: "&m_value",
            private: true,
          },
          entries: {
            "m_value": {
              kind: "var",
              type: "T *",
            },
            "AtomicPointer": {
              kind: "function",
              type: "",
              constexpr: true,
              parameters: [{
                type: "T *",
                name: "value"
              }],
              hints: {
                init: ["m_value(value)"],
                changeAccess: "public",
              }
            },
            "AtomicPointer#2": {
              kind: "function",
              type: "",
              proto: true,
              parameters: [{
                type: "const AtomicPointer &",
                name: "value"
              }],
              hints: {
                delete: true
              }
            },
            "operator=": {
              kind: "function",
              type: "AtomicPointer &",
              proto: true,
              parameters: [{
                type: "const AtomicPointer &",
                name: "value"
              }],
              hints: {
                delete: true
              }
            },
            "SDL_CompareAndSwapAtomicPointer": {
              static: false,
              parameters: [
                { type: "T *", name: "oldval" },
                { type: "T *", name: "newval" },
              ]
            },
            "SDL_SetAtomicPointer": {
              type: "T *",
              static: false,
              parameters: [
                { type: "T *", name: "v" },
              ]
            },
            "SDL_GetAtomicPointer": {
              type: "T *",
              static: false,
              parameters: []
            }
          }
        },
      }
    },
    "SDL_audio.h": {
      localIncludes: ["SDL3pp_iostream.h", "SDL3pp_properties.h", "SDL3pp_stdinc.h"],
      transform: {
        "SDL_AudioSpec": { before: "AudioFormat" },
        "SDL_AUDIO_MASK_BITSIZE": { kind: "var", type: "Uint32", constexpr: true },
        "SDL_AUDIO_MASK_FLOAT": { kind: "var", type: "Uint32", constexpr: true },
        "SDL_AUDIO_MASK_BIG_ENDIAN": { kind: "var", type: "Uint32", constexpr: true },
        "SDL_AUDIO_MASK_SIGNED": { kind: "var", type: "Uint32", constexpr: true },
        "SDL_AudioFormat": {
          before: "SDL_DEFINE_AUDIO_FORMAT",
          wrapper: true,
          entries: {
            "SDL_DEFINE_AUDIO_FORMAT": {
              kind: "function",
              name: "ctor",
              type: "",
              constexpr: true,
              parameters: [
                { type: "bool", name: "sign" },
                { type: "bool", name: "bigendian" },
                { type: "bool", name: "flt" },
                { type: "Uint16", name: "size" },
              ]
            },
          }
        },
        "SDL_DEFINE_AUDIO_FORMAT": {
          name: "DefineAudioFormat",
        },
        "SDL_AUDIO_BITSIZE": {
          kind: "function",
          name: "AudioBitSize",
          constexpr: true,
          immutable: true,
          type: "Uint16",
          parameters: [{ type: "AudioFormatRaw" }],
          hints: { methodName: "GetBitSize" },
        },
        "SDL_AUDIO_BYTESIZE": {
          kind: "function",
          name: "AudioByteSize",
          constexpr: true,
          immutable: true,
          type: "Uint16",
          parameters: [{ type: "AudioFormatRaw" }],
          hints: { methodName: "GetByteSize" },
        },
        "SDL_AUDIO_ISFLOAT": {
          kind: "function",
          name: "IsAudioFloat",
          constexpr: true,
          immutable: true,
          type: "bool",
          parameters: [{ type: "AudioFormatRaw" }],
          hints: { methodName: "IsFloat" },
        },
        "SDL_AUDIO_ISBIGENDIAN": {
          kind: "function",
          name: "IsAudioBigENDIAN",
          constexpr: true,
          immutable: true,
          type: "bool",
          parameters: [{ type: "AudioFormatRaw" }],
          hints: { methodName: "IsBigEndian" },
        },
        "SDL_AUDIO_ISLITTLEENDIAN": {
          kind: "function",
          name: "IsAudioLittleEndian",
          constexpr: true,
          immutable: true,
          type: "bool",
          parameters: [{ type: "AudioFormatRaw" }],
          hints: { methodName: "IsLittleEndian" },
        },
        "SDL_AUDIO_ISSIGNED": {
          kind: "function",
          name: "IsAudioSigned",
          constexpr: true,
          immutable: true,
          type: "bool",
          parameters: [{ type: "AudioFormatRaw" }],
          hints: { methodName: "IsSigned" },
        },
        "SDL_AUDIO_ISINT": {
          kind: "function",
          name: "IsAudioInt",
          constexpr: true,
          immutable: true,
          type: "bool",
          parameters: [{ type: "AudioFormatRaw" }],
          hints: { methodName: "IsInt" },
        },
        "SDL_AUDIO_ISUNSIGNED": {
          kind: "function",
          name: "IsAudioUnsigned",
          constexpr: true,
          immutable: true,
          type: "bool",
          parameters: [{ type: "AudioFormatRaw" }],
          hints: { methodName: "IsUnsigned" },
        },
        "SDL_GetAudioFormatName": { immutable: true },
        "SDL_GetSilenceValueForFormat": {
          immutable: true,
          hints: { methodName: "GetSilenceValue" },
        },
        "SDL_AudioPostmixCallback": {
          before: "SDL_AudioDeviceID",
          callback: "lightweight",
        },
        "SDL_AudioDeviceID": {
          name: "AudioDevice",
          resource: {
            free: "SDL_CloseAudioDevice",
            rawName: "AudioDeviceID",
          },
          entries: {
            "SDL_OpenAudioDevice": {
              name: "ctor",
              parameters: [
                { type: "AudioDeviceParam" },
                { type: "OptionalRef<const AudioSpec>" }
              ]
            },
            "operator<=>": {
              kind: "function",
              constexpr: true,
              immutable: true,
              type: "auto",
              parameters: [{ type: "AudioDeviceParam", name: "other" }],
              hints: { default: true }
            },
            "SDL_GetAudioDeviceName": "immutable",
            "SDL_GetAudioDeviceFormat": {
              immutable: true,
              type: "AudioSpec",
              parameters: [
                {},
                {
                  type: "int *",
                  name: "sample_frames",
                  default: "nullptr"
                }
              ]
            },
            "SDL_GetAudioDeviceChannelMap": {
              immutable: true,
              type: "OwnArray<int>",
              parameters: [{}]
            },
            "SDL_IsAudioDevicePhysical": "immutable",
            "SDL_IsAudioDevicePlayback": "immutable",
            "SDL_PauseAudioDevice": "function",
            "SDL_ResumeAudioDevice": "function",
            "SDL_AudioDevicePaused": "immutable",
            "SDL_GetAudioDeviceGain": "immutable",
            "SDL_SetAudioDeviceGain": "function",
            "SDL_BindAudioStreams": {
              proto: true,
              parameters: [
                {},
                { type: "std::span<AudioStreamRef>" }
              ]
            },
            "SDL_BindAudioStream": {
              proto: true
            },
            "SDL_SetAudioPostmixCallback": {
              name: "SetPostmixCallback"
            },
          }
        },
        "SetAudioPostmixCallback": {
          after: "SDL_SetAudioPostmixCallback",
          kind: "function",
          type: "void",
          parameters: [{
            type: "AudioDeviceParam",
            name: "devid"
          }, {
            type: "AudioPostmixCB",
            name: "callback"
          }],
          hints: { methodName: "SetPostmixCallback" }
        },
        "SDL_AudioStreamCallback": {
          before: "SDL_AudioDeviceID",
          callback: "lightweight",
        },
        "SDL_AudioStream": {
          resource: true,
          entries: {
            "SDL_CreateAudioStream": {
              name: "ctor",
              parameters: [
                { type: "OptionalRef<const AudioSpec>" },
                { type: "OptionalRef<const AudioSpec>" },
              ]
            },
            "SDL_OpenAudioDeviceStream": {
              name: "ctor",
              parameters: [
                {
                  name: "devid",
                  type: "AudioDeviceParam"
                },
                {
                  name: "spec",
                  type: "OptionalRef<const AudioSpec>",
                  default: "std::nullopt"
                },
                {
                  name: "callback",
                  type: "AudioStreamCallback",
                  default: "nullptr"
                },
                {
                  name: "userdata",
                  type: "void *",
                  default: "nullptr"
                },
              ],
            },
            "SDL_GetAudioStreamProperties": "immutable",
            "GetInputFormat": {
              kind: "function",
              type: "AudioSpec",
              immutable: true,
              parameters: []
            },
            "GetOutputFormat": {
              kind: "function",
              type: "AudioSpec",
              immutable: true,
              parameters: []
            },
            "SDL_GetAudioStreamFormat": "immutable",
            "SetInputFormat": {
              kind: "function",
              type: "void",
              parameters: [{ type: "const AudioSpec &", name: "spec" }]
            },
            "SetOutputFormat": {
              kind: "function",
              type: "void",
              parameters: [{ type: "const AudioSpec &", name: "spec" }]
            },
            "SDL_SetAudioStreamFormat": {
              parameters: [
                {},
                { type: "OptionalRef<const AudioSpec>" },
                { type: "OptionalRef<const AudioSpec>" },
              ]
            },
            "SDL_GetAudioStreamFrequencyRatio": "immutable",
            "SDL_SetAudioStreamFrequencyRatio": "function",
            "SDL_GetAudioStreamGain": "immutable",
            "SDL_SetAudioStreamGain": "function",
            "SDL_GetAudioStreamInputChannelMap": {
              type: "OwnArray<int>",
              immutable: true,
              parameters: [{}]
            },
            "SDL_GetAudioStreamOutputChannelMap": {
              type: "OwnArray<int>",
              immutable: true,
              parameters: [{}]
            },
            "SDL_SetAudioStreamInputChannelMap": {
              type: "void",
              parameters: [{}, { type: "std::span<int>" }]
            },
            "SDL_SetAudioStreamOutputChannelMap": {
              type: "void",
              parameters: [{}, { type: "std::span<int>" }]
            },
            "SDL_PutAudioStreamData": {
              parameters: [{}, { type: "SourceBytes", name: "buf" }]
            },
            "SDL_GetAudioStreamData": {
              parameters: [{}, { type: "TargetBytes", name: "buf" }]
            },
            "SDL_GetAudioStreamAvailable": "immutable",
            "SDL_GetAudioStreamQueued": "immutable",
            "SDL_FlushAudioStream": "function",
            "SDL_ClearAudioStream": "function",
            "SDL_PauseAudioStreamDevice": "function",
            "SDL_ResumeAudioStreamDevice": "function",
            "SDL_AudioStreamDevicePaused": "immutable",
            "SDL_LockAudioStream": "function",
            "SDL_UnlockAudioStream": "function",
            "SDL_SetAudioStreamGetCallback": "function",
            "SDL_SetAudioStreamPutCallback": "function",
            "SDL_UnbindAudioStream": "function",
            "SDL_GetAudioStreamDevice": "immutable",
          }
        },
        "OpenAudioDeviceStream": {
          after: "SDL_OpenAudioDeviceStream",
          kind: "function",
          type: "AudioStream",
          parameters: [
            {
              name: "devid",
              type: "AudioDeviceParam"
            },
            {
              name: "spec",
              type: "OptionalRef<const AudioSpec>"
            },
            {
              name: "callback",
              type: "AudioStreamCB"
            }
          ],
          hints: { delegate: "AudioStream" },
        },
        "AudioStream::AudioStream": {
          kind: "function",
          type: "",
          parameters: [
            {
              name: "devid",
              type: "AudioDeviceParam"
            },
            {
              name: "spec",
              type: "OptionalRef<const AudioSpec>"
            },
            {
              name: "callback",
              type: "AudioStreamCB"
            }
          ],
        },
        "SetAudioStreamGetCallback": {
          after: "SDL_SetAudioStreamGetCallback",
          kind: "function",
          type: "void",
          parameters: [
            { name: "stream", type: "AudioStreamParam" },
            { name: "callback", type: "AudioStreamCB" },
          ],
          hints: { methodName: "SetGetCallback" },
        },
        "SetAudioStreamPutCallback": {
          after: "SDL_SetAudioStreamPutCallback",
          kind: "function",
          type: "void",
          parameters: [
            { name: "stream", type: "AudioStreamParam" },
            { name: "callback", type: "AudioStreamCB" },
          ],
          hints: { methodName: "SetPutCallback" },
        },
        "SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK": {
          kind: "var",
          constexpr: true,
          type: "AudioDeviceRaw"
        },
        "SDL_AUDIO_DEVICE_DEFAULT_RECORDING": {
          kind: "var",
          constexpr: true,
          type: "AudioDeviceRaw"
        },
        "SDL_AUDIO_FRAMESIZE": {
          kind: "function",
          name: "AudioFrameSize",
          constexpr: true,
          type: "int",
          parameters: [{
            type: "const AudioSpec &",
            name: "x"
          }]
        },
        "SDL_GetAudioPlaybackDevices": {
          type: "OwnArray<AudioDeviceRef>",
          parameters: []
        },
        "SDL_GetAudioRecordingDevices": {
          type: "OwnArray<AudioDeviceRef>",
          parameters: []
        },
        "SDL_UnbindAudioStreams": {
          parameters: [{ type: "std::span<AudioStreamRef>" }]
        },
        "SDL_LoadWAV_IO": {
          name: "LoadWAV",
          type: "OwnArray<Uint8>",
          parameters: [
            {},
            { name: "spec", type: "AudioSpec *" },
            { name: "closeio", type: "bool", default: "false" },
          ]
        },
        "SDL_LoadWAV": {
          type: "OwnArray<Uint8>",
          parameters: [
            {},
            { name: "spec", type: "AudioSpec *" },
          ]
        },
        "SDL_MixAudio": {
          parameters: [
            {
              name: "dst",
              type: "Uint8 *"
            },
            {
              name: "src",
              type: "SourceBytes"
            },
            {
              name: "format",
              type: "AudioFormat"
            },
            {
              name: "volume",
              type: "float"
            }
          ]
        },
        "MixAudio": {
          kind: "function",
          type: "void",
          parameters: [
            {
              name: "dst",
              type: "TargetBytes"
            },
            {
              name: "src",
              type: "SourceBytes"
            },
            {
              name: "format",
              type: "AudioFormat"
            },
            {
              name: "volume",
              type: "float"
            }
          ]
        },
        "SDL_ConvertAudioSamples": {
          type: "OwnArray<Uint8>",
          parameters: [
            {
              name: "src_spec",
              type: "const AudioSpec &"
            },
            {
              name: "src_data",
              type: "SourceBytes"
            },
            {
              name: "dst_spec",
              type: "const AudioSpec &"
            }
          ]
        }
      }
    },
    "SDL_blendmode.h": {
      transform: {
        "SDL_BlendMode": {
          enum: "SDL_BLENDMODE_",
        }
      }
    },
    "SDL_camera.h": {
      localIncludes: ["SDL3pp_error.h", "SDL3pp_pixels.h", "SDL3pp_properties.h", "SDL3pp_stdinc.h", "SDL3pp_surface.h"],
      transform: {
        "SDL_CameraSpec": { before: "SDL_Camera" },
        "SDL_CameraPosition": { before: "SDL_Camera" },
        "SDL_GetCameras": {
          type: "OwnArray<CameraID>",
          parameters: [],
        },
        "SDL_GetCameraSupportedFormats": {
          type: "OwnArray<CameraSpec *>",
          parameters: [{}],

        },
        "SDL_GetCameraFormat": {
          type: "std::optional<CameraSpec>",
          parameters: [{}],
        },
        "SDL_OpenCamera": {
          parameters: [{}, { type: "OptionalRef<const CameraSpec>", default: "{}" }]
        },
      },
    },
    "SDL_clipboard.h": {
      localIncludes: ["SDL3pp_stdinc.h"],
      transform: {
        "SDL_GetClipboardText": { type: "StringResult" },
        "SDL_GetPrimarySelectionText": { type: "StringResult" },
        "SDL_ClipboardCleanupCallback": { callback: "std" },
        "SDL_ClipboardDataCallback": {
          callback: {
            functorSupport: "std",
            type: "SourceBytes",
            parameters: [{ type: "const char *", name: "mime_type" }]
          }
        },
        "SDL_SetClipboardData": {
          parameters: [
            {
              name: "callback",
              type: "ClipboardDataCallback"
            },
            {
              name: "cleanup",
              type: "ClipboardCleanupCallback"
            },
            {
              name: "userdata",
              type: "void *"
            },
            {
              name: "mime_types",
              type: "std::span<const char *>"
            }
          ]
        },
        "SetClipboardData": {
          kind: "function",
          type: "void",
          parameters: [
            {
              name: "callback",
              type: "ClipboardDataCB"
            },
            {
              name: "cleanup",
              type: "ClipboardCleanupCB"
            },
            {
              name: "mime_types",
              type: "std::span<const char *>"
            }
          ]
        },
        "SDL_GetClipboardData": {
          type: "StringResult",
          parameters: [{
            name: "mime_type",
            type: "StringParam"
          }]
        },
        "GetClipboardDataAs": {
          kind: "function",
          template: [{
            type: "class",
            name: "T"
          }],
          type: "OwnArray<T>",
          parameters: [{
            name: "mime_type",
            type: "StringParam"
          }]
        },
        "SDL_GetClipboardMimeTypes": {
          type: "OwnArray<char *>",
          parameters: []
        }
      }
    },
    "SDL_cpuinfo.h": {
      localIncludes: ["SDL3pp_stdinc.h"],
      transform: {
        "SDL_CACHELINE_SIZE": {
          kind: "var",
          type: "int",
          constexpr: true
        }
      }
    },
    "SDL_dialog.h": {
      localIncludes: ["SDL3pp_properties.h", "SDL3pp_stdinc.h", "SDL3pp_video.h"],
      namespacesMap: {
        "SDL_PROP_FILE_DIALOG_": "prop::FileDialog"
      },
      transform: {
        "SDL_DialogFileCallback": { callback: "std" },
        "ShowOpenFileDialog": {
          "after": "SDL_ShowOpenFileDialog",
          "kind": "function",
          "type": "void",
          "parameters": [
            {
              "type": "DialogFileCB",
              "name": "callback"
            },
            {
              "name": "window",
              "type": "WindowParam"
            },
            {
              "name": "filters",
              "type": "std::span<const DialogFileFilter>"
            },
            {
              "name": "default_location",
              "type": "StringParam"
            },
            {
              "name": "allow_many",
              "type": "bool"
            }
          ]
        },
        "ShowSaveFileDialog": {
          "after": "SDL_ShowSaveFileDialog",
          "kind": "function",
          "type": "void",
          "parameters": [
            {
              "name": "callback",
              "type": "DialogFileCB"
            },
            {
              "name": "window",
              "type": "WindowParam"
            },
            {
              "name": "filters",
              "type": "std::span<const DialogFileFilter>"
            },
            {
              "name": "default_location",
              "type": "StringParam"
            }
          ]
        },
        "ShowOpenFolderDialog": {
          "after": "SDL_ShowOpenFolderDialog",
          "kind": "function",
          "type": "void",
          "parameters": [
            {
              "name": "callback",
              "type": "DialogFileCB"
            },
            {
              "name": "window",
              "type": "WindowParam"
            },
            {
              "name": "default_location",
              "type": "StringParam"
            },
            {
              "name": "allow_many",
              "type": "bool"
            }
          ]
        },
        "ShowFileDialogWithProperties": {
          "after": "SDL_ShowFileDialogWithProperties",
          "kind": "function",
          "type": "void",
          "parameters": [
            {
              "name": "type",
              "type": "FileDialogType"
            },
            {
              "name": "callback",
              "type": "DialogFileCB"
            },
            {
              "name": "props",
              "type": "PropertiesID"
            }
          ]
        },
        "SDL_FileDialogType": {
          enum: "SDL_FILEDIALOG_"
        },
        "SDL_ShowOpenFileDialog": {
          "parameters": [
            {
              "name": "callback",
              "type": "DialogFileCallback"
            },
            {
              "name": "userdata",
              "type": "void *"
            },
            {
              "name": "window",
              "type": "WindowParam"
            },
            {
              "name": "filters",
              "type": "std::span<const DialogFileFilter>"
            },
            {
              "name": "default_location",
              "type": "StringParam"
            },
            {
              "name": "allow_many",
              "type": "bool"
            }
          ]
        },
        "SDL_ShowSaveFileDialog": {
          "parameters": [
            {
              "name": "callback",
              "type": "DialogFileCallback"
            },
            {
              "name": "userdata",
              "type": "void *"
            },
            {
              "name": "window",
              "type": "WindowParam"
            },
            {
              "name": "filters",
              "type": "std::span<const DialogFileFilter>"
            },
            {
              "name": "default_location",
              "type": "StringParam"
            }
          ]
        },
        "SDL_ShowOpenFolderDialog": {
          parameters: [
            {
              "name": "callback",
              "type": "DialogFileCallback"
            },
            {
              "name": "userdata",
              "type": "void *"
            },
            {
              "name": "window",
              "type": "WindowParam"
            },
            {
              "name": "default_location",
              "type": "StringParam"
            },
            {
              "name": "allow_many",
              "type": "bool"
            }
          ]
        }
      }
    },
    "SDL_error.h": {
      includes: ['exception', 'format'],
      localIncludes: ['SDL3pp_strings.h'],
      ignoreEntries: [
        "SDL_SetErrorV"
      ],
      transform: {
        "SDL_SetError": {
          name: "SetErrorUnformatted",
          parameters: [{
            type: "StringParam",
            name: "message"
          }]
        },
        "SetError": {
          kind: "function",
          type: "bool",
          template: [{
            type: "class...",
            name: "ARGS"
          }],
          parameters: [{
            type: "std::string_view",
            name: "fmt"
          },
          {
            type: "ARGS...",
            name: "args"
          }]
        },
        "Error": {
          after: "SDL_GetError",
          kind: "struct",
          type: "std::exception",
          entries: {
            "m_message": {
              kind: "var",
              type: "std::string"
            },
            "Error": {
              kind: "function",
              type: "",
              parameters: [],
              doc: ["Default ctor."],
              hints: { init: ["m_message(SDL_GetError())"] }
            },
            "Error#2": {
              kind: "function",
              type: "",
              parameters: [{ type: "std::string", name: "message" }],
              hints: { init: ["m_message(std::move(message))"] }
            },
            "what": {
              kind: "function",
              type: "const char *",
              immutable: true,
              parameters: [],
              doc: ["Returns the explanatory string."],
              hints: { body: "return GetError();" }
            },
            "str": {
              kind: "function",
              type: "const std::string &",
              immutable: true,
              constexpr: true,
              parameters: []
            }
          }
        },
        "CheckError": {
          name: "CheckError",
          kind: "function",
          type: "void",
          constexpr: true,
          parameters: [{ type: "bool", name: "result" }]
        },
        "CheckError#2": {
          name: "CheckError",
          kind: "function",
          type: "T",
          constexpr: true,
          template: [{ type: "class", name: "T" }],
          parameters: [{ type: "T", name: "result" }]
        },
        "CheckError#3": {
          name: "CheckError",
          kind: "function",
          type: "T",
          constexpr: true,
          template: [{ type: "class", name: "T" }],
          parameters: [
            { type: "T", name: "result" },
            { type: "T", name: "invalidValue" },
          ]
        }
      }
    },
    "SDL_endian.h": {
      localIncludes: ["SDL3pp_stdinc.h"],
      ignoreEntries: [
        "HAS_BUILTIN_BSWAP16",
        "HAS_BUILTIN_BSWAP32",
        "HAS_BUILTIN_BSWAP64",
        "HAS_BROKEN_BSWAP"
      ],
      transform: {
        "SDL_Swap16LE": {
          "kind": "function",
          "type": "Uint16",
          "constexpr": true,
          "parameters": [
            {
              "type": "Uint16",
              "name": "x"
            }
          ]
        },
        "SDL_Swap16BE": {
          "kind": "function",
          "type": "Uint16",
          "constexpr": true,
          "parameters": [
            {
              "type": "Uint16",
              "name": "x"
            }
          ]
        },
        "SDL_Swap32LE": {
          "kind": "function",
          "type": "Uint32",
          "constexpr": true,
          "parameters": [
            {
              "type": "Uint32",
              "name": "x"
            }
          ]
        },
        "SDL_Swap32BE": {
          "kind": "function",
          "type": "Uint32",
          "constexpr": true,
          "parameters": [
            {
              "type": "Uint32",
              "name": "x"
            }
          ]
        },
        "SDL_Swap64LE": {
          "kind": "function",
          "type": "Uint64",
          "constexpr": true,
          "parameters": [
            {
              "type": "Uint64",
              "name": "x"
            }
          ]
        },
        "SDL_Swap64BE": {
          "kind": "function",
          "type": "Uint64",
          "constexpr": true,
          "parameters": [
            {
              "type": "Uint64",
              "name": "x"
            }
          ]
        },
        "SDL_SwapFloatLE": {
          "kind": "function",
          "type": "float",
          "constexpr": true,
          "parameters": [
            {
              "type": "float",
              "name": "x"
            }
          ]
        },
        "SDL_SwapFloatBE": {
          "kind": "function",
          "type": "float",
          "constexpr": true,
          "parameters": [
            {
              "type": "float",
              "name": "x"
            }
          ]
        }
      }
    },
    "SDL_events.h": {
      localIncludes: ["SDL3pp_stdinc.h", "SDL3pp_video.h"],
      transform: {
        "PollEvent": {
          kind: "function",
          after: "SDL_PollEvent",
          type: "std::optional<Event>",
          parameters: []
        },
        "WaitEvent": {
          kind: "function",
          after: "SDL_WaitEvent",
          type: "Event",
          parameters: []
        },
        "WaitEventTimeout": {
          kind: "function",
          after: "SDL_WaitEventTimeout",
          type: "std::optional<Event>",
          parameters: [
            {
              "type": "Sint32",
              "name": "timeoutMS"
            }
          ]
        },
        "WaitEventTimeout#2": {
          kind: "function",
          name: "WaitEventTimeout",
          type: "bool",
          parameters: [
            {
              "type": "Event *",
              "name": "event"
            },
            {
              "type": "std::chrono::milliseconds",
              "name": "timeoutDuration"
            }
          ]
        },
        "WaitEventTimeout#3": {
          kind: "function",
          name: "WaitEventTimeout",
          type: "std::optional<Event>",
          parameters: [
            {
              "type": "std::chrono::milliseconds",
              "name": "timeoutDuration"
            }
          ]
        },
        "PushEvent": {
          "kind": "function",
          "after": "SDL_PushEvent",
          "type": "void",
          "parameters": [
            {
              "type": "const Event &",
              "name": "event"
            }
          ]
        },
        "SetEventFilter": {
          after: "SDL_SetEventFilter",
          kind: "function",
          type: "void",
          parameters: [{ type: "EventFilterCB", name: "filter" }]
        },
        "AddEventWatch": {
          after: "SDL_AddEventWatch",
          kind: "function",
          type: "void",
          parameters: [{ type: "EventWatcherCB", name: "filter" }]
        },
        "FilterEvents": {
          "kind": "function",
          "after": "SDL_FilterEvents",
          "type": "void",
          "parameters": [
            {
              "type": "EventFilterCB",
              "name": "filter"
            }
          ]
        },
        "SDL_EventType": {
          enum: "SDL_EVENT_"
        },
        "SDL_EventFilter": {
          callback: "std"
        },
        "EventWatcherCB": {
          kind: "alias",
          type: "MakeFrontCallback<bool(Event *event)>"
        },
        "SDL_GetWindowFromEvent": {
          parameters: [{ type: "const Event &" }]
        },
        "SDL_Event": { wrapper: false },
      }
    },
    "SDL_filesystem.h": {
      localIncludes: ["SDL3pp_stdinc.h"],
      transform: {
        "Path": {
          kind: "struct",
          type: "StringResult",
          entries: {
            "StringResult::StringResult": "alias",
            "operator+=": {
              kind: "function",
              type: "Path &",
              parameters: [{
                type: "std::string_view",
                name: "other"
              }]
            },
            "operator+=#2": {
              kind: "function",
              type: "Path &",
              parameters: [{
                type: "char",
                name: "ch"
              }]
            },
            "operator+": {
              kind: "function",
              type: "Path",
              immutable: true,
              parameters: [{
                type: "std::string_view",
                name: "other"
              }]
            },
            "operator+#2": {
              kind: "function",
              type: "Path",
              immutable: true,
              parameters: [{
                type: "char",
                name: "ch"
              }]
            },
            "operator/=": {
              kind: "function",
              type: "Path &",
              parameters: [{
                type: "std::string_view",
                name: "other"
              }]
            },
            "operator/": {
              kind: "function",
              type: "Path",
              immutable: true,
              parameters: [{
                type: "std::string_view",
                name: "other"
              }]
            },
          }
        },
        "SDL_EnumerateDirectoryCallback": { callback: "std" },
        "EnumerateDirectory": {
          kind: "function",
          type: "void",
          after: "SDL_EnumerateDirectory",
          parameters: [
            {
              name: "path",
              type: "StringParam"
            },
            {
              name: "callback",
              type: "EnumerateDirectoryCB"
            }
          ],
        },
        "EnumerateDirectory#2": {
          name: "EnumerateDirectory",
          kind: "function",
          type: "std::vector<Path>",
          parameters: [{
            name: "path",
            type: "StringParam"
          }],
        },
        "SDL_PathInfo": {
          wrapper: {
            genMembers: false,
            nullable: true,
          }
        },
        "SDL_GlobFlags": {
          enum: "SDL_GLOB_"
        },
        "SDL_GetPrefPath": {
          type: "Path"
        },
        "SDL_GetPathInfo": {
          type: "PathInfo",
          parameters: [{
            name: "path",
            type: "StringParam"
          }]
        },
        "SDL_GlobDirectory": {
          type: "OwnArray<char *>",
          parameters: [
            {
              name: "path",
              type: "StringParam"
            },
            {
              name: "pattern",
              type: "StringParam"
            },
            {
              name: "flags",
              type: "GlobFlags"
            }
          ],
        },
        "SDL_GetCurrentDirectory": {
          type: "Path"
        }
      }
    },
    "SDL_gamepad.h": {
      localIncludes: [
        "SDL3pp_error.h",
        "SDL3pp_guid.h",
        "SDL3pp_iostream.h",
        "SDL3pp_joystick.h",
        "SDL3pp_power.h",
        "SDL3pp_properties.h",
        "SDL3pp_sensor.h",
        "SDL3pp_stdinc.h",
      ],
      namespacesMap: {
        "SDL_PROP_GAMEPAD_CAP_": "prop::GamepadCap",
      },
      transform: {
        "SDL_GamepadType": { before: "SDL_Gamepad" },
        "SDL_GamepadButton": { before: "SDL_Gamepad" },
        "SDL_GamepadButtonLabel": { before: "SDL_Gamepad" },
        "SDL_GamepadAxis": { before: "SDL_Gamepad" },
        "SDL_GamepadBindingType": { before: "SDL_Gamepad" },
        "SDL_GamepadBinding": { before: "SDL_Gamepad" },
        "SDL_GetGamepadMappings": {
          type: "OwnArray<char *>",
          parameters: [],
        },
        "SDL_GetGamepadMappingForGUID": {
          type: "StringResult",
          parameters: [{}],
        },
        "SDL_GetGamepadMapping": {
          type: "StringResult",
          parameters: [{}],
        },
        "SDL_GetGamepads": {
          type: "OwnArray<JoystickID>",
          parameters: [],
        },
      },
    },
    "SDL_gpu.h": {
      localIncludes: [
        "SDL3pp_pixels.h",
        "SDL3pp_properties.h",
        "SDL3pp_rect.h",
        "SDL3pp_surface.h",
        "SDL3pp_stdinc.h",
        "SDL3pp_video.h",
      ],
      namespacesMap: {
        "SDL_PROP_GPU_DEVICE_": "prop::GpuDevice",
        "SDL_PROP_GPU_COMPUTEPIPELINE_": "prop::GPUComputePipeline",
        "SDL_PROP_GPU_GRAPHICSPIPELINE_": "prop::GPUGraphicsPipeline",
        "SDL_PROP_GPU_SAMPLER_": "prop::GPUSampler",
        "SDL_PROP_GPU_SHADER_": "prop::GPUShader",
        "SDL_PROP_GPU_TEXTURE_": "prop::GPUTexture",
        "SDL_PROP_GPU_BUFFER_": "prop::GPUBuffer",
        "SDL_PROP_GPU_TRANSFERBUFFER_": "prop::GPUTransferBuffer",
      },
      transform: {
        "SDL_GPUBufferCreateInfo": { before: "GPUBuffer" },
        "SDL_GPUBuffer": { before: "SDL_GPUDevice" },
        "SDL_GPUTransferBufferCreateInfo": { before: "GPUTransferBuffer" },
        "SDL_GPUTransferBuffer": { before: "SDL_GPUDevice" },
        "SDL_GPUTextureCreateInfo": { before: "GPUTexture" },
        "SDL_GPUTexture": { before: "SDL_GPUDevice" },
        "SDL_GPUSamplerCreateInfo": { before: "GPUSampler" },
        "SDL_GPUSampler": { before: "SDL_GPUDevice" },
        "SDL_GPUShaderCreateInfo": { before: "GPUShader" },
        "SDL_GPUShader": { before: "SDL_GPUDevice" },
        "SDL_GPUComputePipelineCreateInfo": { before: "GPUComputePipeline" },
        "SDL_GPUComputePipeline": { before: "SDL_GPUDevice" },
        "SDL_GPUGraphicsPipelineCreateInfo": { before: "GPUGraphicsPipeline" },
        "SDL_GPUGraphicsPipeline": { before: "SDL_GPUDevice" },
        "SDL_GPUViewport": { before: "GPURenderPass" },
        "SDL_GPUBufferBinding": { before: "GPURenderPass" },
        "SDL_GPUIndexElementSize": { before: "GPURenderPass" },
        "SDL_GPUTextureSamplerBinding": { before: "GPURenderPass" },
        "SDL_GPURenderPass": { before: "GPUCommandBuffer" },
        "SDL_GPUComputePass": { before: "GPUCommandBuffer" },
        "SDL_GPUBufferRegion": { before: "GPUCopyPass" },
        "SDL_GPUTextureLocation": { before: "GPUCopyPass" },
        "SDL_GPUBufferLocation": { before: "GPUCopyPass" },
        "SDL_GPUTextureRegion": { before: "GPUCopyPass" },
        "SDL_GPUTextureTransferInfo": { before: "GPUCopyPass" },
        "SDL_GPUTransferBufferLocation": { before: "GPUCopyPass" },
        "SDL_GPUCopyPass": { before: "GPUCommandBuffer" },
        "SDL_GPUColorTargetInfo": { before: "GPUCommandBuffer" },
        "SDL_GPUDepthStencilTargetInfo": { before: "GPUCommandBuffer" },
        "SDL_GPUStorageTextureReadWriteBinding": { before: "GPUCommandBuffer" },
        "SDL_GPUStorageBufferReadWriteBinding": { before: "GPUCommandBuffer" },
        "SDL_GPUBlitInfo": { before: "GPUCommandBuffer" },
        "SDL_GPUFence": { before: "GPUCommandBuffer" },
        "SDL_GPUCommandBuffer": { before: "SDL_GPUDevice" },
        "SDL_GPUShaderFormat": {
          before: "SDL_GPUDevice",
          enum: "SDL_GPU_SHADERFORMAT_",
        },
        "SDL_GPUSwapchainComposition": { before: "SDL_GPUDevice" },
        "SDL_GPUPresentMode": { before: "SDL_GPUDevice" },
        "SDL_GPUTextureFormat": { before: "SDL_GPUDevice" },
        "SDL_GPUTextureType": { before: "SDL_GPUDevice" },
        "SDL_GPUTextureUsageFlags": {
          before: "SDL_GPUDevice",
          enum: "SDL_GPU_TEXTUREUSAGE_",
        },
        "SDL_GPUSampleCount": { before: "SDL_GPUDevice" },
        "SDL_GPUBufferUsageFlags": { enum: "SDL_GPU_BUFFERUSAGE_" },
        "SDL_GPUColorComponentFlags": { enum: "SDL_GPU_COLORCOMPONENT_" },
        "SDL_GetGPUShaderFormats": { hints: { methodName: "GetShaderFormats" } },
        "SDL_CreateGPUComputePipeline": { hints: { methodName: "CreateComputePipeline" } },
        "SDL_CreateGPUGraphicsPipeline": { hints: { methodName: "CreateGraphicsPipeline" } },
        "SDL_CreateGPUSampler": { hints: { methodName: "CreateSampler" } },
        "SDL_CreateGPUShader": { hints: { methodName: "CreateShader" } },
        "SDL_CreateGPUTexture": { hints: { methodName: "CreateTexture" } },
        "SDL_CreateGPUBuffer": { hints: { methodName: "CreateBuffer" } },
        "SDL_CreateGPUTransferBuffer": { hints: { methodName: "CreateTransferBuffer" } },
        "SDL_SetGPUBufferName": { hints: { methodName: "SetBufferName" } },
        "SDL_SetGPUTextureName": { hints: { methodName: "SetTextureName" } },
        "SDL_InsertGPUDebugLabel": { hints: { methodName: "InsertDebugLabel" } },
        "SDL_PushGPUDebugGroup": { hints: { methodName: "PushDebugGroup" } },
        "SDL_PopGPUDebugGroup": { hints: { methodName: "PopDebugGroup" } },
        "SDL_ReleaseGPUTexture": { hints: { methodName: "ReleaseTexture" } },
        "SDL_ReleaseGPUSampler": { hints: { methodName: "ReleaseSampler" } },
        "SDL_ReleaseGPUBuffer": { hints: { methodName: "ReleaseBuffer" } },
        "SDL_ReleaseGPUTransferBuffer": { hints: { methodName: "ReleaseTransferBuffer" } },
        "SDL_ReleaseGPUComputePipeline": { hints: { methodName: "ReleaseComputePipeline" } },
        "SDL_ReleaseGPUShader": { hints: { methodName: "ReleaseShader" } },
        "SDL_ReleaseGPUGraphicsPipeline": { hints: { methodName: "ReleaseGraphicsPipeline" } },
        "SDL_AcquireGPUCommandBuffer": { hints: { methodName: "AcquireCommandBuffer" } },
        "SDL_PushGPUVertexUniformData": {
          parameters: [{}, {}, { type: "SourceBytes" }],
          hints: { methodName: "PushVertexUniformData" },
        },
        "SDL_PushGPUFragmentUniformData": {
          parameters: [{}, {}, { type: "SourceBytes" }],
          hints: { methodName: "PushFragmentUniformData" },
        },
        "SDL_PushGPUComputeUniformData": {
          parameters: [{}, {}, { type: "SourceBytes" }],
          hints: { methodName: "PushComputeUniformData" },
        },
        "SDL_BeginGPURenderPass": {
          parameters: [
            {},
            { type: "std::span<const GPUColorTargetInfo>" },
            { type: "OptionalRef<const GPUDepthStencilTargetInfo>", name: "depth_stencil_target_info" },
          ],
          hints: { methodName: "BeginRenderPass" },
        },
        "SDL_BindGPUGraphicsPipeline": { hints: { methodName: "BindPipeline" } },
        "SDL_SetGPUViewport": { hints: { methodName: "SetViewport" } },
        "SDL_SetGPUScissor": { hints: { methodName: "SetScissor" } },
        "SDL_SetGPUBlendConstants": { hints: { methodName: "SetBlendConstants" } },
        "SDL_SetGPUStencilReference": { hints: { methodName: "SetStencilReference" } },
        "SDL_BindGPUVertexBuffers": {
          parameters: [{}, {}, { type: "std::span<const GPUBufferBinding>" }],
          hints: { methodName: "BindVertexBuffers" },
        },
        "SDL_BindGPUIndexBuffer": { hints: { methodName: "BindIndexBuffer" } },
        "SDL_BindGPUVertexSamplers": {
          parameters: [{}, {}, { type: "std::span<const GPUTextureSamplerBinding>" }],
          hints: { methodName: "BindVertexSamplers" },
        },
        "SDL_BindGPUVertexStorageTextures": {
          parameters: [{}, {}, { type: "SpanRef<const GPUTextureRaw>" }],
          hints: { methodName: "BindVertexStorageTextures" },
        },
        "SDL_BindGPUVertexStorageBuffers": {
          parameters: [{}, {}, { type: "SpanRef<const GPUBufferRaw>" }],
          hints: { methodName: "BindVertexStorageBuffers" },
        },
        "SDL_BindGPUFragmentSamplers": {
          parameters: [{}, {}, { type: "std::span<const GPUTextureSamplerBinding>" }],
          hints: { methodName: "BindFragmentSamplers" },
        },
        "SDL_BindGPUFragmentStorageTextures": {
          parameters: [{}, {}, { type: "SpanRef<const GPUTextureRaw>" }],
          hints: { methodName: "BindFragmentStorageTextures" },
        },
        "SDL_BindGPUFragmentStorageBuffers": {
          parameters: [{}, {}, { type: "SpanRef<const GPUBufferRaw>" }],
          hints: { methodName: "BindFragmentStorageBuffers" },
        },
        "SDL_DrawGPUIndexedPrimitives": { hints: { methodName: "DrawIndexedPrimitives" } },
        "SDL_DrawGPUPrimitives": { hints: { methodName: "DrawPrimitives" } },
        "SDL_DrawGPUPrimitivesIndirect": { hints: { methodName: "DrawPrimitivesIndirect" } },
        "SDL_DrawGPUIndexedPrimitivesIndirect": { hints: { methodName: "DrawIndexedPrimitivesIndirect" } },
        "SDL_BeginGPUComputePass": {
          parameters: [
            {},
            { type: "std::span<const GPUStorageTextureReadWriteBinding>" },
            { type: "std::span<const GPUStorageBufferReadWriteBinding>", name: "storage_buffer_bindings" },
          ],
          hints: { methodName: "BeginComputePass" },
        },
        "SDL_BindGPUComputePipeline": { hints: { methodName: "BindPipeline" } },
        "SDL_BindGPUComputeSamplers": {
          parameters: [{}, {}, { type: "std::span<const GPUTextureSamplerBinding>" }],
          hints: { methodName: "BindSamplers" },
        },
        "SDL_BindGPUComputeStorageTextures": {
          parameters: [{}, {}, { type: "SpanRef<const GPUTextureRaw>" }],
          hints: { methodName: "BindStorageTextures" },
        },
        "SDL_BindGPUComputeStorageBuffers": {
          parameters: [{}, {}, { type: "SpanRef<const GPUBufferRaw>" }],
          hints: { methodName: "BindStorageBuffers" },
        },
        "SDL_DispatchGPUCompute": { hints: { methodName: "Dispatch" } },
        "SDL_DispatchGPUComputeIndirect": { hints: { methodName: "DispatchIndirect" } },
        "SDL_MapGPUTransferBuffer": { hints: { methodName: "MapTransferBuffer" } },
        "SDL_UnmapGPUTransferBuffer": { hints: { methodName: "UnmapTransferBuffer" } },
        "SDL_BeginGPUCopyPass": { hints: { methodName: "BeginCopyPass" } },
        "SDL_UploadToGPUTexture": { hints: { methodName: "UploadToTexture" } },
        "SDL_UploadToGPUBuffer": { hints: { methodName: "UploadToBuffer" } },
        "SDL_CopyGPUTextureToTexture": { hints: { methodName: "CopyTextureToTexture" } },
        "SDL_CopyGPUBufferToBuffer": { hints: { methodName: "CopyBufferToBuffer" } },
        "SDL_DownloadFromGPUTexture": { hints: { methodName: "DownloadFromTexture" } },
        "SDL_DownloadFromGPUBuffer": { hints: { methodName: "DownloadFromBuffer" } },
        "SDL_GenerateMipmapsForGPUTexture": { hints: { methodName: "GenerateMipmapsForTexture" } },
        "SDL_BlitGPUTexture": { hints: { methodName: "BlitTexture" } },
        "SDL_WindowSupportsGPUSwapchainComposition": { hints: { methodName: "WindowSupportsSwapchainComposition" } },
        "SDL_WindowSupportsGPUPresentMode": { hints: { methodName: "WindowSupportsPresentMode" } },
        "SDL_ClaimWindowForGPUDevice": { hints: { methodName: "ClaimWindow" } },
        "SDL_ReleaseWindowFromGPUDevice": { hints: { methodName: "ReleaseWindow" } },
        "SDL_SetGPUSwapchainParameters": { hints: { methodName: "SetSwapchainParameters" } },
        "SDL_SetGPUAllowedFramesInFlight": { hints: { methodName: "SetAllowedFramesInFlight" } },
        "SDL_GetGPUSwapchainTextureFormat": { hints: { methodName: "GetSwapchainTextureFormat" } },
        "SDL_AcquireGPUSwapchainTexture": {
          type: "GPUTexture",
          parameters: [
            {}, {},
            { name: "swapchain_texture_width", type: "Uint32 *", default: "nullptr" },
            { name: "swapchain_texture_height", type: "Uint32 *", default: "nullptr" },
          ],
          hints: { methodName: "AcquireSwapchainTexture" },
        },
        "SDL_WaitForGPUSwapchain": { hints: { methodName: "WaitForSwapchain" } },
        "SDL_WaitAndAcquireGPUSwapchainTexture": {
          type: "GPUTexture",
          parameters: [
            {}, {},
            { name: "swapchain_texture_width", type: "Uint32 *", default: "nullptr" },
            { name: "swapchain_texture_height", type: "Uint32 *", default: "nullptr" },
          ],
          hints: { methodName: "WaitAndAcquireSwapchainTexture" },
        },
        "SDL_WaitForGPUIdle": { hints: { methodName: "WaitForIdle" } },
        "SDL_WaitForGPUFences": {
          parameters: [{}, {}, { type: "std::span<GPUFence* const>" }],
          hints: { methodName: "WaitForFences" }
        },
        "SDL_QueryGPUFence": { hints: { methodName: "QueryFence" } },
        "SDL_ReleaseGPUFence": { hints: { methodName: "ReleaseFence" } },
        "SDL_GPUTextureSupportsFormat": { hints: { methodName: "TextureSupportsFormat" } },
        "SDL_GPUTextureSupportsSampleCount": { hints: { methodName: "TextureSupportsSampleCount" } },
        "SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_STENCIL_NUMBER": {
          since: {
            tag: "SDL",
            major: 3,
            minor: 2,
            patch: 12,
          }
        }
      }
    },
    "SDL_guid.h": {
      localIncludes: ['SDL3pp_stdinc.h'],
      transform: {
        "SDL_GUID": {
          wrapper: { genMembers: false },
          entries: {
            "SDL_StringToGUID": "ctor",
            "SDL_GUIDToString": {
              "type": "std::string",
              "immutable": true,
              "parameters": []
            }
          }
        }
      }
    },
    "SDL_joystick.h": {
      localIncludes: [
        "SDL3pp_error.h",
        "SDL3pp_guid.h",
        "SDL3pp_mutex.h",
        "SDL3pp_power.h",
        "SDL3pp_properties.h",
        "SDL3pp_sensor.h",
        "SDL3pp_stdinc.h",
        "SDL3pp_rect.h",
      ],
      ignoreEntries: ["SDL_joystick_lock"],
      namespacesMap: {
        "SDL_PROP_JOYSTICK_CAP_": "prop::JoystickCap",
      },
      transform: {
        "SDL_JoystickID": { before: "SDL_Joystick" },
        "SDL_JoystickType": { before: "JoystickID" },
        "SDL_JoystickConnectionState": { before: "SDL_Joystick" },
        "HatState": {
          kind: "alias",
          type: "Uint8",
          enum: {
            prefix: "SDL_HAT_",
            values: [
              "SDL_HAT_CENTERED",
              "SDL_HAT_UP",
              "SDL_HAT_RIGHT",
              "SDL_HAT_DOWN",
              "SDL_HAT_LEFT",
              "SDL_HAT_RIGHTUP",
              "SDL_HAT_RIGHTDOWN",
              "SDL_HAT_LEFTUP",
              "SDL_HAT_LEFTDOWN",
            ],
          },
          before: "SDL_Joystick",
        },
        "SDL_HAT_CENTERED": { kind: "var", type: "Uint8", constexpr: true, after: "HatState" },
        "SDL_HAT_UP": { kind: "var", type: "Uint8", constexpr: true, after: "HatState" },
        "SDL_HAT_RIGHT": { kind: "var", type: "Uint8", constexpr: true, after: "HatState" },
        "SDL_HAT_DOWN": { kind: "var", type: "Uint8", constexpr: true, after: "HatState" },
        "SDL_HAT_LEFT": { kind: "var", type: "Uint8", constexpr: true, after: "HatState" },
        "SDL_HAT_RIGHTUP": { kind: "var", type: "Uint8", constexpr: true, after: "HatState" },
        "SDL_HAT_RIGHTDOWN": { kind: "var", type: "Uint8", constexpr: true, after: "HatState" },
        "SDL_HAT_LEFTUP": { kind: "var", type: "Uint8", constexpr: true, after: "HatState" },
        "SDL_HAT_LEFTDOWN": { kind: "var", type: "Uint8", constexpr: true, after: "HatState" },
        "SDL_JOYSTICK_AXIS_MAX": {
          kind: "var",
          type: "int",
          constexpr: true,
        },
        "SDL_JOYSTICK_AXIS_MIN": {
          kind: "var",
          type: "int",
          constexpr: true,
        },
        "SDL_GetJoysticks": {
          type: "OwnArray<JoystickID>",
          parameters: [],
        },
      },
    },
    "SDL_haptic.h": {
      localIncludes: ["SDL3pp_error.h", "SDL3pp_joystick.h", "SDL3pp_stdinc.h"],
      transform: {
        "HapticEffectType": {
          before: "SDL_Haptic",
          kind: "alias",
          type: "Uint32",
        },
        "SDL_HAPTIC_CONSTANT": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_SINE": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_SQUARE": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_TRIANGLE": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_SAWTOOTHUP": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_SAWTOOTHDOWN": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_RAMP": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_SPRING": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_DAMPER": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_INERTIA": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_FRICTION": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_LEFTRIGHT": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_RESERVED1": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_RESERVED2": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_RESERVED3": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_CUSTOM": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_GAIN": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_AUTOCENTER": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_STATUS": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "SDL_HAPTIC_PAUSE": { after: "HapticEffectType", kind: "var", type: "HapticEffectType", constexpr: true },
        "HapticDirectionType": {
          before: "SDL_Haptic",
          kind: "alias",
          type: "Uint8",
        },
        "SDL_HAPTIC_POLAR": { after: "HapticDirectionType", kind: "var", type: "HapticDirectionType", constexpr: true },
        "SDL_HAPTIC_CARTESIAN": { after: "HapticDirectionType", kind: "var", type: "HapticDirectionType", constexpr: true },
        "SDL_HAPTIC_SPHERICAL": { after: "HapticDirectionType", kind: "var", type: "HapticDirectionType", constexpr: true },
        "SDL_HAPTIC_STEERING_AXIS": { after: "HapticDirectionType", kind: "var", type: "HapticDirectionType", constexpr: true },
        "SDL_HAPTIC_INFINITY": { before: "SDL_Haptic", kind: "var", type: "Uint32", constexpr: true },
        "SDL_HapticDirection": { before: "SDL_Haptic" },
        "SDL_HapticConstant": { before: "SDL_Haptic" },
        "SDL_HapticPeriodic": { before: "SDL_Haptic" },
        "SDL_HapticCondition": { before: "SDL_Haptic" },
        "SDL_HapticRamp": { before: "SDL_Haptic" },
        "SDL_HapticLeftRight": { before: "SDL_Haptic" },
        "SDL_HapticCustom": { before: "SDL_Haptic" },
        "SDL_HapticEffect": { before: "SDL_Haptic" },
        "SDL_HapticID": { before: "SDL_Haptic" },
        "SDL_Haptic": {
          resource: {
            free: "SDL_CloseHaptic",
            ctors: ["SDL_OpenHapticFromMouse"],
          },
          entries: {
            "SDL_OpenHaptic": "ctor",
            "SDL_OpenHapticFromJoystick": "ctor",
          }
        },
        "SDL_GetHaptics": {
          type: "OwnArray<HapticID>",
          parameters: [],
        },
        "SDL_HapticEffectSupported": {
          parameters: [{}, { type: "const HapticEffect &" }],
        },
        "SDL_CreateHapticEffect": {
          parameters: [{}, { type: "const HapticEffect &" }],
        },
        "SDL_UpdateHapticEffect": {
          parameters: [{}, {}, { type: "const HapticEffect &" }],
        },
      },
    },
    "SDL_hidapi.h": {
      localIncludes: ["SDL3pp_error.h", "SDL3pp_properties.h", "SDL3pp_stdinc.h"],
      transform: {
        "SDL_hid_init": {
          type: "void",
          hints: { mayFail: "0" }
        },
        "SDL_hid_exit": {
          type: "void",
          hints: { mayFail: "0" }
        },
        "SDL_hid_enumerate": {
          hints: { mayFail: true },
        },
        "SDL_hid_bus_type": { before: "SDL_hid_device" },
        "SDL_hid_device_info": { before: "SDL_hid_device" },
        "SDL_hid_device": {
          name: "HidDevice",
          resource: { free: "SDL_hid_close" },
          entries: {
            "SDL_hid_open": "ctor",
            "SDL_hid_open_path": "ctor",
            "SDL_hid_close": {
              name: "close",
              type: "void",
              hints: { mayFail: "0" }
            },
          }
        },
        "SDL_hid_write": {
          parameters: [{}, { type: "SourceBytes" }],
          hints: { methodName: "write" }
        },
        "SDL_hid_read_timeout": {
          parameters: [{}, { type: "TargetBytes" }, { type: "Milliseconds", name: "timeout" }],
          hints: { methodName: "read_timeout" }
        },
        "SDL_hid_read": {
          parameters: [{}, { type: "TargetBytes" }],
          hints: { methodName: "read" }
        },
        "SDL_hid_set_nonblocking": {
          type: "void",
          parameters: [{}, { type: "bool" }],
          hints: {
            methodName: "set_nonblocking",
            mayFail: "0",
          }
        },
        "SDL_hid_send_feature_report": {
          parameters: [{}, { type: "SourceBytes" }],
          hints: { methodName: "send_feature_report" }
        },
        "SDL_hid_get_feature_report": {
          parameters: [{}, { type: "TargetBytes" }],
          hints: { methodName: "get_feature_report" }
        },
        "SDL_hid_get_input_report": {
          parameters: [{}, { type: "TargetBytes" }],
          hints: { methodName: "get_input_report" }
        },
        "SDL_hid_get_manufacturer_string": {
          type: "void",
          hints: {
            methodName: "get_manufacturer_string",
            mayFail: "0",
          }
        },
        "SDL_hid_get_product_string": {
          type: "void",
          hints: {
            methodName: "get_product_string",
            mayFail: "0",
          }
        },
        "SDL_hid_get_serial_number_string": {
          type: "void",
          hints: {
            methodName: "get_serial_number_string",
            mayFail: "0",
          }
        },
        "SDL_hid_get_indexed_string": {
          type: "void",
          hints: {
            methodName: "get_indexed_string",
            mayFail: "0",
          }
        },
        "SDL_hid_get_device_info": {
          hints: {
            methodName: "get_device_info",
            mayFail: true,
          }
        },
        "SDL_hid_get_report_descriptor": {
          parameters: [{}, { type: "TargetBytes" }],
          hints: { methodName: "get_report_descriptor" }
        },
      },
    },
    "SDL_hints.h": {
      localIncludes: ["SDL3pp_callbackWrapper.h", "SDL3pp_error.h", "SDL3pp_strings.h"],
      transform: {
        "SDL_HintPriority": {
          enum: {
            prefix: "SDL_HINT_",
            values: [
              "SDL_HINT_DEFAULT",
              "SDL_HINT_NORMAL",
              "SDL_HINT_OVERRIDE"
            ]
          }
        },
        "SDL_HintCallback": { callback: "lightweight" },
        "AddHintCallback": {
          kind: "function",
          after: "SDL_AddHintCallback",
          type: "void",
          parameters: [{
            type: "StringParam",
            name: "name"
          }, {
            type: "HintCB",
            name: "callback"
          }]
        },
      }
    },
    "SDL_init.h": {
      localIncludes: ["SDL3pp_callbackWrapper.h", "SDL3pp_error.h", "SDL3pp_events.h", "SDL3pp_log.h", "SDL3pp_strings.h"],
      includes: ["atomic"],
      namespacesMap: {
        "SDL_PROP_APP_METADATA_": "prop::appMetaData",
      },
      transform: {
        "SDL_InitFlags": {
          enum: "SDL_INIT_"
        },
        "SDL_MainThreadCallback": { callback: "std" },
        "SDL_RunOnMainThread": {},
        "RunOnMainThread": {
          kind: "function",
          type: "void",
          parameters: [
            {
              type: "MainThreadCB",
              name: "callback"
            },
            {
              type: "bool",
              name: "wait_complete"
            }
          ]
        }
      }
    },
    "SDL_iostream.h": {
      localIncludes: ['SDL3pp_properties.h', 'SDL3pp_stdinc.h'],
      namespacesMap: {
        "SDL_PROP_IOSTREAM_": "prop::IOStream"
      },
      transform: {
        "SDL_IOStream": {
          resource: {
            ctors: ["SDL_IOFromFile", "SDL_IOFromMem", "SDL_IOFromConstMem", "SDL_IOFromDynamicMem", "SDL_OpenIO"],
          },
          entries: {
            "SDL_IOFromFile": { name: "FromFile" },
            "SDL_IOFromMem": {
              name: "FromMem",
              parameters: [{ type: "TargetBytes", name: "mem" }]
            },
            "SDL_IOFromConstMem": {
              name: "FromConstMem",
              parameters: [{ type: "SourceBytes", name: "mem" }]
            },
            "SDL_IOFromDynamicMem": {
              name: "FromDynamicMem",
              static: true,
            },
            "SDL_OpenIO": { name: "Open" },
            "SDL_CloseIO": {
              name: "Close",
              hints: { mayFail: true },
            },
            "SDL_GetIOProperties": {
              "name": "GetProperties",
              "immutable": true
            },
            "SDL_GetIOStatus": {
              "name": "GetStatus",
              "immutable": true
            },
            "SDL_GetIOSize": {
              "name": "GetSize",
              "immutable": true
            },
            "SDL_SeekIO": {
              "name": "Seek"
            },
            "SDL_TellIO": {
              "name": "Tell",
              "immutable": true
            },
            "Read": {
              kind: "function",
              type: "std::string",
              parameters: [{
                type: "size_t",
                name: "size",
                default: "-1"
              }],
            },
            "SDL_ReadIO": {
              name: "Read",
              parameters: [{}, { type: "TargetBytes", name: "buf" }],
            },
            "SDL_WriteIO": {
              name: "Write",
              parameters: [{}, { type: "SourceBytes", name: "buf" }]
            },
            "print": {
              "kind": "function",
              "type": "size_t",
              doc: ["@cat formatted-string"],
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "auto...",
                  "name": "args"
                }
              ]
            },
            "println": {
              "kind": "function",
              "type": "size_t",
              doc: ["@cat formatted-string"],
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "auto...",
                  "name": "args"
                }
              ]
            },
            "SDL_IOprintf": {
              name: "printf",
              proto: false,
            },
            "SDL_IOvprintf": {
              "name": "vprintf"
            },
            "SDL_FlushIO": {
              "name": "Flush"
            },
            "SDL_LoadFile_IO": {
              name: "LoadFile",
              type: "StringResult",
              parameters: [{}]
            },
            "LoadFileAs": {
              kind: "function",
              template: [{ type: "class", name: "T" }],
              type: "OwnArray<T>",
              parameters: []
            },
            "SDL_SaveFile_IO": {
              name: "SaveFile",
              parameters: [{}, { type: "SourceBytes", name: "data" }]
            },
            "SDL_ReadU8": {
              type: "Uint8",
              parameters: []
            },
            "SDL_ReadS8": {
              type: "Sint8",
              parameters: []
            },
            "SDL_ReadU16LE": {
              type: "Uint16",
              parameters: []
            },
            "SDL_ReadS16LE": {
              type: "Sint16",
              parameters: []
            },
            "SDL_ReadU16BE": {
              type: "Uint16",
              parameters: []
            },
            "SDL_ReadS16BE": {
              type: "Sint16",
              parameters: []
            },
            "SDL_ReadU32LE": {
              type: "Uint32",
              parameters: []
            },
            "SDL_ReadS32LE": {
              type: "Sint32",
              parameters: []
            },
            "SDL_ReadU32BE": {
              type: "Uint32",
              parameters: []
            },
            "SDL_ReadS32BE": {
              type: "Sint32",
              parameters: []
            },
            "SDL_ReadU64LE": {
              type: "Uint64",
              parameters: []
            },
            "SDL_ReadS64LE": {
              type: "Sint64",
              parameters: []
            },
            "SDL_ReadU64BE": {
              type: "Uint64",
              parameters: []
            },
            "SDL_ReadS64BE": {
              type: "Sint64",
              parameters: []
            },
            "TryReadU8": {
              kind: "function",
              type: "std::optional<Uint8>",
              parameters: []
            },
            "TryReadS8": {
              kind: "function",
              type: "std::optional<Sint8>",
              parameters: []
            },
            "TryReadU16LE": {
              kind: "function",
              type: "std::optional<Uint16>",
              parameters: []
            },
            "TryReadS16LE": {
              kind: "function",
              type: "std::optional<Sint16>",
              parameters: []
            },
            "TryReadU16BE": {
              kind: "function",
              type: "std::optional<Uint16>",
              parameters: []
            },
            "TryReadS16BE": {
              kind: "function",
              type: "std::optional<Sint16>",
              parameters: []
            },
            "TryReadU32LE": {
              kind: "function",
              type: "std::optional<Uint32>",
              parameters: []
            },
            "TryReadS32LE": {
              kind: "function",
              type: "std::optional<Sint32>",
              parameters: []
            },
            "TryReadU32BE": {
              kind: "function",
              type: "std::optional<Uint32>",
              parameters: []
            },
            "TryReadS32BE": {
              kind: "function",
              type: "std::optional<Sint32>",
              parameters: []
            },
            "TryReadU64LE": {
              kind: "function",
              type: "std::optional<Uint64>",
              parameters: []
            },
            "TryReadS64LE": {
              kind: "function",
              type: "std::optional<Sint64>",
              parameters: []
            },
            "TryReadU64BE": {
              kind: "function",
              type: "std::optional<Uint64>",
              parameters: []
            },
            "TryReadS64BE": {
              kind: "function",
              type: "std::optional<Sint64>",
              parameters: []
            },
            "SDL_WriteU8": "function",
            "SDL_WriteS8": "function",
            "SDL_WriteU16LE": "function",
            "SDL_WriteS16LE": "function",
            "SDL_WriteU16BE": "function",
            "SDL_WriteS16BE": "function",
            "SDL_WriteU32LE": "function",
            "SDL_WriteS32LE": "function",
            "SDL_WriteU32BE": "function",
            "SDL_WriteS32BE": "function",
            "SDL_WriteU64LE": "function",
            "SDL_WriteS64LE": "function",
            "SDL_WriteU64BE": "function",
            "SDL_WriteS64BE": "function",
          }
        },
        "SDL_LoadFile": {
          type: "StringResult",
          parameters: [{}]
        },
        "SDL_LoadFile_IO": {
          name: "LoadFile",
          type: "StringResult",
          parameters: [{}, { name: "closeio", type: "bool", default: "true" }]
        },
        "LoadFileAs": {
          kind: "function",
          template: [{ type: "class", name: "T" }],
          type: "OwnArray<T>",
          parameters: [{
            type: "StringParam",
            name: "file"
          }]
        },
        "SDL_SaveFile": {
          type: "void",
          parameters: [{}, { type: "SourceBytes", name: "data" }]
        },
        "SDL_SaveFile_IO": {
          name: "SaveFile",
          type: "void",
          parameters: [
            {},
            { type: "SourceBytes", name: "data" },
            { name: "closeio", type: "bool", default: "true" }
          ]
        },
      }
    },
    "SDL_keyboard.h": {
      localIncludes: [
        "SDL3pp_keycode.h",
        "SDL3pp_scancode.h",
        "SDL3pp_stdinc.h",
        "SDL3pp_video.h",
      ],
      namespacesMap: {
        "SDL_PROP_TEXTINPUT_": "prop::TextInput"
      },
      transform: {
        "SDL_GetKeyboards": {
          type: "OwnArray<KeyboardID>",
          parameters: [],
        },
        "SDL_GetKeyboardState": {
          type: "std::span<const bool>",
          parameters: [],
        },
        "SDL_GetKeyFromScancode": {
          "name": "Keycode::Keycode"
        },
        "SDL_GetScancodeFromKey": {
          "name": "Keycode::GetScancode",
          "type": "Scancode",
          "immutable": true
        },
        "SDL_SetScancodeName": {
          "name": "Scancode::SetName"
        },
        "SDL_GetScancodeName": {
          "name": "Scancode::GetName",
          "immutable": true
        },
        "SDL_GetScancodeFromName": {
          "name": "Scancode::Scancode"
        },
        "SDL_GetKeyName": {
          "name": "Keycode::GetName",
          "immutable": true
        },
        "SDL_GetKeyFromName": {
          "name": "Keycode::Keycode"
        },
        "SDL_StartTextInput": {
          name: "Window::StartTextInput",
        },
        "SDL_StartTextInputWithProperties": {
          name: "Window::StartTextInput",
        },
        "SDL_TextInputActive": {
          name: "Window::IsTextInputActive",
          immutable: true,
        },
        "SDL_StopTextInput": {
          name: "Window::StopTextInput",
        },
        "SDL_ClearComposition": {
          name: "Window::ClearComposition",
        },
        "SDL_SetTextInputArea": {
          name: "Window::SetTextInputArea",
        },
        "SDL_GetTextInputArea": {
          name: "Window::GetTextInputArea",
        },
        "SDL_ScreenKeyboardShown": {
          name: "Window::IsScreenKeyboardShown",
          immutable: true,
        }
      }
    },
    "SDL_keycode.h": {
      localIncludes: ['SDL3pp_scancode.h', 'SDL3pp_stdinc.h'],
      transform: {
        "SDL_Keymod": {
          enum: {
            prefix: "SDL_KMOD_",
          },
          after: "__begin"
        },
        "SDL_Keycode": {
          enum: {
            prefix: "SDLK_",
            newPrefix: "KEYCODE_"
          },
          wrapper: { ordered: true },
          entries: {
            "SDLK_EXTENDED_MASK": {
              kind: "function",
              name: "IsExtended",
              type: "bool",
              constexpr: true,
              immutable: true,
              parameters: []
            },
            "SDLK_SCANCODE_MASK": {
              kind: "function",
              name: "IsScancode",
              type: "bool",
              constexpr: true,
              immutable: true,
              parameters: []
            }
          }
        },
        "SDL_SCANCODE_TO_KEYCODE": {
          name: "ScancodeToKeycode",
          kind: "function",
          type: "Keycode",
          constexpr: true,
          parameters: [
            {
              type: "Scancode",
              name: "x"
            }
          ]
        }
      }
    },
    "SDL_loadso.h": {
      localIncludes: ['SDL3pp_stdinc.h'],
      transform: {
        "SDL_SharedObject": {
          resource: { free: "SDL_UnloadObject" },
          entries: {
            "SDL_LoadObject": "ctor",
            "SDL_LoadFunction": "function",
            "SDL_UnloadObject": { name: "Unload" },
          }
        }
      },
    },
    "SDL_locale.h": {
      localIncludes: ['SDL3pp_stdinc.h'],
      transform: {
        "SDL_GetPreferredLocales": {
          type: "OwnArray<Locale *>",
          parameters: []
        }
      }
    },
    "SDL_log.h": {
      includes: ["format"],
      localIncludes: ["SDL3pp_callbackWrapper.h", "SDL3pp_error.h", "SDL3pp_strings.h"],
      transform: {
        "SDL_LogPriority": {
          after: "__begin",
          enum: "SDL_LOG_PRIORITY_"
        },
        "ResetLogOutputFunction": {
          kind: "function",
          type: "void",
          after: "SDL_SetLogOutputFunction",
          parameters: []
        },
        "SDL_LogCategory": {
          kind: "struct",
          type: "",
          enum: {
            prefix: "SDL_LOG_CATEGORY_",
            valueType: "LogCategory"
          },
          wrapper: {
            attribute: 'category',
            ordered: true,
          },
          entries: {
            "LogCategory": {
              kind: "function",
              type: "",
              explicit: true,
              constexpr: true,
              parameters: [{
                type: "int",
                name: "category"
              }]
            },
            "SDL_SetLogPriority": {
              "static": false,
              "immutable": true,
              "parameters": [
                {
                  "type": "LogPriority",
                  "name": "priority"
                }
              ]
            },
            "SDL_GetLogPriority": {
              "immutable": true,
              "parameters": []
            },
            "LogUnformatted": {
              "kind": "function",
              "immutable": true,
              "type": "void",
              "parameters": [
                {
                  "type": "LogPriority",
                  "name": "priority"
                },
                {
                  "type": "StringParam",
                  "name": "message"
                }
              ]
            },
            "SDL_LogMessage": {
              "immutable": true,
              "after": "SDL_Log",
              "template": [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              "parameters": [
                {
                  "type": "LogPriority",
                  "name": "priority"
                },
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "ARGS...",
                  "name": "args"
                }
              ]
            },
            "SDL_LogTrace": {
              "immutable": true,
              "template": [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "ARGS &&...",
                  "name": "args"
                }
              ]
            },
            "SDL_LogVerbose": {
              "immutable": true,
              "template": [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "ARGS &&...",
                  "name": "args"
                }
              ]
            },
            "SDL_LogDebug": {
              "immutable": true,
              "template": [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "ARGS &&...",
                  "name": "args"
                }
              ]
            },
            "SDL_LogInfo": {
              "immutable": true,
              "template": [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "ARGS &&...",
                  "name": "args"
                }
              ]
            },
            "SDL_LogWarn": {
              "immutable": true,
              "template": [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "ARGS &&...",
                  "name": "args"
                }
              ]
            },
            "SDL_LogError": {
              "immutable": true,
              "template": [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "ARGS &&...",
                  "name": "args"
                }
              ]
            },
            "SDL_LogCritical": {
              "immutable": true,
              "template": [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "fmt"
                },
                {
                  "type": "ARGS &&...",
                  "name": "args"
                }
              ]
            }
          }
        },
        "SDL_Log": {
          "template": [
            {
              "type": "class...",
              "name": "ARGS"
            }
          ],
          "parameters": [
            {
              "type": "std::string_view",
              "name": "fmt"
            },
            {
              "type": "ARGS &&...",
              "name": "args"
            }
          ]
        },
        "SDL_LogMessage": {
          template: [{
            type: "class...",
            name: "ARGS"
          }],
          parameters: [{
            type: "LogCategory",
            name: "category"
          },
          {
            type: "LogPriority",
            name: "priority"
          },
          {
            type: "std::string_view",
            name: "fmt"
          },
          {
            type: "ARGS...",
            name: "args"
          }]
        },
        "SDL_LogTrace": {
          template: [{
            type: "class...",
            name: "ARGS"
          }],
          parameters: [{
            type: "LogCategory",
            name: "category"
          },
          {
            type: "std::string_view",
            name: "fmt"
          },
          {
            type: "ARGS &&...",
            name: "args"
          }]
        },
        "SDL_LogVerbose": {
          template: [{
            type: "class...",
            name: "ARGS"
          }],
          parameters: [{
            type: "LogCategory",
            name: "category"
          },
          {
            type: "std::string_view",
            name: "fmt"
          },
          {
            type: "ARGS &&...",
            name: "args"
          }]
        },
        "SDL_LogDebug": {
          template: [{
            type: "class...",
            name: "ARGS"
          }],
          parameters: [{
            type: "LogCategory",
            name: "category"
          },
          {
            type: "std::string_view",
            name: "fmt"
          },
          {
            type: "ARGS &&...",
            name: "args"
          }]
        },
        "SDL_LogInfo": {
          template: [{
            type: "class...",
            name: "ARGS"
          }],
          parameters: [{
            type: "LogCategory",
            name: "category"
          },
          {
            type: "std::string_view",
            name: "fmt"
          },
          {
            type: "ARGS &&...",
            name: "args"
          }]
        },
        "SDL_LogWarn": {
          template: [{
            type: "class...",
            name: "ARGS"
          }],
          parameters: [{
            type: "LogCategory",
            name: "category"
          },
          {
            type: "std::string_view",
            name: "fmt"
          },
          {
            type: "ARGS &&...",
            name: "args"
          }]
        },
        "SDL_LogError": {
          template: [{
            type: "class...",
            name: "ARGS"
          }],
          parameters: [{
            type: "LogCategory",
            name: "category"
          },
          {
            type: "std::string_view",
            name: "fmt"
          },
          {
            type: "ARGS &&...",
            name: "args"
          }]
        },
        "SDL_LogCritical": {
          template: [{
            type: "class...",
            name: "ARGS"
          }],
          parameters: [{
            type: "LogCategory",
            name: "category"
          },
          {
            type: "std::string_view",
            name: "fmt"
          },
          {
            type: "ARGS &&...",
            name: "args"
          }]
        },
        "SDL_LogMessageV": {
          "name": "LogUnformatted",
          "parameters": [
            {
              "type": "StringParam",
              "name": "message"
            }
          ]
        },
        "SDL_LogOutputFunction": { callback: "lightweight" },
        "SetLogOutputFunction": {
          kind: "function",
          after: "SDL_SetLogOutputFunction",
          type: "void",
          parameters: [{ type: "LogOutputCB", name: "callback" }]
        },
      }
    },
    "SDL_main.h": {
      localIncludes: ["SDL3pp.h"],
      definitionPrefix: "SDL3PP_",
      ignoreEntries: [
        "main",
        "SDLMAIN_DECLSPEC",
        "SDL_main",
        "SDL_MAIN_AVAILABLE",
        "SDL_MAIN_NEEDED",
        "SDL_PLATFORM_PRIVATE_MAIN",
        "SDL_MAIN_EXPORTED",
        "SDL_PS2_SKIP_IOP_RESET",
        "SDL_AppInit",
        "SDL_AppIterate",
        "SDL_AppEvent",
        "SDL_AppQuit"
      ],
      enableException: false,
      transform: {}
    },
    "SDL_messagebox.h": {
      localIncludes: ["SDL3pp_stdinc.h", "SDL3pp_video.h"],
      transform: {
        "SDL_MessageBoxFlags": {
          enum: {
            values: [
              "SDL_MESSAGEBOX_ERROR",
              "SDL_MESSAGEBOX_WARNING",
              "SDL_MESSAGEBOX_INFORMATION",
              "SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT",
              "SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT"
            ]
          }
        },
        "SDL_MessageBoxButtonFlags": {
          enum: "SDL_MESSAGEBOX_BUTTON_"
        },
        "SDL_MessageBoxColorType": {
          enum: "SDL_MESSAGEBOX_COLOR_"
        },
        "SDL_MessageBoxData": {
          name: "MessageBox",
          wrapper: true,
          entries: {
            "SDL_ShowMessageBox": "function"
          }
        },
        "SDL_ShowSimpleMessageBox": {
          parameters: [{}, {}, {}, { type: "WindowParam" }]
        }
      }
    },
    "SDL_metal.h": {
      localIncludes: ["SDL3pp_video.h"],
      transform: {
        "SDL_Metal_GetLayer": {
          hints: { methodName: "GetLayer" }
        },
      }
    },
    "SDL_mouse.h": {
      localIncludes: ["SDL3pp_stdinc.h", "SDL3pp_video.h"],
      transform: {
        "SDL_SystemCursor": {
          enum: "SDL_SYSTEM_CURSOR_",
          after: "__begin",
        },
        "MouseButton": {
          kind: "alias",
          type: "Uint8",
          enum: {
            prefix: "SDL_BUTTON_",
            values: [
              "SDL_BUTTON_LEFT",
              "SDL_BUTTON_MIDDLE",
              "SDL_BUTTON_RIGHT",
              "SDL_BUTTON_X1",
              "SDL_BUTTON_X2",
            ],
          },
          before: "SDL_MouseButtonFlags",
        },
        "SDL_MouseButtonFlags": {
          enum: {
            prefix: "SDL_BUTTON_",
            values: [
              "SDL_BUTTON_LMASK",
              "SDL_BUTTON_MMASK",
              "SDL_BUTTON_RMASK",
              "SDL_BUTTON_X1MASK",
              "SDL_BUTTON_X2MASK",
            ],
          }
        },
        "SDL_Cursor": {
          resource: true,
          entries: {
            "SDL_CreateCursor": {
              name: "ctor",
              parameters: [
                { type: "const Uint8 *" },
                { type: "const Uint8 *" },
                { type: "const PointRaw &", name: "size" },
                { type: "const PointRaw &", name: "hot" },
              ],
            },
            "SDL_CreateColorCursor": {
              name: "ctor",
              parameters: [
                {},
                { type: "const PointRaw &", name: "hot" },
              ],
            },
            "SDL_CreateSystemCursor": "ctor",
          }
        },
        "SDL_BUTTON_MASK": {
          kind: "function",
          name: "ButtonMask",
          constexpr: true,
          type: "MouseButtonFlags",
          parameters: [{
            type: "MouseButton",
            name: "button"
          }]
        },
        "SDL_GetMice": {
          type: "OwnArray<MouseID>",
          parameters: []
        },
        "SDL_WarpMouseInWindow": {
          name: "Window::WarpMouse"
        },
        "SDL_WarpMouseGlobal": {
          name: "WarpMouse"
        },
        "SDL_SetWindowRelativeMouseMode": {
          name: "Window::SetRelativeMouseMode"
        },
        "SDL_GetWindowRelativeMouseMode": {
          name: "Window::GetRelativeMouseMode",
          immutable: true,
        }
      }
    },
    "SDL_mutex.h": {
      localIncludes: ["SDL3pp_stdinc.h", "SDL3pp_thread.h"],
      ignoreEntries: [
        "SDL_THREAD_ANNOTATION_ATTRIBUTE__",
        "SDL_CAPABILITY",
        "SDL_SCOPED_CAPABILITY",
        "SDL_GUARDED_BY",
        "SDL_PT_GUARDED_BY",
        "SDL_ACQUIRED_BEFORE",
        "SDL_ACQUIRED_AFTER",
        "SDL_REQUIRES",
        "SDL_REQUIRES_SHARED",
        "SDL_ACQUIRE",
        "SDL_ACQUIRE_SHARED",
        "SDL_RELEASE",
        "SDL_RELEASE_SHARED",
        "SDL_RELEASE_GENERIC",
        "SDL_TRY_ACQUIRE",
        "SDL_TRY_ACQUIRE_SHARED",
        "SDL_EXCLUDES",
        "SDL_ASSERT_CAPABILITY",
        "SDL_ASSERT_SHARED_CAPABILITY",
        "SDL_RETURN_CAPABILITY",
        "SDL_NO_THREAD_SAFETY_ANALYSIS",
      ],
      transform: {
        "SDL_Mutex": {
          resource: true,
          entries: {
            "SDL_CreateMutex": "ctor",
            "SDL_LockMutex": {
              parameters: [{
                type: "SDL_Mutex *"
              }]
            },
            "SDL_TryLockMutex": {
              parameters: [{
                type: "SDL_Mutex *"
              }]
            },
            "SDL_UnlockMutex": {
              parameters: [{
                type: "SDL_Mutex *"
              }]
            },
          }
        },
        "SDL_RWLock": {
          resource: true,
          entries: {
            "SDL_CreateRWLock": "ctor",
            "SDL_LockRWLockForReading": {
              parameters: [{
                type: "SDL_RWLock *"
              }]
            },
            "SDL_LockRWLockForWriting": {
              parameters: [{
                type: "SDL_RWLock *"
              }]
            },
            "SDL_TryLockRWLockForReading": {
              parameters: [{
                type: "SDL_RWLock *"
              }]
            },
            "SDL_TryLockRWLockForWriting": {
              parameters: [{
                type: "SDL_RWLock *"
              }]
            },
            "SDL_UnlockRWLock": {
              parameters: [{
                type: "SDL_RWLock *"
              }]
            },
          }
        },
        "SDL_Semaphore": {
          resource: true,
          entries: {
            "SDL_CreateSemaphore": "ctor",
            "SDL_WaitSemaphore": "function",
            "SDL_TryWaitSemaphore": "function",
            "SDL_WaitSemaphoreTimeout": {
              parameters: [
                {},
                {
                  type: "std::chrono::milliseconds",
                  name: "timeout"
                },
              ]
            },
            "SDL_SignalSemaphore": "function",
            "SDL_GetSemaphoreValue": "immutable",
          }
        },
        "SDL_Condition": {
          resource: true,
          entries: {
            "SDL_CreateCondition": "ctor",
            "SDL_SignalCondition": "function",
            "SDL_BroadcastCondition": "function",
            "SDL_WaitCondition": "function",
            "SDL_WaitConditionTimeout": {
              parameters: [
                {},
                {},
                {
                  type: "std::chrono::milliseconds",
                  name: "timeout"
                },
              ]
            },
          }
        },
        "SDL_InitStatus": {
          enum: "INIT_STATUS_",
        },
        "SDL_InitState": {
          wrapper: {
            invalidState: false,
            genCtor: false,
            genMembers: false,
            comparable: false,
          },
          entries: {
            "InitState": {
              kind: "function",
              type: "",
              constexpr: true,
              parameters: [],
              hints: { init: ["SDL_InitState{0}"] }
            },
            "SDL_ShouldInit": "function",
            "SDL_ShouldQuit": "function",
            "SDL_SetInitialized": "function",
          }
        }
      }
    },
    "SDL_pen.h": {
      localIncludes: ["SDL3pp_mouse.h", "SDL3pp_touch.h"],
      transform: {
        "SDL_PEN_MOUSEID": {
          kind: "var",
          constexpr: true,
          type: "MouseID",
        },
        "SDL_PEN_TOUCHID": {
          kind: "var",
          constexpr: true,
          type: "TouchID",
        },
        "SDL_PenInputFlags": {
          enum: "SDL_PEN_INPUT_",
        }
      }
    },
    "SDL_pixels.h": {
      localIncludes: ["SDL3pp_spanRef.h", "SDL3pp_error.h", "SDL3pp_version.h"],
      transform: {
        "SDL_PixelFormatDetails": {
          kind: "alias",
          type: "SDL_PixelFormatDetails",
          after: "__begin"
        },
        "SDL_ALPHA_OPAQUE": {
          kind: "var",
          type: "Uint8",
          constexpr: true,
        },
        "SDL_ALPHA_OPAQUE_FLOAT": {
          kind: "var",
          type: "float",
          constexpr: true,
        },
        "SDL_ALPHA_TRANSPARENT": {
          kind: "var",
          type: "Uint8",
          constexpr: true,
        },
        "SDL_ALPHA_TRANSPARENT_FLOAT": {
          kind: "var",
          type: "float",
          constexpr: true,
        },
        "SDL_PixelFormat": {
          kind: 'struct',
          enum: "SDL_PIXELFORMAT_",
          before: "SDL_DEFINE_PIXELFOURCC",
          wrapper: {
            attribute: "format",
          },
          entries: {
            "SDL_DEFINE_PIXELFORMAT": {
              kind: "function",
              name: "ctor",
              type: "",
              constexpr: true,
              after: "SDL_PixelFormat",
              parameters: [
                {
                  "type": "PixelType",
                  "name": "type"
                },
                {
                  "type": "int",
                  "name": "order"
                },
                {
                  "type": "PackedLayout",
                  "name": "layout"
                },
                {
                  "type": "int",
                  "name": "bits"
                },
                {
                  "type": "int",
                  "name": "bytes"
                }
              ]
            },
            "SDL_GetPixelFormatForMasks": {
              kind: "function",
              name: "ForMasks",
              static: true,
            },
          }
        },
        "SDL_PIXELFORMAT_MJPG": {
          since: { tag: "SDL", major: 3, minor: 2, patch: 10 },
        },
        "SDL_DEFINE_PIXELFOURCC": {
          kind: "function",
          name: "DefinePixelFourCC",
          type: "Uint32",
          constexpr: true,
          parameters: [{
            name: "A",
            type: "Uint8"
          }, {
            name: "B",
            type: "Uint8"
          }, {
            name: "C",
            type: "Uint8"
          }, {
            name: "D",
            type: "Uint8"
          }]
        },
        "SDL_DEFINE_PIXELFORMAT": {
          name: "DefinePixelFormat",
        },
        "SDL_PIXELFLAG": {
          kind: "function",
          name: "PixelFormatFlags",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "Uint8",
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetFlags" },
        },
        "SDL_PIXELTYPE": {
          kind: "function",
          name: "PixelFormatType",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "PixelType",
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetType" },
        },
        "SDL_PIXELORDER": {
          kind: "function",
          name: "PixelFormatOrder",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "int",
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetOrder" },
        },
        "SDL_PIXELLAYOUT": {
          kind: "function",
          name: "PixelFormatLayout",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "PackedLayout",
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetLayout" },
        },
        "SDL_BITSPERPIXEL": {
          kind: "function",
          name: "PixelFormatBitsPerPixel",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "int",
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetBitsPerPixel" },
        },
        "SDL_BYTESPERPIXEL": {
          kind: "function",
          name: "PixelFormatBytesPerPixel",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "int",
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetBytesPerPixel" },
        },
        "SDL_ISPIXELFORMAT_INDEXED": {
          kind: "function",
          name: "IsPixelFormatIndexed",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "bool",
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsIndexed" },
        },
        "SDL_ISPIXELFORMAT_PACKED": {
          kind: "function",
          name: "IsPixelFormatPacked",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "bool",
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsPacked" },
        },
        "SDL_ISPIXELFORMAT_ARRAY": {
          kind: "function",
          name: "IsPixelFormatArray",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "bool",
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsArray" },
        },
        "SDL_ISPIXELFORMAT_10BIT": {
          kind: "function",
          name: "IsPixelFormat10Bit",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "bool",
          immutable: true,
          constexpr: true,
          hints: { methodName: "Is10Bit" },
        },
        "SDL_ISPIXELFORMAT_FLOAT": {
          kind: "function",
          name: "IsPixelFormatFloat",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "bool",
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsFloat" },
        },
        "SDL_ISPIXELFORMAT_ALPHA": {
          kind: "function",
          name: "IsPixelFormatAlpha",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "bool",
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsAlpha" },
        },
        "SDL_ISPIXELFORMAT_FOURCC": {
          kind: "function",
          name: "IsPixelFormatFourCC",
          parameters: [{ type: "PixelFormatRaw" }],
          type: "bool",
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsFourCC" },
        },
        "SDL_Colorspace": {
          kind: "struct",
          enum: "SDL_COLORSPACE_",
          wrapper: {
            attribute: "cspace",
          },
          before: "SDL_DEFINE_COLORSPACE",
          entries: {
            "SDL_DEFINE_COLORSPACE": {
              "kind": "function",
              "name": "ctor",
              "type": "",
              "constexpr": true,
              "parameters": [
                {
                  "type": "ColorType",
                  "name": "type"
                },
                {
                  "type": "ColorRange",
                  "name": "range"
                },
                {
                  "type": "ColorPrimaries",
                  "name": "primaries"
                },
                {
                  "type": "TransferCharacteristics",
                  "name": "transfer"
                },
                {
                  "type": "MatrixCoefficients",
                  "name": "matrix"
                },
                {
                  "type": "ChromaLocation",
                  "name": "chroma"
                }
              ]
            },
          }
        },
        "SDL_DEFINE_COLORSPACE": {
          name: "DefineColorspace",
        },
        "SDL_COLORSPACETYPE": {
          kind: "function",
          name: "ColorspaceType",
          type: "ColorType",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetType" },
        },
        "SDL_COLORSPACERANGE": {
          kind: "function",
          name: "ColorspaceRange",
          type: "ColorRange",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetRange" },
        },
        "SDL_COLORSPACECHROMA": {
          kind: "function",
          name: "ColorspaceChroma",
          type: "ChromaLocation",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetChroma" },
        },
        "SDL_COLORSPACEPRIMARIES": {
          kind: "function",
          name: "ColorspacePrimaries",
          type: "ColorPrimaries",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetPrimaries" },
        },
        "SDL_COLORSPACETRANSFER": {
          kind: "function",
          name: "ColorspaceTransfer",
          type: "TransferCharacteristics",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetTransfer" },
        },
        "SDL_COLORSPACEMATRIX": {
          kind: "function",
          name: "ColorspaceMatrix",
          type: "MatrixCoefficients",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "GetMatrix" },
        },
        "SDL_ISCOLORSPACE_MATRIX_BT601": {
          kind: "function",
          name: "IsColorspaceMatrixBT601",
          type: "bool",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsMatrixBT601" },
        },
        "SDL_ISCOLORSPACE_MATRIX_BT709": {
          kind: "function",
          name: "IsColorspaceMatrixBT709",
          type: "bool",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsMatrixBT709" },
        },
        "SDL_ISCOLORSPACE_MATRIX_BT2020_NCL": {
          kind: "function",
          name: "IsColorspaceMatrixBT2020_NCL",
          type: "bool",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsMatrixBT2020_NCL" },
        },
        "SDL_ISCOLORSPACE_LIMITED_RANGE": {
          kind: "function",
          name: "IsColorspaceLimitedRange",
          type: "bool",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsLimitedRange" },
        },
        "SDL_ISCOLORSPACE_FULL_RANGE": {
          kind: "function",
          name: "IsColorspaceFullRange",
          type: "bool",
          parameters: [{ type: "ColorspaceRaw" }],
          immutable: true,
          constexpr: true,
          hints: { methodName: "IsFullRange" },
        },
        "SDL_Color": {
          wrapper: {
            ordered: true,
            invalidState: false,
            paramType: 'ColorRaw',
          }
        },
        "SDL_FColor": {
          wrapper: {
            attribute: "color",
            ordered: true,
            invalidState: false
          }
        },
        "SDL_Palette": {
          resource: {
            shared: 'refcount',
          },
          entries: {
            "SDL_CreatePalette": "ctor",
            "data": {
              kind: "function",
              type: "const ColorRaw *",
              immutable: true,
              constexpr: true,
              parameters: []
            },
            "size": {
              kind: "function",
              type: "int",
              immutable: true,
              constexpr: true,
              parameters: []
            },
            "operator[]": {
              kind: "function",
              type: "ColorRaw",
              immutable: true,
              constexpr: true,
              parameters: [
                {
                  type: "int",
                  name: "index"
                }
              ]
            }
          }
        },
        "SDL_SetPaletteColors": {
          parameters: [
            {},
            {
              name: "colors",
              type: "SpanRef<const ColorRaw>"
            },
            {
              name: "firstcolor",
              type: "int",
              default: "0"
            }
          ]
        },
        "SDL_GetPixelFormatName": {
          immutable: true,
        },
        "SDL_GetMasksForPixelFormat": {
          immutable: true,
          hints: { methodName: "GetMasks" },
        },
        "SDL_GetPixelFormatDetails": {
          immutable: true,
          hints: { mayFail: true },
        },
        "PixelFormat::operator const PixelFormatDetails &": {
          kind: "function",
          type: "",
          parameters: [],
          immutable: true,
          doc: ["Same as GetDetails()"],
          hints: { delegate: "GetDetails" },
        },
        "MapRGBA": {
          before: "SDL_MapRGBA",
          kind: "function",
          type: "Uint32",
          parameters: [
            { type: "const PixelFormatDetails &", name: "format" },
            { type: "Uint8", name: "r" },
            { type: "Uint8", name: "g" },
            { type: "Uint8", name: "b" },
            { type: "Uint8", name: "a" },
            { type: "PaletteConstParam", name: "palette", default: "{}" },
          ]
        },
        "SDL_MapRGBA": {
          name: "MapColor",
          parameters: [
            { type: "const PixelFormatDetails &", name: "format" },
            { type: "ColorRaw", name: "c" },
            { type: "PaletteConstParam", name: "palette", default: "{}" },
          ]
        },
        "Color::Map": {
          kind: "function",
          type: "Uint32",
          immutable: true,
          parameters: [
            { type: "const PixelFormatDetails &", name: "format" },
            { type: "PaletteConstParam", name: "palette", default: "{}" },
          ]
        },
        "PixelFormat::Map": {
          kind: "function",
          type: "Uint32",
          immutable: true,
          parameters: [
            { type: "ColorRaw", name: "c" },
            { type: "PaletteConstParam", name: "palette", default: "{}" },
          ]
        },
        "GetColor": {
          after: "SDL_GetRGBA",
          kind: "function",
          type: "Color",
          parameters: [
            { type: "Uint32", name: "pixel" },
            { type: "const PixelFormatDetails &", name: "format" },
            { type: "PaletteConstParam", name: "palette", default: "{}" },
          ],
        },
        "Color::Get": {
          kind: "function",
          type: "Color",
          static: true,
          parameters: [
            { type: "Uint32", name: "pixel" },
            { type: "const PixelFormatDetails &", name: "format" },
            { type: "PaletteConstParam", name: "palette", default: "{}" },
          ],
        },
        "PixelFormat::Get": {
          kind: "function",
          type: "Color",
          immutable: true,
          parameters: [
            { type: "Uint32", name: "pixel" },
            { type: "PaletteConstParam", name: "palette", default: "{}" },
          ],
        },
      }
    },
    "SDL_process.h": {
      localIncludes: ["SDL3pp_iostream", "SDL3pp_properties.h", "SDL3pp_stdinc.h"],
      transform: {
        "SDL_ProcessIO": {
          enum: "SDL_PROCESS_STDIO_",
          after: "__begin",
        },
        "SDL_Process": {
          resource: { free: "SDL_DestroyProcess" },
          entries: {
            "SDL_CreateProcess": "ctor",
            "SDL_CreateProcessWithProperties": "ctor",
            "SDL_GetProcessProperties": "immutable",
            "SDL_ReadProcess": {
              type: "StringResult",
              parameters: [
                {
                },
                {
                  type: "int *",
                  name: "exitcode",
                  default: "nullptr",
                }
              ]
            },
            "ReadAs": {
              kind: "function",
              type: "OwnArray<T>",
              template: [{ type: "class", name: "T" }],
              parameters: [{
                type: "int *",
                name: "exitcode",
                default: "nullptr",
              }]
            },
            "SDL_GetProcessInput": "function",
            "SDL_GetProcessOutput": "function",
            "SDL_KillProcess": "function",
            "SDL_WaitProcess": "function",
          }
        }
      },
      namespacesMap: {
        "SDL_PROP_PROCESS_": "prop::process"
      }
    },
    "SDL_properties.h": {
      localIncludes: ['SDL3pp_callbackWrapper.h', 'SDL3pp_error.h', 'SDL3pp_strings.h'],
      transform: {
        "SDL_PropertiesID": {
          name: "Properties",
          resource: {
            ctors: ["SDL_CreateProperties"],
            free: "SDL_DestroyProperties",
            rawName: "PropertiesID",
          },
          entries: {
            "SDL_CreateProperties": {
              name: "Create",
            },
          },
        },
        "SDL_PropertyType": {
          enum: "SDL_PROPERTY_TYPE_",
          before: "SDL_PropertiesID",
        },
        "SDL_EnumeratePropertiesCallback": {
          kind: "alias",
          before: "SDL_PropertiesID",
          callback: "std",
        },
        "SDL_CleanupPropertyCallback": {
          kind: "alias",
          before: "SDL_PropertiesID",
          callback: "std",
        },
        "SetPointerPropertyWithCleanup": {
          kind: "function",
          static: false,
          type: "void",
          after: "SDL_SetPointerPropertyWithCleanup",
          parameters: [{
            type: "PropertiesParam",
            name: "props"
          }, {
            type: "StringParam",
            name: "name"
          },
          {
            type: "void *",
            name: "value"
          },
          {
            type: "CleanupPropertyCB",
            name: "cleanup"
          }]
        },
        "EnumerateProperties": {
          kind: "function",
          type: "void",
          after: "SDL_EnumerateProperties",
          parameters: [{
            type: "PropertiesParam",
            name: "props"
          }, {
            type: "EnumeratePropertiesCB",
            name: "callback"
          }]
        },
        "CountProperties": {
          kind: "function",
          type: "Uint64",
          parameters: [{
            type: "PropertiesParam",
            name: "props"
          }],
          hints: { methodName: "GetCount" },
        },
      }
    },
    "SDL_rect.h": {
      localIncludes: ['SDL3pp_error.h', 'SDL3pp_optionalRef.h', 'SDL3pp_spanRef.h', 'SDL3pp_stdinc.h'],
      transform: {
        "SDL_Point": {
          wrapper: {
            attribute: "p",
            comparable: true,
          },
          entries: {
            "Point": {
              kind: "function",
              type: "",
              constexpr: true,
              explicit: true,
              parameters: [
                {
                  type: "const FPointRaw &",
                  name: "p"
                }
              ]
            },
            "SDL_PointInRect": {
              kind: "function",
              name: "InRect",
              constexpr: true,
              parameters: [
                {
                  name: "self"
                },
                {
                  type: "const RectRaw &"
                }
              ]
            }
          }
        },
        "SDL_FPoint": {
          wrapper: {
            attribute: "p",
            comparable: true,
          },
          entries: {
            "SDL_PointInRectFloat": {
              kind: "function",
              name: "InRect",
              constexpr: true,
              parameters: [
                {
                  name: "self"
                },
                {
                  type: "const FRectRaw &"
                }
              ]
            }
          }
        },
        "SDL_Rect": {
          wrapper: {
            attribute: "r",
            comparable: true,
          },
          entries: {
            "Rect": {
              kind: "function",
              type: "",
              parameters: [
                {
                  name: "corner",
                  type: "const PointRaw &"
                },
                {
                  name: "size",
                  type: "const PointRaw &"
                }
              ]
            },
            "SDL_GetRectEnclosingPoints": {
              type: "Rect",
              static: true,
              parameters: [
                {
                  type: "SpanRef<const PointRaw>",
                  name: "points"
                },
                {
                  type: "OptionalRef<const RectRaw>",
                  name: "clip",
                  default: "std::nullopt"
                }
              ]
            },
            "FromCenter": {
              kind: "function",
              name: "FromCenter",
              constexpr: true,
              static: true,
              type: "Rect",
              parameters: [
                {
                  name: "cx",
                  type: "int"
                },
                {
                  name: "cy",
                  type: "int"
                },
                {
                  name: "w",
                  type: "int"
                },
                {
                  name: "h",
                  type: "int"
                }
              ]
            },
            "FromCenter#2": {
              kind: "function",
              name: "FromCenter",
              constexpr: true,
              static: true,
              type: "Rect",
              parameters: [
                {
                  name: "center",
                  type: "const Point &"
                },
                {
                  name: "size",
                  type: "const Point &"
                }
              ]
            },
            "FromCorners": {
              kind: "function",
              name: "FromCorners",
              static: true,
              constexpr: true,
              type: "Rect",
              parameters: [
                {
                  name: "x1",
                  type: "int"
                },
                {
                  name: "y1",
                  type: "int"
                },
                {
                  name: "x2",
                  type: "int"
                },
                {
                  name: "y2",
                  type: "int"
                }
              ]
            },
            "FromCorners#2": {
              kind: "function",
              name: "FromCorners",
              constexpr: true,
              static: true,
              type: "Rect",
              parameters: [
                {
                  name: "p1",
                  type: "const PointRaw &"
                },
                {
                  name: "p2",
                  type: "const PointRaw &"
                }
              ]
            },
            "GetX2": "function",
            "SetX2": "function",
            "GetY2": "function",
            "SetY2": "function",
            "GetTopLeft": "function",
            "GetTopRight": "function",
            "GetBottomLeft": "function",
            "GetBottomRight": "function",
            "GetSize": "function",
            "GetCentroid": "function",
            "GetLineIntersection": {
              kind: "function",
              type: "bool",
              parameters: [{
                type: "PointRaw *",
                name: "p1"
              }, {
                type: "PointRaw *",
                name: "p2"
              }]
            },
            "SDL_GetRectAndLineIntersection": {
              kind: "function",
              name: "GetLineIntersection"
            },
            "SDL_RectToFRect": {
              name: "operator SDL_FRect",
              static: false,
              immutable: true,
              constexpr: true,
              parameters: [],
              type: ""
            },
            "operator FRect": {
              kind: "function",
              doc: ["@sa operator ToFRect()"],
              name: "operator FRect",
              static: false,
              immutable: true,
              constexpr: true,
              parameters: [],
              type: ""
            },
            "SDL_RectEmpty": {
              constexpr: true
            },
            "operator bool": {
              doc: ["@sa Empty()"],
              kind: "function",
              immutable: true,
              explicit: true,
              constexpr: true,
              parameters: [],
              type: ""
            },
            "SDL_RectsEqual": {
              name: "Equal",
              immutable: true,
              constexpr: true,
              parameters: [{
                name: "self",
              },
              {
                name: "other",
                type: "const RectRaw &"
              }]
            },
            "Contains": {
              kind: "function",
              type: "bool",
              constexpr: true,
              immutable: true,
              parameters: [
                {
                  name: "p",
                  type: "const PointRaw &"
                }
              ],
            },
            "Contains#2": {
              kind: "function",
              type: "bool",
              constexpr: true,
              immutable: true,
              parameters: [
                {
                  name: "other",
                  type: "const RectRaw &"
                }
              ],
            },
            "SDL_HasRectIntersection": {
              kind: "function",
              constexpr: true,
              immutable: true,
              parameters: [
                {
                  name: "self",
                },
                {
                  name: "other",
                  type: "const RectRaw &"
                }
              ]
            },
            "SDL_GetRectIntersection": {
              kind: "function",
              type: "Rect",
              constexpr: true,
              immutable: true,
              parameters: [{
                name: "self",
              }, {
                name: "other",
                type: "const RectRaw &"
              }]
            },
            "SDL_GetRectUnion": {
              kind: "function",
              type: "Rect",
              immutable: true,
              constexpr: true,
              parameters: [{
                name: "self",
              }, {
                name: "other",
                type: "const RectRaw &"
              }]
            }
          }
        },
        "SDL_FRect": {
          wrapper: {
            attribute: "r",
            comparable: true,
          },
          entries: {
            "FRect": {
              kind: "function",
              constexpr: true,
              type: "",
              parameters: [
                {
                  name: "corner",
                  type: "const FPointRaw &"
                },
                {
                  name: "size",
                  type: "const FPointRaw &"
                }
              ]
            },
            "SDL_GetRectEnclosingPointsFloat": {
              type: "FRect",
              constexpr: true,
              parameters: [
                {
                  type: "SpanRef<const FPointRaw>",
                  name: "points"
                },
                {
                  type: "OptionalRef<const FRectRaw>",
                  name: "clip",
                  default: "std::nullopt"
                }
              ]
            },
            "FromCenter": {
              kind: "function",
              name: "FromCenter",
              type: "FRect",
              constexpr: true,
              static: true,
              parameters: [
                {
                  name: "cx",
                  type: "float"
                },
                {
                  name: "cy",
                  type: "float"
                },
                {
                  name: "w",
                  type: "float"
                },
                {
                  name: "h",
                  type: "float"
                }
              ]
            },
            "FromCenter#2": {
              kind: "function",
              name: "FromCenter",
              type: "FRect",
              static: true,
              constexpr: true,
              parameters: [
                {
                  name: "center",
                  type: "const FPointRaw &"
                },
                {
                  name: "size",
                  type: "const FPointRaw &"
                }
              ]
            },
            "FromCorners": {
              kind: "function",
              name: "FromCorners",
              type: "FRect",
              constexpr: true,
              static: true,
              parameters: [
                {
                  name: "x1",
                  type: "float"
                },
                {
                  name: "y1",
                  type: "float"
                },
                {
                  name: "x2",
                  type: "float"
                },
                {
                  name: "y2",
                  type: "float"
                }
              ]
            },
            "FromCorners#2": {
              kind: "function",
              name: "FromCorners",
              static: true,
              constexpr: true,
              type: "FRect",
              parameters: [
                {
                  name: "p1",
                  type: "const FPointRaw &"
                },
                {
                  name: "p2",
                  type: "const FPointRaw &"
                }
              ]
            },
            "GetX2": "function",
            "SetX2": "function",
            "GetY2": "function",
            "SetY2": "function",
            "GetTopLeft": "function",
            "GetTopRight": "function",
            "GetBottomLeft": "function",
            "GetBottomRight": "function",
            "GetSize": "function",
            "GetCentroid": "function",
            "SDL_GetRectAndLineIntersectionFloat": {
              name: "GetLineIntersection"
            },
            "SDL_RectEmptyFloat": {
              name: "Empty",
              constexpr: true
            },
            "operator bool": {
              doc: ["@sa Empty()"],
              kind: "function",
              immutable: true,
              explicit: true,
              constexpr: true,
              parameters: [],
              type: ""
            },
            "SDL_RectsEqualEpsilon": {
              name: "EqualEpsilon",
              immutable: true,
              constexpr: true,
              parameters: [{
                name: "self"
              },
              {
                name: "other",
                type: "const FRectRaw &"
              },
              {
                name: "epsilon",
                type: "const float"
              }]
            },
            "SDL_RectsEqualFloat": {
              name: "Equal",
              immutable: true,
              constexpr: true,
              parameters: [{
                name: "self",
              },
              {
                name: "other",
                type: "const FRectRaw &"
              }]
            },
            "Contains": {
              kind: "function",
              type: "bool",
              parameters: [{
                name: "p",
                type: "const FPointRaw &"
              }],
              constexpr: true,
              immutable: true
            },
            "Contains#2": {
              kind: "function",
              type: "bool",
              parameters: [{
                name: "other",
                type: "const FRectRaw &"
              }],
              constexpr: true,
              immutable: true
            },
            "SDL_HasRectIntersectionFloat": {
              kind: "function",
              immutable: true,
              constexpr: true,
              parameters: [{
                name: "self",
              },
              {
                name: "other",
                type: "const FRectRaw &"
              }]
            },
            "SDL_GetRectIntersectionFloat": {
              kind: "function",
              type: "FRect",
              immutable: true,
              constexpr: true,
              parameters: [{
                name: "self",
              },
              {
                name: "other",
                type: "const FRectRaw &"
              }
              ]
            },
            "SDL_GetRectUnionFloat": {
              kind: "function",
              type: "FRect",
              immutable: true,
              constexpr: true,
              parameters: [{
                name: "self",
              },
              {
                name: "other",
                type: "const FRectRaw &"
              }]
            }
          }
        },
        "SDL_GetRectEnclosingPoints": {
          type: "Rect",
          parameters: [
            {
              type: "SpanRef<const PointRaw>",
              name: "points"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "clip",
              default: "std::nullopt"
            }
          ],
        },
        "SDL_GetRectEnclosingPointsFloat": {
          type: "FRect",
          constexpr: true,
          parameters: [
            {
              type: "SpanRef<const FPointRaw>",
              name: "points"
            },
            {
              type: "OptionalRef<const FRectRaw>",
              name: "clip",
              default: "std::nullopt"
            }
          ],
        },
      },
    },
    "SDL_render.h": {
      localIncludes: ["SDL3pp_blendmode.h", "SDL3pp_events.h", "SDL3pp_pixels.h", "SDL3pp_video.h"],
      namespacesMap: {
        "SDL_PROP_RENDERER_": "prop::Renderer",
        "SDL_PROP_TEXTURE_": "prop::Texture"
      },
      transform: {
        "SDL_SOFTWARE_RENDERER": { kind: "var", constexpr: true, },
        "SDL_RENDERER_VSYNC_DISABLED": { kind: "var", constexpr: true, type: "int" },
        "SDL_RENDERER_VSYNC_ADAPTIVE": { kind: "var", constexpr: true, type: "int" },
        "SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE": { kind: "var", constexpr: true, type: "int" },
        "SDL_Renderer": {
          resource: true,
          entries: {
            "Renderer": {
              kind: "function",
              type: "",
              parameters: [
                {
                  "type": "WindowParam",
                  "name": "window"
                }
              ]
            },
            "SDL_CreateRenderer": "ctor",
            "SDL_CreateRendererWithProperties": "ctor",
            "SDL_CreateSoftwareRenderer": "ctor",
          }
        },
        "SDL_CreateWindowAndRenderer": {
          type: "std::pair<Window, Renderer>",
          parameters: [
            {},
            {
              type: "const PointRaw &",
              name: "size"
            },
            {
              type: "WindowFlags",
              name: "window_flags",
              default: "0"
            }
          ]
        },
        "SDL_GetRenderer": {
          name: "Window::GetRenderer",
          immutable: true,
        },
        "SDL_GetRenderWindow": {
          kind: "function",
          hints: { methodName: "GetWindow" }
        },
        "SDL_GetRendererName": { immutable: true },
        "SDL_GetRenderOutputSize": {
          immutable: true,
          hints: { methodName: "GetOutputSize" },
        },
        "GetRenderOutputSize": {
          kind: "function",
          type: "Point",
          immutable: true,
          parameters: [{ type: "RendererParam", name: "renderer" }],
          hints: { methodName: "GetOutputSize" },
        },
        "SDL_GetCurrentRenderOutputSize": {
          immutable: true,
          hints: { methodName: "GetCurrentOutputSize" },
        },
        "GetCurrentRenderOutputSize": {
          kind: "function",
          type: "Point",
          immutable: true,
          parameters: [{ type: "RendererParam", name: "renderer" }],
          hints: { methodName: "GetCurrentOutputSize" },
        },
        "SDL_GetRendererProperties": { immutable: true },
        "SDL_SetRenderTarget": { hints: { methodName: "SetTarget" } },
        "ResetRenderTarget": {
          kind: "function",
          type: "void",
          parameters: [{ type: "RendererParam", name: "renderer" }],
          hints: { methodName: "ResetTarget" },
        },
        "SDL_GetRenderTarget": {
          immutable: true,
          hints: { methodName: "GetTarget" },
        },
        "SDL_SetRenderLogicalPresentation": {
          parameters: [
            {},
            { type: "const PointRaw &", name: "size" },
            { name: "mode", type: "RendererLogicalPresentation" },
          ],
          hints: { methodName: "SetLogicalPresentation" },
        },
        "SDL_GetRenderLogicalPresentation": {
          immutable: true,
          hints: { methodName: "GetLogicalPresentation" },
        },
        "GetRenderLogicalPresentation": {
          kind: "function",
          type: "void",
          parameters: [
            { type: "RendererParam", name: "renderer" },
            { type: "PointRaw *", name: "size" },
            { name: "mode", type: "RendererLogicalPresentation *" },
          ],
          hints: { methodName: "GetLogicalPresentation" },
        },
        "SDL_GetRenderLogicalPresentationRect": {
          immutable: true,
          type: "FRect",
          parameters: [{}],
          hints: { methodName: "GetLogicalPresentationRect" },
        },
        "SDL_RenderCoordinatesFromWindow": {
          type: "FPoint",
          immutable: true,
          parameters: [
            {},
            { type: "const FPointRaw &", name: "window_coord" },
          ]
        },
        "SDL_RenderCoordinatesToWindow": {
          type: "FPoint",
          immutable: true,
          parameters: [
            {},
            { type: "const FPointRaw &", name: "coord" },
          ]
        },
        "SDL_ConvertEventToRenderCoordinates": { immutable: true },
        "SDL_SetRenderViewport": {
          parameters: [
            {},
            { type: "OptionalRef<const RectRaw>", name: "rect" },
          ],
          hints: { methodName: "SetViewport" },
        },
        "ResetRenderViewport": {
          kind: "function",
          type: "void",
          parameters: [{ type: "RendererParam", name: "renderer" }],
          hints: { methodName: "ResetViewport" },
        },
        "SDL_GetRenderViewport": {
          immutable: true,
          type: "Rect",
          parameters: [{}],
          hints: { methodName: "GetViewport" },
        },
        "SDL_RenderViewportSet": {
          immutable: true,
          hints: { methodName: "ViewportSet" },
        },
        "SDL_GetRenderSafeArea": {
          immutable: true,
          type: "Rect",
          parameters: [{}],
          hints: { methodName: "GetSafeArea" },
        },
        "SDL_SetRenderClipRect": {
          parameters: [
            {},
            {
              type: "OptionalRef<const RectRaw>",
              name: "rect"
            }
          ],
          hints: { methodName: "SetClipRect" },
        },
        "ResetRenderClipRect": {
          kind: "function",
          type: "void",
          parameters: [{ type: "RendererParam", name: "renderer" }],
          hints: { methodName: "ResetClipRect" },
        },
        "SDL_GetRenderClipRect": {
          immutable: true,
          type: "Rect",
          parameters: [{}],
          hints: { methodName: "GetClipRect" },
        },
        "SDL_RenderClipEnabled": {
          immutable: true,
          hints: { methodName: "IsClipEnabled" },
        },
        "SDL_SetRenderScale": {
          parameters: [
            {},
            {
              type: "const FPointRaw &",
              name: "scale"
            }
          ],
          hints: { methodName: "SetScale" },
        },
        "SDL_GetRenderScale": {
          immutable: true,
          hints: { methodName: "GetScale" },
        },
        "GetRenderScale": {
          kind: "function",
          immutable: true,
          type: "FPoint",
          parameters: [{ type: "RendererParam", name: "renderer" }],
          hints: { methodName: "GetScale" },
        },
        "SDL_SetRenderDrawColor": {
          parameters: [
            {},
            {
              type: "ColorRaw",
              name: "c"
            }
          ],
          hints: { methodName: "SetDrawColor" },
        },
        "SDL_SetRenderDrawColorFloat": {
          parameters: [
            {},
            {
              type: "const FColorRaw &",
              name: "c"
            }
          ],
          hints: { methodName: "SetDrawColorFloat" },
        },
        "SDL_GetRenderDrawColor": {
          immutable: true,
          hints: { methodName: "GetDrawColor" },
        },
        "GetRenderDrawColor": {
          kind: "function",
          immutable: true,
          type: "Color",
          parameters: [{ type: "RendererParam", name: "renderer" }],
          hints: { methodName: "GetDrawColor" },
        },
        "SDL_GetRenderDrawColorFloat": {
          immutable: true,
          hints: { methodName: "GetDrawColorFloat" },
        },
        "GetRenderDrawColorFloat": {
          kind: "function",
          immutable: true,
          type: "FColor",
          parameters: [{ type: "RendererParam", name: "renderer" }],
          hints: { methodName: "GetDrawColorFloat" },
        },
        "SDL_SetRenderColorScale": { hints: { methodName: "SetColorScale" } },
        "SDL_GetRenderColorScale": {
          immutable: true,
          type: "float",
          parameters: [{}],
          hints: { methodName: "GetColorScale" },
        },
        "SDL_SetRenderDrawBlendMode": { hints: { methodName: "SetDrawBlendMode" } },
        "SDL_GetRenderDrawBlendMode": {
          immutable: true,
          type: "BlendMode",
          parameters: [{}],
          hints: { methodName: "GetDrawBlendMode" },
        },
        "SDL_RenderPoint": {
          parameters: [
            {},
            {
              "type": "const FPointRaw &",
              "name": "p"
            }
          ]
        },
        "SDL_RenderPoints": {
          parameters: [
            {},
            {
              "type": "SpanRef<const FPointRaw>",
              "name": "points"
            }
          ]
        },
        "SDL_RenderLine": {
          parameters: [
            {},
            {
              "type": "const FPointRaw &",
              "name": "p1"
            },
            {
              "type": "const FPointRaw &",
              "name": "p2"
            }
          ]
        },
        "SDL_RenderLines": {
          parameters: [
            {},
            {
              "type": "SpanRef<const FPointRaw>",
              "name": "points"
            }
          ]
        },
        "SDL_RenderRect": {
          parameters: [
            {},
            {
              "type": "OptionalRef<const FRectRaw>",
              "name": "rect"
            }
          ]
        },
        "SDL_RenderRects": {
          parameters: [
            {},
            {
              "type": "SpanRef<const FRectRaw>",
              "name": "rects"
            }
          ]
        },
        "SDL_RenderFillRect": {
          parameters: [
            {},
            {
              "type": "OptionalRef<const FRectRaw>",
              "name": "rect"
            }
          ]
        },
        "SDL_RenderFillRects": {
          parameters: [
            {},
            {
              "type": "SpanRef<const FRectRaw>",
              "name": "rects"
            }
          ]
        },
        "SDL_RenderTexture": {
          parameters: [
            {},
            {
              "name": "texture",
              "type": "TextureParam"
            },
            {
              "name": "srcrect",
              "type": "OptionalRef<const FRectRaw>"
            },
            {
              "name": "dstrect",
              "type": "OptionalRef<const FRectRaw>"
            }
          ]
        },
        "SDL_RenderTextureRotated": {
          parameters: [
            {},
            {
              "name": "texture",
              "type": "TextureParam"
            },
            {
              "name": "srcrect",
              "type": "OptionalRef<const FRectRaw>"
            },
            {
              "name": "dstrect",
              "type": "OptionalRef<const FRectRaw>"
            },
            {
              "name": "angle",
              "type": "double"
            },
            {
              "name": "center",
              "type": "OptionalRef<const FPointRaw>"
            },
            {
              "name": "flip",
              "type": "FlipMode",
            }
          ]
        },
        "SDL_RenderTextureAffine": {
          parameters: [
            {},
            {
              "name": "texture",
              "type": "TextureParam"
            },
            {
              "name": "srcrect",
              "type": "OptionalRef<const FRectRaw>"
            },
            {
              "name": "origin",
              "type": "OptionalRef<const FPointRaw>"
            },
            {
              "name": "right",
              "type": "OptionalRef<const FPointRaw>"
            },
            {
              "name": "down",
              "type": "OptionalRef<const FPointRaw>"
            }
          ]
        },
        "SDL_RenderTextureTiled": {
          parameters: [
            {},
            {
              "name": "texture",
              "type": "TextureParam"
            },
            {
              "name": "srcrect",
              "type": "OptionalRef<const FRectRaw>"
            },
            {
              "name": "scale",
              "type": "float"
            },
            {
              "name": "dstrect",
              "type": "OptionalRef<const FRectRaw>"
            }
          ]
        },
        "SDL_RenderTexture9Grid": {
          parameters: [
            {},
            {
              "name": "texture",
              "type": "TextureParam"
            },
            {
              "name": "srcrect",
              "type": "OptionalRef<const FRectRaw>"
            },
            {
              "name": "left_width",
              "type": "float"
            },
            {
              "name": "right_width",
              "type": "float"
            },
            {
              "name": "top_height",
              "type": "float"
            },
            {
              "name": "bottom_height",
              "type": "float"
            },
            {
              "name": "scale",
              "type": "float"
            },
            {
              "name": "dstrect",
              "type": "OptionalRef<const FRectRaw>"
            }
          ]
        },
        "SDL_RenderGeometry": {
          parameters: [
            {},
            {
              "name": "texture",
              "type": "TextureParam"
            },
            {
              "name": "vertices",
              "type": "std::span<const Vertex>"
            },
            {
              "name": "indices",
              "type": "std::span<const int>",
            }
          ]
        },
        "SDL_RenderGeometryRaw": {
          parameters: [
            {},
            {
              "name": "texture",
              "type": "TextureParam"
            },
            {
              "name": "xy",
              "type": "const float *"
            },
            {
              "name": "xy_stride",
              "type": "int"
            },
            {
              "name": "color",
              "type": "const FColor *"
            },
            {
              "name": "color_stride",
              "type": "int"
            },
            {
              "name": "uv",
              "type": "const float *"
            },
            {
              "name": "uv_stride",
              "type": "int"
            },
            {
              "name": "num_vertices",
              "type": "int"
            },
            {
              "name": "indices",
              "type": "const void *"
            },
            {
              "name": "num_indices",
              "type": "int"
            },
            {
              "name": "size_indices",
              "type": "int"
            }
          ]
        },
        "SDL_RenderReadPixels": {
          immutable: true,
          type: "Surface",
          parameters: [
            {},
            {
              name: "rect",
              type: "OptionalRef<const RectRaw>",
              default: "{}",
            }
          ],
          hints: { methodName: "ReadPixels" }
        },
        "SDL_RenderPresent": { hints: { methodName: "Present" } },
        "SDL_SetRenderVSync": { hints: { methodName: "SetVSync" } },
        "SDL_GetRenderVSync": {
          type: "int",
          immutable: true,
          parameters: [{}],
          hints: { methodName: "GetVSync" },
        },
        "SDL_RenderDebugText": {
          parameters: [
            {},
            {},
            { type: "StringParam", name: "str" },
          ]
        },
        "SDL_RenderDebugTextFormat": {
          template: [
            {
              "type": "class...",
              "name": "ARGS"
            }
          ],
          parameters: [
            {},
            {},
            { type: "std::string_view", name: "fmt" },
            { type: "ARGS...", name: "args" }
          ]
        },

        "SDL_Texture": {
          resource: {
            shared: "refcount",
          },
          entries: {
            "SDL_CreateTexture": "ctor",
            "SDL_CreateTextureFromSurface": "ctor",
            "SDL_CreateTextureWithProperties": "ctor",
          }
        },
        "SDL_GetTextureProperties": { parameters: [{ type: "TextureConstParam" }] },
        "SDL_GetRendererFromTexture": {
          parameters: [{ type: "TextureConstParam" }],
          hints: { methodName: "GetRenderer" },
        },
        "SDL_GetTextureSize": { parameters: [{ type: "TextureConstParam" }, {}, {}] },
        "GetTextureSize": {
          kind: "function",
          immutable: true,
          type: "Point",
          parameters: [{ type: "TextureConstParam", name: "texture" }]
        },
        "GetTextureSizeFloat": {
          kind: "function",
          immutable: true,
          type: "FPoint",
          parameters: [{ type: "TextureConstParam", name: "texture" }]
        },
        "GetTextureWidth": {
          kind: "function",
          immutable: true,
          type: "int",
          parameters: [{ type: "TextureConstParam", name: "texture" }]
        },
        "GetTextureHeight": {
          kind: "function",
          immutable: true,
          type: "int",
          parameters: [{ type: "TextureConstParam", name: "texture" }]
        },
        "GetTextureFormat": {
          kind: "function",
          immutable: true,
          type: "PixelFormat",
          parameters: [{ type: "TextureConstParam", name: "texture" }]
        },
        "SDL_GetTextureColorMod": { parameters: [{ type: "TextureConstParam" }, {}, {}, {}] },
        "SDL_GetTextureColorModFloat": { parameters: [{ type: "TextureConstParam" }, {}, {}, {}] },
        "SDL_GetTextureAlphaMod": {
          type: "Uint8",
          parameters: [{ type: "TextureConstParam" }],
        },
        "SDL_GetTextureAlphaModFloat": {
          type: "float",
          parameters: [{ type: "TextureConstParam" }],
        },
        "SetTextureMod": {
          kind: "function",
          type: "void",
          parameters: [
            { type: "TextureParam", name: "texture" },
            { type: "Color", name: "c" },
          ]
        },
        "SetTextureModFloat": {
          kind: "function",
          type: "void",
          parameters: [
            { type: "TextureParam", name: "texture" },
            { type: "FColor", name: "c" },
          ]
        },
        "GetTextureMod": {
          kind: "function",
          type: "Color",
          immutable: true,
          parameters: [{ type: "TextureConstParam", name: "texture" }]
        },
        "GetTextureModFloat": {
          kind: "function",
          type: "FColor",
          immutable: true,
          parameters: [{ type: "TextureConstParam", name: "texture" }]
        },
        "SDL_GetTextureBlendMode": {
          type: "BlendMode",
          parameters: [{ type: "TextureConstParam" }]
        },
        "SDL_GetTextureScaleMode": {
          type: "ScaleMode",
          parameters: [{ type: "TextureConstParam" }]
        },
        "SDL_UpdateTexture": {
          parameters: [
            {},
            { type: "OptionalRef<const RectRaw>" },
            {},
            {}
          ]
        },
        "UpdateTexture": {
          kind: "function",
          type: "void",
          parameters: [
            { type: "TextureParam", name: "texture" },
            { type: "SurfaceConstParam", name: "surface" },
            { type: "OptionalRef<const RectRaw>", name: "rect", default: "std::nullopt" },
          ]
        },
        "SDL_UpdateYUVTexture": {
          parameters: [
            {},
            { name: "rect", type: "OptionalRef<const RectRaw>" },
            { name: "Yplane", type: "const Uint8 *" },
            { name: "Ypitch", type: "int" },
            { name: "Uplane", type: "const Uint8 *" },
            { name: "Upitch", type: "int" },
            { name: "Vplane", type: "const Uint8 *" },
            { name: "Vpitch", type: "int" },
          ]
        },
        "SDL_UpdateNVTexture": {
          parameters: [
            {},
            { type: "OptionalRef<const RectRaw>" },
            { name: "Yplane", type: "const Uint8 *" },
            { name: "Ypitch", type: "int" },
            { name: "UVplane", type: "const Uint8 *" },
            { name: "UVpitch", type: "int" },
          ]
        },
        "SDL_LockTexture": {
          parameters: [{}, { type: "OptionalRef<const SDL_Rect>" }, {}, {}]
        },
        "SDL_LockTextureToSurface": {
          parameters: [{}, { type: "OptionalRef<const SDL_Rect>", default: "std::nullopt" }],
          type: "Surface",
        },
      }
    },
    "SDL_scancode.h": {
      localIncludes: ['SDL3pp_stdinc.h'],
      transform: {
        "Keycode": { kind: "forward" },
        "SDL_Scancode": {
          wrapper: { ordered: true }
        },
      }
    },
    "SDL_stdinc.h": {
      includes: ["chrono"],
      localIncludes: [
        "SDL3pp_callbackWrapper.h",
        "SDL3pp_error.h",
        "SDL3pp_optionalRef.h",
        "SDL3pp_ownPtr.h",
        "SDL3pp_spanRef.h",
        "SDL3pp_strings.h",
      ],
      ignoreEntries: [
        "alloca",
        "bool",
        "__bool_true_false_are_defined",
        "false",
        "SDL_INCLUDE_STDBOOL_H",
        "SDL_alignment_test",
        "SDL_const_cast",
        "SDL_DUMMY_ENUM",
        "SDL_IN_BYTECAP",
        "SDL_INOUT_Z_CAP",
        "SDL_OUT_BYTECAP",
        "SDL_OUT_CAP",
        "SDL_OUT_Z_BYTECAP",
        "SDL_OUT_Z_CAP",
        "SDL_PRILL_PREFIX",
        "SDL_PRILLd",
        "SDL_PRILLu",
        "SDL_PRILLx",
        "SDL_PRILLX",
        "SDL_PRINTF_FORMAT_STRING",
        "SDL_PRINTF_VARARG_FUNC",
        "SDL_PRINTF_VARARG_FUNCV",
        "SDL_PRIs32",
        "SDL_PRIu32",
        "SDL_PRIx32",
        "SDL_PRIX32",
        "SDL_PRIs64",
        "SDL_PRIu64",
        "SDL_PRIx64",
        "SDL_PRIX64",
        "SDL_reinterpret_cast",
        "SDL_SCANF_FORMAT_STRING",
        "SDL_SCANF_VARARG_FUNC",
        "SDL_SCANF_VARARG_FUNCV",
        "SDL_size_mul_check_overflow_builtin",
        "SDL_size_add_check_overflow_builtin",
        "SDL_stack_alloc",
        "SDL_stack_free",
        "SDL_static_cast",
        "SDL_WPRINTF_VARARG_FUNC",
        "SDL_WPRINTF_VARARG_FUNCV",
        "true",
        "strlcpy",
        "strlcat",
        "wcslcpy",
        "wcslcat",
        "strdup",
      ],
      transform: {
        "SDL_Environment": {
          resource: true,
          entries: {
            "SDL_CreateEnvironment": "ctor",
            "SDL_GetEnvironmentVariable": "function",
            "SDL_GetEnvironmentVariables": {
              type: "OwnArray<char *>"
            },
            "GetVariableCount": {
              kind: "function",
              type: "Uint64",
              parameters: []
            },
            "SDL_SetEnvironmentVariable": "function",
            "SDL_UnsetEnvironmentVariable": "function",
          }
        },
        "SDL_iconv_t": {
          name: "IConv",
          resource: {
            free: "SDL_iconv_close",
          },
          entries: {
            "SDL_iconv_open": { name: "ctor" },
            "SDL_iconv": "function",
            "SDL_iconv_close": {
              name: "close",
              static: false,
              parameters: [],
            },
          }
        },
        "SDL_ICONV_ERROR": { kind: "var", constexpr: true, type: "size_t" },
        "SDL_ICONV_E2BIG": { kind: "var", constexpr: true, type: "size_t" },
        "SDL_ICONV_EILSEQ": { kind: "var", constexpr: true, type: "size_t" },
        "SDL_ICONV_EINVAL": { kind: "var", constexpr: true, type: "size_t" },
        "SDL_iconv_string": {
          type: "OwnArray<char>",
          parameters: [{}, {}, { type: "SourceBytes" }]
        },
        "SDL_iconv_utf8_locale": {
          kind: "function",
          type: "OwnArray<char>",
          parameters: [{ type: "std::string_view" }],
        },
        "SDL_iconv_utf8_ucs2": {
          kind: "function",
          type: "OwnArray<Uint16>",
          parameters: [{ type: "std::string_view" }],
        },
        "SDL_iconv_utf8_ucs4": {
          kind: "function",
          type: "OwnArray<Uint32>",
          parameters: [{ type: "std::string_view" }],
        },
        "SDL_iconv_wchar_utf8": {
          kind: "function",
          type: "OwnArray<char>",
          parameters: [{ type: "std::wstring_view" }],
        },
        "SDL_CompareCallback_r": { callback: "std" },
        "CompareCallback_rCB": { name: "CompareCB" },
        "Seconds": {
          before: "SDL_Time",
          kind: "alias",
          type: "std::chrono::duration<float>",
          doc: ["Duration in seconds (float)."]
        },
        "Nanoseconds": {
          before: "SDL_Time",
          kind: "alias",
          type: "std::chrono::nanoseconds",
          doc: ["Duration in Nanoseconds (Uint64)."]
        },
        "Milliseconds": {
          before: "SDL_Time",
          kind: "alias",
          type: "std::chrono::milliseconds",
          doc: ["Duration in Miliseconds (Uint32)."]
        },
        "ToSeconds": {
          before: "SDL_Time",
          kind: "function",
          type: "float",
          constexpr: true,
          parameters: [{
            type: "Seconds",
            name: "duration"
          }],
          doc: ["Converts a time duration to seconds (float)."]
        },
        "FromSeconds": {
          before: "SDL_Time",
          kind: "function",
          type: "Seconds",
          constexpr: true,
          parameters: [{
            type: "float",
            name: "duration"
          }],
          doc: ["Converts a float to seconds representation."]
        },
        "ToNS": {
          before: "SDL_Time",
          kind: "function",
          type: "Sint64",
          constexpr: true,
          parameters: [{
            type: "std::chrono::nanoseconds",
            name: "duration"
          }],
          doc: ["Converts a time duration to seconds (float)."]
        },
        "FromNS": {
          before: "SDL_Time",
          kind: "function",
          type: "Nanoseconds",
          constexpr: true,
          parameters: [{
            type: "Sint64",
            name: "duration"
          }],
          doc: ["Converts a float to seconds representation."]
        },
        "SDL_Time": {
          kind: "struct",
          entries: {
            "Time": {
              kind: "function",
              type: "",
              constexpr: true,
              parameters: [
                {
                  "type": "std::chrono::nanoseconds",
                  "name": "time"
                }
              ]
            },
            "operator std::chrono::nanoseconds": {
              "kind": "function",
              "type": "",
              "immutable": true,
              "constexpr": true,
              "parameters": []
            },
            "Current": {},
            "FromNS": {
              "kind": "function",
              "type": "Time",
              "constexpr": true,
              "static": true,
              "parameters": [
                {
                  "type": "Sint64",
                  "name": "time"
                }
              ]
            },
            "ToNS": {
              "kind": "function",
              "type": "Sint64",
              "constexpr": true,
              "immutable": true,
              "parameters": []
            },
            "FromPosix": {},
            "ToPosix": {},
            "FromWindows": {},
            "ToWindows": {},
            "ToSeconds": {
              kind: "function",
              type: "float",
              constexpr: true,
              immutable: true,
              parameters: [],
              doc: ["Converts a time to seconds (float) since epoch."]
            },
            "FromSeconds": {
              kind: "function",
              name: "Time.FromSeconds",
              type: "Time",
              constexpr: true,
              static: true,
              parameters: [{
                type: "float",
                name: "interval"
              }],
              doc: ["Converts a time to seconds (float) since epoch."]
            },
            "operator+=": {
              "kind": "function",
              "type": "Time &",
              "constexpr": true,
              "parameters": [
                {
                  "type": "std::chrono::nanoseconds",
                  "name": "interval"
                }
              ]
            },
            "operator-=": {
              "kind": "function",
              "type": "Time &",
              "constexpr": true,
              "parameters": [
                {
                  "type": "std::chrono::nanoseconds",
                  "name": "interval"
                }
              ]
            }
          }
        },
        "SDL_MAX_TIME": {
          "kind": "var",
          "type": "Time",
          "constexpr": true
        },
        "SDL_MIN_TIME": {
          "kind": "var",
          "type": "Time",
          "constexpr": true
        },
        "SDL_FLT_EPSILON": {
          kind: "var",
          type: "float",
          constexpr: true,
        },
        "SDL_INIT_INTERFACE": {
          kind: "function",
          name: "InitInterface",
          constexpr: true,
          type: "void",
          template: [{ type: "Interface", name: "I" }],
          parameters: [{ type: "I*" }],
        },
        "SDL_MAX_SINT8": {
          "kind": "var",
          "type": "Sint8",
          "constexpr": true
        },
        "SDL_MIN_SINT8": {
          "kind": "var",
          "type": "Sint8",
          "constexpr": true
        },
        "SDL_MAX_UINT8": {
          "kind": "var",
          "type": "Uint8",
          "constexpr": true
        },
        "SDL_MIN_UINT8": {
          "kind": "var",
          "type": "Uint8",
          "constexpr": true
        },
        "SDL_MAX_SINT16": {
          "kind": "var",
          "type": "Sint16",
          "constexpr": true
        },
        "SDL_MIN_SINT16": {
          "kind": "var",
          "type": "Sint16",
          "constexpr": true
        },
        "SDL_MAX_UINT16": {
          "kind": "var",
          "type": "Uint16",
          "constexpr": true
        },
        "SDL_MIN_UINT16": {
          "kind": "var",
          "type": "Uint16",
          "constexpr": true
        },
        "SDL_MAX_SINT32": {
          "kind": "var",
          "type": "Sint32",
          "constexpr": true
        },
        "SDL_MIN_SINT32": {
          "kind": "var",
          "type": "Sint32",
          "constexpr": true
        },
        "SDL_MAX_UINT32": {
          "kind": "var",
          "type": "Uint32",
          "constexpr": true
        },
        "SDL_MIN_UINT32": {
          "kind": "var",
          "type": "Uint8",
          "constexpr": true
        },
        "SDL_MAX_SINT64": {
          "kind": "var",
          "type": "Sint64",
          "constexpr": true
        },
        "SDL_MIN_SINT64": {
          "kind": "var",
          "type": "Sint64",
          "constexpr": true
        },
        "SDL_MAX_UINT64": {
          "kind": "var",
          "type": "Uint64",
          "constexpr": true
        },
        "SDL_MIN_UINT64": {
          "kind": "var",
          "type": "Uint8",
          "constexpr": true
        },
        "SDL_arraysize": {
          "kind": "function",
          "type": "std::size_t",
          "constexpr": true,
          "template": [
            {
              "type": "class",
              "name": "T"
            },
            {
              "type": "std::size_t",
              "name": "N"
            }
          ],
          "parameters": [
            {
              "type": "const T",
              "name": "(&array)[N]"
            }
          ]
        },
        "SDL_FOURCC": {
          "name": "FourCC",
          "type": "Uint32",
          "kind": "function",
          "constexpr": true,
          "parameters": [
            {
              "name": "a",
              "type": "Uint8"
            },
            {
              "name": "b",
              "type": "Uint8"
            },
            {
              "name": "c",
              "type": "Uint8"
            },
            {
              "name": "d",
              "type": "Uint8"
            }
          ]
        },
        "SDL_GetEnvironment": {
          "type": "EnvironmentRaw"
        },
        "SDL_copyp": {
          kind: "function",
          constexpr: true,
          template: [
            { type: "typename", name: "T" },
            { type: "typename", name: "U" },
          ],
          type: "T*",
          parameters: [{ type: "T *" }, { type: "const U *" }],
        },
        "SDL_zero": {
          "kind": "function",
          "type": "void",
          "template": [
            {
              "type": "class",
              "name": "T"
            }
          ],
          "parameters": [
            {
              "type": "T &",
              "name": "x"
            }
          ]
        },
        "SDL_zerop": {
          "kind": "function",
          "type": "void",
          "template": [
            {
              "type": "class",
              "name": "T"
            }
          ],
          "parameters": [
            {
              "type": "T *",
              "name": "x"
            }
          ]
        },
        "SDL_zeroa": {
          "kind": "function",
          "type": "void",
          "template": [
            {
              "type": "class",
              "name": "T"
            },
            {
              "type": "std::size_t",
              "name": "N"
            }
          ],
          "parameters": [
            {
              "type": "T",
              "name": "(&x)[N]"
            }
          ]
        },
        "SDL_min": {
          "kind": "function",
          "type": "T",
          "constexpr": true,
          "template": [
            {
              "type": "class",
              "name": "T"
            },
            {
              "type": "class",
              "name": "U"
            }
          ],
          "parameters": [
            {
              "type": "T",
              "name": "x"
            },
            {
              "type": "U",
              "name": "y"
            }
          ]
        },
        "SDL_max": {
          "kind": "function",
          "type": "T",
          "constexpr": true,
          "template": [
            {
              "type": "class",
              "name": "T"
            },
            {
              "type": "class",
              "name": "U"
            }
          ],
          "parameters": [
            {
              "type": "T",
              "name": "x"
            },
            {
              "type": "U",
              "name": "y"
            }
          ]
        },
        "SDL_clamp": {
          "kind": "function",
          "type": "T",
          "constexpr": true,
          "template": [
            {
              "type": "class",
              "name": "T"
            },
            {
              "type": "class",
              "name": "U"
            },
            {
              "type": "class",
              "name": "V"
            }
          ],
          "parameters": [
            {
              "type": "T",
              "name": "x"
            },
            {
              "type": "U",
              "name": "a"
            },
            {
              "type": "V",
              "name": "b"
            }
          ]
        },
        "SDL_INVALID_UNICODE_CODEPOINT": {
          "kind": "var",
          "type": "Uint32",
          "constexpr": true
        },
        "SDL_PI_D": {
          kind: "var",
          constexpr: true,
          type: "double",
        },
        "SDL_PI_F": {
          kind: "var",
          constexpr: true,
          type: "float",
        },
        "SDL_acosf": { name: "acos" },
        "SDL_asinf": { name: "asin" },
        "SDL_atanf": { name: "atan" },
        "SDL_atan2f": { name: "atan2" },
        "SDL_ceilf": { name: "ceil" },
        "SDL_copysignf": {
          name: "copysign",
          parameters: [{
            name: "x",
            type: "float"
          },
          {
            name: "y",
            type: "float"
          }]
        },
        "SDL_cosf": { name: "cos" },
        "SDL_expf": { name: "exp" },
        "SDL_fabs": { name: "abs" },
        "SDL_fabsf": { name: "abs" },
        "SDL_floorf": { name: "floor" },
        "SDL_truncf": { name: "trunc" },
        "SDL_fmodf": {
          name: "fmod",
          parameters: [{
            name: "x",
            type: "float"
          },
          {
            name: "y",
            type: "float"
          }]
        },
        "SDL_isinff": { name: "isinf" },
        "SDL_isnanf": { name: "isnan" },
        "SDL_logf": { name: "log" },
        "SDL_log10f": { name: "log10" },
        "SDL_modff": { name: "modf" },
        "SDL_powf": {
          name: "pow",
          parameters: [{
            name: "x",
            type: "float"
          },
          {
            name: "y",
            type: "float"
          }]
        },
        "SDL_roundf": { name: "round" },
        "SDL_lroundf": { name: "lround" },
        "SDL_scalbnf": { name: "scalbn" },
        "SDL_sinf": { name: "sin" },
        "SDL_sqrtf": { name: "sqrt" },
        "SDL_tanf": { name: "tan" },
        "Random": {
          after: "SDL_rand_bits",
          kind: "struct",
          entries: {
            "m_state": {
              kind: "var",
              type: "Uint64"
            },
            "Random": {
              kind: "function",
              constexpr: true,
              type: "",
              parameters: []
            },
            "Random#2": {
              kind: "function",
              constexpr: true,
              explicit: true,
              type: "",
              parameters: [{
                type: "Uint64",
                name: "state"
              }]
            },
            "operator Uint64": {
              kind: "function",
              constexpr: true,
              type: "",
              parameters: []
            },
            "SDL_rand_r": {
              name: "rand",
              static: false,
              parameters: [{
                type: "Sint32",
                name: "n"
              }]
            },
            "SDL_randf_r": {
              name: "randf",
              static: false,
              parameters: []
            },
            "SDL_rand_bits_r": {
              name: "rand_bits",
              static: false,
              parameters: []
            }
          }
        },
        "SDL_rand_r": {},
        "SDL_randf_r": {},
        "SDL_rand_bits_r": {},
        "qsort_r": {
          after: "SDL_qsort_r",
          kind: "function",
          type: "void",
          parameters: [{
            name: "base",
            type: "void *"
          },
          {
            name: "nmemb",
            type: "size_t"
          },
          {
            name: "size",
            type: "size_t"
          },
          {
            name: "compare",
            type: "CompareCB"
          }]
        },
        "bsearch_r": {
          after: "SDL_bsearch_r",
          kind: "function",
          type: "void *",
          parameters: [{
            name: "key",
            type: "const void *"
          },
          {
            name: "base",
            type: "const void *"
          },
          {
            name: "nmemb",
            type: "size_t"
          },
          {
            name: "size",
            type: "size_t"
          },
          {
            name: "compare",
            type: "CompareCB"
          }]
        },
      }
    },
    "SDL_storage.h": {
      localIncludes: [
        "SDL3pp_filesystem.h",
        "SDL3pp_properties.h",
        "SDL3pp_stdinc.h",
      ],
      transform: {
        "SDL_GetStorageFileSize": {
          type: "std::optional<Uint64>",
          parameters: [{}, {}]
        },
        "SDL_ReadStorageFile": {
          parameters: [
            {},
            { name: "path", type: "StringParam" },
            { name: "destination", type: "TargetBytes" }
          ]
        },
        "ReadStorageFile": {
          after: "SDL_ReadStorageFile",
          kind: "function",
          type: "std::string",
          parameters: [
            { name: "storage", type: "StorageParam" },
            { name: "path", type: "StringParam" }
          ],
        },
        "ReadStorageFileAs": {
          after: "SDL_ReadStorageFile",
          kind: "function",
          template: [{ type: "class", name: "T" }],
          type: "std::vector<T>",
          parameters: [
            { name: "storage", type: "StorageParam" },
            { name: "path", type: "StringParam" }
          ],
        },
        "SDL_WriteStorageFile": {
          type: "void",
          parameters: [
            {},
            { name: "path", type: "StringParam" },
            { name: "source", type: "SourceBytes" }
          ]
        },
        "EnumerateStorageDirectory": {
          after: "SDL_EnumerateStorageDirectory",
          kind: "function",
          type: "void",
          name: "EnumerateStorageDirectory",
          parameters: [
            { name: "storage", type: "StorageParam" },
            { name: "path", type: "StringParam" },
            { name: "callback", type: "EnumerateDirectoryCB" }
          ]
        },
        "EnumerateStorageDirectory#2": {
          after: "SDL_EnumerateStorageDirectory",
          kind: "function",
          type: "std::vector<Path>",
          name: "EnumerateStorageDirectory",
          parameters: [
            { name: "storage", type: "StorageParam" },
            { name: "path", type: "StringParam" }
          ]
        },
        "SDL_GetStoragePathInfo": {
          type: "PathInfo",
          parameters: [
            {},
            {}
          ]
        },
        "SDL_GlobStorageDirectory": {
          type: "OwnArray<char *>",
          parameters: [
            {}, {}, {}, {}
          ]
        },
      },
    },
    "SDL_sensor.h": {
      localIncludes: ["SDL3pp_error.h", "SDL3pp_properties.h", "SDL3pp_stdinc.h", "SDL3pp_version.h"],
      transform: {
        "SDL_SensorID": { kind: "alias", before: "SDL_Sensor" },
        "SDL_SensorType": { before: "SDL_Sensor" },
        "SDL_GetSensors": {
          parameters: [],
          type: "OwnArray<SensorID>",
        },
        "SDL_STANDARD_GRAVITY": {
          kind: "var",
          type: "float",
          constexpr: true,
        },
        "SDL_SENSOR_COUNT": {
          since: {
            tag: "SDL",
            major: 3,
            minor: 2,
            patch: 22,
          }
        },
      }
    },
    "SDL_surface.h": {
      localIncludes: [
        "SDL3pp_blendmode.h",
        "SDL3pp_error.h",
        "SDL3pp_iostream.h",
        "SDL3pp_optionalRef.h",
        "SDL3pp_pixels.h",
        "SDL3pp_properties.h",
        "SDL3pp_rect.h",
        "SDL3pp_spanRef.h",
        "SDL3pp_strings.h",
        "SDL3pp_version.h",
      ],
      namespacesMap: {
        "SDL_PROP_SURFACE_": "prop::Surface"
      },
      transform: {
        "SDL_SurfaceFlags": {
          enum: {
            values: [
              "SDL_SURFACE_PREALLOCATED",
              "SDL_SURFACE_LOCK_NEEDED",
              "SDL_SURFACE_LOCKED",
              "SDL_SURFACE_SIMD_ALIGNED"
            ]
          }
        },
        "SDL_SCALEMODE_INVALID": {
          since: {
            tag: "SDL",
            major: 3,
            minor: 2,
            patch: 10,
          }
        },
        "SDL_PROP_SURFACE_HOTSPOT_X_NUMBER": {
          since: {
            tag: "SDL",
            major: 3,
            minor: 2,
            patch: 6,
          }
        },
        "SDL_PROP_SURFACE_HOTSPOT_Y_NUMBER": {
          since: {
            tag: "SDL",
            major: 3,
            minor: 2,
            patch: 6,
          }
        },
        "SDL_MUSTLOCK": {
          kind: "function",
          name: "MustLock",
          constexpr: true,
          immutable: true,
          type: "bool",
          parameters: [{ type: "SurfaceConstParam" }],
        },
        "SDL_Surface": {
          resource: {
            shared: 'refcount',
            ctors: ["SDL_LoadBMP_IO", "SDL_LoadBMP"],
          },
          entries: {
            "SDL_CreateSurface": "ctor",
            "SDL_CreateSurfaceFrom": "ctor",
            "SDL_LoadBMP_IO": {
              name: "LoadBMP",
              parameters: [{}, { default: "false" }]
            },
            "SDL_MUSTLOCK": {
              kind: "function",
              name: "MustLock",
              constexpr: true,
              immutable: true,
              type: "bool",
              parameters: [],
              proto: false,
              hints: { delegate: "SDL::MustLock" },
            },
          }
        },
        "SDL_GetSurfaceProperties": { immutable: true },
        "SDL_GetSurfaceColorspace": { immutable: true },
        "SDL_GetSurfacePalette": { immutable: true },
        "SDL_SurfaceHasAlternateImages": { immutable: true },
        "SDL_GetSurfaceImages": {
          type: "OwnArray<SurfaceRaw>",
          parameters: [{ type: "SurfaceConstParam" }]
        },
        "SDL_LoadBMP_IO": {
          name: "LoadBMP",
          parameters: [{}, { default: "false" }]
        },
        "SDL_SaveBMP_IO": {
          name: "SaveBMP",
          parameters: [{ type: "SurfaceConstParam" }, {}, { default: "false" }],
          hints: { methodName: "SaveBMP" },
        },
        "SDL_SaveBMP": { parameters: [{ type: "SurfaceConstParam" }, {}] },
        "SDL_SurfaceHasRLE": { parameters: [{ type: "SurfaceConstParam" }] },
        "SDL_SetSurfaceColorKey": {
          parameters: [
            {},
            {
              type: "std::optional<Uint32>",
              name: "key"
            }
          ]
        },
        "ClearSurfaceColorKey": {
          after: "SDL_SetSurfaceColorKey",
          kind: "function",
          type: "void",
          parameters: [{ type: "SurfaceParam", name: "surface" }]
        },
        "SDL_SurfaceHasColorKey": { parameters: [{ type: "SurfaceConstParam" }] },
        "SDL_GetSurfaceColorKey": {
          type: "std::optional<Uint32>",
          parameters: [{ type: "SurfaceConstParam" }],
        },
        "SDL_GetSurfaceColorMod": { parameters: [{ type: "SurfaceConstParam" }, {}, {}, {}] },
        "SDL_GetSurfaceAlphaMod": {
          type: "Uint8",
          parameters: [{ type: "SurfaceConstParam" }]
        },
        "SetSurfaceMod": {
          kind: "function",
          type: "void",
          parameters: [
            { type: "SurfaceParam", name: "surface" },
            { type: "Color", name: "color" },
          ]
        },
        "GetSurfaceMod": {
          kind: "function",
          type: "Color",
          immutable: true,
          parameters: [{ type: "SurfaceConstParam", name: "surface" }]
        },
        "SDL_GetSurfaceBlendMode": {
          type: "BlendMode",
          parameters: [{ type: "SurfaceConstParam" }]
        },
        "SDL_SetSurfaceClipRect": {
          parameters: [
            {},
            { type: "OptionalRef<const RectRaw>" },
          ]
        },
        "ResetSurfaceClipRect": {
          kind: "function",
          type: "void",
          parameters: [{ type: "SurfaceParam", name: "surface" }],
        },
        "SDL_GetSurfaceClipRect": {
          type: "Rect",
          parameters: [{ type: "SurfaceConstParam" }]
        },
        "SDL_DuplicateSurface": {
          type: "Surface",
          immutable: true
        },
        "SDL_ScaleSurface": {
          type: "Surface",
          immutable: true
        },
        "SDL_ConvertSurface": {
          type: "Surface",
          immutable: true
        },
        "SDL_ConvertSurfaceAndColorspace": {
          type: "Surface",
          immutable: true,
          hints: { methodName: "Convert" },
        },
        "SDL_FillSurfaceRect": {
          parameters: [
            {},
            {
              type: "OptionalRef<const RectRaw>",
              name: "rect"
            },
            {
              type: "Uint32",
              name: "color"
            }
          ]
        },
        "FillSurface": {
          kind: "function",
          type: "void",
          parameters: [
            { type: "SurfaceParam", name: "dst" },
            { type: "Uint32", name: "color" },
          ]
        },
        "SDL_FillSurfaceRects": {
          parameters: [
            {
            },
            {
              type: "SpanRef<const RectRaw>",
              name: "rects"
            },
            {
              type: "Uint32",
              name: "color"
            }
          ]
        },
        "Surface::SDL_BlitSurface": {
          parameters: [
            {
            },
            {
              type: "SurfaceParam",
              name: "src"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "srcrect"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "dstrect"
            }
          ]
        },
        "SDL_BlitSurface": {
          parameters: [{
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "srcrect"
          },
          {
            type: "SurfaceParam",
            name: "dst"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "dstrect"
          }]
        },
        "Surface::BlitAt": {
          kind: "function",
          type: "void",
          static: false,
          parameters: [
            {
              type: "SurfaceParam",
              name: "src"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "srcrect"
            },
            {
              type: "const PointRaw &",
              name: "dstpos"
            }
          ]
        },
        "BlitSurfaceAt": {
          kind: "function",
          type: "void",
          parameters: [{
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "srcrect"
          },
          {
            type: "SurfaceParam",
            name: "dst"
          },
          {
            type: "const PointRaw &",
            name: "dstpos"
          }]
        },
        "Surface::SDL_BlitSurfaceUnchecked": {
          parameters: [
            {
            },
            {
              type: "SurfaceParam",
              name: "src"
            },
            {
              type: "const RectRaw &",
              name: "srcrect"
            },
            {
              type: "const RectRaw &",
              name: "dstrect"
            }
          ]
        },
        "SDL_BlitSurfaceUnchecked": {
          parameters: [{
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "const RectRaw &",
            name: "srcrect"
          },
          {
            type: "SurfaceParam",
            name: "dst"
          },
          {
            type: "const RectRaw &",
            name: "dstrect"
          }]
        },
        "Surface::SDL_BlitSurfaceScaled": {
          parameters: [
            {
            },
            {
              type: "SurfaceParam",
              name: "src"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "srcrect"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "dstrect"
            },
            {
              type: "ScaleMode",
              name: "scaleMode"
            }
          ]
        },
        "SDL_BlitSurfaceScaled": {
          parameters: [{
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "srcrect"
          },
          {
            type: "SurfaceParam",
            name: "dst"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "dstrect"
          },
          {
            type: "ScaleMode",
            name: "scaleMode"
          }]
        },
        "Surface::SDL_BlitSurfaceUncheckedScaled": {
          parameters: [
            {
            },
            {
              type: "SurfaceParam",
              name: "src"
            },
            {
              type: "const RectRaw &",
              name: "srcrect"
            },
            {
              type: "const RectRaw &",
              name: "dstrect"
            },
            {
              type: "ScaleMode",
              name: "scaleMode"
            }
          ]
        },
        "SDL_BlitSurfaceUncheckedScaled": {
          parameters: [{
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "const RectRaw &",
            name: "srcrect"
          },
          {
            type: "SurfaceParam",
            name: "dst"
          },
          {
            type: "const RectRaw &",
            name: "dstrect"
          },
          {
            type: "ScaleMode",
            name: "scaleMode"
          }]
        },
        "Surface::SDL_StretchSurface": {
          parameters: [
            {
            },
            {
              type: "SurfaceParam",
              name: "src"
            },
            {
              type: "OptionalRef<RectRaw>",
              name: "srcrect"
            },
            {
              type: "OptionalRef<RectRaw>",
              name: "dstrect"
            },
            {
              type: "ScaleMode",
              name: "scaleMode"
            }
          ]
        },
        "SDL_StretchSurface": {
          parameters: [{
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "OptionalRef<RectRaw>",
            name: "srcrect"
          },
          {
            type: "SurfaceParam",
            name: "dst"
          },
          {
            type: "OptionalRef<RectRaw>",
            name: "dstrect"
          },
          {
            type: "ScaleMode",
            name: "scaleMode"
          }]
        },
        "Surface::SDL_BlitSurfaceTiled": {
          parameters: [
            {
            },
            {
              type: "SurfaceParam",
              name: "src"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "srcrect"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "dstrect"
            }
          ]
        },
        "SDL_BlitSurfaceTiled": {
          parameters: [{
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "srcrect"
          },
          {
            type: "SurfaceParam",
            name: "dst"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "dstrect"
          }]
        },
        "Surface::SDL_BlitSurfaceTiledWithScale": {
          parameters: [
            {
            },
            {
              type: "SurfaceParam",
              name: "src"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "srcrect"
            },
            {
              type: "float",
              name: "scale"
            },
            {
              type: "SDL_ScaleMode",
              name: "scaleMode"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "dstrect"
            }
          ]
        },
        "SDL_BlitSurfaceTiledWithScale": {
          parameters: [{
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "srcrect"
          },
          {
            type: "float",
            name: "scale"
          },
          {
            type: "SDL_ScaleMode",
            name: "scaleMode"
          },
          {
            type: "SurfaceParam",
            name: "dst"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "dstrect"
          }]
        },
        "Surface::SDL_BlitSurface9Grid": {
          parameters: [{
          },
          {
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "srcrect"
          },
          {
            type: "int",
            name: "left_width"
          },
          {
            type: "int",
            name: "right_width"
          },
          {
            type: "int",
            name: "top_height"
          },
          {
            type: "int",
            name: "bottom_height"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "dstrect"
          },
          {
            type: "float",
            name: "scale",
            default: "1",
          },
          {
            type: "SDL_ScaleMode",
            name: "scaleMode",
            default: "SCALEMODE_NEAREST",
          }]
        },
        "SDL_BlitSurface9Grid": {
          parameters: [{
            type: "SurfaceParam",
            name: "src"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "srcrect"
          },
          {
            type: "int",
            name: "left_width"
          },
          {
            type: "int",
            name: "right_width"
          },
          {
            type: "int",
            name: "top_height"
          },
          {
            type: "int",
            name: "bottom_height"
          },
          {
            type: "SurfaceParam",
            name: "dst"
          },
          {
            type: "OptionalRef<const RectRaw>",
            name: "dstrect"
          },
          {
            type: "float",
            name: "scale",
            default: "1",
          },
          {
            type: "SDL_ScaleMode",
            name: "scaleMode",
            default: "SCALEMODE_NEAREST",
          }]
        },
        "SDL_MapSurfaceRGB": {
          immutable: true
        },
        "SDL_MapSurfaceRGBA": {
          immutable: true
        },
        "SDL_ReadSurfacePixel": {
          immutable: true,
        },
        "ReadSurfacePixel": {
          kind: "function",
          immutable: true,
          type: "Color",
          parameters: [
            { type: "SurfaceConstParam", name: "surface" },
            { type: "const PointRaw &", name: "p" },
          ]
        },
        "SDL_ReadSurfacePixelFloat": {
          immutable: true,
        },
        "ReadSurfacePixelFloat": {
          kind: "function",
          immutable: true,
          type: "FColor",
          parameters: [
            { type: "SurfaceConstParam", name: "surface" },
            { type: "const PointRaw &", name: "p" },
          ]
        },
        "GetSurfaceWidth": {
          after: "SDL_WriteSurfacePixelFloat",
          kind: "function",
          immutable: true,
          type: "int",
          constexpr: true,
          parameters: [{ type: "SurfaceConstParam", name: "surface" }]
        },
        "GetSurfaceHeight": {
          kind: "function",
          immutable: true,
          type: "int",
          constexpr: true,
          parameters: [{ type: "SurfaceConstParam", name: "surface" }]
        },
        "GetSurfaceSize": {
          kind: "function",
          immutable: true,
          type: "Point",
          constexpr: true,
          parameters: [{ type: "SurfaceConstParam", name: "surface" }]
        },
        "GetSurfacePitch": {
          kind: "function",
          type: "int",
          immutable: true,
          constexpr: true,
          parameters: [{ type: "SurfaceConstParam", name: "surface" }]
        },
        "GetSurfaceFormat": {
          kind: "function",
          immutable: true,
          type: "PixelFormat",
          constexpr: true,
          parameters: [{ type: "SurfaceConstParam", name: "surface" }]
        },
        "GetSurfacePixels": {
          kind: "function",
          immutable: true,
          type: "void *",
          constexpr: true,
          parameters: [{ type: "SurfaceConstParam", name: "surface" }]
        },
      }
    },
    "SDL_system.h": {
      localIncludes: ['SDL3pp_stdinc.h'],
      transform: {
        "RequestAndroidPermission": {
          after: "SDL_RequestAndroidPermission",
          kind: "function",
          type: "bool",
          parameters: [
            {
              name: "permission",
              type: "StringParam"
            },
            {
              name: "cb",
              type: "RequestAndroidPermissionCB"
            }
          ],
        },
        "SDL_ANDROID_EXTERNAL_STORAGE_READ": {
          kind: "var",
          type: "Uint32",
          constexpr: true,
        },
        "SDL_ANDROID_EXTERNAL_STORAGE_WRITE": {
          kind: "var",
          type: "Uint32",
          constexpr: true,
        },
        "SDL_RequestAndroidPermissionCallback": { callback: "std" },
      }
    },
    "SDL_thread.h": {
      localIncludes: [
        'SDL3pp_atomic.h',
        'SDL3pp_properties.h',
        'SDL3pp_stdinc.h',
      ],
      namespacesMap: {
        "SDL_PROP_THREAD_": "prop::thread"
      },
      ignoreEntries: [
        "SDL_CreateThreadRuntime",
        "SDL_CreateThreadWithPropertiesRuntime",
      ],
      transform: {
        "ThreadID": { after: "__begin" },
        "SDL_ThreadFunction": {
          after: "__begin",
          callback: {
            functorSupport: "std",
            userdataIndex: 0,
          },
        },
        "SDL_TLSDestructorCallback": { after: "__begin" },
        "SDL_ThreadPriority": {
          enum: "SDL_THREAD_PRIORITY_",
          after: "__begin"
        },
        "SDL_ThreadState": {
          enum: "SDL_THREAD_",
          after: "__begin"
        },
        "SDL_Thread": {
          resource: { free: "SDL_DetachThread" },
          entries: {
            "Thread": {
              kind: "function",
              type: "",
              parameters: [
                { type: "ThreadCB", name: "fn" },
                { type: "StringParam", name: "name" },
              ]
            },
            "SDL_CreateThread": "ctor",
            "SDL_CreateThreadWithProperties": "ctor",
            "SDL_GetThreadName": "immutable",
            "SDL_GetThreadID": "immutable",
            "SDL_SetCurrentThreadPriority": {
              static: true
            },
            "SDL_WaitThread": "function",
            "SDL_GetThreadState": "immutable",
          }
        },
        "SDL_TLSID": {
          type: "AtomicInt",
          before: "SDL_GetTLS",
        }
      }
    },
    "SDL_time.h": {
      localIncludes: ["SDL3pp_stdinc.h"],
      transform: {
        "SDL_DateTime": {
          "entries": {
            "SDL_TimeToDateTime": {
              "name": "DateTime",
              "type": "",
              "parameters": [
                {
                  "type": "Time",
                  "name": "ticks"
                },
                {
                  "type": "bool",
                  "name": "localTime",
                  "default": "true"
                }
              ]
            },
            "SDL_DateTimeToTime": {
              "name": "operator Time",
              "type": "",
              "immutable": true,
              "parameters": []
            }
          }
        },
        "SDL_GetCurrentTime": {
          name: "Time::Current",
          type: "Time"
        },
        "SDL_TimeFromWindows": {
          name: "Time::FromWindows"
        },
        "SDL_TimeToWindows": {
          name: "Time::ToWindows",
          immutable: true
        },
        "SDL_TimeToDateTime": {
          type: "DateTime",
          parameters: [{}, { type: "bool", name: "localTime", default: "true" }],
        },
        "SDL_DateTimeToTime": {
          type: "Time",
          parameters: [{}],
        },
      }
    },
    "SDL_timer.h": {
      localIncludes: ["SDL3pp_stdinc.h"],
      ignoreEntries: [
        "SDL_GetTicks",
        "SDL_TimerCallback",
        "SDL_AddTimer",
        "SDL_DelayNS",
        "SDL_MS_PER_SECOND",
        "SDL_US_PER_SECOND",
        "SDL_NS_PER_SECOND",
        "SDL_NS_PER_MS",
        "SDL_NS_PER_US",
        "SDL_MS_TO_NS",
        "SDL_NS_TO_MS",
        "SDL_US_TO_NS",
        "SDL_NS_TO_US"
      ],
      transform: {
        "SDL_SECONDS_TO_NS": {
          "name": "Time::FromPosix",
          "kind": "function",
          "type": "Time",
          "constexpr": true,
          "static": true,
          "parameters": [
            {
              "type": "Sint64",
              "name": "time"
            }
          ]
        },
        "SDL_NS_TO_SECONDS": {
          "name": "Time::ToPosix",
          "kind": "function",
          "type": "Sint64",
          "constexpr": true,
          "immutable": true,
          "parameters": []
        },
        "SDL_GetTicksNS": {
          "name": "GetTicks",
          "type": "std::chrono::nanoseconds"
        },
        "SDL_Delay": {
          "parameters": [
            {
              "type": "std::chrono::nanoseconds",
              "name": "duration"
            }
          ]
        },
        "SDL_DelayPrecise": {
          "parameters": [
            {
              "type": "std::chrono::nanoseconds",
              "name": "duration"
            }
          ]
        },
        "SDL_NSTimerCallback": {
          name: "TimerCallback",
          type: "SDL_NSTimerCallback",
          kind: "alias",
          callback: {
            functorSupport: "lightweight",
            type: "std::chrono::nanoseconds",
            parameters: [{
              name: "timerID",
              type: "TimerID"
            }, {
              name: "interval",
              type: "std::chrono::nanoseconds"
            }]
          },
        },
        "SDL_AddTimerNS": {
          name: "AddTimer",
          parameters: [
            {
              type: "std::chrono::nanoseconds",
              name: "interval"
            },
            {
              type: "TimerCallback",
              name: "callback"
            },
            {
              type: "void *",
              name: "userdata"
            }
          ]
        },
        "AddTimer": {
          kind: "function",
          type: "TimerID",
          parameters: [
            {
              type: "std::chrono::nanoseconds",
              name: "interval"
            },
            {
              type: "TimerCB",
              name: "callback"
            }
          ]
        },
      }
    },
    "SDL_touch.h": {
      localIncludes: ["SDL3pp_error.h", "SDL3pp_mouse.h", "SDL3pp_stdinc.h"],
      transform: {
        "SDL_Finger": { wrapper: true },
        "SDL_TOUCH_MOUSEID": {
          kind: "var",
          constexpr: true,
          type: "MouseID",
        },
        "SDL_MOUSE_TOUCHID": {
          kind: "var",
          constexpr: true,
          type: "TouchID",
        },
        "SDL_GetTouchDevices": {
          parameters: [],
          type: "OwnArray<TouchID>",
        },
        "SDL_GetTouchFingers": {
          parameters: [{}],
          type: "OwnArray<Finger*>",
        },
      }
    },
    "SDL_tray.h": {
      localIncludes: ['SDL3pp_stdinc.h', "SDL3pp_surface.h"],
      transform: {
        "SDL_TrayEntryFlags": {
          enum: "SDL_TRAYENTRY_",
          after: "__begin",
        },
        "SDL_TrayCallback": { before: "SDL_Tray", callback: "lightweight" },
        "SDL_Tray": {
          resource: true,
          entries: {
            "SDL_CreateTray": "ctor",
            "SDL_SetTrayIcon": "function",
            "SDL_SetTrayTooltip": "function",
            "SDL_CreateTrayMenu": {
              proto: true,
            },
            "SDL_GetTrayMenu": {
              proto: true,
              immutable: true,
            }
          }
        },
        "SDL_TrayEntry": {
          resource: {
            free: "SDL_RemoveTrayEntry",
            owning: false,
          },
          entries: {
            "SDL_CreateTraySubmenu": {
              name: "CreateSubmenu",
            },
            "SDL_GetTraySubmenu": {
              name: "GetSubmenu",
            },
            "SDL_SetTrayEntryLabel": {
              name: "SetLabel",
            },
            "SDL_GetTrayEntryLabel": {
              name: "GetLabel",
              immutable: true,
            },
            "SDL_SetTrayEntryChecked": {
              name: "SetChecked",
            },
            "SDL_GetTrayEntryChecked": {
              name: "GetChecked",
              immutable: true,
            },
            "SDL_SetTrayEntryEnabled": {
              name: "SetEnabled",
            },
            "SDL_GetTrayEntryEnabled": {
              name: "GetEnabled",
              immutable: true,
            },
            "SetCallback": {
              kind: "function",
              type: "void",
              proto: true,
              parameters: [{
                type: "TrayCB",
                name: "callback",
              }],
            },
            "SDL_SetTrayEntryCallback": {
              name: "SetCallback",
            },
            "SDL_ClickTrayEntry": {
              name: "Click",
            },
            "SDL_GetTrayEntryParent": {
              name: "GetParent",
            }
          }
        },
        "SDL_TrayMenu": {
          wrapper: true,
          entries: {
            "SDL_GetTrayEntries": {
              name: "GetEntries",
              type: "std::span<TrayEntry>",
              proto: true,
              parameters: [{}],
            },
            "SDL_InsertTrayEntryAt": {
              name: "InsertEntry",
              type: "TrayEntry",
              proto: true,
            },
            "AppendEntry": {
              kind: "function",
              type: "TrayEntry",
              static: false,
              proto: true,
              parameters: [
                {
                  type: "StringParam",
                  name: "label"
                },
                {
                  type: "TrayEntryFlags",
                  name: "flags"
                },
              ],
            },
            "SDL_GetTrayMenuParentEntry": {
              name: "GetParentEntry",
              type: "TrayEntryParam",
              immutable: true,
              proto: true,
            },
            "SDL_GetTrayMenuParentTray": {
              name: "GetParentTray",
              type: "TrayParam",
              immutable: true,
              proto: true,
            },
          }
        },
      }
    },
    "SDL_version.h": {
      transform: {
        "SDL_MAJOR_VERSION": {
          value: ""
        },
        "SDL_MINOR_VERSION": {
          value: ""
        },
        "SDL_MICRO_VERSION": {
          value: ""
        },
        "SDL3PP_MAJOR_VERSION": {
          after: "SDL_GetRevision",
          kind: "def",
          value: currentVersion[0],
          doc: ["The current major version of SDL3pp wrapper."]
        },
        "SDL3PP_MINOR_VERSION": {
          kind: "def",
          value: currentVersion[1],
          doc: ["The current minor version of SDL3pp wrapper."]
        },
        "SDL3PP_PATCH_VERSION": {
          kind: "def",
          value: currentVersion[2],
          doc: ["The current patch version of SDL3pp wrapper."]
        },
        "SDL3PP_VERSION": {
          kind: "def",
          value: "SDL_VERSIONNUM(SDL3PP_MAJOR_VERSION, SDL3PP_MINOR_VERSION, SDL3PP_MICRO_VERSION)",
          doc: ["This is the version number macro for the current SDL3pp wrapper version."]
        },
      },
    },
    "SDL_video.h": {
      localIncludes: [
        "SDL3pp_properties.h",
        "SDL3pp_rect.h",
        "SDL3pp_surface.h",
      ],
      namespacesMap: {
        "SDL_PROP_GLOBAL_": "prop::Global",
        "SDL_PROP_DISPLAY_": "prop::Display",
        "SDL_PROP_WINDOW_": "prop::Window"
      },
      transform: {
        "RendererRef": { kind: "forward" },
        "SDL_WINDOWPOS_UNDEFINED_MASK": { kind: "var", type: "int", constexpr: true },
        "SDL_WINDOWPOS_UNDEFINED_DISPLAY": {
          kind: "function",
          constexpr: true,
          type: "int",
          parameters: [{ type: "int" }],
        },
        "SDL_WINDOWPOS_UNDEFINED": { kind: "var", type: "int", constexpr: true },
        "SDL_WINDOWPOS_ISUNDEFINED": {
          kind: "function",
          constexpr: true,
          type: "bool",
          parameters: [{ type: "int" }],
        },
        "SDL_WINDOWPOS_CENTERED_MASK": { kind: "var", type: "int", constexpr: true },
        "SDL_WINDOWPOS_CENTERED_DISPLAY": {
          kind: "function",
          constexpr: true,
          type: "int",
          parameters: [{ type: "int" }],
        },
        "SDL_WINDOWPOS_CENTERED": { kind: "var", type: "int", constexpr: true },
        "SDL_WINDOWPOS_ISCENTERED": {
          kind: "function",
          constexpr: true,
          type: "bool",
          parameters: [{ type: "int" }],
        },
        "SDL_DisplayOrientation": { before: "SDL_DisplayID" },
        "SDL_DisplayMode": { before: "SDL_DisplayID" },
        "SDL_DisplayID": {
          name: "Display",
          wrapper: {
            attribute: "displayID",
            rawName: "DisplayID",
          },
          entries: {
            "SDL_GetPrimaryDisplay": { static: true },
            "SDL_GetDisplayForPoint": "function",
            "SDL_GetDisplayForRect": "function",
          }
        },
        "SDL_GetDisplays": {
          kind: "function",
          type: "OwnArray<DisplayID>",
          parameters: [],
        },
        "SDL_GetDisplayProperties": { immutable: true },
        "SDL_GetDisplayName": { immutable: true },
        "SDL_GetDisplayBounds": {
          kind: "function",
          type: "Rect",
          parameters: [{}],
          immutable: true
        },
        "SDL_GetDisplayUsableBounds": {
          kind: "function",
          type: "Rect",
          parameters: [{}],
          immutable: true
        },
        "SDL_GetNaturalDisplayOrientation": {
          kind: "function",
          immutable: true,
        },
        "SDL_GetCurrentDisplayOrientation": {
          kind: "function",
          immutable: true,
        },
        "SDL_GetDisplayContentScale": { immutable: true },
        "SDL_GetFullscreenDisplayModes": {
          kind: "function",
          immutable: true,
          type: "OwnArray<DisplayMode *>",
          parameters: [{}]
        },
        "SDL_GetClosestFullscreenDisplayMode": {
          kind: "function",
          immutable: true,
          type: "DisplayMode",
          parameters: [
            {},
            {},
            {},
            {}
          ],
        },
        "SDL_GetDesktopDisplayMode": {
          kind: "function",
          immutable: true,
        },
        "SDL_GetCurrentDisplayMode": {
          kind: "function",
          immutable: true,
        },
        "SDL_WindowFlags": {
          before: "SDL_Window",
          enum: "SDL_WINDOW_"
        },
        "SDL_FlashOperation": { before: "SDL_Window" },
        "SDL_HitTestResult": { before: "SDL_Window" },
        "SDL_HitTest": {
          before: "SDL_Window",
          callback: {
            functorSupport: "lightweight",
            userdataIndex: 2,
          }
        },
        "SDL_EGLSurface": { before: "SDL_Window" },
        "SDL_PROP_WINDOW_CREATE_CONSTRAIN_POPUP_BOOLEAN": {
          since: {
            tag: "SDL",
            major: 3,
            minor: 2,
            patch: 18,
          }
        },
        "SDL_Window": {
          resource: {
            free: "SDL_DestroyWindow",
          },
          entries: {
            "SDL_CreateWindow": {
              kind: "function",
              name: "ctor",
              parameters: [
                {
                  name: "title",
                  type: "StringParam"
                },
                {
                  name: "size",
                  type: "const PointRaw &"
                },
                {
                  name: "flags",
                  type: "WindowFlags",
                  default: "0"
                }
              ]
            },
            "SDL_CreatePopupWindow": {
              kind: "function",
              name: "ctor",
              parameters: [
                {
                  name: "parent",
                  type: "WindowParam"
                },
                {
                  name: "offset",
                  type: "const PointRaw &"
                },
                {
                  name: "size",
                  type: "const PointRaw &"
                },
                {
                  name: "flags",
                  type: "WindowFlags",
                  default: "0"
                }
              ]
            },
            "SDL_CreateWindowWithProperties": "ctor",
            "SDL_GetWindowFromID": {
              name: "FromID",
              static: true,
            },
            "SDL_GetGrabbedWindow": {
              name: "GetGrabbed",
              static: true,
            },
          }
        },
        "SDL_GetWindows": {
          kind: "function",
          type: "OwnArray<WindowRef>",
          parameters: []
        },
        "SDL_GetDisplayForWindow": {
          immutable: true,
          hints: { methodName: "GetDisplay" },
        },
        "SDL_GetWindowPixelDensity": { immutable: true },
        "SDL_GetWindowDisplayScale": { immutable: true },
        "SDL_SetWindowFullscreenMode": { parameters: [{}, { type: "OptionalRef<const DisplayMode>" }] },
        "SDL_GetWindowFullscreenMode": { immutable: true },
        "SDL_GetWindowICCProfile": {
          kind: "function",
          immutable: true,
          type: "OwnPtr<void>",
          parameters: [{}]
        },
        "SDL_GetWindowPixelFormat": { immutable: true },
        "SDL_GetWindowID": { immutable: true },
        "SDL_GetWindowParent": { immutable: true },
        "SDL_GetWindowProperties": { immutable: true },
        "SDL_GetWindowFlags": { immutable: true },
        "SDL_GetWindowTitle": { immutable: true },
        "SDL_GetWindowPosition": { immutable: true },
        "GetWindowPosition": {
          kind: "function",
          type: "Point",
          immutable: true,
          parameters: [{ type: "WindowParam", name: "window" }]
        },
        "SDL_GetWindowSize": { immutable: true },
        "GetWindowSize": {
          kind: "function",
          type: "Point",
          immutable: true,
          parameters: [{ type: "WindowParam", name: "window" }]
        },
        "SetWindowRect": {
          kind: "function",
          type: "void",
          parameters: [
            { type: "WindowParam", name: "window" },
            { type: "Rect", name: "rect" },
          ]
        },
        "GetWindowRect": {
          kind: "function",
          type: "Rect",
          immutable: true,
          parameters: [{ type: "WindowParam", name: "window" }]
        },
        "SDL_GetWindowSafeArea": {
          type: "Rect",
          immutable: true,
          parameters: [{}]
        },
        "SDL_GetWindowAspectRatio": { immutable: true },
        "SDL_GetWindowBordersSize": { immutable: true },
        "SDL_GetWindowSizeInPixels": { immutable: true },
        "GetWindowSizeInPixels": {
          kind: "function",
          type: "Point",
          immutable: true,
          parameters: [{ type: "WindowParam", name: "window" }]
        },
        "SDL_SetWindowMinimumSize": { parameters: [{}, { type: "const PointRaw &", name: "p" }] },
        "SDL_GetWindowMinimumSize": { immutable: true },
        "SDL_SetWindowMaximumSize": { parameters: [{}, { type: "const PointRaw &", name: "p" }] },
        "SDL_GetWindowMaximumSize": { immutable: true },
        "SDL_WindowHasSurface": { immutable: true },
        "SDL_GetWindowSurfaceVSync": {
          immutable: true,
          type: "int",
          parameters: [{}]
        },
        "SDL_UpdateWindowSurfaceRects": {
          parameters: [
            {},
            { type: "SpanRef<const RectRaw>" },
          ]
        },
        "SDL_GetWindowKeyboardGrab": { immutable: true },
        "SDL_GetWindowMouseGrab": { immutable: true },
        "SDL_GetWindowMouseRect": {
          immutable: true,
          type: "const RectRaw *"
        },
        "SDL_GetWindowOpacity": { immutable: true },
        "SetWindowHitTest": {
          after: "SDL_SetWindowHitTest",
          kind: "function",
          type: "void",
          parameters: [
            { name: "window", type: "WindowParam" },
            { name: "callback", type: "HitTestCB" },
          ]
        },
        "SDL_WINDOW_SURFACE_VSYNC_DISABLED": {
          kind: 'var',
          type: 'int',
          constexpr: true,
        },
        "SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE": {
          kind: 'var',
          type: 'int',
          constexpr: true,
        },

        "SDL_GLProfile": {
          enum: { prefix: "SDL_GL_CONTEXT_PROFILE_" }
        },
        "SDL_GLContextFlag": {
          enum: {
            prefix: "SDL_GL_CONTEXT_",
            values: [
              "SDL_GL_CONTEXT_DEBUG_FLAG",
              "SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG",
              "SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG",
              "SDL_GL_CONTEXT_RESET_ISOLATION_FLAG"
            ]
          }
        },
        "SDL_GLContextReleaseFlag": {
          enum: {
            prefix: "SDL_GL_CONTEXT_RELEASE_",
            values: [
              "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE",
              "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH"
            ]
          }
        },
        "SDL_GLContextResetNotification": {
          enum: {
            prefix: "SDL_GL_CONTEXT_RESET_",
            values: [
              "SDL_GL_CONTEXT_RESET_NO_NOTIFICATION",
              "SDL_GL_CONTEXT_RESET_LOSE_CONTEXT"
            ]
          }
        },
        "SDL_GLContext": {
          resource: {
            free: "SDL_GL_DestroyContext",
            owning: false,
          },
          entries: {
            "SDL_GL_CreateContext": { name: "ctor" },
            "SDL_GL_MakeCurrent": {
              name: "MakeCurrent",
              static: false,
              parameters: [{ type: "WindowParam", name: "window" }]
            },
            "SDL_GL_DestroyContext": {
              name: "Destroy",
              hints: { mayFail: true },
            },
          }
        },
        "SDL_GL_CreateContext": {
          hints: { methodName: "CreateGLContext" }
        },
        "SDL_GL_MakeCurrent": {
          parameters: [{}, {}],
          hints: { methodName: "MakeCurrent" }
        },
        "SDL_EGL_GetWindowSurface": {
          hints: { methodName: "GetEGLSurface" }
        },
      }
    },
    "SDL_vulkan.h": {
      localIncludes: ["SDL3pp.h"],
      ignoreEntries: ["VK_DEFINE_HANDLE", "VK_DEFINE_NON_DISPATCHABLE_HANDLE", "NO_SDL_VULKAN_TYPEDEFS"],
      transform: {
        "SDL_Vulkan_GetInstanceExtensions": {
          type: "std::span<char const * const>",
          parameters: [],
        }
      }
    },

    // SDL satellite Libraries:
    "SDL_image.h": {
      localIncludes: [
        "SDL3pp_error.h",
        "SDL3pp_render.h",
        "SDL3pp_surface.h",
        "SDL3pp_version.h",
        "SDL3pp_video.h",
      ],
      transform: {
        "IMG": {
          kind: "ns",
          before: "IMG_Version",
          entries: {
            "IMG_Version": { name: "Version" },
          },
        },
        "SDL_IMAGE_MAJOR_VERSION": {
          value: ""
        },
        "SDL_IMAGE_MINOR_VERSION": {
          value: ""
        },
        "SDL_IMAGE_MICRO_VERSION": {
          value: ""
        },
        "IMG_Animation": {
          resource: { free: "IMG_FreeAnimation" },
          entries: {
            "IMG_LoadAnimation": "ctor",
            "IMG_LoadAnimation_IO": { name: "ctor" },
          }
        },
        "GetAnimationWidth": {
          kind: "function",
          immutable: true,
          type: "int",
          parameters: [{ type: "AnimationConstParam", name: "anim" }]
        },
        "GetAnimationHeight": {
          kind: "function",
          immutable: true,
          type: "int",
          parameters: [{ type: "AnimationConstParam", name: "anim" }]
        },
        "GetAnimationSize": {
          kind: "function",
          immutable: true,
          type: "Point",
          parameters: [{ type: "AnimationConstParam", name: "anim" }]
        },
        "GetAnimationCount": {
          kind: "function",
          immutable: true,
          type: "int",
          parameters: [{ type: "AnimationConstParam", name: "anim" }]
        },
        "GetAnimationFrame": {
          kind: "function",
          type: "Surface",
          immutable: true,
          parameters: [
            { type: "AnimationConstParam", name: "anim" },
            { type: "int", name: "index" },
          ]
        },
        "GetAnimationDelay": {
          kind: "function",
          type: "int",
          immutable: true,
          parameters: [
            { type: "AnimationConstParam", name: "anim" },
            { type: "int", name: "index" },
          ]
        },
        "IMG_LoadAnimation": { type: "Animation" },
        "IMG_LoadAnimation_IO": {
          type: "Animation",
          name: "LoadAnimation"
        },
        "IMG_LoadAnimationTyped_IO": {
          type: "Animation",
          name: "LoadAnimationTyped",
          parameters: [
            { name: "src", type: "IOStreamParam" },
            { name: "type", type: "StringParam" },
            { name: "closeio", type: "bool", default: "false" },
          ]
        },
        "IMG_LoadTyped_IO": {
          type: "Surface",
          name: "LoadSurfaceTyped",
          parameters: [
            { name: "src", type: "IOStreamParam" },
            { name: "type", type: "StringParam" },
            { name: "closeio", type: "bool", default: "false" },
          ]
        },
        "IMG_Load": {
          type: "",
          name: "Surface::Surface",
          link: {
            type: "Surface",
            name: "LoadSurface",
          }
        },
        "IMG_Load_IO": {
          type: "",
          name: "Surface::Surface",
          link: {
            type: "Surface",
            name: "LoadSurface",
          }
        },
        "IMG_LoadTexture": {
          type: "",
          name: "Texture::Texture",
          link: {
            name: "LoadTexture",
            type: "Texture"
          }
        },
        "IMG_LoadTexture_IO": {
          type: "",
          name: "Texture::Texture",
          link: {
            name: "LoadTexture",
            type: "Texture"
          }
        },
        "IMG_LoadTextureTyped_IO": {
          type: "Texture",
          name: "LoadTextureTyped",
          parameters: [
            {},
            {},
            { name: "type", type: "StringParam" },
            { name: "closeio", type: "bool", default: "false" },
          ]
        },
        "IMG_LoadAVIF_IO": {
          "type": "Surface",
          "name": "LoadAVIF"
        },
        "IMG_LoadBMP_IO": {
          "type": "Surface",
          "name": "LoadBMP",
        },
        "IMG_LoadICO_IO": {
          "type": "Surface",
          "name": "LoadICO"
        },
        "IMG_LoadCUR_IO": {
          "type": "Surface",
          "name": "LoadCUR"
        },
        "IMG_LoadGIF_IO": {
          "type": "Surface",
          "name": "LoadGIF"
        },
        "IMG_LoadJPG_IO": {
          "type": "Surface",
          "name": "LoadJPG"
        },
        "IMG_LoadJXL_IO": {
          "type": "Surface",
          "name": "LoadJXL"
        },
        "IMG_LoadLBM_IO": {
          "type": "Surface",
          "name": "LoadLBM"
        },
        "IMG_LoadPCX_IO": {
          "type": "Surface",
          "name": "LoadPCX"
        },
        "IMG_LoadPNG_IO": {
          "type": "Surface",
          "name": "LoadPNG"
        },
        "IMG_LoadPNM_IO": {
          "type": "Surface",
          "name": "LoadPNM"
        },
        "IMG_LoadSVG_IO": {
          "type": "Surface",
          "name": "LoadSVG"
        },
        "IMG_LoadQOI_IO": {
          "type": "Surface",
          "name": "LoadQOI"
        },
        "IMG_LoadTGA_IO": {
          "type": "Surface",
          "name": "LoadTGA"
        },
        "IMG_LoadTIF_IO": {
          "type": "Surface",
          "name": "LoadTIF"
        },
        "IMG_LoadXCF_IO": {
          "type": "Surface",
          "name": "LoadXCF"
        },
        "IMG_LoadXPM_IO": {
          "type": "Surface",
          "name": "LoadXPM"
        },
        "IMG_LoadXV_IO": {
          "type": "Surface",
          "name": "LoadXV"
        },
        "IMG_LoadWEBP_IO": {
          "type": "Surface",
          "name": "LoadWEBP"
        },
        "IMG_LoadSizedSVG_IO": {
          "type": "Surface",
          "name": "LoadSizedSVG"
        },
        "IMG_ReadXPMFromArray": {
          "type": "Surface",
          "name": "ReadXPMFromArray"
        },
        "IMG_ReadXPMFromArrayToRGB888": {
          "type": "Surface",
          "name": "ReadXPMFromArrayToRGB888"
        },
        "IMG_SaveAVIF_IO": {
          name: "SaveAVIF",
          parameters: [
            {},
            {},
            { name: "quality", type: "int" },
            { name: "closeio", type: "bool", default: "false" },
          ]
        },
        "IMG_SavePNG_IO": {
          name: "SavePNG",
          parameters: [
            {},
            {},
            { name: "closeio", type: "bool", default: "false" },
          ]
        },
        "IMG_SaveJPG_IO": {
          name: "SaveJPG",
          parameters: [
            {},
            {},
            { name: "quality", type: "int" },
            { name: "closeio", type: "bool", default: "false" },
          ]
        },
        "IMG_LoadGIFAnimation_IO": {
          "name": "LoadGIFAnimation"
        },
        "IMG_LoadWEBPAnimation_IO": {
          "name": "LoadWEBPAnimation"
        }
      }
    },
    "SDL_ttf.h": {
      localIncludes: [
        "SDL3pp_gpu.h",
        "SDL3pp_render.h",
        "SDL3pp_surface.h",
        "SDL3pp_version.h",
        "SDL3pp_video.h",
      ],
      transform: {
        "SDL_TTF_MAJOR_VERSION": {
          value: ""
        },
        "SDL_TTF_MINOR_VERSION": {
          value: ""
        },
        "SDL_TTF_MICRO_VERSION": {
          value: ""
        },
        "TTF_FontStyleFlags": {
          enum: "TTF_STYLE_",
          before: "TTF_Font",
        },
        "TTF_HintingFlags": {
          enum: "TTF_HINTING_",
          before: "TTF_Font",
        },
        "TTF_HorizontalAlignment": {
          enum: "TTF_HORIZONTAL_ALIGN_",
          before: "TTF_Font",
        },
        "TTF_Direction": {
          enum: "TTF_DIRECTION_",
          before: "TTF_Font",
        },
        "TTF_ImageType": {
          enum: "TTF_IMAGE_",
          before: "TTF_Font",
        },
        "TTF_PROP_FONT_CREATE_EXISTING_FONT": {
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 },
          name: "CREATE_EXISTING_FONT_POINTER"
        },
        "TTF_Font": {
          resource: true,
          entries: {
            "TTF_OpenFont": "ctor",
            "TTF_OpenFontIO": {
              name: "ctor",
              parameters: [
                {},
                { type: "float", name: "ptsize" },
                { type: "bool", name: "closeio", default: "false" },
              ],
            },
            "TTF_OpenFontWithProperties": "ctor",
            "TTF_CopyFont": {
              immutable: true,
              type: "Font",
              hints: { mayFail: true }
            },
            "TTF_GetFontProperties": "function",
            "TTF_GetFontGeneration": "immutable",
            "TTF_AddFallbackFont": "function",
            "TTF_RemoveFallbackFont": "function",
            "TTF_ClearFallbackFonts": "function",
            "TTF_SetFontSize": "function",
            "TTF_SetFontSizeDPI": "function",
            "TTF_GetFontSize": "immutable",
            "TTF_GetFontDPI": "immutable",
            "TTF_SetFontStyle": "function",
            "TTF_GetFontStyle": "immutable",
            "TTF_SetFontOutline": "function",
            "TTF_GetFontOutline": "immutable",
            "TTF_SetFontHinting": "function",
            "TTF_GetNumFontFaces": "immutable",
            "TTF_GetFontHinting": "immutable",
            "TTF_SetFontSDF": "function",
            "TTF_GetFontSDF": "immutable",
            "TTF_GetFontWeight": {
              immutable: true,
              since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 },
            },
            "TTF_SetFontWrapAlignment": "function",
            "TTF_GetFontWrapAlignment": "immutable",
            "TTF_GetFontHeight": "immutable",
            "TTF_GetFontAscent": "immutable",
            "TTF_GetFontDescent": "immutable",
            "TTF_SetFontLineSkip": "function",
            "TTF_GetFontLineSkip": "immutable",
            "TTF_SetFontKerning": "function",
            "TTF_GetFontKerning": "immutable",
            "TTF_FontIsFixedWidth": "immutable",
            "TTF_FontIsScalable": "immutable",
            "TTF_GetFontFamilyName": "immutable",
            "TTF_GetFontStyleName": "immutable",
            "TTF_SetFontDirection": "function",
            "TTF_GetFontDirection": "immutable",
            "TTF_SetFontScript": "function",
            "TTF_GetFontScript": "immutable",
            "TTF_GetGlyphScript": {
              static: true,
            },
            "TTF_SetFontLanguage": "function",
            "TTF_FontHasGlyph": "immutable",
            "TTF_GetGlyphImage": "immutable",
            "TTF_GetGlyphImageForIndex": "immutable",
            "TTF_GetGlyphMetrics": "immutable",
            "TTF_GetGlyphKerning": {
              "immutable": true,
              "type": "int",
              "parameters": [
                {},
                {},
                {}
              ],
              hints: { mayFail: true }
            },
            "GetStringSize": {
              "kind": "function",
              "immutable": true,
              "type": "Point",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                }
              ],
              hints: { mayFail: true }
            },
            "TTF_GetStringSize": {
              "immutable": true,
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "w",
                  "type": "int *"
                },
                {
                  "name": "h",
                  "type": "int *"
                }
              ]
            },
            "GetStringSizeWrapped": {
              "kind": "function",
              "immutable": true,
              "type": "Point",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "wrap_width",
                  "type": "int"
                }
              ]
            },
            "TTF_GetStringSizeWrapped": {
              "immutable": true,
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "wrap_width",
                  "type": "int"
                },
                {
                  "name": "w",
                  "type": "int *"
                },
                {
                  "name": "h",
                  "type": "int *"
                }
              ]
            },
            "TTF_MeasureString": {
              "immutable": true,
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "max_width",
                  "type": "int"
                },
                {
                  "name": "measured_width",
                  "type": "int *"
                },
                {
                  "name": "measured_length",
                  "type": "size_t *"
                }
              ]
            },
            "TTF_RenderText_Solid": {
              "immutable": true,
              "type": "Surface",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "fg",
                  "type": "Color"
                }
              ]
            },
            "TTF_RenderText_Solid_Wrapped": {
              "immutable": true,
              "type": "Surface",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "fg",
                  "type": "Color"
                },
                {
                  "name": "wrapLength",
                  "type": "int"
                }
              ]
            },
            "TTF_RenderGlyph_Solid": {
              "immutable": true,
              "type": "Surface"
            },
            "TTF_RenderText_Shaded": {
              "immutable": true,
              "type": "Surface",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "fg",
                  "type": "Color"
                },
                {
                  "name": "bg",
                  "type": "Color"
                }
              ]
            },
            "TTF_RenderText_Shaded_Wrapped": {
              "immutable": true,
              "type": "Surface",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "fg",
                  "type": "Color"
                },
                {
                  "name": "bg",
                  "type": "Color"
                },
                {
                  "name": "wrap_width",
                  "type": "int"
                }
              ]
            },
            "TTF_RenderGlyph_Shaded": {
              "immutable": true,
              "type": "Surface"
            },
            "TTF_RenderText_Blended": {
              "immutable": true,
              "type": "Surface",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "fg",
                  "type": "Color"
                }
              ]
            },
            "TTF_RenderText_Blended_Wrapped": {
              "immutable": true,
              "type": "Surface",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "fg",
                  "type": "Color"
                },
                {
                  "name": "wrap_width",
                  "type": "int"
                }
              ]
            },
            "TTF_RenderGlyph_Blended": {
              "immutable": true,
              "type": "Surface"
            },
            "TTF_RenderText_LCD": {
              "immutable": true,
              "type": "Surface",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "fg",
                  "type": "Color"
                },
                {
                  "name": "bg",
                  "type": "Color"
                }
              ]
            },
            "TTF_RenderText_LCD_Wrapped": {
              "immutable": true,
              "type": "Surface",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                },
                {
                  "name": "fg",
                  "type": "Color"
                },
                {
                  "name": "bg",
                  "type": "Color"
                },
                {
                  "name": "wrap_width",
                  "type": "int"
                }
              ]
            },
            "TTF_RenderGlyph_LCD": {
              "immutable": true,
              "type": "Surface"
            },
          }
        },
        "TTF_FONT_WEIGHT_THIN": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_FONT_WEIGHT_EXTRA_LIGHT": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_FONT_WEIGHT_LIGHT": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_FONT_WEIGHT_NORMAL": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_FONT_WEIGHT_MEDIUM": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_FONT_WEIGHT_SEMI_BOLD": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_FONT_WEIGHT_BOLD": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_FONT_WEIGHT_EXTRA_BOLD": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_FONT_WEIGHT_BLACK": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_FONT_WEIGHT_EXTRA_BLACK": {
          kind: "var", type: "int", constexpr: true,
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_OpenFontIO": {
          name: "OpenFont",
          parameters: [
            {},
            { type: "float", name: "ptsize" },
            { type: "bool", name: "closeio", default: "false" },
          ],
        },
        "TTF_TextEngine": {
          resource: {
            ref: false,
            enableAutoMethods: false,
            enableMemberAccess: false,
          },
          entries: {
            "Destroy": {
              kind: "function",
              type: "void",
              parameters: [],
            },
            "TTF_CreateText": {
              type: "Text",
              parameters: [
                {},
                {},
                {
                  type: "std::string_view"
                }
              ],
            }
          },
        },
        "SurfaceTextEngine": {
          after: "TTF_TextEngine",
          kind: "struct",
          type: "TextEngine",
          entries: {
            "TTF_CreateSurfaceTextEngine": "ctor",
            "~SurfaceTextEngine": {
              kind: "function",
              type: "",
              parameters: [],
              hints: { body: "Destroy();" },
            },
          },
        },
        "TTF_CreateSurfaceTextEngine": {
          type: "SurfaceTextEngine",
          hints: { delegate: "SurfaceTextEngine" },
        },
        "TTF_DestroySurfaceTextEngine": {
          name: "SurfaceTextEngine::Destroy",
          static: false,
          parameters: [],
          hints: { body: "DestroySurfaceTextEngine(release());" },
          link: {
            name: "DestroySurfaceTextEngine",
            parameters: [{ type: "TextEngineRaw" }],
          }
        },
        "RendererTextEngine": {
          after: "TTF_TextEngine",
          kind: "struct",
          type: "TextEngine",
          entries: {
            "TTF_CreateRendererTextEngine": "ctor",
            "TTF_CreateRendererTextEngineWithProperties": "ctor",
            "~RendererTextEngine": {
              kind: "function",
              type: "",
              parameters: [],
              hints: { body: "Destroy();" },
            },
          },
        },
        "TTF_CreateRendererTextEngine": {
          type: "RendererTextEngine",
          hints: { delegate: "RendererTextEngine" },
        },
        "TTF_CreateRendererTextEngineWithProperties": {
          type: "RendererTextEngine",
          hints: { delegate: "RendererTextEngine" },
        },
        "TTF_DestroyRendererTextEngine": {
          name: "RendererTextEngine::Destroy",
          static: false,
          parameters: [],
          hints: { body: "DestroyRendererTextEngine(release());" },
          link: {
            name: "DestroyRendererTextEngine",
            parameters: [{ type: "TextEngineRaw" }],
          }
        },
        "GPUTextEngine": {
          after: "TTF_TextEngine",
          kind: "struct",
          type: "TextEngine",
          entries: {
            "TTF_CreateGPUTextEngine": "ctor",
            "TTF_CreateGPUTextEngineWithProperties": "ctor",
            "~GPUTextEngine": {
              kind: "function",
              type: "",
              parameters: [],
              hints: { body: "Destroy();" },
            },
            "TTF_SetGPUTextEngineWinding": {
              "name": "SetGPUWinding"
            },
            "TTF_GetGPUTextEngineWinding": {
              "immutable": true,
              "name": "GetGPUWinding"
            },
          },
        },
        "TTF_CreateGPUTextEngine": {
          type: "GPUTextEngine",
          hints: { delegate: "GPUTextEngine" },
        },
        "TTF_CreateGPUTextEngineWithProperties": {
          type: "GPUTextEngine",
          hints: { delegate: "GPUTextEngine" },
        },
        "TTF_DestroyGPUTextEngine": {
          name: "GPUTextEngine::Destroy",
          static: false,
          parameters: [],
          hints: { body: "DestroyGPUTextEngine(release());" },
          link: {
            name: "DestroyGPUTextEngine",
            parameters: [{ type: "TextEngineRaw" }],
          }
        },
        "TTF_PROP_RENDERER_TEXT_ENGINE_RENDERER": {
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_PROP_RENDERER_TEXT_ENGINE_ATLAS_TEXTURE_SIZE": {
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_PROP_GPU_TEXT_ENGINE_DEVICE": {
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_PROP_GPU_TEXT_ENGINE_ATLAS_TEXTURE_SIZE": {
          since: { tag: "SDL_TTF", major: 3, minor: 2, patch: 2 }
        },
        "TTF_Text": {
          resource: true,
          entries: {
            "TTF_DrawSurfaceText": {
              "immutable": true,
              "parameters": [
                {
                  "type": "Point",
                  "name": "p"
                },
                {
                  "type": "SurfaceParam",
                  "name": "surface"
                }
              ]
            },
            "TTF_DrawRendererText": {
              "immutable": true,
              "parameters": [
                {
                  "type": "FPoint",
                  "name": "p"
                }
              ]
            },
            "TTF_GetGPUTextDrawData": "immutable",
            "TTF_CreateText": {
              "name": "ctor",
              "type": "",
              "parameters": [
                {
                  "type": "TextEngineParam",
                  "name": "engine"
                },
                {
                  "type": "FontParam",
                  "name": "font"
                },
                {
                  "type": "std::string_view",
                  "name": "text"
                }
              ]
            },
            "TTF_GetTextProperties": "immutable",
            "TTF_SetTextEngine": {
              parameters: [
                {},
                {
                  name: "engine",
                  type: "TextEngineParam"
                }
              ],
            },
            "TTF_GetTextEngine": {
              type: "TextEngineParam",
              immutable: true
            },
            "TTF_SetTextFont": "function",
            "TTF_GetTextFont": "immutable",
            "TTF_SetTextDirection": "function",
            "TTF_GetTextDirection": "immutable",
            "TTF_SetTextScript": "function",
            "TTF_GetTextScript": "immutable",
            "TTF_SetTextColor": {
              "static": false,
              "parameters": [
                {
                  "type": "Color",
                  "name": "c"
                }
              ]
            },
            "TTF_SetTextColorFloat": {
              "static": false,
              "parameters": [
                {
                  "type": "FColor",
                  "name": "c"
                }
              ]
            },
            "TTF_GetTextColor": "immutable",
            "TTF_GetTextColorFloat": "immutable",
            "TTF_SetTextPosition": {
              type: "void",
              parameters: [
                {},
                { type: "const PointRaw &", name: "p" }
              ],
              hints: { mayFail: true },
            },
            "TTF_GetTextPosition": {
              immutable: true,
              type: "void",
              hints: { mayFail: true },
            },
            "TTF_SetTextWrapWidth": "function",
            "TTF_GetTextWrapWidth": {
              "immutable": true,
              "type": "int",
              "parameters": []
            },
            "TTF_SetTextWrapWhitespaceVisible": "function",
            "TTF_TextWrapWhitespaceVisible": {
              "name": "IsWrapWhitespaceVisible",
              "immutable": true
            },
            "TTF_SetTextString": {
              "static": false,
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "string"
                }
              ]
            },
            "TTF_InsertTextString": {
              "static": false,
              "parameters": [
                {
                  "type": "int",
                  "name": "offset"
                },
                {
                  "type": "std::string_view",
                  "name": "string"
                }
              ]
            },
            "TTF_AppendTextString": {
              "static": false,
              "parameters": [
                {
                  "type": "std::string_view",
                  "name": "string"
                }
              ]
            },
            "TTF_DeleteTextString": "function",
            "TTF_GetTextSize": "immutable",
            "TTF_GetTextSubString": "immutable",
            "begin": {
              "kind": "function",
              "type": "SubStringIterator",
              "immutable": true,
              "parameters": []
            },
            "end": {
              "kind": "function",
              "type": "SubStringIterator",
              "immutable": true,
              "parameters": []
            },
            "GetSubStringForLine": {
              "kind": "function",
              "type": "SubStringIterator",
              "immutable": true,
              "parameters": [
                {
                  "type": "int",
                  "name": "line"
                }
              ]
            },
            "TTF_GetTextSubStringForLine": "immutable",
            "GetSubStrings": {
              "kind": "function",
              "immutable": true,
              "type": "OwnArray<SubString *>",
              "parameters": []
            },
            "TTF_GetTextSubStringsForRange": {
              "immutable": true,
              "type": "OwnArray<SubString *>",
              "parameters": [
                {},
                {},
                {}
              ]
            },
            "GetSubStringForPoint": {
              "kind": "function",
              "immutable": true,
              "type": "SubStringIterator",
              "parameters": [
                {
                  "type": "Point",
                  "name": "p"
                }
              ]
            },
            "TTF_GetTextSubStringForPoint": {
              "immutable": true,
              "parameters": [
                {
                  "type": "Point",
                  "name": "p"
                },
                {
                  "type": "SubString *",
                  "name": "substring"
                }
              ]
            },
            "TTF_GetPreviousTextSubString": "immutable",
            "TTF_GetNextTextSubString": "immutable",
            "TTF_UpdateText": "function",
            "GetText": {
              kind: "function",
              type: "const char *",
              immutable: true,
              doc: ["A copy of the UTF-8 string that this text object represents, useful for layout, debugging and retrieving substring text"],
              parameters: []
            },
            "GetNumLines": {
              kind: "function",
              type: "int",
              immutable: true,
              doc: ["The number of lines in the text, 0 if it's empty"],
              parameters: []
            }
          }
        },
        "GetTextColor": {
          after: "TTF_GetTextColor",
          kind: "function",
          type: "Color",
          parameters: [{ type: "TextParam", name: "text" }],
        },
        "GetTextColorFloat": {
          after: "TTF_GetTextColorFloat",
          kind: "function",
          type: "FColor",
          parameters: [{ type: "TextParam", name: "text" }],
        },
        "GetTextPosition": {
          after: "TTF_GetTextPosition",
          kind: "function",
          type: "Point",
          parameters: [{ type: "TextParam", name: "text" }]
        },
        "GetTextSize": {
          after: "TTF_GetTextSize",
          kind: "function",
          type: "Point",
          parameters: [{ type: "TextParam", name: "text" }]
        },
        "TTF_SubStringFlags": {
          enum: "TTF_SUBSTRING_",
          before: "TTF_TextEngine",
        },
        "TTF_GPUTextEngineWinding": {
          enum: "TTF_GPU_TEXTENGINE_WINDING_",
          before: "TTF_TextEngine",
        },
        "TTF_GPUAtlasDrawSequence": { after: "TTF_TextEngine" },
        "TTF_SubString": { after: "TTF_TextEngine" },
        "SubStringIterator#forward": {
          name: "SubStringIterator",
          kind: "forward",
          after: "TTF_TextEngine"
        },
        "SubStringIterator": {
          after: "TTF_Text",
          kind: "struct",
          entries: {
            "m_text": {
              "kind": "var",
              "type": "TextRef"
            },
            "m_subString": {
              "kind": "var",
              "type": "SubString"
            },
            "SubStringIterator": {
              kind: "function",
              type: "",
              constexpr: true,
              parameters: [
                {
                  "name": "text",
                  "type": "TextRef"
                }
              ]
            },
            "SubStringIterator#2": {
              kind: "function",
              type: "",
              constexpr: true,
              parameters: [],
              hints: { changeAccess: "public" }
            },
            "operator bool": {
              "kind": "function",
              "type": "",
              "constexpr": true,
              "immutable": true,
              "parameters": []
            },
            "operator*": {
              "kind": "function",
              "type": "const SubString &",
              "constexpr": true,
              "immutable": true,
              "parameters": []
            },
            "operator->": {
              "kind": "function",
              "type": "const SubString *",
              "constexpr": true,
              "immutable": true,
              "parameters": []
            },
            "operator==": {
              "kind": "function",
              "type": "bool",
              "constexpr": true,
              "immutable": true,
              "parameters": [
                {
                  "name": "other",
                  "type": "const SubStringIterator &"
                }
              ]
            },
            "operator++": {
              kind: "function",
              type: "SubStringIterator &",
              constexpr: true,
              parameters: []
            },
            "operator++#2": {
              kind: "function",
              type: "SubStringIterator",
              constexpr: true,
              parameters: [
                { type: "int" }
              ]
            },
            "operator--": {
              kind: "function",
              type: "SubStringIterator &",
              constexpr: true,
              parameters: []
            },
            "operator--#2": {
              kind: "function",
              type: "SubStringIterator",
              constexpr: true,
              parameters: [
                { type: "int" }
              ]
            },
          },
          hints: { private: true },
        },
        "TTF": {
          kind: "ns",
          before: "TTF_Version",
          entries: {},
        },
        "TTF_Version": { name: "TTF.Version" },
        "TTF_Init": { name: "TTF.Init" },
        "TTF_Quit": { name: "TTF.Quit" },
        "TTF_WasInit": { name: "TTF.WasInit" }
      },
      namespacesMap: {
        "TTF_PROP_FONT_": "prop::Font",
        "TTF_PROP_RENDERER_TEXT_ENGINE_": "prop::RendererTextEngine",
        "TTF_PROP_GPU_TEXT_ENGINE_": "prop::GpuTextEngine"
      },
    }
  }
};

if (process.argv[2]) {
  writeJSONSync(process.argv[2], transform);
} else {
  writeJSONSync(1, transform);
}
