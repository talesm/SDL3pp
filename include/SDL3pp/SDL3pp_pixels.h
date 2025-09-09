#ifndef SDL3PP_PIXELS_H_
#define SDL3PP_PIXELS_H_

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_version.h>
#include "SDL3pp_error.h"
#include "SDL3pp_spanRef.h"

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
struct Color;

#ifdef SDL3PP_DOC

/**
 * A fully opaque 8-bit alpha value.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_ALPHA_TRANSPARENT
 */
#define SDL_ALPHA_OPAQUE 255

/**
 * A fully opaque floating point alpha value.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_ALPHA_TRANSPARENT_FLOAT
 */
#define SDL_ALPHA_OPAQUE_FLOAT 1.0f

/**
 * A fully transparent 8-bit alpha value.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_ALPHA_OPAQUE
 */
#define SDL_ALPHA_TRANSPARENT 0

/**
 * A fully transparent floating point alpha value.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_ALPHA_OPAQUE_FLOAT
 */
#define SDL_ALPHA_TRANSPARENT_FLOAT 0.0f

#endif // SDL3PP_DOC

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

constexpr PixelType PIXELTYPE_UNKNOWN = SDL_PIXELTYPE_UNKNOWN; ///< UNKNOWN

constexpr PixelType PIXELTYPE_INDEX1 = SDL_PIXELTYPE_INDEX1; ///< INDEX1

constexpr PixelType PIXELTYPE_INDEX4 = SDL_PIXELTYPE_INDEX4; ///< INDEX4

constexpr PixelType PIXELTYPE_INDEX8 = SDL_PIXELTYPE_INDEX8; ///< INDEX8

constexpr PixelType PIXELTYPE_PACKED8 = SDL_PIXELTYPE_PACKED8; ///< PACKED8

constexpr PixelType PIXELTYPE_PACKED16 = SDL_PIXELTYPE_PACKED16; ///< PACKED16

constexpr PixelType PIXELTYPE_PACKED32 = SDL_PIXELTYPE_PACKED32; ///< PACKED32

constexpr PixelType PIXELTYPE_ARRAYU8 = SDL_PIXELTYPE_ARRAYU8; ///< ARRAYU8

constexpr PixelType PIXELTYPE_ARRAYU16 = SDL_PIXELTYPE_ARRAYU16; ///< ARRAYU16

constexpr PixelType PIXELTYPE_ARRAYU32 = SDL_PIXELTYPE_ARRAYU32; ///< ARRAYU32

constexpr PixelType PIXELTYPE_ARRAYF16 = SDL_PIXELTYPE_ARRAYF16; ///< ARRAYF16

constexpr PixelType PIXELTYPE_ARRAYF32 = SDL_PIXELTYPE_ARRAYF32; ///< ARRAYF32

constexpr PixelType PIXELTYPE_INDEX2 = SDL_PIXELTYPE_INDEX2; ///< INDEX2

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

constexpr BitmapOrder BITMAPORDER_NONE = SDL_BITMAPORDER_NONE; ///< NONE

constexpr BitmapOrder BITMAPORDER_4321 = SDL_BITMAPORDER_4321; ///< 4321

constexpr BitmapOrder BITMAPORDER_1234 = SDL_BITMAPORDER_1234; ///< 1234

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

constexpr PackedOrder PACKEDORDER_NONE = SDL_PACKEDORDER_NONE; ///< NONE

constexpr PackedOrder PACKEDORDER_XRGB = SDL_PACKEDORDER_XRGB; ///< XRGB

constexpr PackedOrder PACKEDORDER_RGBX = SDL_PACKEDORDER_RGBX; ///< RGBX

constexpr PackedOrder PACKEDORDER_ARGB = SDL_PACKEDORDER_ARGB; ///< ARGB

constexpr PackedOrder PACKEDORDER_RGBA = SDL_PACKEDORDER_RGBA; ///< RGBA

constexpr PackedOrder PACKEDORDER_XBGR = SDL_PACKEDORDER_XBGR; ///< XBGR

constexpr PackedOrder PACKEDORDER_BGRX = SDL_PACKEDORDER_BGRX; ///< BGRX

constexpr PackedOrder PACKEDORDER_ABGR = SDL_PACKEDORDER_ABGR; ///< ABGR

constexpr PackedOrder PACKEDORDER_BGRA = SDL_PACKEDORDER_BGRA; ///< BGRA

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

constexpr ArrayOrder ARRAYORDER_NONE = SDL_ARRAYORDER_NONE; ///< NONE

constexpr ArrayOrder ARRAYORDER_RGB = SDL_ARRAYORDER_RGB; ///< RGB

constexpr ArrayOrder ARRAYORDER_RGBA = SDL_ARRAYORDER_RGBA; ///< RGBA

constexpr ArrayOrder ARRAYORDER_ARGB = SDL_ARRAYORDER_ARGB; ///< ARGB

constexpr ArrayOrder ARRAYORDER_BGR = SDL_ARRAYORDER_BGR; ///< BGR

constexpr ArrayOrder ARRAYORDER_BGRA = SDL_ARRAYORDER_BGRA; ///< BGRA

constexpr ArrayOrder ARRAYORDER_ABGR = SDL_ARRAYORDER_ABGR; ///< ABGR

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

constexpr PackedLayout PACKEDLAYOUT_NONE = SDL_PACKEDLAYOUT_NONE; ///< NONE

constexpr PackedLayout PACKEDLAYOUT_332 = SDL_PACKEDLAYOUT_332; ///< 332

constexpr PackedLayout PACKEDLAYOUT_4444 = SDL_PACKEDLAYOUT_4444; ///< 4444

constexpr PackedLayout PACKEDLAYOUT_1555 = SDL_PACKEDLAYOUT_1555; ///< 1555

constexpr PackedLayout PACKEDLAYOUT_5551 = SDL_PACKEDLAYOUT_5551; ///< 5551

constexpr PackedLayout PACKEDLAYOUT_565 = SDL_PACKEDLAYOUT_565; ///< 565

constexpr PackedLayout PACKEDLAYOUT_8888 = SDL_PACKEDLAYOUT_8888; ///< 8888

constexpr PackedLayout PACKEDLAYOUT_2101010 =
  SDL_PACKEDLAYOUT_2101010; ///< 2101010

constexpr PackedLayout PACKEDLAYOUT_1010102 =
  SDL_PACKEDLAYOUT_1010102; ///< 1010102

/// @}

