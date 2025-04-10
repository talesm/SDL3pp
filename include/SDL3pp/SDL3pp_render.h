#ifndef SDL3PP_RENDER_H_
#define SDL3PP_RENDER_H_

#include <SDL3/SDL_render.h>
#include "SDL3pp_blendmode.h"
#include "SDL3pp_error.h"
#include "SDL3pp_events.h"
#include "SDL3pp_objectWrapper.h"
#include "SDL3pp_optionalRef.h"
#include "SDL3pp_pixels.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_stringParam.h"
#include "SDL3pp_surface.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategoryRender 2D Accelerated Rendering
 *
 * Header file for SDL 2D rendering functions.
 *
 * This API supports the following features:
 *
 * - single pixel points
 * - single pixel lines
 * - filled rectangles
 * - texture images
 * - 2D polygons
 *
 * The primitives may be drawn in opaque, blended, or additive modes.
 *
 * The texture images may be drawn in opaque, blended, or additive modes. They
 * can have an additional color tint or alpha modulation applied to them, and
 * may also be stretched with linear interpolation.
 *
 * This API is designed to accelerate simple 2D operations. You may want more
 * functionality such as polygons and particle effects and in that case you
 * should use SDL's OpenGL/Direct3D support, the SDL3 GPU API, or one of the
 * many good 3D engines.
 *
 * These functions must be called from the main thread. See this bug for
 * details: https://github.com/libsdl-org/SDL/issues/986
 *
 * @{
 */

// Forward decl
struct TextureLock;

/**
 * Handle to an owned renderer
 *
 * @cat resource
 *
 * @sa RendererBase
 * @sa RendererRef
 */
using Renderer = RendererBase<ObjectUnique<SDL_Renderer>>;

// Forward decl
template<ObjectBox<SDL_Texture*> T>
struct TextureBase;

/**
 * Handle to a non owned texture
 *
 * @cat resource
 *
 * @sa TextureBase
 * @sa Texture
 */
using TextureRef = TextureBase<ObjectRef<SDL_Texture>>;

/**
 * Handle to an owned texture
 *
 * @cat resource
 *
 * @sa TextureBase
 * @sa TextureRef
 */
using Texture = TextureBase<ObjectUnique<SDL_Texture>>;

#ifdef SDL3PP_DOC

/**
 * The name of the software renderer.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_SOFTWARE_RENDERER "software"

#endif // SDL3PP_DOC

/**
 * Vertex structure.
 *
 */
using Vertex = SDL_Vertex;

/**
 * The access pattern allowed for a texture.
 *
 */
using TextureAccess = SDL_TextureAccess;

/**
 * Changes rarely, not lockable
 */
constexpr TextureAccess TEXTUREACCESS_STATIC = SDL_TEXTUREACCESS_STATIC;

/**
 * Changes frequently, lockable
 */
constexpr TextureAccess TEXTUREACCESS_STREAMING = SDL_TEXTUREACCESS_STREAMING;

/**
 * Texture can be used as a render target
 */
constexpr TextureAccess TEXTUREACCESS_TARGET = SDL_TEXTUREACCESS_TARGET;

/**
 * How the logical size is mapped to the output.
 *
 */
using RendererLogicalPresentation = SDL_RendererLogicalPresentation;

/**
 * There is no logical size in effect
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_DISABLED =
  SDL_LOGICAL_PRESENTATION_DISABLED;

/**
 * The rendered content is stretched to the output resolution
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_STRETCH =
  SDL_LOGICAL_PRESENTATION_STRETCH;

/**
 * The rendered content is fit to the largest dimension and the other dimension
 * is letterboxed with black bars
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_LETTERBOX =
  SDL_LOGICAL_PRESENTATION_LETTERBOX;

/**
 * The rendered content is fit to the smallest dimension and the other dimension
 * extends beyond the output bounds
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_OVERSCAN =
  SDL_LOGICAL_PRESENTATION_OVERSCAN;

/**
 * The rendered content is scaled up by integer multiples to fit the output
 * resolution
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_INTEGER_SCALE =
  SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;

/**
 * A structure representing rendering state
 *
 * @cat resource
 *
 * @sa resource
 * @sa Renderer
 * @sa RendererRef
 */
template<ObjectBox<SDL_Renderer*> T>
struct RendererBase : T
{
  using T::T;

  /**
   * Create a 2D rendering context for a window.
   *
   * By default the rendering size matches the window size in pixels, but you
   * can call SDL_SetRenderLogicalPresentation() to change the content size and
   * scaling options.
   *
   * It renderer creation fails for any reason this object is falsy; call
   * GetError() for more information.
   *
   * @param window the window where rendering is displayed.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  RendererBase(WindowRef window)
    : T(SDL_CreateRenderer(window.get(), nullptr))
  {
  }

  /**
   * Create a 2D rendering context for a window.
   *
   * If you want a specific renderer, you can specify its name here. A list of
   * available renderers can be obtained by calling SDL_GetRenderDriver()
   * multiple times, with indices from 0 to SDL_GetNumRenderDrivers()-1. If you
   * don't need a specific renderer, specify NULL and SDL will attempt to choose
   * the best option for you, based on what is available on the user's system.
   *
   * If `name` is a comma-separated list, SDL will try each name, in the order
   * listed, until one succeeds or all of them fail.
   *
   * By default the rendering size matches the window size in pixels, but you
   * can call SDL_SetRenderLogicalPresentation() to change the content size and
   * scaling options.
   *
   * It renderer creation fails for any reason this object is falsy; call
   * GetError() for more information.
   *
   * @param window the window where rendering is displayed.
   * @param name the name of the rendering driver to initialize
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetNumRenderDrivers()
   * @sa GetRenderDriver()
   * @sa GetRendererName()
   */
  RendererBase(WindowRef window, StringParam name)
    : T(SDL_CreateRenderer(window.get(), name))
  {
  }

  /**
   * Create a 2D rendering context for a window, with the specified properties.
   *
   * These are the supported properties:
   *
   * - `SDL_PROP_RENDERER_CREATE_NAME_STRING`: the name of the rendering driver
   *   to use, if a specific one is desired
   * - `SDL_PROP_RENDERER_CREATE_WINDOW_POINTER`: the window where rendering is
   *   displayed, required if this isn't a software renderer using a surface
   * - `SDL_PROP_RENDERER_CREATE_SURFACE_POINTER`: the surface where rendering
   *   is displayed, if you want a software renderer without a window
   * - `SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER`: an SDL_Colorspace
   *   value describing the colorspace for output to the display, defaults to
   *   SDL_COLORSPACE_SRGB. The direct3d11, direct3d12, and metal renderers
   *   support SDL_COLORSPACE_SRGB_LINEAR, which is a linear color space and
   *   supports HDR output. If you select SDL_COLORSPACE_SRGB_LINEAR, drawing
   *   still uses the sRGB colorspace, but values can go beyond 1.0 and float
   *   (linear) format textures can be used for HDR content.
   * - `SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER`: non-zero if you want
   *   present synchronized with the refresh rate. This property can take any
   *   value that is supported by SDL_SetRenderVSync() for the renderer.
   *
   * With the vulkan renderer:
   *
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_INSTANCE_POINTER`: the VkInstance to use
   *   with the renderer, optional.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_SURFACE_NUMBER`: the VkSurfaceKHR to use
   *   with the renderer, optional.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_PHYSICAL_DEVICE_POINTER`: the
   *   VkPhysicalDevice to use with the renderer, optional.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_DEVICE_POINTER`: the VkDevice to use
   *   with the renderer, optional.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER`: the
   *   queue family index used for rendering.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER`: the
   *   queue family index used for presentation.
   *
   * It renderer creation fails for any reason this object is falsy; call
   * GetError() for more information.
   *
   * @param props the properties to use.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties
   * @sa GetName()
   */
  RendererBase(PropertiesRef props)
    : T(SDL_CreateRendererWithProperties(props.get()))
  {
  }

  /**
   * Create a 2D software rendering context for a surface.
   *
   * Two other API which can be used to create RendererBase:
   * RendererBase.RendererBase() and CreateWindowAndRenderer(). These can _also_
   * create a software renderer, but they are intended to be used with an
   * WindowBase as the final destination and not an SurfaceBase.
   *
   * It renderer creation fails for any reason this object is falsy; call
   * GetError() for more information.
   *
   * @param surface the SDL_Surface structure representing the surface where
   *                rendering is done.
   * @post a valid rendering context or nullptr if there was an error; call
   *       GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RendererBase.Destroy
   */
  RendererBase(SurfaceBase& surface)
    : T(SDL_CreateSoftwareRenderer(surface.get()))
  {
  }

  /**
   * Get the window associated with a renderer.
   *
   * @returns the window on success or NULL on failure; call SDL_GetError() for
   *          more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  WindowRef GetWindow() { return {SDL_GetRenderWindow(T::get())}; }

  /**
   * Get the name of a renderer.
   *
   * @returns the name of the selected renderer, or NULL on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   */
  const char* GetName() const { return SDL_GetRendererName(T::get()); }

  /**
   * Get the output size in pixels of a rendering context.
   *
   * This returns the true output size in pixels, ignoring any render targets or
   * logical size and presentation.
   *
   * @returns Point on success or std::nullopt on failure; call GetError() for
   *          more information.
   */
  std::optional<Point> GetOutputSize() const
  {
    if (Point p; GetOutputSize(&p.x, &p.y)) return p;
    return std::nullopt;
  }

  /**
   * Get the output size in pixels of a rendering context.
   *
   * This returns the true output size in pixels, ignoring any render targets or
   * logical size and presentation.
   *
   * @param w a pointer filled in with the width in pixels.
   * @param h a pointer filled in with the height in pixels.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetCurrentOutputSize()
   */
  bool GetOutputSize(int* w, int* h) const
  {
    return SDL_GetRenderOutputSize(T::get(), w, h);
  }
  /**
   * Get the current output size in pixels of a rendering context.
   *
   * If a rendering target is active, this will return the size of the rendering
   * target in pixels, otherwise if a logical size is set, it will return the
   * logical size, otherwise it will return the value of
   * SDL_GetRenderOutputSize().
   *
   * @returns the size on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetOutputSize()
   */
  std::optional<Point> GetCurrentOutputSize() const
  {
    if (Point p; GetCurrentOutputSize(&p.x, &p.y)) return p;
    return std::nullopt;
  }

  /**
   * Get the current output size in pixels of a rendering context.
   *
   * If a rendering target is active, this will return the size of the rendering
   * target in pixels, otherwise if a logical size is set, it will return the
   * logical size, otherwise it will return the value of
   * SDL_GetRenderOutputSize().
   *
   * @param w a pointer filled in with the current width.
   * @param h a pointer filled in with the current height.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetOutputSize()
   */
  bool GetCurrentOutputSize(int* w, int* h) const
  {
    return SDL_GetCurrentRenderOutputSize(T::get(), w, h);
  }

  /**
   * Get the properties associated with a renderer.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `SDL_PROP_RENDERER_NAME_STRING`: the name of the rendering driver
   * - `SDL_PROP_RENDERER_WINDOW_POINTER`: the window where rendering is
   *   displayed, if any
   * - `SDL_PROP_RENDERER_SURFACE_POINTER`: the surface where rendering is
   *   displayed, if this is a software renderer without a window
   * - `SDL_PROP_RENDERER_VSYNC_NUMBER`: the current vsync setting
   * - `SDL_PROP_RENDERER_MAX_TEXTURE_SIZE_NUMBER`: the maximum texture width
   *   and height
   * - `SDL_PROP_RENDERER_TEXTURE_FORMATS_POINTER`: a (const SDL_PixelFormat *)
   *   array of pixel formats, terminated with SDL_PIXELFORMAT_UNKNOWN,
   *   representing the available texture formats for this renderer.
   * - `SDL_PROP_RENDERER_OUTPUT_COLORSPACE_NUMBER`: an SDL_Colorspace value
   *   describing the colorspace for output to the display, defaults to
   *   SDL_COLORSPACE_SRGB.
   * - `SDL_PROP_RENDERER_HDR_ENABLED_BOOLEAN`: true if the output colorspace is
   *   SDL_COLORSPACE_SRGB_LINEAR and the renderer is showing on a display with
   *   HDR enabled. This property can change dynamically when
   *   SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `SDL_PROP_RENDERER_SDR_WHITE_POINT_FLOAT`: the value of SDR white in the
   *   SDL_COLORSPACE_SRGB_LINEAR colorspace. When HDR is enabled, this value is
   *   automatically multiplied into the color scale. This property can change
   *   dynamically when SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `SDL_PROP_RENDERER_HDR_HEADROOM_FLOAT`: the additional high dynamic range
   *   that can be displayed, in terms of the SDR white point. When HDR is not
   *   enabled, this will be 1.0. This property can change dynamically when
   *   SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   *
   * With the direct3d renderer:
   *
   * - `SDL_PROP_RENDERER_D3D9_DEVICE_POINTER`: the IDirect3DDevice9 associated
   *   with the renderer
   *
   * With the direct3d11 renderer:
   *
   * - `SDL_PROP_RENDERER_D3D11_DEVICE_POINTER`: the ID3D11Device associated
   *   with the renderer
   * - `SDL_PROP_RENDERER_D3D11_SWAPCHAIN_POINTER`: the IDXGISwapChain1
   *   associated with the renderer. This may change when the window is resized.
   *
   * With the direct3d12 renderer:
   *
   * - `SDL_PROP_RENDERER_D3D12_DEVICE_POINTER`: the ID3D12Device associated
   *   with the renderer
   * - `SDL_PROP_RENDERER_D3D12_SWAPCHAIN_POINTER`: the IDXGISwapChain4
   *   associated with the renderer.
   * - `SDL_PROP_RENDERER_D3D12_COMMAND_QUEUE_POINTER`: the ID3D12CommandQueue
   *   associated with the renderer
   *
   * With the vulkan renderer:
   *
   * - `SDL_PROP_RENDERER_VULKAN_INSTANCE_POINTER`: the VkInstance associated
   *   with the renderer
   * - `SDL_PROP_RENDERER_VULKAN_SURFACE_NUMBER`: the VkSurfaceKHR associated
   *   with the renderer
   * - `SDL_PROP_RENDERER_VULKAN_PHYSICAL_DEVICE_POINTER`: the VkPhysicalDevice
   *   associated with the renderer
   * - `SDL_PROP_RENDERER_VULKAN_DEVICE_POINTER`: the VkDevice associated with
   *   the renderer
   * - `SDL_PROP_RENDERER_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER`: the queue
   *   family index used for rendering
   * - `SDL_PROP_RENDERER_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER`: the queue
   *   family index used for presentation
   * - `SDL_PROP_RENDERER_VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER`: the number of
   *   swapchain images, or potential frames in flight, used by the Vulkan
   *   renderer
   *
   * With the gpu renderer:
   *
   * - `SDL_PROP_RENDERER_GPU_DEVICE_POINTER`: the SDL_GPUDevice associated with
   *   the renderer
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return PropertiesRef{SDL_GetRendererProperties(T::get())};
  }

  /**
   * Set target texture back to window
   *
   * This is equivalent to SetTaget(nullptr)
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetTarget(nullptr)
   * @sa GetTarget()
   */
  bool ResetTarget() { return SetTarget(nullptr); }

  /**
   * Set a texture as the current rendering target.
   *
   * The default render target is the window for which the renderer was created.
   * To stop rendering to a texture and render to the window again, call this
   * function with a nullptr `texture`.
   *
   * @param texture the targeted texture, which must be created with the
   *                `SDL_TEXTUREACCESS_TARGET` flag, or NULL to render to the
   *                window instead of a texture.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetTarget()
   */
  bool SetTarget(TextureRef texture);

  /**
   * Get the current render target.
   *
   * The default render target is the window for which the renderer was created,
   * and is reported a NULL here.
   *
   * @returns the current render target or NULL for the default render target.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetTarget()
   */
  TextureRef GetTarget() const;

  /**
   * Set a device independent resolution and presentation mode for rendering.
   *
   * This function sets the width and height of the logical rendering output.
   * The renderer will act as if the window is always the requested dimensions,
   * scaling to the actual window resolution as necessary.
   *
   * This can be useful for games that expect a fixed size, but would like to
   * scale the output to whatever is available, regardless of how a user resizes
   * a window, or if the display is high DPI.
   *
   * You can disable logical coordinates by setting the mode to
   * SDL_LOGICAL_PRESENTATION_DISABLED, and in that case you get the full pixel
   * resolution of the output window; it is safe to toggle logical presentation
   * during the rendering of a frame: perhaps most of the rendering is done to
   * specific dimensions but to make fonts look sharp, the app turns off logical
   * presentation while drawing text.
   *
   * Letterboxing will only happen if logical presentation is enabled during
   * SDL_RenderPresent; be sure to reenable it first if you were using it.
   *
   * You can convert coordinates in an event into rendering coordinates using
   * SDL_ConvertEventToRenderCoordinates().
   *
   * @param size the width and height of the logical resolution.
   * @param mode the presentation mode used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ConvertEventToRenderCoordinates()
   * @sa GetLogicalPresentation()
   * @sa GetLogicalPresentationRect()
   */
  bool SetLogicalPresentation(SDL_Point size, RendererLogicalPresentation mode)
  {
    return SDL_SetRenderLogicalPresentation(T::get(), size.x, size.y, mode);
  }

  /**
   * Get device independent resolution and presentation mode for rendering.
   *
   * This function gets the width and height of the logical rendering output, or
   * the output size in pixels if a logical resolution is not enabled.
   *
   * @param size a Point to be filled with the width and height.
   * @param mode the presentation mode used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   */
  bool GetLogicalPresentation(SDL_Point* size,
                              RendererLogicalPresentation* mode)
  {
    if (!size) return GetLogicalPresentation(nullptr, nullptr, mode);
    return GetLogicalPresentation(&size->x, &size->y, mode);
  }

  /**
   * Get device independent resolution and presentation mode for rendering.
   *
   * This function gets the width and height of the logical rendering output, or
   * the output size in pixels if a logical resolution is not enabled.
   *
   * @param w an int to be filled with the width.
   * @param h an int to be filled with the height.
   * @param mode the presentation mode used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   */
  bool GetLogicalPresentation(int* w,
                              int* h,
                              RendererLogicalPresentation* mode) const
  {
    return SDL_GetRenderLogicalPresentation(T::get(), w, h, mode);
  }

  /**
   * Get the final presentation rectangle for rendering.
   *
   * This function returns the calculated rectangle used for logical
   * presentation, based on the presentation mode and output size. If logical
   * presentation is disabled, it will fill the rectangle with the output size,
   * in pixels.
   *
   * @returns the rect with the final presentation rectangle on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   */
  std::optional<FRect> GetLogicalPresentationRect() const
  {
    if (FRect rect; SDL_GetRenderLogicalPresentationRect(T::get(), &rect)) {
      return rect;
    }
    return std::nullopt;
  }

  /**
   * Get a point in render coordinates when given a point in window coordinates.
   *
   * This takes into account several states:
   *
   * - The window dimensions.
   * - The logical presentation settings (SDL_SetRenderLogicalPresentation)
   * - The scale (SDL_SetRenderScale)
   * - The viewport (SDL_SetRenderViewport)
   *
   * @param window_coord the x, y coordinate in window coordinates.
   * @returns a FPoint containing ther render coordinates on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   * @sa SetScale()
   */
  std::optional<FPoint> RenderCoordinatesFromWindow(
    const SDL_FPoint& window_coord) const
  {
    return SDL_RenderCoordinatesFromWindow(T::get(), window_coord);
  }

  /**
   * Get a point in window coordinates when given a point in render coordinates.
   *
   * This takes into account several states:
   *
   * - The window dimensions.
   * - The logical presentation settings (SDL_SetRenderLogicalPresentation)
   * - The scale (SDL_SetRenderScale)
   * - The viewport (SDL_SetRenderViewport)
   *
   * @param coord the x, y coordinate in render coordinates.
   * @returns a FPoint filled with window coordinates on success or std::nullopt
   * on failure; call SDL_GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   * @sa SetScale()
   * @sa SetViewport()
   */
  std::optional<FPoint> RenderCoordinatesToWindow(const SDL_FPoint& coord) const
  {
    return SDL_RenderCoordinatesToWindow(T::get(), coord);
  }

  /**
   * Convert the coordinates in an event to render coordinates.
   *
   * This takes into account several states:
   *
   * - The window dimensions.
   * - The logical presentation settings (RendererBase.SetLogicalPresentation)
   * - The scale (RendererBase.SetScale)
   * - The viewport (RendererBase.SetViewport)
   *
   * Various event types are converted with this function: mouse, touch, pen,
   * etc.
   *
   * Touch coordinates are converted from normalized coordinates in the window
   * to non-normalized rendering coordinates.
   *
   * Relative mouse coordinates (xrel and yrel event fields) are _also_
   * converted. Applications that do not want these fields converted should use
   * RendererBase.RenderCoordinatesFromWindow() on the specific event fields
   * instead of converting the entire event structure.
   *
   * Once converted, coordinates may be outside the rendering area.
   *
   * @param event the event to modify.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RendererBase.RenderCoordinatesFromWindow
   */
  bool ConvertEventToRenderCoordinates(Event* event) const
  {
    return SDL_ConvertEventToRenderCoordinates(T::get(), event);
  }

  /**
   * Reset the drawing area for rendering to the entire target
   *
   * This is equivalent to `SetViewport(std::nullopt)`
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetViewport()
   * @sa SetViewport()
   * @sa ViewportSet()
   */
  bool ResetViewport() { return SetViewport(std::nullopt); }

  /**
   * Set the drawing area for rendering on the current target.
   *
   * Drawing will clip to this area (separately from any clipping done with
   * SDL_SetRenderClipRect), and the top left of the area will become coordinate
   * (0, 0) for future drawing commands.
   *
   * The area's width and height must be >= 0.
   *
   * Each render target has its own viewport. This function sets the viewport
   * for the current render target.
   *
   * @param rect the SDL_Rect structure representing the drawing area, or
   *             std::nullopt to set the viewport to the entire target.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetViewport()
   * @sa ResetViewport()
   * @sa ViewportSet()
   */
  bool SetViewport(OptionalRef<const SDL_Rect> rect)
  {
    return SDL_SetRenderViewport(T::get(), rect);
  }

  /**
   * Get the drawing area for the current target.
   *
   * Each render target has its own viewport. This function gets the viewport
   * for the current render target.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IsViewportSet()
   * @sa SetViewport()
   */
  std::optional<Rect> GetViewport() const
  {
    if (Rect rect; SDL_GetRenderViewport(T::get(), &rect)) return rect;
    return {};
  }

  /**
   * Return whether an explicit rectangle was set as the viewport.
   *
   * This is useful if you're saving and restoring the viewport and want to know
   * whether you should restore a specific rectangle or NULL. Note that the
   * viewport is always reset when changing rendering targets.
   *
   * Each render target has its own viewport. This function checks the viewport
   * for the current render target.
   *
   * @returns true if the viewport was set to a specific rectangle, or false if
   *          it was set to NULL (the entire target).
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetViewport()
   * @sa SetViewport()
   */
  bool IsViewportSet() const { return SDL_RenderViewportSet(T::get()); }

  /**
   * Get the safe area for rendering within the current viewport.
   *
   * Some devices have portions of the screen which are partially obscured or
   * not interactive, possibly due to on-screen controls, curved edges, camera
   * notches, TV overscan, etc. This function provides the area of the current
   * viewport which is safe to have interactible content. You should continue
   * rendering into the rest of the render target, but it should not contain
   * visually important or interactible content.
   *
   * @returns the rect filled the area that is safe for interactive content on
   * success or std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Rect> GetSafeArea() const
  {
    if (Rect rect; SDL_GetRenderSafeArea(T::get(), &rect)) return rect;
    return {};
  }

  /**
   * Reset the clip rectangle for rendering to the entire render target
   *
   * This is equivalent to `SetClipRect(std::nullopt)`
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetClipRect()
   * @sa SetClipRect()
   * @sa ClipEnabled()
   */
  bool ResetClipRect() { return SetClipRect({}); }

  /**
   * Set the clip rectangle for rendering on the specified target.
   *
   * @param rect an SDL_Rect structure representing the clip area, relative to
   *             the viewport, or NULL to disable clipping.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetClipRect()
   * @sa ResetClipRect()
   * @sa ClipEnabled()
   */
  bool SetClipRect(OptionalRef<const SDL_Rect> rect)
  {
    return SDL_SetRenderClipRect(T::get(), rect);
  }

  /**
   * Get the clip rectangle for the current target.
   *
   * @returns the rect filled in with the current clipping area or an empty
   * rectangle if clipping is disabled on success; std::nullopt on failure, call
   * GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ClipEnabled()
   * @sa SetClipRect()
   */
  std::optional<Rect> GetClipRect() const
  {
    if (Rect rect; SDL_GetRenderClipRect(T::get(), &rect)) return rect;
    return {};
  }

  /**
   * Get whether clipping is enabled on the given renderer.
   *
   * @returns true if clipping is enabled or false if not; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetClipRect()
   * @sa SetClipRect()
   */
  bool IsClipEnabled() const { return SDL_RenderClipEnabled(T::get()); }

  /**
   * Set the drawing scale for rendering on the current target.
   *
   * The drawing coordinates are scaled by the x/y scaling factors before they
   * are used by the renderer. This allows resolution independent drawing with a
   * single coordinate system.
   *
   * If this results in scaling or subpixel drawing by the rendering backend, it
   * will be handled using the appropriate quality hints. For best results use
   * integer scaling factors.
   *
   * @param scale the x, y scaling factors.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetScale()
   */
  bool SetScale(SDL_FPoint scale)
  {
    return SDL_SetRenderScale(T::get(), scale.x, scale.y);
  }

  /**
   * Get the drawing scale for the current target.
   *
   * @returns the scaling factors on success or std::nullopt on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetScale()
   */
  std::optional<FPoint> GetScale() const
  {
    if (FPoint p; GetScale(&p.x, &p.y)) return p;
    return std::nullopt;
  }

  /**
   * Get the drawing scale for the current target.
   *
   * @param scaleX a pointer filled in with the horizontal scaling factor.
   * @param scaleY a pointer filled in with the vertical scaling factor.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetScale()
   */
  bool GetScale(float* scaleX, float* scaleY) const
  {
    return SDL_GetRenderScale(T::get(), scaleX, scaleY);
  }

  /**
   * Set the color used for drawing operations.
   *
   * Set the color for drawing or filling rectangles, lines, and points, and for
   * SDL_RenderClear().
   *
   * @param c the color value used to draw on the rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColor()
   * @sa SetDrawColorFloat()
   */
  bool SetDrawColor(SDL_Color c)
  {
    return SDL_SetRenderDrawColor(T::get(), c.r, c.g, c.b, c.a);
  }

  /**
   * Set the color used for drawing operations (Rect, Line and Clear).
   *
   * Set the color for drawing or filling rectangles, lines, and points, and for
   * SDL_RenderClear().
   *
   * @param c the RGBA values used to draw on the rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColorFloat()
   * @sa SetDrawColor()
   */
  bool SetDrawColor(SDL_FColor c)
  {
    return SDL_SetRenderDrawColorFloat(T::get(), c.r, c.g, c.b, c.a);
  }

  /**
   * Get the color used for drawing operations (Rect, Line and Clear).
   *
   * @returns the color on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColor(SDL_FColor*)
   * @sa SetDrawColor()
   */
  std::optional<FColor> GetDrawColor() const
  {
    if (FColor color; GetDrawColor(&color)) return color;
    return std::nullopt;
  }

  /**
   * Get the color used for drawing operations (Rect, Line and Clear).
   *
   * @param c a pointer filled in with the color channel values used to draw on
   *          the rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColor(SDL_FColor*)
   * @sa SetDrawColor()
   */
  bool GetDrawColor(SDL_Color* c) const
  {
    if (!c) {
      return SDL_GetRenderDrawColor(
        T::get(), nullptr, nullptr, nullptr, nullptr);
    }
    return GetDrawColor(&c->r, &c->g, &c->b, &c->a);
  }

  /**
   * Get the color used for drawing operations (Rect, Line and Clear).
   *
   * @param c a pointer filled in with the color channel values used to draw on
   *          the rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColor(SDL_Color*)
   * @sa SetDrawColor()
   */
  bool GetDrawColor(SDL_FColor* c) const
  {
    if (!c) {
      return SDL_GetRenderDrawColorFloat(
        T::get(), nullptr, nullptr, nullptr, nullptr);
    }
    return GetDrawColor(&c->r, &c->g, &c->b, &c->a);
  }

  /**
   * Get the color used for drawing operations (Rect, Line and Clear).
   *
   * @param r a pointer filled in with the red value used to draw on the
   *          rendering target.
   * @param g a pointer filled in with the green value used to draw on the
   *          rendering target.
   * @param b a pointer filled in with the blue value used to draw on the
   *          rendering target.
   * @param a a pointer filled in with the alpha value used to draw on the
   *          rendering target; usually `SDL_ALPHA_OPAQUE` (255).
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColor(SDL_Color*)
   * @sa SetDrawColor()
   */
  bool GetDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a) const
  {
    return SDL_GetRenderDrawColor(T::get(), r, g, b, a);
  }

  /**
   * Get the color used for drawing operations (Rect, Line and Clear).
   *
   * @param r a pointer filled in with the red value used to draw on the
   *          rendering target.
   * @param g a pointer filled in with the green value used to draw on the
   *          rendering target.
   * @param b a pointer filled in with the blue value used to draw on the
   *          rendering target.
   * @param a a pointer filled in with the alpha value used to draw on the
   *          rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetDrawColorFloat()
   * @sa GetDrawColor()
   */
  bool GetDrawColor(float* r, float* g, float* b, float* a) const
  {
    return SDL_GetRenderDrawColorFloat(T::get(), r, g, b, a);
  }

  /**
   * Set the color scale used for render operations.
   *
   * The color scale is an additional scale multiplied into the pixel color
   * value while rendering. This can be used to adjust the brightness of colors
   * during HDR rendering, or changing HDR video brightness when playing on an
   * SDR display.
   *
   * The color scale does not affect the alpha channel, only the color
   * brightness.
   *
   * @param scale the color scale value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorScale()
   */
  bool SetColorScale(float scale)
  {
    return SDL_SetRenderColorScale(T::get(), scale);
  }

  /**
   * Get the color scale used for render operations.
   *
   * @returns a float representing the current color scale on success or
   * std::nullopt on failure; call SDL_GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorScale()
   */
  std::optional<float> GetColorScale() const
  {
    if (float scale; SDL_GetRenderColorScale(T::get(), &scale)) return scale;
    return std::nullopt;
  }

  /**
   * Set the blend mode used for drawing operations (Fill and Line).
   *
   * If the blend mode is not supported, the closest supported mode is chosen.
   *
   * @param blendMode the SDL_BlendMode to use for blending.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawBlendMode()
   */
  bool SetDrawBlendMode(BlendMode blendMode)
  {
    return SDL_SetRenderDrawBlendMode(T::get(), blendMode);
  }

  /**
   * Get the blend mode used for drawing operations.
   *
   * @returns the current BlendMode on success or std::nullopt on failure; call
   * SDL_GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetDrawBlendMode()
   */
  std::optional<BlendMode> GetDrawBlendMode() const
  {
    if (BlendMode blendMode; SDL_GetRenderDrawBlendMode(T::get(), &blendMode)) {
      return blendMode;
    }
    return std::nullopt;
  }

  /**
   * Clear the current rendering target with the drawing color.
   *
   * This function clears the entire rendering target, ignoring the viewport and
   * the clip rectangle. Note, that clearing will also set/fill all pixels of
   * the rendering target to current renderer draw color, so make sure to invoke
   * SDL_SetRenderDrawColor() when needed.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetDrawColor()
   */
  bool RenderClear() { return SDL_RenderClear(T::get()); }

  /**
   * Draw a point on the current rendering target at subpixel precision.
   *
   * @param p the x, y coordinates of the point.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderPoints
   */
  bool RenderPoint(SDL_FPoint p) { return SDL_RenderPoint(T::get(), p); }

  /**
   * Draw multiple points on the current rendering target at subpixel precision.
   *
   * @param points the points to draw.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderPoint
   */
  bool RenderPoints(SpanRef<const SDL_FPoint> points)
  {
    SDL_assert_paranoid(points.size() < SDL_MAX_SINT32);
    return SDL_RenderPoints(T::get(), points.data(), points.size());
  }

  /**
   * Draw a line on the current rendering target at subpixel precision.
   *
   * @param p1 the x,y coordinate of the start point.
   * @param p2 the x,y coordinate of the end point.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderLines
   */
  bool RenderLine(SDL_FPoint p1, SDL_FPoint p2)
  {
    return SDL_RenderLine(T::get(), p1, p2);
  }

  /**
   * Draw a series of connected lines on the current rendering target at
   * subpixel precision.
   *
   * @param points the points along the lines, drawing count-1 lines.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderLine
   */
  bool RenderLines(SpanRef<const SDL_FPoint> points)
  {
    SDL_assert_paranoid(points.size() < SDL_MAX_SINT32);
    return SDL_RenderLines(T::get(), points.data(), points.size());
  }

  /**
   * Draw a rectangle on the current rendering target at subpixel precision.
   *
   * @param rect a pointer to the destination rectangle, or std::nullopt to
   *             outline the entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderRects
   */
  bool RenderRect(OptionalRef<const SDL_FRect> rect)
  {
    return SDL_RenderRect(T::get(), rect);
  }

  /**
   * Draw some number of rectangles on the current rendering target at subpixel
   * precision.
   *
   * @param rects a pointer to an array of destination rectangles.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderRect
   */
  bool RenderRects(SpanRef<const SDL_FRect> rects)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_SINT32);
    return SDL_RenderRects(T::get(), rects.data(), rects.size());
  }

  /**
   * Fill a rectangle on the current rendering target with the drawing color at
   * subpixel precision.
   *
   * @param rect a pointer to the destination rectangle, or std::nullopt for the
   *             entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderFillRects
   */
  bool RenderFillRect(OptionalRef<const SDL_FRect> rect)
  {
    return SDL_RenderFillRect(T::get(), rect);
  }

  /**
   * Fill some number of rectangles on the current rendering target with the
   * drawing color at subpixel precision.
   *
   * @param rects a pointer to an array of destination rectangles.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderFillRect
   */
  bool RenderFillRects(SpanRef<const SDL_FRect> rects)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_SINT32);
    return SDL_RenderFillRects(T::get(), rects.data(), rects.size());
  }

  /**
   * Copy a portion of the texture to the current rendering target at subpixel
   * precision.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or NULL for the entire
   *                texture.
   * @param dstrect a pointer to the destination rectangle, or NULL for the
   *                entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTextureRotated
   * @sa SDL_RenderTextureTiled
   */
  bool RenderTexture(TextureRef texture,
                     OptionalRef<const SDL_FRect> srcrect,
                     OptionalRef<const SDL_FRect> dstrect);

  /**
   * Copy a portion of the source texture to the current rendering target, with
   * rotation and flipping, at subpixel precision.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or NULL for the entire
   *                texture.
   * @param dstrect a pointer to the destination rectangle, or NULL for the
   *                entire rendering target.
   * @param angle an angle in degrees that indicates the rotation that will be
   *              applied to dstrect, rotating it in a clockwise direction.
   * @param center a pointer to a point indicating the point around which
   *               dstrect will be rotated (if NULL, rotation will be done
   *               around dstrect.w/2, dstrect.h/2).
   * @param flip an SDL_FlipMode value stating which flipping actions should be
   *             performed on the texture.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTexture
   */
  bool RenderTextureRotated(TextureRef texture,
                            OptionalRef<const SDL_FRect> srcrect,
                            OptionalRef<const SDL_FRect> dstrect,
                            double angle,
                            OptionalRef<const SDL_FPoint> center = {},
                            FlipMode flip = SDL_FLIP_NONE);

  /**
   * Copy a portion of the source texture to the current rendering target, with
   * affine transform, at subpixel precision.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or NULL for the entire
   *                texture.
   * @param origin a pointer to a point indicating where the top-left corner of
   *               srcrect should be mapped to, or NULL for the rendering
   *               target's origin.
   * @param right a pointer to a point indicating where the top-right corner of
   *              srcrect should be mapped to, or NULL for the rendering
   *              target's top-right corner.
   * @param down a pointer to a point indicating where the bottom-left corner of
   *             srcrect should be mapped to, or NULL for the rendering target's
   *             bottom-left corner.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety You may only call this function from the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTexture
   */
  bool RenderTextureAffine(TextureRef texture,
                           OptionalRef<const SDL_FRect> srcrect,
                           OptionalRef<const SDL_FPoint> origin,
                           OptionalRef<const SDL_FPoint> right,
                           OptionalRef<const SDL_FPoint> down);

  /**
   * Tile a portion of the texture to the current rendering target at subpixel
   * precision.
   *
   * The pixels in `srcrect` will be repeated as many times as needed to
   * completely fill `dstrect`.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or NULL for the entire
   *                texture.
   * @param scale the scale used to transform srcrect into the destination
   *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
   *              64x64 tiles.
   * @param dstrect a pointer to the destination rectangle, or NULL for the
   *                entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTexture
   */
  bool RenderTextureTiled(TextureRef texture,
                          OptionalRef<const SDL_FRect> srcrect,
                          float scale,
                          OptionalRef<const SDL_FRect> dstrect);

  /**
   * Perform a scaled copy using the 9-grid algorithm to the current rendering
   * target at subpixel precision.
   *
   * The pixels in the texture are split into a 3x3 grid, using the different
   * corner sizes for each corner, and the sides and center making up the
   * remaining pixels. The corners are then scaled using `scale` and fit into
   * the corners of the destination rectangle. The sides and center are then
   * stretched into place to cover the remaining destination rectangle.
   *
   * @param texture the source texture.
   * @param srcrect the SDL_Rect structure representing the rectangle to be used
   *                for the 9-grid, or NULL to use the entire texture.
   * @param left_width the width, in pixels, of the left corners in `srcrect`.
   * @param right_width the width, in pixels, of the right corners in `srcrect`.
   * @param top_height the height, in pixels, of the top corners in `srcrect`.
   * @param bottom_height the height, in pixels, of the bottom corners in
   *                      `srcrect`.
   * @param scale the scale used to transform the corner of `srcrect` into the
   *              corner of `dstrect`, or 0.0f for an unscaled copy.
   * @param dstrect a pointer to the destination rectangle, or NULL for the
   *                entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTexture
   */
  bool RenderTexture9Grid(TextureRef texture,
                          OptionalRef<const SDL_FRect> srcrect,
                          float left_width,
                          float right_width,
                          float top_height,
                          float bottom_height,
                          float scale,
                          OptionalRef<const SDL_FRect> dstrect);

  /**
   * Render a list of triangles, optionally using a texture and indices into the
   * vertex array Color and alpha modulation is done per vertex
   * (SDL_SetTextureColorMod and SDL_SetTextureAlphaMod are ignored).
   *
   * @param texture (optional) The SDL texture to use.
   * @param vertices vertices.
   * @param indices (optional) An array of integer indices into the 'vertices'
   *                array, if NULL all vertices will be rendered in sequential
   *                order.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderGeometryRaw
   */
  bool RenderGeometry(TextureRef texture,
                      std::span<const Vertex> vertices,
                      std::span<const int> indices = {});

  /**
   * Render a list of triangles, optionally using a texture and indices into the
   * vertex arrays Color and alpha modulation is done per vertex
   * (SDL_SetTextureColorMod and SDL_SetTextureAlphaMod are ignored).
   *
   * @param texture (optional) The SDL texture to use.
   * @param xy vertex positions.
   * @param xy_stride byte size to move from one element to the next element.
   * @param color vertex colors (as SDL_FColor).
   * @param color_stride byte size to move from one element to the next element.
   * @param uv vertex normalized texture coordinates.
   * @param uv_stride byte size to move from one element to the next element.
   * @param num_vertices number of vertices.
   * @param indices (optional) An array of indices into the 'vertices' arrays,
   *                if NULL all vertices will be rendered in sequential order.
   * @param num_indices number of indices.
   * @param size_indices index size: 1 (byte), 2 (short), 4 (int).
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderGeometry
   */
  bool RenderGeometryRaw(TextureRef texture,
                         const float* xy,
                         int xy_stride,
                         const FColor* color,
                         int color_stride,
                         const float* uv,
                         int uv_stride,
                         int num_vertices,
                         const void* indices,
                         int num_indices,
                         int size_indices);

  /**
   * Read pixels from the current rendering target.
   *
   * **WARNING**: This is a very slow operation, and should not be used
   * frequently. If you're using this on the main rendering target, it should be
   * called after rendering and before SDL_RenderPresent().
   *
   * @param rect an SDL_Rect structure representing the area in pixels relative
   *             to the to current viewport, or std::nullopt for the entire
   *             viewport.
   * @returns a new SDL_Surface on success or NULL on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Surface RenderReadPixels(OptionalRef<const SDL_Rect> rect) const
  {
    return {SDL_RenderReadPixels(T::get(), rect)};
  }

  /**
   * Update the screen with any rendering performed since the previous call.
   *
   * SDL's rendering functions operate on a backbuffer; that is, calling a
   * rendering function such as SDL_RenderLine() does not directly put a line on
   * the screen, but rather updates the backbuffer. As such, you compose your
   * entire scene and *present* the composed backbuffer to the screen as a
   * complete picture.
   *
   * Therefore, when using SDL's rendering API, one does all drawing intended
   * for the frame, and then calls this function once per frame to present the
   * final drawing to the user.
   *
   * The backbuffer should be considered invalidated after each present; do not
   * assume that previous contents will exist between frames. You are strongly
   * encouraged to call SDL_RenderClear() to initialize the backbuffer before
   * starting each new frame's drawing, even if you plan to overwrite every
   * pixel.
   *
   * Please note, that in case of rendering to a texture - there is **no need**
   * to call `SDL_RenderPresent` after drawing needed objects to a texture, and
   * should not be done; you are only required to change back the rendering
   * target to default via `SDL_SetRenderTarget(renderer, NULL)` afterwards, as
   * textures by themselves do not have a concept of backbuffers. Calling
   * SDL_RenderPresent while rendering to a texture will still update the screen
   * with any current drawing that has been done _to the window itself_.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RenderClear()
   * @sa RenderFillRect()
   * @sa RenderFillRects()
   * @sa RenderLine()
   * @sa RenderLines()
   * @sa RenderPoint()
   * @sa RenderPoints()
   * @sa RenderRect()
   * @sa RenderRects()
   * @sa SetDrawBlendMode()
   * @sa SetDrawColor()
   */
  bool Present() { return SDL_RenderPresent(T::get()); }

  /**
   * Force the rendering context to flush any pending commands and state.
   *
   * You do not need to (and in fact, shouldn't) call this function unless you
   * are planning to call into OpenGL/Direct3D/Metal/whatever directly, in
   * addition to using an SDL_Renderer.
   *
   * This is for a very-specific case: if you are using SDL's render API, and
   * you plan to make OpenGL/D3D/whatever calls in addition to SDL render API
   * calls. If this applies, you should call this function between calls to
   * SDL's render API and the low-level API you're using in cooperation.
   *
   * In all other cases, you can ignore this function.
   *
   * This call makes SDL flush any pending rendering work it was queueing up to
   * do later in a single batch, and marks any internal cached state as invalid,
   * so it'll prepare all its state again later, from scratch.
   *
   * This means you do not need to save state in your rendering code to protect
   * the SDL renderer. However, there lots of arbitrary pieces of Direct3D and
   * OpenGL state that can confuse things; you should use your best judgment and
   * be prepared to make changes if specific state needs to be protected.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Flush() { return SDL_FlushRenderer(T::get()); }

  /**
   * Toggle VSync of the given renderer.
   *
   * When a renderer is created, vsync defaults to SDL_RENDERER_VSYNC_DISABLED.
   *
   * The `vsync` parameter can be 1 to synchronize present with every vertical
   * refresh, 2 to synchronize present with every second vertical refresh, etc.,
   * SDL_RENDERER_VSYNC_ADAPTIVE for late swap tearing (adaptive vsync), or
   * SDL_RENDERER_VSYNC_DISABLED to disable. Not every value is supported by
   * every driver, so you should check the return value to see whether the
   * requested setting is supported.
   *
   * @param vsync the vertical refresh sync interval.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetVSync()
   */
  bool SetVSync(int vsync) { return SDL_SetRenderVSync(T::get(), vsync); }

  /**
   * Get VSync of the given renderer.
   *
   * @returns the current vertical refresh sync interval on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetVSync()
   */
  std::optional<int> GetVSync() const
  {
    if (int vsync; SDL_GetRenderVSync(T::get(), &vsync)) return vsync;
    return std::nullopt;
  }

  /**
   * Draw debug text to an SDL_Renderer.
   *
   * This function will render a string of text to an SDL_Renderer. Note that
   * this is a convenience function for debugging, with severe limitations, and
   * not intended to be used for production apps and games.
   *
   * Among these limitations:
   *
   * - It accepts UTF-8 strings, but will only renders ASCII characters.
   * - It has a single, tiny size (8x8 pixels). One can use logical presentation
   *   or scaling to adjust it, but it will be blurry.
   * - It uses a simple, hardcoded bitmap font. It does not allow different font
   *   selections and it does not support truetype, for proper scaling.
   * - It does no word-wrapping and does not treat newline characters as a line
   *   break. If the text goes out of the window, it's gone.
   *
   * For serious text rendering, there are several good options, such as
   * SDL_ttf, stb_truetype, or other external libraries.
   *
   * On first use, this will create an internal texture for rendering glyphs.
   * This texture will live until the renderer is destroyed.
   *
   * The text is drawn in the color specified by SDL_SetRenderDrawColor().
   *
   * @param p the x, y coordinates where the top-left corner of the text will
   * draw.
   * @param str the string to render.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa DebugTextFormat()
   * @sa SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE
   */
  bool RenderDebugText(FPoint p, StringParam str)
  {
    return SDL_RenderDebugText(T::get(), p.x, p.y, str);
  }

  /**
   * Draw debug text to an SDL_Renderer.
   *
   * This function will render a std::format()-style format string to a
   * renderer. Note that this is a convenience function for debugging, with
   * severe limitations, and is not intended to be used for production apps and
   * games.
   *
   * For the full list of limitations and other useful information, see
   * SDL_RenderDebugText.
   *
   * @param p the x, y coordinates where the top-left corner of the text will
   * draw.
   * @param fmt the format string to draw.
   * @param args additional parameters matching {} tokens in the `fmt` string,
   * if any.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RenderDebugText()
   * @sa SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE
   */
  template<class... ARGS>
  bool RenderDebugTextFormat(FPoint p, std::string_view fmt, ARGS... args)
  {
    return RenderDebugText(p,
                           std::vformat(fmt, std::make_format_args(args...)));
  }

  /**
   * Destroy the rendering context for a window and free all associated
   * textures.
   *
   * This object becomes empty after the call.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CreateRenderer
   */
  void Destroy() { return SDL_DestroyRenderer(T::release()); }
};

