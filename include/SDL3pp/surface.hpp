#ifndef SDL3PP_SURFACE_HPP_
#define SDL3PP_SURFACE_HPP_

#include <SDL3/SDL_surface.h>
#include "objectWrapper.hpp"
#include "pixels.hpp"
#include "properties.hpp"
#include "rect.hpp"
#include "stringParam.hpp"

namespace SDL {

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

/**
 * @brief The flip mode.
 *
 */
using FlipMode = SDL_FlipMode;

template<>
struct ObjectDeleter<SDL_Surface>
{
  void operator()(SDL_Surface* surface) { SDL_DestroySurface(surface); }
};

// Forward decl
template<class T>
struct SurfaceBase;

/**
 * @brief Handle to an owned surface
 */
using Surface = SurfaceBase<ObjectUnique<SDL_Surface>>;
using SurfaceUnique = Surface;

/**
 * @brief Handle to a non owned surface
 */
using SurfaceRef = SurfaceBase<ObjectRef<SDL_Surface>>;

/**
 * @brief A collection of pixels used in software blitting.
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
 */
template<class T>
struct SurfaceBase : T
{
  using T::T;

  /**
   * @brief Allocate a new surface with a specific pixel format.
   *
   * The pixels of the new surface are initialized to zero.
   *
   * @param width the width of the surface.
   * @param height the height of the surface.
   * @param format the PixelFormat for the new surface's pixel format.
   *
   * If fails window converts false; call SDL_GetError() for more information.
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
   * @returns the new SDL_Surface structure that is created or NULL on failure;
   *          call SDL_GetError() for more information.
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
   * @brief  Get the properties associated with a surface.
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
   *          SDL_GetError() for more information.
   */
  PropertiesRef GetProperties()
  {
    return SDL_GetSurfaceProperties(Get<T>(this));
  }

  /**
   * Set the colorspace used by a surface.
   *
   * Setting the colorspace doesn't change the pixels, only how they are
   * interpreted in color operations.
   *
   * @param colorspace a Colorspace value describing the surface colorspace.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool SetSurfaceColorspace(Colorspace colorspace)
  {
    return SDL_SetSurfaceColorspace(Get<T>(this), colorspace);
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
    return SDL_GetSurfaceColorspace(Get<T>(this));
  }

  /**
   * @brief Create a palette and associate it with a surface.
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
   *          the surface didn't have an index format); call SDL_GetError() for
   *          more information.
   */
  PaletteRef CreatePalette() { return SDL_CreateSurfacePalette(Get<T>(this)); }

  /**
   * @brief Get the palette used by a surface.
   *
   * @returns a pointer to the palette used by the surface, or NULL if there is
   *          no palette used.
   */
  PaletteRef GetPalette() const { return SDL_GetSurfacePalette(Get<T>(this)); }

  /**
   * @brief Set the palette used by a surface.
   *
   * A single palette can be shared with many surfaces.
   *
   * @param palette the Palette to use.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool SetPalette(PaletteRef palette)
  {
    return SDL_SetSurfacePalette(Get<T>(this), palette.Get());
  }

  // SDL_AddSurfaceAlternateImage

  /**
   * @brief Set the RLE acceleration hint for a surface.
   *
   * If RLE is enabled, color key and alpha blending blits are much faster, but
   * the surface must be locked before directly accessing the pixels.
   *
   * @param enabled true to enable RLE acceleration, false to disable it.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool SetRLE(bool enabled) { return SDL_SetSurfaceRLE(Get<T>(this), enabled); }

  /**
   * @brief Returns whether the surface is RLE enabled.
   *
   * @returns true if the surface is RLE enabled, false otherwise.
   */
  bool HasRLE() const { return SDL_SurfaceHasRLE(Get<T>(this)); }

