#ifndef SDL3PP_TTF_H_
#define SDL3PP_TTF_H_

#include "SDL3pp_error.h"
#include "SDL3pp_init.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_render.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_surface.h"
#include "SDL3pp_version.h"

#if !defined(SDL3PP_DISABLE_TTF) && !defined(SDL3PP_ENABLE_TTF) &&             \
  __has_include(<SDL3_ttf/SDL_ttf.h>)
#define SDL3PP_ENABLE_TTF
#endif

#if defined(SDL3PP_ENABLE_TTF) || defined(SDL3PP_DOC)

#include <SDL3_ttf/SDL_ttf.h>

namespace SDL {

/**
 * @defgroup CategorySDLTTF True type font support
 *
 * Header file for SDL_ttf library
 *
 * This library is a wrapper around the excellent FreeType 2.0 library,
 * available at: https://www.freetype.org/
 *
 * @{
 */

/**
 * Flag to init TTF
 *
 */
constexpr struct TtfInitFlag : InitFlagsExtra
{
} INIT_TTF; ///< Flag to init TTF support

/// Deleter for TextEngine
using TextEngineDeleter = void (*)(TTF_TextEngine*);

// Forward decl
struct FontRef;

// Forward decl
struct Font;

// Forward decl
struct TextEngineRef;

// Forward decl
struct TextEngine;

// Forward decl
struct TextRef;

// Forward decl
struct Text;

/**
 * @name Font Style Flags
 * @{
 */

/**
 * Font style flags for FontRef
 *
 * These are the flags which can be used to set the style of a font in
 * SDL_ttf. A combination of these flags can be used with functions that set
 * or query font style, such as FontRef.SetStyle or FontRef.GetStyle.
 *
 * @since This datatype is available since SDL_ttf 3.0.0.
 *
 * @sa FontRef.SetStyle
 * @sa FontRef.GetStyle
 */
using FontStyleFlags = Uint32;

constexpr FontStyleFlags STYLE_NORMAL = TTF_STYLE_NORMAL; ///< No special style

constexpr FontStyleFlags STYLE_BOLD = TTF_STYLE_BOLD; ///< Bold style

constexpr FontStyleFlags STYLE_ITALIC = TTF_STYLE_ITALIC; ///< Italic style

constexpr FontStyleFlags STYLE_UNDERLINE =
  TTF_STYLE_UNDERLINE; ///< Underlined text

constexpr FontStyleFlags STYLE_STRIKETHROUGH =
  TTF_STYLE_STRIKETHROUGH; ///< Strikethrough text

/// @}

/**
 * @name Hinting Flags
 * @{
 */

/**
 * Hinting flags for TTF (TrueType Fonts)
 *
 * This enum specifies the level of hinting to be applied to the font
 * rendering. The hinting level determines how much the font's outlines are
 * adjusted for better alignment on the pixel grid.
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 *
 * @sa FontRef.SetHinting
 * @sa FontRef.GetHinting
 */
using HintingFlags = TTF_HintingFlags;

constexpr HintingFlags HINTING_INVALID = TTF_HINTING_INVALID; ///< INVALID

constexpr HintingFlags HINTING_NORMAL =
  TTF_HINTING_NORMAL; ///< Normal hinting applies standard grid-fitting.

/**
 * Light hinting applies subtle adjustments to improve rendering.
 */
constexpr HintingFlags HINTING_LIGHT = TTF_HINTING_LIGHT;

/**
 * Monochrome hinting adjusts the font for better rendering at lower
 * resolutions.
 */
constexpr HintingFlags HINTING_MONO = TTF_HINTING_MONO;

/**
 * No hinting, the font is rendered without any grid-fitting.
 */
constexpr HintingFlags HINTING_NONE = TTF_HINTING_NONE;

/**
 * Light hinting with subpixel rendering for more precise font edges.
 */
constexpr HintingFlags HINTING_LIGHT_SUBPIXEL = TTF_HINTING_LIGHT_SUBPIXEL;

/// @}

/**
 * @name HorizontalAlignments
 * @{
 */

/**
 * The horizontal alignment used when rendering wrapped text.
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 */
using HorizontalAlignment = TTF_HorizontalAlignment;

constexpr HorizontalAlignment HORIZONTAL_ALIGN_INVALID =
  TTF_HORIZONTAL_ALIGN_INVALID; ///< INVALID

constexpr HorizontalAlignment HORIZONTAL_ALIGN_LEFT =
  TTF_HORIZONTAL_ALIGN_LEFT; ///< LEFT

constexpr HorizontalAlignment HORIZONTAL_ALIGN_CENTER =
  TTF_HORIZONTAL_ALIGN_CENTER; ///< CENTER

constexpr HorizontalAlignment HORIZONTAL_ALIGN_RIGHT =
  TTF_HORIZONTAL_ALIGN_RIGHT; ///< RIGHT

/// @}

/**
 * @name Direction Flags
 * @{
 */

/**
 * Direction flags
 *
 * The values here are chosen to match
 * [hb_direction_t](https://harfbuzz.github.io/harfbuzz-hb-common.html#hb-direction-t).
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 *
 * @sa FontRef.SetDirection
 */
using Direction = TTF_Direction;

constexpr Direction DIRECTION_INVALID = TTF_DIRECTION_INVALID; ///< INVALID

constexpr Direction DIRECTION_LTR = TTF_DIRECTION_LTR; ///< Left to Right.

constexpr Direction DIRECTION_RTL = TTF_DIRECTION_RTL; ///< Right to Left.

constexpr Direction DIRECTION_TTB = TTF_DIRECTION_TTB; ///< Top to Bottom.

constexpr Direction DIRECTION_BTT = TTF_DIRECTION_BTT; ///< Bottom to Top.

/// @}

/**
 * @name Image types
 * @{
 */

/**
 * The type of data in a glyph image
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 */
using ImageType = TTF_ImageType;

constexpr ImageType IMAGE_INVALID = TTF_IMAGE_INVALID; ///< INVALID

constexpr ImageType IMAGE_ALPHA =
  TTF_IMAGE_ALPHA; ///< The color channels are white.

constexpr ImageType IMAGE_COLOR =
  TTF_IMAGE_COLOR; ///< The color channels have image data.

/**
 * The alpha channel has signed distance field information.
 */
constexpr ImageType IMAGE_SDF = TTF_IMAGE_SDF;

/// @}

/**
 * @name Flags for SubString
 * @{
 */

/**
 * Flags for SubString
 *
 * @since This datatype is available since SDL_ttf 3.0.0.
 *
 * @sa SubString
 */
using SubStringFlags = Uint32;

constexpr SubStringFlags SUBSTRING_DIRECTION_MASK =
  TTF_SUBSTRING_DIRECTION_MASK; ///< The mask for the flow direction for this
                                ///< substring

constexpr SubStringFlags SUBSTRING_TEXT_START =
  TTF_SUBSTRING_TEXT_START; ///< This substring contains the beginning of the
                            ///< text

/**
 * This substring contains the beginning of line `line_index`
 */
constexpr SubStringFlags SUBSTRING_LINE_START = TTF_SUBSTRING_LINE_START;

/**
 * This substring contains the end of line `line_index`
 */
constexpr SubStringFlags SUBSTRING_LINE_END = TTF_SUBSTRING_LINE_END;

constexpr SubStringFlags SUBSTRING_TEXT_END =
  TTF_SUBSTRING_TEXT_END; ///< This substring contains the end of the text

/// @}

/**
 * @name GPUTextEngineWindings
 * @{
 */

/**
 * The winding order of the vertices returned by TextRef.GetGPUDrawData
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 */
using GPUTextEngineWinding = TTF_GPUTextEngineWinding;

constexpr GPUTextEngineWinding GPU_TEXTENGINE_WINDING_INVALID =
  TTF_GPU_TEXTENGINE_WINDING_INVALID; ///< INVALID

constexpr GPUTextEngineWinding GPU_TEXTENGINE_WINDING_CLOCKWISE =
  TTF_GPU_TEXTENGINE_WINDING_CLOCKWISE; ///< CLOCKWISE

constexpr GPUTextEngineWinding GPU_TEXTENGINE_WINDING_COUNTER_CLOCKWISE =
  TTF_GPU_TEXTENGINE_WINDING_COUNTER_CLOCKWISE; ///< COUNTER_CLOCKWISE

/// @}

#ifdef SDL3PP_DOC

/**
 * @name TTF version
 * @{
 * Printable format: "%d.%d.%d", MAJOR, MINOR, MICRO
 */
#define SDL_TTF_MAJOR_VERSION

#define SDL_TTF_MINOR_VERSION

#define SDL_TTF_MICRO_VERSION

///@}

/**
 * This is the version number macro for the current SDL_ttf version.
 */
#define SDL_TTF_VERSION                                                        \
  SDL_VERSIONNUM(                                                              \
    SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION)

/**
 * This macro will evaluate to true if compiled with SDL_ttf at least X.Y.Z.
 */
#define SDL_TTF_VERSION_ATLEAST(X, Y, Z)                                       \
  ((SDL_TTF_MAJOR_VERSION >= X) &&                                             \
   (SDL_TTF_MAJOR_VERSION > X || SDL_TTF_MINOR_VERSION >= Y) &&                \
   (SDL_TTF_MAJOR_VERSION > X || SDL_TTF_MINOR_VERSION > Y ||                  \
    SDL_TTF_MICRO_VERSION >= Z))

#endif // SDL3PP_DOC

/**
 * This function gets the version of the dynamically linked SDL_ttf library.
 *
 * @returns SDL_ttf version.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline int TTF_Version() { return ::TTF_Version(); }

/**
 * Query the version of the FreeType library in use.
 *
 * Either Init() or InitSubSystem() should with with INIT_TTF be called before
 * calling this function.
 *
 * @param major to be filled in with the major version number. Can be nullptr.
 * @param minor to be filled in with the minor version number. Can be nullptr.
 * @param patch to be filled in with the param version number. Can be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Init
 * @sa InitSubSystem
 */
inline void GetFreeTypeVersion(int* major, int* minor, int* patch)
{
  TTF_GetFreeTypeVersion(major, minor, patch);
}

/**
 * Query the version of the HarfBuzz library in use.
 *
 * If HarfBuzz is not available, the version reported is 0.0.0.
 *
 * @param major to be filled in with the major version number. Can be nullptr.
 * @param minor to be filled in with the minor version number. Can be nullptr.
 * @param patch to be filled in with the param version number. Can be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void GetHarfBuzzVersion(int* major, int* minor, int* patch)
{
  TTF_GetHarfBuzzVersion(major, minor, patch);
}

/**
 * Internal data for TTF_Text
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 */
using TextData = TTF_TextData;

/**
 * The internal structure containing font information.
 *
 * @cat resource
 *
 * @sa Font
 */
struct FontRef : Resource<TTF_Font*>
{
  using Resource::Resource;