/**
 * Destroy the rendering context for a window and free all associated
 * textures.
 *
 * This should be called before destroying the associated window.
 *
 * @param renderer the rendering context.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer
 * @sa RendererBase
 */
template<>
inline void ObjectRef<SDL_Renderer>::doFree(SDL_Renderer* resource)
{
  return SDL_DestroyRenderer(resource);
}

/**
 * An efficient driver-specific representation of pixel data
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa resource
 * @sa Texture
 * @sa TextureRef
 * @sa RendererBase
 */
template<ObjectBox<SDL_Texture*> T>
struct TextureBase : T
{
  using T::T;

  /**
   * Load an image from a filesystem path into a GPU texture.
   *
   * If available, this uses LoadTexture(RendererRef, StringParam), otherwise it
   * uses LoadTextureBMP(RendererRef, StringParam).
   *
   * @param renderer the Renderer to use to create the GPU texture.
   * @param file a path on the filesystem to load an image from.
   * @post the new structure that is created and convertible to true on success
   * or convertible to false on failure; call GetError() for more information.
   *
   * @sa LoadTexture(RendererRef, StringParam)
   * @sa LoadTextureBMP(RendererRef, StringParam)
   */
  TextureBase(RendererRef renderer, StringParam file);

  /**
   * Create a texture for a rendering context.
   *
   * The contents of a texture when first created are not defined.
   *
   * @param renderer the rendering context.
   * @param format one of the enumerated values in SDL_PixelFormat.
   * @param access one of the enumerated values in SDL_TextureAccess.
   * @param w the width of the texture in pixels.
   * @param h the height of the texture in pixels.
   * @post the created texture is convertible to true on success or false on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   * @sa Update()
   */
  TextureBase(RendererRef renderer,
              PixelFormat format,
              TextureAccess access,
              int w,
              int h)
    : T(SDL_CreateTexture(renderer.get(), format, access, w, h))
  {
  }

