#ifndef SDL3PP_PIXELS_HPP_
#define SDL3PP_PIXELS_HPP_

#include <span>
#include <SDL3/SDL_pixels.h>
#include "objectWrapper.hpp"

namespace SDL {

/**
 * @brief Pixel type.
 *
 */
using PixelType = SDL_PixelType;

/**
 * @brief Bitmap pixel order, high bit -> low bit.
 *
 */
using BitmapOrder = SDL_BitmapOrder;

/**
 * @brief Packed component order, high bit -> low bit.
 *
 */
using PackedOrder = SDL_PackedOrder;

/**
 * @brief Array component order, low byte -> high byte.
 *
 */
using ArrayOrder = SDL_ArrayOrder;

/**
 * @brief Packed component layout.
 */
using PackedLayout = SDL_PackedLayout;

/**
 * @brief Details about the format of a pixel.
 */
using PixelFormatDetails = SDL_PixelFormatDetails;

/**
 * @brief Pixel format.
 */
struct PixelFormat
{
  SDL_PixelFormat format;

  constexpr operator bool() const { return format != SDL_PIXELFORMAT_UNKNOWN; }

  constexpr operator SDL_PixelFormat() const { return format; }

  /// @brief Retrieve the type.
  constexpr PixelType Type() const { return PixelType(SDL_PIXELTYPE(format)); }

  /**
   * @brief Retrieve the order.
   *
   * This is usually a value from the BitmapOrder, PackedOrder, or ArrayOrder
   * enumerations, depending on the format type.
   *
   * @returns the order.
   */
  constexpr int Order() const { return SDL_PIXELORDER(format); }

  /// @brief Retrieve the layout.
  constexpr PackedLayout Layout() const
  {
    return PackedLayout(SDL_PIXELLAYOUT(format));
  }

  /**
   * @brief Determine this's bits per pixel.
   *
   * FourCC formats will report zero here, as it rarely makes sense to measure
   * them per-pixel.
   *
   * @return the bits-per-pixel.
   */
  constexpr int BitsPerPixel() const { return SDL_BITSPERPIXEL(format); }

  /**
   * @brief Determine this's bytes per pixel.
   *
   * FourCC formats do their best here, but many of them don't have a
   * meaningful measurement of bytes per pixel.
   *
   * @return the bytes-per-pixel.
   */
  constexpr int BytesPerPixel() const { return SDL_BITSPERPIXEL(format); }

  /// @brief Determine if this is an indexed format.
  constexpr bool IsIndexed() const { return SDL_ISPIXELFORMAT_INDEXED(format); }

  /// @brief Determine if this is a packed format.
  constexpr bool IsPacked() const { return SDL_ISPIXELFORMAT_PACKED(format); }

  /// @brief Determine if this is an array format.
  constexpr bool IsArray() const { return SDL_ISPIXELFORMAT_ARRAY(format); }

  /// @brief Determine if this is a 10-bit format.
  constexpr bool Is10Bit() const { return SDL_ISPIXELFORMAT_10BIT(format); }

  /// @brief Determine if this is a floating point format.
  constexpr bool IsFloat() const { return SDL_ISPIXELFORMAT_FLOAT(format); }

  /// @brief Determine if this is an alpha channel.
  constexpr bool IsAlpha() const { return SDL_ISPIXELFORMAT_ALPHA(format); }

  /**
   * @brief Determine if this is a "FourCC" format.
   *
   * This covers custom and other unusual formats.
   *
   * @return true
   * @return false
   */
  constexpr bool IsFourCC() const { return SDL_ISPIXELFORMAT_ALPHA(format); }