  /**
   * @brief Set the color key (transparent pixel) in a surface.
   *
   * The color key defines a pixel value that will be treated as transparent in
   * a blit. For example, one can use this to specify that cyan pixels should be
   * considered transparent, and therefore not rendered.
   *
   * It is a pixel of the format used by the surface, as generated by
   * SDL_MapRGB().
   *
   * @param enabled true to enable color key, false to disable color key.
   * @param key the transparent pixel.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool SetColorKey(bool enabled, Uint32 key)
  {
    return SDL_SetSurfaceColorKey(Get<T>(this), enabled, key);
  }

  /**
   * @brief Set the color key (transparent pixel) in a surface.
   *
   * The color key defines a pixel value that will be treated as transparent in
   * a blit. For example, one can use this to specify that cyan pixels should be
   * considered transparent, and therefore not rendered.
   *
   * It is a pixel of the format used by the surface, as generated by
   * SDL_MapRGB().
   *
   * @param key the transparent pixel or std::nullopt to disable it.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool SetColorKey(std::optional<Uint32> key)
  {
    return SetColorKey(key.has_value(), key.value_or(0));
  }

  /**
   * @brief Returns whether the surface has a color key.
   *
   * @returns true if the surface has a color key, false otherwise.
   */
  bool HasColorKey() const { return SDL_SurfaceHasColorKey(Get<T>(this)); }

  /**
   * @brief Get the color key (transparent pixel) for a surface.
   *
   * The color key is a pixel of the format used by the surface, as generated by
   * MapRGB().
   *
   * If the surface doesn't have color key enabled this function returns
   * std::nullopt.
   *
   * @returns the color key as pixel or std::nullopt if it does not have one.
   */
  std::optional<Uint32> GetColorKey() const
  {
    if (Uint32 key; SDL_GetSurfaceColorKey(Get<T>(this), &key)) return key;
    return std::nullopt;
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool SetColorMod(Uint8 r, Uint8 g, Uint8 b)
  {
    return SDL_SetSurfaceColorMod(Get<T>(this), r, g, b);
  }

  /**
   * @brief Get the additional color value multiplied into blit operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool GetColorMod(Uint8* r, Uint8* g, Uint8* b) const
  {
    return SDL_GetSurfaceColorMod(Get<T>(this), r, g, b);
  }

  /**
   * @brief Set an additional alpha value used in blit operations.
   *
   * When this surface is blitted, during the blit operation the source alpha
   * value is modulated by this alpha value according to the following formula:
   *
   * `srcA = srcA * (alpha / 255)`
   *
   * @param the alpha value multiplied into blit operations.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool SetAlphaMod(Uint8 alpha)
  {
    return SDL_SetSurfaceAlphaMod(Get<T>(this), alpha);
  }

  /**
   * @brief Get the additional alpha value used in blit operations.
   *
   * @returns alpha on success or std::nullopt on failure; call SDL_GetError()
   * for more information.
   */
  std::optional<Uint8> GetAlphaMod() const
  {
    if (Uint8 alpha; SDL_GetSurfaceAlphaMod(Get<T>(this), &alpha)) return alpha;
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool SetColorAndAlphaMod(SDL_Color color)
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
                 SDL_GetSurfaceAlphaMod(Get<T>(this), &c.a)) {
      return c;
    }
    return std::nullopt;
  }

  /**
   * @briefSet the blend mode used for blit operations.
   *
   * To copy a surface to another surface (or texture) without blending with the
   * existing data, the blendmode of the SOURCE surface should be set to
   * `SDL_BLENDMODE_NONE`.
   *
   * @param blendMode the SDL_BlendMode to use for blit blending.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool SetBlendMode(SDL_BlendMode blendMode)
  {
    return SDL_SetSurfaceBlendMode(Get<T>(this), blendMode);
  }

  /**
   * @brief Get the blend mode used for blit operations.
   *
   * @return the blendMode on success or std::nullopt on failure; call
   * SDL_GetError() for more information.
   */
  std::optional<SDL_BlendMode> GetBlendMode() const
  {
    if (SDL_BlendMode blendMode;
        SDL_GetSurfaceBlendMode(Get<T>(this), &blendMode)) {
      return blendMode;
    }
    return std::nullopt;
  }

  /**
   * @brief Set the clipping rectangle for a surface.
   *
   * When `surface` is the destination of a blit, only the area within the clip
   * rectangle is drawn into.
   *
   * Note that blits are automatically clipped to the edges of the source and
   * destination surfaces.
   *
   * @param rect the SDL_Rect structure representing the clipping rectangle or
   * nullopt to disable it
   * @returns true if the rectangle intersects the surface, otherwise false and
   *          blits will be completely clipped.
   *
   * @sa ResetClipRect()
   */
  bool SetClipRect(std::optional<Rect> rect)
  {
    return SDL_SetSurfaceClipRect(Get<T>(this), rect ? &rect.value() : nullptr);
  }

