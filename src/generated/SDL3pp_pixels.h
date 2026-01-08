#ifndef SDL3PP_PIXELS_H_
#define SDL3PP_PIXELS_H_

#include <SDL3/SDL_pixels.h>
#include "SDL3pp_error.h"
#include "SDL3pp_spanRef.h"
#include "SDL3pp_version.h"

namespace SDL {

/**
 * @defgroup CategoryPixels Category Pixels
 *
 * SDL offers facilities for pixel management.
 *
 * Largely these facilities deal with pixel _format_: what does this set of bits
 * represent?
 *
 * If you mostly want to think of a pixel as some combination of red, green,
 * blue, and maybe alpha intensities, this is all pretty straightforward, and in
 * many cases, is enough information to build a perfectly fine game.
 *
 * However, the actual definition of a pixel is more complex than that:
 *
 * Pixels are a representation of a color in a particular color space.
 *
 * The first characteristic of a color space is the color type. SDL understands
 * two different color types, RGB and YCbCr, or in SDL also referred to as YUV.
 *
 * RGB colors consist of red, green, and blue channels of color that are added
 * together to represent the colors we see on the screen.
 *
 * https://en.wikipedia.org/wiki/RGB_color_model
 *
 * YCbCr colors represent colors as a Y luma brightness component and red and
 * blue chroma color offsets. This color representation takes advantage of the
 * fact that the human eye is more sensitive to brightness than the color in an
 * image. The Cb and Cr components are often compressed and have lower
 * resolution than the luma component.
 *
 * https://en.wikipedia.org/wiki/YCbCr
 *
 * When the color information in YCbCr is compressed, the Y pixels are left at
 * full resolution and each Cr and Cb pixel represents an average of the color
 * information in a block of Y pixels. The chroma location determines where in
 * that block of pixels the color information is coming from.
 *
 * The color range defines how much of the pixel to use when converting a pixel
 * into a color on the display. When the full color range is used, the entire
 * numeric range of the pixel bits is significant. When narrow color range is
 * used, for historical reasons, the pixel uses only a portion of the numeric
 * range to represent colors.
 *
 * The color primaries and white point are a definition of the colors in the
 * color space relative to the standard XYZ color space.
 *
 * https://en.wikipedia.org/wiki/CIE_1931_color_space
 *
 * The transfer characteristic, or opto-electrical transfer function (OETF), is
 * the way a color is converted from mathematically linear space into a
 * non-linear output signals.
 *
 * https://en.wikipedia.org/wiki/Rec._709#Transfer_characteristics
 *
 * The matrix coefficients are used to convert between YCbCr and RGB colors.
 *
 * @{
 */

/// Alias to raw representation for PixelFormat.
using PixelFormatRaw = SDL_PixelFormat;

// Forward decl
struct PixelFormat;

/// Alias to raw representation for Colorspace.
using ColorspaceRaw = SDL_Colorspace;

// Forward decl
struct Colorspace;

/// Alias to raw representation for Color.
using ColorRaw = SDL_Color;

// Forward decl
struct Color;

/// Alias to raw representation for FColor.
using FColorRaw = SDL_FColor;

// Forward decl
struct FColor;

// Forward decl
struct Palette;

/// Alias to raw representation for Palette.
using PaletteRaw = SDL_Palette*;

// Forward decl
struct PaletteRef;

/// Safely wrap Palette for non owning parameters
struct PaletteParam
{
  PaletteRaw value; ///< parameter's PaletteRaw

  /// Constructs from PaletteRaw
  constexpr PaletteParam(PaletteRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr PaletteParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const PaletteParam& other) const = default;

  /// Converts to underlying PaletteRaw
  constexpr operator PaletteRaw() const { return value; }

  /// member access to underlying PaletteRaw.
  constexpr auto operator->() { return value; }
};

/// Safely wrap Palette for non owning const parameters
struct PaletteConstParam
{
  const PaletteRaw value; ///< parameter's const PaletteRaw

  /// Constructs from const PaletteRaw
  constexpr PaletteConstParam(const PaletteRaw value)
    : value(value)
  {
  }

  /// Constructs from PaletteParam
  constexpr PaletteConstParam(PaletteParam value)
    : value(value.value)
  {
  }

  /// Constructs null/invalid
  constexpr PaletteConstParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const PaletteConstParam& other) const = default;

  /// Converts to underlying const PaletteRaw
  constexpr operator const PaletteRaw() const { return value; }

  /// member access to underlying PaletteRaw.
  constexpr auto operator->() { return value; }
};

/**
 * Details about the format of a pixel.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PixelFormatDetails = SDL_PixelFormatDetails;

/**
 * A fully opaque 8-bit alpha value.
 *
 * @since This constant is available since SDL 3.2.0.
 *
 * @sa ALPHA_TRANSPARENT
 */
constexpr Uint8 ALPHA_OPAQUE = SDL_ALPHA_OPAQUE;

/**
 * A fully opaque floating point alpha value.
 *
 * @since This constant is available since SDL 3.2.0.
 *
 * @sa ALPHA_TRANSPARENT_FLOAT
 */
constexpr float ALPHA_OPAQUE_FLOAT = SDL_ALPHA_OPAQUE_FLOAT;

/**
 * A fully transparent 8-bit alpha value.
 *
 * @since This constant is available since SDL 3.2.0.
 *
 * @sa ALPHA_OPAQUE
 */
constexpr Uint8 ALPHA_TRANSPARENT = SDL_ALPHA_TRANSPARENT;

/**
 * A fully transparent floating point alpha value.
 *
 * @since This constant is available since SDL 3.2.0.
 *
 * @sa ALPHA_OPAQUE_FLOAT
 */
constexpr float ALPHA_TRANSPARENT_FLOAT = SDL_ALPHA_TRANSPARENT_FLOAT;

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

/**
 * Bitmap pixel order, high bit -> low bit.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using BitmapOrder = SDL_BitmapOrder;

constexpr BitmapOrder BITMAPORDER_NONE = SDL_BITMAPORDER_NONE; ///< NONE

constexpr BitmapOrder BITMAPORDER_4321 = SDL_BITMAPORDER_4321; ///< 4321

constexpr BitmapOrder BITMAPORDER_1234 = SDL_BITMAPORDER_1234; ///< 1234

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

/**
 * Pixel format.
 *
 * SDL's pixel formats have the following naming convention:
 *
 * - Names with a list of components and a single bit count, such as RGB24 and
 *   ABGR32, define a platform-independent encoding into bytes in the order
 *   specified. For example, in RGB24 data, each pixel is encoded in 3 bytes
 *   (red, green, blue) in that order, and in ABGR32 data, each pixel is encoded
 *   in 4 bytes (alpha, blue, green, red) in that order. Use these names if the
 *   property of a format that is important to you is the order of the bytes in
 *   memory or on disk.
 * - Names with a bit count per component, such as ARGB8888 and XRGB1555, are
 *   "packed" into an appropriately-sized integer in the platform's native
 *   endianness. For example, ARGB8888 is a sequence of 32-bit integers; in each
 *   integer, the most significant bits are alpha, and the least significant
 *   bits are blue. On a little-endian CPU such as x86, the least significant
 *   bits of each integer are arranged first in memory, but on a big-endian CPU
 *   such as s390x, the most significant bits are arranged first. Use these
 *   names if the property of a format that is important to you is the meaning
 *   of each bit position within a native-endianness integer.
 * - In indexed formats such as INDEX4LSB, each pixel is represented by encoding
 *   an index into the palette into the indicated number of bits, with multiple
 *   pixels packed into each byte if appropriate. In LSB formats, the first
 *   (leftmost) pixel is stored in the least-significant bits of the byte; in
 *   MSB formats, it's stored in the most-significant bits. INDEX8 does not need
 *   LSB/MSB variants, because each pixel exactly fills one byte.
 *
 * The 32-bit byte-array encodings such as RGBA32 are aliases for the
 * appropriate 8888 encoding for the current platform. For example, RGBA32 is an
 * alias for ABGR8888 on little-endian CPUs like x86, or an alias for RGBA8888
 * on big-endian CPUs.
 *
 * @since This enum is available since SDL 3.2.0.
 */