/**
 * Details about the format of a pixel.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PixelFormatDetails = SDL_PixelFormatDetails;

#ifdef SDL3PP_DOC

/**
 * A macro for defining custom FourCC pixel formats.
 *
 * For example, defining PIXELFORMAT_YV12 looks like this:
 *
 * ```c
 * SDL_DEFINE_PIXELFOURCC('Y', 'V', '1', '2')
 * ```
 *
 * @param A the first character of the FourCC code.
 * @param B the second character of the FourCC code.
 * @param C the third character of the FourCC code.
 * @param D the fourth character of the FourCC code.
 * @returns a format value in the style of PixelFormat.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_DEFINE_PIXELFOURCC(A, B, C, D) SDL_FOURCC(A, B, C, D)

/**
 * A macro to retrieve the flags of an PixelFormat.
 *
 * This macro is generally not needed directly by an app, which should use
 * specific tests, like SDL_ISPIXELFORMAT_FOURCC, instead.
 *
 * @param format an PixelFormat to check.
 * @returns the flags of `format`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_PIXELFLAG(format) (((format) >> 28) & 0x0F)

#endif // SDL3PP_DOC

/**
 * @name PixelFormats
 * @{
 */

/**
 * Pixel format.
 *
 * SDL's pixel formats have the following naming convention:
 *
 * - Names with a list of components and a single bit count, such as RGB24 and
 *   ABGR32, define a platform-independent encoding into bytes in the order
 *   specified. For example, in RGB24 data, each pixel is encoded in 3 bytes
 *   (red, green, blue) in that order, and in ABGR32 data, each pixel is
 *   encoded in 4 bytes (alpha, blue, green, red) in that order. Use these
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
 * @cat wrap-state
 *
 * @sa wrap-state
 * @sa PixelFormats
 */
class PixelFormat
{
  SDL_PixelFormat m_format;

public:
  /**
   * Wraps PixelFormat.
   *
   * @param format the value to be wrapped
   */
  constexpr PixelFormat(SDL_PixelFormat format = {})
    : m_format(format)
  {
  }

