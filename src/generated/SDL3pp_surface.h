#ifndef SDL3PP_SURFACE_H_
#define SDL3PP_SURFACE_H_

#include <SDL3/SDL_surface.h>
#include "SDL3pp_blendmode.h"
#include "SDL3pp_error.h"
#include "SDL3pp_iostream.h"
#include "SDL3pp_optionalRef.h"
#include "SDL3pp_pixels.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_spanRef.h"
#include "SDL3pp_strings.h"
#include "SDL3pp_version.h"

namespace SDL {

/**
 * @defgroup CategorySurface Category Surface
 *
 * SDL surfaces are buffers of pixels in system RAM. These are useful for
 * passing around and manipulating images that are not stored in GPU memory.
 *
 * Surface makes serious efforts to manage images in various formats, and
 * provides a reasonable toolbox for transforming the data, including copying
 * between surfaces, filling rectangles in the image data, etc.
 *
 * There is also a simple .bmp loader, Surface.LoadBMP(), and a simple .png
 * loader, Surface.LoadPNG(). SDL itself does not provide loaders for other file
 * formats, but there are several excellent external libraries that do,
 * including its own satellite library,
 * [SDL_image](https://wiki.libsdl.org/SDL3_image)
 * .
 *
 * In general these functions are thread-safe in that they can be called on
 * different threads with different surfaces. You should not try to modify any
 * surface from two threads simultaneously.
 *
 * @{
 */

// Forward decl
struct Surface;

/// Alias to raw representation for Surface.
using SurfaceRaw = SDL_Surface*;

/// Safely wrap Surface for non owning parameters
struct SurfaceParam
{
  SurfaceRaw value; ///< parameter's SurfaceRaw

  /// Constructs from SurfaceRaw
  constexpr SurfaceParam(SurfaceRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr SurfaceParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const SurfaceParam& other) const = default;

  /// Converts to underlying SurfaceRaw
  constexpr operator SurfaceRaw() const { return value; }

  /// member access to underlying SurfaceRaw.
  constexpr auto operator->() { return value; }
};

/// Safely wrap Surface for non owning const parameters
struct SurfaceConstParam
{
  const SurfaceRaw value; ///< parameter's const SurfaceRaw

  /// Constructs from const SurfaceRaw
  constexpr SurfaceConstParam(const SurfaceRaw value)
    : value(value)
  {
  }

  /// Constructs from SurfaceParam
  constexpr SurfaceConstParam(SurfaceParam value)
    : value(value.value)
  {
  }

  /// Constructs null/invalid
  constexpr SurfaceConstParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const SurfaceConstParam& other) const = default;

  /// Converts to underlying const SurfaceRaw
  constexpr operator const SurfaceRaw() const { return value; }

  /// member access to underlying SurfaceRaw.
  constexpr auto operator->() { return value; }
};

/**
 * The flags on an Surface.
 *
 * These are generally considered read-only.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using SurfaceFlags = Uint32;

constexpr SurfaceFlags SURFACE_PREALLOCATED =
  SDL_SURFACE_PREALLOCATED; ///< Surface uses preallocated pixel memory

constexpr SurfaceFlags SURFACE_LOCK_NEEDED =
  SDL_SURFACE_LOCK_NEEDED; ///< Surface needs to be locked to access pixels

constexpr SurfaceFlags SURFACE_LOCKED =
  SDL_SURFACE_LOCKED; ///< Surface is currently locked

/// Surface uses pixel memory allocated with aligned_alloc()
constexpr SurfaceFlags SURFACE_SIMD_ALIGNED = SDL_SURFACE_SIMD_ALIGNED;

/**
 * Evaluates to true if the surface needs to be locked before access.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool MustLock(SurfaceConstParam S) const { return SDL_MUSTLOCK(S); }

/**
 * The scaling mode.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ScaleMode = SDL_ScaleMode;

#if SDL_VERSION_ATLEAST(3, 2, 10)

constexpr ScaleMode SCALEMODE_INVALID = SDL_SCALEMODE_INVALID; ///< INVALID

#endif // SDL_VERSION_ATLEAST(3, 2, 10)

constexpr ScaleMode SCALEMODE_NEAREST =
  SDL_SCALEMODE_NEAREST; ///< nearest pixel sampling

constexpr ScaleMode SCALEMODE_LINEAR =
  SDL_SCALEMODE_LINEAR; ///< linear filtering

#if SDL_VERSION_ATLEAST(3, 3, 2)

/**
 * nearest pixel sampling with improved scaling for pixel art, available since
 * SDL 3.4.0
 */
constexpr ScaleMode SCALEMODE_PIXELART = SDL_SCALEMODE_PIXELART;

#endif // SDL_VERSION_ATLEAST(3, 3, 2)

/**
 * The flip mode.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using FlipMode = SDL_FlipMode;

constexpr FlipMode FLIP_NONE = SDL_FLIP_NONE; ///< Do not flip.

constexpr FlipMode FLIP_HORIZONTAL = SDL_FLIP_HORIZONTAL; ///< flip horizontally

constexpr FlipMode FLIP_VERTICAL = SDL_FLIP_VERTICAL; ///< flip vertically

#if SDL_VERSION_ATLEAST(3, 3, 2)

/// flip horizontally and vertically (not a diagonal flip)
constexpr FlipMode FLIP_HORIZONTAL_AND_VERTICAL =
  SDL_FLIP_HORIZONTAL_AND_VERTICAL;

#endif // SDL_VERSION_ATLEAST(3, 3, 2)

/**
 * A collection of pixels used in software blitting.
 *
 * Pixels are arranged in memory in rows, with the top row first. Each row
 * occupies an amount of memory given by the pitch (sometimes known as the row
 * stride in non-SDL APIs).
 *
 * Within each row, pixels are arranged from left to right until the width is
 * reached. Each pixel occupies a number of bits appropriate for its format,
 * with most formats representing each pixel as one or more whole bytes (in
 * some indexed formats, instead multiple pixels are packed into each byte),
 * and a byte order given by the format. After encoding all pixels, any
 * remaining bytes to reach the pitch are used as padding to reach a desired
 * alignment, and have undefined contents.
 *
 * When a surface holds YUV format data, the planes are assumed to be
 * contiguous without padding between them, e.g. a 32x32 surface in NV12
 * format with a pitch of 32 would consist of 32x32 bytes of Y plane followed
 * by 32x16 bytes of UV plane.
 *
 * When a surface holds MJPG format data, pixels points at the compressed JPEG
 * image and pitch is the length of that data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Surface.Surface
 * @sa Surface.Destroy
 *
 * @cat resource
 */
class Surface
{
  SurfaceRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Surface() = default;

  /**
   * Constructs from SurfaceParam.
   *
   * @param resource a SurfaceRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Surface(const SurfaceRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Surface(const Surface& other) { ++m_resource->refcount; }

  /// Move constructor
  constexpr Surface(Surface&& other)
    : Surface(other.release())
  {
  }

  /**
   * Allocate a new surface with a specific pixel format.
   *
   * The pixels of the new surface are initialized to zero.
   *
   * @param width the width of the surface.
   * @param height the height of the surface.
   * @param format the PixelFormat for the new surface's pixel format.
   * @post the new Surface structure that is created or nullptr on failure;
   *          call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Surface
   * @sa Surface.Destroy
   */
  Surface(const PointRaw& size, PixelFormat format)
    : m_resource(SDL_CreateSurface(size, format))
  {
  }

  /**
   * Allocate a new surface with a specific pixel format and existing pixel
   * data.
   *
   * No copy is made of the pixel data. Pixel data is not managed automatically;
   * you must free the surface before you free the pixel data.
   *
   * Pitch is the offset in bytes from one row of pixels to the next, e.g.
   * `width*4` for `PIXELFORMAT_RGBA8888`.
   *
   * You may pass nullptr for pixels and 0 for pitch to create a surface that
   * you will fill in with valid values later.
   *
   * @param width the width of the surface.
   * @param height the height of the surface.
   * @param format the PixelFormat for the new surface's pixel format.
   * @param pixels a pointer to existing pixel data.
   * @param pitch the number of bytes between each row, including padding.
   * @post the new Surface structure that is created or nullptr on failure;
   *          call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Surface
   * @sa Surface.Destroy
   */
  Surface(const PointRaw& size, PixelFormat format, void* pixels, int pitch)
    : m_resource(SDL_CreateSurfaceFrom(size, format, pixels, pitch))
  {
  }

  /**
   * Load an image from a filesystem path into a software surface.
   *
   * An Surface is a buffer of pixels in memory accessible by the CPU. Use
   * this if you plan to hand the data to something else or manipulate it
   * further in code.
   *
   * There are no guarantees about what format the new Surface data will be;
   * in many cases, SDL_image will attempt to supply a surface that exactly
   * matches the provided image, but in others it might have to convert (either
   * because the image is in a format that SDL doesn't directly support or
   * because it's compressed data that could reasonably uncompress to various
   * formats and SDL_image had to pick one). You can inspect an Surface for
   * its specifics, and use Surface.Convert to then migrate to any supported
   * format.
   *
   * If the image format supports a transparent pixel, SDL will set the colorkey
   * for the surface. You can enable RLE acceleration on the surface afterwards
   * by calling: Surface.SetColorKey(image, SDL_RLEACCEL,
   * image->format->colorkey);
   *
   * There is a separate function to read files from an IOStream, if you
   * need an i/o abstraction to provide data from anywhere instead of a simple
   * filesystem read; that function is Surface.Surface().
   *
   * If you are using SDL's 2D rendering API, there is an equivalent call to
   * load images directly into an Texture for use by the GPU without using a
   * software surface: call Texture.Texture() instead.
   *
   * When done with the returned surface, the app should dispose of it with a
   * call to
   * [Surface.Destroy](https://wiki.libsdl.org/SDL3/Surface.Destroy)
   * ().
   *
   * @param file a path on the filesystem to load an image from.
   * @post a new SDL surface, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa LoadSurfaceTyped
   * @sa Surface.Surface
   * @sa Surface.Destroy
   */
  Surface(StringParam file);

  /**
   * Load an image from an SDL data source into a software surface.
   *
   * An Surface is a buffer of pixels in memory accessible by the CPU. Use
   * this if you plan to hand the data to something else or manipulate it
   * further in code.
   *
   * There are no guarantees about what format the new Surface data will be;
   * in many cases, SDL_image will attempt to supply a surface that exactly
   * matches the provided image, but in others it might have to convert (either
   * because the image is in a format that SDL doesn't directly support or
   * because it's compressed data that could reasonably uncompress to various
   * formats and SDL_image had to pick one). You can inspect an Surface for
   * its specifics, and use Surface.Convert to then migrate to any supported
   * format.
   *
   * If the image format supports a transparent pixel, SDL will set the colorkey
   * for the surface. You can enable RLE acceleration on the surface afterwards
   * by calling: Surface.SetColorKey(image, SDL_RLEACCEL,
   * image->format->colorkey);
   *
   * If `closeio` is true, `src` will be closed before returning, whether this
   * function succeeds or not. SDL_image reads everything it needs from `src`
   * during this call in any case.
   *
   * There is a separate function to read files from disk without having to deal
   * with IOStream: `Surface.Surface("filename.jpg")` will call this function
   * and manage those details for you, determining the file type from the
   * filename's extension.
   *
   * There is also LoadSurfaceTyped(), which is equivalent to this function
   * except a file extension (like "BMP", "JPG", etc) can be specified, in case
   * SDL_image cannot autodetect the file format.
   *
   * If you are using SDL's 2D rendering API, there is an equivalent call to
   * load images directly into an Texture for use by the GPU without using a
   * software surface: call Texture.Texture() instead.
   *
   * When done with the returned surface, the app should dispose of it with a
   * call to Surface.Destroy().
   *
   * @param src an IOStream that data will be read from.
   * @param closeio true to close/free the IOStream before returning, false
   *                to leave it open.
   * @post a new SDL surface, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa Surface.Surface
   * @sa LoadSurfaceTyped
   * @sa Surface.Destroy
   */
  Surface(IOStreamParam src, bool closeio);

  /**
   * Safely borrows the from SurfaceParam.
   *
   * @param resource a SurfaceRaw or Surface.
   *
   * This does not takes ownership!
   */
  static constexpr Surface Borrow(SurfaceParam resource)
  {
    if (resource) {
      ++resource.value->refcount;
      return Surface(resource.value);
    }
    return {};
  }

  /**
   * Load a BMP image from a seekable SDL data stream.
   *
   * The new surface should be freed with Surface.Destroy(). Not doing so
   * will result in a memory leak.
   *
   * @param src the data stream for the surface.
   * @param closeio if true, calls IOStream.Close() on `src` before returning,
   * even in the case of an error.
   * @returns a pointer to a new Surface structure or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Destroy
   * @sa Surface.LoadBMP
   * @sa Surface.SaveBMP
   */
  static Surface LoadBMP(IOStreamParam src, bool closeio = false);

