#ifndef SDL3PP_VIDEO_H_
#define SDL3PP_VIDEO_H_

#include <memory>
#include <optional>
#include <vector>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "SDL3pp_properties.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_surface.h"

namespace SDL {
/**
 * @defgroup CategoryVideo Display and Window Management
 *
 * SDL's video subsystem is largely interested in abstracting window
 * management from the underlying operating system. You can create windows,
 * manage them in various ways, set them fullscreen, and get events when
 * interesting things happen with them, such as the mouse or keyboard
 * interacting with a window.
 *
 * The video subsystem is also interested in abstracting away some
 * platform-specific differences in OpenGL: context creation, swapping
 * buffers, etc. This may be crucial to your app, but also you are not
 * required to use OpenGL at all. In fact, SDL can provide rendering to those
 * windows as well, either with an easy-to-use
 * [2D API](https://wiki.libsdl.org/SDL3/CategoryRender)
 * or with a more-powerful
 * [GPU API](https://wiki.libsdl.org/SDL3/CategoryGPU)
 * . Of course, it can simply get out of your way and give you the window
 * handles you need to use Vulkan, Direct3D, Metal, or whatever else you like
 * directly, too.
 *
 * The video subsystem covers a lot of functionality, out of necessity, so it
 * is worth perusing the list of functions just to see what's available, but
 * most apps can get by with simply creating a window and listening for
 * events, so start with SDL_CreateWindow() and SDL_PollEvent().
 * @{
 */

// Forward decl
struct WindowBase;

// Forward decl
struct WindowRef;

// Forward decl
struct Window;

// Forward decl
struct RendererBase;

// Forward decl
struct RendererRef;

/**
 * @name DisplayOrientations
 * @{
 */

/**
 * Display orientation values; the way a display is rotated.
 *
 * @since This enum is available since SDL 3.2.0.
 * @sa ORIENTATION_LANDSCAPE
 * @sa ORIENTATION_PORTRAIT
 * @sa ORIENTATION_UNKNOWN
 */
using DisplayOrientation = SDL_DisplayOrientation;

constexpr DisplayOrientation ORIENTATION_UNKNOWN =
  SDL_ORIENTATION_UNKNOWN; ///< The display orientation can't be determined.

/**
 * The display is in landscape mode, with the right side up, relative to
 * portrait mode.
 */
constexpr DisplayOrientation ORIENTATION_LANDSCAPE = SDL_ORIENTATION_LANDSCAPE;

/**
 * The display is in landscape mode, with the left side up, relative to portrait
 * mode.
 */
constexpr DisplayOrientation ORIENTATION_LANDSCAPE_FLIPPED =
  SDL_ORIENTATION_LANDSCAPE_FLIPPED;

constexpr DisplayOrientation ORIENTATION_PORTRAIT =
  SDL_ORIENTATION_PORTRAIT; ///< The display is in portrait mode.

constexpr DisplayOrientation ORIENTATION_PORTRAIT_FLIPPED =
  SDL_ORIENTATION_PORTRAIT_FLIPPED; ///< The display is in portrait mode, upside
                                    ///< down.

/// @}

/**
 * Internal display mode data.
 *
 * This lives as a field in DisplayMode, as opaque data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa DisplayMode
 */
using DisplayModeData = SDL_DisplayModeData;

/**
 * The structure that defines a display mode.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Display.GetFullscreenModes
 * @sa Display.GetDesktopMode
 * @sa Display.GetCurrentMode
 * @sa WindowBase.SetFullscreenMode
 * @sa WindowBase.GetFullscreenMode
 */
using DisplayMode = SDL_DisplayMode;

/**
 * @name WindowFlags
 * @{
 */

/**
 * The flags on a window.
 *
 * These cover a lot of true/false, or on/off, window state. Some of it is
 * immutable after being set through WindowBase.WindowBase(), some of it can be
 * changed on existing windows by the app, and some of it might be altered by
 * the user or system outside of the app's control.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa WindowBase.GetFlags
 */
using WindowFlags = SDL_WindowFlags;

constexpr WindowFlags WINDOW_FULLSCREEN =
  SDL_WINDOW_FULLSCREEN; ///< window is in fullscreen mode

constexpr WindowFlags WINDOW_OPENGL =
  SDL_WINDOW_OPENGL; ///< window usable with OpenGL context

constexpr WindowFlags WINDOW_OCCLUDED =
  SDL_WINDOW_OCCLUDED; ///< window is occluded

/**
 * window is neither mapped onto the desktop nor shown in the
 * taskbar/dock/window list; WindowBase.Show() is required for it to become
 * visible
 */
constexpr WindowFlags WINDOW_HIDDEN = SDL_WINDOW_HIDDEN;

constexpr WindowFlags WINDOW_BORDERLESS =
  SDL_WINDOW_BORDERLESS; ///< no window decoration

constexpr WindowFlags WINDOW_RESIZABLE =
  SDL_WINDOW_RESIZABLE; ///< window can be resized

constexpr WindowFlags WINDOW_MINIMIZED =
  SDL_WINDOW_MINIMIZED; ///< window is minimized

constexpr WindowFlags WINDOW_MAXIMIZED =
  SDL_WINDOW_MAXIMIZED; ///< window is maximized

constexpr WindowFlags WINDOW_MOUSE_GRABBED =
  SDL_WINDOW_MOUSE_GRABBED; ///< window has grabbed mouse input

constexpr WindowFlags WINDOW_INPUT_FOCUS =
  SDL_WINDOW_INPUT_FOCUS; ///< window has input focus

constexpr WindowFlags WINDOW_MOUSE_FOCUS =
  SDL_WINDOW_MOUSE_FOCUS; ///< window has mouse focus

constexpr WindowFlags WINDOW_EXTERNAL =
  SDL_WINDOW_EXTERNAL; ///< window not created by SDL

constexpr WindowFlags WINDOW_MODAL = SDL_WINDOW_MODAL; ///< window is modal

/**
 * window uses high pixel density back buffer if possible
 */
constexpr WindowFlags WINDOW_HIGH_PIXEL_DENSITY = SDL_WINDOW_HIGH_PIXEL_DENSITY;

/**
 * window has mouse captured (unrelated to MOUSE_GRABBED)
 */
constexpr WindowFlags WINDOW_MOUSE_CAPTURE = SDL_WINDOW_MOUSE_CAPTURE;

constexpr WindowFlags WINDOW_MOUSE_RELATIVE_MODE =
  SDL_WINDOW_MOUSE_RELATIVE_MODE; ///< window has relative mode enabled

constexpr WindowFlags WINDOW_ALWAYS_ON_TOP =
  SDL_WINDOW_ALWAYS_ON_TOP; ///< window should always be above others

/**
 * window should be treated as a utility window, not showing in the task bar and
 * window list
 */
constexpr WindowFlags WINDOW_UTILITY = SDL_WINDOW_UTILITY;

/**
 * window should be treated as a tooltip and does not get mouse or keyboard
 * focus, requires a parent window
 */
constexpr WindowFlags WINDOW_TOOLTIP = SDL_WINDOW_TOOLTIP;

/**
 * window should be treated as a popup menu, requires a parent window
 */
constexpr WindowFlags WINDOW_POPUP_MENU = SDL_WINDOW_POPUP_MENU;

constexpr WindowFlags WINDOW_KEYBOARD_GRABBED =
  SDL_WINDOW_KEYBOARD_GRABBED; ///< window has grabbed keyboard input

constexpr WindowFlags WINDOW_VULKAN =
  SDL_WINDOW_VULKAN; ///< window usable for Vulkan surface

constexpr WindowFlags WINDOW_METAL =
  SDL_WINDOW_METAL; ///< window usable for Metal view

constexpr WindowFlags WINDOW_TRANSPARENT =
  SDL_WINDOW_TRANSPARENT; ///< window with transparent buffer

constexpr WindowFlags WINDOW_NOT_FOCUSABLE =
  SDL_WINDOW_NOT_FOCUSABLE; ///< window should not be focusable

/// @}

/**
 * @name FlashOperations
 * @{
 */

/**
 * Window flash operation.
 *
 * @since This enum is available since SDL 3.2.0.
 * @sa FLASH_CANCEL
 * @sa FLASH_BRIEFLY
 * @sa FLASH_UNTIL_FOCUSED
 */
using FlashOperation = SDL_FlashOperation;

constexpr FlashOperation FLASH_CANCEL =
  SDL_FLASH_CANCEL; ///< Cancel any window flash state.

constexpr FlashOperation FLASH_BRIEFLY =
  SDL_FLASH_BRIEFLY; ///< Flash the window briefly to get attention.

constexpr FlashOperation FLASH_UNTIL_FOCUSED =
  SDL_FLASH_UNTIL_FOCUSED; ///< Flash the window until it gets focus.

/// @}

/**
 * @name HitTestResults
 * Possible return values from the HitTest callback
 *
 * @{
 */

/**
 * Possible return values from the HitTest callback.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa HitTest
 * @sa HITTEST_NORMAL
 */
using HitTestResult = SDL_HitTestResult;

constexpr HitTestResult HITTEST_NORMAL =
  SDL_HITTEST_NORMAL; ///< Region is normal.  No special properties.

constexpr HitTestResult HITTEST_DRAGGABLE =
  SDL_HITTEST_DRAGGABLE; ///< Region can drag entire window.

constexpr HitTestResult HITTEST_RESIZE_TOPLEFT =
  SDL_HITTEST_RESIZE_TOPLEFT; ///< Region is the resizable top-left corner
                              ///< border.

constexpr HitTestResult HITTEST_RESIZE_TOP =
  SDL_HITTEST_RESIZE_TOP; ///< Region is the resizable top border.

constexpr HitTestResult HITTEST_RESIZE_TOPRIGHT =
  SDL_HITTEST_RESIZE_TOPRIGHT; ///< Region is the resizable top-right corner
                               ///< border.

constexpr HitTestResult HITTEST_RESIZE_RIGHT =
  SDL_HITTEST_RESIZE_RIGHT; ///< Region is the resizable right border.

/**
 * Region is the resizable bottom-right corner border.
 */
constexpr HitTestResult HITTEST_RESIZE_BOTTOMRIGHT =
  SDL_HITTEST_RESIZE_BOTTOMRIGHT;

constexpr HitTestResult HITTEST_RESIZE_BOTTOM =
  SDL_HITTEST_RESIZE_BOTTOM; ///< Region is the resizable bottom border.

constexpr HitTestResult HITTEST_RESIZE_BOTTOMLEFT =
  SDL_HITTEST_RESIZE_BOTTOMLEFT; ///< Region is the resizable bottom-left corner
                                 ///< border.

constexpr HitTestResult HITTEST_RESIZE_LEFT =
  SDL_HITTEST_RESIZE_LEFT; ///< Region is the resizable left border.

/// @}

/**
 * @name Callbacks for WindowBase::SetHitTest()
 * @{
 */

/**
 * Callback used for hit-testing.
 *
 * @param win the WindowBase where hit-testing was set on.
 * @param area an Point which should be hit-tested.
 * @param data what was passed as `callback_data` to WindowBase.SetHitTest().
 * @returns an HitTestResult value.
 *
 * @sa WindowBase.SetHitTest
 */
using HitTest = SDL_HitTest;

/**
 * Callback used for hit-testing.
 *
 * @param win the WindowRef where hit-testing was set on.
 * @param area a Point const reference which should be hit-tested.
 * @returns an SDL::HitTestResult value.
 *
 * @sa HitTest
 *
 * @cat listener-callback
 */
using HitTestCB =
  std::function<HitTestResult(WindowRef window, const Point& area)>;

/**
 * A window parameter that might own its value.
 *
 * This is designed to be used on parameter's type and accepts that accepts a
 * std::nullopt, a non-owned WindowRef or an owned Window
 */
using OptionalWindow = OptionalResource<WindowRef, Window>;

/// @}

// Forward decl
struct GLContextBase;

// Forward decl
struct GLContextRef;

// Forward decl
struct GLContext;

/**
 * This is a unique ID for a display for the time it is connected to the
 * system, and is never reused for the lifetime of the application.
 *
 * If the display is disconnected and reconnected, it will get a new ID.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
class Display
{
  SDL_DisplayID m_displayID;

public:
  /**
   * Wraps Display.
   *
   * @param displayID the value to be wrapped
   */
  constexpr Display(SDL_DisplayID displayID = {})
    : m_displayID(displayID)
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const Display& other) const = default;

  /**
   * Unwraps to the underlying Display.
   *
   * @returns the underlying Display.
   */
  constexpr operator SDL_DisplayID() const { return m_displayID; }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return m_displayID != 0; }

  /**
   * Get a list of currently connected displays.
   *
   * @returns a 0 terminated array of display instance IDs or nullptr on
   *          failure; call GetError() for more information. This should be
   *          freed with free() when it is no longer needed.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static OwnArray<Display> GetAll()
  {
    int count = 0;
    auto data = reinterpret_cast<Display*>(SDL_GetDisplays(&count));
    return OwnArray<Display>{data, size_t(count)};
  }

  /**
   * Return the primary display.
   *
   * @returns the instance ID of the primary display on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetAll
   */
  static Display GetPrimary() { return CheckError(SDL_GetPrimaryDisplay()); }

  /**
   * Get the properties associated with a display.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `prop::Display.HDR_ENABLED_BOOLEAN`: true if the display has HDR
   *   headroom above the SDR white point. This is for informational and
   *   diagnostic purposes only, as not all platforms provide this information
   *   at the display level.
   *
   * On KMS/DRM:
   *
   * - `prop::Display.KMSDRM_PANEL_ORIENTATION_NUMBER`: the "panel
   *   orientation" property for the display in degrees of clockwise rotation.
   *   Note that this is provided only as a hint, and the application is
   *   responsible for any coordinate transformations needed to conform to the
   *   requested display orientation.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return CheckError(SDL_GetDisplayProperties(m_displayID));
  }

  /**
   * Get the name of a display in UTF-8 encoding.
   *
   * @returns the name of a display or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetAll
   */
  const char* GetName() const { return SDL_GetDisplayName(m_displayID); }

  /**
   * Get the desktop area represented by a display.
   *
   * The primary display is often located at (0,0), but may be placed at a
   * different location depending on monitor layout.
   *
   * @returns the Rect structure filled in with the display bounds on success
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetUsableBounds
   * @sa Display.GetAll
   */
  Rect GetBounds() const
  {
    Rect bounds;
    SDL_GetDisplayBounds(m_displayID, &bounds);
    return bounds;
  }

  /**
   * Get the usable desktop area represented by a display, in screen
   * coordinates.
   *
   * This is the same area as Display.GetBounds() reports, but with portions
   * reserved by the system removed. For example, on Apple's macOS, this
   * subtracts the area occupied by the menu bar and dock.
   *
   * Setting a window to be fullscreen generally bypasses these unusable areas,
   * so these are good guidelines for the maximum space available to a
   * non-fullscreen window.
   *
   * @returns the Rect structure filled in with the display bounds on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetBounds
   * @sa Display.GetAll
   */
  Rect GetUsableBounds() const
  {
    Rect bounds;
    CheckError(SDL_GetDisplayUsableBounds(m_displayID, &bounds));
    return bounds;
  }

  /**
   * Get the orientation of a display when it is unrotated.
   *
   * @returns the DisplayOrientation enum value of the display, or
   *          `ORIENTATION_UNKNOWN` if it isn't available.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetAll
   */
  DisplayOrientation GetNaturalOrientation() const
  {
    return SDL_GetNaturalDisplayOrientation(m_displayID);
  }

  /**
   * Get the orientation of a display.
   *
   * @returns the DisplayOrientation enum value of the display, or
   *          `ORIENTATION_UNKNOWN` if it isn't available.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetAll
   */
  DisplayOrientation GetCurrentOrientation() const
  {
    return SDL_GetCurrentDisplayOrientation(m_displayID);
  }

  /**
   * Get the content scale of a display.
   *
   * The content scale is the expected scale for content based on the DPI
   * settings of the display. For example, a 4K display might have a 2.0 (200%)
   * display scale, which means that the user expects UI elements to be twice as
   * big on this display, to aid in readability.
   *
   * After window creation, WindowBase.GetDisplayScale() should be used to query
   * the content scale factor for individual windows instead of querying the
   * display for a window and calling this function, as the per-window content
   * scale factor may differ from the base value of the display it is on,
   * particularly on high-DPI and/or multi-monitor desktop configurations.
   *
   * @returns the content scale of the display, or 0.0f on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetDisplayScale
   * @sa Display.GetAll
   */
  float GetContentScale() const
  {
    return SDL_GetDisplayContentScale(m_displayID);
  }

  /**
   * Get a list of fullscreen display modes available on a display.
   *
   * The display modes are sorted in this priority:
   *
   * - w -> largest to smallest
   * - h -> largest to smallest
   * - bits per pixel -> more colors to fewer colors
   * - packed pixel layout -> largest to smallest
   * - refresh rate -> highest to lowest
   * - pixel density -> lowest to highest
   *
   * @returns a nullptr terminated array of display mode pointers on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * This automatically calls SDL_free after result is out of scope.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetAll
   */
  OwnArray<DisplayMode*> GetFullscreenModes() const
  {
    int count = 0;
    auto data = CheckError(SDL_GetFullscreenDisplayModes(m_displayID, &count));
    return OwnArray<DisplayMode*>{data, size_t(count)};
  }

  /**
   * Get the closest match to the requested display mode.
   *
   * The available display modes are scanned and `closest` is filled in with the
   * closest mode matching the requested mode and returned. The mode format and
   * refresh rate default to the desktop mode if they are set to 0. The modes
   * are scanned with size being first priority, format being second priority,
   * and finally checking the refresh rate. If all the available modes are too
   * small, then false is returned.
   *
   * @param w the width in pixels of the desired display mode.
   * @param h the height in pixels of the desired display mode.
   * @param refresh_rate the refresh rate of the desired display mode, or 0.0f
   *                     for the desktop refresh rate.
   * @param include_high_density_modes boolean to include high density modes in
   *                                   the search.
   * @returns the closest display mode equal to or larger than the desired mode
   *          on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetAll
   * @sa Display.GetFullscreenModes
   */
  DisplayMode GetClosestFullscreenMode(int w,
                                       int h,
                                       float refresh_rate,
                                       bool include_high_density_modes) const
  {
    SDL_DisplayMode closest;
    CheckError(SDL_GetClosestFullscreenDisplayMode(
      m_displayID, w, h, refresh_rate, include_high_density_modes, &closest));
    return closest;
  }

  /**
   * Get information about the desktop's display mode.
   *
   * There's a difference between this function and Display.GetCurrentMode()
   * when SDL runs fullscreen and has changed the resolution. In that case this
   * function will return the previous native display mode, and not the current
   * display mode.
   *
   * @returns a pointer to the desktop display mode.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetCurrentMode
   * @sa Display.GetAll
   */
  const DisplayMode* GetDesktopMode() const
  {
    return CheckError(SDL_GetDesktopDisplayMode(m_displayID));
  }

  /**
   * Get information about the current display mode.
   *
   * There's a difference between this function and Display.GetDesktopMode()
   * when SDL runs fullscreen and has changed the resolution. In that case this
   * function will return the current display mode, and not the previous native
   * display mode.
   *
   * @returns a pointer to the desktop display mode or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetDesktopMode
   * @sa Display.GetAll
   */
  const DisplayMode* GetCurrentMode() const
  {
    return SDL_GetCurrentDisplayMode(m_displayID);
  }

  /**
   * Get the display containing a point.
   *
   * @param point the point to query.
   * @returns the instance ID of the display containing the point or 0 on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetBounds
   * @sa Display.GetAll
   */
  static Display GetForPoint(const SDL_Point& point)
  {
    return {SDL_GetDisplayForPoint(&point)};
  }

  /**
   * Get the display primarily containing a rect.
   *
   * @param rect the rect to query.
   * @returns the instance ID of the display entirely containing the rect or
   *          closest to the center of the rect on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetBounds
   * @sa Display.GetAll
   */
  static Display GetForRect(const SDL_Rect& rect)
  {
    return SDL_GetDisplayForRect(&rect);
  }

  /**
   * @brief Get the display associated with a window.
   * @param window the window to query.
   * @returns the instance ID of the display containing the center of the window
   *          on success or 0 on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  static Display GetForWindow(WindowBase& window);
};

/**
 * This is a unique ID for a window.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using WindowID = SDL_WindowID;

namespace prop::Global {

/**
 * The pointer to the global `wl_display` object used by the Wayland video
 * backend.
 *
 * Can be set before the video subsystem is initialized to import an external
 * `wl_display` object from an application or toolkit for use in SDL, or read
 * after initialization to export the `wl_display` used by the Wayland video
 * backend. Setting this property after the video subsystem has been
 * initialized has no effect, and reading it when the video subsystem is
 * uninitialized will either return the user provided value, if one was set
 * prior to initialization, or NULL. See docs/README-wayland.md for more
 * information.
 */