  /**
   * Defining custom non-FourCC pixel formats.
   *
   * For example, defining PIXELFORMAT_RGBA8888 looks like this:
   *
   * ```c
   * PixelFormat(PIXELTYPE_PACKED32, PACKEDORDER_RGBA,
   * PACKEDLAYOUT_8888, 32, 4)
   * ```
   *
   * @param type the type of the new format, probably a PixelType value.
   * @param order the order of the new format, probably a BitmapOrder,
   *              PackedOrder, or ArrayOrder value.
   * @param layout the layout of the new format, probably an PackedLayout
   *               value or zero.
   * @param bits the number of bits per pixel of the new format.
   * @param bytes the number of bytes per pixel of the new format.
   * @post a format value in the style of PixelFormat.
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
    : m_format(SDL_PixelFormat(
        SDL_DEFINE_PIXELFORMAT(type, order, layout, bits, bytes)))
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const PixelFormat& other) const = default;

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(SDL_PixelFormat format) const
  {
    return operator==(PixelFormat(format));
  }

  /**
   * Unwraps to the underlying PixelFormat.
   *
   * @returns the underlying PixelFormat.
   */
  constexpr operator SDL_PixelFormat() const { return m_format; }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return m_format != 0; }

  /**
   * Retrieve the type.
   *
   * @returns the type as PixelType.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr PixelType GetType() const
  {
    return PixelType(SDL_PIXELTYPE(m_format));
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
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr int GetOrder() const { return SDL_PIXELORDER(m_format); }

  /**
   * Retrieve the layout.
   *
   * This is usually a value from the PackedLayout enumeration, or zero if a
   * layout doesn't make sense for the format type.
   *
   * @returns the layout
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr PackedLayout GetLayout() const
  {
    return PackedLayout(SDL_PIXELLAYOUT(m_format));
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
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBytesPerPixel
   */
  constexpr int GetBitsPerPixel() const { return SDL_BITSPERPIXEL(m_format); }

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
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBitsPerPixel
   */
  constexpr int GetBytesPerPixel() const { return SDL_BYTESPERPIXEL(m_format); }

  /**
   * Determine if this is an indexed format.
   *
   * @returns true if the format is indexed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsIndexed() const
  {
    return SDL_ISPIXELFORMAT_INDEXED(m_format);
  }

/**
 * A macro to determine if an PixelFormat is a packed format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is packed, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_ISPIXELFORMAT_PACKED(format)                                       \
  (!SDL_ISPIXELFORMAT_FOURCC(format) &&                                        \
   ((SDL_PIXELTYPE(format) == SDL_PIXELTYPE_PACKED8) ||                        \
    (SDL_PIXELTYPE(format) == SDL_PIXELTYPE_PACKED16) ||                       \
    (SDL_PIXELTYPE(format) == SDL_PIXELTYPE_PACKED32)))

  /**
   * Determine if this is an array format.
   *
   * @returns true if the format is an array, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsArray() const { return SDL_ISPIXELFORMAT_ARRAY(m_format); }

  /**
   * Determine if this is a 10-bit format.
   *
   * @returns true if the format is 10-bit, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Is10Bit() const { return SDL_ISPIXELFORMAT_10BIT(m_format); }

  /**
   * Determine if this is a floating point format.
   *
   * @returns true if the format is 10-bit, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFloat() const { return SDL_ISPIXELFORMAT_FLOAT(m_format); }

  /**
   * Determine if this has an alpha channel.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsAlpha() const { return SDL_ISPIXELFORMAT_ALPHA(m_format); }

  /**
   * Determine if this is a "FourCC" format.
   *
   * This covers custom and other unusual formats.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFourCC() const { return SDL_ISPIXELFORMAT_FOURCC(m_format); }

  /**
   * Get the human readable name of a pixel format.
   *
   * @returns the human readable name of the specified pixel format or
   *          "PIXELFORMAT_UNKNOWN" if the format isn't recognized.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const char* GetName() const { return SDL_GetPixelFormatName(m_format); }

  /**
   * Convert one of the enumerated pixel formats to a bpp value and RGBA masks.
   *
   * @param bpp a bits per pixel value; usually 15, 16, or 32.
   * @param Rmask a pointer filled in with the red mask for the format.
   * @param Gmask a pointer filled in with the green mask for the format.
   * @param Bmask a pointer filled in with the blue mask for the format.
   * @param Amask a pointer filled in with the alpha mask for the format.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PixelFormat.ForMasks
   */
  void GetMasks(int* bpp,
                Uint32* Rmask,
                Uint32* Gmask,
                Uint32* Bmask,
                Uint32* Amask) const
  {
    CheckError(
      SDL_GetMasksForPixelFormat(m_format, bpp, Rmask, Gmask, Bmask, Amask));
  }

  /**
   * Convert a bpp value and RGBA masks to an enumerated pixel format.
   *
   * This will return `PIXELFORMAT_UNKNOWN` if the conversion wasn't
   * possible.
   *
   * @param bpp a bits per pixel value; usually 15, 16, or 32.
   * @param Rmask the red mask for the format.
   * @param Gmask the green mask for the format.
   * @param Bmask the blue mask for the format.
   * @param Amask the alpha mask for the format.
   * @returns the PixelFormat value corresponding to the format masks, or
   *          PIXELFORMAT_UNKNOWN if there isn't a match.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PixelFormat.GetMasks
   */
  static PixelFormat ForMasks(int bpp,
                              Uint32 Rmask,
                              Uint32 Gmask,
                              Uint32 Bmask,
                              Uint32 Amask)
  {
    return SDL_GetPixelFormatForMasks(bpp, Rmask, Gmask, Bmask, Amask);
  }

  /**
   * Create an PixelFormatDetails structure corresponding to a pixel format.
   *
   * Returned structure may come from a shared global cache (i.e. not newly
   * allocated), and hence should not be modified, especially the palette. Weird
   * errors such as `Blit combination not supported` may occur.
   *
   * @returns a pointer to a PixelFormatDetails structure or nullptr on
   *          failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const PixelFormatDetails* GetDetails() const
  {
    return SDL_GetPixelFormatDetails(m_format);
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
  inline Uint32 Map(Color color, SDL_Palette* palette) const;

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
  inline Color Get(Uint32 pixel, SDL_Palette* palette) const;
};

constexpr SDL_PixelFormat PIXELFORMAT_UNKNOWN =
  SDL_PIXELFORMAT_UNKNOWN; ///< UNKNOWN

constexpr SDL_PixelFormat PIXELFORMAT_INDEX1LSB =
  SDL_PIXELFORMAT_INDEX1LSB; ///< INDEX1LSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX1MSB =
  SDL_PIXELFORMAT_INDEX1MSB; ///< INDEX1MSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX2LSB =
  SDL_PIXELFORMAT_INDEX2LSB; ///< INDEX2LSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX2MSB =
  SDL_PIXELFORMAT_INDEX2MSB; ///< INDEX2MSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX4LSB =
  SDL_PIXELFORMAT_INDEX4LSB; ///< INDEX4LSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX4MSB =
  SDL_PIXELFORMAT_INDEX4MSB; ///< INDEX4MSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX8 =
  SDL_PIXELFORMAT_INDEX8; ///< INDEX8

constexpr SDL_PixelFormat PIXELFORMAT_RGB332 =
  SDL_PIXELFORMAT_RGB332; ///< RGB332

constexpr SDL_PixelFormat PIXELFORMAT_XRGB4444 =
  SDL_PIXELFORMAT_XRGB4444; ///< XRGB4444

constexpr SDL_PixelFormat PIXELFORMAT_XBGR4444 =
  SDL_PIXELFORMAT_XBGR4444; ///< XBGR4444

constexpr SDL_PixelFormat PIXELFORMAT_XRGB1555 =
  SDL_PIXELFORMAT_XRGB1555; ///< XRGB1555

constexpr SDL_PixelFormat PIXELFORMAT_XBGR1555 =
  SDL_PIXELFORMAT_XBGR1555; ///< XBGR1555

constexpr SDL_PixelFormat PIXELFORMAT_ARGB4444 =
  SDL_PIXELFORMAT_ARGB4444; ///< ARGB4444

constexpr SDL_PixelFormat PIXELFORMAT_RGBA4444 =
  SDL_PIXELFORMAT_RGBA4444; ///< RGBA4444

constexpr SDL_PixelFormat PIXELFORMAT_ABGR4444 =
  SDL_PIXELFORMAT_ABGR4444; ///< ABGR4444

constexpr SDL_PixelFormat PIXELFORMAT_BGRA4444 =
  SDL_PIXELFORMAT_BGRA4444; ///< BGRA4444

constexpr SDL_PixelFormat PIXELFORMAT_ARGB1555 =
  SDL_PIXELFORMAT_ARGB1555; ///< ARGB1555

constexpr SDL_PixelFormat PIXELFORMAT_RGBA5551 =
  SDL_PIXELFORMAT_RGBA5551; ///< RGBA5551

constexpr SDL_PixelFormat PIXELFORMAT_ABGR1555 =
  SDL_PIXELFORMAT_ABGR1555; ///< ABGR1555

constexpr SDL_PixelFormat PIXELFORMAT_BGRA5551 =
  SDL_PIXELFORMAT_BGRA5551; ///< BGRA5551

constexpr SDL_PixelFormat PIXELFORMAT_RGB565 =
  SDL_PIXELFORMAT_RGB565; ///< RGB565

constexpr SDL_PixelFormat PIXELFORMAT_BGR565 =
  SDL_PIXELFORMAT_BGR565; ///< BGR565

constexpr SDL_PixelFormat PIXELFORMAT_RGB24 = SDL_PIXELFORMAT_RGB24; ///< RGB24

constexpr SDL_PixelFormat PIXELFORMAT_BGR24 = SDL_PIXELFORMAT_BGR24; ///< BGR24

constexpr SDL_PixelFormat PIXELFORMAT_XRGB8888 =
  SDL_PIXELFORMAT_XRGB8888; ///< XRGB8888

constexpr SDL_PixelFormat PIXELFORMAT_RGBX8888 =
  SDL_PIXELFORMAT_RGBX8888; ///< RGBX8888

constexpr SDL_PixelFormat PIXELFORMAT_XBGR8888 =
  SDL_PIXELFORMAT_XBGR8888; ///< XBGR8888

constexpr SDL_PixelFormat PIXELFORMAT_BGRX8888 =
  SDL_PIXELFORMAT_BGRX8888; ///< BGRX8888

constexpr SDL_PixelFormat PIXELFORMAT_ARGB8888 =
  SDL_PIXELFORMAT_ARGB8888; ///< ARGB8888

constexpr SDL_PixelFormat PIXELFORMAT_RGBA8888 =
  SDL_PIXELFORMAT_RGBA8888; ///< RGBA8888

constexpr SDL_PixelFormat PIXELFORMAT_ABGR8888 =
  SDL_PIXELFORMAT_ABGR8888; ///< ABGR8888

constexpr SDL_PixelFormat PIXELFORMAT_BGRA8888 =
  SDL_PIXELFORMAT_BGRA8888; ///< BGRA8888

constexpr SDL_PixelFormat PIXELFORMAT_XRGB2101010 =
  SDL_PIXELFORMAT_XRGB2101010; ///< XRGB2101010

constexpr SDL_PixelFormat PIXELFORMAT_XBGR2101010 =
  SDL_PIXELFORMAT_XBGR2101010; ///< XBGR2101010

constexpr SDL_PixelFormat PIXELFORMAT_ARGB2101010 =
  SDL_PIXELFORMAT_ARGB2101010; ///< ARGB2101010

constexpr SDL_PixelFormat PIXELFORMAT_ABGR2101010 =
  SDL_PIXELFORMAT_ABGR2101010; ///< ABGR2101010

constexpr SDL_PixelFormat PIXELFORMAT_RGB48 = SDL_PIXELFORMAT_RGB48; ///< RGB48

constexpr SDL_PixelFormat PIXELFORMAT_BGR48 = SDL_PIXELFORMAT_BGR48; ///< BGR48

constexpr SDL_PixelFormat PIXELFORMAT_RGBA64 =
  SDL_PIXELFORMAT_RGBA64; ///< RGBA64

constexpr SDL_PixelFormat PIXELFORMAT_ARGB64 =
  SDL_PIXELFORMAT_ARGB64; ///< ARGB64

constexpr SDL_PixelFormat PIXELFORMAT_BGRA64 =
  SDL_PIXELFORMAT_BGRA64; ///< BGRA64

constexpr SDL_PixelFormat PIXELFORMAT_ABGR64 =
  SDL_PIXELFORMAT_ABGR64; ///< ABGR64

constexpr SDL_PixelFormat PIXELFORMAT_RGB48_FLOAT =
  SDL_PIXELFORMAT_RGB48_FLOAT; ///< RGB48_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_BGR48_FLOAT =
  SDL_PIXELFORMAT_BGR48_FLOAT; ///< BGR48_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_RGBA64_FLOAT =
  SDL_PIXELFORMAT_RGBA64_FLOAT; ///< RGBA64_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_ARGB64_FLOAT =
  SDL_PIXELFORMAT_ARGB64_FLOAT; ///< ARGB64_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_BGRA64_FLOAT =
  SDL_PIXELFORMAT_BGRA64_FLOAT; ///< BGRA64_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_ABGR64_FLOAT =
  SDL_PIXELFORMAT_ABGR64_FLOAT; ///< ABGR64_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_RGB96_FLOAT =
  SDL_PIXELFORMAT_RGB96_FLOAT; ///< RGB96_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_BGR96_FLOAT =
  SDL_PIXELFORMAT_BGR96_FLOAT; ///< BGR96_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_RGBA128_FLOAT =
  SDL_PIXELFORMAT_RGBA128_FLOAT; ///< RGBA128_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_ARGB128_FLOAT =
  SDL_PIXELFORMAT_ARGB128_FLOAT; ///< ARGB128_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_BGRA128_FLOAT =
  SDL_PIXELFORMAT_BGRA128_FLOAT; ///< BGRA128_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_ABGR128_FLOAT =
  SDL_PIXELFORMAT_ABGR128_FLOAT; ///< ABGR128_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_YV12 =
  SDL_PIXELFORMAT_YV12; ///< Planar mode: Y + V + U (3 planes)

constexpr SDL_PixelFormat PIXELFORMAT_IYUV =
  SDL_PIXELFORMAT_IYUV; ///< Planar mode: Y + U + V (3 planes)

constexpr SDL_PixelFormat PIXELFORMAT_YUY2 =
  SDL_PIXELFORMAT_YUY2; ///< Packed mode: Y0+U0+Y1+V0 (1 plane)

constexpr SDL_PixelFormat PIXELFORMAT_UYVY =
  SDL_PIXELFORMAT_UYVY; ///< Packed mode: U0+Y0+V0+Y1 (1 plane)

constexpr SDL_PixelFormat PIXELFORMAT_YVYU =
  SDL_PIXELFORMAT_YVYU; ///< Packed mode: Y0+V0+Y1+U0 (1 plane)

constexpr SDL_PixelFormat PIXELFORMAT_NV12 =
  SDL_PIXELFORMAT_NV12; ///< Planar mode: Y + U/V interleaved (2 planes)

constexpr SDL_PixelFormat PIXELFORMAT_NV21 =
  SDL_PIXELFORMAT_NV21; ///< Planar mode: Y + V/U interleaved (2 planes)

constexpr SDL_PixelFormat PIXELFORMAT_P010 =
  SDL_PIXELFORMAT_P010; ///< Planar mode: Y + U/V interleaved (2 planes)

constexpr SDL_PixelFormat PIXELFORMAT_EXTERNAL_OES =
  SDL_PIXELFORMAT_EXTERNAL_OES; ///< Android video texture format.

#if SDL_VERSION_ATLEAST(3, 2, 10)

constexpr SDL_PixelFormat PIXELFORMAT_MJPG =
  SDL_PIXELFORMAT_MJPG; ///< Motion JPEG.

#endif // SDL_VERSION_ATLEAST(3, 2, 10)

constexpr SDL_PixelFormat PIXELFORMAT_RGBA32 =
  SDL_PIXELFORMAT_RGBA32; ///< RGBA32

constexpr SDL_PixelFormat PIXELFORMAT_ARGB32 =
  SDL_PIXELFORMAT_ARGB32; ///< ARGB32

constexpr SDL_PixelFormat PIXELFORMAT_BGRA32 =
  SDL_PIXELFORMAT_BGRA32; ///< BGRA32

constexpr SDL_PixelFormat PIXELFORMAT_ABGR32 =
  SDL_PIXELFORMAT_ABGR32; ///< ABGR32

constexpr SDL_PixelFormat PIXELFORMAT_RGBX32 =
  SDL_PIXELFORMAT_RGBX32; ///< RGBX32

constexpr SDL_PixelFormat PIXELFORMAT_XRGB32 =
  SDL_PIXELFORMAT_XRGB32; ///< XRGB32

constexpr SDL_PixelFormat PIXELFORMAT_BGRX32 =
  SDL_PIXELFORMAT_BGRX32; ///< BGRX32

constexpr SDL_PixelFormat PIXELFORMAT_XBGR32 =
  SDL_PIXELFORMAT_XBGR32; ///< XBGR32

/// @}

/**
 * @name ColorTypes
 * @{
 */

