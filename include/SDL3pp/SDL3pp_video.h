#ifndef SDL3PP_VIDEO_H_
#define SDL3PP_VIDEO_H_

#include <memory>
#include <optional>
#include <vector>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "SDL3pp_error.h"
#include "SDL3pp_freeWrapper.h"
#include "SDL3pp_objectWrapper.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_stringParam.h"
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
template<ObjectBox<SDL_Window*> T>
struct WindowBase;

/**
 * @brief Handle to a non owned window
 */
using WindowRef = WindowBase<ObjectRef<SDL_Window>>;

template<>
struct ObjectDeleter<SDL_Window>
{
  void operator()(WindowRef window) const;
};

/**
 * @brief Handle to an owned window
 */
using Window = WindowBase<ObjectUnique<SDL_Window>>;

// Forward decl
template<ObjectBox<SDL_Renderer*> T>
struct RendererBase;

using RendererRef = RendererBase<ObjectRef<SDL_Renderer>>;

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

/**
 * The display orientation can't be determined
 */
constexpr DisplayOrientation ORIENTATION_UNKNOWN = SDL_ORIENTATION_UNKNOWN;

/**
 * The display is in landscape mode, with the right side up, relative to
 * portrait mode
 */
constexpr DisplayOrientation ORIENTATION_LANDSCAPE = SDL_ORIENTATION_LANDSCAPE;

/**
 * The display is in landscape mode, with the left side up, relative to portrait
 * mode
 */
constexpr DisplayOrientation ORIENTATION_LANDSCAPE_FLIPPED =
  SDL_ORIENTATION_LANDSCAPE_FLIPPED;

/**
 * The display is in portrait mode
 */
constexpr DisplayOrientation ORIENTATION_PORTRAIT = SDL_ORIENTATION_PORTRAIT;

/**
 * The display is in portrait mode, upside down
 */
constexpr DisplayOrientation ORIENTATION_PORTRAIT_FLIPPED =
  SDL_ORIENTATION_PORTRAIT_FLIPPED;

/// @}

/**
 * Internal display mode data.
 *
 * This lives as a field in SDL_DisplayMode, as opaque data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa DisplayMode()
 */
using DisplayModeData = SDL_DisplayModeData;

/**
 * The structure that defines a display mode.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Display.GetFullscreenModes()
 * @sa Display.GetDesktopMode()
 * @sa Display.GetCurrentMode()
 * @sa WindowBase.SetFullscreenMode()
 * @sa WindowBase.GetFullscreenMode()
 */
using DisplayMode = SDL_DisplayMode;

/**
 * The flags on a window.
 *
 * These cover a lot of true/false, or on/off, window state. Some of it is
 * immutable after being set through Window::Window(), some of it can be
 * changed on existing windows by the app, and some of it might be altered by
 * the user or system outside of the app's control.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa WindowBase.GetFlags()
 */
using WindowFlags = SDL_WindowFlags;

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

/**
 * Cancel any window flash state
 */
constexpr FlashOperation FLASH_CANCEL = SDL_FLASH_CANCEL;

/**
 * Flash the window briefly to get attention
 */
constexpr FlashOperation FLASH_BRIEFLY = SDL_FLASH_BRIEFLY;

/**
 * Flash the window until it gets focus
 */
constexpr FlashOperation FLASH_UNTIL_FOCUSED = SDL_FLASH_UNTIL_FOCUSED;

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

/**
 * Region is normal. No special properties.
 */
constexpr HitTestResult HITTEST_NORMAL = SDL_HITTEST_NORMAL;

/**
 * Region can drag entire window.
 */
constexpr HitTestResult HITTEST_DRAGGABLE = SDL_HITTEST_DRAGGABLE;

/**
 * Region is the resizable top-left corner border.
 */
constexpr HitTestResult HITTEST_RESIZE_TOPLEFT = SDL_HITTEST_RESIZE_TOPLEFT;

/**
 * Region is the resizable top border.
 */
constexpr HitTestResult HITTEST_RESIZE_TOP = SDL_HITTEST_RESIZE_TOP;

/**
 * Region is the resizable top-right corner border.
 */
constexpr HitTestResult HITTEST_RESIZE_TOPRIGHT = SDL_HITTEST_RESIZE_TOPRIGHT;

/**
 * Region is the resizable right border.
 */
constexpr HitTestResult HITTEST_RESIZE_RIGHT = SDL_HITTEST_RESIZE_RIGHT;

/**
 * Region is the resizable bottom-right corner border.
 */
constexpr HitTestResult HITTEST_RESIZE_BOTTOMRIGHT =
  SDL_HITTEST_RESIZE_BOTTOMRIGHT;

/**
 * Region is the resizable bottom border.
 */
constexpr HitTestResult HITTEST_RESIZE_BOTTOM = SDL_HITTEST_RESIZE_BOTTOM;

/**
 * Region is the resizable bottom-left corner border.
 */
constexpr HitTestResult HITTEST_RESIZE_BOTTOMLEFT =
  SDL_HITTEST_RESIZE_BOTTOMLEFT;

/**
 * Region is the resizable left border.
 */
constexpr HitTestResult HITTEST_RESIZE_LEFT = SDL_HITTEST_RESIZE_LEFT;

/// @}

/**
 * @name Callbacks for WindowBase::SetHitTest()
 * @{
 */

/**
 * Callback used for hit-testing.
 *
 * @param win the SDL_Window where hit-testing was set on.
 * @param area an SDL_Point which should be hit-tested.
 * @param data what was passed as `callback_data` to WindowBase::SetHitTest().
 * @returns an SDL::HitTestResult value.
 *
 * @sa WindowBase::SetHitTest()
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
 * @sa ListenerCallback
 *
 * @ingroup ListenerCallback
 */
using HitTestFunction =
  std::function<HitTestResult(WindowRef window, const Point& area)>;

/// @}

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
struct Display
{
  SDL_DisplayID displayID;

  constexpr Display(SDL_DisplayID displayID = 0)
    : displayID(displayID)
  {
  }

  constexpr operator bool() const { return displayID != 0; }

  /**
   * Get a list of currently connected displays.
   *
   * @param count a pointer filled in with the number of displays returned, may
   *              be NULL.
   * @returns a 0 terminated array of display instance IDs or NULL on failure;
   *          call GetError() for more information. This should be freed
   *          with SDL_free() when it is no longer needed.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static FreeWrapper<Display[]> GetAll(int* count)
  {
    return wrapArray(reinterpret_cast<Display*>(SDL_GetDisplays(count)));
  }

  /**
   * Return the primary display.
   *
   * @returns the instance ID of the primary display on success or 0 on failure;
   *          call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDisplays()
   */
  static Display GetPrimary() { return {SDL_GetPrimaryDisplay()}; }

  /**
   * Get the properties associated with a display.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN`: true if the display has HDR
   *   headroom above the SDR white point. This is for informational and
   *   diagnostic purposes only, as not all platforms provide this information
   *   at the display level.
   *
   * On KMS/DRM:
   *
   * - `SDL_PROP_DISPLAY_KMSDRM_PANEL_ORIENTATION_NUMBER`: the "panel
   *   orientation" property for the display in degrees of clockwise rotation.
   *   Note that this is provided only as a hint, and the application is
   *   responsible for any coordinate transformations needed to conform to the
   *   requested display orientation.
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return PropertiesRef{SDL_GetDisplayProperties(displayID)};
  }

  /**
   * @brief Get the name of a display in UTF-8 encoding.
   *
   * @returns the name of a display or NULL on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  const char* GetName() const { return SDL_GetDisplayName(displayID); }

  /**
   * @brief Get the desktop area represented by a display.
   *
   * The primary display is often located at (0,0), but may be placed at a
   * different location depending on monitor layout.
   *
   * @returns the Rect structure filled in with the display bounds on success
   * or std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  std::optional<Rect> GetBounds() const
  {
    Rect bounds;
    if (SDL_GetDisplayBounds(displayID, &bounds)) return bounds;
    return std::nullopt;
  }

  /**
   * @brief Get the usable desktop area represented by a display, in screen
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
   * @returns he Rect structure filled in with the display bounds on success
   * or std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  std::optional<Rect> GetUsableBounds() const
  {
    Rect bounds;
    if (SDL_GetDisplayUsableBounds(displayID, &bounds)) return bounds;
    return {};
  }

  /**
   * @brief Get the orientation of a display when it is unrotated.
   *
   * @returns the SDL_DisplayOrientation enum value of the display, or
   *          `SDL_ORIENTATION_UNKNOWN` if it isn't available.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa GetDisplays()
   * @sa Display.GetAll()
   */
  DisplayOrientation GetNaturalOrientation() const
  {
    return SDL_GetNaturalDisplayOrientation(displayID);
  }

  /**
   * @brief  Get the orientation of a display.
   *
   * @returns the SDL_DisplayOrientation enum value of the display, or
   *          `SDL_ORIENTATION_UNKNOWN` if it isn't available.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa Display.GetAll()
   * @sa GetDisplays()
   */
  DisplayOrientation GetCurrentOrientation() const
  {
    return SDL_GetCurrentDisplayOrientation(displayID);
  }

  /**
   * @brief Get the content scale of a display.
   *
   * @returns the content scale of the display, or 0.0f on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa Display.GetAll()
   * @sa WindowBase.GetDisplayScale()
   * @sa GetDisplays()
   */
  float GetContentScale() const
  {
    return SDL_GetDisplayContentScale(displayID);
  }