class PixelFormat
{
  PixelFormatRaw m_format;

public:
  /**
   * Wraps PixelFormat.
   *
   * @param format the value to be wrapped
   */
  constexpr PixelFormat(PixelFormatRaw format = {}) noexcept
    : m_format(format)
  {
  }

  /**
   * A macro for defining custom non-FourCC pixel formats.
   *
   * For example, defining PIXELFORMAT_RGBA8888 looks like this:
   *
   * ```c
   * PixelFormat.PixelFormat(PIXELTYPE_PACKED32, PACKEDORDER_RGBA,
   * PACKEDLAYOUT_8888, 32, 4)
   * ```
   *
   * @param type the type of the new format, probably a PixelType value.
   * @param order the order of the new format, probably a BitmapOrder,
   *              PackedOrder, or ArrayOrder value.
   * @param layout the layout of the new format, probably an PackedLayout value
   *               or zero.
   * @param bits the number of bits per pixel of the new format.
   * @param bytes the number of bytes per pixel of the new format.
   * @post a format value in the style of PixelFormat.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr PixelFormat(PixelType type,
                        int order,
                        PackedLayout layout,
                        int bits,
                        int bytes)
    : m_format(SDL_DEFINE_PIXELFORMAT(type, order, layout, bits, bytes))
  {
  }

  /**
   * Unwraps to the underlying PixelFormat.
   *
   * @returns the underlying PixelFormatRaw.
   */
  constexpr operator PixelFormatRaw() const noexcept { return m_format; }

  /**
   * Convert a bpp value and RGBA masks to an enumerated pixel format.
   *
   * This will return `PIXELFORMAT_UNKNOWN` if the conversion wasn't possible.
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
                              Uint32 Amask);

  /**
   * A macro to retrieve the flags of an PixelFormat.
   *
   * This macro is generally not needed directly by an app, which should use
   * specific tests, like PixelFormat.IsFourCC, instead.
   *
   * @returns the flags of `format`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr Uint8 GetFlags() const;

  /**
   * A macro to retrieve the type of an PixelFormat.
   *
   * This is usually a value from the PixelType enumeration.
   *
   * @returns the type of `format`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr PixelType GetType() const;

  /**
   * A macro to retrieve the order of an PixelFormat.
   *
   * This is usually a value from the BitmapOrder, PackedOrder, or ArrayOrder
   * enumerations, depending on the format type.
   *
   * @returns the order of `format`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr int GetOrder() const;

  /**
   * A macro to retrieve the layout of an PixelFormat.
   *
   * This is usually a value from the PackedLayout enumeration, or zero if a
   * layout doesn't make sense for the format type.
   *
   * @returns the layout of `format`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr PackedLayout GetLayout() const;

  /**
   * A macro to determine an PixelFormat's bits per pixel.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * FourCC formats will report zero here, as it rarely makes sense to measure
   * them per-pixel.
   *
   * @returns the bits-per-pixel of `format`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PixelFormat.GetBytesPerPixel
   */
  constexpr int GetBitsPerPixel() const;

  /**
   * A macro to determine an PixelFormat's bytes per pixel.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * FourCC formats do their best here, but many of them don't have a meaningful
   * measurement of bytes per pixel.
   *
   * @returns the bytes-per-pixel of `format`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PixelFormat.GetBitsPerPixel
   */
  constexpr int GetBytesPerPixel() const;

