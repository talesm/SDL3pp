const { writeJSONSync } = require("./cppfier/src/utils");

/**
 * @import {ApiTransform} from "./cppfier/src/types"
 */

/** @type {ApiTransform} */
const transform = {
  prefixes: ["SDL_", "IMG_", "TTF_"],
  definitionPrefix: "SDL_",
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
    "SDL_IOStream *": "ObjectBox<SDL_IOStream> auto &&",
    "const SDL_Point *": "const SDL_Point &",
    "const SDL_FPoint *": "const SDL_FPoint &",
    "const SDL_Rect *": "const SDL_Rect &",
    "const SDL_FRect *": "const SDL_FRect &",
    "TTF_TextEngine *": "TextEngineRef",
  },
  returnTypeMap: {
    "TTF_TextEngine *": "TextEngine"
  },
  files: {
    "SDL_assert.h": {
      ignoreEntries: [
        "__debugbreak"
      ],
      transform: {
        "SDL_ReportAssertion": {
          "parameters": [
            {},
            {},
            {},
            {
              "type": "int",
              "name": "line"
            }
          ]
        }
      }
    },
    // TODO: Remove??
    "SDL_blendmode.h": {
      enumerations: {
        "SDL_BlendMode": {}
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
        "SDL_DialogFileCallback": {
          "name": "DialogFileCB",
          "kind": "alias",
          "type": "std::function<void(const char *const *, int)>"
        },
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
              "type": "WindowRef"
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
              "type": "WindowRef"
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
              "type": "WindowRef"
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
              "type": "PropertiesRef"
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
              "type": "WindowRef"
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
              "type": "WindowRef"
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
        }
      }
    },
    "SDL_error.h": {
      ignoreEntries: [
        "SDL_SetErrorV"
      ],
      includeAfter: {
        "SDL_SetError": {
          "kind": "function",
          "name": "SetError",
          "type": "bool",
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
              "type": "ARGS...",
              "name": "args"
            }
          ]
        }
      },
      transform: {
        "SDL_SetError": {
          "name": "SetErrorUnformatted",
          "parameters": [
            {
              "type": "StringParam",
              "name": "message"
            }
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
      includeAfter: {
        "SDL_PollEvent": {
          "kind": "function",
          "name": "PollEvent",
          "type": "std::optional<Event>",
          "parameters": []
        },
        "SDL_WaitEvent": {
          "kind": "function",
          "name": "WaitEvent",
          "type": "std::optional<Event>",
          "parameters": []
        },
        "SDL_WaitEventTimeout": [
          {
            "kind": "function",
            "name": "WaitEventTimeout",
            "type": "std::optional<Event>",
            "parameters": [
              {
                "type": "Sint32",
                "name": "timeoutMS"
              }
            ]
          },
          {
            "kind": "function",
            "name": "WaitEventTimeout",
            "type": "bool",
            "parameters": [
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
          {
            "kind": "function",
            "name": "WaitEventTimeout",
            "type": "std::optional<Event>",
            "parameters": [
              {
                "type": "std::chrono::milliseconds",
                "name": "timeoutDuration"
              }
            ]
          }
        ],
        "SDL_PushEvent": {
          "kind": "function",
          "name": "PushEvent",
          "type": "bool",
          "parameters": [
            {
              "type": "const Event &",
              "name": "event"
            }
          ]
        },
        "SDL_EventFilter": [
          {
            "kind": "alias",
            "name": "EventFilterCB",
            "type": "std::function<bool(const Event &)>"
          },
          {
            "doc": "Handle returned by AddEventWatch()",
            "kind": "struct",
            "name": "EventWatchHandle",
            "entries": {
              "id": {
                "kind": "var",
                "type": "void *"
              },
              "EventWatchHandle": {
                "kind": "function",
                "type": "",
                "explicit": true,
                "constexpr": true,
                "parameters": [
                  {
                    "type": "void *",
                    "name": "id",
                    "default": "nullptr"
                  }
                ]
              },
              "get": {
                "kind": "function",
                "type": "void *",
                "constexpr": true,
                "immutable": true,
                "parameters": []
              },
              "operator bool": {
                "kind": "function",
                "type": "",
                "constexpr": true,
                "immutable": true,
                "parameters": []
              }
            }
          }
        ],
        "SDL_SetEventFilter": {
          "kind": "function",
          "name": "SetEventFilter",
          "type": "void",
          "parameters": [
            {
              "type": "EventFilterCB",
              "name": "filter",
              "default": "{}"
            }
          ]
        },
        "SDL_GetEventFilter": [
          {
            "kind": "function",
            "name": "GetEventFilter",
            "type": "EventFilterCB",
            "parameters": []
          },
          {
            "kind": "function",
            "name": "EventWatchAuxCallback",
            "type": "bool",
            "parameters": [
              {
                "type": "void *",
                "name": "userdata"
              },
              {
                "type": "Event *",
                "name": "event"
              }
            ]
          }
        ],
        "SDL_AddEventWatch": {
          "kind": "function",
          "name": "AddEventWatch",
          "type": "EventWatchHandle",
          "parameters": [
            {
              "type": "EventFilterCB",
              "name": "filter"
            }
          ]
        },
        "SDL_RemoveEventWatch": {
          "kind": "function",
          "name": "RemoveEventWatch",
          "type": "void",
          "parameters": [
            {
              "type": "EventWatchHandle",
              "name": "handle"
            }
          ]
        },
        "SDL_FilterEvents": {
          "kind": "function",
          "name": "FilterEvents",
          "type": "void",
          "parameters": [
            {
              "type": "EventFilterCB",
              "name": "filter"
            }
          ]
        }
      },
      enumerations: {
        "SDL_EventType": {
          prefix: "SDL_EVENT_"
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
      enumerations: {
        "SDL_HintPriority": {
          prefix: "SDL_HINT_",
          values: [
            "SDL_HINT_DEFAULT",
            "SDL_HINT_NORMAL",
            "SDL_HINT_OVERRIDE"
          ]
        }
      }
    },
    "SDL_init.h": {
      ignoreEntries: ["SDL_InitSubSystem"],
      namespacesMap: {
        "SDL_PROP_APP_METADATA_": "prop::appMetaData",
      },
      includeAfter: {
        "SDL_InitFlags": {
          "kind": "struct",
          "name": "InitFlagsExtra"
        },
        "SDL_Init": [
          {
            "kind": "function",
            "name": "InitSubSystem",
            "type": "bool",
            "template": [
              {
                "type": "class",
                "name": "FLAG0"
              },
              {
                "type": "class",
                "name": "FLAG1"
              },
              {
                "type": "class...",
                "name": "FLAGS"
              }
            ],
            "parameters": [
              {
                "type": "FLAG0",
                "name": "flag0"
              },
              {
                "type": "FLAG1",
                "name": "flag1"
              },
              {
                "type": "FLAGS...",
                "name": "flags"
              }
            ]
          },
          {
            "kind": "function",
            "name": "InitSubSystem",
            "type": "bool",
            "template": [
              {
                "type": "class",
                "name": "FLAG"
              },
              {
                "type": "class...",
                "name": "FLAGS"
              }
            ],
            "parameters": [
              {
                "type": "FLAG",
                "name": "flag0"
              },
              {
                "type": "FLAG",
                "name": "flag1"
              },
              {
                "type": "FLAGS...",
                "name": "flags"
              }
            ]
          }
        ],
        "SDL_QuitSubSystem": [
          {
            "kind": "function",
            "name": "QuitSubSystem",
            "type": "void",
            "template": [
              {
                "type": "class",
                "name": "FLAG0"
              },
              {
                "type": "class",
                "name": "FLAG1"
              },
              {
                "type": "class...",
                "name": "FLAGS"
              }
            ],
            "parameters": [
              {
                "type": "FLAG0",
                "name": "flag0"
              },
              {
                "type": "FLAG1",
                "name": "flag1"
              },
              {
                "type": "FLAGS...",
                "name": "flags"
              }
            ]
          },
          {
            "kind": "function",
            "name": "QuitSubSystem",
            "type": "void",
            "template": [
              {
                "type": "class",
                "name": "FLAG"
              },
              {
                "type": "class...",
                "name": "FLAGS"
              }
            ],
            "parameters": [
              {
                "type": "FLAG",
                "name": "flag0"
              },
              {
                "type": "FLAG",
                "name": "flag1"
              },
              {
                "type": "FLAGS...",
                "name": "flags"
              }
            ]
          }
        ],
        "SDL_WasInit": [
          {
            "kind": "function",
            "name": "WasInit",
            "type": "bool",
            "template": [
              {
                "type": "class",
                "name": "FLAG0"
              },
              {
                "type": "class",
                "name": "FLAG1"
              },
              {
                "type": "class...",
                "name": "FLAGS"
              }
            ],
            "parameters": [
              {
                "type": "FLAG0",
                "name": "flag0"
              },
              {
                "type": "FLAG1",
                "name": "flag1"
              },
              {
                "type": "FLAGS...",
                "name": "flags"
              }
            ]
          },
          {
            "kind": "function",
            "name": "WasInit",
            "type": "bool",
            "template": [
              {
                "type": "class",
                "name": "FLAG"
              },
              {
                "type": "class...",
                "name": "FLAGS"
              }
            ],
            "parameters": [
              {
                "type": "FLAG",
                "name": "flag0"
              },
              {
                "type": "FLAG",
                "name": "flag1"
              },
              {
                "type": "FLAGS...",
                "name": "flags"
              }
            ]
          }
        ],
        "SDL_Quit": {
          "kind": "struct",
          "name": "SDL",
          "entries": {
            "m_active": {
              "kind": "var",
              "type": "bool"
            },
            "updateActive": {
              "kind": "function",
              "type": "bool",
              "parameters": [
                {
                  "type": "bool",
                  "name": "active"
                }
              ]
            },
            "SDL": [
              {
                "kind": "function",
                "type": "",
                "template": [
                  {
                    "type": "class...",
                    "name": "FLAGS"
                  }
                ],
                "parameters": [
                  {
                    "type": "FLAGS...",
                    "name": "flags"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "constexpr": true,
                "parameters": []
              },
              {
                "kind": "function",
                "type": "",
                "parameters": [
                  {
                    "type": "const SDL &",
                    "name": "other"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "parameters": [
                  {
                    "type": "SDL &&",
                    "name": "other"
                  }
                ]
              }
            ]
          }
        },
        "SDL_MainThreadCallback": {
          "kind": "alias",
          "name": "MainThreadCB",
          "type": "std::function<void()>"
        },
        "SDL_RunOnMainThread": {
          "kind": "function",
          "name": "RunOnMainThread",
          "type": "bool",
          "parameters": [
            {
              "type": "MainThreadCB",
              "name": "callback"
            },
            {
              "type": "bool",
              "name": "wait_complete"
            }
          ]
        }
      },
      enumerations: {
        "SDL_InitFlags": {
          prefix: "SDL_INIT_"
        }
      },
      transform: {
        "SDL_Init": {
          name: "InitSubSystem"
        },
        "SDL_WasInit": {
          type: "bool"
        }
      }
    },
    "SDL_iostream.h": {
      ignoreEntries: [],
      includeAfter: {
        "__begin": [
          {
            "name": "IOStreamBase",
            "kind": "forward",
            "template": [
              {
                "name": "T",
                "type": "ObjectBox<SDL_IOStream *>"
              }
            ]
          },
          {
            "name": "IOStreamRef",
            "kind": "alias",
            "type": "IOStreamBase<ObjectRef<SDL_IOStream>>"
          },
          {
            "name": "IOStream",
            "kind": "alias",
            "type": "IOStreamBase<ObjectUnique<SDL_IOStream>>"
          },
          {
            "name": "IOFromDynamicMem_CtorTag",
            "kind": "struct",
            "doc": "@cat constructor-tag"
          }
        ],
        "SDL_OpenIO": {
          "name": "IOStreamBase.IOStreamBase",
          "kind": "function",
          "type": "",
          "static": false,
          "template": [
            {
              "type": "class",
              "name": "U"
            }
          ],
          "parameters": [
            {
              "type": "std::span<U>",
              "name": "mem"
            }
          ]
        },
        "SDL_IOFromConstMem": [],
        "SDL_SaveFile": [
          {
            "kind": "function",
            "name": "SaveFile",
            "type": "bool",
            "template": [
              {
                "type": "class",
                "name": "T"
              }
            ],
            "parameters": [
              {
                "type": "StringParam",
                "name": "file"
              },
              {
                "type": "std::span<T>",
                "name": "data"
              }
            ]
          },
          {
            "kind": "function",
            "name": "SaveFile",
            "type": "bool",
            "parameters": [
              {
                "type": "StringParam",
                "name": "file"
              },
              {
                "type": "std::string_view",
                "name": "str"
              }
            ]
          }
        ]
      },
      resources: {
        "SDL_IOStream": {
          paramType: "none",
          entries: {
            "SDL_IOFromFile": "ctor",
            "SDL_IOFromMem": "ctor",
            "SDL_IOFromConstMem": "ctor",
            "SDL_IOFromDynamicMem": {
              "name": "IOStreamBase",
              "type": "",
              "static": false,
              "parameters": [
                "IOFromDynamicMem_CtorTag"
              ]
            },
            "SDL_OpenIO": "ctor",
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
            "SDL_ReadIO": {
              "name": "Read"
            },
            "Write": [
              {
                "kind": "function",
                "type": "size_t",
                "template": [
                  {
                    "type": "class",
                    "name": "U"
                  }
                ],
                "parameters": [
                  {
                    "type": "std::span<U>",
                    "name": "data"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "size_t",
                "parameters": [
                  {
                    "type": "std::string_view",
                    "name": "str"
                  }
                ]
              }
            ],
            "SDL_WriteIO": {
              "name": "Write"
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
              "name": "printf"
            },
            "SDL_IOvprintf": {
              "name": "vprintf"
            },
            "SDL_FlushIO": {
              "name": "Flush"
            },
            "SDL_LoadFile_IO": {
              "name": "LoadFile",
              "type": "OwnArray<std::byte>",
              "static": false,
              "parameters": []
            },
            "SaveFile": [
              {
                "kind": "function",
                "type": "bool",
                "template": [
                  {
                    "type": "class",
                    "name": "U"
                  }
                ],
                "parameters": [
                  {
                    "type": "std::span<U>",
                    "name": "data"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "type": "std::string_view",
                    "name": "str"
                  }
                ]
              }
            ],
            "SDL_SaveFile_IO": {
              "name": "SaveFile",
              "parameters": [
                {},
                {},
                {}
              ]
            },
            "SDL_ReadU8": "function",
            "SDL_ReadS8": "function",
            "SDL_ReadU16LE": "function",
            "SDL_ReadS16LE": "function",
            "SDL_ReadU16BE": "function",
            "SDL_ReadS16BE": "function",
            "SDL_ReadU32LE": "function",
            "SDL_ReadS32LE": "function",
            "SDL_ReadU32BE": "function",
            "SDL_ReadS32BE": "function",
            "SDL_ReadU64LE": "function",
            "SDL_ReadS64LE": "function",
            "SDL_ReadU64BE": "function",
            "SDL_ReadS64BE": "function",
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
            "SDL_CloseIO": {
              "name": "Close"
            }
          }
        }
      },
      namespacesMap: {
        "SDL_PROP_IOSTREAM_": "prop::IOStream"
      },
      transform: {
        "SDL_LoadFile": {
          "type": "OwnArray<std::byte>",
          "parameters": [
            {}
          ]
        }
      }
    },
    "SDL_keyboard.h": {
      namespacesMap: {
        "SDL_PROP_TEXTINPUT_": "prop::TextInput"
      },
      transform: {
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
          "name": "WindowBase::StartTextInput",
          "type": "bool"
        },
        "SDL_StartTextInputWithProperties": {
          "name": "WindowBase::StartTextInput",
          "type": "bool"
        },
        "SDL_TextInputActive": {
          "name": "WindowBase::IsTextInputActive",
          "immutable": true,
          "type": "bool"
        },
        "SDL_StopTextInput": {
          "name": "WindowBase::StopTextInput",
          "type": "bool"
        },
        "SDL_ClearComposition": {
          "name": "WindowBase::ClearComposition",
          "type": "bool"
        },
        "SDL_SetTextInputArea": {
          "name": "WindowBase::SetTextInputArea",
          "type": "bool"
        },
        "SDL_GetTextInputArea": {
          "name": "WindowBase::GetTextInputArea",
          "type": "bool"
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
          entries: {
            "SDL_LoadObject": "ctor",
            "SDL_LoadFunction": "function",
            "SDL_UnloadObject": { name: "Unload" },
          }
        }
      }
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
      enumerations: {
        "SDL_LogPriority": {
          "includeAfter": "__begin",
          "prefix": "SDL_LOG_PRIORITY_"
        },
        "SDL_LogCategory": {
          "prefix": "SDL_LOG_CATEGORY_"
        }
      },
      includeAfter: {
        "SDL_LogOutputFunction": {
          "name": "LogOutputFunctionCB",
          "doc": "@sa LogOutputFunction",
          "kind": "alias"
        },
        "SDL_GetLogOutputFunction": {
          "name": "GetLogOutputFunction",
          "kind": "function",
          "type": "LogOutputFunctionCB",
          "parameters": []
        },
        "SDL_SetLogOutputFunction": [
          {
            "name": "SetLogOutputFunction",
            "kind": "function",
            "type": "void",
            "parameters": [
              {
                "type": "LogOutputFunctionCB",
                "name": "callback"
              }
            ]
          },
          {
            "name": "ResetLogOutputFunction",
            "kind": "function",
            "type": "void",
            "parameters": []
          }
        ]
      },
      transform: {
        "SDL_LogCategory": {
          "kind": "struct",
          "type": "",
          "entries": {
            "m_category": {
              "kind": "var",
              "type": "int"
            },
            "LogCategory": [
              {
                "kind": "function",
                "type": "",
                "explicit": true,
                "constexpr": true,
                "parameters": [
                  {
                    "type": "int",
                    "name": "category"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "constexpr": true,
                "parameters": [
                  {
                    "type": "SDL_LogCategory",
                    "name": "category"
                  }
                ]
              }
            ],
            "operator int": {
              "kind": "function",
              "type": "",
              "constexpr": true,
              "parameters": []
            },
            "operator SDL_LogCategory": {
              "kind": "function",
              "type": "",
              "constexpr": true,
              "parameters": []
            },
            "operator<=>": {
              "kind": "function",
              "immutable": true,
              "type": "auto",
              "parameters": [
                {
                  "type": "const LogCategory &",
                  "name": "other"
                }
              ]
            },
            "SDL_SetLogPriorities": "function",
            "SDL_SetLogPriority": {
              "static": false,
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
            "SDL_ResetLogPriorities": {
              "static": true
            },
            "SDL_LogMessage": {
              "name": "LogUnformatted",
              "immutable": true,
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
            "Log": {
              "kind": "function",
              "type": "void",
              "immutable": true,
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
    "SDL_pixels.h": {
      includeAfter: {
        "__begin": {
          "name": "Color",
          "kind": "forward"
        },
        "SDL_PackedLayout": {
          "name": "PixelFormatDetails",
        },
        "SDL_SetPaletteColors": {
          "kind": "function",
          "name": "PaletteBase.SetColors",
          "type": "bool",
          "static": false,
          "parameters": [
            {
              "type": "SpanRef<const SDL_Color>",
              "name": "colors"
            },
            {
              "type": "int",
              "name": "firstcolor",
              "default": "0"
            }
          ]
        }
      },
      enumerations: {
        "SDL_PixelFormat": {
          "kind": "struct",
          "type": "",
          "entries": {
            "format": {
              "kind": "var",
              "type": "SDL_PixelFormat"
            },
            "PixelFormat": {
              "kind": "function",
              "type": "",
              "constexpr": true,
              "parameters": [
                {
                  "type": "SDL_PixelFormat",
                  "name": "format",
                  "default": "SDL_PIXELFORMAT_UNKNOWN"
                }
              ]
            },
            "SDL_DEFINE_PIXELFORMAT": {
              "kind": "function",
              "name": "PixelFormat",
              "type": "",
              "constexpr": true,
              "parameters": [
                {
                  "type": "SDL_PixelType",
                  "name": "type"
                },
                {
                  "type": "int",
                  "name": "order"
                },
                {
                  "type": "SDL_PackedLayout",
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
            "operator bool": {
              "kind": "function",
              "parameters": [],
              "type": "",
              "immutable": true,
              "constexpr": true
            },
            "operator SDL_PixelFormat": {
              "kind": "function",
              "parameters": [],
              "type": "",
              "immutable": true,
              "constexpr": true
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
              "kind": "function",
              "type": "Uint32",
              "immutable": true,
              "parameters": [
                {
                  "type": "Color",
                  "name": "color"
                },
                {
                  "type": "PaletteRef",
                  "name": "palette"
                }
              ]
            },
            "Get": {
              "kind": "function",
              "type": "Color",
              "immutable": true,
              "parameters": [
                {
                  "type": "Uint32",
                  "name": "pixel"
                },
                {
                  "type": "PaletteRef",
                  "name": "palette"
                }
              ]
            }
          }
        },
        "SDL_Colorspace": {
          "kind": "struct",
          "type": "",
          "entries": {
            "colorspace": {
              "kind": "var",
              "type": "SDL_Colorspace"
            },
            "Colorspace": {
              "kind": "function",
              "type": "",
              "constexpr": true,
              "parameters": [
                {
                  "type": "SDL_Colorspace",
                  "name": "colorspace",
                  "default": "SDL_COLORSPACE_UNKNOWN"
                }
              ]
            },
            "SDL_DEFINE_COLORSPACE": {
              "kind": "function",
              "name": "Colorspace",
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
            "operator bool": {
              "kind": "function",
              "parameters": [],
              "type": "",
              "immutable": true,
              "constexpr": true
            },
            "operator SDL_Colorspace": {
              "kind": "function",
              "parameters": [],
              "type": "",
              "immutable": true,
              "constexpr": true
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
        }
      },
      wrappers: {
        "SDL_Color": {
          "ordered": true,
          "invalidState": false
        },
        "SDL_FColor": {
          "attribute": "color",
          "ordered": true,
          "invalidState": false
        }
      },
      resources: {
        "SDL_Palette": {
          "entries": {
            "SDL_CreatePalette": "ctor",
            "GetSize": {
              "kind": "function",
              "type": "int",
              "immutable": true,
              "constexpr": true,
              "parameters": []
            },
            "operator[]": {
              "kind": "function",
              "type": "Color",
              "immutable": true,
              "constexpr": true,
              "parameters": [
                {
                  "type": "int",
                  "name": "index"
                }
              ]
            },
            "SDL_SetPaletteColors": {
              "parameters": [
                {},
                {
                  "name": "colors",
                  "type": "std::span<const SDL_Color>"
                },
                {
                  "name": "firstcolor",
                  "type": "int",
                  "default": "0"
                }
              ]
            },
            "SDL_DestroyPalette": "function"
          }
        }
      }
    },
    "SDL_main.h": {
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
      includeAfter: {
        "__begin": [
          {
            kind: "def",
            name: "SDL_MAIN_HANDLED"
          },
          {
            kind: "def",
            name: "SDL_MAIN_USE_CALLBACKS"
          }
        ]
      }
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
                "kind": "function",
                "type": "",
                "constexpr": true,
                "parameters": [
                  {
                    "type": "const SDL_MessageBoxData &",
                    "name": "messageBox",
                    "default": "{}"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "constexpr": true,
                "parameters": [
                  {
                    "type": "MessageBoxFlags",
                    "name": "flags"
                  },
                  {
                    "type": "WindowRef",
                    "name": "window"
                  },
                  {
                    "type": "const char *",
                    "name": "title"
                  },
                  {
                    "type": "const char *",
                    "name": "message"
                  },
                  {
                    "type": "std::span<const MessageBoxButtonData>",
                    "name": "buttons"
                  },
                  {
                    "type": "OptionalRef<const MessageBoxColorScheme>",
                    "name": "colorScheme"
                  }
                ]
              }
            ],
            "SDL_ShowMessageBox": "function"
          }
        }
      }
    },
    "SDL_properties.h": {
      includeAfter: {
        "__begin": [
          {
            "name": "PropertiesBase",
            "kind": "forward"
          },
          {
            "name": "PropertiesRef",
            "kind": "alias"
          },
          {
            "name": "Properties",
            "kind": "alias"
          },
          {
            "name": "PropertyType",
          },
          {
            "name": "CleanupPropertyCallback",
          },
          {
            "name": "CleanupPropertyCB",
            "doc": "@sa PropertiesRef.CleanupCallback",
            "kind": "alias"
          },
          {
            "name": "EnumeratePropertiesCallback",
          },
          {
            "name": "EnumeratePropertiesCB",
            "doc": "@sa PropertiesRef.EnumerateCallback",
            "kind": "alias"
          },
          {
            "name": "PropertiesLock",
            "kind": "forward"
          }
        ],
        "SDL_PropertiesID": {
          "name": "PropertiesLock",
          "kind": "struct",
          "entries": {
            "properties": {
              "kind": "var",
              "type": "PropertiesRef"
            },
            "PropertiesLock": [
              {
                "doc": "@sa PropertiesBase.Lock()",
                "kind": "function",
                "type": "",
                "explicit": true,
                "parameters": [
                  {
                    "name": "properties",
                    "type": "PropertiesRef"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "parameters": []
              },
              {
                "kind": "function",
                "type": "",
                "parameters": [
                  {
                    "type": "const PropertiesLock &",
                    "name": "other"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "parameters": [
                  {
                    "type": "PropertiesLock &&",
                    "name": "other"
                  }
                ]
              }
            ],
            "~PropertiesLock": {
              "kind": "function",
              "doc": "@sa Unlock()",
              "type": "",
              "parameters": []
            },
            "operator=": {
              "kind": "function",
              "type": "PropertiesLock &",
              "parameters": [
                {
                  "type": "PropertiesLock",
                  "name": "other"
                }
              ]
            },
            "operator bool": {
              "kind": "function",
              "type": "",
              "constexpr": true,
              "immutable": true,
              "parameters": []
            },
            "SDL_UnlockProperties": {
              "name": "Unlock",
              "parameters": []
            }
          }
        }
      },
      enumerations: {
        "SDL_PropertyType": {
          includeAfter: "__begin",
          prefix: "SDL_PROPERTY_TYPE_"
        }
      },
      resources: {
        "SDL_PropertiesID": {
          "uniqueName": "Properties",
          "pointerType": "FancyPointer<SDL_PropertiesID>",
          "entries": {
            "SDL_CopyProperties": {
              "kind": "function",
              "name": "CopyPropertiesTo",
              "immutable": true
            },
            "SDL_LockProperties": {
              "type": "PropertiesLock",
              "reference": 1
            },
            "SetPointerWithCleanup": {
              "kind": "function",
              "static": false,
              "type": "bool",
              "parameters": [
                {
                  "type": "StringParam",
                  "name": "name"
                },
                {
                  "type": "void *",
                  "name": "value"
                },
                {
                  "type": "CleanupPropertyCB",
                  "name": "cleanup"
                }
              ]
            },
            "SDL_SetPointerPropertyWithCleanup": {
              "kind": "function",
              "name": "SetPointerWithCleanup"
            },
            "SDL_SetPointerProperty": {
              "kind": "function",
              "name": "SetPointer"
            },
            "SDL_SetStringProperty": {
              "kind": "function",
              "name": "SetString"
            },
            "SDL_SetNumberProperty": {
              "kind": "function",
              "name": "SetNumber"
            },
            "SDL_SetFloatProperty": {
              "kind": "function",
              "name": "SetFloat"
            },
            "SDL_SetBooleanProperty": {
              "kind": "function",
              "name": "SetBoolean"
            },
            "SDL_HasProperty": {
              "kind": "function",
              "name": "Has",
              "immutable": true
            },
            "SDL_GetPropertyType": {
              "kind": "function",
              "name": "GetType",
              "immutable": true
            },
            "SDL_GetPointerProperty": {
              "kind": "function",
              "name": "GetPointer",
              "immutable": true
            },
            "SDL_GetStringProperty": {
              "kind": "function",
              "name": "GetString",
              "immutable": true
            },
            "SDL_GetNumberProperty": {
              "kind": "function",
              "name": "GetNumber",
              "immutable": true
            },
            "SDL_GetFloatProperty": {
              "kind": "function",
              "name": "GetFloat",
              "immutable": true
            },
            "SDL_GetBooleanProperty": {
              "kind": "function",
              "name": "GetBoolean",
              "immutable": true
            },
            "SDL_ClearProperty": {
              "kind": "function",
              "name": "Clear"
            },
            "Enumerate": [
              {
                "kind": "function",
                "type": "bool",
                "immutable": true,
                "template": [
                  {
                    "type": "std::output_iterator<const char *>",
                    "name": "IT"
                  }
                ],
                "parameters": [
                  {
                    "type": "IT",
                    "name": "outputIter"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "bool",
                "immutable": true,
                "parameters": [
                  {
                    "type": "EnumeratePropertiesCB",
                    "name": "callback"
                  }
                ]
              }
            ],
            "SDL_EnumerateProperties": {
              "kind": "function",
              "name": "Enumerate",
              "immutable": true
            },
            "SDL_DestroyProperties": "function"
          }
        }
      },
      transform: {
        "SDL_CreateProperties": {
          "type": "Properties"
        }
      }
    },
    "SDL_rect.h": {
      includeAfter: {
        "__begin": [
          {
            "name": "FPoint",
            "kind": "forward"
          },
          {
            "name": "Rect",
            "kind": "forward"
          },
          {
            "name": "FRect",
            "kind": "forward"
          }
        ],
        "__end": [
          {
            "kind": "function",
            "name": "Rect.IntersectLine"
          },
          {
            "kind": "function",
            "name": "FRect.IntersectLine"
          }
        ]
      },
      wrappers: {
        "SDL_Point": {
          "attribute": "p",
          "ordered": true,
          "entries": {
            "Point": {
              "kind": "function",
              "type": "",
              "constexpr": true,
              "explicit": true,
              "parameters": [
                {
                  "type": "const SDL_FPoint &",
                  "name": "p"
                }
              ]
            },
            "SDL_PointInRect": {
              "kind": "function",
              "name": "IsInRect",
              "constexpr": true,
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "const Rect &"
                }
              ]
            }
          }
        },
        "SDL_FPoint": {
          "attribute": "p",
          "ordered": true,
          "entries": {
            "SDL_PointInRectFloat": {
              "kind": "function",
              "name": "IsInRect",
              "constexpr": true,
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "const FRect &"
                }
              ]
            }
          }
        },
        "SDL_Rect": {
          "attribute": "r",
          "entries": {
            "Rect": {
              "kind": "function",
              "type": "",
              "parameters": [
                {
                  "name": "corner",
                  "type": "const SDL_Point &"
                },
                {
                  "name": "size",
                  "type": "const SDL_Point &"
                }
              ]
            },
            "SDL_GetRectEnclosingPoints": {
              "type": "std::optional<Rect>",
              "parameters": [
                {
                  "type": "SpanRef<const SDL_Point>",
                  "name": "points"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "clip",
                  "default": "std::nullopt"
                }
              ]
            },
            "FromCenter": [
              {
                "kind": "function",
                "name": "FromCenter",
                "type": "Rect",
                "parameters": [
                  {
                    "name": "cx",
                    "type": "int"
                  },
                  {
                    "name": "cy",
                    "type": "int"
                  },
                  {
                    "name": "w",
                    "type": "int"
                  },
                  {
                    "name": "h",
                    "type": "int"
                  }
                ]
              },
              {
                "kind": "function",
                "name": "FromCenter",
                "type": "Rect",
                "parameters": [
                  {
                    "name": "center",
                    "type": "const Point &"
                  },
                  {
                    "name": "size",
                    "type": "const Point &"
                  }
                ]
              }
            ],
            "FromCorners": [
              {
                "kind": "function",
                "name": "FromCorners",
                "type": "Rect",
                "parameters": [
                  {
                    "name": "x1",
                    "type": "int"
                  },
                  {
                    "name": "y1",
                    "type": "int"
                  },
                  {
                    "name": "x2",
                    "type": "int"
                  },
                  {
                    "name": "y2",
                    "type": "int"
                  }
                ]
              },
              {
                "kind": "function",
                "name": "FromCorners",
                "type": "Rect",
                "parameters": [
                  {
                    "name": "p1",
                    "type": "const Point &"
                  },
                  {
                    "name": "p2",
                    "type": "const Point &"
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
            "SDL_GetRectAndLineIntersection": {
              "kind": "function",
              "name": "IntersectLine"
            },
            "SDL_RectToFRect": {
              "name": "operator FRect",
              "static": false,
              "immutable": true,
              "constexpr": true,
              "parameters": [],
              "type": ""
            },
            "operator SDL_FRect": {
              "doc": "@sa operator FRect()",
              "kind": "function",
              "immutable": true,
              "constexpr": true,
              "parameters": [],
              "type": ""
            },
            "SDL_RectEmpty": {
              "constexpr": true
            },
            "operator bool": {
              "doc": "@sa Empty()",
              "kind": "function",
              "immutable": true,
              "constexpr": true,
              "parameters": [],
              "type": ""
            },
            "SDL_RectsEqual": {
              "name": "Equal",
              "immutable": true,
              "constexpr": true,
              "parameters": [
                {
                  "name": "this",
                  "type": "const Rect &"
                },
                {
                  "name": "other",
                  "type": "const Rect &"
                }
              ]
            },
            "Contains": [
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "name": "p",
                    "type": "const Point &"
                  }
                ],
                "constexpr": true,
                "immutable": true
              },
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "name": "other",
                    "type": "const Rect &"
                  }
                ],
                "constexpr": true,
                "immutable": true
              }
            ],
            "SDL_HasRectIntersection": {
              "kind": "function",
              "immutable": true,
              "parameters": [
                {
                  "name": "this",
                  "type": "const Rect &"
                },
                {
                  "name": "other",
                  "type": "const Rect &"
                }
              ]
            },
            "SDL_GetRectIntersection": {
              "kind": "function",
              "type": "std::optional<Rect>",
              "immutable": true,
              "parameters": [
                {
                  "name": "this",
                  "type": "const Rect &"
                },
                {
                  "name": "other",
                  "type": "const Rect &"
                }
              ]
            },
            "SDL_GetRectUnion": {
              "kind": "function",
              "type": "std::optional<Rect>",
              "immutable": true,
              "parameters": [
                {
                  "name": "this",
                  "type": "const Rect &"
                },
                {
                  "name": "other",
                  "type": "const Rect &"
                }
              ]
            }
          }
        },
        "SDL_FRect": {
          "attribute": "r",
          "entries": {
            "FRect": {
              "kind": "function",
              "type": "",
              "parameters": [
                {
                  "name": "corner",
                  "type": "const SDL_FPoint &"
                },
                {
                  "name": "size",
                  "type": "const SDL_FPoint &"
                }
              ]
            },
            "SDL_GetRectEnclosingPointsFloat": {
              "type": "std::optional<FRect>",
              "parameters": [
                {
                  "type": "SpanRef<const SDL_FPoint>",
                  "name": "points"
                },
                {
                  "type": "OptionalRef<const SDL_FRect>",
                  "name": "clip",
                  "default": "std::nullopt"
                }
              ]
            },
            "FromCenter": [
              {
                "kind": "function",
                "name": "FromCenter",
                "type": "FRect",
                "static": true,
                "parameters": [
                  {
                    "name": "cx",
                    "type": "float"
                  },
                  {
                    "name": "cy",
                    "type": "float"
                  },
                  {
                    "name": "w",
                    "type": "float"
                  },
                  {
                    "name": "h",
                    "type": "float"
                  }
                ]
              },
              {
                "kind": "function",
                "name": "FromCenter",
                "type": "FRect",
                "static": true,
                "parameters": [
                  {
                    "name": "center",
                    "type": "const FPoint &"
                  },
                  {
                    "name": "size",
                    "type": "const FPoint &"
                  }
                ]
              }
            ],
            "FromCorners": [
              {
                "kind": "function",
                "name": "FromCorners",
                "type": "FRect",
                "static": true,
                "parameters": [
                  {
                    "name": "x1",
                    "type": "float"
                  },
                  {
                    "name": "y1",
                    "type": "float"
                  },
                  {
                    "name": "x2",
                    "type": "float"
                  },
                  {
                    "name": "y2",
                    "type": "float"
                  }
                ]
              },
              {
                "kind": "function",
                "name": "FromCorners",
                "static": true,
                "type": "FRect",
                "parameters": [
                  {
                    "name": "p1",
                    "type": "const FPoint &"
                  },
                  {
                    "name": "p2",
                    "type": "const FPoint &"
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
            "SDL_GetRectAndLineIntersectionFloat": {
              "name": "IntersectLine"
            },
            "SDL_RectEmptyFloat": {
              "constexpr": true
            },
            "operator bool": {
              "doc": "@sa Empty()",
              "kind": "function",
              "immutable": true,
              "constexpr": true,
              "parameters": [],
              "type": ""
            },
            "SDL_RectsEqualEpsilon": {
              "name": "EqualEpsilon",
              "immutable": true,
              "constexpr": true,
              "parameters": [
                {
                  "name": "this",
                  "type": "const FRect &"
                },
                {
                  "name": "other",
                  "type": "const FRect &"
                },
                {
                  "name": "epsilon",
                  "type": "const float"
                }
              ]
            },
            "SDL_RectsEqualFloat": {
              "name": "Equal",
              "immutable": true,
              "constexpr": true,
              "parameters": [
                {
                  "name": "this",
                  "type": "const FRect &"
                },
                {
                  "name": "other",
                  "type": "const FRect &"
                }
              ]
            },
            "Contains": [
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "name": "p",
                    "type": "const FPoint &"
                  }
                ],
                "constexpr": true,
                "immutable": true
              },
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "name": "other",
                    "type": "const FRect &"
                  }
                ],
                "constexpr": true,
                "immutable": true
              }
            ],
            "SDL_HasRectIntersectionFloat": {
              "kind": "function",
              "immutable": true,
              "parameters": [
                {
                  "name": "this",
                  "type": "const FRect &"
                },
                {
                  "name": "other",
                  "type": "const FRect &"
                }
              ]
            },
            "SDL_GetRectIntersectionFloat": {
              "kind": "function",
              "type": "std::optional<FRect>",
              "immutable": true,
              "parameters": [
                {
                  "name": "this",
                  "type": "const FRect &"
                },
                {
                  "name": "other",
                  "type": "const FRect &"
                }
              ]
            },
            "SDL_GetRectUnionFloat": {
              "kind": "function",
              "type": "std::optional<FRect>",
              "immutable": true,
              "parameters": [
                {
                  "name": "this",
                  "type": "const FRect &"
                },
                {
                  "name": "other",
                  "type": "const FRect &"
                }
              ]
            }
          }
        }
      }
    },
    "SDL_render.h": {
      ignoreEntries: [
        "SDL_LockTextureToSurface",
        "SDL_RenderDebugTextFormat",
        "SDL_GetRenderer",
        "SDL_GetTextureSize"
      ],
      includeAfter: {
        "__begin": [
          {
            "name": "TextureLock",
            "kind": "forward"
          },
          {
            "name": "Renderer",
            "kind": "alias",
            "type": "RendererBase<ObjectUnique<SDL_Renderer>>"
          }
        ],
        "SDL_Texture": {
          "name": "TextureLock",
          "kind": "struct",
          "entries": {
            "texture": {
              "kind": "var",
              "type": "TextureRef"
            },
            "surface": {
              "kind": "var",
              "type": "SurfaceRef"
            },
            "TextureLock": [
              {
                "doc": "@sa TextureBase.Lock()",
                "kind": "function",
                "type": "",
                "explicit": true,
                "parameters": [
                  {
                    "name": "texture",
                    "type": "TextureRef"
                  },
                  {
                    "name": "rect",
                    "type": "OptionalRef<const SDL_Rect>"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "parameters": []
              },
              {
                "kind": "function",
                "type": "",
                "parameters": [
                  {
                    "type": "const TextureLock &",
                    "name": "other"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "parameters": [
                  {
                    "type": "TextureLock &&",
                    "name": "other"
                  }
                ]
              }
            ],
            "~TextureLock": {
              "kind": "function",
              "doc": "@sa Unlock()",
              "type": "",
              "parameters": []
            },
            "operator=": {
              "kind": "function",
              "type": "TextureLock &",
              "parameters": [
                {
                  "type": "TextureLock",
                  "name": "other"
                }
              ]
            },
            "operator bool": {
              "kind": "function",
              "type": "",
              "constexpr": true,
              "immutable": true,
              "parameters": []
            },
            "SDL_UnlockTexture": {
              "name": "Unlock",
              "parameters": []
            },
            "GetPixels": {
              "kind": "function",
              "type": "void *",
              "immutable": true,
              "parameters": []
            },
            "GetPitch": {
              "kind": "function",
              "type": "int",
              "immutable": true,
              "parameters": []
            },
            "GetFormat": {
              "kind": "function",
              "type": "PixelFormat",
              "immutable": true,
              "parameters": []
            }
          }
        }
      },
      resources: {
        "SDL_Renderer": {
          "prependAliases": false,
          "entries": {
            "RendererBase": {
              "kind": "function",
              "type": "",
              "parameters": [
                {
                  "type": "WindowRef",
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
              "type": "std::optional<Point>",
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
              "type": "std::optional<Point>",
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
              "type": "bool",
              "parameters": []
            },
            "SDL_SetRenderTarget": {
              "name": "SetTarget"
            },
            "SDL_GetRenderTarget": {
              "immutable": true,
              "name": "GetTarget"
            },
            "SDL_SetRenderLogicalPresentation": {
              "name": "SetLogicalPresentation",
              "static": false,
              "parameters": [
                {
                  "type": "SDL_Point",
                  "name": "size"
                },
                {
                  "name": "mode",
                  "type": "RendererLogicalPresentation"
                }
              ]
            },
            "GetLogicalPresentation": {
              "kind": "function",
              "type": "bool",
              "parameters": [
                {
                  "type": "SDL_Point *",
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
              "type": "std::optional<FRect>",
              "parameters": []
            },
            "SDL_RenderCoordinatesFromWindow": {
              "kind": "function",
              "type": "std::optional<FPoint>",
              "immutable": true,
              "parameters": [
                {
                  "type": "const SDL_FPoint &",
                  "name": "window_coord"
                }
              ]
            },
            "SDL_RenderCoordinatesToWindow": {
              "kind": "function",
              "type": "std::optional<FPoint>",
              "immutable": true,
              "parameters": [
                {
                  "type": "const SDL_FPoint &",
                  "name": "coord"
                }
              ]
            },
            "SDL_ConvertEventToRenderCoordinates": "immutable",
            "ResetViewport": {
              "kind": "function",
              "type": "bool",
              "parameters": []
            },
            "SDL_SetRenderViewport": {
              "name": "SetViewport",
              "static": false,
              "parameters": [
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "rect"
                }
              ]
            },
            "SDL_GetRenderViewport": {
              "name": "GetViewport",
              "immutable": true,
              "type": "std::optional<Rect>",
              "parameters": []
            },
            "SDL_RenderViewportSet": {
              "immutable": true,
              "name": "IsViewportSet"
            },
            "SDL_GetRenderSafeArea": {
              "name": "GetSafeArea",
              "immutable": true,
              "type": "std::optional<Rect>",
              "parameters": []
            },
            "ResetClipRect": {
              "kind": "function",
              "type": "bool",
              "parameters": []
            },
            "SDL_SetRenderClipRect": {
              "name": "SetClipRect",
              "static": false,
              "parameters": [
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "rect"
                }
              ]
            },
            "SDL_GetRenderClipRect": {
              "name": "GetClipRect",
              "immutable": true,
              "type": "std::optional<Rect>",
              "parameters": []
            },
            "SDL_RenderClipEnabled": {
              "immutable": true,
              "name": "IsClipEnabled"
            },
            "SDL_SetRenderScale": {
              "name": "SetScale",
              "static": false,
              "parameters": [
                {
                  "type": "SDL_FPoint",
                  "name": "scale"
                }
              ]
            },
            "GetScale": {
              "kind": "function",
              "immutable": true,
              "type": "std::optional<FPoint>",
              "parameters": []
            },
            "SDL_GetRenderScale": {
              "name": "GetScale",
              "immutable": true
            },
            "SDL_SetRenderDrawColor": {
              "name": "SetDrawColor",
              "static": false,
              "parameters": [
                {
                  "type": "SDL_Color",
                  "name": "c"
                }
              ]
            },
            "SDL_SetRenderDrawColorFloat": {
              "name": "SetDrawColor",
              "static": false,
              "parameters": [
                {
                  "type": "SDL_FColor",
                  "name": "c"
                }
              ]
            },
            "GetDrawColor": [
              {
                "kind": "function",
                "immutable": true,
                "type": "std::optional<FColor>",
                "parameters": []
              },
              {
                "kind": "function",
                "immutable": true,
                "type": "bool",
                "parameters": [
                  {
                    "type": "SDL_Color *",
                    "name": "c"
                  }
                ]
              },
              {
                "kind": "function",
                "immutable": true,
                "type": "bool",
                "parameters": [
                  {
                    "type": "SDL_FColor *",
                    "name": "c"
                  }
                ]
              }
            ],
            "SDL_GetRenderDrawColor": {
              "immutable": true,
              "name": "GetDrawColor"
            },
            "SDL_GetRenderDrawColorFloat": {
              "immutable": true,
              "name": "GetDrawColor"
            },
            "SDL_SetRenderColorScale": {
              "name": "SetColorScale"
            },
            "SDL_GetRenderColorScale": {
              "name": "GetColorScale",
              "immutable": true,
              "type": "std::optional<float>",
              "parameters": []
            },
            "SDL_SetRenderDrawBlendMode": {
              "name": "SetDrawBlendMode"
            },
            "SDL_GetRenderDrawBlendMode": {
              "name": "GetDrawBlendMode",
              "immutable": true,
              "type": "std::optional<BlendMode>",
              "parameters": []
            },
            "SDL_RenderClear": "function",
            "SDL_RenderPoint": {
              "static": false,
              "parameters": [
                {
                  "type": "SDL_FPoint",
                  "name": "p"
                }
              ]
            },
            "SDL_RenderPoints": {
              "static": false,
              "parameters": [
                {
                  "type": "SpanRef<const SDL_FPoint>",
                  "name": "points"
                }
              ]
            },
            "SDL_RenderLine": {
              "static": false,
              "parameters": [
                {
                  "type": "SDL_FPoint",
                  "name": "p1"
                },
                {
                  "type": "SDL_FPoint",
                  "name": "p2"
                }
              ]
            },
            "SDL_RenderLines": {
              "static": false,
              "parameters": [
                {
                  "type": "SpanRef<const SDL_FPoint>",
                  "name": "points"
                }
              ]
            },
            "SDL_RenderRect": {
              "static": false,
              "parameters": [
                {
                  "type": "OptionalRef<const SDL_FRect>",
                  "name": "rect"
                }
              ]
            },
            "SDL_RenderRects": {
              "static": false,
              "parameters": [
                {
                  "type": "SpanRef<const SDL_FRect>",
                  "name": "rects"
                }
              ]
            },
            "SDL_RenderFillRect": {
              "static": false,
              "parameters": [
                {
                  "type": "OptionalRef<const SDL_FRect>",
                  "name": "rect"
                }
              ]
            },
            "SDL_RenderFillRects": {
              "static": false,
              "parameters": [
                {
                  "type": "SpanRef<const SDL_FRect>",
                  "name": "rects"
                }
              ]
            },
            "SDL_RenderTexture": {
              "static": false,
              "parameters": [
                {
                  "name": "texture",
                  "type": "TextureRef"
                },
                {
                  "name": "srcrect",
                  "type": "OptionalRef<const SDL_FRect>"
                },
                {
                  "name": "dstrect",
                  "type": "OptionalRef<const SDL_FRect>"
                }
              ]
            },
            "SDL_RenderTextureRotated": {
              "static": false,
              "parameters": [
                {
                  "name": "texture",
                  "type": "TextureRef"
                },
                {
                  "name": "srcrect",
                  "type": "OptionalRef<const SDL_FRect>"
                },
                {
                  "name": "dstrect",
                  "type": "OptionalRef<const SDL_FRect>"
                },
                {
                  "name": "angle",
                  "type": "double"
                },
                {
                  "name": "center",
                  "default": "{}",
                  "type": "OptionalRef<const SDL_FPoint>"
                },
                {
                  "name": "flip",
                  "type": "FlipMode",
                  "default": "SDL_FLIP_NONE"
                }
              ]
            },
            "SDL_RenderTextureAffine": {
              "static": false,
              "parameters": [
                {
                  "name": "texture",
                  "type": "TextureRef"
                },
                {
                  "name": "srcrect",
                  "type": "OptionalRef<const SDL_FRect>"
                },
                {
                  "name": "origin",
                  "type": "OptionalRef<const SDL_FPoint>"
                },
                {
                  "name": "right",
                  "type": "OptionalRef<const SDL_FPoint>"
                },
                {
                  "name": "down",
                  "type": "OptionalRef<const SDL_FPoint>"
                }
              ]
            },
            "SDL_RenderTextureTiled": {
              "static": false,
              "parameters": [
                {
                  "name": "texture",
                  "type": "TextureRef"
                },
                {
                  "name": "srcrect",
                  "type": "OptionalRef<const SDL_FRect>"
                },
                {
                  "name": "scale",
                  "type": "float"
                },
                {
                  "name": "dstrect",
                  "type": "OptionalRef<const SDL_FRect>"
                }
              ]
            },
            "SDL_RenderTexture9Grid": {
              "static": false,
              "parameters": [
                {
                  "name": "texture",
                  "type": "TextureRef"
                },
                {
                  "name": "srcrect",
                  "type": "OptionalRef<const SDL_FRect>"
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
                  "type": "OptionalRef<const SDL_FRect>"
                }
              ]
            },
            "SDL_RenderGeometry": {
              "static": false,
              "parameters": [
                {
                  "name": "texture",
                  "type": "TextureRef"
                },
                {
                  "name": "vertices",
                  "type": "std::span<const Vertex>"
                },
                {
                  "name": "indices",
                  "type": "std::span<const int>",
                  "default": "{}"
                }
              ]
            },
            "SDL_RenderGeometryRaw": {
              "parameters": [
                {},
                {
                  "name": "texture",
                  "type": "TextureRef"
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
              "immutable": true,
              "type": "Surface",
              "parameters": [
                {
                  "name": "rect",
                  "type": "OptionalRef<const SDL_Rect>"
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
              "kind": "function",
              "name": "GetVSync",
              "type": "std::optional<int>",
              "immutable": true,
              "parameters": []
            },
            "SDL_RenderDebugText": {
              "parameters": [
                {
                  "name": "this"
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
              "kind": "function",
              "type": "bool",
              "template": [
                {
                  "type": "class...",
                  "name": "ARGS"
                }
              ],
              "parameters": [
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
            "SDL_DestroyRenderer": "function"
          }
        },
        "SDL_Texture": {
          "entries": {
            "TextureBase": {
              "kind": "function",
              "type": "",
              "parameters": [
                {
                  "type": "RendererRef",
                  "name": "renderer"
                },
                {
                  "type": "StringParam",
                  "name": "file"
                }
              ]
            },
            "SDL_CreateTexture": "ctor",
            "SDL_CreateTextureFromSurface": "ctor",
            "SDL_CreateTextureWithProperties": "ctor",
            "SDL_GetTextureProperties": "immutable",
            "SDL_GetRendererFromTexture": {
              "name": "GetRenderer",
              "immutable": true
            },
            "SetColorAndAlphaMod": [
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "type": "Color",
                    "name": "c"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "type": "FColor",
                    "name": "c"
                  }
                ]
              }
            ],
            "GetColorAndAlphaMod": [
              {
                "kind": "function",
                "type": "std::optional<FColor>",
                "immutable": true,
                "parameters": []
              },
              {
                "kind": "function",
                "type": "bool",
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
                "type": "bool",
                "immutable": true,
                "parameters": [
                  {
                    "type": "FColor *",
                    "name": "c"
                  }
                ]
              }
            ],
            "SDL_SetTextureColorMod": "function",
            "SDL_SetTextureColorModFloat": "function",
            "SDL_GetTextureColorMod": "immutable",
            "SDL_GetTextureColorModFloat": "immutable",
            "SDL_SetTextureAlphaMod": "function",
            "SDL_SetTextureAlphaModFloat": "function",
            "GetAlphaMod": {
              "kind": "function",
              "immutable": true,
              "type": "std::optional<float>",
              "parameters": []
            },
            "SDL_GetTextureAlphaMod": "immutable",
            "SDL_GetTextureAlphaModFloat": "immutable",
            "SDL_SetTextureBlendMode": "function",
            "SDL_GetTextureBlendMode": {
              "kind": "function",
              "immutable": true,
              "type": "std::optional<BlendMode>",
              "parameters": []
            },
            "SDL_SetTextureScaleMode": "function",
            "SDL_GetTextureScaleMode": {
              "kind": "function",
              "immutable": true,
              "type": "std::optional<ScaleMode>",
              "parameters": []
            },
            "SDL_UpdateTexture": {
              "name": "Update",
              "parameters": [
                {},
                {
                  "type": "OptionalRef<const SDL_Rect>"
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
                  "type": "OptionalRef<const SDL_Rect>"
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
                  "type": "OptionalRef<const SDL_Rect>"
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
              "type": "TextureLock",
              "static": false,
              "reference": 1,
              "parameters": [
                {
                  "name": "rect",
                  "type": "OptionalRef<const SDL_Rect>"
                }
              ]
            },
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
            "GetFormat": {
              "kind": "function",
              "immutable": true,
              "type": "PixelFormat",
              "parameters": []
            },
            "SDL_DestroyTexture": "function"
          }
        }
      },
      namespacesMap: {
        "SDL_PROP_RENDERER_": "prop::Renderer",
        "SDL_PROP_TEXTURE_": "prop::Texture"
      },
      transform: {
        "SDL_CreateWindowAndRenderer": {
          type: "std::pair<Window, Renderer>",
          parameters: [
            {},
            {
              type: "SDL_Point",
              name: "size"
            },
            {
              type: "WindowFlags",
              name: "window_flags",
              default: "0"
            }
          ]
        }
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
      includeAfter: {
        "__begin": [
          {
            "name": "EnvironmentBase",
            "kind": "forward",
            "template": [
              {
                "name": "T",
                "type": "ObjectBox<SDL_Environment *>"
              }
            ]
          },
          {
            "name": "EnvironmentRef",
            "kind": "alias",
            "type": "EnvironmentBase<ObjectRef<SDL_Environment>>"
          },
          {
            "name": "Environment",
            "kind": "alias",
            "type": "EnvironmentBase<ObjectUnique<SDL_Environment>>"
          },
          {
            "name": "IConvBase",
            "kind": "forward",
            "template": [
              {
                "name": "T",
                "type": "ObjectBox<SDL_iconv_t>"
              }
            ]
          },
          {
            "name": "IConvRef",
            "kind": "alias",
            "type": "IConvBase<ObjectRef<SDL_iconv_data_t>>"
          },
          {
            "name": "IConv",
            "kind": "alias",
            "type": "IConvBase<ObjectUnique<SDL_iconv_data_t>>"
          }
        ],
        "SDL_rand_bits": {
          "kind": "struct",
          "name": "Random",
          "entries": {
            "m_state": {
              "kind": "var",
              "type": "Uint64"
            },
            "Random": {
              "kind": "function",
              "constexpr": true,
              "type": "",
              "parameters": [
                {
                  "type": "Uint64",
                  "name": "state",
                  "default": "0"
                }
              ]
            },
            "operator Uint64": {
              "kind": "function",
              "constexpr": true,
              "type": "",
              "parameters": []
            },
            "SDL_rand_r": {
              "name": "rand",
              "static": false,
              "parameters": [
                {
                  "type": "Sint32",
                  "name": "n"
                }
              ]
            },
            "SDL_randf_r": {
              "name": "randf",
              "static": false,
              "parameters": []
            },
            "SDL_rand_bits_r": {
              "name": "rand_bits",
              "static": false,
              "parameters": []
            }
          }
        }
      },
      resources: {
        "SDL_Environment": {
          "entries": {
            "SDL_CreateEnvironment": "ctor",
            "SDL_GetEnvironmentVariable": "function",
            "SDL_GetEnvironmentVariables": {
              "type": "OwnArray<char *>"
            },
            "GetVariableCount": {
              "kind": "function",
              "type": "Uint64",
              "parameters": []
            },
            "SDL_SetEnvironmentVariable": "function",
            "SDL_UnsetEnvironmentVariable": "function",
            "SDL_DestroyEnvironment": "function"
          }
        },
        "SDL_iconv_t": {
          "uniqueName": "IConv",
          "type": "SDL_iconv_data_t",
          "entries": {
            "SDL_iconv_open": "ctor",
            "SDL_iconv": "function",
            "SDL_iconv_close": {
              "name": "close"
            }
          }
        }
      },
      transform: {
        "SDL_Time": {
          "kind": "struct",
          "type": "",
          "entries": {
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
          "type": "EnvironmentRef"
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
        }
      }
    },
    "SDL_surface.h": {
      includeAfter: {
        "__begin": {
          "name": "SurfaceLock",
          "kind": "forward"
        },
        "SDL_Surface": {
          "name": "SurfaceLock",
          "kind": "struct",
          "entries": {
            "surface": {
              "kind": "var",
              "type": "SurfaceRef"
            },
            "SurfaceLock": [
              {
                "doc": "@sa SurfaceBase.Lock()",
                "kind": "function",
                "type": "",
                "explicit": true,
                "parameters": [
                  {
                    "name": "surface",
                    "type": "SurfaceRef"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "parameters": []
              },
              {
                "kind": "function",
                "type": "",
                "parameters": [
                  {
                    "type": "const SurfaceLock &",
                    "name": "other"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "",
                "parameters": [
                  {
                    "type": "SurfaceLock &&",
                    "name": "other"
                  }
                ]
              }
            ],
            "~SurfaceLock": {
              "kind": "function",
              "doc": "@sa Unlock()",
              "type": "",
              "parameters": []
            },
            "operator=": {
              "kind": "function",
              "type": "SurfaceLock &",
              "parameters": [
                {
                  "type": "SurfaceLock",
                  "name": "other"
                }
              ]
            },
            "operator bool": {
              "kind": "function",
              "type": "",
              "constexpr": true,
              "immutable": true,
              "parameters": []
            },
            "SDL_UnlockSurface": {
              "name": "Unlock",
              "parameters": []
            },
            "GetPixels": {
              "kind": "function",
              "type": "void *",
              "immutable": true,
              "parameters": []
            },
            "GetPitch": {
              "kind": "function",
              "type": "int",
              "immutable": true,
              "parameters": []
            },
            "GetFormat": {
              "kind": "function",
              "type": "PixelFormat",
              "immutable": true,
              "parameters": []
            }
          }
        }
      },
      resources: {
        "SDL_Surface": {
          "entries": {
            "SurfaceBase": {
              "kind": "function",
              "type": "",
              "parameters": [
                {
                  "type": "StringParam",
                  "name": "file"
                }
              ]
            },
            "SDL_CreateSurface": "ctor",
            "SDL_CreateSurfaceFrom": "ctor",
            "SDL_GetSurfaceProperties": "immutable",
            "SDL_SetSurfaceColorspace": "function",
            "SDL_GetSurfaceColorspace": "immutable",
            "SDL_CreateSurfacePalette": {
              "kind": "function",
              "name": "CreatePalette"
            },
            "SDL_SetSurfacePalette": "function",
            "SDL_GetSurfacePalette": "immutable",
            "SDL_AddSurfaceAlternateImage": {
              "kind": "function",
              "name": "AddAlternateImage"
            },
            "SDL_SurfaceHasAlternateImages": "immutable",
            "SDL_GetSurfaceImages": {
              "kind": "function",
              "immutable": true,
              "type": "OwnArray<SurfaceRef *>",
              "parameters": []
            },
            "SDL_RemoveSurfaceAlternateImages": {
              "kind": "function",
              "name": "RemoveAlternateImages"
            },
            "SDL_MUSTLOCK": {
              "kind": "function",
              "name": "MustLock",
              "constexpr": true,
              "immutable": true,
              "type": "bool",
              "parameters": []
            },
            "SDL_LockSurface": {
              "type": "SurfaceLock",
              "reference": 1
            },
            "SDL_SetSurfaceRLE": "function",
            "SDL_SurfaceHasRLE": "immutable",
            "SetColorKey": {
              "kind": "function",
              "type": "bool",
              "parameters": [
                {
                  "type": "Color",
                  "name": "key"
                }
              ]
            },
            "SDL_SetSurfaceColorKey": {
              "kind": "function",
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "std::optional<Uint32>",
                  "name": "key"
                }
              ]
            },
            "ClearColorKey": {
              "kind": "function",
              "type": "bool",
              "parameters": []
            },
            "SDL_SurfaceHasColorKey": "immutable",
            "GetColorKey": [
              {
                "kind": "function",
                "immutable": true,
                "type": "std::optional<Color>",
                "parameters": []
              },
              {
                "kind": "function",
                "immutable": true,
                "type": "bool",
                "parameters": [
                  {
                    "type": "Color *",
                    "name": "key"
                  }
                ]
              }
            ],
            "SDL_GetSurfaceColorKey": "immutable",
            "SDL_SetSurfaceColorMod": "function",
            "SDL_GetSurfaceColorMod": "immutable",
            "SDL_SetSurfaceAlphaMod": "function",
            "SDL_GetSurfaceAlphaMod": {
              "kind": "function",
              "immutable": true,
              "type": "std::optional<Uint8>",
              "parameters": []
            },
            "SetColorAndAlphaMod": {
              "kind": "function",
              "type": "bool",
              "parameters": [
                {
                  "type": "Color",
                  "name": "color"
                }
              ]
            },
            "GetColorAndAlphaMod": {
              "kind": "function",
              "immutable": true,
              "type": "std::optional<Color>",
              "parameters": []
            },
            "SDL_SetSurfaceBlendMode": "function",
            "SDL_GetSurfaceBlendMode": {
              "kind": "function",
              "immutable": true,
              "type": "std::optional<BlendMode>",
              "parameters": []
            },
            "SDL_SetSurfaceClipRect": {
              "kind": "function",
              "static": false,
              "parameters": [
                {
                  "type": "SDL_Surface *"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>"
                }
              ]
            },
            "ResetClipRect": {
              "kind": "function",
              "type": "bool",
              "parameters": []
            },
            "SDL_GetSurfaceClipRect": {
              "kind": "function",
              "immutable": true,
              "type": "std::optional<Rect>",
              "parameters": []
            },
            "SDL_FlipSurface": {
              "kind": "function",
              "name": "Flip"
            },
            "SDL_DuplicateSurface": {
              "name": "Duplicate",
              "type": "Surface",
              "immutable": true
            },
            "SDL_ScaleSurface": {
              "name": "Scale",
              "type": "Surface",
              "immutable": true
            },
            "SDL_ConvertSurface": {
              "name": "Convert",
              "type": "Surface",
              "immutable": true
            },
            "SDL_ConvertSurfaceAndColorspace": {
              "name": "Convert",
              "type": "Surface",
              "immutable": true
            },
            "SDL_PremultiplySurfaceAlpha": {
              "name": "PremultiplyAlpha"
            },
            "SDL_ClearSurface": {
              "kind": "function",
              "name": "Clear",
              "static": false,
              "parameters": [
                {
                  "type": "SDL_FColor",
                  "name": "color"
                }
              ]
            },
            "Fill": [
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "type": "SDL_Color",
                    "name": "color"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "type": "Uint32",
                    "name": "color"
                  }
                ]
              }
            ],
            "FillRect": {
              "kind": "function",
              "type": "bool",
              "parameters": [
                {
                  "type": "const SDL_Rect &",
                  "name": "rect"
                },
                {
                  "type": "SDL_Color",
                  "name": "color"
                }
              ]
            },
            "SDL_FillSurfaceRect": {
              "kind": "function",
              "name": "FillRect"
            },
            "FillRects": {
              "kind": "function",
              "type": "bool",
              "parameters": [
                {
                  "type": "SpanRef<const SDL_Rect>",
                  "name": "rects"
                },
                {
                  "type": "SDL_Color",
                  "name": "color"
                }
              ]
            },
            "SDL_FillSurfaceRects": {
              "kind": "function",
              "name": "FillRects",
              "parameters": [
                {
                  "type": "SDL_Surface *"
                },
                {
                  "type": "SpanRef<const SDL_Rect>",
                  "name": "rects"
                },
                {
                  "type": "Uint32",
                  "name": "color"
                }
              ]
            },
            "Blit": {
              "kind": "function",
              "name": "Blit",
              "parameters": [
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "srcrect"
                },
                {
                  "type": "const SDL_Point &",
                  "name": "dstpos"
                }
              ]
            },
            "SDL_BlitSurface": {
              "kind": "function",
              "name": "Blit",
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "srcrect"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "dstrect"
                }
              ]
            },
            "SDL_BlitSurfaceUnchecked": {
              "kind": "function",
              "name": "BlitUnchecked",
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "const SDL_Rect &",
                  "name": "srcrect"
                },
                {
                  "type": "const SDL_Rect &",
                  "name": "dstrect"
                }
              ]
            },
            "SDL_BlitSurfaceScaled": {
              "kind": "function",
              "name": "BlitScaled",
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "srcrect"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "dstrect"
                },
                {
                  "type": "ScaleMode",
                  "name": "scaleMode"
                }
              ]
            },
            "SDL_BlitSurfaceUncheckedScaled": {
              "kind": "function",
              "name": "BlitUncheckedScaled",
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "const SDL_Rect &",
                  "name": "srcrect"
                },
                {
                  "type": "const SDL_Rect &",
                  "name": "dstrect"
                },
                {
                  "type": "ScaleMode",
                  "name": "scaleMode"
                }
              ]
            },
            "SDL_StretchSurface": {
              "kind": "function",
              "since": {
                "tag": "SDL",
                "major": 3,
                "minor": 2,
                "patch": 4
              },
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "const SDL_Rect &",
                  "name": "srcrect"
                },
                {
                  "type": "const SDL_Rect &",
                  "name": "dstrect"
                },
                {
                  "type": "ScaleMode",
                  "name": "scaleMode"
                }
              ]
            },
            "SDL_BlitSurfaceTiled": {
              "kind": "function",
              "name": "BlitTiled",
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "srcrect"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "dstrect"
                }
              ]
            },
            "SDL_BlitSurfaceTiledWithScale": {
              "kind": "function",
              "name": "BlitTiledWithScale",
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "srcrect"
                },
                {
                  "type": "float",
                  "name": "scale"
                },
                {
                  "type": "SDL_ScaleMode",
                  "name": "scaleMode"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "dstrect"
                }
              ]
            },
            "Blit9Grid": {
              "kind": "function",
              "name": "Blit9Grid",
              "type": "bool",
              "parameters": [
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "srcrect"
                },
                {
                  "type": "int",
                  "name": "left_width"
                },
                {
                  "type": "int",
                  "name": "right_width"
                },
                {
                  "type": "int",
                  "name": "top_height"
                },
                {
                  "type": "int",
                  "name": "bottom_height"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "dstrect"
                }
              ]
            },
            "SDL_BlitSurface9Grid": {
              "kind": "function",
              "name": "Blit9GridWithScale",
              "parameters": [
                {
                  "name": "this"
                },
                {
                  "type": "SurfaceRef",
                  "name": "src"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "srcrect"
                },
                {
                  "type": "int",
                  "name": "left_width"
                },
                {
                  "type": "int",
                  "name": "right_width"
                },
                {
                  "type": "int",
                  "name": "top_height"
                },
                {
                  "type": "int",
                  "name": "bottom_height"
                },
                {
                  "type": "float",
                  "name": "scale"
                },
                {
                  "type": "SDL_ScaleMode",
                  "name": "scaleMode"
                },
                {
                  "type": "OptionalRef<const SDL_Rect>",
                  "name": "dstrect"
                }
              ]
            },
            "MapColor": {
              "kind": "function",
              "type": "Uint32",
              "immutable": true,
              "parameters": [
                {
                  "type": "SDL_Color",
                  "name": "color"
                }
              ]
            },
            "SDL_MapSurfaceRGB": {
              "kind": "function",
              "name": "MapColor",
              "immutable": true
            },
            "SDL_MapSurfaceRGBA": {
              "kind": "function",
              "name": "MapColor",
              "immutable": true
            },
            "ReadPixel": [
              {
                "kind": "function",
                "name": "ReadPixel",
                "immutable": true,
                "type": "std::optional<Color>",
                "parameters": [
                  {
                    "type": "int",
                    "name": "x"
                  },
                  {
                    "type": "int",
                    "name": "y"
                  }
                ]
              },
              {
                "kind": "function",
                "name": "ReadPixel",
                "immutable": true,
                "type": "bool",
                "parameters": [
                  {
                    "type": "int",
                    "name": "x"
                  },
                  {
                    "type": "int",
                    "name": "y"
                  },
                  {
                    "type": "SDL_Color *",
                    "name": "c"
                  }
                ]
              },
              {
                "kind": "function",
                "name": "ReadPixel",
                "immutable": true,
                "type": "bool",
                "parameters": [
                  {
                    "type": "int",
                    "name": "x"
                  },
                  {
                    "type": "int",
                    "name": "y"
                  },
                  {
                    "type": "SDL_FColor *",
                    "name": "c"
                  }
                ]
              }
            ],
            "SDL_ReadSurfacePixel": {
              "kind": "function",
              "name": "ReadPixel",
              "immutable": true
            },
            "SDL_ReadSurfacePixelFloat": {
              "kind": "function",
              "name": "ReadPixel",
              "immutable": true
            },
            "WritePixel": [
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "type": "int",
                    "name": "x"
                  },
                  {
                    "type": "int",
                    "name": "y"
                  },
                  {
                    "type": "SDL_Color",
                    "name": "c"
                  }
                ]
              },
              {
                "kind": "function",
                "type": "bool",
                "parameters": [
                  {
                    "type": "int",
                    "name": "x"
                  },
                  {
                    "type": "int",
                    "name": "y"
                  },
                  {
                    "type": "SDL_FColor",
                    "name": "c"
                  }
                ]
              }
            ],
            "SDL_WriteSurfacePixel": {
              "kind": "function",
              "name": "WritePixel"
            },
            "SDL_WriteSurfacePixelFloat": {
              "kind": "function",
              "name": "WritePixel"
            },
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
            "GetFormat": {
              "kind": "function",
              "immutable": true,
              "type": "PixelFormat",
              "parameters": []
            },
            "SDL_DestroySurface": "function"
          }
        }
      },
      enumerations: {
        "SDL_SurfaceFlags": {
          "values": [
            "SDL_SURFACE_PREALLOCATED",
            "SDL_SURFACE_LOCK_NEEDED",
            "SDL_SURFACE_LOCKED",
            "SDL_SURFACE_SIMD_ALIGNED"
          ]
        }
      },
      namespacesMap: {
        "SDL_PROP_SURFACE_": "prop::Surface"
      },
      transform: {
        "SDL_LoadBMP_IO": {
          "name": "LoadBMP",
          "type": "Surface",
          "parameters": [
            {
              "type": "ObjectBox<SDL_IOStream> auto &&"
            }
          ]
        },
        "SDL_LoadBMP": {
          "type": "Surface"
        },
        "SDL_SaveBMP_IO": {
          "name": "SaveBMP",
          "parameters": [
            {},
            {
              "type": "ObjectBox<SDL_IOStream> auto &&"
            }
          ]
        }
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
          entries: {
            "ThreadBase": {
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
            "SDL_SetCurrentThreadPriority": "function",
            "SDL_WaitThread": "function",
            "SDL_GetThreadState": "immutable",
            "SDL_DetachThread": "function",
            "SDL_GetTLS": "immutable",
            "SDL_SetTLS": "function",
            "SDL_CleanupTLS": "function",
          }
        }
      },
    },
    "SDL_time.h": {
      wrappers: {
        "SDL_DateTime": {
          "ordered": true,
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
        }
      },
      transform: {
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
        }
      }
    },
    "SDL_timer.h": {
      ignoreEntries: [
        "SDL_GetTicks",
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
      includeAfter: {
        "SDL_TimerCallback": [
          {
            "name": "NSTimerCallback"
          },
          {
            "kind": "alias",
            "name": "TimerCB"
          }
        ],
        "SDL_AddTimerNS": {
          "kind": "function",
          "name": "AddTimer",
          "type": "TimerID",
          "parameters": [
            {
              "type": "std::chrono::nanoseconds",
              "name": "interval"
            },
            {
              "type": "TimerCB",
              "name": "callback"
            }
          ]
        }
      },
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
        "SDL_AddTimer": {
          "parameters": [
            {
              "type": "std::chrono::milliseconds",
              "name": "interval"
            },
            {
              "type": "TimerCallback",
              "name": "callback"
            },
            {
              "type": "void *",
              "name": "userdata"
            }
          ]
        },
        "SDL_AddTimerNS": {
          "name": "AddTimer",
          "parameters": [
            {
              "type": "std::chrono::nanoseconds",
              "name": "interval"
            },
            {
              "type": "NSTimerCallback",
              "name": "callback"
            },
            {
              "type": "void *",
              "name": "userdata"
            }
          ]
        }
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
          kind: "alias",
          type: "std::function<void(TrayEntryRef)>"
        }],
        "SDL_InsertTrayEntryAt": {
          name: "TrayMenu::AppendEntry",
          kind: "function",
          type: "TrayEntry",
          static: false,
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
        }
      },
      resources: {
        "SDL_Tray": {
          entries: {
            "SDL_CreateTray": "ctor",
            "SDL_SetTrayIcon": "function",
            "SDL_SetTrayTooltip": "function",
            "SDL_UpdateTrays": "function",
            "SDL_DestroyTray": "function"
          }
        },
        "SDL_TrayEntry": {
          free: "SDL_RemoveTrayEntry",
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
            },
            "SDL_RemoveTrayEntry": {
              name: "Remove",
            },
          }
        },
      },
      wrappers: {
        "SDL_TrayMenu": {},
      },
      transform: {
        "SDL_CreateTrayMenu": {
          name: "TrayBase::CreateMenu",
        },
        "SDL_GetTrayMenu": {
          name: "TrayBase::GetMenu",
          immutable: true,
        },
        "SDL_GetTrayEntries": {
          name: "TrayMenu::GetEntries",
          type: "RefArray<TrayEntry>",
          parameters: [{}],
        },
        "SDL_InsertTrayEntryAt": {
          name: "TrayMenu::InsertEntry",
          type: "TrayEntry",
        },
        "SDL_GetTrayMenuParentEntry": {
          name: "TrayMenu::GetParentEntry",
          type: "TrayEntryRef",
          immutable: true,
        },
        "SDL_GetTrayMenuParentTray": {
          name: "TrayMenu::GetParentTray",
          type: "TrayRef",
          immutable: true,
        }
      }
    },
    "SDL_video.h": {
      includeAfter: {
        "__begin": [
          {
            "name": "WindowBase",
            "kind": "forward",
            "template": [
              {
                "name": "T",
                "type": "ObjectBox<SDL_Window *>"
              }
            ]
          },
          {
            "name": "WindowRef",
            "kind": "alias",
            "type": "WindowBase<ObjectRef<SDL_Window>>"
          },
          {
            "name": "Window",
            "kind": "alias",
            "type": "WindowBase<ObjectUnique<SDL_Window>>"
          },
          {
            "name": "RendererBase",
            "kind": "forward",
            "template": [
              {
                "type": "ObjectBox<SDL_Renderer *>",
                "name": "T"
              }
            ]
          },
          {
            "name": "RendererRef",
            "kind": "alias",
            "type": "RendererBase<ObjectRef<SDL_Renderer>>"
          },
          {
            "name": "DisplayOrientation",
          },
          {
            "name": "ORIENTATION_UNKNOWN",
          },
          {
            "name": "ORIENTATION_LANDSCAPE",
          },
          {
            "name": "ORIENTATION_LANDSCAPE_FLIPPED",
          },
          {
            "name": "ORIENTATION_PORTRAIT",
          },
          {
            "name": "ORIENTATION_PORTRAIT_FLIPPED",
          },
          {
            "name": "DisplayModeData",
          },
          {
            "name": "DisplayMode",
          },
          {
            "name": "DisplayOrientation",
          },
          {
            "name": "WindowFlags",
          },
          {
            "name": "WINDOW_FULLSCREEN",
          },
          {
            "name": "WINDOW_OPENGL",
          },
          {
            "name": "WINDOW_OCCLUDED",
          },
          {
            "name": "WINDOW_HIDDEN",
          },
          {
            "name": "WINDOW_BORDERLESS",
          },
          {
            "name": "WINDOW_RESIZABLE",
          },
          {
            "name": "WINDOW_MINIMIZED",
          },
          {
            "name": "WINDOW_MAXIMIZED",
          },
          {
            "name": "WINDOW_MOUSE_GRABBED",
          },
          {
            "name": "WINDOW_INPUT_FOCUS",
          },
          {
            "name": "WINDOW_MOUSE_FOCUS",
          },
          {
            "name": "WINDOW_EXTERNAL",
          },
          {
            "name": "WINDOW_MODAL",
          },
          {
            "name": "WINDOW_HIGH_PIXEL_DENSITY",
          },
          {
            "name": "WINDOW_MOUSE_CAPTURE",
          },
          {
            "name": "WINDOW_MOUSE_RELATIVE_MODE",
          },
          {
            "name": "WINDOW_ALWAYS_ON_TOP",
          },
          {
            "name": "WINDOW_UTILITY",
          },
          {
            "name": "WINDOW_TOOLTIP",
          },
          {
            "name": "WINDOW_POPUP_MENU",
          },
          {
            "name": "WINDOW_KEYBOARD_GRABBED",
          },
          {
            "name": "WINDOW_VULKAN",
          },
          {
            "name": "WINDOW_METAL",
          },
          {
            "name": "WINDOW_TRANSPARENT",
          },
          {
            "name": "WINDOW_NOT_FOCUSABLE",
          },
          {
            "name": "FlashOperation",
          },
          {
            "name": "FLASH_CANCEL",
          },
          {
            "name": "FLASH_BRIEFLY",
          },
          {
            "name": "FLASH_UNTIL_FOCUSED",
          },
          {
            "name": "HitTestResult",
          },
          {
            "name": "HITTEST_NORMAL",
          },
          {
            "name": "HITTEST_DRAGGABLE",
          },
          {
            "name": "HITTEST_RESIZE_TOPLEFT",
          },
          {
            "name": "HITTEST_RESIZE_TOP",
          },
          {
            "name": "HITTEST_RESIZE_TOPRIGHT",
          },
          {
            "name": "HITTEST_RESIZE_RIGHT",
          },
          {
            "name": "HITTEST_RESIZE_BOTTOMRIGHT",
          },
          {
            "name": "HITTEST_RESIZE_BOTTOM",
          },
          {
            "name": "HITTEST_RESIZE_BOTTOMLEFT",
          },
          {
            "name": "HITTEST_RESIZE_LEFT",
          },
          {
            "name": "HitTest",
          },
          {
            "kind": "alias",
            "name": "HitTestCB",
            "doc": "@sa HitTest"
          }
        ]
      },
      resources: {
        "SDL_Window": {
          "free": "SDL_DestroyWindow",
          "entries": {
            "SDL_CreateWindow": {
              "kind": "function",
              "name": "WindowBase",
              "type": "",
              "static": false,
              "parameters": [
                {
                  "name": "title",
                  "type": "StringParam"
                },
                {
                  "name": "size",
                  "type": "SDL_Point"
                },
                {
                  "name": "flags",
                  "type": "WindowFlags",
                  "default": "0"
                }
              ]
            },
            "SDL_CreatePopupWindow": {
              "kind": "function",
              "name": "WindowBase",
              "type": "",
              "parameters": [
                {
                  "name": "parent",
                  "type": "WindowRef"
                },
                {
                  "name": "offset",
                  "type": "SDL_Point"
                },
                {
                  "name": "size",
                  "type": "SDL_Point"
                },
                {
                  "name": "flags",
                  "type": "WindowFlags",
                  "default": "0"
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
              "kind": "function",
              "immutable": true,
              "type": "OwnPtr<void>"
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
              "type": "bool",
              "parameters": [
                {
                  "type": "Rect",
                  "name": "rect"
                }
              ]
            },
            "GetRect": {
              "kind": "function",
              "type": "std::optional<Rect>",
              "immutable": true,
              "parameters": []
            },
            "SDL_SetWindowPosition": {
              "static": false,
              "parameters": [
                {
                  "type": "SDL_Point",
                  "name": "p"
                }
              ]
            },
            "GetPosition": {
              "kind": "function",
              "type": "std::optional<Point>",
              "immutable": true,
              "parameters": []
            },
            "SDL_GetWindowPosition": "immutable",
            "SDL_SetWindowSize": {
              "static": false,
              "parameters": [
                {
                  "type": "SDL_Point",
                  "name": "p"
                }
              ]
            },
            "GetSize": {
              "kind": "function",
              "type": "std::optional<Point>",
              "immutable": true,
              "parameters": []
            },
            "SDL_GetWindowSize": "immutable",
            "SDL_GetWindowSafeArea": {
              "kind": "function",
              "type": "std::optional<Rect>",
              "immutable": true,
              "parameters": []
            },
            "SDL_SetWindowAspectRatio": "function",
            "SDL_GetWindowAspectRatio": "immutable",
            "SDL_GetWindowBordersSize": "immutable",
            "GetSizeInPixels": {
              "kind": "function",
              "type": "std::optional<Point>",
              "immutable": true,
              "parameters": []
            },
            "SDL_GetWindowSizeInPixels": "immutable",
            "SDL_SetWindowMinimumSize": {
              "static": false,
              "parameters": [
                {
                  "type": "SDL_Point",
                  "name": "p"
                }
              ]
            },
            "SDL_GetWindowMinimumSize": "immutable",
            "SDL_SetWindowMaximumSize": {
              "static": false,
              "parameters": [
                {
                  "type": "SDL_Point",
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
            "GetRenderer": {
              "kind": "function",
              "type": "RendererRef",
              "immutable": true,
              "parameters": []
            },
            "SDL_WindowHasSurface": "immutable",
            "SDL_GetWindowSurface": "function",
            "SDL_SetWindowSurfaceVSync": "function",
            "SDL_GetWindowSurfaceVSync": {
              "immutable": true,
              "type": "std::optional<int>",
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
                  "type": "SpanRef<const SDL_Rect>"
                }
              ]
            },
            "SDL_DestroyWindowSurface": "function",
            "SDL_SetWindowKeyboardGrab": "function",
            "SDL_SetWindowMouseGrab": "function",
            "SDL_GetWindowKeyboardGrab": "immutable",
            "SDL_GetWindowMouseGrab": "immutable",
            "SDL_SetWindowMouseRect": "function",
            "SDL_GetWindowMouseRect": "immutable",
            "SDL_SetWindowOpacity": "function",
            "SDL_GetWindowOpacity": "immutable",
            "SDL_SetWindowParent": "function",
            "SDL_SetWindowModal": "function",
            "SDL_SetWindowFocusable": "function",
            "SDL_ShowWindowSystemMenu": {
              "static": false,
              "parameters": [
                {
                  "type": "SDL_Point",
                  "name": "p"
                }
              ]
            },
            "SetHitTest": {
              "kind": "function",
              "type": "bool",
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
            "SDL_DestroyWindow": "function"
          }
        },
        "SDL_GLContext": {
          "type": "SDL_GLContextState",
          "entries": {
            "SDL_GL_CreateContext": "ctor",
            "SDL_GL_MakeCurrent": {
              "name": "MakeCurrent",
              "static": false,
              "parameters": [
                {
                  "type": "WindowRef",
                  "name": "window"
                }
              ]
            },
            "SDL_GL_DestroyContext": {
              "name": "Destroy"
            }
          }
        }
      },
      enumerations: {
        "SDL_GLProfile": {
          "prefix": "SDL_GL_CONTEXT_PROFILE_"
        },
        "SDL_GLContextFlag": {
          "prefix": "SDL_GL_CONTEXT_",
          "values": [
            "SDL_GL_CONTEXT_DEBUG_FLAG",
            "SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG",
            "SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG",
            "SDL_GL_CONTEXT_RESET_ISOLATION_FLAG"
          ]
        },
        "SDL_GLContextReleaseFlag": {
          "prefix": "SDL_GL_CONTEXT_RELEASE_",
          "values": [
            "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE",
            "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH"
          ]
        },
        "SDL_GLContextResetNotification": {
          "prefix": "SDL_GL_CONTEXT_RESET_",
          "values": [
            "SDL_GL_CONTEXT_RESET_NO_NOTIFICATION",
            "SDL_GL_CONTEXT_RESET_LOSE_CONTEXT"
          ]
        }
      },
      namespacesMap: {
        "SDL_PROP_GLOBAL_": "prop::Global",
        "SDL_PROP_DISPLAY_": "prop::Display",
        "SDL_PROP_WINDOW_": "prop::Window"
      },
      wrappers: {
        "SDL_DisplayID": {
          "name": "Display",
          "attribute": "displayID",
          "entries": {
            "SDL_GetDisplays": {
              "kind": "function",
              "name": "GetAll",
              "type": "OwnArray<Display>",
              "parameters": []
            },
            "SDL_GetPrimaryDisplay": {
              "kind": "function",
              "name": "GetPrimary"
            },
            "SDL_GetDisplayProperties": "immutable",
            "SDL_GetDisplayName": "immutable",
            "SDL_GetDisplayBounds": {
              "kind": "function",
              "type": "std::optional<Rect>",
              "parameters": [],
              "immutable": true
            },
            "SDL_GetDisplayUsableBounds": {
              "kind": "function",
              "type": "std::optional<Rect>",
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
              "name": "GetClosestFullscreenMode",
              "immutable": true,
              "type": "std::optional<DisplayMode>",
              "parameters": [
                {},
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
        }
      },
      transform: {
        "SDL_GetWindows": {
          "kind": "function",
          "type": "OwnArray<WindowRef>",
          "parameters": []
        },
        "SDL_WINDOW_FULLSCREEN": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_OPENGL": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_OCCLUDED": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_HIDDEN": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_BORDERLESS": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_RESIZABLE": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_MINIMIZED": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_MAXIMIZED": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_MOUSE_GRABBED": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_INPUT_FOCUS": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_MOUSE_FOCUS": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_EXTERNAL": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_MODAL": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_HIGH_PIXEL_DENSITY": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_MOUSE_CAPTURE": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_MOUSE_RELATIVE_MODE": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_ALWAYS_ON_TOP": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_UTILITY": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_TOOLTIP": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_POPUP_MENU": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_KEYBOARD_GRABBED": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_VULKAN": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_METAL": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_TRANSPARENT": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        },
        "SDL_WINDOW_NOT_FOCUSABLE": {
          "kind": "var",
          "constexpr": true,
          "type": "WindowFlags"
        }
      }
    },

    // SDL satellite Libraries:
    "SDL_image.h": {
      ignoreEntries: [
        "IMG_LoadBMP_IO"
      ],
      includeAfter: {
        "__begin": {
          "name": "SDL3PP_ENABLE_IMAGE"
        }
      },
      resources: {
        "IMG_Animation": {
          "returnType": "unique",
          "entries": {
            "IMG_LoadAnimation": "ctor",
            "IMG_LoadAnimation_IO": {
              "name": "AnimationBase",
              "type": "",
              "static": false,
              "parameters": [
                {
                  "name": "src",
                  "type": "ObjectBox<SDL_IOStream> auto &&"
                }
              ]
            },
            "IMG_LoadAnimationTyped_IO": {
              "name": "AnimationBase",
              "type": "",
              "static": false,
              "parameters": [
                {
                  "name": "src",
                  "type": "ObjectBox<SDL_IOStream> auto &&"
                },
                {
                  "name": "type",
                  "type": "StringParam"
                }
              ]
            },
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
            "GetCount": {
              "kind": "function",
              "immutable": true,
              "type": "int",
              "parameters": []
            },
            "GetFrame": {
              "kind": "function",
              "type": "SurfaceRef",
              "immutable": true,
              "parameters": [
                {
                  "type": "int",
                  "name": "index"
                }
              ]
            },
            "GetDelay": {
              "kind": "function",
              "type": "int",
              "immutable": true,
              "parameters": [
                {
                  "type": "int",
                  "name": "index"
                }
              ]
            },
            "IMG_FreeAnimation": "function"
          }
        }
      },
      transform: {
        "IMG_Version": {
          "name": "IMG_Version"
        },
        "IMG_LoadTyped_IO": {
          "type": "Surface",
          "name": "LoadSurface",
          "parameters": [
            {
              "name": "src",
              "type": "ObjectBox<SDL_IOStream> auto &&"
            },
            {
              "name": "type",
              "type": "StringParam"
            }
          ]
        },
        "IMG_Load": {
          "type": "Surface",
          "name": "LoadSurface"
        },
        "IMG_Load_IO": {
          "type": "Surface",
          "name": "LoadSurface",
          "parameters": [
            {
              "name": "src",
              "type": "ObjectBox<SDL_IOStream> auto &&"
            }
          ]
        },
        "IMG_LoadTexture": {
          "type": "Texture"
        },
        "IMG_LoadTexture_IO": {
          "type": "Texture",
          "name": "LoadTexture",
          "parameters": [
            {
              "name": "renderer",
              "type": "RendererRef"
            },
            {
              "name": "src",
              "type": "ObjectBox<SDL_IOStream> auto &&"
            }
          ]
        },
        "IMG_LoadTextureTyped_IO": {
          "type": "Texture",
          "name": "LoadTexture",
          "parameters": [
            {
              "name": "renderer",
              "type": "RendererRef"
            },
            {
              "name": "src",
              "type": "ObjectBox<SDL_IOStream> auto &&"
            },
            {
              "name": "type",
              "type": "StringParam"
            }
          ]
        },
        "IMG_LoadAVIF_IO": {
          "type": "Surface",
          "name": "LoadAVIF"
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
          "parameters": [
            {
              "name": "surface",
              "type": "SurfaceRef"
            },
            {
              "name": "dst",
              "type": "ObjectBox<SDL_IOStream> auto &&"
            },
            {
              "name": "quality",
              "type": "int"
            }
          ]
        },
        "IMG_SavePNG_IO": {
          "name": "SavePNG",
          "parameters": [
            {
              "name": "surface",
              "type": "SurfaceRef"
            },
            {
              "name": "dst",
              "type": "ObjectBox<SDL_IOStream> auto &&"
            }
          ]
        },
        "IMG_SaveJPG_IO": {
          "name": "SaveJPG",
          "parameters": [
            {
              "name": "surface",
              "type": "SurfaceRef"
            },
            {
              "name": "dst",
              "type": "ObjectBox<SDL_IOStream> auto &&"
            },
            {
              "name": "quality",
              "type": "int"
            }
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
      ignoreEntries: [
        "TTF_DestroySurfaceTextEngine",
        "TTF_DestroyRendererTextEngine",
        "TTF_DestroyGPUTextEngine"
      ],
      resources: {
        "TTF_Font": {
          entries: {
            "TTF_OpenFont": "ctor",
            "TTF_OpenFontIO": "ctor",
            "TTF_OpenFontWithProperties": "ctor",
            "TTF_CopyFont": {
              "immutable": true,
              "type": "Font"
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
            "TTF_GetGlyphScript": "immutable",
            "TTF_SetFontLanguage": "function",
            "TTF_FontHasGlyph": "immutable",
            "TTF_GetGlyphImage": "immutable",
            "TTF_GetGlyphImageForIndex": "immutable",
            "TTF_GetGlyphMetrics": "immutable",
            "TTF_GetGlyphKerning": {
              "immutable": true,
              "type": "std::optional<int>",
              "parameters": [
                {},
                {},
                {}
              ]
            },
            "GetStringSize": {
              "kind": "function",
              "immutable": true,
              "type": "std::optional<Point>",
              "parameters": [
                {
                  "name": "text",
                  "type": "std::string_view"
                }
              ]
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
              "type": "std::optional<Point>",
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
            "TTF_CloseFont": "function"
          }
        },
        "TTF_TextEngine": {
          pointerType: "TextEngineWrapper",
          free: "",
          paramType: "ref",
          returnType: "unique",
          entries: {
            "TTF_SetGPUTextEngineWinding": {
              "name": "SetGPUWinding"
            },
            "TTF_GetGPUTextEngineWinding": {
              "immutable": true,
              "name": "GetGPUWinding"
            },
            "Destroy": {
              "kind": "function",
              "type": "void",
              "parameters": []
            }
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
                  "type": "SurfaceRef",
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
              "name": "TextBase",
              "type": "",
              "parameters": [
                {
                  "type": "TextEngineRef",
                  "name": "engine"
                },
                {
                  "type": "FontRef",
                  "name": "font"
                },
                {
                  "type": "std::string_view",
                  "name": "text"
                }
              ]
            },
            "TTF_GetTextProperties": "immutable",
            "TTF_SetTextEngine": "function",
            "TTF_GetTextEngine": {
              "type": "TextEngineRef",
              "immutable": true
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
                "type": "std::optional<FColor>",
                "immutable": true,
                "parameters": []
              },
              {
                "kind": "function",
                "type": "bool",
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
                "type": "bool",
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
              "type": "std::optional<Point>",
              "parameters": []
            },
            "TTF_GetTextPosition": "immutable",
            "TTF_SetTextWrapWidth": "function",
            "TTF_GetTextWrapWidth": {
              "immutable": true,
              "type": "std::optional<int>",
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
              "type": "std::optional<Point>",
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
            },
            "TTF_DestroyText": "function"
          }
        }
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
            "name": "TextEngineWrapper",
            "kind": "struct",
            "entries": {
              "m_ptr": {
                "kind": "var",
                "type": "TTF_TextEngine *"
              }
            }
          }
        ],
        "TTF_TextEngine": [
          {
            "name": "ObjectRef<TTF_TextEngine, TextEngineWrapper>::doFree",
            "kind": "function",
            "type": "void",
            "template": [],
            "parameters": [
              {
                "type": "TextEngineWrapper",
                "name": "resource"
              }
            ]
          },
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
              "type": "TextRef"
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
                    "type": "TextRef"
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
