#ifndef SDL3PP_PIXELS_H_
#define SDL3PP_PIXELS_H_

#include <span>
#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_version.h>
#include "SDL3pp_objectWrapper.h"
#include "SDL3pp_spanRef.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryPixels Pixel Formats and Conversion Routines
 *
 * SDL offers facilities for pixel management.
 *
 * Largely these facilities deal with pixel _format_: what does this set of
 * bits represent?
 *
 * If you mostly want to think of a pixel as some combination of red, green,
 * blue, and maybe alpha intensities, this is all pretty straightforward, and
 * in many cases, is enough information to build a perfectly fine game.
 *
 * However, the actual definition of a pixel is more complex than that:
 *
 * Pixels are a representation of a color in a particular color space.
 *
 * The first characteristic of a color space is the color type. SDL
 * understands two different color types, RGB and YCbCr, or in SDL also
 * referred to as YUV.
 *
 * RGB colors consist of red, green, and blue channels of color that are added
 * together to represent the colors we see on the screen.
 *
 * https://en.wikipedia.org/wiki/RGB_color_model
 *
 * YCbCr colors represent colors as a Y luma brightness component and red and
 * blue chroma color offsets. This color representation takes advantage of the
 * fact that the human eye is more sensitive to brightness than the color in
 * an image. The Cb and Cr components are often compressed and have lower
 * resolution than the luma component.
 *
 * https://en.wikipedia.org/wiki/YCbCr
 *
 * When the color information in YCbCr is compressed, the Y pixels are left at
 * full resolution and each Cr and Cb pixel represents an average of the color
 * information in a block of Y pixels. The chroma location determines where in
 * that block of pixels the color information is coming from.
 *
 * The color range defines how much of the pixel to use when converting a
 * pixel into a color on the display. When the full color range is used, the
 * entire numeric range of the pixel bits is significant. When narrow color
 * range is used, for historical reasons, the pixel uses only a portion of the
 * numeric range to represent colors.
 *
 * The color primaries and white point are a definition of the colors in the
 * color space relative to the standard XYZ color space.
 *
 * https://en.wikipedia.org/wiki/CIE_1931_color_space
 *
 * The transfer characteristic, or opto-electrical transfer function (OETF),
 * is the way a color is converted from mathematically linear space into a
 * non-linear output signals.
 *
 * https://en.wikipedia.org/wiki/Rec._709#Transfer_characteristics
 *
 * The matrix coefficients are used to convert between YCbCr and RGB colors.
 *
 * @{
 */

// Forward decl
template<class T>
struct PaletteBase;

/**
 * Handle to a non owned surface
 *
 * @ingroup resource
 *
 * @sa resource
 * @sa PaletteBase
 * @sa Palette
 */
using PaletteRef = PaletteBase<ObjectRef<SDL_Palette>>;

template<>
struct ObjectDeleter<SDL_Palette>
{
  void operator()(PaletteRef palette) const;
};

/**
 * Handle to an owned surface
 *
 * @ingroup resource
 *
 * @sa resource
 * @sa PaletteBase
 * @sa PaletteRef
 */
using Palette = PaletteBase<ObjectUnique<SDL_Palette>>;

// Forward decl
struct Color;

/**
 * @name PixelTypes
 * @{
 */

/**
 * Pixel type.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PixelType = SDL_PixelType;

constexpr PixelType PIXELTYPE_UNKNOWN = SDL_PIXELTYPE_UNKNOWN;

constexpr PixelType PIXELTYPE_INDEX1 = SDL_PIXELTYPE_INDEX1;

constexpr PixelType PIXELTYPE_INDEX4 = SDL_PIXELTYPE_INDEX4;

constexpr PixelType PIXELTYPE_INDEX8 = SDL_PIXELTYPE_INDEX8;

constexpr PixelType PIXELTYPE_PACKED8 = SDL_PIXELTYPE_PACKED8;

constexpr PixelType PIXELTYPE_PACKED16 = SDL_PIXELTYPE_PACKED16;

constexpr PixelType PIXELTYPE_PACKED32 = SDL_PIXELTYPE_PACKED32;

constexpr PixelType PIXELTYPE_ARRAYU8 = SDL_PIXELTYPE_ARRAYU8;

constexpr PixelType PIXELTYPE_ARRAYU16 = SDL_PIXELTYPE_ARRAYU16;

constexpr PixelType PIXELTYPE_ARRAYU32 = SDL_PIXELTYPE_ARRAYU32;

constexpr PixelType PIXELTYPE_ARRAYF16 = SDL_PIXELTYPE_ARRAYF16;

constexpr PixelType PIXELTYPE_ARRAYF32 = SDL_PIXELTYPE_ARRAYF32;

constexpr PixelType PIXELTYPE_INDEX2 = SDL_PIXELTYPE_INDEX2;

/// @}

/**
 * @name BitmapOrders
 * @{
 */

/**
 * Bitmap pixel order, high bit -> low bit.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using BitmapOrder = SDL_BitmapOrder;

constexpr BitmapOrder BITMAPORDER_NONE = SDL_BITMAPORDER_NONE;

constexpr BitmapOrder BITMAPORDER_4321 = SDL_BITMAPORDER_4321;

constexpr BitmapOrder BITMAPORDER_1234 = SDL_BITMAPORDER_1234;

/// @}

/**
 * @name PackedOrders
 * @{
 */

/**
 * Packed component order, high bit -> low bit.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PackedOrder = SDL_PackedOrder;

constexpr PackedOrder PACKEDORDER_NONE = SDL_PACKEDORDER_NONE;

constexpr PackedOrder PACKEDORDER_XRGB = SDL_PACKEDORDER_XRGB;

constexpr PackedOrder PACKEDORDER_RGBX = SDL_PACKEDORDER_RGBX;

constexpr PackedOrder PACKEDORDER_ARGB = SDL_PACKEDORDER_ARGB;

constexpr PackedOrder PACKEDORDER_RGBA = SDL_PACKEDORDER_RGBA;

constexpr PackedOrder PACKEDORDER_XBGR = SDL_PACKEDORDER_XBGR;

constexpr PackedOrder PACKEDORDER_BGRX = SDL_PACKEDORDER_BGRX;

constexpr PackedOrder PACKEDORDER_ABGR = SDL_PACKEDORDER_ABGR;

constexpr PackedOrder PACKEDORDER_BGRA = SDL_PACKEDORDER_BGRA;

/// @}

/**
 * @name ArrayOrders
 * @{
 */

/**
 * Array component order, low byte -> high byte.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ArrayOrder = SDL_ArrayOrder;

constexpr ArrayOrder ARRAYORDER_NONE = SDL_ARRAYORDER_NONE;

constexpr ArrayOrder ARRAYORDER_RGB = SDL_ARRAYORDER_RGB;

constexpr ArrayOrder ARRAYORDER_RGBA = SDL_ARRAYORDER_RGBA;

constexpr ArrayOrder ARRAYORDER_ARGB = SDL_ARRAYORDER_ARGB;

constexpr ArrayOrder ARRAYORDER_BGR = SDL_ARRAYORDER_BGR;

constexpr ArrayOrder ARRAYORDER_BGRA = SDL_ARRAYORDER_BGRA;

constexpr ArrayOrder ARRAYORDER_ABGR = SDL_ARRAYORDER_ABGR;

/// @}

/**
 * @name PackedLayouts
 * @{
 */

/**
 * Packed component layout.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PackedLayout = SDL_PackedLayout;

constexpr PackedLayout PACKEDLAYOUT_NONE = SDL_PACKEDLAYOUT_NONE;

constexpr PackedLayout PACKEDLAYOUT_332 = SDL_PACKEDLAYOUT_332;

constexpr PackedLayout PACKEDLAYOUT_4444 = SDL_PACKEDLAYOUT_4444;

constexpr PackedLayout PACKEDLAYOUT_1555 = SDL_PACKEDLAYOUT_1555;

constexpr PackedLayout PACKEDLAYOUT_5551 = SDL_PACKEDLAYOUT_5551;

constexpr PackedLayout PACKEDLAYOUT_565 = SDL_PACKEDLAYOUT_565;

constexpr PackedLayout PACKEDLAYOUT_8888 = SDL_PACKEDLAYOUT_8888;

constexpr PackedLayout PACKEDLAYOUT_2101010 = SDL_PACKEDLAYOUT_2101010;

constexpr PackedLayout PACKEDLAYOUT_1010102 = SDL_PACKEDLAYOUT_1010102;

/// @}

/**
 * Details about the format of a pixel.
 */
using PixelFormatDetails = SDL_PixelFormatDetails;

/**
 * Pixel format.
 *
 * SDL's pixel formats have the following naming convention:
 *
 * - Names with a list of components and a single bit count, such as RGB24 and
 *   ABGR32, define a platform-independent encoding into bytes in the order
 *   specified. For example, in RGB24 data, each pixel is encoded in 3 bytes
 *   (red, green, blue) in that order, and in ABGR32 data, each pixel is
 *   encoded in 4 bytes alpha, blue, green, red) in that order. Use these
 *   names if the property of a format that is important to you is the order
 *   of the bytes in memory or on disk.
 * - Names with a bit count per component, such as ARGB8888 and XRGB1555, are
 *   "packed" into an appropriately-sized integer in the platform's native
 *   endianness. For example, ARGB8888 is a sequence of 32-bit integers; in
 *   each integer, the most significant bits are alpha, and the least
 *   significant bits are blue. On a little-endian CPU such as x86, the least
 *   significant bits of each integer are arranged first in memory, but on a
 *   big-endian CPU such as s390x, the most significant bits are arranged
 *   first. Use these names if the property of a format that is important to
 *   you is the meaning of each bit position within a native-endianness
 *   integer.
 * - In indexed formats such as INDEX4LSB, each pixel is represented by
 *   encoding an index into the palette into the indicated number of bits,
 *   with multiple pixels packed into each byte if appropriate. In LSB
 *   formats, the first (leftmost) pixel is stored in the least-significant
 *   bits of the byte; in MSB formats, it's stored in the most-significant
 *   bits. INDEX8 does not need LSB/MSB variants, because each pixel exactly
 *   fills one byte.
 *
 * The 32-bit byte-array encodings such as RGBA32 are aliases for the
 * appropriate 8888 encoding for the current platform. For example, RGBA32 is
 * an alias for ABGR8888 on little-endian CPUs like x86, or an alias for
 * RGBA8888 on big-endian CPUs.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @ingroup wrap-state
 *
 * @sa wrap-state
 * @sa PixelFormats
 */
struct PixelFormat
{
  SDL_PixelFormat format;

  constexpr PixelFormat(SDL_PixelFormat format = SDL_PIXELFORMAT_UNKNOWN)
    : format(format)
  {
  }

  /**
   * Defining custom non-FourCC pixel formats.
   *
   * For example, defining SDL_PIXELFORMAT_RGBA8888 looks like this:
   *
   * ```c
   * PixelFormat format(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_RGBA,
   *   SDL_PACKEDLAYOUT_8888, 32, 4);
   * ```
   *
   * @param type the type of the new format, probably a SDL_PixelType value.
   * @param order the order of the new format, probably a SDL_BitmapOrder,
   *              SDL_PackedOrder, or SDL_ArrayOrder value.
   * @param layout the layout of the new format, probably an SDL_PackedLayout
   *               value or zero.
   * @param bits the number of bits per pixel of the new format.
   * @param bytes the number of bytes per pixel of the new format.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr PixelFormat(SDL_PixelType type,
                        int order,
                        SDL_PackedLayout layout,
                        int bits,
                        int bytes)
    : format(SDL_PixelFormat(
        SDL_DEFINE_PIXELFORMAT(type, order, layout, bits, bytes)))
  {
  }

  constexpr operator bool() const { return format != SDL_PIXELFORMAT_UNKNOWN; }

  constexpr operator SDL_PixelFormat() const { return format; }

  /**
   * Retrieve the type.
   *
   * @returns the type as PixelType.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr PixelType GetType() const
  {
    return PixelType(SDL_PIXELTYPE(format));
  }

  /**
   * Retrieve the order.
   *
   * This is usually a value from the BitmapOrder, PackedOrder, or ArrayOrder
   * enumerations, depending on the format type.
   *
   * @returns the order.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr int GetOrder() const { return SDL_PIXELORDER(format); }

  /**
   * Retrieve the layout.
   *
   * This is usually a value from the SDL_PackedLayout enumeration, or zero if a
   * layout doesn't make sense for the format type.
   *
   * @returns the layout
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr PackedLayout GetLayout() const
  {
    return PackedLayout(SDL_PIXELLAYOUT(format));
  }

  /**
   * Determine this's bits per pixel.
   *
   * FourCC formats will report zero here, as it rarely makes sense to measure
   * them per-pixel.
   *
   * @returns the bits-per-pixel.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @sa PixelFormat.GetBytesPerPixel
   */
  constexpr int GetBitsPerPixel() const { return SDL_BITSPERPIXEL(format); }

  /**
   * Determine this's bytes per pixel.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * FourCC formats do their best here, but many of them don't have a meaningful
   * measurement of bytes per pixel.
   *
   * @return the bytes-per-pixel.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @sa PixelFormat.GetBitsPerPixel
   */
  constexpr int GetBytesPerPixel() const { return SDL_BYTESPERPIXEL(format); }

  /**
   * Determine if this is an indexed format.
   *
   * @returns true if the format is indexed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsIndexed() const { return SDL_ISPIXELFORMAT_INDEXED(format); }

  /**
   * Determine if this is a packed format.
   *
   * @returns true if the format is packed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsPacked() const { return SDL_ISPIXELFORMAT_PACKED(format); }

  /**
   * Determine if this is an array format.
   *
   * @returns true if the format is an array, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsArray() const { return SDL_ISPIXELFORMAT_ARRAY(format); }

  /**
   * Determine if this is a 10-bit format.
   *
   * @returns true if the format is 10-bit, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool Is10Bit() const { return SDL_ISPIXELFORMAT_10BIT(format); }

  /**
   * Determine if this is a floating point format.
   *
   * @returns true if the format is 10-bit, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsFloat() const { return SDL_ISPIXELFORMAT_FLOAT(format); }

  /**
   * Determine if this has an alpha channel.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsAlpha() const { return SDL_ISPIXELFORMAT_ALPHA(format); }

  /**
   * Determine if this is a "FourCC" format.
   *
   * This covers custom and other unusual formats.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsFourCC() const { return SDL_ISPIXELFORMAT_FOURCC(format); }

  /**
   * Get the human readable name of a pixel format.
   *
   * @returns the human readable name of the specified pixel format or
   *          "SDL_PIXELFORMAT_UNKNOWN" if the format isn't recognized.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  inline const char* GetName() const { return SDL_GetPixelFormatName(format); }

  /**
   * Convert one of the enumerated pixel formats to a bpp value and RGBA masks.
   *
   * @param bpp a bits per pixel value; usually 15, 16, or 32.
   * @param Rmask a pointer filled in with the red mask for the format.
   * @param Gmask a pointer filled in with the green mask for the format.
   * @param Bmask a pointer filled in with the blue mask for the format.
   * @param Amask a pointer filled in with the alpha mask for the format.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ForMasks()
   */
  inline bool GetMasks(int* bpp,
                       Uint32* Rmask,
                       Uint32* Gmask,
                       Uint32* Bmask,
                       Uint32* Amask) const
  {
    return SDL_GetMasksForPixelFormat(format, bpp, Rmask, Gmask, Bmask, Amask);
  }

  /**
   * Convert a bpp value and RGBA masks to an enumerated pixel format.
   *
   * This will return `SDL_PIXELFORMAT_UNKNOWN` if the conversion wasn't
   * possible.
   *
   * @param bpp a bits per pixel value; usually 15, 16, or 32.
   * @param Rmask the red mask for the format.
   * @param Gmask the green mask for the format.
   * @param Bmask the blue mask for the format.
   * @param Amask the alpha mask for the format.
   * @returns the SDL_PixelFormat value corresponding to the format masks, or
   *          SDL_PIXELFORMAT_UNKNOWN if there isn't a match.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMasks()
   */
  static inline PixelFormat ForMasks(int bpp,
                                     Uint32 Rmask,
                                     Uint32 Gmask,
                                     Uint32 Bmask,
                                     Uint32 Amask)
  {
    return {SDL_GetPixelFormatForMasks(bpp, Rmask, Gmask, Bmask, Amask)};
  }

  /**
   * Create an SDL_PixelFormatDetails structure corresponding to a pixel format.
   *
   * Returned structure may come from a shared global cache (i.e. not newly
   * allocated), and hence should not be modified, especially the palette. Weird
   * errors such as `Blit combination not supported` may occur.
   *
   * @returns a pointer to a SDL_PixelFormatDetails structure or NULL on
   *          failure; call SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  inline const PixelFormatDetails* GetDetails() const
  {
    return SDL_GetPixelFormatDetails(format);
  }

  /**
   * Map an RGBA quadruple to a pixel value for a given pixel format.
   *
   * This function maps the RGBA color value to the specified pixel format and
   * returns the pixel value best approximating the given RGBA color value for
   * the given pixel format.
   *
   * If the specified pixel format has no alpha component the alpha value will
   * be ignored (as it will be in formats with a palette).
   *
   * If the format has a palette (8-bit) the index of the closest matching color
   * in the palette will be returned.
   *
   * If the pixel format bpp (color depth) is less than 32-bpp then the unused
   * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
   * format the return value can be assigned to a Uint16, and similarly a Uint8
   * for an 8-bpp format).
   *
   * @param color the color components of the pixel in the range 0-255.
   * @param palette an optional palette for indexed formats, may be NULL.
   * @returns a pixel value.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetPixelFormatDetails()
   * @sa Get()
   * @sa MapRGBA()
   * @sa Surface.MapColor()
   */
  inline Uint32 Map(Color color, PaletteRef palette) const;

  /**
   * Get RGBA values from a pixel in the specified format.
   *
   * This function uses the entire 8-bit [0..255] range when converting color
   * components from pixel formats with less than 8-bits per RGB component
   * (e.g., a completely white pixel in 16-bit RGB565 format would return [0xff,
   * 0xff, 0xff] not [0xf8, 0xfc, 0xf8]).
   *
   * If the surface has no alpha component, the alpha will be returned as 0xff
   * (100% opaque).
   *
   * @param pixel a pixel value.
   * @param palette an optional palette for indexed formats, may be NULL.
   * @returns a color value.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetPixelFormatDetails()
   * @sa GetRGBA()
   * @sa Map()
   */
  inline Color Get(Uint32 pixel, PaletteRef palette) const;
};

/**
 * @defgroup PixelFormats Pixel Formats
 *
 * Pixel formats
 * @{
 */
constexpr PixelFormat PIXELFORMAT_UNKNOWN = SDL_PIXELFORMAT_UNKNOWN;

constexpr PixelFormat PIXELFORMAT_INDEX1LSB = SDL_PIXELFORMAT_INDEX1LSB;

constexpr PixelFormat PIXELFORMAT_INDEX1MSB = SDL_PIXELFORMAT_INDEX1MSB;

constexpr PixelFormat PIXELFORMAT_INDEX2LSB = SDL_PIXELFORMAT_INDEX2LSB;

constexpr PixelFormat PIXELFORMAT_INDEX2MSB = SDL_PIXELFORMAT_INDEX2MSB;

constexpr PixelFormat PIXELFORMAT_INDEX4LSB = SDL_PIXELFORMAT_INDEX4LSB;

constexpr PixelFormat PIXELFORMAT_INDEX4MSB = SDL_PIXELFORMAT_INDEX4MSB;

constexpr PixelFormat PIXELFORMAT_INDEX8 = SDL_PIXELFORMAT_INDEX8;

constexpr PixelFormat PIXELFORMAT_RGB332 = SDL_PIXELFORMAT_RGB332;

constexpr PixelFormat PIXELFORMAT_XRGB4444 = SDL_PIXELFORMAT_XRGB4444;

constexpr PixelFormat PIXELFORMAT_XBGR4444 = SDL_PIXELFORMAT_XBGR4444;

constexpr PixelFormat PIXELFORMAT_XRGB1555 = SDL_PIXELFORMAT_XRGB1555;

constexpr PixelFormat PIXELFORMAT_XBGR1555 = SDL_PIXELFORMAT_XBGR1555;

constexpr PixelFormat PIXELFORMAT_ARGB4444 = SDL_PIXELFORMAT_ARGB4444;

constexpr PixelFormat PIXELFORMAT_RGBA4444 = SDL_PIXELFORMAT_RGBA4444;

constexpr PixelFormat PIXELFORMAT_ABGR4444 = SDL_PIXELFORMAT_ABGR4444;

constexpr PixelFormat PIXELFORMAT_BGRA4444 = SDL_PIXELFORMAT_BGRA4444;

constexpr PixelFormat PIXELFORMAT_ARGB1555 = SDL_PIXELFORMAT_ARGB1555;

constexpr PixelFormat PIXELFORMAT_RGBA5551 = SDL_PIXELFORMAT_RGBA5551;

constexpr PixelFormat PIXELFORMAT_ABGR1555 = SDL_PIXELFORMAT_ABGR1555;

constexpr PixelFormat PIXELFORMAT_BGRA5551 = SDL_PIXELFORMAT_BGRA5551;

constexpr PixelFormat PIXELFORMAT_RGB565 = SDL_PIXELFORMAT_RGB565;

constexpr PixelFormat PIXELFORMAT_BGR565 = SDL_PIXELFORMAT_BGR565;

constexpr PixelFormat PIXELFORMAT_RGB24 = SDL_PIXELFORMAT_RGB24;

constexpr PixelFormat PIXELFORMAT_BGR24 = SDL_PIXELFORMAT_BGR24;

constexpr PixelFormat PIXELFORMAT_XRGB8888 = SDL_PIXELFORMAT_XRGB8888;

constexpr PixelFormat PIXELFORMAT_RGBX8888 = SDL_PIXELFORMAT_RGBX8888;

constexpr PixelFormat PIXELFORMAT_XBGR8888 = SDL_PIXELFORMAT_XBGR8888;

constexpr PixelFormat PIXELFORMAT_BGRX8888 = SDL_PIXELFORMAT_BGRX8888;

constexpr PixelFormat PIXELFORMAT_ARGB8888 = SDL_PIXELFORMAT_ARGB8888;

constexpr PixelFormat PIXELFORMAT_RGBA8888 = SDL_PIXELFORMAT_RGBA8888;

constexpr PixelFormat PIXELFORMAT_ABGR8888 = SDL_PIXELFORMAT_ABGR8888;

constexpr PixelFormat PIXELFORMAT_BGRA8888 = SDL_PIXELFORMAT_BGRA8888;

constexpr PixelFormat PIXELFORMAT_XRGB2101010 = SDL_PIXELFORMAT_XRGB2101010;

constexpr PixelFormat PIXELFORMAT_XBGR2101010 = SDL_PIXELFORMAT_XBGR2101010;

constexpr PixelFormat PIXELFORMAT_ARGB2101010 = SDL_PIXELFORMAT_ARGB2101010;

constexpr PixelFormat PIXELFORMAT_ABGR2101010 = SDL_PIXELFORMAT_ABGR2101010;

constexpr PixelFormat PIXELFORMAT_RGB48 = SDL_PIXELFORMAT_RGB48;

constexpr PixelFormat PIXELFORMAT_BGR48 = SDL_PIXELFORMAT_BGR48;

constexpr PixelFormat PIXELFORMAT_RGBA64 = SDL_PIXELFORMAT_RGBA64;

constexpr PixelFormat PIXELFORMAT_ARGB64 = SDL_PIXELFORMAT_ARGB64;

constexpr PixelFormat PIXELFORMAT_BGRA64 = SDL_PIXELFORMAT_BGRA64;

constexpr PixelFormat PIXELFORMAT_ABGR64 = SDL_PIXELFORMAT_ABGR64;

constexpr PixelFormat PIXELFORMAT_RGB48_FLOAT = SDL_PIXELFORMAT_RGB48_FLOAT;

constexpr PixelFormat PIXELFORMAT_BGR48_FLOAT = SDL_PIXELFORMAT_BGR48_FLOAT;

constexpr PixelFormat PIXELFORMAT_RGBA64_FLOAT = SDL_PIXELFORMAT_RGBA64_FLOAT;

constexpr PixelFormat PIXELFORMAT_ARGB64_FLOAT = SDL_PIXELFORMAT_ARGB64_FLOAT;

constexpr PixelFormat PIXELFORMAT_BGRA64_FLOAT = SDL_PIXELFORMAT_BGRA64_FLOAT;

constexpr PixelFormat PIXELFORMAT_ABGR64_FLOAT = SDL_PIXELFORMAT_ABGR64_FLOAT;

constexpr PixelFormat PIXELFORMAT_RGB96_FLOAT = SDL_PIXELFORMAT_RGB96_FLOAT;

constexpr PixelFormat PIXELFORMAT_BGR96_FLOAT = SDL_PIXELFORMAT_BGR96_FLOAT;

constexpr PixelFormat PIXELFORMAT_RGBA128_FLOAT = SDL_PIXELFORMAT_RGBA128_FLOAT;

constexpr PixelFormat PIXELFORMAT_ARGB128_FLOAT = SDL_PIXELFORMAT_ARGB128_FLOAT;

constexpr PixelFormat PIXELFORMAT_BGRA128_FLOAT = SDL_PIXELFORMAT_BGRA128_FLOAT;

constexpr PixelFormat PIXELFORMAT_ABGR128_FLOAT = SDL_PIXELFORMAT_ABGR128_FLOAT;

/**
 * Planar mode: Y + V + U  (3 planes)
 */
constexpr PixelFormat PIXELFORMAT_YV12 = SDL_PIXELFORMAT_YV12;

/**
 * Planar mode: Y + U + V  (3 planes)
 */
constexpr PixelFormat PIXELFORMAT_IYUV = SDL_PIXELFORMAT_IYUV;

/**
 * Packed mode: Y0+U0+Y1+V0 (1 plane)
 */
constexpr PixelFormat PIXELFORMAT_YUY2 = SDL_PIXELFORMAT_YUY2;

/**
 * Packed mode: U0+Y0+V0+Y1 (1 plane)
 */
constexpr PixelFormat PIXELFORMAT_UYVY = SDL_PIXELFORMAT_UYVY;

/**
 * Packed mode: Y0+V0+Y1+U0 (1 plane)
 */
constexpr PixelFormat PIXELFORMAT_YVYU = SDL_PIXELFORMAT_YVYU;

/**
 * Planar mode: Y + U/V interleaved  (2 planes)
 */
constexpr PixelFormat PIXELFORMAT_NV12 = SDL_PIXELFORMAT_NV12;

/**
 * Planar mode: Y + V/U interleaved  (2 planes)
 */
constexpr PixelFormat PIXELFORMAT_NV21 = SDL_PIXELFORMAT_NV21;

/**
 * Planar mode: Y + U/V interleaved  (2 planes)
 */
constexpr PixelFormat PIXELFORMAT_P010 = SDL_PIXELFORMAT_P010;

/**
 * Android video texture format
 */
constexpr PixelFormat PIXELFORMAT_EXTERNAL_OES = SDL_PIXELFORMAT_EXTERNAL_OES;

#if SDL_VERSION_ATLEAST(3, 2, 9)
/**
 * Motion JPEG
 * @since SDL 3.2.10
 */
constexpr PixelFormat PIXELFORMAT_MJPG = SDL_PIXELFORMAT_MJPG;

#endif

constexpr PixelFormat PIXELFORMAT_RGBA32 = SDL_PIXELFORMAT_RGBA32;

constexpr PixelFormat PIXELFORMAT_ARGB32 = SDL_PIXELFORMAT_ARGB32;

constexpr PixelFormat PIXELFORMAT_BGRA32 = SDL_PIXELFORMAT_BGRA32;

constexpr PixelFormat PIXELFORMAT_ABGR32 = SDL_PIXELFORMAT_ABGR32;

constexpr PixelFormat PIXELFORMAT_RGBX32 = SDL_PIXELFORMAT_RGBX32;

constexpr PixelFormat PIXELFORMAT_XRGB32 = SDL_PIXELFORMAT_XRGB32;

constexpr PixelFormat PIXELFORMAT_BGRX32 = SDL_PIXELFORMAT_BGRX32;

constexpr PixelFormat PIXELFORMAT_XBGR32 = SDL_PIXELFORMAT_XBGR32;

/// @}

/**
 * @name Colorspaces
 * @{
 */

/**
 * Colorspace color type.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ColorType = SDL_ColorType;

constexpr ColorType COLOR_TYPE_UNKNOWN = SDL_COLOR_TYPE_UNKNOWN;

constexpr ColorType COLOR_TYPE_RGB = SDL_COLOR_TYPE_RGB;

constexpr ColorType COLOR_TYPE_YCBCR = SDL_COLOR_TYPE_YCBCR;

/// @}

/**
 * @name ColorRanges
 * @{
 */

/**
 * Colorspace color range, as described by
 * https://www.itu.int/rec/R-REC-BT.2100-2-201807-I/en
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ColorRange = SDL_ColorRange;

constexpr ColorRange COLOR_RANGE_UNKNOWN = SDL_COLOR_RANGE_UNKNOWN;

/**
 * Narrow range, e.g. 16-235 for 8-bit RGB and luma, and 16-240 for 8-bit chroma
 */
constexpr ColorRange COLOR_RANGE_LIMITED = SDL_COLOR_RANGE_LIMITED;

/**
 * Full range, e.g. 0-255 for 8-bit RGB and luma, and 1-255 for 8-bit chroma
 */
constexpr ColorRange COLOR_RANGE_FULL = SDL_COLOR_RANGE_FULL;

/// @}

/**
 * @name ColorPrimaries
 * @{
 */

/**
 * Colorspace color primaries, as described by
 * https://www.itu.int/rec/T-REC-H.273-201612-S/en
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ColorPrimaries = SDL_ColorPrimaries;

constexpr ColorPrimaries COLOR_PRIMARIES_UNKNOWN = SDL_COLOR_PRIMARIES_UNKNOWN;

/**
 * ITU-R BT.709-6
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT709 = SDL_COLOR_PRIMARIES_BT709;

constexpr ColorPrimaries COLOR_PRIMARIES_UNSPECIFIED =
  SDL_COLOR_PRIMARIES_UNSPECIFIED;

/**
 * ITU-R BT.470-6 System M
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT470M = SDL_COLOR_PRIMARIES_BT470M;

/**
 * ITU-R BT.470-6 System B, G / ITU-R BT.601-7 625
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT470BG = SDL_COLOR_PRIMARIES_BT470BG;

/**
 * ITU-R BT.601-7 525, SMPTE 170M
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT601 = SDL_COLOR_PRIMARIES_BT601;

/**
 * SMPTE 240M, functionally the same as SDL_COLOR_PRIMARIES_BT601
 */
constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE240 =
  SDL_COLOR_PRIMARIES_SMPTE240;

/**
 * Generic film (color filters using Illuminant C)
 */
constexpr ColorPrimaries COLOR_PRIMARIES_GENERIC_FILM =
  SDL_COLOR_PRIMARIES_GENERIC_FILM;

/**
 * ITU-R BT.2020-2 / ITU-R BT.2100-0
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT2020 = SDL_COLOR_PRIMARIES_BT2020;

/**
 * SMPTE ST 428-1
 */
constexpr ColorPrimaries COLOR_PRIMARIES_XYZ = SDL_COLOR_PRIMARIES_XYZ;

/**
 * SMPTE RP 431-2
 */
constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE431 =
  SDL_COLOR_PRIMARIES_SMPTE431;

/**
 * SMPTE EG 432-1 / DCI P3
 */
constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE432 =
  SDL_COLOR_PRIMARIES_SMPTE432;

/**
 * EBU Tech. 3213-E
 */
constexpr ColorPrimaries COLOR_PRIMARIES_EBU3213 = SDL_COLOR_PRIMARIES_EBU3213;

constexpr ColorPrimaries COLOR_PRIMARIES_CUSTOM = SDL_COLOR_PRIMARIES_CUSTOM;

/// @}

/**
 * @name TransferCharacteristics
 * @{
 */

/**
 * Colorspace transfer characteristics.
 *
 * These are as described by https://www.itu.int/rec/T-REC-H.273-201612-S/en
 *
 * @since This enum is available since SDL 3.2.0.
 */
using TransferCharacteristics = SDL_TransferCharacteristics;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_UNKNOWN =
  SDL_TRANSFER_CHARACTERISTICS_UNKNOWN;

/**
 * Rec. ITU-R BT.709-6 / ITU-R BT1361
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT709 =
  SDL_TRANSFER_CHARACTERISTICS_BT709;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_UNSPECIFIED =
  SDL_TRANSFER_CHARACTERISTICS_UNSPECIFIED;

/**
 * ITU-R BT.470-6 System M / ITU-R BT1700 625 PAL & SECAM
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA22 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA22;

/**
 * ITU-R BT.470-6 System B, G
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA28 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA28;

/**
 * SMPTE ST 170M / ITU-R BT.601-7 525 or 625
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT601 =
  SDL_TRANSFER_CHARACTERISTICS_BT601;

/**
 * SMPTE ST 240M
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE240 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE240;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LINEAR =
  SDL_TRANSFER_CHARACTERISTICS_LINEAR;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100_SQRT10 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100_SQRT10;

/**
 * IEC 61966-2-4
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_IEC61966 =
  SDL_TRANSFER_CHARACTERISTICS_IEC61966;

/**
 * ITU-R BT1361 Extended Colour Gamut
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT1361 =
  SDL_TRANSFER_CHARACTERISTICS_BT1361;

/**
 * IEC 61966-2-1 (sRGB or sYCC)
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SRGB =
  SDL_TRANSFER_CHARACTERISTICS_SRGB;

/**
 * ITU-R BT2020 for 10-bit system
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_10BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_10BIT;

/**
 * ITU-R BT2020 for 12-bit system
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_12BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_12BIT;

/**
 * SMPTE ST 2084 for 10-, 12-, 14- and 16-bit systems
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_PQ =
  SDL_TRANSFER_CHARACTERISTICS_PQ;

/**
 * SMPTE ST 428-1
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE428 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE428;

/**
 * ARIB STD-B67, known as "hybrid log-gamma" (HLG)
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_HLG =
  SDL_TRANSFER_CHARACTERISTICS_HLG;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_CUSTOM =
  SDL_TRANSFER_CHARACTERISTICS_CUSTOM;

/// @}

/**
 * @name MatrixCoefficients
 * @{
 */

/**
 * Colorspace matrix coefficients.
 *
 * These are as described by https://www.itu.int/rec/T-REC-H.273-201612-S/en
 *
 * @since This enum is available since SDL 3.2.0.
 */
using MatrixCoefficients = SDL_MatrixCoefficients;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_IDENTITY =
  SDL_MATRIX_COEFFICIENTS_IDENTITY;

/**
 * ITU-R BT.709-6
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT709 =
  SDL_MATRIX_COEFFICIENTS_BT709;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_UNSPECIFIED =
  SDL_MATRIX_COEFFICIENTS_UNSPECIFIED;

/**
 * US FCC Title 47
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_FCC =
  SDL_MATRIX_COEFFICIENTS_FCC;

/**
 * ITU-R BT.470-6 System B, G / ITU-R BT.601-7 625, functionally the same as
 * SDL_MATRIX_COEFFICIENTS_BT601
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT470BG =
  SDL_MATRIX_COEFFICIENTS_BT470BG;

/**
 * ITU-R BT.601-7 525
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT601 =
  SDL_MATRIX_COEFFICIENTS_BT601;

/**
 * SMPTE 240M
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE240 =
  SDL_MATRIX_COEFFICIENTS_SMPTE240;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_YCGCO =
  SDL_MATRIX_COEFFICIENTS_YCGCO;

/**
 * ITU-R BT.2020-2 non-constant luminance
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_NCL =
  SDL_MATRIX_COEFFICIENTS_BT2020_NCL;

/**
 * ITU-R BT.2020-2 constant luminance
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_CL =
  SDL_MATRIX_COEFFICIENTS_BT2020_CL;

/**
 * SMPTE ST 2085
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE2085 =
  SDL_MATRIX_COEFFICIENTS_SMPTE2085;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL;

/**
 * ITU-R BT.2100-0 ICTCP
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_ICTCP =
  SDL_MATRIX_COEFFICIENTS_ICTCP;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CUSTOM =
  SDL_MATRIX_COEFFICIENTS_CUSTOM;

/// @}

/**
 * @name ChromaLocations
 * @{
 */

/**
 * Colorspace chroma sample location.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ChromaLocation = SDL_ChromaLocation;

/**
 * RGB, no chroma sampling
 */
constexpr ChromaLocation CHROMA_LOCATION_NONE = SDL_CHROMA_LOCATION_NONE;

/**
 * In MPEG-2, MPEG-4, and AVC, Cb and Cr are taken on midpoint of the left-edge
 * of the 2x2 square. In other words, they have the same horizontal location as
 * the top-left pixel, but is shifted one-half pixel down vertically.
 */
constexpr ChromaLocation CHROMA_LOCATION_LEFT = SDL_CHROMA_LOCATION_LEFT;

/**
 * In JPEG/JFIF, H.261, and MPEG-1, Cb and Cr are taken at the center of the 2x2
 * square. In other words, they are offset one-half pixel to the right and
 * one-half pixel down compared to the top-left pixel.
 */
constexpr ChromaLocation CHROMA_LOCATION_CENTER = SDL_CHROMA_LOCATION_CENTER;

/**
 * In HEVC for BT.2020 and BT.2100 content (in particular on Blu-rays), Cb and
 * Cr are sampled at the same location as the group's top-left Y pixel
 * ("co-sited", "co-located").
 */
constexpr ChromaLocation CHROMA_LOCATION_TOPLEFT = SDL_CHROMA_LOCATION_TOPLEFT;

/// @}

/**
 * Colorspace definitions.
 *
 * Since similar colorspaces may vary in their details (matrix, transfer
 * function, etc.), this is not an exhaustive list, but rather a
 * representative sample of the kinds of colorspaces supported in SDL.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @ingroup wrap-state
 *
 * @sa wrap-state
 *
 * @sa Colorspaces
 * @sa ColorPrimaries
 * @sa ColorRange
 * @sa ColorType
 * @sa MatrixCoefficients
 * @sa TransferCharacteristics
 */
class Colorspace
{
  SDL_Colorspace colorspace;

public:
  /**
   * Wrap a SDL_Colorspace
   *
   */
  constexpr Colorspace(SDL_Colorspace colorspace = SDL_COLORSPACE_UNKNOWN)
    : colorspace(colorspace)
  {
  }

  /**
   * Define custom Colorspace formats.
   *
   * For example, defining SDL_COLORSPACE_SRGB looks like this:
   *
   * ```cpp
   * Colorspace colorspace(SDL_COLOR_TYPE_RGB,
   *                       SDL_COLOR_RANGE_FULL,
   *                       SDL_COLOR_PRIMARIES_BT709,
   *                       SDL_TRANSFER_CHARACTERISTICS_SRGB,
   *                       SDL_MATRIX_COEFFICIENTS_IDENTITY,
   *                       SDL_CHROMA_LOCATION_NONE)
   * ```
   *
   * @param type the type of the new format, probably an ColorType value.
   * @param range the range of the new format, probably a ColorRange value.
   * @param primaries the primaries of the new format, probably an
   *                  ColorPrimaries value.
   * @param transfer the transfer characteristics of the new format, probably an
   *                 TransferCharacteristics value.
   * @param matrix the matrix coefficients of the new format, probably an
   *               MatrixCoefficients value.
   * @param chroma the chroma sample location of the new format, probably an
   *               ChromaLocation value.
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr Colorspace(ColorType type,
                       ColorRange range,
                       ColorPrimaries primaries,
                       TransferCharacteristics transfer,
                       MatrixCoefficients matrix,
                       ChromaLocation chroma)
    : colorspace(SDL_Colorspace(SDL_DEFINE_COLORSPACE(type,
                                                      range,
                                                      primaries,
                                                      transfer,
                                                      matrix,
                                                      chroma)))
  {
  }

  constexpr operator bool() const
  {
    return colorspace != SDL_COLORSPACE_UNKNOWN;
  }

  constexpr operator SDL_Colorspace() const { return colorspace; }

  /**
   * A macro to retrieve the type of a Colorspace.
   *
   * @returns the ColorType for `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr ColorType GetType() const { return SDL_COLORSPACETYPE(colorspace); }

  /**
   * A macro to retrieve the range of a Colorspace.
   *
   * @returns the ColorRange of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr ColorRange GetRange() const
  {
    return SDL_COLORSPACERANGE(colorspace);
  }

  /**
   * A macro to retrieve the chroma sample location of a Colorspace.
   *
   * @returns the ChromaLocation of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr ChromaLocation GetChroma() const
  {
    return SDL_COLORSPACECHROMA(colorspace);
  }

  /**
   * A macro to retrieve the primaries of a Colorspace.
   *
   * @returns the ColorPrimaries of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr ColorPrimaries GetPrimaries() const
  {
    return SDL_COLORSPACEPRIMARIES(colorspace);
  }

  /**
   * A macro to retrieve the transfer characteristics of a Colorspace.
   *
   * @returns the TransferCharacteristics of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr TransferCharacteristics GetTransfer() const
  {
    return SDL_COLORSPACETRANSFER(colorspace);
  }

  /**
   * A macro to retrieve the matrix coefficients of a Colorspace.
   *
   * @returns the MatrixCoefficients of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr MatrixCoefficients GetMatrix() const
  {
    return SDL_COLORSPACEMATRIX(colorspace);
  }

  /**
   * A macro to determine if a Colorspace uses BT601 (or BT470BG) matrix
   * coefficients.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if BT601 or BT470BG, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT601() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT601(colorspace);
  }

  /**
   * A macro to determine if a Colorspace uses BT709 matrix coefficients.
   *
   * @returns true if BT709, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT709() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT709(colorspace);
  }

  /**
   * A macro to determine if a Colorspace uses BT2020_NCL matrix
   * coefficients.
   *
   * @returns true if BT2020_NCL, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT2020_NCL() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT2020_NCL(colorspace);
  }

  /**
   * A macro to determine if a Colorspace has a limited range.
   *
   * @returns true if limited range, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsLimitedRange() const
  {
    return SDL_ISCOLORSPACE_LIMITED_RANGE(colorspace);
  }

  /**
   * A macro to determine if a Colorspace has a full range.
   *
   * @returns true if full range, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsFullRange() const
  {
    return SDL_ISCOLORSPACE_FULL_RANGE(colorspace);
  }
};

/**
 * @defgroup Colorspaces Colorspaces
 * @{
 */

constexpr Colorspace COLORSPACE_UNKNOWN = SDL_COLORSPACE_UNKNOWN;

/**
 * Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709
 */
constexpr Colorspace COLORSPACE_SRGB = SDL_COLORSPACE_SRGB;

/**
 * Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709
 */
constexpr Colorspace COLORSPACE_SRGB_LINEAR = SDL_COLORSPACE_SRGB_LINEAR;

/**
 * Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020
 */
constexpr Colorspace COLORSPACE_HDR10 = SDL_COLORSPACE_HDR10;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_NONE_P709_X601
 */
constexpr Colorspace COLORSPACE_JPEG = SDL_COLORSPACE_JPEG;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601
 */
constexpr Colorspace COLORSPACE_BT601_LIMITED = SDL_COLORSPACE_BT601_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601
 */
constexpr Colorspace COLORSPACE_BT601_FULL = SDL_COLORSPACE_BT601_FULL;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709
 */
constexpr Colorspace COLORSPACE_BT709_LIMITED = SDL_COLORSPACE_BT709_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709
 */
constexpr Colorspace COLORSPACE_BT709_FULL = SDL_COLORSPACE_BT709_FULL;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P2020
 */
constexpr Colorspace COLORSPACE_BT2020_LIMITED = SDL_COLORSPACE_BT2020_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P2020
 */
constexpr Colorspace COLORSPACE_BT2020_FULL = SDL_COLORSPACE_BT2020_FULL;

/**
 * The default colorspace for RGB surfaces if no colorspace is specified
 */
constexpr Colorspace COLORSPACE_RGB_DEFAULT = SDL_COLORSPACE_RGB_DEFAULT;

/**
 * The default colorspace for YUV surfaces if no colorspace is specified
 */
constexpr Colorspace COLORSPACE_YUV_DEFAULT = SDL_COLORSPACE_YUV_DEFAULT;

/// @}

/**
 * A structure that represents a color as RGBA components.
 *
 * The bits of this structure can be directly reinterpreted as an
 * integer-packed color which uses the SDL_PIXELFORMAT_RGBA32 format
 * (SDL_PIXELFORMAT_ABGR8888 on little-endian systems and
 * SDL_PIXELFORMAT_RGBA8888 on big-endian systems).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @ingroup wrap-extending-struct
 *
 * @sa wrap-extending-struct
 */
struct Color : SDL_Color
{
  constexpr Color(SDL_Color color = {0})
    : SDL_Color(color)
  {
  }

  constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
    : SDL_Color{r, g, b, a}
  {
  }

  // Auto comparison operator
  auto operator<=>(const Color& other) const = default;

  /**
   * Get the red component from the color
   *
   * @returns The red component from the color
   *
   */
  constexpr Uint8 GetRed() const { return r; }

  /**
   * Set the red component from the color
   *
   * @param[in] nr New red component value
   *
   * @returns Reference to self
   *
   */
  constexpr Color& SetRed(Uint8 nr)
  {
    r = nr;
    return *this;
  }

  /**
   * Get the green component from the color
   *
   * @returns The green component from the color
   *
   */
  constexpr Uint8 GetGreen() const { return g; }

  /**
   * Set the green component from the color
   *
   * @param[in] ng New green component value
   *
   * @returns Reference to self
   *
   */
  constexpr Color& SetGreen(Uint8 ng)
  {
    g = ng;
    return *this;
  }

  /**
   * Get the blue component from the color
   *
   * @returns The blue component from the color
   *
   */
  constexpr Uint8 GetBlue() const { return b; }

  /**
   * Set the blue component from the color
   *
   * @param[in] nb New blue component value
   *
   * @returns Reference to self
   *
   */
  constexpr Color& SetBlue(Uint8 nb)
  {
    b = nb;
    return *this;
  }

  /**
   * Get the alpha component from the color
   *
   * @returns The alpha component from the color
   *
   */
  constexpr Uint8 GetAlpha() const { return a; }

  /**
   * Set the alpha component from the color
   *
   * @param[in] na New alpha component value
   *
   * @returns Reference to self
   *
   */
  constexpr Color& SetAlpha(Uint8 na)
  {
    a = na;
    return *this;
  }

  /**
   * Map an RGBA quadruple to a pixel value for a given pixel format.
   *
   * This function maps the RGBA color value to the specified pixel format and
   * returns the pixel value best approximating the given RGBA color value for
   * the given pixel format.
   *
   * If the specified pixel format has no alpha component the alpha value will
   * be ignored (as it will be in formats with a palette).
   *
   * If the format has a palette (8-bit) the index of the closest matching color
   * in the palette will be returned.
   *
   * If the pixel format bpp (color depth) is less than 32-bpp then the unused
   * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
   * format the return value can be assigned to a Uint16, and similarly a Uint8
   * for an 8-bpp format).
   *
   * @param format a pointer to PixelFormatDetails describing the pixel
   *               format.
   * @param palette an optional palette for indexed formats, may be NULL.
   * @returns a pixel value.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified.
   */
  Uint32 Map(const PixelFormatDetails* format, PaletteRef palette) const;

  /**
   * Get RGBA values from a pixel in the specified format.
   *
   * This function uses the entire 8-bit [0..255] range when converting color
   * components from pixel formats with less than 8-bits per RGB component
   * (e.g., a completely white pixel in 16-bit RGB565 format would return [0xff,
   * 0xff, 0xff] not [0xf8, 0xfc, 0xf8]).
   *
   * If the surface has no alpha component, the alpha will be returned as 0xff
   * (100% opaque).
   *
   * @param pixel a pixel value.
   * @param format a pointer to SDL_PixelFormatDetails describing the pixel
   *               format.
   * @param palette an optional palette for indexed formats, may be NULL.
   * @returns a color value.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PixelFormat.GetDetails()
   * @sa GetRGBA()
   * @sa Map()
   */
  static Color Get(Uint32 pixel,
                   const PixelFormatDetails* format,
                   PaletteRef palette);
};

/**
 * The bits of this structure can be directly reinterpreted as a float-packed
 * color which uses the SDL_PIXELFORMAT_RGBA128_FLOAT format
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @ingroup wrap-extending-struct
 *
 * @sa wrap-extending-struct
 */
struct FColor : SDL_FColor
{
  constexpr FColor(SDL_FColor color = {0})
    : SDL_FColor(color)
  {
  }

  constexpr FColor(float r, float g, float b, float a = 1)
    : SDL_FColor{r, g, b, a}
  {
  }

  // Auto comparison operator
  auto operator<=>(const FColor& other) const = default;

  /**
   * Get the red component from the color
   *
   * @returns The red component from the color
   *
   */
  constexpr float GetRed() const { return r; }

  /**
   * Set the red component from the color
   *
   * @param[in] nr New red component value
   *
   * @returns Reference to self
   *
   */
  constexpr FColor& SetRed(float nr)
  {
    r = nr;
    return *this;
  }

  /**
   * Get the green component from the color
   *
   * @returns The green component from the color
   *
   */
  constexpr float GetGreen() const { return g; }

  /**
   * Set the green component from the color
   *
   * @param[in] ng New green component value
   *
   * @returns Reference to self
   *
   */
  constexpr FColor& SetGreen(float ng)
  {
    g = ng;
    return *this;
  }

  /**
   * Get the blue component from the color
   *
   * @returns The blue component from the color
   *
   */
  constexpr float GetBlue() const { return b; }

  /**
   * Set the blue component from the color
   *
   * @param[in] nb New blue component value
   *
   * @returns Reference to self
   *
   */
  constexpr FColor& SetBlue(float nb)
  {
    b = nb;
    return *this;
  }

  /**
   * Get the alpha component from the color
   *
   * @returns The alpha component from the color
   *
   */
  constexpr float GetAlpha() const { return a; }

  /**
   * Set the alpha component from the color
   *
   * @param[in] na New alpha component value
   *
   * @returns Reference to self
   *
   */
  constexpr FColor& SetAlpha(float na)
  {
    a = na;
    return *this;
  }
};

/**
 * A set of indexed colors representing a palette.
 *
 * @ingroup resource
 *
 * @sa resource
 * @sa Palette
 * @sa PaletteRef
 */
template<class T>
struct PaletteBase : T
{
  // Make default ctors available
  using T::T;

  /**
   * Create a palette structure with the specified number of color entries.
   *
   * The palette entries are initialized to white.
   *
   * @param ncolors represents the number of color entries in the color palette.
   * @post this represents a new Palette structure convertible to true on
   * success or converts to false on failure (e.g. if there wasn't enough
   * memory); call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColors()
   * @sa Surface.SetPalette()
   */
  inline PaletteBase(int ncolors)
    : T(SDL_CreatePalette(ncolors))
  {
  }

  constexpr int GetSize() const { return this->ncolors; }

  constexpr Color operator[](int index) const { return this->colors[index]; }

  /**
   * Set a range of colors in a palette.
   *
   * @param colors an array of SDL_Color structures to copy into the palette.
   * @param firstcolor the index of the first palette entry to modify.
   * @param ncolors the number of entries to modify.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  inline bool SetColors(const Color* colors, int firstcolor, int ncolors)
  {
    return SDL_SetPaletteColors(T::get(), colors, firstcolor, ncolors);
  }

  /**
   * Set a range of colors in a palette.
   *
   * @param colors a span of SDL_Color structures to copy into the palette.
   * @param firstcolor the index of the first palette entry to modify.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   */
  bool SetColors(SpanRef<const SDL_Color> colors, int firstcolor = 0)
  {
    SDL_assert_paranoid(colors.size() < SDL_MAX_SINT32);
    return SetColors(colors.data(), firstcolor, colors.size());
  }

  /**
   * Free a palette
   *
   * After calling, this object becomes empty.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Destroy() { return SDL_DestroyPalette(T::release()); }
};

/**
 * Map an RGB triple to an opaque pixel value for a given pixel format.
 *
 * This function maps the RGB color value to the specified pixel format and
 * returns the pixel value best approximating the given RGB color value for
 * the given pixel format.
 *
 * If the format has a palette (8-bit) the index of the closest matching color
 * in the palette will be returned.
 *
 * If the specified pixel format has an alpha component it will be returned as
 * all 1 bits (fully opaque).
 *
 * If the pixel format bpp (color depth) is less than 32-bpp then the unused
 * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
 * format the return value can be assigned to a Uint16, and similarly a Uint8
 * for an 8-bpp format).
 *
 * @param format a pointer to SDL_PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be NULL.
 * @param r the red component of the pixel in the range 0-255.
 * @param g the green component of the pixel in the range 0-255.
 * @param b the blue component of the pixel in the range 0-255.
 * @returns a pixel value.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPixelFormatDetails()
 * @sa GetRGB()
 * @sa MapRGBA()
 * @sa Surface.MapColor()
 */
inline Uint32 MapRGB(const PixelFormatDetails* format,
                     PaletteRef palette,
                     Uint8 r,
                     Uint8 g,
                     Uint8 b)
{
  return SDL_MapRGB(format, palette.get(), r, g, b);
}

/**
 * Map an RGBA quadruple to a pixel value for a given pixel format.
 *
 * This function maps the RGBA color value to the specified pixel format and
 * returns the pixel value best approximating the given RGBA color value for
 * the given pixel format.
 *
 * If the specified pixel format has no alpha component the alpha value will
 * be ignored (as it will be in formats with a palette).
 *
 * If the format has a palette (8-bit) the index of the closest matching color
 * in the palette will be returned.
 *
 * If the pixel format bpp (color depth) is less than 32-bpp then the unused
 * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
 * format the return value can be assigned to a Uint16, and similarly a Uint8
 * for an 8-bpp format).
 *
 * @param format a pointer to SDL_PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be NULL.
 * @param r the red component of the pixel in the range 0-255.
 * @param g the green component of the pixel in the range 0-255.
 * @param b the blue component of the pixel in the range 0-255.
 * @param a the alpha component of the pixel in the range 0-255.
 * @returns a pixel value.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails()
 * @sa GetRGBA()
 * @sa MapRGB()
 * @sa Surface.MapColor()
 */
inline Uint32 MapRGBA(const PixelFormatDetails* format,
                      PaletteRef palette,
                      Uint8 r,
                      Uint8 g,
                      Uint8 b,
                      Uint8 a)
{
  return SDL_MapRGBA(format, palette.get(), r, g, b, a);
}

/**
 * Get RGB values from a pixel in the specified format.
 *
 * This function uses the entire 8-bit [0..255] range when converting color
 * components from pixel formats with less than 8-bits per RGB component
 * (e.g., a completely white pixel in 16-bit RGB565 format would return [0xff,
 * 0xff, 0xff] not [0xf8, 0xfc, 0xf8]).
 *
 * @param pixel a pixel value.
 * @param format a pointer to SDL_PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be NULL.
 * @param r a pointer filled in with the red component, may be NULL.
 * @param g a pointer filled in with the green component, may be NULL.
 * @param b a pointer filled in with the blue component, may be NULL.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails()
 * @sa GetRGBA()
 * @sa MapRGB()
 * @sa MapRGBA()
 */
inline void GetRGB(Uint32 pixel,
                   const PixelFormatDetails* format,
                   PaletteRef palette,
                   Uint8* r,
                   Uint8* g,
                   Uint8* b)
{
  SDL_GetRGB(pixel, format, palette.get(), r, g, b);
}

/**
 * Get RGBA values from a pixel in the specified format.
 *
 * This function uses the entire 8-bit [0..255] range when converting color
 * components from pixel formats with less than 8-bits per RGB component
 * (e.g., a completely white pixel in 16-bit RGB565 format would return [0xff,
 * 0xff, 0xff] not [0xf8, 0xfc, 0xf8]).
 *
 * If the surface has no alpha component, the alpha will be returned as 0xff
 * (100% opaque).
 *
 * @param pixel a pixel value.
 * @param format a pointer to SDL_PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be NULL.
 * @param r a pointer filled in with the red component, may be NULL.
 * @param g a pointer filled in with the green component, may be NULL.
 * @param b a pointer filled in with the blue component, may be NULL.
 * @param a a pointer filled in with the alpha component, may be NULL.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails()
 * @sa GetRGB()
 * @sa MapRGB()
 * @sa MapRGBA()
 */
inline void GetRGBA(Uint32 pixel,
                    const PixelFormatDetails* format,
                    PaletteRef palette,
                    Uint8* r,
                    Uint8* g,
                    Uint8* b,
                    Uint8* a)
{
  SDL_GetRGBA(pixel, format, palette.get(), r, g, b, a);
}

/** @} */

#pragma region impl

inline void ObjectDeleter<SDL_Palette>::operator()(PaletteRef palette) const
{
  palette.Destroy();
}

inline Uint32 Color::Map(const PixelFormatDetails* format,
                         PaletteRef palette = nullptr) const
{
  return MapRGBA(format, palette.get(), r, g, b, a);
}

inline Color Get(Uint32 pixel,
                 const PixelFormatDetails* format,
                 PaletteRef palette = nullptr)
{
  Color c;
  GetRGBA(pixel, format, palette, &c.r, &c.g, &c.b, &c.a);
  return c;
}

inline Uint32 PixelFormat::Map(Color color, PaletteRef palette = nullptr) const
{
  return color.Map(GetDetails(), palette);
}

inline Color PixelFormat::Get(Uint32 pixel, PaletteRef palette = nullptr) const
{
  return Color::Get(pixel, GetDetails(), palette);
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_PIXELS_H_ */