  /**
   * A macro to determine if an PixelFormat is an indexed format.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if the format is indexed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsIndexed() const;

  /**
   * A macro to determine if an PixelFormat is a packed format.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if the format is packed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsPacked() const;

  /**
   * A macro to determine if an PixelFormat is an array format.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if the format is an array, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsArray() const;

  /**
   * A macro to determine if an PixelFormat is a 10-bit format.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if the format is 10-bit, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Is10Bit() const;

  /**
   * A macro to determine if an PixelFormat is a floating point format.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if the format is a floating point, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFloat() const;

  /**
   * A macro to determine if an PixelFormat has an alpha channel.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsAlpha() const;

  /**
   * A macro to determine if an PixelFormat is a "FourCC" format.
   *
   * This covers custom and other unusual formats.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFourCC() const;

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
  const char* GetName() const;

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
                Uint32* Amask) const;

  /**
   * Create an PixelFormatDetails structure corresponding to a pixel format.
   *
   * Returned structure may come from a shared global cache (i.e. not newly
   * allocated), and hence should not be modified, especially the palette. Weird
   * errors such as `Blit combination not supported` may occur.
   *
   * @returns a pointer to a PixelFormatDetails structure or nullptr on failure;
   *          call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const PixelFormatDetails& GetDetails() const;

  /// Same as GetDetails()
  operator const PixelFormatDetails&() const;

  Uint32 Map(ColorRaw c, PaletteConstParam palette = {}) const;

  Color Get(Uint32 pixel, PaletteConstParam palette = {}) const;
};

constexpr PixelFormat PIXELFORMAT_UNKNOWN =
  SDL_PIXELFORMAT_UNKNOWN; ///< UNKNOWN

constexpr PixelFormat PIXELFORMAT_INDEX1LSB =
  SDL_PIXELFORMAT_INDEX1LSB; ///< INDEX1LSB

constexpr PixelFormat PIXELFORMAT_INDEX1MSB =
  SDL_PIXELFORMAT_INDEX1MSB; ///< INDEX1MSB

constexpr PixelFormat PIXELFORMAT_INDEX2LSB =
  SDL_PIXELFORMAT_INDEX2LSB; ///< INDEX2LSB

constexpr PixelFormat PIXELFORMAT_INDEX2MSB =
  SDL_PIXELFORMAT_INDEX2MSB; ///< INDEX2MSB

constexpr PixelFormat PIXELFORMAT_INDEX4LSB =
  SDL_PIXELFORMAT_INDEX4LSB; ///< INDEX4LSB

constexpr PixelFormat PIXELFORMAT_INDEX4MSB =
  SDL_PIXELFORMAT_INDEX4MSB; ///< INDEX4MSB

constexpr PixelFormat PIXELFORMAT_INDEX8 = SDL_PIXELFORMAT_INDEX8; ///< INDEX8

constexpr PixelFormat PIXELFORMAT_RGB332 = SDL_PIXELFORMAT_RGB332; ///< RGB332

constexpr PixelFormat PIXELFORMAT_XRGB4444 =
  SDL_PIXELFORMAT_XRGB4444; ///< XRGB4444

constexpr PixelFormat PIXELFORMAT_XBGR4444 =
  SDL_PIXELFORMAT_XBGR4444; ///< XBGR4444

constexpr PixelFormat PIXELFORMAT_XRGB1555 =
  SDL_PIXELFORMAT_XRGB1555; ///< XRGB1555

constexpr PixelFormat PIXELFORMAT_XBGR1555 =
  SDL_PIXELFORMAT_XBGR1555; ///< XBGR1555

constexpr PixelFormat PIXELFORMAT_ARGB4444 =
  SDL_PIXELFORMAT_ARGB4444; ///< ARGB4444

constexpr PixelFormat PIXELFORMAT_RGBA4444 =
  SDL_PIXELFORMAT_RGBA4444; ///< RGBA4444

constexpr PixelFormat PIXELFORMAT_ABGR4444 =
  SDL_PIXELFORMAT_ABGR4444; ///< ABGR4444

constexpr PixelFormat PIXELFORMAT_BGRA4444 =
  SDL_PIXELFORMAT_BGRA4444; ///< BGRA4444

constexpr PixelFormat PIXELFORMAT_ARGB1555 =
  SDL_PIXELFORMAT_ARGB1555; ///< ARGB1555

constexpr PixelFormat PIXELFORMAT_RGBA5551 =
  SDL_PIXELFORMAT_RGBA5551; ///< RGBA5551

constexpr PixelFormat PIXELFORMAT_ABGR1555 =
  SDL_PIXELFORMAT_ABGR1555; ///< ABGR1555

constexpr PixelFormat PIXELFORMAT_BGRA5551 =
  SDL_PIXELFORMAT_BGRA5551; ///< BGRA5551

constexpr PixelFormat PIXELFORMAT_RGB565 = SDL_PIXELFORMAT_RGB565; ///< RGB565

constexpr PixelFormat PIXELFORMAT_BGR565 = SDL_PIXELFORMAT_BGR565; ///< BGR565

constexpr PixelFormat PIXELFORMAT_RGB24 = SDL_PIXELFORMAT_RGB24; ///< RGB24

constexpr PixelFormat PIXELFORMAT_BGR24 = SDL_PIXELFORMAT_BGR24; ///< BGR24

constexpr PixelFormat PIXELFORMAT_XRGB8888 =
  SDL_PIXELFORMAT_XRGB8888; ///< XRGB8888

constexpr PixelFormat PIXELFORMAT_RGBX8888 =
  SDL_PIXELFORMAT_RGBX8888; ///< RGBX8888

constexpr PixelFormat PIXELFORMAT_XBGR8888 =
  SDL_PIXELFORMAT_XBGR8888; ///< XBGR8888

constexpr PixelFormat PIXELFORMAT_BGRX8888 =
  SDL_PIXELFORMAT_BGRX8888; ///< BGRX8888

constexpr PixelFormat PIXELFORMAT_ARGB8888 =
  SDL_PIXELFORMAT_ARGB8888; ///< ARGB8888

constexpr PixelFormat PIXELFORMAT_RGBA8888 =
  SDL_PIXELFORMAT_RGBA8888; ///< RGBA8888

constexpr PixelFormat PIXELFORMAT_ABGR8888 =
  SDL_PIXELFORMAT_ABGR8888; ///< ABGR8888

constexpr PixelFormat PIXELFORMAT_BGRA8888 =
  SDL_PIXELFORMAT_BGRA8888; ///< BGRA8888

constexpr PixelFormat PIXELFORMAT_XRGB2101010 =
  SDL_PIXELFORMAT_XRGB2101010; ///< XRGB2101010

constexpr PixelFormat PIXELFORMAT_XBGR2101010 =
  SDL_PIXELFORMAT_XBGR2101010; ///< XBGR2101010

constexpr PixelFormat PIXELFORMAT_ARGB2101010 =
  SDL_PIXELFORMAT_ARGB2101010; ///< ARGB2101010

constexpr PixelFormat PIXELFORMAT_ABGR2101010 =
  SDL_PIXELFORMAT_ABGR2101010; ///< ABGR2101010

constexpr PixelFormat PIXELFORMAT_RGB48 = SDL_PIXELFORMAT_RGB48; ///< RGB48

constexpr PixelFormat PIXELFORMAT_BGR48 = SDL_PIXELFORMAT_BGR48; ///< BGR48

constexpr PixelFormat PIXELFORMAT_RGBA64 = SDL_PIXELFORMAT_RGBA64; ///< RGBA64

constexpr PixelFormat PIXELFORMAT_ARGB64 = SDL_PIXELFORMAT_ARGB64; ///< ARGB64

constexpr PixelFormat PIXELFORMAT_BGRA64 = SDL_PIXELFORMAT_BGRA64; ///< BGRA64

constexpr PixelFormat PIXELFORMAT_ABGR64 = SDL_PIXELFORMAT_ABGR64; ///< ABGR64

constexpr PixelFormat PIXELFORMAT_RGB48_FLOAT =
  SDL_PIXELFORMAT_RGB48_FLOAT; ///< RGB48_FLOAT

constexpr PixelFormat PIXELFORMAT_BGR48_FLOAT =
  SDL_PIXELFORMAT_BGR48_FLOAT; ///< BGR48_FLOAT

constexpr PixelFormat PIXELFORMAT_RGBA64_FLOAT =
  SDL_PIXELFORMAT_RGBA64_FLOAT; ///< RGBA64_FLOAT

constexpr PixelFormat PIXELFORMAT_ARGB64_FLOAT =
  SDL_PIXELFORMAT_ARGB64_FLOAT; ///< ARGB64_FLOAT

constexpr PixelFormat PIXELFORMAT_BGRA64_FLOAT =
  SDL_PIXELFORMAT_BGRA64_FLOAT; ///< BGRA64_FLOAT

constexpr PixelFormat PIXELFORMAT_ABGR64_FLOAT =
  SDL_PIXELFORMAT_ABGR64_FLOAT; ///< ABGR64_FLOAT

constexpr PixelFormat PIXELFORMAT_RGB96_FLOAT =
  SDL_PIXELFORMAT_RGB96_FLOAT; ///< RGB96_FLOAT

constexpr PixelFormat PIXELFORMAT_BGR96_FLOAT =
  SDL_PIXELFORMAT_BGR96_FLOAT; ///< BGR96_FLOAT

constexpr PixelFormat PIXELFORMAT_RGBA128_FLOAT =
  SDL_PIXELFORMAT_RGBA128_FLOAT; ///< RGBA128_FLOAT

constexpr PixelFormat PIXELFORMAT_ARGB128_FLOAT =
  SDL_PIXELFORMAT_ARGB128_FLOAT; ///< ARGB128_FLOAT

constexpr PixelFormat PIXELFORMAT_BGRA128_FLOAT =
  SDL_PIXELFORMAT_BGRA128_FLOAT; ///< BGRA128_FLOAT

constexpr PixelFormat PIXELFORMAT_ABGR128_FLOAT =
  SDL_PIXELFORMAT_ABGR128_FLOAT; ///< ABGR128_FLOAT

constexpr PixelFormat PIXELFORMAT_YV12 =
  SDL_PIXELFORMAT_YV12; ///< Planar mode: Y + V + U (3 planes)

constexpr PixelFormat PIXELFORMAT_IYUV =
  SDL_PIXELFORMAT_IYUV; ///< Planar mode: Y + U + V (3 planes)

constexpr PixelFormat PIXELFORMAT_YUY2 =
  SDL_PIXELFORMAT_YUY2; ///< Packed mode: Y0+U0+Y1+V0 (1 plane)

constexpr PixelFormat PIXELFORMAT_UYVY =
  SDL_PIXELFORMAT_UYVY; ///< Packed mode: U0+Y0+V0+Y1 (1 plane)

constexpr PixelFormat PIXELFORMAT_YVYU =
  SDL_PIXELFORMAT_YVYU; ///< Packed mode: Y0+V0+Y1+U0 (1 plane)

constexpr PixelFormat PIXELFORMAT_NV12 =
  SDL_PIXELFORMAT_NV12; ///< Planar mode: Y + U/V interleaved (2 planes)

constexpr PixelFormat PIXELFORMAT_NV21 =
  SDL_PIXELFORMAT_NV21; ///< Planar mode: Y + V/U interleaved (2 planes)

constexpr PixelFormat PIXELFORMAT_P010 =
  SDL_PIXELFORMAT_P010; ///< Planar mode: Y + U/V interleaved (2 planes)

constexpr PixelFormat PIXELFORMAT_EXTERNAL_OES =
  SDL_PIXELFORMAT_EXTERNAL_OES; ///< Android video texture format

#if SDL_VERSION_ATLEAST(3, 2, 10)

constexpr PixelFormat PIXELFORMAT_MJPG = SDL_PIXELFORMAT_MJPG; ///< Motion JPEG

#endif // SDL_VERSION_ATLEAST(3, 2, 10)

constexpr PixelFormat PIXELFORMAT_RGBA32 = SDL_PIXELFORMAT_RGBA32; ///< RGBA32

constexpr PixelFormat PIXELFORMAT_ARGB32 = SDL_PIXELFORMAT_ARGB32; ///< ARGB32

constexpr PixelFormat PIXELFORMAT_BGRA32 = SDL_PIXELFORMAT_BGRA32; ///< BGRA32

constexpr PixelFormat PIXELFORMAT_ABGR32 = SDL_PIXELFORMAT_ABGR32; ///< ABGR32

constexpr PixelFormat PIXELFORMAT_RGBX32 = SDL_PIXELFORMAT_RGBX32; ///< RGBX32

constexpr PixelFormat PIXELFORMAT_XRGB32 = SDL_PIXELFORMAT_XRGB32; ///< XRGB32

constexpr PixelFormat PIXELFORMAT_BGRX32 = SDL_PIXELFORMAT_BGRX32; ///< BGRX32

constexpr PixelFormat PIXELFORMAT_XBGR32 = SDL_PIXELFORMAT_XBGR32; ///< XBGR32

/**
 * A macro for defining custom FourCC pixel formats.
 *
 * For example, defining PIXELFORMAT_YV12 looks like this:
 *
 * ```c
 * DefinePixelFourCC('Y', 'V', '1', '2')
 * ```
 *
 * @param A the first character of the FourCC code.
 * @param B the second character of the FourCC code.
 * @param C the third character of the FourCC code.
 * @param D the fourth character of the FourCC code.
 * @returns a format value in the style of PixelFormat.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint32 DefinePixelFourCC(Uint8 A, Uint8 B, Uint8 C, Uint8 D)
{
  return SDL_DEFINE_PIXELFOURCC(A, B, C, D);
}

/**
 * A macro for defining custom non-FourCC pixel formats.
 *
 * For example, defining PIXELFORMAT_RGBA8888 looks like this:
 *
 * ```c
 * PixelFormat.PixelFormat(PIXELTYPE_PACKED32, PACKEDORDER_RGBA,
 * PACKEDLAYOUT_8888, 32, 4)
 * ```
 *
 * @param type the type of the new format, probably a PixelType value.
 * @param order the order of the new format, probably a BitmapOrder,
 *              PackedOrder, or ArrayOrder value.
 * @param layout the layout of the new format, probably an PackedLayout value or
 *               zero.
 * @param bits the number of bits per pixel of the new format.
 * @param bytes the number of bytes per pixel of the new format.
 * @returns a format value in the style of PixelFormat.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr PixelFormat DefinePixelFormat(PixelType type,
                                        int order,
                                        PackedLayout layout,
                                        int bits,
                                        int bytes)
{
  return PixelFormat(type, order, layout, bits, bytes);
}

/**
 * A macro to retrieve the flags of an PixelFormat.
 *
 * This macro is generally not needed directly by an app, which should use
 * specific tests, like PixelFormat.IsFourCC, instead.
 *
 * @param format an PixelFormat to check.
 * @returns the flags of `format`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint8 PixelFormatFlags(PixelFormatRaw format)
{
  return SDL_PIXELFLAG(format);
}

constexpr Uint8 PixelFormat::GetFlags() const
{
  return SDL::PixelFormatFlags(m_format);
}

/**
 * A macro to retrieve the type of an PixelFormat.
 *
 * This is usually a value from the PixelType enumeration.
 *
 * @param format an PixelFormat to check.
 * @returns the type of `format`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr PixelType PixelFormatType(PixelFormatRaw format)
{
  return SDL_PIXELTYPE(format);
}

constexpr PixelType PixelFormat::GetType() const
{
  return SDL::PixelFormatType(m_format);
}

/**
 * A macro to retrieve the order of an PixelFormat.
 *
 * This is usually a value from the BitmapOrder, PackedOrder, or ArrayOrder
 * enumerations, depending on the format type.
 *
 * @param format an PixelFormat to check.
 * @returns the order of `format`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr int PixelFormatOrder(PixelFormatRaw format)
{
  return SDL_PIXELORDER(format);
}

constexpr int PixelFormat::GetOrder() const
{
  return SDL::PixelFormatOrder(m_format);
}

/**
 * A macro to retrieve the layout of an PixelFormat.
 *
 * This is usually a value from the PackedLayout enumeration, or zero if a
 * layout doesn't make sense for the format type.
 *
 * @param format an PixelFormat to check.
 * @returns the layout of `format`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr PackedLayout PixelFormatLayout(PixelFormatRaw format)
{
  return SDL_PIXELLAYOUT(format);
}

constexpr PackedLayout PixelFormat::GetLayout() const
{
  return SDL::PixelFormatLayout(m_format);
}

/**
 * A macro to determine an PixelFormat's bits per pixel.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * FourCC formats will report zero here, as it rarely makes sense to measure
 * them per-pixel.
 *
 * @param format an PixelFormat to check.
 * @returns the bits-per-pixel of `format`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetBytesPerPixel
 */
constexpr int PixelFormatBitsPerPixel(PixelFormatRaw format)
{
  return SDL_BITSPERPIXEL(format);
}

constexpr int PixelFormat::GetBitsPerPixel() const
{
  return SDL::PixelFormatBitsPerPixel(m_format);
}

/**
 * A macro to determine an PixelFormat's bytes per pixel.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * FourCC formats do their best here, but many of them don't have a meaningful
 * measurement of bytes per pixel.
 *
 * @param format an PixelFormat to check.
 * @returns the bytes-per-pixel of `format`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetBitsPerPixel
 */
constexpr int PixelFormatBytesPerPixel(PixelFormatRaw format)
{
  return SDL_BYTESPERPIXEL(format);
}

constexpr int PixelFormat::GetBytesPerPixel() const
{
  return SDL::PixelFormatBytesPerPixel(m_format);
}

/**
 * A macro to determine if an PixelFormat is an indexed format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is indexed, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsPixelFormatIndexed(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_INDEXED(format);
}

constexpr bool PixelFormat::IsIndexed() const
{
  return SDL::IsPixelFormatIndexed(m_format);
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
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsPixelFormatPacked(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_PACKED(format);
}

constexpr bool PixelFormat::IsPacked() const
{
  return SDL::IsPixelFormatPacked(m_format);
}

/**
 * A macro to determine if an PixelFormat is an array format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is an array, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsPixelFormatArray(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_ARRAY(format);
}

constexpr bool PixelFormat::IsArray() const
{
  return SDL::IsPixelFormatArray(m_format);
}

/**
 * A macro to determine if an PixelFormat is a 10-bit format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is 10-bit, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsPixelFormat10Bit(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_10BIT(format);
}

constexpr bool PixelFormat::Is10Bit() const
{
  return SDL::IsPixelFormat10Bit(m_format);
}

/**
 * A macro to determine if an PixelFormat is a floating point format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is a floating point, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsPixelFormatFloat(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_FLOAT(format);
}

constexpr bool PixelFormat::IsFloat() const
{
  return SDL::IsPixelFormatFloat(m_format);
}

/**
 * A macro to determine if an PixelFormat has an alpha channel.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format has alpha, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsPixelFormatAlpha(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_ALPHA(format);
}

constexpr bool PixelFormat::IsAlpha() const
{
  return SDL::IsPixelFormatAlpha(m_format);
}

/**
 * A macro to determine if an PixelFormat is a "FourCC" format.
 *
 * This covers custom and other unusual formats.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format has alpha, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsPixelFormatFourCC(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_FOURCC(format);
}

constexpr bool PixelFormat::IsFourCC() const
{
  return SDL::IsPixelFormatFourCC(m_format);
}

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

/// Full range, e.g. 0-255 for 8-bit RGB and luma, and 1-255 for 8-bit chroma
constexpr ColorRange COLOR_RANGE_FULL = SDL_COLOR_RANGE_FULL;

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
  SDL_COLOR_PRIMARIES_BT709; ///< ITU-R BT.709-6

constexpr ColorPrimaries COLOR_PRIMARIES_UNSPECIFIED =
  SDL_COLOR_PRIMARIES_UNSPECIFIED; ///< COLOR_PRIMARIES_UNSPECIFIED

constexpr ColorPrimaries COLOR_PRIMARIES_BT470M =
  SDL_COLOR_PRIMARIES_BT470M; ///< ITU-R BT.470-6 System M

constexpr ColorPrimaries COLOR_PRIMARIES_BT470BG =
  SDL_COLOR_PRIMARIES_BT470BG; ///< ITU-R BT.470-6 System B, G / ITU-R BT.601-7
                               ///< 625

constexpr ColorPrimaries COLOR_PRIMARIES_BT601 =
  SDL_COLOR_PRIMARIES_BT601; ///< ITU-R BT.601-7 525, SMPTE 170M

/// SMPTE 240M, functionally the same as COLOR_PRIMARIES_BT601
constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE240 =
  SDL_COLOR_PRIMARIES_SMPTE240;

constexpr ColorPrimaries COLOR_PRIMARIES_GENERIC_FILM =
  SDL_COLOR_PRIMARIES_GENERIC_FILM; ///< Generic film (color filters using
                                    ///< Illuminant C)

constexpr ColorPrimaries COLOR_PRIMARIES_BT2020 =
  SDL_COLOR_PRIMARIES_BT2020; ///< ITU-R BT.2020-2 / ITU-R BT.2100-0

constexpr ColorPrimaries COLOR_PRIMARIES_XYZ =
  SDL_COLOR_PRIMARIES_XYZ; ///< SMPTE ST 428-1

constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE431 =
  SDL_COLOR_PRIMARIES_SMPTE431; ///< SMPTE RP 431-2

constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE432 =
  SDL_COLOR_PRIMARIES_SMPTE432; ///< SMPTE EG 432-1 / DCI P3

constexpr ColorPrimaries COLOR_PRIMARIES_EBU3213 =
  SDL_COLOR_PRIMARIES_EBU3213; ///< EBU Tech. 3213-E

constexpr ColorPrimaries COLOR_PRIMARIES_CUSTOM =
  SDL_COLOR_PRIMARIES_CUSTOM; ///< COLOR_PRIMARIES_CUSTOM

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
  SDL_TRANSFER_CHARACTERISTICS_BT709; ///< Rec. ITU-R BT.709-6 / ITU-R BT1361

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_UNSPECIFIED =
  SDL_TRANSFER_CHARACTERISTICS_UNSPECIFIED; ///< TRANSFER_CHARACTERISTICS_UNSPECIFIED

/// ITU-R BT.470-6 System M / ITU-R BT1700 625 PAL & SECAM
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA22 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA22;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA28 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA28; ///< ITU-R BT.470-6 System B, G

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT601 =
  SDL_TRANSFER_CHARACTERISTICS_BT601; ///< SMPTE ST 170M / ITU-R BT.601-7 525 or
                                      ///< 625

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE240 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE240; ///< SMPTE ST 240M

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LINEAR =
  SDL_TRANSFER_CHARACTERISTICS_LINEAR; ///< TRANSFER_CHARACTERISTICS_LINEAR

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100; ///< TRANSFER_CHARACTERISTICS_LOG100

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100_SQRT10 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100_SQRT10; ///< TRANSFER_CHARACTERISTICS_LOG100_SQRT10

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_IEC61966 =
  SDL_TRANSFER_CHARACTERISTICS_IEC61966; ///< IEC 61966-2-4

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT1361 =
  SDL_TRANSFER_CHARACTERISTICS_BT1361; ///< ITU-R BT1361 Extended Colour Gamut

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SRGB =
  SDL_TRANSFER_CHARACTERISTICS_SRGB; ///< IEC 61966-2-1 (sRGB or sYCC)

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_10BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_10BIT; ///< ITU-R BT2020 for 10-bit system

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_12BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_12BIT; ///< ITU-R BT2020 for 12-bit system

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_PQ =
  SDL_TRANSFER_CHARACTERISTICS_PQ; ///< SMPTE ST 2084 for 10-, 12-, 14- and
                                   ///< 16-bit systems

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE428 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE428; ///< SMPTE ST 428-1

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_HLG =
  SDL_TRANSFER_CHARACTERISTICS_HLG; ///< ARIB STD-B67, known as "hybrid
                                    ///< log-gamma" (HLG)

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_CUSTOM =
  SDL_TRANSFER_CHARACTERISTICS_CUSTOM; ///< TRANSFER_CHARACTERISTICS_CUSTOM

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
  SDL_MATRIX_COEFFICIENTS_BT709; ///< ITU-R BT.709-6

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_UNSPECIFIED =
  SDL_MATRIX_COEFFICIENTS_UNSPECIFIED; ///< MATRIX_COEFFICIENTS_UNSPECIFIED

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_FCC =
  SDL_MATRIX_COEFFICIENTS_FCC; ///< US FCC Title 47

/**
 * ITU-R BT.470-6 System B, G / ITU-R BT.601-7 625, functionally the same as
 * MATRIX_COEFFICIENTS_BT601
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT470BG =
  SDL_MATRIX_COEFFICIENTS_BT470BG;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT601 =
  SDL_MATRIX_COEFFICIENTS_BT601; ///< ITU-R BT.601-7 525

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE240 =
  SDL_MATRIX_COEFFICIENTS_SMPTE240; ///< SMPTE 240M

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_YCGCO =
  SDL_MATRIX_COEFFICIENTS_YCGCO; ///< MATRIX_COEFFICIENTS_YCGCO

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_NCL =
  SDL_MATRIX_COEFFICIENTS_BT2020_NCL; ///< ITU-R BT.2020-2 non-constant
                                      ///< luminance

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_CL =
  SDL_MATRIX_COEFFICIENTS_BT2020_CL; ///< ITU-R BT.2020-2 constant luminance

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE2085 =
  SDL_MATRIX_COEFFICIENTS_SMPTE2085; ///< SMPTE ST 2085

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL; ///< MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL; ///< MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_ICTCP =
  SDL_MATRIX_COEFFICIENTS_ICTCP; ///< ITU-R BT.2100-0 ICTCP

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CUSTOM =
  SDL_MATRIX_COEFFICIENTS_CUSTOM; ///< MATRIX_COEFFICIENTS_CUSTOM

/**
 * Colorspace chroma sample location.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ChromaLocation = SDL_ChromaLocation;

constexpr ChromaLocation CHROMA_LOCATION_NONE =
  SDL_CHROMA_LOCATION_NONE; ///< RGB, no chroma sampling

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

/**
 * Colorspace definitions.
 *
 * Since similar colorspaces may vary in their details (matrix, transfer
 * function, etc.), this is not an exhaustive list, but rather a representative
 * sample of the kinds of colorspaces supported in SDL.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa ColorPrimaries
 * @sa ColorRange
 * @sa ColorType
 * @sa MatrixCoefficients
 * @sa TransferCharacteristics
 */
class Colorspace
{
  ColorspaceRaw m_cspace;

public:
  /**
   * Wraps Colorspace.
   *
   * @param cspace the value to be wrapped
   */
  constexpr Colorspace(ColorspaceRaw cspace = {}) noexcept
    : m_cspace(cspace)
  {
  }

