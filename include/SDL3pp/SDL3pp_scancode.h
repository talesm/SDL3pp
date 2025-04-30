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
class Scancode
{
  SDL_Scancode m_scancode;

public:
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

  /**
   * Default comparison operator
   */
  constexpr auto operator<=>(const Scancode& other) const = default;

  /**
   * Compares with the underlying type
   */
  constexpr auto operator<=>(SDL_Scancode scancode) const
  {
    return operator<=>(Scancode(scancode));
  }

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
  void SetName(StringParam name);

  // Get name
  const char* GetName() const;
};

constexpr SDL_Scancode SCANCODE_UNKNOWN = SDL_SCANCODE_UNKNOWN; ///< UNKNOWN

constexpr SDL_Scancode SCANCODE_A = SDL_SCANCODE_A; ///< A

constexpr SDL_Scancode SCANCODE_B = SDL_SCANCODE_B; ///< B

constexpr SDL_Scancode SCANCODE_C = SDL_SCANCODE_C; ///< C

constexpr SDL_Scancode SCANCODE_D = SDL_SCANCODE_D; ///< D

constexpr SDL_Scancode SCANCODE_E = SDL_SCANCODE_E; ///< E

constexpr SDL_Scancode SCANCODE_F = SDL_SCANCODE_F; ///< F

constexpr SDL_Scancode SCANCODE_G = SDL_SCANCODE_G; ///< G

constexpr SDL_Scancode SCANCODE_H = SDL_SCANCODE_H; ///< H

constexpr SDL_Scancode SCANCODE_I = SDL_SCANCODE_I; ///< I

constexpr SDL_Scancode SCANCODE_J = SDL_SCANCODE_J; ///< J

constexpr SDL_Scancode SCANCODE_K = SDL_SCANCODE_K; ///< K

constexpr SDL_Scancode SCANCODE_L = SDL_SCANCODE_L; ///< L

constexpr SDL_Scancode SCANCODE_M = SDL_SCANCODE_M; ///< M

constexpr SDL_Scancode SCANCODE_N = SDL_SCANCODE_N; ///< N

constexpr SDL_Scancode SCANCODE_O = SDL_SCANCODE_O; ///< O

constexpr SDL_Scancode SCANCODE_P = SDL_SCANCODE_P; ///< P

constexpr SDL_Scancode SCANCODE_Q = SDL_SCANCODE_Q; ///< Q

constexpr SDL_Scancode SCANCODE_R = SDL_SCANCODE_R; ///< R

constexpr SDL_Scancode SCANCODE_S = SDL_SCANCODE_S; ///< S

constexpr SDL_Scancode SCANCODE_T = SDL_SCANCODE_T; ///< T

constexpr SDL_Scancode SCANCODE_U = SDL_SCANCODE_U; ///< U

constexpr SDL_Scancode SCANCODE_V = SDL_SCANCODE_V; ///< V

constexpr SDL_Scancode SCANCODE_W = SDL_SCANCODE_W; ///< W

constexpr SDL_Scancode SCANCODE_X = SDL_SCANCODE_X; ///< X

constexpr SDL_Scancode SCANCODE_Y = SDL_SCANCODE_Y; ///< Y

constexpr SDL_Scancode SCANCODE_Z = SDL_SCANCODE_Z; ///< Z

constexpr SDL_Scancode SCANCODE_1 = SDL_SCANCODE_1; ///< 1

constexpr SDL_Scancode SCANCODE_2 = SDL_SCANCODE_2; ///< 2

constexpr SDL_Scancode SCANCODE_3 = SDL_SCANCODE_3; ///< 3

constexpr SDL_Scancode SCANCODE_4 = SDL_SCANCODE_4; ///< 4

constexpr SDL_Scancode SCANCODE_5 = SDL_SCANCODE_5; ///< 5

constexpr SDL_Scancode SCANCODE_6 = SDL_SCANCODE_6; ///< 6

constexpr SDL_Scancode SCANCODE_7 = SDL_SCANCODE_7; ///< 7

constexpr SDL_Scancode SCANCODE_8 = SDL_SCANCODE_8; ///< 8

constexpr SDL_Scancode SCANCODE_9 = SDL_SCANCODE_9; ///< 9

constexpr SDL_Scancode SCANCODE_0 = SDL_SCANCODE_0; ///< 0

constexpr SDL_Scancode SCANCODE_RETURN = SDL_SCANCODE_RETURN; ///< RETURN

constexpr SDL_Scancode SCANCODE_ESCAPE = SDL_SCANCODE_ESCAPE; ///< ESCAPE