  /**
   * Create a copy of an existing font.
   *
   * The copy will be distinct from the original, but will share the font file
   * and have the same size and style as the original.
   *
   * @returns a valid Font on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               original font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.Close
   */
  Font Copy() const;

  /**
   * Get the properties associated with a font.
   *
   * The following read-write properties are provided by SDL:
   *
   * - `prop::Font.OUTLINE_LINE_CAP_NUMBER`: The FT_Stroker_LineCap value
   *   used when setting the font outline, defaults to
   *   `FT_STROKER_LINECAP_ROUND`.
   * - `prop::Font.OUTLINE_LINE_JOIN_NUMBER`: The FT_Stroker_LineJoin value
   *   used when setting the font outline, defaults to
   *   `FT_STROKER_LINEJOIN_ROUND`.
   * - `prop::Font.OUTLINE_MITER_LIMIT_NUMBER`: The FT_Fixed miter limit used
   *   when setting the font outline, defaults to 0.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  PropertiesRef GetProperties()
  {
    return CheckError(TTF_GetFontProperties(get()));
  }

  /**
   * Get the font generation.
   *
   * The generation is incremented each time font properties change that require
   * rebuilding glyphs, such as style, size, etc.
   *
   * @returns the font generation or 0 on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Uint32 GetGeneration() const { return TTF_GetFontGeneration(get()); }

  /**
   * Add a fallback font.
   *
   * Add a font that will be used for glyphs that are not in the current font.
   * The fallback font should have the same size and style as the current font.
   *
   * If there are multiple fallback fonts, they are used in the order added.
   *
   * This updates any TextRef objects using this font.
   *
   * @param fallback the font to add as a fallback.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created
   *               both fonts.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.ClearFallbacks
   * @sa FontRef.RemoveFallback
   */
  void AddFallback(FontRef fallback)
  {
    CheckError(TTF_AddFallbackFont(get(), fallback.get()));
  }

  /**
   * Remove a fallback font.
   *
   * This updates any TextRef objects using this font.
   *
   * @param fallback the font to remove as a fallback.
   *
   * @threadsafety This function should be called on the thread that created
   *               both fonts.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.AddFallback
   * @sa FontRef.ClearFallbacks
   */
  void RemoveFallback(FontRef fallback)
  {
    TTF_RemoveFallbackFont(get(), fallback.get());
  }

  /**
   * Remove all fallback fonts.
   *
   * This updates any TextRef objects using this font.
   *
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.AddFallback
   * @sa FontRef.RemoveFallback
   */
  void ClearFallbacks() { TTF_ClearFallbackFonts(get()); }

  /**
   * Set a font's size dynamically.
   *
   * This updates any TextRef objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param ptsize the new point size.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.GetSize
   */
  void SetSize(float ptsize) { CheckError(TTF_SetFontSize(get(), ptsize)); }

  /**
   * Set font size dynamically with target resolutions, in dots per inch.
   *
   * This updates any TextRef objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param ptsize the new point size.
   * @param hdpi the target horizontal DPI.
   * @param vdpi the target vertical DPI.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.GetSize
   * @sa TTF_GetFontSizeDPI
   */
  void SetSizeDPI(float ptsize, int hdpi, int vdpi)
  {
    CheckError(TTF_SetFontSizeDPI(get(), ptsize, hdpi, vdpi));
  }

  /**
   * Get the size of a font.
   *
   * @returns the size of the font, or 0.0f on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetSize
   * @sa FontRef.SetSizeDPI
   */
  float GetSize() const { return TTF_GetFontSize(get()); }

  /**
   * Get font target resolutions, in dots per inch.
   *
   * @param hdpi a pointer filled in with the target horizontal DPI.
   * @param vdpi a pointer filled in with the target vertical DPI.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetSizeDPI
   */
  void GetDPI(int* hdpi, int* vdpi) const
  {
    CheckError(TTF_GetFontDPI(get(), hdpi, vdpi));
  }

  /**
   * Set a font's current style.
   *
   * This updates any TextRef objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * The font styles are a set of bit flags, OR'd together:
   *
   * - `STYLE_NORMAL` (is zero)
   * - `STYLE_BOLD`
   * - `STYLE_ITALIC`
   * - `STYLE_UNDERLINE`
   * - `STYLE_STRIKETHROUGH`
   *
   * @param style the new style values to set, OR'd together.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.GetStyle
   */
  void SetStyle(FontStyleFlags style) { TTF_SetFontStyle(get(), style); }

  /**
   * Query a font's current style.
   *
   * The font styles are a set of bit flags, OR'd together:
   *
   * - `STYLE_NORMAL` (is zero)
   * - `STYLE_BOLD`
   * - `STYLE_ITALIC`
   * - `STYLE_UNDERLINE`
   * - `STYLE_STRIKETHROUGH`
   *
   * @returns the current font style, as a set of bit flags.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetStyle
   */
  FontStyleFlags GetStyle() const { return TTF_GetFontStyle(get()); }

  /**
   * Set a font's current outline.
   *
   * This uses the font properties `prop::Font.OUTLINE_LINE_CAP_NUMBER`,
   * `prop::Font.OUTLINE_LINE_JOIN_NUMBER`, and
   * `prop::Font.OUTLINE_MITER_LIMIT_NUMBER` when setting the font outline.
   *
   * This updates any TextRef objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param outline positive outline value, 0 to default.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.GetOutline
   */
  void SetOutline(int outline)
  {
    CheckError(TTF_SetFontOutline(get(), outline));
  }

  /**
   * Query a font's current outline.
   *
   * @returns the font's current outline value.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetOutline
   */
  int GetOutline() const { return TTF_GetFontOutline(get()); }

  /**
   * Set a font's current hinter setting.
   *
   * This updates any TextRef objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * The hinter setting is a single value:
   *
   * - `HINTING_NORMAL`
   * - `HINTING_LIGHT`
   * - `HINTING_MONO`
   * - `HINTING_NONE`
   * - `HINTING_LIGHT_SUBPIXEL` (available in SDL_ttf 3.0.0 and later)
   *
   * @param hinting the new hinter setting.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.GetHinting
   */
  void SetHinting(HintingFlags hinting) { TTF_SetFontHinting(get(), hinting); }

  /**
   * Query the number of faces of a font.
   *
   * @returns the number of FreeType font faces.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  int GetNumFaces() const { return TTF_GetNumFontFaces(get()); }

  /**
   * Query a font's current FreeType hinter setting.
   *
   * The hinter setting is a single value:
   *
   * - `HINTING_NORMAL`
   * - `HINTING_LIGHT`
   * - `HINTING_MONO`
   * - `HINTING_NONE`
   * - `HINTING_LIGHT_SUBPIXEL` (available in SDL_ttf 3.0.0 and later)
   *
   * @returns the font's current hinter value, or HINTING_INVALID if the
   *          font is invalid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetHinting
   */
  HintingFlags GetHinting() const { return TTF_GetFontHinting(get()); }

  /**
   * Enable Signed Distance Field rendering for a font.
   *
   * SDF is a technique that helps fonts look sharp even when scaling and
   * rotating, and requires special shader support for display.
   *
   * This works with Blended APIs, and generates the raw signed distance values
   * in the alpha channel of the resulting texture.
   *
   * This updates any TextRef objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param enabled true to enable SDF, false to disable.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.GetSDF
   */
  void SetSDF(bool enabled) { CheckError(TTF_SetFontSDF(get(), enabled)); }

  /**
   * Query whether Signed Distance Field rendering is enabled for a font.
   *
   * @returns true if enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetSDF
   */
  bool GetSDF() const { return TTF_GetFontSDF(get()); }

#if SDL_TTF_VERSION_ATLEAST(3, 2, 2)

  /**
   * Query a font's weight, in terms of the lightness/heaviness of the strokes.
   *
   * @returns the font's current weight.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.2.2.
   */
  int GetWeight() const { return TTF_GetFontWeight(get()); }

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 2)

  /**
   * Set a font's current wrap alignment option.
   *
   * This updates any TextRef objects using this font.
   *
   * @param align the new wrap alignment option.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.GetWrapAlignment
   */
  void SetWrapAlignment(HorizontalAlignment align)
  {
    TTF_SetFontWrapAlignment(get(), align);
  }

  /**
   * Query a font's current wrap alignment option.
   *
   * @returns the font's current wrap alignment option.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetWrapAlignment
   */
  HorizontalAlignment GetWrapAlignment() const
  {
    return TTF_GetFontWrapAlignment(get());
  }

  /**
   * Query the total height of a font.
   *
   * This is usually equal to point size.
   *
   * @returns the font's height.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  int GetHeight() const { return TTF_GetFontHeight(get()); }

  /**
   * Query the offset from the baseline to the top of a font.
   *
   * This is a positive value, relative to the baseline.
   *
   * @returns the font's ascent.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  int GetAscent() const { return TTF_GetFontAscent(get()); }

  /**
   * Query the offset from the baseline to the bottom of a font.
   *
   * This is a negative value, relative to the baseline.
   *
   * @returns the font's descent.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  int GetDescent() const { return TTF_GetFontDescent(get()); }

  /**
   * Set the spacing between lines of text for a font.
   *
   * This updates any TextRef objects using this font.
   *
   * @param lineskip the new line spacing for the font.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.GetLineSkip
   */
  void SetLineSkip(int lineskip) { TTF_SetFontLineSkip(get(), lineskip); }

  /**
   * Query the spacing between lines of text for a font.
   *
   * @returns the font's recommended spacing.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetLineSkip
   */
  int GetLineSkip() const { return TTF_GetFontLineSkip(get()); }

