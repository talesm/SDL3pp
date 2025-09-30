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
 * Please refer to the Best Keyboard Practices document for details on how
 * best to accept keyboard input in various types of programs:
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
 * keyboard functionality, including some mice, KVM switches, motherboard
 * power buttons, etc. You should wait for input from a device before you
 * consider it actively in use.
 *
 * @param count a pointer filled in with the number of keyboards returned, may
 *              be nullptr.
 * @returns a 0 terminated array of keyboards instance IDs or nullptr on
 * failure; call GetError() for more information. This should be freed with
 * free() when it is no longer needed.
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
 * The pointer returned is a pointer to an internal SDL array. It will be
 * valid for the whole lifetime of the application and should not be freed by
 * the caller.
 *
 * A array element with a value of true means that the key is pressed and a
 * value of false means that it is not. Indexes into this array are obtained
 * by using Scancode values.
 *
 * Use PumpEvents() to update the state array.
 *
 * This function gives you the current state after all events have been
 * processed, so if a key or button has been pressed and released before you
 * process events, then the pressed state will never show up in the
 * GetKeyboardState() calls.
 *
 * Note: This function doesn't take into account whether shift has been
 * pressed or not.
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
 * The inverse of GetModState(), SetModState() allows you to impose
 * modifier key states on your application. Simply pass your desired modifier
 * states into `modstate`. This value may be a bitwise, OR'd combination of
 * Keymod values.
 *
 * This does not change the keyboard state, only the key modifier flags that
 * SDL reports.
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

/**
 * Get the key code corresponding to the given scancode according to the
 * current keyboard layout.
 *
 * If you want to get the keycode as it would be delivered in key events,
 * including options specified in SDL_HINT_KEYCODE_OPTIONS, then you should
 * pass `key_event` as true. Otherwise this function simply translates the
 * scancode based on the given modifier state.
 *
 * @param scancode the desired Scancode to query.
 * @param modstate the modifier state to use when translating the scancode to
 *                 a keycode.
 * @param key_event true if the keycode will be used in key events.
 * @post the Keycode that corresponds to the given Scancode.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Keycode.GetName
 * @sa Keycode.GetScancode
 */
inline Keycode::Keycode(Scancode scancode, Keymod modstate, bool key_event)
  : m_keycode(SDL_GetKeyFromScancode(scancode, modstate, key_event))
{
}

/**
 * Get a key code from a human-readable name.
 *
 * @param name the human-readable key name.
 * @post key code, or `SDLK_UNKNOWN` if the name wasn't recognized; call
 *          GetError() for more information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Keycode.Keycode
 * @sa Keycode.GetName
 * @sa Scancode.Scancode
 */
inline Keycode::Keycode(StringParam name)
  : m_keycode(SDL_GetKeyFromName(name))
{
}

/**
 * Get the scancode corresponding to the given key code according to the
 * current keyboard layout.
 *
 * Note that there may be multiple scancode+modifier states that can generate
 * this keycode, this will just return the first one found.
 *
 * @param modstate a pointer to the modifier state that would be used when the
 *                 scancode generates this key, may be nullptr.
 * @returns the Scancode that corresponds to the given Keycode.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Keycode.Keycode
 * @sa Scancode.GetName
 */
inline Scancode Keycode::GetScancode(Keymod* modstate) const
{
  return SDL_GetScancodeFromKey(m_keycode, modstate);
}

/**
 * Set a human-readable name for a scancode.
 *
 * @param name the name to use for the scancode, encoded as UTF-8. The string
 *             is not copied, so the pointer given to this function must stay
 *             valid while SDL is being used.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Scancode.GetName
 */
inline void Scancode::SetName(StringParam name)
{
  CheckError(SDL_SetScancodeName(m_scancode, name));
}

/**
 * Get a human-readable name for a scancode.
 *
 * **Warning**: The returned name is by design not stable across platforms,
 * e.g. the name for `SCANCODE_LGUI` is "Left GUI" under Linux but "Left
 * Windows" under Microsoft Windows, and some scancodes like
 * `SCANCODE_NONUSBACKSLASH` don't have any name at all. There are even
 * scancodes that share names, e.g. `SCANCODE_RETURN` and
 * `SCANCODE_RETURN2` (both called "Return"). This function is therefore
 * unsuitable for creating a stable cross-platform two-way mapping between
 * strings and scancodes.
 *
 * @returns a pointer to the name for the scancode. If the scancode doesn't
 *          have a name this function returns an empty string ("").
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Keycode.GetScancode
 * @sa Scancode.Scancode
 * @sa Scancode.SetName
 */
inline const char* Scancode::GetName() const
{
  return SDL_GetScancodeName(m_scancode);
}