constexpr auto VIDEO_WAYLAND_WL_DISPLAY_POINTER =
  SDL_PROP_GLOBAL_VIDEO_WAYLAND_WL_DISPLAY_POINTER;

} // namespace prop::Global

/**
 * @name SystemThemes
 * @{
 */

/**
 * System theme.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using SystemTheme = SDL_SystemTheme;

constexpr SystemTheme SYSTEM_THEME_UNKNOWN =
  SDL_SYSTEM_THEME_UNKNOWN; ///< Unknown system theme.

constexpr SystemTheme SYSTEM_THEME_LIGHT =
  SDL_SYSTEM_THEME_LIGHT; ///< Light colored system theme.

constexpr SystemTheme SYSTEM_THEME_DARK =
  SDL_SYSTEM_THEME_DARK; ///< Dark colored system theme.

/// @}

/**
 * Represents a handle to a window.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa WindowBase.WindowBase
 *
 * @cat resource
 *
 * @sa Window
 * @sa WindowRef
 */
struct WindowBase : Resource<SDL_Window*>
{
  using Resource::Resource;

  /**
   * Create a window with the specified dimensions and flags.
   *
   * The window size is a request and may be different than expected based on
   * the desktop layout and window manager policies. Your application should be
   * prepared to handle a window of any size.
   *
   * `flags` may be any of the following OR'd together:
   *
   * - `WINDOW_FULLSCREEN`: fullscreen window at desktop resolution
   * - `WINDOW_OPENGL`: window usable with an OpenGL context
   * - `WINDOW_OCCLUDED`: window partially or completely obscured by another
   *   window
   * - `WINDOW_HIDDEN`: window is not visible
   * - `WINDOW_BORDERLESS`: no window decoration
   * - `WINDOW_RESIZABLE`: window can be resized
   * - `WINDOW_MINIMIZED`: window is minimized
   * - `WINDOW_MAXIMIZED`: window is maximized
   * - `WINDOW_MOUSE_GRABBED`: window has grabbed mouse focus
   * - `WINDOW_INPUT_FOCUS`: window has input focus
   * - `WINDOW_MOUSE_FOCUS`: window has mouse focus
   * - `WINDOW_EXTERNAL`: window not created by SDL
   * - `WINDOW_MODAL`: window is modal
   * - `WINDOW_HIGH_PIXEL_DENSITY`: window uses high pixel density back
   *   buffer if possible
   * - `WINDOW_MOUSE_CAPTURE`: window has mouse captured (unrelated to
   *   MOUSE_GRABBED)
   * - `WINDOW_ALWAYS_ON_TOP`: window should always be above others
   * - `WINDOW_UTILITY`: window should be treated as a utility window, not
   *   showing in the task bar and window list
   * - `WINDOW_TOOLTIP`: window should be treated as a tooltip and does not
   *   get mouse or keyboard focus, requires a parent window
   * - `WINDOW_POPUP_MENU`: window should be treated as a popup menu,
   *   requires a parent window
   * - `WINDOW_KEYBOARD_GRABBED`: window has grabbed keyboard input
   * - `WINDOW_VULKAN`: window usable with a Vulkan instance
   * - `WINDOW_METAL`: window usable with a Metal instance
   * - `WINDOW_TRANSPARENT`: window with transparent buffer
   * - `WINDOW_NOT_FOCUSABLE`: window should not be focusable
   *
   * The WindowBase is implicitly shown if WINDOW_HIDDEN is not set.
   *
   * On Apple's macOS, you **must** set the NSHighResolutionCapable Info.plist
   * property to YES, otherwise you will not receive a High-DPI OpenGL canvas.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use WindowBase.GetSize() to
   * query the client area's size in window coordinates, and
   * WindowBase.GetSizeInPixels() or RendererBase.GetOutputSize() to query the
   * drawable size in pixels. Note that the drawable size can vary after the
   * window is created and should be queried again if you get an
   * EVENT_WINDOW_PIXEL_SIZE_CHANGED event.
   *
   * If the window is created with any of the WINDOW_OPENGL or
   * WINDOW_VULKAN flags, then the corresponding LoadLibrary function
   * (GL_LoadLibrary or SDL_Vulkan_LoadLibrary) is called and the
   * corresponding UnloadLibrary function is called by WindowRef.reset().
   *
   * If WINDOW_VULKAN is specified and there isn't a working Vulkan driver,
   * WindowBase.WindowBase() will fail, because SDL_Vulkan_LoadLibrary() will
   * fail.
   *
   * If WINDOW_METAL is specified on an OS that does not support Metal,
   * WindowBase.WindowBase() will fail.
   *
   * If you intend to use this window with an RendererBase, you should use
   * CreateWindowAndRenderer() instead of this function, to avoid window
   * flicker.
   *
   * On non-Apple devices, SDL requires you to either not link to the Vulkan
   * loader or link to a dynamic library version. This limitation may be removed
   * in a future version of SDL.
   *
   * @param title the title of the window, in UTF-8 encoding.
   * @param size the width and height of the window.
   * @param flags 0, or one or more WindowFlags OR'd together.
   * @post the window that was created.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa CreateWindowAndRenderer()
   */
  WindowBase(StringParam title, SDL_Point size, WindowFlags flags = 0)
    : Resource(CheckError(SDL_CreateWindow(title, size.x, size.y, flags)))
  {
  }

  /**
   * Create a child popup window of the specified parent window.
   *
   * The window size is a request and may be different than expected based on
   * the desktop layout and window manager policies. Your application should be
   * prepared to handle a window of any size.
   *
   * The flags parameter **must** contain at least one of the following:
   *
   * - `WINDOW_TOOLTIP`: The popup window is a tooltip and will not pass any
   *   input events.
   * - `WINDOW_POPUP_MENU`: The popup window is a popup menu. The topmost
   *   popup menu will implicitly gain the keyboard focus.
   *
   * The following flags are not relevant to popup window creation and will be
   * ignored:
   *
   * - `WINDOW_MINIMIZED`
   * - `WINDOW_MAXIMIZED`
   * - `WINDOW_FULLSCREEN`
   * - `WINDOW_BORDERLESS`
   *
   * The following flags are incompatible with popup window creation and will
   * cause it to fail:
   *
   * - `WINDOW_UTILITY`
   * - `WINDOW_MODAL`
   *
   * The parent parameter **must** be non-null and a valid window. The parent of
   * a popup window can be either a regular, toplevel window, or another popup
   * window.
   *
   * Popup windows cannot be minimized, maximized, made fullscreen, raised,
   * flash, be made a modal window, be the parent of a toplevel window, or grab
   * the mouse and/or keyboard. Attempts to do so will fail.
   *
   * Popup windows implicitly do not have a border/decorations and do not appear
   * on the taskbar/dock or in lists of windows such as alt-tab menus.
   *
   * If a parent window is hidden or destroyed, any child popup windows will be
   * recursively hidden or destroyed as well. Child popup windows not explicitly
   * hidden will be restored when the parent is shown.
   *
   * @param parent the parent of the window, must not be nullptr.
   * @param offset the x, y position of the popup window relative to the origin
   *               of the parent.
   * @param size the width and height of the window.
   * @param flags WINDOW_TOOLTIP or WINDOW_POPUP_MENU, and zero or more
   *              additional WindowFlags OR'd together.
   * @post the window that was created.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetParent
   */
  WindowBase(WindowBase& parent,
             SDL_Point offset,
             SDL_Point size,
             WindowFlags flags = 0)
    : Resource(CheckError(SDL_CreatePopupWindow(parent.get(),
                                                offset.x,
                                                offset.y,
                                                size.x,
                                                size.y,
                                                flags)))
  {
  }