  /**
   * Set if kerning is enabled for a font.
   *
   * Newly-opened fonts default to allowing kerning. This is generally a good
   * policy unless you have a strong reason to disable it, as it tends to
   * produce better rendering (with kerning disabled, some fonts might render
   * the word `kerning` as something that looks like `keming` for example).
   *
   * This updates any TextRef objects using this font.
   *
   * @param enabled true to enable kerning, false to disable.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.GetKerning
   */
  void SetKerning(bool enabled) { TTF_SetFontKerning(get(), enabled); }

  /**
   * Query whether or not kerning is enabled for a font.
   *
   * @returns true if kerning is enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetKerning
   */
  bool GetKerning() const { return TTF_GetFontKerning(get()); }

  /**
   * Query whether a font is fixed-width.
   *
   * A "fixed-width" font means all glyphs are the same width across; a
   * lowercase 'i' will be the same size across as a capital 'W', for example.
   * This is common for terminals and text editors, and other apps that treat
   * text as a grid. Most other things (WYSIWYG word processors, web pages, etc)
   * are more likely to not be fixed-width in most cases.
   *
   * @returns true if the font is fixed-width, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool IsFixedWidth() const { return TTF_FontIsFixedWidth(get()); }

  /**
   * Query whether a font is scalable or not.
   *
   * Scalability lets us distinguish between outline and bitmap fonts.
   *
   * @returns true if the font is scalable, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.SetSDF
   */
  bool IsScalable() const { return TTF_FontIsScalable(get()); }

  /**
   * Query a font's family name.
   *
   * This string is dictated by the contents of the font file.
   *
   * Note that the returned string is to internal storage, and should not be
   * modified or free'd by the caller. The string becomes invalid, with the rest
   * of the font, when `font` is destroyed.
   *
   * @returns the font's family name.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  const char* GetFamilyName() const { return TTF_GetFontFamilyName(get()); }

  /**
   * Query a font's style name.
   *
   * This string is dictated by the contents of the font file.
   *
   * Note that the returned string is to internal storage, and should not be
   * modified or free'd by the caller. The string becomes invalid, with the rest
   * of the font, when `font` is destroyed.
   *
   * @returns the font's style name.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  const char* GetStyleName() const { return TTF_GetFontStyleName(get()); }

  /**
   * Set the direction to be used for text shaping by a font.
   *
   * This function only supports left-to-right text shaping if SDL_ttf was not
   * built with HarfBuzz support.
   *
   * This updates any TextRef objects using this font.
   *
   * @param direction the new direction for text to flow.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void SetDirection(Direction direction)
  {
    CheckError(TTF_SetFontDirection(get(), direction));
  }

  /**
   * Get the direction to be used for text shaping by a font.
   *
   * This defaults to DIRECTION_INVALID if it hasn't been set.
   *
   * @returns the direction to be used for text shaping.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Direction GetDirection() const { return TTF_GetFontDirection(get()); }

  /**
   * Set the script to be used for text shaping by a font.
   *
   * This returns false if SDL_ttf isn't built with HarfBuzz support.
   *
   * This updates any TextRef objects using this font.
   *
   * @param script an
   * [ISO 15924 code](https://unicode.org/iso15924/iso15924-codes.html).
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa StringToTag
   */
  void SetScript(Uint32 script)
  {
    CheckError(TTF_SetFontScript(get(), script));
  }

  /**
   * Get the script used for text shaping a font.
   *
   * @returns an
   * [ISO 15924 code](https://unicode.org/iso15924/iso15924-codes.html) or 0 if
   * a script hasn't been set.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TagToString
   */
  Uint32 GetScript() const { return TTF_GetFontScript(get()); }

  /**
   * Get the script used by a 32-bit codepoint.
   *
   * @param ch the character code to check.
   * @returns an
   *          [ISO 15924 code](https://unicode.org/iso15924/iso15924-codes.html)
   *          on success, or 0 on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is thread-safe.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TagToString
   */
  static Uint32 GetGlyphScript(Uint32 ch) { return TTF_GetGlyphScript(ch); }

  /**
   * Set language to be used for text shaping by a font.
   *
   * If SDL_ttf was not built with HarfBuzz support, this function returns
   * false.
   *
   * This updates any TextRef objects using this font.
   *
   * @param language_bcp47 a null-terminated string containing the desired
   *                       language's BCP47 code. Or null to reset the value.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void SetLanguage(StringParam language_bcp47)
  {
    CheckError(TTF_SetFontLanguage(get(), language_bcp47));
  }

  /**
   * Check whether a glyph is provided by the font for a UNICODE codepoint.
   *
   * @param ch the codepoint to check.
   * @returns true if font provides a glyph for this character, false if not.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool HasGlyph(Uint32 ch) const { return TTF_FontHasGlyph(get(), ch); }

  /**
   * Get the pixel image for a UNICODE codepoint.
   *
   * @param ch the codepoint to check.
   * @param image_type a pointer filled in with the glyph image type, may be
   *                   nullptr.
   * @returns an SurfaceRef containing the glyph, or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  SurfaceRef GetGlyphImage(Uint32 ch, ImageType* image_type = nullptr) const
  {
    return TTF_GetGlyphImage(get(), ch, image_type);
  }

  /**
   * Get the pixel image for a character index.
   *
   * This is useful for text engine implementations, which can call this with
   * the `glyph_index` in a TTF_CopyOperation
   *
   * @param glyph_index the index of the glyph to return.
   * @param image_type a pointer filled in with the glyph image type, may be
   *                   nullptr.
   * @returns an SurfaceRef containing the glyph, or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  SurfaceRef GetGlyphImageForIndex(Uint32 glyph_index,
                                   ImageType* image_type = nullptr) const
  {
    return TTF_GetGlyphImageForIndex(get(), glyph_index, image_type);
  }

  /**
   * Query the metrics (dimensions) of a font's glyph for a UNICODE codepoint.
   *
   * To understand what these metrics mean, here is a useful link:
   *
   * https://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
   *
   * @param ch the codepoint to check.
   * @param minx a pointer filled in with the minimum x coordinate of the glyph
   *             from the left edge of its bounding box. This value may be
   *             negative.
   * @param maxx a pointer filled in with the maximum x coordinate of the glyph
   *             from the left edge of its bounding box.
   * @param miny a pointer filled in with the minimum y coordinate of the glyph
   *             from the bottom edge of its bounding box. This value may be
   *             negative.
   * @param maxy a pointer filled in with the maximum y coordinate of the glyph
   *             from the bottom edge of its bounding box.
   * @param advance a pointer filled in with the distance to the next glyph from
   *                the left edge of this glyph's bounding box.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetGlyphMetrics(Uint32 ch,
                       int* minx,
                       int* maxx,
                       int* miny,
                       int* maxy,
                       int* advance) const
  {
    CheckError(TTF_GetGlyphMetrics(get(), ch, minx, maxx, miny, maxy, advance));
  }

  /**
   * Query the kerning size between the glyphs of two UNICODE codepoints.
   *
   * @param previous_ch the previous codepoint.
   * @param ch the current codepoint.
   * @returns the kerning size between the two glyphs, in pixels.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  int GetGlyphKerning(Uint32 previous_ch, Uint32 ch) const
  {
    if (int r; TTF_GetGlyphKerning(get(), previous_ch, ch, &r)) return r;
    throw Error();
  }

  /**
   * Calculate the dimensions of a rendered string of UTF-8 text.
   *
   * This will report the width and height, in pixels, of the space that the
   * specified string will take to fully render.
   *
   * @param text text to calculate, in UTF-8 encoding.
   * @returns return a Point with the width, height.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Point GetStringSize(std::string_view text) const
  {
    Point p;
    GetStringSize(text, &p.x, &p.y);
    return p;
  }

  /**
   * Calculate the dimensions of a rendered string of UTF-8 text.
   *
   * This will report the width and height, in pixels, of the space that the
   * specified string will take to fully render.
   *
   * @param text text to calculate, in UTF-8 encoding.
   * @param w will be filled with width, in pixels, on return.
   * @param h will be filled with height, in pixels, on return.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetStringSize(std::string_view text, int* w, int* h) const
  {
    CheckError(TTF_GetStringSize(get(), text.data(), text.size(), w, h));
  }

  /**
   * Calculate the dimensions of a rendered string of UTF-8 text.
   *
   * This will report the width and height, in pixels, of the space that the
   * specified string will take to fully render.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * @param text text to calculate, in UTF-8 encoding.
   * @param wrap_width the maximum width or 0 to wrap on newline characters.
   * @returns return a Point with the width, height on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Point GetStringSizeWrapped(std::string_view text, int wrap_width) const
  {
    Point p;
    GetStringSizeWrapped(text, wrap_width, &p.x, &p.y);
    return p;
  }

  /**
   * Calculate the dimensions of a rendered string of UTF-8 text.
   *
   * This will report the width and height, in pixels, of the space that the
   * specified string will take to fully render.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * @param text text to calculate, in UTF-8 encoding.
   * @param wrap_width the maximum width or 0 to wrap on newline characters.
   * @param w will be filled with width, in pixels, on return.
   * @param h will be filled with height, in pixels, on return.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetStringSizeWrapped(std::string_view text,
                            int wrap_width,
                            int* w,
                            int* h) const
  {
    CheckError(TTF_GetStringSizeWrapped(
      get(), text.data(), text.size(), wrap_width, w, h));
  }

  /**
   * Calculate how much of a UTF-8 string will fit in a given width.
   *
   * This reports the number of characters that can be rendered before reaching
   * `max_width`.
   *
   * This does not need to render the string to do this calculation.
   *
   * @param text text to calculate, in UTF-8 encoding.
   * @param max_width maximum width, in pixels, available for the string, or 0
   *                  for unbounded width.
   * @param measured_width a pointer filled in with the width, in pixels, of the
   *                       string that will fit, may be nullptr.
   * @param measured_length a pointer filled in with the length, in bytes, of
   *                        the string that will fit, may be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void MeasureString(std::string_view text,
                     int max_width,
                     int* measured_width,
                     size_t* measured_length) const
  {
    CheckError(TTF_MeasureString(get(),
                                 text.data(),
                                 text.size(),
                                 max_width,
                                 measured_width,
                                 measured_length));
  }

  /**
   * Render UTF-8 text at fast quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the colorkey, giving a transparent background. The 1 pixel
   * will be set to the text color.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * FontRef.RenderText_Solid_Wrapped() instead if you need to wrap the output
   * to multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with FontRef.RenderText_Shaded,
   * FontRef.RenderText_Blended, and FontRef.RenderText_LCD.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @returns a new 8-bit, palettized surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderText_Blended
   * @sa FontRef.RenderText_LCD
   * @sa FontRef.RenderText_Shaded
   * @sa FontRef.RenderText_Solid
   * @sa FontRef.RenderText_Solid_Wrapped
   */
  Surface RenderText_Solid(std::string_view text, Color fg) const
  {
    return Surface{TTF_RenderText_Solid(get(), text.data(), text.size(), fg)};
  }

