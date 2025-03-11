#ifndef SDL3PP_SURFACE_H_
#define SDL3PP_SURFACE_H_

#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_version.h>
#include "SDL3pp_blendmode.h"
#include "SDL3pp_error.h"
#include "SDL3pp_freeWrapper.h"
#include "SDL3pp_objectWrapper.h"
#include "SDL3pp_optionalRef.h"
#include "SDL3pp_pixels.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_spanRef.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_stringParam.h"

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
 * There is also a simple .bmp loader, SDL::LoadBMP(). SDL itself does not
 * provide loaders for various other file formats, but there are several
 * excellent external libraries that do, including its own satellite library,
 * SDL_image:
 *
 * https://github.com/libsdl-org/SDL_image
 * @{
 */

// Forward decl
template<ObjectBox<SDL_Surface*> T>
struct SurfaceBase;

/**
 * @brief Handle to a non owned surface
 */
using SurfaceRef = SurfaceBase<ObjectRef<SDL_Surface>>;

template<>
struct ObjectDeleter<SDL_Surface>
{
  void operator()(SurfaceRef Surface) const;
};

/**
 * @brief Handle to an owned surface
 */
using Surface = SurfaceBase<ObjectUnique<SDL_Surface>>;

// Forward decl
struct SurfaceLock;

/**
 * @brief The flags on an SDL_Surface.
 *
 * These are generally considered read-only.
 *
 */
using SurfaceFlags = SDL_SurfaceFlags;

/**
 * @brief The scaling mode.
 *
 */
using ScaleMode = SDL_ScaleMode;

#if SDL_VERSION_ATLEAST(3, 2, 9)
constexpr ScaleMode SCALEMODE_INVALID = SDL_SCALEMODE_INVALID;
#endif

/**
 * nearest pixel sampling
 */
constexpr ScaleMode SCALEMODE_NEAREST = SDL_SCALEMODE_NEAREST;

/**
 * linear filtering
 */
constexpr ScaleMode SCALEMODE_LINEAR = SDL_SCALEMODE_LINEAR;

/**
 * @brief The flip mode.
 *
 */
using FlipMode = SDL_FlipMode;

/**
 * Do not flip
 */
constexpr FlipMode FLIP_NONE = SDL_FLIP_NONE;

/**
 * flip horizontally
 */
constexpr FlipMode FLIP_HORIZONTAL = SDL_FLIP_HORIZONTAL;

/**
 * flip vertically
 */
constexpr FlipMode FLIP_VERTICAL = SDL_FLIP_VERTICAL;

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
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa SurfaceBase::SurfaceBase()
 *
 * @ingroup resource
 */
template<ObjectBox<SDL_Surface*> T>
struct SurfaceBase : T
{
  using T::T;

  /**
   * Allocate a new surface with a specific pixel format.
   *
   * The pixels of the new surface are initialized to zero.
   *
   * @param width the width of the surface.
   * @param height the height of the surface.
   * @param format the PixelFormat for the new surface's pixel format.
   * @post the new structure that is created and convertible to true on success
   * or convertible to false on failure; call SDL_GetError() for more
   * information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  SurfaceBase(int width, int height, PixelFormat format)
    : T(SDL_CreateSurface(width, height, format))
  {
  }

  /**
   * @brief Allocate a new surface with a specific pixel format and existing
   * pixel data.
   *
   * No copy is made of the pixel data. Pixel data is not managed automatically;
   * you must free the surface before you free the pixel data.
   *
   * Pitch is the offset in bytes from one row of pixels to the next, e.g.
   * `width*4` for `SDL_PIXELFORMAT_RGBA8888`.
   *
   * You may pass NULL for pixels and 0 for pitch to create a surface that you
   * will fill in with valid values later.
   *
   * @param width the width of the surface.
   * @param height the height of the surface.
   * @param format the PixelFormat for the new surface's pixel format.
   * @param pixels a pointer to existing pixel data.
   * @param pitch the number of bytes between each row, including padding.
   * @post the new structure that is created and convertible to true on success
   * or convertible to false on failure; call SDL_GetError() for more
   * information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  SurfaceBase(int width,
              int height,
              PixelFormat format,
              void* pixels,
              int pitch)
    : T(SDL_CreateSurfaceFrom(width, height, format, pixels, pitch))
  {
  }

  /**
   * Free a surface.
   *
   * This makes this object empty
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Destroy() { return SDL_DestroySurface(T::release()); }

  /**
   * Get the properties associated with a surface.
   *
   * The following properties are understood by SDL:
   *
   * - `SDL_PROP_SURFACE_SDR_WHITE_POINT_FLOAT`: for HDR10 and floating point
   *   surfaces, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 203 for HDR10 surfaces and 1.0 for floating point surfaces.
   * - `SDL_PROP_SURFACE_HDR_HEADROOM_FLOAT`: for HDR10 and floating point
   *   surfaces, this defines the maximum dynamic range used by the content, in
   *   terms of the SDR white point. This defaults to 0.0, which disables tone
   *   mapping.
   * - `SDL_PROP_SURFACE_TONEMAP_OPERATOR_STRING`: the tone mapping operator
   *   used when compressing from a surface with high dynamic range to another
   *   with lower dynamic range. Currently this supports "chrome", which uses
   *   the same tone mapping that Chrome uses for HDR content, the form "*=N",
   *   where N is a floating point scale factor applied in linear space, and
   *   "none", which disables tone mapping. This defaults to "chrome".
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return SDL_GetSurfaceProperties(T::get());
  }

  /**
   * Set the colorspace used by a surface.
   *
   * Setting the colorspace doesn't change the pixels, only how they are
   * interpreted in color operations.
   *
   * @param colorspace a Colorspace value describing the surface colorspace.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorspace()
   */
  bool SetColorspace(Colorspace colorspace)
  {
    return SDL_SetSurfaceColorspace(T::get(), colorspace);
  }

  /**
   * @brief Get the colorspace used by a surface.
   *
   * The colorspace defaults to SDL_COLORSPACE_SRGB_LINEAR for floating point
   * formats, SDL_COLORSPACE_HDR10 for 10-bit formats, SDL_COLORSPACE_SRGB for
   * other RGB surfaces and SDL_COLORSPACE_BT709_FULL for YUV textures.
   *
   * @returns the colorspace used by the surface, or SDL_COLORSPACE_UNKNOWN if
   *          the surface is NULL.
   */
  Colorspace GetColorspace() const
  {
    return SDL_GetSurfaceColorspace(T::get());
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
   * Bitmap surfaces (with format SDL_PIXELFORMAT_INDEX1LSB or
   * SDL_PIXELFORMAT_INDEX1MSB) will have the palette initialized with 0 as
   * white and 1 as black. Other surfaces will get a palette initialized with
   * white in every entry.
   *
   * If this function is called for a surface that already has a palette, a new
   * palette will be created to replace it.
   *
   * @returns a new SDL_Palette structure on success or NULL on failure (e.g. if
   *          the surface didn't have an index format); call GetError() for
   *          more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.SetColors()
   */
  PaletteRef CreatePalette() { return SDL_CreateSurfacePalette(T::get()); }

  /**
   * Set the palette used by a surface.
   *
   * A single palette can be shared with many surfaces.
   *
   * @param palette the Palette to use.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette::Palette()
   * @sa GetPalette()
   */
  bool SetPalette(PaletteRef palette)
  {
    return SDL_SetSurfacePalette(T::get(), palette.get());
  }

  /**
   * Get the palette used by a surface.
   *
   * @returns a pointer to the palette used by the surface, or NULL if there is
   *          no palette used.
   */
  PaletteRef GetPalette() const { return SDL_GetSurfacePalette(T::get()); }

  /**
   * Add an alternate version of a surface.
   *
   * This function adds an alternate version of this surface, usually used for
   * content with high DPI representations like cursors or icons. The size,
   * format, and content do not need to match the original surface, and these
   * alternate versions will not be updated when the original surface changes.
   *
   * This function adds a reference to the alternate version, so you should call
   * SDL_DestroySurface() on the image after this call.
   *
   * @param image a pointer to an alternate SDL_Surface to associate with this
   *              surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RemoveAlternateImages()
   * @sa GetImages()
   * @sa HasAlternateImages()
   */
  bool AddAlternateImage(SurfaceRef image)
  {
    return SDL_AddSurfaceAlternateImage(T::get(), image);
  }

  /**
   * Return whether a surface has alternate versions available.
   *
   * @returns true if alternate versions are available or false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AddAlternateImage()
   * @sa RemoveAlternateImages()
   * @sa GetImages()
   */
  bool HasAlternateImages() const
  {
    return SDL_SurfaceHasAlternateImages(T::get());
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
   * @param count a pointer filled in with the number of surface pointers
   *              returned, may be NULL.
   * @returns a NULL terminated array of SDL_Surface pointers or NULL on
   *          failure; call SDL_GetError() for more information. This should be
   *          freed with SDL_free() when it is no longer needed.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AddAlternateImage()
   * @sa RemoveAlternateImages()
   * @sa HasAlternateImages()
   */
  FreeWrapper<SurfaceRef*[]> GetImages(int* count = nullptr) const
  {
    return SDL_GetSurfaceImages(T::get(), count);
  }

  /**
   * Remove all alternate versions of a surface.
   *
   * This function removes a reference from all the alternative versions,
   * destroying them if this is the last reference to them.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AddAlternateImage()
   * @sa GetImages()
   * @sa HasAlternateImages()
   */
  void RemoveAlternateImages() { SDL_RemoveSurfaceAlternateImages(T::get()); }

  /**
   * Evaluates to true if the surface needs to be locked before access.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool MustLock() const { return SDL_MUSTLOCK(T::get()); }

  /**
   * Set up a surface for directly accessing the pixels.
   *
   * Between calls to SurfaceBase.Lock() / Unlock(), you can write
   * to and read from `GetPixels()`, using the pixel format stored in
   * `GetFormat()`. Once you are done accessing the surface, you should use
   * Unlock() to release it or let the destructor take care of this
   * for you.
   *
   * Not all surfaces require locking. If `SurfaceBase.MustLock(surface)`
   * evaluates to false, then you can read and write to the surface at any time,
   * and the pixel format of the surface will not change.
   *
   * @returns SurfaceLock object that converts to true on success or false on
   * failure; call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MustLock()
   * @sa SurfaceLock.Unlock()
   */
  SurfaceLock Lock() &;

  /**
   * @brief Set the RLE acceleration hint for a surface.
   *
   * If RLE is enabled, color key and alpha blending blits are much faster, but
   * the surface must be locked before directly accessing the pixels.
   *
   * @param enabled true to enable RLE acceleration, false to disable it.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool SetRLE(bool enabled) { return SDL_SetSurfaceRLE(T::get(), enabled); }

  /**
   * @brief Returns whether the surface is RLE enabled.
   *
   * @returns true if the surface is RLE enabled, false otherwise.
   */
  bool HasRLE() const { return SDL_SurfaceHasRLE(T::get()); }

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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorKey()
   * @sa SetRLE()
   * @sa HasColorKey()
   */
  bool SetColorKey(Color key) { return SetColorKey(MapColor(key)); }

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
   * @param key the transparent pixel or std::nullopt to disable it.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorKey()
   * @sa SetRLE()
   * @sa HasColorKey()
   */
  bool SetColorKey(std::optional<Uint32> key)
  {
    return SDL_SetSurfaceColorKey(T::get(), key.has_value(), key.value_or(0));
  }

  /**
   * Unset the color key (transparent pixel) in a surface.
   *
   * The color key defines a pixel value that will be treated as transparent in
   * a blit. For example, one can use this to specify that cyan pixels should be
   * considered transparent, and therefore not rendered.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool ClearColorKey() { return SDL_SetSurfaceColorKey(T::get(), false, 0); }

  /**
   * @brief Returns whether the surface has a color key.
   *
   * @returns true if the surface has a color key, false otherwise.
   */
  bool HasColorKey() const { return SDL_SurfaceHasColorKey(T::get()); }

  /**
   * @brief Get the color key (transparent pixel) for a surface.
   *
   * The color key is a pixel of the format used by the surface, as generated by
   * MapRGB().
   *
   * If the surface doesn't have color key enabled this function returns
   * std::nullopt.
   *
   * @returns the color key as Color or std::nullopt if it does not have one.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorKey()
   * @sa HasColorKey()
   */
  std::optional<Color> GetColorKey() const
  {
    if (Color color; GetColorKey(&color)) return color;
    return std::nullopt;
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorKey()
   * @sa HasColorKey()
   */
  bool GetColorKey(Color* key) const
  {
    if (Uint32 color; GetColorKey(&color)) {
      *key = MapColor(color);
      return true;
    }
    return false;
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorKey()
   * @sa HasColorKey()
   */
  bool GetColorKey(Uint32* key) const
  {
    return SDL_GetSurfaceColorKey(T::get(), key);
  }

  /**
   * @brief Set an additional color value multiplied into blit operations.
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool SetColorMod(Uint8 r, Uint8 g, Uint8 b)
  {
    return SDL_SetSurfaceColorMod(T::get(), r, g, b);
  }

  /**
   * @brief Get the additional color value multiplied into blit operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool GetColorMod(Uint8* r, Uint8* g, Uint8* b) const
  {
    return SDL_GetSurfaceColorMod(T::get(), r, g, b);
  }

  /**
   * @brief Set an additional alpha value used in blit operations.
   *
   * When this surface is blitted, during the blit operation the source alpha
   * value is modulated by this alpha value according to the following formula:
   *
   * `srcA = srcA * (alpha / 255)`
   *
   * @param alpha the alpha value multiplied into blit operations.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool SetAlphaMod(Uint8 alpha)
  {
    return SDL_SetSurfaceAlphaMod(T::get(), alpha);
  }

  /**
   * @brief Get the additional alpha value used in blit operations.
   *
   * @returns alpha on success or std::nullopt on failure; call GetError()
   * for more information.
   */
  std::optional<Uint8> GetAlphaMod() const
  {
    if (Uint8 alpha; SDL_GetSurfaceAlphaMod(T::get(), &alpha)) return alpha;
    return std::nullopt;
  }

  /**
   * @brief Set an additional color and alpha value multiplied into blit
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool SetColorAndAlphaMod(Color color)
  {
    return SetColorMod(color.r, color.g, color.b) && SetAlphaMod(color.a);
  }

  /**
   * @brief Get the additional color and alpha value multiplied into blit
   * operations.
   *
   * @returns a Color containing RGBA value on success or std::nullopt on
   * failure; call GetError() for more information.
   */
  std::optional<Color> GetColorAndAlphaMod() const
  {
    if (Color c; GetColorMod(&c.r, &c.g, &c.b) &&
                 SDL_GetSurfaceAlphaMod(T::get(), &c.a)) {
      return c;
    }
    return std::nullopt;
  }

  /**
   * Set the blend mode used for blit operations.
   *
   * To copy a surface to another surface (or texture) without blending with the
   * existing data, the blendmode of the SOURCE surface should be set to
   * `SDL_BLENDMODE_NONE`.
   *
   * @param blendMode the SDL_BlendMode to use for blit blending.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBlendMode()
   */
  bool SetBlendMode(BlendMode blendMode)
  {
    return SDL_SetSurfaceBlendMode(T::get(), blendMode);
  }

  /**
   * Get the blend mode used for blit operations.
   *
   * @return the blendMode on success or std::nullopt on failure; call
   * GetError() for more information.
   */
  std::optional<BlendMode> GetBlendMode() const
  {
    if (BlendMode blendMode; SDL_GetSurfaceBlendMode(T::get(), &blendMode)) {
      return blendMode;
    }
    return std::nullopt;
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
   * @param rect the SDL_Rect structure representing the clipping rectangle or
   *        nullopt to disable it
   * @returns true if the rectangle intersects the surface, otherwise false and
   *          blits will be completely clipped.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ResetClipRect()
   */
  bool SetClipRect(OptionalRef<const SDL_Rect> rect)
  {
    return SDL_SetSurfaceClipRect(T::get(), rect);
  }

  /**
   * @brief Disable the clipping rectangle for a surface.
   *
   * @returns true if clips was reset successfully
   *
   * @sa SetClipRect()
   */
  bool ResetClipRect() { return SDL_SetSurfaceClipRect(T::get(), nullptr); }

  /**
   * @brief Get the clipping rectangle for a surface.
   *
   * When `surface` is the destination of a blit, only the area within the clip
   * rectangle is drawn into.
   *
   * @returns the Rect structure filled in with the clipping rectangle for the
   * surface on success, or std::nullopt on failure; call GetError() for
   * more information.
   */
  std::optional<Rect> GetClipRect() const
  {
    if (Rect r; SDL_GetSurfaceClipRect(T::get(), &r)) { return r; }
    return std::nullopt;
  }

  /**
   * Flip a surface vertically or horizontally.
   *
   * @param flip the direction to flip.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Flip(FlipMode flip) { return SDL_FlipSurface(T::get(), flip); }

  /**
   * @brief Creates a new surface identical to the existing surface.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * The returned surface  automatically calls SDL_free after result is out of
   * scope.
   *
   * @returns a copy of the surface or NULL on failure; call GetError() for
   *          more information.
   */
  Surface Duplicate() const { return {SDL_DuplicateSurface(T::get())}; }

  /**
   * @brief Creates a new surface identical to the existing surface, scaled to
   * the desired size.
   *
   * The returned surface  automatically calls SDL_free after result is out of
   * scope.
   *
   * @param width the width of the new surface.
   * @param height the height of the new surface.
   * @param scaleMode the ScaleMode to be used.
   * @returns a copy of the surface or NULL on failure; call GetError() for
   *          more information.
   */
  Surface Scale(int width, int height, ScaleMode scaleMode) const
  {
    return {SDL_ScaleSurface(T::get(), width, height, scaleMode)};
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
   * palette, you can use SDL_ConvertSurfaceAndColorspace() instead.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * @param format the new pixel format.
   * @returns the new Surface that is created or NULL on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Surface Convert(PixelFormat format) const
  {
    return {SDL_ConvertSurface(T::get(), format)};
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
   * @param format the new pixel format.
   * @param palette an optional palette to use for indexed formats, may be NULL.
   * @param colorspace the new colorspace.
   * @param props an SDL_PropertiesID with additional color properties, or 0.
   * @returns the new SDL_Surface structure that is created or NULL on failure;
   *          call SDL_GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Surface Convert(PixelFormat format,
                  PaletteRef palette,
                  Colorspace colorspace,
                  PropertiesRef props) const
  {
    return SDL_ConvertSurfaceAndColorspace(
      T::get(), format, palette, colorspace, props);
  }

  /**
   * Premultiply the alpha in a surface.
   *
   * This is safe to use with src == dst, but not for other overlapping areas.
   *
   * @param linear true to convert from sRGB to linear space for the alpha
   *               multiplication, false to do multiplication in sRGB space.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool PremultiplyAlpha(bool linear)
  {
    return SDL_PremultiplySurfaceAlpha(T::get(), linear);
  }

  // TODO SDL_ConvertSurfaceAndColorspace

  /**
   * Clear a surface with a specific color, with floating point precision.
   *
   * This function handles all surface formats, and ignores any clip rectangle.
   *
   * If the surface is YUV, the color is assumed to be in the sRGB colorspace,
   * otherwise the color is assumed to be in the colorspace of the surface.
   *
   * @param color the color of the pixel, normally in the range 0-1.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Clear(SDL_FColor color)
  {
    return SDL_ClearSurface(T::get(), color.r, color.g, color.b, color.a);
  }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by SDL_MapRGB() or SDL_MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * the clip rectangle.
   *
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool Fill(SDL_Color color) { return Fill(MapColor(color)); }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by SDL_MapRGB() or SDL_MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * the clip rectangle.
   *
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool Fill(Uint32 color)
  {
    return SDL_FillSurfaceRect(T::get(), nullptr, color);
  }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rect the SDL_Rect structure representing the rectangle to fill.
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool FillRect(const SDL_Rect& rect, SDL_Color color)
  {
    return FillRect(rect, MapColor(color));
  }

  /**
   * @brief Perform a fast fill of a rectangle with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by SDL_MapRGB() or SDL_MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rect the SDL_Rect structure representing the rectangle to fill.
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool FillRect(const SDL_Rect& rect, Uint32 color)
  {
    return SDL_FillSurfaceRect(T::get(), &rect, color);
  }

  /**
   * Perform a fast fill of a set of rectangles with a specific color.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rects an array of SDL_Rects representing the rectangles to fill.
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FillRect()
   */
  bool FillRects(SpanRef<const SDL_Rect> rects, SDL_Color color)
  {
    return FillRects(rects, MapColor(color));
  }

  /**
   * Perform a fast fill of a set of rectangles with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by SDL_MapRGB() or SDL_MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rects an array of SDL_Rects representing the rectangles to fill.
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FillRect()
   */
  bool FillRects(SpanRef<const SDL_Rect> rects, Uint32 color)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_UINT32);
    return SDL_FillSurfaceRects(T::get(), rects.data(), rects.size(), color);
  }

  /**
   * Performs a fast blit from the source surface to the destination surface
   * with clipping.
   *
   * If either `srcrect` or `dstrect` are NULL, the entire surface (`src` or
   * `dst`) is copied while ensuring clipping to `dst->clip_rect`.
   *
   * The final blit rectangles are saved in `srcrect` and `dstrect` after all
   * clipping is performed.
   *
   * The blit function should not be called on a locked surface.
   *
   * The blit semantics for surfaces with and without blending and colorkey are
   * defined as follows:
   *
   * ```
   *    RGBA->RGB:
   *      Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGBA:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB, set destination alpha to source per-surface alpha value.
   *     both:
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       source color key.
   *
   *   RGBA->RGBA:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy all of RGBA to the destination.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGB:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
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
   *                SDL_BlitSurfaceScaled().
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa BlitScaled()
   */
  bool Blit(SurfaceRef src,
            OptionalRef<const SDL_Rect> srcrect,
            const SDL_Point& dstpos)
  {
    return Blit(src, srcrect, Rect{dstpos, {}});
  }

  /**
   * Performs a fast blit from the source surface to the destination surface
   * with clipping.
   *
   * If either `srcrect` or `dstrect` are NULL, the entire surface (`src` or
   * `dst`) is copied while ensuring clipping to `dst->clip_rect`.
   *
   * The final blit rectangles are saved in `srcrect` and `dstrect` after all
   * clipping is performed.
   *
   * The blit function should not be called on a locked surface.
   *
   * The blit semantics for surfaces with and without blending and colorkey are
   * defined as follows:
   *
   * ```
   *    RGBA->RGB:
   *      Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGBA:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB, set destination alpha to source per-surface alpha value.
   *     both:
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       source color key.
   *
   *   RGBA->RGBA:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy all of RGBA to the destination.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGB:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB.
   *     both:
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       source color key.
   * ```
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param dstrect the SDL_Point structure representing the x and y position in
   *                the destination surface, or NULL for (0,0). The width and
   *                height are ignored, and are copied from `srcrect`. If you
   *                want a specific width and height, you should use
   *                SDL_BlitSurfaceScaled().
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa BlitScaled()
   */
  bool Blit(SurfaceRef src,
            OptionalRef<const SDL_Rect> srcrect,
            OptionalRef<const SDL_Rect> dstrect)
  {
    return SDL_BlitSurface(src.get(), srcrect, T::get(), dstrect);
  }

  /**
   * Perform low-level surface blitting only.
   *
   * This is a semi-private blit function and it performs low-level surface
   * blitting, assuming the input rectangles have already been clipped.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, may not be NULL.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, may not be NULL.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool BlitUnchecked(SurfaceRef src,
                     const SDL_Rect& srcrect,
                     const SDL_Rect& dstrect)
  {
    return SDL_BlitSurfaceUnchecked(src.get(), srcrect, T::get(), dstrect);
  }

  /**
   * Perform a scaled blit to a destination surface, which may be of a different
   * format.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire
   *                destination surface.
   * @param scaleMode the SDL_ScaleMode to be used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool BlitScaled(SurfaceRef src,
                  OptionalRef<const SDL_Rect> srcrect,
                  OptionalRef<const SDL_Rect> dstrect,
                  ScaleMode scaleMode)
  {
    return SDL_BlitSurfaceScaled(
      src.get(), srcrect, T::get(), dstrect, scaleMode);
  }

  /**
   * Perform low-level surface scaled blitting only.
   *
   * This is a semi-private function and it performs low-level surface blitting,
   * assuming the input rectangles have already been clipped.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, may not be NULL.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, may not be NULL.
   * @param scaleMode the SDL_ScaleMode to be used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa BlitScaled()
   */
  bool BlitUncheckedScaled(SurfaceRef src,
                           const SDL_Rect& srcrect,
                           const SDL_Rect& dstrect,
                           ScaleMode scaleMode)
  {
    return SDL_BlitSurfaceScaled(
      src.get(), srcrect, T::get(), dstrect, scaleMode);
  }

#if SDL_VERSION_ATLEAST(3, 2, 4)
  /**
   * Perform a stretched pixel copy from one surface to another.
   *
   * @param src the Surface structure to be copied from.
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface.
   * @param scaleMode the ScaleMode to be used.
   * @returns true on success or false on failure; GetError() for more
   *          information.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.4.
   *
   * @sa BlitScaled()
   */
  bool Stretch(SurfaceRef src,
               const SDL_Rect& srcrect,
               const SDL_Rect& dstrect,
               ScaleMode scaleMode)
  {
    return SDL_StretchSurface(
      src.get(), &srcrect, T::get(), &dstrect, scaleMode);
  }
#endif

  /**
   * Perform a tiled blit to a destination surface, which may be of a different
   * format.
   *
   * The pixels in `srcrect` will be repeated as many times as needed to
   * completely fill `dstrect`.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool BlitTiled(SurfaceRef src,
                 OptionalRef<const SDL_Rect> srcrect,
                 OptionalRef<const SDL_Rect> dstrect)
  {
    return SDL_BlitSurfaceTiled(src.get(), srcrect, T::get(), dstrect);
  }

  /**
   * Perform a scaled and tiled blit to a destination surface, which may be of a
   * different format.
   *
   * The pixels in `srcrect` will be scaled and repeated as many times as needed
   * to completely fill `dstrect`.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param scale the scale used to transform srcrect into the destination
   *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
   *              64x64 tiles.
   * @param scaleMode scale algorithm to be used.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool BlitTiledWithScale(SurfaceRef src,
                          OptionalRef<const SDL_Rect> srcrect,
                          float scale,
                          SDL_ScaleMode scaleMode,
                          OptionalRef<const SDL_Rect> dstrect)
  {
    return SDL_BlitSurfaceTiledWithScale(
      src.get(), srcrect, scale, scaleMode, T::get(), dstrect);
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
   * @param left_width the width, in pixels, of the left corners in `srcrect`.
   * @param right_width the width, in pixels, of the right corners in `srcrect`.
   * @param top_height the height, in pixels, of the top corners in `srcrect`.
   * @param bottom_height the height, in pixels, of the bottom corners in
   *                      `srcrect`.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire surface.
   * @param srcrect the SDL_Rect structure representing the rectangle to be used
   *                for the 9-grid, or NULL to use the entire surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool Blit9Grid(SurfaceRef src,
                 OptionalRef<const SDL_Rect> srcrect,
                 int left_width,
                 int right_width,
                 int top_height,
                 int bottom_height,
                 OptionalRef<const SDL_Rect> dstrect)
  {
    return Blit9GridWithScale(src,
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
   * @param left_width the width, in pixels, of the left corners in `srcrect`.
   * @param right_width the width, in pixels, of the right corners in `srcrect`.
   * @param top_height the height, in pixels, of the top corners in `srcrect`.
   * @param bottom_height the height, in pixels, of the bottom corners in
   *                      `srcrect`.
   * @param scale the scale used to transform the corner of `srcrect` into the
   *              corner of `dstrect`, or 0.0f for an unscaled blit.
   * @param scaleMode scale algorithm to be used.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire surface.
   * @param srcrect the SDL_Rect structure representing the rectangle to be used
   *                for the 9-grid, or NULL to use the entire surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool Blit9GridWithScale(SurfaceRef src,
                          OptionalRef<const SDL_Rect> srcrect,
                          int left_width,
                          int right_width,
                          int top_height,
                          int bottom_height,
                          float scale,
                          SDL_ScaleMode scaleMode,
                          OptionalRef<const SDL_Rect> dstrect)
  {
    return SDL_BlitSurface9Grid(src.get(),
                                srcrect,
                                left_width,
                                right_width,
                                top_height,
                                bottom_height,
                                scale,
                                scaleMode,
                                T::get(),
                                dstrect);
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
   */
  Uint32 MapColor(Uint8 r, Uint8 g, Uint8 b) const
  {
    return SDL_MapSurfaceRGB(T::get(), r, g, b);
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
   * @return a pixel value.
   */
  Uint32 MapColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
  {
    return SDL_MapSurfaceRGBA(T::get(), r, g, b, a);
  }

  /**
   * @brief This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like SDL_GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @returns color  on success or std::nullopt on failure; call GetError() for
   * more information.
   */
  std::optional<Color> ReadPixel(int x, int y) const
  {
    if (Color c; ReadPixel(x, y, &c)) return c;
    return std::nullopt;
  }

  /**
   * @brief This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like SDL_GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param c a color pointer to be filled with the color information. Must not
   *          be NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, SDL_Color* c) const
  {
    return ReadPixel(x, y, &c->r, &c->g, &c->b, &c->a);
  }

  /**
   * @brief Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param c a color pointer to be filled with the color information. Must not
   *          be NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, SDL_FColor* c) const
  {
    return ReadPixel(x, y, &c->r, &c->g, &c->b, &c->a);
  }

  /**
   * Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * Like SDL_GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r a pointer filled in with the red channel, 0-255, or NULL to ignore
   *          this channel.
   * @param g a pointer filled in with the green channel, 0-255, or NULL to
   *          ignore this channel.
   * @param b a pointer filled in with the blue channel, 0-255, or NULL to
   *          ignore this channel.
   * @param a a pointer filled in with the alpha channel, 0-255, or NULL to
   *          ignore this channel.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadPixel(int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a) const
  {
    return SDL_ReadSurfacePixel(T::get(), x, y, r, g, b, a);
  }

  /**
   * @brief Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r a pointer filled in with the red channel, normally in the range
   *          0-1, or NULL to ignore this channel.
   * @param g a pointer filled in with the green channel, normally in the range
   *          0-1, or NULL to ignore this channel.
   * @param b a pointer filled in with the blue channel, normally in the range
   *          0-1, or NULL to ignore this channel.
   * @param a a pointer filled in with the alpha channel, normally in the range
   *          0-1, or NULL to ignore this channel.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, float* r, float* g, float* b, float* a) const
  {
    return SDL_ReadSurfacePixelFloat(T::get(), x, y, r, g, b, a);
  }

  /**
   * @brief This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like MapRGBA(), this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param c the color values, 0-255.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool WritePixel(int x, int y, SDL_Color c)
  {
    return WritePixel(x, y, c.r, c.g, c.b, c.a);
  }

  /**
   * @brief Writes a single pixel to a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param c the color values, normally in the range 0-1.
   * @returns true on success or false on failure; call GetError() for
   * more information.
   */
  bool WritePixel(int x, int y, SDL_FColor c)
  {
    return WritePixel(x, y, c.r, c.g, c.b, c.a);
  }

  /**
   * @brief This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like MapRGBA(), this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r the red channel value, 0-255.
   * @param g the green channel value, 0-255.
   * @param b the blue channel value, 0-255.
   * @param a the alpha channel value, 0-255.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool WritePixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
  {
    return SDL_WriteSurfacePixel(T::get(), x, y, r, g, b, a);
  }

  /**
   * @brief Writes a single pixel to a surface.
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool WritePixel(int x, int y, float r, float g, float b, float a)
  {
    return SDL_WriteSurfacePixelFloat(T::get(), x, y, r, g, b, a);
  }

  // Convenience functions to avoid dereferencing
  int GetWidth() const { return T::get()->w; }

  int GetHeight() const { return T::get()->w; }

  Point GetSize() const { return Point(GetWidth(), GetHeight()); }

  PixelFormat GetFormat() const { return T::get()->format; }
};

/**
 * @brief Locks a Surface for access to its pixels
 *
 * Only really necessary if Surface.MustLock() returns t
 */
class SurfaceLock
{
  SurfaceRef surface;

  /**
   * @sa SurfaceBase.Lock()
   */
  explicit SurfaceLock(SurfaceRef surface)
    : surface(std::move(surface))
  {
    if (!SDL_LockSurface(this->surface.get())) this->surface.reset();
  }

public:
  // default ctor
  SurfaceLock()
    : surface(nullptr)
  {
  }

  // Copy ctor
  SurfaceLock(const SurfaceLock& other) = delete;

  // Move ctor
  SurfaceLock(SurfaceLock&& other)
    : surface(other.surface.release())
  {
  }

  /**
   * destructor
   * @sa Unlock()
   */
  ~SurfaceLock() { Unlock(); }

  // Assignment operator
  SurfaceLock& operator=(SurfaceLock other)
  {
    std::swap(surface, other.surface);
    return *this;
  }

  /**
   * @brief Returns true if lock is active
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
   * @brief Get the pixel format
   */
  PixelFormat GetFormat() const { return surface->format; }

  template<ObjectBox<SDL_Surface*> T>
  friend class SurfaceBase;
};

/**
 * Load a BMP image from a seekable SDL data stream.
 *
 * @param src the data stream for the surface.
 * @param closeio if true, calls SDL_CloseIO() on `src` before returning, even
 *                in the case of an error.
 * @returns a Surface with the loaded content or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SaveBMP()
 */
inline Surface LoadBMP(SDL_IOStream* src, bool closeio)
{
  return Surface{SDL_LoadBMP_IO(src, closeio)};
}

/**
 * Load a BMP image from a file.
 *
 * @param file the BMP file to load.
 * @returns a Surface with the loaded content or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SaveBMP()
 */
inline Surface LoadBMP(StringParam file) { return Surface{SDL_LoadBMP(file)}; }

/**
 * Save a surface to a seekable SDL data stream in BMP format.
 *
 * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
 * BMP directly. Other RGB formats with 8-bit or higher get converted to a
 * 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
 * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
 * not supported.
 *
 * @param surface the SDL_Surface structure containing the image to be saved.
 * @param dst a data stream to save to.
 * @param closeio if true, calls SDL_CloseIO() on `dst` before returning, even
 *                in the case of an error.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SaveBMP()
 */
inline bool SaveBMP(SurfaceRef surface, SDL_IOStream* dst, bool closeio)
{
  return SDL_SaveBMP_IO(surface.get(), dst, closeio);
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
 * @param surface the SDL_Surface structure containing the image to be saved.
 * @param file a file to save to.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LoadBMP()
 */
inline bool SaveBMP(SurfaceRef surface, StringParam file)
{
  return SDL_SaveBMP(surface.get(), file);
}

/**
 * Copy a block of pixels of one format to another format.
 *
 * @param width the width of the block to copy, in pixels.
 * @param height the height of the block to copy, in pixels.
 * @param src_format an SDL_PixelFormat value of the `src` pixels format.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an SDL_PixelFormat value of the `dst` pixels format.
 * @param dst a pointer to be filled in with new pixel data.
 * @param dst_pitch the pitch of the destination pixels, in bytes.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ConvertPixelsAndColorspace()
 */
inline bool ConvertPixels(int width,
                          int height,
                          PixelFormat src_format,
                          const void* src,
                          int src_pitch,
                          PixelFormat dst_format,
                          void* dst,
                          int dst_pitch)
{
  return SDL_ConvertPixels(
    width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch);
}

/**
 * Copy a block of pixels of one format and colorspace to another format and
 * colorspace.
 *
 * @param width the width of the block to copy, in pixels.
 * @param height the height of the block to copy, in pixels.
 * @param src_format an SDL_PixelFormat value of the `src` pixels format.
 * @param src_colorspace an SDL_Colorspace value describing the colorspace of
 *                       the `src` pixels.
 * @param src_properties an SDL_PropertiesID with additional source color
 *                       properties, or 0.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an SDL_PixelFormat value of the `dst` pixels format.
 * @param dst_colorspace an SDL_Colorspace value describing the colorspace of
 *                       the `dst` pixels.
 * @param dst_properties an SDL_PropertiesID with additional destination color
 *                       properties, or 0.
 * @param dst a pointer to be filled in with new pixel data.
 * @param dst_pitch the pitch of the destination pixels, in bytes.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ConvertPixels()
 */
inline bool ConvertPixelsAndColorspace(int width,
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
  return SDL_ConvertPixelsAndColorspace(width,
                                        height,
                                        src_format,
                                        src_colorspace,
                                        src_properties,
                                        src,
                                        src_pitch,
                                        dst_format,
                                        dst_colorspace,
                                        dst_properties,
                                        dst,
                                        dst_pitch);
}

/**
 * Premultiply the alpha on a block of pixels.
 *
 * This is safe to use with src == dst, but not for other overlapping areas.
 *
 * @param width the width of the block to convert, in pixels.
 * @param height the height of the block to convert, in pixels.
 * @param src_format an SDL_PixelFormat value of the `src` pixels format.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an SDL_PixelFormat value of the `dst` pixels format.
 * @param dst a pointer to be filled in with premultiplied pixel data.
 * @param dst_pitch the pitch of the destination pixels, in bytes.
 * @param linear true to convert from sRGB to linear space for the alpha
 *               multiplication, false to do multiplication in sRGB space.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool PremultiplyAlpha(int width,
                             int height,
                             PixelFormat src_format,
                             const void* src,
                             int src_pitch,
                             PixelFormat dst_format,
                             void* dst,
                             int dst_pitch,
                             bool linear)
{
  return SDL_PremultiplyAlpha(width,
                              height,
                              src_format,
                              src,
                              src_pitch,
                              dst_format,
                              dst,
                              dst_pitch,
                              linear);
}

#pragma region impl
/// @}

inline void ObjectDeleter<SDL_Surface>::operator()(SurfaceRef surface) const
{
  surface.Destroy();
}

template<ObjectBox<SDL_Surface*> T>
SurfaceLock SurfaceBase<T>::Lock() &
{
  return SurfaceLock{*this};
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_SURFACE_H_ */