/**
 * Colorspace color type.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ColorType = SDL_ColorType;

constexpr ColorType COLOR_TYPE_UNKNOWN =
  SDL_COLOR_TYPE_UNKNOWN; ///< COLOR_TYPE_UNKNOWN

constexpr ColorType COLOR_TYPE_RGB = SDL_COLOR_TYPE_RGB; ///< COLOR_TYPE_RGB

constexpr ColorType COLOR_TYPE_YCBCR =
  SDL_COLOR_TYPE_YCBCR; ///< COLOR_TYPE_YCBCR

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

constexpr ColorRange COLOR_RANGE_UNKNOWN =
  SDL_COLOR_RANGE_UNKNOWN; ///< COLOR_RANGE_UNKNOWN

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

constexpr ColorPrimaries COLOR_PRIMARIES_UNKNOWN =
  SDL_COLOR_PRIMARIES_UNKNOWN; ///< COLOR_PRIMARIES_UNKNOWN

constexpr ColorPrimaries COLOR_PRIMARIES_BT709 =
  SDL_COLOR_PRIMARIES_BT709; ///< ITU-R BT.709-6.

constexpr ColorPrimaries COLOR_PRIMARIES_UNSPECIFIED =
  SDL_COLOR_PRIMARIES_UNSPECIFIED; ///< COLOR_PRIMARIES_UNSPECIFIED

constexpr ColorPrimaries COLOR_PRIMARIES_BT470M =
  SDL_COLOR_PRIMARIES_BT470M; ///< ITU-R BT.470-6 System M.

constexpr ColorPrimaries COLOR_PRIMARIES_BT470BG =
  SDL_COLOR_PRIMARIES_BT470BG; ///< ITU-R BT.470-6 System B, G / ITU-R BT.601-7
                               ///< 625.

constexpr ColorPrimaries COLOR_PRIMARIES_BT601 =
  SDL_COLOR_PRIMARIES_BT601; ///< ITU-R BT.601-7 525, SMPTE 170M.

/**
 * SMPTE 240M, functionally the same as COLOR_PRIMARIES_BT601.
 */
constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE240 =
  SDL_COLOR_PRIMARIES_SMPTE240;

constexpr ColorPrimaries COLOR_PRIMARIES_GENERIC_FILM =
  SDL_COLOR_PRIMARIES_GENERIC_FILM; ///< Generic film (color filters using
                                    ///< Illuminant C)

constexpr ColorPrimaries COLOR_PRIMARIES_BT2020 =
  SDL_COLOR_PRIMARIES_BT2020; ///< ITU-R BT.2020-2 / ITU-R BT.2100-0.

constexpr ColorPrimaries COLOR_PRIMARIES_XYZ =
  SDL_COLOR_PRIMARIES_XYZ; ///< SMPTE ST 428-1.

constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE431 =
  SDL_COLOR_PRIMARIES_SMPTE431; ///< SMPTE RP 431-2.

constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE432 =
  SDL_COLOR_PRIMARIES_SMPTE432; ///< SMPTE EG 432-1 / DCI P3.

constexpr ColorPrimaries COLOR_PRIMARIES_EBU3213 =
  SDL_COLOR_PRIMARIES_EBU3213; ///< EBU Tech.  3213-E

constexpr ColorPrimaries COLOR_PRIMARIES_CUSTOM =
  SDL_COLOR_PRIMARIES_CUSTOM; ///< COLOR_PRIMARIES_CUSTOM

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
  SDL_TRANSFER_CHARACTERISTICS_UNKNOWN; ///< TRANSFER_CHARACTERISTICS_UNKNOWN

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT709 =
  SDL_TRANSFER_CHARACTERISTICS_BT709; ///< Rec.  ITU-R BT.709-6 / ITU-R BT1361

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_UNSPECIFIED =
  SDL_TRANSFER_CHARACTERISTICS_UNSPECIFIED; ///< TRANSFER_CHARACTERISTICS_UNSPECIFIED

/**
 * ITU-R BT.470-6 System M / ITU-R BT1700 625 PAL & SECAM.
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA22 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA22;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA28 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA28; ///< ITU-R BT.470-6 System B, G.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT601 =
  SDL_TRANSFER_CHARACTERISTICS_BT601; ///< SMPTE ST 170M / ITU-R BT.601-7 525 or
                                      ///< 625.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE240 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE240; ///< SMPTE ST 240M.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LINEAR =
  SDL_TRANSFER_CHARACTERISTICS_LINEAR; ///< TRANSFER_CHARACTERISTICS_LINEAR

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100; ///< TRANSFER_CHARACTERISTICS_LOG100

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100_SQRT10 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100_SQRT10; ///< TRANSFER_CHARACTERISTICS_LOG100_SQRT10

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_IEC61966 =
  SDL_TRANSFER_CHARACTERISTICS_IEC61966; ///< IEC 61966-2-4.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT1361 =
  SDL_TRANSFER_CHARACTERISTICS_BT1361; ///< ITU-R BT1361 Extended Colour Gamut.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SRGB =
  SDL_TRANSFER_CHARACTERISTICS_SRGB; ///< IEC 61966-2-1 (sRGB or sYCC)

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_10BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_10BIT; ///< ITU-R BT2020 for 10-bit
                                             ///< system.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_12BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_12BIT; ///< ITU-R BT2020 for 12-bit
                                             ///< system.

/**
 * SMPTE ST 2084 for 10-, 12-, 14- and 16-bit systems.
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_PQ =
  SDL_TRANSFER_CHARACTERISTICS_PQ;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE428 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE428; ///< SMPTE ST 428-1.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_HLG =
  SDL_TRANSFER_CHARACTERISTICS_HLG; ///< ARIB STD-B67, known as "hybrid
                                    ///< log-gamma" (HLG)

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_CUSTOM =
  SDL_TRANSFER_CHARACTERISTICS_CUSTOM; ///< TRANSFER_CHARACTERISTICS_CUSTOM

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
  SDL_MATRIX_COEFFICIENTS_IDENTITY; ///< MATRIX_COEFFICIENTS_IDENTITY

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT709 =
  SDL_MATRIX_COEFFICIENTS_BT709; ///< ITU-R BT.709-6.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_UNSPECIFIED =
  SDL_MATRIX_COEFFICIENTS_UNSPECIFIED; ///< MATRIX_COEFFICIENTS_UNSPECIFIED

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_FCC =
  SDL_MATRIX_COEFFICIENTS_FCC; ///< US FCC Title 47.

/**
 * ITU-R BT.470-6 System B, G / ITU-R BT.601-7 625, functionally the same as
 * MATRIX_COEFFICIENTS_BT601.
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT470BG =
  SDL_MATRIX_COEFFICIENTS_BT470BG;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT601 =
  SDL_MATRIX_COEFFICIENTS_BT601; ///< ITU-R BT.601-7 525.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE240 =
  SDL_MATRIX_COEFFICIENTS_SMPTE240; ///< SMPTE 240M.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_YCGCO =
  SDL_MATRIX_COEFFICIENTS_YCGCO; ///< MATRIX_COEFFICIENTS_YCGCO

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_NCL =
  SDL_MATRIX_COEFFICIENTS_BT2020_NCL; ///< ITU-R BT.2020-2 non-constant
                                      ///< luminance.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_CL =
  SDL_MATRIX_COEFFICIENTS_BT2020_CL; ///< ITU-R BT.2020-2 constant luminance.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE2085 =
  SDL_MATRIX_COEFFICIENTS_SMPTE2085; ///< SMPTE ST 2085.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL; ///< MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL; ///< MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_ICTCP =
  SDL_MATRIX_COEFFICIENTS_ICTCP; ///< ITU-R BT.2100-0 ICTCP.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CUSTOM =
  SDL_MATRIX_COEFFICIENTS_CUSTOM; ///< MATRIX_COEFFICIENTS_CUSTOM

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

constexpr ChromaLocation CHROMA_LOCATION_NONE =
  SDL_CHROMA_LOCATION_NONE; ///< RGB, no chroma sampling.

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
 * @name Colorspaces
 * @{
 */

