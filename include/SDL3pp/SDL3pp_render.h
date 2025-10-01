#ifndef SDL3PP_RENDER_H_
#define SDL3PP_RENDER_H_

#include <SDL3/SDL_render.h>
#include "SDL3pp_blendmode.h"
#include "SDL3pp_events.h"
#include "SDL3pp_pixels.h"
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
struct Renderer;

/// Alias to raw representation for Renderer.
using RendererRaw = SDL_Renderer*;

// Forward decl
struct RendererRef;

/// Safely wrap Renderer for non owning parameters
struct RendererParam
{
  RendererRaw value; ///< parameter's RendererRaw

  /// Constructs from RendererRaw
  constexpr RendererParam(RendererRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr RendererParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const RendererParam& other) const = default;

  /// Converts to underlying RendererRaw
  constexpr operator RendererRaw() const { return value; }
};

// Forward decl
struct Texture;

/// Alias to raw representation for Texture.
using TextureRaw = SDL_Texture*;

/// Safely wrap Texture for non owning parameters
struct TextureParam
{
  TextureRaw value; ///< parameter's TextureRaw

  /// Constructs from TextureRaw
  constexpr TextureParam(TextureRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr TextureParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const TextureParam& other) const = default;

  /// Converts to underlying TextureRaw
  constexpr operator TextureRaw() const { return value; }
};

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
 * @since This struct is available since SDL 3.2.0.
 */
using Vertex = SDL_Vertex;