  /**
   * Render word-wrapped UTF-8 text at fast quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the colorkey, giving a transparent background. The 1 pixel
   * will be set to the text color.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrapLength` in pixels.
   *
   * If wrapLength is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with
   * FontRef.RenderText_Shaded_Wrapped, FontRef.RenderText_Blended_Wrapped,
   * and FontRef.RenderText_LCD_Wrapped.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param wrapLength the maximum width of the text surface or 0 to wrap on
   *                   newline characters.
   * @returns a new 8-bit, palettized surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderText_Blended_Wrapped
   * @sa FontRef.RenderText_LCD_Wrapped
   * @sa FontRef.RenderText_Shaded_Wrapped
   * @sa FontRef.RenderText_Solid
   */
  Surface RenderText_Solid_Wrapped(std::string_view text,
                                   Color fg,
                                   int wrapLength) const
  {
    return Surface{TTF_RenderText_Solid_Wrapped(
      get(), text.data(), text.size(), fg, wrapLength)};
  }

  /**
   * Render a single 32-bit glyph at fast quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the colorkey, giving a transparent background. The 1 pixel
   * will be set to the text color.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with FontRef.RenderGlyph_Shaded,
   * FontRef.RenderGlyph_Blended, and FontRef.RenderGlyph_LCD.
   *
   * @param ch the character to render.
   * @param fg the foreground color for the text.
   * @returns a new 8-bit, palettized surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderGlyph_Blended
   * @sa FontRef.RenderGlyph_LCD
   * @sa FontRef.RenderGlyph_Shaded
   */
  Surface RenderGlyph_Solid(Uint32 ch, Color fg) const
  {
    return Surface{TTF_RenderGlyph_Solid(get(), ch, fg)};
  }

  /**
   * Render UTF-8 text at high quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the specified background color, while other pixels have
   * varying degrees of the foreground color. This function returns the new
   * surface, or nullptr if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * FontRef.RenderText_Shaded_Wrapped() instead if you need to wrap the output
   * to multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with FontRef.RenderText_Solid,
   * FontRef.RenderText_Blended, and FontRef.RenderText_LCD.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @returns a new 8-bit, palettized surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderText_Blended
   * @sa FontRef.RenderText_LCD
   * @sa FontRef.RenderText_Shaded_Wrapped
   * @sa FontRef.RenderText_Solid
   */
  Surface RenderText_Shaded(std::string_view text, Color fg, Color bg) const
  {
    return Surface{
      TTF_RenderText_Shaded(get(), text.data(), text.size(), fg, bg)};
  }

  /**
   * Render word-wrapped UTF-8 text at high quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the specified background color, while other pixels have
   * varying degrees of the foreground color. This function returns the new
   * surface, or nullptr if there was an error.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with
   * FontRef.RenderText_Solid_Wrapped, FontRef.RenderText_Blended_Wrapped, and
   * FontRef.RenderText_LCD_Wrapped.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @param wrap_width the maximum width of the text surface or 0 to wrap on
   *                   newline characters.
   * @returns a new 8-bit, palettized surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderText_Blended_Wrapped
   * @sa FontRef.RenderText_LCD_Wrapped
   * @sa FontRef.RenderText_Shaded
   * @sa FontRef.RenderText_Solid_Wrapped
   */
  Surface RenderText_Shaded_Wrapped(std::string_view text,
                                    Color fg,
                                    Color bg,
                                    int wrap_width) const
  {
    return Surface{TTF_RenderText_Shaded_Wrapped(
      get(), text.data(), text.size(), fg, bg, wrap_width)};
  }

  /**
   * Render a single UNICODE codepoint at high quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the specified background color, while other pixels have
   * varying degrees of the foreground color. This function returns the new
   * surface, or nullptr if there was an error.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with FontRef.RenderGlyph_Solid,
   * FontRef.RenderGlyph_Blended, and FontRef.RenderGlyph_LCD.
   *
   * @param ch the codepoint to render.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @returns a new 8-bit, palettized surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderGlyph_Blended
   * @sa FontRef.RenderGlyph_LCD
   * @sa FontRef.RenderGlyph_Solid
   */
  Surface RenderGlyph_Shaded(Uint32 ch, Color fg, Color bg) const
  {
    return Surface{TTF_RenderGlyph_Shaded(get(), ch, fg, bg)};
  }

  /**
   * Render UTF-8 text at high quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, using alpha
   * blending to dither the font with the given color. This function returns the
   * new surface, or nullptr if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * FontRef.RenderText_Blended_Wrapped() instead if you need to wrap the
   * output to multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with FontRef.RenderText_Solid,
   * FontRef.RenderText_Shaded, and FontRef.RenderText_LCD.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @returns a new 32-bit, ARGB surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderText_Blended_Wrapped
   * @sa FontRef.RenderText_LCD
   * @sa FontRef.RenderText_Shaded
   * @sa FontRef.RenderText_Solid
   */
  Surface RenderText_Blended(std::string_view text, Color fg) const
  {
    return Surface{TTF_RenderText_Blended(get(), text.data(), text.size(), fg)};
  }

  /**
   * Render word-wrapped UTF-8 text at high quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, using alpha
   * blending to dither the font with the given color. This function returns the
   * new surface, or nullptr if there was an error.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with
   * FontRef.RenderText_Solid_Wrapped, FontRef.RenderText_Shaded_Wrapped, and
   * FontRef.RenderText_LCD_Wrapped.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param wrap_width the maximum width of the text surface or 0 to wrap on
   *                   newline characters.
   * @returns a new 32-bit, ARGB surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderText_Blended
   * @sa FontRef.RenderText_LCD_Wrapped
   * @sa FontRef.RenderText_Shaded_Wrapped
   * @sa FontRef.RenderText_Solid_Wrapped
   */
  Surface RenderText_Blended_Wrapped(std::string_view text,
                                     Color fg,
                                     int wrap_width) const
  {
    return Surface{TTF_RenderText_Blended_Wrapped(
      get(), text.data(), text.size(), fg, wrap_width)};
  }

  /**
   * Render a single UNICODE codepoint at high quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, using alpha
   * blending to dither the font with the given color. This function returns the
   * new surface, or nullptr if there was an error.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with FontRef.RenderGlyph_Solid,
   * FontRef.RenderGlyph_Shaded, and FontRef.RenderGlyph_LCD.
   *
   * @param ch the codepoint to render.
   * @param fg the foreground color for the text.
   * @returns a new 32-bit, ARGB surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderGlyph_LCD
   * @sa FontRef.RenderGlyph_Shaded
   * @sa FontRef.RenderGlyph_Solid
   */
  Surface RenderGlyph_Blended(Uint32 ch, Color fg) const
  {
    return Surface{TTF_RenderGlyph_Blended(get(), ch, fg)};
  }

  /**
   * Render UTF-8 text at LCD subpixel quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, and render
   * alpha-blended text using FreeType's LCD subpixel rendering. This function
   * returns the new surface, or nullptr if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * FontRef.RenderText_LCD_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with FontRef.RenderText_Solid,
   * FontRef.RenderText_Shaded, and FontRef.RenderText_Blended.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @returns a new 32-bit, ARGB surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderText_Blended
   * @sa FontRef.RenderText_LCD_Wrapped
   * @sa FontRef.RenderText_Shaded
   * @sa FontRef.RenderText_Solid
   */
  Surface RenderText_LCD(std::string_view text, Color fg, Color bg) const
  {
    return Surface{TTF_RenderText_LCD(get(), text.data(), text.size(), fg, bg)};
  }

  /**
   * Render word-wrapped UTF-8 text at LCD subpixel quality to a new ARGB
   * surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, and render
   * alpha-blended text using FreeType's LCD subpixel rendering. This function
   * returns the new surface, or nullptr if there was an error.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with
   * FontRef.RenderText_Solid_Wrapped, FontRef.RenderText_Shaded_Wrapped, and
   * FontRef.RenderText_Blended_Wrapped.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @param wrap_width the maximum width of the text surface or 0 to wrap on
   *                   newline characters.
   * @returns a new 32-bit, ARGB surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderText_Blended_Wrapped
   * @sa FontRef.RenderText_LCD
   * @sa FontRef.RenderText_Shaded_Wrapped
   * @sa FontRef.RenderText_Solid_Wrapped
   */
  Surface RenderText_LCD_Wrapped(std::string_view text,
                                 Color fg,
                                 Color bg,
                                 int wrap_width) const
  {
    return Surface{TTF_RenderText_LCD_Wrapped(
      get(), text.data(), text.size(), fg, bg, wrap_width)};
  }

  /**
   * Render a single UNICODE codepoint at LCD subpixel quality to a new ARGB
   * surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, and render
   * alpha-blended text using FreeType's LCD subpixel rendering. This function
   * returns the new surface, or nullptr if there was an error.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with FontRef.RenderGlyph_Solid,
   * FontRef.RenderGlyph_Shaded, and FontRef.RenderGlyph_Blended.
   *
   * @param ch the codepoint to render.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @returns a new 32-bit, ARGB surface, or nullptr if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontRef.RenderGlyph_Blended
   * @sa FontRef.RenderGlyph_Shaded
   * @sa FontRef.RenderGlyph_Solid
   */
  Surface RenderGlyph_LCD(Uint32 ch, Color fg, Color bg) const
  {
    return Surface{TTF_RenderGlyph_LCD(get(), ch, fg, bg)};
  }

