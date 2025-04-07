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

/**
 * Wraps the TTF_TextEngine so we can store its
 * Destroy function with it
 *
 * @private
 */
class TextEngineWrapper
{
  TTF_TextEngine* m_ptr;
  void (*m_destroy)(TTF_TextEngine* engine);

public:
  constexpr TextEngineWrapper(std::nullptr_t = nullptr)
    : m_ptr(nullptr)
    , m_destroy(nullptr)
  {
  }

  constexpr TextEngineWrapper(TTF_TextEngine* ptr,
                              void (*destroy)(TTF_TextEngine* engine))
    : m_ptr(ptr)
    , m_destroy(destroy)
  {
  }

  constexpr auto operator<=>(const TextEngineWrapper& other) const = default;

  constexpr operator bool() const { return m_ptr != nullptr; }

  constexpr bool operator==(nullptr_t) const { return bool(*this); }

  constexpr operator TTF_TextEngine*() const { return m_ptr; }

  /**
   * @brief Never call this directly!!!!!
   *
   * @private
   */
  void doDestroyThis()
  {
    if (m_destroy) m_destroy(m_ptr);
    m_ptr = nullptr;
    m_destroy = nullptr;
  }
};

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

// Forward decl
template<ObjectBox<TextEngineWrapper> T>
struct TextEngineBase;

/**
 * Handle to a non owned textEngine
 *
 * @cat resource
 *
 * @sa TextEngineBase
 * @sa TextEngine
 */
using TextEngineRef =
  TextEngineBase<ObjectRef<TTF_TextEngine, TextEngineWrapper>>;

/**
 * Handle to an owned textEngine
 *
 * @cat resource
 *
 * @sa TextEngineBase
 * @sa TextEngineRef
 */
using TextEngine = TextEngineBase<
  ObjectUnique<TTF_TextEngine,
               ObjectDeleter<ObjectRef<TTF_TextEngine, TextEngineWrapper>>>>;

// Forward decl
template<ObjectBox<TTF_Text*> T>
struct TextBase;

/**
 * Handle to a non owned text
 *
 * @cat resource
 *
 * @sa TextBase
 * @sa Text
 */
using TextRef = TextBase<ObjectRef<TTF_Text>>;

/**
 * Handle to an owned text
 *
 * @cat resource
 *
 * @sa TextBase
 * @sa TextRef
 */
using Text = TextBase<ObjectUnique<TTF_Text>>;

/**
 * @name Font Style Flags
 * @{
 */

/**
 * Font style flags for FontBase
 *
 * These are the flags which can be used to set the style of a font in
 * SDL_ttf. A combination of these flags can be used with functions that set
 * or query font style, such as FontBase.SetStyle or FontBase.GetStyle.
 *
 * @since This datatype is available since SDL_ttf 3.0.0.
 *
 * @sa FontBase.SetStyle
 * @sa FontBase.GetStyle
 */
using FontStyleFlags = Uint32;

/**
 * No special style
 */
constexpr FontStyleFlags STYLE_NORMAL = TTF_STYLE_NORMAL;

/**
 * Bold style
 */
constexpr FontStyleFlags STYLE_BOLD = TTF_STYLE_BOLD;

/**
 * Italic style
 */
constexpr FontStyleFlags STYLE_ITALIC = TTF_STYLE_ITALIC;

/**
 * Underlined text
 */
constexpr FontStyleFlags STYLE_UNDERLINE = TTF_STYLE_UNDERLINE;

/**
 * Strikethrough text
 */
constexpr FontStyleFlags STYLE_STRIKETHROUGH = TTF_STYLE_STRIKETHROUGH;

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
 * @sa FontBase.SetHinting
 * @sa FontBase.GetHinting
 */
using HintingFlags = TTF_HintingFlags;

/// Invalid grid-fitting
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

/// Invalid alignment
constexpr HorizontalAlignment HORIZONTAL_ALIGN_INVALID =
  TTF_HORIZONTAL_ALIGN_INVALID;

constexpr HorizontalAlignment HORIZONTAL_ALIGN_LEFT = TTF_HORIZONTAL_ALIGN_LEFT;

constexpr HorizontalAlignment HORIZONTAL_ALIGN_CENTER =
  TTF_HORIZONTAL_ALIGN_CENTER;

constexpr HorizontalAlignment HORIZONTAL_ALIGN_RIGHT =
  TTF_HORIZONTAL_ALIGN_RIGHT;

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
 * @sa FontBase.SetDirection
 */
using Direction = TTF_Direction;

/// Invalid direction
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

/// Invalid state
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

/**
 * The mask for the flow direction for this substring
 */
constexpr SubStringFlags SUBSTRING_DIRECTION_MASK =
  TTF_SUBSTRING_DIRECTION_MASK;

/**
 * This substring contains the beginning of the text
 */
constexpr SubStringFlags SUBSTRING_TEXT_START = TTF_SUBSTRING_TEXT_START;

/**
 * This substring contains the beginning of line `line_index`
 */
constexpr SubStringFlags SUBSTRING_LINE_START = TTF_SUBSTRING_LINE_START;

/**
 * This substring contains the end of line `line_index`
 */
constexpr SubStringFlags SUBSTRING_LINE_END = TTF_SUBSTRING_LINE_END;

/**
 * This substring contains the end of the text
 */
constexpr SubStringFlags SUBSTRING_TEXT_END = TTF_SUBSTRING_TEXT_END;