/**
 * Get a scancode from a human-readable name.
 *
 * @param name the human-readable scancode name.
 * @post the Scancode, or `SCANCODE_UNKNOWN` if the name wasn't
 *          recognized; call GetError() for more information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Keycode.Keycode
 * @sa Keycode.GetScancode
 * @sa Scancode.GetName
 */
inline Scancode::Scancode(StringParam name)
  : m_scancode(SDL_GetScancodeFromName(name))
{
}

/**
 * Get a human-readable name for a key.
 *
 * If the key doesn't have a name, this function returns an empty string ("").
 *
 * Letters will be presented in their uppercase form, if applicable.
 *
 * @returns a UTF-8 encoded string of the key name.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Keycode.Keycode
 * @sa Keycode.Keycode
 * @sa Keycode.GetScancode
 */
inline const char* Keycode::GetName() const
{
  return SDL_GetKeyName(m_keycode);
}

/**
 * Start accepting Unicode text input events in a window.
 *
 * This function will enable text input (EVENT_TEXT_INPUT and
 * EVENT_TEXT_EDITING events) in the specified window. Please use this
 * function paired with Window.StopTextInput().
 *
 * Text input events are not received by default.
 *
 * On some platforms using this function shows the screen keyboard and/or
 * activates an IME, which can prevent some key press events from being passed
 * through.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Window.SetTextInputArea
 * @sa Window.StartTextInput
 * @sa Window.StopTextInput
 * @sa Window.IsTextInputActive
 */
inline void Window::StartTextInput()
{
  CheckError(SDL_StartTextInput(m_resource));
}

/**
 * Start accepting Unicode text input events in a window, with properties
 * describing the input.
 *
 * This function will enable text input (EVENT_TEXT_INPUT and
 * EVENT_TEXT_EDITING events) in the specified window. Please use this
 * function paired with Window.StopTextInput().
 *
 * Text input events are not received by default.
 *
 * On some platforms using this function shows the screen keyboard and/or
 * activates an IME, which can prevent some key press events from being passed
 * through.
 *
 * These are the supported properties:
 *
 * - `prop::TextInput.TYPE_NUMBER` - an TextInputType value that
 *   describes text being input, defaults to TEXTINPUT_TYPE_TEXT.
 * - `prop::TextInput.CAPITALIZATION_NUMBER` - an Capitalization value
 *   that describes how text should be capitalized, defaults to
 *   CAPITALIZE_SENTENCES for normal text entry, CAPITALIZE_WORDS for
 *   TEXTINPUT_TYPE_TEXT_NAME, and CAPITALIZE_NONE for e-mail
 *   addresses, usernames, and passwords.
 * - `prop::TextInput.AUTOCORRECT_BOOLEAN` - true to enable auto completion
 *   and auto correction, defaults to true.
 * - `prop::TextInput.MULTILINE_BOOLEAN` - true if multiple lines of text
 *   are allowed. This defaults to true if SDL_HINT_RETURN_KEY_HIDES_IME is
 *   "0" or is not set, and defaults to false if SDL_HINT_RETURN_KEY_HIDES_IME
 *   is "1".
 *
 * On Android you can directly specify the input type:
 *
 * - `prop::TextInput.ANDROID_INPUTTYPE_NUMBER` - the text input type to
 *   use, overriding other properties. This is documented at
 *   https://developer.android.com/reference/android/text/InputType
 *
 * @param props the properties to use.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Window.SetTextInputArea
 * @sa Window.StartTextInput
 * @sa Window.StopTextInput
 * @sa Window.IsTextInputActive
 */
inline void Window::StartTextInput(PropertiesParam props)
{
  CheckError(SDL_StartTextInputWithProperties(m_resource, props));
}

/**
 * Text input type.
 *
 * These are the valid values for prop::TextInput.TYPE_NUMBER. Not every
 * value is valid on every platform, but where a value isn't supported, a
 * reasonable fallback will be used.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa Window.StartTextInput
 */
using TextInputType = SDL_TextInputType;

constexpr TextInputType TEXTINPUT_TYPE_TEXT =
  SDL_TEXTINPUT_TYPE_TEXT; ///< The input is text.

constexpr TextInputType TEXTINPUT_TYPE_TEXT_NAME =
  SDL_TEXTINPUT_TYPE_TEXT_NAME; ///< The input is a person's name.

constexpr TextInputType TEXTINPUT_TYPE_TEXT_EMAIL =
  SDL_TEXTINPUT_TYPE_TEXT_EMAIL; ///< The input is an e-mail address.

constexpr TextInputType TEXTINPUT_TYPE_TEXT_USERNAME =
  SDL_TEXTINPUT_TYPE_TEXT_USERNAME; ///< The input is a username.

constexpr TextInputType TEXTINPUT_TYPE_TEXT_PASSWORD_HIDDEN =
  SDL_TEXTINPUT_TYPE_TEXT_PASSWORD_HIDDEN; ///< The input is a secure password
                                           ///< that is hidden.