  bool ResetClipRect() { return SDL_SetSurfaceClipRect(Get<T>(this), nullptr); }

  /**
   * @brief Get the clipping rectangle for a surface.
   *
   * When `surface` is the destination of a blit, only the area within the clip
   * rectangle is drawn into.
   *
   * @returns the Rect structure filled in with the clipping rectangle for the
   * surface on success, or std::nullopt on failure; call SDL_GetError() for
   * more information.
   */
  std::optional<Rect> GetClipRect() const
  {
    if (Rect r; SDL_GetSurfaceClipRect(Get<T>(this), &r)) { return r; }
    return std::nullopt;
  }

  /**
   * @brief Flip a surface vertically or horizontally.
   *
   * @param flipMode the direction to flip.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool FlipSurface(FlipMode flipMode)
  {
    return SDL_FlipSurface(Get<T>(this), flipMode);
  }

  /**
   * @brief Creates a new surface identical to the existing surface.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * The returned surface  automatically calls SDL_free after result is out of
   * scope.
   *
   * @returns a copy of the surface or NULL on failure; call SDL_GetError() for
   *          more information.
   */
  Surface DuplicateSurface() const
  {
    return {SDL_DuplicateSurface(Get<T>(this))};
  }

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
   * @returns a copy of the surface or NULL on failure; call SDL_GetError() for
   *          more information.
   */
  Surface ScaleSurface(int width, int height, ScaleMode scaleMode) const
  {
    return {SDL_ScaleSurface(Get<T>(this), width, height, scaleMode)};
  }

  /**
   * @brief Copy an existing surface to a new surface of the specified format.
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
   * @returns the new Surface  that is created or NULL on failure; call
   *          SDL_GetError() for more information.
   */
  Surface ConvertSurface(PixelFormat format)
  {
    return {SDL_ConvertSurface(Get<T>(this), format)};
  }

  // TODO SDL_ConvertSurfaceAndColorspace

  /**
   * @brief Clear a surface with a specific color, with floating point
   * precision.
   *
   * This function handles all surface formats, and ignores any clip rectangle.
   *
   * If the surface is YUV, the color is assumed to be in the sRGB colorspace,
   * otherwise the color is assumed to be in the colorspace of the surface.
   *
   * @param c the color of the pixel, normally in the range 0-1.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool ClearSurface(SDL_FColor c)
  {
    return SDL_ClearSurface(Get<T>(this), c.r, c.g, c.b, c.a);
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
   * the clip rectangle.
   *
   * @param color the color to fill with.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool Fill(Uint32 color)
  {
    return SDL_FillSurfaceRect(Get<T>(this), nullptr, color);
  }

  bool Fill(SDL_Color color) { return Fill(MapColor(color)); }

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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool FillRect(const SDL_Rect& rect, Uint32 color)
  {
    return SDL_FillSurfaceRect(Get<T>(this), &rect, color);
  }

  bool FillRect(const SDL_Rect& rect, SDL_Color color)
  {
    return FillRect(&rect, MapColor(color));
  }

  /**
   * @brief Perform a fast fill of a set of rectangles with a specific color.
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
   * @param count the number of rectangles in the array.
   * @param color the color to fill with.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool FillRects(const SDL_Rect* rects, int count, Uint32 color)
  {
    return SDL_FillSurfaceRects(Get<T>(this), rects, count, color);
  }
  bool FillRects(const SDL_Rect* rects, int count, SDL_Color color)
  {
    return FillRects(rects, count, MapColor(color));
  }
  bool FillRects(std::span<const SDL_Rect> rects, Uint32 color)
  {
    return FillRects(rects.data(), rects.size(), color);
  }
  bool FillRects(std::span<const SDL_Rect> rects, SDL_Color color)
  {
    return FillRects(rects.data(), rects.size(), MapColor(color));
  }

  /**
   * @brief Map an RGB triple to an opaque pixel value for a surface.
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
    return SDL_MapSurfaceRGB(Get<T>(this), r, g, b);
  }

  /**
   * @brief Map an RGBA quadruple to a pixel value for a surface.
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
    return SDL_MapSurfaceRGBA(Get<T>(this), r, g, b, a);
  }

  /**
   * @brief Map an RGBA quadruple to a pixel value for a surface.
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
   * @param c the color.
   * @return a pixel value.
   */
  Uint32 MapColor(SDL_Color c) const { return MapColor(c.r, c.g, c.b, c.a); }

