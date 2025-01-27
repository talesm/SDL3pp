#ifndef SDL3PP_VIDEO_HPP_
#define SDL3PP_VIDEO_HPP_

#include <memory>
#include <optional>
#include <vector>
#include <SDL3/SDL_video.h>
#include "objectWrapper.hpp"

namespace SDL {

using DisplayID = SDL_DisplayID;

using WindowID = SDL_WindowID;

using SystemTheme = SDL_SystemTheme;

using DisplayMode = SDL_DisplayMode;

using DisplayOrientation = SDL_DisplayOrientation;

template<class T>
using WindowBase = ObjectBase<T, SDL_Window>;

using WindowUnique = ObjectUnique<SDL_Window>;

using Window = ObjectUnique<SDL_Window>;

using WindowWrapper = ObjectWrapper<SDL_Window>;

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
 *          call SDL_GetError() for more information. This should be freed
 *          with SDL_free() when it is no longer needed.
 *
 * @sa Display
 */
inline DisplayID* GetDisplays(int* count = nullptr)
{
  return SDL_GetDisplays(count);
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
  DisplayMode** GetFullscreenModes(int* count = nullptr) const
  {
    return SDL_GetFullscreenDisplayModes(displayID, count);
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
 * @brief Represents a handle to a window
 */
template<class T>
struct ObjectBase<T, SDL_Window>
{};

inline Display Display::GetForWindow(WindowWrapper window)
{
  return {SDL_GetDisplayForWindow(window.Get())};
}

} // namespace SDL

#endif /* SDL3PP_VIDEO_HPP_ */