  /**
   * @brief Get the human readable name of a pixel format.
   *
   * @returns the human readable name of the specified pixel format or
   *          "SDL_PIXELFORMAT_UNKNOWN" if the format isn't recognized.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  const char* GetName() const { return SDL_GetPixelFormatName(format); }

  /**
   * @brief Convert one of the enumerated pixel formats to a bpp value and RGBA
   * masks.
   *
   * @param bpp[out] a bits per pixel value; usually 15, 16, or 32.
   * @param Rmask[out] a pointer filled in with the red mask for the format.
   * @param Gmask[out] a pointer filled in with the green mask for the format.
   * @param Bmask[out] a pointer filled in with the blue mask for the format.
   * @param Amask[out] a pointer filled in with the alpha mask for the format.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool GetMasks(int* bpp,
                Uint32* Rmask,
                Uint32* Gmask,
                Uint32* Bmask,
                Uint32* Amask) const
  {
    return SDL_GetMasksForPixelFormat(format, bpp, Rmask, Gmask, Bmask, Amask);
  }

  /**
   * @brief Convert a bpp value and RGBA masks to an enumerated pixel format.
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
   */
  static PixelFormat FromMask(int bpp,
                              Uint32 Rmask,
                              Uint32 Gmask,
                              Uint32 Bmask,
                              Uint32 Amask)
  {
    return {SDL_GetPixelFormatForMasks(bpp, Rmask, Gmask, Bmask, Amask)};
  }

  /**
   * @brief Create an SDL_PixelFormatDetails structure corresponding to a pixel
   * format.
   *
   * Returned structure may come from a shared global cache (i.e. not newly
   * allocated), and hence should not be modified, especially the palette. Weird
   * errors such as `Blit combination not supported` may occur.
   *
   * @returns a pointer to a SDL_PixelFormatDetails structure or NULL on
   *          failure; call SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  const PixelFormatDetails* GetDetails() const
  {
    return SDL_GetPixelFormatDetails(format);
  }
};

/**
 * @brief Pixel format static attributes.
 *
 * @tparam FORMAT the specific format to query:
 *
 * ```cpp
 * int a = Format<SDL_PIXELFORMAT_RGBA32>.BytesPerPixel();
 * ```
 */
template<SDL_PixelFormat FORMAT>
constexpr const PixelFormat Format{FORMAT};

/**
 * @brief Colorspace color type.
 *
 */
using ColorType = SDL_ColorType;

/**
 * @brief Colorspace color range, as described by
 * https://www.itu.int/rec/R-REC-BT.2100-2-201807-I/en
 *
 */
using ColorRange = SDL_ColorRange;

/**
 * @brief Colorspace color primaries, as described by
 * https://www.itu.int/rec/T-REC-H.273-201612-S/en
 *
 */
using ColorPrimaries = SDL_ColorPrimaries;

/**
 * @brief Colorspace transfer characteristics.
 *
 * These are as described by https://www.itu.int/rec/T-REC-H.273-201612-S/en
 *
 */
using TransferCharacteristics = SDL_TransferCharacteristics;

/**
 * @brief Colorspace matrix coefficients.
 *
 * These are as described by https://www.itu.int/rec/T-REC-H.273-201612-S/en
 *
 */
using MatrixCoefficients = SDL_MatrixCoefficients;

// TODO Wrap SDL_COLORSPACE macros
/**
 * @brief Colorspace definitions.
 *
 */
using Colorspace = SDL_Colorspace;
// TODO Wrap Colorspace like PixelFormat

// Forward decl
template<class T>
struct PaletteConstBase;

// Forward decl
template<class T>
struct PaletteBase;

/**
 * @brief Handle to a non owned surface
 */
using PaletteWrapper = PaletteBase<ObjectWrapper<SDL_Palette>>;

/**
 * @brief Handle to a non owned const surface
 *
 * The constness implies only methods that don't change its internal state are
 * allowed.
 */
using PaletteConstWrapper = PaletteBase<ObjectWrapper<const SDL_Palette>>;

/**
 * @brief A structure that represents a color as RGBA components.
 *
 * The bits of this structure can be directly reinterpreted as an
 * integer-packed color which uses the SDL_PIXELFORMAT_RGBA32 format
 * (SDL_PIXELFORMAT_ABGR8888 on little-endian systems and
 * SDL_PIXELFORMAT_RGBA8888 on big-endian systems).
 *
 */
struct Color : SDL_Color
{
  constexpr Color(SDL_Color color)
    : SDL_Color(color)
  {
  }

  constexpr Color()
    : Color({0})
  {
  }

  constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
    : SDL_Color{r, g, b, a}
  {
  }

  // Auto comparison operator
  auto operator<=>(const Color&) const = default;

  /**
   * @brief Get the red component from the color
   *
   * @returns The red component from the color
   *
   */
  constexpr Uint8 GetRed() const { return r; }