/**
 * The access pattern allowed for a texture.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using TextureAccess = SDL_TextureAccess;

constexpr TextureAccess TEXTUREACCESS_STATIC =
  SDL_TEXTUREACCESS_STATIC; ///< Changes rarely, not lockable.

constexpr TextureAccess TEXTUREACCESS_STREAMING =
  SDL_TEXTUREACCESS_STREAMING; ///< Changes frequently, lockable.

constexpr TextureAccess TEXTUREACCESS_TARGET =
  SDL_TEXTUREACCESS_TARGET; ///< Texture can be used as a render target.

/**
 * How the logical size is mapped to the output.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using RendererLogicalPresentation = SDL_RendererLogicalPresentation;

constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_DISABLED =
  SDL_LOGICAL_PRESENTATION_DISABLED; ///< There is no logical size in effect.

/// The rendered content is stretched to the output resolution.
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_STRETCH =
  SDL_LOGICAL_PRESENTATION_STRETCH;

/**
 * The rendered content is fit to the largest dimension and the other dimension
 * is letterboxed with black bars.
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_LETTERBOX =
  SDL_LOGICAL_PRESENTATION_LETTERBOX;

/**
 * The rendered content is fit to the smallest dimension and the other dimension
 * extends beyond the output bounds.
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_OVERSCAN =
  SDL_LOGICAL_PRESENTATION_OVERSCAN;

/**
 * The rendered content is scaled up by integer multiples to fit the output
 * resolution.
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_INTEGER_SCALE =
  SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;

/**
 * A structure representing rendering state
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class Renderer
{
  RendererRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Renderer() = default;

  /**
   * Constructs from RendererParam.
   *
   * @param resource a RendererRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Renderer(const RendererRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Renderer(const Renderer& other) = delete;

  /// Move constructor
  constexpr Renderer(Renderer&& other)
    : Renderer(other.release())
  {
  }

  constexpr Renderer(const RendererRef& other) = delete;

  constexpr Renderer(RendererRef&& other) = delete;

  /**
   * Create a 2D rendering context for a window.
   *
   * If you want a specific renderer, you can specify its name here. A list of
   * available renderers can be obtained by calling GetRenderDriver()
   * multiple times, with indices from 0 to GetNumRenderDrivers()-1. If you
   * don't need a specific renderer, specify nullptr and SDL will attempt to
   * choose the best option for you, based on what is available on the user's
   * system.
   *
   * If `name` is a comma-separated list, SDL will try each name, in the order
   * listed, until one succeeds or all of them fail.
   *
   * By default the rendering size matches the window size in pixels, but you
   * can call Renderer.SetLogicalPresentation() to change the content size and
   * scaling options.
   *
   * @param window the window where rendering is displayed.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.Renderer
   * @sa Renderer.Renderer
   * @sa Renderer.Destroy
   * @sa GetNumRenderDrivers
   * @sa GetRenderDriver
   * @sa Renderer.GetName
   */
  Renderer(WindowParam window)
    : m_resource(CheckError(SDL_CreateRenderer(window, nullptr)))
  {
  }

  /**
   * Create a 2D rendering context for a window.
   *
   * If you want a specific renderer, you can specify its name here. A list of
   * available renderers can be obtained by calling GetRenderDriver()
   * multiple times, with indices from 0 to GetNumRenderDrivers()-1. If you
   * don't need a specific renderer, specify nullptr and SDL will attempt to
   * choose the best option for you, based on what is available on the user's
   * system.
   *
   * If `name` is a comma-separated list, SDL will try each name, in the order
   * listed, until one succeeds or all of them fail.
   *
   * By default the rendering size matches the window size in pixels, but you
   * can call Renderer.SetLogicalPresentation() to change the content size and
   * scaling options.
   *
   * @param window the window where rendering is displayed.
   * @param name the name of the rendering driver to initialize, or nullptr to
   * let SDL choose one.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.Renderer
   * @sa Renderer.Renderer
   * @sa Renderer.Destroy
   * @sa GetNumRenderDrivers
   * @sa GetRenderDriver
   * @sa Renderer.GetName
   */
  Renderer(WindowParam window, StringParam name)
    : m_resource(CheckError(SDL_CreateRenderer(window, name)))
  {
  }

  /**
   * Create a 2D rendering context for a window, with the specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::Renderer.CREATE_NAME_STRING`: the name of the rendering driver
   *   to use, if a specific one is desired
   * - `prop::Renderer.CREATE_WINDOW_POINTER`: the window where rendering is
   *   displayed, required if this isn't a software renderer using a surface
   * - `prop::Renderer.CREATE_SURFACE_POINTER`: the surface where rendering
   *   is displayed, if you want a software renderer without a window
   * - `prop::Renderer.CREATE_OUTPUT_COLORSPACE_NUMBER`: an Colorspace
   *   value describing the colorspace for output to the display, defaults to
   *   COLORSPACE_SRGB. The direct3d11, direct3d12, and metal renderers
   *   support COLORSPACE_SRGB_LINEAR, which is a linear color space and
   *   supports HDR output. If you select COLORSPACE_SRGB_LINEAR, drawing
   *   still uses the sRGB colorspace, but values can go beyond 1.0 and float
   *   (linear) format textures can be used for HDR content.
   * - `prop::Renderer.CREATE_PRESENT_VSYNC_NUMBER`: non-zero if you want
   *   present synchronized with the refresh rate. This property can take any
   *   value that is supported by Renderer.SetVSync() for the renderer.
   *
   * With the vulkan renderer:
   *
   * - `prop::Renderer.CREATE_VULKAN_INSTANCE_POINTER`: the VkInstance to use
   *   with the renderer, optional.
   * - `prop::Renderer.CREATE_VULKAN_SURFACE_NUMBER`: the VkSurfaceKHR to use
   *   with the renderer, optional.
   * - `prop::Renderer.CREATE_VULKAN_PHYSICAL_DEVICE_POINTER`: the
   *   VkPhysicalDevice to use with the renderer, optional.
   * - `prop::Renderer.CREATE_VULKAN_DEVICE_POINTER`: the VkDevice to use
   *   with the renderer, optional.
   * - `prop::Renderer.CREATE_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER`: the
   *   queue family index used for rendering.
   * - `prop::Renderer.CREATE_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER`: the
   *   queue family index used for presentation.
   *
   * @param props the properties to use.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Create
   * @sa Renderer.Renderer
   * @sa Renderer.Renderer
   * @sa Renderer.Destroy
   * @sa Renderer.GetName
   */
  Renderer(PropertiesParam props)
    : m_resource(CheckError(SDL_CreateRendererWithProperties(props)))
  {
  }

  /**
   * Create a 2D software rendering context for a surface.
   *
   * Two other API which can be used to create Renderer:
   * Renderer.Renderer() and CreateWindowAndRenderer(). These can _also_
   * create a software renderer, but they are intended to be used with an
   * Window as the final destination and not an Surface.
   *
   * @param surface the Surface structure representing the surface where
   *                rendering is done.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.Destroy
   */
  Renderer(SurfaceParam surface)
    : m_resource(CheckError(SDL_CreateSoftwareRenderer(surface)))
  {
  }

  /// Destructor
  ~Renderer() { SDL_DestroyRenderer(m_resource); }

  /// Assignment operator.
  Renderer& operator=(Renderer other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying RendererRaw.
  constexpr RendererRaw get() const { return m_resource; }

  /// Retrieves underlying RendererRaw and clear this.
  constexpr RendererRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Renderer& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to RendererParam
  constexpr operator RendererParam() const { return {m_resource}; }

  /**
   * Destroy the rendering context for a window and free all associated
   * textures.
   *
   * This should be called before destroying the associated window.
   *
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.Renderer
   */
  void Destroy();

  /**
   * Get the window associated with a renderer.
   *
   * @returns the window on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  WindowRef GetWindow();

  /**
   * Get the name of a renderer.
   *
   * @returns the name of the selected renderer, or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.Renderer
   */
  const char* GetName() const;

  /**
   * Get the output size in pixels of a rendering context.
   *
   * This returns the true output size in pixels, ignoring any render targets or
   * logical size and presentation.
   *
   * @returns Point on success.
   * @throws Error on failure.
   */
  Point GetOutputSize() const
  {
    Point p;
    GetOutputSize(&p.x, &p.y);
    return p;
  }

  /**
   * Get the output size in pixels of a rendering context.
   *
   * This returns the true output size in pixels, ignoring any render targets or
   * logical size and presentation.
   *
   * For the output size of the current rendering target, with logical size
   * adjustments, use Renderer.GetCurrentOutputSize() instead.
   *
   * @param w a pointer filled in with the width in pixels.
   * @param h a pointer filled in with the height in pixels.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetCurrentOutputSize
   */
  void GetOutputSize(int* w, int* h) const;

  /**
   * Get the current output size in pixels of a rendering context.
   *
   * If a rendering target is active, this will return the size of the rendering
   * target in pixels, otherwise if a logical size is set, it will return the
   * logical size, otherwise it will return the value of GetOutputSize().
   *
   * @returns the size on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetOutputSize()
   */
  Point GetCurrentOutputSize() const
  {
    Point p;
    GetCurrentOutputSize(&p.x, &p.y);
    return p;
  }

  /**
   * Get the current output size in pixels of a rendering context.
   *
   * If a rendering target is active, this will return the size of the rendering
   * target in pixels, otherwise return the value of Renderer.GetOutputSize().
   *
   * Rendering target or not, the output will be adjusted by the current logical
   * presentation state, dictated by Renderer.SetLogicalPresentation().
   *
   * @param w a pointer filled in with the current width.
   * @param h a pointer filled in with the current height.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetOutputSize
   */
  void GetCurrentOutputSize(int* w, int* h) const;

  /**
   * Get the properties associated with a renderer.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `prop::Renderer.NAME_STRING`: the name of the rendering driver
   * - `prop::Renderer.WINDOW_POINTER`: the window where rendering is
   *   displayed, if any
   * - `prop::Renderer.SURFACE_POINTER`: the surface where rendering is
   *   displayed, if this is a software renderer without a window
   * - `prop::Renderer.VSYNC_NUMBER`: the current vsync setting
   * - `prop::Renderer.MAX_TEXTURE_SIZE_NUMBER`: the maximum texture width
   *   and height
   * - `prop::Renderer.TEXTURE_FORMATS_POINTER`: a (const PixelFormat *)
   *   array of pixel formats, terminated with PIXELFORMAT_UNKNOWN,
   *   representing the available texture formats for this renderer.
   * - `prop::Renderer.OUTPUT_COLORSPACE_NUMBER`: an Colorspace value
   *   describing the colorspace for output to the display, defaults to
   *   COLORSPACE_SRGB.
   * - `prop::Renderer.HDR_ENABLED_BOOLEAN`: true if the output colorspace is
   *   COLORSPACE_SRGB_LINEAR and the renderer is showing on a display with
   *   HDR enabled. This property can change dynamically when
   *   EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `prop::Renderer.SDR_WHITE_POINT_FLOAT`: the value of SDR white in the
   *   COLORSPACE_SRGB_LINEAR colorspace. When HDR is enabled, this value is
   *   automatically multiplied into the color scale. This property can change
   *   dynamically when EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `prop::Renderer.HDR_HEADROOM_FLOAT`: the additional high dynamic range
   *   that can be displayed, in terms of the SDR white point. When HDR is not
   *   enabled, this will be 1.0. This property can change dynamically when
   *   EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   *
   * With the direct3d renderer:
   *
   * - `prop::Renderer.D3D9_DEVICE_POINTER`: the IDirect3DDevice9 associated
   *   with the renderer
   *
   * With the direct3d11 renderer:
   *
   * - `prop::Renderer.D3D11_DEVICE_POINTER`: the ID3D11Device associated
   *   with the renderer
   * - `prop::Renderer.D3D11_SWAPCHAIN_POINTER`: the IDXGISwapChain1
   *   associated with the renderer. This may change when the window is resized.
   *
   * With the direct3d12 renderer:
   *
   * - `prop::Renderer.D3D12_DEVICE_POINTER`: the ID3D12Device associated
   *   with the renderer
   * - `prop::Renderer.D3D12_SWAPCHAIN_POINTER`: the IDXGISwapChain4
   *   associated with the renderer.
   * - `prop::Renderer.D3D12_COMMAND_QUEUE_POINTER`: the ID3D12CommandQueue
   *   associated with the renderer
   *
   * With the vulkan renderer:
   *
   * - `prop::Renderer.VULKAN_INSTANCE_POINTER`: the VkInstance associated
   *   with the renderer
   * - `prop::Renderer.VULKAN_SURFACE_NUMBER`: the VkSurfaceKHR associated
   *   with the renderer
   * - `prop::Renderer.VULKAN_PHYSICAL_DEVICE_POINTER`: the VkPhysicalDevice
   *   associated with the renderer
   * - `prop::Renderer.VULKAN_DEVICE_POINTER`: the VkDevice associated with
   *   the renderer
   * - `prop::Renderer.VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER`: the queue
   *   family index used for rendering
   * - `prop::Renderer.VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER`: the queue
   *   family index used for presentation
   * - `prop::Renderer.VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER`: the number of
   *   swapchain images, or potential frames in flight, used by the Vulkan
   *   renderer
   *
   * With the gpu renderer:
   *
   * - `prop::Renderer.GPU_DEVICE_POINTER`: the SDL_GPUDevice associated with
   *   the renderer
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const;

  /**
   * Set target texture back to window
   *
   * This is equivalent to SetTarget(nullptr)
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetTarget(nullptr)
   * @sa GetTarget()
   */
  void ResetTarget() { SetTarget(nullptr); }

  /**
   * Set a texture as the current rendering target.
   *
   * The default render target is the window for which the renderer was created.
   * To stop rendering to a texture and render to the window again, call this
   * function with a nullptr `texture`.
   *
   * Viewport, cliprect, scale, and logical presentation are unique to each
   * render target. Get and set functions for these states apply to the current
   * render target set by this function, and those states persist on each target
   * when the current render target changes.
   *
   * @param texture the targeted texture, which must be created with the
   *                `TEXTUREACCESS_TARGET` flag, or nullptr to render to the
   *                window instead of a texture.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetTarget
   */
  void SetTarget(TextureParam texture);

  /**
   * Get the current render target.
   *
   * The default render target is the window for which the renderer was created,
   * and is reported a nullptr here.
   *
   * @returns the current render target or nullptr for the default render
   * target.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetTarget
   */
  Texture GetTarget() const;

  /**
   * Set a device-independent resolution and presentation mode for rendering.
   *
   * This function sets the width and height of the logical rendering output.
   * The renderer will act as if the current render target is always the
   * requested dimensions, scaling to the actual resolution as necessary.
   *
   * This can be useful for games that expect a fixed size, but would like to
   * scale the output to whatever is available, regardless of how a user resizes
   * a window, or if the display is high DPI.
   *
   * Logical presentation can be used with both render target textures and the
   * renderer's window; the state is unique to each render target, and this
   * function sets the state for the current render target. It might be useful
   * to draw to a texture that matches the window dimensions with logical
   * presentation enabled, and then draw that texture across the entire window
   * with logical presentation disabled. Be careful not to render both with
   * logical presentation enabled, however, as this could produce
   * double-letterboxing, etc.
   *
   * You can disable logical coordinates by setting the mode to
   * LOGICAL_PRESENTATION_DISABLED, and in that case you get the full pixel
   * resolution of the render target; it is safe to toggle logical presentation
   * during the rendering of a frame: perhaps most of the rendering is done to
   * specific dimensions but to make fonts look sharp, the app turns off logical
   * presentation while drawing text, for example.
   *
   * For the renderer's window, letterboxing is drawn into the framebuffer if
   * logical presentation is enabled during Renderer.Present; be sure to
   * reenable it before presenting if you were toggling it, otherwise the
   * letterbox areas might have artifacts from previous frames (or artifacts
   * from external overlays, etc). Letterboxing is never drawn into texture
   * render targets; be sure to call Renderer.RenderClear() before drawing into
   * the texture so the letterboxing areas are cleared, if appropriate.
   *
   * You can convert coordinates in an event into rendering coordinates using
   * Renderer.ConvertEventToRenderCoordinates().
   *
   * @param size the width and height of the logical resolution.
   * @param mode the presentation mode used.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.ConvertEventToRenderCoordinates
   * @sa Renderer.GetLogicalPresentation
   * @sa Renderer.GetLogicalPresentationRect
   */
  void SetLogicalPresentation(const PointRaw& size,
                              RendererLogicalPresentation mode);

  /**
   * Get device independent resolution and presentation mode for rendering.
   *
   * This function gets the width and height of the logical rendering output, or
   * the output size in pixels if a logical resolution is not enabled.
   *
   * @param size a Point to be filled with the width and height.
   * @param mode the presentation mode used.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetLogicalPresentation
   */
  void GetLogicalPresentation(PointRaw* size, RendererLogicalPresentation* mode)
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
   * Each render target has its own logical presentation state. This function
   * gets the state for the current render target.
   *
   * @param w an int to be filled with the width.
   * @param h an int to be filled with the height.
   * @param mode the presentation mode used.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetLogicalPresentation
   */
  void GetLogicalPresentation(int* w,
                              int* h,
                              RendererLogicalPresentation* mode) const;

  /**
   * Get the final presentation rectangle for rendering.
   *
   * This function returns the calculated rectangle used for logical
   * presentation, based on the presentation mode and output size. If logical
   * presentation is disabled, it will fill the rectangle with the output size,
   * in pixels.
   *
   * Each render target has its own logical presentation state. This function
   * gets the rectangle for the current render target.
   *
   * @returns the final presentation rectangle.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetLogicalPresentation
   */
  FRect GetLogicalPresentationRect() const;

  /**
   * Get a point in render coordinates when given a point in window coordinates.
   *
   * This takes into account several states:
   *
   * - The window dimensions.
   * - The logical presentation settings (Renderer.SetLogicalPresentation)
   * - The scale (Renderer.SetScale)
   * - The viewport (Renderer.SetViewport)
   *
   * @param window_coord the x, y coordinate in window coordinates.
   * @returns a FPoint containing ther render coordinates on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetLogicalPresentation
   * @sa Renderer.SetScale
   */
  FPoint RenderCoordinatesFromWindow(const FPointRaw& window_coord) const;

  /**
   * Get a point in window coordinates when given a point in render coordinates.
   *
   * This takes into account several states:
   *
   * - The window dimensions.
   * - The logical presentation settings (RendererRef.SetLogicalPresentation)
   * - The scale (RendererRef.SetScale)
   * - The viewport (RendererRef.SetViewport)
   *
   * @param coord the x, y coordinate in render coordinates.
   * @returns a FPoint filled with window coordinates on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetLogicalPresentation
   * @sa Renderer.SetScale
   * @sa Renderer.SetViewport
   */
  FPoint RenderCoordinatesToWindow(const FPointRaw& coord) const;

  /**
   * Convert the coordinates in an event to render coordinates.
   *
   * This takes into account several states:
   *
   * - The window dimensions.
   * - The logical presentation settings (Renderer.SetLogicalPresentation)
   * - The scale (Renderer.SetScale)
   * - The viewport (Renderer.SetViewport)
   *
   * Various event types are converted with this function: mouse, touch, pen,
   * etc.
   *
   * Touch coordinates are converted from normalized coordinates in the window
   * to non-normalized rendering coordinates.
   *
   * Relative mouse coordinates (xrel and yrel event fields) are _also_
   * converted. Applications that do not want these fields converted should use
   * Renderer.RenderCoordinatesFromWindow() on the specific event fields instead
   * of converting the entire event structure.
   *
   * Once converted, coordinates may be outside the rendering area.
   *
   * @param event the event to modify.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderCoordinatesFromWindow
   */
  void ConvertEventToRenderCoordinates(Event* event) const;

  /**
   * Reset the drawing area for rendering to the entire target
   *
   * This is equivalent to `SetViewport(std::nullopt)`
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetViewport()
   * @sa SetViewport()
   * @sa ViewportSet()
   */
  void ResetViewport() { SetViewport(std::nullopt); }

  /**
   * Set the drawing area for rendering on the current target.
   *
   * Drawing will clip to this area (separately from any clipping done with
   * Renderer.SetClipRect), and the top left of the area will become coordinate
   * (0, 0) for future drawing commands.
   *
   * The area's width and height must be >= 0.
   *
   * Each render target has its own viewport. This function sets the viewport
   * for the current render target.
   *
   * @param rect the Rect structure representing the drawing area, or nullptr
   *             to set the viewport to the entire target.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetViewport
   * @sa Renderer.IsViewportSet
   */
  void SetViewport(OptionalRef<const RectRaw> rect);

  /**
   * Get the drawing area for the current target.
   *
   * Each render target has its own viewport. This function gets the viewport
   * for the current render target.
   *
   * @returns an Rect with the current drawing area.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.IsViewportSet
   * @sa Renderer.SetViewport
   */
  Rect GetViewport() const;

  /**
   * Return whether an explicit rectangle was set as the viewport.
   *
   * This is useful if you're saving and restoring the viewport and want to know
   * whether you should restore a specific rectangle or nullptr.
   *
   * Each render target has its own viewport. This function checks the viewport
   * for the current render target.
   *
   * @returns true if the viewport was set to a specific rectangle, or false if
   *          it was set to nullptr (the entire target).
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetViewport
   * @sa Renderer.SetViewport
   */
  bool IsViewportSet() const;

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
   * @returns the rect filled the area that is safe for interactive content.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Rect GetSafeArea() const;

  /**
   * Reset the clip rectangle for rendering to the entire render target
   *
   * This is equivalent to `SetClipRect(std::nullopt)`
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetClipRect()
   * @sa SetClipRect()
   * @sa ClipEnabled()
   */
  void ResetClipRect() { SetClipRect({}); }

  /**
   * Set the clip rectangle for rendering on the specified target.
   *
   * Each render target has its own clip rectangle. This function sets the
   * cliprect for the current render target.
   *
   * @param rect an Rect structure representing the clip area, relative to
   *             the viewport, or nullptr to disable clipping.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetClipRect
   * @sa Renderer.ResetClipRect()
   * @sa Renderer.IsClipEnabled
   */
  void SetClipRect(OptionalRef<const RectRaw> rect);

  /**
   * Get the clip rectangle for the current target.
   *
   * Each render target has its own clip rectangle. This function gets the
   * cliprect for the current render target.
   *
   * @returns a Rect structure filled in with the current clipping area or an
   *          empty rectangle if clipping is disabled.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.IsClipEnabled
   * @sa Renderer.SetClipRect
   */
  Rect GetClipRect() const;

  /**
   * Get whether clipping is enabled on the given render target.
   *
   * Each render target has its own clip rectangle. This function checks the
   * cliprect for the current render target.
   *
   * @returns true if clipping is enabled or false if not; call GetError()
   *          for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetClipRect
   * @sa Renderer.SetClipRect
   */
  bool IsClipEnabled() const;

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
   * Each render target has its own scale. This function sets the scale for the
   * current render target.
   *
   * @param scale the x, y scaling factors.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetScale
   */
  void SetScale(const FPointRaw& scale);

  /**
   * Get the drawing scale for the current target.
   *
   * Each render target has its own scale. This function gets the scale for the
   * current render target.
   *
   * @returns the scaling factors on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetScale
   */
  FPoint GetScale() const
  {
    FPoint p;
    GetScale(&p.x, &p.y);
    return p;
  }

  /**
   * Get the drawing scale for the current target.
   *
   * Each render target has its own scale. This function gets the scale for the
   * current render target.
   *
   * @param scaleX a pointer filled in with the horizontal scaling factor.
   * @param scaleY a pointer filled in with the vertical scaling factor.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetScale
   */
  void GetScale(float* scaleX, float* scaleY) const;

  /**
   * Set the color used for drawing operations.
   *
   * Set the color for drawing or filling rectangles, lines, and points, and for
   * Renderer.RenderClear().
   *
   * @param c the color value used to draw on the rendering target.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetDrawColor
   * @sa Renderer.SetDrawColorFloat
   */
  void SetDrawColor(ColorRaw c);

  /**
   * Set the color used for drawing operations (Rect, Line and Clear).
   *
   * Set the color for drawing or filling rectangles, lines, and points, and for
   * Renderer.RenderClear().
   *
   * @param c the RGBA values used to draw on the rendering target.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetDrawColorFloat
   * @sa Renderer.SetDrawColor
   */
  void SetDrawColorFloat(const FColorRaw& c);

  /**
   * Get the color used for drawing operations (Rect, Line and Clear).
   *
   * @returns the color channel values used to draw on the rendering target.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColor(SDL_FColor*)
   * @sa SetDrawColor()
   */
  Color GetDrawColor() const
  {
    Color c;
    GetDrawColor(&c.r, &c.g, &c.b, &c.a);
    return c;
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
   *          rendering target; usually `ALPHA_OPAQUE` (255).
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetDrawColorFloat
   * @sa Renderer.SetDrawColor
   */
  void GetDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a) const;

  /**
   * Get the color used for drawing operations (Rect, Line and Clear).
   *
   * @returns the color on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColor(SDL_FColor*)
   * @sa SetDrawColor()
   */
  FColor GetDrawColorFloat() const
  {
    FColor c;
    GetDrawColorFloat(&c.r, &c.g, &c.b, &c.a);
    return c;
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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetDrawColorFloat
   * @sa Renderer.GetDrawColor
   */
  void GetDrawColorFloat(float* r, float* g, float* b, float* a) const;

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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetColorScale
   */
  void SetColorScale(float scale);

  /**
   * Get the color scale used for render operations.
   *
   * @returns a float representing the current color scale on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetColorScale
   */
  float GetColorScale() const;

  /**
   * Set the blend mode used for drawing operations (Fill and Line).
   *
   * If the blend mode is not supported, the closest supported mode is chosen.
   *
   * @param blendMode the BlendMode to use for blending.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetDrawBlendMode
   */
  void SetDrawBlendMode(BlendMode blendMode);

  /**
   * Get the blend mode used for drawing operations.
   *
   * @returns the current BlendMode on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetDrawBlendMode
   */
  BlendMode GetDrawBlendMode() const;

  /**
   * Clear the current rendering target with the drawing color.
   *
   * This function clears the entire rendering target, ignoring the viewport and
   * the clip rectangle. Note, that clearing will also set/fill all pixels of
   * the rendering target to current renderer draw color, so make sure to invoke
   * Renderer.SetDrawColor() when needed.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetDrawColor
   */
  void RenderClear();

  /**
   * Draw a point on the current rendering target at subpixel precision.
   *
   * @param p the x, y coordinates of the point.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderPoints
   */
  void RenderPoint(const FPointRaw& p);

  /**
   * Draw multiple points on the current rendering target at subpixel precision.
   *
   * @param points the points to draw.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderPoint
   */
  void RenderPoints(SpanRef<const FPointRaw> points);

  /**
   * Draw a line on the current rendering target at subpixel precision.
   *
   * @param p1 the x,y coordinate of the start point.
   * @param p2 the x,y coordinate of the end point.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderLines
   */
  void RenderLine(const FPointRaw& p1, const FPointRaw& p2);

  /**
   * Draw a series of connected lines on the current rendering target at
   * subpixel precision.
   *
   * @param points the points along the lines.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderLine
   */
  void RenderLines(SpanRef<const FPointRaw> points);

  /**
   * Draw a rectangle on the current rendering target at subpixel precision.
   *
   * @param rect a pointer to the destination rectangle, or std::nullopt to
   *             outline the entire rendering target.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderRects
   */
  void RenderRect(OptionalRef<const FRectRaw> rect);

  /**
   * Draw some number of rectangles on the current rendering target at subpixel
   * precision.
   *
   * @param rects a pointer to an array of destination rectangles.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderRect
   */
  void RenderRects(SpanRef<const FRectRaw> rects);

  /**
   * Fill a rectangle on the current rendering target with the drawing color at
   * subpixel precision.
   *
   * @param rect a pointer to the destination rectangle, or std::nullopt for the
   *             entire rendering target.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderFillRects
   */
  void RenderFillRect(OptionalRef<const FRectRaw> rect);

  /**
   * Fill some number of rectangles on the current rendering target with the
   * drawing color at subpixel precision.
   *
   * @param rects a pointer to an array of destination rectangles.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderFillRect
   */
  void RenderFillRects(SpanRef<const FRectRaw> rects);

  /**
   * Copy a portion of the texture to the current rendering target at subpixel
   * precision.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or nullptr for the entire
   *                texture.
   * @param dstrect a pointer to the destination rectangle, or nullptr for the
   *                entire rendering target.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderTextureRotated
   * @sa Renderer.RenderTextureTiled
   */
  void RenderTexture(TextureParam texture,
                     OptionalRef<const FRectRaw> srcrect,
                     OptionalRef<const FRectRaw> dstrect);

  /**
   * Copy a portion of the source texture to the current rendering target, with
   * rotation and flipping, at subpixel precision.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or nullptr for the entire
   *                texture.
   * @param dstrect a pointer to the destination rectangle, or nullptr for the
   *                entire rendering target.
   * @param angle an angle in degrees that indicates the rotation that will be
   *              applied to dstrect, rotating it in a clockwise direction.
   * @param center a pointer to a point indicating the point around which
   *               dstrect will be rotated (if nullptr, rotation will be done
   *               around dstrect.w/2, dstrect.h/2).
   * @param flip an FlipMode value stating which flipping actions should be
   *             performed on the texture.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderTexture
   */
  void RenderTextureRotated(TextureParam texture,
                            OptionalRef<const FRectRaw> srcrect,
                            OptionalRef<const FRectRaw> dstrect,
                            double angle,
                            OptionalRef<const FPointRaw> center,
                            FlipMode flip = FlipMode::SDL_FLIP_NONE);

  /**
   * Copy a portion of the source texture to the current rendering target, with
   * affine transform, at subpixel precision.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or nullptr for the entire
   *                texture.
   * @param origin a pointer to a point indicating where the top-left corner of
   *               srcrect should be mapped to, or nullptr for the rendering
   *               target's origin.
   * @param right a pointer to a point indicating where the top-right corner of
   *              srcrect should be mapped to, or nullptr for the rendering
   *              target's top-right corner.
   * @param down a pointer to a point indicating where the bottom-left corner of
   *             srcrect should be mapped to, or nullptr for the rendering
   * target's bottom-left corner.
   * @throws Error on failure.
   *
   * @threadsafety You may only call this function from the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderTexture
   */
  void RenderTextureAffine(TextureParam texture,
                           OptionalRef<const FRectRaw> srcrect,
                           OptionalRef<const FPointRaw> origin,
                           OptionalRef<const FPointRaw> right,
                           OptionalRef<const FPointRaw> down);

  /**
   * Tile a portion of the texture to the current rendering target at subpixel
   * precision.
   *
   * The pixels in `srcrect` will be repeated as many times as needed to
   * completely fill `dstrect`.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or nullptr for the entire
   *                texture.
   * @param scale the scale used to transform srcrect into the destination
   *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
   *              64x64 tiles.
   * @param dstrect a pointer to the destination rectangle, or nullptr for the
   *                entire rendering target.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderTexture
   */
  void RenderTextureTiled(TextureParam texture,
                          OptionalRef<const FRectRaw> srcrect,
                          float scale,
                          OptionalRef<const FRectRaw> dstrect);

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
   * @param srcrect the Rect structure representing the rectangle to be used
   *                for the 9-grid, or nullptr to use the entire texture.
   * @param left_width the width, in pixels, of the left corners in `srcrect`.
   * @param right_width the width, in pixels, of the right corners in `srcrect`.
   * @param top_height the height, in pixels, of the top corners in `srcrect`.
   * @param bottom_height the height, in pixels, of the bottom corners in
   *                      `srcrect`.
   * @param scale the scale used to transform the corner of `srcrect` into the
   *              corner of `dstrect`, or 0.0f for an unscaled copy.
   * @param dstrect a pointer to the destination rectangle, or nullptr for the
   *                entire rendering target.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderTexture
   */
  void RenderTexture9Grid(TextureParam texture,
                          OptionalRef<const FRectRaw> srcrect,
                          float left_width,
                          float right_width,
                          float top_height,
                          float bottom_height,
                          float scale,
                          OptionalRef<const FRectRaw> dstrect);

  /**
   * Render a list of triangles, optionally using a texture and indices into the
   * vertex array Color and alpha modulation is done per vertex
   * (Texture.SetColorMod and Texture.SetAlphaMod are ignored).
   *
   * @param texture (optional) The SDL texture to use.
   * @param vertices vertices.
   * @param indices (optional) An array of integer indices into the 'vertices'
   *                array, if nullptr all vertices will be rendered in
   *                sequential order.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderGeometryRaw
   */
  void RenderGeometry(TextureParam texture,
                      std::span<const Vertex> vertices,
                      std::span<const int> indices = {});

  /**
   * Render a list of triangles, optionally using a texture and indices into the
   * vertex arrays Color and alpha modulation is done per vertex
   * (Texture.SetColorMod and Texture.SetAlphaMod are ignored).
   *
   * @param texture (optional) The SDL texture to use.
   * @param xy vertex positions.
   * @param xy_stride byte size to move from one element to the next element.
   * @param color vertex colors (as FColor).
   * @param color_stride byte size to move from one element to the next element.
   * @param uv vertex normalized texture coordinates.
   * @param uv_stride byte size to move from one element to the next element.
   * @param num_vertices number of vertices.
   * @param indices (optional) An array of indices into the 'vertices' arrays,
   *                if nullptr all vertices will be rendered in sequential
   * order.
   * @param num_indices number of indices.
   * @param size_indices index size: 1 (byte), 2 (short), 4 (int).
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderGeometry
   */
  void RenderGeometryRaw(TextureParam texture,
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
   * The returned surface contains pixels inside the desired area clipped to the
   * current viewport, and should be freed with Surface.Destroy().
   *
   * Note that this returns the actual pixels on the screen, so if you are using
   * logical presentation you should use Renderer.GetLogicalPresentationRect()
   * to get the area containing your content.
   *
   * **WARNING**: This is a very slow operation, and should not be used
   * frequently. If you're using this on the main rendering target, it should be
   * called after rendering and before Renderer.Present().
   *
   * @param rect an Rect structure representing the area to read, which will
   *             be clipped to the current viewport, or nullptr for the entire
   *             viewport.
   * @returns a new Surface on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Surface ReadPixels(OptionalRef<const RectRaw> rect = {}) const;

  /**
   * Update the screen with any rendering performed since the previous call.
   *
   * SDL's rendering functions operate on a backbuffer; that is, calling a
   * rendering function such as Renderer.RenderLine() does not directly put a
   * line on the screen, but rather updates the backbuffer. As such, you compose
   * your entire scene and *present* the composed backbuffer to the screen as a
   * complete picture.
   *
   * Therefore, when using SDL's rendering API, one does all drawing intended
   * for the frame, and then calls this function once per frame to present the
   * final drawing to the user.
   *
   * The backbuffer should be considered invalidated after each present; do not
   * assume that previous contents will exist between frames. You are strongly
   * encouraged to call Renderer.RenderClear() to initialize the backbuffer
   * before starting each new frame's drawing, even if you plan to overwrite
   * every pixel.
   *
   * Please note, that in case of rendering to a texture - there is **no need**
   * to call `Renderer.Present` after drawing needed objects to a texture, and
   * should not be done; you are only required to change back the rendering
   * target to default via `Renderer.SetTarget(renderer, nullptr)` afterwards,
   * as textures by themselves do not have a concept of backbuffers. Calling
   * Renderer.Present while rendering to a texture will still update the screen
   * with any current drawing that has been done _to the window itself_.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.Renderer
   * @sa Renderer.RenderClear
   * @sa Renderer.RenderFillRect
   * @sa Renderer.RenderFillRects
   * @sa Renderer.RenderLine
   * @sa Renderer.RenderLines
   * @sa Renderer.RenderPoint
   * @sa Renderer.RenderPoints
   * @sa Renderer.RenderRect
   * @sa Renderer.RenderRects
   * @sa Renderer.SetDrawBlendMode
   * @sa Renderer.SetDrawColor
   */
  void Present();

  /**
   * Force the rendering context to flush any pending commands and state.
   *
   * You do not need to (and in fact, shouldn't) call this function unless you
   * are planning to call into OpenGL/Direct3D/Metal/whatever directly, in
   * addition to using an Renderer.
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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Flush();

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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetVSync
   */
  void SetVSync(int vsync);

  /**
   * Get VSync of the given renderer.
   *
   * @returns the current vertical refresh sync interval on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.SetVSync
   */
  int GetVSync() const;

  /**
   * Draw debug text to an Renderer.
   *
   * This function will render a string of text to an Renderer. Note that
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
   * The text is drawn in the color specified by Renderer.SetDrawColor().
   *
   * @param p the x, y coordinates where the top-left corner of the text will
   *          draw.
   * @param str the string to render.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderDebugTextFormat
   * @sa SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE
   */
  void RenderDebugText(FPoint p, StringParam str);

  /**
   * Draw debug text to an Renderer.
   *
   * This function will render a std::format()-style format string to a
   * renderer. Note that this is a convenience function for debugging, with
   * severe limitations, and is not intended to be used for production apps and
   * games.
   *
   * For the full list of limitations and other useful information, see
   * Renderer.RenderDebugText.
   *
   * @param p the x, y coordinates where the top-left corner of the text will
   *          draw.
   * @param fmt the format string to draw.
   * @param args additional parameters matching {} tokens in the `fmt` string,
   *             if any.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.RenderDebugText
   * @sa SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE
   */
  template<class... ARGS>
  void RenderDebugTextFormat(FPoint p, std::string_view fmt, ARGS... args)
  {
    RenderDebugText(p, std::vformat(fmt, std::make_format_args(args...)));
  }

  /**
   * Create a texture for a rendering context.
   *
   * The contents of a texture when first created are not defined.
   *
   * @param format one of the enumerated values in PixelFormat.
   * @param access one of the enumerated values in TextureAccess.
   * @param size the width and height of the texture in pixels.
   * @returns the created texture or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.Texture
   * @sa Texture.Texture
   * @sa Texture.Destroy
   * @sa Texture.GetSize
   * @sa Texture.Update
   */
  Texture CreateTexture(PixelFormat format,
                        TextureAccess access,
                        const PointRaw& size);

  /**
   * Create a texture from an existing surface.
   *
   * The surface is not modified or freed by this function.
   *
   * The TextureAccess hint for the created texture is
   * `TEXTUREACCESS_STATIC`.
   *
   * The pixel format of the created texture may be different from the pixel
   * format of the surface, and can be queried using the
   * prop::Texture.FORMAT_NUMBER property.
   *
   * @param surface the Surface structure containing pixel data used to fill
   *                the texture.
   * @returns the created texture or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.Texture
   * @sa Texture.Texture
   * @sa Texture.Destroy
   */
  Texture CreateTextureFromSurface(SurfaceParam surface);

  /**
   * Create a texture for a rendering context with the specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::Texture.CREATE_COLORSPACE_NUMBER`: an Colorspace value
   *   describing the texture colorspace, defaults to COLORSPACE_SRGB_LINEAR
   *   for floating point textures, COLORSPACE_HDR10 for 10-bit textures,
   *   COLORSPACE_SRGB for other RGB textures and COLORSPACE_JPEG for
   *   YUV textures.
   * - `prop::Texture.CREATE_FORMAT_NUMBER`: one of the enumerated values in
   *   PixelFormat, defaults to the best RGBA format for the renderer
   * - `prop::Texture.CREATE_ACCESS_NUMBER`: one of the enumerated values in
   *   TextureAccess, defaults to TEXTUREACCESS_STATIC
   * - `prop::Texture.CREATE_WIDTH_NUMBER`: the width of the texture in
   *   pixels, required
   * - `prop::Texture.CREATE_HEIGHT_NUMBER`: the height of the texture in
   *   pixels, required
   * - `prop::Texture.CREATE_SDR_WHITE_POINT_FLOAT`: for HDR10 and floating
   *   point textures, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 100 for HDR10 textures and 1.0 for floating point textures.
   * - `prop::Texture.CREATE_HDR_HEADROOM_FLOAT`: for HDR10 and floating
   *   point textures, this defines the maximum dynamic range used by the
   *   content, in terms of the SDR white point. This would be equivalent to
   *   maxCLL / prop::Texture.CREATE_SDR_WHITE_POINT_FLOAT for HDR10 content.
   *   If this is defined, any values outside the range supported by the display
   *   will be scaled into the available HDR headroom, otherwise they are
   *   clipped.
   *
   * With the direct3d11 renderer:
   *
   * - `prop::Texture.CREATE_D3D11_TEXTURE_POINTER`: the ID3D11Texture2D
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_D3D11_TEXTURE_U_POINTER`: the ID3D11Texture2D
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_D3D11_TEXTURE_V_POINTER`: the ID3D11Texture2D
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the direct3d12 renderer:
   *
   * - `prop::Texture.CREATE_D3D12_TEXTURE_POINTER`: the ID3D12Resource
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_D3D12_TEXTURE_U_POINTER`: the ID3D12Resource
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_D3D12_TEXTURE_V_POINTER`: the ID3D12Resource
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the metal renderer:
   *
   * - `prop::Texture.CREATE_METAL_PIXELBUFFER_POINTER`: the CVPixelBufferRef
   *   associated with the texture, if you want to create a texture from an
   *   existing pixel buffer.
   *
   * With the opengl renderer:
   *
   * - `prop::Texture.CREATE_OPENGL_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_OPENGL_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_OPENGL_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_OPENGL_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the opengles2 renderer:
   *
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the vulkan renderer:
   *
   * - `prop::Texture.CREATE_VULKAN_TEXTURE_NUMBER`: the VkImage with layout
   *   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL associated with the texture, if
   *   you want to wrap an existing texture.
   *
   * @param props the properties to use.
   * @returns the created texture or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Create
   * @sa Texture.Texture
   * @sa Texture.Texture
   * @sa Texture.Destroy
   * @sa Texture.GetSize
   * @sa Texture.Update
   */
  Texture CreateTextureWithProperties(PropertiesParam props);

  /**
   * Get the CAMetalLayer associated with the given Metal renderer.
   *
   * This function returns `void *`, so SDL doesn't have to include Metal's
   * headers, but it can be safely cast to a `CAMetalLayer *`.
   *
   * @returns a `CAMetalLayer *` on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetRenderMetalCommandEncoder
   */
  void* GetRenderMetalLayer();

  /**
   * Get the Metal command encoder for the current frame.
   *
   * This function returns `void *`, so SDL doesn't have to include Metal's
   * headers, but it can be safely cast to an `id<MTLRenderCommandEncoder>`.
   *
   * This will return nullptr if Metal refuses to give SDL a drawable to render
   * to, which might happen if the window is hidden/minimized/offscreen. This
   * doesn't apply to command encoders for render targets, just the window's
   * backbuffer. Check your return values!
   *
   * @returns an `id<MTLRenderCommandEncoder>` on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetRenderMetalLayer
   */
  void* GetRenderMetalCommandEncoder();

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
   * prop::Renderer.VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER will give you the
   * maximum number of semaphores you'll need.
   *
   * @param wait_stage_mask the VkPipelineStageFlags for the wait.
   * @param wait_semaphore a VkSempahore to wait on before rendering the current
   *                       frame, or 0 if not needed.
   * @param signal_semaphore a VkSempahore that SDL will signal when rendering
   *                         for the current frame is complete, or 0 if not
   *                         needed.
   * @throws Error on failure.
   *
   * @threadsafety It is **NOT** safe to call this function from two threads at
   *               once.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void AddVulkanRenderSemaphores(Uint32 wait_stage_mask,
                                 Sint64 wait_semaphore,
                                 Sint64 signal_semaphore);
};

/// Semi-safe reference for Renderer.
struct RendererRef : Renderer
{
  /**
   * Constructs from RendererParam.
   *
   * @param resource a RendererRaw or Renderer.
   *
   * This does not takes ownership!
   */
  RendererRef(RendererParam resource)
    : Renderer(resource.value)
  {
  }

  /// Copy constructor.
  RendererRef(const RendererRef& other)
    : Renderer(other.get())
  {
  }

  /// Destructor
  ~RendererRef() { release(); }
};

/**
 * An efficient driver-specific representation of pixel data
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Texture.Texture
 * @sa Texture.Texture
 * @sa Texture.Texture
 * @sa Texture.Destroy
 *
 * @cat resource
 */
class Texture
{
  TextureRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Texture() = default;

  /**
   * Constructs from TextureParam.
   *
   * @param resource a TextureRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Texture(const TextureRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Texture(const Texture& other) { ++m_resource->refcount; }

  /// Move constructor
  constexpr Texture(Texture&& other)
    : Texture(other.release())
  {
  }

  /**
   * Create a texture for a rendering context.
   *
   * The contents of a texture when first created are not defined.
   *
   * @param renderer the rendering context.
   * @param format one of the enumerated values in PixelFormat.
   * @param access one of the enumerated values in TextureAccess.
   * @param size the width and height of the texture in pixels.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.Texture
   * @sa Texture.Texture
   * @sa Texture.Destroy
   * @sa Texture.GetSize
   * @sa Texture.Update
   */
  Texture(RendererParam renderer,
          PixelFormat format,
          TextureAccess access,
          const PointRaw& size)
    : m_resource(
        CheckError(SDL_CreateTexture(renderer, format, access, size.x, size.y)))
  {
  }

  /**
   * Create a texture from an existing surface.
   *
   * The surface is not modified or freed by this function.
   *
   * The TextureAccess hint for the created texture is
   * `TEXTUREACCESS_STATIC`.
   *
   * The pixel format of the created texture may be different from the pixel
   * format of the surface, and can be queried using the
   * prop::Texture.FORMAT_NUMBER property.
   *
   * @param renderer the rendering context.
   * @param surface the Surface structure containing pixel data used to fill
   *                the texture.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.Texture
   * @sa Texture.Destroy
   */
  Texture(RendererParam renderer, SurfaceParam surface)
    : m_resource(CheckError(SDL_CreateTextureFromSurface(renderer, surface)))
  {
  }

  /**
   * Create a texture for a rendering context with the specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::Texture.CREATE_COLORSPACE_NUMBER`: an Colorspace value
   *   describing the texture colorspace, defaults to COLORSPACE_SRGB_LINEAR
   *   for floating point textures, COLORSPACE_HDR10 for 10-bit textures,
   *   COLORSPACE_SRGB for other RGB textures and COLORSPACE_JPEG for
   *   YUV textures.
   * - `prop::Texture.CREATE_FORMAT_NUMBER`: one of the enumerated values in
   *   PixelFormat, defaults to the best RGBA format for the renderer
   * - `prop::Texture.CREATE_ACCESS_NUMBER`: one of the enumerated values in
   *   TextureAccess, defaults to TEXTUREACCESS_STATIC
   * - `prop::Texture.CREATE_WIDTH_NUMBER`: the width of the texture in
   *   pixels, required
   * - `prop::Texture.CREATE_HEIGHT_NUMBER`: the height of the texture in
   *   pixels, required
   * - `prop::Texture.CREATE_SDR_WHITE_POINT_FLOAT`: for HDR10 and floating
   *   point textures, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 100 for HDR10 textures and 1.0 for floating point textures.
   * - `prop::Texture.CREATE_HDR_HEADROOM_FLOAT`: for HDR10 and floating
   *   point textures, this defines the maximum dynamic range used by the
   *   content, in terms of the SDR white point. This would be equivalent to
   *   maxCLL / prop::Texture.CREATE_SDR_WHITE_POINT_FLOAT for HDR10 content.
   *   If this is defined, any values outside the range supported by the display
   *   will be scaled into the available HDR headroom, otherwise they are
   *   clipped.
   *
   * With the direct3d11 renderer:
   *
   * - `prop::Texture.CREATE_D3D11_TEXTURE_POINTER`: the ID3D11Texture2D
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_D3D11_TEXTURE_U_POINTER`: the ID3D11Texture2D
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_D3D11_TEXTURE_V_POINTER`: the ID3D11Texture2D
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the direct3d12 renderer:
   *
   * - `prop::Texture.CREATE_D3D12_TEXTURE_POINTER`: the ID3D12Resource
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_D3D12_TEXTURE_U_POINTER`: the ID3D12Resource
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_D3D12_TEXTURE_V_POINTER`: the ID3D12Resource
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the metal renderer:
   *
   * - `prop::Texture.CREATE_METAL_PIXELBUFFER_POINTER`: the CVPixelBufferRef
   *   associated with the texture, if you want to create a texture from an
   *   existing pixel buffer.
   *
   * With the opengl renderer:
   *
   * - `prop::Texture.CREATE_OPENGL_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_OPENGL_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_OPENGL_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_OPENGL_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the opengles2 renderer:
   *
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the vulkan renderer:
   *
   * - `prop::Texture.CREATE_VULKAN_TEXTURE_NUMBER`: the VkImage with layout
   *   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL associated with the texture, if
   *   you want to wrap an existing texture.
   *
   * @param renderer the rendering context.
   * @param props the properties to use.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Create
   * @sa Texture.Texture
   * @sa Texture.Texture
   * @sa Texture.Destroy
   * @sa Texture.GetSize
   * @sa Texture.Update
   */
  Texture(RendererParam renderer, PropertiesParam props)
    : m_resource(CheckError(SDL_CreateTextureWithProperties(renderer, props)))
  {
  }

  /**
   * Load an image from a filesystem path into a GPU texture.
   *
   * An Texture represents an image in GPU memory, usable by SDL's 2D Render
   * API. This can be significantly more efficient than using a CPU-bound
   * Surface if you don't need to manipulate the image directly after
   * loading it.
   *
   * If the loaded image has transparency or a colorkey, a texture with an alpha
   * channel will be created. Otherwise, SDL_image will attempt to create an
   * Texture in the most format that most reasonably represents the image
   * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
   * RGBA).
   *
   * There is a separate function to read files from an IOStream, if you
   * need an i/o abstraction to provide data from anywhere instead of a simple
   * filesystem read; that function is Texture.Texture().
   *
   * If you would rather decode an image to an Surface (a buffer of pixels
   * in CPU memory), call Surface.Surface() instead.
   *
   * @param renderer the Renderer to use to create the GPU texture.
   * @param file a path on the filesystem to load an image from.
   * @post a new texture, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa LoadTextureTyped
   * @sa Texture.Texture
   */
  Texture(RendererParam renderer, StringParam file);

  /**
   * Load an image from an SDL data source into a GPU texture.
   *
   * An Texture represents an image in GPU memory, usable by SDL's 2D Render
   * API. This can be significantly more efficient than using a CPU-bound
   * Surface if you don't need to manipulate the image directly after
   * loading it.
   *
   * If the loaded image has transparency or a colorkey, a texture with an alpha
   * channel will be created. Otherwise, SDL_image will attempt to create an
   * Texture in the most format that most reasonably represents the image
   * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
   * RGBA).
   *
   * If `closeio` is true, `src` will be closed before returning, whether this
   * function succeeds or not. SDL_image reads everything it needs from `src`
   * during this call in any case.
   *
   * There is a separate function to read files from disk without having to deal
   * with IOStream: `Texture.Texture(renderer, "filename.jpg")` will call
   * this function and manage those details for you, determining the file type
   * from the filename's extension.
   *
   * There is also LoadTextureTyped(), which is equivalent to this
   * function except a file extension (like "BMP", "JPG", etc) can be specified,
   * in case SDL_image cannot autodetect the file format.
   *
   * If you would rather decode an image to an Surface (a buffer of pixels
   * in CPU memory), call Surface.Surface() instead.
   *
   * @param renderer the Renderer to use to create the GPU texture.
   * @param src an IOStream that data will be read from.
   * @param closeio true to close/free the IOStream before returning, false
   *                to leave it open.
   * @post a new texture, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa Texture.Texture
   * @sa LoadTextureTyped
   * @sa Texture.Destroy
   */
  Texture(RendererParam renderer, IOStreamParam src, bool closeio = false);

  /**
   * Safely borrows the from TextureParam.
   *
   * @param resource a TextureRaw or Texture.
   *
   * This does not takes ownership!
   */
  static constexpr Texture Borrow(TextureParam resource)
  {
    if (resource) {
      ++resource.value->refcount;
      return Texture(resource.value);
    }
    return {};
  }

  /// Destructor
  ~Texture() { SDL_DestroyTexture(m_resource); }

  /// Assignment operator.
  Texture& operator=(Texture other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying TextureRaw.
  constexpr TextureRaw get() const { return m_resource; }

  /// Retrieves underlying TextureRaw and clear this.
  constexpr TextureRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Texture& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to TextureParam
  constexpr operator TextureParam() const { return {m_resource}; }

  /**
   * Destroy the specified texture.
   *
   * Passing nullptr or an otherwise invalid texture will set the SDL error
   * message to "Invalid texture".
   *
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.Texture
   * @sa Texture.Texture
   */
  void Destroy();

  /**
   * Get the properties associated with a texture.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `prop::Texture.COLORSPACE_NUMBER`: an Colorspace value describing
   *   the texture colorspace.
   * - `prop::Texture.FORMAT_NUMBER`: one of the enumerated values in
   *   PixelFormat.
   * - `prop::Texture.ACCESS_NUMBER`: one of the enumerated values in
   *   TextureAccess.
   * - `prop::Texture.WIDTH_NUMBER`: the width of the texture in pixels.
   * - `prop::Texture.HEIGHT_NUMBER`: the height of the texture in pixels.
   * - `prop::Texture.SDR_WHITE_POINT_FLOAT`: for HDR10 and floating point
   *   textures, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 100 for HDR10 textures and 1.0 for other textures.
   * - `prop::Texture.HDR_HEADROOM_FLOAT`: for HDR10 and floating point
   *   textures, this defines the maximum dynamic range used by the content, in
   *   terms of the SDR white point. If this is defined, any values outside the
   *   range supported by the display will be scaled into the available HDR
   *   headroom, otherwise they are clipped. This defaults to 1.0 for SDR
   *   textures, 4.0 for HDR10 textures, and no default for floating point
   *   textures.
   *
   * With the direct3d11 renderer:
   *
   * - `prop::Texture.D3D11_TEXTURE_POINTER`: the ID3D11Texture2D associated
   *   with the texture
   * - `prop::Texture.D3D11_TEXTURE_U_POINTER`: the ID3D11Texture2D
   *   associated with the U plane of a YUV texture
   * - `prop::Texture.D3D11_TEXTURE_V_POINTER`: the ID3D11Texture2D
   *   associated with the V plane of a YUV texture
   *
   * With the direct3d12 renderer:
   *
   * - `prop::Texture.D3D12_TEXTURE_POINTER`: the ID3D12Resource associated
   *   with the texture
   * - `prop::Texture.D3D12_TEXTURE_U_POINTER`: the ID3D12Resource associated
   *   with the U plane of a YUV texture
   * - `prop::Texture.D3D12_TEXTURE_V_POINTER`: the ID3D12Resource associated
   *   with the V plane of a YUV texture
   *
   * With the vulkan renderer:
   *
   * - `prop::Texture.VULKAN_TEXTURE_NUMBER`: the VkImage associated with the
   *   texture
   *
   * With the opengl renderer:
   *
   * - `prop::Texture.OPENGL_TEXTURE_NUMBER`: the GLuint texture associated
   *   with the texture
   * - `prop::Texture.OPENGL_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture
   * - `prop::Texture.OPENGL_TEXTURE_U_NUMBER`: the GLuint texture associated
   *   with the U plane of a YUV texture
   * - `prop::Texture.OPENGL_TEXTURE_V_NUMBER`: the GLuint texture associated
   *   with the V plane of a YUV texture
   * - `prop::Texture.OPENGL_TEXTURE_TARGET_NUMBER`: the GLenum for the
   *   texture target (`GL_TEXTURE_2D`, `GL_TEXTURE_RECTANGLE_ARB`, etc)
   * - `prop::Texture.OPENGL_TEX_W_FLOAT`: the texture coordinate width of
   *   the texture (0.0 - 1.0)
   * - `prop::Texture.OPENGL_TEX_H_FLOAT`: the texture coordinate height of
   *   the texture (0.0 - 1.0)
   *
   * With the opengles2 renderer:
   *
   * - `prop::Texture.OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture
   * - `prop::Texture.OPENGLES2_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture
   * - `prop::Texture.OPENGLES2_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture
   * - `prop::Texture.OPENGLES2_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture
   * - `prop::Texture.OPENGLES2_TEXTURE_TARGET_NUMBER`: the GLenum for the
   *   texture target (`GL_TEXTURE_2D`, `GL_TEXTURE_EXTERNAL_OES`, etc)
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const;

  /**
   * Get the renderer that created an Texture.
   *
   * @returns a pointer to the Renderer that created the texture, or nullptr on
   *          failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  RendererRef GetRenderer() const;

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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetMod(Color c)
  {
    SetColorMod(c.r, c.g, c.b);
    SetAlphaMod(c.a);
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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetModFloat(FColor c)
  {
    SetColorMod(c.r, c.g, c.b);
    SetAlphaMod(c.a);
  }

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @returns the color channels (0-1) on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetAlphaMod()
   * @sa SetColorMod()
   */
  Color GetMod() const
  {
    Color c;
    GetColorMod(&c.r, &c.g, &c.b);
    c.a = GetAlphaMod();
    return c;
  }

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @returns the color channels (0-1) on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetAlphaMod()
   * @sa SetColorMod()
   */
  FColor GetModFloat() const
  {
    FColor c;
    GetColorModFloat(&c.r, &c.g, &c.b);
    c.a = GetAlphaModFloat();
    return c;
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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetColorMod
   * @sa Texture.SetAlphaMod
   * @sa Texture.SetColorModFloat
   */
  void SetColorMod(Uint8 r, Uint8 g, Uint8 b);

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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetColorModFloat
   * @sa Texture.SetAlphaModFloat
   * @sa Texture.SetColorMod
   */
  void SetColorModFloat(float r, float g, float b);

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetAlphaMod
   * @sa Texture.GetColorModFloat
   * @sa Texture.SetColorMod
   */
  void GetColorMod(Uint8* r, Uint8* g, Uint8* b) const;

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetAlphaModFloat
   * @sa Texture.GetColorMod
   * @sa Texture.SetColorModFloat
   */
  void GetColorModFloat(float* r, float* g, float* b) const;

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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetAlphaMod
   * @sa Texture.SetAlphaModFloat
   * @sa Texture.SetColorMod
   */
  void SetAlphaMod(Uint8 alpha);

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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetAlphaModFloat
   * @sa Texture.SetAlphaMod
   * @sa Texture.SetColorModFloat
   */
  void SetAlphaModFloat(float alpha);

  /**
   * Get the additional alpha value multiplied into render copy operations.
   *
   * @returns the current alpha value.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetAlphaModFloat
   * @sa Texture.GetColorMod
   * @sa Texture.SetAlphaMod
   */
  Uint8 GetAlphaMod() const;

  /**
   * Get the additional alpha value multiplied into render copy operations.
   *
   * @returns the current alpha value.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetAlphaMod
   * @sa Texture.GetColorModFloat
   * @sa Texture.SetAlphaModFloat
   */
  float GetAlphaModFloat() const;

  /**
   * Set the blend mode for a texture, used by Renderer.RenderTexture().
   *
   * If the blend mode is not supported, the closest supported mode is chosen
   * and this function returns false.
   *
   * @param blendMode the BlendMode to use for texture blending.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetBlendMode
   */
  void SetBlendMode(BlendMode blendMode);

  /**
   * Get the blend mode used for texture copy operations.
   *
   * @returns the current SDL_BlendMode on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.SetBlendMode
   */
  BlendMode GetBlendMode() const;

  /**
   * Set the scale mode used for texture scale operations.
   *
   * The default texture scale mode is SCALEMODE_LINEAR.
   *
   * If the scale mode is not supported, the closest supported mode is chosen.
   *
   * @param scaleMode the ScaleMode to use for texture scaling.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.GetScaleMode
   */
  void SetScaleMode(ScaleMode scaleMode);

  /**
   * Get the scale mode used for texture scale operations.
   *
   * @returns the current scale mode on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.SetScaleMode
   */
  ScaleMode GetScaleMode() const;

  /**
   * Update the given texture rectangle with new pixel data.
   *
   * The pixel data must be in the pixel format of the texture, which can be
   * queried using the prop::Texture.FORMAT_NUMBER property.
   *
   * This is a fairly slow function, intended for use with static textures that
   * do not change often.
   *
   * If the texture is intended to be updated often, it is preferred to create
   * the texture as streaming and use the locking functions referenced below.
   * While this function will work with streaming textures, for optimization
   * reasons you may not get the pixels back if you lock the texture afterward.
   *
   * @param rect an Rect structure representing the area to update, or nullptr
   *             to update the entire texture.
   * @param pixels the raw pixel data in the format of the texture.
   * @param pitch the number of bytes in a row of pixel data, including padding
   *              between lines.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.Lock
   * @sa Texture.Unlock
   * @sa Texture.UpdateNV
   * @sa Texture.UpdateYUV
   */
  void Update(OptionalRef<const RectRaw> rect, const void* pixels, int pitch);

  /**
   * Update a rectangle within a planar YV12 or IYUV texture with new pixel
   * data.
   *
   * You can use Texture.Update() as long as your pixel data is a contiguous
   * block of Y and U/V planes in the proper order, but this function is
   * available if your pixel data is not contiguous.
   *
   * @param rect a pointer to the rectangle of pixels to update, or nullptr to
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
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.UpdateNV
   * @sa Texture.Update
   */
  void UpdateYUV(OptionalRef<const RectRaw> rect,
                 const Uint8* Yplane,
                 int Ypitch,
                 const Uint8* Uplane,
                 int Upitch,
                 const Uint8* Vplane,
                 int Vpitch);

  /**
   * Update a rectangle within a planar NV12 or NV21 texture with new pixels.
   *
   * You can use Texture.Update() as long as your pixel data is a contiguous
   * block of NV12/21 planes in the proper order, but this function is available
   * if your pixel data is not contiguous.
   *
   * @param rect a pointer to the rectangle of pixels to update, or nullptr to
   *             update the entire texture.
   * @param Yplane the raw pixel data for the Y plane.
   * @param Ypitch the number of bytes between rows of pixel data for the Y
   *               plane.
   * @param UVplane the raw pixel data for the UV plane.
   * @param UVpitch the number of bytes between rows of pixel data for the UV
   *                plane.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.Update
   * @sa Texture.UpdateYUV
   */
  void UpdateNV(OptionalRef<const RectRaw> rect,
                const Uint8* Yplane,
                int Ypitch,
                const Uint8* UVplane,
                int UVpitch);

  /**
   * Lock a portion of the texture for **write-only** pixel access.
   *
   * As an optimization, the pixels made available for editing don't necessarily
   * contain the old texture data. This is a write-only operation, and if you
   * need to keep a copy of the texture data you should do that at the
   * application level.
   *
   * You must use Texture.Unlock() to unlock the pixels and apply any
   * changes.
   *
   *                `TEXTUREACCESS_STREAMING`.
   * @param rect an Rect structure representing the area to lock for access;
   *             nullptr to lock the entire texture.
   * @param pixels this is filled in with a pointer to the locked pixels,
   *               appropriately offset by the locked area.
   * @param pitch this is filled in with the pitch of the locked pixels; the
   *              pitch is the length of one row in bytes.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.LockToSurface
   * @sa Texture.Unlock
   */
  void Lock(OptionalRef<const SDL_Rect> rect, void** pixels, int* pitch);

  /**
   * Lock a portion of the texture for **write-only** pixel access, and expose
   * it as a SDL surface.
   *
   * Besides providing an Surface instead of raw pixel data, this function
   * operates like Texture.Lock.
   *
   * As an optimization, the pixels made available for editing don't necessarily
   * contain the old texture data. This is a write-only operation, and if you
   * need to keep a copy of the texture data you should do that at the
   * application level.
   *
   * You must use Texture.Unlock() to unlock the pixels and apply any
   * changes.
   *
   * The returned surface is freed internally after calling Texture.Unlock()
   * or Texture.Destroy(). The caller should not free it.
   *
   *                `TEXTUREACCESS_STREAMING`.
   * @param rect a pointer to the rectangle to lock for access. If the rect is
   *             nullptr, the entire texture will be locked.
   * @returns a surface of size **rect**. Don't assume any specific pixel
   *          content.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.Lock
   * @sa Texture.Unlock
   */
  Surface LockToSurface(OptionalRef<const SDL_Rect> rect = std::nullopt);

  /**
   * Unlock a texture, uploading the changes to video memory, if needed.
   *
   * **Warning**: Please note that Texture.Lock() is intended to be
   * write-only; it will not guarantee the previous contents of the texture will
   * be provided. You must fully initialize any area of a texture that you lock
   * before unlocking it, as the pixels might otherwise be uninitialized memory.
   *
   * Which is to say: locking and immediately unlocking a texture can result in
   * corrupted textures, depending on the renderer in use.
   *
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Texture.Lock
   */
  void Unlock();

  /**
   * Get the width in pixels.
   */
  int GetWidth() const { return m_resource->w; }

  /**
   * Get the height in pixels.
   */
  int GetHeight() const { return m_resource->h; }

  /**
   * Get the size in pixels.
   */
  Point GetSize() const { return Point(GetWidth(), GetHeight()); }

  /**
   * Get the size of a texture, as floating point values.
   *
   * @param w a pointer filled in with the width of the texture in pixels. This
   *          argument can be nullptr if you don't need this information.
   * @param h a pointer filled in with the height of the texture in pixels. This
   *          argument can be nullptr if you don't need this information.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void GetSize(float* w, float* h) const;

  /**
   * Get the size in pixels.
   */
  FPoint GetSizeFloat() const
  {
    FPoint p;
    GetSize(&p.x, &p.y);
    return p;
  }

  /**
   * Get the pixel format.
   */
  PixelFormat GetFormat() const { return m_resource->format; }
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
 * @sa Renderer.Renderer
 * @sa GetRenderDriver
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
 *              GetNumRenderDrivers() - 1.
 * @returns the name of the rendering driver at the requested index, or nullptr
 *          if an invalid index was specified.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumRenderDrivers
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
 *                     Window.Window()).
 * @returns a pair with window and renderer.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.Renderer
 * @sa Window.Window
 */
inline std::pair<Window, Renderer> CreateWindowAndRenderer(
  StringParam title,
  const PointRaw& size,
  WindowFlags window_flags = 0)
{
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  CheckError(SDL_CreateWindowAndRenderer(
    title, size.x, size.y, window_flags, &window, &renderer));
  return {Window{window}, Renderer(renderer)};
}

/**
 * Create a 2D rendering context for a window.
 *
 * If you want a specific renderer, you can specify its name here. A list of
 * available renderers can be obtained by calling GetRenderDriver()
 * multiple times, with indices from 0 to GetNumRenderDrivers()-1. If you
 * don't need a specific renderer, specify nullptr and SDL will attempt to
 * choose the best option for you, based on what is available on the user's
 * system.
 *
 * If `name` is a comma-separated list, SDL will try each name, in the order
 * listed, until one succeeds or all of them fail.
 *
 * By default the rendering size matches the window size in pixels, but you
 * can call Renderer.SetLogicalPresentation() to change the content size and
 * scaling options.
 *
 * @param window the window where rendering is displayed.
 * @param name the name of the rendering driver to initialize, or nullptr to let
 *             SDL choose one.
 * @returns a valid rendering context or nullptr if there was an error; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.Renderer
 * @sa Renderer.Renderer
 * @sa Renderer.Destroy
 * @sa GetNumRenderDrivers
 * @sa GetRenderDriver
 * @sa Renderer.GetName
 */
inline Renderer CreateRenderer(WindowParam window, StringParam name)
{
  return Renderer(window, std::move(name));
}

/**
 * Create a 2D rendering context for a window, with the specified properties.
 *
 * These are the supported properties:
 *
 * - `prop::Renderer.CREATE_NAME_STRING`: the name of the rendering driver
 *   to use, if a specific one is desired
 * - `prop::Renderer.CREATE_WINDOW_POINTER`: the window where rendering is
 *   displayed, required if this isn't a software renderer using a surface
 * - `prop::Renderer.CREATE_SURFACE_POINTER`: the surface where rendering
 *   is displayed, if you want a software renderer without a window
 * - `prop::Renderer.CREATE_OUTPUT_COLORSPACE_NUMBER`: an Colorspace
 *   value describing the colorspace for output to the display, defaults to
 *   COLORSPACE_SRGB. The direct3d11, direct3d12, and metal renderers
 *   support COLORSPACE_SRGB_LINEAR, which is a linear color space and
 *   supports HDR output. If you select COLORSPACE_SRGB_LINEAR, drawing
 *   still uses the sRGB colorspace, but values can go beyond 1.0 and float
 *   (linear) format textures can be used for HDR content.
 * - `prop::Renderer.CREATE_PRESENT_VSYNC_NUMBER`: non-zero if you want
 *   present synchronized with the refresh rate. This property can take any
 *   value that is supported by Renderer.SetVSync() for the renderer.
 *
 * With the vulkan renderer:
 *
 * - `prop::Renderer.CREATE_VULKAN_INSTANCE_POINTER`: the VkInstance to use
 *   with the renderer, optional.
 * - `prop::Renderer.CREATE_VULKAN_SURFACE_NUMBER`: the VkSurfaceKHR to use
 *   with the renderer, optional.
 * - `prop::Renderer.CREATE_VULKAN_PHYSICAL_DEVICE_POINTER`: the
 *   VkPhysicalDevice to use with the renderer, optional.
 * - `prop::Renderer.CREATE_VULKAN_DEVICE_POINTER`: the VkDevice to use
 *   with the renderer, optional.
 * - `prop::Renderer.CREATE_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER`: the
 *   queue family index used for rendering.
 * - `prop::Renderer.CREATE_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER`: the
 *   queue family index used for presentation.
 *
 * @param props the properties to use.
 * @returns a valid rendering context or nullptr if there was an error; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Create
 * @sa Renderer.Renderer
 * @sa Renderer.Renderer
 * @sa Renderer.Destroy
 * @sa Renderer.GetName
 */
inline Renderer CreateRendererWithProperties(PropertiesParam props)
{
  return Renderer(props);
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

/**
 * Create a 2D software rendering context for a surface.
 *
 * Two other API which can be used to create Renderer:
 * Renderer.Renderer() and CreateWindowAndRenderer(). These can _also_
 * create a software renderer, but they are intended to be used with an
 * Window as the final destination and not an Surface.
 *
 * @param surface the Surface structure representing the surface where
 *                rendering is done.
 * @returns a valid rendering context or nullptr if there was an error; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.Destroy
 */
inline Renderer CreateSoftwareRenderer(SurfaceParam surface)
{
  return Renderer(surface);
}

/**
 * Get the renderer associated with a window.
 *
 * @returns the rendering context on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline RendererRef Window::GetRenderer() const
{
  return {CheckError(SDL_GetRenderer(m_resource))};
}

/**
 * Get the window associated with a renderer.
 *
 * @param renderer the renderer to query.
 * @returns the window on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline WindowRef GetRenderWindow(RendererParam renderer)
{
  return {CheckError(SDL_GetRenderWindow(renderer))};
}

inline WindowRef Renderer::GetWindow()
{
  return SDL::GetRenderWindow(m_resource);
}

/**
 * Get the name of a renderer.
 *
 * @param renderer the rendering context.
 * @returns the name of the selected renderer, or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.Renderer
 * @sa Renderer.Renderer
 */
inline const char* GetRendererName(RendererParam renderer)
{
  return SDL_GetRendererName(renderer);
}

inline const char* Renderer::GetName() const
{
  return SDL::GetRendererName(m_resource);
}

/**
 * Get the properties associated with a renderer.
 *
 * The following read-only properties are provided by SDL:
 *
 * - `prop::Renderer.NAME_STRING`: the name of the rendering driver
 * - `prop::Renderer.WINDOW_POINTER`: the window where rendering is
 *   displayed, if any
 * - `prop::Renderer.SURFACE_POINTER`: the surface where rendering is
 *   displayed, if this is a software renderer without a window
 * - `prop::Renderer.VSYNC_NUMBER`: the current vsync setting
 * - `prop::Renderer.MAX_TEXTURE_SIZE_NUMBER`: the maximum texture width
 *   and height
 * - `prop::Renderer.TEXTURE_FORMATS_POINTER`: a (const PixelFormat *)
 *   array of pixel formats, terminated with PIXELFORMAT_UNKNOWN,
 *   representing the available texture formats for this renderer.
 * - `prop::Renderer.OUTPUT_COLORSPACE_NUMBER`: an Colorspace value
 *   describing the colorspace for output to the display, defaults to
 *   COLORSPACE_SRGB.
 * - `prop::Renderer.HDR_ENABLED_BOOLEAN`: true if the output colorspace is
 *   COLORSPACE_SRGB_LINEAR and the renderer is showing on a display with
 *   HDR enabled. This property can change dynamically when
 *   EVENT_WINDOW_HDR_STATE_CHANGED is sent.
 * - `prop::Renderer.SDR_WHITE_POINT_FLOAT`: the value of SDR white in the
 *   COLORSPACE_SRGB_LINEAR colorspace. When HDR is enabled, this value is
 *   automatically multiplied into the color scale. This property can change
 *   dynamically when EVENT_WINDOW_HDR_STATE_CHANGED is sent.
 * - `prop::Renderer.HDR_HEADROOM_FLOAT`: the additional high dynamic range
 *   that can be displayed, in terms of the SDR white point. When HDR is not
 *   enabled, this will be 1.0. This property can change dynamically when
 *   EVENT_WINDOW_HDR_STATE_CHANGED is sent.
 *
 * With the direct3d renderer:
 *
 * - `prop::Renderer.D3D9_DEVICE_POINTER`: the IDirect3DDevice9 associated
 *   with the renderer
 *
 * With the direct3d11 renderer:
 *
 * - `prop::Renderer.D3D11_DEVICE_POINTER`: the ID3D11Device associated
 *   with the renderer
 * - `prop::Renderer.D3D11_SWAPCHAIN_POINTER`: the IDXGISwapChain1
 *   associated with the renderer. This may change when the window is resized.
 *
 * With the direct3d12 renderer:
 *
 * - `prop::Renderer.D3D12_DEVICE_POINTER`: the ID3D12Device associated
 *   with the renderer
 * - `prop::Renderer.D3D12_SWAPCHAIN_POINTER`: the IDXGISwapChain4
 *   associated with the renderer.
 * - `prop::Renderer.D3D12_COMMAND_QUEUE_POINTER`: the ID3D12CommandQueue
 *   associated with the renderer
 *
 * With the vulkan renderer:
 *
 * - `prop::Renderer.VULKAN_INSTANCE_POINTER`: the VkInstance associated
 *   with the renderer
 * - `prop::Renderer.VULKAN_SURFACE_NUMBER`: the VkSurfaceKHR associated
 *   with the renderer
 * - `prop::Renderer.VULKAN_PHYSICAL_DEVICE_POINTER`: the VkPhysicalDevice
 *   associated with the renderer
 * - `prop::Renderer.VULKAN_DEVICE_POINTER`: the VkDevice associated with
 *   the renderer
 * - `prop::Renderer.VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER`: the queue
 *   family index used for rendering
 * - `prop::Renderer.VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER`: the queue
 *   family index used for presentation
 * - `prop::Renderer.VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER`: the number of
 *   swapchain images, or potential frames in flight, used by the Vulkan
 *   renderer
 *
 * With the gpu renderer:
 *
 * - `prop::Renderer.GPU_DEVICE_POINTER`: the SDL_GPUDevice associated with
 *   the renderer
 *
 * @param renderer the rendering context.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetRendererProperties(RendererParam renderer)
{
  return {CheckError(SDL_GetRendererProperties(renderer))};
}

inline PropertiesRef Renderer::GetProperties() const
{
  return SDL::GetRendererProperties(m_resource);
}

/**
 * Get the output size in pixels of a rendering context.
 *
 * This returns the true output size in pixels, ignoring any render targets or
 * logical size and presentation.
 *
 * For the output size of the current rendering target, with logical size
 * adjustments, use Renderer.GetCurrentOutputSize() instead.
 *
 * @param renderer the rendering context.
 * @param w a pointer filled in with the width in pixels.
 * @param h a pointer filled in with the height in pixels.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetCurrentOutputSize
 */
inline void GetRenderOutputSize(RendererParam renderer, int* w, int* h)
{
  CheckError(SDL_GetRenderOutputSize(renderer, w, h));
}

inline void Renderer::GetOutputSize(int* w, int* h) const
{
  SDL::GetRenderOutputSize(m_resource, w, h);
}

/**
 * Get the current output size in pixels of a rendering context.
 *
 * If a rendering target is active, this will return the size of the rendering
 * target in pixels, otherwise return the value of Renderer.GetOutputSize().
 *
 * Rendering target or not, the output will be adjusted by the current logical
 * presentation state, dictated by Renderer.SetLogicalPresentation().
 *
 * @param renderer the rendering context.
 * @param w a pointer filled in with the current width.
 * @param h a pointer filled in with the current height.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetOutputSize
 */
inline void GetCurrentRenderOutputSize(RendererParam renderer, int* w, int* h)
{
  CheckError(SDL_GetCurrentRenderOutputSize(renderer, w, h));
}

inline void Renderer::GetCurrentOutputSize(int* w, int* h) const
{
  SDL::GetCurrentRenderOutputSize(m_resource, w, h);
}

inline Texture Renderer::CreateTexture(PixelFormat format,
                                       TextureAccess access,
                                       const PointRaw& size)
{
  return Texture(m_resource, format, access, size);
}

/**
 * Create a texture for a rendering context.
 *
 * The contents of a texture when first created are not defined.
 *
 * @param renderer the rendering context.
 * @param format one of the enumerated values in PixelFormat.
 * @param access one of the enumerated values in TextureAccess.
 * @param size the width and height of the texture in pixels.
 * @returns the created texture or nullptr on failure; call GetError() for
 *          more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.Texture
 * @sa Texture.Texture
 * @sa Texture.Destroy
 * @sa Texture.GetSize
 * @sa Texture.Update
 */
inline Texture CreateTexture(RendererParam renderer,
                             PixelFormat format,
                             TextureAccess access,
                             const PointRaw& size)
{
  return Texture(renderer, format, access, size);
}

inline Texture Renderer::CreateTextureFromSurface(SurfaceParam surface)
{
  return Texture(m_resource, surface);
}

/**
 * Create a texture from an existing surface.
 *
 * The surface is not modified or freed by this function.
 *
 * The TextureAccess hint for the created texture is
 * `TEXTUREACCESS_STATIC`.
 *
 * The pixel format of the created texture may be different from the pixel
 * format of the surface, and can be queried using the
 * prop::Texture.FORMAT_NUMBER property.
 *
 * @param renderer the rendering context.
 * @param surface the Surface structure containing pixel data used to fill
 *                the texture.
 * @returns the created texture or nullptr on failure; call GetError() for
 *          more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.Texture
 * @sa Texture.Texture
 * @sa Texture.Destroy
 */
inline Texture CreateTextureFromSurface(RendererParam renderer,
                                        SurfaceParam surface)
{
  return Texture(SDL_CreateTextureFromSurface(renderer, surface));
}

inline Texture Renderer::CreateTextureWithProperties(PropertiesParam props)
{
  return Texture(m_resource, props);
}

/**
 * Create a texture for a rendering context with the specified properties.
 *
 * These are the supported properties:
 *
 * - `prop::Texture.CREATE_COLORSPACE_NUMBER`: an Colorspace value
 *   describing the texture colorspace, defaults to COLORSPACE_SRGB_LINEAR
 *   for floating point textures, COLORSPACE_HDR10 for 10-bit textures,
 *   COLORSPACE_SRGB for other RGB textures and COLORSPACE_JPEG for
 *   YUV textures.
 * - `prop::Texture.CREATE_FORMAT_NUMBER`: one of the enumerated values in
 *   PixelFormat, defaults to the best RGBA format for the renderer
 * - `prop::Texture.CREATE_ACCESS_NUMBER`: one of the enumerated values in
 *   TextureAccess, defaults to TEXTUREACCESS_STATIC
 * - `prop::Texture.CREATE_WIDTH_NUMBER`: the width of the texture in
 *   pixels, required
 * - `prop::Texture.CREATE_HEIGHT_NUMBER`: the height of the texture in
 *   pixels, required
 * - `prop::Texture.CREATE_SDR_WHITE_POINT_FLOAT`: for HDR10 and floating
 *   point textures, this defines the value of 100% diffuse white, with higher
 *   values being displayed in the High Dynamic Range headroom. This defaults
 *   to 100 for HDR10 textures and 1.0 for floating point textures.
 * - `prop::Texture.CREATE_HDR_HEADROOM_FLOAT`: for HDR10 and floating
 *   point textures, this defines the maximum dynamic range used by the
 *   content, in terms of the SDR white point. This would be equivalent to
 *   maxCLL / prop::Texture.CREATE_SDR_WHITE_POINT_FLOAT for HDR10 content.
 *   If this is defined, any values outside the range supported by the display
 *   will be scaled into the available HDR headroom, otherwise they are
 *   clipped.
 *
 * With the direct3d11 renderer:
 *
 * - `prop::Texture.CREATE_D3D11_TEXTURE_POINTER`: the ID3D11Texture2D
 *   associated with the texture, if you want to wrap an existing texture.
 * - `prop::Texture.CREATE_D3D11_TEXTURE_U_POINTER`: the ID3D11Texture2D
 *   associated with the U plane of a YUV texture, if you want to wrap an
 *   existing texture.
 * - `prop::Texture.CREATE_D3D11_TEXTURE_V_POINTER`: the ID3D11Texture2D
 *   associated with the V plane of a YUV texture, if you want to wrap an
 *   existing texture.
 *
 * With the direct3d12 renderer:
 *
 * - `prop::Texture.CREATE_D3D12_TEXTURE_POINTER`: the ID3D12Resource
 *   associated with the texture, if you want to wrap an existing texture.
 * - `prop::Texture.CREATE_D3D12_TEXTURE_U_POINTER`: the ID3D12Resource
 *   associated with the U plane of a YUV texture, if you want to wrap an
 *   existing texture.
 * - `prop::Texture.CREATE_D3D12_TEXTURE_V_POINTER`: the ID3D12Resource
 *   associated with the V plane of a YUV texture, if you want to wrap an
 *   existing texture.
 *
 * With the metal renderer:
 *
 * - `prop::Texture.CREATE_METAL_PIXELBUFFER_POINTER`: the CVPixelBufferRef
 *   associated with the texture, if you want to create a texture from an
 *   existing pixel buffer.
 *
 * With the opengl renderer:
 *
 * - `prop::Texture.CREATE_OPENGL_TEXTURE_NUMBER`: the GLuint texture
 *   associated with the texture, if you want to wrap an existing texture.
 * - `prop::Texture.CREATE_OPENGL_TEXTURE_UV_NUMBER`: the GLuint texture
 *   associated with the UV plane of an NV12 texture, if you want to wrap an
 *   existing texture.
 * - `prop::Texture.CREATE_OPENGL_TEXTURE_U_NUMBER`: the GLuint texture
 *   associated with the U plane of a YUV texture, if you want to wrap an
 *   existing texture.
 * - `prop::Texture.CREATE_OPENGL_TEXTURE_V_NUMBER`: the GLuint texture
 *   associated with the V plane of a YUV texture, if you want to wrap an
 *   existing texture.
 *
 * With the opengles2 renderer:
 *
 * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
 *   associated with the texture, if you want to wrap an existing texture.
 * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
 *   associated with the texture, if you want to wrap an existing texture.
 * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_UV_NUMBER`: the GLuint texture
 *   associated with the UV plane of an NV12 texture, if you want to wrap an
 *   existing texture.
 * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_U_NUMBER`: the GLuint texture
 *   associated with the U plane of a YUV texture, if you want to wrap an
 *   existing texture.
 * - `prop::Texture.CREATE_OPENGLES2_TEXTURE_V_NUMBER`: the GLuint texture
 *   associated with the V plane of a YUV texture, if you want to wrap an
 *   existing texture.
 *
 * With the vulkan renderer:
 *
 * - `prop::Texture.CREATE_VULKAN_TEXTURE_NUMBER`: the VkImage with layout
 *   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL associated with the texture, if
 *   you want to wrap an existing texture.
 *
 * @param renderer the rendering context.
 * @param props the properties to use.
 * @returns the created texture or nullptr on failure; call GetError() for
 *          more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Create
 * @sa Texture.Texture
 * @sa Texture.Texture
 * @sa Texture.Destroy
 * @sa Texture.GetSize
 * @sa Texture.Update
 */
inline Texture CreateTextureWithProperties(RendererParam renderer,
                                           PropertiesParam props)
{
  return Texture(SDL_CreateTextureWithProperties(renderer, props));
}

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
 * Get the properties associated with a texture.
 *
 * The following read-only properties are provided by SDL:
 *
 * - `prop::Texture.COLORSPACE_NUMBER`: an Colorspace value describing
 *   the texture colorspace.
 * - `prop::Texture.FORMAT_NUMBER`: one of the enumerated values in
 *   PixelFormat.
 * - `prop::Texture.ACCESS_NUMBER`: one of the enumerated values in
 *   TextureAccess.
 * - `prop::Texture.WIDTH_NUMBER`: the width of the texture in pixels.
 * - `prop::Texture.HEIGHT_NUMBER`: the height of the texture in pixels.
 * - `prop::Texture.SDR_WHITE_POINT_FLOAT`: for HDR10 and floating point
 *   textures, this defines the value of 100% diffuse white, with higher
 *   values being displayed in the High Dynamic Range headroom. This defaults
 *   to 100 for HDR10 textures and 1.0 for other textures.
 * - `prop::Texture.HDR_HEADROOM_FLOAT`: for HDR10 and floating point
 *   textures, this defines the maximum dynamic range used by the content, in
 *   terms of the SDR white point. If this is defined, any values outside the
 *   range supported by the display will be scaled into the available HDR
 *   headroom, otherwise they are clipped. This defaults to 1.0 for SDR
 *   textures, 4.0 for HDR10 textures, and no default for floating point
 *   textures.
 *
 * With the direct3d11 renderer:
 *
 * - `prop::Texture.D3D11_TEXTURE_POINTER`: the ID3D11Texture2D associated
 *   with the texture
 * - `prop::Texture.D3D11_TEXTURE_U_POINTER`: the ID3D11Texture2D
 *   associated with the U plane of a YUV texture
 * - `prop::Texture.D3D11_TEXTURE_V_POINTER`: the ID3D11Texture2D
 *   associated with the V plane of a YUV texture
 *
 * With the direct3d12 renderer:
 *
 * - `prop::Texture.D3D12_TEXTURE_POINTER`: the ID3D12Resource associated
 *   with the texture
 * - `prop::Texture.D3D12_TEXTURE_U_POINTER`: the ID3D12Resource associated
 *   with the U plane of a YUV texture
 * - `prop::Texture.D3D12_TEXTURE_V_POINTER`: the ID3D12Resource associated
 *   with the V plane of a YUV texture
 *
 * With the vulkan renderer:
 *
 * - `prop::Texture.VULKAN_TEXTURE_NUMBER`: the VkImage associated with the
 *   texture
 *
 * With the opengl renderer:
 *
 * - `prop::Texture.OPENGL_TEXTURE_NUMBER`: the GLuint texture associated
 *   with the texture
 * - `prop::Texture.OPENGL_TEXTURE_UV_NUMBER`: the GLuint texture
 *   associated with the UV plane of an NV12 texture
 * - `prop::Texture.OPENGL_TEXTURE_U_NUMBER`: the GLuint texture associated
 *   with the U plane of a YUV texture
 * - `prop::Texture.OPENGL_TEXTURE_V_NUMBER`: the GLuint texture associated
 *   with the V plane of a YUV texture
 * - `prop::Texture.OPENGL_TEXTURE_TARGET_NUMBER`: the GLenum for the
 *   texture target (`GL_TEXTURE_2D`, `GL_TEXTURE_RECTANGLE_ARB`, etc)
 * - `prop::Texture.OPENGL_TEX_W_FLOAT`: the texture coordinate width of
 *   the texture (0.0 - 1.0)
 * - `prop::Texture.OPENGL_TEX_H_FLOAT`: the texture coordinate height of
 *   the texture (0.0 - 1.0)
 *
 * With the opengles2 renderer:
 *
 * - `prop::Texture.OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
 *   associated with the texture
 * - `prop::Texture.OPENGLES2_TEXTURE_UV_NUMBER`: the GLuint texture
 *   associated with the UV plane of an NV12 texture
 * - `prop::Texture.OPENGLES2_TEXTURE_U_NUMBER`: the GLuint texture
 *   associated with the U plane of a YUV texture
 * - `prop::Texture.OPENGLES2_TEXTURE_V_NUMBER`: the GLuint texture
 *   associated with the V plane of a YUV texture
 * - `prop::Texture.OPENGLES2_TEXTURE_TARGET_NUMBER`: the GLenum for the
 *   texture target (`GL_TEXTURE_2D`, `GL_TEXTURE_EXTERNAL_OES`, etc)
 *
 * @param texture the texture to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetTextureProperties(TextureParam texture)
{
  return {CheckError(SDL_GetTextureProperties(texture))};
}

inline PropertiesRef Texture::GetProperties() const
{
  return SDL::GetTextureProperties(m_resource);
}

/**
 * Get the renderer that created an Texture.
 *
 * @param texture the texture to query.
 * @returns a pointer to the Renderer that created the texture, or nullptr on
 *          failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline RendererRef GetRendererFromTexture(TextureParam texture)
{
  return {SDL_GetRendererFromTexture(texture)};
}

inline RendererRef Texture::GetRenderer() const
{
  return SDL::GetRendererFromTexture(m_resource);
}

/**
 * Get the size of a texture, as floating point values.
 *
 * @param texture the texture to query.
 * @param w a pointer filled in with the width of the texture in pixels. This
 *          argument can be nullptr if you don't need this information.
 * @param h a pointer filled in with the height of the texture in pixels. This
 *          argument can be nullptr if you don't need this information.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GetTextureSize(TextureParam texture, float* w, float* h)
{
  CheckError(SDL_GetTextureSize(texture, w, h));
}

inline void Texture::GetSize(float* w, float* h) const
{
  SDL::GetTextureSize(m_resource, w, h);
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
 * @param texture the texture to update.
 * @param r the red color value multiplied into copy operations.
 * @param g the green color value multiplied into copy operations.
 * @param b the blue color value multiplied into copy operations.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetColorMod
 * @sa Texture.SetAlphaMod
 * @sa Texture.SetColorModFloat
 */
inline void SetTextureColorMod(TextureParam texture, Uint8 r, Uint8 g, Uint8 b)
{
  CheckError(SDL_SetTextureColorMod(texture, r, g, b));
}

inline void Texture::SetColorMod(Uint8 r, Uint8 g, Uint8 b)
{
  SDL::SetTextureColorMod(m_resource, r, g, b);
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
 * @param texture the texture to update.
 * @param r the red color value multiplied into copy operations.
 * @param g the green color value multiplied into copy operations.
 * @param b the blue color value multiplied into copy operations.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetColorModFloat
 * @sa Texture.SetAlphaModFloat
 * @sa Texture.SetColorMod
 */
inline void SetTextureColorModFloat(TextureParam texture,
                                    float r,
                                    float g,
                                    float b)
{
  CheckError(SDL_SetTextureColorModFloat(texture, r, g, b));
}

inline void Texture::SetColorModFloat(float r, float g, float b)
{
  SDL::SetTextureColorModFloat(m_resource, r, g, b);
}

/**
 * Get the additional color value multiplied into render copy operations.
 *
 * @param texture the texture to query.
 * @param r a pointer filled in with the current red color value.
 * @param g a pointer filled in with the current green color value.
 * @param b a pointer filled in with the current blue color value.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetAlphaMod
 * @sa Texture.GetColorModFloat
 * @sa Texture.SetColorMod
 */
inline void GetTextureColorMod(TextureParam texture,
                               Uint8* r,
                               Uint8* g,
                               Uint8* b)
{
  CheckError(SDL_GetTextureColorMod(texture, r, g, b));
}

inline void Texture::GetColorMod(Uint8* r, Uint8* g, Uint8* b) const
{
  SDL::GetTextureColorMod(m_resource, r, g, b);
}

/**
 * Get the additional color value multiplied into render copy operations.
 *
 * @param texture the texture to query.
 * @param r a pointer filled in with the current red color value.
 * @param g a pointer filled in with the current green color value.
 * @param b a pointer filled in with the current blue color value.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetAlphaModFloat
 * @sa Texture.GetColorMod
 * @sa Texture.SetColorModFloat
 */
inline void GetTextureColorModFloat(TextureParam texture,
                                    float* r,
                                    float* g,
                                    float* b)
{
  CheckError(SDL_GetTextureColorModFloat(texture, r, g, b));
}

inline void Texture::GetColorModFloat(float* r, float* g, float* b) const
{
  SDL::GetTextureColorModFloat(m_resource, r, g, b);
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
 * @param texture the texture to update.
 * @param alpha the source alpha value multiplied into copy operations.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetAlphaMod
 * @sa Texture.SetAlphaModFloat
 * @sa Texture.SetColorMod
 */
inline void SetTextureAlphaMod(TextureParam texture, Uint8 alpha)
{
  CheckError(SDL_SetTextureAlphaMod(texture, alpha));
}

inline void Texture::SetAlphaMod(Uint8 alpha)
{
  SDL::SetTextureAlphaMod(m_resource, alpha);
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
 * @param texture the texture to update.
 * @param alpha the source alpha value multiplied into copy operations.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetAlphaModFloat
 * @sa Texture.SetAlphaMod
 * @sa Texture.SetColorModFloat
 */
inline void SetTextureAlphaModFloat(TextureParam texture, float alpha)
{
  CheckError(SDL_SetTextureAlphaModFloat(texture, alpha));
}

inline void Texture::SetAlphaModFloat(float alpha)
{
  SDL::SetTextureAlphaModFloat(m_resource, alpha);
}

/**
 * Get the additional alpha value multiplied into render copy operations.
 *
 * @param texture the texture to query.
 * @returns the current alpha value.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetAlphaModFloat
 * @sa Texture.GetColorMod
 * @sa Texture.SetAlphaMod
 */
inline Uint8 GetTextureAlphaMod(TextureParam texture)
{
  Uint8 alpha;
  CheckError(SDL_GetTextureAlphaMod(texture, &alpha));
  return alpha;
}

inline Uint8 Texture::GetAlphaMod() const
{
  return SDL::GetTextureAlphaMod(m_resource);
}

/**
 * Get the additional alpha value multiplied into render copy operations.
 *
 * @param texture the texture to query.
 * @returns the current alpha value.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetAlphaMod
 * @sa Texture.GetColorModFloat
 * @sa Texture.SetAlphaModFloat
 */
inline float GetTextureAlphaModFloat(TextureParam texture)
{
  float alpha;
  CheckError(SDL_GetTextureAlphaModFloat(texture, &alpha));
  return alpha;
}

inline float Texture::GetAlphaModFloat() const
{
  return SDL::GetTextureAlphaModFloat(m_resource);
}

/**
 * Set the blend mode for a texture, used by Renderer.RenderTexture().
 *
 * If the blend mode is not supported, the closest supported mode is chosen
 * and this function returns false.
 *
 * @param texture the texture to update.
 * @param blendMode the BlendMode to use for texture blending.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetBlendMode
 */
inline void SetTextureBlendMode(TextureParam texture, BlendMode blendMode)
{
  CheckError(SDL_SetTextureBlendMode(texture, blendMode));
}

inline void Texture::SetBlendMode(BlendMode blendMode)
{
  SDL::SetTextureBlendMode(m_resource, blendMode);
}

/**
 * Get the blend mode used for texture copy operations.
 *
 * @param texture the texture to query.
 * @return the current BlendMode.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.SetBlendMode
 */
inline BlendMode GetTextureBlendMode(TextureParam texture)
{
  BlendMode blendMode;
  CheckError(SDL_GetTextureBlendMode(texture, &blendMode));
  return blendMode;
}

inline BlendMode Texture::GetBlendMode() const
{
  return SDL::GetTextureBlendMode(m_resource);
}

/**
 * Set the scale mode used for texture scale operations.
 *
 * The default texture scale mode is SCALEMODE_LINEAR.
 *
 * If the scale mode is not supported, the closest supported mode is chosen.
 *
 * @param texture the texture to update.
 * @param scaleMode the ScaleMode to use for texture scaling.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.GetScaleMode
 */
inline void SetTextureScaleMode(TextureParam texture, ScaleMode scaleMode)
{
  CheckError(SDL_SetTextureScaleMode(texture, scaleMode));
}

inline void Texture::SetScaleMode(ScaleMode scaleMode)
{
  SDL::SetTextureScaleMode(m_resource, scaleMode);
}

/**
 * Get the scale mode used for texture scale operations.
 *
 * @param texture the texture to query.
 * @return the current scale mode.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.SetScaleMode
 */
inline ScaleMode GetTextureScaleMode(TextureParam texture)
{
  ScaleMode scaleMode;
  CheckError(SDL_GetTextureScaleMode(texture, &scaleMode));
  return scaleMode;
}

inline ScaleMode Texture::GetScaleMode() const
{
  return SDL::GetTextureScaleMode(m_resource);
}

/**
 * Update the given texture rectangle with new pixel data.
 *
 * The pixel data must be in the pixel format of the texture, which can be
 * queried using the prop::Texture.FORMAT_NUMBER property.
 *
 * This is a fairly slow function, intended for use with static textures that
 * do not change often.
 *
 * If the texture is intended to be updated often, it is preferred to create
 * the texture as streaming and use the locking functions referenced below.
 * While this function will work with streaming textures, for optimization
 * reasons you may not get the pixels back if you lock the texture afterward.
 *
 * @param texture the texture to update.
 * @param rect an Rect structure representing the area to update, or nullptr
 *             to update the entire texture.
 * @param pixels the raw pixel data in the format of the texture.
 * @param pitch the number of bytes in a row of pixel data, including padding
 *              between lines.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.Lock
 * @sa Texture.Unlock
 * @sa Texture.UpdateNV
 * @sa Texture.UpdateYUV
 */
inline void UpdateTexture(TextureParam texture,
                          OptionalRef<const RectRaw> rect,
                          const void* pixels,
                          int pitch)
{
  CheckError(SDL_UpdateTexture(texture, rect, pixels, pitch));
}

inline void Texture::Update(OptionalRef<const RectRaw> rect,
                            const void* pixels,
                            int pitch)
{
  SDL::UpdateTexture(m_resource, rect, pixels, pitch);
}

/**
 * Update a rectangle within a planar YV12 or IYUV texture with new pixel
 * data.
 *
 * You can use Texture.Update() as long as your pixel data is a contiguous
 * block of Y and U/V planes in the proper order, but this function is
 * available if your pixel data is not contiguous.
 *
 * @param texture the texture to update.
 * @param rect a pointer to the rectangle of pixels to update, or nullptr to
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
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.UpdateNV
 * @sa Texture.Update
 */
inline void UpdateYUVTexture(TextureParam texture,
                             OptionalRef<const RectRaw> rect,
                             const Uint8* Yplane,
                             int Ypitch,
                             const Uint8* Uplane,
                             int Upitch,
                             const Uint8* Vplane,
                             int Vpitch)
{
  CheckError(SDL_UpdateYUVTexture(
    texture, rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch));
}

inline void Texture::UpdateYUV(OptionalRef<const RectRaw> rect,
                               const Uint8* Yplane,
                               int Ypitch,
                               const Uint8* Uplane,
                               int Upitch,
                               const Uint8* Vplane,
                               int Vpitch)
{
  SDL::UpdateYUVTexture(
    m_resource, rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
}

/**
 * Update a rectangle within a planar NV12 or NV21 texture with new pixels.
 *
 * You can use Texture.Update() as long as your pixel data is a contiguous
 * block of NV12/21 planes in the proper order, but this function is available
 * if your pixel data is not contiguous.
 *
 * @param texture the texture to update.
 * @param rect a pointer to the rectangle of pixels to update, or nullptr to
 *             update the entire texture.
 * @param Yplane the raw pixel data for the Y plane.
 * @param Ypitch the number of bytes between rows of pixel data for the Y
 *               plane.
 * @param UVplane the raw pixel data for the UV plane.
 * @param UVpitch the number of bytes between rows of pixel data for the UV
 *                plane.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.Update
 * @sa Texture.UpdateYUV
 */
inline void UpdateNVTexture(TextureParam texture,
                            OptionalRef<const RectRaw> rect,
                            const Uint8* Yplane,
                            int Ypitch,
                            const Uint8* UVplane,
                            int UVpitch)
{
  CheckError(
    SDL_UpdateNVTexture(texture, rect, Yplane, Ypitch, UVplane, UVpitch));
}

inline void Texture::UpdateNV(OptionalRef<const RectRaw> rect,
                              const Uint8* Yplane,
                              int Ypitch,
                              const Uint8* UVplane,
                              int UVpitch)
{
  SDL::UpdateNVTexture(m_resource, rect, Yplane, Ypitch, UVplane, UVpitch);
}

/**
 * Lock a portion of the texture for **write-only** pixel access.
 *
 * As an optimization, the pixels made available for editing don't necessarily
 * contain the old texture data. This is a write-only operation, and if you
 * need to keep a copy of the texture data you should do that at the
 * application level.
 *
 * You must use Texture.Unlock() to unlock the pixels and apply any
 * changes.
 *
 * @param texture the texture to lock for access, which was created with
 *                `TEXTUREACCESS_STREAMING`.
 * @param rect an Rect structure representing the area to lock for access;
 *             nullptr to lock the entire texture.
 * @param pixels this is filled in with a pointer to the locked pixels,
 *               appropriately offset by the locked area.
 * @param pitch this is filled in with the pitch of the locked pixels; the
 *              pitch is the length of one row in bytes.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.LockToSurface
 * @sa Texture.Unlock
 */
inline void LockTexture(TextureParam texture,
                        OptionalRef<const SDL_Rect> rect,
                        void** pixels,
                        int* pitch)
{
  CheckError(SDL_LockTexture(texture, rect, pixels, pitch));
}

inline void Texture::Lock(OptionalRef<const SDL_Rect> rect,
                          void** pixels,
                          int* pitch)
{
  SDL::LockTexture(m_resource, rect, pixels, pitch);
}

/**
 * Lock a portion of the texture for **write-only** pixel access, and expose
 * it as a SDL surface.
 *
 * Besides providing an Surface instead of raw pixel data, this function
 * operates like Texture.Lock.
 *
 * As an optimization, the pixels made available for editing don't necessarily
 * contain the old texture data. This is a write-only operation, and if you
 * need to keep a copy of the texture data you should do that at the
 * application level.
 *
 * You must use Texture.Unlock() to unlock the pixels and apply any
 * changes.
 *
 * The returned surface is freed internally after calling Texture.Unlock()
 * or Texture.Destroy(). The caller should not free it.
 *
 * @param texture the texture to lock for access, which must be created with
 *                `TEXTUREACCESS_STREAMING`.
 * @param rect a pointer to the rectangle to lock for access. If the rect is
 *             nullptr, the entire texture will be locked.
 * @returns a surface of size **rect**. Don't assume any specific pixel content.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.Lock
 * @sa Texture.Unlock
 */
inline Surface LockTextureToSurface(
  TextureParam texture,
  OptionalRef<const SDL_Rect> rect = std::nullopt)
{
  SurfaceRaw surface = nullptr;
  CheckError(SDL_LockTextureToSurface(texture, rect, &surface));
  return Surface::Borrow(surface);
}

inline Surface Texture::LockToSurface(OptionalRef<const SDL_Rect> rect)
{
  return SDL::LockTextureToSurface(m_resource, rect);
}

/**
 * Unlock a texture, uploading the changes to video memory, if needed.
 *
 * **Warning**: Please note that Texture.Lock() is intended to be
 * write-only; it will not guarantee the previous contents of the texture will
 * be provided. You must fully initialize any area of a texture that you lock
 * before unlocking it, as the pixels might otherwise be uninitialized memory.
 *
 * Which is to say: locking and immediately unlocking a texture can result in
 * corrupted textures, depending on the renderer in use.
 *
 * @param texture a texture locked by Texture.Lock().
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.Lock
 */
inline void UnlockTexture(TextureParam texture) { SDL_UnlockTexture(texture); }

inline void Texture::Unlock() { SDL::UnlockTexture(m_resource); }

/**
 * Set a texture as the current rendering target.
 *
 * The default render target is the window for which the renderer was created.
 * To stop rendering to a texture and render to the window again, call this
 * function with a nullptr `texture`.
 *
 * Viewport, cliprect, scale, and logical presentation are unique to each
 * render target. Get and set functions for these states apply to the current
 * render target set by this function, and those states persist on each target
 * when the current render target changes.
 *
 * @param renderer the rendering context.
 * @param texture the targeted texture, which must be created with the
 *                `TEXTUREACCESS_TARGET` flag, or nullptr to render to the
 *                window instead of a texture.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetTarget
 */
inline void SetRenderTarget(RendererParam renderer, TextureParam texture)
{
  CheckError(SDL_SetRenderTarget(renderer, texture));
}

inline void Renderer::SetTarget(TextureParam texture)
{
  SDL::SetRenderTarget(m_resource, texture);
}

/**
 * Get the current render target.
 *
 * The default render target is the window for which the renderer was created,
 * and is reported a nullptr here.
 *
 * @param renderer the rendering context.
 * @returns the current render target or nullptr for the default render target.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetTarget
 */
inline Texture GetRenderTarget(RendererParam renderer)
{
  TextureRaw texture = SDL_GetRenderTarget(renderer);
  if (texture) return Texture::Borrow(texture);
  return {};
}

inline Texture Renderer::GetTarget() const
{
  return SDL::GetRenderTarget(m_resource);
}

/**
 * Set a device-independent resolution and presentation mode for rendering.
 *
 * This function sets the width and height of the logical rendering output.
 * The renderer will act as if the current render target is always the
 * requested dimensions, scaling to the actual resolution as necessary.
 *
 * This can be useful for games that expect a fixed size, but would like to
 * scale the output to whatever is available, regardless of how a user resizes
 * a window, or if the display is high DPI.
 *
 * Logical presentation can be used with both render target textures and the
 * renderer's window; the state is unique to each render target, and this
 * function sets the state for the current render target. It might be useful
 * to draw to a texture that matches the window dimensions with logical
 * presentation enabled, and then draw that texture across the entire window
 * with logical presentation disabled. Be careful not to render both with
 * logical presentation enabled, however, as this could produce
 * double-letterboxing, etc.
 *
 * You can disable logical coordinates by setting the mode to
 * LOGICAL_PRESENTATION_DISABLED, and in that case you get the full pixel
 * resolution of the render target; it is safe to toggle logical presentation
 * during the rendering of a frame: perhaps most of the rendering is done to
 * specific dimensions but to make fonts look sharp, the app turns off logical
 * presentation while drawing text, for example.
 *
 * For the renderer's window, letterboxing is drawn into the framebuffer if
 * logical presentation is enabled during Renderer.Present; be sure to
 * reenable it before presenting if you were toggling it, otherwise the
 * letterbox areas might have artifacts from previous frames (or artifacts
 * from external overlays, etc). Letterboxing is never drawn into texture
 * render targets; be sure to call Renderer.RenderClear() before drawing into
 * the texture so the letterboxing areas are cleared, if appropriate.
 *
 * You can convert coordinates in an event into rendering coordinates using
 * Renderer.ConvertEventToRenderCoordinates().
 *
 * @param renderer the rendering context.
 * @param size the width and height of the logical resolution.
 * @param mode the presentation mode used.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.ConvertEventToRenderCoordinates
 * @sa Renderer.GetLogicalPresentation
 * @sa Renderer.GetLogicalPresentationRect
 */
inline void SetRenderLogicalPresentation(RendererParam renderer,
                                         const PointRaw& size,
                                         RendererLogicalPresentation mode)
{
  CheckError(SDL_SetRenderLogicalPresentation(renderer, size.x, size.y, mode));
}

inline void Renderer::SetLogicalPresentation(const PointRaw& size,
                                             RendererLogicalPresentation mode)
{
  SDL::SetRenderLogicalPresentation(m_resource, size, mode);
}

/**
 * Get device independent resolution and presentation mode for rendering.
 *
 * This function gets the width and height of the logical rendering output, or
 * the output size in pixels if a logical resolution is not enabled.
 *
 * Each render target has its own logical presentation state. This function
 * gets the state for the current render target.
 *
 * @param renderer the rendering context.
 * @param w an int to be filled with the width.
 * @param h an int to be filled with the height.
 * @param mode the presentation mode used.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetLogicalPresentation
 */
inline void GetRenderLogicalPresentation(RendererParam renderer,
                                         int* w,
                                         int* h,
                                         RendererLogicalPresentation* mode)
{
  CheckError(SDL_GetRenderLogicalPresentation(renderer, w, h, mode));
}

inline void Renderer::GetLogicalPresentation(
  int* w,
  int* h,
  RendererLogicalPresentation* mode) const
{
  SDL::GetRenderLogicalPresentation(m_resource, w, h, mode);
}

/**
 * Get the final presentation rectangle for rendering.
 *
 * This function returns the calculated rectangle used for logical
 * presentation, based on the presentation mode and output size. If logical
 * presentation is disabled, it will fill the rectangle with the output size,
 * in pixels.
 *
 * Each render target has its own logical presentation state. This function
 * gets the rectangle for the current render target.
 *
 * @param renderer the rendering context.
 * @returns with the final presentation rectangle.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetLogicalPresentation
 */
inline FRect GetRenderLogicalPresentationRect(RendererParam renderer)
{
  FRect rect;
  CheckError(SDL_GetRenderLogicalPresentationRect(renderer, &rect));
  return rect;
}

inline FRect Renderer::GetLogicalPresentationRect() const
{
  return SDL::GetRenderLogicalPresentationRect(m_resource);
}

/**
 * Get a point in render coordinates when given a point in window coordinates.
 *
 * This takes into account several states:
 *
 * - The window dimensions.
 * - The logical presentation settings (Renderer.SetLogicalPresentation)
 * - The scale (Renderer.SetScale)
 * - The viewport (Renderer.SetViewport)
 *
 * @param renderer the rendering context.
 * @param window_coord the x, y coordinate in window coordinates.
 * @returns the x, y coordinate in render coordinates.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetLogicalPresentation
 * @sa Renderer.SetScale
 */
inline FPoint RenderCoordinatesFromWindow(RendererParam renderer,
                                          const FPointRaw& window_coord)
{
  FPoint p;
  CheckError(SDL_RenderCoordinatesFromWindow(
    renderer, window_coord.x, window_coord.y, &p.x, &p.y));
  return p;
}

inline FPoint Renderer::RenderCoordinatesFromWindow(
  const FPointRaw& window_coord) const
{
  return SDL::RenderCoordinatesFromWindow(m_resource, window_coord);
}

/**
 * Get a point in window coordinates when given a point in render coordinates.
 *
 * This takes into account several states:
 *
 * - The window dimensions.
 * - The logical presentation settings (Renderer.SetLogicalPresentation)
 * - The scale (Renderer.SetScale)
 * - The viewport (Renderer.SetViewport)
 *
 * @param renderer the rendering context.
 * @param coord the x, y coordinates in render coordinates.
 * @returns x, y in window coordinates.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetLogicalPresentation
 * @sa Renderer.SetScale
 * @sa Renderer.SetViewport
 */
inline FPoint RenderCoordinatesToWindow(RendererParam renderer,
                                        const FPointRaw& coord)
{
  FPoint p;
  CheckError(
    SDL_RenderCoordinatesToWindow(renderer, coord.x, coord.y, &p.x, &p.y));
  return p;
}

inline FPoint Renderer::RenderCoordinatesToWindow(const FPointRaw& coord) const
{
  return SDL::RenderCoordinatesToWindow(m_resource, coord);
}

/**
 * Convert the coordinates in an event to render coordinates.
 *
 * This takes into account several states:
 *
 * - The window dimensions.
 * - The logical presentation settings (Renderer.SetLogicalPresentation)
 * - The scale (Renderer.SetScale)
 * - The viewport (Renderer.SetViewport)
 *
 * Various event types are converted with this function: mouse, touch, pen,
 * etc.
 *
 * Touch coordinates are converted from normalized coordinates in the window
 * to non-normalized rendering coordinates.
 *
 * Relative mouse coordinates (xrel and yrel event fields) are _also_
 * converted. Applications that do not want these fields converted should use
 * Renderer.RenderCoordinatesFromWindow() on the specific event fields instead
 * of converting the entire event structure.
 *
 * Once converted, coordinates may be outside the rendering area.
 *
 * @param renderer the rendering context.
 * @param event the event to modify.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderCoordinatesFromWindow
 */
inline void ConvertEventToRenderCoordinates(RendererParam renderer,
                                            Event* event)
{
  CheckError(SDL_ConvertEventToRenderCoordinates(renderer, event));
}

inline void Renderer::ConvertEventToRenderCoordinates(Event* event) const
{
  SDL::ConvertEventToRenderCoordinates(m_resource, event);
}

/**
 * Set the drawing area for rendering on the current target.
 *
 * Drawing will clip to this area (separately from any clipping done with
 * Renderer.SetClipRect), and the top left of the area will become coordinate
 * (0, 0) for future drawing commands.
 *
 * The area's width and height must be >= 0.
 *
 * Each render target has its own viewport. This function sets the viewport
 * for the current render target.
 *
 * @param renderer the rendering context.
 * @param rect the Rect structure representing the drawing area, or nullptr
 *             to set the viewport to the entire target.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetViewport
 * @sa Renderer.IsViewportSet
 */
inline void SetRenderViewport(RendererParam renderer,
                              OptionalRef<const RectRaw> rect)
{
  CheckError(SDL_SetRenderViewport(renderer, rect));
}

inline void Renderer::SetViewport(OptionalRef<const RectRaw> rect)
{
  SDL::SetRenderViewport(m_resource, rect);
}

/**
 * Get the drawing area for the current target.
 *
 * Each render target has its own viewport. This function gets the viewport
 * for the current render target.
 *
 * @param renderer the rendering context.
 * @returns an Rect structure filled in with the current drawing area.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.IsViewportSet
 * @sa Renderer.SetViewport
 */
inline Rect GetRenderViewport(RendererParam renderer)
{
  Rect rect;
  CheckError(SDL_GetRenderViewport(renderer, &rect));
  return rect;
}

inline Rect Renderer::GetViewport() const
{
  return SDL::GetRenderViewport(m_resource);
}

/**
 * Return whether an explicit rectangle was set as the viewport.
 *
 * This is useful if you're saving and restoring the viewport and want to know
 * whether you should restore a specific rectangle or nullptr.
 *
 * Each render target has its own viewport. This function checks the viewport
 * for the current render target.
 *
 * @param renderer the rendering context.
 * @returns true if the viewport was set to a specific rectangle, or false if
 *          it was set to nullptr (the entire target).
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetViewport
 * @sa Renderer.SetViewport
 */
inline bool RenderViewportSet(RendererParam renderer)
{
  return SDL_RenderViewportSet(renderer);
}

inline bool Renderer::IsViewportSet() const
{
  return SDL::RenderViewportSet(m_resource);
}

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
 * @param renderer the rendering context.
 * @returns the area that is safe for interactive content.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Rect GetRenderSafeArea(RendererParam renderer)
{
  Rect rect;
  CheckError(SDL_GetRenderSafeArea(renderer, &rect));
  return rect;
}

inline Rect Renderer::GetSafeArea() const
{
  return SDL::GetRenderSafeArea(m_resource);
}

/**
 * Set the clip rectangle for rendering on the specified target.
 *
 * Each render target has its own clip rectangle. This function sets the
 * cliprect for the current render target.
 *
 * @param renderer the rendering context.
 * @param rect an Rect structure representing the clip area, relative to
 *             the viewport, or nullptr to disable clipping.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetClipRect
 * @sa Renderer.IsClipEnabled
 */
inline void SetRenderClipRect(RendererParam renderer,
                              OptionalRef<const RectRaw> rect)
{
  CheckError(SDL_SetRenderClipRect(renderer, rect));
}

inline void Renderer::SetClipRect(OptionalRef<const RectRaw> rect)
{
  SDL::SetRenderClipRect(m_resource, rect);
}

/**
 * Get the clip rectangle for the current target.
 *
 * Each render target has its own clip rectangle. This function gets the
 * cliprect for the current render target.
 *
 * @param renderer the rendering context.
 * @returns an Rect structure filled in with the current clipping area
 *          or an empty rectangle if clipping is disabled.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.IsClipEnabled
 * @sa Renderer.SetClipRect
 */
inline Rect GetRenderClipRect(RendererParam renderer)
{
  Rect rect;
  CheckError(SDL_GetRenderClipRect(renderer, &rect));
  return rect;
}

inline Rect Renderer::GetClipRect() const
{
  return SDL::GetRenderClipRect(m_resource);
}

/**
 * Get whether clipping is enabled on the given render target.
 *
 * Each render target has its own clip rectangle. This function checks the
 * cliprect for the current render target.
 *
 * @param renderer the rendering context.
 * @returns true if clipping is enabled or false if not; call GetError()
 *          for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetClipRect
 * @sa Renderer.SetClipRect
 */
inline bool RenderClipEnabled(RendererParam renderer)
{
  return SDL_RenderClipEnabled(renderer);
}

inline bool Renderer::IsClipEnabled() const
{
  return SDL::RenderClipEnabled(m_resource);
}

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
 * Each render target has its own scale. This function sets the scale for the
 * current render target.
 *
 * @param renderer the rendering context.
 * @param scale the horizontal and vertical scaling factors.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetScale
 */
inline void SetRenderScale(RendererParam renderer, const FPointRaw& scale)
{
  CheckError(SDL_SetRenderScale(renderer, scale.x, scale.y));
}

inline void Renderer::SetScale(const FPointRaw& scale)
{
  SDL::SetRenderScale(m_resource, scale);
}

/**
 * Get the drawing scale for the current target.
 *
 * Each render target has its own scale. This function gets the scale for the
 * current render target.
 *
 * @param renderer the rendering context.
 * @param scaleX a pointer filled in with the horizontal scaling factor.
 * @param scaleY a pointer filled in with the vertical scaling factor.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetScale
 */
inline void GetRenderScale(RendererParam renderer, float* scaleX, float* scaleY)
{
  CheckError(SDL_GetRenderScale(renderer, scaleX, scaleY));
}

inline void Renderer::GetScale(float* scaleX, float* scaleY) const
{
  SDL::GetRenderScale(m_resource, scaleX, scaleY);
}

/**
 * Set the color used for drawing operations.
 *
 * Set the color for drawing or filling rectangles, lines, and points, and for
 * Renderer.RenderClear().
 *
 * @param renderer the rendering context.
 * @param c the color used to draw on the rendering target.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetDrawColor
 * @sa Renderer.SetDrawColorFloat
 */
inline void SetRenderDrawColor(RendererParam renderer, ColorRaw c)
{
  CheckError(SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a));
}

inline void Renderer::SetDrawColor(ColorRaw c)
{
  SDL::SetRenderDrawColor(m_resource, c);
}

/**
 * Set the color used for drawing operations (Rect, Line and Clear).
 *
 * Set the color for drawing or filling rectangles, lines, and points, and for
 * Renderer.RenderClear().
 *
 * @param renderer the rendering context.
 * @param c the color used to draw on the rendering target.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetDrawColorFloat
 * @sa Renderer.SetDrawColor
 */
inline void SetRenderDrawColorFloat(RendererParam renderer, const FColorRaw& c)
{
  CheckError(SDL_SetRenderDrawColorFloat(renderer, c.r, c.g, c.b, c.a));
}

inline void Renderer::SetDrawColorFloat(const FColorRaw& c)
{
  SDL::SetRenderDrawColorFloat(m_resource, c);
}

/**
 * Get the color used for drawing operations (Rect, Line and Clear).
 *
 * @param renderer the rendering context.
 * @param r a pointer filled in with the red value used to draw on the
 *          rendering target.
 * @param g a pointer filled in with the green value used to draw on the
 *          rendering target.
 * @param b a pointer filled in with the blue value used to draw on the
 *          rendering target.
 * @param a a pointer filled in with the alpha value used to draw on the
 *          rendering target; usually `ALPHA_OPAQUE` (255).
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetDrawColorFloat
 * @sa Renderer.SetDrawColor
 */
inline void GetRenderDrawColor(RendererParam renderer,
                               Uint8* r,
                               Uint8* g,
                               Uint8* b,
                               Uint8* a)
{
  CheckError(SDL_GetRenderDrawColor(renderer, r, g, b, a));
}

inline void Renderer::GetDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a) const
{
  SDL::GetRenderDrawColor(m_resource, r, g, b, a);
}

/**
 * Get the color used for drawing operations (Rect, Line and Clear).
 *
 * @param renderer the rendering context.
 * @param r a pointer filled in with the red value used to draw on the
 *          rendering target.
 * @param g a pointer filled in with the green value used to draw on the
 *          rendering target.
 * @param b a pointer filled in with the blue value used to draw on the
 *          rendering target.
 * @param a a pointer filled in with the alpha value used to draw on the
 *          rendering target.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetDrawColorFloat
 * @sa Renderer.GetDrawColor
 */
inline void GetRenderDrawColorFloat(RendererParam renderer,
                                    float* r,
                                    float* g,
                                    float* b,
                                    float* a)
{
  CheckError(SDL_GetRenderDrawColorFloat(renderer, r, g, b, a));
}

inline void Renderer::GetDrawColorFloat(float* r,
                                        float* g,
                                        float* b,
                                        float* a) const
{
  SDL::GetRenderDrawColorFloat(m_resource, r, g, b, a);
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
 * @param renderer the rendering context.
 * @param scale the color scale value.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetColorScale
 */
inline void SetRenderColorScale(RendererParam renderer, float scale)
{
  CheckError(SDL_SetRenderColorScale(renderer, scale));
}

inline void Renderer::SetColorScale(float scale)
{
  SDL::SetRenderColorScale(m_resource, scale);
}

/**
 * Get the color scale used for render operations.
 *
 * @param renderer the rendering context.
 * @return the current color scale value.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetColorScale
 */
inline float GetRenderColorScale(RendererParam renderer)
{
  float scale;
  CheckError(SDL_GetRenderColorScale(renderer, &scale));
  return scale;
}

inline float Renderer::GetColorScale() const
{
  return SDL::GetRenderColorScale(m_resource);
}

/**
 * Set the blend mode used for drawing operations (Fill and Line).
 *
 * If the blend mode is not supported, the closest supported mode is chosen.
 *
 * @param renderer the rendering context.
 * @param blendMode the BlendMode to use for blending.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetDrawBlendMode
 */
inline void SetRenderDrawBlendMode(RendererParam renderer, BlendMode blendMode)
{
  CheckError(SDL_SetRenderDrawBlendMode(renderer, blendMode));
}

inline void Renderer::SetDrawBlendMode(BlendMode blendMode)
{
  SDL::SetRenderDrawBlendMode(m_resource, blendMode);
}

/**
 * Get the blend mode used for drawing operations.
 *
 * @param renderer the rendering context.
 * @return the current BlendMode.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetDrawBlendMode
 */
inline BlendMode GetRenderDrawBlendMode(RendererParam renderer)
{
  BlendMode blendMode;
  CheckError(SDL_GetRenderDrawBlendMode(renderer, &blendMode));
  return blendMode;
}

inline BlendMode Renderer::GetDrawBlendMode() const
{
  return SDL::GetRenderDrawBlendMode(m_resource);
}

/**
 * Clear the current rendering target with the drawing color.
 *
 * This function clears the entire rendering target, ignoring the viewport and
 * the clip rectangle. Note, that clearing will also set/fill all pixels of
 * the rendering target to current renderer draw color, so make sure to invoke
 * Renderer.SetDrawColor() when needed.
 *
 * @param renderer the rendering context.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetDrawColor
 */
inline void RenderClear(RendererParam renderer)
{
  CheckError(SDL_RenderClear(renderer));
}

inline void Renderer::RenderClear() { SDL::RenderClear(m_resource); }

/**
 * Draw a point on the current rendering target at subpixel precision.
 *
 * @param renderer the renderer which should draw a point.
 * @param p the x, y coordinate of the point.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderPoints
 */
inline void RenderPoint(RendererParam renderer, const FPointRaw& p)
{
  CheckError(SDL_RenderPoint(renderer, p.x, p.y));
}

inline void Renderer::RenderPoint(const FPointRaw& p)
{
  SDL::RenderPoint(m_resource, p);
}

/**
 * Draw multiple points on the current rendering target at subpixel precision.
 *
 * @param renderer the renderer which should draw multiple points.
 * @param points the points to draw.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderPoint
 */
inline void RenderPoints(RendererParam renderer,
                         SpanRef<const FPointRaw> points)
{
  CheckError(SDL_RenderPoints(renderer, points.data(), points.size()));
}

inline void Renderer::RenderPoints(SpanRef<const FPointRaw> points)
{
  SDL::RenderPoints(m_resource, points);
}

/**
 * Draw a line on the current rendering target at subpixel precision.
 *
 * @param renderer the renderer which should draw a line.
 * @param p1 the x,y coordinates of the start point.
 * @param p2 the x,y coordinates of the end point.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderLines
 */
inline void RenderLine(RendererParam renderer,
                       const FPointRaw& p1,
                       const FPointRaw& p2)
{
  CheckError(SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y));
}

inline void Renderer::RenderLine(const FPointRaw& p1, const FPointRaw& p2)
{
  SDL::RenderLine(m_resource, p1, p2);
}

/**
 * Draw a series of connected lines on the current rendering target at
 * subpixel precision.
 *
 * @param renderer the renderer which should draw multiple lines.
 * @param points the points along the lines.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderLine
 */
inline void RenderLines(RendererParam renderer, SpanRef<const FPointRaw> points)
{
  CheckError(SDL_RenderLines(renderer, points.data(), points.size()));
}

inline void Renderer::RenderLines(SpanRef<const FPointRaw> points)
{
  SDL::RenderLines(m_resource, points);
}

/**
 * Draw a rectangle on the current rendering target at subpixel precision.
 *
 * @param renderer the renderer which should draw a rectangle.
 * @param rect a pointer to the destination rectangle, or nullptr to outline the
 *             entire rendering target.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderRects
 */
inline void RenderRect(RendererParam renderer, OptionalRef<const FRectRaw> rect)
{
  CheckError(SDL_RenderRect(renderer, rect));
}

inline void Renderer::RenderRect(OptionalRef<const FRectRaw> rect)
{
  SDL::RenderRect(m_resource, rect);
}

/**
 * Draw some number of rectangles on the current rendering target at subpixel
 * precision.
 *
 * @param renderer the renderer which should draw multiple rectangles.
 * @param rects a pointer to an array of destination rectangles.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderRect
 */
inline void RenderRects(RendererParam renderer, SpanRef<const FRectRaw> rects)
{
  CheckError(SDL_RenderRects(renderer, rects.data(), rects.size()));
}

inline void Renderer::RenderRects(SpanRef<const FRectRaw> rects)
{
  SDL::RenderRects(m_resource, rects);
}

/**
 * Fill a rectangle on the current rendering target with the drawing color at
 * subpixel precision.
 *
 * @param renderer the renderer which should fill a rectangle.
 * @param rect a pointer to the destination rectangle, or nullptr for the entire
 *             rendering target.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderFillRects
 */
inline void RenderFillRect(RendererParam renderer,
                           OptionalRef<const FRectRaw> rect)
{
  CheckError(SDL_RenderFillRect(renderer, rect));
}

inline void Renderer::RenderFillRect(OptionalRef<const FRectRaw> rect)
{
  SDL::RenderFillRect(m_resource, rect);
}

/**
 * Fill some number of rectangles on the current rendering target with the
 * drawing color at subpixel precision.
 *
 * @param renderer the renderer which should fill multiple rectangles.
 * @param rects a pointer to an array of destination rectangles.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderFillRect
 */
inline void RenderFillRects(RendererParam renderer,
                            SpanRef<const FRectRaw> rects)
{
  CheckError(SDL_RenderFillRects(renderer, rects.data(), rects.size()));
}

inline void Renderer::RenderFillRects(SpanRef<const FRectRaw> rects)
{
  SDL::RenderFillRects(m_resource, rects);
}

/**
 * Copy a portion of the texture to the current rendering target at subpixel
 * precision.
 *
 * @param renderer the renderer which should copy parts of a texture.
 * @param texture the source texture.
 * @param srcrect a pointer to the source rectangle, or nullptr for the entire
 *                texture.
 * @param dstrect a pointer to the destination rectangle, or nullptr for the
 *                entire rendering target.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderTextureRotated
 * @sa Renderer.RenderTextureTiled
 */
inline void RenderTexture(RendererParam renderer,
                          TextureParam texture,
                          OptionalRef<const FRectRaw> srcrect,
                          OptionalRef<const FRectRaw> dstrect)
{
  CheckError(SDL_RenderTexture(renderer, texture, srcrect, dstrect));
}

inline void Renderer::RenderTexture(TextureParam texture,
                                    OptionalRef<const FRectRaw> srcrect,
                                    OptionalRef<const FRectRaw> dstrect)
{
  SDL::RenderTexture(m_resource, texture, srcrect, dstrect);
}

/**
 * Copy a portion of the source texture to the current rendering target, with
 * rotation and flipping, at subpixel precision.
 *
 * @param renderer the renderer which should copy parts of a texture.
 * @param texture the source texture.
 * @param srcrect a pointer to the source rectangle, or nullptr for the entire
 *                texture.
 * @param dstrect a pointer to the destination rectangle, or nullptr for the
 *                entire rendering target.
 * @param angle an angle in degrees that indicates the rotation that will be
 *              applied to dstrect, rotating it in a clockwise direction.
 * @param center a pointer to a point indicating the point around which
 *               dstrect will be rotated (if nullptr, rotation will be done
 *               around dstrect.w/2, dstrect.h/2).
 * @param flip an FlipMode value stating which flipping actions should be
 *             performed on the texture.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderTexture
 */
inline void RenderTextureRotated(RendererParam renderer,
                                 TextureParam texture,
                                 OptionalRef<const FRectRaw> srcrect,
                                 OptionalRef<const FRectRaw> dstrect,
                                 double angle,
                                 OptionalRef<const FPointRaw> center,
                                 FlipMode flip = FlipMode::SDL_FLIP_NONE)
{
  CheckError(SDL_RenderTextureRotated(
    renderer, texture, srcrect, dstrect, angle, center, flip));
}

inline void Renderer::RenderTextureRotated(TextureParam texture,
                                           OptionalRef<const FRectRaw> srcrect,
                                           OptionalRef<const FRectRaw> dstrect,
                                           double angle,
                                           OptionalRef<const FPointRaw> center,
                                           FlipMode flip)
{
  SDL::RenderTextureRotated(
    m_resource, texture, srcrect, dstrect, angle, center, flip);
}

/**
 * Copy a portion of the source texture to the current rendering target, with
 * affine transform, at subpixel precision.
 *
 * @param renderer the renderer which should copy parts of a texture.
 * @param texture the source texture.
 * @param srcrect a pointer to the source rectangle, or nullptr for the entire
 *                texture.
 * @param origin a pointer to a point indicating where the top-left corner of
 *               srcrect should be mapped to, or nullptr for the rendering
 *               target's origin.
 * @param right a pointer to a point indicating where the top-right corner of
 *              srcrect should be mapped to, or nullptr for the rendering
 *              target's top-right corner.
 * @param down a pointer to a point indicating where the bottom-left corner of
 *             srcrect should be mapped to, or nullptr for the rendering
 * target's bottom-left corner.
 * @throws Error on failure.
 *
 * @threadsafety You may only call this function from the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderTexture
 */
inline void RenderTextureAffine(RendererParam renderer,
                                TextureParam texture,
                                OptionalRef<const FRectRaw> srcrect,
                                OptionalRef<const FPointRaw> origin,
                                OptionalRef<const FPointRaw> right,
                                OptionalRef<const FPointRaw> down)
{
  CheckError(
    SDL_RenderTextureAffine(renderer, texture, srcrect, origin, right, down));
}

inline void Renderer::RenderTextureAffine(TextureParam texture,
                                          OptionalRef<const FRectRaw> srcrect,
                                          OptionalRef<const FPointRaw> origin,
                                          OptionalRef<const FPointRaw> right,
                                          OptionalRef<const FPointRaw> down)
{
  SDL::RenderTextureAffine(m_resource, texture, srcrect, origin, right, down);
}

/**
 * Tile a portion of the texture to the current rendering target at subpixel
 * precision.
 *
 * The pixels in `srcrect` will be repeated as many times as needed to
 * completely fill `dstrect`.
 *
 * @param renderer the renderer which should copy parts of a texture.
 * @param texture the source texture.
 * @param srcrect a pointer to the source rectangle, or nullptr for the entire
 *                texture.
 * @param scale the scale used to transform srcrect into the destination
 *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
 *              64x64 tiles.
 * @param dstrect a pointer to the destination rectangle, or nullptr for the
 *                entire rendering target.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderTexture
 */
inline void RenderTextureTiled(RendererParam renderer,
                               TextureParam texture,
                               OptionalRef<const FRectRaw> srcrect,
                               float scale,
                               OptionalRef<const FRectRaw> dstrect)
{
  CheckError(
    SDL_RenderTextureTiled(renderer, texture, srcrect, scale, dstrect));
}

inline void Renderer::RenderTextureTiled(TextureParam texture,
                                         OptionalRef<const FRectRaw> srcrect,
                                         float scale,
                                         OptionalRef<const FRectRaw> dstrect)
{
  SDL::RenderTextureTiled(m_resource, texture, srcrect, scale, dstrect);
}

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
 * @param renderer the renderer which should copy parts of a texture.
 * @param texture the source texture.
 * @param srcrect the Rect structure representing the rectangle to be used
 *                for the 9-grid, or nullptr to use the entire texture.
 * @param left_width the width, in pixels, of the left corners in `srcrect`.
 * @param right_width the width, in pixels, of the right corners in `srcrect`.
 * @param top_height the height, in pixels, of the top corners in `srcrect`.
 * @param bottom_height the height, in pixels, of the bottom corners in
 *                      `srcrect`.
 * @param scale the scale used to transform the corner of `srcrect` into the
 *              corner of `dstrect`, or 0.0f for an unscaled copy.
 * @param dstrect a pointer to the destination rectangle, or nullptr for the
 *                entire rendering target.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderTexture
 */
inline void RenderTexture9Grid(RendererParam renderer,
                               TextureParam texture,
                               OptionalRef<const FRectRaw> srcrect,
                               float left_width,
                               float right_width,
                               float top_height,
                               float bottom_height,
                               float scale,
                               OptionalRef<const FRectRaw> dstrect)
{
  CheckError(SDL_RenderTexture9Grid(renderer,
                                    texture,
                                    srcrect,
                                    left_width,
                                    right_width,
                                    top_height,
                                    bottom_height,
                                    scale,
                                    dstrect));
}

inline void Renderer::RenderTexture9Grid(TextureParam texture,
                                         OptionalRef<const FRectRaw> srcrect,
                                         float left_width,
                                         float right_width,
                                         float top_height,
                                         float bottom_height,
                                         float scale,
                                         OptionalRef<const FRectRaw> dstrect)
{
  SDL::RenderTexture9Grid(m_resource,
                          texture,
                          srcrect,
                          left_width,
                          right_width,
                          top_height,
                          bottom_height,
                          scale,
                          dstrect);
}

/**
 * Render a list of triangles, optionally using a texture and indices into the
 * vertex array Color and alpha modulation is done per vertex
 * (Texture.SetColorMod and Texture.SetAlphaMod are ignored).
 *
 * @param renderer the rendering context.
 * @param texture (optional) The SDL texture to use.
 * @param vertices vertices.
 * @param indices (optional) An array of integer indices into the 'vertices'
 *                array, if nullptr all vertices will be rendered in sequential
 *                order.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderGeometryRaw
 */
inline void RenderGeometry(RendererParam renderer,
                           TextureParam texture,
                           std::span<const Vertex> vertices,
                           std::span<const int> indices = {})
{
  CheckError(SDL_RenderGeometry(renderer,
                                texture,
                                vertices.data(),
                                vertices.size(),
                                indices.data(),
                                indices.size()));
}

inline void Renderer::RenderGeometry(TextureParam texture,
                                     std::span<const Vertex> vertices,
                                     std::span<const int> indices)
{
  SDL::RenderGeometry(m_resource, texture, vertices, indices);
}

/**
 * Render a list of triangles, optionally using a texture and indices into the
 * vertex arrays Color and alpha modulation is done per vertex
 * (Texture.SetColorMod and Texture.SetAlphaMod are ignored).
 *
 * @param renderer the rendering context.
 * @param texture (optional) The SDL texture to use.
 * @param xy vertex positions.
 * @param xy_stride byte size to move from one element to the next element.
 * @param color vertex colors (as FColor).
 * @param color_stride byte size to move from one element to the next element.
 * @param uv vertex normalized texture coordinates.
 * @param uv_stride byte size to move from one element to the next element.
 * @param num_vertices number of vertices.
 * @param indices (optional) An array of indices into the 'vertices' arrays,
 *                if nullptr all vertices will be rendered in sequential order.
 * @param num_indices number of indices.
 * @param size_indices index size: 1 (byte), 2 (short), 4 (int).
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderGeometry
 */
inline void RenderGeometryRaw(RendererParam renderer,
                              TextureParam texture,
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
  CheckError(SDL_RenderGeometryRaw(renderer,
                                   texture,
                                   xy,
                                   xy_stride,
                                   color,
                                   color_stride,
                                   uv,
                                   uv_stride,
                                   num_vertices,
                                   indices,
                                   num_indices,
                                   size_indices));
}

inline void Renderer::RenderGeometryRaw(TextureParam texture,
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
  SDL::RenderGeometryRaw(m_resource,
                         texture,
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

/**
 * Read pixels from the current rendering target.
 *
 * The returned surface contains pixels inside the desired area clipped to the
 * current viewport, and should be freed with Surface.Destroy().
 *
 * Note that this returns the actual pixels on the screen, so if you are using
 * logical presentation you should use Renderer.GetLogicalPresentationRect()
 * to get the area containing your content.
 *
 * **WARNING**: This is a very slow operation, and should not be used
 * frequently. If you're using this on the main rendering target, it should be
 * called after rendering and before Renderer.Present().
 *
 * @param renderer the rendering context.
 * @param rect an Rect structure representing the area to read, which will
 *             be clipped to the current viewport, or nullptr for the entire
 *             viewport.
 * @returns a new Surface on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Surface RenderReadPixels(RendererParam renderer,
                                OptionalRef<const RectRaw> rect = {})
{
  return Surface{CheckError(SDL_RenderReadPixels(renderer, rect))};
}

inline Surface Renderer::ReadPixels(OptionalRef<const RectRaw> rect) const
{
  return SDL::RenderReadPixels(m_resource, rect);
}

/**
 * Update the screen with any rendering performed since the previous call.
 *
 * SDL's rendering functions operate on a backbuffer; that is, calling a
 * rendering function such as Renderer.RenderLine() does not directly put a line
 * on the screen, but rather updates the backbuffer. As such, you compose your
 * entire scene and *present* the composed backbuffer to the screen as a
 * complete picture.
 *
 * Therefore, when using SDL's rendering API, one does all drawing intended
 * for the frame, and then calls this function once per frame to present the
 * final drawing to the user.
 *
 * The backbuffer should be considered invalidated after each present; do not
 * assume that previous contents will exist between frames. You are strongly
 * encouraged to call Renderer.RenderClear() to initialize the backbuffer before
 * starting each new frame's drawing, even if you plan to overwrite every
 * pixel.
 *
 * Please note, that in case of rendering to a texture - there is **no need**
 * to call `Renderer.Present` after drawing needed objects to a texture, and
 * should not be done; you are only required to change back the rendering
 * target to default via `Renderer.SetTarget(renderer, nullptr)` afterwards, as
 * textures by themselves do not have a concept of backbuffers. Calling
 * Renderer.Present while rendering to a texture will still update the screen
 * with any current drawing that has been done _to the window itself_.
 *
 * @param renderer the rendering context.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.Renderer
 * @sa Renderer.RenderClear
 * @sa Renderer.RenderFillRect
 * @sa Renderer.RenderFillRects
 * @sa Renderer.RenderLine
 * @sa Renderer.RenderLines
 * @sa Renderer.RenderPoint
 * @sa Renderer.RenderPoints
 * @sa Renderer.RenderRect
 * @sa Renderer.RenderRects
 * @sa Renderer.SetDrawBlendMode
 * @sa Renderer.SetDrawColor
 */
inline void RenderPresent(RendererParam renderer)
{
  CheckError(SDL_RenderPresent(renderer));
}

inline void Renderer::Present() { SDL::RenderPresent(m_resource); }

/**
 * Destroy the specified texture.
 *
 * Passing nullptr or an otherwise invalid texture will set the SDL error
 * message to "Invalid texture".
 *
 * @param texture the texture to destroy.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Texture.Texture
 * @sa Texture.Texture
 */
inline void DestroyTexture(TextureRaw texture) { SDL_DestroyTexture(texture); }

inline void Texture::Destroy()
{
  SDL_DestroyTexture(m_resource);
  m_resource = nullptr;
}

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
 * @sa Renderer.Renderer
 */
inline void DestroyRenderer(RendererRaw renderer)
{
  SDL_DestroyRenderer(renderer);
}

inline void Renderer::Destroy()
{
  SDL_DestroyRenderer(m_resource);
  m_resource = nullptr;
}

/**
 * Force the rendering context to flush any pending commands and state.
 *
 * You do not need to (and in fact, shouldn't) call this function unless you
 * are planning to call into OpenGL/Direct3D/Metal/whatever directly, in
 * addition to using an Renderer.
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
 * @param renderer the rendering context.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void FlushRenderer(RendererParam renderer)
{
  CheckError(SDL_FlushRenderer(renderer));
}

inline void Renderer::Flush() { SDL::FlushRenderer(m_resource); }

/**
 * Get the CAMetalLayer associated with the given Metal renderer.
 *
 * This function returns `void *`, so SDL doesn't have to include Metal's
 * headers, but it can be safely cast to a `CAMetalLayer *`.
 *
 * @param renderer the renderer to query.
 * @returns a `CAMetalLayer *` on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetRenderMetalCommandEncoder
 */
inline void* GetRenderMetalLayer(RendererParam renderer)
{
  return CheckError(SDL_GetRenderMetalLayer(renderer));
}

inline void* Renderer::GetRenderMetalLayer()
{
  return SDL::GetRenderMetalLayer(m_resource);
}

/**
 * Get the Metal command encoder for the current frame.
 *
 * This function returns `void *`, so SDL doesn't have to include Metal's
 * headers, but it can be safely cast to an `id<MTLRenderCommandEncoder>`.
 *
 * This will return nullptr if Metal refuses to give SDL a drawable to render
 * to, which might happen if the window is hidden/minimized/offscreen. This
 * doesn't apply to command encoders for render targets, just the window's
 * backbuffer. Check your return values!
 *
 * @param renderer the renderer to query.
 * @returns an `id<MTLRenderCommandEncoder>` on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetRenderMetalLayer
 */
inline void* GetRenderMetalCommandEncoder(RendererParam renderer)
{
  return CheckError(SDL_GetRenderMetalCommandEncoder(renderer));
}

inline void* Renderer::GetRenderMetalCommandEncoder()
{
  return SDL::GetRenderMetalCommandEncoder(m_resource);
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
 * prop::Renderer.VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER will give you the
 * maximum number of semaphores you'll need.
 *
 * @param renderer the rendering context.
 * @param wait_stage_mask the VkPipelineStageFlags for the wait.
 * @param wait_semaphore a VkSempahore to wait on before rendering the current
 *                       frame, or 0 if not needed.
 * @param signal_semaphore a VkSempahore that SDL will signal when rendering
 *                         for the current frame is complete, or 0 if not
 *                         needed.
 * @throws Error on failure.
 *
 * @threadsafety It is **NOT** safe to call this function from two threads at
 *               once.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void AddVulkanRenderSemaphores(RendererParam renderer,
                                      Uint32 wait_stage_mask,
                                      Sint64 wait_semaphore,
                                      Sint64 signal_semaphore)
{
  CheckError(SDL_AddVulkanRenderSemaphores(
    renderer, wait_stage_mask, wait_semaphore, signal_semaphore));
}

inline void Renderer::AddVulkanRenderSemaphores(Uint32 wait_stage_mask,
                                                Sint64 wait_semaphore,
                                                Sint64 signal_semaphore)
{
  SDL::AddVulkanRenderSemaphores(
    m_resource, wait_stage_mask, wait_semaphore, signal_semaphore);
}

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
 * @param renderer the renderer to toggle.
 * @param vsync the vertical refresh sync interval.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.GetVSync
 */
inline void SetRenderVSync(RendererParam renderer, int vsync)
{
  CheckError(SDL_SetRenderVSync(renderer, vsync));
}

inline void Renderer::SetVSync(int vsync)
{
  SDL::SetRenderVSync(m_resource, vsync);
}

/// Constant for disabling renderer vsync
#define SDL_RENDERER_VSYNC_DISABLED 0

/// Constant for enabling asaptive renderer vsync
#define SDL_RENDERER_VSYNC_ADAPTIVE (-1)

/**
 * Get VSync of the given renderer.
 *
 * @param renderer the renderer to toggle.
 * @returns the current vertical refresh sync interval. See Renderer.SetVSync()
 *          for the meaning of the value.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.SetVSync
 */
inline int GetRenderVSync(RendererParam renderer)
{
  int vsync;
  CheckError(SDL_GetRenderVSync(renderer, &vsync));
  return vsync;
}

inline int Renderer::GetVSync() const
{
  return SDL::GetRenderVSync(m_resource);
}

/**
 * The size, in pixels, of a single Renderer.RenderDebugText() character.
 *
 * The font is monospaced and square, so this applies to all characters.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderDebugText
 */
#define SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE 8

/**
 * Draw debug text to an Renderer.
 *
 * This function will render a string of text to an Renderer. Note that
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
 * The text is drawn in the color specified by Renderer.SetDrawColor().
 *
 * @param renderer the renderer which should draw a line of text.
 * @param p the x,y coordinates where the top-left corner of the text will draw.
 * @param str the string to render.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderDebugTextFormat
 * @sa SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE
 */
inline void RenderDebugText(RendererParam renderer, FPoint p, StringParam str)
{
  CheckError(SDL_RenderDebugText(renderer, p.x, p.y, str));
}

inline void Renderer::RenderDebugText(FPoint p, StringParam str)
{
  SDL::RenderDebugText(m_resource, p, std::move(str));
}

/**
 * Draw debug text to an Renderer.
 *
 * This function will render a printf()-style format string to a renderer.
 * Note that this is a convenience function for debugging, with severe
 * limitations, and is not intended to be used for production apps and games.
 *
 * For the full list of limitations and other useful information, see
 * Renderer.RenderDebugText.
 *
 * @param renderer the renderer which should draw the text.
 * @param p the x,y coordinate where the top-left corner of the text will draw.
 * @param fmt the format string to draw.
 * @param args additional parameters matching % tokens in the `fmt` string, if
 *            any.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer.RenderDebugText
 * @sa SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE
 */
template<class... ARGS>
inline void RenderDebugTextFormat(RendererParam renderer,
                                  FPoint p,
                                  std::string_view fmt,
                                  ARGS... args)
{
  RenderDebugText(
    renderer, p, std::vformat(fmt, std::make_format_args(args...)));
}

/// @}

} // namespace SDL

#endif /* SDL3PP_RENDER_H_ */