  /**
   * @brief Get a list of fullscreen display modes available on a display.
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
   * @param count a pointer filled in with the number of display modes returned,
   *              may be NULL.
   * @returns a NULL terminated array of display mode pointers or NULL on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * This automatically calls SDL_free after result is out of scope.
   *
   * @sa Display.GetAll()
   * @sa GetDisplays()
   */
  FreeWrapper<DisplayMode*[]> GetFullscreenModes(int* count = nullptr) const
  {
    return wrapArray(SDL_GetFullscreenDisplayModes(displayID, count));
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
   * on success or std::nullopt on failure; call GetError() for more
   * information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetAll()
   * @sa Display.GetFullscreenModes()
   */
  std::optional<DisplayMode> GetClosestFullscreenMode(
    int w,
    int h,
    float refresh_rate,
    bool include_high_density_modes) const
  {
    if (SDL_DisplayMode closest;
        SDL_GetClosestFullscreenDisplayMode(displayID,
                                            w,
                                            h,
                                            refresh_rate,
                                            include_high_density_modes,
                                            &closest)) {
      return closest;
    }
    return {};
  }

  /**
   * @brief Get information about the desktop's display mode.
   *
   * There's a difference between this function and SDL_GetCurrentDisplayMode()
   * when SDL runs fullscreen and has changed the resolution. In that case this
   * function will return the previous native display mode, and not the current
   * display mode.
   *
   * @returns a pointer to the desktop display mode or NULL on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa Display.GetCurrentMode()
   * @sa Display.GetAll()
   * @sa GetDisplays()
   */
  const DisplayMode* GetDesktopMode() const
  {
    return SDL_GetDesktopDisplayMode(displayID);
  }

  /**
   * @brief Get information about the current display mode.
   *
   * There's a difference between this function and SDL_GetDesktopDisplayMode()
   * when SDL runs fullscreen and has changed the resolution. In that case this
   * function will return the current display mode, and not the previous native
   * display mode.
   *
   * @returns a pointer to the desktop display mode or NULL on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa Display.GetDesktopMode()
   * @sa Display.GetAll()
   * @sa GetDisplays()
   */
  const DisplayMode* GetCurrentMode() const
  {
    return SDL_GetCurrentDisplayMode(displayID);
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
   * @sa GetBounds()
   * @sa GetAll()
   */
  static Display GetForPoint(const SDL_Point& point)
  {
    return {SDL_GetDisplayForPoint(&point)};
  }

  /**
   * @brief Get the display primarily containing a rect.
   * @param rect the rect to query.
   * @returns the instance ID of the display entirely containing the rect or
   *          closest to the center of the rect on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  static Display GetForRect(const SDL_Rect& rect)
  {
    return {SDL_GetDisplayForRect(&rect)};
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
  static Display GetForWindow(WindowRef window);
};

/**
 * This is a unique ID for a window.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using WindowID = SDL_WindowID;

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

/**
 * Unknown system theme
 */
constexpr SystemTheme SYSTEM_THEME_UNKNOWN = SDL_SYSTEM_THEME_UNKNOWN;

/**
 * Light colored system theme
 */
constexpr SystemTheme SYSTEM_THEME_LIGHT = SDL_SYSTEM_THEME_LIGHT;

/**
 * Dark colored system theme
 */
constexpr SystemTheme SYSTEM_THEME_DARK = SDL_SYSTEM_THEME_DARK;

/// @}

/**
 * @brief Represents a handle to a window
 * @ingroup resource
 */
template<ObjectBox<SDL_Window*> T>
struct WindowBase : T
{
  using T::T;

  /**
   * Create a window with the specified dimensions and flags.
   *
   * `flags` may be any of the following OR'd together:
   *
   * - `SDL_WINDOW_FULLSCREEN`: fullscreen window at desktop resolution
   * - `SDL_WINDOW_OPENGL`: window usable with an OpenGL context
   * - `SDL_WINDOW_OCCLUDED`: window partially or completely obscured by another
   *   window
   * - `SDL_WINDOW_HIDDEN`: window is not visible
   * - `SDL_WINDOW_BORDERLESS`: no window decoration
   * - `SDL_WINDOW_RESIZABLE`: window can be resized
   * - `SDL_WINDOW_MINIMIZED`: window is minimized
   * - `SDL_WINDOW_MAXIMIZED`: window is maximized
   * - `SDL_WINDOW_MOUSE_GRABBED`: window has grabbed mouse focus
   * - `SDL_WINDOW_INPUT_FOCUS`: window has input focus
   * - `SDL_WINDOW_MOUSE_FOCUS`: window has mouse focus
   * - `SDL_WINDOW_EXTERNAL`: window not created by SDL
   * - `SDL_WINDOW_MODAL`: window is modal
   * - `SDL_WINDOW_HIGH_PIXEL_DENSITY`: window uses high pixel density back
   *   buffer if possible
   * - `SDL_WINDOW_MOUSE_CAPTURE`: window has mouse captured (unrelated to
   *   MOUSE_GRABBED)
   * - `SDL_WINDOW_ALWAYS_ON_TOP`: window should always be above others
   * - `SDL_WINDOW_UTILITY`: window should be treated as a utility window, not
   *   showing in the task bar and window list
   * - `SDL_WINDOW_TOOLTIP`: window should be treated as a tooltip and does not
   *   get mouse or keyboard focus, requires a parent window
   * - `SDL_WINDOW_POPUP_MENU`: window should be treated as a popup menu,
   *   requires a parent window
   * - `SDL_WINDOW_KEYBOARD_GRABBED`: window has grabbed keyboard input
   * - `SDL_WINDOW_VULKAN`: window usable with a Vulkan instance
   * - `SDL_WINDOW_METAL`: window usable with a Metal instance
   * - `SDL_WINDOW_TRANSPARENT`: window with transparent buffer
   * - `SDL_WINDOW_NOT_FOCUSABLE`: window should not be focusable
   *
   * The SDL_Window is implicitly shown if SDL_WINDOW_HIDDEN is not set.
   *
   * On Apple's macOS, you **must** set the NSHighResolutionCapable Info.plist
   * property to YES, otherwise you will not receive a High-DPI OpenGL canvas.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use SDL_GetWindowSize() to query
   * the client area's size in window coordinates, and
   * SDL_GetWindowSizeInPixels() or SDL_GetRenderOutputSize() to query the
   * drawable size in pixels. Note that the drawable size can vary after the
   * window is created and should be queried again if you get an
   * SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED event.
   *
   * If the window is created with any of the SDL_WINDOW_OPENGL or
   * SDL_WINDOW_VULKAN flags, then the corresponding LoadLibrary function
   * (SDL_GL_LoadLibrary or SDL_Vulkan_LoadLibrary) is called and the
   * corresponding UnloadLibrary function is called by SDL_DestroyWindow().
   *
   * If SDL_WINDOW_VULKAN is specified and there isn't a working Vulkan driver,
   * this will fail, because SDL_Vulkan_LoadLibrary() will fail.
   *
   * If SDL_WINDOW_METAL is specified on an OS that does not support Metal,
   * this will fail.
   *
   * If you intend to use this window with an SDL_Renderer, you should use
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
   * @post the window that was created, convertible to true on success or
   * convertible to false on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa CreateWindowAndRenderer()
   */
  WindowBase(StringParam title, SDL_Point size, WindowFlags flags = 0)
    : T(SDL_CreateWindow(title, size.x, size.y, flags))
  {
  }

  /**
   * Create a child popup window of the specified parent window.
   *
   * The flags parameter **must** contain at least one of the following:
   *
   * - `SDL_WINDOW_TOOLTIP`: The popup window is a tooltip and will not pass any
   *   input events.
   * - `SDL_WINDOW_POPUP_MENU`: The popup window is a popup menu. The topmost
   *   popup menu will implicitly gain the keyboard focus.
   *
   * The following flags are not relevant to popup window creation and will be
   * ignored:
   *
   * - `SDL_WINDOW_MINIMIZED`
   * - `SDL_WINDOW_MAXIMIZED`
   * - `SDL_WINDOW_FULLSCREEN`
   * - `SDL_WINDOW_BORDERLESS`
   *
   * The following flags are incompatible with popup window creation and will
   * cause it to fail:
   *
   * - `SDL_WINDOW_UTILITY`
   * - `SDL_WINDOW_MODAL`
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
   * @param parent the parent of the window, must not be NULL.
   * @param offset the x, y position of the popup window relative to the origin
   *               of the parent.
   * @param size the width and height of the window.
   * @param flags SDL_WINDOW_TOOLTIP or SDL_WINDOW_POPUP_MENU, and zero or more
   *              additional SDL_WindowFlags OR'd together.
   * @post the window that was created, convertible to true on success or
   * convertible to false on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetParent()
   */
  WindowBase(WindowRef parent,
             SDL_Point offset,
             SDL_Point size,
             WindowFlags flags = 0)
    : T(SDL_CreatePopupWindow(parent.get(),
                              offset.x,
                              offset.y,
                              size.x,
                              size.y,
                              flags))
  {
  }

  /**
   * Create a window with the specified properties.
   *
   * These are the supported properties:
   *
   * - `SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN`: true if the window should
   *   be always on top
   * - `SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN`: true if the window has no
   *   window decoration
   * - `SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN`: true if the
   *   window will be used with an externally managed graphics context.
   * - `SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN`: true if the window should
   *   accept keyboard input (defaults true)
   * - `SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN`: true if the window should
   *   start in fullscreen mode at desktop resolution
   * - `SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER`: the height of the window
   * - `SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN`: true if the window should start
   *   hidden
   * - `SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN`: true if the window
   *   uses a high pixel density buffer if possible
   * - `SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN`: true if the window should
   *   start maximized
   * - `SDL_PROP_WINDOW_CREATE_MENU_BOOLEAN`: true if the window is a popup menu
   * - `SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN`: true if the window will be used
   *   with Metal rendering
   * - `SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN`: true if the window should
   *   start minimized
   * - `SDL_PROP_WINDOW_CREATE_MODAL_BOOLEAN`: true if the window is modal to
   *   its parent
   * - `SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN`: true if the window starts
   *   with grabbed mouse focus
   * - `SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN`: true if the window will be used
   *   with OpenGL rendering
   * - `SDL_PROP_WINDOW_CREATE_PARENT_POINTER`: an SDL_Window that will be the
   *   parent of this window, required for windows with the "tooltip", "menu",
   *   and "modal" properties
   * - `SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN`: true if the window should be
   *   resizable
   * - `SDL_PROP_WINDOW_CREATE_TITLE_STRING`: the title of the window, in UTF-8
   *   encoding
   * - `SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN`: true if the window show
   *   transparent in the areas with alpha of 0
   * - `SDL_PROP_WINDOW_CREATE_TOOLTIP_BOOLEAN`: true if the window is a tooltip
   * - `SDL_PROP_WINDOW_CREATE_UTILITY_BOOLEAN`: true if the window is a utility
   *   window, not showing in the task bar and window list
   * - `SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN`: true if the window will be used
   *   with Vulkan rendering
   * - `SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER`: the width of the window
   * - `SDL_PROP_WINDOW_CREATE_X_NUMBER`: the x position of the window, or
   *   `SDL_WINDOWPOS_CENTERED`, defaults to `SDL_WINDOWPOS_UNDEFINED`. This is
   *   relative to the parent for windows with the "tooltip" or "menu" property
   *   set.
   * - `SDL_PROP_WINDOW_CREATE_Y_NUMBER`: the y position of the window, or
   *   `SDL_WINDOWPOS_CENTERED`, defaults to `SDL_WINDOWPOS_UNDEFINED`. This is
   *   relative to the parent for windows with the "tooltip" or "menu" property
   *   set.
   *
   * These are additional supported properties on macOS:
   *
   * - `SDL_PROP_WINDOW_CREATE_COCOA_WINDOW_POINTER`: the
   *   `(__unsafe_unretained)` NSWindow associated with the window, if you want
   *   to wrap an existing window.
   * - `SDL_PROP_WINDOW_CREATE_COCOA_VIEW_POINTER`: the `(__unsafe_unretained)`
   *   NSView associated with the window, defaults to `[window contentView]`
   *
   * These are additional supported properties on Wayland:
   *
   * - `SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN` - true if
   *   the application wants to use the Wayland surface for a custom role and
   *   does not want it attached to an XDG toplevel window. See
   *   [README/wayland](README/wayland) for more information on using custom
   *   surfaces.
   * - `SDL_PROP_WINDOW_CREATE_WAYLAND_CREATE_EGL_WINDOW_BOOLEAN` - true if the
   *   application wants an associated `wl_egl_window` object to be created and
   *   attached to the window, even if the window does not have the OpenGL
   *   property or `SDL_WINDOW_OPENGL` flag set.
   * - `SDL_PROP_WINDOW_CREATE_WAYLAND_WL_SURFACE_POINTER` - the wl_surface
   *   associated with the window, if you want to wrap an existing window. See
   *   [README/wayland](README/wayland) for more information.
   *
   * These are additional supported properties on Windows:
   *
   * - `SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER`: the HWND associated with the
   *   window, if you want to wrap an existing window.
   * - `SDL_PROP_WINDOW_CREATE_WIN32_PIXEL_FORMAT_HWND_POINTER`: optional,
   *   another window to share pixel format with, useful for OpenGL windows
   *
   * These are additional supported properties with X11:
   *
   * - `SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER`: the X11 Window associated
   *   with the window, if you want to wrap an existing window.
   *
   * The window is implicitly shown if the "hidden" property is not set.
   *
   * Windows with the "tooltip" and "menu" properties are popup windows and have
   * the behaviors and guidelines outlined in SDL_CreatePopupWindow().
   *
   * If this window is being created to be used with an SDL_Renderer, you should
   * not add a graphics API specific property
   * (`SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN`, etc), as SDL will handle that
   * internally when it chooses a renderer. However, SDL might need to recreate
   * your window at that point, which may cause the window to appear briefly,
   * and then flicker as it is recreated. The correct approach to this is to
   * create the window with the `SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN` property
   * set to true, then create the renderer, then show the window with
   * SDL_ShowWindow().
   *
   * @param props the properties to use.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef
   * @sa Properties
   */
  WindowBase(PropertiesRef props)
    : T(SDL_CreateWindowWithProperties(props.get()))
  {
  }

  /**
   * @brief Get the display associated with a window.
   *
   * @returns the instance ID of the display containing the center of the window
   *          on success or 0 on failure; call GetError() for more
   *          information.
   */
  Display GetDisplay() const { return Display::GetForWindow(T::get()); }

  /**
   * @brief Get the pixel density of a window.
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
   * @sa WindowBase.GetDisplayScale()
   */
  float GetPixelDensity() const { return SDL_GetWindowPixelDensity(T::get()); }

  /**
   * @brief Get the content display scale relative to a window's pixel size.
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
   */
  float GetDisplayScale() const { return SDL_GetWindowDisplayScale(T::get()); }

  /**
   * Set the display mode to use when a window is visible and fullscreen.
   *
   * This only affects the display mode used when the window is fullscreen. To
   * change the window size when the window is not fullscreen, use
   * SDL_SetWindowSize().
   *
   * If the window is currently in the fullscreen state, this request is
   * asynchronous on some windowing systems and the new mode dimensions may not
   * be applied immediately upon the return of this function. If an immediate
   * change is required, call SDL_SyncWindow() to block until the changes have
   * taken effect.
   *
   * When the new mode takes effect, an SDL_EVENT_WINDOW_RESIZED and/or an
   * SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED event will be emitted with the new
   * mode dimensions.
   *
   * @param mode a pointer to the display mode to use, which can be NULL for
   *             borderless fullscreen desktop mode, or one of the fullscreen
   *             modes returned by SDL_GetFullscreenDisplayModes() to set an
   *             exclusive fullscreen mode.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFullscreenMode()
   * @sa SetFullscreen()
   * @sa Sync()
   */
  bool SetFullscreenMode(OptionalRef<const DisplayMode> mode)
  {
    return SDL_SetWindowFullscreenMode(T::get(), mode);
  }

  /**
   * Query the display mode to use when a window is visible at fullscreen.
   *
   * @returns a pointer to the exclusive fullscreen mode to use or NULL for
   *          borderless fullscreen desktop mode.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetFullscreenMode()
   * @sa SetFullscreen()
   */
  const DisplayMode* GetFullscreenMode() const
  {
    return SDL_GetWindowFullscreenMode(T::get());
  }

  /**
   * Get the raw ICC profile data for the screen the window is currently on.
   *
   * @param size the size of the ICC profile.
   * @returns the raw ICC profile data on success or NULL on failure; call
   *          GetError() for more information. This should be freed with
   *          SDL_free() when it is no longer needed.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  FreeWrapper<void*> GetICCProfile(size_t* size) const
  {
    return {SDL_GetWindowICCProfile(T::get(), size)};
  }

  /**
   * Get the pixel format associated with the window.
   *
   * @returns the pixel format of the window on success or
   *          SDL_PIXELFORMAT_UNKNOWN on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PixelFormat GetPixelFormat() const
  {
    return SDL_GetWindowPixelFormat(T::get());
  }

  /**
   * @brief Get a window from a stored ID.
   *
   * The numeric ID is what SDL_WindowEvent references, and is necessary to map
   * these events to specific SDL_Window objects.
   *
   * @returns the ID of the window on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  WindowID GetID() const { return SDL_GetWindowID(T::get()); }

  /**
   * Get parent of a window.
   *
   * @returns the parent of the window on success or NULL if the window has no
   *          parent.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  WindowRef GetParent() const { return SDL_GetWindowParent(T::get()); }

  /**
   * @brief Get the properties associated with a window.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `SDL_PROP_WINDOW_SHAPE_POINTER`: the surface associated with a shaped
   *   window
   * - `SDL_PROP_WINDOW_HDR_ENABLED_BOOLEAN`: true if the window has HDR
   *   headroom above the SDR white point. This property can change dynamically
   *   when SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `SDL_PROP_WINDOW_SDR_WHITE_LEVEL_FLOAT`: the value of SDR white in the
   *   SDL_COLORSPACE_SRGB_LINEAR colorspace. On Windows this corresponds to
   * the SDR white level in scRGB colorspace, and on Apple platforms this is
   *   always 1.0 for EDR content. This property can change dynamically when
   *   SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `SDL_PROP_WINDOW_HDR_HEADROOM_FLOAT`: the additional high dynamic range
   *   that can be displayed, in terms of the SDR white point. When HDR is not
   *   enabled, this will be 1.0. This property can change dynamically when
   *   SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   *
   * On Android:
   *
   * - `SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER`: the ANativeWindow associated
   *   with the window
   * - `SDL_PROP_WINDOW_ANDROID_SURFACE_POINTER`: the EGLSurface associated
   * with the window
   *
   * On iOS:
   *
   * - `SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER`: the `(__unsafe_unretained)`
   *   UIWindow associated with the window
   * - `SDL_PROP_WINDOW_UIKIT_METAL_VIEW_TAG_NUMBER`: the NSInteger tag
   *   associated with metal views on the window
   * - `SDL_PROP_WINDOW_UIKIT_OPENGL_FRAMEBUFFER_NUMBER`: the OpenGL view's
   *   framebuffer object. It must be bound when rendering to the screen using
   *   OpenGL.
   * - `SDL_PROP_WINDOW_UIKIT_OPENGL_RENDERBUFFER_NUMBER`: the OpenGL view's
   *   renderbuffer object. It must be bound when SDL_GL_SwapWindow is called.
   * - `SDL_PROP_WINDOW_UIKIT_OPENGL_RESOLVE_FRAMEBUFFER_NUMBER`: the OpenGL
   *   view's resolve framebuffer, when MSAA is used.
   *
   * On KMS/DRM:
   *
   * - `SDL_PROP_WINDOW_KMSDRM_DEVICE_INDEX_NUMBER`: the device index
   * associated with the window (e.g. the X in /dev/dri/cardX)
   * - `SDL_PROP_WINDOW_KMSDRM_DRM_FD_NUMBER`: the DRM FD associated with the
   *   window
   * - `SDL_PROP_WINDOW_KMSDRM_GBM_DEVICE_POINTER`: the GBM device associated
   *   with the window
   *
   * On macOS:
   *
   * - `SDL_PROP_WINDOW_COCOA_WINDOW_POINTER`: the `(__unsafe_unretained)`
   *   NSWindow associated with the window
   * - `SDL_PROP_WINDOW_COCOA_METAL_VIEW_TAG_NUMBER`: the NSInteger tag
   *   assocated with metal views on the window
   *
   * On OpenVR:
   *
   * - `SDL_PROP_WINDOW_OPENVR_OVERLAY_ID`: the OpenVR Overlay Handle ID for
   * the associated overlay window.
   *
   * On Vivante:
   *
   * - `SDL_PROP_WINDOW_VIVANTE_DISPLAY_POINTER`: the EGLNativeDisplayType
   *   associated with the window
   * - `SDL_PROP_WINDOW_VIVANTE_WINDOW_POINTER`: the EGLNativeWindowType
   *   associated with the window
   * - `SDL_PROP_WINDOW_VIVANTE_SURFACE_POINTER`: the EGLSurface associated
   * with the window
   *
   * On Windows:
   *
   * - `SDL_PROP_WINDOW_WIN32_HWND_POINTER`: the HWND associated with the
   * window
   * - `SDL_PROP_WINDOW_WIN32_HDC_POINTER`: the HDC associated with the window
   * - `SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER`: the HINSTANCE associated with
   *   the window
   *
   * On Wayland:
   *
   * Note: The `xdg_*` window objects do not internally persist across window
   * show/hide calls. They will be null if the window is hidden and must be
   * queried each time it is shown.
   *
   * - `SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER`: the wl_display associated
   * with the window
   * - `SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER`: the wl_surface associated
   * with the window
   * - `SDL_PROP_WINDOW_WAYLAND_VIEWPORT_POINTER`: the wp_viewport associated
   *   with the window
   * - `SDL_PROP_WINDOW_WAYLAND_EGL_WINDOW_POINTER`: the wl_egl_window
   *   associated with the window
   * - `SDL_PROP_WINDOW_WAYLAND_XDG_SURFACE_POINTER`: the xdg_surface
   * associated with the window
   * - `SDL_PROP_WINDOW_WAYLAND_XDG_TOPLEVEL_POINTER`: the xdg_toplevel role
   *   associated with the window
   * - 'SDL_PROP_WINDOW_WAYLAND_XDG_TOPLEVEL_EXPORT_HANDLE_STRING': the export
   *   handle associated with the window
   * - `SDL_PROP_WINDOW_WAYLAND_XDG_POPUP_POINTER`: the xdg_popup role
   *   associated with the window
   * - `SDL_PROP_WINDOW_WAYLAND_XDG_POSITIONER_POINTER`: the xdg_positioner
   *   associated with the window, in popup mode
   *
   * On X11:
   *
   * - `SDL_PROP_WINDOW_X11_DISPLAY_POINTER`: the X11 Display associated with
   *   the window
   * - `SDL_PROP_WINDOW_X11_SCREEN_NUMBER`: the screen number associated with
   *   the window
   * - `SDL_PROP_WINDOW_X11_WINDOW_NUMBER`: the X11 Window associated with the
   *   window
   *
   * @returns a valid property ID on success or falsy one on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  PropertiesRef GetProperties() const
  {
    return {SDL_GetWindowProperties(T::get())};
  }

  /**
   * Get the window flags.
   *
   * @returns a mask of the SDL_WindowFlags associated with `window`.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Hide()
   * @sa Maximize()
   * @sa Minimize()
   * @sa SetFullscreen()
   * @sa SetMouseGrab()
   * @sa Show()
   */
  WindowFlags GetFlags() const { return SDL_GetWindowFlags(T::get()); }

  /**
   * Set the title of a window.
   *
   * This string is expected to be in UTF-8 encoding.
   *
   * @param title the desired window title in UTF-8 format.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetTitle()
   */
  bool SetTitle(StringParam title)
  {
    return SDL_SetWindowTitle(T::get(), title);
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
   * @sa SetTitle()
   */
  const char* GetTitle() const { return SDL_GetWindowTitle(T::get()); }

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
   * @param icon an SDL_Surface structure containing the icon for the window.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetIcon(SurfaceRef icon) { return SDL_SetWindowIcon(T::get(), icon); }

  /**
   * @brief Request the window's position and size to be set.
   *
   * @param rect the rect containing the new coordinates
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa SetPosition()
   * @sa SetSize()
   */
  bool SetRect(Rect rect)
  {
    return SetPosition(rect.GetTopLeft()) && SetSize(rect.GetSize());
  }

  /**
   * Get the position and client size of a window.
   *
   * This is the current position of the window as last reported by the
   * windowing system.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use SDL_GetWindowSizeInPixels()
   * or SDL_GetRenderOutputSize() to get the real client area size in pixels.
   *
   * @return std::optional<Rect>
   */
  std::optional<Rect> GetRect() const
  {
    if (Rect rect; GetPosition(rect.x, rect.y) && GetSize(rect.w, rect.h))
      return rect;
    return std::nullopt;
  }

  /**
   * Request that the window's position be set.
   *
   * If the window is in an exclusive fullscreen or maximized state, this
   * request has no effect.
   *
   * This can be used to reposition fullscreen-desktop windows onto a different
   * display, however, as exclusive fullscreen windows are locked to a specific
   * display, they can only be repositioned programmatically via
   * SetFullscreenMode().
   *
   * On some windowing systems this request is asynchronous and the new
   * coordinates may not have have been applied immediately upon the return of
   * this function. If an immediate change is required, call Sync() to block
   * until the changes have taken effect.
   *
   * When the window position changes, an SDL_EVENT_WINDOW_MOVED event will be
   * emitted with the window's new coordinates. Note that the new coordinates
   * may not match the exact coordinates requested, as some windowing systems
   * can restrict the position of the window in certain scenarios (e.g.
   * constraining the position so the window is always within desktop bounds).
   * Additionally, as this is just a request, it can be denied by the windowing
   * system.
   *
   * @param p the x, y coordinate of the window, or `SDL_WINDOWPOS_CENTERED` or
   *          `SDL_WINDOWPOS_UNDEFINED`.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetPosition()
   * @sa Sync()
   */
  bool SetPosition(SDL_Point p)
  {
    return SDL_SetWindowPosition(T::get(), p.x, p.y);
  }

  /**
   * Get the position of a window.
   *
   * This is the current position of the window as last reported by the
   * windowing system.
   *
   * @returns the position on success or false on std::nullopt; call
   * GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetPosition()
   * @sa SetPosition(int *, int *)
   */
  std::optional<Point> GetPosition() const
  {
    if (Point p; GetPosition(&p.x, &p.y)) return p;
    return std::nullopt;
  }

  /**
   * Get the position of a window.
   *
   * This is the current position of the window as last reported by the
   * windowing system.
   *
   * If you do not need the value for one of the positions a NULL may be passed
   * in the `x` or `y` parameter.
   *
   * @param x a pointer filled in with the x position of the window, may be
   *          NULL.
   * @param y a pointer filled in with the y position of the window, may be
   *          NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetPosition()
   */
  bool GetPosition(int* x, int* y) const
  {
    return SDL_GetWindowPosition(T::get(), x, y);
  }

  /**
   * Request that the size of a window's client area be set.
   *
   * If the window is in a fullscreen or maximized state, this request has no
   * effect.
   *
   * To change the exclusive fullscreen mode of a window, use
   * SDL_SetWindowFullscreenMode().
   *
   * On some windowing systems, this request is asynchronous and the new window
   * size may not have have been applied immediately upon the return of this
   * function. If an immediate change is required, call SDL_SyncWindow() to
   * block until the changes have taken effect.
   *
   * When the window size changes, an SDL_EVENT_WINDOW_RESIZED event will be
   * emitted with the new window dimensions. Note that the new dimensions may
   * not match the exact size requested, as some windowing systems can restrict
   * the window size in certain scenarios (e.g. constraining the size of the
   * content area to remain within the usable desktop bounds). Additionally, as
   * this is just a request, it can be denied by the windowing system.
   *
   * @param p the width, height of the window, both must be > 0.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   * @sa SetFullscreenMode()
   * @sa Sync()
   */
  bool SetSize(SDL_Point p) { return SDL_SetWindowSize(T::get(), p.x, p.y); }

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
  std::optional<Point> GetSize() const
  {
    if (Point p; GetSize(&p.x, &p.y)) return p;
    return std::nullopt;
  }

  /**
   * Get the size of a window's client area.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use GetSizeInPixels()
   * or Renderer.GetOutputSize() to get the real client area size in pixels.
   *
   * @param w a pointer filled in with the width of the window, may be NULL.
   * @param h a pointer filled in with the height of the window, may be NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetOutputSize()
   * @sa GetSizeInPixels()
   * @sa SetSize()
   */
  bool GetSize(int* w, int* h) const
  {
    return SDL_GetWindowSize(T::get(), w, h);
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
   * @returns the client area that is safe for interactive content on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Rect> GetSafeArea() const
  {
    if (Rect rect; SDL_GetWindowSafeArea(T::get(), &rect)) return rect;
    return std::nullopt;
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
   * this function. If an immediate change is required, call SDL_SyncWindow()
   * to block until the changes have taken effect.
   *
   * When the window size changes, an SDL_EVENT_WINDOW_RESIZED event will be
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetAspectRatio()
   * @sa Sync()
   */
  bool SetAspectRatio(float min_aspect, float max_aspect)
  {
    return SDL_SetWindowAspectRatio(T::get(), min_aspect, max_aspect);
  }

  /**
   * Get the size of a window's client area.
   *
   * @param min_aspect a pointer filled in with the minimum aspect ratio of the
   *                   window, may be NULL.
   * @param max_aspect a pointer filled in with the maximum aspect ratio of the
   *                   window, may be NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetspectRatio()
   */
  bool GetAspectRatio(float* min_aspect, float* max_aspect) const
  {
    return SDL_GetWindowAspectRatio(T::get(), min_aspect, max_aspect);
  }

  /**
   * Get the size of a window's borders (decorations) around the client area.
   *
   * Note: If this function fails (returns false), the size values will be
   * initialized to 0, 0, 0, 0 (if a non-NULL pointer is provided), as if the
   * window in question was borderless.
   *
   * Note: This function may fail on systems where the window has not yet been
   * decorated by the display server (for example, immediately after calling
   * WindowBase::WindowBase()). It is recommended that you wait at least until
   * the window has been presented and composited, so that the window system has
   * a chance to decorate the window and provide the border dimensions to SDL.
   *
   * This function also returns false if getting the information is not
   * supported.
   *
   * @param top pointer to variable for storing the size of the top border;
   * NULL is permitted.
   * @param left pointer to variable for storing the size of the left border;
   *             NULL is permitted.
   * @param bottom pointer to variable for storing the size of the bottom
   *               border; NULL is permitted.
   * @param right pointer to variable for storing the size of the right border;
   *              NULL is permitted.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   */
  bool GetBordersSize(int* top, int* left, int* bottom, int* right) const
  {
    return SDL_GetWindowBordersSize(T::get(), top, left, bottom, right);
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
  std::optional<Point> GetSizeInPixels() const
  {
    if (Point p; GetSizeInPixels(&p.x, &p.y)) return p;
    return std::nullopt;
  }

  /**
   * Get the size of a window's client area, in pixels.
   *
   * @param w a pointer to variable for storing the width in pixels, may be
   *          NULL.
   * @param h a pointer to variable for storing the height in pixels, may be
   *          NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   */
  bool GetSizeInPixels(int* w, int* h) const
  {
    return SDL_GetWindowSizeInPixels(T::get(), w, h);
  }

  /**
   * Set the minimum size of a window's client area.
   *
   * @param p the minimum width and height of the window, or 0 for no limit.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMinimumSize()
   * @sa SetMaximumSize()
   */
  bool SetMinimumSize(SDL_Point p)
  {
    return SDL_SetWindowMinimumSize(T::get(), p.x, p.y);
  }

  /**
   * Get the minimum size of a window's client area.
   *
   * @param w a pointer filled in with the minimum width of the window, may be
   *          NULL.
   * @param h a pointer filled in with the minimum height of the window, may be
   *          NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMaximumSize()
   * @sa SetMinimumSize()
   */
  bool GetMinimumSize(int* w, int* h) const
  {
    return SDL_GetWindowMinimumSize(T::get(), w, h);
  }

  /**
   * Set the maximum size of a window's client area.
   *
   * @param p the maximum width and height of the window, or 0 for no limit.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMaximumSize()
   * @sa SetMinimumSize()
   */
  bool SetMaximumSize(SDL_Point p)
  {
    return SDL_SetWindowMaximumSize(T::get(), p.x, p.y);
  }

  /**
   * Get the maximum size of a window's client area.
   *
   * @param w a pointer filled in with the maximum width of the window, may be
   *          NULL.
   * @param h a pointer filled in with the maximum height of the window, may be
   *          NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMinimumSize()
   * @sa SetMaximumSize()
   */
  bool GetMaximumSize(int* w, int* h) const
  {
    return SDL_GetWindowMaximumSize(T::get(), w, h);
  }

  /**
   * Set the border state of a window.
   *
   * This will add or remove the window's `SDL_WINDOW_BORDERLESS` flag and add
   * or remove the border from the actual window. This is a no-op if the
   * window's border already matches the requested state.
   *
   * You can't change the border state of a fullscreen window.
   *
   * @param bordered false to remove border, true to add border.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFlags()
   */
  bool SetBordered(bool bordered)
  {
    return SDL_SetWindowBordered(T::get(), bordered);
  }

  /**
   * Set the user-resizable state of a window.
   *
   * This will add or remove the window's `SDL_WINDOW_RESIZABLE` flag and
   * allow/disallow user resizing of the window. This is a no-op if the
   * window's resizable state already matches the requested state.
   *
   * You can't change the resizable state of a fullscreen window.
   *
   * @param resizable true to allow resizing, false to disallow.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetWindowFlags()
   */
  bool SetResizable(bool resizable)
  {
    return SDL_SetWindowResizable(T::get(), resizable);
  }

  /**
   * Set the window to always be above the others.
   *
   * This will add or remove the window's `SDL_WINDOW_ALWAYS_ON_TOP` flag. This
   * will bring the window to the front and keep the window above the rest.
   *
   * @param on_top true to set the window always on top, false to disable.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFlags()
   */
  bool SetAlwaysOnTop(bool on_top)
  {
    return SDL_SetWindowAlwaysOnTop(T::get(), on_top);
  }

  /**
   * Show a window.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Hide()
   * @sa Raise()
   */
  bool Show() { return SDL_ShowWindow(T::get()); }

  /**
   * Hide a window.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Show()
   * @sa SDL_WINDOW_HIDDEN
   */
  bool Hide() { return SDL_HideWindow(T::get()); }

  /**
   * Request that a window be raised above other windows and gain the input
   * focus.
   *
   * The result of this request is subject to desktop window manager policy,
   * particularly if raising the requested window would result in stealing
   * focus from another application. If the window is successfully raised and
   * gains input focus, an SDL_EVENT_WINDOW_FOCUS_GAINED event will be emitted,
   * and the window will have the SDL_WINDOW_INPUT_FOCUS flag set.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Raise() { return SDL_RaiseWindow(T::get()); }

  /**
   * Request that the window be made as large as possible.
   *
   * Non-resizable windows can't be maximized. The window must have the
   * SDL_WINDOW_RESIZABLE flag set, or this will have no effect.
   *
   * On some windowing systems this request is asynchronous and the new window
   * state may not have have been applied immediately upon the return of this
   * function. If an immediate change is required, call SDL_SyncWindow() to
   * block until the changes have taken effect.
   *
   * When the window state changes, an SDL_EVENT_WINDOW_MAXIMIZED event will be
   * emitted. Note that, as this is just a request, the windowing system can
   * deny the state change.
   *
   * When maximizing a window, whether the constraints set via
   * SDL_SetWindowMaximumSize() are honored depends on the policy of the window
   * manager. Win32 and macOS enforce the constraints when maximizing, while
   * X11 and Wayland window managers may vary.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Minimize()
   * @sa Restore()
   * @sa Sync()
   */
  bool Maximize() { return SDL_MaximizeWindow(T::get()); }

  /**
   * Request that the window be minimized to an iconic representation.
   *
   * If the window is in a fullscreen state, this request has no direct effect.
   * It may alter the state the window is returned to when leaving fullscreen.
   *
   * On some windowing systems this request is asynchronous and the new window
   * state may not have been applied immediately upon the return of this
   * function. If an immediate change is required, call SDL_SyncWindow() to
   * block until the changes have taken effect.
   *
   * When the window state changes, an SDL_EVENT_WINDOW_MINIMIZED event will be
   * emitted. Note that, as this is just a request, the windowing system can
   * deny the state change.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Maximize()
   * @sa Restore()
   * @sa Sync()
   */
  bool Minimize() { return SDL_MinimizeWindow(T::get()); }

  /**
   * Request that the size and position of a minimized or maximized window be
   * restored.
   *
   * If the window is in a fullscreen state, this request has no direct effect.
   * It may alter the state the window is returned to when leaving fullscreen.
   *
   * On some windowing systems this request is asynchronous and the new window
   * state may not have have been applied immediately upon the return of this
   * function. If an immediate change is required, call SDL_SyncWindow() to
   * block until the changes have taken effect.
   *
   * When the window state changes, an SDL_EVENT_WINDOW_RESTORED event will be
   * emitted. Note that, as this is just a request, the windowing system can
   * deny the state change.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Maximize()
   * @sa Minimize()
   * @sa Sync()
   */
  bool Restore() { return SDL_RestoreWindow(T::get()); }

  /**
   * Request that the window's fullscreen state be changed.
   *
   * By default a window in fullscreen state uses borderless fullscreen desktop
   * mode, but a specific exclusive display mode can be set using
   * SDL_SetWindowFullscreenMode().
   *
   * On some windowing systems this request is asynchronous and the new
   * fullscreen state may not have have been applied immediately upon the
   * return of this function. If an immediate change is required, call
   * SDL_SyncWindow() to block until the changes have taken effect.
   *
   * When the window state changes, an SDL_EVENT_WINDOW_ENTER_FULLSCREEN or
   * SDL_EVENT_WINDOW_LEAVE_FULLSCREEN event will be emitted. Note that, as
   * this is just a request, it can be denied by the windowing system.
   *
   * @param fullscreen true for fullscreen mode, false for windowed mode.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFullscreenMode()
   * @sa SetFullscreenMode()
   * @sa Sync()
   * @sa SDL_WINDOW_FULLSCREEN
   */
  bool SetFullscreen(bool fullscreen)
  {
    return SDL_SetWindowFullscreen(T::get(), fullscreen);
  }

  /**
   * Block until any pending window state is finalized.
   *
   * On asynchronous windowing systems, this acts as a synchronization barrier
   * for pending window state. It will attempt to wait until any pending window
   * state has been applied and is guaranteed to return within finite time.
   * Note that for how long it can potentially block depends on the underlying
   * window system, as window state changes may involve somewhat lengthy
   * animations that must complete before the window is in its final requested
   * state.
   *
   * On windowing systems where changes are immediate, this does nothing.
   *
   * @returns true on success or false if the operation timed out before the
   *          window was in the requested state.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetSize()
   * @sa SetPosition()
   * @sa SetFullscreen()
   * @sa Minimize()
   * @sa Maximize()
   * @sa Restore()
   * @sa SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS
   */
  bool Sync() { return SDL_SyncWindow(T::get()); }

  /**
   * Get the renderer associated with a window.
   *
   * @returns the rendering context on success or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  RendererRef GetRenderer() const;

  /**
   * @brief Return whether the window has a surface associated with it.
   * @returns true if there is a surface associated with the window, or false
   *          otherwise.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa WindowBase.GetSurface()
   */
  bool HasSurface() const { return SDL_WindowHasSurface(T::get()); }

  /**
   * @brief Get the SDL surface associated with the window.
   *
   * A new surface will be created with the optimal format for the window, if
   * necessary. This surface will be freed when the window is destroyed. Do not
   * free this surface.
   *
   * This surface will be invalidated if the window is resized. After resizing
   * a window this function must be called again to return a valid surface.
   *
   * You may not combine this with 3D or the rendering API on this window.
   *
   * This function is affected by `SDL_HINT_FRAMEBUFFER_ACCELERATION`.
   * @return the surface associated with the window, or NULL on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa DestroySurface()
   * @sa HasSurface()
   * @sa UpdateSurface()
   * @sa UpdateSurfaceRects()
   */
  SurfaceRef GetSurface() { return SDL_GetWindowSurface(T::get()); }

  /**
   * Toggle VSync for the window surface.
   *
   * When a window surface is created, vsync defaults to
   * SDL_WINDOW_SURFACE_VSYNC_DISABLED.
   *
   * The `vsync` parameter can be 1 to synchronize present with every vertical
   * refresh, 2 to synchronize present with every second vertical refresh,
   * etc., SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE for late swap tearing (adaptive
   * vsync), or SDL_WINDOW_SURFACE_VSYNC_DISABLED to disable. Not every value
   * is supported by every driver, so you should check the return value to see
   * whether the requested setting is supported.
   *
   * @param vsync the vertical refresh sync interval.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSurfaceVSync()
   */
  bool SetSurfaceVSync(int vsync)
  {
    return SDL_SetWindowSurfaceVSync(T::get(), vsync);
  }

  /**
   * Get VSync for the window surface.
   *
   * @returns the current vertical refresh sync interval on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetSurfaceVSync()
   */
  std::optional<int> GetSurfaceVSync() const
  {
    if (int vsync; SDL_GetWindowSurfaceVSync(T::get(), vsync)) return vsync;
    return std::nullopt;
  }

  // TODO SDL_SetWindowSurfaceVSync

  /**
   * @brief Copy the window surface to the screen.
   *
   * This is the function you use to reflect any changes to the surface on the
   * screen.
   *
   * This function is equivalent to the SDL 1.2 API SDL_Flip().
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa GetSurface()
   * @sa UpdateSurfaceRects()
   */
  bool UpdateSurface() { return SDL_UpdateWindowSurface(T::get()); }

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
   * @param rects an array of SDL_Rect structures representing areas of the
   *              surface to copy, in pixels.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSurface()
   * @sa UpdateSurface()
   */
  bool UpdateSurfaceRects(SpanRef<const SDL_Rect> rects)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_SINT32);
    return SDL_UpdateWindowSurfaceRects(T::get(), rects.data(), rects.size());
  }

  /**
   * Destroy the surface associated with the window.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSurface()
   * @sa HasSurface()
   */
  bool DestroySurface() { return SDL_DestroyWindowSurface(T::get()); }

  /**
   * Set a window's keyboard grab mode.
   *
   * Keyboard grab enables capture of system keyboard shortcuts like Alt+Tab or
   * the Meta/Super key. Note that not all system keyboard shortcuts can be
   * captured by applications (one example is Ctrl+Alt+Del on Windows).
   *
   * This is primarily intended for specialized applications such as VNC
   * clients or VM frontends. Normal games should not use keyboard grab.
   *
   * When keyboard grab is enabled, SDL will continue to handle Alt+Tab when
   * the window is full-screen to ensure the user is not trapped in your
   * application. If you have a custom keyboard shortcut to exit fullscreen
   * mode, you may suppress this behavior with
   * `SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED`.
   *
   * If the caller enables a grab while another window is currently grabbed,
   * the other window loses its grab in favor of the caller's window.
   *
   * @param grabbed this is true to grab keyboard, and false to release.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetKeyboardGrab()
   * @sa SetMouseGrab()
   */
  bool SetKeyboardGrab(bool grabbed)
  {
    return SDL_SetWindowKeyboardGrab(T::get(), grabbed);
  }

  /**
   * Set a window's mouse grab mode.
   *
   * Mouse grab confines the mouse cursor to the window.
   *
   * @param grabbed this is true to grab mouse, and false to release.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMouseRect()
   * @sa SetMouseRect()
   * @sa SetMouseGrab()
   * @sa SetKeyboardGrab()
   */
  bool SetMouseGrab(bool grabbed)
  {
    return SDL_SetWindowMouseGrab(T::get(), grabbed);
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
   * @sa SetKeyboardGrab()
   */
  bool GetKeyboardGrab() const { return SDL_GetWindowKeyboardGrab(T::get()); }

  /**
   * Get a window's mouse grab mode.
   *
   * @returns true if mouse is grabbed, and false otherwise.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMouseRect()
   * @sa SetMouseRect()
   * @sa SetMouseGrab()
   * @sa SetKeyboardGrab()
   */
  bool GetMouseGrab() const { return SDL_GetWindowMouseGrab(T::get()); }

  /**
   * Confines the cursor to the specified area of a window.
   *
   * Note that this does NOT grab the cursor, it only defines the area a cursor
   * is restricted to when the window has mouse focus.
   *
   * @param rect a rectangle area in window-relative coordinates. If NULL the
   *             barrier for the specified window will be destroyed.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMouseRect()
   * @sa GetMouseGrab()
   * @sa SetMouseGrab()
   */
  bool SetMouseRect(const SDL_Rect& rect)
  {
    return SDL_SetWindowMouseRect(T::get(), rect);
  }

  /**
   * Get the mouse confinement rectangle of a window.
   *
   * @returns a pointer to the mouse confinement rectangle of a window, or NULL
   *          if there isn't one.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetMouseRect()
   * @sa GetMouseGrab()
   * @sa SetMouseGrab()
   */
  const Rect* GetMouseRect() const { return SDL_GetWindowMouseRect(T::get()); }

  /**
   * Set the opacity for a window.
   *
   * The parameter `opacity` will be clamped internally between 0.0f
   * (transparent) and 1.0f (opaque).
   *
   * This function also returns false if setting the opacity isn't supported.
   *
   * @param opacity the opacity value (0.0f - transparent, 1.0f - opaque).
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetOpacity()
   */
  bool SetOpacity(float opacity)
  {
    return SDL_SetWindowOpacity(T::get(), opacity);
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
   * @sa SetOpacity()
   */
  float GetOpacity() const { return SDL_GetWindowOpacity(T::get()); }

  /**
   * Set the window as a child of a parent window.
   *
   * If the window is already the child of an existing window, it will be
   * reparented to the new owner. Setting the parent window to NULL unparents
   * the window and removes child window status.
   *
   * If a parent window is hidden or destroyed, the operation will be
   * recursively applied to child windows. Child windows hidden with the parent
   * that did not have their hidden status explicitly set will be restored when
   * the parent is shown.
   *
   * Attempting to set the parent of a window that is currently in the modal
   * state will fail. Use SDL_SetWindowModal() to cancel the modal status
   * before attempting to change the parent.
   *
   * Popup windows cannot change parents and attempts to do so will fail.
   *
   * Setting a parent window that is currently the sibling or descendent of the
   * child window results in undefined behavior.
   *
   * @param parent the new parent window for the child window.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetModal()
   */
  bool SetParent(WindowRef parent)
  {
    return SDL_SetWindowParent(T::get(), parent.get());
  }

  /**
   * Toggle the state of the window as modal.
   *
   * To enable modal status on a window, the window must currently be the child
   * window of a parent, or toggling modal status on will fail.
   *
   * @param modal true to toggle modal status on, false to toggle it off.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetParent()
   * @sa SDL_WINDOW_MODAL
   */
  bool SetModal(bool modal) { return SDL_SetWindowModal(T::get(), modal); }

  /**
   * Set whether the window may have input focus.
   *
   * @param focusable true to allow input focus, false to not allow input
   * focus.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetFocusable(bool focusable)
  {
    return SDL_SetWindowFocusable(T::get(), focusable);
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
   * of the client area.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ShowSystemMenu(SDL_Point p)
  {
    return SDL_ShowWindowSystemMenu(T::get(), p.x, p.y);
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
   * Specifying NULL for a callback disables hit-testing. Hit-testing is
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ListenerCallback
   *
   * @ingroup ListenerCallback
   */
  bool SetHitTest(HitTestFunction callback)
  {
    using Wrapper = KeyValueWrapper<SDL_Window*, HitTestFunction>;
    void* cbHandle = Wrapper::Wrap(T::get(), std::move(callback));
    return SetHitTest(
      [](SDL_Window* win, const SDL_Point* area, void* data) {
        auto& cb = Wrapper::at(data);
        return cb(WindowRef{win}, Point(*area));
      },
      cbHandle);
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
   * Specifying NULL for a callback disables hit-testing. Hit-testing is
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetHitTest(HitTest callback, void* callback_data)
  {
    return SDL_SetWindowHitTest(T::get(), callback, callback_data);
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
   * The shape is copied inside this function, so you can free it afterwards.
   * If your shape surface changes, you should call SDL_SetWindowShape() again
   * to update the window. This is an expensive operation, so should be done
   * sparingly.
   *
   * The window must have been created with the SDL_WINDOW_TRANSPARENT flag.
   *
   * @param shape the surface representing the shape of the window, or NULL to
   *              remove any current shape.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetShape(SurfaceRef shape)
  {
    return SDL_SetWindowShape(T::get(), shape);
  }

  /**
   * Request a window to demand attention from the user.
   *
   * @param operation the operation to perform.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Flash(FlashOperation operation)
  {
    return SDL_FlashWindow(T::get(), operation);
  }

  /**
   * Destroy a window.
   *
   * Any child windows owned by the window will be recursively destroyed as
   * well.
   *
   * Note that on some platforms, the visible window may not actually be removed
   * from the screen until the SDL event loop is pumped again, even though the
   * SDL_Window is no longer valid after this call.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Destroy()
  {
    auto window = T::release();
    KeyValueWrapper<SDL_Window*, HitTestFunction>::erase(window);
    return SDL_DestroyWindow(window);
  }
};

/**
 * An opaque handle to an OpenGL context.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GL_CreateContext()
 */
using GLContext = SDL_GLContext;

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
 * with `EGL_NONE`. If this function returns NULL, the SDL_CreateWindow
 * process will fail gracefully.
 *
 * The returned pointer should be allocated with SDL_malloc() and will be
 * passed to SDL_free().
 *
 * The arrays returned by each callback will be appended to the existing
 * attribute arrays defined by SDL.
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 * @returns a newly-allocated array of attributes, terminated with `EGL_NONE`.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EGL_SetAttributeCallbacks()
 */
using EGLAttribArrayCallback = SDL_EGLAttribArrayCallback;

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
 * with `EGL_NONE`. If this function returns NULL, the SDL_CreateWindow
 * process will fail gracefully.
 *
 * The returned pointer should be allocated with SDL_malloc() and will be
 * passed to SDL_free().
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
 * @sa EGL_SetAttributeCallbacks()
 */
using EGLIntArrayCallback = SDL_EGLIntArrayCallback;

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
 * SDL_GL_SetAttribute() and SDL_GL_GetAttribute().
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
 * to 3.
 */
constexpr GLAttr GL_RED_SIZE = SDL_GL_RED_SIZE;

/**
 * the minimum number of bits for the green channel of the color buffer;
 * defaults to 3.
 */
constexpr GLAttr GL_GREEN_SIZE = SDL_GL_GREEN_SIZE;

/**
 * the minimum number of bits for the blue channel of the color buffer; defaults
 * to 2.
 */
constexpr GLAttr GL_BLUE_SIZE = SDL_GL_BLUE_SIZE;

/**
 * the minimum number of bits for the alpha channel of the color buffer;
 * defaults to 0.
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

/**
 * whether the output is stereo 3D; defaults to off.
 */
constexpr GLAttr GL_STEREO = SDL_GL_STEREO;

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

/**
 * not used (deprecated).
 */
constexpr GLAttr GL_RETAINED_BACKING = SDL_GL_RETAINED_BACKING;

/**
 * OpenGL context major version.
 */
constexpr GLAttr GL_CONTEXT_MAJOR_VERSION = SDL_GL_CONTEXT_MAJOR_VERSION;

/**
 * OpenGL context minor version.
 */
constexpr GLAttr GL_CONTEXT_MINOR_VERSION = SDL_GL_CONTEXT_MINOR_VERSION;

/**
 * some combination of 0 or more of elements of the SDL_GLContextFlag
 * enumeration; defaults to 0.
 */
constexpr GLAttr GL_CONTEXT_FLAGS = SDL_GL_CONTEXT_FLAGS;

/**
 * type of GL context (Core, Compatibility, ES). See SDL_GLProfile; default
 * value depends on platform.
 */
constexpr GLAttr GL_CONTEXT_PROFILE_MASK = SDL_GL_CONTEXT_PROFILE_MASK;

/**
 * OpenGL context sharing; defaults to 0.
 */
constexpr GLAttr GL_SHARE_WITH_CURRENT_CONTEXT =
  SDL_GL_SHARE_WITH_CURRENT_CONTEXT;

/**
 * requests sRGB capable visual; defaults to 0.
 */
constexpr GLAttr GL_FRAMEBUFFER_SRGB_CAPABLE = SDL_GL_FRAMEBUFFER_SRGB_CAPABLE;

/**
 * sets context the release behavior. See SDL_GLContextReleaseFlag; defaults to
 * FLUSH.
 */
constexpr GLAttr GL_CONTEXT_RELEASE_BEHAVIOR = SDL_GL_CONTEXT_RELEASE_BEHAVIOR;

/**
 * set context reset notification. See SDL_GLContextResetNotification; defaults
 * to NO_NOTIFICATION.
 */
constexpr GLAttr GL_CONTEXT_RESET_NOTIFICATION =
  SDL_GL_CONTEXT_RESET_NOTIFICATION;

constexpr GLAttr GL_CONTEXT_NO_ERROR = SDL_GL_CONTEXT_NO_ERROR;

constexpr GLAttr GL_FLOATBUFFERS = SDL_GL_FLOATBUFFERS;

constexpr GLAttr GL_EGL_PLATFORM = SDL_GL_EGL_PLATFORM;

/// @}

/**
 * Possible values to be set for the SDL_GL_CONTEXT_PROFILE_MASK attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLProfile = SDL_GLProfile;

/**
 * Possible flags to be set for the SDL_GL_CONTEXT_FLAGS attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLContextFlag = SDL_GLContextFlag;

/**
 * Possible values to be set for the SDL_GL_CONTEXT_RELEASE_BEHAVIOR
 * attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLContextReleaseFlag = SDL_GLContextReleaseFlag;

/**
 * Possible values to be set SDL_GL_CONTEXT_RESET_NOTIFICATION attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLContextResetNotification = SDL_GLContextResetNotification;

/**
 * Get the number of video drivers compiled into SDL.
 *
 * @returns the number of built in video drivers.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetVideoDriver()
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
 * @sa GetNumVideoDrivers()
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
 * @returns the name of the current video driver or NULL if no driver has been
 *          initialized.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumVideoDrivers()
 * @sa GetVideoDriver()
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

/**
 * Get a list of valid windows.
 *
 * @param count a pointer filled in with the number of windows returned, may
 *              be NULL.
 * @returns a NULL terminated array of SDL_Window pointers or NULL on failure;
 *          call GetError() for more information. This is a single
 *          allocation that should be freed with SDL_free() when it is no
 *          longer needed.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline FreeWrapper<WindowRef[]> GetWindows(int* count)
{
  return wrapArray(reinterpret_cast<WindowRef*>(SDL_GetWindows(count)));
}

/**
 * Get a window from a stored ID.
 *
 * The numeric ID is what SDL_WindowEvent references, and is necessary to map
 * these events to specific SDL_Window objects.
 *
 * @param id the ID of the window.
 * @returns the window associated with `id` or NULL if it doesn't exist; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetID()
 */
inline WindowRef GetWindowFromID(WindowID id)
{
  return SDL_GetWindowFromID(id);
}

/**
 * Get the window that currently has an input grab enabled.
 *
 * @returns the window if input is grabbed or NULL otherwise.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetMouseGrab()
 * @sa SetKeyboardGrab()
 */
inline WindowRef GetGrabbedWindow() { return SDL_GetGrabbedWindow(); }

/**
 * @brief  Check whether the screensaver is currently enabled.
 *
 * The screensaver is disabled by default.
 *
 * The default can also be changed using `SDL_HINT_VIDEO_ALLOW_SCREENSAVER`.
 *
 * @returns true if the screensaver is enabled, false if it is disabled.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @sa DisableScreenSaver()
 * @sa EnableScreenSaver()
 */
inline bool ScreenSaverEnabled() { return SDL_ScreenSaverEnabled(); }

/**
 * @brief Allow the screen to be blanked by a screen saver.
 *
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @sa DisableScreenSaver()
 * @sa ScreenSaverEnabled()
 */
inline bool EnableScreenSaver() { return SDL_EnableScreenSaver(); }

/**
 * @brief Prevent the screen from being blanked by a screen saver.
 *
 * If you disable the screensaver, it is automatically re-enabled when SDL
 * quits.
 *
 * The screensaver is disabled by default, but this may by changed by
 * SDL_HINT_VIDEO_ALLOW_SCREENSAVER.
 *
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @sa EnableScreenSaver()
 * @sa ScreenSaverEnabled()
 */
inline bool DisableScreenSaver() { return SDL_DisableScreenSaver(); }

/**
 * Dynamically load an OpenGL library.
 *
 * This should be done after initializing the video driver, but before
 * creating any OpenGL windows. If no OpenGL library is loaded, the default
 * library will be loaded upon creation of the first OpenGL window.
 *
 * If you do this, you need to retrieve all of the GL functions used in your
 * program from the dynamic library using SDL_GL_GetProcAddress().
 *
 * @param path the platform dependent OpenGL library name, or NULL to open the
 *             default OpenGL library.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetProcAddress()
 * @sa GL_UnloadLibrary()
 */
inline bool GL_LoadLibrary(StringParam path)
{
  return SDL_GL_LoadLibrary(path);
}

/**
 * Get an OpenGL function by name.
 *
 * If the GL library is loaded at runtime with SDL_GL_LoadLibrary(), then all
 * GL functions must be retrieved this way. Usually this is used to retrieve
 * function pointers to OpenGL extensions.
 *
 * There are some quirks to looking up OpenGL functions that require some
 * extra care from the application. If you code carefully, you can handle
 * these quirks without any platform-specific code, though:
 *
 * - On Windows, function pointers are specific to the current GL context;
 *   this means you need to have created a GL context and made it current
 *   before calling SDL_GL_GetProcAddress(). If you recreate your context or
 *   create a second context, you should assume that any existing function
 *   pointers aren't valid to use with it. This is (currently) a
 *   Windows-specific limitation, and in practice lots of drivers don't suffer
 *   this limitation, but it is still the way the wgl API is documented to
 *   work and you should expect crashes if you don't respect it. Store a copy
 *   of the function pointers that comes and goes with context lifespan.
 * - On X11, function pointers returned by this function are valid for any
 *   context, and can even be looked up before a context is created at all.
 *   This means that, for at least some common OpenGL implementations, if you
 *   look up a function that doesn't exist, you'll get a non-NULL result that
 *   is _NOT_ safe to call. You must always make sure the function is actually
 *   available for a given GL context before calling it, by checking for the
 *   existence of the appropriate extension with SDL_GL_ExtensionSupported(),
 *   or verifying that the version of OpenGL you're using offers the function
 *   as core functionality.
 * - Some OpenGL drivers, on all platforms, *will* return NULL if a function
 *   isn't supported, but you can't count on this behavior. Check for
 *   extensions you use, and if you get a NULL anyway, act as if that
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
 * @sa GL_ExtensionSupported()
 * @sa GL_LoadLibrary()
 * @sa GL_UnloadLibrary()
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
 * @sa EGL_GetCurrentDisplay()
 */
inline FunctionPointer EGL_GetProcAddress(StringParam proc)
{
  return SDL_EGL_GetProcAddress(proc);
}

/**
 * Unload the OpenGL library previously loaded by SDL_GL_LoadLibrary().
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_LoadLibrary()
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
 * @sa GL_GetAttribute()
 * @sa GL_SetAttribute()
 */
inline void GL_ResetAttributes() { SDL_GL_ResetAttributes(); }

/**
 * Set an OpenGL window attribute before window creation.
 *
 * This function sets the OpenGL attribute `attr` to `value`. The requested
 * attributes should be set before creating an OpenGL window. You should use
 * SDL_GL_GetAttribute() to check the values after creating the OpenGL
 * context, since the values obtained can differ from the requested ones.
 *
 * @param attr an SDL_GLAttr enum value specifying the OpenGL attribute to
 *             set.
 * @param value the desired value for the attribute.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetAttribute()
 * @sa GL_ResetAttributes()
 */
inline bool GL_SetAttribute(GLAttr attr, int value)
{
  return SDL_GL_SetAttribute(attr, value);
}

/**
 * Get the actual value for an attribute from the current context.
 *
 * @param attr an SDL_GLAttr enum value specifying the OpenGL attribute to
 *             get.
 * @param value a pointer filled in with the current value of `attr`.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_ResetAttributes()
 * @sa GL_SetAttribute()
 */
inline bool GL_GetAttribute(GLAttr attr, int* value)
{
  return SDL_GL_GetAttribute(attr, value);
}

/**
 * Create an OpenGL context for an OpenGL window, and make it current.
 *
 * Windows users new to OpenGL should note that, for historical reasons, GL
 * functions added after OpenGL version 1.1 are not available by default.
 * Those functions must be loaded at run-time, either with an OpenGL
 * extension-handling library or with SDL_GL_GetProcAddress() and its related
 * functions.
 *
 * SDL_GLContext is opaque to the application.
 *
 * @param window the window to associate with the context.
 * @returns the OpenGL context associated with `window` or NULL on failure;
 *          call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_DestroyContext()
 * @sa GL_MakeCurrent()
 */
inline GLContext GL_CreateContext(WindowRef window)
{
  return SDL_GL_CreateContext(window.get());
}

/**
 * Set up an OpenGL context for rendering into an OpenGL window.
 *
 * The context must have been created with a compatible window.
 *
 * @param window the window to associate with the context.
 * @param context the OpenGL context to associate with the window.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_CreateContext()
 */
inline bool GL_MakeCurrent(WindowRef window, GLContext context)
{
  return SDL_GL_MakeCurrent(window.get(), context);
}

/**
 * Get the currently active OpenGL window.
 *
 * @returns the currently active OpenGL window on success or NULL on failure;
 *          call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline WindowRef GL_GetCurrentWindow() { return SDL_GL_GetCurrentWindow(); }

/**
 * Get the currently active OpenGL context.
 *
 * @returns the currently active OpenGL context or NULL on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_MakeCurrent()
 */
inline GLContext GL_GetCurrentContext() { return SDL_GL_GetCurrentContext(); }

/**
 * Get the currently active EGL display.
 *
 * @returns the currently active EGL display or NULL on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline EGLDisplay EGL_GetCurrentDisplay()
{
  return SDL_EGL_GetCurrentDisplay();
}

/**
 * Get the currently active EGL config.
 *
 * @returns the currently active EGL config or NULL on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline EGLConfig EGL_GetCurrentConfig() { return SDL_EGL_GetCurrentConfig(); }

/**
 * Get the EGL surface associated with the window.
 *
 * @param window the window to query.
 * @returns the EGLSurface pointer associated with the window, or NULL on
 *          failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline EGLSurface EGL_GetWindowSurface(WindowRef window)
{
  return SDL_EGL_GetWindowSurface(window.get());
}

/**
 * Sets the callbacks for defining custom EGLAttrib arrays for EGL
 * initialization.
 *
 * Callbacks that aren't needed can be set to NULL.
 *
 * NOTE: These callback pointers will be reset after SDL_GL_ResetAttributes.
 *
 * @param platformAttribCallback callback for attributes to pass to
 *                               eglGetPlatformDisplay. May be NULL.
 * @param surfaceAttribCallback callback for attributes to pass to
 *                              eglCreateSurface. May be NULL.
 * @param contextAttribCallback callback for attributes to pass to
 *                              eglCreateContext. May be NULL.
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
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetSwapInterval()
 */
inline bool GL_SetSwapInterval(int interval)
{
  return SDL_GL_SetSwapInterval(interval);
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
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_SetSwapInterval()
 */
inline bool GL_GetSwapInterval(int* interval)
{
  return SDL_GL_GetSwapInterval(interval);
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
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GL_SwapWindow(WindowRef window)
{
  return SDL_GL_SwapWindow(window.get());
}

/**
 * Delete an OpenGL context.
 *
 * @param context the OpenGL context to be deleted.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_CreateContext()
 */
inline bool GL_DestroyContext(GLContext context)
{
  return SDL_GL_DestroyContext(context);
}

#pragma region impl

/// @}

inline void ObjectDeleter<SDL_Window>::operator()(WindowRef window) const
{
  window.Destroy();
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_VIDEO_H_ */