  /**
   * Create a window with the specified properties.
   *
   * The window size is a request and may be different than expected based on
   * the desktop layout and window manager policies. Your application should be
   * prepared to handle a window of any size.
   *
   * These are the supported properties:
   *
   * - `prop::Window.CREATE_ALWAYS_ON_TOP_BOOLEAN`: true if the window should
   *   be always on top
   * - `prop::Window.CREATE_BORDERLESS_BOOLEAN`: true if the window has no
   *   window decoration
   * - `prop::Window.CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN`: true if the
   *   window will be used with an externally managed graphics context.
   * - `prop::Window.CREATE_FOCUSABLE_BOOLEAN`: true if the window should
   *   accept keyboard input (defaults true)
   * - `prop::Window.CREATE_FULLSCREEN_BOOLEAN`: true if the window should
   *   start in fullscreen mode at desktop resolution
   * - `prop::Window.CREATE_HEIGHT_NUMBER`: the height of the window
   * - `prop::Window.CREATE_HIDDEN_BOOLEAN`: true if the window should start
   *   hidden
   * - `prop::Window.CREATE_HIGH_PIXEL_DENSITY_BOOLEAN`: true if the window
   *   uses a high pixel density buffer if possible
   * - `prop::Window.CREATE_MAXIMIZED_BOOLEAN`: true if the window should
   *   start maximized
   * - `prop::Window.CREATE_MENU_BOOLEAN`: true if the window is a popup menu
   * - `prop::Window.CREATE_METAL_BOOLEAN`: true if the window will be used
   *   with Metal rendering
   * - `prop::Window.CREATE_MINIMIZED_BOOLEAN`: true if the window should
   *   start minimized
   * - `prop::Window.CREATE_MODAL_BOOLEAN`: true if the window is modal to
   *   its parent
   * - `prop::Window.CREATE_MOUSE_GRABBED_BOOLEAN`: true if the window starts
   *   with grabbed mouse focus
   * - `prop::Window.CREATE_OPENGL_BOOLEAN`: true if the window will be used
   *   with OpenGL rendering
   * - `prop::Window.CREATE_PARENT_POINTER`: an WindowBase that will be the
   *   parent of this window, required for windows with the "tooltip", "menu",
   *   and "modal" properties
   * - `prop::Window.CREATE_RESIZABLE_BOOLEAN`: true if the window should be
   *   resizable
   * - `prop::Window.CREATE_TITLE_STRING`: the title of the window, in UTF-8
   *   encoding
   * - `prop::Window.CREATE_TRANSPARENT_BOOLEAN`: true if the window show
   *   transparent in the areas with alpha of 0
   * - `prop::Window.CREATE_TOOLTIP_BOOLEAN`: true if the window is a tooltip
   * - `prop::Window.CREATE_UTILITY_BOOLEAN`: true if the window is a utility
   *   window, not showing in the task bar and window list
   * - `prop::Window.CREATE_VULKAN_BOOLEAN`: true if the window will be used
   *   with Vulkan rendering
   * - `prop::Window.CREATE_WIDTH_NUMBER`: the width of the window
   * - `prop::Window.CREATE_X_NUMBER`: the x position of the window, or
   *   `SDL_WINDOWPOS_CENTERED`, defaults to `SDL_WINDOWPOS_UNDEFINED`. This is
   *   relative to the parent for windows with the "tooltip" or "menu" property
   *   set.
   * - `prop::Window.CREATE_Y_NUMBER`: the y position of the window, or
   *   `SDL_WINDOWPOS_CENTERED`, defaults to `SDL_WINDOWPOS_UNDEFINED`. This is
   *   relative to the parent for windows with the "tooltip" or "menu" property
   *   set.
   *
   * These are additional supported properties on macOS:
   *
   * - `prop::Window.CREATE_COCOA_WINDOW_POINTER`: the
   *   `(__unsafe_unretained)` NSWindow associated with the window, if you want
   *   to wrap an existing window.
   * - `prop::Window.CREATE_COCOA_VIEW_POINTER`: the `(__unsafe_unretained)`
   *   NSView associated with the window, defaults to `[window contentView]`
   *
   * These are additional supported properties on Wayland:
   *
   * - `prop::Window.CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN` - true if
   *   the application wants to use the Wayland surface for a custom role and
   *   does not want it attached to an XDG toplevel window. See
   *   [README/wayland](README/wayland) for more information on using custom
   *   surfaces.
   * - `prop::Window.CREATE_WAYLAND_CREATE_EGL_WINDOW_BOOLEAN` - true if the
   *   application wants an associated `wl_egl_window` object to be created and
   *   attached to the window, even if the window does not have the OpenGL
   *   property or `WINDOW_OPENGL` flag set.
   * - `prop::Window.CREATE_WAYLAND_WL_SURFACE_POINTER` - the wl_surface
   *   associated with the window, if you want to wrap an existing window. See
   *   [README/wayland](README/wayland) for more information.
   *
   * These are additional supported properties on Windows:
   *
   * - `prop::Window.CREATE_WIN32_HWND_POINTER`: the HWND associated with the
   *   window, if you want to wrap an existing window.
   * - `prop::Window.CREATE_WIN32_PIXEL_FORMAT_HWND_POINTER`: optional,
   *   another window to share pixel format with, useful for OpenGL windows
   *
   * These are additional supported properties with X11:
   *
   * - `prop::Window.CREATE_X11_WINDOW_NUMBER`: the X11 Window associated
   *   with the window, if you want to wrap an existing window.
   *
   * The window is implicitly shown if the "hidden" property is not set.
   *
   * Windows with the "tooltip" and "menu" properties are popup windows and have
   * the behaviors and guidelines outlined in WindowBase.WindowBase().
   *
   * If this window is being created to be used with an RendererBase, you should
   * not add a graphics API specific property
   * (`prop::Window.CREATE_OPENGL_BOOLEAN`, etc), as SDL will handle that
   * internally when it chooses a renderer. However, SDL might need to recreate
   * your window at that point, which may cause the window to appear briefly,
   * and then flicker as it is recreated. The correct approach to this is to
   * create the window with the `prop::Window.CREATE_HIDDEN_BOOLEAN` property
   * set to true, then create the renderer, then show the window with
   * WindowBase.Show().
   *
   * @param props the properties to use.
   * @post the window that was created.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Properties
   */
  WindowBase(PropertiesBase& props)
    : Resource(CheckError(SDL_CreateWindowWithProperties(props.get())))
  {
  }

  /**
   * Get the display associated with a window.
   *
   * @returns the instance ID of the display containing the center of the window
   *          on success or 0 on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetBounds
   * @sa Display.GetAll
   */
  Display GetDisplay() const { return SDL_GetDisplayForWindow(get()); }

  /**
   * Get the pixel density of a window.
   *
   * This is a ratio of pixel size to window size. For example, if the window is
   * 1920x1080 and it has a high density back buffer of 3840x2160 pixels, it
   * would have a pixel density of 2.0.
   *
   * @returns the pixel density or 0.0f on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetDisplayScale
   */
  float GetPixelDensity() const { return SDL_GetWindowPixelDensity(get()); }

  /**
   * Get the content display scale relative to a window's pixel size.
   *
   * This is a combination of the window pixel density and the display content
   * scale, and is the expected scale for displaying content in this window. For
   * example, if a 3840x2160 window had a display scale of 2.0, the user expects
   * the content to take twice as many pixels and be the same physical size as
   * if it were being displayed in a 1920x1080 window with a display scale of
   * 1.0.
   *
   * Conceptually this value corresponds to the scale display setting, and is
   * updated when that setting is changed, or the window moves to a display with
   * a different scale setting.
   *
   * @returns the display scale, or 0.0f on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  float GetDisplayScale() const { return SDL_GetWindowDisplayScale(get()); }

  /**
   * Set the display mode to use when a window is visible and fullscreen.
   *
   * This only affects the display mode used when the window is fullscreen. To
   * change the window size when the window is not fullscreen, use
   * WindowBase.SetSize().
   *
   * If the window is currently in the fullscreen state, this request is
   * asynchronous on some windowing systems and the new mode dimensions may not
   * be applied immediately upon the return of this function. If an immediate
   * change is required, call WindowBase.Sync() to block until the changes have
   * taken effect.
   *
   * When the new mode takes effect, an EVENT_WINDOW_RESIZED and/or an
   * EVENT_WINDOW_PIXEL_SIZE_CHANGED event will be emitted with the new mode
   * dimensions.
   *
   * @param mode the display mode to use, which can be nullptr for borderless
   *             fullscreen desktop mode, or one of the fullscreen modes
   *             returned by Display.GetFullscreenModes() to set an exclusive
   *             fullscreen mode.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetFullscreenMode
   * @sa WindowBase.SetFullscreen
   * @sa WindowBase.Sync
   */
  void SetFullscreenMode(OptionalRef<const DisplayMode> mode)
  {
    CheckError(SDL_SetWindowFullscreenMode(get(), mode));
  }

  /**
   * Query the display mode to use when a window is visible at fullscreen.
   *
   * @returns a pointer to the exclusive fullscreen mode to use or nullptr for
   *          borderless fullscreen desktop mode.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetFullscreenMode
   * @sa WindowBase.SetFullscreen
   */
  const DisplayMode* GetFullscreenMode() const
  {
    return SDL_GetWindowFullscreenMode(get());
  }

  /**
   * Get the raw ICC profile data for the screen the window is currently on.
   *
   * @param size the size of the ICC profile.
   * @returns the raw ICC profile data on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  OwnPtr<void> GetICCProfile(size_t* size) const
  {
    return OwnPtr<void>{CheckError(SDL_GetWindowICCProfile(get(), size))};
  }

  /**
   * Get the pixel format associated with the window.
   *
   * @returns the pixel format of the window on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PixelFormat GetPixelFormat() const
  {
    return CheckError(SDL_GetWindowPixelFormat(get()));
  }

  /**
   * Get the numeric ID of a window.
   *
   * The numeric ID is what WindowEvent references, and is necessary to map
   * these events to specific WindowBase objects.
   *
   * @returns the ID of the window on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowRef.FromID
   */
  WindowID GetID() const { return CheckError(SDL_GetWindowID(get())); }

  /**
   * Get parent of a window.
   *
   * @returns the parent of the window on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.WindowBase
   */
  WindowRef GetParent() const;

  /**
   * Get the properties associated with a window.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `prop::Window.SHAPE_POINTER`: the surface associated with a shaped
   *   window
   * - `prop::Window.HDR_ENABLED_BOOLEAN`: true if the window has HDR
   *   headroom above the SDR white point. This property can change dynamically
   *   when EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `prop::Window.SDR_WHITE_LEVEL_FLOAT`: the value of SDR white in the
   *   COLORSPACE_SRGB_LINEAR colorspace. On Windows this corresponds to the
   *   SDR white level in scRGB colorspace, and on Apple platforms this is
   *   always 1.0 for EDR content. This property can change dynamically when
   *   EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `prop::Window.HDR_HEADROOM_FLOAT`: the additional high dynamic range
   *   that can be displayed, in terms of the SDR white point. When HDR is not
   *   enabled, this will be 1.0. This property can change dynamically when
   *   EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   *
   * On Android:
   *
   * - `prop::Window.ANDROID_WINDOW_POINTER`: the ANativeWindow associated
   *   with the window
   * - `prop::Window.ANDROID_SURFACE_POINTER`: the EGLSurface associated with
   *   the window
   *
   * On iOS:
   *
   * - `prop::Window.UIKIT_WINDOW_POINTER`: the `(__unsafe_unretained)`
   *   UIWindow associated with the window
   * - `prop::Window.UIKIT_METAL_VIEW_TAG_NUMBER`: the NSInteger tag
   *   associated with metal views on the window
   * - `prop::Window.UIKIT_OPENGL_FRAMEBUFFER_NUMBER`: the OpenGL view's
   *   framebuffer object. It must be bound when rendering to the screen using
   *   OpenGL.
   * - `prop::Window.UIKIT_OPENGL_RENDERBUFFER_NUMBER`: the OpenGL view's
   *   renderbuffer object. It must be bound when GL_SwapWindow is called.
   * - `prop::Window.UIKIT_OPENGL_RESOLVE_FRAMEBUFFER_NUMBER`: the OpenGL
   *   view's resolve framebuffer, when MSAA is used.
   *
   * On KMS/DRM:
   *
   * - `prop::Window.KMSDRM_DEVICE_INDEX_NUMBER`: the device index associated
   *   with the window (e.g. the X in /dev/dri/cardX)
   * - `prop::Window.KMSDRM_DRM_FD_NUMBER`: the DRM FD associated with the
   *   window
   * - `prop::Window.KMSDRM_GBM_DEVICE_POINTER`: the GBM device associated
   *   with the window
   *
   * On macOS:
   *
   * - `prop::Window.COCOA_WINDOW_POINTER`: the `(__unsafe_unretained)`
   *   NSWindow associated with the window
   * - `prop::Window.COCOA_METAL_VIEW_TAG_NUMBER`: the NSInteger tag
   *   assocated with metal views on the window
   *
   * On OpenVR:
   *
   * - `prop::Window.OPENVR_OVERLAY_ID`: the OpenVR Overlay Handle ID for the
   *   associated overlay window.
   *
   * On Vivante:
   *
   * - `prop::Window.VIVANTE_DISPLAY_POINTER`: the EGLNativeDisplayType
   *   associated with the window
   * - `prop::Window.VIVANTE_WINDOW_POINTER`: the EGLNativeWindowType
   *   associated with the window
   * - `prop::Window.VIVANTE_SURFACE_POINTER`: the EGLSurface associated with
   *   the window
   *
   * On Windows:
   *
   * - `prop::Window.WIN32_HWND_POINTER`: the HWND associated with the window
   * - `prop::Window.WIN32_HDC_POINTER`: the HDC associated with the window
   * - `prop::Window.WIN32_INSTANCE_POINTER`: the HINSTANCE associated with
   *   the window
   *
   * On Wayland:
   *
   * Note: The `xdg_*` window objects do not internally persist across window
   * show/hide calls. They will be null if the window is hidden and must be
   * queried each time it is shown.
   *
   * - `prop::Window.WAYLAND_DISPLAY_POINTER`: the wl_display associated with
   *   the window
   * - `prop::Window.WAYLAND_SURFACE_POINTER`: the wl_surface associated with
   *   the window
   * - `prop::Window.WAYLAND_VIEWPORT_POINTER`: the wp_viewport associated
   *   with the window
   * - `prop::Window.WAYLAND_EGL_WINDOW_POINTER`: the wl_egl_window
   *   associated with the window
   * - `prop::Window.WAYLAND_XDG_SURFACE_POINTER`: the xdg_surface associated
   *   with the window
   * - `prop::Window.WAYLAND_XDG_TOPLEVEL_POINTER`: the xdg_toplevel role
   *   associated with the window
   * - 'prop::Window.WAYLAND_XDG_TOPLEVEL_EXPORT_HANDLE_STRING': the export
   *   handle associated with the window
   * - `prop::Window.WAYLAND_XDG_POPUP_POINTER`: the xdg_popup role
   *   associated with the window
   * - `prop::Window.WAYLAND_XDG_POSITIONER_POINTER`: the xdg_positioner
   *   associated with the window, in popup mode
   *
   * On X11:
   *
   * - `prop::Window.X11_DISPLAY_POINTER`: the X11 Display associated with
   *   the window
   * - `prop::Window.X11_SCREEN_NUMBER`: the screen number associated with
   *   the window
   * - `prop::Window.X11_WINDOW_NUMBER`: the X11 Window associated with the
   *   window
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return CheckError(SDL_GetWindowProperties(get()));
  }

  /**
   * Get the window flags.
   *
   * @returns a mask of the WindowFlags associated with `window`.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.WindowBase
   * @sa WindowBase.Hide
   * @sa WindowBase.Maximize
   * @sa WindowBase.Minimize
   * @sa WindowBase.SetFullscreen
   * @sa WindowBase.SetMouseGrab
   * @sa WindowBase.Show
   */
  WindowFlags GetFlags() const { return SDL_GetWindowFlags(get()); }

  /**
   * Set the title of a window.
   *
   * This string is expected to be in UTF-8 encoding.
   *
   * @param title the desired window title in UTF-8 format.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetTitle
   */
  void SetTitle(StringParam title)
  {
    CheckError(SDL_SetWindowTitle(get(), title));
  }