  /**
   * Dispose of a previously-created font.
   *
   * Call this when done with a font. This function will free any resources
   * associated with it. It is safe to call this function on nullptr, for
   * example on the result of a failed call to Font.Open().
   *
   * The font is not valid after being passed to this function. String pointers
   * from functions that return information on this font, such as
   * FontRef.GetFamilyName() and FontRef.GetStyleName(), are no longer valid
   * after this call, as well.
   *
   * @param resource the font to dispose of.
   *
   * @threadsafety This function should not be called while any other thread is
   *               using the font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.Open
   * @sa Font.Open
   */
  static void reset(TTF_Font* resource) { TTF_CloseFont(resource); }
};

/**
 * Handle to an owned font
 *
 * @cat resource
 *
 * @sa FontRef
 */
struct Font : ResourceUnique<FontRef>
{
  using ResourceUnique::ResourceUnique;

  /**
   * Create a font from a file, using a specified point size.
   *
   * Some .fon fonts will have several sizes embedded in the file, so the point
   * size becomes the index of choosing which size. If the value is too high,
   * the last indexed size will be the default.
   *
   * @param file path to font file.
   * @param ptsize point size to use for the newly-opened font.
   * @returns a valid FontRef on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.Close
   */
  static Font Open(StringParam file, float ptsize)
  {
    return Font(CheckError(TTF_OpenFont(file, ptsize)));
  }

  /**
   * Create a font from an IOStreamRef, using a specified point size.
   *
   * Some .fon fonts will have several sizes embedded in the file, so the point
   * size becomes the index of choosing which size. If the value is too high,
   * the last indexed size will be the default.
   *
   * @param src an IOStreamRef to provide a font file's data.
   * @param ptsize point size to use for the newly-opened font.
   * @returns a valid FontRef on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.Close
   */
  static Font Open(IOStreamRef src, float ptsize)
  {
    return Font(CheckError(TTF_OpenFontIO(src, false, ptsize)));
  }

  /**
   * Create a font with the specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::Font.CREATE_FILENAME_STRING`: the font file to open, if an
   *   IOStreamRef isn't being used. This is required if
   *   `prop::Font.CREATE_IOSTREAM_POINTER` and
   *   `prop::Font.CREATE_EXISTING_FONT_POINTER` aren't set.
   * - `prop::Font.CREATE_IOSTREAM_POINTER`: an IOStreamRef containing the
   *   font to be opened. This should not be closed until the font is closed.
   *   This is required if `prop::Font.CREATE_FILENAME_STRING` and
   *   `prop::Font.CREATE_EXISTING_FONT_POINTER` aren't set.
   * - `prop::Font.CREATE_IOSTREAM_OFFSET_NUMBER`: the offset in the iostream
   *   for the beginning of the font, defaults to 0.
   * - `prop::Font.CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN`: true if closing the
   *   font should also close the associated IOStreamRef.
   * - `prop::Font.CREATE_SIZE_FLOAT`: the point size of the font. Some .fon
   *   fonts will have several sizes embedded in the file, so the point size
   *   becomes the index of choosing which size. If the value is too high, the
   *   last indexed size will be the default.
   * - `prop::Font.CREATE_FACE_NUMBER`: the face index of the font, if the
   *   font contains multiple font faces.
   * - `prop::Font.CREATE_HORIZONTAL_DPI_NUMBER`: the horizontal DPI to use
   *   for font rendering, defaults to
   *   `prop::Font.CREATE_VERTICAL_DPI_NUMBER` if set, or 72 otherwise.
   * - `prop::Font.CREATE_VERTICAL_DPI_NUMBER`: the vertical DPI to use for
   *   font rendering, defaults to `prop::Font.CREATE_HORIZONTAL_DPI_NUMBER`
   *   if set, or 72 otherwise.
   * - `prop::Font.CREATE_EXISTING_FONT_POINTER`: an optional FontRef that, if
   * set, will be used as the font data source and the initial size and style of
   *   the new font.
   *
   * @param props the properties to use.
   * @returns a valid FontRef on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.Close
   */
  static Font OpenWithProperties(PropertiesRef props)
  {
    return Font(CheckError(TTF_OpenFontWithProperties(props)));
  }

  /**
   * Dispose of a previously-created font.
   *
   * Call this when done with a font. This function will free any resources
   * associated with it. It is safe to call this function on nullptr, for
   * example on the result of a failed call to Font.Open().
   *
   * The font is not valid after being passed to this function. String pointers
   * from functions that return information on this font, such as
   * FontRef.GetFamilyName() and FontRef.GetStyleName(), are no longer valid
   * after this call, as well.
   *
   *
   * @threadsafety This function should not be called while any other thread is
   *               using the font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.Open
   */
  void Close() { reset(); }
};

/**
 * Unsafe Handle to font
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa FontRef
 */
struct FontUnsafe : ResourceUnsafe<FontRef>
{
  using ResourceUnsafe::ResourceUnsafe;

  /**
   * Constructs FontUnsafe from Font.
   */
  constexpr explicit FontUnsafe(Font&& other)
    : FontUnsafe(other.release())
  {
  }
};

/**
 * Initialize SDL_ttf.
 *
 * You must successfully call this function before it is safe to call any
 * other function in this library.
 *
 * It is safe to call this more than once, and each successful InitSubSystem()
 * call should be paired with a matching QuitSubSystem(TtfInitFlag) call.
 *
 * @param _ An INIT_TTF value;
 * @throws Error on failure.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa QuitSubSystem(TtfInitFlag)
 */
inline void InitSubSystem(TtfInitFlag _) { CheckError(TTF_Init()); }

/**
 * A text engine used to create text objects.
 *
 * This is a public interface that can be used by applications and libraries
 * to perform customize rendering with text objects. See
 * <SDL3_ttf/SDL_textengine.h> for details.
 *
 * There are three text engines provided with the library:
 *
 * - Drawing to an SDL_Surface, created with CreateSurfaceTextEngine()
 * - Drawing with an SDL 2D renderer, created with CreateRendererTextEngine()
 * - Drawing with the SDL GPU API, created with CreateGPUTextEngine()
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @cat resource
 *
 * @sa TextEngine
 * @sa TextEngineRef
 */
struct TextEngineRef : Resource<TTF_TextEngine*>
{
  using Resource::Resource;

  /**
   * Sets the winding order of the vertices returned by TextRef.GetGPUDrawData
   * for a particular GPU text engine.
   *
   * @param winding the new winding order option.
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngineRef.GetGPUWinding
   */
  void SetGPUWinding(GPUTextEngineWinding winding)
  {
    TTF_SetGPUTextEngineWinding(get(), winding);
  }

  /**
   * Get the winding order of the vertices returned by TextRef.GetGPUDrawData
   * for a particular GPU text engine
   *
   * @returns the winding order used by the GPU text engine or
   *          GPU_TEXTENGINE_WINDING_INVALID in case of error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngineRef.SetGPUWinding
   */
  GPUTextEngineWinding GetGPUWinding() const
  {
    return TTF_GetGPUTextEngineWinding(get());
  }
  /**
   * Destroy a text engine created for drawing text on SDL surfaces.
   *
   * All text created by this engine should be destroyed before calling this
   * function.
   *
   * @param engine a TextEngineRef object created with
   *               TextEngine.CreateSurface().
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngine.CreateSurface
   */
  static void DestroySurface(TTF_TextEngine* engine)
  {
    TTF_DestroySurfaceTextEngine(engine);
  }

  /**
   * Destroy a text engine created for drawing text on an SDL renderer.
   *
   * All text created by this engine should be destroyed before calling this
   * function.
   *
   * @param engine a TextEngineRef object created with
   *               TextEngine.CreateRenderer().
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngine.CreateRenderer
   */
  static void DestroyRenderer(TTF_TextEngine* engine)
  {
    TTF_DestroyRendererTextEngine(engine);
  }

  /**
   * Destroy a text engine created for drawing text with the SDL GPU API.
   *
   * All text created by this engine should be destroyed before calling this
   * function.
   *
   * @param engine a TextEngineRef object created with
   *               TextEngine.CreateGPU().
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngine.CreateGPU
   */
  static void DestroyGPU(TTF_TextEngine* engine)
  {
    TTF_DestroyGPUTextEngine(engine);
  }
};

/**
 * Handle to an owned textEngine
 *
 * @cat resource
 *
 * @sa TextEngineRef
 */
struct TextEngine : ResourceUnique<TextEngineRef, TextEngineDeleter>
{
  using ResourceUnique::ResourceUnique;

  /**
   * Create a text engine for drawing text on SDL surfaces.
   *
   * @returns a TextEngineRef object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_DestroySurfaceTextEngine
   * @sa TextRef.DrawSurface
   */
  static TextEngine CreateSurface()
  {
    return TextEngine(TTF_CreateSurfaceTextEngine(),
                      TextEngineRef::DestroySurface);
  }

  /**
   * Create a text engine for drawing text on an SDL renderer.
   *
   * @param renderer the renderer to use for creating textures and drawing text.
   * @returns a TextEngineRef object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               renderer.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_DestroyRendererTextEngine
   * @sa TextRef.DrawRenderer
   * @sa TextEngine.CreateRendererWithProperties
   */
  static TextEngine CreateRenderer(RendererRef renderer)
  {
    return TextEngine(TTF_CreateRendererTextEngine(renderer),
                      TextEngineRef::DestroyRenderer);
  }

  /**
   * Create a text engine for drawing text on an SDL renderer, with the
   * specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::RendererTextEngine.RENDERER_POINTER`: the renderer to use for
   *   creating textures and drawing text
   * - `prop::RendererTextEngine.ATLAS_TEXTURE_SIZE_NUMBER`: the size of the
   *   texture atlas
   *
   * @param props the properties to use.
   * @returns a TextEngineRef object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               renderer.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngine.CreateRenderer
   * @sa TTF_DestroyRendererTextEngine
   * @sa TextRef.DrawRenderer
   */
  static TextEngine CreateRendererWithProperties(PropertiesRef props)
  {
    return TextEngine(TTF_CreateRendererTextEngineWithProperties(props),
                      TextEngineRef::DestroyRenderer);
  }