constexpr SDL_Scancode SCANCODE_BACKSPACE =
  SDL_SCANCODE_BACKSPACE; ///< BACKSPACE

constexpr SDL_Scancode SCANCODE_TAB = SDL_SCANCODE_TAB; ///< TAB

constexpr SDL_Scancode SCANCODE_SPACE = SDL_SCANCODE_SPACE; ///< SPACE

constexpr SDL_Scancode SCANCODE_MINUS = SDL_SCANCODE_MINUS; ///< MINUS

constexpr SDL_Scancode SCANCODE_EQUALS = SDL_SCANCODE_EQUALS; ///< EQUALS

constexpr SDL_Scancode SCANCODE_LEFTBRACKET =
  SDL_SCANCODE_LEFTBRACKET; ///< LEFTBRACKET

constexpr SDL_Scancode SCANCODE_RIGHTBRACKET =
  SDL_SCANCODE_RIGHTBRACKET; ///< RIGHTBRACKET

/**
 * Located at the lower left of the return key on ISO keyboards and at the right
 * end of the QWERTY row on ANSI keyboards.  Produces REVERSE SOLIDUS
 * (backslash) and VERTICAL LINE in a US layout, REVERSE SOLIDUS and VERTICAL
 * LINE in a UK Mac layout, NUMBER SIGN and TILDE in a UK Windows layout, DOLLAR
 * SIGN and POUND SIGN in a Swiss German layout, NUMBER SIGN and APOSTROPHE in a
 * German layout, GRAVE ACCENT and POUND SIGN in a French Mac layout, and
 * ASTERISK and MICRO SIGN in a French Windows layout.
 */
constexpr SDL_Scancode SCANCODE_BACKSLASH = SDL_SCANCODE_BACKSLASH;

/**
 * ISO USB keyboards actually use this code instead of 49 for the same key, but
 * all OSes I've seen treat the two codes identically.  So, as an implementor,
 * unless your keyboard generates both of those codes and your OS treats them
 * differently, you should generate SCANCODE_BACKSLASH instead of this code. As
 * a user, you should not rely on this code because SDL will never generate it
 * with most (all?) keyboards.
 */
constexpr SDL_Scancode SCANCODE_NONUSHASH = SDL_SCANCODE_NONUSHASH;

constexpr SDL_Scancode SCANCODE_SEMICOLON =
  SDL_SCANCODE_SEMICOLON; ///< SEMICOLON

constexpr SDL_Scancode SCANCODE_APOSTROPHE =
  SDL_SCANCODE_APOSTROPHE; ///< APOSTROPHE

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
constexpr SDL_Scancode SCANCODE_GRAVE = SDL_SCANCODE_GRAVE;

constexpr SDL_Scancode SCANCODE_COMMA = SDL_SCANCODE_COMMA; ///< COMMA

constexpr SDL_Scancode SCANCODE_PERIOD = SDL_SCANCODE_PERIOD; ///< PERIOD

constexpr SDL_Scancode SCANCODE_SLASH = SDL_SCANCODE_SLASH; ///< SLASH

constexpr SDL_Scancode SCANCODE_CAPSLOCK = SDL_SCANCODE_CAPSLOCK; ///< CAPSLOCK

constexpr SDL_Scancode SCANCODE_F1 = SDL_SCANCODE_F1; ///< F1

constexpr SDL_Scancode SCANCODE_F2 = SDL_SCANCODE_F2; ///< F2

constexpr SDL_Scancode SCANCODE_F3 = SDL_SCANCODE_F3; ///< F3

constexpr SDL_Scancode SCANCODE_F4 = SDL_SCANCODE_F4; ///< F4

constexpr SDL_Scancode SCANCODE_F5 = SDL_SCANCODE_F5; ///< F5

constexpr SDL_Scancode SCANCODE_F6 = SDL_SCANCODE_F6; ///< F6

constexpr SDL_Scancode SCANCODE_F7 = SDL_SCANCODE_F7; ///< F7

constexpr SDL_Scancode SCANCODE_F8 = SDL_SCANCODE_F8; ///< F8

constexpr SDL_Scancode SCANCODE_F9 = SDL_SCANCODE_F9; ///< F9

constexpr SDL_Scancode SCANCODE_F10 = SDL_SCANCODE_F10; ///< F10

constexpr SDL_Scancode SCANCODE_F11 = SDL_SCANCODE_F11; ///< F11

constexpr SDL_Scancode SCANCODE_F12 = SDL_SCANCODE_F12; ///< F12

