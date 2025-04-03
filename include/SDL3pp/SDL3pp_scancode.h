#ifndef SDL3PP_SCANCODE_H_
#define SDL3PP_SCANCODE_H_

#include <SDL3/SDL_scancode.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 *
 * @defgroup CategoryScancode Keyboard Scancodes
 *
 * Defines keyboard scancodes.
 *
 * Please refer to the Best Keyboard Practices document for details on what
 * this information means and how best to use it.
 *
 * https://wiki.libsdl.org/SDL3/BestKeyboardPractices
 *
 * @{
 */

// Forward decl
struct Keycode;

/**
 * The SDL keyboard scancode representation.
 *
 * An SDL scancode is the physical representation of a key on the keyboard,
 * independent of language and keyboard mapping.
 *
 * Values of this type are used to represent keyboard keys, among other places
 * in the `scancode` field of the KeyboardEvent structure.
 *
 * The values in this enumeration are based on the USB usage page standard:
 * https://usb.org/sites/default/files/hut1_5.pdf
 *
 * @since This enum is available since SDL 3.2.0.
 */
struct Scancode
{
  SDL_Scancode m_scancode;

  /**
   * Wraps Scancode.
   *
   * @param scancode the value to be wrapped
   */
  constexpr Scancode(SDL_Scancode scancode = {})
    : m_scancode(scancode)
  {
  }

  // Get scan code from name
  Scancode(StringParam name);

  constexpr auto operator<=>(const Scancode& other) const = default;

  /**
   * Unwraps to the underlying Scancode.
   *
   * @returns the underlying Scancode.
   */
  constexpr operator SDL_Scancode() const { return m_scancode; }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const
  {
    return m_scancode != SDL_SCANCODE_UNKNOWN;
  }

  // Set name
  bool SetName(StringParam name);

  // Get name
  const char* GetName() const;
};

constexpr Scancode SCANCODE_UNKNOWN = SDL_SCANCODE_UNKNOWN;

constexpr Scancode SCANCODE_A = SDL_SCANCODE_A;

constexpr Scancode SCANCODE_B = SDL_SCANCODE_B;

constexpr Scancode SCANCODE_C = SDL_SCANCODE_C;

constexpr Scancode SCANCODE_D = SDL_SCANCODE_D;

constexpr Scancode SCANCODE_E = SDL_SCANCODE_E;

constexpr Scancode SCANCODE_F = SDL_SCANCODE_F;

constexpr Scancode SCANCODE_G = SDL_SCANCODE_G;

constexpr Scancode SCANCODE_H = SDL_SCANCODE_H;

constexpr Scancode SCANCODE_I = SDL_SCANCODE_I;

constexpr Scancode SCANCODE_J = SDL_SCANCODE_J;

constexpr Scancode SCANCODE_K = SDL_SCANCODE_K;

constexpr Scancode SCANCODE_L = SDL_SCANCODE_L;

constexpr Scancode SCANCODE_M = SDL_SCANCODE_M;

constexpr Scancode SCANCODE_N = SDL_SCANCODE_N;

constexpr Scancode SCANCODE_O = SDL_SCANCODE_O;

constexpr Scancode SCANCODE_P = SDL_SCANCODE_P;

constexpr Scancode SCANCODE_Q = SDL_SCANCODE_Q;

constexpr Scancode SCANCODE_R = SDL_SCANCODE_R;

constexpr Scancode SCANCODE_S = SDL_SCANCODE_S;

constexpr Scancode SCANCODE_T = SDL_SCANCODE_T;

constexpr Scancode SCANCODE_U = SDL_SCANCODE_U;

constexpr Scancode SCANCODE_V = SDL_SCANCODE_V;

constexpr Scancode SCANCODE_W = SDL_SCANCODE_W;

constexpr Scancode SCANCODE_X = SDL_SCANCODE_X;

constexpr Scancode SCANCODE_Y = SDL_SCANCODE_Y;

constexpr Scancode SCANCODE_Z = SDL_SCANCODE_Z;

constexpr Scancode SCANCODE_1 = SDL_SCANCODE_1;

constexpr Scancode SCANCODE_2 = SDL_SCANCODE_2;

constexpr Scancode SCANCODE_3 = SDL_SCANCODE_3;

constexpr Scancode SCANCODE_4 = SDL_SCANCODE_4;

constexpr Scancode SCANCODE_5 = SDL_SCANCODE_5;

constexpr Scancode SCANCODE_6 = SDL_SCANCODE_6;

constexpr Scancode SCANCODE_7 = SDL_SCANCODE_7;

constexpr Scancode SCANCODE_8 = SDL_SCANCODE_8;

constexpr Scancode SCANCODE_9 = SDL_SCANCODE_9;

constexpr Scancode SCANCODE_0 = SDL_SCANCODE_0;

constexpr Scancode SCANCODE_RETURN = SDL_SCANCODE_RETURN;

constexpr Scancode SCANCODE_ESCAPE = SDL_SCANCODE_ESCAPE;

constexpr Scancode SCANCODE_BACKSPACE = SDL_SCANCODE_BACKSPACE;

constexpr Scancode SCANCODE_TAB = SDL_SCANCODE_TAB;

constexpr Scancode SCANCODE_SPACE = SDL_SCANCODE_SPACE;

constexpr Scancode SCANCODE_MINUS = SDL_SCANCODE_MINUS;

constexpr Scancode SCANCODE_EQUALS = SDL_SCANCODE_EQUALS;

constexpr Scancode SCANCODE_LEFTBRACKET = SDL_SCANCODE_LEFTBRACKET;

constexpr Scancode SCANCODE_RIGHTBRACKET = SDL_SCANCODE_RIGHTBRACKET;

/**
 * Located at the lower left of the return key on ISO keyboards and at the right
 * end of the QWERTY row on ANSI keyboards.  Produces REVERSE SOLIDUS
 * (backslash) and VERTICAL LINE in a US layout, REVERSE SOLIDUS and VERTICAL
 * LINE in a UK Mac layout, NUMBER SIGN and TILDE in a UK Windows layout, DOLLAR
 * SIGN and POUND SIGN in a Swiss German layout, NUMBER SIGN and APOSTROPHE in a
 * German layout, GRAVE ACCENT and POUND SIGN in a French Mac layout, and
 * ASTERISK and MICRO SIGN in a French Windows layout.
 */
constexpr Scancode SCANCODE_BACKSLASH = SDL_SCANCODE_BACKSLASH;

/**
 * ISO USB keyboards actually use this code instead of 49 for the same key, but
 * all OSes I've seen treat the two codes identically.  So, as an implementor,
 * unless your keyboard generates both of those codes and your OS treats them
 * differently, you should generate SCANCODE_BACKSLASH instead of this code. As
 * a user, you should not rely on this code because SDL will never generate it
 * with most (all?) keyboards.
 */
constexpr Scancode SCANCODE_NONUSHASH = SDL_SCANCODE_NONUSHASH;

constexpr Scancode SCANCODE_SEMICOLON = SDL_SCANCODE_SEMICOLON;

constexpr Scancode SCANCODE_APOSTROPHE = SDL_SCANCODE_APOSTROPHE;

/**
 * Located in the top left corner (on both ANSI and ISO keyboards).  Produces
 * GRAVE ACCENT and TILDE in a US Windows layout and in US and UK Mac layouts on
 * ANSI keyboards, GRAVE ACCENT and NOT SIGN in a UK Windows layout, SECTION
 * SIGN and PLUS-MINUS SIGN in US and UK Mac layouts on ISO keyboards, SECTION
 * SIGN and DEGREE SIGN in a Swiss German layout (Mac: only on ISO keyboards),
 * CIRCUMFLEX ACCENT and DEGREE SIGN in a German layout (Mac: only on ISO
 * keyboards), SUPERSCRIPT TWO and TILDE in a French Windows layout, COMMERCIAL
 * AT and NUMBER SIGN in a French Mac layout on ISO keyboards, and LESS-THAN
 * SIGN and GREATER-THAN SIGN in a Swiss German, German, or French Mac layout on
 * ANSI keyboards.
 */
constexpr Scancode SCANCODE_GRAVE = SDL_SCANCODE_GRAVE;

constexpr Scancode SCANCODE_COMMA = SDL_SCANCODE_COMMA;

constexpr Scancode SCANCODE_PERIOD = SDL_SCANCODE_PERIOD;

constexpr Scancode SCANCODE_SLASH = SDL_SCANCODE_SLASH;

constexpr Scancode SCANCODE_CAPSLOCK = SDL_SCANCODE_CAPSLOCK;

constexpr Scancode SCANCODE_F1 = SDL_SCANCODE_F1;

constexpr Scancode SCANCODE_F2 = SDL_SCANCODE_F2;

constexpr Scancode SCANCODE_F3 = SDL_SCANCODE_F3;

constexpr Scancode SCANCODE_F4 = SDL_SCANCODE_F4;

constexpr Scancode SCANCODE_F5 = SDL_SCANCODE_F5;

constexpr Scancode SCANCODE_F6 = SDL_SCANCODE_F6;

constexpr Scancode SCANCODE_F7 = SDL_SCANCODE_F7;

constexpr Scancode SCANCODE_F8 = SDL_SCANCODE_F8;

constexpr Scancode SCANCODE_F9 = SDL_SCANCODE_F9;

constexpr Scancode SCANCODE_F10 = SDL_SCANCODE_F10;

constexpr Scancode SCANCODE_F11 = SDL_SCANCODE_F11;

constexpr Scancode SCANCODE_F12 = SDL_SCANCODE_F12;

constexpr Scancode SCANCODE_PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN;

constexpr Scancode SCANCODE_SCROLLLOCK = SDL_SCANCODE_SCROLLLOCK;

constexpr Scancode SCANCODE_PAUSE = SDL_SCANCODE_PAUSE;

/**
 * insert on PC, help on some Mac keyboards (but does send code 73, not 117)
 */
constexpr Scancode SCANCODE_INSERT = SDL_SCANCODE_INSERT;

constexpr Scancode SCANCODE_HOME = SDL_SCANCODE_HOME;

constexpr Scancode SCANCODE_PAGEUP = SDL_SCANCODE_PAGEUP;

constexpr Scancode SCANCODE_DELETE = SDL_SCANCODE_DELETE;

constexpr Scancode SCANCODE_END = SDL_SCANCODE_END;

constexpr Scancode SCANCODE_PAGEDOWN = SDL_SCANCODE_PAGEDOWN;

constexpr Scancode SCANCODE_RIGHT = SDL_SCANCODE_RIGHT;

constexpr Scancode SCANCODE_LEFT = SDL_SCANCODE_LEFT;

constexpr Scancode SCANCODE_DOWN = SDL_SCANCODE_DOWN;

constexpr Scancode SCANCODE_UP = SDL_SCANCODE_UP;

/**
 * num lock on PC, clear on Mac keyboards
 */
constexpr Scancode SCANCODE_NUMLOCKCLEAR = SDL_SCANCODE_NUMLOCKCLEAR;

constexpr Scancode SCANCODE_KP_DIVIDE = SDL_SCANCODE_KP_DIVIDE;

constexpr Scancode SCANCODE_KP_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY;

constexpr Scancode SCANCODE_KP_MINUS = SDL_SCANCODE_KP_MINUS;

constexpr Scancode SCANCODE_KP_PLUS = SDL_SCANCODE_KP_PLUS;

constexpr Scancode SCANCODE_KP_ENTER = SDL_SCANCODE_KP_ENTER;

constexpr Scancode SCANCODE_KP_1 = SDL_SCANCODE_KP_1;

constexpr Scancode SCANCODE_KP_2 = SDL_SCANCODE_KP_2;

constexpr Scancode SCANCODE_KP_3 = SDL_SCANCODE_KP_3;

constexpr Scancode SCANCODE_KP_4 = SDL_SCANCODE_KP_4;

constexpr Scancode SCANCODE_KP_5 = SDL_SCANCODE_KP_5;

constexpr Scancode SCANCODE_KP_6 = SDL_SCANCODE_KP_6;

constexpr Scancode SCANCODE_KP_7 = SDL_SCANCODE_KP_7;

constexpr Scancode SCANCODE_KP_8 = SDL_SCANCODE_KP_8;

constexpr Scancode SCANCODE_KP_9 = SDL_SCANCODE_KP_9;

constexpr Scancode SCANCODE_KP_0 = SDL_SCANCODE_KP_0;

constexpr Scancode SCANCODE_KP_PERIOD = SDL_SCANCODE_KP_PERIOD;

/**
 * This is the additional key that ISO keyboards have over ANSI ones, located
 * between left shift and Y.  Produces GRAVE ACCENT and TILDE in a US or UK Mac
 * layout, REVERSE SOLIDUS (backslash) and VERTICAL LINE in a US or UK Windows
 * layout, and LESS-THAN SIGN and GREATER-THAN SIGN in a Swiss German, German,
 * or French layout.
 */
constexpr Scancode SCANCODE_NONUSBACKSLASH = SDL_SCANCODE_NONUSBACKSLASH;

/**
 * windows contextual menu, compose
 */
constexpr Scancode SCANCODE_APPLICATION = SDL_SCANCODE_APPLICATION;

/**
 * The USB document says this is a status flag, not a physical key - but some
 * Mac keyboards do have a power key.
 */
constexpr Scancode SCANCODE_POWER = SDL_SCANCODE_POWER;

constexpr Scancode SCANCODE_KP_EQUALS = SDL_SCANCODE_KP_EQUALS;

constexpr Scancode SCANCODE_F13 = SDL_SCANCODE_F13;

constexpr Scancode SCANCODE_F14 = SDL_SCANCODE_F14;

constexpr Scancode SCANCODE_F15 = SDL_SCANCODE_F15;

constexpr Scancode SCANCODE_F16 = SDL_SCANCODE_F16;

constexpr Scancode SCANCODE_F17 = SDL_SCANCODE_F17;

constexpr Scancode SCANCODE_F18 = SDL_SCANCODE_F18;

constexpr Scancode SCANCODE_F19 = SDL_SCANCODE_F19;

constexpr Scancode SCANCODE_F20 = SDL_SCANCODE_F20;

constexpr Scancode SCANCODE_F21 = SDL_SCANCODE_F21;

constexpr Scancode SCANCODE_F22 = SDL_SCANCODE_F22;

constexpr Scancode SCANCODE_F23 = SDL_SCANCODE_F23;

constexpr Scancode SCANCODE_F24 = SDL_SCANCODE_F24;

constexpr Scancode SCANCODE_EXECUTE = SDL_SCANCODE_EXECUTE;

constexpr Scancode SCANCODE_HELP =
  SDL_SCANCODE_HELP; ///< AL Integrated Help Center.

constexpr Scancode SCANCODE_MENU = SDL_SCANCODE_MENU; ///< Menu (show menu)

constexpr Scancode SCANCODE_SELECT = SDL_SCANCODE_SELECT;

constexpr Scancode SCANCODE_STOP = SDL_SCANCODE_STOP; ///< AC Stop.

constexpr Scancode SCANCODE_AGAIN = SDL_SCANCODE_AGAIN; ///< AC Redo/Repeat.

constexpr Scancode SCANCODE_UNDO = SDL_SCANCODE_UNDO; ///< AC Undo.

constexpr Scancode SCANCODE_CUT = SDL_SCANCODE_CUT; ///< AC Cut.

constexpr Scancode SCANCODE_COPY = SDL_SCANCODE_COPY; ///< AC Copy.

constexpr Scancode SCANCODE_PASTE = SDL_SCANCODE_PASTE; ///< AC Paste.

constexpr Scancode SCANCODE_FIND = SDL_SCANCODE_FIND; ///< AC Find.

constexpr Scancode SCANCODE_MUTE = SDL_SCANCODE_MUTE;

constexpr Scancode SCANCODE_VOLUMEUP = SDL_SCANCODE_VOLUMEUP;

constexpr Scancode SCANCODE_VOLUMEDOWN = SDL_SCANCODE_VOLUMEDOWN;

constexpr Scancode SCANCODE_KP_COMMA = SDL_SCANCODE_KP_COMMA;

constexpr Scancode SCANCODE_KP_EQUALSAS400 = SDL_SCANCODE_KP_EQUALSAS400;

/**
 * used on Asian keyboards, see footnotes in USB doc
 */
constexpr Scancode SCANCODE_INTERNATIONAL1 = SDL_SCANCODE_INTERNATIONAL1;

constexpr Scancode SCANCODE_INTERNATIONAL2 = SDL_SCANCODE_INTERNATIONAL2;

constexpr Scancode SCANCODE_INTERNATIONAL3 =
  SDL_SCANCODE_INTERNATIONAL3; ///< Yen.

constexpr Scancode SCANCODE_INTERNATIONAL4 = SDL_SCANCODE_INTERNATIONAL4;

constexpr Scancode SCANCODE_INTERNATIONAL5 = SDL_SCANCODE_INTERNATIONAL5;

constexpr Scancode SCANCODE_INTERNATIONAL6 = SDL_SCANCODE_INTERNATIONAL6;

constexpr Scancode SCANCODE_INTERNATIONAL7 = SDL_SCANCODE_INTERNATIONAL7;

constexpr Scancode SCANCODE_INTERNATIONAL8 = SDL_SCANCODE_INTERNATIONAL8;

constexpr Scancode SCANCODE_INTERNATIONAL9 = SDL_SCANCODE_INTERNATIONAL9;

constexpr Scancode SCANCODE_LANG1 =
  SDL_SCANCODE_LANG1; ///< Hangul/English toggle.

constexpr Scancode SCANCODE_LANG2 = SDL_SCANCODE_LANG2; ///< Hanja conversion.

constexpr Scancode SCANCODE_LANG3 = SDL_SCANCODE_LANG3; ///< Katakana.

constexpr Scancode SCANCODE_LANG4 = SDL_SCANCODE_LANG4; ///< Hiragana.

constexpr Scancode SCANCODE_LANG5 = SDL_SCANCODE_LANG5; ///< Zenkaku/Hankaku.

constexpr Scancode SCANCODE_LANG6 = SDL_SCANCODE_LANG6; ///< reserved

constexpr Scancode SCANCODE_LANG7 = SDL_SCANCODE_LANG7; ///< reserved

constexpr Scancode SCANCODE_LANG8 = SDL_SCANCODE_LANG8; ///< reserved

constexpr Scancode SCANCODE_LANG9 = SDL_SCANCODE_LANG9; ///< reserved

constexpr Scancode SCANCODE_ALTERASE = SDL_SCANCODE_ALTERASE; ///< Erase-Eaze.

constexpr Scancode SCANCODE_SYSREQ = SDL_SCANCODE_SYSREQ;

constexpr Scancode SCANCODE_CANCEL = SDL_SCANCODE_CANCEL; ///< AC Cancel.

constexpr Scancode SCANCODE_CLEAR = SDL_SCANCODE_CLEAR;

constexpr Scancode SCANCODE_PRIOR = SDL_SCANCODE_PRIOR;

constexpr Scancode SCANCODE_RETURN2 = SDL_SCANCODE_RETURN2;

constexpr Scancode SCANCODE_SEPARATOR = SDL_SCANCODE_SEPARATOR;

constexpr Scancode SCANCODE_OUT = SDL_SCANCODE_OUT;

constexpr Scancode SCANCODE_OPER = SDL_SCANCODE_OPER;

constexpr Scancode SCANCODE_CLEARAGAIN = SDL_SCANCODE_CLEARAGAIN;

constexpr Scancode SCANCODE_CRSEL = SDL_SCANCODE_CRSEL;

constexpr Scancode SCANCODE_EXSEL = SDL_SCANCODE_EXSEL;

constexpr Scancode SCANCODE_KP_00 = SDL_SCANCODE_KP_00;

constexpr Scancode SCANCODE_KP_000 = SDL_SCANCODE_KP_000;

constexpr Scancode SCANCODE_THOUSANDSSEPARATOR =
  SDL_SCANCODE_THOUSANDSSEPARATOR;

constexpr Scancode SCANCODE_DECIMALSEPARATOR = SDL_SCANCODE_DECIMALSEPARATOR;

constexpr Scancode SCANCODE_CURRENCYUNIT = SDL_SCANCODE_CURRENCYUNIT;

constexpr Scancode SCANCODE_CURRENCYSUBUNIT = SDL_SCANCODE_CURRENCYSUBUNIT;

constexpr Scancode SCANCODE_KP_LEFTPAREN = SDL_SCANCODE_KP_LEFTPAREN;

constexpr Scancode SCANCODE_KP_RIGHTPAREN = SDL_SCANCODE_KP_RIGHTPAREN;

constexpr Scancode SCANCODE_KP_LEFTBRACE = SDL_SCANCODE_KP_LEFTBRACE;

constexpr Scancode SCANCODE_KP_RIGHTBRACE = SDL_SCANCODE_KP_RIGHTBRACE;

constexpr Scancode SCANCODE_KP_TAB = SDL_SCANCODE_KP_TAB;

constexpr Scancode SCANCODE_KP_BACKSPACE = SDL_SCANCODE_KP_BACKSPACE;

constexpr Scancode SCANCODE_KP_A = SDL_SCANCODE_KP_A;

constexpr Scancode SCANCODE_KP_B = SDL_SCANCODE_KP_B;

constexpr Scancode SCANCODE_KP_C = SDL_SCANCODE_KP_C;

constexpr Scancode SCANCODE_KP_D = SDL_SCANCODE_KP_D;

constexpr Scancode SCANCODE_KP_E = SDL_SCANCODE_KP_E;

constexpr Scancode SCANCODE_KP_F = SDL_SCANCODE_KP_F;

constexpr Scancode SCANCODE_KP_XOR = SDL_SCANCODE_KP_XOR;

constexpr Scancode SCANCODE_KP_POWER = SDL_SCANCODE_KP_POWER;

constexpr Scancode SCANCODE_KP_PERCENT = SDL_SCANCODE_KP_PERCENT;

constexpr Scancode SCANCODE_KP_LESS = SDL_SCANCODE_KP_LESS;

constexpr Scancode SCANCODE_KP_GREATER = SDL_SCANCODE_KP_GREATER;

constexpr Scancode SCANCODE_KP_AMPERSAND = SDL_SCANCODE_KP_AMPERSAND;

constexpr Scancode SCANCODE_KP_DBLAMPERSAND = SDL_SCANCODE_KP_DBLAMPERSAND;

constexpr Scancode SCANCODE_KP_VERTICALBAR = SDL_SCANCODE_KP_VERTICALBAR;

constexpr Scancode SCANCODE_KP_DBLVERTICALBAR = SDL_SCANCODE_KP_DBLVERTICALBAR;

constexpr Scancode SCANCODE_KP_COLON = SDL_SCANCODE_KP_COLON;

constexpr Scancode SCANCODE_KP_HASH = SDL_SCANCODE_KP_HASH;

constexpr Scancode SCANCODE_KP_SPACE = SDL_SCANCODE_KP_SPACE;

constexpr Scancode SCANCODE_KP_AT = SDL_SCANCODE_KP_AT;

constexpr Scancode SCANCODE_KP_EXCLAM = SDL_SCANCODE_KP_EXCLAM;

constexpr Scancode SCANCODE_KP_MEMSTORE = SDL_SCANCODE_KP_MEMSTORE;

constexpr Scancode SCANCODE_KP_MEMRECALL = SDL_SCANCODE_KP_MEMRECALL;

constexpr Scancode SCANCODE_KP_MEMCLEAR = SDL_SCANCODE_KP_MEMCLEAR;

constexpr Scancode SCANCODE_KP_MEMADD = SDL_SCANCODE_KP_MEMADD;

constexpr Scancode SCANCODE_KP_MEMSUBTRACT = SDL_SCANCODE_KP_MEMSUBTRACT;

constexpr Scancode SCANCODE_KP_MEMMULTIPLY = SDL_SCANCODE_KP_MEMMULTIPLY;

constexpr Scancode SCANCODE_KP_MEMDIVIDE = SDL_SCANCODE_KP_MEMDIVIDE;

constexpr Scancode SCANCODE_KP_PLUSMINUS = SDL_SCANCODE_KP_PLUSMINUS;

constexpr Scancode SCANCODE_KP_CLEAR = SDL_SCANCODE_KP_CLEAR;

constexpr Scancode SCANCODE_KP_CLEARENTRY = SDL_SCANCODE_KP_CLEARENTRY;

constexpr Scancode SCANCODE_KP_BINARY = SDL_SCANCODE_KP_BINARY;

constexpr Scancode SCANCODE_KP_OCTAL = SDL_SCANCODE_KP_OCTAL;

constexpr Scancode SCANCODE_KP_DECIMAL = SDL_SCANCODE_KP_DECIMAL;

constexpr Scancode SCANCODE_KP_HEXADECIMAL = SDL_SCANCODE_KP_HEXADECIMAL;

constexpr Scancode SCANCODE_LCTRL = SDL_SCANCODE_LCTRL;

constexpr Scancode SCANCODE_LSHIFT = SDL_SCANCODE_LSHIFT;

constexpr Scancode SCANCODE_LALT = SDL_SCANCODE_LALT; ///< alt, option

/**
 * windows, command (apple), meta
 */
constexpr Scancode SCANCODE_LGUI = SDL_SCANCODE_LGUI;

constexpr Scancode SCANCODE_RCTRL = SDL_SCANCODE_RCTRL;

constexpr Scancode SCANCODE_RSHIFT = SDL_SCANCODE_RSHIFT;

constexpr Scancode SCANCODE_RALT = SDL_SCANCODE_RALT; ///< alt gr, option

/**
 * windows, command (apple), meta
 */
constexpr Scancode SCANCODE_RGUI = SDL_SCANCODE_RGUI;

/**
 * I'm not sure if this is really not covered by any of the above, but since
 * there's a special KMOD_MODE for it I'm adding it here.
 */
constexpr Scancode SCANCODE_MODE = SDL_SCANCODE_MODE;

constexpr Scancode SCANCODE_SLEEP = SDL_SCANCODE_SLEEP; ///< Sleep.

constexpr Scancode SCANCODE_WAKE = SDL_SCANCODE_WAKE; ///< Wake.

/**
 * Channel Increment.
 */
constexpr Scancode SCANCODE_CHANNEL_INCREMENT = SDL_SCANCODE_CHANNEL_INCREMENT;

/**
 * Channel Decrement.
 */
constexpr Scancode SCANCODE_CHANNEL_DECREMENT = SDL_SCANCODE_CHANNEL_DECREMENT;

constexpr Scancode SCANCODE_MEDIA_PLAY = SDL_SCANCODE_MEDIA_PLAY; ///< Play.

constexpr Scancode SCANCODE_MEDIA_PAUSE = SDL_SCANCODE_MEDIA_PAUSE; ///< Pause.

constexpr Scancode SCANCODE_MEDIA_RECORD =
  SDL_SCANCODE_MEDIA_RECORD; ///< Record.

/**
 * Fast Forward.
 */
constexpr Scancode SCANCODE_MEDIA_FAST_FORWARD =
  SDL_SCANCODE_MEDIA_FAST_FORWARD;

constexpr Scancode SCANCODE_MEDIA_REWIND =
  SDL_SCANCODE_MEDIA_REWIND; ///< Rewind.

/**
 * Next Track.
 */
constexpr Scancode SCANCODE_MEDIA_NEXT_TRACK = SDL_SCANCODE_MEDIA_NEXT_TRACK;

/**
 * Previous Track.
 */
constexpr Scancode SCANCODE_MEDIA_PREVIOUS_TRACK =
  SDL_SCANCODE_MEDIA_PREVIOUS_TRACK;

constexpr Scancode SCANCODE_MEDIA_STOP = SDL_SCANCODE_MEDIA_STOP; ///< Stop.

constexpr Scancode SCANCODE_MEDIA_EJECT = SDL_SCANCODE_MEDIA_EJECT; ///< Eject.

/**
 * Play / Pause.
 */
constexpr Scancode SCANCODE_MEDIA_PLAY_PAUSE = SDL_SCANCODE_MEDIA_PLAY_PAUSE;

constexpr Scancode SCANCODE_MEDIA_SELECT = SDL_SCANCODE_MEDIA_SELECT;

constexpr Scancode SCANCODE_AC_NEW = SDL_SCANCODE_AC_NEW; ///< AC New.

constexpr Scancode SCANCODE_AC_OPEN = SDL_SCANCODE_AC_OPEN; ///< AC Open.

constexpr Scancode SCANCODE_AC_CLOSE = SDL_SCANCODE_AC_CLOSE; ///< AC Close.

constexpr Scancode SCANCODE_AC_EXIT = SDL_SCANCODE_AC_EXIT; ///< AC Exit.

constexpr Scancode SCANCODE_AC_SAVE = SDL_SCANCODE_AC_SAVE; ///< AC Save.

constexpr Scancode SCANCODE_AC_PRINT = SDL_SCANCODE_AC_PRINT; ///< AC Print.

/**
 * AC Properties.
 */
constexpr Scancode SCANCODE_AC_PROPERTIES = SDL_SCANCODE_AC_PROPERTIES;

constexpr Scancode SCANCODE_AC_SEARCH = SDL_SCANCODE_AC_SEARCH; ///< AC Search.

constexpr Scancode SCANCODE_AC_HOME = SDL_SCANCODE_AC_HOME; ///< AC Home.

constexpr Scancode SCANCODE_AC_BACK = SDL_SCANCODE_AC_BACK; ///< AC Back.

constexpr Scancode SCANCODE_AC_FORWARD =
  SDL_SCANCODE_AC_FORWARD; ///< AC Forward.

constexpr Scancode SCANCODE_AC_STOP = SDL_SCANCODE_AC_STOP; ///< AC Stop.

constexpr Scancode SCANCODE_AC_REFRESH =
  SDL_SCANCODE_AC_REFRESH; ///< AC Refresh.

/**
 * AC Bookmarks.
 */
constexpr Scancode SCANCODE_AC_BOOKMARKS = SDL_SCANCODE_AC_BOOKMARKS;

/**
 * Usually situated below the display on phones and used as a multi-function
 * feature key for selecting a software defined function shown on the bottom
 * left of the display.
 */
constexpr Scancode SCANCODE_SOFTLEFT = SDL_SCANCODE_SOFTLEFT;

/**
 * Usually situated below the display on phones and used as a multi-function
 * feature key for selecting a software defined function shown on the bottom
 * right of the display.
 */
constexpr Scancode SCANCODE_SOFTRIGHT = SDL_SCANCODE_SOFTRIGHT;

/**
 * Used for accepting phone calls.
 */
constexpr Scancode SCANCODE_CALL = SDL_SCANCODE_CALL;

/**
 * Used for rejecting phone calls.
 */
constexpr Scancode SCANCODE_ENDCALL = SDL_SCANCODE_ENDCALL;

/**
 * 400-500 reserved for dynamic keycodes
 */
constexpr Scancode SCANCODE_RESERVED = SDL_SCANCODE_RESERVED;

/**
 * not a key, just marks the number of scancodes for array bounds
 */
constexpr Scancode SCANCODE_COUNT = SDL_SCANCODE_COUNT;

/// @}
} // namespace SDL

#endif /* SDL3PP_SCANCODE_H_ */