  /**
   * Create a texture from an existing surface.
   *
   * The surface is not modified or freed by this function.
   *
   * The TextureAccess hint for the created texture is TEXTUREACCESS_STATIC`.
   *
   * The pixel format of the created texture may be different from the pixel
   * format of the surface, and can be queried using the
   * prop::Texture.FORMAT_NUMBER property.
   *
   * @param renderer the rendering context.
   * @param surface the SurfaceBase structure containing pixel data used to fill
   *                the texture.
   * @post the created texture or nullptr on failure; call GetError() for
   *       more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TextureBase.TextureBase
   * @sa TextureBase.Destroy
   */
  TextureBase(RendererRef renderer, SurfaceBase& surface)
    : T(SDL_CreateTextureFromSurface(renderer.get(), surface.get()))
  {
  }

  /**
   * Create a texture for a rendering context with the specified properties.
   *
   * These are the supported properties:
   *
   * - `SDL_PROP_TEXTURE_CREATE_COLORSPACE_NUMBER`: an SDL_Colorspace value
   *   describing the texture colorspace, defaults to SDL_COLORSPACE_SRGB_LINEAR
   *   for floating point textures, SDL_COLORSPACE_HDR10 for 10-bit textures,
   *   SDL_COLORSPACE_SRGB for other RGB textures and SDL_COLORSPACE_JPEG for
   *   YUV textures.
   * - `SDL_PROP_TEXTURE_CREATE_FORMAT_NUMBER`: one of the enumerated values in
   *   SDL_PixelFormat, defaults to the best RGBA format for the renderer
   * - `SDL_PROP_TEXTURE_CREATE_ACCESS_NUMBER`: one of the enumerated values in
   *   SDL_TextureAccess, defaults to SDL_TEXTUREACCESS_STATIC
   * - `SDL_PROP_TEXTURE_CREATE_WIDTH_NUMBER`: the width of the texture in
   *   pixels, required
   * - `SDL_PROP_TEXTURE_CREATE_HEIGHT_NUMBER`: the height of the texture in
   *   pixels, required
   * - `SDL_PROP_TEXTURE_CREATE_SDR_WHITE_POINT_FLOAT`: for HDR10 and floating
   *   point textures, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 100 for HDR10 textures and 1.0 for floating point textures.
   * - `SDL_PROP_TEXTURE_CREATE_HDR_HEADROOM_FLOAT`: for HDR10 and floating
   *   point textures, this defines the maximum dynamic range used by the
   *   content, in terms of the SDR white point. This would be equivalent to
   *   maxCLL / SDL_PROP_TEXTURE_CREATE_SDR_WHITE_POINT_FLOAT for HDR10 content.
   *   If this is defined, any values outside the range supported by the display
   *   will be scaled into the available HDR headroom, otherwise they are
   *   clipped.
   *
   * With the direct3d11 renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_POINTER`: the ID3D11Texture2D
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_U_POINTER`: the ID3D11Texture2D
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_V_POINTER`: the ID3D11Texture2D
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the direct3d12 renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_POINTER`: the ID3D12Resource
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_U_POINTER`: the ID3D12Resource
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_V_POINTER`: the ID3D12Resource
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the metal renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_METAL_PIXELBUFFER_POINTER`: the CVPixelBufferRef
   *   associated with the texture, if you want to create a texture from an
   *   existing pixel buffer.
   *
   * With the opengl renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the opengles2 renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the vulkan renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_VULKAN_TEXTURE_NUMBER`: the VkImage with layout
   *   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL associated with the texture, if
   *   you want to wrap an existing texture.
   *
   * @param renderer the rendering context.
   * @param props the properties to use.
   * @post the created texture is convertible to true on success or false on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CreateProperties
   * @sa SDL_CreateTexture
   * @sa SDL_CreateTextureFromSurface
   * @sa SDL_DestroyTexture
   * @sa SDL_GetTextureSize
   * @sa SDL_UpdateTexture
   */
  TextureBase(RendererRef renderer, PropertiesRef props)
    : T(SDL_CreateTextureWithProperties(renderer.get(), props))
  {
  }

