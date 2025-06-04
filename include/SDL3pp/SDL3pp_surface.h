#ifndef SDL3PP_SURFACE_H_
#define SDL3PP_SURFACE_H_

#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_version.h>
#include "SDL3pp_blendmode.h"
#include "SDL3pp_iostream.h"
#include "SDL3pp_pixels.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategorySurface Surface Creation and Simple Drawing
 *
 * SDL surfaces are buffers of pixels in system RAM. These are useful for
 * passing around and manipulating images that are not stored in GPU memory.
 *
 * SDL_Surface makes serious efforts to manage images in various formats, and
 * provides a reasonable toolbox for transforming the data, including copying
 * between surfaces, filling rectangles in the image data, etc.
 *
 * There is also a simple .bmp loader, Surface.LoadBMP(). SDL itself does not
 * provide loaders for various other file formats, but there are several
 * excellent external libraries that do, including its own satellite library,
 * SDL_image:
 *
 * https://github.com/libsdl-org/SDL_image
 * @{
 */

// Forward decl
struct SurfaceLock;

// Forward decl
struct SurfaceRef;

// Forward decl
struct Surface;

/**
 * The flags on an SurfaceRef.
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

/**
 * Surface uses pixel memory allocated with aligned_alloc()
 */
constexpr SurfaceFlags SURFACE_SIMD_ALIGNED = SDL_SURFACE_SIMD_ALIGNED;

/**
 * The scaling mode.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ScaleMode = SDL_ScaleMode;

#if SDL_VERSION_ATLEAST(3, 2, 10)

/**
 * @since SDL 3.2.10
 */
constexpr ScaleMode SCALEMODE_INVALID = SDL_SCALEMODE_INVALID;

#endif // SDL_VERSION_ATLEAST(3, 2, 10)

constexpr ScaleMode SCALEMODE_NEAREST =
  SDL_SCALEMODE_NEAREST; ///< nearest pixel sampling

constexpr ScaleMode SCALEMODE_LINEAR =
  SDL_SCALEMODE_LINEAR; ///< linear filtering

/**
 * The flip mode.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using FlipMode = SDL_FlipMode;

constexpr FlipMode FLIP_NONE = SDL_FLIP_NONE; ///< Do not flip.

constexpr FlipMode FLIP_HORIZONTAL = SDL_FLIP_HORIZONTAL; ///< flip horizontally

constexpr FlipMode FLIP_VERTICAL = SDL_FLIP_VERTICAL; ///< flip vertically

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
 * @sa Surface.Create
 * @sa Surface.CreateFrom
 * @sa Surface.Destroy
 *
 * @cat resource
 *
 * @sa Surface
 * @sa SurfaceRef
 */
struct SurfaceRef : Resource<SDL_Surface*>
{
  using Resource::Resource;

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
  PropertiesRef GetProperties() const
  {
    return CheckError(SDL_GetSurfaceProperties(get()));
  }

  /**
   * Set the colorspace used by a surface.
   *
   * Setting the colorspace doesn't change the pixels, only how they are
   * interpreted in color operations.
   *
   * @param colorspace a Colorspace value describing the surface colorspace.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.GetColorspace
   */
  void SetColorspace(Colorspace colorspace)
  {
    CheckError(SDL_SetSurfaceColorspace(get(), colorspace));
  }

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
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.SetColorspace
   */
  Colorspace GetColorspace() const { return SDL_GetSurfaceColorspace(get()); }

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
   * @returns a new PaletteRef structure on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PaletteRef.SetColors
   */
  PaletteRef CreatePalette()
  {
    return CheckError(SDL_CreateSurfacePalette(get()));
  }

  /**
   * Set the palette used by a surface.
   *
   * A single palette can be shared with many surfaces.
   *
   * @param palette the PaletteRef structure to use.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.Create
   * @sa SurfaceRef.GetPalette
   */
  void SetPalette(PaletteRef palette)
  {
    CheckError(SDL_SetSurfacePalette(get(), palette.get()));
  }

  /**
   * Get the palette used by a surface.
   *
   * @returns a pointer to the palette used by the surface, or nullptr if there
   *          is no palette used.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.SetPalette
   */
  PaletteRef GetPalette() const { return SDL_GetSurfacePalette(get()); }

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
   * @param image an alternate SurfaceRef to associate with this surface.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.RemoveAlternateImages
   * @sa SurfaceRef.GetImages
   * @sa SurfaceRef.HasAlternateImages
   */
  void AddAlternateImage(SurfaceRef image)
  {
    CheckError(SDL_AddSurfaceAlternateImage(get(), image.get()));
  }

  /**
   * Return whether a surface has alternate versions available.
   *
   * @returns true if alternate versions are available or false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.AddAlternateImage
   * @sa SurfaceRef.RemoveAlternateImages
   * @sa SurfaceRef.GetImages
   */
  bool HasAlternateImages() const
  {
    return SDL_SurfaceHasAlternateImages(get());
  }

  /**
   * Get an array including all versions of a surface.
   *
   * This returns all versions of a surface, with the surface being queried as
   * the first element in the returned array.
   *
   * @returns a NULL terminated array of SurfaceRef pointers or nullptr on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.AddAlternateImage
   * @sa SurfaceRef.RemoveAlternateImages
   * @sa SurfaceRef.HasAlternateImages
   */
  OwnArray<SurfaceRef> GetImages() const
  {
    int count = 0;
    auto data =
      reinterpret_cast<SurfaceRef*>(SDL_GetSurfaceImages(get(), &count));
    return OwnArray<SurfaceRef>{data, size_t(count)};
  }

  /**
   * Remove all alternate versions of a surface.
   *
   * This function removes a reference from all the alternative versions,
   * destroying them if this is the last reference to them.
   *
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.AddAlternateImage
   * @sa SurfaceRef.GetImages
   * @sa SurfaceRef.HasAlternateImages
   */
  void RemoveAlternateImages() { SDL_RemoveSurfaceAlternateImages(get()); }

  /**
   * Evaluates to true if the surface needs to be locked before access.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool MustLock() const { return SDL_MUSTLOCK(get()); }

  /**
   * Set up a surface for directly accessing the pixels.
   *
   * Between calls to SurfaceRef.Lock() / SurfaceLock.Unlock(), you can write to
   * and read from `surface->pixels`, using the pixel format stored in
   * `surface->format`. Once you are done accessing the surface, you should use
   * SurfaceLock.Unlock() to release it or let the destructor take care of this
   * for you.
   *
   * Not all surfaces require locking. If `SurfaceRef.MustLock(surface)`
   * evaluates to false, then you can read and write to the surface at any time,
   * and the pixel format of the surface will not change.
   *
   * @returns SurfaceLock object on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.MustLock
   * @sa SurfaceLock.Unlock
   */
  SurfaceLock Lock() &;

  /**
   * Set the RLE acceleration hint for a surface.
   *
   * If RLE is enabled, color key and alpha blending blits are much faster, but
   * the surface must be locked before directly accessing the pixels.
   *
   * @param enabled true to enable RLE acceleration, false to disable it.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.Blit
   * @sa SurfaceRef.Lock
   * @sa SurfaceLock.Unlock
   */
  void SetRLE(bool enabled) { CheckError(SDL_SetSurfaceRLE(get(), enabled)); }

  /**
   * Returns whether the surface is RLE enabled.
   *
   * @returns true if the surface is RLE enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.SetRLE
   */
  bool HasRLE() const { return SDL_SurfaceHasRLE(get()); }

  /**
   * Set the color key (transparent pixel) in a surface.
   *
   * The color key defines a pixel value that will be treated as transparent in
   * a blit. For example, one can use this to specify that cyan pixels should be
   * considered transparent, and therefore not rendered.
   *
   * It is a pixel of the format used by the surface, as generated by
   * SDL_MapRGB().
   *
   * @param key the transparent pixel color.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorKey()
   * @sa SetRLE()
   * @sa HasColorKey()
   */
  void SetColorKey(Color key) { SetColorKey(MapColor(key)); }

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
   * @param key the transparent pixel or std::nullopt to disable it.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.GetColorKey
   * @sa SurfaceRef.SetRLE
   * @sa SurfaceRef.HasColorKey
   */
  void SetColorKey(std::optional<Uint32> key)
  {
    CheckError(SDL_SetSurfaceColorKey(get(), key.has_value(), key.value_or(0)));
  }

  /**
   * Unset the color key (transparent pixel) in a surface.
   *
   * The color key defines a pixel value that will be treated as transparent in
   * a blit. For example, one can use this to specify that cyan pixels should be
   * considered transparent, and therefore not rendered.
   *
   * @throws Error on failure.
   */
  void ClearColorKey() { CheckError(SDL_SetSurfaceColorKey(get(), false, 0)); }

  /**
   * Returns whether the surface has a color key.
   *
   * @returns true if the surface has a color key, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.SetColorKey
   * @sa SurfaceRef.GetColorKey
   */
  bool HasColorKey() const { return SDL_SurfaceHasColorKey(get()); }

  /**
   * Get the color key (transparent pixel) for a surface.
   *
   * The color key is a pixel of the format used by the surface, as generated by
   * MapRGB().
   *
   * If the surface doesn't have color key enabled this function returns
   * std::nullopt.
   *
   * @returns the color key as Color or std::nullopt if it does not have one.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorKey()
   * @sa HasColorKey()
   */
  Color GetColorKey() const
  {
    Color color;
    GetColorKey(&color);
    return color;
  }

  /**
   * Get the color key (transparent pixel) for a surface.
   *
   * The color key is a pixel of the format used by the surface, as generated by
   * SDL_MapRGB().
   *
   * If the surface doesn't have color key enabled this function returns false.
   *
   * @param key a pointer filled in with the transparent pixel.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorKey()
   * @sa HasColorKey()
   */
  void GetColorKey(Color* key) const
  {
    Uint32 color;
    GetColorKey(&color);
    *key = GetFormat().Get(color, GetPalette());
  }

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
   * @sa SurfaceRef.SetColorKey
   * @sa SurfaceRef.HasColorKey
   */
  void GetColorKey(Uint32* key) const
  {
    CheckError(SDL_GetSurfaceColorKey(get(), key));
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
   * @param r the red color value multiplied into blit operations.
   * @param g the green color value multiplied into blit operations.
   * @param b the blue color value multiplied into blit operations.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.GetColorMod
   * @sa SurfaceRef.SetAlphaMod
   */
  void SetColorMod(Uint8 r, Uint8 g, Uint8 b)
  {
    CheckError(SDL_SetSurfaceColorMod(get(), r, g, b));
  }

  /**
   * Get the additional color value multiplied into blit operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.GetAlphaMod
   * @sa SurfaceRef.SetColorMod
   */
  void GetColorMod(Uint8* r, Uint8* g, Uint8* b) const
  {
    CheckError(SDL_GetSurfaceColorMod(get(), r, g, b));
  }

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
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.GetAlphaMod
   * @sa SurfaceRef.SetColorMod
   */
  void SetAlphaMod(Uint8 alpha)
  {
    CheckError(SDL_SetSurfaceAlphaMod(get(), alpha));
  }

  /**
   * Get the additional alpha value used in blit operations.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.GetColorMod
   * @sa SurfaceRef.SetAlphaMod
   */
  Uint8 GetAlphaMod() const
  {
    Uint8 alpha;
    CheckError(SDL_GetSurfaceAlphaMod(get(), &alpha));
    return alpha;
  }

  /**
   * Set an additional color and alpha value multiplied into blit
   * operations.
   *
   * When this surface is blitted, during the blit operation each source color
   * channel is modulated by the appropriate color value according to the
   * following formula:
   *
   * `srcC = srcC * (color / 255)`
   * `srcA = srcA * (alpha / 255)`
   *
   * @param color the color to be multiplied in blit operations
   * @throws Error on failure.
   */
  void SetMod(Color color)
  {
    SetColorMod(color.r, color.g, color.b);
    SetAlphaMod(color.a);
  }

  /**
   * Get the additional color and alpha value multiplied into blit
   * operations.
   *
   * @returns a Color containing RGBA value on success or std::nullopt on
   * failure; call GetError() for more information.
   */
  Color GetMod() const
  {
    Color c;
    GetColorMod(&c.r, &c.g, &c.b);
    c.a = GetAlphaMod();
    return c;
  }

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
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.GetBlendMode
   */
  void SetBlendMode(BlendMode blendMode)
  {
    CheckError(SDL_SetSurfaceBlendMode(get(), blendMode));
  }

  /**
   * Get the blend mode used for blit operations.
   *
   * @return the blendMode on success or BLENDMODE_INVALID on failure; call
   * GetError() for more information.
   */
  BlendMode GetBlendMode() const
  {
    if (BlendMode blendMode; SDL_GetSurfaceBlendMode(get(), &blendMode)) {
      return blendMode;
    }
    return BLENDMODE_INVALID;
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
   * @param rect the Rect structure representing the clipping rectangle, or
   *             std::nullopt to disable clipping.
   * @returns true if the rectangle intersects the surface, otherwise false and
   *          blits will be completely clipped.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.ResetClipRect()
   * @sa SurfaceRef.GetClipRect
   */
  bool SetClipRect(OptionalRef<const SDL_Rect> rect)
  {
    return SDL_SetSurfaceClipRect(get(), rect);
  }

  /**
   * Disable the clipping rectangle for a surface.
   *
   * @sa SetClipRect()
   */
  void ResetClipRect() { SDL_SetSurfaceClipRect(get(), nullptr); }

  /**
   * Get the clipping rectangle for a surface.
   *
   * When `surface` is the destination of a blit, only the area within the clip
   * rectangle is drawn into.
   *
   * @returns the Rect structure filled in with the clipping rectangle for the
   *          surface on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.SetClipRect
   */
  Rect GetClipRect() const
  {
    Rect r;
    CheckError(SDL_GetSurfaceClipRect(get(), &r));
    return r;
  }

  /**
   * Flip a surface vertically or horizontally.
   *
   * @param flip the direction to flip.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Flip(FlipMode flip) { CheckError(SDL_FlipSurface(get(), flip)); }

  /**
   * Creates a new surface identical to the existing surface.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * @returns a copy of the surface or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.Destroy
   */
  Surface Duplicate() const;

  /**
   * Creates a new surface identical to the existing surface, scaled to the
   * desired size.
   *
   * @param width the width of the new surface.
   * @param height the height of the new surface.
   * @param scaleMode the ScaleMode to be used.
   * @returns a copy of the surface or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.Destroy
   */
  Surface Scale(int width, int height, ScaleMode scaleMode) const;

  /**
   * Copy an existing surface to a new surface of the specified format.
   *
   * This function is used to optimize images for faster *repeat* blitting. This
   * is accomplished by converting the original and storing the result as a new
   * surface. The new, optimized surface can then be used as the source for
   * future blits, making them faster.
   *
   * If you are converting to an indexed surface and want to map colors to a
   * palette, you can use SurfaceRef.Convert() instead.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * @param format the new pixel format.
   * @returns the new SurfaceRef structure that is created or nullptr on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.Convert
   * @sa SurfaceRef.Destroy
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
   *                nullptr.
   * @param colorspace the new colorspace.
   * @param props an PropertiesRef with additional color properties, or 0.
   * @returns the new SurfaceRef structure that is created or nullptr on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.Convert
   * @sa SurfaceRef.Destroy
   */
  Surface Convert(PixelFormat format,
                  PaletteRef palette,
                  Colorspace colorspace,
                  PropertiesRef props) const;

  /**
   * Premultiply the alpha in a surface.
   *
   * This is safe to use with src == dst, but not for other overlapping areas.
   *
   * @param linear true to convert from sRGB to linear space for the alpha
   *               multiplication, false to do multiplication in sRGB space.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void PremultiplyAlpha(bool linear)
  {
    CheckError(SDL_PremultiplySurfaceAlpha(get(), linear));
  }

  /**
   * Clear a surface with a specific color, with floating point precision.
   *
   * This function handles all surface formats, and ignores any clip rectangle.
   *
   * If the surface is YUV, the color is assumed to be in the sRGB colorspace,
   * otherwise the color is assumed to be in the colorspace of the surface.
   *
   * @param color the color of the pixel, normally in the range 0-1.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Clear(SDL_FColor color)
  {
    CheckError(SDL_ClearSurface(get(), color.r, color.g, color.b, color.a));
  }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * If there is a clip rectangle set on the destination (set via
   * SurfaceRef.SetClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param color the color to fill with.
   * @throws Error on failure.
   */
  void Fill(SDL_Color color) { return FillRect({}, color); }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by MapRGB() or MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SurfaceRef.SetClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param color the color to fill with.
   * @throws Error on failure.
   */
  void Fill(Uint32 color) { return FillRect({}, color); }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * If there is a clip rectangle set on the destination (set via
   * SurfaceRef.SetClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rect the SDL_Rect structure representing the rectangle to fill.
   * @param color the color to fill with.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.FillRects
   */
  void FillRect(OptionalRef<const SDL_Rect> rect, SDL_Color color)
  {
    FillRect(rect, MapColor(color));
  }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by MapRGB() or MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SurfaceRef.SetClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rect the SDL_Rect structure representing the rectangle to fill.
   * @param color the color to fill with.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.FillRects
   */
  void FillRect(OptionalRef<const SDL_Rect> rect, Uint32 color)
  {
    CheckError(SDL_FillSurfaceRect(get(), rect, color));
  }

  /**
   * Perform a fast fill of a set of rectangles with a specific color.
   *
   * If there is a clip rectangle set on the destination (set via
   * SurfaceRef.SetClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rects an array of SDL_Rects representing the rectangles to fill.
   * @param color the color to fill with.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FillRect()
   */
  void FillRects(SpanRef<const SDL_Rect> rects, SDL_Color color)
  {
    FillRects(rects, MapColor(color));
  }

  /**
   * Perform a fast fill of a set of rectangles with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by MapRGB() or MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SurfaceRef.SetClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rects an array of SDL_Rects representing the rectangles to fill.
   * @param color the color to fill with.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.FillRect
   */
  void FillRects(SpanRef<const SDL_Rect> rects, Uint32 color)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_UINT32);
    CheckError(SDL_FillSurfaceRects(get(), rects.data(), rects.size(), color));
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
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param dstpos  the SDL_Point structure representing the x and y position in
   *                the destination surface, or NULL for (0,0). The width and
   *                height are ignored, and are copied from `srcrect`. If you
   *                want a specific width and height, you should use
   *                SurfaceRef.BlitScaled().
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.BlitScaled
   */
  void Blit(SurfaceRef src,
            OptionalRef<const SDL_Rect> srcrect,
            const SDL_Point& dstpos)
  {
    Blit(src, srcrect, Rect{dstpos, {}});
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
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or std::nullopt to copy the entire surface.
   * @param dstrect the SDL_Rect structure representing the x and y position in
   *                the destination surface, or std::nullopt for (0,0). The
   *                width and height are ignored, and are copied from `srcrect`.
   *                If you want a specific width and height, you should use
   *                SurfaceRef.BlitScaled().
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.BlitScaled
   */
  void Blit(SurfaceRef src,
            OptionalRef<const SDL_Rect> srcrect,
            OptionalRef<const SDL_Rect> dstrect)
  {
    CheckError(SDL_BlitSurface(src.get(), srcrect, get(), dstrect));
  }

  /**
   * Perform low-level surface blitting only.
   *
   * This is a semi-private blit function and it performs low-level surface
   * blitting, assuming the input rectangles have already been clipped.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, may not be nullptr.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, may not be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.Blit
   */
  void BlitUnchecked(SurfaceRef src,
                     const SDL_Rect& srcrect,
                     const SDL_Rect& dstrect)
  {
    CheckError(SDL_BlitSurfaceUnchecked(src.get(), &srcrect, get(), &dstrect));
  }

  /**
   * Perform a scaled blit to a destination surface, which may be of a different
   * format.
   *
   * @param src the Surface structure to be copied from.
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, or nullptr to copy the entire surface.
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
   * @sa SurfaceRef.Blit
   */
  void BlitScaled(SurfaceRef src,
                  OptionalRef<const SDL_Rect> srcrect,
                  OptionalRef<const SDL_Rect> dstrect,
                  ScaleMode scaleMode)
  {
    CheckError(
      SDL_BlitSurfaceScaled(src.get(), srcrect, get(), dstrect, scaleMode));
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
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, may not be nullptr.
   * @param scaleMode the ScaleMode to be used.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.BlitScaled
   */
  void BlitUncheckedScaled(SurfaceRef src,
                           const SDL_Rect& srcrect,
                           const SDL_Rect& dstrect,
                           ScaleMode scaleMode)
  {
    CheckError(
      SDL_BlitSurfaceScaled(src.get(), &srcrect, get(), &dstrect, scaleMode));
  }

#if SDL_VERSION_ATLEAST(3, 2, 4)

  /**
   * Perform a stretched pixel copy from one surface to another.
   *
   * @param src the Surface structure to be copied from.
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, may not be nullptr.
   * @param scaleMode the ScaleMode to be used.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.4.
   *
   * @sa SurfaceRef.BlitScaled
   */
  void Stretch(SurfaceRef src,
               OptionalRef<SDL_Rect> srcrect,
               OptionalRef<SDL_Rect> dstrect,
               ScaleMode scaleMode)
  {
    CheckError(
      SDL_StretchSurface(src.get(), srcrect, get(), dstrect, scaleMode));
  }

#endif // SDL_VERSION_ATLEAST(3, 2, 4)

  /**
   * Perform a tiled blit to a destination surface, which may be of a different
   * format.
   *
   * The pixels in `srcrect` will be repeated as many times as needed to
   * completely fill `dstrect`.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, or nullptr to copy the entire surface.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, or nullptr to fill the entire
   *                surface.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.Blit
   */
  void BlitTiled(SurfaceRef src,
                 OptionalRef<const SDL_Rect> srcrect,
                 OptionalRef<const SDL_Rect> dstrect)
  {
    CheckError(SDL_BlitSurfaceTiled(src.get(), srcrect, get(), dstrect));
  }

  /**
   * Perform a scaled and tiled blit to a destination surface, which may be of a
   * different format.
   *
   * The pixels in `srcrect` will be scaled and repeated as many times as needed
   * to completely fill `dstrect`.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied, or nullptr to copy the entire surface.
   * @param scale the scale used to transform srcrect into the destination
   *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
   *              64x64 tiles.
   * @param scaleMode scale algorithm to be used.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface, or nullptr to fill the entire
   *                surface.
   * @throws Error on failure.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.Blit
   */
  void BlitTiledWithScale(SurfaceRef src,
                          OptionalRef<const SDL_Rect> srcrect,
                          float scale,
                          SDL_ScaleMode scaleMode,
                          OptionalRef<const SDL_Rect> dstrect)
  {
    CheckError(SDL_BlitSurfaceTiledWithScale(
      src.get(), srcrect, scale, scaleMode, get(), dstrect));
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
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the Rect structure representing the rectangle to be used
   *                for the 9-grid, or nullptr to use the entire surface.
   * @param left_width the width, in pixels, of the left corners in `srcrect`.
   * @param right_width the width, in pixels, of the right corners in `srcrect`.
   * @param top_height the height, in pixels, of the top corners in `srcrect`.
   * @param bottom_height the height, in pixels, of the bottom corners in
   *                      `srcrect`.
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
   * @sa SurfaceRef.Blit
   */
  void Blit9Grid(SurfaceRef src,
                 OptionalRef<const SDL_Rect> srcrect,
                 int left_width,
                 int right_width,
                 int top_height,
                 int bottom_height,
                 OptionalRef<const SDL_Rect> dstrect)
  {
    Blit9GridWithScale(src,
                       srcrect,
                       left_width,
                       right_width,
                       top_height,
                       bottom_height,
                       0.0,
                       SDL_SCALEMODE_NEAREST,
                       dstrect);
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
   * @param src the SDL_Surface structure to be copied from.
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
   * @sa SurfaceRef.Blit
   */
  void Blit9GridWithScale(SurfaceRef src,
                          OptionalRef<const SDL_Rect> srcrect,
                          int left_width,
                          int right_width,
                          int top_height,
                          int bottom_height,
                          float scale,
                          SDL_ScaleMode scaleMode,
                          OptionalRef<const SDL_Rect> dstrect)
  {
    CheckError(SDL_BlitSurface9Grid(src.get(),
                                    srcrect,
                                    left_width,
                                    right_width,
                                    top_height,
                                    bottom_height,
                                    scale,
                                    scaleMode,
                                    get(),
                                    dstrect));
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
   * @param color the color components
   * @return a pixel value.
   */
  Uint32 MapColor(SDL_Color color) const
  {
    return MapColor(color.r, color.g, color.b, color.a);
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
   * @param r the red component of the pixel in the range 0-255.
   * @param g the green component of the pixel in the range 0-255.
   * @param b the blue component of the pixel in the range 0-255.
   * @returns a pixel value.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.MapColor
   */
  Uint32 MapColor(Uint8 r, Uint8 g, Uint8 b) const
  {
    return SDL_MapSurfaceRGB(get(), r, g, b);
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
   * @param r the red component of the pixel in the range 0-255.
   * @param g the green component of the pixel in the range 0-255.
   * @param b the blue component of the pixel in the range 0-255.
   * @param a the alpha component of the pixel in the range 0-255.
   * @returns a pixel value.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SurfaceRef.MapColor
   */
  Uint32 MapColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
  {
    return SDL_MapSurfaceRGBA(get(), r, g, b, a);
  }

  /**
   * This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like SDL_GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param p the coordinates, 0 <= x < width and 0 <= y < height.
   * @returns color on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   */
  Color ReadPixel(const SDL_Point& p) const
  {
    Color c;
    ReadPixel(p, &c);
    return c;
  }

  /**
   * This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like SDL_GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param p the coordinates, 0 <= x < width and 0 <= y < height.
   * @param c a color pointer to be filled with the color information. Must not
   *          be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   */
  void ReadPixel(const SDL_Point& p, SDL_Color* c) const
  {
    ReadPixel(p, &c->r, &c->g, &c->b, &c->a);
  }

  /**
   * Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param p the coordinates, 0 <= x < width and 0 <= y < height.
   * @param c a color pointer to be filled with the color information. Must not
   *          be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   */
  void ReadPixel(const SDL_Point& p, SDL_FColor* c) const
  {
    return ReadPixel(p, &c->r, &c->g, &c->b, &c->a);
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
   * @param p the coordinates, 0 <= x < width and 0 <= y < height.
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
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReadPixel(const SDL_Point& p,
                 Uint8* r,
                 Uint8* g,
                 Uint8* b,
                 Uint8* a) const
  {
    CheckError(SDL_ReadSurfacePixel(get(), p.x, p.y, r, g, b, a));
  }

  /**
   * Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param p the coordinates, 0 <= x < width and 0 <= y < height.
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
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReadPixel(const SDL_Point& p,
                 float* r,
                 float* g,
                 float* b,
                 float* a) const
  {
    CheckError(SDL_ReadSurfacePixelFloat(get(), p.x, p.y, r, g, b, a));
  }

  /**
   * Writes a single pixel to a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * Like MapRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param p the coordinates, 0 <= x < width and 0 <= y < height.
   * @param c the color values, 0-255.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WritePixel(const SDL_Point& p, SDL_Color c)
  {
    CheckError(SDL_WriteSurfacePixel(get(), p.x, p.y, c.r, c.g, c.b, c.a));
  }

  /**
   * Writes a single pixel to a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param p the coordinates, 0 <= x < width and 0 <= y < height.
   * @param c the color values, normally in the range 0-1.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WritePixel(const SDL_Point& p, SDL_FColor c)
  {
    CheckError(SDL_WriteSurfacePixelFloat(get(), p.x, p.y, c.r, c.g, c.b, c.a));
  }

  /**
   * Get the width in pixels.
   */
  constexpr int GetWidth() const { return get()->w; }

  /**
   * Get the height in pixels.
   */
  constexpr int GetHeight() const { return get()->h; }

  /**
   * Get the size in pixels.
   */
  constexpr Point GetSize() const { return Point(GetWidth(), GetHeight()); }

  /**
   * Get pitch in bytes.
   */
  constexpr int GetPitch() const { return get()->pitch; }

  /**
   * Get the pixel format.
   */
  constexpr PixelFormat GetFormat() const { return get()->format; }

  /**
   * Get the pixels.
   */
  constexpr void* GetPixels() const { return get()->pixels; }

  /**
   * Free a surface.
   *
   * It is safe to pass nullptr to this function.
   *
   * @param resource the Surface to free.
   *
   * @threadsafety No other thread should be using the surface when it is freed.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Create
   * @sa Surface.CreateFrom
   */
  static void reset(SDL_Surface* resource) { SDL_DestroySurface(resource); }
};

/**
 * Handle to an owned surface
 *
 * @cat resource
 *
 * @sa SurfaceRef
 */
struct Surface : ResourceUnique<SurfaceRef>
{
  using ResourceUnique::ResourceUnique;

  /**
   * Load an image from a filesystem path into a software surface.
   *
   * If available, this uses LoadSurface(StringParam), otherwise it uses
   * LoadBMP(StringParam).
   *
   * @param file a path on the filesystem to load an image from.
   * @returns the new Surface with loaded contents on success or nullptr on
   *          failure; call GetError() for more information.
   *
   * @sa LoadSurface(StringParam)
   * @sa Surface.LoadBMP(StringParam)
   */
  static Surface Load(StringParam file);

  /**
   * Load an image from a filesystem path into a software surface.
   *
   * If available, this uses LoadSurface(StringParam), otherwise it uses
   * LoadBMP(StringParam).
   *
   * @param src an IOStreamRef to load an image from.
   * @returns the new Surface with loaded contents on success or nullptr on
   *          failure; call GetError() for more information.
   *
   * @sa LoadSurface(IOStreamRef)
   * @sa Surface.LoadBMP(IOStreamRef)
   */
  static Surface Load(IOStreamRef src);

  /**
   * Load a BMP image from a seekable SDL data stream.
   *
   * The new surface should be freed with Surface.Destroy(). Not doing so
   * will result in a memory leak.
   *
   * @param src the data stream for the surface.
   * @returns a pointer to a new SurfaceRef structure or nullptr on failure;
   * call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Destroy
   * @sa Surface.LoadBMP
   * @sa SaveBMP
   */
  static Surface LoadBMP(IOStreamRef src)
  {
    return Surface(SDL_LoadBMP_IO(src, false));
  }

  /**
   * Load a BMP image from a file.
   *
   * The new surface should be freed with Surface.Destroy(). Not doing so
   * will result in a memory leak.
   *
   * @param file the BMP file to load.
   * @returns a pointer to a new SurfaceRef structure or nullptr on failure;
   * call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Destroy
   * @sa Surface.LoadBMP
   * @sa SaveBMP
   */
  static Surface LoadBMP(StringParam file)
  {
    return Surface(SDL_LoadBMP(file));
  }

  /**
   * Allocate a new surface with a specific pixel format.
   *
   * The pixels of the new surface are initialized to zero.
   *
   * @param size the width and height of the surface.
   * @param format the PixelFormat for the new surface's pixel format.
   * @returns the new Surface structure that is created on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.CreateFrom
   * @sa Surface.Destroy
   */
  static Surface Create(const SDL_Point& size, PixelFormat format)
  {
    return Surface(SDL_CreateSurface(size.x, size.y, format));
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
   * @param size the width and height of the surface.
   * @param format the PixelFormat for the new surface's pixel format.
   * @param pixels a pointer to existing pixel data.
   * @param pitch the number of bytes between each row, including padding.
   * @returns the new SurfaceRef structure that is created or nullptr on
   * failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Create
   * @sa Surface.Destroy
   */
  static Surface CreateFrom(const SDL_Point& size,
                            PixelFormat format,
                            void* pixels,
                            int pitch)
  {
    return Surface(
      SDL_CreateSurfaceFrom(size.x, size.y, format, pixels, pitch));
  }

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
   * @sa Surface.Create
   * @sa Surface.CreateFrom
   */
  void Destroy() { reset(); }
};

/**
 * Unsafe Handle to surface
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa SurfaceRef
 */
struct SurfaceUnsafe : ResourceUnsafe<SurfaceRef>
{
  using ResourceUnsafe::ResourceUnsafe;

  /**
   * Constructs SurfaceUnsafe from Surface.
   */
  constexpr explicit SurfaceUnsafe(Surface&& other)
    : SurfaceUnsafe(other.release())
  {
  }
};

/**
 * Locks a Surface for access to its pixels
 *
 * Only really necessary if Surface.MustLock() returns t
 */
class SurfaceLock
{
  SurfaceRef surface;

  /**
   * @sa SurfaceRef.Lock()
   */
  explicit SurfaceLock(SurfaceRef surface)
    : surface(std::move(surface))
  {
    if (!SDL_LockSurface(this->surface.get())) this->surface.release();
  }

public:
  // default ctor
  SurfaceLock()
    : surface(nullptr)
  {
  }

  /// Copy ctor
  SurfaceLock(const SurfaceLock& other) = delete;

  /// Move ctor
  SurfaceLock(SurfaceLock&& other)
    : surface(other.surface.release())
  {
  }

  /**
   * destructor
   * @sa Unlock()
   */
  ~SurfaceLock() { Unlock(); }

  /// Assignment operator
  SurfaceLock& operator=(SurfaceLock other)
  {
    std::swap(surface, other.surface);
    return *this;
  }

  /**
   * Returns true if lock is active
   */
  constexpr operator bool() const { return bool(surface); }

  /**
   * Release the locked surface after directly accessing the pixels.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Lock()
   */
  void Unlock() { return SDL_UnlockSurface(surface.release()); }

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

  friend class SurfaceRef;
};

namespace prop::Surface {

constexpr auto SDR_WHITE_POINT_FLOAT = SDL_PROP_SURFACE_SDR_WHITE_POINT_FLOAT;

constexpr auto HDR_HEADROOM_FLOAT = SDL_PROP_SURFACE_HDR_HEADROOM_FLOAT;

constexpr auto TONEMAP_OPERATOR_STRING =
  SDL_PROP_SURFACE_TONEMAP_OPERATOR_STRING;

#if SDL_VERSION_ATLEAST(3, 2, 6)

constexpr auto HOTSPOT_X_NUMBER = SDL_PROP_SURFACE_HOTSPOT_X_NUMBER;

constexpr auto HOTSPOT_Y_NUMBER = SDL_PROP_SURFACE_HOTSPOT_Y_NUMBER;

#endif // SDL_VERSION_ATLEAST(3, 2, 6)

} // namespace prop::Surface

/**
 * Save a surface to a seekable SDL data stream in BMP format.
 *
 * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
 * BMP directly. Other RGB formats with 8-bit or higher get converted to a
 * 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
 * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
 * not supported.
 *
 * @param surface the SurfaceRef structure containing the image to be saved.
 * @param dst a data stream to save to.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LoadBMP
 */
inline void SaveBMP(SurfaceRef surface, IOStreamRef dst)
{
  CheckError(SDL_SaveBMP_IO(surface.get(), dst.get(), false));
}

/**
 * Save a surface to a file.
 *
 * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
 * BMP directly. Other RGB formats with 8-bit or higher get converted to a
 * 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
 * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
 * not supported.
 *
 * @param surface the SurfaceRef structure containing the image to be saved.
 * @param file a file to save to.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LoadBMP
 */
inline void SaveBMP(SurfaceRef surface, StringParam file)
{
  CheckError(SDL_SaveBMP(surface.get(), file));
}

inline Surface SurfaceRef::Duplicate() const
{
  return Surface{SDL_DuplicateSurface(get())};
}

inline Surface SurfaceRef::Scale(int width,
                                 int height,
                                 ScaleMode scaleMode) const
{
  return Surface{SDL_ScaleSurface(get(), width, height, scaleMode)};
}

inline Surface SurfaceRef::Convert(PixelFormat format) const
{
  return Surface{SDL_ConvertSurface(get(), format)};
}

inline Surface SurfaceRef::Convert(PixelFormat format,
                                   PaletteRef palette,
                                   Colorspace colorspace,
                                   PropertiesRef props) const
{
  return Surface{SDL_ConvertSurfaceAndColorspace(
    get(), format, palette.get(), colorspace, props.get())};
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
inline void ConvertPixels(int width,
                          int height,
                          PixelFormat src_format,
                          const void* src,
                          int src_pitch,
                          PixelFormat dst_format,
                          void* dst,
                          int dst_pitch)
{
  CheckError(SDL_ConvertPixels(
    width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch));
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
 * @param src_properties an PropertiesRef with additional source color
 *                       properties, or 0.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an PixelFormat value of the `dst` pixels format.
 * @param dst_colorspace an Colorspace value describing the colorspace of
 *                       the `dst` pixels.
 * @param dst_properties an PropertiesRef with additional destination color
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
inline void ConvertPixelsAndColorspace(int width,
                                       int height,
                                       PixelFormat src_format,
                                       Colorspace src_colorspace,
                                       PropertiesRef src_properties,
                                       const void* src,
                                       int src_pitch,
                                       PixelFormat dst_format,
                                       Colorspace dst_colorspace,
                                       PropertiesRef dst_properties,
                                       void* dst,
                                       int dst_pitch)
{
  CheckError(SDL_ConvertPixelsAndColorspace(width,
                                            height,
                                            src_format,
                                            src_colorspace,
                                            src_properties.get(),
                                            src,
                                            src_pitch,
                                            dst_format,
                                            dst_colorspace,
                                            dst_properties.get(),
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
inline void PremultiplyAlpha(int width,
                             int height,
                             PixelFormat src_format,
                             const void* src,
                             int src_pitch,
                             PixelFormat dst_format,
                             void* dst,
                             int dst_pitch,
                             bool linear)
{
  CheckError(SDL_PremultiplyAlpha(width,
                                  height,
                                  src_format,
                                  src,
                                  src_pitch,
                                  dst_format,
                                  dst,
                                  dst_pitch,
                                  linear));
}

/// @}

#pragma region impl

inline SurfaceLock SurfaceRef::Lock() & { return SurfaceLock{get()}; }

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_SURFACE_H_ */
