#ifndef SDL3PP_TTF_H_
#define SDL3PP_TTF_H_

#include "SDL3pp_error.h"
#include "SDL3pp_init.h"
#include "SDL3pp_objectWrapper.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_render.h"
#include "SDL3pp_stringParam.h"
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

// Forward decl
template<ObjectBox<TTF_Font*> T>
struct FontBase;

/**
 * Handle to a non owned font
 *
 * @cat resource
 *
 * @sa FontBase
 * @sa Font
 */
using FontRef = FontBase<ObjectRef<TTF_Font>>;

/**
 * Handle to an owned font
 *
 * @cat resource
 *
 * @sa FontBase
 * @sa FontRef
 */
using Font = FontBase<ObjectUnique<TTF_Font>>;

/**
 * Flag to init TTF
 *
 */
constexpr struct TtfInitFlag : InitFlagsExtra
{
} INIT_TTF;

/**
 * Font style flags for TTF_Font
 *
 * These are the flags which can be used to set the style of a font in
 * SDL_ttf. A combination of these flags can be used with functions that set
 * or query font style, such as TTF_SetFontStyle or TTF_GetFontStyle.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SetFontStyle
 * @sa TTF_GetFontStyle
 */
using FontStyleFlags = TTF_FontStyleFlags;

/**
 * Hinting flags for TTF (TrueType Fonts)
 *
 * This enum specifies the level of hinting to be applied to the font
 * rendering. The hinting level determines how much the font's outlines are
 * adjusted for better alignment on the pixel grid.
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SetFontHinting
 * @sa TTF_GetFontHinting
 */
using HintingFlags = TTF_HintingFlags;

/**
 * The horizontal alignment used when rendering wrapped text.
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 */
using HorizontalAlignment = TTF_HorizontalAlignment;

/**
 * Direction flags
 *
 * The values here are chosen to match
 * [hb_direction_t](https://harfbuzz.github.io/harfbuzz-hb-common.html#hb-direction-t).
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SetFontDirection
 */
using Direction = TTF_Direction;

/**
 * The type of data in a glyph image
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 */
using ImageType = TTF_ImageType;

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
 * TTF_Init() should be called before calling this function.
 *
 * @param major to be filled in with the major version number. Can be NULL.
 * @param minor to be filled in with the minor version number. Can be NULL.
 * @param patch to be filled in with the param version number. Can be NULL.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_Init
 */
inline void GetFreeTypeVersion(int* major, int* minor, int* patch)
{
  return TTF_GetFreeTypeVersion(major, minor, patch);
}

/**
 * Query the version of the HarfBuzz library in use.
 *
 * If HarfBuzz is not available, the version reported is 0.0.0.
 *
 * @param major to be filled in with the major version number. Can be NULL.
 * @param minor to be filled in with the minor version number. Can be NULL.
 * @param patch to be filled in with the param version number. Can be NULL.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void GetHarfBuzzVersion(int* major, int* minor, int* patch)
{
  return TTF_GetHarfBuzzVersion(major, minor, patch);
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
 */
template<ObjectBox<TTF_Font*> T>
struct FontBase : T
{
  using T::T;

  /**
   * Create a font from a file, using a specified point size.
   *
   * Some .fon fonts will have several sizes embedded in the file, so the point
   * size becomes the index of choosing which size. If the value is too high,
   * the last indexed size will be the default.
   *
   * When done with the returned TTF_Font, use TTF_CloseFont() to dispose of it.
   *
   * @param file path to font file.
   * @param ptsize point size to use for the newly-opened font.
   * @post a valid TTF_Font, or NULL on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_CloseFont
   */
  FontBase(StringParam file, float ptsize)
    : T(TTF_OpenFont(file, ptsize))
  {
  }

  /**
   * Create a font from an SDL_IOStream, using a specified point size.
   *
   * Some .fon fonts will have several sizes embedded in the file, so the point
   * size becomes the index of choosing which size. If the value is too high,
   * the last indexed size will be the default.
   *
   * If `closeio` is true, `src` will be automatically closed once the font is
   * closed. Otherwise you should close `src` yourself after closing the font.
   *
   * When done with the returned TTF_Font, use TTF_CloseFont() to dispose of it.
   *
   * @param src an SDL_IOStream to provide a font file's data.
   * @param closeio true to close `src` when the font is closed, false to leave
   *                it open.
   * @param ptsize point size to use for the newly-opened font.
   * @post a valid TTF_Font, or NULL on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_CloseFont
   */
  FontBase(ObjectBox<SDL_IOStream> auto&& src, bool closeio, float ptsize)
    : T(TTF_OpenFontIO(src, closeio, ptsize))
  {
  }

  /**
   * Create a font with the specified properties.
   *
   * These are the supported properties:
   *
   * - `TTF_PROP_FONT_CREATE_FILENAME_STRING`: the font file to open, if an
   *   SDL_IOStream isn't being used. This is required if
   *   `TTF_PROP_FONT_CREATE_IOSTREAM_POINTER` and
   *   `TTF_PROP_FONT_CREATE_EXISTING_FONT` aren't set.
   * - `TTF_PROP_FONT_CREATE_IOSTREAM_POINTER`: an SDL_IOStream containing the
   *   font to be opened. This should not be closed until the font is closed.
   *   This is required if `TTF_PROP_FONT_CREATE_FILENAME_STRING` and
   *   `TTF_PROP_FONT_CREATE_EXISTING_FONT` aren't set.
   * - `TTF_PROP_FONT_CREATE_IOSTREAM_OFFSET_NUMBER`: the offset in the iostream
   *   for the beginning of the font, defaults to 0.
   * - `TTF_PROP_FONT_CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN`: true if closing the
   *   font should also close the associated SDL_IOStream.
   * - `TTF_PROP_FONT_CREATE_SIZE_FLOAT`: the point size of the font. Some .fon
   *   fonts will have several sizes embedded in the file, so the point size
   *   becomes the index of choosing which size. If the value is too high, the
   *   last indexed size will be the default.
   * - `TTF_PROP_FONT_CREATE_FACE_NUMBER`: the face index of the font, if the
   *   font contains multiple font faces.
   * - `TTF_PROP_FONT_CREATE_HORIZONTAL_DPI_NUMBER`: the horizontal DPI to use
   *   for font rendering, defaults to
   *   `TTF_PROP_FONT_CREATE_VERTICAL_DPI_NUMBER` if set, or 72 otherwise.
   * - `TTF_PROP_FONT_CREATE_VERTICAL_DPI_NUMBER`: the vertical DPI to use for
   *   font rendering, defaults to `TTF_PROP_FONT_CREATE_HORIZONTAL_DPI_NUMBER`
   *   if set, or 72 otherwise.
   * - `TTF_PROP_FONT_CREATE_EXISTING_FONT`: an optional TTF_Font that, if set,
   *   will be used as the font data source and the initial size and style of
   *   the new font.
   *
   * @param props the properties to use.
   * @post a valid TTF_Font, or NULL on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_CloseFont
   */
  FontBase(PropertiesRef props)
    : T(TTF_OpenFontWithProperties(props))
  {
  }

  /**
   * Create a copy of an existing font.
   *
   * The copy will be distinct from the original, but will share the font file
   * and have the same size and style as the original.
   *
   * When done with the returned TTF_Font, use TTF_CloseFont() to dispose of it.
   *
   * @returns a valid TTF_Font, or NULL on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               original font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_CloseFont
   */
  Font Copy() const { return TTF_CopyFont(T::get()); }