constexpr SDL_Scancode SCANCODE_PRINTSCREEN =
  SDL_SCANCODE_PRINTSCREEN; ///< PRINTSCREEN

constexpr SDL_Scancode SCANCODE_SCROLLLOCK =
  SDL_SCANCODE_SCROLLLOCK; ///< SCROLLLOCK

constexpr SDL_Scancode SCANCODE_PAUSE = SDL_SCANCODE_PAUSE; ///< PAUSE

/**
 * insert on PC, help on some Mac keyboards (but does send code 73, not 117)
 */
constexpr SDL_Scancode SCANCODE_INSERT = SDL_SCANCODE_INSERT;

constexpr SDL_Scancode SCANCODE_HOME = SDL_SCANCODE_HOME; ///< HOME

constexpr SDL_Scancode SCANCODE_PAGEUP = SDL_SCANCODE_PAGEUP; ///< PAGEUP

constexpr SDL_Scancode SCANCODE_DELETE = SDL_SCANCODE_DELETE; ///< DELETE

constexpr SDL_Scancode SCANCODE_END = SDL_SCANCODE_END; ///< END

constexpr SDL_Scancode SCANCODE_PAGEDOWN = SDL_SCANCODE_PAGEDOWN; ///< PAGEDOWN

constexpr SDL_Scancode SCANCODE_RIGHT = SDL_SCANCODE_RIGHT; ///< RIGHT

constexpr SDL_Scancode SCANCODE_LEFT = SDL_SCANCODE_LEFT; ///< LEFT

constexpr SDL_Scancode SCANCODE_DOWN = SDL_SCANCODE_DOWN; ///< DOWN

constexpr SDL_Scancode SCANCODE_UP = SDL_SCANCODE_UP; ///< UP

constexpr SDL_Scancode SCANCODE_NUMLOCKCLEAR =
  SDL_SCANCODE_NUMLOCKCLEAR; ///< num lock on PC, clear on Mac keyboards

constexpr SDL_Scancode SCANCODE_KP_DIVIDE =
  SDL_SCANCODE_KP_DIVIDE; ///< KP_DIVIDE

constexpr SDL_Scancode SCANCODE_KP_MULTIPLY =
  SDL_SCANCODE_KP_MULTIPLY; ///< KP_MULTIPLY

constexpr SDL_Scancode SCANCODE_KP_MINUS = SDL_SCANCODE_KP_MINUS; ///< KP_MINUS

constexpr SDL_Scancode SCANCODE_KP_PLUS = SDL_SCANCODE_KP_PLUS; ///< KP_PLUS

constexpr SDL_Scancode SCANCODE_KP_ENTER = SDL_SCANCODE_KP_ENTER; ///< KP_ENTER

constexpr SDL_Scancode SCANCODE_KP_1 = SDL_SCANCODE_KP_1; ///< KP_1

constexpr SDL_Scancode SCANCODE_KP_2 = SDL_SCANCODE_KP_2; ///< KP_2

constexpr SDL_Scancode SCANCODE_KP_3 = SDL_SCANCODE_KP_3; ///< KP_3

constexpr SDL_Scancode SCANCODE_KP_4 = SDL_SCANCODE_KP_4; ///< KP_4

constexpr SDL_Scancode SCANCODE_KP_5 = SDL_SCANCODE_KP_5; ///< KP_5

constexpr SDL_Scancode SCANCODE_KP_6 = SDL_SCANCODE_KP_6; ///< KP_6

constexpr SDL_Scancode SCANCODE_KP_7 = SDL_SCANCODE_KP_7; ///< KP_7

constexpr SDL_Scancode SCANCODE_KP_8 = SDL_SCANCODE_KP_8; ///< KP_8

constexpr SDL_Scancode SCANCODE_KP_9 = SDL_SCANCODE_KP_9; ///< KP_9

constexpr SDL_Scancode SCANCODE_KP_0 = SDL_SCANCODE_KP_0; ///< KP_0

constexpr SDL_Scancode SCANCODE_KP_PERIOD =
  SDL_SCANCODE_KP_PERIOD; ///< KP_PERIOD

/**
 * This is the additional key that ISO keyboards have over ANSI ones, located
 * between left shift and Y.  Produces GRAVE ACCENT and TILDE in a US or UK Mac
 * layout, REVERSE SOLIDUS (backslash) and VERTICAL LINE in a US or UK Windows
 * layout, and LESS-THAN SIGN and GREATER-THAN SIGN in a Swiss German, German,
 * or French layout.
 */
constexpr SDL_Scancode SCANCODE_NONUSBACKSLASH = SDL_SCANCODE_NONUSBACKSLASH;