  /**
   * Load a BMP image from a file.
   *
   * The new surface should be freed with Surface.Destroy(). Not doing so
   * will result in a memory leak.
   *
   * @param file the BMP file to load.
   * @returns a pointer to a new Surface structure or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Destroy
   * @sa Surface.LoadBMP
   * @sa Surface.SaveBMP
   */
  static Surface LoadBMP(StringParam file);

#if SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Load a PNG image from a seekable SDL data stream.
   *
   * The new surface should be freed with Surface.Destroy(). Not doing so
   * will result in a memory leak.
   *
   * @param src the data stream for the surface.
   * @param closeio if true, calls IOStream.Close() on `src` before returning,
   * even in the case of an error.
   * @returns a pointer to a new Surface structure or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.4.0.
   *
   * @sa Surface.Destroy
   * @sa Surface.LoadPNG
   * @sa Surface.SavePNG
   */
  static Surface LoadPNG(IOStreamParam src, bool closeio = false);

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Load a PNG image from a file.
   *
   * The new surface should be freed with Surface.Destroy(). Not doing so
   * will result in a memory leak.
   *
   * @param file the PNG file to load.
   * @returns a pointer to a new Surface structure or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.4.0.
   *
   * @sa Surface.Destroy
   * @sa Surface.LoadPNG
   * @sa Surface.SavePNG
   */
  static Surface LoadPNG(StringParam file);

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

  /// member access to underlying SurfaceRaw.
  constexpr const SurfaceRaw operator->() const { return m_resource; }

  /// member access to underlying SurfaceRaw.
  constexpr SurfaceRaw operator->() { return m_resource; }

  /// Destructor
  ~Surface() { SDL_DestroySurface(m_resource); }