  /**
   * Create a text engine for drawing text with the SDL GPU API.
   *
   * @param device the SDL_GPUDevice to use for creating textures and drawing
   *               text.
   * @returns a TextEngineRef object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               device.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngine.CreateGPUWithProperties
   * @sa TTF_DestroyGPUTextEngine
   * @sa TextRef.GetGPUDrawData
   */
  static TextEngine CreateGPU(SDL_GPUDevice* device)
  {
    return TextEngine(TTF_CreateGPUTextEngine(device),
                      TextEngineRef::DestroyGPU);
  }

  /**
   * Create a text engine for drawing text with the SDL GPU API, with the
   * specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::GpuTextEngine.DEVICE_POINTER`: the SDL_GPUDevice to use for
   * creating textures and drawing text.
   * - `prop::GpuTextEngine.ATLAS_TEXTURE_SIZE_NUMBER`: the size of the texture
   *   atlas
   *
   * @param props the properties to use.
   * @returns a TextEngineRef object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               device.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngine.CreateGPU
   * @sa TTF_DestroyGPUTextEngine
   * @sa TextRef.GetGPUDrawData
   */
  static TextEngine CreateGPUWithProperties(PropertiesRef props)
  {
    return TextEngine(TTF_CreateGPUTextEngineWithProperties(props),
                      TextEngineRef::DestroyGPU);
  }
};

/**
 * Unsafe Handle to textEngine
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa TextEngineRef
 */
struct TextEngineUnsafe : ResourceUnsafe<TextEngineRef, TextEngineDeleter>
{
  using ResourceUnsafe::ResourceUnsafe;

  /**
   * Constructs TextEngineUnsafe from TextEngine.
   */
  constexpr explicit TextEngineUnsafe(TextEngine&& other)
    : TextEngineUnsafe(other.release())
  {
  }
};

/**
 * Draw sequence returned by TextRef.GetGPUDrawData
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa TextRef.GetGPUDrawData
 */
using GPUAtlasDrawSequence = TTF_GPUAtlasDrawSequence;

/**
 * The representation of a substring within text.
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa TextRef.GetNextSubString
 * @sa TextRef.GetPreviousSubString
 * @sa TextRef.GetSubString
 * @sa TextRef.GetSubStrings
 * @sa TextRef.GetSubStringForLine
 * @sa TextRef.GetSubStringForPoint
 * @sa TextRef.GetSubStringsForRange
 */
using SubString = TTF_SubString;

// Forward decl
struct SubStringIterator;

namespace prop::Font {

constexpr auto CREATE_FILENAME_STRING = TTF_PROP_FONT_CREATE_FILENAME_STRING;

constexpr auto CREATE_IOSTREAM_POINTER = TTF_PROP_FONT_CREATE_IOSTREAM_POINTER;

constexpr auto CREATE_IOSTREAM_OFFSET_NUMBER =
  TTF_PROP_FONT_CREATE_IOSTREAM_OFFSET_NUMBER;

constexpr auto CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN =
  TTF_PROP_FONT_CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN;

constexpr auto CREATE_SIZE_FLOAT = TTF_PROP_FONT_CREATE_SIZE_FLOAT;

constexpr auto CREATE_FACE_NUMBER = TTF_PROP_FONT_CREATE_FACE_NUMBER;

constexpr auto CREATE_HORIZONTAL_DPI_NUMBER =
  TTF_PROP_FONT_CREATE_HORIZONTAL_DPI_NUMBER;

constexpr auto CREATE_VERTICAL_DPI_NUMBER =
  TTF_PROP_FONT_CREATE_VERTICAL_DPI_NUMBER;

#if SDL_TTF_VERSION_ATLEAST(3, 2, 3)

constexpr auto CREATE_EXISTING_FONT_POINTER =
  TTF_PROP_FONT_CREATE_EXISTING_FONT_POINTER;

#else

constexpr auto CREATE_EXISTING_FONT_POINTER =
  TTF_PROP_FONT_CREATE_EXISTING_FONT;

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 3)

constexpr auto OUTLINE_LINE_CAP_NUMBER = TTF_PROP_FONT_OUTLINE_LINE_CAP_NUMBER;

constexpr auto OUTLINE_LINE_JOIN_NUMBER =
  TTF_PROP_FONT_OUTLINE_LINE_JOIN_NUMBER;

constexpr auto OUTLINE_MITER_LIMIT_NUMBER =
  TTF_PROP_FONT_OUTLINE_MITER_LIMIT_NUMBER;

} // namespace prop::Font

inline Font FontRef::Copy() const { return Font{TTF_CopyFont(get())}; }

#ifdef SDL3PP_DOC

/**
 * Thin (100) named font weight value
 */
#define SDL_FONT_WEIGHT_THIN TTF_FONT_WEIGHT_THIN

/**
 * ExtraLight (200) named font weight value
 */
#define SDL_FONT_WEIGHT_EXTRA_LIGHT TTF_FONT_WEIGHT_EXTRA_LIGHT

/**
 * Light (300) named font weight value
 */
#define SDL_FONT_WEIGHT_LIGHT TTF_FONT_WEIGHT_LIGHT

/**
 * Normal (400) named font weight value
 */
#define SDL_FONT_WEIGHT_NORMAL TTF_FONT_WEIGHT_NORMAL

/**
 * Medium (500) named font weight value
 */
#define SDL_FONT_WEIGHT_MEDIUM TTF_FONT_WEIGHT_MEDIUM

/**
 * SemiBold (600) named font weight value
 */
#define SDL_FONT_WEIGHT_SEMI_BOLD TTF_FONT_WEIGHT_SEMI_BOLD

/**
 * Bold (700) named font weight value
 */
#define SDL_FONT_WEIGHT_BOLD TTF_FONT_WEIGHT_BOLD

/**
 * ExtraBold (800) named font weight value
 */
#define SDL_FONT_WEIGHT_EXTRA_BOLD TTF_FONT_WEIGHT_EXTRA_BOLD

/**
 * Black (900) named font weight value
 */
#define SDL_FONT_WEIGHT_BLACK TTF_FONT_WEIGHT_BLACK

/**
 * ExtraBlack (950) named font weight value
 */
#define SDL_FONT_WEIGHT_EXTRA_BLACK TTF_FONT_WEIGHT_EXTRA_BLACK

#endif // SDL3PP_DOC

/**
 * Convert from a 4 character string to a 32-bit tag.
 *
 * @param string the 4 character string to convert.
 * @returns the 32-bit representation of the string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TagToString
 */
inline Uint32 StringToTag(StringParam string)
{
  return TTF_StringToTag(string);
}

/**
 * Convert from a 32-bit tag to a 4 character string.
 *
 * @param tag the 32-bit tag to convert.
 * @param string a pointer filled in with the 4 character representation of
 *               the tag.
 * @param size the size of the buffer pointed at by string, should be at least
 *             4.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TagToString
 */
inline void TagToString(Uint32 tag, char* string, size_t size)
{
  TTF_TagToString(tag, string, size);
}

/**
 * Text created with Text.Create()
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @cat resource
 *
 * @sa Text.Create
 * @sa TextRef.GetProperties
 * @sa Text.Destroy
 * @sa Text
 */
struct TextRef : Resource<TTF_Text*>
{
  using Resource::Resource;

  /**
   * Draw text to an SDL surface.
   *
   * `text` must have been created using a TextEngineRef from
   * TextEngine.CreateSurface().
   *
   * @param p the (x, y) coordinate in pixels, positive from the left edge
   *          towards the right and from the top edge towards the bottom.
   * @param surface the surface to draw on.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngine.CreateSurface
   * @sa Text.Create
   */
  void DrawSurface(Point p, SurfaceRef surface) const
  {
    CheckError(TTF_DrawSurfaceText(get(), p.x, p.y, surface.get()));
  }

  /**
   * Draw text to an SDL renderer.
   *
   * `text` must have been created using a TextEngineRef from
   * TextEngine.CreateRenderer(), and will draw using the renderer passed to
   * that function.
   *
   * @param p the (x, y) coordinate in pixels, positive from the left edge
   *          towards the right and from the top edge towards the bottom.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngine.CreateRenderer
   * @sa Text.Create
   */
  void DrawRenderer(FPoint p) const
  {
    CheckError(TTF_DrawRendererText(get(), p.x, p.y));
  }

  /**
   * Get the geometry data needed for drawing the text.
   *
   * `text` must have been created using a TextEngineRef from
   * TextEngine.CreateGPU().
   *
   * The positive X-axis is taken towards the right and the positive Y-axis is
   * taken upwards for both the vertex and the texture coordinates, i.e, it
   * follows the same convention used by the SDL_GPU API. If you want to use a
   * different coordinate system you will need to transform the vertices
   * yourself.
   *
   * If the text looks blocky use linear filtering.
   *
   * @returns a nullptr terminated linked list of GPUAtlasDrawSequence objects
   *          or nullptr if the passed text is empty or in case of failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngine.CreateGPU
   * @sa Text.Create
   */
  GPUAtlasDrawSequence* GetGPUDrawData() const
  {
    return TTF_GetGPUTextDrawData(get());
  }

  /**
   * Get the properties associated with a text object.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  PropertiesRef GetProperties() const
  {
    return CheckError(TTF_GetTextProperties(get()));
  }

  /**
   * Set the text engine used by a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param engine the text engine to use for drawing.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.GetEngine
   */
  void SetEngine(TextEngineRef engine)
  {
    CheckError(TTF_SetTextEngine(get(), engine));
  }

  /**
   * Get the text engine used by a text object.
   *
   * @returns the TextEngineRef used by the text on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.SetEngine
   */
  TextEngineRef GetEngine() const
  {
    return CheckError(TTF_GetTextEngine(get()));
  }

  /**
   * Set the font used by a text object.
   *
   * When a text object has a font, any changes to the font will automatically
   * regenerate the text. If you set the font to nullptr, the text will continue
   * to render but changes to the font will no longer affect the text.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param font the font to use, may be nullptr.
   * @returns false if the text pointer is null; otherwise, true. call
   *          GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.GetFont
   */
  bool SetFont(FontRef font) { return TTF_SetTextFont(get(), font); }

  /**
   * Get the font used by a text object.
   *
   * @returns the FontRef used by the text on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.SetFont
   */
  FontRef GetFont() const { return CheckError(TTF_GetTextFont(get())); }