  /**
   * Get the properties associated with a texture.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `SDL_PROP_TEXTURE_COLORSPACE_NUMBER`: an SDL_Colorspace value describing
   *   the texture colorspace.
   * - `SDL_PROP_TEXTURE_FORMAT_NUMBER`: one of the enumerated values in
   *   SDL_PixelFormat.
   * - `SDL_PROP_TEXTURE_ACCESS_NUMBER`: one of the enumerated values in
   *   SDL_TextureAccess.
   * - `SDL_PROP_TEXTURE_WIDTH_NUMBER`: the width of the texture in pixels.
   * - `SDL_PROP_TEXTURE_HEIGHT_NUMBER`: the height of the texture in pixels.
   * - `SDL_PROP_TEXTURE_SDR_WHITE_POINT_FLOAT`: for HDR10 and floating point
   *   textures, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 100 for HDR10 textures and 1.0 for other textures.
   * - `SDL_PROP_TEXTURE_HDR_HEADROOM_FLOAT`: for HDR10 and floating point
   *   textures, this defines the maximum dynamic range used by the content, in
   *   terms of the SDR white point. If this is defined, any values outside the
   *   range supported by the display will be scaled into the available HDR
   *   headroom, otherwise they are clipped. This defaults to 1.0 for SDR
   *   textures, 4.0 for HDR10 textures, and no default for floating point
   *   textures.
   *
   * With the direct3d11 renderer:
   *
   * - `SDL_PROP_TEXTURE_D3D11_TEXTURE_POINTER`: the ID3D11Texture2D associated
   *   with the texture
   * - `SDL_PROP_TEXTURE_D3D11_TEXTURE_U_POINTER`: the ID3D11Texture2D
   *   associated with the U plane of a YUV texture
   * - `SDL_PROP_TEXTURE_D3D11_TEXTURE_V_POINTER`: the ID3D11Texture2D
   *   associated with the V plane of a YUV texture
   *
   * With the direct3d12 renderer:
   *
   * - `SDL_PROP_TEXTURE_D3D12_TEXTURE_POINTER`: the ID3D12Resource associated
   *   with the texture
   * - `SDL_PROP_TEXTURE_D3D12_TEXTURE_U_POINTER`: the ID3D12Resource associated
   *   with the U plane of a YUV texture
   * - `SDL_PROP_TEXTURE_D3D12_TEXTURE_V_POINTER`: the ID3D12Resource associated
   *   with the V plane of a YUV texture
   *
   * With the vulkan renderer:
   *
   * - `SDL_PROP_TEXTURE_VULKAN_TEXTURE_NUMBER`: the VkImage associated with the
   *   texture
   *
   * With the opengl renderer:
   *
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_NUMBER`: the GLuint texture associated
   *   with the texture
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_U_NUMBER`: the GLuint texture associated
   *   with the U plane of a YUV texture
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_V_NUMBER`: the GLuint texture associated
   *   with the V plane of a YUV texture
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_TARGET_NUMBER`: the GLenum for the
   *   texture target (`GL_TEXTURE_2D`, `GL_TEXTURE_RECTANGLE_ARB`, etc)
   * - `SDL_PROP_TEXTURE_OPENGL_TEX_W_FLOAT`: the texture coordinate width of
   *   the texture (0.0 - 1.0)
   * - `SDL_PROP_TEXTURE_OPENGL_TEX_H_FLOAT`: the texture coordinate height of
   *   the texture (0.0 - 1.0)
   *
   * With the opengles2 renderer:
   *
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_TARGET_NUMBER`: the GLenum for the
   *   texture target (`GL_TEXTURE_2D`, `GL_TEXTURE_EXTERNAL_OES`, etc)
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return PropertiesRef{SDL_GetTextureProperties(T::get())};
  }

  /**
   * Get the renderer that created an SDL_Texture.
   *
   * @returns a pointer to the SDL_Renderer that created the texture, or NULL on
   *          failure; call SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  RendererRef GetRenderer() const
  {
    return SDL_GetRendererFromTexture(T::get());
  }

  /**
   * Set an additional color and alpha values multiplied into render copy
   * operations.
   *
   * When this texture is rendered, during the copy operation each source color
   * and alpha channels are modulated by the appropriate color value according
   * to the following formula:
   *
   *      srcC = srcC * (color / 255)
   *      srcA = srcA * (alpha / 255)
   *
   * Color and alpha modulation is not always supported by the renderer; it will
   * return false if either modulation is not supported.
   *
   * @param c the color and alpha channel values multiplied into copy
   *          operations.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetColorAndAlphaMod(Color c)
  {
    return SetColorMod(c.r, c.g, c.b) && SetAlphaMod(c.a);
  }

  /**
   * Set an additional color and alpha values multiplied into render copy
   * operations.
   *
   * When this texture is rendered, during the copy operation each source color
   * and alpha channels are modulated by the appropriate color value according
   * to the following formula:
   *
   *      srcC = srcC * (color / 255)
   *      srcA = srcA * (alpha / 255)
   *
   * Color and alpha modulation is not always supported by the renderer; it will
   * return false if either modulation is not supported.
   *
   * @param c the color and alpha channel values multiplied into copy
   *          operations.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetColorAndAlphaMod(FColor c)
  {
    return SetColorMod(c.r, c.g, c.b) && SetAlphaMod(c.a);
  }

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @returns the color channels (0-1) on success or false on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetAlphaMod()
   * @sa SetColorMod()
   */
  std::optional<FColor> GetColorAndAlphaMod() const
  {
    if (FColor color; GetColorAndAlphaMod(&color)) return color;
    return std::nullopt;
  }

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @param c a pointer filled in with the current color and alpha mod values.
   * @returns true success or false on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetAlphaMod()
   * @sa SetColorMod()
   */
  bool GetColorAndAlphaMod(Color* c) const
  {
    SDL_assert(c != nullptr);
    return GetColorMod(&c->r, &c->g, &c->b) && GetAlphaMod(&c->a);
  }

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @param c a pointer filled in with the current color and alpha mod values.
   * @returns true success or false on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetAlphaMod()
   * @sa SetColorMod()
   */
  bool GetColorAndAlphaMod(FColor* c) const
  {
    SDL_assert(c != nullptr);
    return GetColorMod(&c->r, &c->g, &c->b) && GetAlphaMod(&c->a);
  }