  /**
   * A macro for defining custom Colorspace formats.
   *
   * For example, defining COLORSPACE_SRGB looks like this:
   *
   * ```c
   * Colorspace.Colorspace(COLOR_TYPE_RGB,
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
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr Colorspace(ColorType type,
                       ColorRange range,
                       ColorPrimaries primaries,
                       TransferCharacteristics transfer,
                       MatrixCoefficients matrix,
                       ChromaLocation chroma)
    : m_cspace(
        SDL_DEFINE_COLORSPACE(type, range, primaries, transfer, matrix, chroma))
  {
  }

  /**
   * Unwraps to the underlying Colorspace.
   *
   * @returns the underlying ColorspaceRaw.
   */
  constexpr operator ColorspaceRaw() const noexcept { return m_cspace; }

  /**
   * A macro to retrieve the type of an Colorspace.
   *
   * @returns the ColorType for `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ColorType GetType() const;

  /**
   * A macro to retrieve the range of an Colorspace.
   *
   * @returns the ColorRange of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ColorRange GetRange() const;

  /**
   * A macro to retrieve the chroma sample location of an Colorspace.
   *
   * @returns the ChromaLocation of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ChromaLocation GetChroma() const;

  /**
   * A macro to retrieve the primaries of an Colorspace.
   *
   * @returns the ColorPrimaries of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ColorPrimaries GetPrimaries() const;

  /**
   * A macro to retrieve the transfer characteristics of an Colorspace.
   *
   * @returns the TransferCharacteristics of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr TransferCharacteristics GetTransfer() const;

  /**
   * A macro to retrieve the matrix coefficients of an Colorspace.
   *
   * @returns the MatrixCoefficients of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr MatrixCoefficients GetMatrix() const;

  /**
   * A macro to determine if an Colorspace uses BT601 (or BT470BG) matrix
   * coefficients.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if BT601 or BT470BG, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT601() const;

  /**
   * A macro to determine if an Colorspace uses BT709 matrix coefficients.
   *
   * @returns true if BT709, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT709() const;

  /**
   * A macro to determine if an Colorspace uses BT2020_NCL matrix coefficients.
   *
   * @returns true if BT2020_NCL, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT2020_NCL() const;

  /**
   * A macro to determine if an Colorspace has a limited range.
   *
   * @returns true if limited range, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsLimitedRange() const;

  /**
   * A macro to determine if an Colorspace has a full range.
   *
   * @returns true if full range, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFullRange() const;
};

constexpr Colorspace COLORSPACE_UNKNOWN = SDL_COLORSPACE_UNKNOWN; ///< UNKNOWN

/// Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709
constexpr Colorspace COLORSPACE_SRGB = SDL_COLORSPACE_SRGB;

/// Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709
constexpr Colorspace COLORSPACE_SRGB_LINEAR = SDL_COLORSPACE_SRGB_LINEAR;

/// Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020
constexpr Colorspace COLORSPACE_HDR10 = SDL_COLORSPACE_HDR10;

/// Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_NONE_P709_X601
constexpr Colorspace COLORSPACE_JPEG = SDL_COLORSPACE_JPEG;

/// Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601
constexpr Colorspace COLORSPACE_BT601_LIMITED = SDL_COLORSPACE_BT601_LIMITED;

/// Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601
constexpr Colorspace COLORSPACE_BT601_FULL = SDL_COLORSPACE_BT601_FULL;

/// Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709
constexpr Colorspace COLORSPACE_BT709_LIMITED = SDL_COLORSPACE_BT709_LIMITED;

/// Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709
constexpr Colorspace COLORSPACE_BT709_FULL = SDL_COLORSPACE_BT709_FULL;

/// Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P2020
constexpr Colorspace COLORSPACE_BT2020_LIMITED = SDL_COLORSPACE_BT2020_LIMITED;

/// Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P2020
constexpr Colorspace COLORSPACE_BT2020_FULL = SDL_COLORSPACE_BT2020_FULL;

/// The default colorspace for RGB surfaces if no colorspace is specified
constexpr Colorspace COLORSPACE_RGB_DEFAULT = SDL_COLORSPACE_RGB_DEFAULT;

/// The default colorspace for YUV surfaces if no colorspace is specified
constexpr Colorspace COLORSPACE_YUV_DEFAULT = SDL_COLORSPACE_YUV_DEFAULT;

/**
 * A macro for defining custom Colorspace formats.
 *
 * For example, defining COLORSPACE_SRGB looks like this:
 *
 * ```c
 * Colorspace.Colorspace(COLOR_TYPE_RGB,
 *                       COLOR_RANGE_FULL,
 *                       COLOR_PRIMARIES_BT709,
 *                       TRANSFER_CHARACTERISTICS_SRGB,
 *                       MATRIX_COEFFICIENTS_IDENTITY,
 *                       CHROMA_LOCATION_NONE)
 * ```
 *
 * @param type the type of the new format, probably an ColorType value.
 * @param range the range of the new format, probably a ColorRange value.
 * @param primaries the primaries of the new format, probably an ColorPrimaries
 *                  value.
 * @param transfer the transfer characteristics of the new format, probably an
 *                 TransferCharacteristics value.
 * @param matrix the matrix coefficients of the new format, probably an
 *               MatrixCoefficients value.
 * @param chroma the chroma sample location of the new format, probably an
 *               ChromaLocation value.
 * @returns a format value in the style of Colorspace.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Colorspace DefineColorspace(ColorType type,
                                      ColorRange range,
                                      ColorPrimaries primaries,
                                      TransferCharacteristics transfer,
                                      MatrixCoefficients matrix,
                                      ChromaLocation chroma)
{
  return Colorspace(type, range, primaries, transfer, matrix, chroma);
}

/**
 * A macro to retrieve the type of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the ColorType for `cspace`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr ColorType ColorspaceType(ColorspaceRaw cspace)
{
  return SDL_COLORSPACETYPE(cspace);
}

constexpr ColorType Colorspace::GetType() const
{
  return SDL::ColorspaceType(m_cspace);
}

/**
 * A macro to retrieve the range of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the ColorRange of `cspace`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr ColorRange ColorspaceRange(ColorspaceRaw cspace)
{
  return SDL_COLORSPACERANGE(cspace);
}

constexpr ColorRange Colorspace::GetRange() const
{
  return SDL::ColorspaceRange(m_cspace);
}

/**
 * A macro to retrieve the chroma sample location of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the ChromaLocation of `cspace`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr ChromaLocation ColorspaceChroma(ColorspaceRaw cspace)
{
  return SDL_COLORSPACECHROMA(cspace);
}

constexpr ChromaLocation Colorspace::GetChroma() const
{
  return SDL::ColorspaceChroma(m_cspace);
}

/**
 * A macro to retrieve the primaries of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the ColorPrimaries of `cspace`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr ColorPrimaries ColorspacePrimaries(ColorspaceRaw cspace)
{
  return SDL_COLORSPACEPRIMARIES(cspace);
}

constexpr ColorPrimaries Colorspace::GetPrimaries() const
{
  return SDL::ColorspacePrimaries(m_cspace);
}

/**
 * A macro to retrieve the transfer characteristics of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the TransferCharacteristics of `cspace`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr TransferCharacteristics ColorspaceTransfer(ColorspaceRaw cspace)
{
  return SDL_COLORSPACETRANSFER(cspace);
}

constexpr TransferCharacteristics Colorspace::GetTransfer() const
{
  return SDL::ColorspaceTransfer(m_cspace);
}

/**
 * A macro to retrieve the matrix coefficients of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the MatrixCoefficients of `cspace`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr MatrixCoefficients ColorspaceMatrix(ColorspaceRaw cspace)
{
  return SDL_COLORSPACEMATRIX(cspace);
}

constexpr MatrixCoefficients Colorspace::GetMatrix() const
{
  return SDL::ColorspaceMatrix(m_cspace);
}

/**
 * A macro to determine if an Colorspace uses BT601 (or BT470BG) matrix
 * coefficients.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param cspace an Colorspace to check.
 * @returns true if BT601 or BT470BG, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsColorspaceMatrixBT601(ColorspaceRaw cspace)
{
  return SDL_ISCOLORSPACE_MATRIX_BT601(cspace);
}

constexpr bool Colorspace::IsMatrixBT601() const
{
  return SDL::IsColorspaceMatrixBT601(m_cspace);
}

/**
 * A macro to determine if an Colorspace uses BT709 matrix coefficients.
 *
 * @param cspace an Colorspace to check.
 * @returns true if BT709, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsColorspaceMatrixBT709(ColorspaceRaw cspace)
{
  return SDL_ISCOLORSPACE_MATRIX_BT709(cspace);
}

constexpr bool Colorspace::IsMatrixBT709() const
{
  return SDL::IsColorspaceMatrixBT709(m_cspace);
}

/**
 * A macro to determine if an Colorspace uses BT2020_NCL matrix coefficients.
 *
 * @param cspace an Colorspace to check.
 * @returns true if BT2020_NCL, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsColorspaceMatrixBT2020_NCL(ColorspaceRaw cspace)
{
  return SDL_ISCOLORSPACE_MATRIX_BT2020_NCL(cspace);
}

constexpr bool Colorspace::IsMatrixBT2020_NCL() const
{
  return SDL::IsColorspaceMatrixBT2020_NCL(m_cspace);
}

/**
 * A macro to determine if an Colorspace has a limited range.
 *
 * @param cspace an Colorspace to check.
 * @returns true if limited range, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsColorspaceLimitedRange(ColorspaceRaw cspace)
{
  return SDL_ISCOLORSPACE_LIMITED_RANGE(cspace);
}

constexpr bool Colorspace::IsLimitedRange() const
{
  return SDL::IsColorspaceLimitedRange(m_cspace);
}

/**
 * A macro to determine if an Colorspace has a full range.
 *
 * @param cspace an Colorspace to check.
 * @returns true if full range, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsColorspaceFullRange(ColorspaceRaw cspace)
{
  return SDL_ISCOLORSPACE_FULL_RANGE(cspace);
}

constexpr bool Colorspace::IsFullRange() const
{
  return SDL::IsColorspaceFullRange(m_cspace);
}

/// Comparison operator for Color.
constexpr bool operator==(ColorRaw lhs, ColorRaw rhs) noexcept
{
  return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

/// Comparison operator for FColor.
constexpr bool operator==(const FColorRaw& lhs, const FColorRaw& rhs) noexcept
{
  return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

/// Spaceship operator for Color.
constexpr auto operator<=>(ColorRaw lhs, ColorRaw rhs) noexcept
{
  if (lhs.r != rhs.r) return lhs.r <=> rhs.r;
  if (lhs.g != rhs.g) return lhs.g <=> rhs.g;
  if (lhs.b != rhs.b) return lhs.b <=> rhs.b;
  return lhs.a <=> rhs.a;
}

/// Spaceship operator for FColor.
constexpr auto operator<=>(const FColorRaw& lhs, const FColorRaw& rhs) noexcept
{
  if (lhs.r != rhs.r) return lhs.r <=> rhs.r;
  if (lhs.g != rhs.g) return lhs.g <=> rhs.g;
  if (lhs.b != rhs.b) return lhs.b <=> rhs.b;
  return lhs.a <=> rhs.a;
}

/**
 * A structure that represents a color as RGBA components.
 *
 * The bits of this structure can be directly reinterpreted as an integer-packed
 * color which uses the PIXELFORMAT_RGBA32 format (PIXELFORMAT_ABGR8888 on
 * little-endian systems and PIXELFORMAT_RGBA8888 on big-endian systems).
 *
 * @since This struct is available since SDL 3.2.0.
 */
struct Color : ColorRaw
{
  /**
   * Wraps Color.
   *
   * @param color the value to be wrapped
   */
  constexpr Color(ColorRaw color = {}) noexcept
    : ColorRaw(color)
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
  constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) noexcept
    : ColorRaw{r, g, b, a}
  {
  }

