#ifndef SDL3PP_MOUSE_H_
#define SDL3PP_MOUSE_H_

#include <SDL3/SDL_mouse.h>
#include "SDL3pp_stdinc.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategoryMouse Mouse Support
 *
 * Any GUI application has to deal with the mouse, and SDL provides functions to
 * manage mouse input and the displayed cursor.
 *
 * Most interactions with the mouse will come through the event subsystem.
 * Moving a mouse generates an EVENT_MOUSE_MOTION event, pushing a button
 * generates EVENT_MOUSE_BUTTON_DOWN, etc, but one can also query the current
 * state of the mouse at any time with GetMouseState().
 *
 * For certain games, it's useful to disassociate the mouse cursor from mouse
 * input. An FPS, for example, would not want the player's motion to stop as the
 * mouse hits the edge of the window. For these scenarios, use
 * Window.SetRelativeMouseMode(), which hides the cursor, grabs mouse input to
 * the window, and reads mouse input no matter how far it moves.
 *
 * Games that want the system to track the mouse but want to draw their own
 * cursor can use HideCursor() and ShowCursor(). It might be more efficient to
 * let the system manage the cursor, if possible, using Cursor.Set() with a
 * custom image made through Cursor.Cursor(), or perhaps just a specific system
 * cursor from Cursor.Cursor().
 *
 * SDL can, on many platforms, differentiate between multiple connected mice,
 * allowing for interesting input scenarios and multiplayer games. They can be
 * enumerated with GetMice(), and SDL will send EVENT_MOUSE_ADDED and
 * EVENT_MOUSE_REMOVED events as they are connected and unplugged.
 *
 * Since many apps only care about basic mouse input, SDL offers a virtual mouse
 * device for touch and pen input, which often can make a desktop application
 * work on a touchscreen phone without any code changes. Apps that care about
 * touch/pen separately from mouse input should filter out events with a `which`
 * field of TOUCH_MOUSEID/PEN_MOUSEID.
 *
 * @{
 */

// Forward decl
struct Cursor;

/// Alias to raw representation for Cursor.
using CursorRaw = SDL_Cursor*;

// Forward decl
struct CursorRef;

/// Safely wrap Cursor for non owning parameters
struct CursorParam
{
  CursorRaw value; ///< parameter's CursorRaw

  /// Constructs from CursorRaw
  constexpr CursorParam(CursorRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr CursorParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const CursorParam& other) const = default;

  /// Converts to underlying CursorRaw
  constexpr operator CursorRaw() const { return value; }
};

/**
 * Cursor types for Cursor.Cursor().
 *
 * @since This enum is available since SDL 3.2.0.
 */
using SystemCursor = SDL_SystemCursor;

constexpr SystemCursor SYSTEM_CURSOR_DEFAULT =
  SDL_SYSTEM_CURSOR_DEFAULT; ///< Default cursor. Usually an arrow.

constexpr SystemCursor SYSTEM_CURSOR_TEXT =
  SDL_SYSTEM_CURSOR_TEXT; ///< Text selection. Usually an I-beam.

/// Wait. Usually an hourglass or watch or spinning ball.
constexpr SystemCursor SYSTEM_CURSOR_WAIT = SDL_SYSTEM_CURSOR_WAIT;

constexpr SystemCursor SYSTEM_CURSOR_CROSSHAIR =
  SDL_SYSTEM_CURSOR_CROSSHAIR; ///< Crosshair.

/// Program is busy but still interactive. Usually it's WAIT with an arrow.
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

/// Four pointed arrow pointing north, south, east, and west.
constexpr SystemCursor SYSTEM_CURSOR_MOVE = SDL_SYSTEM_CURSOR_MOVE;

/// Not permitted. Usually a slashed circle or crossbones.
constexpr SystemCursor SYSTEM_CURSOR_NOT_ALLOWED =
  SDL_SYSTEM_CURSOR_NOT_ALLOWED;

/// Pointer that indicates a link. Usually a pointing hand.
constexpr SystemCursor SYSTEM_CURSOR_POINTER = SDL_SYSTEM_CURSOR_POINTER;

/**
 * Window resize top-left. This may be a single arrow or a double arrow like
 * NWSE_RESIZE.
 */
constexpr SystemCursor SYSTEM_CURSOR_NW_RESIZE = SDL_SYSTEM_CURSOR_NW_RESIZE;

constexpr SystemCursor SYSTEM_CURSOR_N_RESIZE =
  SDL_SYSTEM_CURSOR_N_RESIZE; ///< Window resize top. May be NS_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_NE_RESIZE =
  SDL_SYSTEM_CURSOR_NE_RESIZE; ///< Window resize top-right. May be NESW_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_E_RESIZE =
  SDL_SYSTEM_CURSOR_E_RESIZE; ///< Window resize right. May be EW_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_SE_RESIZE =
  SDL_SYSTEM_CURSOR_SE_RESIZE; ///< Window resize bottom-right. May be
                               ///< NWSE_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_S_RESIZE =
  SDL_SYSTEM_CURSOR_S_RESIZE; ///< Window resize bottom. May be NS_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_SW_RESIZE =
  SDL_SYSTEM_CURSOR_SW_RESIZE; ///< Window resize bottom-left. May be
                               ///< NESW_RESIZE.

constexpr SystemCursor SYSTEM_CURSOR_W_RESIZE =
  SDL_SYSTEM_CURSOR_W_RESIZE; ///< Window resize left. May be EW_RESIZE.

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
 */
class Cursor
{
  CursorRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Cursor(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from CursorParam.
   *
   * @param resource a CursorRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Cursor(const CursorRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Cursor(const Cursor& other) noexcept = default;

public:
  /// Move constructor
  constexpr Cursor(Cursor&& other) noexcept
    : Cursor(other.release())
  {
  }

  constexpr Cursor(const CursorRef& other) = delete;

  constexpr Cursor(CursorRef&& other) = delete;

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
   * Cursors created with this function must be freed with Cursor.Destroy().
   *
   * If you want to have a color cursor, or create your cursor from an Surface,
   * you should use Cursor.Cursor(). Alternately, you can hide the cursor and
   * draw your own as part of your game's rendering, but it will be bound to the
   * framerate.
   *
   * Also, Cursor.Cursor() is available, which provides several
   * readily-available system cursors to pick from.
   *
   * @param data the color value for each pixel of the cursor.
   * @param mask the mask value for each pixel of the cursor.
   * @param size the width and height of the cursor.
   * @param hot the x position of the cursor hot spot, from the top-left, in the
   *            range of 0 to `size.x` - 1 and 0 to `size.y` - 1.
   * @post a new cursor with the specified parameters on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa CreateAnimatedCursor
   * @sa Cursor.Cursor
   * @sa Cursor.Cursor
   * @sa Cursor.Destroy
   * @sa Cursor.Set
   */
  Cursor(const Uint8* data,
         const Uint8* mask,
         const PointRaw& size,
         const PointRaw& hot)
    : m_resource(
        CheckError(SDL_CreateCursor(data, mask, size.x, size.y, hot.x, hot.y)))
  {
  }

  /**
   * Create a color cursor.
   *
   * If this function is passed a surface with alternate representations added
   * with Surface.AddAlternateImage(), the surface will be interpreted as the
   * content to be used for 100% display scale, and the alternate
   * representations will be used for high DPI situations if
   * SDL_HINT_MOUSE_DPI_SCALE_CURSORS is enabled. For example, if the original
   * surface is 32x32, then on a 2x macOS display or 200% display scale on
   * Windows, a 64x64 version of the image will be used, if available. If a
   * matching version of the image isn't available, the closest larger size
   * image will be downscaled to the appropriate size and be used instead, if
   * available. Otherwise, the closest smaller image will be upscaled and be
   * used instead.
   *
   * @param surface an Surface structure representing the cursor image.
   * @param hot the x, y position of the cursor hot spot.
   * @post the new cursor on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.AddAlternateImage
   * @sa CreateAnimatedCursor
   * @sa Cursor.Cursor
   * @sa Cursor.Cursor
   * @sa Cursor.Destroy
   * @sa Cursor.Set
   */
  Cursor(SurfaceParam surface, const PointRaw& hot)
    : m_resource(CheckError(SDL_CreateColorCursor(surface, hot.x, hot.y)))
  {
  }

  /**
   * Create a system cursor.
   *
   * @param id an SystemCursor enum value.
   * @post a cursor on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Cursor.Destroy
   */
  Cursor(SystemCursor id)
    : m_resource(CheckError(SDL_CreateSystemCursor(id)))
  {
  }

  /// Destructor
  ~Cursor() { SDL_DestroyCursor(m_resource); }

  /// Assignment operator.
  constexpr Cursor& operator=(Cursor&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Cursor& operator=(const Cursor& other) noexcept = default;

public:
  /// Retrieves underlying CursorRaw.
  constexpr CursorRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying CursorRaw and clear this.
  constexpr CursorRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Cursor& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to CursorParam
  constexpr operator CursorParam() const noexcept { return {m_resource}; }

  /**
   * Free a previously-created cursor.
   *
   * Use this function to free cursor resources created with Cursor.Cursor(),
   * Cursor.Cursor() or Cursor.Cursor().
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa CreateAnimatedCursor
   * @sa Cursor.Cursor
   * @sa Cursor.Cursor
   * @sa Cursor.Cursor
   */
  void Destroy();

  /**
   * Set the active cursor.
   *
   * This function sets the currently active cursor to the specified one. If the
   * cursor is currently visible, the change will be immediately represented on
   * the display. Cursor.Set(nullptr) can be used to force cursor redraw, if
   * this is desired for any reason.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetCursor
   */
  void Set();
};

/// Semi-safe reference for Cursor.
struct CursorRef : Cursor
{
  using Cursor::Cursor;

  /**
   * Constructs from CursorParam.
   *
   * @param resource a CursorRaw or Cursor.
   *
   * This does not takes ownership!
   */
  CursorRef(CursorParam resource) noexcept
    : Cursor(resource.value)
  {
  }

  /**
   * Constructs from CursorParam.
   *
   * @param resource a CursorRaw or Cursor.
   *
   * This does not takes ownership!
   */
  CursorRef(CursorRaw resource) noexcept
    : Cursor(resource)
  {
  }

  /// Copy constructor.
  CursorRef(const CursorRef& other) noexcept
    : Cursor(other.get())
  {
  }

  /// Destructor
  ~CursorRef() { release(); }
};

/**
 * Scroll direction types for the Scroll event
 *
 * @since This enum is available since SDL 3.2.0.
 */
using MouseWheelDirection = SDL_MouseWheelDirection;

constexpr MouseWheelDirection MOUSEWHEEL_NORMAL =
  SDL_MOUSEWHEEL_NORMAL; ///< The scroll direction is normal

constexpr MouseWheelDirection MOUSEWHEEL_FLIPPED =
  SDL_MOUSEWHEEL_FLIPPED; ///< The scroll direction is flipped / natural

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Animated cursor frame info.
 *
 * @since This struct is available since SDL 3.4.0.
 */
using CursorFrameInfo = SDL_CursorFrameInfo;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Represents a button index.
 *
 */
using MouseButton = Uint8;

constexpr MouseButton BUTTON_LEFT = SDL_BUTTON_LEFT; ///< Left button

constexpr MouseButton BUTTON_MIDDLE = SDL_BUTTON_MIDDLE; ///< Middle button

constexpr MouseButton BUTTON_RIGHT = SDL_BUTTON_RIGHT; ///< Right button

constexpr MouseButton BUTTON_X1 = SDL_BUTTON_X1; ///< X1 button

constexpr MouseButton BUTTON_X2 = SDL_BUTTON_X2; ///< X2 button

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

constexpr MouseButtonFlags BUTTON_LMASK = SDL_BUTTON_LMASK; ///< LMASK

constexpr MouseButtonFlags BUTTON_MMASK = SDL_BUTTON_MMASK; ///< MMASK

constexpr MouseButtonFlags BUTTON_RMASK = SDL_BUTTON_RMASK; ///< RMASK

constexpr MouseButtonFlags BUTTON_X1MASK = SDL_BUTTON_X1MASK; ///< X1MASK

constexpr MouseButtonFlags BUTTON_X2MASK = SDL_BUTTON_X2MASK; ///< X2MASK

/** Returns mask for button */
constexpr MouseButtonFlags ButtonMask(MouseButton button)
{
  return SDL_BUTTON_MASK(button);
}

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * A callback used to transform mouse motion delta from raw values.
 *
 * This is called during SDL's handling of platform mouse events to scale the
 * values of the resulting motion delta.
 *
 * @param userdata what was passed as `userdata` to SetRelativeMouseTransform().
 * @param timestamp the associated time at which this mouse motion event was
 *                  received.
 * @param window the associated window to which this mouse motion event was
 *               addressed.
 * @param mouseID the associated mouse from which this mouse motion event was
 *                emitted.
 * @param x pointer to a variable that will be treated as the resulting x-axis
 *          motion.
 * @param y pointer to a variable that will be treated as the resulting y-axis
 *          motion.
 *
 * @threadsafety This callback is called by SDL's internal mouse input
 *               processing procedure, which may be a thread separate from the
 *               main event loop that is run at realtime priority. Stalling this
 *               thread with too much work in the callback can therefore
 *               potentially freeze the entire system. Care should be taken with
 *               proper synchronization practices when adding other side effects
 *               beyond mutation of the x and y values.
 *
 * @since This datatype is available since SDL 3.4.0.
 *
 * @sa SetRelativeMouseTransform
 */
using MouseMotionTransformCallback = void(SDLCALL*)(void* userdata,
                                                    Uint64 timestamp,
                                                    SDL_Window* window,
                                                    MouseID mouseID,
                                                    float* x,
                                                    float* y);

/**
 * A callback used to transform mouse motion delta from raw values.
 *
 * This is called during SDL's handling of platform mouse events to scale the
 * values of the resulting motion delta.
 *
 * @param timestamp the associated time at which this mouse motion event was
 *                  received.
 * @param window the associated window to which this mouse motion event was
 *               addressed.
 * @param mouseID the associated mouse from which this mouse motion event was
 *                emitted.
 * @param x pointer to a variable that will be treated as the resulting x-axis
 *          motion.
 * @param y pointer to a variable that will be treated as the resulting y-axis
 *          motion.
 *
 * @threadsafety This callback is called by SDL's internal mouse input
 *               processing procedure, which may be a thread separate from the
 *               main event loop that is run at realtime priority. Stalling this
 *               thread with too much work in the callback can therefore
 *               potentially freeze the entire system. Care should be taken with
 *               proper synchronization practices when adding other side effects
 *               beyond mutation of the x and y values.
 *
 * @since This datatype is available since SDL 3.4.0.
 *
 * @sa SetRelativeMouseTransform
 *
 * @sa MouseMotionTransformCallback
 */
using MouseMotionTransformCB = MakeFrontCallback<void(Uint64 timestamp,
                                                      SDL_Window* window,
                                                      MouseID mouseID,
                                                      float* x,
                                                      float* y)>;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

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
 * Note that this will include any device or virtual driver that includes mouse
 * functionality, including some game controllers, KVM switches, etc. You should
 * wait for input from a device before you consider it actively in use.
 *
 * @returns a 0 terminated array of mouse instance IDs.
 * @throws Error on failure.
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
  auto data = CheckError(SDL_GetMice(&count));
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
inline WindowRef GetMouseFocus() { return {SDL_GetMouseFocus()}; }

/**
 * Query SDL's cache for the synchronous mouse button state and the
 * window-relative SDL-cursor position.
 *
 * This function returns the cached synchronous state as SDL understands it from
 * the last pump of the event queue.
 *
 * To query the platform for immediate asynchronous state, use
 * GetGlobalMouseState.
 *
 * Passing non-nullptr pointers to `x` or `y` will write the destination with
 * respective x or y coordinates relative to the focused window.
 *
 * In Relative Mode, the SDL-cursor's position usually contradicts the
 * platform-cursor's position as manually calculated from GetGlobalMouseState()
 * and Window.GetPosition.
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
 * Window.GetPosition.
 *
 * This function can be useful if you need to track the mouse outside of a
 * specific window and CaptureMouse() doesn't fit your needs. For example, it
 * could be useful if you need to track the mouse while dragging a window, where
 * coordinates relative to a window might not be in sync at all times.
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
 * This function returns the cached synchronous state as SDL understands it from
 * the last pump of the event queue.
 *
 * To query the platform for immediate asynchronous state, use
 * GetGlobalMouseState.
 *
 * Passing non-nullptr pointers to `x` or `y` will write the destination with
 * respective x or y deltas accumulated since the last call to this function (or
 * since event initialization).
 *
 * This function is useful for reducing overhead by processing relative mouse
 * inputs in one go per-frame instead of individually per-event, at the expense
 * of losing the order between events within the frame (e.g. quickly pressing
 * and releasing a button within the same frame).
 *
 * @param x a pointer to receive the x mouse delta accumulated since last call,
 *          can be nullptr if unused.
 * @param y a pointer to receive the y mouse delta accumulated since last call,
 *          can be nullptr if unused.
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

inline void Window::WarpMouse(const FPointRaw& p)
{
  SDL_WarpMouseInWindow(m_resource, p.x, p.y);
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
 * @param p the x, y coordinates;
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Window.WarpMouse
 */
inline void WarpMouse(const FPointRaw& p)
{
  CheckError(SDL_WarpMouseGlobal(p.x, p.y));
}

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Set a user-defined function by which to transform relative mouse inputs.
 *
 * This overrides the relative system scale and relative speed scale hints.
 * Should be called prior to enabling relative mouse mode, fails otherwise.
 *
 * @param callback a callback used to transform relative mouse motion, or
 *                 nullptr for default behavior.
 * @param userdata a pointer that will be passed to `callback`.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.4.0.
 */
inline void SetRelativeMouseTransform(MouseMotionTransformCallback callback,
                                      void* userdata)
{
  CheckError(SDL_SetRelativeMouseTransform(callback, userdata));
}

/**
 * Set a user-defined function by which to transform relative mouse inputs.
 *
 * This overrides the relative system scale and relative speed scale hints.
 * Should be called prior to enabling relative mouse mode, fails otherwise.
 *
 * @param callback a callback used to transform relative mouse motion, or
 *                 nullptr for default behavior.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.4.0.
 */
inline void SetRelativeMouseTransform(MouseMotionTransformCB callback)
{
  SetRelativeMouseTransform(callback.wrapper, callback.data);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

inline void Window::SetRelativeMouseMode(bool enabled)
{
  CheckError(SDL_SetWindowRelativeMouseMode(m_resource, enabled));
}

inline bool Window::GetRelativeMouseMode() const
{
  return SDL_GetWindowRelativeMouseMode(m_resource);
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
 * This function may also deny mouse input to other windows--both those in your
 * application and others on the system--so you should use this function
 * sparingly, and in small bursts. For example, you might want to track the
 * mouse while the user is dragging something, until the user releases a mouse
 * button. It is not recommended that you capture the mouse for long periods of
 * time, such as the entire time your app is running. For that, you should
 * probably use Window.SetRelativeMouseMode() or Window.SetMouseGrab(),
 * depending on your goals.
 *
 * While captured, mouse events still report coordinates relative to the current
 * (foreground) window, but those coordinates may be outside the bounds of the
 * window (including negative values). Capturing is only allowed for the
 * foreground window. If the window loses focus while capturing, the capture
 * will be disabled automatically.
 *
 * While capturing is enabled, the current window will have the
 * `WINDOW_MOUSE_CAPTURE` flag set.
 *
 * Please note that SDL will attempt to "auto capture" the mouse while the user
 * is pressing a button; this is to try and make mouse behavior more consistent
 * between platforms, and deal with the common case of a user dragging the mouse
 * outside of the window. This means that if you are calling CaptureMouse() only
 * to deal with this situation, you do not have to (although it is safe to do
 * so). If this causes problems for your app, you can disable auto capture by
 * setting the `SDL_HINT_MOUSE_AUTO_CAPTURE` hint to zero.
 *
 * @param enabled true to enable capturing, false to disable.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGlobalMouseState
 */
inline void CaptureMouse(bool enabled)
{
  CheckError(SDL_CaptureMouse(enabled));
}

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
 * Cursors created with this function must be freed with Cursor.Destroy().
 *
 * If you want to have a color cursor, or create your cursor from an Surface,
 * you should use Cursor.Cursor(). Alternately, you can hide the cursor and draw
 * your own as part of your game's rendering, but it will be bound to the
 * framerate.
 *
 * Also, Cursor.Cursor() is available, which provides several readily-available
 * system cursors to pick from.
 *
 * @param data the color value for each pixel of the cursor.
 * @param mask the mask value for each pixel of the cursor.
 * @param size the width and height of the cursor.
 * @param hot the x position of the cursor hot spot, from the top-left, in the
 *            range of 0 to `size.x` - 1 and 0 to `size.y` - 1.
 * @returns a new cursor with the specified parameters on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa CreateAnimatedCursor
 * @sa Cursor.Cursor
 * @sa Cursor.Cursor
 * @sa Cursor.Destroy
 * @sa Cursor.Set
 */
inline Cursor CreateCursor(const Uint8* data,
                           const Uint8* mask,
                           const PointRaw& size,
                           const PointRaw& hot)
{
  return Cursor(data, mask, size, hot);
}

/**
 * Create a color cursor.
 *
 * If this function is passed a surface with alternate representations added
 * with Surface.AddAlternateImage(), the surface will be interpreted as the
 * content to be used for 100% display scale, and the alternate representations
 * will be used for high DPI situations if SDL_HINT_MOUSE_DPI_SCALE_CURSORS is
 * enabled. For example, if the original surface is 32x32, then on a 2x macOS
 * display or 200% display scale on Windows, a 64x64 version of the image will
 * be used, if available. If a matching version of the image isn't available,
 * the closest larger size image will be downscaled to the appropriate size and
 * be used instead, if available. Otherwise, the closest smaller image will be
 * upscaled and be used instead.
 *
 * @param surface an Surface structure representing the cursor image.
 * @param hot the position of the cursor hot spot.
 * @returns the new cursor on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.AddAlternateImage
 * @sa CreateAnimatedCursor
 * @sa Cursor.Cursor
 * @sa Cursor.Cursor
 * @sa Cursor.Destroy
 * @sa Cursor.Set
 */
inline Cursor CreateColorCursor(SurfaceParam surface, const PointRaw& hot)
{
  return Cursor(surface, hot);
}

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Create an animated color cursor.
 *
 * Animated cursors are composed of a sequential array of frames, specified as
 * surfaces and durations in an array of CursorFrameInfo structs. The hot spot
 * coordinates are universal to all frames, and all frames must have the same
 * dimensions.
 *
 * Frame durations are specified in milliseconds. A duration of 0 implies an
 * infinite frame time, and the animation will stop on that frame. To create a
 * one-shot animation, set the duration of the last frame in the sequence to 0.
 *
 * If this function is passed surfaces with alternate representations added with
 * Surface.AddAlternateImage(), the surfaces will be interpreted as the content
 * to be used for 100% display scale, and the alternate representations will be
 * used for high DPI situations. For example, if the original surfaces are
 * 32x32, then on a 2x macOS display or 200% display scale on Windows, a 64x64
 * version of the image will be used, if available. If a matching version of the
 * image isn't available, the closest larger size image will be downscaled to
 * the appropriate size and be used instead, if available. Otherwise, the
 * closest smaller image will be upscaled and be used instead.
 *
 * If the underlying platform does not support animated cursors, this function
 * will fall back to creating a static color cursor using the first frame in the
 * sequence.
 *
 * @param frames an array of cursor images composing the animation.
 * @param frame_count the number of frames in the sequence.
 * @param hot_x the x position of the cursor hot spot.
 * @param hot_y the y position of the cursor hot spot.
 * @returns the new cursor on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.4.0.
 *
 * @sa Surface.AddAlternateImage
 * @sa Cursor.Cursor
 * @sa Cursor.Cursor
 * @sa Cursor.Cursor
 * @sa Cursor.Destroy
 * @sa Cursor.Set
 */
inline CursorRef CreateAnimatedCursor(CursorFrameInfo* frames,
                                      int frame_count,
                                      int hot_x,
                                      int hot_y)
{
  return CheckError(
    SDL_CreateAnimatedCursor(frames, frame_count, hot_x, hot_y));
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Create a system cursor.
 *
 * @param id an SystemCursor enum value.
 * @returns a cursor on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Cursor.Destroy
 */
inline Cursor CreateSystemCursor(SystemCursor id) { return Cursor(id); }

/**
 * Set the active cursor.
 *
 * This function sets the currently active cursor to the specified one. If the
 * cursor is currently visible, the change will be immediately represented on
 * the display. Cursor.Set(nullptr) can be used to force cursor redraw, if this
 * is desired for any reason.
 *
 * @param cursor a cursor to make active.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetCursor
 */
inline void SetCursor(CursorParam cursor) { CheckError(SDL_SetCursor(cursor)); }

inline void Cursor::Set() { SDL::SetCursor(m_resource); }

/**
 * Get the active cursor.
 *
 * This function returns a pointer to the current cursor which is owned by the
 * library. It is not necessary to free the cursor with Cursor.Destroy().
 *
 * @returns the active cursor or nullptr if there is no mouse.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Cursor.Set
 */
inline CursorRef GetCursor() { return {SDL_GetCursor()}; }

/**
 * Get the default cursor.
 *
 * You do not have to call Cursor.Destroy() on the return value, but it is safe
 * to do so.
 *
 * @returns the default cursor on success.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline CursorRef GetDefaultCursor()
{
  return {CheckError(SDL_GetDefaultCursor())};
}

/**
 * Free a previously-created cursor.
 *
 * Use this function to free cursor resources created with Cursor.Cursor(),
 * Cursor.Cursor() or Cursor.Cursor().
 *
 * @param cursor the cursor to free.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa CreateAnimatedCursor
 * @sa Cursor.Cursor
 * @sa Cursor.Cursor
 * @sa Cursor.Cursor
 */
inline void DestroyCursor(CursorRaw cursor) { SDL_DestroyCursor(cursor); }

inline void Cursor::Destroy() { DestroyCursor(release()); }

/**
 * Show the cursor.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa CursorVisible
 * @sa HideCursor
 */
inline void ShowCursor() { CheckError(SDL_ShowCursor()); }

/**
 * Hide the cursor.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa CursorVisible
 * @sa ShowCursor
 */
inline void HideCursor() { CheckError(SDL_HideCursor()); }

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
