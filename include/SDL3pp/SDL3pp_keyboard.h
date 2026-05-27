#ifndef SDL3PP_KEYBOARD_H_
#define SDL3PP_KEYBOARD_H_

#include <SDL3/SDL_keyboard.h>
#include "SDL3pp_keycode.h"
#include "SDL3pp_scancode.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategoryKeyboard Keyboard Support
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
 * @returns a 0 terminated array of keyboards instance IDs on success..
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetKeyboardNameForID
 * @sa HasKeyboard
 */
inline OwnArray<KeyboardID> GetKeyboards()
{
  int count;
  auto data = CheckError(SDL_GetKeyboards(&count));
  return OwnArray<KeyboardID>{data, size_t(count)};
}

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
inline WindowRef GetKeyboardFocus() { return {SDL_GetKeyboardFocus()}; }

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
  int count;
  auto data = SDL_GetKeyboardState(&count);
  return std::span{data, size_t(count)};
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
 * @returns an OR'd combination of the modifier keys for the keyboard.
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

/**
 * Start accepting Unicode text input events in a window.
 *
 * This function will enable text input (EVENT_TEXT_INPUT and EVENT_TEXT_EDITING
 * events) in the specified window. Please use this function paired with
 * StopTextInput().
 *
 * Text input events are not received by default.
 *
 * On some platforms using this function shows the screen keyboard and/or
 * activates an IME, which can prevent some key press events from being passed
 * through.
 *
 * @param window the window to enable text input.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetTextInputArea
 * @sa StartTextInputWithProperties
 * @sa StopTextInput
 * @sa TextInputActive
 */
inline void StartTextInput(WindowRef window)
{
  CheckError(SDL_StartTextInput(window));
}

inline void WindowBase::StartTextInput() { SDL::StartTextInput(get()); }

/**
 * Text input type.
 *
 * These are the valid values for prop.TextInput.TYPE_NUMBER. Not every value is
 * valid on every platform, but where a value isn't supported, a reasonable
 * fallback will be used.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa StartTextInputWithProperties
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
 * These are the valid values for prop.TextInput.CAPITALIZATION_NUMBER. Not
 * every value is valid on every platform, but where a value isn't supported, a
 * reasonable fallback will be used.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa StartTextInputWithProperties
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

/**
 * Start accepting Unicode text input events in a window, with properties
 * describing the input.
 *
 * This function will enable text input (EVENT_TEXT_INPUT and EVENT_TEXT_EDITING
 * events) in the specified window. Please use this function paired with
 * StopTextInput().
 *
 * Text input events are not received by default.
 *
 * On some platforms using this function shows the screen keyboard and/or
 * activates an IME, which can prevent some key press events from being passed
 * through.
 *
 * These are the supported properties:
 *
 * - `prop.TextInput.TYPE_NUMBER` - an TextInputType value that describes text
 *   being input, defaults to TEXTINPUT_TYPE_TEXT.
 * - `prop.TextInput.CAPITALIZATION_NUMBER` - an Capitalization value that
 *   describes how text should be capitalized, defaults to CAPITALIZE_SENTENCES
 *   for normal text entry, CAPITALIZE_WORDS for TEXTINPUT_TYPE_TEXT_NAME, and
 *   CAPITALIZE_NONE for e-mail addresses, usernames, and passwords.
 * - `prop.TextInput.AUTOCORRECT_BOOLEAN` - true to enable auto completion and
 *   auto correction, defaults to true.
 * - `prop.TextInput.MULTILINE_BOOLEAN` - true if multiple lines of text are
 *   allowed. This defaults to true if SDL_HINT_RETURN_KEY_HIDES_IME is "0" or
 *   is not set, and defaults to false if SDL_HINT_RETURN_KEY_HIDES_IME is "1".
 *
 * On Android you can directly specify the input type:
 *
 * - `prop.TextInput.ANDROID_INPUTTYPE_NUMBER` - the text input type to use,
 *   overriding other properties. This is documented at
 *   https://developer.android.com/reference/android/text/InputType
 *
 * @param window the window to enable text input.
 * @param props the properties to use.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetTextInputArea
 * @sa StartTextInput
 * @sa StopTextInput
 * @sa TextInputActive
 */
inline void StartTextInputWithProperties(WindowRef window, PropertiesRef props)
{
  CheckError(SDL_StartTextInputWithProperties(window, props));
}

inline void WindowBase::StartTextInputWithProperties(PropertiesRef props)
{
  SDL::StartTextInputWithProperties(get(), props);
}