  /**
   * Get the properties associated with a font.
   *
   * The following read-write properties are provided by SDL:
   *
   * - `TTF_PROP_FONT_OUTLINE_LINE_CAP_NUMBER`: The FT_Stroker_LineCap value
   *   used when setting the font outline, defaults to
   *   `FT_STROKER_LINECAP_ROUND`.
   * - `TTF_PROP_FONT_OUTLINE_LINE_JOIN_NUMBER`: The FT_Stroker_LineJoin value
   *   used when setting the font outline, defaults to
   *   `FT_STROKER_LINEJOIN_ROUND`.
   * - `TTF_PROP_FONT_OUTLINE_MITER_LIMIT_NUMBER`: The FT_Fixed miter limit used
   *   when setting the font outline, defaults to 0.
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  PropertiesRef GetProperties() { return TTF_GetFontProperties(T::get()); }

  /**
   * Get the font generation.
   *
   * The generation is incremented each time font properties change that require
   * rebuilding glyphs, such as style, size, etc.
   *
   * @returns the font generation or 0 on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Uint32 GetGeneration() const { return TTF_GetFontGeneration(T::get()); }

  /**
   * Add a fallback font.
   *
   * Add a font that will be used for glyphs that are not in the current font.
   * The fallback font should have the same size and style as the current font.
   *
   * If there are multiple fallback fonts, they are used in the order added.
   *
   * This updates any TTF_Text objects using this font.
   *
   * @param fallback the font to add as a fallback.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created
   *               both fonts.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_ClearFallbackFonts
   * @sa TTF_RemoveFallbackFont
   */
  bool AddFallback(FontRef fallback)
  {
    return TTF_AddFallbackFont(T::get(), fallback);
  }

  /**
   * Remove a fallback font.
   *
   * This updates any TTF_Text objects using this font.
   *
   * @param fallback the font to remove as a fallback.
   *
   * @threadsafety This function should be called on the thread that created
   *               both fonts.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_AddFallbackFont
   * @sa TTF_ClearFallbackFonts
   */
  void RemoveFallback(FontRef fallback)
  {
    return TTF_RemoveFallbackFont(T::get(), fallback);
  }

  /**
   * Remove all fallback fonts.
   *
   * This updates any TTF_Text objects using this font.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_AddFallbackFont
   * @sa TTF_RemoveFallbackFont
   */
  void ClearFallbacks() { return TTF_ClearFallbackFonts(T::get()); }

  /**
   * Set a font's size dynamically.
   *
   * This updates any TTF_Text objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param ptsize the new point size.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_GetFontSize
   */
  bool SetSize(float ptsize) { return TTF_SetFontSize(T::get(), ptsize); }

  /**
   * Set font size dynamically with target resolutions, in dots per inch.
   *
   * This updates any TTF_Text objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param ptsize the new point size.
   * @param hdpi the target horizontal DPI.
   * @param vdpi the target vertical DPI.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_GetFontSize
   * @sa TTF_GetFontSizeDPI
   */
  bool SetSizeDPI(float ptsize, int hdpi, int vdpi)
  {
    return TTF_SetFontSizeDPI(T::get(), ptsize, hdpi, vdpi);
  }

  /**
   * Get the size of a font.
   *
   * @returns the size of the font, or 0.0f on failure; call SDL_GetError() for
   *          more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_SetFontSize
   * @sa TTF_SetFontSizeDPI
   */
  float GetSize() const { return TTF_GetFontSize(T::get()); }

  /**
   * Get font target resolutions, in dots per inch.
   *
   * @param hdpi a pointer filled in with the target horizontal DPI.
   * @param vdpi a pointer filled in with the target vertical DPI.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_SetFontSizeDPI
   */
  bool GetDPI(int* hdpi, int* vdpi) const
  {
    return TTF_GetFontDPI(T::get(), hdpi, vdpi);
  }

  /**
   * Set a font's current style.
   *
   * This updates any TTF_Text objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * The font styles are a set of bit flags, OR'd together:
   *
   * - `TTF_STYLE_NORMAL` (is zero)
   * - `TTF_STYLE_BOLD`
   * - `TTF_STYLE_ITALIC`
   * - `TTF_STYLE_UNDERLINE`
   * - `TTF_STYLE_STRIKETHROUGH`
   *
   * @param style the new style values to set, OR'd together.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_GetFontStyle
   */
  void SetStyle(FontStyleFlags style)
  {
    return TTF_SetFontStyle(T::get(), style);
  }

  /**
   * Query a font's current style.
   *
   * The font styles are a set of bit flags, OR'd together:
   *
   * - `TTF_STYLE_NORMAL` (is zero)
   * - `TTF_STYLE_BOLD`
   * - `TTF_STYLE_ITALIC`
   * - `TTF_STYLE_UNDERLINE`
   * - `TTF_STYLE_STRIKETHROUGH`
   *
   * @returns the current font style, as a set of bit flags.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_SetFontStyle
   */
  FontStyleFlags GetStyle() const { return TTF_GetFontStyle(T::get()); }

  /**
   * Set a font's current outline.
   *
   * This uses the font properties `TTF_PROP_FONT_OUTLINE_LINE_CAP_NUMBER`,
   * `TTF_PROP_FONT_OUTLINE_LINE_JOIN_NUMBER`, and
   * `TTF_PROP_FONT_OUTLINE_MITER_LIMIT_NUMBER` when setting the font outline.
   *
   * This updates any TTF_Text objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param outline positive outline value, 0 to default.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_GetFontOutline
   */
  bool SetOutline(int outline) { return TTF_SetFontOutline(T::get(), outline); }

  /**
   * Query a font's current outline.
   *
   * @returns the font's current outline value.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_SetFontOutline
   */
  int GetOutline() const { return TTF_GetFontOutline(T::get()); }

  /**
   * Set a font's current hinter setting.
   *
   * This updates any TTF_Text objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * The hinter setting is a single value:
   *
   * - `TTF_HINTING_NORMAL`
   * - `TTF_HINTING_LIGHT`
   * - `TTF_HINTING_MONO`
   * - `TTF_HINTING_NONE`
   * - `TTF_HINTING_LIGHT_SUBPIXEL` (available in SDL_ttf 3.0.0 and later)
   *
   * @param hinting the new hinter setting.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_GetFontHinting
   */
  void SetHinting(HintingFlags hinting)
  {
    return TTF_SetFontHinting(T::get(), hinting);
  }

  /**
   * Query the number of faces of a font.
   *
   * @returns the number of FreeType font faces.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  int GetNumFaces() const { return TTF_GetNumFontFaces(T::get()); }

  /**
   * Query a font's current FreeType hinter setting.
   *
   * The hinter setting is a single value:
   *
   * - `TTF_HINTING_NORMAL`
   * - `TTF_HINTING_LIGHT`
   * - `TTF_HINTING_MONO`
   * - `TTF_HINTING_NONE`
   * - `TTF_HINTING_LIGHT_SUBPIXEL` (available in SDL_ttf 3.0.0 and later)
   *
   * @returns the font's current hinter value.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_SetFontHinting
   */
  HintingFlags GetHinting() const { return TTF_GetFontHinting(T::get()); }

  /**
   * Enable Signed Distance Field rendering for a font.
   *
   * SDF is a technique that helps fonts look sharp even when scaling and
   * rotating, and requires special shader support for display.
   *
   * This works with Blended APIs, and generates the raw signed distance values
   * in the alpha channel of the resulting texture.
   *
   * This updates any TTF_Text objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param enabled true to enable SDF, false to disable.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_GetFontSDF
   */
  bool SetSDF(bool enabled) { return TTF_SetFontSDF(T::get(), enabled); }

  /**
   * Query whether Signed Distance Field rendering is enabled for a font.
   *
   * @returns true if enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_SetFontSDF
   */
  bool GetSDF() const { return TTF_GetFontSDF(T::get()); }