  /**
   * Set an additional color value multiplied into render copy operations.
   *
   * When this texture is rendered, during the copy operation each source color
   * channel is modulated by the appropriate color value according to the
   * following formula:
   *
   * `srcC = srcC * (color / 255)`
   *
   * Color modulation is not always supported by the renderer; it will return
   * false if color modulation is not supported.
   *
   * @param r the red color value multiplied into copy operations.
   * @param g the green color value multiplied into copy operations.
   * @param b the blue color value multiplied into copy operations.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorMod()
   * @sa SetAlphaMod()
   */
  bool SetColorMod(Uint8 r, Uint8 g, Uint8 b)
  {
    return SDL_SetTextureColorMod(T::get(), r, g, b);
  }

  /**
   * Set an additional color value multiplied into render copy operations.
   *
   * When this texture is rendered, during the copy operation each source color
   * channel is modulated by the appropriate color value according to the
   * following formula:
   *
   * `srcC = srcC * color`
   *
   * Color modulation is not always supported by the renderer; it will return
   * false if color modulation is not supported.
   *
   * @param r the red color value multiplied into copy operations.
   * @param g the green color value multiplied into copy operations.
   * @param b the blue color value multiplied into copy operations.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureColorModFloat
   * @sa SDL_SetTextureAlphaModFloat
   * @sa SDL_SetTextureColorMod
   */
  bool SetColorMod(float r, float g, float b)
  {
    return SDL_SetTextureColorModFloat(T::get(), r, g, b);
  }

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetAlphaMod()
   * @sa SetColorMod()
   */
  bool GetColorMod(Uint8* r, Uint8* g, Uint8* b) const
  {
    return SDL_GetTextureColorMod(T::get(), r, g, b);
  }

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureAlphaModFloat
   * @sa SDL_GetTextureColorMod
   * @sa SDL_SetTextureColorModFloat
   */
  bool GetColorMod(float* r, float* g, float* b) const
  {
    return SDL_GetTextureColorModFloat(T::get(), r, g, b);
  }