constexpr TextInputType TEXTINPUT_TYPE_TEXT_PASSWORD_VISIBLE =
  SDL_TEXTINPUT_TYPE_TEXT_PASSWORD_VISIBLE; ///< The input is a secure password
                                            ///< that is visible.

constexpr TextInputType TEXTINPUT_TYPE_NUMBER =
  SDL_TEXTINPUT_TYPE_NUMBER; ///< The input is a number.

constexpr TextInputType TEXTINPUT_TYPE_NUMBER_PASSWORD_HIDDEN =
  SDL_TEXTINPUT_TYPE_NUMBER_PASSWORD_HIDDEN; ///< The input is a secure PIN that
                                             ///< is hidden.

constexpr TextInputType TEXTINPUT_TYPE_NUMBER_PASSWORD_VISIBLE =
  SDL_TEXTINPUT_TYPE_NUMBER_PASSWORD_VISIBLE; ///< The input is a secure PIN
                                              ///< that is visible.

/**
 * Auto capitalization type.
 *
 * These are the valid values for prop::TextInput.CAPITALIZATION_NUMBER.
 * Not every value is valid on every platform, but where a value isn't
 * supported, a reasonable fallback will be used.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa Window.StartTextInput
 */
using Capitalization = SDL_Capitalization;

constexpr Capitalization CAPITALIZE_NONE =
  SDL_CAPITALIZE_NONE; ///< No auto-capitalization will be done.

constexpr Capitalization CAPITALIZE_SENTENCES =
  SDL_CAPITALIZE_SENTENCES; ///< The first letter of sentences will be
                            ///< capitalized.

constexpr Capitalization CAPITALIZE_WORDS =
  SDL_CAPITALIZE_WORDS; ///< The first letter of words will be capitalized.

constexpr Capitalization CAPITALIZE_LETTERS =
  SDL_CAPITALIZE_LETTERS; ///< All letters will be capitalized.

namespace prop::TextInput {

constexpr auto TYPE_NUMBER = SDL_PROP_TEXTINPUT_TYPE_NUMBER;

constexpr auto CAPITALIZATION_NUMBER = SDL_PROP_TEXTINPUT_CAPITALIZATION_NUMBER;

constexpr auto AUTOCORRECT_BOOLEAN = SDL_PROP_TEXTINPUT_AUTOCORRECT_BOOLEAN;

constexpr auto MULTILINE_BOOLEAN = SDL_PROP_TEXTINPUT_MULTILINE_BOOLEAN;

constexpr auto ANDROID_INPUTTYPE_NUMBER =
  SDL_PROP_TEXTINPUT_ANDROID_INPUTTYPE_NUMBER;

} // namespace prop::TextInput

/**
 * Check whether or not Unicode text input events are enabled for a window.
 *
 * @returns true if text input events are enabled else false.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Window.StartTextInput
 */
inline bool Window::IsTextInputActive() const
{
  return SDL_TextInputActive(m_resource);
}

/**
 * Stop receiving any text input events in a window.
 *
 * If Window.StartTextInput() showed the screen keyboard, this function will
 * hide it.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Window.StartTextInput
 */
inline void Window::StopTextInput()
{
  CheckError(SDL_StopTextInput(m_resource));
}

/**
 * Dismiss the composition window/IME without disabling the subsystem.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Window.StartTextInput
 * @sa Window.StopTextInput
 */
inline void Window::ClearComposition()
{
  CheckError(SDL_ClearComposition(m_resource));
}

/**
 * Set the area used to type Unicode text input.
 *
 * Native input methods may place a window with word suggestions near the
 * cursor, without covering the text being entered.
 *
 * @param rect the Rect representing the text input area, in window
 *             coordinates, or nullptr to clear it.
 * @param cursor the offset of the current cursor location relative to
 *               `rect->x`, in window coordinates.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Window.GetTextInputArea
 * @sa Window.StartTextInput
 */
inline void Window::SetTextInputArea(const RectRaw& rect, int cursor)
{
  CheckError(SDL_SetTextInputArea(m_resource, rect, cursor));
}

/**
 * Get the area used to type Unicode text input.
 *
 * This returns the values previously set by Window.SetTextInputArea().
 *
 * @param rect a pointer to an Rect filled in with the text input area,
 *             may be nullptr.
 * @param cursor a pointer to the offset of the current cursor location
 *               relative to `rect->x`, may be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Window.SetTextInputArea
 */
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

/**
 * Check whether the screen keyboard is shown for given window.
 *
 * @returns true if screen keyboard is shown or false if not.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasScreenKeyboardSupport
 */
inline bool Window::IsScreenKeyboardShown() const
{
  return SDL_ScreenKeyboardShown(m_resource);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_KEYBOARD_H_ */