  /// Assignment operator.
  Surface& operator=(Surface other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying SurfaceRaw.
  constexpr SurfaceRaw get() const { return m_resource; }

  /// Retrieves underlying SurfaceRaw and clear this.
  constexpr SurfaceRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Surface& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to SurfaceParam
  constexpr operator SurfaceParam() const { return {m_resource}; }

  /**
   * Free a surface.
   *
   * It is safe to pass nullptr to this function.
   *
   *
   * @threadsafety No other thread should be using the surface when it is freed.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Surface
   * @sa Surface.Surface
   */
  void Destroy();

  /**
   * Evaluates to true if the surface needs to be locked before access.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool MustLock() const { return SDL::MustLock(m_resource); }

  /**
   * Get the properties associated with a surface.
   *
   * The following properties are understood by SDL:
   *
   * - `prop::Surface.SDR_WHITE_POINT_FLOAT`: for HDR10 and floating point
   *   surfaces, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 203 for HDR10 surfaces and 1.0 for floating point surfaces.
   * - `prop::Surface.HDR_HEADROOM_FLOAT`: for HDR10 and floating point
   *   surfaces, this defines the maximum dynamic range used by the content, in
   *   terms of the SDR white point. This defaults to 0.0, which disables tone
   *   mapping.
   * - `prop::Surface.TONEMAP_OPERATOR_STRING`: the tone mapping operator
   *   used when compressing from a surface with high dynamic range to another
   *   with lower dynamic range. Currently this supports "chrome", which uses
   *   the same tone mapping that Chrome uses for HDR content, the form "*=N",
   *   where N is a floating point scale factor applied in linear space, and
   *   "none", which disables tone mapping. This defaults to "chrome".
   * - `prop::Surface.HOTSPOT_X_NUMBER`: the hotspot pixel offset from the
   *   left edge of the image, if this surface is being used as a cursor.
   * - `prop::Surface.HOTSPOT_Y_NUMBER`: the hotspot pixel offset from the
   *   top edge of the image, if this surface is being used as a cursor.
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
   * Set the colorspace used by a surface.
   *
   * Setting the colorspace doesn't change the pixels, only how they are
   * interpreted in color operations.
   *
   * @param colorspace an Colorspace value describing the surface
   *                   colorspace.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.GetColorspace
   */
  void SetColorspace(Colorspace colorspace);

  /**
   * Get the colorspace used by a surface.
   *
   * The colorspace defaults to COLORSPACE_SRGB_LINEAR for floating point
   * formats, COLORSPACE_HDR10 for 10-bit formats, COLORSPACE_SRGB for
   * other RGB surfaces and COLORSPACE_BT709_FULL for YUV textures.
   *
   * @returns the colorspace used by the surface, or COLORSPACE_UNKNOWN if
   *          the surface is nullptr.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.SetColorspace
   */
  Colorspace GetColorspace() const;

  /**
   * Create a palette and associate it with a surface.
   *
   * This function creates a palette compatible with the provided surface. The
   * palette is then returned for you to modify, and the surface will
   * automatically use the new palette in future operations. You do not need to
   * destroy the returned palette, it will be freed when the reference count
   * reaches 0, usually when the surface is destroyed.
   *
   * Bitmap surfaces (with format PIXELFORMAT_INDEX1LSB or
   * PIXELFORMAT_INDEX1MSB) will have the palette initialized with 0 as
   * white and 1 as black. Other surfaces will get a palette initialized with
   * white in every entry.
   *
   * If this function is called for a surface that already has a palette, a new
   * palette will be created to replace it.
   *
   * @returns a new Palette structure on success.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.SetColors
   */
  Palette CreatePalette();

  /**
   * Set the palette used by a surface.
   *
   * Setting the palette keeps an internal reference to the palette, which can
   * be safely destroyed afterwards.
   *
   * A single palette can be shared with many surfaces.
   *
   * @param palette the Palette structure to use.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.Palette
   * @sa Surface.GetPalette
   */
  void SetPalette(PaletteParam palette);

  /**
   * Get the palette used by a surface.
   *
   * @returns a pointer to the palette used by the surface, or nullptr if there
   * is no palette used.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.SetPalette
   */
  Palette GetPalette() const;

  /**
   * Add an alternate version of a surface.
   *
   * This function adds an alternate version of this surface, usually used for
   * content with high DPI representations like cursors or icons. The size,
   * format, and content do not need to match the original surface, and these
   * alternate versions will not be updated when the original surface changes.
   *
   * This function adds a reference to the alternate version, so you should call
   * Surface.Destroy() on the image after this call.
   *
   * @param image a pointer to an alternate Surface to associate with this
   *              surface.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.RemoveAlternateImages
   * @sa Surface.GetImages
   * @sa Surface.HasAlternateImages
   */
  void AddAlternateImage(SurfaceParam image);

  /**
   * Return whether a surface has alternate versions available.
   *
   * @returns true if alternate versions are available or false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.AddAlternateImage
   * @sa Surface.RemoveAlternateImages
   * @sa Surface.GetImages
   */
  bool HasAlternateImages() const;

  /**
   * Get an array including all versions of a surface.
   *
   * This returns all versions of a surface, with the surface being queried as
   * the first element in the returned array.
   *
   * Freeing the array of surfaces does not affect the surfaces in the array.
   * They are still referenced by the surface being queried and will be cleaned
   * up normally.
   *
   * @param count a pointer filled in with the number of surface pointers
   *              returned, may be nullptr.
   * @returns a nullptr terminated array of Surface pointers or nullptr on
   *          failure; call GetError() for more information. This should be
   *          freed with free() when it is no longer needed.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.AddAlternateImage
   * @sa Surface.RemoveAlternateImages
   * @sa Surface.HasAlternateImages
   */
  OwnArray<SurfaceRaw> GetImages() const;

  /**
   * Remove all alternate versions of a surface.
   *
   * This function removes a reference from all the alternative versions,
   * destroying them if this is the last reference to them.
   *
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.AddAlternateImage
   * @sa Surface.GetImages
   * @sa Surface.HasAlternateImages
   */
  void RemoveAlternateImages();

  /**
   * Set up a surface for directly accessing the pixels.
   *
   * Between calls to Surface.Lock() / Surface.Unlock(), you can write to
   * and read from `surface->pixels`, using the pixel format stored in
   * `surface->format`. Once you are done accessing the surface, you should use
   * Surface.Unlock() to release it.
   *
   * Not all surfaces require locking. If `Surface.MustLock(surface)` evaluates
   * to 0, then you can read and write to the surface at any time, and the pixel
   * format of the surface will not change.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces. The locking referred to by this function
   *               is making the pixels available for direct access, not
   *               thread-safe locking.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.MustLock
   * @sa Surface.Unlock
   */
  void Lock();

  /**
   * Release a surface after directly accessing the pixels.
   *
   *
   * @threadsafety This function is not thread safe. The locking referred to by
   *               this function is making the pixels available for direct
   *               access, not thread-safe locking.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Lock
   */
  void Unlock();

  /**
   * Save a surface to a seekable SDL data stream in BMP format.
   *
   * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
   * BMP directly. Other RGB formats with 8-bit or higher get converted to a
   * 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
   * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
   * not supported.
   *
   * @param dst a data stream to save to.
   * @param closeio if true, calls IOStream.Close() on `dst` before returning,
   * even in the case of an error.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.LoadBMP
   * @sa Surface.SaveBMP
   */
  void SaveBMP(IOStreamParam dst, bool closeio = false) const;

  /**
   * Save a surface to a file in BMP format.
   *
   * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
   * BMP directly. Other RGB formats with 8-bit or higher get converted to a
   * 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
   * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
   * not supported.
   *
   * @param file a file to save to.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.LoadBMP
   * @sa Surface.SaveBMP
   */
  void SaveBMP(StringParam file) const;

#if SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Save a surface to a seekable SDL data stream in PNG format.
   *
   * @param dst a data stream to save to.
   * @param closeio if true, calls IOStream.Close() on `dst` before returning,
   * even in the case of an error.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.4.0.
   *
   * @sa Surface.LoadPNG
   * @sa Surface.SavePNG
   */
  void SavePNG(IOStreamParam dst, bool closeio = false) const;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Save a surface to a file in PNG format.
   *
   * @param file a file to save to.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.4.0.
   *
   * @sa Surface.LoadPNG
   * @sa Surface.SavePNG
   */
  void SavePNG(StringParam file) const;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Set the RLE acceleration hint for a surface.
   *
   * If RLE is enabled, color key and alpha blending blits are much faster, but
   * the surface must be locked before directly accessing the pixels.
   *
   * @param enabled true to enable RLE acceleration, false to disable it.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Blit
   * @sa Surface.Lock
   * @sa Surface.Unlock
   */
  void SetRLE(bool enabled);

  /**
   * Returns whether the surface is RLE enabled.
   *
   * It is safe to pass a nullptr `surface` here; it will return false.
   *
   * @returns true if the surface is RLE enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.SetRLE
   */
  bool HasRLE() const;

  /**
   * Set the color key (transparent pixel) in a surface.
   *
   * The color key defines a pixel value that will be treated as transparent in
   * a blit. For example, one can use this to specify that cyan pixels should be
   * considered transparent, and therefore not rendered.
   *
   * It is a pixel of the format used by the surface, as generated by
   * MapRGB().
   *
   * @param enabled true to enable color key, false to disable color key.
   * @param key the transparent pixel.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.GetColorKey
   * @sa Surface.SetRLE
   * @sa Surface.HasColorKey
   */
  void SetColorKey(std::optional<Uint32> key);

  void ClearColorKey();

  /**
   * Returns whether the surface has a color key.
   *
   * It is safe to pass a nullptr `surface` here; it will return false.
   *
   * @returns true if the surface has a color key, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.SetColorKey
   * @sa Surface.GetColorKey
   */
  bool HasColorKey() const;

  /**
   * Get the color key (transparent pixel) for a surface.
   *
   * The color key is a pixel of the format used by the surface, as generated by
   * MapRGB().
   *
   * If the surface doesn't have color key enabled this function returns false.
   *
   * @param key a pointer filled in with the transparent pixel.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.SetColorKey
   * @sa Surface.HasColorKey
   */
  std::optional<Uint32> GetColorKey() const;

  /**
   * Set an additional color value multiplied into blit operations.
   *
   * When this surface is blitted, during the blit operation each source color
   * channel is modulated by the appropriate color value according to the
   * following formula:
   *
   * `srcC = srcC * (color / 255)`
   *
   * @param r the red color value multiplied into blit operations.
   * @param g the green color value multiplied into blit operations.
   * @param b the blue color value multiplied into blit operations.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.GetColorMod
   * @sa Surface.SetAlphaMod
   */
  void SetColorMod(Uint8 r, Uint8 g, Uint8 b);

  /**
   * Get the additional color value multiplied into blit operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.GetAlphaMod
   * @sa Surface.SetColorMod
   */
  void GetColorMod(Uint8* r, Uint8* g, Uint8* b) const;

  /**
   * Set an additional alpha value used in blit operations.
   *
   * When this surface is blitted, during the blit operation the source alpha
   * value is modulated by this alpha value according to the following formula:
   *
   * `srcA = srcA * (alpha / 255)`
   *
   * @param alpha the alpha value multiplied into blit operations.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.GetAlphaMod
   * @sa Surface.SetColorMod
   */
  void SetAlphaMod(Uint8 alpha);

  /**
   * Get the additional alpha value used in blit operations.
   *
   * @param alpha a pointer filled in with the current alpha value.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.GetColorMod
   * @sa Surface.SetAlphaMod
   */
  Uint8 GetAlphaMod() const;

  void SetMod(Color color);

  Color GetMod() const;

  /**
   * Set the blend mode used for blit operations.
   *
   * To copy a surface to another surface (or texture) without blending with the
   * existing data, the blendmode of the SOURCE surface should be set to
   * `BLENDMODE_NONE`.
   *
   * @param blendMode the BlendMode to use for blit blending.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.GetBlendMode
   */
  void SetBlendMode(BlendMode blendMode);

  /**
   * Get the blend mode used for blit operations.
   *
   * @param blendMode a pointer filled in with the current BlendMode.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.SetBlendMode
   */
  BlendMode GetBlendMode() const;

  /**
   * Set the clipping rectangle for a surface.
   *
   * When `surface` is the destination of a blit, only the area within the clip
   * rectangle is drawn into.
   *
   * Note that blits are automatically clipped to the edges of the source and
   * destination surfaces.
   *
   * @param rect the Rect structure representing the clipping rectangle, or
   *             nullptr to disable clipping.
   * @returns true if the rectangle intersects the surface, otherwise false and
   *          blits will be completely clipped.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.GetClipRect
   */
  bool SetClipRect(OptionalRef<const RectRaw> rect);

  void ResetClipRect();

  /**
   * Get the clipping rectangle for a surface.
   *
   * When `surface` is the destination of a blit, only the area within the clip
   * rectangle is drawn into.
   *
   *                clipped.
   * @param rect an Rect structure filled in with the clipping rectangle for
   *             the surface.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.SetClipRect
   */
  Rect GetClipRect() const;

  /**
   * Flip a surface vertically or horizontally.
   *
   * @param flip the direction to flip.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Flip(FlipMode flip);

#if SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Return a copy of a surface rotated clockwise a number of degrees.
   *
   * The angle of rotation can be negative for counter-clockwise rotation.
   *
   * When the rotation isn't a multiple of 90 degrees, the resulting surface is
   * larger than the original, with the background filled in with the colorkey,
   * if available, or RGBA 255/255/255/0 if not.
   *
   * @param angle the rotation angle, in degrees.
   * @returns a rotated copy of the surface or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.4.0.
   */
  Surface Rotate(float angle);

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Creates a new surface identical to the existing surface.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * The returned surface should be freed with Surface.Destroy().
   *
   * @returns a copy of the surface or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Destroy
   */
  Surface Duplicate() const;

  /**
   * Creates a new surface identical to the existing surface, scaled to the
   * desired size.
   *
   * The returned surface should be freed with Surface.Destroy().
   *
   * @param width the width of the new surface.
   * @param height the height of the new surface.
   * @param scaleMode the ScaleMode to be used.
   * @returns a copy of the surface or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Destroy
   */
  Surface Scale(const PointRaw& size, ScaleMode scaleMode) const;

  /**
   * Copy an existing surface to a new surface of the specified format.
   *
   * This function is used to optimize images for faster *repeat* blitting. This
   * is accomplished by converting the original and storing the result as a new
   * surface. The new, optimized surface can then be used as the source for
   * future blits, making them faster.
   *
   * If you are converting to an indexed surface and want to map colors to a
   * palette, you can use Surface.Convert() instead.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * @param format the new pixel format.
   * @returns the new Surface structure that is created or nullptr on failure;
   *          call GetError() for more information.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Convert
   * @sa Surface.Destroy
   */
  Surface Convert(PixelFormat format) const;

  /**
   * Copy an existing surface to a new surface of the specified format and
   * colorspace.
   *
   * This function converts an existing surface to a new format and colorspace
   * and returns the new surface. This will perform any pixel format and
   * colorspace conversion needed.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * @param format the new pixel format.
   * @param palette an optional palette to use for indexed formats, may be
   * nullptr.
   * @param colorspace the new colorspace.
   * @param props an Properties with additional color properties, or 0.
   * @returns the new Surface structure that is created or nullptr on failure;
   *          call GetError() for more information.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Convert
   * @sa Surface.Destroy
   */
  Surface Convert(PixelFormat format,
                  PaletteParam palette,
                  Colorspace colorspace,
                  PropertiesParam props) const;

  /**
   * Premultiply the alpha in a surface.
   *
   * This is safe to use with src == dst, but not for other overlapping areas.
   *
   * @param linear true to convert from sRGB to linear space for the alpha
   *               multiplication, false to do multiplication in sRGB space.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void PremultiplyAlpha(bool linear);

  /**
   * Clear a surface with a specific color, with floating point precision.
   *
   * This function handles all surface formats, and ignores any clip rectangle.
   *
   * If the surface is YUV, the color is assumed to be in the sRGB colorspace,
   * otherwise the color is assumed to be in the colorspace of the suface.
   *
   * @param r the red component of the pixel, normally in the range 0-1.
   * @param g the green component of the pixel, normally in the range 0-1.
   * @param b the blue component of the pixel, normally in the range 0-1.
   * @param a the alpha component of the pixel, normally in the range 0-1.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Clear(const FColorRaw& c);

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by MapRGB() or MapColor(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * Surface.SetClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rect the Rect structure representing the rectangle to fill, or
   *             nullptr to fill the entire surface.
   * @param color the color to fill with.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.FillRects
   */
  void FillRect(OptionalRef<const RectRaw> rect, Uint32 color);

  void Fill(Uint32 color);

  /**
   * Perform a fast fill of a set of rectangles with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by MapRGB() or MapColor(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * Surface.SetClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rects an array of SDL_Rects representing the rectangles to fill.
   * @param count the number of rectangles in the array.
   * @param color the color to fill with.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.FillRect
   */
  void FillRects(SpanRef<const RectRaw> rects, Uint32 color);

  /**
   * Performs a fast blit from the source surface to the destination surface
   * with clipping.
   *
   * If either `srcrect` or `dstrect` are nullptr, the entire surface (`src` or
   * `dst`) is copied while ensuring clipping to `dst->clip_rect`.
   *
   * The blit function should not be called on a locked surface.
   *
   * The blit semantics for surfaces with and without blending and colorkey are
   * defined as follows:
   *
   * ```
   *    RGBA->RGB:
   *      Source surface blend mode set to BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to BLENDMODE_NONE:
   *       copy RGB.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGBA:
   *     Source surface blend mode set to BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to BLENDMODE_NONE:
   *       copy RGB, set destination alpha to source per-surface alpha value.
   *     both:
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       source color key.
   *
   *   RGBA->RGBA:
   *     Source surface blend mode set to BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to BLENDMODE_NONE:
   *       copy all of RGBA to the destination.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGB:
   *     Source surface blend mode set to BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to BLENDMODE_NONE:
   *       copy RGB.
   *     both:
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       source color key.
   * ```
   *
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, or nullptr to copy the entire surface.
   * @param dst the Surface structure that is the blit target.
   * @param dstrect the Rect structure representing the x and y position in
   *                the destination surface, or nullptr for (0,0). The width and
   *                height are ignored, and are copied from `srcrect`. If you
   *                want a specific width and height, you should use
   *                Surface.BlitScaled().
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.BlitScaled
   */
  void Blit(SurfaceParam src,
            OptionalRef<const RectRaw> srcrect,
            OptionalRef<const RectRaw> dstrect);

  void BlitAt(SurfaceParam src,
              OptionalRef<const RectRaw> srcrect,
              const PointRaw& dstpos);

  /**
   * Perform low-level surface blitting only.
   *
   * This is a semi-private blit function and it performs low-level surface
   * blitting, assuming the input rectangles have already been clipped.
   *
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, may not be nullptr.
   * @param dst the Surface structure that is the blit target.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, may not be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Blit
   */
  void BlitUnchecked(SurfaceParam src,
                     const RectRaw& srcrect,
                     const RectRaw& dstrect);

  /**
   * Perform a scaled blit to a destination surface, which may be of a different
   * format.
   *
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, or nullptr to copy the entire surface.
   * @param dst the Surface structure that is the blit target.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, or nullptr to fill the entire
   *                destination surface.
   * @param scaleMode the ScaleMode to be used.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Blit
   */
  void BlitScaled(SurfaceParam src,
                  OptionalRef<const RectRaw> srcrect,
                  OptionalRef<const RectRaw> dstrect,
                  ScaleMode scaleMode);

  /**
   * Perform low-level surface scaled blitting only.
   *
   * This is a semi-private function and it performs low-level surface blitting,
   * assuming the input rectangles have already been clipped.
   *
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, may not be nullptr.
   * @param dst the Surface structure that is the blit target.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, may not be nullptr.
   * @param scaleMode the ScaleMode to be used.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.BlitScaled
   */
  void BlitUncheckedScaled(SurfaceParam src,
                           const RectRaw& srcrect,
                           const RectRaw& dstrect,
                           ScaleMode scaleMode);

#if SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Perform a stretched pixel copy from one surface to another.
   *
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, or nullptr to copy the entire surface.
   * @param dst the Surface structure that is the blit target.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, or nullptr to fill the entire
   *                destination surface.
   * @param scaleMode the ScaleMode to be used.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.4.0.
   *
   * @sa Surface.BlitScaled
   */
  void Stretch(SurfaceParam src,
               OptionalRef<RectRaw> srcrect,
               OptionalRef<RectRaw> dstrect,
               ScaleMode scaleMode);

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Perform a tiled blit to a destination surface, which may be of a different
   * format.
   *
   * The pixels in `srcrect` will be repeated as many times as needed to
   * completely fill `dstrect`.
   *
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, or nullptr to copy the entire surface.
   * @param dst the Surface structure that is the blit target.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, or nullptr to fill the entire
   * surface.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Blit
   */
  void BlitTiled(SurfaceParam src,
                 OptionalRef<const RectRaw> srcrect,
                 OptionalRef<const RectRaw> dstrect);

  /**
   * Perform a scaled and tiled blit to a destination surface, which may be of a
   * different format.
   *
   * The pixels in `srcrect` will be scaled and repeated as many times as needed
   * to completely fill `dstrect`.
   *
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, or nullptr to copy the entire surface.
   * @param scale the scale used to transform srcrect into the destination
   *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
   *              64x64 tiles.
   * @param scaleMode scale algorithm to be used.
   * @param dst the Surface structure that is the blit target.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, or nullptr to fill the entire
   * surface.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Blit
   */
  void BlitTiledWithScale(SurfaceParam src,
                          OptionalRef<const RectRaw> srcrect,
                          float scale,
                          SDL_ScaleMode scaleMode,
                          OptionalRef<const RectRaw> dstrect);

  /**
   * Perform a scaled blit using the 9-grid algorithm to a destination surface,
   * which may be of a different format.
   *
   * The pixels in the source surface are split into a 3x3 grid, using the
   * different corner sizes for each corner, and the sides and center making up
   * the remaining pixels. The corners are then scaled using `scale` and fit
   * into the corners of the destination rectangle. The sides and center are
   * then stretched into place to cover the remaining destination rectangle.
   *
   * @param srcrect the Rect structure representing the rectangle to be used
   *                for the 9-grid, or nullptr to use the entire surface.
   * @param left_width the width, in pixels, of the left corners in `srcrect`.
   * @param right_width the width, in pixels, of the right corners in `srcrect`.
   * @param top_height the height, in pixels, of the top corners in `srcrect`.
   * @param bottom_height the height, in pixels, of the bottom corners in
   *                      `srcrect`.
   * @param scale the scale used to transform the corner of `srcrect` into the
   *              corner of `dstrect`, or 0.0f for an unscaled blit.
   * @param scaleMode scale algorithm to be used.
   * @param dst the Surface structure that is the blit target.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, or nullptr to fill the entire
   * surface.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Blit
   */
  void Blit9Grid(SurfaceParam src,
                 OptionalRef<const RectRaw> srcrect,
                 int left_width,
                 int right_width,
                 int top_height,
                 int bottom_height,
                 OptionalRef<const RectRaw> dstrect,
                 float scale = 1,
                 SDL_ScaleMode scaleMode = SCALEMODE_NEAREST);

  /**
   * Map an RGB triple to an opaque pixel value for a surface.
   *
   * This function maps the RGB color value to the specified pixel format and
   * returns the pixel value best approximating the given RGB color value for
   * the given pixel format.
   *
   * If the surface has a palette, the index of the closest matching color in
   * the palette will be returned.
   *
   * If the surface pixel format has an alpha component it will be returned as
   * all 1 bits (fully opaque).
   *
   * If the pixel format bpp (color depth) is less than 32-bpp then the unused
   * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
   * format the return value can be assigned to a Uint16, and similarly a Uint8
   * for an 8-bpp format).
   *
   * @param r the red component of the pixel in the range 0-255.
   * @param g the green component of the pixel in the range 0-255.
   * @param b the blue component of the pixel in the range 0-255.
   * @returns a pixel value.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.MapRGBA
   */
  Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b) const;

  /**
   * Map an RGBA quadruple to a pixel value for a surface.
   *
   * This function maps the RGBA color value to the specified pixel format and
   * returns the pixel value best approximating the given RGBA color value for
   * the given pixel format.
   *
   * If the surface pixel format has no alpha component the alpha value will be
   * ignored (as it will be in formats with a palette).
   *
   * If the surface has a palette, the index of the closest matching color in
   * the palette will be returned.
   *
   * If the pixel format bpp (color depth) is less than 32-bpp then the unused
   * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
   * format the return value can be assigned to a Uint16, and similarly a Uint8
   * for an 8-bpp format).
   *
   * @param r the red component of the pixel in the range 0-255.
   * @param g the green component of the pixel in the range 0-255.
   * @param b the blue component of the pixel in the range 0-255.
   * @param a the alpha component of the pixel in the range 0-255.
   * @returns a pixel value.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.MapRGB
   */
  Uint32 MapRGBA(ColorRaw c) const;