  /**
   * Set an additional alpha value multiplied into render copy operations.
   *
   * When this texture is rendered, during the copy operation the source alpha
   * value is modulated by this alpha value according to the following formula:
   *
   * `srcA = srcA * (alpha / 255)`
   *
   * Alpha modulation is not always supported by the renderer; it will return
   * false if alpha modulation is not supported.
   *
   * @param alpha the source alpha value multiplied into copy operations.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureAlphaMod
   * @sa SDL_SetTextureAlphaModFloat
   * @sa SDL_SetTextureColorMod
   */
  bool SetAlphaMod(Uint8 alpha)
  {
    return SDL_SetTextureAlphaMod(T::get(), alpha);
  }

  /**
   * Set an additional alpha value multiplied into render copy operations.
   *
   * When this texture is rendered, during the copy operation the source alpha
   * value is modulated by this alpha value according to the following formula:
   *
   * `srcA = srcA * alpha`
   *
   * Alpha modulation is not always supported by the renderer; it will return
   * false if alpha modulation is not supported.
   *
   * @param alpha the source alpha value multiplied into copy operations.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureAlphaModFloat
   * @sa SDL_SetTextureAlphaMod
   * @sa SDL_SetTextureColorModFloat
   */
  bool SetAlphaMod(float alpha)
  {
    return SDL_SetTextureAlphaModFloat(T::get(), alpha);
  }

  /**
   * Get the additional alpha value multiplied into render copy operations.
   *
   * @returns the current alpha value on success or false on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  std::optional<float> GetAlphaMod() const
  {
    if (float alpha; GetAlphaMod(&alpha)) return alpha;
    return std::nullopt;
  }

  /**
   * Get the additional alpha value multiplied into render copy operations.
   *
   * @param alpha a pointer filled in with the current alpha value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetAlphaMod(Uint8* alpha) const
  {
    return SDL_GetTextureAlphaMod(T::get(), alpha);
  }

  /**
   * Get the additional alpha value multiplied into render copy operations.
   *
   * @param alpha a pointer filled in with the current alpha value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetAlphaMod(float* alpha) const
  {
    return SDL_GetTextureAlphaModFloat(T::get(), alpha);
  }

  /**
   * Set the blend mode for a texture, used by RendererBase::RenderTexture().
   *
   * If the blend mode is not supported, the closest supported mode is chosen
   * and this function returns false.
   *
   * @param blendMode the SDL_BlendMode to use for texture blending.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureBlendMode
   */
  bool SetBlendMode(BlendMode blendMode)
  {
    return SDL_SetTextureBlendMode(T::get(), blendMode);
  }

  /**
   * Get the blend mode used for texture copy operations.
   *
   * @returns the current SDL_BlendMode on success or std::nullopt on failure;
   * call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_SetTextureBlendMode
   */
  std::optional<BlendMode> GetBlendMode() const
  {
    if (BlendMode blendMode; SDL_GetTextureBlendMode(T::get(), &blendMode)) {
      return blendMode;
    }
    return std::nullopt;
  }

  /**
   * Set the scale mode used for texture scale operations.
   *
   * The default texture scale mode is SDL_SCALEMODE_LINEAR.
   *
   * If the scale mode is not supported, the closest supported mode is chosen.
   *
   * @param scaleMode the SDL_ScaleMode to use for texture scaling.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureScaleMode
   */
  bool SetScaleMode(ScaleMode scaleMode)
  {
    return SDL_SetTextureScaleMode(T::get(), scaleMode);
  }

  /**
   * Get the scale mode used for texture scale operations.
   *
   * @returns the current scale mode on success or std::nullopt on failure; call
   * SDL_GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_SetTextureScaleMode
   */
  std::optional<ScaleMode> GetScaleMode() const
  {
    if (ScaleMode scaleMode; SDL_GetTextureScaleMode(T::get(), &scaleMode)) {
      return scaleMode;
    }
    return std::nullopt;
  }

  /**
   * Update the given texture rectangle with new pixel data.
   *
   * The pixel data must be in the pixel format of the texture, which can be
   * queried using the SDL_PROP_TEXTURE_FORMAT_NUMBER property.
   *
   * This is a fairly slow function, intended for use with static textures that
   * do not change often.
   *
   * If the texture is intended to be updated often, it is preferred to create
   * the texture as streaming and use the locking functions referenced below.
   * While this function will work with streaming textures, for optimization
   * reasons you may not get the pixels back if you lock the texture afterward.
   *
   * @param rect an SDL_Rect structure representing the area to update, or NULL
   *             to update the entire texture.
   * @param pixels the raw pixel data in the format of the texture.
   * @param pitch the number of bytes in a row of pixel data, including padding
   *              between lines.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_LockTexture
   * @sa SDL_UnlockTexture
   * @sa SDL_UpdateNVTexture
   * @sa SDL_UpdateYUVTexture
   */
  bool Update(OptionalRef<const SDL_Rect> rect, const void* pixels, int pitch)
  {
    return SDL_UpdateTexture(T::get(), rect, pixels, pitch);
  }

  /**
   * Update a rectangle within a planar YV12 or IYUV texture with new pixel
   * data.
   *
   * You can use SDL_UpdateTexture() as long as your pixel data is a contiguous
   * block of Y and U/V planes in the proper order, but this function is
   * available if your pixel data is not contiguous.
   *
   * @param rect a pointer to the rectangle of pixels to update, or NULL to
   *             update the entire texture.
   * @param Yplane the raw pixel data for the Y plane.
   * @param Ypitch the number of bytes between rows of pixel data for the Y
   *               plane.
   * @param Uplane the raw pixel data for the U plane.
   * @param Upitch the number of bytes between rows of pixel data for the U
   *               plane.
   * @param Vplane the raw pixel data for the V plane.
   * @param Vpitch the number of bytes between rows of pixel data for the V
   *               plane.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_UpdateNVTexture
   * @sa SDL_UpdateTexture
   */
  bool UpdateYUV(OptionalRef<const SDL_Rect> rect,
                 const Uint8* Yplane,
                 int Ypitch,
                 const Uint8* Uplane,
                 int Upitch,
                 const Uint8* Vplane,
                 int Vpitch)
  {
    return SDL_UpdateYUVTexture(
      T::get(), rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
  }

  /**
   * Update a rectangle within a planar NV12 or NV21 texture with new pixels.
   *
   * You can use SDL_UpdateTexture() as long as your pixel data is a contiguous
   * block of NV12/21 planes in the proper order, but this function is available
   * if your pixel data is not contiguous.
   *
   * @param rect a pointer to the rectangle of pixels to update, or NULL to
   *             update the entire texture.
   * @param Yplane the raw pixel data for the Y plane.
   * @param Ypitch the number of bytes between rows of pixel data for the Y
   *               plane.
   * @param UVplane the raw pixel data for the UV plane.
   * @param UVpitch the number of bytes between rows of pixel data for the UV
   *                plane.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_UpdateTexture
   * @sa SDL_UpdateYUVTexture
   */
  bool UpdateNV(OptionalRef<const SDL_Rect> rect,
                const Uint8* Yplane,
                int Ypitch,
                const Uint8* UVplane,
                int UVpitch)
  {
    return SDL_UpdateNVTexture(
      T::get(), rect, Yplane, Ypitch, UVplane, UVpitch);
  }

  /**
   * Lock a portion of the texture for **write-only** pixel access.
   *
   * As an optimization, the pixels made available for editing don't necessarily
   * contain the old texture data. This is a write-only operation, and if you
   * need to keep a copy of the texture data you should do that at the
   * application level.
   *
   * You must use SDL_UnlockTexture() to unlock the pixels and apply any
   * changes.
   *
   * @param rect an SDL_Rect structure representing the area to lock for access;
   *             NULL to lock the entire texture.
   * @returns TextureLock on success or false if the texture is not valid or was
   * not created with `SDL_TEXTUREACCESS_STREAMING`; call GetError() for more
   * information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TextureLock.UnlockTexture
   */
  TextureLock Lock(OptionalRef<const SDL_Rect> rect) &;

  /**
   * Get the width in pixels.
   */
  int GetWidth() const { return T::get()->w; }

  /**
   * Get the height in pixels.
   */
  int GetHeight() const { return T::get()->h; }

  /**
   * Get the size in pixels.
   */
  Point GetSize() const { return Point(GetWidth(), GetHeight()); }

  /**
   * Get the pixel format.
   */
  PixelFormat GetFormat() const { return T::get()->format; }

  /**
   * Destroy the texture.
   *
   * This object becomes empty after the call.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CreateTexture
   * @sa SDL_CreateTextureFromSurface
   */
  void Destroy() { return SDL_DestroyTexture(T::release()); }
};

/**
 * Callback for texture resource cleanup
 *
 * @private
 */
template<>
inline void ObjectRef<SDL_Texture>::doFree(SDL_Texture* resource)
{
  return SDL_DestroyTexture(resource);
}

/**
 * Locks a Texture for access to its pixels
 */
class TextureLock
{
  TextureRef texture;
  SurfaceRef surface;