  /**
   * Get the r.
   *
   * @returns current r value.
   */
  constexpr Uint8 GetR() const noexcept { return r; }

  /**
   * Set the r.
   *
   * @param newR the new r value.
   * @returns Reference to self.
   */
  constexpr Color& SetR(Uint8 newR) noexcept
  {
    r = newR;
    return *this;
  }

  /**
   * Get the g.
   *
   * @returns current g value.
   */
  constexpr Uint8 GetG() const noexcept { return g; }

  /**
   * Set the g.
   *
   * @param newG the new g value.
   * @returns Reference to self.
   */
  constexpr Color& SetG(Uint8 newG) noexcept
  {
    g = newG;
    return *this;
  }

  /**
   * Get the b.
   *
   * @returns current b value.
   */
  constexpr Uint8 GetB() const noexcept { return b; }

  /**
   * Set the b.
   *
   * @param newB the new b value.
   * @returns Reference to self.
   */
  constexpr Color& SetB(Uint8 newB) noexcept
  {
    b = newB;
    return *this;
  }

  /**
   * Get the a.
   *
   * @returns current a value.
   */
  constexpr Uint8 GetA() const noexcept { return a; }

  /**
   * Set the a.
   *
   * @param newA the new a value.
   * @returns Reference to self.
   */
  constexpr Color& SetA(Uint8 newA) noexcept
  {
    a = newA;
    return *this;
  }

