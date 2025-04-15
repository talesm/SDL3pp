#ifndef SDL3PP_MOUSE_H_
#define SDL3PP_MOUSE_H_

#include <SDL3/SDL_mouse.h>
#include "SDL3pp_stdinc.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 *
 * @defgroup CategoryMouse Mouse Support
 *
 * Any GUI application has to deal with the mouse, and SDL provides functions
 * to manage mouse input and the displayed cursor.
 *
 * Most interactions with the mouse will come through the event subsystem.
 * Moving a mouse generates an EVENT_MOUSE_MOTION event, pushing a button
 * generates EVENT_MOUSE_BUTTON_DOWN, etc, but one can also query the
 * current state of the mouse at any time with GetMouseState().
 *
 * For certain games, it's useful to disassociate the mouse cursor from mouse
 * input. An FPS, for example, would not want the player's motion to stop as
 * the mouse hits the edge of the window. For these scenarios, use
 * WindowBase.SetRelativeMouseMode(), which hides the cursor, grabs mouse input
 * to the window, and reads mouse input no matter how far it moves.
 *
 * Games that want the system to track the mouse but want to draw their own
 * cursor can use HideCursor() and ShowCursor(). It might be more
 * efficient to let the system manage the cursor, if possible, using
 * SetCursor() with a custom image made through CursorBase.CursorBase(),
 * or perhaps just a specific system cursor from CursorBase.CursorBase().
 *
 * SDL can, on many platforms, differentiate between multiple connected mice,
 * allowing for interesting input scenarios and multiplayer games. They can be
 * enumerated with GetMice(), and SDL will send EVENT_MOUSE_ADDED and
 * EVENT_MOUSE_REMOVED events as they are connected and unplugged.
 *
 * Since many apps only care about basic mouse input, SDL offers a virtual
 * mouse device for touch and pen input, which often can make a desktop
 * application work on a touchscreen phone without any code changes. Apps that
 * care about touch/pen separately from mouse input should filter out events
 * with a `which` field of SDL_TOUCH_MOUSEID/SDL_PEN_MOUSEID.
 *
 * @{
 */

// Forward decl
struct CursorBase;

// Forward decl
struct CursorRef;

// Forward decl
struct Cursor;

/**
 * Cursor types for CursorBase.CursorBase().
 *
 * @since This enum is available since SDL 3.2.0.
 */
using SystemCursor = SDL_SystemCursor;

constexpr SystemCursor SYSTEM_CURSOR_DEFAULT =
  SDL_SYSTEM_CURSOR_DEFAULT; ///< Default cursor.  Usually an arrow.

constexpr SystemCursor SYSTEM_CURSOR_TEXT =
  SDL_SYSTEM_CURSOR_TEXT; ///< Text selection.  Usually an I-beam.

/**
 * Wait.  Usually an hourglass or watch or spinning ball.
 */
constexpr SystemCursor SYSTEM_CURSOR_WAIT = SDL_SYSTEM_CURSOR_WAIT;

constexpr SystemCursor SYSTEM_CURSOR_CROSSHAIR =
  SDL_SYSTEM_CURSOR_CROSSHAIR; ///< Crosshair.

/**
 * Program is busy but still interactive.  Usually it's WAIT with an arrow.
 */
constexpr SystemCursor SYSTEM_CURSOR_PROGRESS = SDL_SYSTEM_CURSOR_PROGRESS;

constexpr SystemCursor SYSTEM_CURSOR_NWSE_RESIZE =
  SDL_SYSTEM_CURSOR_NWSE_RESIZE; ///< Double arrow pointing northwest and
                                 ///< southeast.

constexpr SystemCursor SYSTEM_CURSOR_NESW_RESIZE =
  SDL_SYSTEM_CURSOR_NESW_RESIZE; ///< Double arrow pointing northeast and
                                 ///< southwest.

constexpr SystemCursor SYSTEM_CURSOR_EW_RESIZE =
  SDL_SYSTEM_CURSOR_EW_RESIZE; ///< Double arrow pointing west and east.

constexpr SystemCursor SYSTEM_CURSOR_NS_RESIZE =
  SDL_SYSTEM_CURSOR_NS_RESIZE; ///< Double arrow pointing north and south.

/**
 * Four pointed arrow pointing north, south, east, and west.
 */
constexpr SystemCursor SYSTEM_CURSOR_MOVE = SDL_SYSTEM_CURSOR_MOVE;

/**
 * Not permitted.  Usually a slashed circle or crossbones.
 */
constexpr SystemCursor SYSTEM_CURSOR_NOT_ALLOWED =
  SDL_SYSTEM_CURSOR_NOT_ALLOWED;

/**
 * Pointer that indicates a link.  Usually a pointing hand.
 */
constexpr SystemCursor SYSTEM_CURSOR_POINTER = SDL_SYSTEM_CURSOR_POINTER;

/**
 * Window resize top-left.  This may be a single arrow or a double arrow like
 * NWSE_RESIZE.
 */
constexpr SystemCursor SYSTEM_CURSOR_NW_RESIZE = SDL_SYSTEM_CURSOR_NW_RESIZE;

constexpr SystemCursor SYSTEM_CURSOR_N_RESIZE =
  SDL_SYSTEM_CURSOR_N_RESIZE; ///< Window resize top.  May be NS_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_NE_RESIZE =
  SDL_SYSTEM_CURSOR_NE_RESIZE; ///< Window resize top-right.  May be
                               ///< NESW_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_E_RESIZE =
  SDL_SYSTEM_CURSOR_E_RESIZE; ///< Window resize right.  May be EW_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_SE_RESIZE =
  SDL_SYSTEM_CURSOR_SE_RESIZE; ///< Window resize bottom-right.  May be
                               ///< NWSE_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_S_RESIZE =
  SDL_SYSTEM_CURSOR_S_RESIZE; ///< Window resize bottom.  May be NS_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_SW_RESIZE =
  SDL_SYSTEM_CURSOR_SW_RESIZE; ///< Window resize bottom-left.  May be
                               ///< NESW_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_W_RESIZE =
  SDL_SYSTEM_CURSOR_W_RESIZE; ///< Window resize left.  May be EW_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_COUNT = SDL_SYSTEM_CURSOR_COUNT; ///< COUNT

/**
 * This is a unique ID for a mouse for the time it is connected to the system,
 * and is never reused for the lifetime of the application.
 *
 * If the mouse is disconnected and reconnected, it will get a new ID.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using MouseID = SDL_MouseID;

/**
 * The structure used to identify an SDL cursor.
 *
 * This is opaque data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Cursor
 * @sa CursorRef
 */
struct CursorBase : Resource<SDL_Cursor*>
{
  using Resource::Resource;

  /**
   * Create a cursor using the specified bitmap data and mask (in MSB format).
   *
   * `mask` has to be in MSB (Most Significant Bit) format.
   *
   * The cursor width (`w`) must be a multiple of 8 bits.
   *
   * The cursor is created in black and white according to the following:
   *
   * - data=0, mask=1: white
   * - data=1, mask=1: black
   * - data=0, mask=0: transparent
   * - data=1, mask=0: inverted color if possible, black if not.
   *
   * Cursors created with this function must be freed with CursorRef.reset().
   *
   * If you want to have a color cursor, or create your cursor from an
   * SurfaceBase, you should use CursorBase.CursorBase(). Alternately, you can
   * hide the cursor and draw your own as part of your game's rendering, but it
   * will be bound to the framerate.
   *
   * Also, CursorBase.CursorBase() is available, which provides several
   * readily-available system cursors to pick from.
   *
   * @param data the color value for each pixel of the cursor.
   * @param mask the mask value for each pixel of the cursor.
   * @param w the width of the cursor.
   * @param h the height of the cursor.
   * @param hot_x the x-axis offset from the left of the cursor image to the
   *              mouse x position, in the range of 0 to `w` - 1.
   * @param hot_y the y-axis offset from the top of the cursor image to the
   *              mouse y position, in the range of 0 to `h` - 1.
   * @post a new cursor with the specified parameters on success or nullptr on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa CursorBase.CursorBase
   * @sa CursorBase.CursorBase
   * @sa CursorRef.reset
   * @sa SetCursor
   */
  CursorBase(const Uint8* data,
             const Uint8* mask,
             int w,
             int h,
             int hot_x,
             int hot_y)
    : Resource(SDL_CreateCursor(data, mask, w, h, hot_x, hot_y))
  {
  }

  /**
   * Create a color cursor.
   *
   * If this function is passed a surface with alternate representations, the
   * surface will be interpreted as the content to be used for 100% display
   * scale, and the alternate representations will be used for high DPI
   * situations. For example, if the original surface is 32x32, then on a 2x
   * macOS display or 200% display scale on Windows, a 64x64 version of the
   * image will be used, if available. If a matching version of the image isn't
   * available, the closest larger size image will be downscaled to the
   * appropriate size and be used instead, if available. Otherwise, the closest
   * smaller image will be upscaled and be used instead.
   *
   * @param surface an SurfaceBase structure representing the cursor image.
   * @param hot_x the x position of the cursor hot spot.
   * @param hot_y the y position of the cursor hot spot.
   * @post the new cursor on success or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa CursorBase.CursorBase
   * @sa CursorBase.CursorBase
   * @sa CursorRef.reset
   * @sa SetCursor
   */
  CursorBase(SurfaceBase& surface, int hot_x, int hot_y)
    : Resource(SDL_CreateColorCursor(surface.get(), hot_x, hot_y))
  {
  }

  /**
   * Create a system cursor.
   *
   * @param id an SystemCursor enum value.
   * @post a cursor on success or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa CursorRef.reset
   */
  CursorBase(SystemCursor id)
    : Resource(SDL_CreateSystemCursor(id))
  {
  }
};

/**
 * Handle to a non owned cursor
 *
 * @cat resource
 *
 * @sa CursorBase
 * @sa Cursor
 */
struct CursorRef : CursorBase
{
  using CursorBase::CursorBase;

  /**
   * Copy constructor.
   */
  constexpr CursorRef(const CursorRef& other)
    : CursorBase(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr CursorRef(CursorRef&& other)
    : CursorBase(other.release())
  {
  }

  /**
   * Default constructor
   */
  constexpr ~CursorRef() = default;

  /**
   * Assignment operator.
   */
  CursorRef& operator=(CursorRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Free a previously-created cursor.
   *
   * Use this function to free cursor resources created with
   * CursorBase.CursorBase(), CursorBase.CursorBase() or
   * CursorBase.CursorBase().
   *
   * @param cursor the cursor to free.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa CursorBase.CursorBase
   * @sa CursorBase.CursorBase
   * @sa CursorBase.CursorBase
   */
  void reset(SDL_Cursor* newResource = {})
  {
    SDL_DestroyCursor(release(newResource));
  }
};

/**
 * Handle to an owned cursor
 *
 * @cat resource
 *
 * @sa CursorBase
 * @sa CursorRef
 */
struct Cursor : CursorRef
{
  using CursorRef::CursorRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Cursor(SDL_Cursor* resource = {})
    : CursorRef(resource)
  {
  }

  constexpr Cursor(const Cursor& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Cursor(Cursor&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Cursor() { reset(); }

  /**
   * Assignment operator.
   */
  Cursor& operator=(Cursor other)
  {
    reset(other.release());
    return *this;
  }
};

using MouseButton = Uint8;

constexpr MouseButton BUTTON_LEFT = SDL_BUTTON_LEFT; ///< LEFT

constexpr MouseButton BUTTON_MIDDLE = SDL_BUTTON_MIDDLE; ///< MIDDLE

constexpr MouseButton BUTTON_RIGHT = SDL_BUTTON_RIGHT; ///< RIGHT

constexpr MouseButton BUTTON_X1 = SDL_BUTTON_X1; ///< X1

constexpr MouseButton BUTTON_X2 = SDL_BUTTON_X2; ///< X2

/**
 * Scroll direction types for the Scroll event
 *
 * @since This enum is available since SDL 3.2.0.
 */
using MouseWheelDirection = SDL_MouseWheelDirection;

constexpr MouseWheelDirection MOUSEWHEEL_NORMAL =
  SDL_MOUSEWHEEL_NORMAL; ///< The scroll direction is normal.

constexpr MouseWheelDirection MOUSEWHEEL_FLIPPED =
  SDL_MOUSEWHEEL_FLIPPED; ///< The scroll direction is flipped / natural.

/**
 * A bitmask of pressed mouse buttons, as reported by GetMouseState, etc.
 *
 * - Button 1: Left mouse button
 * - Button 2: Middle mouse button
 * - Button 3: Right mouse button
 * - Button 4: Side mouse button 1
 * - Button 5: Side mouse button 2
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GetMouseState
 * @sa GetGlobalMouseState
 * @sa GetRelativeMouseState
 */
using MouseButtonFlags = Uint32;

constexpr MouseButtonFlags ButtonMask(MouseButton button)
{
  return SDL_BUTTON_MASK(button);
}

constexpr MouseButtonFlags BUTTON_LMASK = SDL_BUTTON_LMASK; ///< LMASK

constexpr MouseButtonFlags BUTTON_MMASK = SDL_BUTTON_MMASK; ///< MMASK

constexpr MouseButtonFlags BUTTON_RMASK = SDL_BUTTON_RMASK; ///< RMASK

constexpr MouseButtonFlags BUTTON_X1MASK = SDL_BUTTON_X1MASK; ///< X1MASK

constexpr MouseButtonFlags BUTTON_X2MASK = SDL_BUTTON_X2MASK; ///< X2MASK

/**
 * Return whether a mouse is currently connected.
 *
 * @returns true if a mouse is connected, false otherwise.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetMice
 */
inline bool HasMouse() { return SDL_HasMouse(); }

/**
 * Get a list of currently connected mice.
 *
 * Note that this will include any device or virtual driver that includes
 * mouse functionality, including some game controllers, KVM switches, etc.
 * You should wait for input from a device before you consider it actively in
 * use.
 *
 * @param count a pointer filled in with the number of mice returned, may be
 *              nullptr.
 * @returns a 0 terminated array of mouse instance IDs or nullptr on failure;
 *          call GetError() for more information. This should be freed
 *          with free() when it is no longer needed.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetMouseNameForID
 * @sa HasMouse
 */
inline OwnArray<MouseID> GetMice()
{
  int count;
  auto data = SDL_GetMice(&count);
  if (!data) return {};
  return OwnArray<MouseID>{data, size_t(count)};
}

/**
 * Get the name of a mouse.
 *
 * This function returns "" if the mouse doesn't have a name.
 *
 * @param instance_id the mouse instance ID.
 * @returns the name of the selected mouse, or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetMice
 */
inline const char* GetMouseNameForID(MouseID instance_id)
{
  return SDL_GetMouseNameForID(instance_id);
}

/**
 * Get the window which currently has mouse focus.
 *
 * @returns the window with mouse focus.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline WindowRef GetMouseFocus() { return SDL_GetMouseFocus(); }

/**
 * Query SDL's cache for the synchronous mouse button state and the
 * window-relative SDL-cursor position.
 *
 * This function returns the cached synchronous state as SDL understands it
 * from the last pump of the event queue.
 *
 * To query the platform for immediate asynchronous state, use
 * GetGlobalMouseState.
 *
 * Passing non-nullptr pointers to `x` or `y` will write the destination with
 * respective x or y coordinates relative to the focused window.
 *
 * In Relative Mode, the SDL-cursor's position usually contradicts the
 * platform-cursor's position as manually calculated from
 * GetGlobalMouseState() and WindowBase.GetPosition.
 *
 * @param x a pointer to receive the SDL-cursor's x-position from the focused
 *          window's top left corner, can be nullptr if unused.
 * @param y a pointer to receive the SDL-cursor's y-position from the focused
 *          window's top left corner, can be nullptr if unused.
 * @returns a 32-bit bitmask of the button state that can be bitwise-compared
 *          against the ButtonMask(X) macro.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGlobalMouseState
 * @sa GetRelativeMouseState
 */
inline MouseButtonFlags GetMouseState(float* x, float* y)
{
  return SDL_GetMouseState(x, y);
}

/**
 * Query the platform for the asynchronous mouse button state and the
 * desktop-relative platform-cursor position.
 *
 * This function immediately queries the platform for the most recent
 * asynchronous state, more costly than retrieving SDL's cached state in
 * GetMouseState().
 *
 * Passing non-nullptr pointers to `x` or `y` will write the destination with
 * respective x or y coordinates relative to the desktop.
 *
 * In Relative Mode, the platform-cursor's position usually contradicts the
 * SDL-cursor's position as manually calculated from GetMouseState() and
 * WindowBase.GetPosition.
 *
 * This function can be useful if you need to track the mouse outside of a
 * specific window and CaptureMouse() doesn't fit your needs. For example,
 * it could be useful if you need to track the mouse while dragging a window,
 * where coordinates relative to a window might not be in sync at all times.
 *
 * @param x a pointer to receive the platform-cursor's x-position from the
 *          desktop's top left corner, can be nullptr if unused.
 * @param y a pointer to receive the platform-cursor's y-position from the
 *          desktop's top left corner, can be nullptr if unused.
 * @returns a 32-bit bitmask of the button state that can be bitwise-compared
 *          against the ButtonMask(X) macro.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa CaptureMouse
 * @sa GetMouseState
 * @sa GetGlobalMouseState
 */
inline MouseButtonFlags GetGlobalMouseState(float* x, float* y)
{
  return SDL_GetGlobalMouseState(x, y);
}

/**
 * Query SDL's cache for the synchronous mouse button state and accumulated
 * mouse delta since last call.
 *
 * This function returns the cached synchronous state as SDL understands it
 * from the last pump of the event queue.
 *
 * To query the platform for immediate asynchronous state, use
 * GetGlobalMouseState.
 *
 * Passing non-nullptr pointers to `x` or `y` will write the destination with
 * respective x or y deltas accumulated since the last call to this function
 * (or since event initialization).
 *
 * This function is useful for reducing overhead by processing relative mouse
 * inputs in one go per-frame instead of individually per-event, at the
 * expense of losing the order between events within the frame (e.g. quickly
 * pressing and releasing a button within the same frame).
 *
 * @param x a pointer to receive the x mouse delta accumulated since last
 *          call, can be nullptr if unused.
 * @param y a pointer to receive the y mouse delta accumulated since last
 *          call, can be nullptr if unused.
 * @returns a 32-bit bitmask of the button state that can be bitwise-compared
 *          against the ButtonMask(X) macro.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetMouseState
 * @sa GetGlobalMouseState
 */
inline MouseButtonFlags GetRelativeMouseState(float* x, float* y)
{
  return SDL_GetRelativeMouseState(x, y);
}

/**
 * Move the mouse cursor to the given position within the window.
 *
 * This function generates a mouse motion event if relative mode is not
 * enabled. If relative mode is enabled, you can force mouse events for the
 * warp by setting the SDL_HINT_MOUSE_RELATIVE_WARP_MOTION hint.
 *
 * Note that this function will appear to succeed, but not actually move the
 * mouse when used over Microsoft Remote Desktop.
 *
 *               mouse focus.
 * @param x the x coordinate within the window.
 * @param y the y coordinate within the window.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa WarpMouse
 */
inline void WindowBase::WarpMouse(float x, float y)
{
  SDL_WarpMouseInWindow(get(), x, y);
}

/**
 * Move the mouse to the given position in global screen space.
 *
 * This function generates a mouse motion event.
 *
 * A failure of this function usually means that it is unsupported by a
 * platform.
 *
 * Note that this function will appear to succeed, but not actually move the
 * mouse when used over Microsoft Remote Desktop.
 *
 * @param x the x coordinate.
 * @param y the y coordinate.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa WindowBase.WarpMouse
 */
inline bool WarpMouse(float x, float y) { return SDL_WarpMouseGlobal(x, y); }

/**
 * Set relative mouse mode for a window.
 *
 * While the window has focus and relative mouse mode is enabled, the cursor
 * is hidden, the mouse position is constrained to the window, and SDL will
 * report continuous relative mouse motion even if the mouse is at the edge of
 * the window.
 *
 * If you'd like to keep the mouse position fixed while in relative mode you
 * can use WindowBase.SetMouseRect(). If you'd like the cursor to be at a
 * specific location when relative mode ends, you should use
 * WindowBase.WarpMouse() before disabling relative mode.
 *
 * This function will flush any pending mouse motion for this window.
 *
 * @param enabled true to enable relative mode, false to disable.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa WindowBase.GetRelativeMouseMode
 */
inline bool WindowBase::SetRelativeMouseMode(bool enabled)
{
  return SDL_SetWindowRelativeMouseMode(get(), enabled);
}

/**
 * Query whether relative mouse mode is enabled for a window.
 *
 * @returns true if relative mode is enabled for a window or false otherwise.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa WindowBase.SetRelativeMouseMode
 */
inline bool WindowBase::GetRelativeMouseMode() const
{
  return SDL_GetWindowRelativeMouseMode(get());
}

/**
 * Capture the mouse and to track input outside an SDL window.
 *
 * Capturing enables your app to obtain mouse events globally, instead of just
 * within your window. Not all video targets support this function. When
 * capturing is enabled, the current window will get all mouse events, but
 * unlike relative mode, no change is made to the cursor and it is not
 * restrained to your window.
 *
 * This function may also deny mouse input to other windows--both those in
 * your application and others on the system--so you should use this function
 * sparingly, and in small bursts. For example, you might want to track the
 * mouse while the user is dragging something, until the user releases a mouse
 * button. It is not recommended that you capture the mouse for long periods
 * of time, such as the entire time your app is running. For that, you should
 * probably use WindowBase.SetRelativeMouseMode() or WindowBase.SetMouseGrab(),
 * depending on your goals.
 *
 * While captured, mouse events still report coordinates relative to the
 * current (foreground) window, but those coordinates may be outside the
 * bounds of the window (including negative values). Capturing is only allowed
 * for the foreground window. If the window loses focus while capturing, the
 * capture will be disabled automatically.
 *
 * While capturing is enabled, the current window will have the
 * `WINDOW_MOUSE_CAPTURE` flag set.
 *
 * Please note that SDL will attempt to "auto capture" the mouse while the
 * user is pressing a button; this is to try and make mouse behavior more
 * consistent between platforms, and deal with the common case of a user
 * dragging the mouse outside of the window. This means that if you are
 * calling CaptureMouse() only to deal with this situation, you do not
 * have to (although it is safe to do so). If this causes problems for your
 * app, you can disable auto capture by setting the
 * `SDL_HINT_MOUSE_AUTO_CAPTURE` hint to zero.
 *
 * @param enabled true to enable capturing, false to disable.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGlobalMouseState
 */
inline bool CaptureMouse(bool enabled) { return SDL_CaptureMouse(enabled); }

/**
 * Set the active cursor.
 *
 * This function sets the currently active cursor to the specified one. If the
 * cursor is currently visible, the change will be immediately represented on
 * the display. SetCursor(nullptr) can be used to force cursor redraw, if
 * this is desired for any reason.
 *
 * @param cursor a cursor to make active.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetCursor
 */
inline bool SetCursor(CursorBase& cursor)
{
  return SDL_SetCursor(cursor.get());
}

/**
 * Get the active cursor.
 *
 * This function returns a pointer to the current cursor which is owned by the
 * library. It is not necessary to free the cursor with CursorRef.reset().
 *
 * @returns the active cursor or nullptr if there is no mouse.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetCursor
 */
inline CursorRef GetCursor() { return SDL_GetCursor(); }

/**
 * Get the default cursor.
 *
 * You do not have to call CursorRef.reset() on the return value, but it is
 * safe to do so.
 *
 * @returns the default cursor on success or nullptr on failuree; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline CursorRef GetDefaultCursor() { return SDL_GetDefaultCursor(); }

/**
 * Show the cursor.
 *
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa CursorVisible
 * @sa HideCursor
 */
inline bool ShowCursor() { return SDL_ShowCursor(); }

/**
 * Hide the cursor.
 *
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa CursorVisible
 * @sa ShowCursor
 */
inline bool HideCursor() { return SDL_HideCursor(); }

/**
 * Return whether the cursor is currently being shown.
 *
 * @returns `true` if the cursor is being shown, or `false` if the cursor is
 *          hidden.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HideCursor
 * @sa ShowCursor
 */
inline bool CursorVisible() { return SDL_CursorVisible(); }

/// @}
} // namespace SDL

#endif /* SDL3PP_MOUSE_H_ */