/**
 * Properties for text input to be used on StartTextInputWithProperties.
 *
 * @sa StartTextInputWithProperties
 */
namespace prop::TextInput {

/**
 * The type of text input to be used.
 *
 * @see TextInputType
 */
constexpr auto TYPE_NUMBER = SDL_PROP_TEXTINPUT_TYPE_NUMBER;

/**
 * The auto-capitalization type to be used.
 *
 * @see Capitalization
 */
constexpr auto CAPITALIZATION_NUMBER = SDL_PROP_TEXTINPUT_CAPITALIZATION_NUMBER;

constexpr auto AUTOCORRECT_BOOLEAN =
  SDL_PROP_TEXTINPUT_AUTOCORRECT_BOOLEAN; ///< Autocorrect enabled.

constexpr auto MULTILINE_BOOLEAN =
  SDL_PROP_TEXTINPUT_MULTILINE_BOOLEAN; ///< Multiline enabled.

constexpr auto ANDROID_INPUTTYPE_NUMBER =
  SDL_PROP_TEXTINPUT_ANDROID_INPUTTYPE_NUMBER; ///< Number for android
                                               ///< inputtype.

} // namespace prop::TextInput

/**
 * Check whether or not Unicode text input events are enabled for a window.
 *
 * @param window the window to check.
 * @returns true if text input events are enabled else false.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa StartTextInput
 */
inline bool TextInputActive(WindowRef window)
{
  return SDL_TextInputActive(window);
}

inline bool WindowBase::IsTextInputActive() const
{
  return TextInputActive(get());
}

/**
 * Stop receiving any text input events in a window.
 *
 * If StartTextInput() showed the screen keyboard, this function will hide it.
 *
 * @param window the window to disable text input.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa StartTextInput
 * @sa StartTextInputWithProperties
 */
inline void StopTextInput(WindowRef window)
{
  CheckError(SDL_StopTextInput(window));
}

inline void WindowBase::StopTextInput() { SDL::StopTextInput(get()); }

/**
 * Dismiss the composition window/IME without disabling the subsystem.
 *
 * @param window the window to affect.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa StartTextInput
 * @sa StopTextInput
 */
inline void ClearComposition(WindowRef window)
{
  CheckError(SDL_ClearComposition(window));
}

inline void WindowBase::ClearComposition() { SDL::ClearComposition(get()); }

/**
 * Set the area used to type Unicode text input.
 *
 * Native input methods may place a window with word suggestions near the
 * cursor, without covering the text being entered.
 *
 * @param window the window for which to set the text input area.
 * @param rect the Rect representing the text input area, in window coordinates,
 *             or nullptr to clear it.
 * @param cursor the offset of the current cursor location relative to
 *               `rect->x`, in window coordinates.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetTextInputArea
 * @sa StartTextInput
 */
inline void SetTextInputArea(WindowRef window, const RectRaw& rect, int cursor)
{
  CheckError(SDL_SetTextInputArea(window, &rect, cursor));
}

inline void WindowBase::SetTextInputArea(const RectRaw& rect, int cursor)
{
  SDL::SetTextInputArea(get(), rect, cursor);
}

/**
 * Get the area used to type Unicode text input.
 *
 * This returns the values previously set by SetTextInputArea().
 *
 * @param window the window for which to query the text input area.
 * @param rect a pointer to an Rect filled in with the text input area, may be
 *             nullptr.
 * @param cursor a pointer to the offset of the current cursor location relative
 *               to `rect->x`, may be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetTextInputArea
 */
inline void GetTextInputArea(WindowRef window, RectRaw* rect, int* cursor)
{
  CheckError(SDL_GetTextInputArea(window, rect, cursor));
}

inline void WindowBase::GetTextInputArea(RectRaw* rect, int* cursor) const
{
  SDL::GetTextInputArea(get(), rect, cursor);
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
 * @sa StartTextInput
 * @sa ScreenKeyboardShown
 */
inline bool HasScreenKeyboardSupport()
{
  return SDL_HasScreenKeyboardSupport();
}

/**
 * Check whether the screen keyboard is shown for given window.
 *
 * @param window the window for which screen keyboard should be queried.
 * @returns true if screen keyboard is shown or false if not.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasScreenKeyboardSupport
 */
inline bool ScreenKeyboardShown(WindowRef window)
{
  return SDL_ScreenKeyboardShown(window);
}

inline bool WindowBase::IsScreenKeyboardShown() const
{
  return ScreenKeyboardShown(get());
}

/// @}

} // namespace SDL

#endif /* SDL3PP_KEYBOARD_H_ */
