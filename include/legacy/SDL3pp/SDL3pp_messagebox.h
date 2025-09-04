#ifndef SDL3PP_MESSAGEBOX_H_
#define SDL3PP_MESSAGEBOX_H_

#include <SDL3/SDL_messagebox.h>
#include "SDL3pp_stdinc.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 *
 * @defgroup CategoryMessagebox Message Boxes
 *
 * SDL offers a simple message box API, which is useful for simple alerts,
 * such as informing the user when something fatal happens at startup without
 * the need to build a UI for it (or informing the user _before_ your UI is
 * ready).
 *
 * These message boxes are native system dialogs where possible.
 *
 * There is both a customizable function (MessageBox.Show()) that offers
 * lots of options for what to display and reports on what choice the user
 * made, and also a much-simplified version (ShowSimpleMessageBox()),
 * merely takes a text message and title, and waits until the user presses a
 * single "OK" UI button. Often, this is all that is necessary.
 *
 * @{
 */

/**
 * @name MessageBoxFlags
 * @{
 */

/**
 * Message box flags.
 *
 * If supported will display warning icon, etc.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using MessageBoxFlags = Uint32;

constexpr MessageBoxFlags MESSAGEBOX_ERROR =
  SDL_MESSAGEBOX_ERROR; ///< error dialog

constexpr MessageBoxFlags MESSAGEBOX_WARNING =
  SDL_MESSAGEBOX_WARNING; ///< warning dialog

constexpr MessageBoxFlags MESSAGEBOX_INFORMATION =
  SDL_MESSAGEBOX_INFORMATION; ///< informational dialog

constexpr MessageBoxFlags MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT =
  SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT; ///< buttons placed left to right

constexpr MessageBoxFlags MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT =
  SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT; ///< buttons placed right to left

/// @}

/**
 * @name MessageBoxButtonFlags
 * @{
 */

/**
 * MessageBoxButtonData flags.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using MessageBoxButtonFlags = Uint32;

constexpr MessageBoxButtonFlags MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT =
  SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT; ///< Marks the default button when
                                           ///< return is hit

constexpr MessageBoxButtonFlags MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT =
  SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT; ///< Marks the default button when
                                           ///< escape is hit

/// @}

/**
 * Individual button data.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MessageBoxButtonData = SDL_MessageBoxButtonData;

/**
 * RGB value used in a message box color scheme
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MessageBoxColor = SDL_MessageBoxColor;

/**
 * @name MessageBoxColorTypes
 * @{
 */

/**
 * An enumeration of indices inside the colors array of
 * MessageBoxColorScheme.
 */
using MessageBoxColorType = SDL_MessageBoxColorType;

constexpr MessageBoxColorType MESSAGEBOX_COLOR_BACKGROUND =
  SDL_MESSAGEBOX_COLOR_BACKGROUND; ///< BACKGROUND

constexpr MessageBoxColorType MESSAGEBOX_COLOR_TEXT =
  SDL_MESSAGEBOX_COLOR_TEXT; ///< TEXT

constexpr MessageBoxColorType MESSAGEBOX_COLOR_BUTTON_BORDER =
  SDL_MESSAGEBOX_COLOR_BUTTON_BORDER; ///< BUTTON_BORDER

constexpr MessageBoxColorType MESSAGEBOX_COLOR_BUTTON_BACKGROUND =
  SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND; ///< BUTTON_BACKGROUND

constexpr MessageBoxColorType MESSAGEBOX_COLOR_BUTTON_SELECTED =
  SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED; ///< BUTTON_SELECTED

constexpr MessageBoxColorType MESSAGEBOX_COLOR_COUNT =
  SDL_MESSAGEBOX_COLOR_COUNT; ///< Count

/// @}

/**
 * A set of colors to use for message box dialogs
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MessageBoxColorScheme = SDL_MessageBoxColorScheme;

/**
 * MessageBox structure containing title, text, window, etc.
 *
 * @since This struct is available since SDL 3.2.0.
 */
struct MessageBox : SDL_MessageBoxData
{
  /**
   * Wraps MessageBox.
   *
   * @param messageBox the value to be wrapped
   */
  constexpr MessageBox(const SDL_MessageBoxData& messageBox = {})
    : SDL_MessageBoxData{messageBox}
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param flags the value for flags.
   * @param window the value for window.
   * @param title the value for title.
   * @param message the value for message.
   * @param buttons the value for buttons.
   * @param colorScheme the value for colorScheme.
   */
  constexpr MessageBox(MessageBoxFlags flags,
                       WindowRef window,
                       const char* title,
                       const char* message,
                       std::span<const MessageBoxButtonData> buttons,
                       OptionalRef<const MessageBoxColorScheme> colorScheme)
    : SDL_MessageBoxData{flags,
                         window.get(),
                         title,
                         message,
                         int(buttons.size()),
                         buttons.data(),
                         colorScheme}
  {
  }

  /**
   * Create a modal message box.
   *
   * If your needs aren't complex, it might be easier to use
   * ShowSimpleMessageBox.
   *
   * This function should be called on the thread that created the parent
   * window, or on the main thread if the messagebox has no parent. It will
   * block execution of that thread until the user clicks a button or closes the
   * messagebox.
   *
   * This function may be called at any time, even before InitSubSystem(). This
   * makes it useful for reporting errors like a failure to create a renderer or
   * OpenGL context.
   *
   * On X11, SDL rolls its own dialog box with X11 primitives instead of a
   * formal toolkit like GTK+ or Qt.
   *
   * Note that if InitSubSystem() would fail because there isn't any available
   * video target, this function is likely to fail for the same reasons. If this
   * is a concern, check the return value from this function and fall back to
   * writing to stderr if you can.
   *
   * @param buttonid the pointer to which user id of hit button should be
   *                 copied.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ShowSimpleMessageBox
   */
  void Show(int* buttonid) const
  {
    CheckError(SDL_ShowMessageBox(this, buttonid));
  }

  /**
   * Get the flags.
   *
   * @returns current flags value.
   */
  constexpr SDL_MessageBoxFlags GetFlags() const { return flags; }

  /**
   * Set the flags.
   *
   * @param newFlags the new flags value.
   * @returns Reference to self.
   */
  constexpr MessageBox& SetFlags(SDL_MessageBoxFlags newFlags)
  {
    flags = newFlags;
    return *this;
  }

  /**
   * Get the window.
   *
   * @returns current window value.
   */
  constexpr WindowRef GetWindow() const { return window; }

  /**
   * Set the window.
   *
   * @param newWindow the new window value.
   * @returns Reference to self.
   */
  constexpr MessageBox& SetWindow(WindowRef newWindow)
  {
    window = newWindow.get();
    return *this;
  }

  /**
   * Get the title.
   *
   * @returns current title value.
   */
  constexpr const char* GetTitle() const { return title; }

  /**
   * Set the title.
   *
   * @param newTitle the new title value.
   * @returns Reference to self.
   */
  constexpr MessageBox& SetTitle(const char* newTitle)
  {
    title = newTitle;
    return *this;
  }

  /**
   * Get the message.
   *
   * @returns current message value.
   */
  constexpr const char* GetMessage() const { return message; }

  /**
   * Set the message.
   *
   * @param newMessage the new message value.
   * @returns Reference to self.
   */
  constexpr MessageBox& SetMessage(const char* newMessage)
  {
    message = newMessage;
    return *this;
  }

  /**
   * Get the buttons.
   *
   * @returns current buttons value.
   */
  constexpr std::span<const MessageBoxButtonData> GetButtons() const
  {
    if (numbuttons == 0) return {};
    return std::span(buttons, size_t(numbuttons));
  }

  /**
   * Set the buttons.
   *
   * @param newButtons the new buttons value.
   * @returns Reference to self.
   */
  constexpr MessageBox& SetButtons(
    std::span<const MessageBoxButtonData> newButtons)
  {
    if (newButtons.empty()) {
      numbuttons = 0;
      buttons = nullptr;
    } else {
      numbuttons = newButtons.size();
      buttons = newButtons.data();
    }
    return *this;
  }

  /**
   * Get the colorScheme.
   *
   * @returns current colorScheme value.
   */
  constexpr const MessageBoxColorScheme* GetColorScheme() const
  {
    return colorScheme;
  }

  /**
   * Set the colorScheme.
   *
   * @param newColorScheme the new colorScheme value.
   * @returns Reference to self.
   */
  constexpr MessageBox& SetColorScheme(
    OptionalRef<const MessageBoxColorScheme> newColorScheme)
  {
    colorScheme = newColorScheme;
    return *this;
  }
};

/**
 * Display a simple modal message box.
 *
 * If your needs aren't complex, this function is preferred over
 * MessageBox.Show.
 *
 * `flags` may be any of the following:
 *
 * - `MESSAGEBOX_ERROR`: error dialog
 * - `MESSAGEBOX_WARNING`: warning dialog
 * - `MESSAGEBOX_INFORMATION`: informational dialog
 *
 * This function should be called on the thread that created the parent
 * window, or on the main thread if the messagebox has no parent. It will
 * block execution of that thread until the user clicks a button or closes the
 * messagebox.
 *
 * This function may be called at any time, even before InitSubSystem(). This
 * makes it useful for reporting errors like a failure to create a renderer or
 * OpenGL context.
 *
 * On X11, SDL rolls its own dialog box with X11 primitives instead of a
 * formal toolkit like GTK+ or Qt.
 *
 * Note that if InitSubSystem() would fail because there isn't any available
 * video target, this function is likely to fail for the same reasons. If this
 * is a concern, check the return value from this function and fall back to
 * writing to stderr if you can.
 *
 * @param flags an MessageBoxFlags value.
 * @param title UTF-8 title text.
 * @param message UTF-8 message text.
 * @param window the parent window, or nullptr for no parent.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa MessageBox.Show
 */
inline void ShowSimpleMessageBox(MessageBoxFlags flags,
                                 StringParam title,
                                 StringParam message,
                                 WindowRef window)
{
  CheckError(SDL_ShowSimpleMessageBox(flags, title, message, window.get()));
}

/// @}

} // namespace SDL

#endif /* SDL3PP_MESSAGEBOX_H_ */