  /**
   * Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * Like GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r a pointer filled in with the red channel, 0-255, or nullptr to
   * ignore this channel.
   * @param g a pointer filled in with the green channel, 0-255, or nullptr to
   *          ignore this channel.
   * @param b a pointer filled in with the blue channel, 0-255, or nullptr to
   *          ignore this channel.
   * @param a a pointer filled in with the alpha channel, 0-255, or nullptr to
   *          ignore this channel.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReadPixel(const PointRaw& p,
                 Uint8* r,
                 Uint8* g,
                 Uint8* b,
                 Uint8* a) const;

  /**
   * Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * Like GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r a pointer filled in with the red channel, 0-255, or nullptr to
   * ignore this channel.
   * @param g a pointer filled in with the green channel, 0-255, or nullptr to
   *          ignore this channel.
   * @param b a pointer filled in with the blue channel, 0-255, or nullptr to
   *          ignore this channel.
   * @param a a pointer filled in with the alpha channel, 0-255, or nullptr to
   *          ignore this channel.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Color ReadPixel(const PointRaw& p) const;

  /**
   * Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r a pointer filled in with the red channel, normally in the range
   *          0-1, or nullptr to ignore this channel.
   * @param g a pointer filled in with the green channel, normally in the range
   *          0-1, or nullptr to ignore this channel.
   * @param b a pointer filled in with the blue channel, normally in the range
   *          0-1, or nullptr to ignore this channel.
   * @param a a pointer filled in with the alpha channel, normally in the range
   *          0-1, or nullptr to ignore this channel.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReadPixelFloat(const PointRaw& p,
                      float* r,
                      float* g,
                      float* b,
                      float* a) const;

  /**
   * Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r a pointer filled in with the red channel, normally in the range
   *          0-1, or nullptr to ignore this channel.
   * @param g a pointer filled in with the green channel, normally in the range
   *          0-1, or nullptr to ignore this channel.
   * @param b a pointer filled in with the blue channel, normally in the range
   *          0-1, or nullptr to ignore this channel.
   * @param a a pointer filled in with the alpha channel, normally in the range
   *          0-1, or nullptr to ignore this channel.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   */
  FColor ReadPixelFloat(const PointRaw& p) const;

