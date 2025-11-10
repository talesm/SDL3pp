#ifndef SDL3PP_KEYBOARD_H_
#define SDL3PP_KEYBOARD_H_

#include <SDL3/SDL_keyboard.h>
#include "SDL3pp_keycode.h"
#include "SDL3pp_scancode.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategoryKeyboard Category Keyboard
 *
 * SDL keyboard management.
 *
 * Please refer to the Best Keyboard Practices document for details on how best
 * to accept keyboard input in various types of programs:
 *
 * https://wiki.libsdl.org/SDL3/BestKeyboardPractices
 *
 * @{
 */

/**
 * This is a unique ID for a keyboard for the time it is connected to the
 * system, and is never reused for the lifetime of the application.
 *
 * If the keyboard is disconnected and reconnected, it will get a new ID.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using KeyboardID = SDL_KeyboardID;

/**
 * Return whether a keyboard is currently connected.
 *
 * @returns true if a keyboard is connected, false otherwise.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetKeyboards
 */
inline bool HasKeyboard() { return SDL_HasKeyboard(); }

/**
 * Get a list of currently connected keyboards.
 *
 * Note that this will include any device or virtual driver that includes
 * keyboard functionality, including some mice, KVM switches, motherboard power
 * buttons, etc. You should wait for input from a device before you consider it
 * actively in use.
 *
 * @param count a pointer filled in with the number of keyboards returned, may
 *              be nullptr.
 * @returns a 0 terminated array of keyboards instance IDs or nullptr on
 *          failure; call GetError() for more information. This should be freed
 *          with free() when it is no longer needed.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetKeyboardNameForID
 * @sa HasKeyboard
 */
inline OwnArray<KeyboardID> GetKeyboards() { return SDL_GetKeyboards(); }

/**
 * Get the name of a keyboard.
 *
 * This function returns "" if the keyboard doesn't have a name.
 *
 * @param instance_id the keyboard instance ID.
 * @returns the name of the selected keyboard or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetKeyboards
 */
inline const char* GetKeyboardNameForID(KeyboardID instance_id)
{
  return SDL_GetKeyboardNameForID(instance_id);
}

/**
 * Query the window which currently has keyboard focus.
 *
 * @returns the window with keyboard focus.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline WindowRef GetKeyboardFocus() { return SDL_GetKeyboardFocus(); }

/**
 * Get a snapshot of the current state of the keyboard.
 *
 * The pointer returned is a pointer to an internal SDL array. It will be valid
 * for the whole lifetime of the application and should not be freed by the
 * caller.
 *
 * A array element with a value of true means that the key is pressed and a
 * value of false means that it is not. Indexes into this array are obtained by
 * using Scancode values.
 *
 * Use PumpEvents() to update the state array.
 *
 * This function gives you the current state after all events have been
 * processed, so if a key or button has been pressed and released before you
 * process events, then the pressed state will never show up in the
 * GetKeyboardState() calls.
 *
 * Note: This function doesn't take into account whether shift has been pressed
 * or not.
 *
 * @param numkeys if non-nullptr, receives the length of the returned array.
 * @returns a pointer to an array of key states.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PumpEvents
 * @sa ResetKeyboard
 */
inline std::span<const bool> GetKeyboardState()
{
  return SDL_GetKeyboardState();
}

/**
 * Clear the state of the keyboard.
 *
 * This function will generate key up events for all pressed keys.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetKeyboardState
 */
inline void ResetKeyboard() { SDL_ResetKeyboard(); }

/**
 * Get the current key modifier state for the keyboard.
 *
 * @returns an OR'd combination of the modifier keys for the keyboard. See
 *          Keymod for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetKeyboardState
 * @sa SetModState
 */
inline Keymod GetModState() { return SDL_GetModState(); }

/**
 * Set the current key modifier state for the keyboard.
 *
 * The inverse of GetModState(), SetModState() allows you to impose modifier key
 * states on your application. Simply pass your desired modifier states into
 * `modstate`. This value may be a bitwise, OR'd combination of Keymod values.
 *
 * This does not change the keyboard state, only the key modifier flags that SDL
 * reports.
 *
 * @param modstate the desired Keymod for the keyboard.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetModState
 */
inline void SetModState(Keymod modstate) { SDL_SetModState(modstate); }

inline Keycode::Keycode(Scancode scancode, Keymod modstate, bool key_event)
  : m_keycode(SDL_GetKeyFromScancode(scancode, modstate, key_event))
{
}

inline Keycode::Keycode(StringParam name)
  : m_keycode(SDL_GetKeyFromName(name))
{
}

inline Scancode Keycode::GetScancode(Keymod* modstate) const
{
  return SDL_GetScancodeFromKey(m_keycode, modstate);
}

inline void Scancode::SetName(StringParam name)
{
  CheckError(SDL_SetScancodeName(m_scancode, name));
}

inline const char* Scancode::GetName() const
{
  return SDL_GetScancodeName(m_scancode);
}

inline Scancode::Scancode(StringParam name)
  : m_scancode(SDL_GetScancodeFromName(name))
{
}

inline const char* Keycode::GetName() const
{
  return SDL_GetKeyName(m_keycode);
}

inline void Window::StartTextInput()
{
  CheckError(SDL_StartTextInput(m_resource));
}

inline void Window::StartTextInput(PropertiesParam props)
{
  CheckError(SDL_StartTextInputWithProperties(m_resource, props));
}

/**
 * Text input type.
 *
 * These are the valid values for prop::TextInput.TYPE_NUMBER. Not every value
 * is valid on every platform, but where a value isn't supported, a reasonable
 * fallback will be used.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa Window.StartTextInput
 */
using TextInputType = SDL_TextInputType;

constexpr TextInputType TEXTINPUT_TYPE_TEXT =
  SDL_TEXTINPUT_TYPE_TEXT; ///< The input is text

constexpr TextInputType TEXTINPUT_TYPE_TEXT_NAME =
  SDL_TEXTINPUT_TYPE_TEXT_NAME; ///< The input is a person's name

constexpr TextInputType TEXTINPUT_TYPE_TEXT_EMAIL =
  SDL_TEXTINPUT_TYPE_TEXT_EMAIL; ///< The input is an e-mail address

constexpr TextInputType TEXTINPUT_TYPE_TEXT_USERNAME =
  SDL_TEXTINPUT_TYPE_TEXT_USERNAME; ///< The input is a username

constexpr TextInputType TEXTINPUT_TYPE_TEXT_PASSWORD_HIDDEN =
  SDL_TEXTINPUT_TYPE_TEXT_PASSWORD_HIDDEN; ///< The input is a secure password
                                           ///< that is hidden

constexpr TextInputType TEXTINPUT_TYPE_TEXT_PASSWORD_VISIBLE =
  SDL_TEXTINPUT_TYPE_TEXT_PASSWORD_VISIBLE; ///< The input is a secure password
                                            ///< that is visible

constexpr TextInputType TEXTINPUT_TYPE_NUMBER =
  SDL_TEXTINPUT_TYPE_NUMBER; ///< The input is a number

constexpr TextInputType TEXTINPUT_TYPE_NUMBER_PASSWORD_HIDDEN =
  SDL_TEXTINPUT_TYPE_NUMBER_PASSWORD_HIDDEN; ///< The input is a secure PIN that
                                             ///< is hidden

constexpr TextInputType TEXTINPUT_TYPE_NUMBER_PASSWORD_VISIBLE =
  SDL_TEXTINPUT_TYPE_NUMBER_PASSWORD_VISIBLE; ///< The input is a secure PIN
                                              ///< that is visible

/**
 * Auto capitalization type.
 *
 * These are the valid values for prop::TextInput.CAPITALIZATION_NUMBER. Not
 * every value is valid on every platform, but where a value isn't supported, a
 * reasonable fallback will be used.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa Window.StartTextInput
 */
using Capitalization = SDL_Capitalization;

constexpr Capitalization CAPITALIZE_NONE =
  SDL_CAPITALIZE_NONE; ///< No auto-capitalization will be done

constexpr Capitalization CAPITALIZE_SENTENCES =
  SDL_CAPITALIZE_SENTENCES; ///< The first letter of sentences will be
                            ///< capitalized

constexpr Capitalization CAPITALIZE_WORDS =
  SDL_CAPITALIZE_WORDS; ///< The first letter of words will be capitalized

constexpr Capitalization CAPITALIZE_LETTERS =
  SDL_CAPITALIZE_LETTERS; ///< All letters will be capitalized

namespace prop::TextInput {

constexpr auto TYPE_NUMBER = SDL_PROP_TEXTINPUT_TYPE_NUMBER;

constexpr auto CAPITALIZATION_NUMBER = SDL_PROP_TEXTINPUT_CAPITALIZATION_NUMBER;

constexpr auto AUTOCORRECT_BOOLEAN = SDL_PROP_TEXTINPUT_AUTOCORRECT_BOOLEAN;

constexpr auto MULTILINE_BOOLEAN = SDL_PROP_TEXTINPUT_MULTILINE_BOOLEAN;

constexpr auto ANDROID_INPUTTYPE_NUMBER =
  SDL_PROP_TEXTINPUT_ANDROID_INPUTTYPE_NUMBER;

} // namespace prop::TextInput

inline bool Window::IsTextInputActive() const
{
  return SDL_TextInputActive(m_resource);
}

inline void Window::StopTextInput()
{
  CheckError(SDL_StopTextInput(m_resource));
}

inline void Window::ClearComposition()
{
  CheckError(SDL_ClearComposition(m_resource));
}

inline void Window::SetTextInputArea(const RectRaw& rect, int cursor)
{
  CheckError(SDL_SetTextInputArea(m_resource, rect, cursor));
}

inline void Window::GetTextInputArea(RectRaw* rect, int* cursor)
{
  CheckError(SDL_GetTextInputArea(m_resource, rect, cursor));
}

/**
 * Check whether the platform has screen keyboard support.
 *
 * @returns true if the platform has some screen keyboard support or false if
 *          not.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Window.StartTextInput
 * @sa Window.IsScreenKeyboardShown
 */
inline bool HasScreenKeyboardSupport()
{
  return SDL_HasScreenKeyboardSupport();
}

inline bool Window::IsScreenKeyboardShown() const
{
  return SDL_ScreenKeyboardShown(m_resource);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_KEYBOARD_H_ */
