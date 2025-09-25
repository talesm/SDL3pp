const { writeJSONSync } = require("./cppfier/src/utils");

/**
 * @import {ApiTransform} from "./cppfier/src/types"
 */

/** @type {ApiTransform} */
const transform = {
  prefixes: ["SDL_", "IMG_", "TTF_"],
  definitionPrefix: "SDL_",
  sourceIncludePrefix: 'SDL3/',
  namespace: "SDL",
  renameRules: [{
    // @ts-ignore
    pattern: "^SDL_(.*)\\.h$",
    replacement: "SDL3pp_$1.h"
  }],
  minVersions: {
    SDL: { tag: "SDL", major: 3, minor: 2, patch: 0 },
    SDL_TTF: { tag: "SDL_TTF", major: 3, minor: 2, patch: 0 },
    SDL_IMAGE: { tag: "SDL_IMAGE", major: 3, minor: 2, patch: 0 },
  },
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
        "SDL_SetAssertionHandler": {},
        "SetAssertionHandler": {
          kind: "function",
          type: "void",
          parameters: [{ type: "AssertionHandlerCB", name: "handler" }]
        },
        "SDL_GetAssertionHandler": {},
        "GetAssertionHandler": {
          kind: "function",
          type: "AssertionHandlerCB",
          parameters: []
        },
      }
    },
    "SDL_atomic.h": {
      ignoreEntries: [
        "SDL_SpinLock",
        "SDL_TryLockSpinlock",
        "SDL_LockSpinlock",
        "SDL_UnlockSpinlock",
      ],
      includeAfter: {
        "__begin": [
          { name: "MemoryBarrierRelease" },
          { name: "MemoryBarrierAcquire" },
        ],
        "SDL_AtomicU32": {
          name: "AtomicPointer",
          kind: "struct",
          template: [{
            type: "class",
            name: "T"
          }],
          hints: {
            self: "&m_value",
          },
          entries: {
            "m_value": {
              kind: "var",
              type: "T *",
            },
            "AtomicPointer": [
              {
                kind: "function",
                type: "",
                constexpr: true,
                parameters: [{
                  type: "T *",
                  name: "value"
                }],
                hints: {
                  init: ["m_value(value)"]
                }
              },
              {
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
              }
            ],
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
      },
      transform: {
        "SDL_MemoryBarrierReleaseFunction": { name: "MemoryBarrierRelease" },
        "SDL_MemoryBarrierAcquireFunction": { name: "MemoryBarrierAcquire" },
        "SDL_AtomicInt": {
          hints: {
            self: "&m_value",
          },
          entries: {
            "m_value": {
              kind: "var",
              type: "SDL_AtomicInt",
            },
            "AtomicInt": [
              {
                kind: "function",
                type: "",
                constexpr: true,
                parameters: [{
                  type: "int",
                  name: "value"
                }],
                hints: {
                  init: ["m_value(value)"]
                }
              },
              {
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
              }
            ],
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
            "operator SDL_AtomicInt *": {
              kind: "function",
              type: "",
              parameters: [],
              constexpr: true,
              hints: {
                body: "return &m_value;"
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
          hints: {
            self: "&m_value",
          },
          entries: {
            "m_value": {
              kind: "var",
              type: "SDL_AtomicU32",
            },
            "AtomicU32": [
              {
                kind: "function",
                type: "",
                constexpr: true,
                parameters: [{
                  type: "Uint32",
                  name: "value"
                }],
                hints: {
                  init: ["m_value(value)"]
                }
              },
              {
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
              }
            ],
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
            "operator SDL_AtomicU32 *": {
              kind: "function",
              type: "",
              parameters: [],
              constexpr: true,
              hints: {
                body: "return &m_value;"
              }
            },
            "SDL_CompareAndSwapAtomicU32": "function",
            "SDL_SetAtomicU32": "function",
            "SDL_GetAtomicU32": "function",
          }
        },
      }
    },
    "SDL_audio.h": {
      includeBefore: {
        "SDL_AudioDeviceID": [
          { name: "AudioPostmixCallback" },
          { name: "AudioPostmixCB" },
        ],
        "SDL_AudioStream": [
          { name: "AudioStreamCallback" },
          { name: "AudioStreamCB" },
        ],
      },
      includeAfter: {
        "__begin": {
          name: "AudioSpec"
        },
        "SDL_MixAudio": {
          kind: "function",
          name: "MixAudio",
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
      },
      wrappers: {
        "SDL_AudioFormat": {
          entries: {
            "SDL_DEFINE_AUDIO_FORMAT": {
              name: "ctor",
              constexpr: true,
              parameters: [
                { type: "bool", name: "sign" },
                { type: "bool", name: "bigendian" },
                { type: "bool", name: "flt" },
                { type: "Uint16", name: "size" },
              ]
            },
            "SDL_AUDIO_BITSIZE": {
              kind: "function",
              name: "GetBitSize",
              constexpr: true,
              type: "Uint16",
              static: false,
              parameters: []
            },
            "SDL_AUDIO_BYTESIZE": {
              kind: "function",
              name: "GetByteSize",
              constexpr: true,
              type: "Uint16",
              static: false,
              parameters: []
            },
            "SDL_AUDIO_ISFLOAT": {
              kind: "function",
              name: "IsFloat",
              constexpr: true,
              type: "bool",
              static: false,
              parameters: []
            },
            "SDL_AUDIO_ISBIGENDIAN": {
              kind: "function",
              name: "IsBigEndian",
              constexpr: true,
              type: "bool",
              static: false,
              parameters: []
            },
            "SDL_AUDIO_ISLITTLEENDIAN": {
              kind: "function",
              name: "IsLittleEndian",
              constexpr: true,
              type: "bool",
              static: false,
              parameters: []
            },
            "SDL_AUDIO_ISSIGNED": {
              kind: "function",
              name: "IsSigned",
              constexpr: true,
              type: "bool",
              static: false,
              parameters: []
            },
            "SDL_AUDIO_ISINT": {
              kind: "function",
              name: "IsInt",
              constexpr: true,
              type: "bool",
              static: false,
              parameters: []
            },
            "SDL_AUDIO_ISUNSIGNED": {
              kind: "function",
              name: "IsUnsigned",
              constexpr: true,
              type: "bool",
              static: false,
              parameters: []
            },
            "SDL_GetAudioFormatName": "immutable",
            "SDL_GetSilenceValueForFormat": {
              immutable: true,
              name: "GetSilenceValue",
            },
          }
        },
      },
      resources: {
        "SDL_AudioDeviceID": {
          name: "AudioDevice",
          free: "SDL_CloseAudioDevice",
          entries: {
            "SDL_OpenAudioDevice": {
              name: "ctor",
              parameters: [
                { type: "AudioDeviceParam" },
                { type: "OptionalRef<const AudioSpecRaw>" }
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
            "SetPostmixCallback": {
              kind: "function",
              proto: true,
              type: "void",
              parameters: [{
                type: "AudioPostmixCB",
                name: "callback"
              }]
            },
            "SDL_SetAudioPostmixCallback": {
              name: "SetPostmixCallback"
            },
          }
        },
        "SDL_AudioStream": {
          lock: true,
          lockFunction: "SDL_LockAudioStream",
          unlockFunction: "SDL_UnlockAudioStream",
          entries: {
            "SDL_CreateAudioStream": {
              name: "ctor",
              parameters: [
                { type: "OptionalRef<const AudioSpec>" },
                { type: "OptionalRef<const AudioSpec>" },
              ]
            },
            "OpenAudioDeviceStream": {
              kind: "function",
              type: "AudioStream",
              name: "ctor",
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
                }
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
            "SDL_LockAudioStream": {
              type: "AudioStreamLock"
            },
            "SetGetCallback": {
              kind: "function",
              type: "void",
              proto: true,
              parameters: [{
                name: "callback",
                type: "AudioStreamCB"
              }],
            },
            "SDL_SetAudioStreamGetCallback": "function",
            "SetPutCallback": {
              kind: "function",
              type: "void",
              proto: true,
              parameters: [{
                name: "callback",
                type: "AudioStreamCB"
              }],
            },
            "SDL_SetAudioStreamPutCallback": "function",
            "Bind": {
              kind: "function",
              type: "void",
              doc: "@see AudioDeviceRef.BindAudioStream",
              parameters: [{
                type: "AudioDeviceParam",
                name: "devid"
              }]
            },
            "SDL_UnbindAudioStream": "function",
            "SDL_GetAudioStreamDevice": "immutable",
          }
        }
      },
      transform: {
        "AudioPostmixCB": {
          type: "std::function<void(const AudioSpec &spec, std::span<float> buffer)>"
        },
        "AudioStreamCB": {
          type: "std::function<void(AudioStreamRef stream, int additional_amount, int total_amount)>"
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
    "SDL_clipboard.h": {
      includeAfter: {
        "SDL_SetClipboardData": {
          name: "SetClipboardData",
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
          name: "GetClipboardDataAs",
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
      },
      transform: {
        "SDL_GetClipboardText": { type: "StringResult" },
        "SDL_GetPrimarySelectionText": { type: "StringResult" },
        "ClipboardDataCB": {
          type: "std::function<SourceBytes(const char *mime_type)>"
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
        "SDL_GetClipboardData": {
          type: "StringResult",
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
      transform: {
        "SDL_CACHELINE_SIZE": {
          kind: "var",
          type: "int",
          constexpr: true
        }
      }
    },
    "SDL_dialog.h": {
      namespacesMap: {
        "SDL_PROP_FILE_DIALOG_": "prop::FileDialog"
      },
      includeAfter: {
        "SDL_ShowOpenFileDialog": {
          "name": "ShowOpenFileDialog",
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
        "SDL_ShowSaveFileDialog": {
          "name": "ShowSaveFileDialog",
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
        "SDL_ShowOpenFolderDialog": {
          "name": "ShowOpenFolderDialog",
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
        "SDL_ShowFileDialogWithProperties": {
          "name": "ShowFileDialogWithProperties",
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
        }
      },
      enumerations: {
        "SDL_FileDialogType": {
          "prefix": "SDL_FILEDIALOG_"
        }
      },
      transform: {
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
      includeAfter: {
        "SDL_GetError": [],
      },
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
            "Error": [{
              kind: "function",
              type: "",
              parameters: [],
              doc: "Default ctor.",
              hints: { init: ["m_message(SDL_GetError())"] }
            }, {
              kind: "function",
              type: "",
              parameters: [{ type: "std::string", name: "message" }],
              hints: { init: ["m_message(std::move(message))"] }
            }],
            "what": {
              kind: "function",
              type: "const char *",
              immutable: true,
              parameters: [],
              doc: "Returns the explanatory string.",
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
      definitionPrefix: "SDL_",
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
        "EventWatchHandle": {
          doc: "Handle returned by AddEventWatch()",
          kind: "struct",
          after: "SDL_EventFilter",
          type: "CallbackHandle",
          entries: { "CallbackHandle::CallbackHandle": "alias" }
        },
        "SetEventFilter": {
          "kind": "function",
          "after": "SDL_SetEventFilter",
          "type": "void",
          "parameters": [
            {
              "type": "EventFilterCB",
              "name": "filter",
              "default": "{}"
            }
          ]
        },
        "GetEventFilter": {
          kind: "function",
          after: "SDL_GetEventFilter",
          type: "EventFilterCB",
          parameters: []
        },
        "EventWatchAuxCallback": {
          kind: "function",
          type: "bool",
          parameters: [
            {
              "type": "void *",
              "name": "userdata"
            },
            {
              "type": "Event *",
              "name": "event"
            }
          ]
        },
        "AddEventWatch": {
          "kind": "function",
          "after": "SDL_AddEventWatch",
          "type": "EventWatchHandle",
          "parameters": [
            {
              "type": "EventFilterCB",
              "name": "filter"
            }
          ]
        },
        "RemoveEventWatch": {
          "kind": "function",
          "after": "SDL_RemoveEventWatch",
          "type": "void",
          "parameters": [
            {
              "type": "EventWatchHandle",
              "name": "handle"
            }
          ]
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
        "EventFilterCB": {
          kind: "alias",
          type: "std::function<bool(const Event &)>",
          after: "EventFilter",
        },
        "SDL_GetWindowFromEvent": {
          parameters: [{ type: "const Event &" }]
        }
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
            "operator+=": [{
              kind: "function",
              type: "Path &",
              parameters: [{
                type: "std::string_view",
                name: "other"
              }]
            }, {
              kind: "function",
              type: "Path &",
              parameters: [{
                type: "char",
                name: "ch"
              }]
            }],
            "operator+": [{
              kind: "function",
              type: "Path",
              immutable: true,
              parameters: [{
                type: "std::string_view",
                name: "other"
              }]
            }, {
              kind: "function",
              type: "Path",
              immutable: true,
              parameters: [{
                type: "char",
                name: "ch"
              }]
            }],
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
        "EnumerateDirectoryCB": {
          kind: "alias",
          type: "std::function<EnumerationResult(const char *dirname, const char *fname)>",
          after: "SDL_EnumerateDirectoryCallback",
        },
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
    "SDL_guid.h": {
      transform: {
        "SDL_GUID": {
          "kind": "struct",
          "type": "SDL_GUID",
          "entries": {
            "GUID": [
              {
                "kind": "function",
                "constexpr": true,
                "type": "",
                "parameters": []
              },
              {
                "kind": "function",
                "constexpr": true,
                "type": "",
                "parameters": [
                  {
                    "type": "SDL_GUID",
                    "name": "guid"
                  }
                ]
              }
            ],
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
        "HintCallbackHandle": {
          after: "SDL_HintCallback",
          doc: "Handle returned by AddHintCallback()",
          kind: "struct",
          name: "HintCallbackHandle",
          type: "CallbackHandle",
          entries: { "CallbackHandle::CallbackHandle": "alias" }
        },
        "SDL_AddHintCallback": {},
        "AddHintCallback": {
          kind: "function",
          type: "HintCallbackHandle",
          parameters: [{
            name: "name",
            type: "StringParam"
          },
          {
            name: "callback",
            type: "HintCB"
          }],
        },
        "SDL_RemoveHintCallback": {},
        "RemoveHintCallback": {
          kind: "function",
          type: "void",
          parameters: [{
            name: "name",
            type: "StringParam"
          }, {
            type: "HintCallbackHandle",
            name: "handle",
          }],
        },
      }
    },
    "SDL_init.h": {
      localIncludes: ["SDL3pp_callbackWrapper.h", "SDL3pp_error.h", "SDL3pp_log.h", "SDL3pp_strings.h"],
      includes: ["atomic"],
      namespacesMap: {
        "SDL_PROP_APP_METADATA_": "prop::appMetaData",
      },
      transform: {
        "SDL_InitFlags": {
          enum: "SDL_INIT_"
        },
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
              "doc": "@cat formatted-string",
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
              "doc": "@cat formatted-string",
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
          parameters: [
            {
              type: "StringParam",
              name: "file"
            },
            {
              type: "SourceBytes",
              name: "data"
            }
          ]
        },
      }
    },
    "SDL_keyboard.h": {
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
      enumerations: {
        "SDL_Keycode": {
          prefix: "SDLK_",
          newPrefix: "KEYCODE_"
        },
        "SDL_Keymod": {
          prefix: "SDL_KMOD_",
          includeAfter: "__begin"
        }
      },
      wrappers: {
        "SDL_Keycode": {
          ordered: true,
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
        }
      },
      transform: {
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
      resources: {
        "SDL_SharedObject": {
          free: "SDL_UnloadObject",
          ctors: ["SDL_LoadObject"],
          entries: {
            "SDL_LoadObject": { name: "Load" },
            "SDL_LoadFunction": "function",
            "SDL_UnloadObject": { name: "Unload" },
          }
        }
      },
    },
    "SDL_locale.h": {
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
        "SDL_GetLogOutputFunction": {},
        "GetLogOutputFunction": {
          name: "GetLogOutputFunction",
          kind: "function",
          type: "LogOutputCB",
          parameters: [],
          after: "SDL_GetLogOutputFunction",
        },
        "SDL_SetLogOutputFunction": {},
        "SetLogOutputFunction": {
          name: "SetLogOutputFunction",
          kind: "function",
          type: "void",
          after: "SDL_SetLogOutputFunction",
          parameters: [{
            type: "LogOutputCB",
            name: "callback"
          }]
        },
        "ResetLogOutputFunction": {
          kind: "function",
          type: "void",
          parameters: []
        },
        "LogOutputCB": {
          type: "std::function<void(LogCategory, LogPriority, const char *)>"
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
        }
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
        "SDL_AppInit",
        "SDL_AppIterate",
        "SDL_AppEvent",
        "SDL_AppQuit"
      ],
      enableException: false,
      transform: {}
    },
    "SDL_messagebox.h": {
      enumerations: {
        "SDL_MessageBoxFlags": {
          values: [
            "SDL_MESSAGEBOX_ERROR",
            "SDL_MESSAGEBOX_WARNING",
            "SDL_MESSAGEBOX_INFORMATION",
            "SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT",
            "SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT"
          ]
        },
        "SDL_MessageBoxButtonFlags": {
          prefix: "SDL_MESSAGEBOX_BUTTON_"
        },
        "SDL_MessageBoxColorType": {
          prefix: "SDL_MESSAGEBOX_COLOR_"
        }
      },
      transform: {
        "SDL_MessageBoxData": {
          name: "MessageBox",
          type: "SDL_MessageBoxData",
          entries: {
            "MessageBox": [
              {
                kind: "function",
                type: "",
                constexpr: true,
                parameters: [
                  {
                    type: "const SDL_MessageBoxData &",
                    name: "messageBox",
                    default: "{}"
                  }
                ]
              },
              {
                kind: "function",
                type: "",
                constexpr: true,
                parameters: [
                  {
                    type: "MessageBoxFlags",
                    name: "flags"
                  },
                  {
                    type: "WindowParam",
                    name: "window"
                  },
                  {
                    type: "const char *",
                    name: "title"
                  },
                  {
                    type: "const char *",
                    name: "message"
                  },
                  {
                    type: "std::span<const MessageBoxButtonData>",
                    name: "buttons"
                  },
                  {
                    type: "OptionalRef<const MessageBoxColorScheme>",
                    name: "colorScheme"
                  }
                ]
              }
            ],
            "SDL_ShowMessageBox": "function"
          }
        },
        "SDL_ShowSimpleMessageBox": {
          parameters: [{}, {}, {}, { type: "WindowParam" }]
        }
      }
    },
    "SDL_mouse.h": {
      enumerations: {
        "SDL_SystemCursor": {
          prefix: "SDL_SYSTEM_CURSOR_",
          includeAfter: "__begin",
        },
        "MouseButton": {
          kind: "alias",
          type: "Uint8",
          prefix: "SDL_BUTTON_",
          includeAfter: "SDL_Cursor",
          values: [
            "SDL_BUTTON_LEFT",
            "SDL_BUTTON_MIDDLE",
            "SDL_BUTTON_RIGHT",
            "SDL_BUTTON_X1",
            "SDL_BUTTON_X2",
          ],
        },
        "SDL_MouseButtonFlags": {
          prefix: "SDL_BUTTON_",
          values: [
            "SDL_BUTTON_LMASK",
            "SDL_BUTTON_MMASK",
            "SDL_BUTTON_RMASK",
            "SDL_BUTTON_X1MASK",
            "SDL_BUTTON_X2MASK",
          ],
        },
      },
      resources: {
        "SDL_Cursor": {
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
        }
      },
      transform: {
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
      resources: {
        "SDL_Mutex": {
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
      },
      enumerations: {
        "SDL_InitStatus": {
          prefix: "INIT_STATUS_",
        },
      },
      wrappers: {
        "SDL_InitState": {
          invalidState: false,
          genCtor: false,
          genMembers: false,
          comparable: false,
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
    "SDL_pixels.h": {
      localIncludes: ["SDL3pp_spanRef.h", "SDL3pp_error.h", "SDL3pp_version.h"],
      transform: {
        "Color": { kind: "forward" },
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
        "SDL_DEFINE_PIXELFOURCC": {
          kind: "function",
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
        "SDL_PixelFormat": {
          kind: 'struct',
          enum: "SDL_PIXELFORMAT_",
          before: "SDL_DEFINE_PIXELFORMAT",
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
            "SDL_PIXELFLAG": {
              kind: "function",
              name: "GetFlags",
              parameters: [],
              type: "Uint8",
              immutable: true,
              constexpr: true,
            },
            "SDL_PIXELTYPE": {
              "kind": "function",
              "name": "GetType",
              "parameters": [],
              "type": "PixelType",
              "immutable": true,
              "constexpr": true
            },
            "SDL_PIXELORDER": {
              "kind": "function",
              "name": "GetOrder",
              "parameters": [],
              "type": "int",
              "immutable": true,
              "constexpr": true
            },
            "SDL_PIXELLAYOUT": {
              "kind": "function",
              "name": "GetLayout",
              "parameters": [],
              "type": "PackedLayout",
              "immutable": true,
              "constexpr": true
            },
            "SDL_BITSPERPIXEL": {
              "kind": "function",
              "name": "GetBitsPerPixel",
              "parameters": [],
              "type": "int",
              "immutable": true,
              "constexpr": true
            },
            "SDL_BYTESPERPIXEL": {
              "kind": "function",
              "name": "GetBytesPerPixel",
              "parameters": [],
              "type": "int",
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISPIXELFORMAT_INDEXED": {
              "kind": "function",
              "name": "IsIndexed",
              "parameters": [],
              "type": "bool",
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISPIXELFORMAT_PACKED": {
              "kind": "function",
              "name": "IsPacked",
              "parameters": [],
              "type": "bool",
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISPIXELFORMAT_ARRAY": {
              "kind": "function",
              "name": "IsArray",
              "parameters": [],
              "type": "bool",
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISPIXELFORMAT_10BIT": {
              "kind": "function",
              "name": "Is10Bit",
              "parameters": [],
              "type": "bool",
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISPIXELFORMAT_FLOAT": {
              "kind": "function",
              "name": "IsFloat",
              "parameters": [],
              "type": "bool",
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISPIXELFORMAT_ALPHA": {
              "kind": "function",
              "name": "IsAlpha",
              "parameters": [],
              "type": "bool",
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISPIXELFORMAT_FOURCC": {
              "kind": "function",
              "name": "IsFourCC",
              "parameters": [],
              "type": "bool",
              "immutable": true,
              "constexpr": true
            },
            "SDL_GetPixelFormatName": "immutable",
            "SDL_GetMasksForPixelFormat": {
              "kind": "function",
              "immutable": true,
              "name": "GetMasks"
            },
            "SDL_GetPixelFormatForMasks": {
              "kind": "function",
              "name": "ForMasks"
            },
            "SDL_GetPixelFormatDetails": "immutable",
            "Map": {
              kind: "function",
              type: "Uint32",
              immutable: true,
              proto: true,
              parameters: [
                {
                  "type": "Color",
                  "name": "color"
                },
                {
                  "type": "PaletteConstParam",
                  "name": "palette"
                }
              ]
            },
            "Get": {
              kind: "function",
              type: "Color",
              immutable: true,
              proto: true,
              parameters: [
                {
                  "type": "Uint32",
                  "name": "pixel"
                },
                {
                  "type": "PaletteConstParam",
                  "name": "palette"
                }
              ]
            }
          }
        },
        "SDL_PIXELFORMAT_MJPG": {
          since: { tag: "SDL", major: 3, minor: 2, patch: 10 },
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
            "SDL_COLORSPACETYPE": {
              "kind": "function",
              "name": "GetType",
              "type": "ColorType",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_COLORSPACERANGE": {
              "kind": "function",
              "name": "GetRange",
              "type": "ColorRange",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_COLORSPACECHROMA": {
              "kind": "function",
              "name": "GetChroma",
              "type": "ChromaLocation",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_COLORSPACEPRIMARIES": {
              "kind": "function",
              "name": "GetPrimaries",
              "type": "ColorPrimaries",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_COLORSPACETRANSFER": {
              "kind": "function",
              "name": "GetTransfer",
              "type": "TransferCharacteristics",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_COLORSPACEMATRIX": {
              "kind": "function",
              "name": "GetMatrix",
              "type": "MatrixCoefficients",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISCOLORSPACE_MATRIX_BT601": {
              "kind": "function",
              "name": "IsMatrixBT601",
              "type": "bool",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISCOLORSPACE_MATRIX_BT709": {
              "kind": "function",
              "name": "IsMatrixBT709",
              "type": "bool",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISCOLORSPACE_MATRIX_BT2020_NCL": {
              "kind": "function",
              "name": "IsMatrixBT2020_NCL",
              "type": "bool",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISCOLORSPACE_LIMITED_RANGE": {
              "kind": "function",
              "name": "IsLimitedRange",
              "type": "bool",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            },
            "SDL_ISCOLORSPACE_FULL_RANGE": {
              "kind": "function",
              "name": "IsFullRange",
              "type": "bool",
              "parameters": [],
              "immutable": true,
              "constexpr": true
            }
          }
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
            enableConstParam: true,
          },
          entries: {
            "SDL_CreatePalette": "ctor",
            "GetSize": {
              kind: "function",
              type: "int",
              immutable: true,
              constexpr: true,
              parameters: []
            },
            "operator[]": {
              kind: "function",
              type: "Color",
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
        }
      }
    },
    "SDL_process.h": {
      enumerations: {
        "SDL_ProcessIO": {
          prefix: "SDL_PROCESS_STDIO_",
          includeAfter: "__begin",
        },
      },
      resources: {
        "SDL_Process": {
          free: "SDL_DestroyProcess",
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
            enableAutoMethods: false,
          },
          entries: {
            "SDL_CreateProperties": {
              name: "Create",
            },
            "SDL_DestroyProperties": "function",
            "SDL_LockProperties": "function",
            "SDL_UnlockProperties": "function",
            "SDL_SetPointerProperty": "function",
            "SDL_SetStringProperty": "function",
            "SDL_SetNumberProperty": "function",
            "SDL_SetFloatProperty": "function",
            "SDL_SetBooleanProperty": "function",
            "SDL_HasProperty": "immutable",
            "SDL_GetPropertyType": "immutable",
            "SDL_GetPointerProperty": "immutable",
            "SDL_GetStringProperty": "immutable",
            "SDL_GetNumberProperty": "immutable",
            "SDL_GetFloatProperty": "immutable",
            "SDL_GetBooleanProperty": "immutable",
            "SDL_ClearProperty": "immutable",
            "Enumerate": [{
              kind: "function",
              type: "void",
              immutable: true,
              template: [{
                type: "std::output_iterator<const char *>",
                name: "IT"
              }],
              parameters: [{
                type: "IT",
                name: "outputIter"
              }]
            },
            {
              kind: "function",
              type: "void",
              immutable: true,
              proto: true,
              parameters: [{
                type: "EnumeratePropertiesCB",
                name: "callback"
              }]
            }],
            "SDL_EnumerateProperties": "immutable",
            "GetCount": {
              kind: "function",
              immutable: true,
              type: "Uint64",
              parameters: [],
              proto: true,
            },
          },
        },
        "SDL_PropertyType": {
          enum: "SDL_PROPERTY_TYPE_",
          before: "SDL_PropertiesID",
        },
        "SDL_EnumeratePropertiesCallback": {
          kind: "alias",
          type: "SDL_EnumeratePropertiesCallback",
          before: "SDL_PropertiesID",
        },
        "EnumeratePropertiesCB": {
          kind: "alias",
          type: "std::function<void(PropertiesID props, const char *name)>",
          before: "SDL_PropertiesID",
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
        "SDL_EnumerateProperties": {},
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
        },
      }
    },
    "SDL_rect.h": {
      localIncludes: ['SDL3pp_error.h', 'SDL3pp_optionalRef.h', 'SDL3pp_spanRef.h', 'SDL3pp_stdinc.h'],
      transform: {
        "FPoint": { kind: 'forward' },
        "Rect": { kind: 'forward' },
        "FRect": { kind: 'forward' },
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
            "FromCenter": [
              {
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
              {
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
              }
            ],
            "FromCorners": [
              {
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
              {
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
              }
            ],
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
              doc: "@sa operator ToFRect()",
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
              doc: "@sa Empty()",
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
            Contains: [{
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
            {
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
            }],
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
              type: "std::optional<Rect>",
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
            "FromCenter": [{
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
            {
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
            }],
            "FromCorners": [{
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
            {
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
            }],
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
              doc: "@sa Empty()",
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
            "Contains": [
              {
                kind: "function",
                type: "bool",
                parameters: [{
                  name: "p",
                  type: "const FPointRaw &"
                }],
                constexpr: true,
                immutable: true
              },
              {
                kind: "function",
                type: "bool",
                parameters: [{
                  name: "other",
                  type: "const FRectRaw &"
                }],
                constexpr: true,
                immutable: true
              }
            ],
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
          type: "bool",
          static: true,
          parameters: [
            {
              type: "SpanRef<const PointRaw>",
              name: "points"
            },
            {
              type: "OptionalRef<const RectRaw>",
              name: "clip"
            },
            {
              type: "RectRaw *",
              name: "result"
            }
          ]
        },
        "SDL_GetRectEnclosingPointsFloat": {
          type: "bool",
          static: true,
          parameters: [
            {
              type: "SpanRef<const FPointRaw>",
              name: "points"
            },
            {
              type: "OptionalRef<const FRectRaw>",
              name: "clip"
            },
            {
              type: "FRectRaw *",
              name: "result"
            }
          ]
        },
      },
    },
    "SDL_render.h": {
      localIncludes: ["SDL3pp_blendmode.h", "SDL3pp_events.h", "SDL3pp_pixels.h", "SDL3pp_video.h"],
      ignoreEntries: [
        "SDL_RenderDebugTextFormat"
      ],
      namespacesMap: {
        "SDL_PROP_RENDERER_": "prop::Renderer",
        "SDL_PROP_TEXTURE_": "prop::Texture"
      },
      transform: {
        "SDL_Renderer": {
          resource: true,
          entries: {
            "Renderer": {
              kind: "function",
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
            "SDL_GetRenderWindow": {
              "kind": "function",
              "name": "GetWindow"
            },
            "SDL_GetRendererName": "immutable",
            "GetOutputSize": {
              "kind": "function",
              "type": "Point",
              "immutable": true,
              "parameters": []
            },
            "SDL_GetRenderOutputSize": {
              "kind": "function",
              "name": "GetOutputSize",
              "immutable": true
            },
            "GetCurrentOutputSize": {
              "kind": "function",
              "type": "Point",
              "immutable": true,
              "parameters": []
            },
            "SDL_GetCurrentRenderOutputSize": {
              "kind": "function",
              "name": "GetCurrentOutputSize",
              "immutable": true
            },
            "SDL_GetRendererProperties": "immutable",
            "ResetTarget": {
              "kind": "function",
              "type": "void",
              "parameters": []
            },
            "SDL_SetRenderTarget": { name: "SetTarget" },
            "SDL_GetRenderTarget": {
              name: "GetTarget",
              immutable: true,
              proto: true,
              hints: { delegate: "GetRenderTarget" },
            },
            "SDL_SetRenderLogicalPresentation": {
              name: "SetLogicalPresentation",
              parameters: [
                {},
                {
                  type: "const PointRaw &",
                  name: "size"
                },
                {
                  name: "mode",
                  type: "RendererLogicalPresentation"
                }
              ]
            },
            "GetLogicalPresentation": {
              "kind": "function",
              "type": "void",
              "parameters": [
                {
                  "type": "PointRaw *",
                  "name": "size"
                },
                {
                  "name": "mode",
                  "type": "RendererLogicalPresentation *"
                }
              ]
            },
            "SDL_GetRenderLogicalPresentation": {
              "name": "GetLogicalPresentation",
              "immutable": true
            },
            "SDL_GetRenderLogicalPresentationRect": {
              "immutable": true,
              "name": "GetLogicalPresentationRect",
              "type": "FRect",
              "parameters": [{}]
            },
            "SDL_RenderCoordinatesFromWindow": {
              "kind": "function",
              "type": "FPoint",
              "immutable": true,
              "parameters": [
                {},
                {
                  "type": "const FPointRaw &",
                  "name": "window_coord"
                }
              ]
            },
            "SDL_RenderCoordinatesToWindow": {
              "kind": "function",
              "type": "FPoint",
              "immutable": true,
              "parameters": [
                {},
                {
                  "type": "const FPointRaw &",
                  "name": "coord"
                }
              ]
            },
            "SDL_ConvertEventToRenderCoordinates": "immutable",
            "ResetViewport": {
              "kind": "function",
              "type": "void",
              "parameters": []
            },
            "SDL_SetRenderViewport": {
              "name": "SetViewport",
              "parameters": [
                {},
                {
                  "type": "OptionalRef<const RectRaw>",
                  "name": "rect"
                }
              ]
            },
            "SDL_GetRenderViewport": {
              "name": "GetViewport",
              "immutable": true,
              "type": "Rect",
              "parameters": [{}]
            },
            "SDL_RenderViewportSet": {
              "immutable": true,
              "name": "IsViewportSet"
            },
            "SDL_GetRenderSafeArea": {
              "name": "GetSafeArea",
              "immutable": true,
              "type": "Rect",
              "parameters": [{}]
            },
            "ResetClipRect": {
              "kind": "function",
              "type": "void",
              "parameters": []
            },
            "SDL_SetRenderClipRect": {
              "name": "SetClipRect",
              "parameters": [
                {},
                {
                  "type": "OptionalRef<const RectRaw>",
                  "name": "rect"
                }
              ]
            },
            "SDL_GetRenderClipRect": {
              "name": "GetClipRect",
              "immutable": true,
              "type": "Rect",
              "parameters": [{}]
            },
            "SDL_RenderClipEnabled": {
              "immutable": true,
              "name": "IsClipEnabled"
            },
            "SDL_SetRenderScale": {
              name: "SetScale",
              parameters: [
                {},
                {
                  type: "const FPointRaw &",
                  name: "scale"
                }
              ]
            },
            "GetScale": {
              "kind": "function",
              "immutable": true,
              "type": "FPoint",
              "parameters": []
            },
            "SDL_GetRenderScale": {
              "name": "GetScale",
              "immutable": true
            },
            "SDL_SetRenderDrawColor": {
              "name": "SetDrawColor",
              "parameters": [
                {},
                {
                  "type": "ColorRaw",
                  "name": "c"
                }
              ]
            },
            "SDL_SetRenderDrawColorFloat": {
              "name": "SetDrawColorFloat",
              "parameters": [
                {},
                {
                  "type": "const FColorRaw &",
                  "name": "c"
                }
              ]
            },
            "GetDrawColor": {
              kind: "function",
              immutable: true,
              type: "Color",
              parameters: []
            },
            "GetDrawColorFloat": {
              kind: "function",
              immutable: true,
              type: "FColor",
              parameters: []
            },
            "SDL_GetRenderDrawColor": {
              "immutable": true,
              "name": "GetDrawColor"
            },
            "SDL_GetRenderDrawColorFloat": {
              "immutable": true,
              "name": "GetDrawColorFloat"
            },
            "SDL_SetRenderColorScale": {
              "name": "SetColorScale"
            },
            "SDL_GetRenderColorScale": {
              "name": "GetColorScale",
              "immutable": true,
              "type": "float",
              "parameters": [{}]
            },
            "SDL_SetRenderDrawBlendMode": {
              "name": "SetDrawBlendMode"
            },
            "SDL_GetRenderDrawBlendMode": {
              "name": "GetDrawBlendMode",
              "immutable": true,
              "type": "BlendMode",
              "parameters": [{}]
            },
            "SDL_RenderClear": "function",
            "SDL_RenderPoint": {
              "parameters": [
                {},
                {
                  "type": "const FPointRaw &",
                  "name": "p"
                }
              ]
            },
            "SDL_RenderPoints": {
              "static": false,
              "parameters": [
                {
                  "type": "SpanRef<const FPointRaw>",
                  "name": "points"
                }
              ]
            },
            "SDL_RenderLine": {
              "static": false,
              "parameters": [
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
              "static": false,
              "parameters": [
                {
                  "type": "SpanRef<const FPointRaw>",
                  "name": "points"
                }
              ]
            },
            "SDL_RenderRect": {
              "static": false,
              "parameters": [
                {
                  "type": "OptionalRef<const FRectRaw>",
                  "name": "rect"
                }
              ]
            },
            "SDL_RenderRects": {
              "static": false,
              "parameters": [
                {
                  "type": "SpanRef<const FRectRaw>",
                  "name": "rects"
                }
              ]
            },
            "SDL_RenderFillRect": {
              "static": false,
              "parameters": [
                {
                  "type": "OptionalRef<const FRectRaw>",
                  "name": "rect"
                }
              ]
            },
            "SDL_RenderFillRects": {
              "static": false,
              "parameters": [
                {
                  "type": "SpanRef<const FRectRaw>",
                  "name": "rects"
                }
              ]
            },
            "SDL_RenderTexture": {
              static: false,
              proto: true,
              parameters: [
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
              static: false,
              proto: true,
              parameters: [
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
              static: false,
              proto: true,
              parameters: [
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
              static: false,
              proto: true,
              parameters: [
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
              static: false,
              proto: true,
              parameters: [
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
              static: false,
              proto: true,
              parameters: [
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
              static: false,
              proto: true,
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
              name: "ReadPixels",
              immutable: true,
              type: "Surface",
              parameters: [
                {
                  name: "rect",
                  type: "OptionalRef<const RectRaw>",
                  default: "{}",
                }
              ]
            },
            "SDL_RenderPresent": {
              "kind": "function",
              "name": "Present"
            },
            "SDL_FlushRenderer": "function",
            "SDL_SetRenderVSync": {
              "kind": "function",
              "name": "SetVSync"
            },
            "SDL_GetRenderVSync": {
              kind: "function",
              name: "GetVSync",
              type: "int",
              immutable: true,
              parameters: []
            },
            "SDL_RenderDebugText": {
              parameters: [
                {
                },
                {
                  "type": "FPoint",
                  "name": "p"
                },
                {
                  "type": "StringParam",
                  "name": "str"
                }
              ]
            },
            "RenderDebugTextFormat": {
              kind: "function",
              type: "void",
              template: [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              parameters: [
                {
                  "type": "FPoint",
                  "name": "p"
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
          }
        },
        "SDL_Texture": {
          resource: {
            shared: "refcount",
          },
          entries: {
            "SDL_CreateTexture": "ctor",
            "SDL_CreateTextureFromSurface": "ctor",
            "SDL_CreateTextureWithProperties": "ctor",
            "SDL_GetTextureProperties": "immutable",
            "SDL_GetRendererFromTexture": {
              "name": "GetRenderer",
              "immutable": true
            },
            "SetMod": {
              kind: "function",
              type: "void",
              parameters: [
                {
                  "type": "Color",
                  "name": "c"
                }
              ]
            },
            "SetModFloat": {
              kind: "function",
              type: "void",
              parameters: [
                {
                  "type": "FColor",
                  "name": "c"
                }
              ]
            },
            "GetMod": {
              kind: "function",
              type: "Color",
              immutable: true,
              parameters: []
            },
            "GetModFloat": {
              kind: "function",
              type: "FColor",
              immutable: true,
              parameters: []
            },
            "SDL_SetTextureColorMod": "function",
            "SDL_SetTextureColorModFloat": "function",
            "SDL_GetTextureColorMod": "immutable",
            "SDL_GetTextureColorModFloat": "immutable",
            "SDL_SetTextureAlphaMod": "function",
            "SDL_SetTextureAlphaModFloat": "function",
            "SDL_GetTextureAlphaMod": {
              immutable: true,
              type: "Uint8",
              parameters: [{}],
            },
            "SDL_GetTextureAlphaModFloat": {
              immutable: true,
              type: "float",
              parameters: [{}],
            },
            "SDL_SetTextureBlendMode": "function",
            "SDL_GetTextureBlendMode": {
              "kind": "function",
              "immutable": true,
              "type": "BlendMode",
              "parameters": [{}]
            },
            "SDL_SetTextureScaleMode": "function",
            "SDL_GetTextureScaleMode": {
              "kind": "function",
              "immutable": true,
              "type": "ScaleMode",
              "parameters": [{}]
            },
            "SDL_UpdateTexture": {
              "name": "Update",
              "parameters": [
                {},
                {
                  "type": "OptionalRef<const RectRaw>"
                },
                {},
                {}
              ]
            },
            "SDL_UpdateYUVTexture": {
              "name": "UpdateYUV",
              "parameters": [
                {},
                {
                  "name": "rect",
                  "type": "OptionalRef<const RectRaw>"
                },
                {
                  "name": "Yplane",
                  "type": "const Uint8 *"
                },
                {
                  "name": "Ypitch",
                  "type": "int"
                },
                {
                  "name": "Uplane",
                  "type": "const Uint8 *"
                },
                {
                  "name": "Upitch",
                  "type": "int"
                },
                {
                  "name": "Vplane",
                  "type": "const Uint8 *"
                },
                {
                  "name": "Vpitch",
                  "type": "int"
                }
              ]
            },
            "SDL_UpdateNVTexture": {
              "name": "UpdateNV",
              "parameters": [
                {},
                {
                  "type": "OptionalRef<const RectRaw>"
                },
                {
                  "name": "Yplane",
                  "type": "const Uint8 *"
                },
                {
                  "name": "Ypitch",
                  "type": "int"
                },
                {
                  "name": "UVplane",
                  "type": "const Uint8 *"
                },
                {
                  "name": "UVpitch",
                  "type": "int"
                }
              ]
            },
            "SDL_LockTexture": {
              parameters: [{}, { type: "OptionalRef<const SDL_Rect>" }, {}, {}]
            },
            "SDL_LockTextureToSurface": {
              parameters: [{}, { type: "OptionalRef<const SDL_Rect>", default: "std::nullopt" }],
              type: "Surface",
            },
            "SDL_UnlockTexture": "function",
            "GetWidth": {
              "kind": "function",
              "immutable": true,
              "type": "int",
              "parameters": []
            },
            "GetHeight": {
              "kind": "function",
              "immutable": true,
              "type": "int",
              "parameters": []
            },
            "GetSize": {
              "kind": "function",
              "immutable": true,
              "type": "Point",
              "parameters": []
            },
            "GetSizeFloat": {
              "kind": "function",
              "immutable": true,
              "type": "FPoint",
              "parameters": []
            },
            "SDL_GetTextureSize": "immutable",
            "GetFormat": {
              "kind": "function",
              "immutable": true,
              "type": "PixelFormat",
              "parameters": []
            },
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
      }
    },
    "SDL_scancode.h": {
      includeAfter: {
        "__begin": {
          name: "Keycode",
          kind: "forward"
        }
      },
      wrappers: {
        "SDL_Scancode": {
          ordered: true
        }
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
        "true"
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
            // type: "SDL_iconv_data_t",
            free: "SDL_iconv_close",
            ctors: ["SDL_iconv_open"],
          },
          entries: {
            "SDL_iconv_open": { name: "open" },
            "SDL_iconv": "function",
            "SDL_iconv_close": {
              name: "close",
              static: false,
              parameters: [],
            },
          }
        },
        "CompareCallback_rCB": { name: "CompareCB" },
        "Seconds": {
          before: "SDL_Time",
          kind: "alias",
          type: "std::chrono::duration<float>",
          doc: "Duration in seconds (float)."
        },
        "Nanoseconds": {
          before: "SDL_Time",
          kind: "alias",
          name: "Nanoseconds",
          type: "std::chrono::nanoseconds",
          doc: "Duration in Nanoseconds (Uint64)."
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
          doc: "Converts a time duration to seconds (float)."
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
          doc: "Converts a float to seconds representation."
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
          doc: "Converts a time duration to seconds (float)."
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
          doc: "Converts a float to seconds representation."
        },
        "SDL_Time": {
          kind: "struct",
          type: "",
          entries: {
            "m_value": {
              "kind": "var",
              "type": "std::chrono::nanoseconds"
            },
            "Time": [
              {
                "kind": "function",
                "constexpr": true,
                "type": "",
                "parameters": []
              },
              {
                "kind": "function",
                "type": "",
                "constexpr": true,
                "parameters": [
                  {
                    "type": "std::chrono::nanoseconds",
                    "name": "time"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "explicit": true,
                "constexpr": true,
                "parameters": [
                  {
                    "type": "SDL_Time",
                    "name": "time"
                  }
                ]
              }
            ],
            "operator bool": {
              "kind": "function",
              "type": "",
              "explicit": true,
              "immutable": true,
              "constexpr": true,
              "parameters": []
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
              doc: "Converts a time to seconds (float) since epoch."
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
              doc: "Converts a time to seconds (float) since epoch."
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
        "SDL_iconv_string": {
          "type": "OwnPtr<char>"
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
            "Random": [{
              kind: "function",
              constexpr: true,
              type: "",
              parameters: []
            }, {
              kind: "function",
              constexpr: true,
              explicit: true,
              type: "",
              parameters: [{
                type: "Uint64",
                name: "state"
              }]
            }],
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
        "SDL_Surface": {
          resource: {
            shared: 'refcount',
            ctors: ["SDL_LoadBMP_IO", "SDL_LoadBMP"],
            enableConstParam: true,
          },
          entries: {
            "SDL_CreateSurface": {
              name: "ctor",
              parameters: [
                {
                  name: "size",
                  type: "const PointRaw &"
                },
                {
                  name: "format",
                  type: "PixelFormat"
                }
              ]
            },
            "SDL_CreateSurfaceFrom": {
              name: "ctor",
              parameters: [
                {
                  name: "size",
                  type: "const PointRaw &"
                },
                {
                  name: "format",
                  type: "PixelFormat"
                },
                {
                  name: "pixels",
                  type: "void *"
                },
                {
                  name: "pitch",
                  type: "int"
                }
              ]
            },
            "SDL_GetSurfaceProperties": "immutable",
            "SDL_SetSurfaceColorspace": "function",
            "SDL_GetSurfaceColorspace": "immutable",
            "SDL_CreateSurfacePalette": { type: "Palette" },
            "SDL_SetSurfacePalette": "function",
            "SDL_GetSurfacePalette": { immutable: true, type: "Palette" },
            "SDL_AddSurfaceAlternateImage": "function",
            "SDL_SurfaceHasAlternateImages": "immutable",
            "SDL_GetSurfaceImages": {
              kind: "function",
              immutable: true,
              type: "OwnArray<SurfaceRaw>",
              parameters: []
            },
            "SDL_RemoveSurfaceAlternateImages": "function",
            "SDL_MUSTLOCK": {
              kind: "function",
              name: "MustLock",
              constexpr: true,
              immutable: true,
              type: "bool",
              parameters: []
            },
            "SDL_LockSurface": "function",
            "SDL_UnlockSurface": "function",
            "SDL_LoadBMP_IO": {
              name: "LoadBMP",
              type: "Surface",
              static: true,
              parameters: [{}, { default: "false" }]
            },
            "SDL_LoadBMP": {
              type: "Surface",
              static: true,
            },
            "SDL_SaveBMP_IO": {
              name: "SaveBMP",
              immutable: true,
              parameters: [{}, {}, { default: "false" }]
            },
            "SDL_SaveBMP": "immutable",
            "SDL_SetSurfaceRLE": "function",
            "SDL_SurfaceHasRLE": "immutable",
            "SDL_SetSurfaceColorKey": {
              kind: "function",
              parameters: [
                {},
                {
                  type: "std::optional<Uint32>",
                  name: "key"
                }
              ]
            },
            "ClearColorKey": {
              kind: "function",
              type: "void",
              parameters: []
            },
            "SDL_SurfaceHasColorKey": "immutable",
            "SDL_GetSurfaceColorKey": {
              immutable: true,
              type: "std::optional<Uint32>",
              parameters: [{}],
            },
            "SDL_SetSurfaceColorMod": "function",
            "SDL_GetSurfaceColorMod": "immutable",
            "SDL_SetSurfaceAlphaMod": "function",
            "SDL_GetSurfaceAlphaMod": {
              kind: "function",
              immutable: true,
              type: "Uint8",
              parameters: [{}]
            },
            "SetMod": {
              kind: "function",
              type: "void",
              parameters: [{
                type: "Color",
                name: "color"
              }]
            },
            "GetMod": {
              kind: "function",
              immutable: true,
              type: "Color",
              parameters: []
            },
            "SDL_SetSurfaceBlendMode": "function",
            "SDL_GetSurfaceBlendMode": {
              kind: "function",
              immutable: true,
              type: "BlendMode",
              parameters: [{}]
            },
            "SDL_SetSurfaceClipRect": {
              kind: "function",
              static: false,
              parameters: [
                {},
                { type: "OptionalRef<const RectRaw>" },
              ]
            },
            "ResetClipRect": {
              kind: "function",
              type: "void",
              parameters: []
            },
            "SDL_GetSurfaceClipRect": {
              kind: "function",
              immutable: true,
              type: "Rect",
              parameters: [{}]
            },
            "SDL_FlipSurface": "function",
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
              name: "Convert",
              immutable: true
            },
            "SDL_PremultiplySurfaceAlpha": "function",
            "SDL_ClearSurface": {
              parameters: [
                {},
                {
                  type: "const FColorRaw &",
                  name: "color"
                }
              ]
            },
            "Fill": {
              kind: "function",
              type: "void",
              parameters: [
                {
                  type: "Uint32",
                  name: "color"
                }
              ]
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
            "SDL_FillSurfaceRects": {
              kind: "function",
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
            "SDL_BlitSurface": {
              kind: "function",
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
            "BlitAt": {
              kind: "function",
              type: "void",
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
            "SDL_BlitSurfaceUnchecked": {
              kind: "function",
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
            "SDL_BlitSurfaceScaled": {
              kind: "function",
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
            "SDL_BlitSurfaceUncheckedScaled": {
              kind: "function",
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
            "SDL_StretchSurface": {
              kind: "function",
              since: {
                tag: "SDL",
                major: 3,
                minor: 2,
                patch: 4
              },
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
            "SDL_BlitSurfaceTiled": {
              kind: "function",
              name: "BlitTiled",
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
            "SDL_BlitSurfaceTiledWithScale": {
              kind: "function",
              name: "BlitTiledWithScale",
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
            "Blit9Grid": {
              kind: "function",
              name: "Blit9Grid",
              type: "void",
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
                }
              ]
            },
            "SDL_BlitSurface9Grid": {
              kind: "function",
              name: "Blit9GridWithScale",
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
            "SDL_MapSurfaceRGB": {
              kind: "function",
              immutable: true
            },
            "SDL_MapSurfaceRGBA": {
              kind: "function",
              immutable: true
            },
            "ReadPixel": {
              kind: "function",
              name: "ReadPixel",
              immutable: true,
              type: "Color",
              parameters: [
                {
                  type: "const PointRaw &",
                  name: "p"
                },
              ]
            },
            "ReadPixelFloat": {
              kind: "function",
              immutable: true,
              type: "FColor",
              parameters: [
                {
                  type: "const PointRaw &",
                  name: "p"
                },
              ]
            },
            "SDL_ReadSurfacePixel": {
              immutable: true,
              parameters: [
                {},
                {
                  type: "const PointRaw &",
                  name: "p"
                },
                {
                  type: "Uint8 *",
                  name: "r"
                },
                {
                  type: "Uint8 *",
                  name: "g"
                },
                {
                  type: "Uint8 *",
                  name: "b"
                },
                {
                  type: "Uint8 *",
                  name: "a"
                }
              ]
            },
            "SDL_ReadSurfacePixelFloat": {
              immutable: true,
              parameters: [
                {},
                {
                  type: "const PointRaw &",
                  name: "p"
                },
                {
                  type: "float *",
                  name: "r"
                },
                {
                  type: "float *",
                  name: "g"
                },
                {
                  type: "float *",
                  name: "b"
                },
                {
                  type: "float *",
                  name: "a"
                }
              ]
            },
            "SDL_WriteSurfacePixel": {
              kind: "function",
              parameters: [
                {},
                {
                  type: "const PointRaw &",
                  name: "p"
                },
                {
                  type: "ColorRaw",
                  name: "c"
                }
              ]
            },
            "SDL_WriteSurfacePixelFloat": {
              kind: "function",
              parameters: [
                {},
                {
                  type: "const PointRaw &",
                  name: "p"
                },
                {
                  type: "const FColorRaw &",
                  name: "c"
                }
              ]
            },
            "GetWidth": {
              kind: "function",
              immutable: true,
              type: "int",
              parameters: []
            },
            "GetHeight": {
              kind: "function",
              immutable: true,
              type: "int",
              parameters: []
            },
            "GetSize": {
              kind: "function",
              immutable: true,
              type: "Point",
              parameters: []
            },
            "GetPitch": {
              kind: "function",
              type: "int",
              immutable: true,
              parameters: []
            },
            "GetFormat": {
              kind: "function",
              immutable: true,
              type: "PixelFormat",
              parameters: []
            },
            "GetPixels": {
              kind: "function",
              immutable: true,
              type: "void *",
              parameters: []
            }
          }
        },
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
        "SDL_BlitSurface": {
          kind: "function",
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
        "SDL_BlitSurfaceUnchecked": {
          kind: "function",
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
        "SDL_BlitSurfaceScaled": {
          kind: "function",
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
        "SDL_BlitSurfaceUncheckedScaled": {
          kind: "function",
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
        "SDL_StretchSurface": {
          kind: "function",
          since: {
            tag: "SDL",
            major: 3,
            minor: 2,
            patch: 4
          },
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
        "SDL_BlitSurfaceTiled": {
          kind: "function",
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
        "SDL_BlitSurfaceTiledWithScale": {
          kind: "function",
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
        "SDL_BlitSurface9Grid": {
          kind: "function",
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
        "BlitSurface9Grid": {
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
          }]
        },
        "SDL_LoadBMP_IO": {
          name: "LoadBMP",
          parameters: [{}, { default: "false" }]
        },
        "SDL_SaveBMP_IO": {
          name: "SaveBMP",
          parameters: [{ type: "SurfaceConstParam" }, {}, { default: "false" }],
        },
      }
    },
    "SDL_system.h": {
      includeAfter: {
        "SDL_SetWindowsMessageHook": {
          name: "SetWindowsMessageHook",
          kind: "function",
          type: "void",
          parameters: [
            {
              name: "callback",
              type: "WindowsMessageHookCB"
            }
          ],
        },
        "SDL_SetX11EventHook": {
          name: "SetX11EventHook",
          kind: "function",
          type: "void",
          parameters: [
            {
              name: "callback",
              type: "X11EventHookCB"
            }
          ],
        },
        "SDL_SetiOSAnimationCallback": {
          name: "SetiOSAnimationCallback",
          kind: "function",
          type: "void",
          parameters: [
            {
              name: "window",
              type: "WindowParam"
            },
            {
              name: "interval",
              type: "int"
            },
            {
              name: "callback",
              type: "iOSAnimationCB"
            }
          ],
        },
        "SDL_RequestAndroidPermission": {
          name: "RequestAndroidPermission",
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
      },
      transform: {
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
        "WindowsMessageHookCB": { type: "std::function<bool(MSG *msg)>" },
        "iOSAnimationCB": { type: "std::function<void()>" },
        "RequestAndroidPermissionCB": { type: "std::function<void(const char *permission, bool granted)>" },
      }
    },
    "SDL_thread.h": {
      includeAfter: {
        "__begin": [
          { name: "ThreadID" },
          { name: "TLSID" },
          { name: "ThreadFunction" },
          {
            name: "ThreadCB",
            kind: "alias",
            type: "std::function<int()>"
          },
          { name: "TLSDestructorCallback" },
        ]
      },
      namespacesMap: {
        "SDL_PROP_THREAD_": "prop::thread"
      },
      enumerations: {
        "SDL_ThreadPriority": {
          prefix: "SDL_THREAD_PRIORITY_",
          includeAfter: "__begin"
        },
        "SDL_ThreadState": {
          prefix: "SDL_THREAD_",
          includeAfter: "__begin"
        }
      },
      resources: {
        "SDL_Thread": {
          free: "SDL_DetachThread",
          entries: {
            "Create": {
              kind: "function",
              name: "ctor",
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
        }
      },
      transform: {
        "SDL_TLSID": {
          type: "AtomicInt"
        }
      }
    },
    "SDL_time.h": {
      localIncludes: ["SDL3pp_stdinc.h"],
      transform: {
        "SDL_DateTime": {
          wrapper: true,
          // "ordered": true,
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
          kind: "alias"
        },
        "TimerCB": {
          type: "std::function<std::chrono::nanoseconds(TimerID, std::chrono::nanoseconds)>"
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
    "SDL_tray.h": {
      enumerations: {
        "SDL_TrayEntryFlags": {
          prefix: "SDL_TRAYENTRY_",
          includeAfter: "__begin",
        }
      },
      includeAfter: {
        "__begin": {
          name: "TrayMenu",
          kind: "forward"
        },
        "SDL_Tray": [{
          name: "TrayCallback"
        }, {
          name: "TrayCB",
        }],
      },
      resources: {
        "SDL_Tray": {
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
          free: "SDL_RemoveTrayEntry",
          aliasDetached: true,
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
      },
      wrappers: {
        "SDL_TrayMenu": {
          entries: {
            "SDL_GetTrayEntries": {
              name: "GetEntries",
              type: "std::span<TrayEntry>",
              proto: true,
              parameters: [{}],
            },
            "SDL_InsertTrayEntryAt": {
              name: "InsertEntry",
              type: "DetachedTrayEntry",
              proto: true,
            },
            "AppendEntry": {
              kind: "function",
              type: "DetachedTrayEntry",
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
      },
      transform: {
        "TrayCB": { type: "std::function<void(TrayEntryRef)>" },
      }
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
        "SDL_DisplayOrientation": { before: "SDL_DisplayID" },
        "SDL_DisplayMode": { before: "SDL_DisplayID" },
        "SDL_DisplayID": {
          name: "Display",
          wrapper: {
            attribute: "displayID",
            rawName: "DisplayID",
          },
          entries: {
            "SDL_GetDisplays": {
              kind: "function",
              name: "GetAll",
              type: "OwnArray<DisplayID>",
              parameters: [],
              static: true,
            },
            "SDL_GetPrimaryDisplay": {
              kind: "function",
              name: "GetPrimary",
              static: true,
            },
            "SDL_GetDisplayProperties": "immutable",
            "SDL_GetDisplayName": "immutable",
            "SDL_GetDisplayBounds": {
              "kind": "function",
              "type": "Rect",
              "parameters": [],
              "immutable": true
            },
            "SDL_GetDisplayUsableBounds": {
              "kind": "function",
              "type": "Rect",
              "parameters": [],
              "immutable": true
            },
            "SDL_GetNaturalDisplayOrientation": {
              "kind": "function",
              "immutable": true,
              "name": "GetNaturalOrientation"
            },
            "SDL_GetCurrentDisplayOrientation": {
              "kind": "function",
              "immutable": true,
              "name": "GetCurrentOrientation"
            },
            "SDL_GetDisplayContentScale": "immutable",
            "SDL_GetFullscreenDisplayModes": {
              "kind": "function",
              "name": "GetFullscreenModes",
              "immutable": true,
              "type": "OwnArray<DisplayMode *>",
              "parameters": []
            },
            "SDL_GetClosestFullscreenDisplayMode": {
              "kind": "function",
              "immutable": true,
              "type": "DisplayMode",
              "parameters": [
                {},
                {},
                {},
                {}
              ]
            },
            "SDL_GetDesktopDisplayMode": {
              "kind": "function",
              "name": "GetDesktopMode",
              "immutable": true
            },
            "SDL_GetCurrentDisplayMode": {
              "kind": "function",
              "name": "GetCurrentMode",
              "immutable": true
            },
            "SDL_GetDisplayForPoint": "function",
            "SDL_GetDisplayForRect": "function"
          }
        },
        "SDL_WindowFlags": {
          before: "SDL_Window",
          enum: "SDL_WINDOW_"
        },
        "SDL_FlashOperation": { before: "SDL_Window" },
        "SDL_HitTestResult": { before: "SDL_Window" },
        "SDL_HitTest": { before: "SDL_Window" },
        "HitTestCB": {
          before: "SDL_Window",
          kind: "alias",
          type: "HitTest",
          doc: "@sa HitTest"
        },
        "SDL_Window": {
          resource: {
            free: "SDL_DestroyWindow",
            enableAutoMethods: false,
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
            "SDL_GetDisplayForWindow": {
              "kind": "function",
              "name": "GetDisplay",
              "immutable": true
            },
            "SDL_GetWindowPixelDensity": "immutable",
            "SDL_GetWindowDisplayScale": "immutable",
            "SDL_SetWindowFullscreenMode": {
              "kind": "function",
              "parameters": [
                {},
                {
                  "type": "OptionalRef<const DisplayMode>"
                }
              ]
            },
            "SDL_GetWindowFullscreenMode": "immutable",
            "SDL_GetWindowICCProfile": {
              kind: "function",
              immutable: true,
              type: "OwnPtr<void>",
              parameters: [{}]
            },
            "SDL_GetWindowPixelFormat": "immutable",
            "SDL_GetWindowID": "immutable",
            "SDL_GetWindowParent": "immutable",
            "SDL_GetWindowProperties": "immutable",
            "SDL_GetWindowFlags": "immutable",
            "SDL_SetWindowTitle": "function",
            "SDL_GetWindowTitle": "immutable",
            "SDL_SetWindowIcon": "function",
            "SetRect": {
              "kind": "function",
              "type": "void",
              "parameters": [
                {
                  "type": "Rect",
                  "name": "rect"
                }
              ]
            },
            "GetRect": {
              "kind": "function",
              "type": "Rect",
              "immutable": true,
              "parameters": []
            },
            "SDL_SetWindowPosition": {
              "static": false,
              "parameters": [
                {
                  "type": "const PointRaw &",
                  "name": "p"
                }
              ]
            },
            "GetPosition": {
              "kind": "function",
              "type": "Point",
              "immutable": true,
              "parameters": []
            },
            "SDL_GetWindowPosition": "immutable",
            "SDL_SetWindowSize": {
              "static": false,
              "parameters": [
                {
                  "type": "const PointRaw &",
                  "name": "p"
                }
              ]
            },
            "GetSize": {
              "kind": "function",
              "type": "Point",
              "immutable": true,
              "parameters": []
            },
            "SDL_GetWindowSize": "immutable",
            "SDL_GetWindowSafeArea": {
              "kind": "function",
              "type": "Rect",
              "immutable": true,
              "parameters": []
            },
            "SDL_SetWindowAspectRatio": "function",
            "SDL_GetWindowAspectRatio": "immutable",
            "SDL_GetWindowBordersSize": "immutable",
            "GetSizeInPixels": {
              "kind": "function",
              "type": "Point",
              "immutable": true,
              "parameters": []
            },
            "SDL_GetWindowSizeInPixels": "immutable",
            "SDL_SetWindowMinimumSize": {
              "static": false,
              "parameters": [
                {
                  "type": "const PointRaw &",
                  "name": "p"
                }
              ]
            },
            "SDL_GetWindowMinimumSize": "immutable",
            "SDL_SetWindowMaximumSize": {
              "static": false,
              "parameters": [
                {
                  "type": "const PointRaw &",
                  "name": "p"
                }
              ]
            },
            "SDL_GetWindowMaximumSize": "immutable",
            "SDL_SetWindowBordered": "function",
            "SDL_SetWindowResizable": "function",
            "SDL_SetWindowAlwaysOnTop": "function",
            "SDL_ShowWindow": "function",
            "SDL_HideWindow": "function",
            "SDL_RaiseWindow": "function",
            "SDL_MaximizeWindow": "function",
            "SDL_MinimizeWindow": "function",
            "SDL_RestoreWindow": "function",
            "SDL_SetWindowFullscreen": "function",
            "SDL_SyncWindow": "function",
            "SDL_WindowHasSurface": "immutable",
            "SDL_GetWindowSurface": "function",
            "SDL_SetWindowSurfaceVSync": "function",
            "SDL_GetWindowSurfaceVSync": {
              "immutable": true,
              "type": "int",
              "parameters": []
            },
            "SDL_UpdateWindowSurface": {
              "kind": "function",
              "name": "UpdateSurface"
            },
            "SDL_UpdateWindowSurfaceRects": {
              "kind": "function",
              "name": "UpdateSurfaceRects",
              "parameters": [
                {},
                {
                  "type": "SpanRef<const RectRaw>"
                }
              ]
            },
            "SDL_DestroyWindowSurface": "function",
            "SDL_SetWindowKeyboardGrab": "function",
            "SDL_SetWindowMouseGrab": "function",
            "SDL_GetWindowKeyboardGrab": "immutable",
            "SDL_GetWindowMouseGrab": "immutable",
            "SDL_SetWindowMouseRect": "function",
            "SDL_GetWindowMouseRect": {
              immutable: true,
              type: "const RectRaw *"
            },
            "SDL_SetWindowOpacity": "function",
            "SDL_GetWindowOpacity": "immutable",
            "SDL_SetWindowParent": {
              parameters: [
                {},
                { type: "WindowParam" }
              ]
            },
            "SDL_SetWindowModal": "function",
            "SDL_SetWindowFocusable": "function",
            "SDL_ShowWindowSystemMenu": {
              "static": false,
              "parameters": [
                {
                  "type": "const PointRaw &",
                  "name": "p"
                }
              ]
            },
            "SetHitTest": {
              "kind": "function",
              "type": "void",
              "parameters": [
                {
                  "name": "callback",
                  "type": "HitTestCB"
                }
              ]
            },
            "SDL_SetWindowHitTest": "function",
            "SDL_SetWindowShape": "function",
            "SDL_FlashWindow": "function",
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
              "name": "MakeCurrent",
              "static": false,
              "parameters": [
                {
                  "type": "WindowParam",
                  "name": "window"
                }
              ]
            },
            "SDL_GL_DestroyContext": {
              name: "Destroy",
              hints: { mayFail: true },
            },
          }
        },
        "SDL_GL_MakeCurrent": {
          parameters: [{}, {}],
        },
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
        "IMG_Animation": {
          resource: { free: "IMG_FreeAnimation" },
          entries: {
            "IMG_LoadAnimation": "ctor",
            "IMG_LoadAnimation_IO": { name: "ctor" },
            "GetWidth": {
              kind: "function",
              immutable: true,
              type: "int",
              parameters: []
            },
            "GetHeight": {
              kind: "function",
              immutable: true,
              type: "int",
              parameters: []
            },
            "GetSize": {
              kind: "function",
              immutable: true,
              type: "Point",
              parameters: []
            },
            "GetCount": {
              kind: "function",
              immutable: true,
              type: "int",
              parameters: []
            },
            "GetFrame": {
              kind: "function",
              type: "Surface",
              immutable: true,
              parameters: [
                {
                  "type": "int",
                  "name": "index"
                }
              ]
            },
            "GetDelay": {
              kind: "function",
              type: "int",
              immutable: true,
              parameters: [
                {
                  "type": "int",
                  "name": "index"
                }
              ]
            }
          }
        },
        "IMG_LoadAnimation": { type: "Animation" },
        "IMG_LoadAnimation_IO": {
          type: "Animation",
          name: "LoadAnimation"
        },
        "IMG_LoadAnimationTyped_IO": {
          type: "Animation",
          name: "LoadAnimationTyped"
        },
        "IMG_LoadTyped_IO": {
          type: "Surface",
          name: "LoadSurfaceTyped",
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
          "name": "SaveAVIF",
        },
        "IMG_SavePNG_IO": {
          "name": "SavePNG",
        },
        "IMG_SaveJPG_IO": {
          "name": "SaveJPG",
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
      resources: {
        "TTF_Font": {
          ctors: ["TTF_OpenFontIO"],
          entries: {
            "TTF_OpenFont": "ctor",
            "TTF_OpenFontIO": {
              name: "Open",
              parameters: [{}, {
                type: "float",
                name: "ptsize"
              }]
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
            "TTF_GetFontWeight": "immutable",
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
        "TTF_TextEngine": {
          returnType: "unique",
          extraParameters: ["TextEngineDeleter"],
          entries: {
            "TTF_CreateSurfaceTextEngine": "ctor",
            "TTF_CreateRendererTextEngine": "ctor",
            "TTF_CreateRendererTextEngineWithProperties": "ctor",
            "TTF_CreateGPUTextEngine": "ctor",
            "TTF_CreateGPUTextEngineWithProperties": "ctor",
            "TTF_SetGPUTextEngineWinding": {
              "name": "SetGPUWinding"
            },
            "TTF_GetGPUTextEngineWinding": {
              "immutable": true,
              "name": "GetGPUWinding"
            },
            "TTF_DestroySurfaceTextEngine": {
              static: true,
              parameters: [{
                name: "engine",
                type: "TTF_TextEngine *"
              }],
            },
            "TTF_DestroyRendererTextEngine": {
              static: true,
              parameters: [{
                name: "engine",
                type: "TTF_TextEngine *"
              }],
            },
            "TTF_DestroyGPUTextEngine": {
              static: true,
              parameters: [{
                name: "engine",
                type: "TTF_TextEngine *"
              }],
            },
          }
        },
        "TTF_Text": {
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
            "GetColor": [
              {
                "kind": "function",
                "type": "FColor",
                "immutable": true,
                "parameters": []
              },
              {
                "kind": "function",
                "type": "void",
                "immutable": true,
                "parameters": [
                  {
                    "type": "Color *",
                    "name": "c"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "void",
                "immutable": true,
                "parameters": [
                  {
                    "type": "FColor *",
                    "name": "c"
                  }
                ]
              }
            ],
            "TTF_GetTextColor": "immutable",
            "TTF_GetTextColorFloat": "immutable",
            "TTF_SetTextPosition": {
              "static": false,
              "parameters": [
                {
                  "type": "Point",
                  "name": "p"
                }
              ]
            },
            "GetPosition": {
              "kind": "function",
              "immutable": true,
              "type": "Point",
              "parameters": []
            },
            "TTF_GetTextPosition": "immutable",
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
            "GetSize": {
              "kind": "function",
              "type": "Point",
              "immutable": true,
              "parameters": []
            },
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
              "kind": "function",
              "type": "const char *",
              "immutable": true,
              "doc": "A copy of the UTF-8 string that this text object represents, useful for layout, debugging and retrieving substring text",
              "parameters": []
            },
            "GetNumLines": {
              "kind": "function",
              "type": "int",
              "immutable": true,
              "doc": "The number of lines in the text, 0 if it's empty",
              "parameters": []
            }
          }
        },
      },
      enumerations: {
        "TTF_FontStyleFlags": {
          "prefix": "TTF_STYLE_",
          "includeAfter": "__begin"
        },
        "TTF_HintingFlags": {
          "prefix": "TTF_HINTING_",
          "includeAfter": "__begin"
        },
        "TTF_HorizontalAlignment": {
          "prefix": "TTF_HORIZONTAL_ALIGN_",
          "includeAfter": "__begin"
        },
        "TTF_Direction": {
          "prefix": "TTF_DIRECTION_",
          "includeAfter": "__begin"
        },
        "TTF_ImageType": {
          "prefix": "TTF_IMAGE_",
          "includeAfter": "__begin"
        },
        "TTF_SubStringFlags": {
          "prefix": "TTF_SUBSTRING_",
          "includeAfter": "__begin"
        },
        "TTF_GPUTextEngineWinding": {
          "prefix": "TTF_GPU_TEXTENGINE_WINDING_",
          "includeAfter": "__begin"
        }
      },
      namespacesMap: {
        "TTF_PROP_FONT_": "prop::Font",
        "TTF_PROP_RENDERER_TEXT_ENGINE_": "prop::RendererTextEngine",
        "TTF_PROP_GPU_TEXT_ENGINE_": "prop::GpuTextEngine"
      },
      includeAfter: {
        "__begin": [
          {
            "name": "SDL3PP_ENABLE_TTF",
            "kind": "def"
          },
          {
            "kind": "struct",
            "name": "TtfInitFlag",
            "type": "InitFlagsExtra"
          },
          {
            kind: "alias",
            name: "TextEngineDeleter",
            type: "void (*)(TTF_TextEngine *)",
          }
        ],
        "TTF_TextEngine": [
          {
            "name": "GPUAtlasDrawSequence",
          },
          {
            "name": "SubString",
          },
          {
            "name": "SubStringIterator",
            "kind": "forward"
          }
        ],
        "TTF_Text": {
          "name": "SubStringIterator",
          "kind": "struct",
          "entries": {
            "m_text": {
              "kind": "var",
              "type": "TextParam"
            },
            "m_subString": {
              "kind": "var",
              "type": "SubString"
            },
            "SubStringIterator": [
              {
                "kind": "function",
                "type": "",
                "constexpr": true,
                "parameters": [
                  {
                    "name": "text",
                    "type": "TextParam"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "constexpr": true,
                "parameters": []
              }
            ],
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
            "operator++": [
              {
                "kind": "function",
                "type": "SubStringIterator &",
                "constexpr": true,
                "parameters": []
              },
              {
                "kind": "function",
                "type": "SubStringIterator",
                "constexpr": true,
                "parameters": [
                  "int"
                ]
              }
            ],
            "operator--": [
              {
                "kind": "function",
                "type": "SubStringIterator &",
                "constexpr": true,
                "parameters": []
              },
              {
                "kind": "function",
                "type": "SubStringIterator",
                "constexpr": true,
                "parameters": [
                  "int"
                ]
              }
            ]
          }
        },
        "TTF_Init": {
          "name": "GPUTextEngineWinding",
        }
      },
      transform: {
        "TTF_Version": {
          "name": "TTF_Version"
        },
        "TTF_Init": {
          "name": "InitSubSystem",
          "parameters": [
            {
              "type": "TtfInitFlag",
              "name": "_"
            }
          ]
        },
        "TTF_Quit": {
          "name": "QuitSubSystem",
          "parameters": [
            {
              "type": "TtfInitFlag",
              "name": "_"
            }
          ]
        },
        "TTF_WasInit": {
          "name": "WasInit",
          "parameters": [
            {
              "type": "TtfInitFlag",
              "name": "_"
            }
          ]
        }
      }
    }
  }
};

if (process.argv[2]) {
  writeJSONSync(process.argv[2], transform);
} else {
  writeJSONSync(1, transform);
}