  /**
   * Set the direction to be used for text shaping a text object.
   *
   * This function only supports left-to-right text shaping if SDL_ttf was not
   * built with HarfBuzz support.
   *
   * @param direction the new direction for text to flow.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void SetDirection(Direction direction)
  {
    CheckError(TTF_SetTextDirection(get(), direction));
  }

  /**
   * Get the direction to be used for text shaping a text object.
   *
   * This defaults to the direction of the font used by the text object.
   *
   * @returns the direction to be used for text shaping.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Direction GetDirection() const { return TTF_GetTextDirection(get()); }

  /**
   * Set the script to be used for text shaping a text object.
   *
   * This returns false if SDL_ttf isn't built with HarfBuzz support.
   *
   * @param script an [ISO 15924
   * code](https://unicode.org/iso15924/iso15924-codes.html).
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa StringToTag
   */
  void SetScript(Uint32 script)
  {
    CheckError(TTF_SetTextScript(get(), script));
  }

  /**
   * Get the script used for text shaping a text object.
   *
   * This defaults to the script of the font used by the text object.
   *
   * @returns an
   *          [ISO 15924 code](https://unicode.org/iso15924/iso15924-codes.html)
   *          or 0 if a script hasn't been set on either the text object or the
   *          font.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TagToString
   */
  Uint32 GetScript() const { return TTF_GetTextScript(get()); }

  /**
   * Set the color of a text object.
   *
   * The default text color is white (255, 255, 255, 255).
   *
   * @param c the color values in the range of 0-255.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.GetColor(Color*)
   * @sa TextRef.SetColor(FColor)
   */
  void SetColor(Color c)
  {
    CheckError(TTF_SetTextColor(get(), c.r, c.g, c.b, c.a));
  }

  /**
   * Set the color of a text object.
   *
   * The default text color is white (1.0f, 1.0f, 1.0f, 1.0f).
   *
   * @param c the color value, normally in the range of 0-1.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.GetColor(FColor*)
   * @sa TextRef.SetColor(Color)
   */
  void SetColor(FColor c)
  {
    CheckError(TTF_SetTextColorFloat(get(), c.r, c.g, c.b, c.a));
  }

  /**
   * Get the color of a text object.
   *
   * @return The color in the range of 0-1 on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @sa SetColor(FColor)
   */
  FColor GetColor() const
  {
    FColor c;
    GetColor(&c);
    return c;
  }

  /**
   * Get the color of a text object.
   *
   * @param c a pointer filled in with red color value in the range of 0-255,
   *          __must__ not be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetColor(Color)
   */
  void GetColor(Color* c) const
  {
    SDL_assert_paranoid(c != nullptr);
    GetColor(&c->r, &c->g, &c->b, &c->a);
  }

  /**
   * Get the color of a text object.
   *
   * @param c a pointer filled in with red color value in the range of 0-1,
   *          __must__ not be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetColor(FColor)
   */
  void GetColor(FColor* c) const
  {
    SDL_assert_paranoid(c != nullptr);
    GetColor(&c->r, &c->g, &c->b, &c->a);
  }

  /**
   * Get the color of a text object.
   *
   * @param r a pointer filled in with the red color value in the range of
   *          0-255, may be nullptr.
   * @param g a pointer filled in with the green color value in the range of
   *          0-255, may be nullptr.
   * @param b a pointer filled in with the blue color value in the range of
   *          0-255, may be nullptr.
   * @param a a pointer filled in with the alpha value in the range of 0-255,
   *          may be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetColor(Color)
   */
  void GetColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a) const
  {
    CheckError(TTF_GetTextColor(get(), r, g, b, a));
  }

  /**
   * Get the color of a text object.
   *
   * @param r a pointer filled in with the red color value, normally in the
   *          range of 0-1, may be nullptr.
   * @param g a pointer filled in with the green color value, normally in the
   *          range of 0-1, may be nullptr.
   * @param b a pointer filled in with the blue color value, normally in the
   *          range of 0-1, may be nullptr.
   * @param a a pointer filled in with the alpha value in the range of 0-1, may
   *          be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetColor(FColor)
   */
  void GetColor(float* r, float* g, float* b, float* a) const
  {
    CheckError(TTF_GetTextColorFloat(get(), r, g, b, a));
  }

  /**
   * Set the position of a text object.
   *
   * This can be used to position multiple text objects within a single wrapping
   * text area.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param p the x, y offset of the upper left corner of this text in pixels.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.GetPosition
   */
  bool SetPosition(Point p) { return TTF_SetTextPosition(get(), p.x, p.y); }

  /**
   * Get the position of a text object.
   *
   * @returns a Point with the offset of the upper left corner of this text in
   *          pixels on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetPosition()
   */
  Point GetPosition() const
  {
    Point p;
    GetPosition(&p.x, &p.y);
    return p;
  }

  /**
   * Get the position of a text object.
   *
   * @param x a pointer filled in with the x offset of the upper left corner of
   *          this text in pixels, may be nullptr.
   * @param y a pointer filled in with the y offset of the upper left corner of
   *          this text in pixels, may be nullptr.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.SetPosition
   */
  bool GetPosition(int* x, int* y) const
  {
    return TTF_GetTextPosition(get(), x, y);
  }

  /**
   * Set whether wrapping is enabled on a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param wrap_width the maximum width in pixels, 0 to wrap on newline
   *                   characters.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.GetWrapWidth
   */
  void SetWrapWidth(int wrap_width)
  {
    CheckError(TTF_SetTextWrapWidth(get(), wrap_width));
  }

  /**
   * Get whether wrapping is enabled on a text object.
   *
   * @returns an int with the maximum width in pixels or 0 if the text is being
   *          wrapped on newline characters on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.SetWrapWidth
   */
  int GetWrapWidth() const
  {
    int w;
    CheckError(TTF_GetTextWrapWidth(get(), &w));
    return w;
  }

  /**
   * Set whether whitespace should be visible when wrapping a text object.
   *
   * If the whitespace is visible, it will take up space for purposes of
   * alignment and wrapping. This is good for editing, but looks better when
   * centered or aligned if whitespace around line wrapping is hidden. This
   * defaults false.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param visible true to show whitespace when wrapping text, false to hide
   *                it.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.IsWrapWhitespaceVisible
   */
  void SetWrapWhitespaceVisible(bool visible)
  {
    CheckError(TTF_SetTextWrapWhitespaceVisible(get(), visible));
  }

  /**
   * Return whether whitespace is shown when wrapping a text object.
   *
   * @returns true if whitespace is shown when wrapping text, or false
   *          otherwise.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.SetWrapWhitespaceVisible
   */
  bool IsWrapWhitespaceVisible() const
  {
    return TTF_TextWrapWhitespaceVisible(get());
  }

  /**
   * Set the UTF-8 text used by a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param string the UTF-8 text to use.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.AppendString
   * @sa TextRef.DeleteString
   * @sa TextRef.InsertString
   */
  void SetString(std::string_view string)
  {
    CheckError(TTF_SetTextString(get(), string.data(), string.size()));
  }

  /**
   * Insert UTF-8 text into a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param offset the offset, in bytes, from the beginning of the string if >=
   *               0, the offset from the end of the string if < 0. Note that
   *               this does not do UTF-8 validation, so you should only insert
   *               at UTF-8 sequence boundaries.
   * @param string the UTF-8 text to insert.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.AppendString
   * @sa TextRef.DeleteString
   * @sa TextRef.SetString
   */
  void InsertString(int offset, std::string_view string)
  {
    CheckError(
      TTF_InsertTextString(get(), offset, string.data(), string.size()));
  }

  /**
   * Append UTF-8 text to a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param string the UTF-8 text to insert.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.DeleteString
   * @sa TextRef.InsertString
   * @sa TextRef.SetString
   */
  void AppendString(std::string_view string)
  {
    CheckError(TTF_AppendTextString(get(), string.data(), string.size()));
  }

  /**
   * Delete UTF-8 text from a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param offset the offset, in bytes, from the beginning of the string if >=
   *               0, the offset from the end of the string if < 0. Note that
   *               this does not do UTF-8 validation, so you should only delete
   *               at UTF-8 sequence boundaries.
   * @param length the length of text to delete, in bytes, or -1 for the
   *               remainder of the string.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextRef.AppendString
   * @sa TextRef.InsertString
   * @sa TextRef.SetString
   */
  void DeleteString(int offset, int length = -1)
  {
    CheckError(TTF_DeleteTextString(get(), offset, length));
  }

  /**
   * Get the size of a text object.
   *
   * The size of the text may change when the font or font style and size
   * change.
   *
   * @returns Point containing the size on x and y axis on success or
   *          std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @see GetSize(int*,int*) if might want only one of the coordinates
   */
  Point GetSize() const
  {
    Point p;
    GetSize(&p.x, &p.y);
    return p;
  }

  /**
   * Get the size of a text object.
   *
   * The size of the text may change when the font or font style and size
   * change.
   *
   * @param w a pointer filled in with the width of the text, in pixels, may be
   *          nullptr.
   * @param h a pointer filled in with the height of the text, in pixels, may be
   *          nullptr.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetSize(int* w, int* h) const
  {
    CheckError(TTF_GetTextSize(get(), w, h));
  }

  /**
   * Get the substring of a text object that surrounds a text offset.
   *
   * If `offset` is less than 0, this will return a zero length substring at the
   * beginning of the text with the SUBSTRING_TEXT_START flag set. If `offset`
   * is greater than or equal to the length of the text string, this will return
   * a zero length substring at the end of the text with the SUBSTRING_TEXT_END
   * flag set.
   *
   * @param offset a byte offset into the text string.
   * @param substring a pointer filled in with the substring containing the
   *                  offset.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetSubString(int offset, SubString* substring) const
  {
    CheckError(TTF_GetTextSubString(get(), offset, substring));
  }

  /**
   * Get iterator to first substring
   */
  SubStringIterator begin() const;

  /**
   * Get iterator to one past last substring
   */
  SubStringIterator end() const;

  /**
   * Get iterator to substring of a text object that contains the given line.
   *
   * If `line` is greater than or equal to `text->num_lines` this will return an
   * iterator equal to end().
   *
   * @param line a zero-based line index, in the range [0 .. text->num_lines-1].
   * @returns iterator on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  SubStringIterator GetSubStringForLine(int line) const;

  /**
   * Get the substring of a text object that contains the given line.
   *
   * If `line` is less than 0, this will return a zero length substring at the
   * beginning of the text with the SUBSTRING_TEXT_START flag set. If `line` is
   * greater than or equal to `text->num_lines` this will return a zero length
   * substring at the end of the text with the SUBSTRING_TEXT_END flag set.
   *
   * @param line a zero-based line index, in the range [0 .. text->num_lines-1].
   * @param substring a pointer filled in with the substring containing the
   *                  offset.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetSubStringForLine(int line, SubString* substring) const
  {
    CheckError(TTF_GetTextSubStringForLine(get(), line, substring));
  }

  /**
   * Get all substrings of a text object.
   *
   * @returns a nullptr terminated array of substring pointers or nullptr on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  OwnArray<SubString*> GetSubStrings() const
  {
    return GetSubStringsForRange(0);
  }

  /**
   * Get the substrings of a text object that contain a range of text.
   *
   * @param offset a byte offset into the text string.
   * @param length the length of the range being queried, in bytes, or -1 for
   *               the remainder of the string.
   * @returns a nullptr terminated array of substring pointers or nullptr on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  OwnArray<SubString*> GetSubStringsForRange(int offset, int length = -1) const
  {
    int count = 0;
    auto data = TTF_GetTextSubStringsForRange(get(), offset, length, &count);
    return OwnArray<SubString*>{data, size_t(count)};
  }

  /**
   * Get the portion of a text string that is closest to a point.
   *
   * This will return the closest substring of text to the given point.
   *
   * @param p the coordinates relative to the top-left side of the text, may be
   *          outside the bounds of the text area.
   * @returns the iterator on success or false on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  SubStringIterator GetSubStringForPoint(Point p) const;

  /**
   * Get the portion of a text string that is closest to a point.
   *
   * This will return the closest substring of text to the given point.
   *
   * @param p the coordinates relative to the top-left side of the text, may be
   *          outside the bounds of the text area.
   * @param substring a pointer filled in with the closest substring of text to
   *                  the given point.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetSubStringForPoint(Point p, SubString* substring) const
  {
    CheckError(TTF_GetTextSubStringForPoint(get(), p.x, p.y, substring));
  }

  /**
   * Get the previous substring in a text object
   *
   * If called at the start of the text, this will return a zero length
   * substring with the SUBSTRING_TEXT_START flag set.
   *
   * @param substring the SubString to query.
   * @param previous a pointer filled in with the previous substring.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetPreviousSubString(const SubString& substring,
                            SubString* previous) const
  {
    CheckError(TTF_GetPreviousTextSubString(get(), &substring, previous));
  }

  /**
   * Get the next substring in a text object
   *
   * If called at the end of the text, this will return a zero length substring
   * with the SUBSTRING_TEXT_END flag set.
   *
   * @param substring the SubString to query.
   * @param next a pointer filled in with the next substring.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetNextSubString(const SubString& substring, SubString* next) const
  {
    CheckError(TTF_GetNextTextSubString(get(), &substring, next));
  }

  /**
   * Update the layout of a text object.
   *
   * This is automatically done when the layout is requested or the text is
   * rendered, but you can call this if you need more control over the timing of
   * when the layout and text engine representation are updated.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void Update() { CheckError(TTF_UpdateText(get())); }

  /**
   * A copy of the UTF-8 string that this text object represents, useful for
   * layout, debugging and retrieving substring text
   */
  const char* GetText() const { return get()->text; }

  /**
   * The number of lines in the text, 0 if it's empty
   */
  int GetNumLines() const { return get()->num_lines; }

  /**
   * Destroy a text object created by a text engine.
   *
   * @param resource the text to destroy.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.Create
   */
  static void reset(TTF_Text* resource) { TTF_DestroyText(resource); }
};