  /**
   * Get the title of a window.
   *
   * @returns the title of the window in UTF-8 format or "" if there is no
   *          title.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetTitle
   */
  const char* GetTitle() const { return SDL_GetWindowTitle(get()); }

  /**
   * Set the icon for a window.
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
   * @param icon an SurfaceBase structure containing the icon for the window.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetIcon(SurfaceBase& icon)
  {
    CheckError(SDL_SetWindowIcon(get(), icon.get()));
  }

  /**
   * @brief Request the window's position and size to be set.
   *
   * @param rect the rect containing the new coordinates
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa WindowBase.SetPosition()
   * @sa WindowBase.SetSize()
   */
  void SetRect(Rect rect)
  {
    SetPosition(rect.GetTopLeft());
    SetSize(rect.GetSize());
  }

  /**
   * Get the position and client size of a window.
   *
   * This is the current position of the window as last reported by the
   * windowing system.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use Window.GetSizeInPixels()
   * or RendererBase.GetOutputSize() to get the real client area size in pixels.
   *
   * @return Rect with the position and size
   * @throws Error on failure.
   */
  Rect GetRect() const { return Rect{GetPosition(), GetSize()}; }

  /**
   * Request that the window's position be set.
   *
   * If the window is in an exclusive fullscreen or maximized state, this
   * request has no effect.
   *
   * This can be used to reposition fullscreen-desktop windows onto a different
   * display, however, as exclusive fullscreen windows are locked to a specific
   * display, they can only be repositioned programmatically via
   * WindowBase.SetFullscreenMode().
   *
   * On some windowing systems this request is asynchronous and the new
   * coordinates may not have have been applied immediately upon the return of
   * this function. If an immediate change is required, call WindowBase.Sync()
   * to block until the changes have taken effect.
   *
   * When the window position changes, an EVENT_WINDOW_MOVED event will be
   * emitted with the window's new coordinates. Note that the new coordinates
   * may not match the exact coordinates requested, as some windowing systems
   * can restrict the position of the window in certain scenarios (e.g.
   * constraining the position so the window is always within desktop bounds).
   * Additionally, as this is just a request, it can be denied by the windowing
   * system.
   *
   * @param p the x, y coordinate of the window, or `WINDOWPOS_CENTERED` or
   *          `WINDOWPOS_UNDEFINED`.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetPosition
   * @sa WindowBase.Sync
   */
  void SetPosition(SDL_Point p)
  {
    CheckError(SDL_SetWindowPosition(get(), p.x, p.y));
  }

  /**
   * Get the position of a window.
   *
   * This is the current position of the window as last reported by the
   * windowing system.
   *
   * @returns the position on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetPosition()
   * @sa SetPosition(int *, int *)
   */
  Point GetPosition() const
  {
    Point p;
    GetPosition(&p.x, &p.y);
    return p;
  }

  /**
   * Get the position of a window.
   *
   * This is the current position of the window as last reported by the
   * windowing system.
   *
   * If you do not need the value for one of the positions a nullptr may be
   * passed in the `x` or `y` parameter.
   *
   * @param x a pointer filled in with the x position of the window, may be
   *          nullptr.
   * @param y a pointer filled in with the y position of the window, may be
   *          nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetPosition
   */
  void GetPosition(int* x, int* y) const
  {
    CheckError(SDL_GetWindowPosition(get(), x, y));
  }

  /**
   * Request that the size of a window's client area be set.
   *
   * If the window is in a fullscreen or maximized state, this request has no
   * effect.
   *
   * To change the exclusive fullscreen mode of a window, use
   * WindowBase.SetFullscreenMode().
   *
   * On some windowing systems, this request is asynchronous and the new window
   * size may not have have been applied immediately upon the return of this
   * function. If an immediate change is required, call WindowBase.Sync() to
   * block until the changes have taken effect.
   *
   * When the window size changes, an EVENT_WINDOW_RESIZED event will be
   * emitted with the new window dimensions. Note that the new dimensions may
   * not match the exact size requested, as some windowing systems can restrict
   * the window size in certain scenarios (e.g. constraining the size of the
   * content area to remain within the usable desktop bounds). Additionally, as
   * this is just a request, it can be denied by the windowing system.
   *
   * @param p the width, height of the window, both must be > 0.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetSize
   * @sa WindowBase.SetFullscreenMode
   * @sa WindowBase.Sync
   */
  void SetSize(SDL_Point p) { CheckError(SDL_SetWindowSize(get(), p.x, p.y)); }

  /**
   * Get the size of a window's client area.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use GetSizeInPixels()
   * or Renderer.GetOutputSize() to get the real client area size in pixels.
   *
   * @returns a point with width and height on success or std::nullopt on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetOutputSize()
   * @sa GetSizeInPixels()
   * @sa SetSize()
   * @sa GetSize(int *, int *)
   */
  Point GetSize() const
  {
    Point p;
    GetSize(&p.x, &p.y);
    return p;
  }

  /**
   * Get the size of a window's client area.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use WindowBase.GetSizeInPixels()
   * or RendererBase.GetOutputSize() to get the real client area size in pixels.
   *
   * @param w a pointer filled in with the width of the window, may be nullptr.
   * @param h a pointer filled in with the height of the window, may be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RendererBase.GetOutputSize
   * @sa WindowBase.GetSizeInPixels
   * @sa WindowBase.SetSize
   */
  void GetSize(int* w, int* h) const
  {
    CheckError(SDL_GetWindowSize(get(), w, h));
  }

  /**
   * Get the safe area for this window.
   *
   * Some devices have portions of the screen which are partially obscured or
   * not interactive, possibly due to on-screen controls, curved edges, camera
   * notches, TV overscan, etc. This function provides the area of the window
   * which is safe to have interactable content. You should continue rendering
   * into the rest of the window, but it should not contain visually important
   * or interactible content.
   *
   * @returns the client area that is safe for interactive content on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Rect GetSafeArea() const
  {
    Rect rect;
    CheckError(SDL_GetWindowSafeArea(get(), &rect));
    return rect;
  }

  /**
   * Request that the aspect ratio of a window's client area be set.
   *
   * The aspect ratio is the ratio of width divided by height, e.g. 2560x1600
   * would be 1.6. Larger aspect ratios are wider and smaller aspect ratios are
   * narrower.
   *
   * If, at the time of this request, the window in a fixed-size state, such as
   * maximized or fullscreen, the request will be deferred until the window
   * exits this state and becomes resizable again.
   *
   * On some windowing systems, this request is asynchronous and the new window
   * aspect ratio may not have have been applied immediately upon the return of
   * this function. If an immediate change is required, call WindowBase.Sync()
   * to block until the changes have taken effect.
   *
   * When the window size changes, an EVENT_WINDOW_RESIZED event will be
   * emitted with the new window dimensions. Note that the new dimensions may
   * not match the exact aspect ratio requested, as some windowing systems can
   * restrict the window size in certain scenarios (e.g. constraining the size
   * of the content area to remain within the usable desktop bounds).
   * Additionally, as this is just a request, it can be denied by the windowing
   * system.
   *
   * @param min_aspect the minimum aspect ratio of the window, or 0.0f for no
   *                   limit.
   * @param max_aspect the maximum aspect ratio of the window, or 0.0f for no
   *                   limit.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetAspectRatio
   * @sa WindowBase.Sync
   */
  void SetAspectRatio(float min_aspect, float max_aspect)
  {
    CheckError(SDL_SetWindowAspectRatio(get(), min_aspect, max_aspect));
  }

  /**
   * Get the size of a window's client area.
   *
   * @param min_aspect a pointer filled in with the minimum aspect ratio of the
   *                   window, may be nullptr.
   * @param max_aspect a pointer filled in with the maximum aspect ratio of the
   *                   window, may be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetAspectRatio
   */
  void GetAspectRatio(float* min_aspect, float* max_aspect) const
  {
    CheckError(SDL_GetWindowAspectRatio(get(), min_aspect, max_aspect));
  }

  /**
   * Get the size of a window's borders (decorations) around the client area.
   *
   * Note: If this function fails (returns false), the size values will be
   * initialized to 0, 0, 0, 0 (if a non-nullptr pointer is provided), as if the
   * window in question was borderless.
   *
   * Note: This function may fail on systems where the window has not yet been
   * decorated by the display server (for example, immediately after calling
   * WindowBase.WindowBase). It is recommended that you wait at least until the
   * window has been presented and composited, so that the window system has a
   * chance to decorate the window and provide the border dimensions to SDL.
   *
   * This function also returns false if getting the information is not
   * supported.
   *
   * @param top pointer to variable for storing the size of the top border;
   *            nullptr is permitted.
   * @param left pointer to variable for storing the size of the left border;
   *             nullptr is permitted.
   * @param bottom pointer to variable for storing the size of the bottom
   *               border; nullptr is permitted.
   * @param right pointer to variable for storing the size of the right border;
   *              nullptr is permitted.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetSize
   */
  void GetBordersSize(int* top, int* left, int* bottom, int* right) const
  {
    CheckError(SDL_GetWindowBordersSize(get(), top, left, bottom, right));
  }

  /**
   * Get the size of a window's client area, in pixels.
   *
   * @returns the size on success or std::nullopt on failure; call GetError()
   * for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   * @sa GetSizeInPixels(int*, int*)
   */
  Point GetSizeInPixels() const
  {
    Point p;
    GetSizeInPixels(&p.x, &p.y);
    return p;
  }

  /**
   * Get the size of a window's client area, in pixels.
   *
   * @param w a pointer to variable for storing the width in pixels, may be
   *          nullptr.
   * @param h a pointer to variable for storing the height in pixels, may be
   *          nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.WindowBase
   * @sa WindowBase.GetSize
   */
  void GetSizeInPixels(int* w, int* h) const
  {
    CheckError(SDL_GetWindowSizeInPixels(get(), w, h));
  }

  /**
   * Set the minimum size of a window's client area.
   *
   * @param p the minimum width and height of the window, or 0 for no limit.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetMinimumSize
   * @sa WindowBase.SetMaximumSize
   */
  void SetMinimumSize(SDL_Point p)
  {
    CheckError(SDL_SetWindowMinimumSize(get(), p.x, p.y));
  }

  /**
   * Get the minimum size of a window's client area.
   *
   * @param w a pointer filled in with the minimum width of the window, may be
   *          nullptr.
   * @param h a pointer filled in with the minimum height of the window, may be
   *          nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetMaximumSize
   * @sa WindowBase.SetMinimumSize
   */
  void GetMinimumSize(int* w, int* h) const
  {
    CheckError(SDL_GetWindowMinimumSize(get(), w, h));
  }

  /**
   * Set the maximum size of a window's client area.
   *
   * @param p the maximum width and height of the window, or 0 for no limit.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetMaximumSize
   * @sa WindowBase.SetMinimumSize
   */
  void SetMaximumSize(SDL_Point p)
  {
    CheckError(SDL_SetWindowMaximumSize(get(), p.x, p.y));
  }

  /**
   * Get the maximum size of a window's client area.
   *
   * @param w a pointer filled in with the maximum width of the window, may be
   *          nullptr.
   * @param h a pointer filled in with the maximum height of the window, may be
   *          nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetMinimumSize
   * @sa WindowBase.SetMaximumSize
   */
  void GetMaximumSize(int* w, int* h) const
  {
    CheckError(SDL_GetWindowMaximumSize(get(), w, h));
  }

  /**
   * Set the border state of a window.
   *
   * This will add or remove the window's `WINDOW_BORDERLESS` flag and add
   * or remove the border from the actual window. This is a no-op if the
   * window's border already matches the requested state.
   *
   * You can't change the border state of a fullscreen window.
   *
   * @param bordered false to remove border, true to add border.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetFlags
   */
  void SetBordered(bool bordered)
  {
    CheckError(SDL_SetWindowBordered(get(), bordered));
  }

  /**
   * Set the user-resizable state of a window.
   *
   * This will add or remove the window's `WINDOW_RESIZABLE` flag and
   * allow/disallow user resizing of the window. This is a no-op if the window's
   * resizable state already matches the requested state.
   *
   * You can't change the resizable state of a fullscreen window.
   *
   * @param resizable true to allow resizing, false to disallow.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetFlags
   */
  void SetResizable(bool resizable)
  {
    CheckError(SDL_SetWindowResizable(get(), resizable));
  }

  /**
   * Set the window to always be above the others.
   *
   * This will add or remove the window's `WINDOW_ALWAYS_ON_TOP` flag. This
   * will bring the window to the front and keep the window above the rest.
   *
   * @param on_top true to set the window always on top, false to disable.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetFlags
   */
  void SetAlwaysOnTop(bool on_top)
  {
    CheckError(SDL_SetWindowAlwaysOnTop(get(), on_top));
  }

  /**
   * Show a window.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.Hide
   * @sa WindowBase.Raise
   */
  void Show() { CheckError(SDL_ShowWindow(get())); }

  /**
   * Hide a window.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.Show
   * @sa WINDOW_HIDDEN
   */
  void Hide() { CheckError(SDL_HideWindow(get())); }

  /**
   * Request that a window be raised above other windows and gain the input
   * focus.
   *
   * The result of this request is subject to desktop window manager policy,
   * particularly if raising the requested window would result in stealing focus
   * from another application. If the window is successfully raised and gains
   * input focus, an EVENT_WINDOW_FOCUS_GAINED event will be emitted, and
   * the window will have the WINDOW_INPUT_FOCUS flag set.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Raise() { CheckError(SDL_RaiseWindow(get())); }

  /**
   * Request that the window be made as large as possible.
   *
   * Non-resizable windows can't be maximized. The window must have the
   * WINDOW_RESIZABLE flag set, or this will have no effect.
   *
   * On some windowing systems this request is asynchronous and the new window
   * state may not have have been applied immediately upon the return of this
   * function. If an immediate change is required, call WindowBase.Sync() to
   * block until the changes have taken effect.
   *
   * When the window state changes, an EVENT_WINDOW_MAXIMIZED event will be
   * emitted. Note that, as this is just a request, the windowing system can
   * deny the state change.
   *
   * When maximizing a window, whether the constraints set via
   * WindowBase.SetMaximumSize() are honored depends on the policy of the window
   * manager. Win32 and macOS enforce the constraints when maximizing, while X11
   * and Wayland window managers may vary.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.Minimize
   * @sa WindowBase.Restore
   * @sa WindowBase.Sync
   */
  void Maximize() { CheckError(SDL_MaximizeWindow(get())); }

  /**
   * Request that the window be minimized to an iconic representation.
   *
   * If the window is in a fullscreen state, this request has no direct effect.
   * It may alter the state the window is returned to when leaving fullscreen.
   *
   * On some windowing systems this request is asynchronous and the new window
   * state may not have been applied immediately upon the return of this
   * function. If an immediate change is required, call WindowBase.Sync() to
   * block until the changes have taken effect.
   *
   * When the window state changes, an EVENT_WINDOW_MINIMIZED event will be
   * emitted. Note that, as this is just a request, the windowing system can
   * deny the state change.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.Maximize
   * @sa WindowBase.Restore
   * @sa WindowBase.Sync
   */
  void Minimize() { CheckError(SDL_MinimizeWindow(get())); }

