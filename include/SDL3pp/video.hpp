#ifndef SDL3PP_VIDEO_HPP_
#define SDL3PP_VIDEO_HPP_

#include <memory>
#include <optional>
#include <vector>
#include <SDL3/SDL_video.h>
#include "freeWrapper.hpp"
#include "objectWrapper.hpp"
#include "stringWrapper.hpp"

namespace SDL {

using DisplayID = SDL_DisplayID;

using WindowID = SDL_WindowID;

using SystemTheme = SDL_SystemTheme;

using DisplayMode = SDL_DisplayMode;

using DisplayOrientation = SDL_DisplayOrientation;

using WindowUnique = ObjectUnique<SDL_Window>;

using Window = ObjectUnique<SDL_Window>;

using WindowWrapper = ObjectWrapper<SDL_Window>;

using WindowConstWrapper = ObjectWrapper<const SDL_Window>;

using WindowFlags = SDL_WindowFlags;

using FlashOperation = SDL_FlashOperation;

/**
 * @brief Get the number of video drivers compiled into SDL.
 *
 * @returns the number of built in video drivers.
 *
 * @sa GetVideoDriver
 */
inline int GetNumVideoDrivers() { return SDL_GetNumVideoDrivers(); }

/**
 * @brief Get the name of a built in video driver.
 *
 * @param index the index of a video driver.
 *
 * @returns the name of the video driver with the given **index**.
 */
inline const char* GetVideoDriver(int index)
{
  return SDL_GetVideoDriver(index);
}

/**
 * @brief Get the name of the currently initialized video driver.
 *
 * @returns the name of the current video driver or NULL if no driver has been
 *          initialized.
 *
 * @sa GetNumVideoDrivers
 * @sa GetVideoDriver
 */
inline const char* GetCurrentVideoDriver()
{
  return SDL_GetCurrentVideoDriver();
}

/**
 * @brief Get the current system theme.
 *
 * @return the current system theme, light, dark, or unknown.
 */
inline SystemTheme GetSystemTheme() { return SDL_GetSystemTheme(); }

/**
 * @brief Get a list of currently connected displays.
 *
 * @param count a pointer filled in with the number of displays returned, may
 *              be NULL.
 * @returns a 0 terminated array of display instance IDs or NULL on failure;
 *          call SDL_GetError() for more information.
 *
 * This automatically calls SDL_free after result is out of scope.
 *
 * @sa Display
 * @sa Display.GetAll()
 */
inline FreeWrapper<DisplayID[]> GetDisplays(int* count = nullptr)
{
  return wrapArray(SDL_GetDisplays(count));
}

/**
 * @brief Return the primary display.
 *
 * @returns the instance ID of the primary display on success or 0 on failure;
 *          call SDL_GetError() for more information.
 */
inline DisplayID GetPrimaryDisplay() { return SDL_GetPrimaryDisplay(); }

/**
 * @brief Represents a handle for a display
 */
struct Display
{
  DisplayID displayID;

  // TODO: GetDisplayProperties()

  /**
   * @brief Get the name of a display in UTF-8 encoding.
   *
   * @returns the name of a display or NULL on failure; call SDL_GetError() for
   *          more information.
   */
  const char* GetName() const { return SDL_GetDisplayName(displayID); }