  /**
   * @brief Set the red component from the color
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
   * @brief Get the green component from the color
   *
   * @returns The green component from the color
   *
   */
  constexpr Uint8 GetGreen() const { return g; }

  /**
   * @brief Set the green component from the color
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
   * @brief Get the blue component from the color
   *
   * @returns The blue component from the color
   *
   */
  constexpr Uint8 GetBlue() const { return b; }

  /**
   * @brief Set the blue component from the color
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
   * @brief Get the alpha component from the color
   *
   * @returns The alpha component from the color
   *
   */
  constexpr Uint8 GetAlpha() const { return a; }

  /**
   * @brief Set the alpha component from the color
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

  // TODO Get() Map()
};

/**
 * @brief The bits of this structure can be directly reinterpreted as a
 * float-packed color which uses the SDL_PIXELFORMAT_RGBA128_FLOAT format
 *
 */
struct FColor : SDL_FColor
{
  constexpr FColor(SDL_FColor color)
    : SDL_FColor(color)
  {
  }

  constexpr FColor()
    : FColor({0})
  {
  }

  constexpr FColor(float r, float g, float b, float a)
    : SDL_FColor{r, g, b, a}
  {
  }

  // Auto comparison operator
  auto operator<=>(const FColor&) const = default;

  /**
   * @brief Get the red component from the color
   *
   * @returns The red component from the color
   *
   */
  constexpr float GetRed() const { return r; }

  /**
   * @brief Set the red component from the color
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
   * @brief Get the green component from the color
   *
   * @returns The green component from the color
   *
   */
  constexpr float GetGreen() const { return g; }

  /**
   * @brief Set the green component from the color
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
   * @brief Get the blue component from the color
   *
   * @returns The blue component from the color
   *
   */
  constexpr float GetBlue() const { return b; }

  /**
   * @brief Set the blue component from the color
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
   * @brief Get the alpha component from the color
   *
   * @returns The alpha component from the color
   *
   */
  constexpr float GetAlpha() const { return a; }

  /**
   * @brief Set the alpha component from the color
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

template<>
struct ObjectDeleter<SDL_Palette>
{
  void operator()(SDL_Palette* palette) { SDL_DestroyPalette(palette); }
};

/**
 * @brief Handle to an owned surface
 */
using Palette = PaletteBase<ObjectUnique<SDL_Palette>>;
using PaletteUnique = Palette;

/**
 * @brief Represents a handle to a const palette
 */
template<class T>
struct PaletteConstBase : T
{
  // Make default ctors available
  using T::T;

  int GetSize() const { return this->ncolors; }

  // TODO Bounds check
  Color Map(int index) const { return this->colors[index]; }
  Color operator[](int index) const { return this->colors[index]; }

  // TODO SDL_MapRGB()
};

/**
 * @brief Represents a handle to a palette
 */
template<class T>
struct PaletteBase : PaletteConstBase<T>
{
  // Make default ctors available
  using PaletteConstBase<T>::PaletteConstBase;

  /**
   * @brief Create a palette structure with the specified number of color
   * entries.
   *
   * The palette entries are initialized to white.
   *
   * @param nColors represents the number of color entries in the color palette.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  PaletteBase(int nColors)
    : PaletteConstBase<T>(SDL_CreatePalette(nColors))
  {
  }

  /**
   * @brief Set a range of colors in a palette.
   *
   * @param colors an array of SDL_Color structures to copy into the palette.
   * @param firstColor the index of the first palette entry to modify.
   * @param nColors the number of entries to modify.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   */
  bool SetColors(const SDL_Color* colors, int firstColor, int nColors)
  {
    return SDL_SetPaletteColors(Get<T>(this), colors, firstColor, nColors);
  }

  /**
   * @brief Set a range of colors in a palette.
   *
   * @param colors a span of SDL_Color structures to copy into the palette.
   * @param firstColor the index of the first palette entry to modify.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   */
  bool SetColors(std::span<const SDL_Color> colors, int firstColor)
  {
    return SetColors(colors.data(), firstColor, colors.size());
  }

  // TODO GetColor/ MapColor()
};

// TODO SDL_PixelFormatDetails

} // namespace SDL

#endif /* SDL3PP_PIXELS_HPP_ */