  /**
   * @brief This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a) const
  {
    return SDL_ReadSurfacePixel(Get<T>(this), x, y, r, g, b, a);
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, SDL_Color* c) const
  {
    return ReadPixel(x, y, &c->r, &c->g, &c->b, &c->a);
  }

  std::optional<Color> ReadPixel(int x, int y) const
  {
    if (Color c; ReadPixel(x, y, &c)) return c;
    return std::nullopt;
  }

  /**
   * @brief Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param surface the surface to read.
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, float* r, float* g, float* b, float* a) const
  {
    return SDL_ReadSurfacePixelFloat(Get<T>(this), x, y, r, g, b, a);
  }

  /**
   * @brief Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param surface the surface to read.
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param c a color pointer to be filled with the color information. Must not
   *          be NULL.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, SDL_FColor* c) const
  {
    return ReadPixel(x, y, &c->r, &c->g, &c->b, &c->a);
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool WritePixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
  {
    return SDL_WriteSurfacePixel(Get<T>(this), x, y, r, g, b, a);
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
   * @returns true on success or false on failure; call SDL_GetError() for more
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
   * @param r the red channel value, normally in the range 0-1.
   * @param g the green channel value, normally in the range 0-1.
   * @param b the blue channel value, normally in the range 0-1.
   * @param a the alpha channel value, normally in the range 0-1.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   */
  bool WritePixel(int x, int y, float r, float g, float b, float a)
  {
    return SDL_WriteSurfacePixelFloat(Get<T>(this), x, y, r, g, b, a);
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
   * @returns true on success or false on failure; call SDL_GetError() for
   * more information.
   */
  bool WritePixel(int x, int y, SDL_FColor c)
  {
    return WritePixel(x, y, c.r, c.g, c.b, c.a);
  }

  // Convenience functions to avoid dereferencing
  int GetWidth() const { return Get<T>(this)->w; }

  int GetHeight() const { return Get<T>(this)->w; }

  Point GetSize() const { return Point(GetWidth(), GetHeight()); }

  PixelFormat GetFormat() const { return Get<T>(this)->format; }
};

/**
 * @brief Load a BMP image from a seekable SDL data stream.
 *
 * The new surface is automatically freed when out of scope
 * if you want to manually manage its lifetime call Surface.Release()
 * to get its raw pointer (and optionally assign it to SurfaceRef).
 *
 * @param src the data stream for the surface.
 * @param closeio if true, calls SDL_CloseIO() on `src` before returning, even
 *                in the case of an error.
 * @returns a handler to a new SDL_Surface structure or falsy on failure; call
 *          SDL_GetError() for more information.
 */
inline Surface LoadBMP(SDL_IOStream* src, bool closeio)
{
  return Surface{SDL_LoadBMP_IO(src, closeio)};
}

/**
 * @brief  Load a BMP image from a file.
 *
 * The new surface is automatically freed when out of scope
 * if you want to manually manage its lifetime call Surface.Release()
 * to get its raw pointer (and optionally assign it to SurfaceRef).
 *
 * @param file the BMP file to load.
 * @returns a handler to a new SDL_Surface structure or falsy on failure; call
 *          SDL_GetError() for more information.
 */
inline Surface LoadBMP(StringParam file) { return Surface{SDL_LoadBMP(file)}; }

/**
 * @brief Save a surface to a seekable SDL data stream in BMP format.
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
 */
inline bool SaveBMP(SurfaceRef surface, SDL_IOStream* dst, bool closeio)
{
  return SDL_SaveBMP_IO(const_cast<SDL_Surface*>(surface.Get()), dst, closeio);
}

/**
 * @brief Save a surface to a file.
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
 */
inline bool SaveBMP(SurfaceRef surface, StringParam file)
{
  return SDL_SaveBMP(const_cast<SDL_Surface*>(surface.Get()), file);
}

// TODO SDL_ConvertPixels

/**
 * @brief Performs a fast blit from the source surface to the destination
 * surface with clipping.
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
 * @param srcRect the SDL_Rect structure representing the rectangle to be
 *                copied, may not be NULL.
 * @param dst the SDL_Surface structure that is the blit target.
 * @param dstRect the SDL_Rect structure representing the target rectangle in
 *                the destination surface, may not be NULL.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety The same destination surface should not be used from two
 *               threads at once. It is safe to use the same source surface
 *               from multiple threads.
 */
inline bool BlitSurface(SurfaceRef src,
                        std::optional<Rect> srcRect,
                        SurfaceRef dst,
                        const SDL_Rect& dstRect = {0, 0})
{
  return SDL_BlitSurface(
    src.Get(), srcRect ? &*srcRect : nullptr, dst.Get(), &dstRect);
}
inline bool BlitSurface(SurfaceRef src,
                        SurfaceRef dst,
                        const SDL_Rect& dstRect = {0, 0})
{
  return BlitSurface(src, std::nullopt, dst, dstRect);
}

/**
 * @brief Perform low-level surface blitting only.
 *
 * This is a semi-private blit function and it performs low-level surface
 * blitting, assuming the input rectangles have already been clipped.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety The same destination surface should not be used from two
 *               threads at once. It is safe to use the same source surface
 *               from multiple threads.
 *
 */
inline bool BlitSurfaceUnchecked(SurfaceRef src,
                                 const SDL_Rect& srcRect,
                                 SurfaceRef dst,
                                 const SDL_Rect& dstRect = {0, 0})
{
  return SDL_BlitSurfaceUnchecked(src.Get(), &srcRect, dst.Get(), &dstRect);
}

/**
 * @brief Perform a scaled blit to a destination surface, which may be of a
 * different format.
 *
 * @param src the SDL_Surface structure to be copied from.
 * @param srcrect the SDL_Rect structure representing the rectangle to be
 *                copied, or std::nullopt to copy the entire surface.
 * @param dst the SDL_Surface structure that is the blit target.
 * @param dstrect the SDL_Rect structure representing the target rectangle in
 *                the destination surface, or std::nullopt to fill the entire
 *                destination surface.
 * @param scaleMode the SDL_ScaleMode to be used.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety The same destination surface should not be used from two
 *               threads at once. It is safe to use the same source surface
 *               from multiple threads.
 */
inline bool BlitSurfaceScaled(SurfaceRef src,
                              std::optional<Rect> srcRect,
                              SurfaceRef dst,
                              std::optional<Rect> dstRect,
                              ScaleMode scaleMode)
{
  return SDL_BlitSurfaceScaled(src.Get(),
                               srcRect ? &*srcRect : nullptr,
                               dst.Get(),
                               dstRect ? &*dstRect : nullptr,
                               scaleMode);
}

/**
 * @brief Perform low-level surface scaled blitting only.
 *
 * This is a semi-private function and it performs low-level surface blitting,
 * assuming the input rectangles have already been clipped.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface.
 * @param scaleMode the ScaleMode to be used.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety The same destination surface should not be used from two
 *               threads at once. It is safe to use the same source surface
 *               from multiple threads.
 */
inline bool BlitSurfaceUncheckedScaled(SurfaceRef src,
                                       const SDL_Rect& srcRect,
                                       SurfaceRef dst,
                                       const SDL_Rect& dstRect,
                                       ScaleMode scaleMode)
{
  return SDL_BlitSurfaceUncheckedScaled(
    src.Get(), &srcRect, dst.Get(), &dstRect, scaleMode);
}

/**
 * @brief Perform a tiled blit to a destination surface, which may be of a
 * different format.
 *
 * The pixels in `srcrect` will be repeated as many times as needed to
 * completely fill `dstrect`.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied, or std::nullopt to copy the entire surface.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface, or std::nullopt to fill the entire
 *                surface.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety The same destination surface should not be used from two
 *               threads at once. It is safe to use the same source surface
 *               from multiple threads.
 */
inline bool BlitSurfaceTiled(SurfaceRef src,
                             std::optional<Rect> srcRect,
                             SurfaceRef dst,
                             std::optional<Rect> dstRect)
{
  return SDL_BlitSurfaceTiled(src.Get(),
                              srcRect ? &*srcRect : nullptr,
                              dst.Get(),
                              dstRect ? &*dstRect : nullptr);
}

/**
 * @brief Perform a scaled and tiled blit to a destination surface, which may be
 * of a different format.
 *
 * The pixels in `srcrect` will be scaled and repeated as many times as needed
 * to completely fill `dstrect`.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be
 *                copied, or std::nullopt to copy the entire surface.
 * @param scale the scale used to transform srcrect into the destination
 *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
 *              64x64 tiles.
 * @param scaleMode scale algorithm to be used.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in
 *                the destination surface, or std::nullopt to fill the entire
 *                surface.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety The same destination surface should not be used from two
 *               threads at once. It is safe to use the same source surface
 *               from multiple threads.
 */
inline bool BlitSurfaceTiledWithScale(SurfaceRef src,
                                      std::optional<Rect> srcRect,
                                      float scale,
                                      ScaleMode scaleMode,
                                      SurfaceRef dst,
                                      std::optional<Rect> dstRect)
{
  return SDL_BlitSurfaceTiledWithScale(src.Get(),
                                       srcRect ? &*srcRect : nullptr,
                                       scale,
                                       scaleMode,
                                       dst.Get(),
                                       dstRect ? &*dstRect : nullptr);
}

/**
 * @brief Perform a scaled blit using the 9-grid algorithm to a destination
 * surface, which may be of a different format.
 *
 * The pixels in the source surface are split into a 3x3 grid, using the
 * different corner sizes for each corner, and the sides and center making up
 * the remaining pixels. The corners are then scaled using `scale` and fit
 * into the corners of the destination rectangle. The sides and center are
 * then stretched into place to cover the remaining destination rectangle.
 *
 * @param src the Surface structure to be copied from.
 * @param srcrect the Rect structure representing the rectangle to be used
 *                for the 9-grid, or std::nullopt to use the entire surface.
 * @param left_width the width, in pixels, of the left corners in `srcrect`.
 * @param right_width the width, in pixels, of the right corners in `srcrect`.
 * @param top_height the height, in pixels, of the top corners in `srcrect`.
 * @param bottom_height the height, in pixels, of the bottom corners in
 *                      `srcrect`.
 * @param scale the scale used to transform the corner of `srcrect` into the
 *              corner of `dstrect`, or 0.0f for an unscaled blit.
 * @param scaleMode scale algorithm to be used.
 * @param dst the Surface structure that is the blit target.
 * @param dstrect the Rect structure representing the target rectangle in the
 *                destination surface, or std::nullopt to fill the entire
 *                surface.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety The same destination surface should not be used from two
 *               threads at once. It is safe to use the same source surface
 *               from multiple threads.
 */
inline bool BlitSurface9Grid(SurfaceRef src,
                             std::optional<Rect> srcRect,
                             int leftWidth,
                             int rightWidth,
                             int topHeight,
                             int bottomHeight,
                             float scale,
                             SDL_ScaleMode scaleMode,
                             SurfaceRef dst,
                             std::optional<Rect> dstRect)
{
  return SDL_BlitSurface9Grid(src.Get(),
                              srcRect ? &*srcRect : nullptr,
                              leftWidth,
                              rightWidth,
                              topHeight,
                              bottomHeight,
                              scale,
                              scaleMode,
                              dst.Get(),
                              dstRect ? &*dstRect : nullptr);
}
inline bool BlitSurface9Grid(SurfaceRef src,
                             std::optional<Rect> srcRect,
                             int leftWidth,
                             int rightWidth,
                             int topHeight,
                             int bottomHeight,
                             SurfaceRef dst,
                             std::optional<Rect> dstRect)
{
  return BlitSurface9Grid(src,
                          srcRect,
                          leftWidth,
                          rightWidth,
                          topHeight,
                          bottomHeight,
                          0,
                          SDL_SCALEMODE_NEAREST,
                          dst,
                          dstRect);
}

} // namespace SDL

#endif /* SDL3PP_SURFACE_HPP_ */