constexpr SDL_Scancode SCANCODE_APPLICATION =
  SDL_SCANCODE_APPLICATION; ///< windows contextual menu, compose

/**
 * The USB document says this is a status flag, not a physical key - but some
 * Mac keyboards do have a power key.
 */
constexpr SDL_Scancode SCANCODE_POWER = SDL_SCANCODE_POWER;

constexpr SDL_Scancode SCANCODE_KP_EQUALS =
  SDL_SCANCODE_KP_EQUALS; ///< KP_EQUALS

constexpr SDL_Scancode SCANCODE_F13 = SDL_SCANCODE_F13; ///< F13

constexpr SDL_Scancode SCANCODE_F14 = SDL_SCANCODE_F14; ///< F14

constexpr SDL_Scancode SCANCODE_F15 = SDL_SCANCODE_F15; ///< F15

constexpr SDL_Scancode SCANCODE_F16 = SDL_SCANCODE_F16; ///< F16

constexpr SDL_Scancode SCANCODE_F17 = SDL_SCANCODE_F17; ///< F17

constexpr SDL_Scancode SCANCODE_F18 = SDL_SCANCODE_F18; ///< F18

constexpr SDL_Scancode SCANCODE_F19 = SDL_SCANCODE_F19; ///< F19

constexpr SDL_Scancode SCANCODE_F20 = SDL_SCANCODE_F20; ///< F20

constexpr SDL_Scancode SCANCODE_F21 = SDL_SCANCODE_F21; ///< F21

constexpr SDL_Scancode SCANCODE_F22 = SDL_SCANCODE_F22; ///< F22

constexpr SDL_Scancode SCANCODE_F23 = SDL_SCANCODE_F23; ///< F23

constexpr SDL_Scancode SCANCODE_F24 = SDL_SCANCODE_F24; ///< F24

constexpr SDL_Scancode SCANCODE_EXECUTE = SDL_SCANCODE_EXECUTE; ///< EXECUTE

constexpr SDL_Scancode SCANCODE_HELP =
  SDL_SCANCODE_HELP; ///< AL Integrated Help Center.

constexpr SDL_Scancode SCANCODE_MENU = SDL_SCANCODE_MENU; ///< Menu (show menu)

constexpr SDL_Scancode SCANCODE_SELECT = SDL_SCANCODE_SELECT; ///< SELECT

constexpr SDL_Scancode SCANCODE_STOP = SDL_SCANCODE_STOP; ///< AC Stop.

constexpr SDL_Scancode SCANCODE_AGAIN = SDL_SCANCODE_AGAIN; ///< AC Redo/Repeat.

constexpr SDL_Scancode SCANCODE_UNDO = SDL_SCANCODE_UNDO; ///< AC Undo.

constexpr SDL_Scancode SCANCODE_CUT = SDL_SCANCODE_CUT; ///< AC Cut.

constexpr SDL_Scancode SCANCODE_COPY = SDL_SCANCODE_COPY; ///< AC Copy.

constexpr SDL_Scancode SCANCODE_PASTE = SDL_SCANCODE_PASTE; ///< AC Paste.

constexpr SDL_Scancode SCANCODE_FIND = SDL_SCANCODE_FIND; ///< AC Find.

constexpr SDL_Scancode SCANCODE_MUTE = SDL_SCANCODE_MUTE; ///< MUTE

constexpr SDL_Scancode SCANCODE_VOLUMEUP = SDL_SCANCODE_VOLUMEUP; ///< VOLUMEUP

constexpr SDL_Scancode SCANCODE_VOLUMEDOWN =
  SDL_SCANCODE_VOLUMEDOWN; ///< VOLUMEDOWN

constexpr SDL_Scancode SCANCODE_KP_COMMA = SDL_SCANCODE_KP_COMMA; ///< KP_COMMA

constexpr SDL_Scancode SCANCODE_KP_EQUALSAS400 =
  SDL_SCANCODE_KP_EQUALSAS400; ///< KP_EQUALSAS400

constexpr SDL_Scancode SCANCODE_INTERNATIONAL1 =
  SDL_SCANCODE_INTERNATIONAL1; ///< used on Asian keyboards, see footnotes in
                               ///< USB doc

constexpr SDL_Scancode SCANCODE_INTERNATIONAL2 =
  SDL_SCANCODE_INTERNATIONAL2; ///< INTERNATIONAL2

constexpr SDL_Scancode SCANCODE_INTERNATIONAL3 =
  SDL_SCANCODE_INTERNATIONAL3; ///< Yen.