  Uint32 Map(const PixelFormatDetails& format,
             PaletteConstParam palette = {}) const;

  static Color Get(Uint32 pixel,
                   const PixelFormatDetails& format,
                   PaletteConstParam palette = {});
};

/**
 * The bits of this structure can be directly reinterpreted as a float-packed
 * color which uses the PIXELFORMAT_RGBA128_FLOAT format
 *
 * @since This struct is available since SDL 3.2.0.
 */
struct FColor : FColorRaw
{
  /**
   * Wraps FColor.
   *
   * @param color the value to be wrapped
   */
  constexpr FColor(const FColorRaw& color = {}) noexcept
    : FColorRaw(color)
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
  constexpr FColor(float r, float g, float b, float a) noexcept
    : FColorRaw{r, g, b, a}
  {
  }

  /**
   * Get the r.
   *
   * @returns current r value.
   */
  constexpr float GetR() const noexcept { return r; }

  /**
   * Set the r.
   *
   * @param newR the new r value.
   * @returns Reference to self.
   */
  constexpr FColor& SetR(float newR) noexcept
  {
    r = newR;
    return *this;
  }

  /**
   * Get the g.
   *
   * @returns current g value.
   */
  constexpr float GetG() const noexcept { return g; }

  /**
   * Set the g.
   *
   * @param newG the new g value.
   * @returns Reference to self.
   */
  constexpr FColor& SetG(float newG) noexcept
  {
    g = newG;
    return *this;
  }