  /**
   * @sa TextureBase.Lock()
   */
  explicit TextureLock(TextureRef texture, OptionalRef<const SDL_Rect> rect)
    : texture(std::move(texture))
    , surface(nullptr)
  {
    SDL_Surface* maybeLock;
    if (SDL_LockTextureToSurface(this->texture.get(), rect, &maybeLock)) {
      surface = maybeLock;
    } else {
      texture.release();
    }
  }

public:
  /// default ctor
  TextureLock()
    : texture(nullptr)
    , surface(nullptr)
  {
  }

  // Copy ctor
  TextureLock(const TextureLock& other) = delete;

  /// Move ctor
  TextureLock(TextureLock&& other)
    : texture(other.texture.release())
    , surface(other.surface.release())
  {
  }

  /**
   * destructor
   * @sa Unlock()
   */
  ~TextureLock() { Unlock(); }

  /// Assignment operator
  TextureLock& operator=(TextureLock other)
  {
    std::swap(texture, other.texture);
    std::swap(surface, other.surface);
    return *this;
  }

  /**
   * Returns true if lock is active
   */
  constexpr operator bool() const { return bool(texture); }

  /**
   * Unlock a texture, uploading the changes to video memory, if needed.
   *
   * **Warning**: Please note that SDL_LockTexture() is intended to be
   * write-only; it will not guarantee the previous contents of the texture will
   * be provided. You must fully initialize any area of a texture that you lock
   * before unlocking it, as the pixels might otherwise be uninitialized memory.
   *
   * Which is to say: locking and immediately unlocking a texture can result in
   * corrupted textures, depending on the renderer in use.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_LockTexture
   */
  void Unlock()
  {
    surface.release();
    return SDL_UnlockTexture(texture.release());
  }

  /**
   * Get the pixels
   */
  void* GetPixels() const { return surface->pixels; }

  /**
   * Get pitch (the number of bytes between the start of one row the next)
   */
  int GetPitch() const { return surface->pitch; }

  /**
   * Get the pixel format
   */
  PixelFormat GetFormat() const { return surface->format; }

  template<ObjectBox<SDL_Texture*> T>
  friend class TextureBase;
};

/**
 * Get the number of 2D rendering drivers available for the current display.
 *
 * A render driver is a set of code that handles rendering and texture
 * management on a particular display. Normally there is only one, but some
 * drivers may have several available with different capabilities.
 *
 * There may be none if SDL was compiled without render support.
 *
 * @returns the number of built in render drivers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDriver()
 */
inline int GetNumRenderDrivers() { return SDL_GetNumRenderDrivers(); }

/**
 * Use this function to get the name of a built in 2D rendering driver.
 *
 * The list of rendering drivers is given in the order that they are normally
 * initialized by default; the drivers that seem more reasonable to choose
 * first (as far as the SDL developers believe) are earlier in the list.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "opengl",
 * "direct3d12" or "metal". These never have Unicode characters, and are not
 * meant to be proper names.
 *
 * @param index the index of the rendering driver; the value ranges from 0 to
 *              SDL_GetNumRenderDrivers() - 1.
 * @returns the name of the rendering driver at the requested index, or NULL
 *          if an invalid index was specified.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetNumRenderDrivers
 */
inline const char* GetRenderDriver(int index)
{
  return SDL_GetRenderDriver(index);
}

/**
 * Create a window and default renderer.
 *
 * @param title the title of the window, in UTF-8 encoding.
 * @param size the width and height of the window.
 * @param window_flags the flags used to create the window (see
 *                     Window::Window()).
 * @returns a pair with Window and Renderer on success or a pair of std::nullptr
 * on failure; call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer
 * @sa Window
 */
inline std::pair<Window, Renderer> CreateWindowAndRenderer(
  StringParam title,
  SDL_Point size,
  WindowFlags window_flags = 0)
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_CreateWindowAndRenderer(
    title, size.x, size.y, window_flags, &window, &renderer);
  return {Window{window}, Renderer{renderer}};
}

namespace prop::Renderer {

constexpr auto CREATE_NAME_STRING = SDL_PROP_RENDERER_CREATE_NAME_STRING;

constexpr auto CREATE_WINDOW_POINTER = SDL_PROP_RENDERER_CREATE_WINDOW_POINTER;

constexpr auto CREATE_SURFACE_POINTER =
  SDL_PROP_RENDERER_CREATE_SURFACE_POINTER;

constexpr auto CREATE_OUTPUT_COLORSPACE_NUMBER =
  SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER;

constexpr auto CREATE_PRESENT_VSYNC_NUMBER =
  SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER;

constexpr auto CREATE_VULKAN_INSTANCE_POINTER =
  SDL_PROP_RENDERER_CREATE_VULKAN_INSTANCE_POINTER;

constexpr auto CREATE_VULKAN_SURFACE_NUMBER =
  SDL_PROP_RENDERER_CREATE_VULKAN_SURFACE_NUMBER;

constexpr auto CREATE_VULKAN_PHYSICAL_DEVICE_POINTER =
  SDL_PROP_RENDERER_CREATE_VULKAN_PHYSICAL_DEVICE_POINTER;

constexpr auto CREATE_VULKAN_DEVICE_POINTER =
  SDL_PROP_RENDERER_CREATE_VULKAN_DEVICE_POINTER;

constexpr auto CREATE_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER =
  SDL_PROP_RENDERER_CREATE_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER;

constexpr auto CREATE_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER =
  SDL_PROP_RENDERER_CREATE_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER;

constexpr auto NAME_STRING = SDL_PROP_RENDERER_NAME_STRING;

constexpr auto WINDOW_POINTER = SDL_PROP_RENDERER_WINDOW_POINTER;

constexpr auto SURFACE_POINTER = SDL_PROP_RENDERER_SURFACE_POINTER;

constexpr auto VSYNC_NUMBER = SDL_PROP_RENDERER_VSYNC_NUMBER;

constexpr auto MAX_TEXTURE_SIZE_NUMBER =
  SDL_PROP_RENDERER_MAX_TEXTURE_SIZE_NUMBER;

constexpr auto TEXTURE_FORMATS_POINTER =
  SDL_PROP_RENDERER_TEXTURE_FORMATS_POINTER;

constexpr auto OUTPUT_COLORSPACE_NUMBER =
  SDL_PROP_RENDERER_OUTPUT_COLORSPACE_NUMBER;

constexpr auto HDR_ENABLED_BOOLEAN = SDL_PROP_RENDERER_HDR_ENABLED_BOOLEAN;

constexpr auto SDR_WHITE_POINT_FLOAT = SDL_PROP_RENDERER_SDR_WHITE_POINT_FLOAT;

constexpr auto HDR_HEADROOM_FLOAT = SDL_PROP_RENDERER_HDR_HEADROOM_FLOAT;

constexpr auto D3D9_DEVICE_POINTER = SDL_PROP_RENDERER_D3D9_DEVICE_POINTER;

constexpr auto D3D11_DEVICE_POINTER = SDL_PROP_RENDERER_D3D11_DEVICE_POINTER;

constexpr auto D3D11_SWAPCHAIN_POINTER =
  SDL_PROP_RENDERER_D3D11_SWAPCHAIN_POINTER;

constexpr auto D3D12_DEVICE_POINTER = SDL_PROP_RENDERER_D3D12_DEVICE_POINTER;

constexpr auto D3D12_SWAPCHAIN_POINTER =
  SDL_PROP_RENDERER_D3D12_SWAPCHAIN_POINTER;

constexpr auto D3D12_COMMAND_QUEUE_POINTER =
  SDL_PROP_RENDERER_D3D12_COMMAND_QUEUE_POINTER;

constexpr auto VULKAN_INSTANCE_POINTER =
  SDL_PROP_RENDERER_VULKAN_INSTANCE_POINTER;

constexpr auto VULKAN_SURFACE_NUMBER = SDL_PROP_RENDERER_VULKAN_SURFACE_NUMBER;

constexpr auto VULKAN_PHYSICAL_DEVICE_POINTER =
  SDL_PROP_RENDERER_VULKAN_PHYSICAL_DEVICE_POINTER;

constexpr auto VULKAN_DEVICE_POINTER = SDL_PROP_RENDERER_VULKAN_DEVICE_POINTER;

constexpr auto VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER =
  SDL_PROP_RENDERER_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER;

constexpr auto VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER =
  SDL_PROP_RENDERER_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER;

constexpr auto VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER =
  SDL_PROP_RENDERER_VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER;

constexpr auto GPU_DEVICE_POINTER = SDL_PROP_RENDERER_GPU_DEVICE_POINTER;

} // namespace prop::Renderer