constexpr SDL_Scancode SCANCODE_INTERNATIONAL4 =
  SDL_SCANCODE_INTERNATIONAL4; ///< INTERNATIONAL4

constexpr SDL_Scancode SCANCODE_INTERNATIONAL5 =
  SDL_SCANCODE_INTERNATIONAL5; ///< INTERNATIONAL5

constexpr SDL_Scancode SCANCODE_INTERNATIONAL6 =
  SDL_SCANCODE_INTERNATIONAL6; ///< INTERNATIONAL6

constexpr SDL_Scancode SCANCODE_INTERNATIONAL7 =
  SDL_SCANCODE_INTERNATIONAL7; ///< INTERNATIONAL7

constexpr SDL_Scancode SCANCODE_INTERNATIONAL8 =
  SDL_SCANCODE_INTERNATIONAL8; ///< INTERNATIONAL8

constexpr SDL_Scancode SCANCODE_INTERNATIONAL9 =
  SDL_SCANCODE_INTERNATIONAL9; ///< INTERNATIONAL9

constexpr SDL_Scancode SCANCODE_LANG1 =
  SDL_SCANCODE_LANG1; ///< Hangul/English toggle.

constexpr SDL_Scancode SCANCODE_LANG2 =
  SDL_SCANCODE_LANG2; ///< Hanja conversion.

constexpr SDL_Scancode SCANCODE_LANG3 = SDL_SCANCODE_LANG3; ///< Katakana.

constexpr SDL_Scancode SCANCODE_LANG4 = SDL_SCANCODE_LANG4; ///< Hiragana.

constexpr SDL_Scancode SCANCODE_LANG5 =
  SDL_SCANCODE_LANG5; ///< Zenkaku/Hankaku.

constexpr SDL_Scancode SCANCODE_LANG6 = SDL_SCANCODE_LANG6; ///< reserved

constexpr SDL_Scancode SCANCODE_LANG7 = SDL_SCANCODE_LANG7; ///< reserved

constexpr SDL_Scancode SCANCODE_LANG8 = SDL_SCANCODE_LANG8; ///< reserved

constexpr SDL_Scancode SCANCODE_LANG9 = SDL_SCANCODE_LANG9; ///< reserved

constexpr SDL_Scancode SCANCODE_ALTERASE =
  SDL_SCANCODE_ALTERASE; ///< Erase-Eaze.

constexpr SDL_Scancode SCANCODE_SYSREQ = SDL_SCANCODE_SYSREQ; ///< SYSREQ

constexpr SDL_Scancode SCANCODE_CANCEL = SDL_SCANCODE_CANCEL; ///< AC Cancel.

constexpr SDL_Scancode SCANCODE_CLEAR = SDL_SCANCODE_CLEAR; ///< CLEAR

constexpr SDL_Scancode SCANCODE_PRIOR = SDL_SCANCODE_PRIOR; ///< PRIOR

constexpr SDL_Scancode SCANCODE_RETURN2 = SDL_SCANCODE_RETURN2; ///< RETURN2

constexpr SDL_Scancode SCANCODE_SEPARATOR =
  SDL_SCANCODE_SEPARATOR; ///< SEPARATOR

constexpr SDL_Scancode SCANCODE_OUT = SDL_SCANCODE_OUT; ///< OUT

constexpr SDL_Scancode SCANCODE_OPER = SDL_SCANCODE_OPER; ///< OPER

constexpr SDL_Scancode SCANCODE_CLEARAGAIN =
  SDL_SCANCODE_CLEARAGAIN; ///< CLEARAGAIN

constexpr SDL_Scancode SCANCODE_CRSEL = SDL_SCANCODE_CRSEL; ///< CRSEL

constexpr SDL_Scancode SCANCODE_EXSEL = SDL_SCANCODE_EXSEL; ///< EXSEL

constexpr SDL_Scancode SCANCODE_KP_00 = SDL_SCANCODE_KP_00; ///< KP_00

constexpr SDL_Scancode SCANCODE_KP_000 = SDL_SCANCODE_KP_000; ///< KP_000

constexpr SDL_Scancode SCANCODE_THOUSANDSSEPARATOR =
  SDL_SCANCODE_THOUSANDSSEPARATOR; ///< THOUSANDSSEPARATOR

constexpr SDL_Scancode SCANCODE_DECIMALSEPARATOR =
  SDL_SCANCODE_DECIMALSEPARATOR; ///< DECIMALSEPARATOR