/**
 * Handle to an owned text
 *
 * @cat resource
 *
 * @sa TextRef
 */
struct Text : ResourceUnique<TextRef>
{
  using ResourceUnique::ResourceUnique;

  /**
   * Create a text object from UTF-8 text and a text engine.
   *
   * @param engine the text engine to use when creating the text object, may be
   *               nullptr.
   * @param font the font to render with.
   * @param text the text to use, in UTF-8 encoding.
   * @returns a TextRef object or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font and text engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.Destroy
   */
  static Text Create(TextEngineRef engine, FontRef font, std::string_view text)
  {
    return Text(TTF_CreateText(engine, font, text.data(), text.size()));
  }

  /**
   * Destroy a text object created by a text engine.
   *
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.Create
   */
  void Destroy() { reset(); }
};

/**
 * Unsafe Handle to text
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa TextRef
 */
struct TextUnsafe : ResourceUnsafe<TextRef>
{
  using ResourceUnsafe::ResourceUnsafe;

  /**
   * Constructs TextUnsafe from Text.
   */
  constexpr explicit TextUnsafe(Text&& other)
    : TextUnsafe(other.release())
  {
  }
};

/**
 * Iterator for substrings
 *
 */
class SubStringIterator
{
  TextRef m_text;

  SubString m_subString;

  constexpr SubStringIterator(TextRef text)
    : m_text(std::move(text))
    , m_subString(0)
  {
  }

public:
  /// Default constructor.
  constexpr SubStringIterator()
    : SubStringIterator(nullptr)
  {
  }

  /// True if pointing to valid SubString.
  constexpr operator bool() const { return m_text != nullptr; }

  /// Retrieve SubString
  constexpr const SubString& operator*() const { return m_subString; }

  /// Retrieve SubString.
  constexpr const SubString* operator->() const { return &m_subString; }

  /// Comparison.
  constexpr bool operator==(const SubStringIterator& other) const
  {
    return m_subString.offset == other.m_subString.offset;
  }

  /// Increment operator.
  constexpr SubStringIterator& operator++()
  {
    m_text.GetNextSubString(m_subString, &m_subString);
    return *this;
  }

  /// Increment operator.
  constexpr SubStringIterator operator++(int)
  {
    auto curr = *this;
    m_text.GetNextSubString(m_subString, &m_subString);
    return curr;
  }

  /// Decrement operator.
  constexpr SubStringIterator& operator--()
  {
    m_text.GetPreviousSubString(m_subString, &m_subString);
    return *this;
  }

  /// Decrement operator.
  constexpr SubStringIterator operator--(int)
  {
    auto curr = *this;
    m_text.GetPreviousSubString(m_subString, &m_subString);
    return curr;
  }

  friend class TextRef;
};

namespace prop::RendererTextEngine {

#if SDL_TTF_VERSION_ATLEAST(3, 2, 3)

constexpr auto RENDERER_POINTER =
  TTF_PROP_RENDERER_TEXT_ENGINE_RENDERER_POINTER;

constexpr auto ATLAS_TEXTURE_SIZE_NUMBER =
  TTF_PROP_RENDERER_TEXT_ENGINE_ATLAS_TEXTURE_SIZE_NUMBER;

#else

constexpr auto RENDERER_POINTER = TTF_PROP_RENDERER_TEXT_ENGINE_RENDERER;

constexpr auto ATLAS_TEXTURE_SIZE_NUMBER =
  TTF_PROP_RENDERER_TEXT_ENGINE_ATLAS_TEXTURE_SIZE;

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 3)

} // namespace prop::RendererTextEngine

namespace prop::GpuTextEngine {

#if SDL_TTF_VERSION_ATLEAST(3, 2, 3)

constexpr auto DEVICE_POINTER = TTF_PROP_GPU_TEXT_ENGINE_DEVICE_POINTER;

constexpr auto ATLAS_TEXTURE_SIZE_NUMBER =
  TTF_PROP_GPU_TEXT_ENGINE_ATLAS_TEXTURE_SIZE_NUMBER;

#else

constexpr auto DEVICE_POINTER = TTF_PROP_GPU_TEXT_ENGINE_DEVICE;

constexpr auto ATLAS_TEXTURE_SIZE_NUMBER =
  TTF_PROP_GPU_TEXT_ENGINE_ATLAS_TEXTURE_SIZE;

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 3)

} // namespace prop::GpuTextEngine

/**
 * Deinitialize SDL_ttf.
 *
 * You must call this when done with the library, to free internal resources.
 * It is safe to call this when the library isn't initialized, as it will just
 * return immediately.
 *
 * Once you have as many quit calls as you have had successful calls to
 * InitSubSystem(TtfInitFlag), the library will actually deinitialize.
 *
 * Please note that this does not automatically close any fonts that are still
 * open at the time of deinitialization, and it is possibly not safe to close
 * them afterwards, as parts of the library will no longer be initialized to
 * deal with it.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void QuitSubSystem(TtfInitFlag _) { TTF_Quit(); }

/**
 * Check if SDL_ttf is initialized.
 *
 * This reports the number of times the library has been initialized by a call
 * to InitSubSystem(TtfInitFlag), without a paired deinitialization request from
 * QuitSubSystem(TtfInitFlag).
 *
 * In short: if it's greater than zero, the library is currently initialized
 * and ready to work. If zero, it is not initialized.
 *
 * Despite the return value being a signed integer, this function should not
 * return a negative number.
 *
 * @returns the current number of initialization calls, that need to
 *          eventually be paired with this many calls to
 *          QuitSubSystem(TtfInitFlag).
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa InitSubSystem(TtfInitFlag)
 * @sa QuitSubSystem(TtfInitFlag)
 */
inline int WasInit(TtfInitFlag _) { return TTF_WasInit(); }

/// @}

#pragma region impl

inline SubStringIterator TextRef::begin() const
{
  SubStringIterator it{get()};
  GetSubString(0, &it.m_subString);
  return it;
}

inline SubStringIterator TextRef::end() const
{
  SubStringIterator it{get()};
  GetSubString(INT_MAX, &it.m_subString);
  return it;
}

inline SubStringIterator TextRef::GetSubStringForLine(int line) const
{
  SubStringIterator it{get()};
  GetSubStringForLine(line, &it.m_subString);
  return it;
}

inline SubStringIterator TextRef::GetSubStringForPoint(Point p) const
{
  SubStringIterator it{get()};
  GetSubStringForPoint(p, &it.m_subString);
  return it;
}

#pragma endregion impl

} // namespace SDL

#endif // defined(SDL3PP_ENABLE_TTF) || defined(SDL3PP_DOC)

#endif /* SDL3PP_TTF_H_ */