  /**
   * @brief Get the desktop area represented by a display.
   *
   * The primary display is often located at (0,0), but may be placed at a
   * different location depending on monitor layout.
   *
   * @returns he SDL_Rect structure filled in with the display bounds on success
   * or std::nullopt on failure; call SDL_GetError() for more information.
   */
  std::optional<SDL_Rect> GetBounds() const
  {
    SDL_Rect bounds;
    if (SDL_GetDisplayBounds(displayID, &bounds)) return bounds;
    return {};
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
   * @returns he SDL_Rect structure filled in with the display bounds on success
   * or std::nullopt on failure; call SDL_GetError() for more information.
   */
  std::optional<SDL_Rect> GetUsableBounds() const
  {
    SDL_Rect bounds;
    if (SDL_GetDisplayUsableBounds(displayID, &bounds)) return bounds;
    return {};
  }

  /**
   * @brief Get the orientation of a display when it is unrotated.
   * @returns the SDL_DisplayOrientation enum value of the display, or
   *          `SDL_ORIENTATION_UNKNOWN` if it isn't available.
   */
  DisplayOrientation GetNaturalOrientation() const
  {
    return SDL_GetNaturalDisplayOrientation(displayID);
  }

  /**
   * @brief  Get the orientation of a display.
   * @returns the SDL_DisplayOrientation enum value of the display, or
   *          `SDL_ORIENTATION_UNKNOWN` if it isn't available.
   */
  DisplayOrientation GetCurrentOrientation() const
  {
    return SDL_GetCurrentDisplayOrientation(displayID);
  }

  /**
   * @brief Get the content scale of a display.
   * @returns the content scale of the display, or 0.0f on failure; call
   *          SDL_GetError() for more information.
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
   *          failure; call SDL_GetError() for more information. This is a
   *          single allocation that should be freed with SDL_free() when it is
   *          no longer needed.
   */
  FreeWrapper<DisplayMode*[]> GetFullscreenModes(int* count = nullptr) const
  {
    return wrapArray(SDL_GetFullscreenDisplayModes(displayID, count));
  }

  /**
   * @brief Get the closest match to the requested display mode.
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
   * @param refreshRate the refresh rate of the desired display mode, or 0.0f
   *                     for the desktop refresh rate.
   * @param includeHighDensityModes boolean to include high density modes in
   *                                   the search.
   * @returns The display mode on success, nullopt on failure; call
   *          SDL_GetError() for more information.
   */
  std::optional<DisplayMode> GetClosestFullscreenMode(
    int w,
    int h,
    float refreshRate,
    bool includeHighDensityModes) const
  {
    SDL_DisplayMode closest;
    if (SDL_GetClosestFullscreenDisplayMode(
          displayID, w, h, refreshRate, includeHighDensityModes, &closest)) {
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
   *          SDL_GetError() for more information.
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
   *          SDL_GetError() for more information.
   */
  const DisplayMode* GetCurrentMode() const
  {
    return SDL_GetCurrentDisplayMode(displayID);
  }

  constexpr operator bool() const { return displayID != 0; }

  /**
   * @brief Get a list of currently connected displays.
   * @returns a 0 terminated array of display instance IDs or NULL on failure;
   *          call SDL_GetError() for more information.
   *
   * This automatically calls SDL_free after result is out of scope.
   *
   * This might be violating C++ aliasing rules, so I would have to remove it in
   * the future
   *
   * @sa GetDisplays()
   */
  static FreeWrapper<Display[]> GetAll(int* count = nullptr)
  {
    return wrapArray(reinterpret_cast<Display*>(SDL_GetDisplays(count)));
  }

  /**
   * @brief Get the display containing a point.
   * @param point the point to query.
   * @returns the instance ID of the display containing the point or 0 on
   *          failure; call SDL_GetError() for more information.
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
   *          SDL_GetError() for more information.
   */
  static Display GetForRect(const SDL_Rect& rect)
  {
    return {SDL_GetDisplayForRect(&rect)};
  }

  /**
   * @brief Get the display associated with a window.
   * @param window the window to query.
   * @returns the instance ID of the display containing the center of the window
   *          on success or 0 on failure; call SDL_GetError() for more
   *          information.
   */
  static Display GetForWindow(WindowWrapper window);
};

/**
 * @brief Represents a handle to a const window
 */
template<class T>
struct ObjectBase<T, const SDL_Window>
{
  /**
   * @brief Get the display associated with a window.
   *
   * @returns the instance ID of the display containing the center of the window
   *          on success or 0 on failure; call SDL_GetError() for more
   *          information.
   */
  Display GetDisplay() const { return Display::GetForWindow(Get<T>(this)); }

  /**
   * @brief Get the pixel density of a window.
   *
   * This is a ratio of pixel size to window size. For example, if the window is
   * 1920x1080 and it has a high density back buffer of 3840x2160 pixels, it
   * would have a pixel density of 2.0.
   *
   * @returns the pixel density or 0.0f on failure; call SDL_GetError() for more
   *          information.
   *
   * @sa GetDisplayScale()
   */
  float GetPixelDensity() const
  {
    return SDL_GetWindowPixelDensity(Get<T>(this));
  }

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
   * @returns the display scale, or 0.0f on failure; call SDL_GetError() for
   *          more information.
   */
  float GetDisplayScale() const
  {
    return SDL_GetWindowDisplayScale(Get<T>(this));
  }

  // TODO SDL_SetWindowFullscreenMode()

  /**
   * @brief Get a window from a stored ID.
   *
   * The numeric ID is what SDL_WindowEvent references, and is necessary to map
   * these events to specific SDL_Window objects.
   *
   * @returns the ID of the window on success or 0 on failure; call
   *          SDL_GetError() for more information.
   */
  WindowID GetID() const { return SDL_GetWindowID(Get<T>()); }

  /**
   * @brief Get a window from a stored ID.
   *
   * The numeric ID is what SDL_WindowEvent references, and is necessary to map
   * these events to specific SDL_Window objects.
   *
   * @param id the ID of the window.
   * @returns the window associated with `id` or NULL if it doesn't exist; call
   *          SDL_GetError() for more information.
   */
  static WindowConstWrapper GetFromWindowID(WindowID id)
  {
    return SDL_GetWindowFromID(id);
  }

  // TODO SDL_GetWindowParent()

  /**
   * @brief Return whether the window has a surface associated with it.
   * @returns true if there is a surface associated with the window, or false
   *          otherwise.
   *
   * @sa GetSurface()
   */
  bool HasSurface() const { return SDL_WindowHasSurface(Get<T>()); }

  // TODO GetSurfaceIfExists()

  // TODO SDL_SetWindowSurfaceVSync

  // TODO SDL_SetWindowKeyboardGrab
};

/**
 * @brief Represents a handle to a window
 */
template<class T>
struct ObjectBase<T, SDL_Window> : ObjectBase<T, const SDL_Window>
{
  // TODO SDL_SetWindowFullscreenMode()

  /**
   * @brief Create a window with the specified dimensions and flags.
   * @param title the title of the window, in UTF-8 encoding.
   * @param w the width of the window.
   * @param h the height of the window.
   * @param flags 0, or one or more WindowFlags OR'd together.
   * @returns the window that was created or NULL on failure; call
   *          SDL_GetError() for more information.
   */
  static SDL_Window* Create(StringWrapper title,
                            int w,
                            int h,
                            SDL_WindowFlags flags = 0)
  {
    return SDL_CreateWindow(title.Get(), w, h, flags);
  }

  /**
   * @brief Get a window from a stored ID.
   *
   * The numeric ID is what SDL_WindowEvent references, and is necessary to map
   * these events to specific SDL_Window objects.
   *
   * @param id the ID of the window.
   * @returns the window associated with `id` or NULL if it doesn't exist; call
   *          SDL_GetError() for more information.
   */
  static WindowWrapper GetFromWindowID(WindowID id)
  {
    return SDL_GetWindowFromID(id);
  }

  // TODO SDL_GetWindowParent()

  /**
   * @brief Get the SDL surface associated with the window.
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
   * @return the surface associated with the window, or NULL on failure; call
   *          SDL_GetError() for more information.
   *
   * @sa DestroySurface()
   * @sa HasSurface()
   * @sa UpdateSurface()
   * @sa UpdateSurfaceRects()
   */
  SDL_Surface* GetSurface()
  {
    // Not const, as this might change window state
    // TODO Wrap into SDL::Surface
    return SDL_GetWindowSurface(Get<T>(this));
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @sa GetSurface()
   * @sa UpdateSurfaceRects()
   */
  bool UpdateSurface() { return SDL_UpdateWindowSurface(Get<T>(this)); }

  /**
   * @brief  Copy areas of the window surface to the screen.
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
   * @param numRects the number of rectangles.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool UpdateSurfaceRects(const SDL_Rect* rects, int numRects)
  {
    return SDL_UpdateWindowSurfaceRects(Get<T>(this), rects, numRects);
  }

  /**
   * @brief Destroy the surface associated with the window
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool DestroySurface() { return SDL_DestroyWindowSurface(Get<T>(this)); }

  // TODO SDL_SetWindowKeyboardGrab
protected:
  static void Destroy(SDL_Window* window) { SDL_DestroyWindow(window); }
};

inline Display Display::GetForWindow(WindowWrapper window)
{
  return {SDL_GetDisplayForWindow(window.Get())};
}

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
 * @returns true on success or false on failure; call SDL_GetError() for more
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
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @sa EnableScreenSaver()
 * @sa ScreenSaverEnabled()
 */
inline bool DisableScreenSaver() { return SDL_DisableScreenSaver(); }

} // namespace SDL

#endif /* SDL3PP_VIDEO_HPP_ */