constexpr SDL_Scancode SCANCODE_CURRENCYUNIT =
  SDL_SCANCODE_CURRENCYUNIT; ///< CURRENCYUNIT

constexpr SDL_Scancode SCANCODE_CURRENCYSUBUNIT =
  SDL_SCANCODE_CURRENCYSUBUNIT; ///< CURRENCYSUBUNIT

constexpr SDL_Scancode SCANCODE_KP_LEFTPAREN =
  SDL_SCANCODE_KP_LEFTPAREN; ///< KP_LEFTPAREN

constexpr SDL_Scancode SCANCODE_KP_RIGHTPAREN =
  SDL_SCANCODE_KP_RIGHTPAREN; ///< KP_RIGHTPAREN

constexpr SDL_Scancode SCANCODE_KP_LEFTBRACE =
  SDL_SCANCODE_KP_LEFTBRACE; ///< KP_LEFTBRACE

constexpr SDL_Scancode SCANCODE_KP_RIGHTBRACE =
  SDL_SCANCODE_KP_RIGHTBRACE; ///< KP_RIGHTBRACE

constexpr SDL_Scancode SCANCODE_KP_TAB = SDL_SCANCODE_KP_TAB; ///< KP_TAB

constexpr SDL_Scancode SCANCODE_KP_BACKSPACE =
  SDL_SCANCODE_KP_BACKSPACE; ///< KP_BACKSPACE

constexpr SDL_Scancode SCANCODE_KP_A = SDL_SCANCODE_KP_A; ///< KP_A

constexpr SDL_Scancode SCANCODE_KP_B = SDL_SCANCODE_KP_B; ///< KP_B

constexpr SDL_Scancode SCANCODE_KP_C = SDL_SCANCODE_KP_C; ///< KP_C

constexpr SDL_Scancode SCANCODE_KP_D = SDL_SCANCODE_KP_D; ///< KP_D

constexpr SDL_Scancode SCANCODE_KP_E = SDL_SCANCODE_KP_E; ///< KP_E

constexpr SDL_Scancode SCANCODE_KP_F = SDL_SCANCODE_KP_F; ///< KP_F

constexpr SDL_Scancode SCANCODE_KP_XOR = SDL_SCANCODE_KP_XOR; ///< KP_XOR

constexpr SDL_Scancode SCANCODE_KP_POWER = SDL_SCANCODE_KP_POWER; ///< KP_POWER

constexpr SDL_Scancode SCANCODE_KP_PERCENT =
  SDL_SCANCODE_KP_PERCENT; ///< KP_PERCENT

constexpr SDL_Scancode SCANCODE_KP_LESS = SDL_SCANCODE_KP_LESS; ///< KP_LESS

constexpr SDL_Scancode SCANCODE_KP_GREATER =
  SDL_SCANCODE_KP_GREATER; ///< KP_GREATER

constexpr SDL_Scancode SCANCODE_KP_AMPERSAND =
  SDL_SCANCODE_KP_AMPERSAND; ///< KP_AMPERSAND

constexpr SDL_Scancode SCANCODE_KP_DBLAMPERSAND =
  SDL_SCANCODE_KP_DBLAMPERSAND; ///< KP_DBLAMPERSAND

constexpr SDL_Scancode SCANCODE_KP_VERTICALBAR =
  SDL_SCANCODE_KP_VERTICALBAR; ///< KP_VERTICALBAR

constexpr SDL_Scancode SCANCODE_KP_DBLVERTICALBAR =
  SDL_SCANCODE_KP_DBLVERTICALBAR; ///< KP_DBLVERTICALBAR

constexpr SDL_Scancode SCANCODE_KP_COLON = SDL_SCANCODE_KP_COLON; ///< KP_COLON

constexpr SDL_Scancode SCANCODE_KP_HASH = SDL_SCANCODE_KP_HASH; ///< KP_HASH

constexpr SDL_Scancode SCANCODE_KP_SPACE = SDL_SCANCODE_KP_SPACE; ///< KP_SPACE

constexpr SDL_Scancode SCANCODE_KP_AT = SDL_SCANCODE_KP_AT; ///< KP_AT

constexpr SDL_Scancode SCANCODE_KP_EXCLAM =
  SDL_SCANCODE_KP_EXCLAM; ///< KP_EXCLAM

constexpr SDL_Scancode SCANCODE_KP_MEMSTORE =
  SDL_SCANCODE_KP_MEMSTORE; ///< KP_MEMSTORE

constexpr SDL_Scancode SCANCODE_KP_MEMRECALL =
  SDL_SCANCODE_KP_MEMRECALL; ///< KP_MEMRECALL