/**
 * Colorspace definitions.
 *
 * Since similar colorspaces may vary in their details (matrix, transfer
 * function, etc.), this is not an exhaustive list, but rather a
 * representative sample of the kinds of colorspaces supported in SDL.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @cat wrap-state
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
  SDL_Colorspace m_colorspace;

public:
  /**
   * Wraps Colorspace.
   *
   * @param colorspace the value to be wrapped
   */
  constexpr Colorspace(SDL_Colorspace colorspace = {})
    : m_colorspace(colorspace)
  {
  }

  /**
   * Define custom Colorspace formats.
   *
   * For example, defining COLORSPACE_SRGB looks like this:
   *
   * ```cpp
   * Colorspace colorspace(COLOR_TYPE_RGB,
   *                       COLOR_RANGE_FULL,
   *                       COLOR_PRIMARIES_BT709,
   *                       TRANSFER_CHARACTERISTICS_SRGB,
   *                       MATRIX_COEFFICIENTS_IDENTITY,
   *                       CHROMA_LOCATION_NONE)
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
   * @post a format value in the style of Colorspace.
   *
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
    : m_colorspace(SDL_Colorspace(SDL_DEFINE_COLORSPACE(type,
                                                        range,
                                                        primaries,
                                                        transfer,
                                                        matrix,
                                                        chroma)))
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const Colorspace& other) const = default;

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(SDL_Colorspace colorspace) const
  {
    return operator==(Colorspace(colorspace));
  }

  /// True if a valid colorspace
  /**
   * Unwraps to the underlying Colorspace.
   *
   * @returns the underlying Colorspace.
   */
  constexpr operator SDL_Colorspace() const { return m_colorspace; }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return m_colorspace != 0; }

  /**
   * Retrieve the type of a Colorspace.
   *
   * @returns the ColorType for `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ColorType GetType() const
  {
    return SDL_COLORSPACETYPE(m_colorspace);
  }

  /**
   * Retrieve the range of a Colorspace.
   *
   * @returns the ColorRange of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ColorRange GetRange() const
  {
    return SDL_COLORSPACERANGE(m_colorspace);
  }

  /**
   * Retrieve the chroma sample location of an Colorspace.
   *
   * @returns the ChromaLocation of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ChromaLocation GetChroma() const
  {
    return SDL_COLORSPACECHROMA(m_colorspace);
  }

  /**
   * Retrieve the primaries of an Colorspace.
   *
   * @returns the ColorPrimaries of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ColorPrimaries GetPrimaries() const
  {
    return SDL_COLORSPACEPRIMARIES(m_colorspace);
  }

  /**
   * Retrieve the transfer characteristics of an Colorspace.
   *
   * @returns the TransferCharacteristics of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr TransferCharacteristics GetTransfer() const
  {
    return SDL_COLORSPACETRANSFER(m_colorspace);
  }

  /**
   * Retrieve the matrix coefficients of an Colorspace.
   *
   * @returns the MatrixCoefficients of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr MatrixCoefficients GetMatrix() const
  {
    return SDL_COLORSPACEMATRIX(m_colorspace);
  }

  /**
   * Determine if a Colorspace uses BT601 (or BT470BG) matrix coefficients.
   *
   * @returns true if BT601 or BT470BG, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT601() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT601(m_colorspace);
  }

  /**
   * Determine if an Colorspace uses BT709 matrix coefficients.
   *
   * @returns true if BT709, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT709() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT709(m_colorspace);
  }

  /**
   * Determine if an Colorspace uses BT2020_NCL matrix coefficients.
   *
   * @returns true if BT2020_NCL, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT2020_NCL() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT2020_NCL(m_colorspace);
  }

  /**
   * A function to determine if an Colorspace has a limited range.
   *
   * @returns true if limited range, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsLimitedRange() const
  {
    return SDL_ISCOLORSPACE_LIMITED_RANGE(m_colorspace);
  }

  /**
   * A function to determine if an Colorspace has a full range.
   *
   * @returns true if full range, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFullRange() const
  {
    return SDL_ISCOLORSPACE_FULL_RANGE(m_colorspace);
  }
};

constexpr SDL_Colorspace COLORSPACE_UNKNOWN =
  SDL_COLORSPACE_UNKNOWN; ///< UNKNOWN

/**
 * Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709.
 */
constexpr SDL_Colorspace COLORSPACE_SRGB = SDL_COLORSPACE_SRGB;

constexpr SDL_Colorspace COLORSPACE_SRGB_LINEAR =
  SDL_COLORSPACE_SRGB_LINEAR; ///< [object Object]

constexpr SDL_Colorspace COLORSPACE_HDR10 =
  SDL_COLORSPACE_HDR10; ///< [object Object]

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_NONE_P709_X601.
 */
constexpr SDL_Colorspace COLORSPACE_JPEG = SDL_COLORSPACE_JPEG;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601.
 */
constexpr SDL_Colorspace COLORSPACE_BT601_LIMITED =
  SDL_COLORSPACE_BT601_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601.
 */
constexpr SDL_Colorspace COLORSPACE_BT601_FULL = SDL_COLORSPACE_BT601_FULL;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709.
 */
constexpr SDL_Colorspace COLORSPACE_BT709_LIMITED =
  SDL_COLORSPACE_BT709_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709.
 */
constexpr SDL_Colorspace COLORSPACE_BT709_FULL = SDL_COLORSPACE_BT709_FULL;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P2020.
 */
constexpr SDL_Colorspace COLORSPACE_BT2020_LIMITED =
  SDL_COLORSPACE_BT2020_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P2020.
 */
constexpr SDL_Colorspace COLORSPACE_BT2020_FULL = SDL_COLORSPACE_BT2020_FULL;

/**
 * The default colorspace for RGB surfaces if no colorspace is specified.
 */
constexpr SDL_Colorspace COLORSPACE_RGB_DEFAULT = SDL_COLORSPACE_RGB_DEFAULT;

/**
 * The default colorspace for YUV surfaces if no colorspace is specified.
 */
constexpr SDL_Colorspace COLORSPACE_YUV_DEFAULT = SDL_COLORSPACE_YUV_DEFAULT;

/// @}

/**
 * A structure that represents a color as RGBA components.
 *
 * The bits of this structure can be directly reinterpreted as an
 * integer-packed color which uses the PIXELFORMAT_RGBA32 format
 * (PIXELFORMAT_ABGR8888 on little-endian systems and
 * PIXELFORMAT_RGBA8888 on big-endian systems).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 * @sa FColor
 */