  /**
   * Writes a single pixel to a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * Like MapColor, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r the red channel value, 0-255.
   * @param g the green channel value, 0-255.
   * @param b the blue channel value, 0-255.
   * @param a the alpha channel value, 0-255.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WritePixel(const PointRaw& p, ColorRaw c);

  /**
   * Writes a single pixel to a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r the red channel value, normally in the range 0-1.
   * @param g the green channel value, normally in the range 0-1.
   * @param b the blue channel value, normally in the range 0-1.
   * @param a the alpha channel value, normally in the range 0-1.
   * @throws Error on failure.
   *
   * @threadsafety This function can be called on different threads with
   *               different surfaces.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WritePixelFloat(const PointRaw& p, const FColorRaw& c);

  constexpr int GetWidth() const;

  constexpr int GetHeight() const;

  constexpr Point GetSize() const;

  constexpr int GetPitch() const;

  constexpr PixelFormat GetFormat() const;

  constexpr void* GetPixels() const;
};

/**
 * Allocate a new surface with a specific pixel format.
 *
 * The pixels of the new surface are initialized to zero.
 *
 * @param width the width of the surface.
 * @param height the height of the surface.
 * @param format the PixelFormat for the new surface's pixel format.
 * @returns the new Surface structure that is created or nullptr on failure;
 *          call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Surface
 * @sa Surface.Destroy
 */
inline Surface CreateSurface(const PointRaw& size, PixelFormat format)
{
  return Surface(size, format);
}

/**
 * Allocate a new surface with a specific pixel format and existing pixel
 * data.
 *
 * No copy is made of the pixel data. Pixel data is not managed automatically;
 * you must free the surface before you free the pixel data.
 *
 * Pitch is the offset in bytes from one row of pixels to the next, e.g.
 * `width*4` for `PIXELFORMAT_RGBA8888`.
 *
 * You may pass nullptr for pixels and 0 for pitch to create a surface that you
 * will fill in with valid values later.
 *
 * @param width the width of the surface.
 * @param height the height of the surface.
 * @param format the PixelFormat for the new surface's pixel format.
 * @param pixels a pointer to existing pixel data.
 * @param pitch the number of bytes between each row, including padding.
 * @returns the new Surface structure that is created or nullptr on failure;
 *          call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Surface
 * @sa Surface.Destroy
 */
inline Surface CreateSurfaceFrom(const PointRaw& size,
                                 PixelFormat format,
                                 void* pixels,
                                 int pitch)
{
  return Surface(size, format, pixels, pitch);
}

/**
 * Free a surface.
 *
 * It is safe to pass nullptr to this function.
 *
 * @param surface the Surface to free.
 *
 * @threadsafety No other thread should be using the surface when it is freed.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Surface
 * @sa Surface.Surface
 */
inline void DestroySurface(SurfaceRaw surface) { SDL_DestroySurface(surface); }

inline void Surface::Destroy() { DestroySurface(release()); }

/**
 * Get the properties associated with a surface.
 *
 * The following properties are understood by SDL:
 *
 * - `prop::Surface.SDR_WHITE_POINT_FLOAT`: for HDR10 and floating point
 *   surfaces, this defines the value of 100% diffuse white, with higher
 *   values being displayed in the High Dynamic Range headroom. This defaults
 *   to 203 for HDR10 surfaces and 1.0 for floating point surfaces.
 * - `prop::Surface.HDR_HEADROOM_FLOAT`: for HDR10 and floating point
 *   surfaces, this defines the maximum dynamic range used by the content, in
 *   terms of the SDR white point. This defaults to 0.0, which disables tone
 *   mapping.
 * - `prop::Surface.TONEMAP_OPERATOR_STRING`: the tone mapping operator
 *   used when compressing from a surface with high dynamic range to another
 *   with lower dynamic range. Currently this supports "chrome", which uses
 *   the same tone mapping that Chrome uses for HDR content, the form "*=N",
 *   where N is a floating point scale factor applied in linear space, and
 *   "none", which disables tone mapping. This defaults to "chrome".
 * - `prop::Surface.HOTSPOT_X_NUMBER`: the hotspot pixel offset from the
 *   left edge of the image, if this surface is being used as a cursor.
 * - `prop::Surface.HOTSPOT_Y_NUMBER`: the hotspot pixel offset from the
 *   top edge of the image, if this surface is being used as a cursor.
 *
 * @param surface the Surface structure to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetSurfaceProperties(SurfaceConstParam surface)
{
  return CheckError(SDL_GetSurfaceProperties(surface));
}

inline PropertiesRef Surface::GetProperties() const
{
  return SDL::GetSurfaceProperties(m_resource);
}

namespace prop::Surface {

constexpr auto SDR_WHITE_POINT_FLOAT = SDL_PROP_SURFACE_SDR_WHITE_POINT_FLOAT;

constexpr auto HDR_HEADROOM_FLOAT = SDL_PROP_SURFACE_HDR_HEADROOM_FLOAT;

constexpr auto TONEMAP_OPERATOR_STRING =
  SDL_PROP_SURFACE_TONEMAP_OPERATOR_STRING;

#if SDL_VERSION_ATLEAST(3, 2, 6)

constexpr auto HOTSPOT_X_NUMBER = SDL_PROP_SURFACE_HOTSPOT_X_NUMBER;

#endif // SDL_VERSION_ATLEAST(3, 2, 6)

#if SDL_VERSION_ATLEAST(3, 2, 6)

constexpr auto HOTSPOT_Y_NUMBER = SDL_PROP_SURFACE_HOTSPOT_Y_NUMBER;

#endif // SDL_VERSION_ATLEAST(3, 2, 6)

} // namespace prop::Surface

/**
 * Set the colorspace used by a surface.
 *
 * Setting the colorspace doesn't change the pixels, only how they are
 * interpreted in color operations.
 *
 * @param surface the Surface structure to update.
 * @param colorspace an Colorspace value describing the surface
 *                   colorspace.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.GetColorspace
 */
inline void SetSurfaceColorspace(SurfaceParam surface, Colorspace colorspace)
{
  CheckError(SDL_SetSurfaceColorspace(surface, colorspace));
}

inline void Surface::SetColorspace(Colorspace colorspace)
{
  SDL::SetSurfaceColorspace(m_resource, colorspace);
}

/**
 * Get the colorspace used by a surface.
 *
 * The colorspace defaults to COLORSPACE_SRGB_LINEAR for floating point
 * formats, COLORSPACE_HDR10 for 10-bit formats, COLORSPACE_SRGB for
 * other RGB surfaces and COLORSPACE_BT709_FULL for YUV textures.
 *
 * @param surface the Surface structure to query.
 * @returns the colorspace used by the surface, or COLORSPACE_UNKNOWN if
 *          the surface is nullptr.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.SetColorspace
 */
inline Colorspace GetSurfaceColorspace(SurfaceConstParam surface)
{
  return SDL_GetSurfaceColorspace(surface);
}

inline Colorspace Surface::GetColorspace() const
{
  return SDL::GetSurfaceColorspace(m_resource);
}

/**
 * Create a palette and associate it with a surface.
 *
 * This function creates a palette compatible with the provided surface. The
 * palette is then returned for you to modify, and the surface will
 * automatically use the new palette in future operations. You do not need to
 * destroy the returned palette, it will be freed when the reference count
 * reaches 0, usually when the surface is destroyed.
 *
 * Bitmap surfaces (with format PIXELFORMAT_INDEX1LSB or
 * PIXELFORMAT_INDEX1MSB) will have the palette initialized with 0 as
 * white and 1 as black. Other surfaces will get a palette initialized with
 * white in every entry.
 *
 * If this function is called for a surface that already has a palette, a new
 * palette will be created to replace it.
 *
 * @param surface the Surface structure to update.
 * @returns a new Palette structure on success.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Palette.SetColors
 */
inline Palette CreateSurfacePalette(SurfaceParam surface)
{
  return CheckError(SDL_CreateSurfacePalette(surface));
}

inline Palette Surface::CreatePalette()
{
  return SDL::CreateSurfacePalette(m_resource);
}

/**
 * Set the palette used by a surface.
 *
 * Setting the palette keeps an internal reference to the palette, which can
 * be safely destroyed afterwards.
 *
 * A single palette can be shared with many surfaces.
 *
 * @param surface the Surface structure to update.
 * @param palette the Palette structure to use.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Palette.Palette
 * @sa Surface.GetPalette
 */
inline void SetSurfacePalette(SurfaceParam surface, PaletteParam palette)
{
  CheckError(SDL_SetSurfacePalette(surface, palette));
}

inline void Surface::SetPalette(PaletteParam palette)
{
  SDL::SetSurfacePalette(m_resource, palette);
}

/**
 * Get the palette used by a surface.
 *
 * @param surface the Surface structure to query.
 * @returns a pointer to the palette used by the surface, or nullptr if there is
 *          no palette used.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.SetPalette
 */
inline Palette GetSurfacePalette(SurfaceConstParam surface)
{
  return SDL_GetSurfacePalette(surface);
}

inline Palette Surface::GetPalette() const
{
  return SDL::GetSurfacePalette(m_resource);
}

/**
 * Add an alternate version of a surface.
 *
 * This function adds an alternate version of this surface, usually used for
 * content with high DPI representations like cursors or icons. The size,
 * format, and content do not need to match the original surface, and these
 * alternate versions will not be updated when the original surface changes.
 *
 * This function adds a reference to the alternate version, so you should call
 * Surface.Destroy() on the image after this call.
 *
 * @param surface the Surface structure to update.
 * @param image a pointer to an alternate Surface to associate with this
 *              surface.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.RemoveAlternateImages
 * @sa Surface.GetImages
 * @sa Surface.HasAlternateImages
 */
inline void AddSurfaceAlternateImage(SurfaceParam surface, SurfaceParam image)
{
  CheckError(SDL_AddSurfaceAlternateImage(surface, image));
}

inline void Surface::AddAlternateImage(SurfaceParam image)
{
  SDL::AddSurfaceAlternateImage(m_resource, image);
}

/**
 * Return whether a surface has alternate versions available.
 *
 * @param surface the Surface structure to query.
 * @returns true if alternate versions are available or false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.AddAlternateImage
 * @sa Surface.RemoveAlternateImages
 * @sa Surface.GetImages
 */
inline bool SurfaceHasAlternateImages(SurfaceConstParam surface)
{
  return SDL_SurfaceHasAlternateImages(surface);
}

inline bool Surface::HasAlternateImages() const
{
  return SDL::SurfaceHasAlternateImages(m_resource);
}

/**
 * Get an array including all versions of a surface.
 *
 * This returns all versions of a surface, with the surface being queried as
 * the first element in the returned array.
 *
 * Freeing the array of surfaces does not affect the surfaces in the array.
 * They are still referenced by the surface being queried and will be cleaned
 * up normally.
 *
 * @param surface the Surface structure to query.
 * @param count a pointer filled in with the number of surface pointers
 *              returned, may be nullptr.
 * @returns a nullptr terminated array of Surface pointers or nullptr on
 *          failure; call GetError() for more information. This should be
 *          freed with free() when it is no longer needed.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.AddAlternateImage
 * @sa Surface.RemoveAlternateImages
 * @sa Surface.HasAlternateImages
 */
inline OwnArray<SurfaceRaw> GetSurfaceImages(SurfaceConstParam surface)
{
  return SDL_GetSurfaceImages(surface);
}

inline OwnArray<SurfaceRaw> Surface::GetImages() const
{
  return SDL::GetSurfaceImages(m_resource);
}

/**
 * Remove all alternate versions of a surface.
 *
 * This function removes a reference from all the alternative versions,
 * destroying them if this is the last reference to them.
 *
 * @param surface the Surface structure to update.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.AddAlternateImage
 * @sa Surface.GetImages
 * @sa Surface.HasAlternateImages
 */
inline void RemoveSurfaceAlternateImages(SurfaceParam surface)
{
  SDL_RemoveSurfaceAlternateImages(surface);
}

inline void Surface::RemoveAlternateImages()
{
  SDL::RemoveSurfaceAlternateImages(m_resource);
}

/**
 * Set up a surface for directly accessing the pixels.
 *
 * Between calls to Surface.Lock() / Surface.Unlock(), you can write to
 * and read from `surface->pixels`, using the pixel format stored in
 * `surface->format`. Once you are done accessing the surface, you should use
 * Surface.Unlock() to release it.
 *
 * Not all surfaces require locking. If `Surface.MustLock(surface)` evaluates to
 * 0, then you can read and write to the surface at any time, and the pixel
 * format of the surface will not change.
 *
 * @param surface the Surface structure to be locked.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces. The locking referred to by this function
 *               is making the pixels available for direct access, not
 *               thread-safe locking.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.MustLock
 * @sa Surface.Unlock
 */
inline void LockSurface(SurfaceParam surface)
{
  CheckError(SDL_LockSurface(surface));
}

inline void Surface::Lock() { SDL::LockSurface(m_resource); }

/**
 * Release a surface after directly accessing the pixels.
 *
 * @param surface the Surface structure to be unlocked.
 *
 * @threadsafety This function is not thread safe. The locking referred to by
 *               this function is making the pixels available for direct
 *               access, not thread-safe locking.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Lock
 */
inline void UnlockSurface(SurfaceParam surface) { SDL_UnlockSurface(surface); }

inline void Surface::Unlock() { SDL::UnlockSurface(m_resource); }

/**
 * Load a BMP image from a seekable SDL data stream.
 *
 * The new surface should be freed with Surface.Destroy(). Not doing so
 * will result in a memory leak.
 *
 * @param src the data stream for the surface.
 * @param closeio if true, calls IOStream.Close() on `src` before returning,
 * even in the case of an error.
 * @returns a pointer to a new Surface structure or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Destroy
 * @sa Surface.LoadBMP
 * @sa Surface.SaveBMP
 */
inline Surface LoadBMP(IOStreamParam src, bool closeio = false)
{
  return Surface(SDL_LoadBMP_IO(src, closeio));
}

/**
 * Load a BMP image from a file.
 *
 * The new surface should be freed with Surface.Destroy(). Not doing so
 * will result in a memory leak.
 *
 * @param file the BMP file to load.
 * @returns a pointer to a new Surface structure or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Destroy
 * @sa Surface.LoadBMP
 * @sa Surface.SaveBMP
 */
inline Surface LoadBMP(StringParam file) { return Surface(SDL_LoadBMP(file)); }

inline Surface Surface::LoadBMP(IOStreamParam src, bool closeio)
{
  return SDL::LoadBMP(src, closeio);
}

inline Surface Surface::LoadBMP(StringParam file)
{
  return SDL::LoadBMP(std::move(file));
}

/**
 * Save a surface to a seekable SDL data stream in BMP format.
 *
 * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
 * BMP directly. Other RGB formats with 8-bit or higher get converted to a
 * 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
 * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
 * not supported.
 *
 * @param surface the Surface structure containing the image to be saved.
 * @param dst a data stream to save to.
 * @param closeio if true, calls IOStream.Close() on `dst` before returning,
 * even in the case of an error.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.LoadBMP
 * @sa Surface.SaveBMP
 */
inline void SaveBMP(SurfaceConstParam surface,
                    IOStreamParam dst,
                    bool closeio = false)
{
  CheckError(SDL_SaveBMP_IO(surface, dst, closeio));
}

/**
 * Save a surface to a file in BMP format.
 *
 * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
 * BMP directly. Other RGB formats with 8-bit or higher get converted to a
 * 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
 * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
 * not supported.
 *
 * @param surface the Surface structure containing the image to be saved.
 * @param file a file to save to.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.LoadBMP
 * @sa Surface.SaveBMP
 */
inline void SaveBMP(SurfaceConstParam surface, StringParam file)
{
  CheckError(SDL_SaveBMP(surface, file));
}

inline void Surface::SaveBMP(IOStreamParam dst, bool closeio) const
{
  SDL::SaveBMP(m_resource, dst, closeio);
}

inline void Surface::SaveBMP(StringParam file) const
{
  SDL::SaveBMP(m_resource, std::move(file));
}

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Load a PNG image from a seekable SDL data stream.
 *
 * The new surface should be freed with Surface.Destroy(). Not doing so
 * will result in a memory leak.
 *
 * @param src the data stream for the surface.
 * @param closeio if true, calls IOStream.Close() on `src` before returning,
 * even in the case of an error.
 * @returns a pointer to a new Surface structure or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.4.0.
 *
 * @sa Surface.Destroy
 * @sa Surface.LoadPNG
 * @sa Surface.SavePNG
 */
inline Surface LoadPNG(IOStreamParam src, bool closeio = false)
{
  return Surface(SDL_LoadPNG_IO(src, closeio));
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Load a PNG image from a file.
 *
 * The new surface should be freed with Surface.Destroy(). Not doing so
 * will result in a memory leak.
 *
 * @param file the PNG file to load.
 * @returns a pointer to a new Surface structure or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.4.0.
 *
 * @sa Surface.Destroy
 * @sa Surface.LoadPNG
 * @sa Surface.SavePNG
 */
inline Surface LoadPNG(StringParam file) { return Surface(SDL_LoadPNG(file)); }

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

inline Surface Surface::LoadPNG(IOStreamParam src, bool closeio)
{
  return SDL::LoadPNG(src, closeio);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

inline Surface Surface::LoadPNG(StringParam file)
{
  return SDL::LoadPNG(std::move(file));
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Save a surface to a seekable SDL data stream in PNG format.
 *
 * @param surface the Surface structure containing the image to be saved.
 * @param dst a data stream to save to.
 * @param closeio if true, calls IOStream.Close() on `dst` before returning,
 * even in the case of an error.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.4.0.
 *
 * @sa Surface.LoadPNG
 * @sa Surface.SavePNG
 */
inline void SavePNG(SurfaceConstParam surface,
                    IOStreamParam dst,
                    bool closeio = false)
{
  CheckError(SDL_SavePNG_IO(surface, dst, closeio));
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Save a surface to a file in PNG format.
 *
 * @param surface the Surface structure containing the image to be saved.
 * @param file a file to save to.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.4.0.
 *
 * @sa Surface.LoadPNG
 * @sa Surface.SavePNG
 */
inline void SavePNG(SurfaceConstParam surface, StringParam file)
{
  CheckError(SDL_SavePNG(surface, file));
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

inline void Surface::SavePNG(IOStreamParam dst, bool closeio) const
{
  SDL::SavePNG(m_resource, dst, closeio);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

inline void Surface::SavePNG(StringParam file) const
{
  SDL::SavePNG(m_resource, std::move(file));
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Set the RLE acceleration hint for a surface.
 *
 * If RLE is enabled, color key and alpha blending blits are much faster, but
 * the surface must be locked before directly accessing the pixels.
 *
 * @param surface the Surface structure to optimize.
 * @param enabled true to enable RLE acceleration, false to disable it.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Blit
 * @sa Surface.Lock
 * @sa Surface.Unlock
 */
inline void SetSurfaceRLE(SurfaceParam surface, bool enabled)
{
  CheckError(SDL_SetSurfaceRLE(surface, enabled));
}

inline void Surface::SetRLE(bool enabled)
{
  SDL::SetSurfaceRLE(m_resource, enabled);
}

/**
 * Returns whether the surface is RLE enabled.
 *
 * It is safe to pass a nullptr `surface` here; it will return false.
 *
 * @param surface the Surface structure to query.
 * @returns true if the surface is RLE enabled, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.SetRLE
 */
inline bool SurfaceHasRLE(SurfaceConstParam surface)
{
  return SDL_SurfaceHasRLE(surface);
}

inline bool Surface::HasRLE() const { return SDL::SurfaceHasRLE(m_resource); }

/**
 * Set the color key (transparent pixel) in a surface.
 *
 * The color key defines a pixel value that will be treated as transparent in
 * a blit. For example, one can use this to specify that cyan pixels should be
 * considered transparent, and therefore not rendered.
 *
 * It is a pixel of the format used by the surface, as generated by
 * MapRGB().
 *
 * @param surface the Surface structure to update.
 * @param enabled true to enable color key, false to disable color key.
 * @param key the transparent pixel.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.GetColorKey
 * @sa Surface.SetRLE
 * @sa Surface.HasColorKey
 */
inline void SetSurfaceColorKey(SurfaceParam surface, std::optional<Uint32> key)
{
  CheckError(SDL_SetSurfaceColorKey(surface, key));
}

inline void Surface::SetColorKey(std::optional<Uint32> key)
{
  SDL::SetSurfaceColorKey(m_resource, key);
}

inline void ClearSurfaceColorKey(SurfaceParam surface)
{
  static_assert(false, "Not implemented");
}

inline void Surface::ClearColorKey() { SDL::ClearSurfaceColorKey(m_resource); }

/**
 * Returns whether the surface has a color key.
 *
 * It is safe to pass a nullptr `surface` here; it will return false.
 *
 * @param surface the Surface structure to query.
 * @returns true if the surface has a color key, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.SetColorKey
 * @sa Surface.GetColorKey
 */
inline bool SurfaceHasColorKey(SurfaceConstParam surface)
{
  return SDL_SurfaceHasColorKey(surface);
}

inline bool Surface::HasColorKey() const
{
  return SDL::SurfaceHasColorKey(m_resource);
}

/**
 * Get the color key (transparent pixel) for a surface.
 *
 * The color key is a pixel of the format used by the surface, as generated by
 * MapRGB().
 *
 * If the surface doesn't have color key enabled this function returns false.
 *
 * @param surface the Surface structure to query.
 * @param key a pointer filled in with the transparent pixel.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.SetColorKey
 * @sa Surface.HasColorKey
 */
inline std::optional<Uint32> GetSurfaceColorKey(SurfaceConstParam surface)
{
  return CheckError(SDL_GetSurfaceColorKey(surface));
}

inline std::optional<Uint32> Surface::GetColorKey() const
{
  return SDL::GetSurfaceColorKey(m_resource);
}

/**
 * Set an additional color value multiplied into blit operations.
 *
 * When this surface is blitted, during the blit operation each source color
 * channel is modulated by the appropriate color value according to the
 * following formula:
 *
 * `srcC = srcC * (color / 255)`
 *
 * @param surface the Surface structure to update.
 * @param r the red color value multiplied into blit operations.
 * @param g the green color value multiplied into blit operations.
 * @param b the blue color value multiplied into blit operations.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.GetColorMod
 * @sa Surface.SetAlphaMod
 */
inline void SetSurfaceColorMod(SurfaceParam surface, Uint8 r, Uint8 g, Uint8 b)
{
  CheckError(SDL_SetSurfaceColorMod(surface, r, g, b));
}

inline void Surface::SetColorMod(Uint8 r, Uint8 g, Uint8 b)
{
  SDL::SetSurfaceColorMod(m_resource, r, g, b);
}

/**
 * Get the additional color value multiplied into blit operations.
 *
 * @param surface the Surface structure to query.
 * @param r a pointer filled in with the current red color value.
 * @param g a pointer filled in with the current green color value.
 * @param b a pointer filled in with the current blue color value.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.GetAlphaMod
 * @sa Surface.SetColorMod
 */
inline void GetSurfaceColorMod(SurfaceConstParam surface,
                               Uint8* r,
                               Uint8* g,
                               Uint8* b)
{
  CheckError(SDL_GetSurfaceColorMod(surface, r, g, b));
}

inline void Surface::GetColorMod(Uint8* r, Uint8* g, Uint8* b) const
{
  SDL::GetSurfaceColorMod(m_resource, r, g, b);
}

/**
 * Set an additional alpha value used in blit operations.
 *
 * When this surface is blitted, during the blit operation the source alpha
 * value is modulated by this alpha value according to the following formula:
 *
 * `srcA = srcA * (alpha / 255)`
 *
 * @param surface the Surface structure to update.
 * @param alpha the alpha value multiplied into blit operations.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.GetAlphaMod
 * @sa Surface.SetColorMod
 */
inline void SetSurfaceAlphaMod(SurfaceParam surface, Uint8 alpha)
{
  CheckError(SDL_SetSurfaceAlphaMod(surface, alpha));
}

inline void Surface::SetAlphaMod(Uint8 alpha)
{
  SDL::SetSurfaceAlphaMod(m_resource, alpha);
}

/**
 * Get the additional alpha value used in blit operations.
 *
 * @param surface the Surface structure to query.
 * @param alpha a pointer filled in with the current alpha value.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.GetColorMod
 * @sa Surface.SetAlphaMod
 */
inline Uint8 GetSurfaceAlphaMod(SurfaceConstParam surface)
{
  return CheckError(SDL_GetSurfaceAlphaMod(surface));
}

inline Uint8 Surface::GetAlphaMod() const
{
  return SDL::GetSurfaceAlphaMod(m_resource);
}

inline void SetSurfaceMod(SurfaceParam surface, Color color)
{
  static_assert(false, "Not implemented");
}

inline void Surface::SetMod(Color color)
{
  SDL::SetSurfaceMod(m_resource, color);
}

inline Color GetSurfaceMod(SurfaceConstParam surface)
{
  static_assert(false, "Not implemented");
}

inline Color Surface::GetMod() const { return SDL::GetSurfaceMod(m_resource); }

/**
 * Set the blend mode used for blit operations.
 *
 * To copy a surface to another surface (or texture) without blending with the
 * existing data, the blendmode of the SOURCE surface should be set to
 * `BLENDMODE_NONE`.
 *
 * @param surface the Surface structure to update.
 * @param blendMode the BlendMode to use for blit blending.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.GetBlendMode
 */
inline void SetSurfaceBlendMode(SurfaceParam surface, BlendMode blendMode)
{
  CheckError(SDL_SetSurfaceBlendMode(surface, blendMode));
}

inline void Surface::SetBlendMode(BlendMode blendMode)
{
  SDL::SetSurfaceBlendMode(m_resource, blendMode);
}

/**
 * Get the blend mode used for blit operations.
 *
 * @param surface the Surface structure to query.
 * @param blendMode a pointer filled in with the current BlendMode.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.SetBlendMode
 */
inline BlendMode GetSurfaceBlendMode(SurfaceConstParam surface)
{
  return CheckError(SDL_GetSurfaceBlendMode(surface));
}

inline BlendMode Surface::GetBlendMode() const
{
  return SDL::GetSurfaceBlendMode(m_resource);
}

/**
 * Set the clipping rectangle for a surface.
 *
 * When `surface` is the destination of a blit, only the area within the clip
 * rectangle is drawn into.
 *
 * Note that blits are automatically clipped to the edges of the source and
 * destination surfaces.
 *
 * @param surface the Surface structure to be clipped.
 * @param rect the Rect structure representing the clipping rectangle, or
 *             nullptr to disable clipping.
 * @returns true if the rectangle intersects the surface, otherwise false and
 *          blits will be completely clipped.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.GetClipRect
 */
inline bool SetSurfaceClipRect(SurfaceParam surface,
                               OptionalRef<const RectRaw> rect)
{
  return SDL_SetSurfaceClipRect(surface, rect);
}

inline bool Surface::SetClipRect(OptionalRef<const RectRaw> rect)
{
  return SDL::SetSurfaceClipRect(m_resource, rect);
}

inline void ResetSurfaceClipRect(SurfaceParam surface)
{
  static_assert(false, "Not implemented");
}

inline void Surface::ResetClipRect() { SDL::ResetSurfaceClipRect(m_resource); }

/**
 * Get the clipping rectangle for a surface.
 *
 * When `surface` is the destination of a blit, only the area within the clip
 * rectangle is drawn into.
 *
 * @param surface the Surface structure representing the surface to be
 *                clipped.
 * @param rect an Rect structure filled in with the clipping rectangle for
 *             the surface.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.SetClipRect
 */
inline Rect GetSurfaceClipRect(SurfaceConstParam surface)
{
  return CheckError(SDL_GetSurfaceClipRect(surface));
}

inline Rect Surface::GetClipRect() const
{
  return SDL::GetSurfaceClipRect(m_resource);
}

/**
 * Flip a surface vertically or horizontally.
 *
 * @param surface the surface to flip.
 * @param flip the direction to flip.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void FlipSurface(SurfaceParam surface, FlipMode flip)
{
  CheckError(SDL_FlipSurface(surface, flip));
}

inline void Surface::Flip(FlipMode flip) { SDL::FlipSurface(m_resource, flip); }

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Return a copy of a surface rotated clockwise a number of degrees.
 *
 * The angle of rotation can be negative for counter-clockwise rotation.
 *
 * When the rotation isn't a multiple of 90 degrees, the resulting surface is
 * larger than the original, with the background filled in with the colorkey,
 * if available, or RGBA 255/255/255/0 if not.
 *
 * @param surface the surface to rotate.
 * @param angle the rotation angle, in degrees.
 * @returns a rotated copy of the surface or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.4.0.
 */
inline Surface RotateSurface(SurfaceParam surface, float angle)
{
  return SDL_RotateSurface(surface, angle);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

inline Surface Surface::Rotate(float angle)
{
  return SDL::RotateSurface(m_resource, angle);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Creates a new surface identical to the existing surface.
 *
 * If the original surface has alternate images, the new surface will have a
 * reference to them as well.
 *
 * The returned surface should be freed with Surface.Destroy().
 *
 * @param surface the surface to duplicate.
 * @returns a copy of the surface or nullptr on failure; call GetError() for
 *          more information.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Destroy
 */
inline Surface DuplicateSurface(SurfaceConstParam surface)
{
  return SDL_DuplicateSurface(surface);
}

inline Surface Surface::Duplicate() const
{
  return SDL::DuplicateSurface(m_resource);
}

/**
 * Creates a new surface identical to the existing surface, scaled to the
 * desired size.
 *
 * The returned surface should be freed with Surface.Destroy().
 *
 * @param surface the surface to duplicate and scale.
 * @param width the width of the new surface.
 * @param height the height of the new surface.
 * @param scaleMode the ScaleMode to be used.
 * @returns a copy of the surface or nullptr on failure; call GetError() for
 *          more information.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Destroy
 */
inline Surface ScaleSurface(SurfaceConstParam surface,
                            const PointRaw& size,
                            ScaleMode scaleMode)
{
  return SDL_ScaleSurface(surface, size, scaleMode);
}

inline Surface Surface::Scale(const PointRaw& size, ScaleMode scaleMode) const
{
  return SDL::ScaleSurface(m_resource, size, scaleMode);
}

/**
 * Copy an existing surface to a new surface of the specified format.
 *
 * This function is used to optimize images for faster *repeat* blitting. This
 * is accomplished by converting the original and storing the result as a new
 * surface. The new, optimized surface can then be used as the source for
 * future blits, making them faster.
 *
 * If you are converting to an indexed surface and want to map colors to a
 * palette, you can use Surface.Convert() instead.
 *
 * If the original surface has alternate images, the new surface will have a
 * reference to them as well.
 *
 * @param surface the existing Surface structure to convert.
 * @param format the new pixel format.
 * @returns the new Surface structure that is created or nullptr on failure;
 *          call GetError() for more information.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Convert
 * @sa Surface.Destroy
 */
inline Surface ConvertSurface(SurfaceConstParam surface, PixelFormat format)
{
  return SDL_ConvertSurface(surface, format);
}

inline Surface Surface::Convert(PixelFormat format) const
{
  return SDL::ConvertSurface(m_resource, format);
}

inline Surface Surface::Convert(PixelFormat format,
                                PaletteParam palette,
                                Colorspace colorspace,
                                PropertiesParam props) const
{
  return SDL::ConvertSurfaceAndColorspace(
    m_resource, format, palette, colorspace, props);
}

/**
 * Copy an existing surface to a new surface of the specified format and
 * colorspace.
 *
 * This function converts an existing surface to a new format and colorspace
 * and returns the new surface. This will perform any pixel format and
 * colorspace conversion needed.
 *
 * If the original surface has alternate images, the new surface will have a
 * reference to them as well.
 *
 * @param surface the existing Surface structure to convert.
 * @param format the new pixel format.
 * @param palette an optional palette to use for indexed formats, may be
 * nullptr.
 * @param colorspace the new colorspace.
 * @param props an Properties with additional color properties, or 0.
 * @returns the new Surface structure that is created or nullptr on failure;
 *          call GetError() for more information.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Convert
 * @sa Surface.Destroy
 */
inline Surface ConvertSurfaceAndColorspace(SurfaceConstParam surface,
                                           PixelFormat format,
                                           PaletteParam palette,
                                           Colorspace colorspace,
                                           PropertiesParam props)
{
  return SDL_ConvertSurfaceAndColorspace(
    surface, format, palette, colorspace, props);
}

/**
 * Copy a block of pixels of one format to another format.
 *
 * @param width the width of the block to copy, in pixels.
 * @param height the height of the block to copy, in pixels.
 * @param src_format an PixelFormat value of the `src` pixels format.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an PixelFormat value of the `dst` pixels format.
 * @param dst a pointer to be filled in with new pixel data.
 * @param dst_pitch the pitch of the destination pixels, in bytes.
 * @throws Error on failure.
 *
 * @threadsafety The same destination pixels should not be used from two
 *               threads at once. It is safe to use the same source pixels
 *               from multiple threads.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ConvertPixelsAndColorspace
 */
inline void ConvertPixels(const PointRaw& size,
                          PixelFormat src_format,
                          const void* src,
                          int src_pitch,
                          PixelFormat dst_format,
                          void* dst,
                          int dst_pitch)
{
  CheckError(SDL_ConvertPixels(
    size, src_format, src, src_pitch, dst_format, dst, dst_pitch));
}

/**
 * Copy a block of pixels of one format and colorspace to another format and
 * colorspace.
 *
 * @param width the width of the block to copy, in pixels.
 * @param height the height of the block to copy, in pixels.
 * @param src_format an PixelFormat value of the `src` pixels format.
 * @param src_colorspace an Colorspace value describing the colorspace of
 *                       the `src` pixels.
 * @param src_properties an Properties with additional source color
 *                       properties, or 0.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an PixelFormat value of the `dst` pixels format.
 * @param dst_colorspace an Colorspace value describing the colorspace of
 *                       the `dst` pixels.
 * @param dst_properties an Properties with additional destination color
 *                       properties, or 0.
 * @param dst a pointer to be filled in with new pixel data.
 * @param dst_pitch the pitch of the destination pixels, in bytes.
 * @throws Error on failure.
 *
 * @threadsafety The same destination pixels should not be used from two
 *               threads at once. It is safe to use the same source pixels
 *               from multiple threads.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ConvertPixels
 */
inline void ConvertPixelsAndColorspace(const PointRaw& size,
                                       PixelFormat src_format,
                                       Colorspace src_colorspace,
                                       PropertiesParam src_properties,
                                       const void* src,
                                       int src_pitch,
                                       PixelFormat dst_format,
                                       Colorspace dst_colorspace,
                                       PropertiesParam dst_properties,
                                       void* dst,
                                       int dst_pitch)
{
  CheckError(SDL_ConvertPixelsAndColorspace(size,
                                            src_format,
                                            src_colorspace,
                                            src_properties,
                                            src,
                                            src_pitch,
                                            dst_format,
                                            dst_colorspace,
                                            dst_properties,
                                            dst,
                                            dst_pitch));
}

/**
 * Premultiply the alpha on a block of pixels.
 *
 * This is safe to use with src == dst, but not for other overlapping areas.
 *
 * @param width the width of the block to convert, in pixels.
 * @param height the height of the block to convert, in pixels.
 * @param src_format an PixelFormat value of the `src` pixels format.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an PixelFormat value of the `dst` pixels format.
 * @param dst a pointer to be filled in with premultiplied pixel data.
 * @param dst_pitch the pitch of the destination pixels, in bytes.
 * @param linear true to convert from sRGB to linear space for the alpha
 *               multiplication, false to do multiplication in sRGB space.
 * @throws Error on failure.
 *
 * @threadsafety The same destination pixels should not be used from two
 *               threads at once. It is safe to use the same source pixels
 *               from multiple threads.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void PremultiplyAlpha(const PointRaw& size,
                             PixelFormat src_format,
                             const void* src,
                             int src_pitch,
                             PixelFormat dst_format,
                             void* dst,
                             int dst_pitch,
                             bool linear)
{
  CheckError(SDL_PremultiplyAlpha(
    size, src_format, src, src_pitch, dst_format, dst, dst_pitch, linear));
}

/**
 * Premultiply the alpha in a surface.
 *
 * This is safe to use with src == dst, but not for other overlapping areas.
 *
 * @param surface the surface to modify.
 * @param linear true to convert from sRGB to linear space for the alpha
 *               multiplication, false to do multiplication in sRGB space.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void PremultiplySurfaceAlpha(SurfaceParam surface, bool linear)
{
  CheckError(SDL_PremultiplySurfaceAlpha(surface, linear));
}

inline void Surface::PremultiplyAlpha(bool linear)
{
  SDL::PremultiplySurfaceAlpha(m_resource, linear);
}

/**
 * Clear a surface with a specific color, with floating point precision.
 *
 * This function handles all surface formats, and ignores any clip rectangle.
 *
 * If the surface is YUV, the color is assumed to be in the sRGB colorspace,
 * otherwise the color is assumed to be in the colorspace of the suface.
 *
 * @param surface the Surface to clear.
 * @param r the red component of the pixel, normally in the range 0-1.
 * @param g the green component of the pixel, normally in the range 0-1.
 * @param b the blue component of the pixel, normally in the range 0-1.
 * @param a the alpha component of the pixel, normally in the range 0-1.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ClearSurface(SurfaceParam surface, const FColorRaw& c)
{
  CheckError(SDL_ClearSurface(surface, c));
}

inline void Surface::Clear(const FColorRaw& c)
{
  SDL::ClearSurface(m_resource, c);
}

/**
 * Perform a fast fill of a rectangle with a specific color.
 *
 * `color` should be a pixel of the format used by the surface, and can be
 * generated by MapRGB() or MapColor(). If the color value contains an
 * alpha component then the destination is simply filled with that alpha
 * information, no blending takes place.
 *
 * If there is a clip rectangle set on the destination (set via
 * Surface.SetClipRect()), then this function will fill based on the
 * intersection of the clip rectangle and `rect`.
 *
 * @param dst the Surface structure that is the drawing target.
 * @param rect the Rect structure representing the rectangle to fill, or
 *             nullptr to fill the entire surface.
 * @param color the color to fill with.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.FillRects
 */
inline void FillSurfaceRect(SurfaceParam dst,
                            OptionalRef<const RectRaw> rect,
                            Uint32 color)
{
  CheckError(SDL_FillSurfaceRect(dst, rect, color));
}

inline void Surface::FillRect(OptionalRef<const RectRaw> rect, Uint32 color)
{
  SDL::FillSurfaceRect(m_resource, rect, color);
}

inline void FillSurface(SurfaceParam dst, Uint32 color)
{
  static_assert(false, "Not implemented");
}

inline void Surface::Fill(Uint32 color) { SDL::FillSurface(m_resource, color); }

/**
 * Perform a fast fill of a set of rectangles with a specific color.
 *
 * `color` should be a pixel of the format used by the surface, and can be
 * generated by MapRGB() or MapColor(). If the color value contains an
 * alpha component then the destination is simply filled with that alpha
 * information, no blending takes place.
 *
 * If there is a clip rectangle set on the destination (set via
 * Surface.SetClipRect()), then this function will fill based on the
 * intersection of the clip rectangle and `rect`.
 *
 * @param dst the Surface structure that is the drawing target.
 * @param rects an array of SDL_Rects representing the rectangles to fill.
 * @param count the number of rectangles in the array.
 * @param color the color to fill with.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.FillRect
 */
inline void FillSurfaceRects(SurfaceParam dst,
                             SpanRef<const RectRaw> rects,
                             Uint32 color)
{
  CheckError(SDL_FillSurfaceRects(dst, rects, color));
}

inline void Surface::FillRects(SpanRef<const RectRaw> rects, Uint32 color)
{
  SDL::FillSurfaceRects(m_resource, rects, color);
}

/**
 * Performs a fast blit from the source surface to the destination surface
 * with clipping.
 *
 * If either `srcrect` or `dstrect` are nullptr, the entire surface (`src` or
 * `dst`) is copied while ensuring clipping to `dst->clip_rect`.
 *
 * The blit function should not be called on a locked surface.
 *
 * The blit semantics for surfaces with and without blending and colorkey are
 * defined as follows:
 *
 * ```
 *    RGBA->RGB:
 *      Source surface blend mode set to BLENDMODE_BLEND:
 *       alpha-blend (using the source alpha-channel and per-surface alpha)
 *       SDL_SRCCOLORKEY ignored.
 *     Source surface blend mode set to BLENDMODE_NONE:
 *       copy RGB.
 *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
 *       RGB values of the source color key, ignoring alpha in the
 *       comparison.
 *
 *   RGB->RGBA:
 *     Source surface blend mode set to BLENDMODE_BLEND:
 *       alpha-blend (using the source per-surface alpha)
 *     Source surface blend mode set to BLENDMODE_NONE:
 *       copy RGB, set destination alpha to source per-surface alpha value.
 *     both:
 *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
 *       source color key.
 *
 *   RGBA->RGBA:
 *     Source surface blend mode set to BLENDMODE_BLEND:
 *       alpha-blend (using the source alpha-channel and per-surface alpha)
 *       SDL_SRCCOLORKEY ignored.
 *     Source surface blend mode set to BLENDMODE_NONE:
 *       copy all of RGBA to the destination.
 *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
 *       RGB values of the source color key, ignoring alpha in the
 *       comparison.
 *
 *   RGB->RGB:
 *     Source surface blend mode set to BLENDMODE_BLEND:
 *       alpha-blend (using the source per-surface alpha)
 *     Source surface blend mode set to BLENDMODE_NONE:
 *       copy RGB.
 *     both:
 *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
 *       source color key.
 * ```
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied, or nullptr to copy the entire surface.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the x and y position in
 *                the destination surface, or nullptr for (0,0). The width and
 *                height are ignored, and are copied from `srcrect`. If you
 *                want a specific width and height, you should use
 *                Surface.BlitScaled().
 * @throws Error on failure.
 *
 * @threadsafety Only one thread should be using the `src` and `dst` surfaces
 *               at any given time.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.BlitScaled
 */
inline void BlitSurface(SurfaceParam src,
                        OptionalRef<const RectRaw> srcrect,
                        SurfaceParam dst,
                        OptionalRef<const RectRaw> dstrect)
{
  CheckError(SDL_BlitSurface(src, srcrect, dst, dstrect));
}

inline void Surface::Blit(SurfaceParam src,
                          OptionalRef<const RectRaw> srcrect,
                          OptionalRef<const RectRaw> dstrect)
{
  SDL::BlitSurface(m_resource, src, srcrect, dstrect);
}

inline void Surface::BlitAt(SurfaceParam src,
                            OptionalRef<const RectRaw> srcrect,
                            const PointRaw& dstpos)
{
  static_assert(false, "Not implemented");
}

inline void BlitSurfaceAt(SurfaceParam src,
                          OptionalRef<const RectRaw> srcrect,
                          SurfaceParam dst,
                          const PointRaw& dstpos)
{
  static_assert(false, "Not implemented");
}

/**
 * Perform low-level surface blitting only.
 *
 * This is a semi-private blit function and it performs low-level surface
 * blitting, assuming the input rectangles have already been clipped.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied, may not be nullptr.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface, may not be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety Only one thread should be using the `src` and `dst` surfaces
 *               at any given time.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Blit
 */
inline void BlitSurfaceUnchecked(SurfaceParam src,
                                 const RectRaw& srcrect,
                                 SurfaceParam dst,
                                 const RectRaw& dstrect)
{
  CheckError(SDL_BlitSurfaceUnchecked(src, srcrect, dst, dstrect));
}

inline void Surface::BlitUnchecked(SurfaceParam src,
                                   const RectRaw& srcrect,
                                   const RectRaw& dstrect)
{
  SDL::BlitSurfaceUnchecked(m_resource, src, srcrect, dstrect);
}

/**
 * Perform a scaled blit to a destination surface, which may be of a different
 * format.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied, or nullptr to copy the entire surface.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface, or nullptr to fill the entire
 *                destination surface.
 * @param scaleMode the ScaleMode to be used.
 * @throws Error on failure.
 *
 * @threadsafety Only one thread should be using the `src` and `dst` surfaces
 *               at any given time.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Blit
 */
inline void BlitSurfaceScaled(SurfaceParam src,
                              OptionalRef<const RectRaw> srcrect,
                              SurfaceParam dst,
                              OptionalRef<const RectRaw> dstrect,
                              ScaleMode scaleMode)
{
  CheckError(SDL_BlitSurfaceScaled(src, srcrect, dst, dstrect, scaleMode));
}

inline void Surface::BlitScaled(SurfaceParam src,
                                OptionalRef<const RectRaw> srcrect,
                                OptionalRef<const RectRaw> dstrect,
                                ScaleMode scaleMode)
{
  SDL::BlitSurfaceScaled(m_resource, src, srcrect, dstrect, scaleMode);
}

/**
 * Perform low-level surface scaled blitting only.
 *
 * This is a semi-private function and it performs low-level surface blitting,
 * assuming the input rectangles have already been clipped.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied, may not be nullptr.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface, may not be nullptr.
 * @param scaleMode the ScaleMode to be used.
 * @throws Error on failure.
 *
 * @threadsafety Only one thread should be using the `src` and `dst` surfaces
 *               at any given time.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.BlitScaled
 */
inline void BlitSurfaceUncheckedScaled(SurfaceParam src,
                                       const RectRaw& srcrect,
                                       SurfaceParam dst,
                                       const RectRaw& dstrect,
                                       ScaleMode scaleMode)
{
  CheckError(
    SDL_BlitSurfaceUncheckedScaled(src, srcrect, dst, dstrect, scaleMode));
}

inline void Surface::BlitUncheckedScaled(SurfaceParam src,
                                         const RectRaw& srcrect,
                                         const RectRaw& dstrect,
                                         ScaleMode scaleMode)
{
  SDL::BlitSurfaceUncheckedScaled(m_resource, src, srcrect, dstrect, scaleMode);
}

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Perform a stretched pixel copy from one surface to another.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied, or nullptr to copy the entire surface.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface, or nullptr to fill the entire
 *                destination surface.
 * @param scaleMode the ScaleMode to be used.
 * @throws Error on failure.
 *
 * @threadsafety Only one thread should be using the `src` and `dst` surfaces
 *               at any given time.
 *
 * @since This function is available since SDL 3.4.0.
 *
 * @sa Surface.BlitScaled
 */
inline void StretchSurface(SurfaceParam src,
                           OptionalRef<RectRaw> srcrect,
                           SurfaceParam dst,
                           OptionalRef<RectRaw> dstrect,
                           ScaleMode scaleMode)
{
  CheckError(SDL_StretchSurface(src, srcrect, dst, dstrect, scaleMode));
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

inline void Surface::Stretch(SurfaceParam src,
                             OptionalRef<RectRaw> srcrect,
                             OptionalRef<RectRaw> dstrect,
                             ScaleMode scaleMode)
{
  SDL::StretchSurface(m_resource, src, srcrect, dstrect, scaleMode);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Perform a tiled blit to a destination surface, which may be of a different
 * format.
 *
 * The pixels in `srcrect` will be repeated as many times as needed to
 * completely fill `dstrect`.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied, or nullptr to copy the entire surface.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface, or nullptr to fill the entire
 * surface.
 * @throws Error on failure.
 *
 * @threadsafety Only one thread should be using the `src` and `dst` surfaces
 *               at any given time.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Blit
 */
inline void BlitSurfaceTiled(SurfaceParam src,
                             OptionalRef<const RectRaw> srcrect,
                             SurfaceParam dst,
                             OptionalRef<const RectRaw> dstrect)
{
  CheckError(SDL_BlitSurfaceTiled(src, srcrect, dst, dstrect));
}

inline void Surface::BlitTiled(SurfaceParam src,
                               OptionalRef<const RectRaw> srcrect,
                               OptionalRef<const RectRaw> dstrect)
{
  SDL::BlitSurfaceTiled(m_resource, src, srcrect, dstrect);
}

/**
 * Perform a scaled and tiled blit to a destination surface, which may be of a
 * different format.
 *
 * The pixels in `srcrect` will be scaled and repeated as many times as needed
 * to completely fill `dstrect`.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied, or nullptr to copy the entire surface.
 * @param scale the scale used to transform srcrect into the destination
 *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
 *              64x64 tiles.
 * @param scaleMode scale algorithm to be used.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface, or nullptr to fill the entire
 * surface.
 * @throws Error on failure.
 *
 * @threadsafety Only one thread should be using the `src` and `dst` surfaces
 *               at any given time.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Blit
 */
inline void BlitSurfaceTiledWithScale(SurfaceParam src,
                                      OptionalRef<const RectRaw> srcrect,
                                      float scale,
                                      SDL_ScaleMode scaleMode,
                                      SurfaceParam dst,
                                      OptionalRef<const RectRaw> dstrect)
{
  CheckError(SDL_BlitSurfaceTiledWithScale(
    src, srcrect, scale, scaleMode, dst, dstrect));
}

inline void Surface::BlitTiledWithScale(SurfaceParam src,
                                        OptionalRef<const RectRaw> srcrect,
                                        float scale,
                                        SDL_ScaleMode scaleMode,
                                        OptionalRef<const RectRaw> dstrect)
{
  SDL::BlitSurfaceTiledWithScale(
    m_resource, src, srcrect, scale, scaleMode, dstrect);
}

/**
 * Perform a scaled blit using the 9-grid algorithm to a destination surface,
 * which may be of a different format.
 *
 * The pixels in the source surface are split into a 3x3 grid, using the
 * different corner sizes for each corner, and the sides and center making up
 * the remaining pixels. The corners are then scaled using `scale` and fit
 * into the corners of the destination rectangle. The sides and center are
 * then stretched into place to cover the remaining destination rectangle.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be used
 *                for the 9-grid, or nullptr to use the entire surface.
 * @param left_width the width, in pixels, of the left corners in `srcrect`.
 * @param right_width the width, in pixels, of the right corners in `srcrect`.
 * @param top_height the height, in pixels, of the top corners in `srcrect`.
 * @param bottom_height the height, in pixels, of the bottom corners in
 *                      `srcrect`.
 * @param scale the scale used to transform the corner of `srcrect` into the
 *              corner of `dstrect`, or 0.0f for an unscaled blit.
 * @param scaleMode scale algorithm to be used.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface, or nullptr to fill the entire
 * surface.
 * @throws Error on failure.
 *
 * @threadsafety Only one thread should be using the `src` and `dst` surfaces
 *               at any given time.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.Blit
 */
inline void BlitSurface9Grid(SurfaceParam src,
                             OptionalRef<const RectRaw> srcrect,
                             int left_width,
                             int right_width,
                             int top_height,
                             int bottom_height,
                             SurfaceParam dst,
                             OptionalRef<const RectRaw> dstrect,
                             float scale = 1,
                             SDL_ScaleMode scaleMode = SCALEMODE_NEAREST)
{
  CheckError(SDL_BlitSurface9Grid(src,
                                  srcrect,
                                  left_width,
                                  right_width,
                                  top_height,
                                  bottom_height,
                                  dst,
                                  dstrect,
                                  scale,
                                  scaleMode));
}

inline void Surface::Blit9Grid(SurfaceParam src,
                               OptionalRef<const RectRaw> srcrect,
                               int left_width,
                               int right_width,
                               int top_height,
                               int bottom_height,
                               OptionalRef<const RectRaw> dstrect,
                               float scale,
                               SDL_ScaleMode scaleMode)
{
  SDL::BlitSurface9Grid(m_resource,
                        src,
                        srcrect,
                        left_width,
                        right_width,
                        top_height,
                        bottom_height,
                        dstrect,
                        scale,
                        scaleMode);
}

/**
 * Map an RGB triple to an opaque pixel value for a surface.
 *
 * This function maps the RGB color value to the specified pixel format and
 * returns the pixel value best approximating the given RGB color value for
 * the given pixel format.
 *
 * If the surface has a palette, the index of the closest matching color in
 * the palette will be returned.
 *
 * If the surface pixel format has an alpha component it will be returned as
 * all 1 bits (fully opaque).
 *
 * If the pixel format bpp (color depth) is less than 32-bpp then the unused
 * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
 * format the return value can be assigned to a Uint16, and similarly a Uint8
 * for an 8-bpp format).
 *
 * @param surface the surface to use for the pixel format and palette.
 * @param r the red component of the pixel in the range 0-255.
 * @param g the green component of the pixel in the range 0-255.
 * @param b the blue component of the pixel in the range 0-255.
 * @returns a pixel value.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.MapRGBA
 */
inline Uint32 MapSurfaceRGB(SurfaceConstParam surface,
                            Uint8 r,
                            Uint8 g,
                            Uint8 b)
{
  return SDL_MapSurfaceRGB(surface, r, g, b);
}

inline Uint32 Surface::MapRGB(Uint8 r, Uint8 g, Uint8 b) const
{
  return SDL::MapSurfaceRGB(m_resource, r, g, b);
}

/**
 * Map an RGBA quadruple to a pixel value for a surface.
 *
 * This function maps the RGBA color value to the specified pixel format and
 * returns the pixel value best approximating the given RGBA color value for
 * the given pixel format.
 *
 * If the surface pixel format has no alpha component the alpha value will be
 * ignored (as it will be in formats with a palette).
 *
 * If the surface has a palette, the index of the closest matching color in
 * the palette will be returned.
 *
 * If the pixel format bpp (color depth) is less than 32-bpp then the unused
 * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
 * format the return value can be assigned to a Uint16, and similarly a Uint8
 * for an 8-bpp format).
 *
 * @param surface the surface to use for the pixel format and palette.
 * @param r the red component of the pixel in the range 0-255.
 * @param g the green component of the pixel in the range 0-255.
 * @param b the blue component of the pixel in the range 0-255.
 * @param a the alpha component of the pixel in the range 0-255.
 * @returns a pixel value.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Surface.MapRGB
 */
inline Uint32 MapSurfaceRGBA(SurfaceConstParam surface, ColorRaw c)
{
  return SDL_MapSurfaceRGBA(surface, c);
}

inline Uint32 Surface::MapRGBA(ColorRaw c) const
{
  return SDL::MapSurfaceRGBA(m_resource, c);
}

/**
 * Retrieves a single pixel from a surface.
 *
 * This function prioritizes correctness over speed: it is suitable for unit
 * tests, but is not intended for use in a game engine.
 *
 * Like GetRGBA, this uses the entire 0..255 range when converting color
 * components from pixel formats with less than 8 bits per RGB component.
 *
 * @param surface the surface to read.
 * @param x the horizontal coordinate, 0 <= x < width.
 * @param y the vertical coordinate, 0 <= y < height.
 * @param r a pointer filled in with the red channel, 0-255, or nullptr to
 * ignore this channel.
 * @param g a pointer filled in with the green channel, 0-255, or nullptr to
 *          ignore this channel.
 * @param b a pointer filled in with the blue channel, 0-255, or nullptr to
 *          ignore this channel.
 * @param a a pointer filled in with the alpha channel, 0-255, or nullptr to
 *          ignore this channel.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReadSurfacePixel(SurfaceConstParam surface,
                             const PointRaw& p,
                             Uint8* r,
                             Uint8* g,
                             Uint8* b,
                             Uint8* a)
{
  CheckError(SDL_ReadSurfacePixel(surface, p, r, g, b, a));
}

/**
 * Retrieves a single pixel from a surface.
 *
 * This function prioritizes correctness over speed: it is suitable for unit
 * tests, but is not intended for use in a game engine.
 *
 * Like GetRGBA, this uses the entire 0..255 range when converting color
 * components from pixel formats with less than 8 bits per RGB component.
 *
 * @param surface the surface to read.
 * @param x the horizontal coordinate, 0 <= x < width.
 * @param y the vertical coordinate, 0 <= y < height.
 * @param r a pointer filled in with the red channel, 0-255, or nullptr to
 * ignore this channel.
 * @param g a pointer filled in with the green channel, 0-255, or nullptr to
 *          ignore this channel.
 * @param b a pointer filled in with the blue channel, 0-255, or nullptr to
 *          ignore this channel.
 * @param a a pointer filled in with the alpha channel, 0-255, or nullptr to
 *          ignore this channel.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Color ReadSurfacePixel(SurfaceConstParam surface, const PointRaw& p)
{
  static_assert(false, "Not implemented");
}

inline void Surface::ReadPixel(const PointRaw& p,
                               Uint8* r,
                               Uint8* g,
                               Uint8* b,
                               Uint8* a) const
{
  SDL::ReadSurfacePixel(m_resource, p, r, g, b, a);
}

inline Color Surface::ReadPixel(const PointRaw& p) const
{
  return SDL::ReadSurfacePixel(m_resource, p);
}

/**
 * Retrieves a single pixel from a surface.
 *
 * This function prioritizes correctness over speed: it is suitable for unit
 * tests, but is not intended for use in a game engine.
 *
 * @param surface the surface to read.
 * @param x the horizontal coordinate, 0 <= x < width.
 * @param y the vertical coordinate, 0 <= y < height.
 * @param r a pointer filled in with the red channel, normally in the range
 *          0-1, or nullptr to ignore this channel.
 * @param g a pointer filled in with the green channel, normally in the range
 *          0-1, or nullptr to ignore this channel.
 * @param b a pointer filled in with the blue channel, normally in the range
 *          0-1, or nullptr to ignore this channel.
 * @param a a pointer filled in with the alpha channel, normally in the range
 *          0-1, or nullptr to ignore this channel.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReadSurfacePixelFloat(SurfaceConstParam surface,
                                  const PointRaw& p,
                                  float* r,
                                  float* g,
                                  float* b,
                                  float* a)
{
  CheckError(SDL_ReadSurfacePixelFloat(surface, p, r, g, b, a));
}

/**
 * Retrieves a single pixel from a surface.
 *
 * This function prioritizes correctness over speed: it is suitable for unit
 * tests, but is not intended for use in a game engine.
 *
 * @param surface the surface to read.
 * @param x the horizontal coordinate, 0 <= x < width.
 * @param y the vertical coordinate, 0 <= y < height.
 * @param r a pointer filled in with the red channel, normally in the range
 *          0-1, or nullptr to ignore this channel.
 * @param g a pointer filled in with the green channel, normally in the range
 *          0-1, or nullptr to ignore this channel.
 * @param b a pointer filled in with the blue channel, normally in the range
 *          0-1, or nullptr to ignore this channel.
 * @param a a pointer filled in with the alpha channel, normally in the range
 *          0-1, or nullptr to ignore this channel.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline FColor ReadSurfacePixelFloat(SurfaceConstParam surface,
                                    const PointRaw& p)
{
  static_assert(false, "Not implemented");
}

inline void Surface::ReadPixelFloat(const PointRaw& p,
                                    float* r,
                                    float* g,
                                    float* b,
                                    float* a) const
{
  SDL::ReadSurfacePixelFloat(m_resource, p, r, g, b, a);
}

inline FColor Surface::ReadPixelFloat(const PointRaw& p) const
{
  return SDL::ReadSurfacePixelFloat(m_resource, p);
}

/**
 * Writes a single pixel to a surface.
 *
 * This function prioritizes correctness over speed: it is suitable for unit
 * tests, but is not intended for use in a game engine.
 *
 * Like MapColor, this uses the entire 0..255 range when converting color
 * components from pixel formats with less than 8 bits per RGB component.
 *
 * @param surface the surface to write.
 * @param x the horizontal coordinate, 0 <= x < width.
 * @param y the vertical coordinate, 0 <= y < height.
 * @param r the red channel value, 0-255.
 * @param g the green channel value, 0-255.
 * @param b the blue channel value, 0-255.
 * @param a the alpha channel value, 0-255.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteSurfacePixel(SurfaceParam surface,
                              const PointRaw& p,
                              ColorRaw c)
{
  CheckError(SDL_WriteSurfacePixel(surface, p, c));
}

inline void Surface::WritePixel(const PointRaw& p, ColorRaw c)
{
  SDL::WriteSurfacePixel(m_resource, p, c);
}

/**
 * Writes a single pixel to a surface.
 *
 * This function prioritizes correctness over speed: it is suitable for unit
 * tests, but is not intended for use in a game engine.
 *
 * @param surface the surface to write.
 * @param x the horizontal coordinate, 0 <= x < width.
 * @param y the vertical coordinate, 0 <= y < height.
 * @param r the red channel value, normally in the range 0-1.
 * @param g the green channel value, normally in the range 0-1.
 * @param b the blue channel value, normally in the range 0-1.
 * @param a the alpha channel value, normally in the range 0-1.
 * @throws Error on failure.
 *
 * @threadsafety This function can be called on different threads with
 *               different surfaces.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteSurfacePixelFloat(SurfaceParam surface,
                                   const PointRaw& p,
                                   const FColorRaw& c)
{
  CheckError(SDL_WriteSurfacePixelFloat(surface, p, c));
}

inline void Surface::WritePixelFloat(const PointRaw& p, const FColorRaw& c)
{
  SDL::WriteSurfacePixelFloat(m_resource, p, c);
}

constexpr int GetSurfaceWidth(SurfaceConstParam surface)
{
  static_assert(false, "Not implemented");
}

constexpr int Surface::GetWidth() const
{
  return SDL::GetSurfaceWidth(m_resource);
}

constexpr int GetSurfaceHeight(SurfaceConstParam surface)
{
  static_assert(false, "Not implemented");
}

constexpr int Surface::GetHeight() const
{
  return SDL::GetSurfaceHeight(m_resource);
}

constexpr Point GetSurfaceSize(SurfaceConstParam surface)
{
  static_assert(false, "Not implemented");
}

constexpr Point Surface::GetSize() const
{
  return SDL::GetSurfaceSize(m_resource);
}

constexpr int GetSurfacePitch(SurfaceConstParam surface)
{
  static_assert(false, "Not implemented");
}

constexpr int Surface::GetPitch() const
{
  return SDL::GetSurfacePitch(m_resource);
}

constexpr PixelFormat GetSurfaceFormat(SurfaceConstParam surface)
{
  static_assert(false, "Not implemented");
}

constexpr PixelFormat Surface::GetFormat() const
{
  return SDL::GetSurfaceFormat(m_resource);
}

constexpr void* GetSurfacePixels(SurfaceConstParam surface)
{
  static_assert(false, "Not implemented");
}

constexpr void* Surface::GetPixels() const
{
  return SDL::GetSurfacePixels(m_resource);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_SURFACE_H_ */