  /**
   * Get the b.
   *
   * @returns current b value.
   */
  constexpr float GetB() const noexcept { return b; }

  /**
   * Set the b.
   *
   * @param newB the new b value.
   * @returns Reference to self.
   */
  constexpr FColor& SetB(float newB) noexcept
  {
    b = newB;
    return *this;
  }

  /**
   * Get the a.
   *
   * @returns current a value.
   */
  constexpr float GetA() const noexcept { return a; }

  /**
   * Set the a.
   *
   * @param newA the new a value.
   * @returns Reference to self.
   */
  constexpr FColor& SetA(float newA) noexcept
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
  PaletteRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Palette(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from PaletteParam.
   *
   * @param resource a PaletteRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Palette(const PaletteRaw resource) noexcept
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Palette(const Palette& other)
    : m_resource(other.m_resource)
  {
    ++m_resource->refcount;
  }

  /// Move constructor
  constexpr Palette(Palette&& other) noexcept
    : Palette(other.release())
  {
  }

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
   * @sa Surface.SetPalette
   */
  Palette(int ncolors)
    : m_resource(CheckError(SDL_CreatePalette(ncolors)))
  {
  }

  /**
   * Safely borrows the from PaletteParam.
   *
   * @param resource a PaletteRaw or Palette.
   *
   * This does not takes ownership!
   */
  static constexpr Palette Borrow(PaletteParam resource)
  {
    if (resource) {
      ++resource.value->refcount;
      return Palette(resource.value);
    }
    return {};
  }

  /// member access to underlying PaletteRaw.
  constexpr const PaletteRaw operator->() const noexcept { return m_resource; }

  /// member access to underlying PaletteRaw.
  constexpr PaletteRaw operator->() noexcept { return m_resource; }

  /// Destructor
  ~Palette() { SDL_DestroyPalette(m_resource); }

  /// Assignment operator.
  constexpr Palette& operator=(Palette&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Assignment operator.
  constexpr Palette& operator=(const Palette& other) noexcept = default;

  /// Retrieves underlying PaletteRaw.
  constexpr PaletteRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying PaletteRaw and clear this.
  constexpr PaletteRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Palette& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to PaletteParam
  constexpr operator PaletteParam() const noexcept { return {m_resource}; }

  /**
   * Free a palette created with Palette.Palette().
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.Palette
   */
  void Destroy();

  constexpr const ColorRaw* data() const
  {
    static_assert(false, "Not implemented");
  }

  constexpr int size() const { static_assert(false, "Not implemented"); }

  constexpr ColorRaw operator[](int index) const
  {
    static_assert(false, "Not implemented");
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
  void SetColors(SpanRef<const ColorRaw> colors, int firstcolor = 0);
};

/// Safe reference for Palette.
struct PaletteRef : Palette
{
  using Palette::Palette;

  /**
   * Constructs from PaletteRaw.
   *
   * @param resource a PaletteRaw.
   *
   * This borrows the ownership, increments the refcount!
   */
  PaletteRef(PaletteRaw resource) noexcept
    : Palette(Borrow(resource))
  {
  }

  /// Constructs from Palette.
  PaletteRef(Palette resource) noexcept
    : Palette(std::move(resource))
  {
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
inline const char* GetPixelFormatName(PixelFormatRaw format)
{
  return SDL_GetPixelFormatName(format);
}

inline const char* PixelFormat::GetName() const
{
  return SDL::GetPixelFormatName(m_format);
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
inline void GetMasksForPixelFormat(PixelFormatRaw format,
                                   int* bpp,
                                   Uint32* Rmask,
                                   Uint32* Gmask,
                                   Uint32* Bmask,
                                   Uint32* Amask)
{
  CheckError(
    SDL_GetMasksForPixelFormat(format, bpp, Rmask, Gmask, Bmask, Amask));
}

inline void PixelFormat::GetMasks(int* bpp,
                                  Uint32* Rmask,
                                  Uint32* Gmask,
                                  Uint32* Bmask,
                                  Uint32* Amask) const
{
  SDL::GetMasksForPixelFormat(m_format, bpp, Rmask, Gmask, Bmask, Amask);
}

/**
 * Convert a bpp value and RGBA masks to an enumerated pixel format.
 *
 * This will return `PIXELFORMAT_UNKNOWN` if the conversion wasn't possible.
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

inline PixelFormat PixelFormat::ForMasks(int bpp,
                                         Uint32 Rmask,
                                         Uint32 Gmask,
                                         Uint32 Bmask,
                                         Uint32 Amask)
{
  return SDL::GetPixelFormatForMasks(bpp, Rmask, Gmask, Bmask, Amask);
}

/**
 * Create an PixelFormatDetails structure corresponding to a pixel format.
 *
 * Returned structure may come from a shared global cache (i.e. not newly
 * allocated), and hence should not be modified, especially the palette. Weird
 * errors such as `Blit combination not supported` may occur.
 *
 * @param format one of the PixelFormat values.
 * @returns a pointer to a PixelFormatDetails structure or nullptr on failure;
 *          call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const PixelFormatDetails& GetPixelFormatDetails(PixelFormatRaw format)
{
  return CheckError(SDL_GetPixelFormatDetails(format));
}

inline const PixelFormatDetails& PixelFormat::GetDetails() const
{
  return SDL::GetPixelFormatDetails(m_format);
}

inline PixelFormat::operator const PixelFormatDetails&() const
{
  return GetDetails();
}

/**
 * Create a palette structure with the specified number of color entries.
 *
 * The palette entries are initialized to white.
 *
 * @param ncolors represents the number of color entries in the color palette.
 * @returns a new Palette structure on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Palette.Destroy
 * @sa Palette.SetColors
 * @sa Surface.SetPalette
 */
inline Palette CreatePalette(int ncolors) { return Palette(ncolors); }

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
inline void SetPaletteColors(PaletteParam palette,
                             SpanRef<const ColorRaw> colors,
                             int firstcolor = 0)
{
  CheckError(SDL_SetPaletteColors(palette, colors, firstcolor));
}

inline void Palette::SetColors(SpanRef<const ColorRaw> colors, int firstcolor)
{
  SDL::SetPaletteColors(m_resource, colors, firstcolor);
}

/**
 * Free a palette created with Palette.Palette().
 *
 * @param palette the Palette structure to be freed.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified or destroyed in another thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Palette.Palette
 */
inline void DestroyPalette(PaletteRaw palette) { SDL_DestroyPalette(palette); }

inline void Palette::Destroy() { DestroyPalette(release()); }

/**
 * Map an RGB triple to an opaque pixel value for a given pixel format.
 *
 * This function maps the RGB color value to the specified pixel format and
 * returns the pixel value best approximating the given RGB color value for the
 * given pixel format.
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
 * @param format a pointer to PixelFormatDetails describing the pixel format.
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
 * @sa MapColor
 * @sa Surface.MapRGB
 */
inline Uint32 MapRGB(const PixelFormatDetails& format,
                     PaletteConstParam palette,
                     Uint8 r,
                     Uint8 g,
                     Uint8 b)
{
  return SDL_MapRGB(&format, palette, r, g, b);
}

inline Uint32 MapRGBA(const PixelFormatDetails& format,
                      Uint8 r,
                      Uint8 g,
                      Uint8 b,
                      Uint8 a,
                      PaletteConstParam palette = {})
{
  static_assert(false, "Not implemented");
}

/**
 * Map an RGBA quadruple to a pixel value for a given pixel format.
 *
 * This function maps the RGBA color value to the specified pixel format and
 * returns the pixel value best approximating the given RGBA color value for the
 * given pixel format.
 *
 * If the specified pixel format has no alpha component the alpha value will be
 * ignored (as it will be in formats with a palette).
 *
 * If the format has a palette (8-bit) the index of the closest matching color
 * in the palette will be returned.
 *
 * If the pixel format bpp (color depth) is less than 32-bpp then the unused
 * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
 * format the return value can be assigned to a Uint16, and similarly a Uint8
 * for an 8-bpp format).
 *
 * @param format a pointer to PixelFormatDetails describing the pixel format.
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
 * @sa Surface.MapRGBA
 */
inline Uint32 MapColor(const PixelFormatDetails& format,
                       ColorRaw c,
                       PaletteConstParam palette = {})
{
  return SDL_MapRGBA(&format, c, palette);
}

inline Uint32 Color::Map(const PixelFormatDetails& format,
                         PaletteConstParam palette) const
{
  static_assert(false, "Not implemented");
}

inline Uint32 PixelFormat::Map(ColorRaw c, PaletteConstParam palette) const
{
  static_assert(false, "Not implemented");
}

/**
 * Get RGB values from a pixel in the specified format.
 *
 * This function uses the entire 8-bit [0..255] range when converting color
 * components from pixel formats with less than 8-bits per RGB component (e.g.,
 * a completely white pixel in 16-bit RGB565 format would return [0xff, 0xff,
 * 0xff] not [0xf8, 0xfc, 0xf8]).
 *
 * @param pixelvalue a pixel value.
 * @param format a pointer to PixelFormatDetails describing the pixel format.
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
 * @sa MapColor
 */
inline void GetRGB(Uint32 pixelvalue,
                   const PixelFormatDetails& format,
                   PaletteConstParam palette,
                   Uint8* r,
                   Uint8* g,
                   Uint8* b)
{
  SDL_GetRGB(pixelvalue, &format, palette, r, g, b);
}

/**
 * Get RGBA values from a pixel in the specified format.
 *
 * This function uses the entire 8-bit [0..255] range when converting color
 * components from pixel formats with less than 8-bits per RGB component (e.g.,
 * a completely white pixel in 16-bit RGB565 format would return [0xff, 0xff,
 * 0xff] not [0xf8, 0xfc, 0xf8]).
 *
 * If the surface has no alpha component, the alpha will be returned as 0xff
 * (100% opaque).
 *
 * @param pixelvalue a pixel value.
 * @param format a pointer to PixelFormatDetails describing the pixel format.
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
 * @sa MapColor
 */
inline void GetRGBA(Uint32 pixelvalue,
                    const PixelFormatDetails& format,
                    PaletteConstParam palette,
                    Uint8* r,
                    Uint8* g,
                    Uint8* b,
                    Uint8* a)
{
  SDL_GetRGBA(pixelvalue, &format, palette, r, g, b, a);
}

inline Color GetColor(Uint32 pixel,
                      const PixelFormatDetails& format,
                      PaletteConstParam palette = {})
{
  static_assert(false, "Not implemented");
}

inline Color Color::Get(Uint32 pixel,
                        const PixelFormatDetails& format,
                        PaletteConstParam palette)
{
  static_assert(false, "Not implemented");
}

inline Color PixelFormat::Get(Uint32 pixel, PaletteConstParam palette) const
{
  static_assert(false, "Not implemented");
}

/// @}

} // namespace SDL

#endif /* SDL3PP_PIXELS_H_ */