struct Color : SDL_Color
{
  /**
   * Wraps Color.
   *
   * @param color the value to be wrapped
   */
  constexpr Color(const SDL_Color& color = {})
    : SDL_Color(color)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param r the value for channel r.
   * @param g the value for channel g.
   * @param b the value for channel b.
   * @param a the value for channel a.
   */
  constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
    : SDL_Color{r, g, b, a}
  {
  }

  /**
   * Default comparison operator
   */
  constexpr auto operator<=>(const Color& other) const
  {
    auto c = r <=> other.r;
    if (c != std::strong_ordering::equal) return c;
    c = g <=> other.g;
    if (c != std::strong_ordering::equal) return c;
    c = b <=> other.b;
    if (c != std::strong_ordering::equal) return c;
    return a <=> other.a;
  }

  /**
   * Compares with the underlying type
   */
  constexpr auto operator<=>(const SDL_Color& color) const
  {
    return operator<=>(Color(color));
  }

  /**
   * Get the red component from the color.
   *
   * @returns The red component from the color.
   */
  constexpr Uint8 GetR() const { return r; }

  /**
   * Set the red component from the color.
   *
   * @param newR New red component value.
   * @returns Reference to self.
   */
  constexpr Color& SetR(Uint8 newR)
  {
    r = newR;
    return *this;
  }

  /**
   * Get the green component from the color.
   *
   * @returns The green component from the color.
   *
   */
  constexpr Uint8 GetG() const { return g; }

  /**
   * Set the green component from the color.
   *
   * @param newG New green component value.
   * @returns Reference to self.
   */
  constexpr Color& SetG(Uint8 newG)
  {
    g = newG;
    return *this;
  }

  /**
   * Get the blue component from the color.
   *
   * @returns The blue component from the color.
   *
   */
  constexpr Uint8 GetB() const { return b; }

  /**
   * Set the blue component from the color.
   *
   * @param newB New blue component value.
   * @returns Reference to self.
   */
  constexpr Color& SetB(Uint8 newB)
  {
    b = newB;
    return *this;
  }

  /**
   * Get the alpha component from the color.
   *
   * @returns The alpha component from the color.
   *
   */
  constexpr Uint8 GetA() const { return a; }

  /**
   * Set the alpha component from the color.
   *
   * @param newA New alpha component value.
   * @returns Reference to self.
   */
  constexpr Color& SetA(Uint8 newA)
  {
    a = newA;
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
  Uint32 Map(const PixelFormatDetails& format,
             const SDL_Palette* palette) const;

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
                   const PixelFormatDetails& format,
                   SDL_Palette* palette);
};

/**
 * The bits of this structure can be directly reinterpreted as a float-packed
 * color which uses the PIXELFORMAT_RGBA128_FLOAT format
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa Color
 */
struct FColor : SDL_FColor
{
  /**
   * Wraps SDL_FColor.
   *
   * @param color the value to be wrapped
   */
  constexpr FColor(const SDL_FColor& color = {})
    : SDL_FColor(color)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param r the value for r.
   * @param g the value for g.
   * @param b the value for b.
   * @param a the value for a.
   */
  constexpr FColor(float r, float g, float b, float a = 1)
    : SDL_FColor{r, g, b, a}
  {
  }

  /**
   * Default comparison operator
   */
  constexpr auto operator<=>(const FColor& other) const
  {
    auto c = r <=> other.r;
    if (c != std::partial_ordering::equivalent) return c;
    c = g <=> other.g;
    if (c != std::partial_ordering::equivalent) return c;
    c = b <=> other.b;
    if (c != std::partial_ordering::equivalent) return c;
    return a <=> other.a;
  }

  /**
   * Compares with the underlying type
   */
  constexpr auto operator<=>(const SDL_FColor& color) const
  {
    return operator<=>(FColor(color));
  }

  /**
   * Get the red component from the color.
   *
   * @returns The red component from the color.
   */
  constexpr float GetR() const { return r; }

  /**
   * Set the red component from the color.
   *
   * @param newR New red component value.
   * @returns Reference to self.
   */
  constexpr FColor& SetR(float newR)
  {
    r = newR;
    return *this;
  }

  /**
   * Get the green component from the color.
   *
   * @returns The green component from the color.
   *
   */
  constexpr float GetG() const { return g; }

  /**
   * Set the green component from the color.
   *
   * @param newG New green component value.
   * @returns Reference to self.
   */
  constexpr FColor& SetG(float newG)
  {
    g = newG;
    return *this;
  }

  /**
   * Get the blue component from the color.
   *
   * @returns The blue component from the color.
   *
   */
  constexpr float GetB() const { return b; }

  /**
   * Set the blue component from the color.
   *
   * @param newB New blue component value.
   * @returns Reference to self.
   */
  constexpr FColor& SetB(float newB)
  {
    b = newB;
    return *this;
  }

  /**
   * Get the alpha component from the color.
   *
   * @returns The alpha component from the color.
   *
   */
  constexpr float GetA() const { return a; }

  /**
   * Set the alpha component from the color.
   *
   * @param newA New alpha component value.
   * @returns Reference to self.
   */
  constexpr FColor& SetA(float newA)
  {
    a = newA;
    return *this;
  }
};

/**
 * A set of indexed colors representing a palette.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Palette.SetColors
 *
 * @cat resource
 */
class Palette
{

  SDL_Palette* m_resource = nullptr;

public:
  constexpr Palette() = default;

  constexpr explicit Palette(SDL_Palette* resource)
    : m_resource(resource)
  {
  }

  Palette(const Palette& other) { ++m_resource->refcount; }

  constexpr Palette(Palette&& other) { other.m_resource = nullptr; }

  /**
   * Create a palette structure with the specified number of color entries.
   *
   * The palette entries are initialized to white.
   *
   * @param ncolors represents the number of color entries in the color palette.
   * @post a new Palette structure on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.Destroy
   * @sa Palette.SetColors
   * @sa SetSurfacePalette
   */
  Palette(int ncolors)
    : m_resource(CheckError(SDL_CreatePalette(ncolors)))
  {
  }

  ~Palette() { SDL_DestroyPalette(m_resource); }

  Palette& operator=(Palette other) { std::swap(m_resource, other.m_resource); }

  SDL_Palette* get() const { return m_resource; }

  SDL_Palette* release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  constexpr int GetSize() const { return m_resource->ncolors; }

  constexpr Color operator[](int index) const
  {
    return m_resource->colors[index];
  }