/// @}

/**
 * @name GPUTextEngineWindings
 * @{
 */

/**
 * The winding order of the vertices returned by TTF_GetGPUTextDrawData
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 */
using GPUTextEngineWinding = TTF_GPUTextEngineWinding;

/// Invalid state
constexpr GPUTextEngineWinding GPU_TEXTENGINE_WINDING_INVALID =
  TTF_GPU_TEXTENGINE_WINDING_INVALID;

constexpr GPUTextEngineWinding GPU_TEXTENGINE_WINDING_CLOCKWISE =
  TTF_GPU_TEXTENGINE_WINDING_CLOCKWISE;

constexpr GPUTextEngineWinding GPU_TEXTENGINE_WINDING_COUNTER_CLOCKWISE =
  TTF_GPU_TEXTENGINE_WINDING_COUNTER_CLOCKWISE;

/// @}

#ifdef SDL3PP_DOC

/**
 * Printable format: "%d.%d.%d", MAJOR, MINOR, MICRO
 */
#define SDL_TTF_MAJOR_VERSION

#define SDL_TTF_MINOR_VERSION

#define SDL_TTF_MICRO_VERSION

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
 * @sa Init()
 * @sa InitSubSystem()
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
   * @param file path to font file.
   * @param ptsize point size to use for the newly-opened font.
   * @post a valid FontBase, or nullptr on failure; call GetError() for more
   *       information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.Close
   */
  FontBase(StringParam file, float ptsize)
    : T(TTF_OpenFont(file, ptsize))
  {
  }

  /**
   * Create a font from an IOStreamBase, using a specified point size.
   *
   * Some .fon fonts will have several sizes embedded in the file, so the point
   * size becomes the index of choosing which size. If the value is too high,
   * the last indexed size will be the default.
   *
   * If `closeio` is true, `src` will be automatically closed once the font is
   * closed. Otherwise you should close `src` yourself after closing the font.
   *
   * @param src an IOStreamBase to provide a font file's data.
   * @param closeio true to close `src` when the font is closed, false to leave
   *                it open.
   * @param ptsize point size to use for the newly-opened font.
   * @post a valid FontBase, or nullptr on failure; call GetError() for more
   *       information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.Close
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
   *   IOStreamBase isn't being used. This is required if
   *   `prop::Font.CREATE_IOSTREAM_POINTER` and
   *   `prop::Font.CREATE_EXISTING_FONT` aren't set.
   * - `prop::Font.CREATE_IOSTREAM_POINTER`: an IOStreamBase containing the
   *   font to be opened. This should not be closed until the font is closed.
   *   This is required if `TTF_PROP_FONT_CREATE_FILENAME_STRING` and
   *   `prop::Font.CREATE_EXISTING_FONT` aren't set.
   * - `prop::Font.CREATE_IOSTREAM_OFFSET_NUMBER`: the offset in the iostream
   *   for the beginning of the font, defaults to 0.
   * - `prop::Font.CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN`: true if closing the
   *   font should also close the associated IOStreamBase.
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
   * - `prop::Font.CREATE_EXISTING_FONT`: an optional FontBase that, if set,
   *   will be used as the font data source and the initial size and style of
   *   the new font.
   *
   * @param props the properties to use.
   * @post a valid FontBase, or nullptr on failure; call GetError() for more
   *       information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.Close
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
   * @returns a valid Font, or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               original font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Font Copy() const { return TTF_CopyFont(T::get()); }

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
   * @returns a valid property ID on success or 0 on failure; call
   *          GetError() for more information.
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
   * @returns the font generation or 0 on failure; call GetError() for more
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
   * This updates any TextBase objects using this font.
   *
   * @param fallback the font to add as a fallback.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created
   *               both fonts.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.ClearFallbacks
   * @sa FontBase.RemoveFallback
   */
  bool AddFallback(FontRef fallback)
  {
    return TTF_AddFallbackFont(T::get(), fallback);
  }

  /**
   * Remove a fallback font.
   *
   * This updates any TextBase objects using this font.
   *
   * @param fallback the font to remove as a fallback.
   *
   * @threadsafety This function should be called on the thread that created
   *               both fonts.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.AddFallback
   * @sa FontBase.ClearFallbacks
   */
  void RemoveFallback(FontRef fallback)
  {
    TTF_RemoveFallbackFont(T::get(), fallback);
  }

  /**
   * Remove all fallback fonts.
   *
   * This updates any TextBase objects using this font.
   *
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.AddFallback
   * @sa FontBase.RemoveFallback
   */
  void ClearFallbacks() { TTF_ClearFallbackFonts(T::get()); }

  /**
   * Set a font's size dynamically.
   *
   * This updates any TextBase objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param ptsize the new point size.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.GetSize
   */
  bool SetSize(float ptsize) { return TTF_SetFontSize(T::get(), ptsize); }

  /**
   * Set font size dynamically with target resolutions, in dots per inch.
   *
   * This updates any TextBase objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param ptsize the new point size.
   * @param hdpi the target horizontal DPI.
   * @param vdpi the target vertical DPI.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.GetSize
   * @sa FontBase.GetSizeDPI
   */
  bool SetSizeDPI(float ptsize, int hdpi, int vdpi)
  {
    return TTF_SetFontSizeDPI(T::get(), ptsize, hdpi, vdpi);
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
   * @sa FontBase.SetSize
   * @sa FontBase.SetSizeDPI
   */
  float GetSize() const { return TTF_GetFontSize(T::get()); }

  /**
   * Get font target resolutions, in dots per inch.
   *
   * @param hdpi a pointer filled in with the target horizontal DPI.
   * @param vdpi a pointer filled in with the target vertical DPI.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.SetSizeDPI
   */
  bool GetDPI(int* hdpi, int* vdpi) const
  {
    return TTF_GetFontDPI(T::get(), hdpi, vdpi);
  }

  /**
   * Set a font's current style.
   *
   * This updates any TextBase objects using this font, and clears
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
   * @sa FontBase.GetStyle
   */
  void SetStyle(FontStyleFlags style) { TTF_SetFontStyle(T::get(), style); }

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
   * @sa FontBase.SetStyle
   */
  FontStyleFlags GetStyle() const { return TTF_GetFontStyle(T::get()); }

  /**
   * Set a font's current outline.
   *
   * This uses the font properties `prop::Font.OUTLINE_LINE_CAP_NUMBER`,
   * `prop::Font.OUTLINE_LINE_JOIN_NUMBER`, and
   * `prop::Font.OUTLINE_MITER_LIMIT_NUMBER` when setting the font outline.
   *
   * This updates any TextBase objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param outline positive outline value, 0 to default.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.GetOutline
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
   * @sa FontBase.SetOutline
   */
  int GetOutline() const { return TTF_GetFontOutline(T::get()); }

  /**
   * Set a font's current hinter setting.
   *
   * This updates any TextBase objects using this font, and clears
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
   * @sa FontBase.GetHinting
   */
  void SetHinting(HintingFlags hinting)
  {
    TTF_SetFontHinting(T::get(), hinting);
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
   * @sa FontBase.SetHinting
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
   * This updates any TextBase objects using this font, and clears
   * already-generated glyphs, if any, from the cache.
   *
   * @param enabled true to enable SDF, false to disable.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.GetSDF
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
   * @sa FontBase.SetSDF
   */
  bool GetSDF() const { return TTF_GetFontSDF(T::get()); }

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
  int GetWeight() const { return TTF_GetFontWeight(T::get()); }

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 2)

  /**
   * Set a font's current wrap alignment option.
   *
   * This updates any TextBase objects using this font.
   *
   * @param align the new wrap alignment option.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.GetWrapAlignment
   */
  void SetWrapAlignment(HorizontalAlignment align)
  {
    TTF_SetFontWrapAlignment(T::get(), align);
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
   * @sa FontBase.SetWrapAlignment
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
   * This updates any TextBase objects using this font.
   *
   * @param lineskip the new line spacing for the font.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.GetLineSkip
   */
  void SetLineSkip(int lineskip) { TTF_SetFontLineSkip(T::get(), lineskip); }

  /**
   * Query the spacing between lines of text for a font.
   *
   * @returns the font's recommended spacing.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.SetLineSkip
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
   * This updates any TextBase objects using this font.
   *
   * @param enabled true to enable kerning, false to disable.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.GetKerning
   */
  void SetKerning(bool enabled) { TTF_SetFontKerning(T::get(), enabled); }

  /**
   * Query whether or not kerning is enabled for a font.
   *
   * @returns true if kerning is enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.SetKerning
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
   * @sa FontBase.SetSDF
   */
  bool IsScalable() const { return TTF_FontIsScalable(T::get()); }

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
  const char* GetFamilyName() const { return TTF_GetFontFamilyName(T::get()); }

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
  const char* GetStyleName() const { return TTF_GetFontStyleName(T::get()); }

  /**
   * Set the direction to be used for text shaping by a font.
   *
   * This function only supports left-to-right text shaping if SDL_ttf was not
   * built with HarfBuzz support.
   *
   * This updates any TextBase objects using this font.
   *
   * @param direction the new direction for text to flow.
   * @returns true on success or false on failure; call GetError() for more
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
   * This defaults to DIRECTION_INVALID if it hasn't been set.
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
   * This updates any TextBase objects using this font.
   *
   * @param script an
   * [ISO 15924 code](https://unicode.org/iso15924/iso15924-codes.html).
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa StringToTag
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
   * @sa TagToString
   */
  Uint32 GetScript() const { return TTF_GetFontScript(T::get()); }

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
   * This updates any TextBase objects using this font.
   *
   * @param language_bcp47 a null-terminated string containing the desired
   *                       language's BCP47 code. Or null to reset the value.
   * @returns true on success or false on failure; call GetError() for more
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
   *                   nullptr.
   * @returns an SurfaceBase containing the glyph, or nullptr on failure; call
   *          GetError() for more information.
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
   * @returns true on success or false on failure; call GetError() for more
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
   * @returns true on success or false on failure; call GetError() for more
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
   *                       string that will fit, may be nullptr.
   * @param measured_length a pointer filled in with the length, in bytes, of
   *                        the string that will fit, may be nullptr.
   * @returns true on success or false on failure; call GetError() for more
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
   * FontBase.RenderText_Solid_Wrapped() instead if you need to wrap the output
   * to multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with FontBase.RenderText_Shaded,
   * FontBase.RenderText_Blended, and FontBase.RenderText_LCD.
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
   * @sa FontBase.RenderText_Blended
   * @sa FontBase.RenderText_LCD
   * @sa FontBase.RenderText_Shaded
   * @sa FontBase.RenderText_Solid
   * @sa FontBase.RenderText_Solid_Wrapped
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
   * You can render at other quality levels with
   * FontBase.RenderText_Shaded_Wrapped, FontBase.RenderText_Blended_Wrapped,
   * and FontBase.RenderText_LCD_Wrapped.
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
   * @sa FontBase.RenderText_Blended_Wrapped
   * @sa FontBase.RenderText_LCD_Wrapped
   * @sa FontBase.RenderText_Shaded_Wrapped
   * @sa FontBase.RenderText_Solid
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
   * You can render at other quality levels with FontBase.RenderGlyph_Shaded,
   * FontBase.RenderGlyph_Blended, and FontBase.RenderGlyph_LCD.
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
   * @sa FontBase.RenderGlyph_Blended
   * @sa FontBase.RenderGlyph_LCD
   * @sa FontBase.RenderGlyph_Shaded
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
   * surface, or nullptr if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * FontBase.RenderText_Shaded_Wrapped() instead if you need to wrap the output
   * to multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with FontBase.RenderText_Solid,
   * FontBase.RenderText_Blended, and FontBase.RenderText_LCD.
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
   * @sa FontBase.RenderText_Blended
   * @sa FontBase.RenderText_LCD
   * @sa FontBase.RenderText_Shaded_Wrapped
   * @sa FontBase.RenderText_Solid
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
   * surface, or nullptr if there was an error.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with
   * FontBase.RenderText_Solid_Wrapped, FontBase.RenderText_Blended_Wrapped, and
   * FontBase.RenderText_LCD_Wrapped.
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
   * @sa FontBase.RenderText_Blended_Wrapped
   * @sa FontBase.RenderText_LCD_Wrapped
   * @sa FontBase.RenderText_Shaded
   * @sa FontBase.RenderText_Solid_Wrapped
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
   * surface, or nullptr if there was an error.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with FontBase.RenderGlyph_Solid,
   * FontBase.RenderGlyph_Blended, and FontBase.RenderGlyph_LCD.
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
   * @sa FontBase.RenderGlyph_Blended
   * @sa FontBase.RenderGlyph_LCD
   * @sa FontBase.RenderGlyph_Solid
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
   * new surface, or nullptr if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * FontBase.RenderText_Blended_Wrapped() instead if you need to wrap the
   * output to multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with FontBase.RenderText_Solid,
   * FontBase.RenderText_Shaded, and FontBase.RenderText_LCD.
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
   * @sa FontBase.RenderText_Blended_Wrapped
   * @sa FontBase.RenderText_LCD
   * @sa FontBase.RenderText_Shaded
   * @sa FontBase.RenderText_Solid
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
   * new surface, or nullptr if there was an error.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with
   * FontBase.RenderText_Solid_Wrapped, FontBase.RenderText_Shaded_Wrapped, and
   * FontBase.RenderText_LCD_Wrapped.
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
   * @sa FontBase.RenderText_Blended
   * @sa FontBase.RenderText_LCD_Wrapped
   * @sa FontBase.RenderText_Shaded_Wrapped
   * @sa FontBase.RenderText_Solid_Wrapped
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
   * new surface, or nullptr if there was an error.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with FontBase.RenderGlyph_Solid,
   * FontBase.RenderGlyph_Shaded, and FontBase.RenderGlyph_LCD.
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
   * @sa FontBase.RenderGlyph_LCD
   * @sa FontBase.RenderGlyph_Shaded
   * @sa FontBase.RenderGlyph_Solid
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
   * returns the new surface, or nullptr if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * FontBase.RenderText_LCD_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with FontBase.RenderText_Solid,
   * FontBase.RenderText_Shaded, and FontBase.RenderText_Blended.
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
   * @sa FontBase.RenderText_Blended
   * @sa FontBase.RenderText_LCD_Wrapped
   * @sa FontBase.RenderText_Shaded
   * @sa FontBase.RenderText_Solid
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
   * returns the new surface, or nullptr if there was an error.
   *
   * Text is wrapped to multiple lines on line endings and on word boundaries if
   * it extends beyond `wrap_width` in pixels.
   *
   * If wrap_width is 0, this function will only wrap on newline characters.
   *
   * You can render at other quality levels with
   * FontBase.RenderText_Solid_Wrapped, FontBase.RenderText_Shaded_Wrapped, and
   * FontBase.RenderText_Blended_Wrapped.
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
   * @sa FontBase.RenderText_Blended_Wrapped
   * @sa FontBase.RenderText_LCD
   * @sa FontBase.RenderText_Shaded_Wrapped
   * @sa FontBase.RenderText_Solid_Wrapped
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
   * returns the new surface, or nullptr if there was an error.
   *
   * The glyph is rendered without any padding or centering in the X direction,
   * and aligned normally in the Y direction.
   *
   * You can render at other quality levels with FontBase.RenderGlyph_Solid,
   * FontBase.RenderGlyph_Shaded, and FontBase.RenderGlyph_Blended.
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
   * @sa FontBase.RenderGlyph_Blended
   * @sa FontBase.RenderGlyph_Shaded
   * @sa FontBase.RenderGlyph_Solid
   */
  Surface RenderGlyph_LCD(Uint32 ch, Color fg, Color bg) const
  {
    return TTF_RenderGlyph_LCD(T::get(), ch, fg, bg);
  }

  /**
   * Dispose of a previously-created font.
   *
   * Call this when done with a font. This function will free any resources
   * associated with it. It is safe to call this function on nullptr, for
   * example on the result of a failed call to FontBase.FontBase().
   *
   * The font is not valid after being passed to this function. String pointers
   * from functions that return information on this font, such as
   * FontBase.GetFamilyName() and FontBase.GetStyleName(), are no longer valid
   * after this call, as well.
   *
   * @threadsafety This function should not be called while any other thread is
   *               using the font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa FontBase.FontBase
   * @sa FontBase.FontBase
   */
  void Close() { T::free(); }
};

/**
 * Callback for font resource cleanup
 *
 * @private
 */
template<>
inline void ObjectRef<TTF_Font>::doFree(TTF_Font* resource)
{
  TTF_CloseFont(resource);
}

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
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa QuitSubSystem(TtfInitFlag)
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
 * - Drawing to an SDL_Surface, created with CreateSurfaceTextEngine()
 * - Drawing with an SDL 2D renderer, created with CreateRendererTextEngine()
 * - Drawing with the SDL GPU API, created with CreateGPUTextEngine()
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 */
template<ObjectBox<TextEngineWrapper> T>
struct TextEngineBase : T
{
  using T::T;

  /**
   * Sets the winding order of the vertices returned by TextBase.GetGPUDrawData
   * for a particular GPU text engine.
   *
   * @param winding the new winding order option.
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextEngineBase.GetGPUWinding
   */
  void SetGPUWinding(GPUTextEngineWinding winding)
  {
    TTF_SetGPUTextEngineWinding(T::get(), winding);
  }

  /**
   * Get the winding order of the vertices returned by TextBase.GetGPUDrawData
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
   * @sa TextEngineBase.SetGPUWinding
   */
  GPUTextEngineWinding GetGPUWinding() const
  {
    return TTF_GetGPUTextEngineWinding(T::get());
  }

  /**
   * Destroy the text engine
   *
   */
  void Destroy() { T::free(); }
};

/**
 * Callback for text engine resource cleanup
 *
 * @private
 */
template<>
inline void ObjectRef<TTF_TextEngine, TextEngineWrapper>::doFree(
  TextEngineWrapper resource)
{
  resource.doDestroyThis();
}

/**
 * Draw sequence returned by TextBase.GetGPUDrawData
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa TextBase.GetGPUDrawData
 */
using GPUAtlasDrawSequence = TTF_GPUAtlasDrawSequence;

/**
 * The representation of a substring within text.
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa TextBase.GetNextSubString
 * @sa TextBase.GetPreviousSubString
 * @sa TextBase.GetSubString
 * @sa TextBase.GetSubStrings
 * @sa TextBase.GetSubStringForLine
 * @sa TextBase.GetSubStringForPoint
 * @sa TextBase.GetSubStringsForRange
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

constexpr auto CREATE_EXISTING_FONT = TTF_PROP_FONT_CREATE_EXISTING_FONT;

constexpr auto OUTLINE_LINE_CAP_NUMBER = TTF_PROP_FONT_OUTLINE_LINE_CAP_NUMBER;

constexpr auto OUTLINE_LINE_JOIN_NUMBER =
  TTF_PROP_FONT_OUTLINE_LINE_JOIN_NUMBER;

constexpr auto OUTLINE_MITER_LIMIT_NUMBER =
  TTF_PROP_FONT_OUTLINE_MITER_LIMIT_NUMBER;

} // namespace prop::Font

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
 * Text
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa TextBase.TextBase
 * @sa TextBase.GetProperties
 * @sa TextBase.Destroy
 */
template<ObjectBox<TTF_Text*> T>
struct TextBase : T
{
  using T::T;

  /**
   * Draw text to an SDL surface.
   *
   * `text` must have been created using a TextEngineBase from
   * CreateSurfaceTextEngine().
   *
   * @param p the (x, y) coordinate in pixels, positive from the left edge
   *          towards the right and from the top edge towards the bottom.
   * @param surface the surface to draw on.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa CreateSurfaceTextEngine
   * @sa TextBase.TextBase
   */
  bool DrawSurface(Point p, SurfaceRef surface) const
  {
    return TTF_DrawSurfaceText(T::get(), p.x, p.y, surface.get());
  }

  /**
   * Draw text to an SDL renderer.
   *
   * `text` must have been created using a TextEngineBase from
   * CreateRendererTextEngine(), and will draw using the renderer passed to
   * that function.
   *
   * @param p the (x, y) coordinate in pixels, positive from the left edge
   *          towards the right and from the top edge towards the bottom.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa CreateRendererTextEngine
   * @sa TextBase.TextBase
   */
  bool DrawRenderer(FPoint p) const
  {
    return TTF_DrawRendererText(T::get(), p.x, p.y);
  }

  /**
   * Get the geometry data needed for drawing the text.
   *
   * `text` must have been created using a TextEngineBase from
   * CreateGPUTextEngine().
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
   * @sa CreateGPUTextEngine
   * @sa TextBase.TextBase
   */
  GPUAtlasDrawSequence* GetGPUDrawData() const
  {
    return TTF_GetGPUTextDrawData(T::get());
  }

  /**
   * Create a text object from UTF-8 text and a text engine.
   *
   * @param engine the text engine to use when creating the text object, may be
   *               nullptr.
   * @param font the font to render with.
   * @param text the text to use, in UTF-8 encoding.
   * @post a TextBase object or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font and text engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.Destroy
   */
  TextBase(TextEngineRef engine, FontRef font, std::string_view text)
    : T(TTF_CreateText(engine.get(), font.get(), text.data(), text.size()))
  {
  }

  /**
   * Get the properties associated with a text object.
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  PropertiesRef GetProperties() const
  {
    return PropertiesRef{TTF_GetTextProperties(T::get())};
  }

  /**
   * Set the text engine used by a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param engine the text engine to use for drawing.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.GetEngine
   */
  bool SetEngine(TextEngineRef engine)
  {
    return TTF_SetTextEngine(T::get(), engine.get());
  }

  /**
   * Get the text engine used by a text object.
   *
   * @returns the TextEngineBase used by the text on success or nullptr on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.SetEngine
   */
  TextEngineRef GetEngine() const
  {
    return TextEngineWrapper{TTF_GetTextEngine(T::get()), nullptr};
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
   * @sa TextBase.GetFont
   */
  bool SetFont(FontRef font) { return TTF_SetTextFont(T::get(), font.get()); }

  /**
   * Get the font used by a text object.
   *
   * @returns the FontBase used by the text on success or nullptr on failure;
   * call GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.SetFont
   */
  FontRef GetFont() const { return TTF_GetTextFont(T::get()); }

  /**
   * Set the direction to be used for text shaping a text object.
   *
   * This function only supports left-to-right text shaping if SDL_ttf was not
   * built with HarfBuzz support.
   *
   * @param direction the new direction for text to flow.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool SetDirection(Direction direction)
  {
    return TTF_SetTextDirection(T::get(), direction);
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
  Direction GetDirection() const { return TTF_GetTextDirection(T::get()); }

  /**
   * Set the script to be used for text shaping a text object.
   *
   * This returns false if SDL_ttf isn't built with HarfBuzz support.
   *
   * @param script an [ISO 15924
   * code](https://unicode.org/iso15924/iso15924-codes.html).
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa StringToTag
   */
  bool SetScript(Uint32 script) { return TTF_SetTextScript(T::get(), script); }

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
  Uint32 GetScript() const { return TTF_GetTextScript(T::get()); }

  /**
   * Set the color of a text object.
   *
   * The default text color is white (255, 255, 255, 255).
   *
   * @param c the color value in the range of 0-255.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.GetColor(Color*)
   * @sa TextBase.SetColor(FColor)
   */
  bool SetColor(Color c)
  {
    return TTF_SetTextColor(T::get(), c.r, c.g, c.b, c.a);
  }

  /**
   * Set the color of a text object.
   *
   * The default text color is white (1.0f, 1.0f, 1.0f, 1.0f).
   *
   * @param c the color value, normally in the range of 0-1.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.GetColor(FColor*)
   * @sa TextBase.SetColor(Color)
   */
  bool SetColor(FColor c)
  {
    return TTF_SetTextColorFloat(T::get(), c.r, c.g, c.b, c.a);
  }

  /**
   * Get the color of a text object.
   *
   * @return The color in the range of 0-1 or std::nullopt on failure; call
   * GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @sa SetColor(FColor)
   */
  std::optional<FColor> GetColor() const
  {
    if (FColor c; GetColor(&c)) return c;
    return std::nullopt;
  }

  /**
   * Get the color of a text object.
   *
   * @param c a pointer filled in with red color value in the range of 0-255,
   * __must__ not be nullptr.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetColor(Color)
   */
  bool GetColor(Color* c) const
  {
    SDL_assert_paranoid(c != nullptr);
    return GetColor(&c->r, &c->g, &c->b, &c->a);
  }

  /**
   * Get the color of a text object.
   *
   * @param c a pointer filled in with red color value in the range of 0-1,
   * __must__ not be nullptr.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetColor(FColor)
   */
  bool GetColor(FColor* c) const
  {
    SDL_assert_paranoid(c != nullptr);
    return GetColor(&c->r, &c->g, &c->b, &c->a);
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetColor(Color)
   */
  bool GetColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a) const
  {
    return TTF_GetTextColor(T::get(), r, g, b, a);
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetColor(FColor)
   */
  bool GetColor(float* r, float* g, float* b, float* a) const
  {
    return TTF_GetTextColorFloat(T::get(), r, g, b, a);
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
   * @sa TextBase.GetPosition
   */
  bool SetPosition(Point p) { return TTF_SetTextPosition(T::get(), p.x, p.y); }

  /**
   * Get the position of a text object.
   *
   * @returns a Point with the offset of the upper left corner of this text in
   *          pixels or std::nullopt on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SetPosition()
   */
  std::optional<Point> GetPosition() const
  {
    if (Point p; GetPosition(&p.x, &p.y)) return p;
    return std::nullopt;
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
   * @sa TextBase.SetPosition
   */
  bool GetPosition(int* x, int* y) const
  {
    return TTF_GetTextPosition(T::get(), x, y);
  }

  /**
   * Set whether wrapping is enabled on a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param wrap_width the maximum width in pixels, 0 to wrap on newline
   *                   characters.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.GetWrapWidth
   */
  bool SetWrapWidth(int wrap_width)
  {
    return TTF_SetTextWrapWidth(T::get(), wrap_width);
  }

  /**
   * Get whether wrapping is enabled on a text object.
   *
   * @returns an int with the maximum width in pixels or 0 if the text is being
   *          wrapped on newline characters on success or std::nullopt on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.SetWrapWidth
   */
  std::optional<int> GetWrapWidth() const
  {
    if (int w; TTF_GetTextWrapWidth(T::get(), &w)) return w;
    return std::nullopt;
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.IsWrapWhitespaceVisible
   */
  bool SetWrapWhitespaceVisible(bool visible)
  {
    return TTF_SetTextWrapWhitespaceVisible(T::get(), visible);
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
   * @sa TextBase.SetWrapWhitespaceVisible
   */
  bool IsWrapWhitespaceVisible() const
  {
    return TTF_TextWrapWhitespaceVisible(T::get());
  }

  /**
   * Set the UTF-8 text used by a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param string the UTF-8 text to use.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.AppendString
   * @sa TextBase.DeleteString
   * @sa TextBase.InsertString
   */
  bool SetString(std::string_view string)
  {
    return TTF_SetTextString(T::get(), string.data(), string.size());
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.AppendString
   * @sa TextBase.DeleteString
   * @sa TextBase.SetString
   */
  bool InsertString(int offset, std::string_view string)
  {
    return TTF_InsertTextString(T::get(), offset, string.data(), string.size());
  }

  /**
   * Append UTF-8 text to a text object.
   *
   * This function may cause the internal text representation to be rebuilt.
   *
   * @param string the UTF-8 text to insert.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.DeleteString
   * @sa TextBase.InsertString
   * @sa TextBase.SetString
   */
  bool AppendString(std::string_view string)
  {
    return TTF_AppendTextString(T::get(), string.data(), string.size());
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.AppendString
   * @sa TextBase.InsertString
   * @sa TextBase.SetString
   */
  bool DeleteString(int offset, int length = -1)
  {
    return TTF_DeleteTextString(T::get(), offset, length);
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
  std::optional<Point> GetSize() const
  {
    if (Point p; GetSize(&p.x, &p.y)) return p;
    return std::nullopt;
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @see GetSize() if you need both coordinates
   */
  bool GetSize(int* w, int* h) const { return TTF_GetTextSize(T::get(), w, h); }

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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool GetSubString(int offset, SubString* substring) const
  {
    return TTF_GetTextSubString(T::get(), offset, substring);
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool GetSubStringForLine(int line, SubString* substring) const
  {
    return TTF_GetTextSubStringForLine(T::get(), line, substring);
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
    auto data = TTF_GetTextSubStringsForRange(T::get(), offset, length, &count);
    return OwnArray<SubString*>{data, count};
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool GetSubStringForPoint(Point p, SubString* substring) const
  {
    return TTF_GetTextSubStringForPoint(T::get(), p.x, p.y, substring);
  }

  /**
   * Get the previous substring in a text object
   *
   * If called at the start of the text, this will return a zero length
   * substring with the SUBSTRING_TEXT_START flag set.
   *
   * @param substring the SubString to query.
   * @param previous a pointer filled in with the previous substring.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool GetPreviousSubString(const SubString& substring,
                            SubString* previous) const
  {
    return TTF_GetPreviousTextSubString(T::get(), &substring, previous);
  }

  /**
   * Get the next substring in a text object
   *
   * If called at the end of the text, this will return a zero length substring
   * with the SUBSTRING_TEXT_END flag set.
   *
   * @param substring the SubString to query.
   * @param next a pointer filled in with the next substring.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool GetNextSubString(const SubString& substring, SubString* next) const
  {
    return TTF_GetNextTextSubString(T::get(), &substring, next);
  }

  /**
   * Update the layout of a text object.
   *
   * This is automatically done when the layout is requested or the text is
   * rendered, but you can call this if you need more control over the timing of
   * when the layout and text engine representation are updated.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  bool Update() { return TTF_UpdateText(T::get()); }

  /**
   * A copy of the UTF-8 string that this text object represents, useful for
   * layout, debugging and retrieving substring text
   */
  const char* GetText() const { return T::get()->text; }

  /**
   * The number of lines in the text, 0 if it's empty
   */
  int GetNumLines() const { return T::get()->num_lines; }

  /**
   * Destroy a text object created by a text engine.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TextBase.TextBase
   */
  void Destroy() { T::free(); }
};

/**
 * Callback for text resource cleanup
 *
 * @private
 */
template<>
inline void ObjectRef<TTF_Text>::doFree(TTF_Text* resource)
{
  TTF_DestroyText(resource);
}

/**
 * SubString iterator.
 *
 * @todo document the iterator invalidation characteristics.
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
  constexpr SubStringIterator()
    : SubStringIterator(nullptr)
  {
  }

  constexpr operator bool() const { return m_text != nullptr; }

  constexpr const SubString& operator*() const { return m_subString; }

  constexpr const SubString* operator->() const { return &m_subString; }

  constexpr bool operator==(const SubStringIterator& other) const
  {
    return m_subString.offset == other.m_subString.offset;
  }

  constexpr SubStringIterator& operator++()
  {
    m_text.GetNextSubString(m_subString, &m_subString);
    return *this;
  }

  constexpr SubStringIterator operator++(int)
  {
    auto curr = *this;
    m_text.GetNextSubString(m_subString, &m_subString);
    return curr;
  }

  constexpr SubStringIterator& operator--()
  {
    m_text.GetPreviousSubString(m_subString, &m_subString);
    return *this;
  }

  constexpr SubStringIterator operator--(int)
  {
    auto curr = *this;
    m_text.GetPreviousSubString(m_subString, &m_subString);
    return curr;
  }

  template<ObjectBox<TTF_Text*> T>
  friend class TextBase;
};

/**
 * Create a text engine for drawing text on SDL surfaces.
 *
 * @returns a TextEngine object or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text
 * @sa TextBase.DrawSurface
 */
inline TextEngine CreateSurfaceTextEngine()
{
  return TextEngine{TextEngineWrapper{TTF_CreateSurfaceTextEngine(),
                                      TTF_DestroySurfaceTextEngine}};
}

/**
 * Create a text engine for drawing text on an SDL renderer.
 *
 * @param renderer the renderer to use for creating textures and drawing text.
 * @returns a TextEngine object or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               renderer.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TextBase.DrawRenderer
 * @sa Text
 * @sa CreateRendererTextEngineWithProperties
 */
inline TextEngine CreateRendererTextEngine(RendererRef renderer)
{
  return TextEngine{
    TextEngineWrapper{TTF_CreateRendererTextEngine(renderer.get()),
                      TTF_DestroyRendererTextEngine}};
}

/**
 * Create a text engine for drawing text on an SDL renderer, with the
 * specified properties.
 *
 * These are the supported properties:
 *
 * - `TTF_PROP_RENDERER_TEXT_ENGINE_RENDERER`: the renderer to use for
 *   creating textures and drawing text
 * - `prop::RendererTextEngine.ATLAS_TEXTURE_SIZE`: the size of the
 *   texture atlas
 *
 * @param props the properties to use.
 * @returns a TextEngine object or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               renderer.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa CreateRendererTextEngine
 * @sa Text
 * @sa TextBase.DrawRenderer
 */
inline TextEngine CreateRendererTextEngineWithProperties(PropertiesRef props)
{
  return TextEngine{
    TextEngineWrapper{TTF_CreateRendererTextEngineWithProperties(props.get()),
                      TTF_DestroyRendererTextEngine}};
}

namespace prop::RendererTextEngine {

constexpr auto RENDERER = TTF_PROP_RENDERER_TEXT_ENGINE_RENDERER;

constexpr auto ATLAS_TEXTURE_SIZE =
  TTF_PROP_RENDERER_TEXT_ENGINE_ATLAS_TEXTURE_SIZE;

} // namespace prop::RendererTextEngine

/**
 * Create a text engine for drawing text with the SDL GPU API.
 *
 * @param device the SDL_GPUDevice to use for creating textures and drawing
 *               text.
 * @returns a TextEngine object or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               device.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa CreateGPUTextEngineWithProperties
 * @sa Text
 * @sa TextBase.GetGPUDrawData
 */
inline TextEngine CreateGPUTextEngine(SDL_GPUDevice* device)
{
  return TextEngine{TextEngineWrapper{TTF_CreateGPUTextEngine(device),
                                      TTF_DestroyGPUTextEngine}};
}

/**
 * Create a text engine for drawing text with the SDL GPU API, with the
 * specified properties.
 *
 * These are the supported properties:
 *
 * - `TTF_PROP_GPU_TEXT_ENGINE_DEVICE`: the SDL_GPUDevice to use for creating
 *   textures and drawing text.
 * - `prop::GpuTextEngine.ATLAS_TEXTURE_SIZE`: the size of the texture
 *   atlas
 *
 * @param props the properties to use.
 * @returns a TextEngine object or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               device.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa CreateGPUTextEngine
 * @sa Text
 * @sa TextBase.GetGPUDrawData
 */
inline TextEngine CreateGPUTextEngineWithProperties(PropertiesRef props)
{
  return TextEngine{
    TextEngineWrapper{TTF_CreateGPUTextEngineWithProperties(props.get()),
                      TTF_DestroyGPUTextEngine}};
}

namespace prop::GpuTextEngine {

constexpr auto DEVICE = TTF_PROP_GPU_TEXT_ENGINE_DEVICE;

constexpr auto ATLAS_TEXTURE_SIZE = TTF_PROP_GPU_TEXT_ENGINE_ATLAS_TEXTURE_SIZE;

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
 * deal with it. A well-written program should call FontBase.Close() on any
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
 * QuitSubSystem(TtfInitFlag).
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

template<ObjectBox<TTF_Text*> T>
SubStringIterator TextBase<T>::begin() const
{
  if (SubStringIterator it{*this}; GetSubString(0, &it.m_subString)) return it;
  return {};
}

template<ObjectBox<TTF_Text*> T>
SubStringIterator TextBase<T>::end() const
{
  if (SubStringIterator it{*this}; GetSubString(INT_MAX, &it.m_subString)) {
    return it;
  }
  return {};
}

template<ObjectBox<TTF_Text*> T>
SubStringIterator TextBase<T>::GetSubStringForLine(int line) const
{
  if (SubStringIterator it{*this}; GetSubStringForLine(line, &it.m_subString)) {
    return it;
  }
  return {};
}

template<ObjectBox<TTF_Text*> T>
SubStringIterator TextBase<T>::GetSubStringForPoint(Point p) const
{
  if (SubStringIterator it{*this}; GetSubStringForPoint(p, &it.m_subString)) {
    return it;
  }
  return {};
}

#pragma endregion impl

} // namespace SDL

#endif // defined(SDL3PP_ENABLE_TTF) || defined(SDL3PP_DOC)

#endif /* SDL3PP_TTF_H_ */