namespace prop::Texture {

constexpr auto CREATE_COLORSPACE_NUMBER =
  SDL_PROP_TEXTURE_CREATE_COLORSPACE_NUMBER;

constexpr auto CREATE_FORMAT_NUMBER = SDL_PROP_TEXTURE_CREATE_FORMAT_NUMBER;

constexpr auto CREATE_ACCESS_NUMBER = SDL_PROP_TEXTURE_CREATE_ACCESS_NUMBER;

constexpr auto CREATE_WIDTH_NUMBER = SDL_PROP_TEXTURE_CREATE_WIDTH_NUMBER;

constexpr auto CREATE_HEIGHT_NUMBER = SDL_PROP_TEXTURE_CREATE_HEIGHT_NUMBER;

constexpr auto CREATE_SDR_WHITE_POINT_FLOAT =
  SDL_PROP_TEXTURE_CREATE_SDR_WHITE_POINT_FLOAT;

constexpr auto CREATE_HDR_HEADROOM_FLOAT =
  SDL_PROP_TEXTURE_CREATE_HDR_HEADROOM_FLOAT;

constexpr auto CREATE_D3D11_TEXTURE_POINTER =
  SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_POINTER;

constexpr auto CREATE_D3D11_TEXTURE_U_POINTER =
  SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_U_POINTER;

constexpr auto CREATE_D3D11_TEXTURE_V_POINTER =
  SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_V_POINTER;

constexpr auto CREATE_D3D12_TEXTURE_POINTER =
  SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_POINTER;

constexpr auto CREATE_D3D12_TEXTURE_U_POINTER =
  SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_U_POINTER;

constexpr auto CREATE_D3D12_TEXTURE_V_POINTER =
  SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_V_POINTER;

constexpr auto CREATE_METAL_PIXELBUFFER_POINTER =
  SDL_PROP_TEXTURE_CREATE_METAL_PIXELBUFFER_POINTER;

constexpr auto CREATE_OPENGL_TEXTURE_NUMBER =
  SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_NUMBER;

constexpr auto CREATE_OPENGL_TEXTURE_UV_NUMBER =
  SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_UV_NUMBER;

constexpr auto CREATE_OPENGL_TEXTURE_U_NUMBER =
  SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_U_NUMBER;

constexpr auto CREATE_OPENGL_TEXTURE_V_NUMBER =
  SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_V_NUMBER;

constexpr auto CREATE_OPENGLES2_TEXTURE_NUMBER =
  SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_NUMBER;

constexpr auto CREATE_OPENGLES2_TEXTURE_UV_NUMBER =
  SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_UV_NUMBER;

constexpr auto CREATE_OPENGLES2_TEXTURE_U_NUMBER =
  SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_U_NUMBER;

constexpr auto CREATE_OPENGLES2_TEXTURE_V_NUMBER =
  SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_V_NUMBER;

constexpr auto CREATE_VULKAN_TEXTURE_NUMBER =
  SDL_PROP_TEXTURE_CREATE_VULKAN_TEXTURE_NUMBER;

constexpr auto COLORSPACE_NUMBER = SDL_PROP_TEXTURE_COLORSPACE_NUMBER;

constexpr auto FORMAT_NUMBER = SDL_PROP_TEXTURE_FORMAT_NUMBER;

constexpr auto ACCESS_NUMBER = SDL_PROP_TEXTURE_ACCESS_NUMBER;

constexpr auto WIDTH_NUMBER = SDL_PROP_TEXTURE_WIDTH_NUMBER;

constexpr auto HEIGHT_NUMBER = SDL_PROP_TEXTURE_HEIGHT_NUMBER;

constexpr auto SDR_WHITE_POINT_FLOAT = SDL_PROP_TEXTURE_SDR_WHITE_POINT_FLOAT;

constexpr auto HDR_HEADROOM_FLOAT = SDL_PROP_TEXTURE_HDR_HEADROOM_FLOAT;

constexpr auto D3D11_TEXTURE_POINTER = SDL_PROP_TEXTURE_D3D11_TEXTURE_POINTER;

constexpr auto D3D11_TEXTURE_U_POINTER =
  SDL_PROP_TEXTURE_D3D11_TEXTURE_U_POINTER;

constexpr auto D3D11_TEXTURE_V_POINTER =
  SDL_PROP_TEXTURE_D3D11_TEXTURE_V_POINTER;

constexpr auto D3D12_TEXTURE_POINTER = SDL_PROP_TEXTURE_D3D12_TEXTURE_POINTER;

constexpr auto D3D12_TEXTURE_U_POINTER =
  SDL_PROP_TEXTURE_D3D12_TEXTURE_U_POINTER;

constexpr auto D3D12_TEXTURE_V_POINTER =
  SDL_PROP_TEXTURE_D3D12_TEXTURE_V_POINTER;

constexpr auto OPENGL_TEXTURE_NUMBER = SDL_PROP_TEXTURE_OPENGL_TEXTURE_NUMBER;

constexpr auto OPENGL_TEXTURE_UV_NUMBER =
  SDL_PROP_TEXTURE_OPENGL_TEXTURE_UV_NUMBER;

constexpr auto OPENGL_TEXTURE_U_NUMBER =
  SDL_PROP_TEXTURE_OPENGL_TEXTURE_U_NUMBER;

constexpr auto OPENGL_TEXTURE_V_NUMBER =
  SDL_PROP_TEXTURE_OPENGL_TEXTURE_V_NUMBER;

constexpr auto OPENGL_TEXTURE_TARGET_NUMBER =
  SDL_PROP_TEXTURE_OPENGL_TEXTURE_TARGET_NUMBER;

constexpr auto OPENGL_TEX_W_FLOAT = SDL_PROP_TEXTURE_OPENGL_TEX_W_FLOAT;

constexpr auto OPENGL_TEX_H_FLOAT = SDL_PROP_TEXTURE_OPENGL_TEX_H_FLOAT;

constexpr auto OPENGLES2_TEXTURE_NUMBER =
  SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_NUMBER;

constexpr auto OPENGLES2_TEXTURE_UV_NUMBER =
  SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_UV_NUMBER;

constexpr auto OPENGLES2_TEXTURE_U_NUMBER =
  SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_U_NUMBER;

constexpr auto OPENGLES2_TEXTURE_V_NUMBER =
  SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_V_NUMBER;

constexpr auto OPENGLES2_TEXTURE_TARGET_NUMBER =
  SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_TARGET_NUMBER;

constexpr auto VULKAN_TEXTURE_NUMBER = SDL_PROP_TEXTURE_VULKAN_TEXTURE_NUMBER;

} // namespace prop::Texture

/**
 * Get the CAMetalLayer associated with the given Metal renderer.
 *
 * This function returns `void *`, so SDL doesn't have to include Metal's
 * headers, but it can be safely cast to a `CAMetalLayer *`.
 *
 * @param renderer the renderer to query.
 * @returns a `CAMetalLayer *` on success, or NULL if the renderer isn't a
 *          Metal renderer.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetRenderMetalCommandEncoder
 */
inline void* GetRenderMetalLayer(RendererRef renderer)
{
  return SDL_GetRenderMetalLayer(renderer.get());
}

/**
 * Get the Metal command encoder for the current frame.
 *
 * This function returns `void *`, so SDL doesn't have to include Metal's
 * headers, but it can be safely cast to an `id<MTLRenderCommandEncoder>`.
 *
 * This will return NULL if Metal refuses to give SDL a drawable to render to,
 * which might happen if the window is hidden/minimized/offscreen. This
 * doesn't apply to command encoders for render targets, just the window's
 * backbuffer. Check your return values!
 *
 * @param renderer the renderer to query.
 * @returns an `id<MTLRenderCommandEncoder>` on success, or NULL if the
 *          renderer isn't a Metal renderer or there was an error.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetRenderMetalLayer
 */
inline void* GetRenderMetalCommandEncoder(RendererRef renderer)
{
  return SDL_GetRenderMetalCommandEncoder(renderer.get());
}

/**
 * Add a set of synchronization semaphores for the current frame.
 *
 * The Vulkan renderer will wait for `wait_semaphore` before submitting
 * rendering commands and signal `signal_semaphore` after rendering commands
 * are complete for this frame.
 *
 * This should be called each frame that you want semaphore synchronization.
 * The Vulkan renderer may have multiple frames in flight on the GPU, so you
 * should have multiple semaphores that are used for synchronization. Querying
 * SDL_PROP_RENDERER_VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER will give you the
 * maximum number of semaphores you'll need.
 *
 * @param renderer the rendering context.
 * @param wait_stage_mask the VkPipelineStageFlags for the wait.
 * @param wait_semaphore a VkSempahore to wait on before rendering the current
 *                       frame, or 0 if not needed.
 * @param signal_semaphore a VkSempahore that SDL will signal when rendering
 *                         for the current frame is complete, or 0 if not
 *                         needed.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is **NOT** safe to call this function from two threads at
 *               once.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool AddVulkanRenderSemaphores(RendererRef renderer,
                                      Uint32 wait_stage_mask,
                                      Sint64 wait_semaphore,
                                      Sint64 signal_semaphore)
{
  return SDL_AddVulkanRenderSemaphores(
    renderer.get(), wait_stage_mask, wait_semaphore, signal_semaphore);
}

#ifdef SDL3PP_DOC

/// Disable vsync
#define SDL_RENDERER_VSYNC_DISABLED 0

/// Adaptative vsync
#define SDL_RENDERER_VSYNC_ADAPTIVE (-1)

/**
 * The size, in pixels, of a single SDL_RenderDebugText() character.
 *
 * The font is monospaced and square, so this applies to all characters.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_RenderDebugText
 */
#define SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE 8

#endif // SDL3PP_DOC

/**
 * Load a BMP texture from a seekable SDL data stream.
 *
 * @param renderer the renderer to create texture
 * @param src the data stream for the surface.
 * @returns a Texture with loaded content or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 */
inline Texture LoadTextureBMP(RendererRef renderer,
                              ObjectBox<SDL_IOStream> auto&& src)
{
  Surface surface{LoadBMP(src)};
  return Texture(renderer, surface);
}

/**
 * Load a BMP texture from a file.
 *
 * @param renderer the renderer to create texture
 * @param file the BMP file to load.
 * @returns a Texture with loaded content or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Texture LoadTextureBMP(RendererRef renderer, StringParam file)
{
  Surface surface{SDL_LoadBMP(file)};
  return Texture(renderer, surface);
}

#pragma region impl
/// @}

template<ObjectBox<SDL_Window*> T>
inline RendererRef WindowBase<T>::GetRenderer() const
{
  return {SDL_GetRenderer(T::get())};
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::SetTarget(TextureRef texture)
{
  return SDL_SetRenderTarget(T::get(), texture);
}

template<ObjectBox<SDL_Renderer*> T>
TextureRef RendererBase<T>::GetTarget() const
{
  return SDL_GetRenderTarget(T::get());
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTexture(TextureRef texture,
                                    OptionalRef<const SDL_FRect> srcrect,
                                    OptionalRef<const SDL_FRect> dstrect)
{
  return SDL_RenderTexture(T::get(), texture.get(), srcrect, dstrect);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTextureRotated(TextureRef texture,
                                           OptionalRef<const SDL_FRect> srcrect,
                                           OptionalRef<const SDL_FRect> dstrect,
                                           double angle,
                                           OptionalRef<const SDL_FPoint> center,
                                           FlipMode flip)
{
  return SDL_RenderTextureRotated(
    T::get(), texture.get(), srcrect, dstrect, angle, center, flip);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTextureAffine(TextureRef texture,
                                          OptionalRef<const SDL_FRect> srcrect,
                                          OptionalRef<const SDL_FPoint> origin,
                                          OptionalRef<const SDL_FPoint> right,
                                          OptionalRef<const SDL_FPoint> down)
{
  return SDL_RenderTextureAffine(
    T::get(), texture.get(), srcrect, origin, right, down);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTextureTiled(TextureRef texture,
                                         OptionalRef<const SDL_FRect> srcrect,
                                         float scale,
                                         OptionalRef<const SDL_FRect> dstrect)
{
  return SDL_RenderTextureTiled(
    T::get(), texture.get(), srcrect, scale, dstrect);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTexture9Grid(TextureRef texture,
                                         OptionalRef<const SDL_FRect> srcrect,
                                         float left_width,
                                         float right_width,
                                         float top_height,
                                         float bottom_height,
                                         float scale,
                                         OptionalRef<const SDL_FRect> dstrect)
{
  return SDL_RenderTexture9Grid(T::get(),
                                texture.get(),
                                srcrect,
                                left_width,
                                right_width,
                                top_height,
                                bottom_height,
                                scale,
                                dstrect);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderGeometry(TextureRef texture,
                                     std::span<const Vertex> vertices,
                                     std::span<const int> indices)
{
  if (indices.empty()) {
    return SDL_RenderGeometry(
      T::get(), texture.get(), vertices.data(), vertices.size(), nullptr, 0);
  }
  return SDL_RenderGeometry(T::get(),
                            texture.get(),
                            vertices.data(),
                            vertices.size(),
                            indices.data(),
                            indices.size());
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderGeometryRaw(TextureRef texture,
                                        const float* xy,
                                        int xy_stride,
                                        const FColor* color,
                                        int color_stride,
                                        const float* uv,
                                        int uv_stride,
                                        int num_vertices,
                                        const void* indices,
                                        int num_indices,
                                        int size_indices)
{
  return SDL_RenderGeometryRaw(T::get(),
                               texture.get(),
                               xy,
                               xy_stride,
                               color,
                               color_stride,
                               uv,
                               uv_stride,
                               num_vertices,
                               indices,
                               num_indices,
                               size_indices);
}

template<ObjectBox<SDL_Texture*> T>
TextureLock TextureBase<T>::Lock(OptionalRef<const SDL_Rect> rect) &
{
  return TextureLock{*this, rect};
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_RENDER_H_ */