  /**
   * Set a range of colors in a palette.
   *
   * @param colors an array of Color structures to copy into the palette.
   * @param firstcolor the index of the first palette entry to modify.
   * @param ncolors the number of entries to modify.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetColors(SpanRef<const SDL_Color> colors, int firstcolor = 0)
  {
    CheckError(SDL_SetPaletteColors(
      m_resource, colors.data(), firstcolor, colors.size()));
  }

  /**
   * Free a palette created with Palette.Palette().
   *
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.Palette
   */
  void Destroy()
  {
    SDL_DestroyPalette(m_resource);
    m_resource = nullptr;
  }
};

/**
 * Get the human readable name of a pixel format.
 *
 * @param format the pixel format to query.
 * @returns the human readable name of the specified pixel format or
 *          "PIXELFORMAT_UNKNOWN" if the format isn't recognized.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetPixelFormatName(PixelFormat format)
{
  return SDL_GetPixelFormatName(format);
}

/**
 * Convert one of the enumerated pixel formats to a bpp value and RGBA masks.
 *
 * @param format one of the PixelFormat values.
 * @param bpp a bits per pixel value; usually 15, 16, or 32.
 * @param Rmask a pointer filled in with the red mask for the format.
 * @param Gmask a pointer filled in with the green mask for the format.
 * @param Bmask a pointer filled in with the blue mask for the format.
 * @param Amask a pointer filled in with the alpha mask for the format.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.ForMasks
 */
inline void GetMasksForPixelFormat(PixelFormat format,
                                   int* bpp,
                                   Uint32* Rmask,
                                   Uint32* Gmask,
                                   Uint32* Bmask,
                                   Uint32* Amask)
{
  CheckError(
    SDL_GetMasksForPixelFormat(format, bpp, Rmask, Gmask, Bmask, Amask));
}

/**
 * Convert a bpp value and RGBA masks to an enumerated pixel format.
 *
 * This will return `PIXELFORMAT_UNKNOWN` if the conversion wasn't
 * possible.
 *
 * @param bpp a bits per pixel value; usually 15, 16, or 32.
 * @param Rmask the red mask for the format.
 * @param Gmask the green mask for the format.
 * @param Bmask the blue mask for the format.
 * @param Amask the alpha mask for the format.
 * @returns the PixelFormat value corresponding to the format masks, or
 *          PIXELFORMAT_UNKNOWN if there isn't a match.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetMasks
 */
inline PixelFormat GetPixelFormatForMasks(int bpp,
                                          Uint32 Rmask,
                                          Uint32 Gmask,
                                          Uint32 Bmask,
                                          Uint32 Amask)
{
  return SDL_GetPixelFormatForMasks(bpp, Rmask, Gmask, Bmask, Amask);
}

/**
 * Create an PixelFormatDetails structure corresponding to a pixel format.
 *
 * Returned structure may come from a shared global cache (i.e. not newly
 * allocated), and hence should not be modified, especially the palette. Weird
 * errors such as `Blit combination not supported` may occur.
 *
 * @param format one of the PixelFormat values.
 * @returns a pointer to a PixelFormatDetails structure or nullptr on
 *          failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const PixelFormatDetails* GetPixelFormatDetails(PixelFormat format)
{
  return SDL_GetPixelFormatDetails(format);
}

/**
 * Set a range of colors in a palette.
 *
 * @param palette the Palette structure to modify.
 * @param colors an array of Color structures to copy into the palette.
 * @param firstcolor the index of the first palette entry to modify.
 * @param ncolors the number of entries to modify.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified or destroyed in another thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetPaletteColors(SDL_Palette* palette,
                             SpanRef<const SDL_Color> colors,
                             int firstcolor)
{
  CheckError(
    SDL_SetPaletteColors(palette, colors.data(), firstcolor, colors.size()));
}

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
 * @param format a pointer to PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be nullptr.
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
 * @sa PixelFormat.GetDetails
 * @sa GetRGB
 * @sa MapRGBA
 * @sa MapSurfaceRGB
 */
inline Uint32 MapRGB(const PixelFormatDetails* format,
                     const SDL_Palette* palette,
                     Uint8 r,
                     Uint8 g,
                     Uint8 b)
{
  return SDL_MapRGB(format, palette, r, g, b);
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
 * @param palette an optional palette for indexed formats, may be nullptr.
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
 * @sa PixelFormat.GetDetails
 * @sa GetRGBA
 * @sa MapRGB
 * @sa MapSurfaceRGBA
 */
inline Uint32 MapRGBA(const PixelFormatDetails* format,
                      const SDL_Palette* palette,
                      Uint8 r,
                      Uint8 g,
                      Uint8 b,
                      Uint8 a)
{
  return SDL_MapRGBA(format, palette, r, g, b, a);
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
 * @param format a pointer to PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be nullptr.
 * @param r a pointer filled in with the red component, may be nullptr.
 * @param g a pointer filled in with the green component, may be nullptr.
 * @param b a pointer filled in with the blue component, may be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails
 * @sa GetRGBA
 * @sa MapRGB
 * @sa MapRGBA
 */
inline void GetRGB(Uint32 pixel,
                   const PixelFormatDetails* format,
                   const SDL_Palette* palette,
                   Uint8* r,
                   Uint8* g,
                   Uint8* b)
{
  SDL_GetRGB(pixel, format, palette, r, g, b);
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
 * @param format a pointer to PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be nullptr.
 * @param r a pointer filled in with the red component, may be nullptr.
 * @param g a pointer filled in with the green component, may be nullptr.
 * @param b a pointer filled in with the blue component, may be nullptr.
 * @param a a pointer filled in with the alpha component, may be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails
 * @sa GetRGB
 * @sa MapRGB
 * @sa MapRGBA
 */
inline void GetRGBA(Uint32 pixel,
                    const PixelFormatDetails* format,
                    const SDL_Palette* palette,
                    Uint8* r,
                    Uint8* g,
                    Uint8* b,
                    Uint8* a)
{
  SDL_GetRGBA(pixel, format, palette, r, g, b, a);
}

/// @}

inline Uint32 Color::Map(const PixelFormatDetails& format,
                         const SDL_Palette* palette = nullptr) const
{
  return MapRGBA(&format, palette, r, g, b, a);
}

inline Color Color::Get(Uint32 pixel,
                        const PixelFormatDetails& format,
                        SDL_Palette* palette = nullptr)
{
  Color c;
  GetRGBA(pixel, &format, palette, &c.r, &c.g, &c.b, &c.a);
  return c;
}

inline Uint32 PixelFormat::Map(Color color,
                               SDL_Palette* palette = nullptr) const
{
  return color.Map(*GetDetails(), palette);
}

inline Color PixelFormat::Get(Uint32 pixel,
                              SDL_Palette* palette = nullptr) const
{
  return Color::Get(pixel, *GetDetails(), palette);
}

} // namespace SDL

#endif /* SDL3PP_PIXELS_H_ */