  /**
   * Set a font's current wrap alignment option.
   *
   * This updates any TTF_Text objects using this font.
   *
   * @param align the new wrap alignment option.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_GetFontWrapAlignment
   */
  void SetWrapAlignment(HorizontalAlignment align)
  {
    return TTF_SetFontWrapAlignment(T::get(), align);
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
   * @sa TTF_SetFontWrapAlignment
   */
  HorizontalAlignment GetWrapAlignment() const
  {
    return TTF_GetFontWrapAlignment(T::get());
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
  int GetHeight() const { return TTF_GetFontHeight(T::get()); }

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
  int GetAscent() const { return TTF_GetFontAscent(T::get()); }

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
  int GetDescent() const { return TTF_GetFontDescent(T::get()); }

  /**
   * Set the spacing between lines of text for a font.
   *
   * This updates any TTF_Text objects using this font.
   *
   * @param lineskip the new line spacing for the font.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_GetFontLineSkip
   */
  void SetLineSkip(int lineskip)
  {
    return TTF_SetFontLineSkip(T::get(), lineskip);
  }

  /**
   * Query the spacing between lines of text for a font.
   *
   * @returns the font's recommended spacing.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_SetFontLineSkip
   */
  int GetLineSkip() const { return TTF_GetFontLineSkip(T::get()); }

  /**
   * Set if kerning is enabled for a font.
   *
   * Newly-opened fonts default to allowing kerning. This is generally a good
   * policy unless you have a strong reason to disable it, as it tends to
   * produce better rendering (with kerning disabled, some fonts might render
   * the word `kerning` as something that looks like `keming` for example).
   *
   * This updates any TTF_Text objects using this font.
   *
   * @param enabled true to enable kerning, false to disable.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_GetFontKerning
   */
  void SetKerning(bool enabled)
  {
    return TTF_SetFontKerning(T::get(), enabled);
  }

  /**
   * Query whether or not kerning is enabled for a font.
   *
   * @returns true if kerning is enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_SetFontKerning
   */
  bool GetKerning() const { return TTF_GetFontKerning(T::get()); }

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
  bool IsFixedWidth() const { return TTF_FontIsFixedWidth(T::get()); }

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
   * @sa TTF_SetFontSDF
   */
  bool IsScalable() const { return TTF_FontIsScalable(T::get()); }

  /**
   * Query a font's family name.
   *
   * This string is dictated by the contents of the font file.
   *
   * Note that the returned string is to internal storage, and should not be
   * modified or free'd by the caller. The string becomes invalid, with the rest
   * of the font, when `font` is handed to TTF_CloseFont().
   *
   * @returns the font's family name.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  const char* GetFamilyName() const { return TTF_GetFontFamilyName(T::get()); }

  /**
   * Query a font's style name.
   *
   * This string is dictated by the contents of the font file.
   *
   * Note that the returned string is to internal storage, and should not be
   * modified or free'd by the caller. The string becomes invalid, with the rest
   * of the font, when `font` is handed to TTF_CloseFont().
   *
   * @returns the font's style name.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  const char* GetStyleName() const { return TTF_GetFontStyleName(T::get()); }

  /**
   * Set the direction to be used for text shaping by a font.
   *
   * This function only supports left-to-right text shaping if SDL_ttf was not
   * built with HarfBuzz support.
   *
   * This updates any TTF_Text objects using this font.
   *
   * @param direction the new direction for text to flow.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool SetDirection(Direction direction)
  {
    return TTF_SetFontDirection(T::get(), direction);
  }

  /**
   * Get the direction to be used for text shaping by a font.
   *
   * This defaults to TTF_DIRECTION_INVALID if it hasn't been set.
   *
   * @returns the direction to be used for text shaping.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Direction GetDirection() const { return TTF_GetFontDirection(T::get()); }

  /**
   * Set the script to be used for text shaping by a font.
   *
   * This returns false if SDL_ttf isn't built with HarfBuzz support.
   *
   * This updates any TTF_Text objects using this font.
   *
   * @param script an
   * [ISO 15924 code](https://unicode.org/iso15924/iso15924-codes.html).
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_StringToTag
   */
  bool SetScript(Uint32 script) { return TTF_SetFontScript(T::get(), script); }

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
   * @sa TTF_TagToString
   */
  Uint32 GetScript() const { return TTF_GetFontScript(T::get()); }

  /**
   * Get the script used by a 32-bit codepoint.
   *
   * @param ch the character code to check.
   * @returns an
   *          [ISO 15924 code](https://unicode.org/iso15924/iso15924-codes.html)
   *          on success, or 0 on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function is thread-safe.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_TagToString
   */
  Uint32 GetGlyphScript(Uint32 ch) const
  {
    return TTF_GetGlyphScript(T::get(), ch);
  }

  /**
   * Set language to be used for text shaping by a font.
   *
   * If SDL_ttf was not built with HarfBuzz support, this function returns
   * false.
   *
   * This updates any TTF_Text objects using this font.
   *
   * @param language_bcp47 a null-terminated string containing the desired
   *                       language's BCP47 code. Or null to reset the value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool SetLanguage(StringParam language_bcp47)
  {
    return TTF_SetFontLanguage(T::get(), language_bcp47);
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
  bool HasGlyph(Uint32 ch) const { return TTF_FontHasGlyph(T::get(), ch); }

  /**
   * Get the pixel image for a UNICODE codepoint.
   *
   * @param ch the codepoint to check.
   * @param image_type a pointer filled in with the glyph image type, may be
   *                   NULL.
   * @returns an SDL_Surface containing the glyph, or NULL on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  SurfaceRef GetGlyphImage(Uint32 ch, ImageType* image_type) const
  {
    return TTF_GetGlyphImage(T::get(), ch, image_type);
  }

  /**
   * Get the pixel image for a character index.
   *
   * This is useful for text engine implementations, which can call this with
   * the `glyph_index` in a TTF_CopyOperation
   *
   * @param glyph_index the index of the glyph to return.
   * @param image_type a pointer filled in with the glyph image type, may be
   *                   NULL.
   * @returns an SDL_Surface containing the glyph, or NULL on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  SurfaceRef GetGlyphImageForIndex(Uint32 glyph_index,
                                   ImageType* image_type) const
  {
    return TTF_GetGlyphImageForIndex(T::get(), glyph_index, image_type);
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool GetGlyphMetrics(Uint32 ch,
                       int* minx,
                       int* maxx,
                       int* miny,
                       int* maxy,
                       int* advance) const
  {
    return TTF_GetGlyphMetrics(T::get(), ch, minx, maxx, miny, maxy, advance);
  }

  /**
   * Query the kerning size between the glyphs of two UNICODE codepoints.
   *
   * @param previous_ch the previous codepoint.
   * @param ch the current codepoint.
   * @returns the kerning size between the two glyphs, in pixels, on success or
   * false on failure; call GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  std::optional<int> GetGlyphKerning(Uint32 previous_ch, Uint32 ch) const
  {
    if (int r; TTF_GetGlyphKerning(T::get(), previous_ch, ch, &r)) return r;
    return std::nullopt;
  }

  /**
   * Calculate the dimensions of a rendered string of UTF-8 text.
   *
   * This will report the width and height, in pixels, of the space that the
   * specified string will take to fully render.
   *
   * @param text text to calculate, in UTF-8 encoding.
   * @returns return a Point with the width, height on success or std::nullopt
   *          on failure; call GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  std::optional<Point> GetStringSize(std::string_view text) const
  {
    if (Point p; GetStringSize(text, &p.x, &p.y)) return p;
    return std::nullopt;
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool GetStringSize(std::string_view text, int* w, int* h) const
  {
    return TTF_GetStringSize(T::get(), text.data(), text.size(), w, h);
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
   * @returns return a Point with the width, height on success or std::nullopt
   *          on failure; call GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  std::optional<Point> GetStringSizeWrapped(std::string_view text,
                                            int wrap_width) const
  {
    if (Point p; GetStringSizeWrapped(text, wrap_width, &p.x, &p.y)) return p;
    return std::nullopt;
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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool GetStringSizeWrapped(std::string_view text,
                            int wrap_width,
                            int* w,
                            int* h) const
  {
    return TTF_GetStringSizeWrapped(
      T::get(), text.data(), text.size(), wrap_width, w, h);
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
   *                       string that will fit, may be NULL.
   * @param measured_length a pointer filled in with the length, in bytes, of
   *                        the string that will fit, may be NULL.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool MeasureString(std::string_view text,
                     int max_width,
                     int* measured_width,
                     size_t* measured_length) const
  {
    return TTF_MeasureString(T::get(),
                             text.data(),
                             text.size(),
                             max_width,
                             measured_width,
                             measured_length);
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
   * TTF_RenderText_Solid_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with TTF_RenderText_Shaded,
   * TTF_RenderText_Blended, and TTF_RenderText_LCD.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @returns a new 8-bit, palettized surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderText_Blended
   * @sa TTF_RenderText_LCD
   * @sa TTF_RenderText_Shaded
   * @sa TTF_RenderText_Solid
   * @sa TTF_RenderText_Solid_Wrapped
   */
  Surface RenderText_Solid(std::string_view text, Color fg) const
  {
    return TTF_RenderText_Solid(T::get(), text.data(), text.size(), fg);
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
   * You can render at other quality levels with TTF_RenderText_Shaded_Wrapped,
   * TTF_RenderText_Blended_Wrapped, and TTF_RenderText_LCD_Wrapped.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param wrapLength the maximum width of the text surface or 0 to wrap on
   *                   newline characters.
   * @returns a new 8-bit, palettized surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderText_Blended_Wrapped
   * @sa TTF_RenderText_LCD_Wrapped
   * @sa TTF_RenderText_Shaded_Wrapped
   * @sa TTF_RenderText_Solid
   */
  Surface RenderText_Solid_Wrapped(std::string_view text,
                                   Color fg,
                                   int wrapLength) const
  {
    return TTF_RenderText_Solid_Wrapped(
      T::get(), text.data(), text.size(), fg, wrapLength);
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
   * You can render at other quality levels with TTF_RenderGlyph_Shaded,
   * TTF_RenderGlyph_Blended, and TTF_RenderGlyph_LCD.
   *
   * @param ch the character to render.
   * @param fg the foreground color for the text.
   * @returns a new 8-bit, palettized surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderGlyph_Blended
   * @sa TTF_RenderGlyph_LCD
   * @sa TTF_RenderGlyph_Shaded
   */
  Surface RenderGlyph_Solid(Uint32 ch, Color fg) const
  {
    return TTF_RenderGlyph_Solid(T::get(), ch, fg);
  }

  /**
   * Render UTF-8 text at high quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the specified background color, while other pixels have
   * varying degrees of the foreground color. This function returns the new
   * surface, or NULL if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * TTF_RenderText_Shaded_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with TTF_RenderText_Solid,
   * TTF_RenderText_Blended, and TTF_RenderText_LCD.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @returns a new 8-bit, palettized surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderText_Blended
   * @sa TTF_RenderText_LCD
   * @sa TTF_RenderText_Shaded_Wrapped
   * @sa TTF_RenderText_Solid
   */
  Surface RenderText_Shaded(std::string_view text, Color fg, Color bg) const
  {
    return TTF_RenderText_Shaded(T::get(), text.data(), text.size(), fg, bg);
  }

  /**
   * Render word-wrapped UTF-8 text at high quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the specified background color, while other pixels have
   * varying degrees of the foreground color. This function returns the new
   * surface, or NULL if there was an error.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with TTF_RenderText_Solid_Wrapped,
   * TTF_RenderText_Blended_Wrapped, and TTF_RenderText_LCD_Wrapped.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @param wrap_width the maximum width of the text surface or 0 to wrap on
   *                   newline characters.
   * @returns a new 8-bit, palettized surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderText_Blended_Wrapped
   * @sa TTF_RenderText_LCD_Wrapped
   * @sa TTF_RenderText_Shaded
   * @sa TTF_RenderText_Solid_Wrapped
   */
  Surface RenderText_Shaded_Wrapped(std::string_view text,
                                    Color fg,
                                    Color bg,
                                    int wrap_width) const
  {
    return TTF_RenderText_Shaded_Wrapped(
      T::get(), text.data(), text.size(), fg, bg, wrap_width);
  }

  /**
   * Render a single UNICODE codepoint at high quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the specified background color, while other pixels have
   * varying degrees of the foreground color. This function returns the new
   * surface, or NULL if there was an error.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with TTF_RenderGlyph_Solid,
   * TTF_RenderGlyph_Blended, and TTF_RenderGlyph_LCD.
   *
   * @param ch the codepoint to render.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @returns a new 8-bit, palettized surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderGlyph_Blended
   * @sa TTF_RenderGlyph_LCD
   * @sa TTF_RenderGlyph_Solid
   */
  Surface RenderGlyph_Shaded(Uint32 ch, Color fg, Color bg) const
  {
    return TTF_RenderGlyph_Shaded(T::get(), ch, fg, bg);
  }

  /**
   * Render UTF-8 text at high quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, using alpha
   * blending to dither the font with the given color. This function returns the
   * new surface, or NULL if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * TTF_RenderText_Blended_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with TTF_RenderText_Solid,
   * TTF_RenderText_Shaded, and TTF_RenderText_LCD.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @returns a new 32-bit, ARGB surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderText_Blended_Wrapped
   * @sa TTF_RenderText_LCD
   * @sa TTF_RenderText_Shaded
   * @sa TTF_RenderText_Solid
   */
  Surface RenderText_Blended(std::string_view text, Color fg) const
  {
    return TTF_RenderText_Blended(T::get(), text.data(), text.size(), fg);
  }

  /**
   * Render word-wrapped UTF-8 text at high quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, using alpha
   * blending to dither the font with the given color. This function returns the
   * new surface, or NULL if there was an error.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with TTF_RenderText_Solid_Wrapped,
   * TTF_RenderText_Shaded_Wrapped, and TTF_RenderText_LCD_Wrapped.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param wrap_width the maximum width of the text surface or 0 to wrap on
   *                   newline characters.
   * @returns a new 32-bit, ARGB surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderText_Blended
   * @sa TTF_RenderText_LCD_Wrapped
   * @sa TTF_RenderText_Shaded_Wrapped
   * @sa TTF_RenderText_Solid_Wrapped
   */
  Surface RenderText_Blended_Wrapped(std::string_view text,
                                     Color fg,
                                     int wrap_width) const
  {
    return TTF_RenderText_Blended_Wrapped(
      T::get(), text.data(), text.size(), fg, wrap_width);
  }

  /**
   * Render a single UNICODE codepoint at high quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, using alpha
   * blending to dither the font with the given color. This function returns the
   * new surface, or NULL if there was an error.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with TTF_RenderGlyph_Solid,
   * TTF_RenderGlyph_Shaded, and TTF_RenderGlyph_LCD.
   *
   * @param ch the codepoint to render.
   * @param fg the foreground color for the text.
   * @returns a new 32-bit, ARGB surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderGlyph_LCD
   * @sa TTF_RenderGlyph_Shaded
   * @sa TTF_RenderGlyph_Solid
   */
  Surface RenderGlyph_Blended(Uint32 ch, Color fg) const
  {
    return TTF_RenderGlyph_Blended(T::get(), ch, fg);
  }

  /**
   * Render UTF-8 text at LCD subpixel quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, and render
   * alpha-blended text using FreeType's LCD subpixel rendering. This function
   * returns the new surface, or NULL if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * TTF_RenderText_LCD_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with TTF_RenderText_Solid,
   * TTF_RenderText_Shaded, and TTF_RenderText_Blended.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @returns a new 32-bit, ARGB surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderText_Blended
   * @sa TTF_RenderText_LCD_Wrapped
   * @sa TTF_RenderText_Shaded
   * @sa TTF_RenderText_Solid
   */
  Surface RenderText_LCD(std::string_view text, Color fg, Color bg) const
  {
    return TTF_RenderText_LCD(T::get(), text.data(), text.size(), fg, bg);
  }

  /**
   * Render word-wrapped UTF-8 text at LCD subpixel quality to a new ARGB
   * surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, and render
   * alpha-blended text using FreeType's LCD subpixel rendering. This function
   * returns the new surface, or NULL if there was an error.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with TTF_RenderText_Solid_Wrapped,
   * TTF_RenderText_Shaded_Wrapped, and TTF_RenderText_Blended_Wrapped.
   *
   * @param text text to render, in UTF-8 encoding.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @param wrap_width the maximum width of the text surface or 0 to wrap on
   *                   newline characters.
   * @returns a new 32-bit, ARGB surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderText_Blended_Wrapped
   * @sa TTF_RenderText_LCD
   * @sa TTF_RenderText_Shaded_Wrapped
   * @sa TTF_RenderText_Solid_Wrapped
   */
  Surface RenderText_LCD_Wrapped(std::string_view text,
                                 Color fg,
                                 Color bg,
                                 int wrap_width) const
  {
    return TTF_RenderText_LCD_Wrapped(
      T::get(), text.data(), text.size(), fg, bg, wrap_width);
  }

  /**
   * Render a single UNICODE codepoint at LCD subpixel quality to a new ARGB
   * surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, and render
   * alpha-blended text using FreeType's LCD subpixel rendering. This function
   * returns the new surface, or NULL if there was an error.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with TTF_RenderGlyph_Solid,
   * TTF_RenderGlyph_Shaded, and TTF_RenderGlyph_Blended.
   *
   * @param ch the codepoint to render.
   * @param fg the foreground color for the text.
   * @param bg the background color for the text.
   * @returns a new 32-bit, ARGB surface, or NULL if there was an error.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_RenderGlyph_Blended
   * @sa TTF_RenderGlyph_Shaded
   * @sa TTF_RenderGlyph_Solid
   */
  Surface RenderGlyph_LCD(Uint32 ch, Color fg, Color bg) const
  {
    return TTF_RenderGlyph_LCD(T::get(), ch, fg, bg);
  }

  /**
   * Dispose of a previously-created font.
   *
   * Call this when done with a font. This function will free any resources
   * associated with it. It is safe to call this function on NULL, for example
   * on the result of a failed call to TTF_OpenFont().
   *
   * The font is not valid after being passed to this function. String pointers
   * from functions that return information on this font, such as
   * TTF_GetFontFamilyName() and TTF_GetFontStyleName(), are no longer valid
   * after this call, as well.
   *
   * @threadsafety This function should not be called while any other thread is
   *               using the font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TTF_OpenFont
   * @sa TTF_OpenFontIO
   */
  void Close() { T::free(); }
};

/**
 * Initialize SDL_ttf.
 *
 * You must successfully call this function before it is safe to call any
 * other function in this library.
 *
 * It is safe to call this more than once, and each successful TTF_Init() call
 * should be paired with a matching TTF_Quit() call.
 *
 * @param _ An INIT_TTF value;
 *
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_Quit
 */
inline bool InitSubSystem(TtfInitFlag _) { return TTF_Init(); }

/**
 * A text engine used to create text objects.
 *
 * This is a public interface that can be used by applications and libraries
 * to perform customize rendering with text objects. See
 * <SDL3_ttf/SDL_textengine.h> for details.
 *
 * There are three text engines provided with the library:
 *
 * - Drawing to an SDL_Surface, created with TTF_CreateSurfaceTextEngine()
 * - Drawing with an SDL 2D renderer, created with
 *   TTF_CreateRendererTextEngine()
 * - Drawing with the SDL GPU API, created with TTF_CreateGPUTextEngine()
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 */
using TextEngine = TTF_TextEngine;

constexpr HintingFlags HINTING_INVALID = TTF_HINTING_INVALID;

/**
 * Normal hinting applies standard grid-fitting.
 */
constexpr HintingFlags HINTING_NORMAL = TTF_HINTING_NORMAL;

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

constexpr HorizontalAlignment HORIZONTAL_ALIGN_INVALID =
  TTF_HORIZONTAL_ALIGN_INVALID;

constexpr HorizontalAlignment HORIZONTAL_ALIGN_LEFT = TTF_HORIZONTAL_ALIGN_LEFT;

constexpr HorizontalAlignment HORIZONTAL_ALIGN_CENTER =
  TTF_HORIZONTAL_ALIGN_CENTER;

constexpr HorizontalAlignment HORIZONTAL_ALIGN_RIGHT =
  TTF_HORIZONTAL_ALIGN_RIGHT;

constexpr Direction DIRECTION_INVALID = TTF_DIRECTION_INVALID;

/**
 * Left to Right.
 */
constexpr Direction DIRECTION_LTR = TTF_DIRECTION_LTR;

/**
 * Right to Left.
 */
constexpr Direction DIRECTION_RTL = TTF_DIRECTION_RTL;

/**
 * Top to Bottom.
 */
constexpr Direction DIRECTION_TTB = TTF_DIRECTION_TTB;

/**
 * Bottom to Top.
 */
constexpr Direction DIRECTION_BTT = TTF_DIRECTION_BTT;

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
 * @sa TTF_TagToString
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
 * @sa TTF_TagToString
 */
inline void TagToString(Uint32 tag, char* string, size_t size)
{
  return TTF_TagToString(tag, string, size);
}

constexpr ImageType IMAGE_INVALID = TTF_IMAGE_INVALID;

/**
 * The color channels are white.
 */
constexpr ImageType IMAGE_ALPHA = TTF_IMAGE_ALPHA;

/**
 * The color channels have image data.
 */
constexpr ImageType IMAGE_COLOR = TTF_IMAGE_COLOR;

/**
 * The alpha channel has signed distance field information.
 */
constexpr ImageType IMAGE_SDF = TTF_IMAGE_SDF;

/**
 * Internal data for TTF_Text
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 */
using TextData = TTF_TextData;

/**
 * Text created with TTF_CreateText()
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateText
 * @sa TTF_GetTextProperties
 * @sa TTF_DestroyText
 */
using Text = TTF_Text;

/**
 * Create a text engine for drawing text on SDL surfaces.
 *
 * @returns a TTF_TextEngine object or NULL on failure; call SDL_GetError()
 *          for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_DestroySurfaceTextEngine
 * @sa TTF_DrawSurfaceText
 */
inline TextEngine* CreateSurfaceTextEngine()
{
  return TTF_CreateSurfaceTextEngine();
}

/**
 * Draw text to an SDL surface.
 *
 * `text` must have been created using a TTF_TextEngine from
 * TTF_CreateSurfaceTextEngine().
 *
 * @param text the text to draw.
 * @param x the x coordinate in pixels, positive from the left edge towards
 *          the right.
 * @param y the y coordinate in pixels, positive from the top edge towards the
 *          bottom.
 * @param surface the surface to draw on.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateSurfaceTextEngine
 * @sa TTF_CreateText
 */
inline bool DrawSurfaceText(Text* text, int x, int y, SurfaceRef surface)
{
  return TTF_DrawSurfaceText(text, x, y, surface.get());
}

/**
 * Destroy a text engine created for drawing text on SDL surfaces.
 *
 * All text created by this engine should be destroyed before calling this
 * function.
 *
 * @param engine a TTF_TextEngine object created with
 *               TTF_CreateSurfaceTextEngine().
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateSurfaceTextEngine
 */
inline void DestroySurfaceTextEngine(TextEngine* engine)
{
  return TTF_DestroySurfaceTextEngine(engine);
}

/**
 * Create a text engine for drawing text on an SDL renderer.
 *
 * @param renderer the renderer to use for creating textures and drawing text.
 * @returns a TTF_TextEngine object or NULL on failure; call SDL_GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               renderer.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_DestroyRendererTextEngine
 * @sa TTF_DrawRendererText
 * @sa TTF_CreateRendererTextEngineWithProperties
 */
inline TextEngine* CreateRendererTextEngine(RendererRef renderer)
{
  return TTF_CreateRendererTextEngine(renderer.get());
}

/**
 * Create a text engine for drawing text on an SDL renderer, with the
 * specified properties.
 *
 * These are the supported properties:
 *
 * - `TTF_PROP_RENDERER_TEXT_ENGINE_RENDERER`: the renderer to use for
 *   creating textures and drawing text
 * - `TTF_PROP_RENDERER_TEXT_ENGINE_ATLAS_TEXTURE_SIZE`: the size of the
 *   texture atlas
 *
 * @param props the properties to use.
 * @returns a TTF_TextEngine object or NULL on failure; call SDL_GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               renderer.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateRendererTextEngine
 * @sa TTF_DestroyRendererTextEngine
 * @sa TTF_DrawRendererText
 */
inline TextEngine* CreateRendererTextEngineWithProperties(PropertiesRef props)
{
  return TTF_CreateRendererTextEngineWithProperties(props.get());
}

/**
 * Draw text to an SDL renderer.
 *
 * `text` must have been created using a TTF_TextEngine from
 * TTF_CreateRendererTextEngine(), and will draw using the renderer passed to
 * that function.
 *
 * @param text the text to draw.
 * @param x the x coordinate in pixels, positive from the left edge towards
 *          the right.
 * @param y the y coordinate in pixels, positive from the top edge towards the
 *          bottom.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateRendererTextEngine
 * @sa TTF_CreateText
 */
inline bool DrawRendererText(Text* text, float x, float y)
{
  return TTF_DrawRendererText(text, x, y);
}

/**
 * Destroy a text engine created for drawing text on an SDL renderer.
 *
 * All text created by this engine should be destroyed before calling this
 * function.
 *
 * @param engine a TTF_TextEngine object created with
 *               TTF_CreateRendererTextEngine().
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateRendererTextEngine
 */
inline void DestroyRendererTextEngine(TextEngine* engine)
{
  return TTF_DestroyRendererTextEngine(engine);
}

/**
 * Create a text engine for drawing text with the SDL GPU API.
 *
 * @param device the SDL_GPUDevice to use for creating textures and drawing
 *               text.
 * @returns a TTF_TextEngine object or NULL on failure; call SDL_GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               device.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateGPUTextEngineWithProperties
 * @sa TTF_DestroyGPUTextEngine
 * @sa TTF_GetGPUTextDrawData
 */
inline TextEngine* CreateGPUTextEngine(SDL_GPUDevice* device)
{
  return TTF_CreateGPUTextEngine(device);
}

/**
 * Create a text engine for drawing text with the SDL GPU API, with the
 * specified properties.
 *
 * These are the supported properties:
 *
 * - `TTF_PROP_GPU_TEXT_ENGINE_DEVICE`: the SDL_GPUDevice to use for creating
 *   textures and drawing text.
 * - `TTF_PROP_GPU_TEXT_ENGINE_ATLAS_TEXTURE_SIZE`: the size of the texture
 *   atlas
 *
 * @param props the properties to use.
 * @returns a TTF_TextEngine object or NULL on failure; call SDL_GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               device.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateGPUTextEngine
 * @sa TTF_DestroyGPUTextEngine
 * @sa TTF_GetGPUTextDrawData
 */
inline TextEngine* CreateGPUTextEngineWithProperties(PropertiesRef props)
{
  return TTF_CreateGPUTextEngineWithProperties(props);
}

/**
 * Draw sequence returned by TTF_GetGPUTextDrawData
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetGPUTextDrawData
 */
using GPUAtlasDrawSequence = TTF_GPUAtlasDrawSequence;

/**
 * Get the geometry data needed for drawing the text.
 *
 * `text` must have been created using a TTF_TextEngine from
 * TTF_CreateGPUTextEngine().
 *
 * The positive X-axis is taken towards the right and the positive Y-axis is
 * taken upwards for both the vertex and the texture coordinates, i.e, it
 * follows the same convention used by the SDL_GPU API. If you want to use a
 * different coordinate system you will need to transform the vertices
 * yourself.
 *
 * If the text looks blocky use linear filtering.
 *
 * @param text the text to draw.
 * @returns a NULL terminated linked list of TTF_GPUAtlasDrawSequence objects
 *          or NULL if the passed text is empty or in case of failure; call
 *          SDL_GetError() for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateGPUTextEngine
 * @sa TTF_CreateText
 */
inline GPUAtlasDrawSequence* GetGPUTextDrawData(Text* text)
{
  return TTF_GetGPUTextDrawData(text);
}

/**
 * Destroy a text engine created for drawing text with the SDL GPU API.
 *
 * All text created by this engine should be destroyed before calling this
 * function.
 *
 * @param engine a TTF_TextEngine object created with
 *               TTF_CreateGPUTextEngine().
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateGPUTextEngine
 */
inline void DestroyGPUTextEngine(TextEngine* engine)
{
  return TTF_DestroyGPUTextEngine(engine);
}

/**
 * The winding order of the vertices returned by TTF_GetGPUTextDrawData
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 */
using GPUTextEngineWinding = TTF_GPUTextEngineWinding;

constexpr GPUTextEngineWinding GPU_TEXTENGINE_WINDING_INVALID =
  TTF_GPU_TEXTENGINE_WINDING_INVALID;

constexpr GPUTextEngineWinding GPU_TEXTENGINE_WINDING_CLOCKWISE =
  TTF_GPU_TEXTENGINE_WINDING_CLOCKWISE;

constexpr GPUTextEngineWinding GPU_TEXTENGINE_WINDING_COUNTER_CLOCKWISE =
  TTF_GPU_TEXTENGINE_WINDING_COUNTER_CLOCKWISE;

/**
 * Sets the winding order of the vertices returned by TTF_GetGPUTextDrawData
 * for a particular GPU text engine.
 *
 * @param engine a TTF_TextEngine object created with
 *               TTF_CreateGPUTextEngine().
 * @param winding the new winding order option.
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetGPUTextEngineWinding
 */
inline void SetGPUTextEngineWinding(TextEngine* engine,
                                    GPUTextEngineWinding winding)
{
  return TTF_SetGPUTextEngineWinding(engine, winding);
}

/**
 * Get the winding order of the vertices returned by TTF_GetGPUTextDrawData
 * for a particular GPU text engine
 *
 * @param engine a TTF_TextEngine object created with
 *               TTF_CreateGPUTextEngine().
 * @returns the winding order used by the GPU text engine or
 *          TTF_GPU_TEXTENGINE_WINDING_INVALID in case of error.
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SetGPUTextEngineWinding
 */
inline GPUTextEngineWinding GetGPUTextEngineWinding(const TextEngine* engine)
{
  return TTF_GetGPUTextEngineWinding(engine);
}

/**
 * Create a text object from UTF-8 text and a text engine.
 *
 * @param engine the text engine to use when creating the text object, may be
 *               NULL.
 * @param font the font to render with.
 * @param text the text to use, in UTF-8 encoding.
 * @param length the length of the text, in bytes, or 0 for null terminated
 *               text.
 * @returns a TTF_Text object or NULL on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font and text engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_DestroyText
 */
inline Text* CreateText(TextEngine* engine,
                        FontRef font,
                        StringParam text,
                        size_t length)
{
  return TTF_CreateText(engine, font.get(), text, length);
}

/**
 * Get the properties associated with a text object.
 *
 * @param text the TTF_Text to query.
 * @returns a valid property ID on success or 0 on failure; call
 *          SDL_GetError() for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline PropertiesRef GetTextProperties(Text* text)
{
  return {TTF_GetTextProperties(text)};
}

/**
 * Set the text engine used by a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the TTF_Text to modify.
 * @param engine the text engine to use for drawing.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetTextEngine
 */
inline bool SetTextEngine(Text* text, TextEngine* engine)
{
  return TTF_SetTextEngine(text, engine);
}

/**
 * Get the text engine used by a text object.
 *
 * @param text the TTF_Text to query.
 * @returns the TTF_TextEngine used by the text on success or NULL on failure;
 *          call SDL_GetError() for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SetTextEngine
 */
inline TextEngine* GetTextEngine(Text* text) { return TTF_GetTextEngine(text); }

/**
 * Set the font used by a text object.
 *
 * When a text object has a font, any changes to the font will automatically
 * regenerate the text. If you set the font to NULL, the text will continue to
 * render but changes to the font will no longer affect the text.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the TTF_Text to modify.
 * @param font the font to use, may be NULL.
 * @returns false if the text pointer is null; otherwise, true. call
 *          SDL_GetError() for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetTextFont
 */
inline bool SetTextFont(Text* text, FontRef font)
{
  return TTF_SetTextFont(text, font.get());
}

/**
 * Get the font used by a text object.
 *
 * @param text the TTF_Text to query.
 * @returns the TTF_Font used by the text on success or NULL on failure; call
 *          SDL_GetError() for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SetTextFont
 */
inline FontRef GetTextFont(Text* text) { return TTF_GetTextFont(text); }

/**
 * Set the direction to be used for text shaping a text object.
 *
 * This function only supports left-to-right text shaping if SDL_ttf was not
 * built with HarfBuzz support.
 *
 * @param text the text to modify.
 * @param direction the new direction for text to flow.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool SetTextDirection(Text* text, Direction direction)
{
  return TTF_SetTextDirection(text, direction);
}

/**
 * Get the direction to be used for text shaping a text object.
 *
 * This defaults to the direction of the font used by the text object.
 *
 * @param text the text to query.
 * @returns the direction to be used for text shaping.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline Direction GetTextDirection(Text* text)
{
  return TTF_GetTextDirection(text);
}

/**
 * Set the script to be used for text shaping a text object.
 *
 * This returns false if SDL_ttf isn't built with HarfBuzz support.
 *
 * @param text the text to modify.
 * @param script an
 * [ISO 15924 * code](https://unicode.org/iso15924/iso15924-codes.html).
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_StringToTag
 */
inline bool SetTextScript(Text* text, Uint32 script)
{
  return TTF_SetTextScript(text, script);
}

/**
 * Get the script used for text shaping a text object.
 *
 * This defaults to the script of the font used by the text object.
 *
 * @param text the text to query.
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
 * @sa TTF_TagToString
 */
inline Uint32 GetTextScript(Text* text) { return TTF_GetTextScript(text); }

/**
 * Set the color of a text object.
 *
 * The default text color is white (255, 255, 255, 255).
 *
 * @param text the TTF_Text to modify.
 * @param r the red color value in the range of 0-255.
 * @param g the green color value in the range of 0-255.
 * @param b the blue color value in the range of 0-255.
 * @param a the alpha value in the range of 0-255.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetTextColor
 * @sa TTF_SetTextColorFloat
 */
inline bool SetTextColor(Text* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  return TTF_SetTextColor(text, r, g, b, a);
}

/**
 * Set the color of a text object.
 *
 * The default text color is white (1.0f, 1.0f, 1.0f, 1.0f).
 *
 * @param text the TTF_Text to modify.
 * @param r the red color value, normally in the range of 0-1.
 * @param g the green color value, normally in the range of 0-1.
 * @param b the blue color value, normally in the range of 0-1.
 * @param a the alpha value in the range of 0-1.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetTextColorFloat
 * @sa TTF_SetTextColor
 */
inline bool SetTextColorFloat(Text* text, float r, float g, float b, float a)
{
  return TTF_SetTextColorFloat(text, r, g, b, a);
}

/**
 * Get the color of a text object.
 *
 * @param text the TTF_Text to query.
 * @param r a pointer filled in with the red color value in the range of
 *          0-255, may be NULL.
 * @param g a pointer filled in with the green color value in the range of
 *          0-255, may be NULL.
 * @param b a pointer filled in with the blue color value in the range of
 *          0-255, may be NULL.
 * @param a a pointer filled in with the alpha value in the range of 0-255,
 *          may be NULL.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetTextColorFloat
 * @sa TTF_SetTextColor
 */
inline bool GetTextColor(Text* text, Uint8* r, Uint8* g, Uint8* b, Uint8* a)
{
  return TTF_GetTextColor(text, r, g, b, a);
}

/**
 * Get the color of a text object.
 *
 * @param text the TTF_Text to query.
 * @param r a pointer filled in with the red color value, normally in the
 *          range of 0-1, may be NULL.
 * @param g a pointer filled in with the green color value, normally in the
 *          range of 0-1, may be NULL.
 * @param b a pointer filled in with the blue color value, normally in the
 *          range of 0-1, may be NULL.
 * @param a a pointer filled in with the alpha value in the range of 0-1, may
 *          be NULL.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetTextColor
 * @sa TTF_SetTextColorFloat
 */
inline bool GetTextColorFloat(Text* text,
                              float* r,
                              float* g,
                              float* b,
                              float* a)
{
  return TTF_GetTextColorFloat(text, r, g, b, a);
}

/**
 * Set the position of a text object.
 *
 * This can be used to position multiple text objects within a single wrapping
 * text area.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the TTF_Text to modify.
 * @param x the x offset of the upper left corner of this text in pixels.
 * @param y the y offset of the upper left corner of this text in pixels.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetTextPosition
 */
inline bool SetTextPosition(Text* text, int x, int y)
{
  return TTF_SetTextPosition(text, x, y);
}

/**
 * Get the position of a text object.
 *
 * @param text the TTF_Text to query.
 * @param x a pointer filled in with the x offset of the upper left corner of
 *          this text in pixels, may be NULL.
 * @param y a pointer filled in with the y offset of the upper left corner of
 *          this text in pixels, may be NULL.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SetTextPosition
 */
inline bool GetTextPosition(Text* text, int* x, int* y)
{
  return TTF_GetTextPosition(text, x, y);
}

/**
 * Set whether wrapping is enabled on a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the TTF_Text to modify.
 * @param wrap_width the maximum width in pixels, 0 to wrap on newline
 *                   characters.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetTextWrapWidth
 */
inline bool SetTextWrapWidth(Text* text, int wrap_width)
{
  return TTF_SetTextWrapWidth(text, wrap_width);
}

/**
 * Get whether wrapping is enabled on a text object.
 *
 * @param text the TTF_Text to query.
 * @param wrap_width a pointer filled in with the maximum width in pixels or 0
 *                   if the text is being wrapped on newline characters.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SetTextWrapWidth
 */
inline bool GetTextWrapWidth(Text* text, int* wrap_width)
{
  return TTF_GetTextWrapWidth(text, wrap_width);
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
 * @param text the TTF_Text to modify.
 * @param visible true to show whitespace when wrapping text, false to hide
 *                it.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_TextWrapWhitespaceVisible
 */
inline bool SetTextWrapWhitespaceVisible(Text* text, bool visible)
{
  return TTF_SetTextWrapWhitespaceVisible(text, visible);
}

/**
 * Return whether whitespace is shown when wrapping a text object.
 *
 * @param text the TTF_Text to query.
 * @returns true if whitespace is shown when wrapping text, or false
 *          otherwise.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SetTextWrapWhitespaceVisible
 */
inline bool TextWrapWhitespaceVisible(Text* text)
{
  return TTF_TextWrapWhitespaceVisible(text);
}

/**
 * Set the UTF-8 text used by a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the TTF_Text to modify.
 * @param string the UTF-8 text to use, may be NULL.
 * @param length the length of the text, in bytes, or 0 for null terminated
 *               text.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_AppendTextString
 * @sa TTF_DeleteTextString
 * @sa TTF_InsertTextString
 */
inline bool SetTextString(Text* text, StringParam string, size_t length)
{
  return TTF_SetTextString(text, string, length);
}

/**
 * Insert UTF-8 text into a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the TTF_Text to modify.
 * @param offset the offset, in bytes, from the beginning of the string if >=
 *               0, the offset from the end of the string if < 0. Note that
 *               this does not do UTF-8 validation, so you should only insert
 *               at UTF-8 sequence boundaries.
 * @param string the UTF-8 text to insert.
 * @param length the length of the text, in bytes, or 0 for null terminated
 *               text.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_AppendTextString
 * @sa TTF_DeleteTextString
 * @sa TTF_SetTextString
 */
inline bool InsertTextString(Text* text,
                             int offset,
                             StringParam string,
                             size_t length)
{
  return TTF_InsertTextString(text, offset, string, length);
}

/**
 * Append UTF-8 text to a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the TTF_Text to modify.
 * @param string the UTF-8 text to insert.
 * @param length the length of the text, in bytes, or 0 for null terminated
 *               text.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_DeleteTextString
 * @sa TTF_InsertTextString
 * @sa TTF_SetTextString
 */
inline bool AppendTextString(Text* text, StringParam string, size_t length)
{
  return TTF_AppendTextString(text, string, length);
}

/**
 * Delete UTF-8 text from a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the TTF_Text to modify.
 * @param offset the offset, in bytes, from the beginning of the string if >=
 *               0, the offset from the end of the string if < 0. Note that
 *               this does not do UTF-8 validation, so you should only delete
 *               at UTF-8 sequence boundaries.
 * @param length the length of text to delete, in bytes, or -1 for the
 *               remainder of the string.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_AppendTextString
 * @sa TTF_InsertTextString
 * @sa TTF_SetTextString
 */
inline bool DeleteTextString(Text* text, int offset, int length)
{
  return TTF_DeleteTextString(text, offset, length);
}

/**
 * Get the size of a text object.
 *
 * The size of the text may change when the font or font style and size
 * change.
 *
 * @param text the TTF_Text to query.
 * @param w a pointer filled in with the width of the text, in pixels, may be
 *          NULL.
 * @param h a pointer filled in with the height of the text, in pixels, may be
 *          NULL.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool GetTextSize(Text* text, int* w, int* h)
{
  return TTF_GetTextSize(text, w, h);
}

/**
 * Flags for TTF_SubString
 *
 * @since This datatype is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_SubString
 */
using SubStringFlags = TTF_SubStringFlags;

/**
 * The representation of a substring within text.
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_GetNextTextSubString
 * @sa TTF_GetPreviousTextSubString
 * @sa TTF_GetTextSubString
 * @sa TTF_GetTextSubStringForLine
 * @sa TTF_GetTextSubStringForPoint
 * @sa TTF_GetTextSubStringsForRange
 */
using SubString = TTF_SubString;

/**
 * Get the substring of a text object that surrounds a text offset.
 *
 * If `offset` is less than 0, this will return a zero length substring at the
 * beginning of the text with the TTF_SUBSTRING_TEXT_START flag set. If
 * `offset` is greater than or equal to the length of the text string, this
 * will return a zero length substring at the end of the text with the
 * TTF_SUBSTRING_TEXT_END flag set.
 *
 * @param text the TTF_Text to query.
 * @param offset a byte offset into the text string.
 * @param substring a pointer filled in with the substring containing the
 *                  offset.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool GetTextSubString(Text* text, int offset, SubString* substring)
{
  return TTF_GetTextSubString(text, offset, substring);
}

/**
 * Get the substring of a text object that contains the given line.
 *
 * If `line` is less than 0, this will return a zero length substring at the
 * beginning of the text with the TTF_SUBSTRING_TEXT_START flag set. If `line`
 * is greater than or equal to `text->num_lines` this will return a zero
 * length substring at the end of the text with the TTF_SUBSTRING_TEXT_END
 * flag set.
 *
 * @param text the TTF_Text to query.
 * @param line a zero-based line index, in the range [0 .. text->num_lines-1].
 * @param substring a pointer filled in with the substring containing the
 *                  offset.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool GetTextSubStringForLine(Text* text, int line, SubString* substring)
{
  return TTF_GetTextSubStringForLine(text, line, substring);
}

/**
 * Get the substrings of a text object that contain a range of text.
 *
 * @param text the TTF_Text to query.
 * @param offset a byte offset into the text string.
 * @param length the length of the range being queried, in bytes, or -1 for
 *               the remainder of the string.
 * @param count a pointer filled in with the number of substrings returned,
 *              may be NULL.
 * @returns a NULL terminated array of substring pointers or NULL on failure;
 *          call SDL_GetError() for more information. This is a single
 *          allocation that should be freed with SDL_free() when it is no
 *          longer needed.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline TTF_SubString** GetTextSubStringsForRange(Text* text,
                                                 int offset,
                                                 int length,
                                                 int* count)
{
  return TTF_GetTextSubStringsForRange(text, offset, length, count);
}

/**
 * Get the portion of a text string that is closest to a point.
 *
 * This will return the closest substring of text to the given point.
 *
 * @param text the TTF_Text to query.
 * @param x the x coordinate relative to the left side of the text, may be
 *          outside the bounds of the text area.
 * @param y the y coordinate relative to the top side of the text, may be
 *          outside the bounds of the text area.
 * @param substring a pointer filled in with the closest substring of text to
 *                  the given point.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool GetTextSubStringForPoint(Text* text,
                                     int x,
                                     int y,
                                     SubString* substring)
{
  return TTF_GetTextSubStringForPoint(text, x, y, substring);
}

/**
 * Get the previous substring in a text object
 *
 * If called at the start of the text, this will return a zero length
 * substring with the TTF_SUBSTRING_TEXT_START flag set.
 *
 * @param text the TTF_Text to query.
 * @param substring the TTF_SubString to query.
 * @param previous a pointer filled in with the previous substring.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool GetPreviousTextSubString(Text* text,
                                     const SubString* substring,
                                     SubString* previous)
{
  return TTF_GetPreviousTextSubString(text, substring, previous);
}

/**
 * Get the next substring in a text object
 *
 * If called at the end of the text, this will return a zero length substring
 * with the TTF_SUBSTRING_TEXT_END flag set.
 *
 * @param text the TTF_Text to query.
 * @param substring the TTF_SubString to query.
 * @param next a pointer filled in with the next substring.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool GetNextTextSubString(Text* text,
                                 const SubString* substring,
                                 SubString* next)
{
  return TTF_GetNextTextSubString(text, substring, next);
}

/**
 * Update the layout of a text object.
 *
 * This is automatically done when the layout is requested or the text is
 * rendered, but you can call this if you need more control over the timing of
 * when the layout and text engine representation are updated.
 *
 * @param text the TTF_Text to update.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool UpdateText(Text* text) { return TTF_UpdateText(text); }

/**
 * Destroy a text object created by a text engine.
 *
 * @param text the text to destroy.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_CreateText
 */
inline void DestroyText(Text* text) { return TTF_DestroyText(text); }

/**
 * Dispose of a previously-created font.
 *
 * Call this when done with a font. This function will free any resources
 * associated with it. It is safe to call this function on NULL, for example
 * on the result of a failed call to FontBase::FontBase().
 *
 * The font is not valid after being passed to this function. String pointers
 * from functions that return information on this font, such as
 * FontBase::GetFamilyName() andFontBase::GetStyleName(), are no longer valid
 * after this call, as well.
 *
 * @param resource the font to dispose of.
 *
 * @threadsafety This function should not be called while any other thread is
 *               using the font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_Font
 * @sa TTF_FontBase
 */
template<>
inline void ObjectRef<TTF_Font>::doFree(TTF_Font* resource)
{
  return TTF_CloseFont(resource);
}

/**
 * Deinitialize SDL_ttf.
 *
 * You must call this when done with the library, to free internal resources.
 * It is safe to call this when the library isn't initialized, as it will just
 * return immediately.
 *
 * Once you have as many quit calls as you have had successful calls to
 * TTF_Init, the library will actually deinitialize.
 *
 * Please note that this does not automatically close any fonts that are still
 * open at the time of deinitialization, and it is possibly not safe to close
 * them afterwards, as parts of the library will no longer be initialized to
 * deal with it. A well-written program should call TTF_CloseFont() on any
 * open fonts before calling this function!
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
 * to TTF_Init(), without a paired deinitialization request from TTF_Quit().
 *
 * In short: if it's greater than zero, the library is currently initialized
 * and ready to work. If zero, it is not initialized.
 *
 * Despite the return value being a signed integer, this function should not
 * return a negative number.
 *
 * @returns the current number of initialization calls, that need to
 *          eventually be paired with this many calls to TTF_Quit().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF_Init
 * @sa TTF_Quit
 */
inline int WasInit(TtfInitFlag _) { return TTF_WasInit(); }

/// @}

} // namespace SDL

#endif // defined(SDL3PP_ENABLE_TTF) || defined(SDL3PP_DOC)

#endif /* SDL3PP_TTF_H_ */