constexpr SDL_Scancode SCANCODE_KP_MEMCLEAR =
  SDL_SCANCODE_KP_MEMCLEAR; ///< KP_MEMCLEAR

constexpr SDL_Scancode SCANCODE_KP_MEMADD =
  SDL_SCANCODE_KP_MEMADD; ///< KP_MEMADD

constexpr SDL_Scancode SCANCODE_KP_MEMSUBTRACT =
  SDL_SCANCODE_KP_MEMSUBTRACT; ///< KP_MEMSUBTRACT

constexpr SDL_Scancode SCANCODE_KP_MEMMULTIPLY =
  SDL_SCANCODE_KP_MEMMULTIPLY; ///< KP_MEMMULTIPLY

constexpr SDL_Scancode SCANCODE_KP_MEMDIVIDE =
  SDL_SCANCODE_KP_MEMDIVIDE; ///< KP_MEMDIVIDE

constexpr SDL_Scancode SCANCODE_KP_PLUSMINUS =
  SDL_SCANCODE_KP_PLUSMINUS; ///< KP_PLUSMINUS

constexpr SDL_Scancode SCANCODE_KP_CLEAR = SDL_SCANCODE_KP_CLEAR; ///< KP_CLEAR

constexpr SDL_Scancode SCANCODE_KP_CLEARENTRY =
  SDL_SCANCODE_KP_CLEARENTRY; ///< KP_CLEARENTRY

constexpr SDL_Scancode SCANCODE_KP_BINARY =
  SDL_SCANCODE_KP_BINARY; ///< KP_BINARY

constexpr SDL_Scancode SCANCODE_KP_OCTAL = SDL_SCANCODE_KP_OCTAL; ///< KP_OCTAL

constexpr SDL_Scancode SCANCODE_KP_DECIMAL =
  SDL_SCANCODE_KP_DECIMAL; ///< KP_DECIMAL

constexpr SDL_Scancode SCANCODE_KP_HEXADECIMAL =
  SDL_SCANCODE_KP_HEXADECIMAL; ///< KP_HEXADECIMAL

constexpr SDL_Scancode SCANCODE_LCTRL = SDL_SCANCODE_LCTRL; ///< LCTRL

constexpr SDL_Scancode SCANCODE_LSHIFT = SDL_SCANCODE_LSHIFT; ///< LSHIFT

constexpr SDL_Scancode SCANCODE_LALT = SDL_SCANCODE_LALT; ///< alt, option

constexpr SDL_Scancode SCANCODE_LGUI =
  SDL_SCANCODE_LGUI; ///< windows, command (apple), meta

constexpr SDL_Scancode SCANCODE_RCTRL = SDL_SCANCODE_RCTRL; ///< RCTRL

constexpr SDL_Scancode SCANCODE_RSHIFT = SDL_SCANCODE_RSHIFT; ///< RSHIFT

constexpr SDL_Scancode SCANCODE_RALT = SDL_SCANCODE_RALT; ///< alt gr, option

constexpr SDL_Scancode SCANCODE_RGUI =
  SDL_SCANCODE_RGUI; ///< windows, command (apple), meta

/**
 * I'm not sure if this is really not covered by any of the above, but since
 * there's a special KMOD_MODE for it I'm adding it here.
 */
constexpr SDL_Scancode SCANCODE_MODE = SDL_SCANCODE_MODE;

constexpr SDL_Scancode SCANCODE_SLEEP = SDL_SCANCODE_SLEEP; ///< Sleep.

constexpr SDL_Scancode SCANCODE_WAKE = SDL_SCANCODE_WAKE; ///< Wake.

constexpr SDL_Scancode SCANCODE_CHANNEL_INCREMENT =
  SDL_SCANCODE_CHANNEL_INCREMENT; ///< Channel Increment.

constexpr SDL_Scancode SCANCODE_CHANNEL_DECREMENT =
  SDL_SCANCODE_CHANNEL_DECREMENT; ///< Channel Decrement.

constexpr SDL_Scancode SCANCODE_MEDIA_PLAY = SDL_SCANCODE_MEDIA_PLAY; ///< Play.

constexpr SDL_Scancode SCANCODE_MEDIA_PAUSE =
  SDL_SCANCODE_MEDIA_PAUSE; ///< Pause.

constexpr SDL_Scancode SCANCODE_MEDIA_RECORD =
  SDL_SCANCODE_MEDIA_RECORD; ///< Record.

constexpr SDL_Scancode SCANCODE_MEDIA_FAST_FORWARD =
  SDL_SCANCODE_MEDIA_FAST_FORWARD; ///< Fast Forward.