  /**
   * Request that the size and position of a minimized or maximized window be
   * restored.
   *
   * If the window is in a fullscreen state, this request has no direct effect.
   * It may alter the state the window is returned to when leaving fullscreen.
   *
   * On some windowing systems this request is asynchronous and the new window
   * state may not have have been applied immediately upon the return of this
   * function. If an immediate change is required, call WindowBase.Sync() to
   * block until the changes have taken effect.
   *
   * When the window state changes, an EVENT_WINDOW_RESTORED event will be
   * emitted. Note that, as this is just a request, the windowing system can
   * deny the state change.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.Maximize
   * @sa WindowBase.Minimize
   * @sa WindowBase.Sync
   */
  void Restore() { CheckError(SDL_RestoreWindow(get())); }

  /**
   * Request that the window's fullscreen state be changed.
   *
   * By default a window in fullscreen state uses borderless fullscreen desktop
   * mode, but a specific exclusive display mode can be set using
   * WindowBase.SetFullscreenMode().
   *
   * On some windowing systems this request is asynchronous and the new
   * fullscreen state may not have have been applied immediately upon the return
   * of this function. If an immediate change is required, call
   * WindowBase.Sync() to block until the changes have taken effect.
   *
   * When the window state changes, an EVENT_WINDOW_ENTER_FULLSCREEN or
   * EVENT_WINDOW_LEAVE_FULLSCREEN event will be emitted. Note that, as this
   * is just a request, it can be denied by the windowing system.
   *
   * @param fullscreen true for fullscreen mode, false for windowed mode.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetFullscreenMode
   * @sa WindowBase.SetFullscreenMode
   * @sa WindowBase.Sync
   * @sa WINDOW_FULLSCREEN
   */
  void SetFullscreen(bool fullscreen)
  {
    CheckError(SDL_SetWindowFullscreen(get(), fullscreen));
  }

  /**
   * Block until any pending window state is finalized.
   *
   * On asynchronous windowing systems, this acts as a synchronization barrier
   * for pending window state. It will attempt to wait until any pending window
   * state has been applied and is guaranteed to return within finite time. Note
   * that for how long it can potentially block depends on the underlying window
   * system, as window state changes may involve somewhat lengthy animations
   * that must complete before the window is in its final requested state.
   *
   * On windowing systems where changes are immediate, this does nothing.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetSize
   * @sa WindowBase.SetPosition
   * @sa WindowBase.SetFullscreen
   * @sa WindowBase.Minimize
   * @sa WindowBase.Maximize
   * @sa WindowBase.Restore
   * @sa SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS
   */
  void Sync() { CheckError(SDL_SyncWindow(get())); }

  /**
   * Return whether the window has a surface associated with it.
   *
   * @returns true if there is a surface associated with the window, or false
   *          otherwise.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetSurface
   */
  bool HasSurface() const { return SDL_WindowHasSurface(get()); }

  /**
   * Get the SDL surface associated with the window.
   *
   * A new surface will be created with the optimal format for the window, if
   * necessary. This surface will be freed when the window is destroyed. Do not
   * free this surface.
   *
   * This surface will be invalidated if the window is resized. After resizing a
   * window this function must be called again to return a valid surface.
   *
   * You may not combine this with 3D or the rendering API on this window.
   *
   * This function is affected by `SDL_HINT_FRAMEBUFFER_ACCELERATION`.
   *
   * @returns the surface associated with the window, or nullptr on failure;
   *          call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.DestroySurface
   * @sa WindowBase.HasSurface
   * @sa WindowBase.UpdateSurface
   * @sa WindowBase.UpdateSurfaceRects
   */
  SurfaceRef GetSurface() { return SDL_GetWindowSurface(get()); }

  /**
   * Toggle VSync for the window surface.
   *
   * When a window surface is created, vsync defaults to
   * SDL_WINDOW_SURFACE_VSYNC_DISABLED.
   *
   * The `vsync` parameter can be 1 to synchronize present with every vertical
   * refresh, 2 to synchronize present with every second vertical refresh, etc.,
   * SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE for late swap tearing (adaptive vsync),
   * or SDL_WINDOW_SURFACE_VSYNC_DISABLED to disable. Not every value is
   * supported by every driver, so you should check the return value to see
   * whether the requested setting is supported.
   *
   * @param vsync the vertical refresh sync interval.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetSurfaceVSync
   */
  void SetSurfaceVSync(int vsync)
  {
    CheckError(SDL_SetWindowSurfaceVSync(get(), vsync));
  }

  /**
   * Get VSync for the window surface.
   *
   * @returns the current vertical refresh sync interval on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetSurfaceVSync
   */
  int GetSurfaceVSync() const
  {
    int vsync;
    CheckError(SDL_GetWindowSurfaceVSync(get(), &vsync));
    return vsync;
  }

  /**
   * Copy the window surface to the screen.
   *
   * This is the function you use to reflect any changes to the surface on the
   * screen.
   *
   * This function is equivalent to the SDL 1.2 API SDL_Flip().
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetSurface
   * @sa WindowBase.UpdateSurfaceRects
   */
  void UpdateSurface() { CheckError(SDL_UpdateWindowSurface(get())); }

  /**
   * Copy areas of the window surface to the screen.
   *
   * This is the function you use to reflect changes to portions of the surface
   * on the screen.
   *
   * This function is equivalent to the SDL 1.2 API SDL_UpdateRects().
   *
   * Note that this function will update _at least_ the rectangles specified,
   * but this is only intended as an optimization; in practice, this might
   * update more of the screen (or all of the screen!), depending on what method
   * SDL uses to send pixels to the system.
   *
   * @param rects an array of Rect structures representing areas of the
   *              surface to copy, in pixels.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetSurface
   * @sa WindowBase.UpdateSurface
   */
  void UpdateSurfaceRects(SpanRef<const SDL_Rect> rects)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_SINT32);
    CheckError(SDL_UpdateWindowSurfaceRects(get(), rects.data(), rects.size()));
  }

  /**
   * Destroy the surface associated with the window.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetSurface
   * @sa WindowBase.HasSurface
   */
  void DestroySurface() { CheckError(SDL_DestroyWindowSurface(get())); }

  /**
   * Set a window's keyboard grab mode.
   *
   * Keyboard grab enables capture of system keyboard shortcuts like Alt+Tab or
   * the Meta/Super key. Note that not all system keyboard shortcuts can be
   * captured by applications (one example is Ctrl+Alt+Del on Windows).
   *
   * This is primarily intended for specialized applications such as VNC clients
   * or VM frontends. Normal games should not use keyboard grab.
   *
   * When keyboard grab is enabled, SDL will continue to handle Alt+Tab when the
   * window is full-screen to ensure the user is not trapped in your
   * application. If you have a custom keyboard shortcut to exit fullscreen
   * mode, you may suppress this behavior with
   * `SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED`.
   *
   * If the caller enables a grab while another window is currently grabbed, the
   * other window loses its grab in favor of the caller's window.
   *
   * @param grabbed this is true to grab keyboard, and false to release.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetKeyboardGrab
   * @sa WindowBase.SetMouseGrab
   */
  void SetKeyboardGrab(bool grabbed)
  {
    CheckError(SDL_SetWindowKeyboardGrab(get(), grabbed));
  }

  /**
   * Set a window's mouse grab mode.
   *
   * Mouse grab confines the mouse cursor to the window.
   *
   * @param grabbed this is true to grab mouse, and false to release.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetMouseRect
   * @sa WindowBase.SetMouseRect
   * @sa WindowBase.SetMouseGrab
   * @sa WindowBase.SetKeyboardGrab
   */
  void SetMouseGrab(bool grabbed)
  {
    CheckError(SDL_SetWindowMouseGrab(get(), grabbed));
  }

  /**
   * Get a window's keyboard grab mode.
   *
   * @returns true if keyboard is grabbed, and false otherwise.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetKeyboardGrab
   */
  bool GetKeyboardGrab() const { return SDL_GetWindowKeyboardGrab(get()); }

  /**
   * Get a window's mouse grab mode.
   *
   * @returns true if mouse is grabbed, and false otherwise.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetMouseRect
   * @sa WindowBase.SetMouseRect
   * @sa WindowBase.SetMouseGrab
   * @sa WindowBase.SetKeyboardGrab
   */
  bool GetMouseGrab() const { return SDL_GetWindowMouseGrab(get()); }

  /**
   * Confines the cursor to the specified area of a window.
   *
   * Note that this does NOT grab the cursor, it only defines the area a cursor
   * is restricted to when the window has mouse focus.
   *
   * @param rect a rectangle area in window-relative coordinates. If nullptr the
   *             barrier for the specified window will be destroyed.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetMouseRect
   * @sa WindowBase.GetMouseGrab
   * @sa WindowBase.SetMouseGrab
   */
  void SetMouseRect(const SDL_Rect& rect)
  {
    CheckError(SDL_SetWindowMouseRect(get(), &rect));
  }

  /**
   * Get the mouse confinement rectangle of a window.
   *
   * @returns a pointer to the mouse confinement rectangle of a window, or
   *          nullptr if there isn't one.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetMouseRect
   * @sa WindowBase.GetMouseGrab
   * @sa WindowBase.SetMouseGrab
   */
  const SDL_Rect* GetMouseRect() const { return SDL_GetWindowMouseRect(get()); }

  /**
   * Set the opacity for a window.
   *
   * The parameter `opacity` will be clamped internally between 0.0f
   * (transparent) and 1.0f (opaque).
   *
   * This function also returns false if setting the opacity isn't supported.
   *
   * @param opacity the opacity value (0.0f - transparent, 1.0f - opaque).
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetOpacity
   */
  void SetOpacity(float opacity)
  {
    CheckError(SDL_SetWindowOpacity(get(), opacity));
  }

  /**
   * Get the opacity of a window.
   *
   * If transparency isn't supported on this platform, opacity will be returned
   * as 1.0f without error.
   *
   * @returns the opacity, (0.0f - transparent, 1.0f - opaque), or -1.0f on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetOpacity
   */
  float GetOpacity() const { return SDL_GetWindowOpacity(get()); }

  /**
   * Set the window as a child of a parent window.
   *
   * If the window is already the child of an existing window, it will be
   * reparented to the new owner. Setting the parent window to nullptr unparents
   * the window and removes child window status.
   *
   * If a parent window is hidden or destroyed, the operation will be
   * recursively applied to child windows. Child windows hidden with the parent
   * that did not have their hidden status explicitly set will be restored when
   * the parent is shown.
   *
   * Attempting to set the parent of a window that is currently in the modal
   * state will fail. Use WindowBase.SetModal() to cancel the modal status
   * before attempting to change the parent.
   *
   * Popup windows cannot change parents and attempts to do so will fail.
   *
   * Setting a parent window that is currently the sibling or descendent of the
   * child window results in undefined behavior.
   *
   * @param parent the new parent window for the child window.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetModal
   */
  void SetParent(OptionalWindow parent);

  /**
   * Toggle the state of the window as modal.
   *
   * To enable modal status on a window, the window must currently be the child
   * window of a parent, or toggling modal status on will fail.
   *
   * @param modal true to toggle modal status on, false to toggle it off.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetParent
   * @sa WINDOW_MODAL
   */
  void SetModal(bool modal) { CheckError(SDL_SetWindowModal(get(), modal)); }

  /**
   * Set whether the window may have input focus.
   *
   * @param focusable true to allow input focus, false to not allow input focus.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetFocusable(bool focusable)
  {
    CheckError(SDL_SetWindowFocusable(get(), focusable));
  }

  /**
   * Display the system-level window menu.
   *
   * This default window menu is provided by the system and on some platforms
   * provides functionality for setting or changing privileged state on the
   * window, such as moving it between workspaces or displays, or toggling the
   * always-on-top property.
   *
   * On platforms or desktops where this is unsupported, this function does
   * nothing.
   *
   * @param p the x, y coordinate of the menu, relative to the origin (top-left)
   *          of the client area.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ShowSystemMenu(SDL_Point p)
  {
    CheckError(SDL_ShowWindowSystemMenu(get(), p.x, p.y));
  }

  /**
   * Provide a callback that decides if a window region has special properties.
   *
   * Normally windows are dragged and resized by decorations provided by the
   * system window manager (a title bar, borders, etc), but for some apps, it
   * makes sense to drag them from somewhere else inside the window itself; for
   * example, one might have a borderless window that wants to be draggable from
   * any part, or simulate its own title bar, etc.
   *
   * This function lets the app provide a callback that designates pieces of a
   * given window as special. This callback is run during event processing if we
   * need to tell the OS to treat a region of the window specially; the use of
   * this callback is known as "hit testing."
   *
   * Mouse input may not be delivered to your application if it is within a
   * special area; the OS will often apply that input to moving the window or
   * resizing the window and not deliver it to the application.
   *
   * Specifying nullptr for a callback disables hit-testing. Hit-testing is
   * disabled by default.
   *
   * Platforms that don't support this functionality will return false
   * unconditionally, even if you're attempting to disable hit-testing.
   *
   * Your callback may fire at any time, and its firing does not indicate any
   * specific behavior (for example, on Windows, this certainly might fire when
   * the OS is deciding whether to drag your window, but it fires for lots of
   * other reasons, too, some unrelated to anything you probably care about _and
   * when the mouse isn't actually at the location it is testing_). Since this
   * can fire at any time, you should try to keep your callback efficient,
   * devoid of allocations, etc.
   *
   * @param callback the function to call when doing a hit-test.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat listener-callback
   */
  void SetHitTest(HitTestCB callback);

  /**
   * Provide a callback that decides if a window region has special properties.
   *
   * Normally windows are dragged and resized by decorations provided by the
   * system window manager (a title bar, borders, etc), but for some apps, it
   * makes sense to drag them from somewhere else inside the window itself; for
   * example, one might have a borderless window that wants to be draggable from
   * any part, or simulate its own title bar, etc.
   *
   * This function lets the app provide a callback that designates pieces of a
   * given window as special. This callback is run during event processing if we
   * need to tell the OS to treat a region of the window specially; the use of
   * this callback is known as "hit testing."
   *
   * Mouse input may not be delivered to your application if it is within a
   * special area; the OS will often apply that input to moving the window or
   * resizing the window and not deliver it to the application.
   *
   * Specifying nullptr for a callback disables hit-testing. Hit-testing is
   * disabled by default.
   *
   * Platforms that don't support this functionality will return false
   * unconditionally, even if you're attempting to disable hit-testing.
   *
   * Your callback may fire at any time, and its firing does not indicate any
   * specific behavior (for example, on Windows, this certainly might fire when
   * the OS is deciding whether to drag your window, but it fires for lots of
   * other reasons, too, some unrelated to anything you probably care about _and
   * when the mouse isn't actually at the location it is testing_). Since this
   * can fire at any time, you should try to keep your callback efficient,
   * devoid of allocations, etc.
   *
   * @param callback the function to call when doing a hit-test.
   * @param callback_data an app-defined void pointer passed to **callback**.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetHitTest(HitTest callback, void* callback_data)
  {
    CheckError(SDL_SetWindowHitTest(get(), callback, callback_data));
  }

  /**
   * Set the shape of a transparent window.
   *
   * This sets the alpha channel of a transparent window and any fully
   * transparent areas are also transparent to mouse clicks. If you are using
   * something besides the SDL render API, then you are responsible for drawing
   * the alpha channel of the window to match the shape alpha channel to get
   * consistent cross-platform results.
   *
   * The shape is copied inside this function, so you can free it afterwards. If
   * your shape surface changes, you should call WindowBase.SetShape() again to
   * update the window. This is an expensive operation, so should be done
   * sparingly.
   *
   * The window must have been created with the WINDOW_TRANSPARENT flag.
   *
   * @param shape the surface representing the shape of the window, or nullptr
   *              to remove any current shape.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetShape(SurfaceBase& shape)
  {
    CheckError(SDL_SetWindowShape(get(), shape.get()));
  }

  /**
   * Request a window to demand attention from the user.
   *
   * @param operation the operation to perform.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Flash(FlashOperation operation)
  {
    CheckError(SDL_FlashWindow(get(), operation));
  }

  RendererRef GetRenderer() const;

  void StartTextInput();

  void StartTextInput(PropertiesBase& props);

  bool IsTextInputActive() const;

  void StopTextInput();

  void ClearComposition();

  void SetTextInputArea(const SDL_Rect& rect, int cursor);

  void GetTextInputArea(Rect* rect, int* cursor);

  bool IsScreenKeyboardShown() const;

  void WarpMouse(float x, float y);

  void SetRelativeMouseMode(bool enabled);

  bool GetRelativeMouseMode() const;
};

/**
 * Handle to a non owned window
 *
 * @cat resource
 *
 * @sa WindowBase
 * @sa Window
 */