constexpr SDL_Scancode SCANCODE_MEDIA_REWIND =
  SDL_SCANCODE_MEDIA_REWIND; ///< Rewind.

constexpr SDL_Scancode SCANCODE_MEDIA_NEXT_TRACK =
  SDL_SCANCODE_MEDIA_NEXT_TRACK; ///< Next Track.

constexpr SDL_Scancode SCANCODE_MEDIA_PREVIOUS_TRACK =
  SDL_SCANCODE_MEDIA_PREVIOUS_TRACK; ///< Previous Track.

constexpr SDL_Scancode SCANCODE_MEDIA_STOP = SDL_SCANCODE_MEDIA_STOP; ///< Stop.

constexpr SDL_Scancode SCANCODE_MEDIA_EJECT =
  SDL_SCANCODE_MEDIA_EJECT; ///< Eject.

constexpr SDL_Scancode SCANCODE_MEDIA_PLAY_PAUSE =
  SDL_SCANCODE_MEDIA_PLAY_PAUSE; ///< Play / Pause.

constexpr SDL_Scancode SCANCODE_MEDIA_SELECT =
  SDL_SCANCODE_MEDIA_SELECT; ///< MEDIA_SELECT

constexpr SDL_Scancode SCANCODE_AC_NEW = SDL_SCANCODE_AC_NEW; ///< AC New.

constexpr SDL_Scancode SCANCODE_AC_OPEN = SDL_SCANCODE_AC_OPEN; ///< AC Open.

constexpr SDL_Scancode SCANCODE_AC_CLOSE = SDL_SCANCODE_AC_CLOSE; ///< AC Close.

constexpr SDL_Scancode SCANCODE_AC_EXIT = SDL_SCANCODE_AC_EXIT; ///< AC Exit.

constexpr SDL_Scancode SCANCODE_AC_SAVE = SDL_SCANCODE_AC_SAVE; ///< AC Save.

constexpr SDL_Scancode SCANCODE_AC_PRINT = SDL_SCANCODE_AC_PRINT; ///< AC Print.

constexpr SDL_Scancode SCANCODE_AC_PROPERTIES =
  SDL_SCANCODE_AC_PROPERTIES; ///< AC Properties.

constexpr SDL_Scancode SCANCODE_AC_SEARCH =
  SDL_SCANCODE_AC_SEARCH; ///< AC Search.

constexpr SDL_Scancode SCANCODE_AC_HOME = SDL_SCANCODE_AC_HOME; ///< AC Home.

constexpr SDL_Scancode SCANCODE_AC_BACK = SDL_SCANCODE_AC_BACK; ///< AC Back.

constexpr SDL_Scancode SCANCODE_AC_FORWARD =
  SDL_SCANCODE_AC_FORWARD; ///< AC Forward.

constexpr SDL_Scancode SCANCODE_AC_STOP = SDL_SCANCODE_AC_STOP; ///< AC Stop.

constexpr SDL_Scancode SCANCODE_AC_REFRESH =
  SDL_SCANCODE_AC_REFRESH; ///< AC Refresh.

constexpr SDL_Scancode SCANCODE_AC_BOOKMARKS =
  SDL_SCANCODE_AC_BOOKMARKS; ///< AC Bookmarks.

/**
 * Usually situated below the display on phones and used as a multi-function
 * feature key for selecting a software defined function shown on the bottom
 * left of the display.
 */
constexpr SDL_Scancode SCANCODE_SOFTLEFT = SDL_SCANCODE_SOFTLEFT;

/**
 * Usually situated below the display on phones and used as a multi-function
 * feature key for selecting a software defined function shown on the bottom
 * right of the display.
 */
constexpr SDL_Scancode SCANCODE_SOFTRIGHT = SDL_SCANCODE_SOFTRIGHT;

constexpr SDL_Scancode SCANCODE_CALL =
  SDL_SCANCODE_CALL; ///< Used for accepting phone calls.

constexpr SDL_Scancode SCANCODE_ENDCALL =
  SDL_SCANCODE_ENDCALL; ///< Used for rejecting phone calls.

constexpr SDL_Scancode SCANCODE_RESERVED =
  SDL_SCANCODE_RESERVED; ///< 400-500 reserved for dynamic keycodes

/**
 * not a key, just marks the number of scancodes for array bounds
 */
constexpr SDL_Scancode SCANCODE_COUNT = SDL_SCANCODE_COUNT;

/// @}
} // namespace SDL

#endif /* SDL3PP_SCANCODE_H_ */