struct WindowRef : WindowBase
{
  using WindowBase::WindowBase;

  /**
   * Copy constructor.
   */
  constexpr WindowRef(const WindowRef& other)
    : WindowBase(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr WindowRef(WindowRef&& other)
    : WindowBase(other.release())
  {
  }

  /**
   * Default constructor
   */
  constexpr ~WindowRef() = default;

  /**
   * Assignment operator.
   */
  WindowRef& operator=(WindowRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Destroy a window.
   *
   * Any child windows owned by the window will be recursively destroyed as
   * well.
   *
   * Note that on some platforms, the visible window may not actually be removed
   * from the screen until the SDL event loop is pumped again, even though the
   * WindowBase is no longer valid after this call.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void reset(SDL_Window* newResource = {})
  {
    SDL_DestroyWindow(release(newResource));
  }

  /**
   * Get a window from a stored ID.
   *
   * The numeric ID is what WindowEvent references, and is necessary to map
   * these events to specific WindowBase objects.
   *
   * @param id the ID of the window.
   * @returns the window associated with `id` or nullptr if it doesn't exist;
   * call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.GetID
   */
  static WindowRef FromID(WindowID id);

  /**
   * Get the window that currently has an input grab enabled.
   *
   * @returns the window if input is grabbed or nullptr otherwise.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa WindowBase.SetMouseGrab
   * @sa WindowBase.SetKeyboardGrab
   */
  static WindowRef GetGrabbed();
};

/**
 * Handle to an owned window
 *
 * @cat resource
 *
 * @sa WindowBase
 * @sa WindowRef
 */
struct Window : WindowRef
{
  using WindowRef::WindowRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Window(SDL_Window* resource = {})
    : WindowRef(resource)
  {
  }

  constexpr Window(const Window& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Window(Window&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Window() { reset(); }

  /**
   * Assignment operator.
   */
  Window& operator=(Window other)
  {
    reset(other.release());
    return *this;
  }
};

#ifdef SDL3PP_DOC

/**
 * A magic value used with SDL_WINDOWPOS_UNDEFINED.
 *
 * Generally this macro isn't used directly, but rather through
 * SDL_WINDOWPOS_UNDEFINED or SDL_WINDOWPOS_UNDEFINED_DISPLAY.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_WINDOWPOS_UNDEFINED_MASK 0x1FFF0000u

/**
 * Used to indicate that you don't care what the window position is.
 *
 * If you _really_ don't care, SDL_WINDOWPOS_UNDEFINED is the same, but always
 * uses the primary display instead of specifying one.
 *
 * @param X the Display of the display to use.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_WINDOWPOS_UNDEFINED_DISPLAY(X) (SDL_WINDOWPOS_UNDEFINED_MASK | (X))

/**
 * Used to indicate that you don't care what the window position/display is.
 *
 * This always uses the primary display.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_WINDOWPOS_UNDEFINED SDL_WINDOWPOS_UNDEFINED_DISPLAY(0)

/**
 * A macro to test if the window position is marked as "undefined."
 *
 * @param X the window position value.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_WINDOWPOS_ISUNDEFINED(X)                                           \
  (((X) & 0xFFFF0000) == SDL_WINDOWPOS_UNDEFINED_MASK)

/**
 * A magic value used with SDL_WINDOWPOS_CENTERED.
 *
 * Generally this macro isn't used directly, but rather through
 * SDL_WINDOWPOS_CENTERED or SDL_WINDOWPOS_CENTERED_DISPLAY.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_WINDOWPOS_CENTERED_MASK 0x2FFF0000u

/**
 * Used to indicate that the window position should be centered.
 *
 * SDL_WINDOWPOS_CENTERED is the same, but always uses the primary display
 * instead of specifying one.
 *
 * @param X the Display of the display to use.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_WINDOWPOS_CENTERED_DISPLAY(X) (SDL_WINDOWPOS_CENTERED_MASK | (X))

/**
 * Used to indicate that the window position should be centered.
 *
 * This always uses the primary display.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_WINDOWPOS_CENTERED SDL_WINDOWPOS_CENTERED_DISPLAY(0)

/**
 * A macro to test if the window position is marked as "centered."
 *
 * @param X the window position value.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_WINDOWPOS_ISCENTERED(X)                                            \
  (((X) & 0xFFFF0000) == SDL_WINDOWPOS_CENTERED_MASK)

#endif // SDL3PP_DOC

/**
 * An opaque handle to an OpenGL context.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GLContextBase.GLContextBase
 *
 * @cat resource
 *
 * @sa GLContext
 * @sa GLContextRef
 */
struct GLContextBase : Resource<SDL_GLContextState*>
{
  using Resource::Resource;

  /**
   * Create an OpenGL context for an OpenGL window, and make it current.
   *
   * Windows users new to OpenGL should note that, for historical reasons, GL
   * functions added after OpenGL version 1.1 are not available by default.
   * Those functions must be loaded at run-time, either with an OpenGL
   * extension-handling library or with GL_GetProcAddress() and its related
   * functions.
   *
   * GLContextBase is opaque to the application.
   *
   * @param window the window to associate with the context.
   * @post the OpenGL context associated with `window`.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GLContextRef.reset
   * @sa GLContextBase.MakeCurrent
   */
  GLContextBase(WindowBase& window)
    : Resource(CheckError(SDL_GL_CreateContext(window.get())))
  {
  }

  /**
   * Set up an OpenGL context for rendering into an OpenGL window.
   *
   * The context must have been created with a compatible window.
   *
   * @param window the window to associate with the context.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GLContextBase.GLContextBase
   */
  void MakeCurrent(WindowBase& window)
  {
    CheckError(SDL_GL_MakeCurrent(window.get(), get()));
  }
};

/**
 * Handle to a non owned gLContext
 *
 * @cat resource
 *
 * @sa GLContextBase
 * @sa GLContext
 */
struct GLContextRef : GLContextBase
{
  using GLContextBase::GLContextBase;

  /**
   * Copy constructor.
   */
  constexpr GLContextRef(const GLContextRef& other)
    : GLContextBase(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr GLContextRef(GLContextRef&& other)
    : GLContextBase(other.release())
  {
  }

  /**
   * Default constructor
   */
  constexpr ~GLContextRef() = default;

  /**
   * Assignment operator.
   */
  GLContextRef& operator=(GLContextRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Delete an OpenGL context.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GLContextBase.GLContextBase
   */
  bool reset(SDL_GLContextState* newResource = {})
  {
    return SDL_GL_DestroyContext(release(newResource));
  }
};

/**
 * Handle to an owned gLContext
 *
 * @cat resource
 *
 * @sa GLContextBase
 * @sa GLContextRef
 */
struct GLContext : GLContextRef
{
  using GLContextRef::GLContextRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit GLContext(SDL_GLContextState* resource = {})
    : GLContextRef(resource)
  {
  }

  constexpr GLContext(const GLContext& other) = delete;

  /**
   * Move constructor.
   */
  constexpr GLContext(GLContext&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~GLContext() { reset(); }

  /**
   * Assignment operator.
   */
  GLContext& operator=(GLContext other)
  {
    reset(other.release());
    return *this;
  }
};

/**
 * Opaque type for an EGL display.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLDisplay = SDL_EGLDisplay;

/**
 * Opaque type for an EGL config.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLConfig = SDL_EGLConfig;

/**
 * Opaque type for an EGL surface.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLSurface = SDL_EGLSurface;

/**
 * An EGL attribute, used when creating an EGL context.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLAttrib = SDL_EGLAttrib;

/**
 * An EGL integer attribute, used when creating an EGL surface.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLint = SDL_EGLint;

/**
 * EGL platform attribute initialization callback.
 *
 * This is called when SDL is attempting to create an EGL context, to let the
 * app add extra attributes to its eglGetPlatformDisplay() call.
 *
 * The callback should return a pointer to an EGL attribute array terminated
 * with `EGL_NONE`. If this function returns nullptr, the WindowBase.WindowBase
 * process will fail gracefully.
 *
 * The returned pointer should be allocated with malloc() and will be
 * passed to free().
 *
 * The arrays returned by each callback will be appended to the existing
 * attribute arrays defined by SDL.
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 * @returns a newly-allocated array of attributes, terminated with `EGL_NONE`.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EGL_SetAttributeCallbacks
 */
using EGLAttribArrayCallback = SDL_EGLAttribArrayCallback;

/**
 * EGL platform attribute initialization callback.
 *
 * This is called when SDL is attempting to create an EGL context, to let the
 * app add extra attributes to its eglGetPlatformDisplay() call.
 *
 * The callback should return a pointer to an EGL attribute array terminated
 * with `EGL_NONE`. If this function returns nullptr, the WindowBase.WindowBase
 * process will fail gracefully.
 *
 * The returned pointer should be allocated with malloc() and will be
 * passed to free().
 *
 * The arrays returned by each callback will be appended to the existing
 * attribute arrays defined by SDL.
 *
 * @returns a newly-allocated array of attributes, terminated with `EGL_NONE`.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EGL_SetAttributeCallbacks
 * @sa EGLAttribArrayCallback
 */
using EGLAttribArrayCB = std::function<SDL_EGLAttrib*()>;

/**
 * EGL surface/context attribute initialization callback types.
 *
 * This is called when SDL is attempting to create an EGL surface, to let the
 * app add extra attributes to its eglCreateWindowSurface() or
 * eglCreateContext calls.
 *
 * For convenience, the EGLDisplay and EGLConfig to use are provided to the
 * callback.
 *
 * The callback should return a pointer to an EGL attribute array terminated
 * with `EGL_NONE`. If this function returns nullptr, the WindowBase.WindowBase
 * process will fail gracefully.
 *
 * The returned pointer should be allocated with malloc() and will be
 * passed to free().
 *
 * The arrays returned by each callback will be appended to the existing
 * attribute arrays defined by SDL.
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 * @param display the EGL display to be used.
 * @param config the EGL config to be used.
 * @returns a newly-allocated array of attributes, terminated with `EGL_NONE`.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EGL_SetAttributeCallbacks
 */
using EGLIntArrayCallback = SDL_EGLIntArrayCallback;

/**
 * EGL surface/context attribute initialization callback types.
 *
 * This is called when SDL is attempting to create an EGL surface, to let the
 * app add extra attributes to its eglCreateWindowSurface() or
 * eglCreateContext calls.
 *
 * For convenience, the EGLDisplay and EGLConfig to use are provided to the
 * callback.
 *
 * The callback should return a pointer to an EGL attribute array terminated
 * with `EGL_NONE`. If this function returns nullptr, the WindowBase.WindowBase
 * process will fail gracefully.
 *
 * The returned pointer should be allocated with malloc() and will be
 * passed to free().
 *
 * The arrays returned by each callback will be appended to the existing
 * attribute arrays defined by SDL.
 *
 * @param display the EGL display to be used.
 * @param config the EGL config to be used.
 * @returns a newly-allocated array of attributes, terminated with `EGL_NONE`.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EGL_SetAttributeCallbacks
 * @sa EGLIntArrayCallback
 */
using EGLIntArrayCB = std::function<SDL_EGLint*(SDL_EGLDisplay, SDL_EGLConfig)>;

/**
 * @name GLAttrs
 * @{
 */

/**
 * An enumeration of OpenGL configuration attributes.
 *
 * While you can set most OpenGL attributes normally, the attributes listed
 * above must be known before SDL creates the window that will be used with
 * the OpenGL context. These attributes are set and read with
 * GL_SetAttribute() and GL_GetAttribute().
 *
 * In some cases, these attributes are minimum requests; the GL does not
 * promise to give you exactly what you asked for. It's possible to ask for a
 * 16-bit depth buffer and get a 24-bit one instead, for example, or to ask
 * for no stencil buffer and still have one available. Context creation should
 * fail if the GL can't provide your requested attributes at a minimum, but
 * you should check to see exactly what you got.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using GLAttr = SDL_GLAttr;

/**
 * the minimum number of bits for the red channel of the color buffer; defaults
 * to 8.
 */
constexpr GLAttr GL_RED_SIZE = SDL_GL_RED_SIZE;

/**
 * the minimum number of bits for the green channel of the color buffer;
 * defaults to 8.
 */
constexpr GLAttr GL_GREEN_SIZE = SDL_GL_GREEN_SIZE;

/**
 * the minimum number of bits for the blue channel of the color buffer; defaults
 * to 8.
 */
constexpr GLAttr GL_BLUE_SIZE = SDL_GL_BLUE_SIZE;

/**
 * the minimum number of bits for the alpha channel of the color buffer;
 * defaults to 8.
 */
constexpr GLAttr GL_ALPHA_SIZE = SDL_GL_ALPHA_SIZE;

/**
 * the minimum number of bits for frame buffer size; defaults to 0.
 */
constexpr GLAttr GL_BUFFER_SIZE = SDL_GL_BUFFER_SIZE;

/**
 * whether the output is single or double buffered; defaults to double buffering
 * on.
 */
constexpr GLAttr GL_DOUBLEBUFFER = SDL_GL_DOUBLEBUFFER;

/**
 * the minimum number of bits in the depth buffer; defaults to 16.
 */
constexpr GLAttr GL_DEPTH_SIZE = SDL_GL_DEPTH_SIZE;

/**
 * the minimum number of bits in the stencil buffer; defaults to 0.
 */
constexpr GLAttr GL_STENCIL_SIZE = SDL_GL_STENCIL_SIZE;

/**
 * the minimum number of bits for the red channel of the accumulation buffer;
 * defaults to 0.
 */
constexpr GLAttr GL_ACCUM_RED_SIZE = SDL_GL_ACCUM_RED_SIZE;

/**
 * the minimum number of bits for the green channel of the accumulation buffer;
 * defaults to 0.
 */
constexpr GLAttr GL_ACCUM_GREEN_SIZE = SDL_GL_ACCUM_GREEN_SIZE;

/**
 * the minimum number of bits for the blue channel of the accumulation buffer;
 * defaults to 0.
 */
constexpr GLAttr GL_ACCUM_BLUE_SIZE = SDL_GL_ACCUM_BLUE_SIZE;

/**
 * the minimum number of bits for the alpha channel of the accumulation buffer;
 * defaults to 0.
 */
constexpr GLAttr GL_ACCUM_ALPHA_SIZE = SDL_GL_ACCUM_ALPHA_SIZE;

constexpr GLAttr GL_STEREO =
  SDL_GL_STEREO; ///< whether the output is stereo 3D; defaults to off.

/**
 * the number of buffers used for multisample anti-aliasing; defaults to 0.
 */
constexpr GLAttr GL_MULTISAMPLEBUFFERS = SDL_GL_MULTISAMPLEBUFFERS;

/**
 * the number of samples used around the current pixel used for multisample
 * anti-aliasing.
 */
constexpr GLAttr GL_MULTISAMPLESAMPLES = SDL_GL_MULTISAMPLESAMPLES;

/**
 * set to 1 to require hardware acceleration, set to 0 to force software
 * rendering; defaults to allow either.
 */
constexpr GLAttr GL_ACCELERATED_VISUAL = SDL_GL_ACCELERATED_VISUAL;

constexpr GLAttr GL_RETAINED_BACKING =
  SDL_GL_RETAINED_BACKING; ///< not used (deprecated).

constexpr GLAttr GL_CONTEXT_MAJOR_VERSION =
  SDL_GL_CONTEXT_MAJOR_VERSION; ///< OpenGL context major version.

constexpr GLAttr GL_CONTEXT_MINOR_VERSION =
  SDL_GL_CONTEXT_MINOR_VERSION; ///< OpenGL context minor version.

/**
 * some combination of 0 or more of elements of the GLContextFlag enumeration;
 * defaults to 0.
 */
constexpr GLAttr GL_CONTEXT_FLAGS = SDL_GL_CONTEXT_FLAGS;

/**
 * type of GL context (Core, Compatibility, ES).  See GLProfile; default value
 * depends on platform.
 */
constexpr GLAttr GL_CONTEXT_PROFILE_MASK = SDL_GL_CONTEXT_PROFILE_MASK;

constexpr GLAttr GL_SHARE_WITH_CURRENT_CONTEXT =
  SDL_GL_SHARE_WITH_CURRENT_CONTEXT; ///< OpenGL context sharing; defaults to 0.

constexpr GLAttr GL_FRAMEBUFFER_SRGB_CAPABLE =
  SDL_GL_FRAMEBUFFER_SRGB_CAPABLE; ///< requests sRGB capable visual; defaults
                                   ///< to 0.

/**
 * sets context the release behavior.  See GLContextReleaseFlag; defaults to
 * FLUSH.
 */
constexpr GLAttr GL_CONTEXT_RELEASE_BEHAVIOR = SDL_GL_CONTEXT_RELEASE_BEHAVIOR;

/**
 * set context reset notification.  See GLContextResetNotification; defaults to
 * NO_NOTIFICATION.
 */
constexpr GLAttr GL_CONTEXT_RESET_NOTIFICATION =
  SDL_GL_CONTEXT_RESET_NOTIFICATION;

constexpr GLAttr GL_CONTEXT_NO_ERROR =
  SDL_GL_CONTEXT_NO_ERROR; ///< GL_CONTEXT_NO_ERROR

constexpr GLAttr GL_FLOATBUFFERS = SDL_GL_FLOATBUFFERS; ///< GL_FLOATBUFFERS

constexpr GLAttr GL_EGL_PLATFORM = SDL_GL_EGL_PLATFORM; ///< GL_EGL_PLATFORM

/// @}

/**
 * @name GLProfiles
 * @{
 */

/**
 * Possible values to be set for the GL_CONTEXT_PROFILE_MASK attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLProfile = Uint32;

constexpr GLProfile GL_CONTEXT_PROFILE_CORE =
  SDL_GL_CONTEXT_PROFILE_CORE; ///< OpenGL Core Profile context

constexpr GLProfile GL_CONTEXT_PROFILE_COMPATIBILITY =
  SDL_GL_CONTEXT_PROFILE_COMPATIBILITY; ///< OpenGL Compatibility Profile
                                        ///< context

constexpr GLProfile GL_CONTEXT_PROFILE_ES =
  SDL_GL_CONTEXT_PROFILE_ES; ///< GLX_CONTEXT_ES2_PROFILE_BIT_EXT

/// @}

/**
 * @name GLContextFlags
 * @{
 */

/**
 * Possible flags to be set for the GL_CONTEXT_FLAGS attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLContextFlag = Uint32;

constexpr GLContextFlag GL_CONTEXT_DEBUG_FLAG =
  SDL_GL_CONTEXT_DEBUG_FLAG; ///< DEBUG_FLAG

constexpr GLContextFlag GL_CONTEXT_FORWARD_COMPATIBLE_FLAG =
  SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG; ///< FORWARD_COMPATIBLE_FLAG

constexpr GLContextFlag GL_CONTEXT_ROBUST_ACCESS_FLAG =
  SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG; ///< ROBUST_ACCESS_FLAG

constexpr GLContextFlag GL_CONTEXT_RESET_ISOLATION_FLAG =
  SDL_GL_CONTEXT_RESET_ISOLATION_FLAG; ///< RESET_ISOLATION_FLAG

/// @}

/**
 * @name GLContextFlags
 * @{
 */

/**
 * Possible values to be set for the GL_CONTEXT_RELEASE_BEHAVIOR
 * attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLContextReleaseFlag = Uint32;

constexpr GLContextReleaseFlag GL_CONTEXT_RELEASE_BEHAVIOR_NONE =
  SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE; ///< BEHAVIOR_NONE

constexpr GLContextReleaseFlag GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH =
  SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH; ///< BEHAVIOR_FLUSH

/// @}

/**
 * @name GLContextResetNotifications
 * @{
 */

/**
 * Possible values to be set GL_CONTEXT_RESET_NOTIFICATION attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLContextResetNotification = Uint32;

constexpr GLContextResetNotification GL_CONTEXT_RESET_NO_NOTIFICATION =
  SDL_GL_CONTEXT_RESET_NO_NOTIFICATION; ///< NO_NOTIFICATION

constexpr GLContextResetNotification GL_CONTEXT_RESET_LOSE_CONTEXT =
  SDL_GL_CONTEXT_RESET_LOSE_CONTEXT; ///< LOSE_CONTEXT

/// @}

/**
 * Get the number of video drivers compiled into SDL.
 *
 * @returns the number of built in video drivers.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetVideoDriver
 */
inline int GetNumVideoDrivers() { return SDL_GetNumVideoDrivers(); }

/**
 * Get the name of a built in video driver.
 *
 * The video drivers are presented in the order in which they are normally
 * checked during initialization.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "cocoa",
 * "x11" or "windows". These never have Unicode characters, and are not meant
 * to be proper names.
 *
 * @param index the index of a video driver.
 * @returns the name of the video driver with the given **index**.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumVideoDrivers
 */
inline const char* GetVideoDriver(int index)
{
  return SDL_GetVideoDriver(index);
}

/**
 * Get the name of the currently initialized video driver.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "cocoa",
 * "x11" or "windows". These never have Unicode characters, and are not meant
 * to be proper names.
 *
 * @returns the name of the current video driver or nullptr if no driver has
 *          been initialized.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumVideoDrivers
 * @sa GetVideoDriver
 */
inline const char* GetCurrentVideoDriver()
{
  return SDL_GetCurrentVideoDriver();
}

/**
 * Get the current system theme.
 *
 * @returns the current system theme, light, dark, or unknown.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline SystemTheme GetSystemTheme() { return SDL_GetSystemTheme(); }

namespace prop::Display {

constexpr auto HDR_ENABLED_BOOLEAN = SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN;

constexpr auto KMSDRM_PANEL_ORIENTATION_NUMBER =
  SDL_PROP_DISPLAY_KMSDRM_PANEL_ORIENTATION_NUMBER;

} // namespace prop::Display

/**
 * Get a list of valid windows.
 *
 * @returns a nullptr terminated array of WindowRef pointers.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<WindowRef> GetWindows()
{
  int count = 0;
  auto data = CheckError(reinterpret_cast<WindowRef*>(SDL_GetWindows(&count)));
  return OwnArray<WindowRef>{data, size_t(count)};
}

namespace prop::Window {

constexpr auto CREATE_ALWAYS_ON_TOP_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN;

constexpr auto CREATE_BORDERLESS_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN;

constexpr auto CREATE_FOCUSABLE_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN;

constexpr auto CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN;

constexpr auto CREATE_FLAGS_NUMBER = SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER;

constexpr auto CREATE_FULLSCREEN_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN;

constexpr auto CREATE_HEIGHT_NUMBER = SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER;

constexpr auto CREATE_HIDDEN_BOOLEAN = SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN;

constexpr auto CREATE_HIGH_PIXEL_DENSITY_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN;

constexpr auto CREATE_MAXIMIZED_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN;

constexpr auto CREATE_MENU_BOOLEAN = SDL_PROP_WINDOW_CREATE_MENU_BOOLEAN;

constexpr auto CREATE_METAL_BOOLEAN = SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN;

constexpr auto CREATE_MINIMIZED_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN;

constexpr auto CREATE_MODAL_BOOLEAN = SDL_PROP_WINDOW_CREATE_MODAL_BOOLEAN;

constexpr auto CREATE_MOUSE_GRABBED_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN;

constexpr auto CREATE_OPENGL_BOOLEAN = SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN;

constexpr auto CREATE_PARENT_POINTER = SDL_PROP_WINDOW_CREATE_PARENT_POINTER;

constexpr auto CREATE_RESIZABLE_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN;

constexpr auto CREATE_TITLE_STRING = SDL_PROP_WINDOW_CREATE_TITLE_STRING;

constexpr auto CREATE_TRANSPARENT_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN;

constexpr auto CREATE_TOOLTIP_BOOLEAN = SDL_PROP_WINDOW_CREATE_TOOLTIP_BOOLEAN;

constexpr auto CREATE_UTILITY_BOOLEAN = SDL_PROP_WINDOW_CREATE_UTILITY_BOOLEAN;

constexpr auto CREATE_VULKAN_BOOLEAN = SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN;

constexpr auto CREATE_WIDTH_NUMBER = SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER;

constexpr auto CREATE_X_NUMBER = SDL_PROP_WINDOW_CREATE_X_NUMBER;

constexpr auto CREATE_Y_NUMBER = SDL_PROP_WINDOW_CREATE_Y_NUMBER;

constexpr auto CREATE_COCOA_WINDOW_POINTER =
  SDL_PROP_WINDOW_CREATE_COCOA_WINDOW_POINTER;

constexpr auto CREATE_COCOA_VIEW_POINTER =
  SDL_PROP_WINDOW_CREATE_COCOA_VIEW_POINTER;

constexpr auto CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN;

constexpr auto CREATE_WAYLAND_CREATE_EGL_WINDOW_BOOLEAN =
  SDL_PROP_WINDOW_CREATE_WAYLAND_CREATE_EGL_WINDOW_BOOLEAN;

constexpr auto CREATE_WAYLAND_WL_SURFACE_POINTER =
  SDL_PROP_WINDOW_CREATE_WAYLAND_WL_SURFACE_POINTER;

constexpr auto CREATE_WIN32_HWND_POINTER =
  SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER;

constexpr auto CREATE_WIN32_PIXEL_FORMAT_HWND_POINTER =
  SDL_PROP_WINDOW_CREATE_WIN32_PIXEL_FORMAT_HWND_POINTER;

constexpr auto CREATE_X11_WINDOW_NUMBER =
  SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER;

constexpr auto SHAPE_POINTER = SDL_PROP_WINDOW_SHAPE_POINTER;

constexpr auto HDR_ENABLED_BOOLEAN = SDL_PROP_WINDOW_HDR_ENABLED_BOOLEAN;

constexpr auto SDR_WHITE_LEVEL_FLOAT = SDL_PROP_WINDOW_SDR_WHITE_LEVEL_FLOAT;

constexpr auto HDR_HEADROOM_FLOAT = SDL_PROP_WINDOW_HDR_HEADROOM_FLOAT;

constexpr auto ANDROID_WINDOW_POINTER = SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER;

constexpr auto ANDROID_SURFACE_POINTER =
  SDL_PROP_WINDOW_ANDROID_SURFACE_POINTER;

constexpr auto UIKIT_WINDOW_POINTER = SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER;

constexpr auto UIKIT_METAL_VIEW_TAG_NUMBER =
  SDL_PROP_WINDOW_UIKIT_METAL_VIEW_TAG_NUMBER;

constexpr auto UIKIT_OPENGL_FRAMEBUFFER_NUMBER =
  SDL_PROP_WINDOW_UIKIT_OPENGL_FRAMEBUFFER_NUMBER;

constexpr auto UIKIT_OPENGL_RENDERBUFFER_NUMBER =
  SDL_PROP_WINDOW_UIKIT_OPENGL_RENDERBUFFER_NUMBER;

constexpr auto UIKIT_OPENGL_RESOLVE_FRAMEBUFFER_NUMBER =
  SDL_PROP_WINDOW_UIKIT_OPENGL_RESOLVE_FRAMEBUFFER_NUMBER;

constexpr auto KMSDRM_DEVICE_INDEX_NUMBER =
  SDL_PROP_WINDOW_KMSDRM_DEVICE_INDEX_NUMBER;

constexpr auto KMSDRM_DRM_FD_NUMBER = SDL_PROP_WINDOW_KMSDRM_DRM_FD_NUMBER;

constexpr auto KMSDRM_GBM_DEVICE_POINTER =
  SDL_PROP_WINDOW_KMSDRM_GBM_DEVICE_POINTER;

constexpr auto COCOA_WINDOW_POINTER = SDL_PROP_WINDOW_COCOA_WINDOW_POINTER;

constexpr auto COCOA_METAL_VIEW_TAG_NUMBER =
  SDL_PROP_WINDOW_COCOA_METAL_VIEW_TAG_NUMBER;

constexpr auto OPENVR_OVERLAY_ID = SDL_PROP_WINDOW_OPENVR_OVERLAY_ID;

constexpr auto VIVANTE_DISPLAY_POINTER =
  SDL_PROP_WINDOW_VIVANTE_DISPLAY_POINTER;

constexpr auto VIVANTE_WINDOW_POINTER = SDL_PROP_WINDOW_VIVANTE_WINDOW_POINTER;

constexpr auto VIVANTE_SURFACE_POINTER =
  SDL_PROP_WINDOW_VIVANTE_SURFACE_POINTER;

constexpr auto WIN32_HWND_POINTER = SDL_PROP_WINDOW_WIN32_HWND_POINTER;

constexpr auto WIN32_HDC_POINTER = SDL_PROP_WINDOW_WIN32_HDC_POINTER;

constexpr auto WIN32_INSTANCE_POINTER = SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER;

constexpr auto WAYLAND_DISPLAY_POINTER =
  SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER;

constexpr auto WAYLAND_SURFACE_POINTER =
  SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER;

constexpr auto WAYLAND_VIEWPORT_POINTER =
  SDL_PROP_WINDOW_WAYLAND_VIEWPORT_POINTER;

constexpr auto WAYLAND_EGL_WINDOW_POINTER =
  SDL_PROP_WINDOW_WAYLAND_EGL_WINDOW_POINTER;

constexpr auto WAYLAND_XDG_SURFACE_POINTER =
  SDL_PROP_WINDOW_WAYLAND_XDG_SURFACE_POINTER;

constexpr auto WAYLAND_XDG_TOPLEVEL_POINTER =
  SDL_PROP_WINDOW_WAYLAND_XDG_TOPLEVEL_POINTER;

constexpr auto WAYLAND_XDG_TOPLEVEL_EXPORT_HANDLE_STRING =
  SDL_PROP_WINDOW_WAYLAND_XDG_TOPLEVEL_EXPORT_HANDLE_STRING;

constexpr auto WAYLAND_XDG_POPUP_POINTER =
  SDL_PROP_WINDOW_WAYLAND_XDG_POPUP_POINTER;

constexpr auto WAYLAND_XDG_POSITIONER_POINTER =
  SDL_PROP_WINDOW_WAYLAND_XDG_POSITIONER_POINTER;

constexpr auto X11_DISPLAY_POINTER = SDL_PROP_WINDOW_X11_DISPLAY_POINTER;

constexpr auto X11_SCREEN_NUMBER = SDL_PROP_WINDOW_X11_SCREEN_NUMBER;

constexpr auto X11_WINDOW_NUMBER = SDL_PROP_WINDOW_X11_WINDOW_NUMBER;

} // namespace prop::Window

inline WindowRef WindowRef::FromID(WindowID id)
{
  return SDL_GetWindowFromID(id);
}

inline WindowRef WindowBase::GetParent() const
{
  return SDL_GetWindowParent(get());
}

#ifdef SDL3PP_DOC

/// Disable vsync
#define SDL_WINDOW_SURFACE_VSYNC_DISABLED 0

/// Adaptative vsync
#define SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE (-1)

#endif // SDL3PP_DOC

inline WindowRef WindowRef::GetGrabbed() { return SDL_GetGrabbedWindow(); }

/**
 * Check whether the screensaver is currently enabled.
 *
 * The screensaver is disabled by default.
 *
 * The default can also be changed using `SDL_HINT_VIDEO_ALLOW_SCREENSAVER`.
 *
 * @returns true if the screensaver is enabled, false if it is disabled.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa DisableScreenSaver
 * @sa EnableScreenSaver
 */
inline bool ScreenSaverEnabled() { return SDL_ScreenSaverEnabled(); }

/**
 * Allow the screen to be blanked by a screen saver.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa DisableScreenSaver
 * @sa ScreenSaverEnabled
 */
inline void EnableScreenSaver() { CheckError(SDL_EnableScreenSaver()); }

/**
 * Prevent the screen from being blanked by a screen saver.
 *
 * If you disable the screensaver, it is automatically re-enabled when SDL
 * quits.
 *
 * The screensaver is disabled by default, but this may by changed by
 * SDL_HINT_VIDEO_ALLOW_SCREENSAVER.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa EnableScreenSaver
 * @sa ScreenSaverEnabled
 */
inline void DisableScreenSaver() { CheckError(SDL_DisableScreenSaver()); }

/**
 * Dynamically load an OpenGL library.
 *
 * This should be done after initializing the video driver, but before
 * creating any OpenGL windows. If no OpenGL library is loaded, the default
 * library will be loaded upon creation of the first OpenGL window.
 *
 * If you do this, you need to retrieve all of the GL functions used in your
 * program from the dynamic library using GL_GetProcAddress().
 *
 * @param path the platform dependent OpenGL library name, or nullptr to open
 *             the default OpenGL library.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetProcAddress
 * @sa GL_UnloadLibrary
 */
inline void GL_LoadLibrary(StringParam path)
{
  CheckError(SDL_GL_LoadLibrary(path));
}

/**
 * Get an OpenGL function by name.
 *
 * If the GL library is loaded at runtime with GL_LoadLibrary(), then all
 * GL functions must be retrieved this way. Usually this is used to retrieve
 * function pointers to OpenGL extensions.
 *
 * There are some quirks to looking up OpenGL functions that require some
 * extra care from the application. If you code carefully, you can handle
 * these quirks without any platform-specific code, though:
 *
 * - On Windows, function pointers are specific to the current GL context;
 *   this means you need to have created a GL context and made it current
 *   before calling GL_GetProcAddress(). If you recreate your context or
 *   create a second context, you should assume that any existing function
 *   pointers aren't valid to use with it. This is (currently) a
 *   Windows-specific limitation, and in practice lots of drivers don't suffer
 *   this limitation, but it is still the way the wgl API is documented to
 *   work and you should expect crashes if you don't respect it. Store a copy
 *   of the function pointers that comes and goes with context lifespan.
 * - On X11, function pointers returned by this function are valid for any
 *   context, and can even be looked up before a context is created at all.
 *   This means that, for at least some common OpenGL implementations, if you
 *   look up a function that doesn't exist, you'll get a non-nullptr result that
 *   is _NOT_ safe to call. You must always make sure the function is actually
 *   available for a given GL context before calling it, by checking for the
 *   existence of the appropriate extension with GL_ExtensionSupported(),
 *   or verifying that the version of OpenGL you're using offers the function
 *   as core functionality.
 * - Some OpenGL drivers, on all platforms, *will* return nullptr if a function
 *   isn't supported, but you can't count on this behavior. Check for
 *   extensions you use, and if you get a nullptr anyway, act as if that
 *   extension wasn't available. This is probably a bug in the driver, but you
 *   can code defensively for this scenario anyhow.
 * - Just because you're on Linux/Unix, don't assume you'll be using X11.
 *   Next-gen display servers are waiting to replace it, and may or may not
 *   make the same promises about function pointers.
 * - OpenGL function pointers must be declared `APIENTRY` as in the example
 *   code. This will ensure the proper calling convention is followed on
 *   platforms where this matters (Win32) thereby avoiding stack corruption.
 *
 * @param proc the name of an OpenGL function.
 * @returns a pointer to the named OpenGL function. The returned pointer
 *          should be cast to the appropriate function signature.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_ExtensionSupported
 * @sa GL_LoadLibrary
 * @sa GL_UnloadLibrary
 */
inline FunctionPointer GL_GetProcAddress(StringParam proc)
{
  return SDL_GL_GetProcAddress(proc);
}

/**
 * Get an EGL library function by name.
 *
 * If an EGL library is loaded, this function allows applications to get entry
 * points for EGL functions. This is useful to provide to an EGL API and
 * extension loader.
 *
 * @param proc the name of an EGL function.
 * @returns a pointer to the named EGL function. The returned pointer should
 *          be cast to the appropriate function signature.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa EGL_GetCurrentDisplay
 */
inline FunctionPointer EGL_GetProcAddress(StringParam proc)
{
  return SDL_EGL_GetProcAddress(proc);
}

/**
 * Unload the OpenGL library previously loaded by GL_LoadLibrary().
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_LoadLibrary
 */
inline void GL_UnloadLibrary() { SDL_GL_UnloadLibrary(); }

/**
 * Check if an OpenGL extension is supported for the current context.
 *
 * This function operates on the current GL context; you must have created a
 * context and it must be current before calling this function. Do not assume
 * that all contexts you create will have the same set of extensions
 * available, or that recreating an existing context will offer the same
 * extensions again.
 *
 * While it's probably not a massive overhead, this function is not an O(1)
 * operation. Check the extensions you care about after creating the GL
 * context and save that information somewhere instead of calling the function
 * every time you need to know.
 *
 * @param extension the name of the extension to check.
 * @returns true if the extension is supported, false otherwise.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GL_ExtensionSupported(StringParam extension)
{
  return SDL_GL_ExtensionSupported(extension);
}

/**
 * Reset all previously set OpenGL context attributes to their default values.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetAttribute
 * @sa GL_SetAttribute
 */
inline void GL_ResetAttributes() { SDL_GL_ResetAttributes(); }

/**
 * Set an OpenGL window attribute before window creation.
 *
 * This function sets the OpenGL attribute `attr` to `value`. The requested
 * attributes should be set before creating an OpenGL window. You should use
 * GL_GetAttribute() to check the values after creating the OpenGL
 * context, since the values obtained can differ from the requested ones.
 *
 * @param attr an GLAttr enum value specifying the OpenGL attribute to
 *             set.
 * @param value the desired value for the attribute.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetAttribute
 * @sa GL_ResetAttributes
 */
inline void GL_SetAttribute(GLAttr attr, int value)
{
  CheckError(SDL_GL_SetAttribute(attr, value));
}

/**
 * Get the actual value for an attribute from the current context.
 *
 * @param attr an GLAttr enum value specifying the OpenGL attribute to
 *             get.
 * @param value a pointer filled in with the current value of `attr`.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_ResetAttributes
 * @sa GL_SetAttribute
 */
inline void GL_GetAttribute(GLAttr attr, int* value)
{
  CheckError(SDL_GL_GetAttribute(attr, value));
}

/**
 * Get the currently active OpenGL window.
 *
 * @returns the currently active OpenGL window on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline WindowRef GL_GetCurrentWindow()
{
  return CheckError(SDL_GL_GetCurrentWindow());
}

/**
 * Get the currently active OpenGL context.
 *
 * @returns the currently active OpenGL context.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GLContextBase.MakeCurrent
 */
inline GLContextRef GL_GetCurrentContext()
{
  return CheckError(SDL_GL_GetCurrentContext());
}

/**
 * Get the currently active EGL display.
 *
 * @returns the currently active EGL display.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline EGLDisplay EGL_GetCurrentDisplay()
{
  return CheckError(SDL_EGL_GetCurrentDisplay());
}

/**
 * Get the currently active EGL config.
 *
 * @returns the currently active EGL config.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline EGLConfig EGL_GetCurrentConfig()
{
  return CheckError(SDL_EGL_GetCurrentConfig());
}

/**
 * Get the EGL surface associated with the window.
 *
 * @param window the window to query.
 * @returns the EGLSurface pointer associated with the window.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline EGLSurface EGL_GetWindowSurface(WindowBase& window)
{
  return CheckError(SDL_EGL_GetWindowSurface(window.get()));
}

/**
 * Sets the callbacks for defining custom EGLAttrib arrays for EGL
 * initialization.
 *
 * Callbacks that aren't needed can be set to nullptr.
 *
 * NOTE: These callback pointers will be reset after GL_ResetAttributes.
 *
 * @param platformAttribCallback callback for attributes to pass to
 *                               eglGetPlatformDisplay. May be nullptr.
 * @param surfaceAttribCallback callback for attributes to pass to
 *                              eglCreateSurface. May be nullptr.
 * @param contextAttribCallback callback for attributes to pass to
 *                              eglCreateContext. May be nullptr.
 * @param userdata a pointer that is passed to the callbacks.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EGL_SetAttributeCallbacks(
  EGLAttribArrayCallback platformAttribCallback,
  EGLIntArrayCallback surfaceAttribCallback,
  EGLIntArrayCallback contextAttribCallback,
  void* userdata)
{
  SDL_EGL_SetAttributeCallbacks(platformAttribCallback,
                                surfaceAttribCallback,
                                contextAttribCallback,
                                userdata);
}

/**
 * Set the swap interval for the current OpenGL context.
 *
 * Some systems allow specifying -1 for the interval, to enable adaptive
 * vsync. Adaptive vsync works the same as vsync, but if you've already missed
 * the vertical retrace for a given frame, it swaps buffers immediately, which
 * might be less jarring for the user during occasional framerate drops. If an
 * application requests adaptive vsync and the system does not support it,
 * this function will fail and return false. In such a case, you should
 * probably retry the call with 1 for the interval.
 *
 * Adaptive vsync is implemented for some glX drivers with
 * GLX_EXT_swap_control_tear, and for some Windows drivers with
 * WGL_EXT_swap_control_tear.
 *
 * Read more on the Khronos wiki:
 * https://www.khronos.org/opengl/wiki/Swap_Interval#Adaptive_Vsync
 *
 * @param interval 0 for immediate updates, 1 for updates synchronized with
 *                 the vertical retrace, -1 for adaptive vsync.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetSwapInterval
 */
inline void GL_SetSwapInterval(int interval)
{
  CheckError(SDL_GL_SetSwapInterval(interval));
}

/**
 * Get the swap interval for the current OpenGL context.
 *
 * If the system can't determine the swap interval, or there isn't a valid
 * current context, this function will set *interval to 0 as a safe default.
 *
 * @param interval output interval value. 0 if there is no vertical retrace
 *                 synchronization, 1 if the buffer swap is synchronized with
 *                 the vertical retrace, and -1 if late swaps happen
 *                 immediately instead of waiting for the next retrace.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_SetSwapInterval
 */
inline void GL_GetSwapInterval(int* interval)
{
  CheckError(SDL_GL_GetSwapInterval(interval));
}

/**
 * Update a window with OpenGL rendering.
 *
 * This is used with double-buffered OpenGL contexts, which are the default.
 *
 * On macOS, make sure you bind 0 to the draw framebuffer before swapping the
 * window, otherwise nothing will happen. If you aren't using
 * glBindFramebuffer(), this is the default and you won't have to do anything
 * extra.
 *
 * @param window the window to change.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GL_SwapWindow(WindowBase& window)
{
  CheckError(SDL_GL_SwapWindow(window.get()));
}

#pragma region impl

/// @}

inline void WindowBase::SetHitTest(HitTestCB callback)
{
  using Wrapper = KeyValueCallbackWrapper<SDL_Window*, HitTestCB>;
  void* cbHandle = Wrapper::Wrap(get(), std::move(callback));
  SetHitTest(
    [](SDL_Window* win, const SDL_Point* area, void* data) {
      return Wrapper::Call(data, WindowRef{win}, Point(*area));
    },
    cbHandle);
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_VIDEO_H_ */
