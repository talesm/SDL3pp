#ifndef SDL3PP_TTF_H_
#define SDL3PP_TTF_H_

#include "SDL3pp_gpu.h"
#include "SDL3pp_render.h"
#include "SDL3pp_surface.h"
#include "SDL3pp_version.h"
#include "SDL3pp_video.h"

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
struct Font;

/// Alias to raw representation for Font.
using FontRaw = TTF_Font*;

// Forward decl
struct FontRef;

/// Safely wrap Font for non owning parameters
struct FontParam
{
  FontRaw value; ///< parameter's FontRaw

  /// Constructs from FontRaw
  constexpr FontParam(FontRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr FontParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const FontParam& other) const = default;

  /// Converts to underlying FontRaw
  constexpr operator FontRaw() const { return value; }
};

// Forward decl
struct TextEngine;

/// Alias to raw representation for TextEngine.
using TextEngineRaw = TTF_TextEngine*;

/// Safely wrap TextEngine for non owning parameters
struct TextEngineParam
{
  TextEngineRaw value; ///< parameter's TextEngineRaw

  /// Constructs from TextEngineRaw
  constexpr TextEngineParam(TextEngineRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr TextEngineParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const TextEngineParam& other) const = default;

  /// Converts to underlying TextEngineRaw
  constexpr operator TextEngineRaw() const { return value; }
};

// Forward decl
struct Text;

/// Alias to raw representation for Text.
using TextRaw = TTF_Text*;

// Forward decl
struct TextRef;

/// Safely wrap Text for non owning parameters
struct TextParam
{
  TextRaw value; ///< parameter's TextRaw

  /// Constructs from TextRaw
  constexpr TextParam(TextRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr TextParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const TextParam& other) const = default;

  /// Converts to underlying TextRaw
  constexpr operator TextRaw() const { return value; }

  /// member access to underlying TextRaw.
  constexpr auto operator->() { return value; }
};

/// Safely wrap Text for non owning const parameters
struct TextConstParam
{
  const TextRaw value; ///< parameter's const TextRaw

  /// Constructs from const TextRaw
  constexpr TextConstParam(const TextRaw value)
    : value(value)
  {
  }

  /// Constructs from TextParam
  constexpr TextConstParam(TextParam value)
    : value(value.value)
  {
  }

  /// Constructs null/invalid
  constexpr TextConstParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const TextConstParam& other) const = default;

  /// Converts to underlying const TextRaw
  constexpr operator const TextRaw() const { return value; }

  /// member access to underlying TextRaw.
  constexpr auto operator->() { return value; }
};

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

namespace TTF {

/**
 * This function gets the version of the dynamically linked SDL_ttf library.
 *
 * @returns SDL_ttf version.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline int Version() { return TTF_Version(); }

/**
 * Initialize SDL_ttf.
 *
 * You must successfully call this function before it is safe to call any
 * other function in this library.
 *
 * It is safe to call this more than once, and each successful TTF.Init() call
 * should be paired with a matching TTF.Quit() call.
 *
 * @throws Error on failure.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF.Quit
 */
inline void Init() { CheckError(TTF_Init()); }

/**
 * Deinitialize SDL_ttf.
 *
 * You must call this when done with the library, to free internal resources.
 * It is safe to call this when the library isn't initialized, as it will just
 * return immediately.
 *
 * Once you have as many quit calls as you have had successful calls to
 * TTF.Init, the library will actually deinitialize.
 *
 * Please note that this does not automatically close any fonts that are still
 * open at the time of deinitialization, and it is possibly not safe to close
 * them afterwards, as parts of the library will no longer be initialized to
 * deal with it. A well-written program should call Font.Close() on any
 * open fonts before calling this function!
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void Quit() { TTF_Quit(); }

/**
 * Check if SDL_ttf is initialized.
 *
 * This reports the number of times the library has been initialized by a call
 * to TTF.Init(), without a paired deinitialization request from TTF.Quit().
 *
 * In short: if it's greater than zero, the library is currently initialized
 * and ready to work. If zero, it is not initialized.
 *
 * Despite the return value being a signed integer, this function should not
 * return a negative number.
 *
 * @returns the current number of initialization calls, that need to
 *          eventually be paired with this many calls to TTF.Quit().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF.Init
 * @sa TTF.Quit
 */
inline int WasInit() { return TTF_WasInit(); }

} // namespace TTF

/**
 * Query the version of the FreeType library in use.
 *
 * TTF.Init() should be called before calling this function.
 *
 * @param major to be filled in with the major version number. Can be nullptr.
 * @param minor to be filled in with the minor version number. Can be nullptr.
 * @param patch to be filled in with the param version number. Can be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TTF.Init
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
 * Font style flags for Font
 *
 * These are the flags which can be used to set the style of a font in
 * SDL_ttf. A combination of these flags can be used with functions that set
 * or query font style, such as Font.SetStyle or Font.GetStyle.
 *
 * @since This datatype is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetStyle
 * @sa Font.GetStyle
 */
using FontStyleFlags = Uint32;

constexpr FontStyleFlags STYLE_NORMAL = TTF_STYLE_NORMAL; ///< No special style

constexpr FontStyleFlags STYLE_BOLD = TTF_STYLE_BOLD; ///< Bold style

constexpr FontStyleFlags STYLE_ITALIC = TTF_STYLE_ITALIC; ///< Italic style

constexpr FontStyleFlags STYLE_UNDERLINE =
  TTF_STYLE_UNDERLINE; ///< Underlined text

constexpr FontStyleFlags STYLE_STRIKETHROUGH =
  TTF_STYLE_STRIKETHROUGH; ///< Strikethrough text

/**
 * Hinting flags for TTF (TrueType Fonts)
 *
 * This enum specifies the level of hinting to be applied to the font
 * rendering. The hinting level determines how much the font's outlines are
 * adjusted for better alignment on the pixel grid.
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetHinting
 * @sa Font.GetHinting
 */
using HintingFlags = TTF_HintingFlags;

constexpr HintingFlags HINTING_INVALID = TTF_HINTING_INVALID; ///< INVALID

constexpr HintingFlags HINTING_NORMAL =
  TTF_HINTING_NORMAL; ///< Normal hinting applies standard grid-fitting.

/// Light hinting applies subtle adjustments to improve rendering.
constexpr HintingFlags HINTING_LIGHT = TTF_HINTING_LIGHT;

/**
 * Monochrome hinting adjusts the font for better rendering at lower
 * resolutions.
 */
constexpr HintingFlags HINTING_MONO = TTF_HINTING_MONO;

/// No hinting, the font is rendered without any grid-fitting.
constexpr HintingFlags HINTING_NONE = TTF_HINTING_NONE;

/// Light hinting with subpixel rendering for more precise font edges.
constexpr HintingFlags HINTING_LIGHT_SUBPIXEL = TTF_HINTING_LIGHT_SUBPIXEL;

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

/**
 * Direction flags
 *
 * The values here are chosen to match
 * [hb_direction_t](https://harfbuzz.github.io/harfbuzz-hb-common.html#hb-direction-t)
 *
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetDirection
 */
using Direction = TTF_Direction;

constexpr Direction DIRECTION_INVALID = TTF_DIRECTION_INVALID; ///< INVALID

constexpr Direction DIRECTION_LTR = TTF_DIRECTION_LTR; ///< Left to Right

constexpr Direction DIRECTION_RTL = TTF_DIRECTION_RTL; ///< Right to Left

constexpr Direction DIRECTION_TTB = TTF_DIRECTION_TTB; ///< Top to Bottom

constexpr Direction DIRECTION_BTT = TTF_DIRECTION_BTT; ///< Bottom to Top

/**
 * The type of data in a glyph image
 *
 * @since This enum is available since SDL_ttf 3.0.0.
 */
using ImageType = TTF_ImageType;

constexpr ImageType IMAGE_INVALID = TTF_IMAGE_INVALID; ///< INVALID

constexpr ImageType IMAGE_ALPHA =
  TTF_IMAGE_ALPHA; ///< The color channels are white

constexpr ImageType IMAGE_COLOR =
  TTF_IMAGE_COLOR; ///< The color channels have image data

/// The alpha channel has signed distance field information
constexpr ImageType IMAGE_SDF = TTF_IMAGE_SDF;

/**
 * The internal structure containing font information.
 *
 * Opaque data!
 *
 * @cat resource
 */
class Font
{
  FontRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Font() = default;

  /**
   * Constructs from FontParam.
   *
   * @param resource a FontRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Font(const FontRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Font(const Font& other) = delete;

  /// Move constructor
  constexpr Font(Font&& other)
    : Font(other.release())
  {
  }

  constexpr Font(const FontRef& other) = delete;

  constexpr Font(FontRef&& other) = delete;

  /**
   * Create a font from a file, using a specified point size.
   *
   * Some .fon fonts will have several sizes embedded in the file, so the point
   * size becomes the index of choosing which size. If the value is too high,
   * the last indexed size will be the default.
   *
   * @param file path to font file.
   * @param ptsize point size to use for the newly-opened font.
   * @post a valid Font on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.Close
   */
  Font(StringParam file, float ptsize)
    : m_resource(CheckError(TTF_OpenFont(file, ptsize)))
  {
  }

  /**
   * Create a font from an IOStream, using a specified point size.
   *
   * Some .fon fonts will have several sizes embedded in the file, so the point
   * size becomes the index of choosing which size. If the value is too high,
   * the last indexed size will be the default.
   *
   * If `closeio` is true, `src` will be automatically closed once the font is
   * closed. Otherwise you should keep `src` open until the font is closed.
   *
   * @param src an IOStream to provide a font file's data.
   * @param ptsize point size to use for the newly-opened font.
   * @param closeio true to close `src` when the font is closed, false to leave
   *                it open.
   * @post a valid Font on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.Close
   */
  Font(IOStreamParam src, float ptsize, bool closeio = false)
    : m_resource(CheckError(TTF_OpenFontIO(src, closeio, ptsize)))
  {
  }

  /**
   * Create a font with the specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::Font.CREATE_FILENAME_STRING`: the font file to open, if an
   *   IOStream isn't being used. This is required if
   *   `prop::Font.CREATE_IOSTREAM_POINTER` and
   *   `prop::Font.CREATE_EXISTING_FONT_POINTER` aren't set.
   * - `prop::Font.CREATE_IOSTREAM_POINTER`: an IOStream containing the
   *   font to be opened. This should not be closed until the font is closed.
   *   This is required if `prop::Font.CREATE_FILENAME_STRING` and
   *   `prop::Font.CREATE_EXISTING_FONT_POINTER` aren't set.
   * - `prop::Font.CREATE_IOSTREAM_OFFSET_NUMBER`: the offset in the iostream
   *   for the beginning of the font, defaults to 0.
   * - `prop::Font.CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN`: true if closing the
   *   font should also close the associated IOStream.
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
   * - `prop::Font.CREATE_EXISTING_FONT_POINTER`: an optional Font that, if set,
   *   will be used as the font data source and the initial size and style of
   *   the new font.
   *
   * @param props the properties to use.
   * @post a valid Font on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.Close
   */
  Font(PropertiesParam props)
    : m_resource(CheckError(TTF_OpenFontWithProperties(props)))
  {
  }

  /// Destructor
  ~Font() { TTF_CloseFont(m_resource); }

  /// Assignment operator.
  Font& operator=(Font other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying FontRaw.
  constexpr FontRaw get() const { return m_resource; }

  /// Retrieves underlying FontRaw and clear this.
  constexpr FontRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Font& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to FontParam
  constexpr operator FontParam() const { return {m_resource}; }

  /**
   * Dispose of a previously-created font.
   *
   * Call this when done with a font. This function will free any resources
   * associated with it. It is safe to call this function on nullptr, for
   * example on the result of a failed call to Font.Font().
   *
   * The font is not valid after being passed to this function. String pointers
   * from functions that return information on this font, such as
   * Font.GetFamilyName() and Font.GetStyleName(), are no longer valid
   * after this call, as well.
   *
   * @threadsafety This function should not be called while any other thread is
   *               using the font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.Font
   */
  void Close();

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
   * @sa Font.Close
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
  PropertiesRef GetProperties();

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
  Uint32 GetGeneration() const;

  /**
   * Add a fallback font.
   *
   * Add a font that will be used for glyphs that are not in the current font.
   * The fallback font should have the same size and style as the current font.
   *
   * If there are multiple fallback fonts, they are used in the order added.
   *
   * This updates any Text objects using this font.
   *
   * @param fallback the font to add as a fallback.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created
   *               both fonts.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.ClearFallbacks
   * @sa Font.RemoveFallback
   */
  void AddFallback(FontParam fallback);

  /**
   * Remove a fallback font.
   *
   * This updates any Text objects using this font.
   *
   * @param fallback the font to remove as a fallback.
   *
   * @threadsafety This function should be called on the thread that created
   *               both fonts.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.AddFallback
   * @sa Font.ClearFallbacks
   */
  void RemoveFallback(FontParam fallback);

  /**
   * Remove all fallback fonts.
   *
   * This updates any Text objects using this font.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.AddFallback
   * @sa Font.RemoveFallback
   */
  void ClearFallbacks();

  /**
   * Set a font's size dynamically.
   *
   * This updates any Text objects using this font, and clears
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
   * @sa Font.GetSize
   */
  void SetSize(float ptsize);

  /**
   * Set font size dynamically with target resolutions, in dots per inch.
   *
   * This updates any Text objects using this font, and clears
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
   * @sa Font.GetSize
   * @sa TTF_GetFontSizeDPI
   */
  void SetSizeDPI(float ptsize, int hdpi, int vdpi);

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
   * @sa Font.SetSize
   * @sa Font.SetSizeDPI
   */
  float GetSize() const;

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
   * @sa Font.SetSizeDPI
   */
  void GetDPI(int* hdpi, int* vdpi) const;

  /**
   * Set a font's current style.
   *
   * This updates any Text objects using this font, and clears
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
   * @sa Font.GetStyle
   */
  void SetStyle(FontStyleFlags style);

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
   * @sa Font.SetStyle
   */
  FontStyleFlags GetStyle() const;

  /**
   * Set a font's current outline.
   *
   * This uses the font properties `prop::Font.OUTLINE_LINE_CAP_NUMBER`,
   * `prop::Font.OUTLINE_LINE_JOIN_NUMBER`, and
   * `prop::Font.OUTLINE_MITER_LIMIT_NUMBER` when setting the font outline.
   *
   * This updates any Text objects using this font, and clears
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
   * @sa Font.GetOutline
   */
  void SetOutline(int outline);

  /**
   * Query a font's current outline.
   *
   * @returns the font's current outline value.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.SetOutline
   */
  int GetOutline() const;

  /**
   * Set a font's current hinter setting.
   *
   * This updates any Text objects using this font, and clears
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
   * @sa Font.GetHinting
   */
  void SetHinting(HintingFlags hinting);

  /**
   * Query the number of faces of a font.
   *
   * @returns the number of FreeType font faces.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  int GetNumFaces() const;

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
   * @sa Font.SetHinting
   */
  HintingFlags GetHinting() const;

  /**
   * Enable Signed Distance Field rendering for a font.
   *
   * SDF is a technique that helps fonts look sharp even when scaling and
   * rotating, and requires special shader support for display.
   *
   * This works with Blended APIs, and generates the raw signed distance values
   * in the alpha channel of the resulting texture.
   *
   * This updates any Text objects using this font, and clears
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
   * @sa Font.GetSDF
   */
  void SetSDF(bool enabled);

  /**
   * Query whether Signed Distance Field rendering is enabled for a font.
   *
   * @returns true if enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.SetSDF
   */
  bool GetSDF() const;

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
  int GetWeight() const;

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 2)

  /**
   * Set a font's current wrap alignment option.
   *
   * This updates any Text objects using this font.
   *
   * @param align the new wrap alignment option.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.GetWrapAlignment
   */
  void SetWrapAlignment(HorizontalAlignment align);

  /**
   * Query a font's current wrap alignment option.
   *
   * @returns the font's current wrap alignment option.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.SetWrapAlignment
   */
  HorizontalAlignment GetWrapAlignment() const;

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
  int GetHeight() const;

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
  int GetAscent() const;

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
  int GetDescent() const;

  /**
   * Set the spacing between lines of text for a font.
   *
   * This updates any Text objects using this font.
   *
   * @param lineskip the new line spacing for the font.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.GetLineSkip
   */
  void SetLineSkip(int lineskip);

  /**
   * Query the spacing between lines of text for a font.
   *
   * @returns the font's recommended spacing.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.SetLineSkip
   */
  int GetLineSkip() const;

  /**
   * Set if kerning is enabled for a font.
   *
   * Newly-opened fonts default to allowing kerning. This is generally a good
   * policy unless you have a strong reason to disable it, as it tends to
   * produce better rendering (with kerning disabled, some fonts might render
   * the word `kerning` as something that looks like `keming` for example).
   *
   * This updates any Text objects using this font.
   *
   * @param enabled true to enable kerning, false to disable.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.GetKerning
   */
  void SetKerning(bool enabled);

  /**
   * Query whether or not kerning is enabled for a font.
   *
   * @returns true if kerning is enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Font.SetKerning
   */
  bool GetKerning() const;

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
  bool IsFixedWidth() const;

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
   * @sa Font.SetSDF
   */
  bool IsScalable() const;

  /**
   * Query a font's family name.
   *
   * This string is dictated by the contents of the font file.
   *
   * Note that the returned string is to internal storage, and should not be
   * modified or free'd by the caller. The string becomes invalid, with the rest
   * of the font, when `font` is handed to Font.Close().
   *
   * @returns the font's family name.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  const char* GetFamilyName() const;

  /**
   * Query a font's style name.
   *
   * This string is dictated by the contents of the font file.
   *
   * Note that the returned string is to internal storage, and should not be
   * modified or free'd by the caller. The string becomes invalid, with the rest
   * of the font, when `font` is handed to Font.Close().
   *
   * @returns the font's style name.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  const char* GetStyleName() const;

  /**
   * Set the direction to be used for text shaping by a font.
   *
   * This function only supports left-to-right text shaping if SDL_ttf was not
   * built with HarfBuzz support.
   *
   * This updates any Text objects using this font.
   *
   * @param direction the new direction for text to flow.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void SetDirection(Direction direction);

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
  Direction GetDirection() const;

  /**
   * Set the script to be used for text shaping by a font.
   *
   * This returns false if SDL_ttf isn't built with HarfBuzz support.
   *
   * This updates any Text objects using this font.
   *
   * @param script an [ISO 15924
   *               code](https://unicode.org/iso15924/iso15924-codes.html).
   *
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa StringToTag
   */
  void SetScript(Uint32 script);

  /**
   * Get the script used for text shaping a font.
   *
   * @returns an
   *          [ISO 15924 code](https://unicode.org/iso15924/iso15924-codes.html)
   *          or 0 if a script hasn't been set.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa TagToString
   */
  Uint32 GetScript() const;

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
  static Uint32 GetGlyphScript(Uint32 ch);

  /**
   * Set language to be used for text shaping by a font.
   *
   * If SDL_ttf was not built with HarfBuzz support, this function returns
   * false.
   *
   * This updates any Text objects using this font.
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
  void SetLanguage(StringParam language_bcp47);

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
  bool HasGlyph(Uint32 ch) const;

  /**
   * Get the pixel image for a UNICODE codepoint.
   *
   * @param ch the codepoint to check.
   * @param image_type a pointer filled in with the glyph image type, may be
   *                   nullptr.
   * @returns an Surface containing the glyph, or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Surface GetGlyphImage(Uint32 ch, ImageType* image_type) const;

  /**
   * Get the pixel image for a character index.
   *
   * This is useful for text engine implementations, which can call this with
   * the `glyph_index` in a TTF_CopyOperation
   *
   * @param glyph_index the index of the glyph to return.
   * @param image_type a pointer filled in with the glyph image type, may be
   *                   nullptr.
   * @returns an Surface containing the glyph, or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  Surface GetGlyphImageForIndex(Uint32 glyph_index,
                                ImageType* image_type) const;

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
                       int* advance) const;

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
  int GetGlyphKerning(Uint32 previous_ch, Uint32 ch) const;

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
  void GetStringSize(std::string_view text, int* w, int* h) const;

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
                            int* h) const;

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
                     size_t* measured_length) const;

  /**
   * Render UTF-8 text at fast quality to a new 8-bit surface.
   *
   * This function will allocate a new 8-bit, palettized surface. The surface's
   * 0 pixel will be the colorkey, giving a transparent background. The 1 pixel
   * will be set to the text color.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * Font.RenderText_Solid_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with Font.RenderText_Shaded,
   * Font.RenderText_Blended, and Font.RenderText_LCD.
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
   * @sa Font.RenderText_Blended
   * @sa Font.RenderText_LCD
   * @sa Font.RenderText_Shaded
   * @sa Font.RenderText_Solid
   * @sa Font.RenderText_Solid_Wrapped
   */
  Surface RenderText_Solid(std::string_view text, Color fg) const;

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
   * You can render at other quality levels with Font.RenderText_Shaded_Wrapped,
   * Font.RenderText_Blended_Wrapped, and Font.RenderText_LCD_Wrapped.
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
   * @sa Font.RenderText_Blended_Wrapped
   * @sa Font.RenderText_LCD_Wrapped
   * @sa Font.RenderText_Shaded_Wrapped
   * @sa Font.RenderText_Solid
   */
  Surface RenderText_Solid_Wrapped(std::string_view text,
                                   Color fg,
                                   int wrapLength) const;

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
   * You can render at other quality levels with Font.RenderGlyph_Shaded,
   * Font.RenderGlyph_Blended, and Font.RenderGlyph_LCD.
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
   * @sa Font.RenderGlyph_Blended
   * @sa Font.RenderGlyph_LCD
   * @sa Font.RenderGlyph_Shaded
   */
  Surface RenderGlyph_Solid(Uint32 ch, ColorRaw fg) const;

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
   * Font.RenderText_Shaded_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with Font.RenderText_Solid,
   * Font.RenderText_Blended, and Font.RenderText_LCD.
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
   * @sa Font.RenderText_Blended
   * @sa Font.RenderText_LCD
   * @sa Font.RenderText_Shaded_Wrapped
   * @sa Font.RenderText_Solid
   */
  Surface RenderText_Shaded(std::string_view text, Color fg, Color bg) const;

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
   * You can render at other quality levels with Font.RenderText_Solid_Wrapped,
   * Font.RenderText_Blended_Wrapped, and Font.RenderText_LCD_Wrapped.
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
   * @sa Font.RenderText_Blended_Wrapped
   * @sa Font.RenderText_LCD_Wrapped
   * @sa Font.RenderText_Shaded
   * @sa Font.RenderText_Solid_Wrapped
   */
  Surface RenderText_Shaded_Wrapped(std::string_view text,
                                    Color fg,
                                    Color bg,
                                    int wrap_width) const;

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
   * You can render at other quality levels with Font.RenderGlyph_Solid,
   * Font.RenderGlyph_Blended, and Font.RenderGlyph_LCD.
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
   * @sa Font.RenderGlyph_Blended
   * @sa Font.RenderGlyph_LCD
   * @sa Font.RenderGlyph_Solid
   */
  Surface RenderGlyph_Shaded(Uint32 ch, ColorRaw fg, ColorRaw bg) const;

  /**
   * Render UTF-8 text at high quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, using alpha
   * blending to dither the font with the given color. This function returns the
   * new surface, or nullptr if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * Font.RenderText_Blended_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with Font.RenderText_Solid,
   * Font.RenderText_Shaded, and Font.RenderText_LCD.
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
   * @sa Font.RenderText_Blended_Wrapped
   * @sa Font.RenderText_LCD
   * @sa Font.RenderText_Shaded
   * @sa Font.RenderText_Solid
   */
  Surface RenderText_Blended(std::string_view text, Color fg) const;

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
   * You can render at other quality levels with Font.RenderText_Solid_Wrapped,
   * Font.RenderText_Shaded_Wrapped, and Font.RenderText_LCD_Wrapped.
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
   * @sa Font.RenderText_Blended
   * @sa Font.RenderText_LCD_Wrapped
   * @sa Font.RenderText_Shaded_Wrapped
   * @sa Font.RenderText_Solid_Wrapped
   */
  Surface RenderText_Blended_Wrapped(std::string_view text,
                                     Color fg,
                                     int wrap_width) const;

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
   * You can render at other quality levels with Font.RenderGlyph_Solid,
   * Font.RenderGlyph_Shaded, and Font.RenderGlyph_LCD.
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
   * @sa Font.RenderGlyph_LCD
   * @sa Font.RenderGlyph_Shaded
   * @sa Font.RenderGlyph_Solid
   */
  Surface RenderGlyph_Blended(Uint32 ch, ColorRaw fg) const;

  /**
   * Render UTF-8 text at LCD subpixel quality to a new ARGB surface.
   *
   * This function will allocate a new 32-bit, ARGB surface, and render
   * alpha-blended text using FreeType's LCD subpixel rendering. This function
   * returns the new surface, or nullptr if there was an error.
   *
   * This will not word-wrap the string; you'll get a surface with a single line
   * of text, as long as the string requires. You can use
   * Font.RenderText_LCD_Wrapped() instead if you need to wrap the output to
   * multiple lines.
   *
   * This will not wrap on newline characters.
   *
   * You can render at other quality levels with Font.RenderText_Solid,
   * Font.RenderText_Shaded, and Font.RenderText_Blended.
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
   * @sa Font.RenderText_Blended
   * @sa Font.RenderText_LCD_Wrapped
   * @sa Font.RenderText_Shaded
   * @sa Font.RenderText_Solid
   */
  Surface RenderText_LCD(std::string_view text, Color fg, Color bg) const;

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
   * You can render at other quality levels with Font.RenderText_Solid_Wrapped,
   * Font.RenderText_Shaded_Wrapped, and Font.RenderText_Blended_Wrapped.
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
   * @sa Font.RenderText_Blended_Wrapped
   * @sa Font.RenderText_LCD
   * @sa Font.RenderText_Shaded_Wrapped
   * @sa Font.RenderText_Solid_Wrapped
   */
  Surface RenderText_LCD_Wrapped(std::string_view text,
                                 Color fg,
                                 Color bg,
                                 int wrap_width) const;

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
   * You can render at other quality levels with Font.RenderGlyph_Solid,
   * Font.RenderGlyph_Shaded, and Font.RenderGlyph_Blended.
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
   * @sa Font.RenderGlyph_Blended
   * @sa Font.RenderGlyph_Shaded
   * @sa Font.RenderGlyph_Solid
   */
  Surface RenderGlyph_LCD(Uint32 ch, ColorRaw fg, ColorRaw bg) const;
};

/// Semi-safe reference for Font.
struct FontRef : Font
{
  /**
   * Constructs from FontParam.
   *
   * @param resource a FontRaw or Font.
   *
   * This does not takes ownership!
   */
  FontRef(FontParam resource)
    : Font(resource.value)
  {
  }

  /// Copy constructor.
  FontRef(const FontRef& other)
    : Font(other.get())
  {
  }

  /// Destructor
  ~FontRef() { release(); }
};

/**
 * Create a font from a file, using a specified point size.
 *
 * Some .fon fonts will have several sizes embedded in the file, so the point
 * size becomes the index of choosing which size. If the value is too high,
 * the last indexed size will be the default.
 *
 * When done with the returned Font, use Font.Close() to dispose of it.
 *
 * @param file path to font file.
 * @param ptsize point size to use for the newly-opened font.
 * @returns a valid Font on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.Close
 */
inline Font OpenFont(StringParam file, float ptsize)
{
  return Font(std::move(file), ptsize);
}

/**
 * Create a font from an IOStream, using a specified point size.
 *
 * Some .fon fonts will have several sizes embedded in the file, so the point
 * size becomes the index of choosing which size. If the value is too high,
 * the last indexed size will be the default.
 *
 * If `closeio` is true, `src` will be automatically closed once the font is
 * closed. Otherwise you should keep `src` open until the font is closed.
 *
 * When done with the returned Font, use Font.Close() to dispose of it.
 *
 * @param src an IOStream to provide a font file's data.
 * @param ptsize point size to use for the newly-opened font.
 * @param closeio true to close `src` when the font is closed, false to leave
 *                it open.
 * @returns a valid Font on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.Close
 */
inline Font OpenFont(IOStreamParam src, float ptsize, bool closeio = false)
{
  return Font(src, ptsize, closeio);
}

/**
 * Create a font with the specified properties.
 *
 * These are the supported properties:
 *
 * - `prop::Font.CREATE_FILENAME_STRING`: the font file to open, if an
 *   IOStream isn't being used. This is required if
 *   `prop::Font.CREATE_IOSTREAM_POINTER` and
 *   `prop::Font.CREATE_EXISTING_FONT_POINTER` aren't set.
 * - `prop::Font.CREATE_IOSTREAM_POINTER`: an IOStream containing the
 *   font to be opened. This should not be closed until the font is closed.
 *   This is required if `prop::Font.CREATE_FILENAME_STRING` and
 *   `prop::Font.CREATE_EXISTING_FONT_POINTER` aren't set.
 * - `prop::Font.CREATE_IOSTREAM_OFFSET_NUMBER`: the offset in the iostream
 *   for the beginning of the font, defaults to 0.
 * - `prop::Font.CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN`: true if closing the
 *   font should also close the associated IOStream.
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
 * - `prop::Font.CREATE_EXISTING_FONT_POINTER`: an optional Font that, if set,
 *   will be used as the font data source and the initial size and style of
 *   the new font.
 *
 * @param props the properties to use.
 * @returns a valid Font on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.Close
 */
inline Font OpenFontWithProperties(PropertiesParam props)
{
  return Font(props);
}

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

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 3)

constexpr auto OUTLINE_LINE_CAP_NUMBER = TTF_PROP_FONT_OUTLINE_LINE_CAP_NUMBER;

constexpr auto OUTLINE_LINE_JOIN_NUMBER =
  TTF_PROP_FONT_OUTLINE_LINE_JOIN_NUMBER;

constexpr auto OUTLINE_MITER_LIMIT_NUMBER =
  TTF_PROP_FONT_OUTLINE_MITER_LIMIT_NUMBER;

} // namespace prop::Font

/**
 * Create a copy of an existing font.
 *
 * The copy will be distinct from the original, but will share the font file
 * and have the same size and style as the original.
 *
 * When done with the returned Font, use Font.Close() to dispose of it.
 *
 * @param existing_font the font to copy.
 * @returns a valid Font on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               original font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.Close
 */
inline Font CopyFont(FontParam existing_font)
{
  return Font(CheckError(TTF_CopyFont(existing_font)));
}

inline Font Font::Copy() const { return SDL::CopyFont(m_resource); }

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
 * @param font the font to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline PropertiesRef GetFontProperties(FontParam font)
{
  return {CheckError(TTF_GetFontProperties(font))};
}

inline PropertiesRef Font::GetProperties()
{
  return SDL::GetFontProperties(m_resource);
}

/**
 * Get the font generation.
 *
 * The generation is incremented each time font properties change that require
 * rebuilding glyphs, such as style, size, etc.
 *
 * @param font the font to query.
 * @returns the font generation or 0 on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline Uint32 GetFontGeneration(FontParam font)
{
  return TTF_GetFontGeneration(font);
}

inline Uint32 Font::GetGeneration() const
{
  return SDL::GetFontGeneration(m_resource);
}

/**
 * Add a fallback font.
 *
 * Add a font that will be used for glyphs that are not in the current font.
 * The fallback font should have the same size and style as the current font.
 *
 * If there are multiple fallback fonts, they are used in the order added.
 *
 * This updates any Text objects using this font.
 *
 * @param font the font to modify.
 * @param fallback the font to add as a fallback.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created
 *               both fonts.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.ClearFallbacks
 * @sa Font.RemoveFallback
 */
inline void AddFallbackFont(FontParam font, FontParam fallback)
{
  CheckError(TTF_AddFallbackFont(font, fallback));
}

inline void Font::AddFallback(FontParam fallback)
{
  SDL::AddFallbackFont(m_resource, fallback);
}

/**
 * Remove a fallback font.
 *
 * This updates any Text objects using this font.
 *
 * @param font the font to modify.
 * @param fallback the font to remove as a fallback.
 *
 * @threadsafety This function should be called on the thread that created
 *               both fonts.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.AddFallback
 * @sa Font.ClearFallbacks
 */
inline void RemoveFallbackFont(FontParam font, FontParam fallback)
{
  TTF_RemoveFallbackFont(font, fallback);
}

inline void Font::RemoveFallback(FontParam fallback)
{
  SDL::RemoveFallbackFont(m_resource, fallback);
}

/**
 * Remove all fallback fonts.
 *
 * This updates any Text objects using this font.
 *
 * @param font the font to modify.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.AddFallback
 * @sa Font.RemoveFallback
 */
inline void ClearFallbackFonts(FontParam font) { TTF_ClearFallbackFonts(font); }

inline void Font::ClearFallbacks() { SDL::ClearFallbackFonts(m_resource); }

/**
 * Set a font's size dynamically.
 *
 * This updates any Text objects using this font, and clears
 * already-generated glyphs, if any, from the cache.
 *
 * @param font the font to resize.
 * @param ptsize the new point size.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.GetSize
 */
inline void SetFontSize(FontParam font, float ptsize)
{
  CheckError(TTF_SetFontSize(font, ptsize));
}

inline void Font::SetSize(float ptsize)
{
  SDL::SetFontSize(m_resource, ptsize);
}

/**
 * Set font size dynamically with target resolutions, in dots per inch.
 *
 * This updates any Text objects using this font, and clears
 * already-generated glyphs, if any, from the cache.
 *
 * @param font the font to resize.
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
 * @sa Font.GetSize
 * @sa TTF_GetFontSizeDPI
 */
inline void SetFontSizeDPI(FontParam font, float ptsize, int hdpi, int vdpi)
{
  CheckError(TTF_SetFontSizeDPI(font, ptsize, hdpi, vdpi));
}

inline void Font::SetSizeDPI(float ptsize, int hdpi, int vdpi)
{
  SDL::SetFontSizeDPI(m_resource, ptsize, hdpi, vdpi);
}

/**
 * Get the size of a font.
 *
 * @param font the font to query.
 * @returns the size of the font, or 0.0f on failure; call GetError() for
 *          more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetSize
 * @sa Font.SetSizeDPI
 */
inline float GetFontSize(FontParam font) { return TTF_GetFontSize(font); }

inline float Font::GetSize() const { return SDL::GetFontSize(m_resource); }

/**
 * Get font target resolutions, in dots per inch.
 *
 * @param font the font to query.
 * @param hdpi a pointer filled in with the target horizontal DPI.
 * @param vdpi a pointer filled in with the target vertical DPI.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetSizeDPI
 */
inline void GetFontDPI(FontParam font, int* hdpi, int* vdpi)
{
  CheckError(TTF_GetFontDPI(font, hdpi, vdpi));
}

inline void Font::GetDPI(int* hdpi, int* vdpi) const
{
  SDL::GetFontDPI(m_resource, hdpi, vdpi);
}

/**
 * Set a font's current style.
 *
 * This updates any Text objects using this font, and clears
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
 * @param font the font to set a new style on.
 * @param style the new style values to set, OR'd together.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.GetStyle
 */
inline void SetFontStyle(FontParam font, FontStyleFlags style)
{
  TTF_SetFontStyle(font, style);
}

inline void Font::SetStyle(FontStyleFlags style)
{
  SDL::SetFontStyle(m_resource, style);
}

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
 * @param font the font to query.
 * @returns the current font style, as a set of bit flags.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetStyle
 */
inline FontStyleFlags GetFontStyle(FontParam font)
{
  return TTF_GetFontStyle(font);
}

inline FontStyleFlags Font::GetStyle() const
{
  return SDL::GetFontStyle(m_resource);
}

/**
 * Set a font's current outline.
 *
 * This uses the font properties `prop::Font.OUTLINE_LINE_CAP_NUMBER`,
 * `prop::Font.OUTLINE_LINE_JOIN_NUMBER`, and
 * `prop::Font.OUTLINE_MITER_LIMIT_NUMBER` when setting the font outline.
 *
 * This updates any Text objects using this font, and clears
 * already-generated glyphs, if any, from the cache.
 *
 * @param font the font to set a new outline on.
 * @param outline positive outline value, 0 to default.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.GetOutline
 */
inline void SetFontOutline(FontParam font, int outline)
{
  CheckError(TTF_SetFontOutline(font, outline));
}

inline void Font::SetOutline(int outline)
{
  SDL::SetFontOutline(m_resource, outline);
}

/**
 * Query a font's current outline.
 *
 * @param font the font to query.
 * @returns the font's current outline value.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetOutline
 */
inline int GetFontOutline(FontParam font) { return TTF_GetFontOutline(font); }

inline int Font::GetOutline() const { return SDL::GetFontOutline(m_resource); }

/**
 * Set a font's current hinter setting.
 *
 * This updates any Text objects using this font, and clears
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
 * @param font the font to set a new hinter setting on.
 * @param hinting the new hinter setting.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.GetHinting
 */
inline void SetFontHinting(FontParam font, HintingFlags hinting)
{
  TTF_SetFontHinting(font, hinting);
}

inline void Font::SetHinting(HintingFlags hinting)
{
  SDL::SetFontHinting(m_resource, hinting);
}

/**
 * Query the number of faces of a font.
 *
 * @param font the font to query.
 * @returns the number of FreeType font faces.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline int GetNumFontFaces(FontParam font) { return TTF_GetNumFontFaces(font); }

inline int Font::GetNumFaces() const
{
  return SDL::GetNumFontFaces(m_resource);
}

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
 * @param font the font to query.
 * @returns the font's current hinter value, or HINTING_INVALID if the
 *          font is invalid.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetHinting
 */
inline HintingFlags GetFontHinting(FontParam font)
{
  return TTF_GetFontHinting(font);
}

inline HintingFlags Font::GetHinting() const
{
  return SDL::GetFontHinting(m_resource);
}

/**
 * Enable Signed Distance Field rendering for a font.
 *
 * SDF is a technique that helps fonts look sharp even when scaling and
 * rotating, and requires special shader support for display.
 *
 * This works with Blended APIs, and generates the raw signed distance values
 * in the alpha channel of the resulting texture.
 *
 * This updates any Text objects using this font, and clears
 * already-generated glyphs, if any, from the cache.
 *
 * @param font the font to set SDF support on.
 * @param enabled true to enable SDF, false to disable.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.GetSDF
 */
inline void SetFontSDF(FontParam font, bool enabled)
{
  CheckError(TTF_SetFontSDF(font, enabled));
}

inline void Font::SetSDF(bool enabled) { SDL::SetFontSDF(m_resource, enabled); }

/**
 * Query whether Signed Distance Field rendering is enabled for a font.
 *
 * @param font the font to query.
 * @returns true if enabled, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetSDF
 */
inline bool GetFontSDF(FontParam font) { return TTF_GetFontSDF(font); }

inline bool Font::GetSDF() const { return SDL::GetFontSDF(m_resource); }

#if SDL_TTF_VERSION_ATLEAST(3, 2, 2)

/**
 * Query a font's weight, in terms of the lightness/heaviness of the strokes.
 *
 * @param font the font to query.
 * @returns the font's current weight.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.2.2.
 */
inline int GetFontWeight(FontParam font) { return TTF_GetFontWeight(font); }

inline int Font::GetWeight() const { return SDL::GetFontWeight(m_resource); }

constexpr int FONT_WEIGHT_THIN =
  TTF_FONT_WEIGHT_THIN; ///< Thin (100) named font weight value

constexpr int FONT_WEIGHT_EXTRA_LIGHT =
  TTF_FONT_WEIGHT_EXTRA_LIGHT; ///< ExtraLight (200) named font weight value

constexpr int FONT_WEIGHT_LIGHT =
  TTF_FONT_WEIGHT_LIGHT; ///< Light (300) named font weight value

constexpr int FONT_WEIGHT_NORMAL =
  TTF_FONT_WEIGHT_NORMAL; ///< Normal (400) named font weight value

constexpr int FONT_WEIGHT_MEDIUM =
  TTF_FONT_WEIGHT_MEDIUM; ///< Medium (500) named font weight value

constexpr int FONT_WEIGHT_SEMI_BOLD =
  TTF_FONT_WEIGHT_SEMI_BOLD; ///< SemiBold (600) named font weight value

constexpr int FONT_WEIGHT_BOLD =
  TTF_FONT_WEIGHT_BOLD; ///< Bold (700) named font weight value

constexpr int FONT_WEIGHT_EXTRA_BOLD =
  TTF_FONT_WEIGHT_EXTRA_BOLD; ///< ExtraBold (800) named font weight value

constexpr int FONT_WEIGHT_BLACK =
  TTF_FONT_WEIGHT_BLACK; ///< Black (900) named font weight value

constexpr int FONT_WEIGHT_EXTRA_BLACK =
  TTF_FONT_WEIGHT_EXTRA_BLACK; ///< ExtraBlack (950) named font weight value

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 2)

/**
 * Set a font's current wrap alignment option.
 *
 * This updates any Text objects using this font.
 *
 * @param font the font to set a new wrap alignment option on.
 * @param align the new wrap alignment option.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.GetWrapAlignment
 */
inline void SetFontWrapAlignment(FontParam font, HorizontalAlignment align)
{
  TTF_SetFontWrapAlignment(font, align);
}

inline void Font::SetWrapAlignment(HorizontalAlignment align)
{
  SDL::SetFontWrapAlignment(m_resource, align);
}

/**
 * Query a font's current wrap alignment option.
 *
 * @param font the font to query.
 * @returns the font's current wrap alignment option.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetWrapAlignment
 */
inline HorizontalAlignment GetFontWrapAlignment(FontParam font)
{
  return TTF_GetFontWrapAlignment(font);
}

inline HorizontalAlignment Font::GetWrapAlignment() const
{
  return SDL::GetFontWrapAlignment(m_resource);
}

/**
 * Query the total height of a font.
 *
 * This is usually equal to point size.
 *
 * @param font the font to query.
 * @returns the font's height.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline int GetFontHeight(FontParam font) { return TTF_GetFontHeight(font); }

inline int Font::GetHeight() const { return SDL::GetFontHeight(m_resource); }

/**
 * Query the offset from the baseline to the top of a font.
 *
 * This is a positive value, relative to the baseline.
 *
 * @param font the font to query.
 * @returns the font's ascent.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline int GetFontAscent(FontParam font) { return TTF_GetFontAscent(font); }

inline int Font::GetAscent() const { return SDL::GetFontAscent(m_resource); }

/**
 * Query the offset from the baseline to the bottom of a font.
 *
 * This is a negative value, relative to the baseline.
 *
 * @param font the font to query.
 * @returns the font's descent.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline int GetFontDescent(FontParam font) { return TTF_GetFontDescent(font); }

inline int Font::GetDescent() const { return SDL::GetFontDescent(m_resource); }

/**
 * Set the spacing between lines of text for a font.
 *
 * This updates any Text objects using this font.
 *
 * @param font the font to modify.
 * @param lineskip the new line spacing for the font.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.GetLineSkip
 */
inline void SetFontLineSkip(FontParam font, int lineskip)
{
  TTF_SetFontLineSkip(font, lineskip);
}

inline void Font::SetLineSkip(int lineskip)
{
  SDL::SetFontLineSkip(m_resource, lineskip);
}

/**
 * Query the spacing between lines of text for a font.
 *
 * @param font the font to query.
 * @returns the font's recommended spacing.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetLineSkip
 */
inline int GetFontLineSkip(FontParam font) { return TTF_GetFontLineSkip(font); }

inline int Font::GetLineSkip() const
{
  return SDL::GetFontLineSkip(m_resource);
}

/**
 * Set if kerning is enabled for a font.
 *
 * Newly-opened fonts default to allowing kerning. This is generally a good
 * policy unless you have a strong reason to disable it, as it tends to
 * produce better rendering (with kerning disabled, some fonts might render
 * the word `kerning` as something that looks like `keming` for example).
 *
 * This updates any Text objects using this font.
 *
 * @param font the font to set kerning on.
 * @param enabled true to enable kerning, false to disable.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.GetKerning
 */
inline void SetFontKerning(FontParam font, bool enabled)
{
  TTF_SetFontKerning(font, enabled);
}

inline void Font::SetKerning(bool enabled)
{
  SDL::SetFontKerning(m_resource, enabled);
}

/**
 * Query whether or not kerning is enabled for a font.
 *
 * @param font the font to query.
 * @returns true if kerning is enabled, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetKerning
 */
inline bool GetFontKerning(FontParam font) { return TTF_GetFontKerning(font); }

inline bool Font::GetKerning() const { return SDL::GetFontKerning(m_resource); }

/**
 * Query whether a font is fixed-width.
 *
 * A "fixed-width" font means all glyphs are the same width across; a
 * lowercase 'i' will be the same size across as a capital 'W', for example.
 * This is common for terminals and text editors, and other apps that treat
 * text as a grid. Most other things (WYSIWYG word processors, web pages, etc)
 * are more likely to not be fixed-width in most cases.
 *
 * @param font the font to query.
 * @returns true if the font is fixed-width, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool FontIsFixedWidth(FontParam font)
{
  return TTF_FontIsFixedWidth(font);
}

inline bool Font::IsFixedWidth() const
{
  return SDL::FontIsFixedWidth(m_resource);
}

/**
 * Query whether a font is scalable or not.
 *
 * Scalability lets us distinguish between outline and bitmap fonts.
 *
 * @param font the font to query.
 * @returns true if the font is scalable, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.SetSDF
 */
inline bool FontIsScalable(FontParam font) { return TTF_FontIsScalable(font); }

inline bool Font::IsScalable() const { return SDL::FontIsScalable(m_resource); }

/**
 * Query a font's family name.
 *
 * This string is dictated by the contents of the font file.
 *
 * Note that the returned string is to internal storage, and should not be
 * modified or free'd by the caller. The string becomes invalid, with the rest
 * of the font, when `font` is handed to Font.Close().
 *
 * @param font the font to query.
 * @returns the font's family name.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline const char* GetFontFamilyName(FontParam font)
{
  return TTF_GetFontFamilyName(font);
}

inline const char* Font::GetFamilyName() const
{
  return SDL::GetFontFamilyName(m_resource);
}

/**
 * Query a font's style name.
 *
 * This string is dictated by the contents of the font file.
 *
 * Note that the returned string is to internal storage, and should not be
 * modified or free'd by the caller. The string becomes invalid, with the rest
 * of the font, when `font` is handed to Font.Close().
 *
 * @param font the font to query.
 * @returns the font's style name.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline const char* GetFontStyleName(FontParam font)
{
  return TTF_GetFontStyleName(font);
}

inline const char* Font::GetStyleName() const
{
  return SDL::GetFontStyleName(m_resource);
}

/**
 * Set the direction to be used for text shaping by a font.
 *
 * This function only supports left-to-right text shaping if SDL_ttf was not
 * built with HarfBuzz support.
 *
 * This updates any Text objects using this font.
 *
 * @param font the font to modify.
 * @param direction the new direction for text to flow.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void SetFontDirection(FontParam font, Direction direction)
{
  CheckError(TTF_SetFontDirection(font, direction));
}

inline void Font::SetDirection(Direction direction)
{
  SDL::SetFontDirection(m_resource, direction);
}

/**
 * Get the direction to be used for text shaping by a font.
 *
 * This defaults to DIRECTION_INVALID if it hasn't been set.
 *
 * @param font the font to query.
 * @returns the direction to be used for text shaping.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline Direction GetFontDirection(FontParam font)
{
  return TTF_GetFontDirection(font);
}

inline Direction Font::GetDirection() const
{
  return SDL::GetFontDirection(m_resource);
}

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
 * Set the script to be used for text shaping by a font.
 *
 * This returns false if SDL_ttf isn't built with HarfBuzz support.
 *
 * This updates any Text objects using this font.
 *
 * @param font the font to modify.
 * @param script an [ISO 15924
 * code](https://unicode.org/iso15924/iso15924-codes.html).
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa StringToTag
 */
inline void SetFontScript(FontParam font, Uint32 script)
{
  CheckError(TTF_SetFontScript(font, script));
}

inline void Font::SetScript(Uint32 script)
{
  SDL::SetFontScript(m_resource, script);
}

/**
 * Get the script used for text shaping a font.
 *
 * @param font the font to query.
 * @returns an
 *          [ISO 15924 code](https://unicode.org/iso15924/iso15924-codes.html)
 *          or 0 if a script hasn't been set.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa TagToString
 */
inline Uint32 GetFontScript(FontParam font) { return TTF_GetFontScript(font); }

inline Uint32 Font::GetScript() const { return SDL::GetFontScript(m_resource); }

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
inline Uint32 GetGlyphScript(Uint32 ch) { return TTF_GetGlyphScript(ch); }

inline Uint32 Font::GetGlyphScript(Uint32 ch)
{
  return SDL::GetGlyphScript(ch);
}

/**
 * Set language to be used for text shaping by a font.
 *
 * If SDL_ttf was not built with HarfBuzz support, this function returns
 * false.
 *
 * This updates any Text objects using this font.
 *
 * @param font the font to specify a language for.
 * @param language_bcp47 a null-terminated string containing the desired
 *                       language's BCP47 code. Or null to reset the value.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void SetFontLanguage(FontParam font, StringParam language_bcp47)
{
  CheckError(TTF_SetFontLanguage(font, language_bcp47));
}

inline void Font::SetLanguage(StringParam language_bcp47)
{
  SDL::SetFontLanguage(m_resource, std::move(language_bcp47));
}

/**
 * Check whether a glyph is provided by the font for a UNICODE codepoint.
 *
 * @param font the font to query.
 * @param ch the codepoint to check.
 * @returns true if font provides a glyph for this character, false if not.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline bool FontHasGlyph(FontParam font, Uint32 ch)
{
  return TTF_FontHasGlyph(font, ch);
}

inline bool Font::HasGlyph(Uint32 ch) const
{
  return SDL::FontHasGlyph(m_resource, ch);
}

/**
 * Get the pixel image for a UNICODE codepoint.
 *
 * @param font the font to query.
 * @param ch the codepoint to check.
 * @param image_type a pointer filled in with the glyph image type, may be
 *                   nullptr.
 * @returns an Surface containing the glyph, or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline Surface GetGlyphImage(FontParam font, Uint32 ch, ImageType* image_type)
{
  return Surface{TTF_GetGlyphImage(font, ch, image_type)};
}

inline Surface Font::GetGlyphImage(Uint32 ch, ImageType* image_type) const
{
  return SDL::GetGlyphImage(m_resource, ch, image_type);
}

/**
 * Get the pixel image for a character index.
 *
 * This is useful for text engine implementations, which can call this with
 * the `glyph_index` in a TTF_CopyOperation
 *
 * @param font the font to query.
 * @param glyph_index the index of the glyph to return.
 * @param image_type a pointer filled in with the glyph image type, may be
 *                   nullptr.
 * @returns an Surface containing the glyph, or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline Surface GetGlyphImageForIndex(FontParam font,
                                     Uint32 glyph_index,
                                     ImageType* image_type)
{
  return Surface(TTF_GetGlyphImageForIndex(font, glyph_index, image_type));
}

inline Surface Font::GetGlyphImageForIndex(Uint32 glyph_index,
                                           ImageType* image_type) const
{
  return SDL::GetGlyphImageForIndex(m_resource, glyph_index, image_type);
}

/**
 * Query the metrics (dimensions) of a font's glyph for a UNICODE codepoint.
 *
 * To understand what these metrics mean, here is a useful link:
 *
 * https://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
 *
 * @param font the font to query.
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
inline void GetGlyphMetrics(FontParam font,
                            Uint32 ch,
                            int* minx,
                            int* maxx,
                            int* miny,
                            int* maxy,
                            int* advance)
{
  CheckError(TTF_GetGlyphMetrics(font, ch, minx, maxx, miny, maxy, advance));
}

inline void Font::GetGlyphMetrics(Uint32 ch,
                                  int* minx,
                                  int* maxx,
                                  int* miny,
                                  int* maxy,
                                  int* advance) const
{
  SDL::GetGlyphMetrics(m_resource, ch, minx, maxx, miny, maxy, advance);
}

/**
 * Query the kerning size between the glyphs of two UNICODE codepoints.
 *
 * @param font the font to query.
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
inline int GetGlyphKerning(FontParam font, Uint32 previous_ch, Uint32 ch)
{
  if (int r; TTF_GetGlyphKerning(font, previous_ch, ch, &r)) return r;
  throw Error();
}

inline int Font::GetGlyphKerning(Uint32 previous_ch, Uint32 ch) const
{
  return SDL::GetGlyphKerning(m_resource, previous_ch, ch);
}

/**
 * Calculate the dimensions of a rendered string of UTF-8 text.
 *
 * This will report the width and height, in pixels, of the space that the
 * specified string will take to fully render.
 *
 * @param font the font to query.
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
inline void GetStringSize(FontParam font, std::string_view text, int* w, int* h)
{
  CheckError(TTF_GetStringSize(font, text.data(), text.size(), w, h));
}

inline void Font::GetStringSize(std::string_view text, int* w, int* h) const
{
  SDL::GetStringSize(m_resource, text, w, h);
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
 * @param font the font to query.
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
inline void GetStringSizeWrapped(FontParam font,
                                 std::string_view text,
                                 int wrap_width,
                                 int* w,
                                 int* h)
{
  CheckError(
    TTF_GetStringSizeWrapped(font, text.data(), text.size(), wrap_width, w, h));
}

inline void Font::GetStringSizeWrapped(std::string_view text,
                                       int wrap_width,
                                       int* w,
                                       int* h) const
{
  SDL::GetStringSizeWrapped(m_resource, text, wrap_width, w, h);
}

/**
 * Calculate how much of a UTF-8 string will fit in a given width.
 *
 * This reports the number of characters that can be rendered before reaching
 * `max_width`.
 *
 * This does not need to render the string to do this calculation.
 *
 * @param font the font to query.
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
inline void MeasureString(FontParam font,
                          std::string_view text,
                          int max_width,
                          int* measured_width,
                          size_t* measured_length)
{
  CheckError(TTF_MeasureString(font,
                               text.data(),
                               text.size(),
                               max_width,
                               measured_width,
                               measured_length));
}

inline void Font::MeasureString(std::string_view text,
                                int max_width,
                                int* measured_width,
                                size_t* measured_length) const
{
  SDL::MeasureString(
    m_resource, text, max_width, measured_width, measured_length);
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
 * Font.RenderText_Solid_Wrapped() instead if you need to wrap the output to
 * multiple lines.
 *
 * This will not wrap on newline characters.
 *
 * You can render at other quality levels with Font.RenderText_Shaded,
 * Font.RenderText_Blended, and Font.RenderText_LCD.
 *
 * @param font the font to render with.
 * @param text text to render, in UTF-8 encoding.
 * @param fg the foreground color for the text.
 * @returns a new 8-bit, palettized surface, or nullptr if there was an error.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.RenderText_Blended
 * @sa Font.RenderText_LCD
 * @sa Font.RenderText_Shaded
 * @sa Font.RenderText_Solid
 * @sa Font.RenderText_Solid_Wrapped
 */
inline Surface RenderText_Solid(FontParam font, std::string_view text, Color fg)
{
  return Surface{TTF_RenderText_Solid(font, text.data(), text.size(), fg)};
}

inline Surface Font::RenderText_Solid(std::string_view text, Color fg) const
{
  return SDL::RenderText_Solid(m_resource, text, fg);
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
 * You can render at other quality levels with Font.RenderText_Shaded_Wrapped,
 * Font.RenderText_Blended_Wrapped, and Font.RenderText_LCD_Wrapped.
 *
 * @param font the font to render with.
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
 * @sa Font.RenderText_Blended_Wrapped
 * @sa Font.RenderText_LCD_Wrapped
 * @sa Font.RenderText_Shaded_Wrapped
 * @sa Font.RenderText_Solid
 */
inline Surface RenderText_Solid_Wrapped(FontParam font,
                                        std::string_view text,
                                        Color fg,
                                        int wrapLength)
{
  return Surface(TTF_RenderText_Solid_Wrapped(
    font, text.data(), text.size(), fg, wrapLength));
}

inline Surface Font::RenderText_Solid_Wrapped(std::string_view text,
                                              Color fg,
                                              int wrapLength) const
{
  return SDL::RenderText_Solid_Wrapped(m_resource, text, fg, wrapLength);
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
 * You can render at other quality levels with Font.RenderGlyph_Shaded,
 * Font.RenderGlyph_Blended, and Font.RenderGlyph_LCD.
 *
 * @param font the font to render with.
 * @param ch the character to render.
 * @param fg the foreground color for the text.
 * @returns a new 8-bit, palettized surface, or nullptr if there was an error.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.RenderGlyph_Blended
 * @sa Font.RenderGlyph_LCD
 * @sa Font.RenderGlyph_Shaded
 */
inline Surface RenderGlyph_Solid(FontParam font, Uint32 ch, ColorRaw fg)
{
  return Surface(TTF_RenderGlyph_Solid(font, ch, fg));
}

inline Surface Font::RenderGlyph_Solid(Uint32 ch, ColorRaw fg) const
{
  return SDL::RenderGlyph_Solid(m_resource, ch, fg);
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
 * Font.RenderText_Shaded_Wrapped() instead if you need to wrap the output to
 * multiple lines.
 *
 * This will not wrap on newline characters.
 *
 * You can render at other quality levels with Font.RenderText_Solid,
 * Font.RenderText_Blended, and Font.RenderText_LCD.
 *
 * @param font the font to render with.
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
 * @sa Font.RenderText_Blended
 * @sa Font.RenderText_LCD
 * @sa Font.RenderText_Shaded_Wrapped
 * @sa Font.RenderText_Solid
 */
inline Surface RenderText_Shaded(FontParam font,
                                 std::string_view text,
                                 Color fg,
                                 Color bg)
{
  return Surface(TTF_RenderText_Shaded(font, text.data(), text.size(), fg, bg));
}

inline Surface Font::RenderText_Shaded(std::string_view text,
                                       Color fg,
                                       Color bg) const
{
  return SDL::RenderText_Shaded(m_resource, text, fg, bg);
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
 * You can render at other quality levels with Font.RenderText_Solid_Wrapped,
 * Font.RenderText_Blended_Wrapped, and Font.RenderText_LCD_Wrapped.
 *
 * @param font the font to render with.
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
 * @sa Font.RenderText_Blended_Wrapped
 * @sa Font.RenderText_LCD_Wrapped
 * @sa Font.RenderText_Shaded
 * @sa Font.RenderText_Solid_Wrapped
 */
inline Surface RenderText_Shaded_Wrapped(FontParam font,
                                         std::string_view text,
                                         Color fg,
                                         Color bg,
                                         int wrap_width)
{
  return Surface(TTF_RenderText_Shaded_Wrapped(
    font, text.data(), text.size(), fg, bg, wrap_width));
}

inline Surface Font::RenderText_Shaded_Wrapped(std::string_view text,
                                               Color fg,
                                               Color bg,
                                               int wrap_width) const
{
  return SDL::RenderText_Shaded_Wrapped(m_resource, text, fg, bg, wrap_width);
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
 * You can render at other quality levels with Font.RenderGlyph_Solid,
 * Font.RenderGlyph_Blended, and Font.RenderGlyph_LCD.
 *
 * @param font the font to render with.
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
 * @sa Font.RenderGlyph_Blended
 * @sa Font.RenderGlyph_LCD
 * @sa Font.RenderGlyph_Solid
 */
inline Surface RenderGlyph_Shaded(FontParam font,
                                  Uint32 ch,
                                  ColorRaw fg,
                                  ColorRaw bg)
{
  return Surface(TTF_RenderGlyph_Shaded(font, ch, fg, bg));
}

inline Surface Font::RenderGlyph_Shaded(Uint32 ch,
                                        ColorRaw fg,
                                        ColorRaw bg) const
{
  return SDL::RenderGlyph_Shaded(m_resource, ch, fg, bg);
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
 * Font.RenderText_Blended_Wrapped() instead if you need to wrap the output to
 * multiple lines.
 *
 * This will not wrap on newline characters.
 *
 * You can render at other quality levels with Font.RenderText_Solid,
 * Font.RenderText_Shaded, and Font.RenderText_LCD.
 *
 * @param font the font to render with.
 * @param text text to render, in UTF-8 encoding.
 * @param fg the foreground color for the text.
 * @returns a new 32-bit, ARGB surface, or nullptr if there was an error.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.RenderText_Blended_Wrapped
 * @sa Font.RenderText_LCD
 * @sa Font.RenderText_Shaded
 * @sa Font.RenderText_Solid
 */
inline Surface RenderText_Blended(FontParam font,
                                  std::string_view text,
                                  Color fg)
{
  return Surface(TTF_RenderText_Blended(font, text.data(), text.size(), fg));
}

inline Surface Font::RenderText_Blended(std::string_view text, Color fg) const
{
  return SDL::RenderText_Blended(m_resource, text, fg);
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
 * You can render at other quality levels with Font.RenderText_Solid_Wrapped,
 * Font.RenderText_Shaded_Wrapped, and Font.RenderText_LCD_Wrapped.
 *
 * @param font the font to render with.
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
 * @sa Font.RenderText_Blended
 * @sa Font.RenderText_LCD_Wrapped
 * @sa Font.RenderText_Shaded_Wrapped
 * @sa Font.RenderText_Solid_Wrapped
 */
inline Surface RenderText_Blended_Wrapped(FontParam font,
                                          std::string_view text,
                                          Color fg,
                                          int wrap_width)
{
  return Surface(TTF_RenderText_Blended_Wrapped(
    font, text.data(), text.size(), fg, wrap_width));
}

inline Surface Font::RenderText_Blended_Wrapped(std::string_view text,
                                                Color fg,
                                                int wrap_width) const
{
  return SDL::RenderText_Blended_Wrapped(m_resource, text, fg, wrap_width);
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
 * You can render at other quality levels with Font.RenderGlyph_Solid,
 * Font.RenderGlyph_Shaded, and Font.RenderGlyph_LCD.
 *
 * @param font the font to render with.
 * @param ch the codepoint to render.
 * @param fg the foreground color for the text.
 * @returns a new 32-bit, ARGB surface, or nullptr if there was an error.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.RenderGlyph_LCD
 * @sa Font.RenderGlyph_Shaded
 * @sa Font.RenderGlyph_Solid
 */
inline Surface RenderGlyph_Blended(FontParam font, Uint32 ch, ColorRaw fg)
{
  return Surface(TTF_RenderGlyph_Blended(font, ch, fg));
}

inline Surface Font::RenderGlyph_Blended(Uint32 ch, ColorRaw fg) const
{
  return SDL::RenderGlyph_Blended(m_resource, ch, fg);
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
 * Font.RenderText_LCD_Wrapped() instead if you need to wrap the output to
 * multiple lines.
 *
 * This will not wrap on newline characters.
 *
 * You can render at other quality levels with Font.RenderText_Solid,
 * Font.RenderText_Shaded, and Font.RenderText_Blended.
 *
 * @param font the font to render with.
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
 * @sa Font.RenderText_Blended
 * @sa Font.RenderText_LCD_Wrapped
 * @sa Font.RenderText_Shaded
 * @sa Font.RenderText_Solid
 */
inline Surface RenderText_LCD(FontParam font,
                              std::string_view text,
                              Color fg,
                              Color bg)
{
  return Surface(TTF_RenderText_LCD(font, text.data(), text.size(), fg, bg));
}

inline Surface Font::RenderText_LCD(std::string_view text,
                                    Color fg,
                                    Color bg) const
{
  return SDL::RenderText_LCD(m_resource, text, fg, bg);
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
 * You can render at other quality levels with Font.RenderText_Solid_Wrapped,
 * Font.RenderText_Shaded_Wrapped, and Font.RenderText_Blended_Wrapped.
 *
 * @param font the font to render with.
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
 * @sa Font.RenderText_Blended_Wrapped
 * @sa Font.RenderText_LCD
 * @sa Font.RenderText_Shaded_Wrapped
 * @sa Font.RenderText_Solid_Wrapped
 */
inline Surface RenderText_LCD_Wrapped(FontParam font,
                                      std::string_view text,
                                      Color fg,
                                      Color bg,
                                      int wrap_width)
{
  return Surface(TTF_RenderText_LCD_Wrapped(
    font, text.data(), text.size(), fg, bg, wrap_width));
}

inline Surface Font::RenderText_LCD_Wrapped(std::string_view text,
                                            Color fg,
                                            Color bg,
                                            int wrap_width) const
{
  return SDL::RenderText_LCD_Wrapped(m_resource, text, fg, bg, wrap_width);
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
 * You can render at other quality levels with Font.RenderGlyph_Solid,
 * Font.RenderGlyph_Shaded, and Font.RenderGlyph_Blended.
 *
 * @param font the font to render with.
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
 * @sa Font.RenderGlyph_Blended
 * @sa Font.RenderGlyph_Shaded
 * @sa Font.RenderGlyph_Solid
 */
inline Surface RenderGlyph_LCD(FontParam font,
                               Uint32 ch,
                               ColorRaw fg,
                               ColorRaw bg)
{
  return Surface(TTF_RenderGlyph_LCD(font, ch, fg, bg));
}

inline Surface Font::RenderGlyph_LCD(Uint32 ch, ColorRaw fg, ColorRaw bg) const
{
  return SDL::RenderGlyph_LCD(m_resource, ch, fg, bg);
}

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

/// This substring contains the beginning of line `line_index`
constexpr SubStringFlags SUBSTRING_LINE_START = TTF_SUBSTRING_LINE_START;

/// This substring contains the end of line `line_index`
constexpr SubStringFlags SUBSTRING_LINE_END = TTF_SUBSTRING_LINE_END;

constexpr SubStringFlags SUBSTRING_TEXT_END =
  TTF_SUBSTRING_TEXT_END; ///< This substring contains the end of the text

/**
 * The winding order of the vertices returned by Text.GetGPUDrawData
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

/**
 * A text engine used to create text objects.
 *
 * This is a public interface that can be used by applications and libraries
 * to perform customize rendering with text objects. See
 * <SDL3_ttf/SDL_textengine.h> for details.
 *
 * There are three text engines provided with the library:
 *
 * - Drawing to an Surface, created with SurfaceTextEngine.SurfaceTextEngine()
 * - Drawing with an SDL 2D renderer, created with
 *   RendererTextEngine.RendererTextEngine()
 * - Drawing with the SDL GPU API, created with GPUTextEngine.GPUTextEngine()
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @cat resource
 */
class TextEngine
{
  TextEngineRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr TextEngine() = default;

  /**
   * Constructs from TextEngineParam.
   *
   * @param resource a TextEngineRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit TextEngine(const TextEngineRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr TextEngine(const TextEngine& other) = delete;

  /// Move constructor
  constexpr TextEngine(TextEngine&& other)
    : TextEngine(other.release())
  {
  }

  /// Destructor
  virtual ~TextEngine() = default;

  /// Assignment operator.
  TextEngine& operator=(TextEngine&& other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Assignment operator.
  TextEngine& operator=(const TextEngine& other) = delete;

  /// Retrieves underlying TextEngineRaw.
  constexpr TextEngineRaw get() const { return m_resource; }

  /// Retrieves underlying TextEngineRaw and clear this.
  constexpr TextEngineRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const TextEngine& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to TextEngineParam
  constexpr operator TextEngineParam() const { return {m_resource}; }

  /// frees up textEngine. Pure virtual
  virtual void Destroy() = 0;

  /**
   * Create a text object from UTF-8 text and a text engine.
   *
   *               nullptr.
   * @param font the font to render with.
   * @param text the text to use, in UTF-8 encoding.
   * @returns a Text object or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font and text engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.Destroy
   */
  Text CreateText(FontParam font, std::string_view text);
};

/// A surface based text engine
struct SurfaceTextEngine : TextEngine
{
  /**
   * Create a text engine for drawing text on SDL surfaces.
   *
   * @post a TextEngine object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SurfaceTextEngine.Destroy
   * @sa Text.DrawSurface
   */
  SurfaceTextEngine()
    : TextEngine(TTF_CreateSurfaceTextEngine())
  {
  }

  ~SurfaceTextEngine() { Destroy(); }

  /**
   * Destroy a text engine created for drawing text on SDL surfaces.
   *
   * All text created by this engine should be destroyed before calling this
   * function.
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SurfaceTextEngine.SurfaceTextEngine
   */
  void Destroy() final;
};

/// A renderer based text engine
struct RendererTextEngine : TextEngine
{
  /**
   * Create a text engine for drawing text on an SDL renderer.
   *
   * @param renderer the renderer to use for creating textures and drawing text.
   * @post a TextEngine object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               renderer.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa RendererTextEngine.Destroy
   * @sa Text.DrawRenderer
   * @sa RendererTextEngine.RendererTextEngine
   */
  RendererTextEngine(RendererParam renderer)
    : TextEngine(TTF_CreateRendererTextEngine(renderer))
  {
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
   * @post a TextEngine object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               renderer.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa RendererTextEngine.RendererTextEngine
   * @sa RendererTextEngine.Destroy
   * @sa Text.DrawRenderer
   */
  RendererTextEngine(PropertiesParam props)
    : TextEngine(TTF_CreateRendererTextEngineWithProperties(props))
  {
  }

  ~RendererTextEngine() { Destroy(); }

  /**
   * Destroy a text engine created for drawing text on an SDL renderer.
   *
   * All text created by this engine should be destroyed before calling this
   * function.
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa RendererTextEngine.RendererTextEngine
   */
  void Destroy() final;
};

/// A GPU based text engine
struct GPUTextEngine : TextEngine
{
  /**
   * Create a text engine for drawing text with the SDL GPU API.
   *
   * @param device the GPUDevice to use for creating textures and drawing
   *               text.
   * @post a TextEngine object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               device.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa GPUTextEngine.GPUTextEngine
   * @sa GPUTextEngine.Destroy
   * @sa Text.GetGPUDrawData
   */
  GPUTextEngine(GPUDeviceParam device)
    : TextEngine(TTF_CreateGPUTextEngine(device))
  {
  }

  /**
   * Create a text engine for drawing text with the SDL GPU API, with the
   * specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::GpuTextEngine.DEVICE_POINTER`: the GPUDevice to use for creating
   *   textures and drawing text.
   * - `prop::GpuTextEngine.ATLAS_TEXTURE_SIZE_NUMBER`: the size of the texture
   *   atlas
   *
   * @param props the properties to use.
   * @post a TextEngine object or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               device.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa GPUTextEngine.GPUTextEngine
   * @sa GPUTextEngine.Destroy
   * @sa Text.GetGPUDrawData
   */
  GPUTextEngine(PropertiesParam props)
    : TextEngine(TTF_CreateGPUTextEngineWithProperties(props))
  {
  }

  ~GPUTextEngine() { Destroy(); }

  /**
   * Sets the winding order of the vertices returned by Text.GetGPUDrawData
   * for a particular GPU text engine.
   *
   * @param winding the new winding order option.
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa GPUTextEngine.GetGPUWinding
   */
  void SetGPUWinding(GPUTextEngineWinding winding);

  /**
   * Get the winding order of the vertices returned by Text.GetGPUDrawData
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
   * @sa GPUTextEngine.SetGPUWinding
   */
  GPUTextEngineWinding GetGPUWinding() const;

  /**
   * Destroy a text engine created for drawing text with the SDL GPU API.
   *
   * All text created by this engine should be destroyed before calling this
   * function.
   *
   * @threadsafety This function should be called on the thread that created the
   *               engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa GPUTextEngine.GPUTextEngine
   */
  void Destroy() final;
};

/**
 * Draw sequence returned by Text.GetGPUDrawData
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetGPUDrawData
 */
using GPUAtlasDrawSequence = TTF_GPUAtlasDrawSequence;

/**
 * The representation of a substring within text.
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetNextSubString
 * @sa Text.GetPreviousSubString
 * @sa Text.GetSubString
 * @sa Text.GetSubStringForLine
 * @sa Text.GetSubStringForPoint
 * @sa Text.GetSubStringsForRange
 */
using SubString = TTF_SubString;

// Forward decl
struct SubStringIterator;

/**
 * Internal data for Text
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 */
using TextData = TTF_TextData;

/**
 * Text created with Text.Text()
 *
 * @since This struct is available since SDL_ttf 3.0.0.
 *
 * @sa Text.Text
 * @sa Text.GetProperties
 * @sa Text.Destroy
 *
 * @cat resource
 */
class Text
{
  TextRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Text() = default;

  /**
   * Constructs from TextParam.
   *
   * @param resource a TextRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Text(const TextRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Text(const Text& other) = delete;

  /// Move constructor
  constexpr Text(Text&& other)
    : Text(other.release())
  {
  }

  constexpr Text(const TextRef& other) = delete;

  constexpr Text(TextRef&& other) = delete;

  /**
   * Create a text object from UTF-8 text and a text engine.
   *
   * @param engine the text engine to use when creating the text object, may be
   *               nullptr.
   * @param font the font to render with.
   * @param text the text to use, in UTF-8 encoding.
   * @post a Text object or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should be called on the thread that created the
   *               font and text engine.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.Destroy
   */
  Text(TextEngineParam engine, FontParam font, std::string_view text)
    : m_resource(TTF_CreateText(engine, font, text.data(), text.size()))
  {
  }

  /// member access to underlying TextRaw.
  constexpr const TextRaw operator->() const { return m_resource; }

  /// member access to underlying TextRaw.
  constexpr TextRaw operator->() { return m_resource; }

  /// Destructor
  ~Text() { TTF_DestroyText(m_resource); }

  /// Assignment operator.
  Text& operator=(Text other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying TextRaw.
  constexpr TextRaw get() const { return m_resource; }

  /// Retrieves underlying TextRaw and clear this.
  constexpr TextRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Text& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to TextParam
  constexpr operator TextParam() const { return {m_resource}; }

  /**
   * Destroy a text object created by a text engine.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.Text
   */
  void Destroy();

  /**
   * Draw text to an SDL surface.
   *
   * `text` must have been created using a TextEngine from
   * SurfaceTextEngine.SurfaceTextEngine().
   *
   * @param p the x,y coordinates in pixels, positive from the top-left edge
   *          towards the bottom-right.
   * @param surface the surface to draw on.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa SurfaceTextEngine.SurfaceTextEngine
   * @sa Text.Text
   */
  void DrawSurface(Point p, SurfaceParam surface) const;

  /**
   * Draw text to an SDL renderer.
   *
   * `text` must have been created using a TextEngine from
   * RendererTextEngine.RendererTextEngine(), and will draw using the renderer
   * passed to that function.
   *
   * @param p the x,y coordinates in pixels, positive from the top-left edge
   *          towards the bottom-right.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa RendererTextEngine.RendererTextEngine
   * @sa Text.Text
   */
  void DrawRenderer(FPoint p) const;

  /**
   * Get the geometry data needed for drawing the text.
   *
   * `text` must have been created using a TextEngine from
   * GPUTextEngine.GPUTextEngine().
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
   * @sa GPUTextEngine.GPUTextEngine
   * @sa Text.Text
   */
  GPUAtlasDrawSequence* GetGPUDrawData() const;

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
  PropertiesRef GetProperties() const;

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
   * @sa Text.GetEngine
   */
  void SetEngine(TextEngineParam engine);

  /**
   * Get the text engine used by a text object.
   *
   * @returns the TextEngine used by the text on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.SetEngine
   */
  TextEngineParam GetEngine() const;

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
   * @sa Text.GetFont
   */
  bool SetFont(FontParam font);

  /**
   * Get the font used by a text object.
   *
   * @returns the Font used by the text on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.SetFont
   */
  FontRef GetFont() const;

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
  void SetDirection(Direction direction);

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
  Direction GetDirection() const;

  /**
   * Set the script to be used for text shaping a text object.
   *
   * This returns false if SDL_ttf isn't built with HarfBuzz support.
   *
   * @param script an [ISO 15924
   *               code](https://unicode.org/iso15924/iso15924-codes.html).
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa StringToTag
   */
  void SetScript(Uint32 script);

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
  Uint32 GetScript() const;

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
   * @sa Text.GetColor
   * @sa Text.SetColorFloat
   */
  void SetColor(Color c);

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
   * @sa Text.GetColorFloat
   * @sa Text.SetColor
   */
  void SetColorFloat(FColor c);

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
   * @sa Text.GetColorFloat
   * @sa Text.SetColor
   */
  void GetColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a) const;

  /**
   * Get the color of a text object.
   *
   * @return The color in the range of 0-255 on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.GetColorFloat
   * @sa Text.SetColor
   */
  Color GetColor() const;

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
   * @sa Text.GetColor
   * @sa Text.SetColorFloat
   */
  void GetColorFloat(float* r, float* g, float* b, float* a) const;

  /**
   * Get the color of a text object.
   *
   * @return The color in the range of 0-1 on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.GetColor
   * @sa Text.SetColorFloat
   */
  FColor GetColorFloat() const;

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
   * @sa Text.GetPosition
   */
  bool SetPosition(Point p);

  /**
   * Get the position of a text object.
   *
   * @param x a pointer filled in with the x offset of the upper left corner of
   *          this text in pixels, may be nullptr.
   * @param y a pointer filled in with the y offset of the upper left corner of
   *          this text in pixels, may be nullptr.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa Text.SetPosition
   */
  bool GetPosition(int* x, int* y) const;

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
   * @sa Text.SetPosition
   */
  Point GetPosition() const;

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
   * @sa Text.GetWrapWidth
   */
  void SetWrapWidth(int wrap_width);

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
   * @sa Text.SetWrapWidth
   */
  int GetWrapWidth() const;

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
   * @sa Text.IsWrapWhitespaceVisible
   */
  void SetWrapWhitespaceVisible(bool visible);

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
   * @sa Text.SetWrapWhitespaceVisible
   */
  bool IsWrapWhitespaceVisible() const;

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
   * @sa Text.AppendString
   * @sa Text.DeleteString
   * @sa Text.InsertString
   */
  void SetString(std::string_view string);

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
   * @sa Text.AppendString
   * @sa Text.DeleteString
   * @sa Text.SetString
   */
  void InsertString(int offset, std::string_view string);

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
   * @sa Text.DeleteString
   * @sa Text.InsertString
   * @sa Text.SetString
   */
  void AppendString(std::string_view string);

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
   * @sa Text.AppendString
   * @sa Text.InsertString
   * @sa Text.SetString
   */
  void DeleteString(int offset, int length);

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
  void GetSize(int* w, int* h) const;

  /**
   * Get the size of a text object.
   *
   * The size of the text may change when the font or font style and size
   * change.
   *
   * @returns Point containing the size on x and y axis on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   *
   * @sa GetSize(int*,int*) if might want only one of the coordinates
   */
  Point GetSize() const;

  /**
   * Get the substring of a text object that surrounds a text offset.
   *
   * If `offset` is less than 0, this will return a zero length substring at the
   * beginning of the text with the SUBSTRING_TEXT_START flag set. If
   * `offset` is greater than or equal to the length of the text string, this
   * will return a zero length substring at the end of the text with the
   * SUBSTRING_TEXT_END flag set.
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
  void GetSubString(int offset, SubString* substring) const;

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
   * beginning of the text with the SUBSTRING_TEXT_START flag set. If `line`
   * is greater than or equal to `text->num_lines` this will return a zero
   * length substring at the end of the text with the SUBSTRING_TEXT_END
   * flag set.
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
  void GetSubStringForLine(int line, SubString* substring) const;

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
   * failure; call GetError() for more information. This is a single allocation
   * that should be freed with free() when it is no longer needed.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  OwnArray<SubString*> GetSubStringsForRange(int offset, int length = -1) const;

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
  void GetSubStringForPoint(Point p, SubString* substring) const;

  /**
   * Get the previous substring in a text object
   *
   * If called at the start of the text, this will return a zero length
   * substring with the SUBSTRING_TEXT_START flag set.
   *
   * @param substring the SubString to query.
   * @param previous a pointer filled in with the previous substring in the text
   *                 object.
   * @throws Error on failure.
   *
   * @threadsafety This function should be called on the thread that created the
   *               text.
   *
   * @since This function is available since SDL_ttf 3.0.0.
   */
  void GetPreviousSubString(const SubString& substring,
                            SubString* previous) const;

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
  void GetNextSubString(const SubString& substring, SubString* next) const;

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
  void Update();

  /**
   * A copy of the UTF-8 string that this text object represents, useful for
   * layout, debugging and retrieving substring text
   */
  const char* GetText() const { return m_resource->text; }

  /// The number of lines in the text, 0 if it's empty
  int GetNumLines() const { return m_resource->num_lines; }
};

/// Semi-safe reference for Text.
struct TextRef : Text
{
  /**
   * Constructs from TextParam.
   *
   * @param resource a TextRaw or Text.
   *
   * This does not takes ownership!
   */
  TextRef(TextParam resource = nullptr)
    : Text(resource.value)
  {
  }

  /// Copy constructor.
  TextRef(const TextRef& other)
    : Text(other.get())
  {
  }

  /// Destructor
  ~TextRef() { release(); }
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
    : m_text(text)
    , m_subString(0)
  {
  }

public:
  /// Default constructor.
  constexpr SubStringIterator()
    : SubStringIterator(TextRef{})
  {
  }

  /// True if pointing to valid SubString.
  constexpr operator bool() const { return bool(m_text); }

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

  friend class Text;
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
 * @sa SurfaceTextEngine.Destroy
 * @sa Text.DrawSurface
 */
inline SurfaceTextEngine CreateSurfaceTextEngine()
{
  return SurfaceTextEngine();
}

/**
 * Draw text to an SDL surface.
 *
 * `text` must have been created using a TextEngine from
 * SurfaceTextEngine.SurfaceTextEngine().
 *
 * @param text the text to draw.
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
 * @sa SurfaceTextEngine.SurfaceTextEngine
 * @sa Text.Text
 */
inline void DrawSurfaceText(TextConstParam text, Point p, SurfaceParam surface)
{
  CheckError(TTF_DrawSurfaceText(text, p.x, p.y, surface));
}

inline void Text::DrawSurface(Point p, SurfaceParam surface) const
{
  SDL::DrawSurfaceText(m_resource, p, surface);
}

/**
 * Destroy a text engine created for drawing text on SDL surfaces.
 *
 * All text created by this engine should be destroyed before calling this
 * function.
 *
 * @param engine a TextEngine object created with
 *               SurfaceTextEngine.SurfaceTextEngine().
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa SurfaceTextEngine.SurfaceTextEngine
 */
inline void DestroySurfaceTextEngine(TextEngineRaw engine)
{
  TTF_DestroySurfaceTextEngine(engine);
}

inline void SurfaceTextEngine::Destroy()
{
  DestroySurfaceTextEngine(release());
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
 * @sa RendererTextEngine.Destroy
 * @sa Text.DrawRenderer
 * @sa RendererTextEngine.RendererTextEngine
 */
inline RendererTextEngine CreateRendererTextEngine(RendererParam renderer)
{
  return RendererTextEngine(renderer);
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
 * @returns a TextEngine object or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               renderer.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa RendererTextEngine.RendererTextEngine
 * @sa RendererTextEngine.Destroy
 * @sa Text.DrawRenderer
 */
inline RendererTextEngine CreateRendererTextEngineWithProperties(
  PropertiesParam props)
{
  return RendererTextEngine(props);
}

namespace prop::RendererTextEngine {

#if SDL_TTF_VERSION_ATLEAST(3, 2, 3)

constexpr auto RENDERER_POINTER =
  TTF_PROP_RENDERER_TEXT_ENGINE_RENDERER_POINTER;

constexpr auto ATLAS_TEXTURE_SIZE_NUMBER =
  TTF_PROP_RENDERER_TEXT_ENGINE_ATLAS_TEXTURE_SIZE_NUMBER;

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 3)

} // namespace prop::RendererTextEngine

/**
 * Draw text to an SDL renderer.
 *
 * `text` must have been created using a TextEngine from
 * RendererTextEngine.RendererTextEngine(), and will draw using the renderer
 * passed to that function.
 *
 * @param text the text to draw.
 * @param p the (x, y) coordinate in pixels, positive from the left edge
 *          towards the right and from the top edge towards the bottom.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa RendererTextEngine.RendererTextEngine
 * @sa GPUTextEngine.GPUTextEngine
 */
inline void DrawRendererText(TextConstParam text, FPoint p)
{
  CheckError(TTF_DrawRendererText(text, p.x, p.y));
}

inline void Text::DrawRenderer(FPoint p) const
{
  SDL::DrawRendererText(m_resource, p);
}

/**
 * Destroy a text engine created for drawing text on an SDL renderer.
 *
 * All text created by this engine should be destroyed before calling this
 * function.
 *
 * @param engine a TextEngine object created with
 *               RendererTextEngine.RendererTextEngine().
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa RendererTextEngine.RendererTextEngine
 */
inline void DestroyRendererTextEngine(TextEngineRaw engine)
{
  TTF_DestroyRendererTextEngine(engine);
}

inline void RendererTextEngine::Destroy()
{
  DestroyRendererTextEngine(release());
}

/**
 * Create a text engine for drawing text with the SDL GPU API.
 *
 * @param device the GPUDevice to use for creating textures and drawing
 *               text.
 * @returns a TextEngine object or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               device.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa GPUTextEngine.GPUTextEngine
 * @sa GPUTextEngine.Destroy
 * @sa Text.GetGPUDrawData
 */
inline GPUTextEngine CreateGPUTextEngine(GPUDeviceParam device)
{
  return GPUTextEngine(device);
}

/**
 * Create a text engine for drawing text with the SDL GPU API, with the
 * specified properties.
 *
 * These are the supported properties:
 *
 * - `prop::GpuTextEngine.DEVICE_POINTER`: the GPUDevice to use for creating
 *   textures and drawing text.
 * - `prop::GpuTextEngine.ATLAS_TEXTURE_SIZE_NUMBER`: the size of the texture
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
 * @sa GPUTextEngine.GPUTextEngine
 * @sa GPUTextEngine.Destroy
 * @sa Text.GetGPUDrawData
 */
inline GPUTextEngine CreateGPUTextEngineWithProperties(PropertiesParam props)
{
  return GPUTextEngine(props);
}

namespace prop::GpuTextEngine {

#if SDL_TTF_VERSION_ATLEAST(3, 2, 3)

constexpr auto DEVICE_POINTER = TTF_PROP_GPU_TEXT_ENGINE_DEVICE_POINTER;

constexpr auto ATLAS_TEXTURE_SIZE_NUMBER =
  TTF_PROP_GPU_TEXT_ENGINE_ATLAS_TEXTURE_SIZE_NUMBER;

#endif // SDL_TTF_VERSION_ATLEAST(3, 2, 3)

} // namespace prop::GpuTextEngine

/**
 * Get the geometry data needed for drawing the text.
 *
 * `text` must have been created using a TextEngine from
 * GPUTextEngine.GPUTextEngine().
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
 * @returns a nullptr terminated linked list of GPUAtlasDrawSequence objects
 *          or nullptr if the passed text is empty or in case of failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa GPUTextEngine.GPUTextEngine
 * @sa GPUTextEngine.GPUTextEngine
 */
inline GPUAtlasDrawSequence* GetGPUTextDrawData(TextConstParam text)
{
  return TTF_GetGPUTextDrawData(text);
}

inline GPUAtlasDrawSequence* Text::GetGPUDrawData() const
{
  return SDL::GetGPUTextDrawData(m_resource);
}

/**
 * Destroy a text engine created for drawing text with the SDL GPU API.
 *
 * All text created by this engine should be destroyed before calling this
 * function.
 *
 * @param engine a TextEngine object created with
 *               GPUTextEngine.GPUTextEngine().
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa GPUTextEngine.GPUTextEngine
 */
inline void DestroyGPUTextEngine(TextEngineRaw engine)
{
  TTF_DestroyGPUTextEngine(engine);
}

inline void GPUTextEngine::Destroy() { DestroyGPUTextEngine(release()); }

/**
 * Sets the winding order of the vertices returned by Text.GetGPUDrawData
 * for a particular GPU text engine.
 *
 * @param engine a TextEngine object created with
 *               GPUTextEngine.GPUTextEngine().
 * @param winding the new winding order option.
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa GPUTextEngine.GetGPUWinding
 */
inline void SetGPUTextEngineWinding(TextEngineParam engine,
                                    GPUTextEngineWinding winding)
{
  TTF_SetGPUTextEngineWinding(engine, winding);
}

inline void GPUTextEngine::SetGPUWinding(GPUTextEngineWinding winding)
{
  SDL::SetGPUTextEngineWinding(get(), winding);
}

/**
 * Get the winding order of the vertices returned by Text.GetGPUDrawData
 * for a particular GPU text engine
 *
 * @param engine a TextEngine object created with
 *               GPUTextEngine.GPUTextEngine().
 * @returns the winding order used by the GPU text engine or
 *          GPU_TEXTENGINE_WINDING_INVALID in case of error.
 *
 * @threadsafety This function should be called on the thread that created the
 *               engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa GPUTextEngine.SetGPUWinding
 */
inline GPUTextEngineWinding GetGPUTextEngineWinding(TextEngineParam engine)
{
  return TTF_GetGPUTextEngineWinding(engine);
}

inline GPUTextEngineWinding GPUTextEngine::GetGPUWinding() const
{
  return SDL::GetGPUTextEngineWinding(get());
}

/**
 * Create a text object from UTF-8 text and a text engine.
 *
 * @param engine the text engine to use when creating the text object, may be
 *               nullptr.
 * @param font the font to render with.
 * @param text the text to use, in UTF-8 encoding.
 * @returns a Text object or nullptr on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               font and text engine.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.Destroy
 */
inline Text CreateText(TextEngineParam engine,
                       FontParam font,
                       std::string_view text)
{
  return Text(engine, font, text);
}

inline Text TextEngine::CreateText(FontParam font, std::string_view text)
{
  return Text(m_resource, font, text);
}

/**
 * Get the properties associated with a text object.
 *
 * @param text the Text to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline PropertiesRef GetTextProperties(TextConstParam text)
{
  return {CheckError(TTF_GetTextProperties(text))};
}

inline PropertiesRef Text::GetProperties() const
{
  return SDL::GetTextProperties(m_resource);
}

/**
 * Set the text engine used by a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the Text to modify.
 * @param engine the text engine to use for drawing.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetEngine
 */
inline void SetTextEngine(TextParam text, TextEngineParam engine)
{
  CheckError(TTF_SetTextEngine(text, engine));
}

inline void Text::SetEngine(TextEngineParam engine)
{
  SDL::SetTextEngine(m_resource, engine);
}

/**
 * Get the text engine used by a text object.
 *
 * @param text the Text to query.
 * @returns the TextEngine used by the text on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.SetEngine
 */
inline TextEngineParam GetTextEngine(TextConstParam text)
{
  return CheckError(TTF_GetTextEngine(text));
}

inline TextEngineParam Text::GetEngine() const
{
  return SDL::GetTextEngine(m_resource);
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
 * @param text the Text to modify.
 * @param font the font to use, may be nullptr.
 * @returns false if the text pointer is null; otherwise, true. call
 *          GetError() for more information.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetFont
 */
inline bool SetTextFont(TextParam text, FontParam font)
{
  return TTF_SetTextFont(text, font);
}

inline bool Text::SetFont(FontParam font)
{
  return SDL::SetTextFont(m_resource, font);
}

/**
 * Get the font used by a text object.
 *
 * @param text the Text to query.
 * @returns the Font used by the text on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.SetFont
 */
inline FontRef GetTextFont(TextConstParam text)
{
  return {CheckError(TTF_GetTextFont(text))};
}

inline FontRef Text::GetFont() const { return SDL::GetTextFont(m_resource); }

/**
 * Set the direction to be used for text shaping a text object.
 *
 * This function only supports left-to-right text shaping if SDL_ttf was not
 * built with HarfBuzz support.
 *
 * @param text the text to modify.
 * @param direction the new direction for text to flow.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void SetTextDirection(TextParam text, Direction direction)
{
  CheckError(TTF_SetTextDirection(text, direction));
}

inline void Text::SetDirection(Direction direction)
{
  SDL::SetTextDirection(m_resource, direction);
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
inline Direction GetTextDirection(TextConstParam text)
{
  return TTF_GetTextDirection(text);
}

inline Direction Text::GetDirection() const
{
  return SDL::GetTextDirection(m_resource);
}

/**
 * Set the script to be used for text shaping a text object.
 *
 * This returns false if SDL_ttf isn't built with HarfBuzz support.
 *
 * @param text the text to modify.
 * @param script an [ISO 15924
 *               code](https://unicode.org/iso15924/iso15924-codes.html).
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa StringToTag
 */
inline void SetTextScript(TextParam text, Uint32 script)
{
  CheckError(TTF_SetTextScript(text, script));
}

inline void Text::SetScript(Uint32 script)
{
  SDL::SetTextScript(m_resource, script);
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
 * @sa TagToString
 */
inline Uint32 GetTextScript(TextConstParam text)
{
  return TTF_GetTextScript(text);
}

inline Uint32 Text::GetScript() const { return SDL::GetTextScript(m_resource); }

/**
 * Set the color of a text object.
 *
 * The default text color is white (255, 255, 255, 255).
 *
 * @param text the Text to modify.
 * @param c the color value in the range of 0-255.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetColor
 * @sa Text.SetColorFloat
 */
inline void SetTextColor(TextParam text, Color c)
{
  CheckError(TTF_SetTextColor(text, c.r, c.g, c.b, c.a));
}

inline void Text::SetColor(Color c) { SDL::SetTextColor(m_resource, c); }

/**
 * Set the color of a text object.
 *
 * The default text color is white (1.0f, 1.0f, 1.0f, 1.0f).
 *
 * @param text the Text to modify.
 * @param c the color value, normally in the range of 0-1.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetColorFloat
 * @sa Text.SetColor
 */
inline void SetTextColorFloat(TextParam text, FColor c)
{
  CheckError(TTF_SetTextColorFloat(text, c.r, c.g, c.b, c.a));
}

inline void Text::SetColorFloat(FColor c)
{
  SDL::SetTextColorFloat(m_resource, c);
}

/**
 * Get the color of a text object.
 *
 * @param text the Text to query.
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
 * @sa Text.GetColorFloat
 * @sa Text.SetColor
 */
inline void GetTextColor(TextConstParam text,
                         Uint8* r,
                         Uint8* g,
                         Uint8* b,
                         Uint8* a)
{
  CheckError(TTF_GetTextColor(text, r, g, b, a));
}

/**
 * Get the color of a text object.
 *
 * @param text the Text to query.
 * @return The color in the range of 0-255 on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetColorFloat
 * @sa Text.SetColor
 */
inline Color GetTextColor(TextParam text)
{
  Color c;
  GetTextColor(text, &c.r, &c.g, &c.b, &c.a);
  return c;
}

inline void Text::GetColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a) const
{
  SDL::GetTextColor(m_resource, r, g, b, a);
}

inline Color Text::GetColor() const { return SDL::GetTextColor(m_resource); }

/**
 * Get the color of a text object.
 *
 * @param text the Text to query.
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
 * @sa Text.GetColor
 * @sa Text.SetColorFloat
 */
inline void GetTextColorFloat(TextConstParam text,
                              float* r,
                              float* g,
                              float* b,
                              float* a)
{
  CheckError(TTF_GetTextColorFloat(text, r, g, b, a));
}

/**
 * Get the color of a text object.
 *
 * @param text the Text to query.
 * @return The color in the range of 0-1 on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetColor
 * @sa Text.SetColorFloat
 */
inline FColor GetTextColorFloat(TextParam text)
{
  FColor c;
  GetTextColorFloat(text, &c.r, &c.g, &c.b, &c.a);
  return c;
}

inline void Text::GetColorFloat(float* r, float* g, float* b, float* a) const
{
  SDL::GetTextColorFloat(m_resource, r, g, b, a);
}

inline FColor Text::GetColorFloat() const
{
  return SDL::GetTextColorFloat(m_resource);
}

/**
 * Set the position of a text object.
 *
 * This can be used to position multiple text objects within a single wrapping
 * text area.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the Text to modify.
 * @param p the x, y offset of the upper left corner of this text in pixels.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetPosition
 */
inline bool SetTextPosition(TextParam text, Point p)
{
  return TTF_SetTextPosition(text, p.x, p.y);
}

inline bool Text::SetPosition(Point p)
{
  return SDL::SetTextPosition(m_resource, p);
}

/**
 * Get the position of a text object.
 *
 * @param text the Text to query.
 * @param x a pointer filled in with the x offset of the upper left corner of
 *          this text in pixels, may be nullptr.
 * @param y a pointer filled in with the y offset of the upper left corner of
 *          this text in pixels, may be nullptr.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.SetPosition
 */
inline bool GetTextPosition(TextConstParam text, int* x, int* y)
{
  return TTF_GetTextPosition(text, x, y);
}

/**
 * Get the position of a text object.
 *
 * @param text the Text to query.
 * @returns a Point with the offset of the upper left corner of this text in
 *          pixels on success.
 * @throws Error on failure.
 *
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.SetPosition
 */
inline Point GetTextPosition(TextParam text)
{
  Point p;
  CheckError(GetTextPosition(text, &p.x, &p.y));
  return p;
}

inline bool Text::GetPosition(int* x, int* y) const
{
  return SDL::GetTextPosition(m_resource, x, y);
}

inline Point Text::GetPosition() const
{
  return SDL::GetTextPosition(m_resource);
}

/**
 * Set whether wrapping is enabled on a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the Text to modify.
 * @param wrap_width the maximum width in pixels, 0 to wrap on newline
 *                   characters.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.GetWrapWidth
 */
inline void SetTextWrapWidth(TextParam text, int wrap_width)
{
  CheckError(TTF_SetTextWrapWidth(text, wrap_width));
}

inline void Text::SetWrapWidth(int wrap_width)
{
  SDL::SetTextWrapWidth(m_resource, wrap_width);
}

/**
 * Get whether wrapping is enabled on a text object.
 *
 * @param text the Text to query.
 * @returns maximum width in pixels or 0 if the text is being wrapped on newline
 *          characters.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.SetWrapWidth
 */
inline int GetTextWrapWidth(TextConstParam text)
{
  int w;
  CheckError(TTF_GetTextWrapWidth(text, &w));
  return w;
}

inline int Text::GetWrapWidth() const
{
  return SDL::GetTextWrapWidth(m_resource);
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
 * @param text the Text to modify.
 * @param visible true to show whitespace when wrapping text, false to hide
 *                it.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.IsWrapWhitespaceVisible
 */
inline void SetTextWrapWhitespaceVisible(TextParam text, bool visible)
{
  CheckError(TTF_SetTextWrapWhitespaceVisible(text, visible));
}

inline void Text::SetWrapWhitespaceVisible(bool visible)
{
  SDL::SetTextWrapWhitespaceVisible(m_resource, visible);
}

/**
 * Return whether whitespace is shown when wrapping a text object.
 *
 * @param text the Text to query.
 * @returns true if whitespace is shown when wrapping text, or false
 *          otherwise.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.SetWrapWhitespaceVisible
 */
inline bool TextWrapWhitespaceVisible(TextConstParam text)
{
  return TTF_TextWrapWhitespaceVisible(text);
}

inline bool Text::IsWrapWhitespaceVisible() const
{
  return SDL::TextWrapWhitespaceVisible(m_resource);
}

/**
 * Set the UTF-8 text used by a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the Text to modify.
 * @param string the UTF-8 text to use, may be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.AppendString
 * @sa Text.DeleteString
 * @sa Text.InsertString
 */
inline void SetTextString(TextParam text, std::string_view string)
{
  CheckError(TTF_SetTextString(text, string.data(), string.size()));
}

inline void Text::SetString(std::string_view string)
{
  SDL::SetTextString(m_resource, string);
}

/**
 * Insert UTF-8 text into a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the Text to modify.
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
 * @sa Text.AppendString
 * @sa Text.DeleteString
 * @sa Text.SetString
 */
inline void InsertTextString(TextParam text,
                             int offset,
                             std::string_view string)
{
  CheckError(TTF_InsertTextString(text, offset, string.data(), string.size()));
}

inline void Text::InsertString(int offset, std::string_view string)
{
  SDL::InsertTextString(m_resource, offset, string);
}

/**
 * Append UTF-8 text to a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the Text to modify.
 * @param string the UTF-8 text to insert.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Text.DeleteString
 * @sa Text.InsertString
 * @sa Text.SetString
 */
inline void AppendTextString(TextParam text, std::string_view string)
{
  CheckError(TTF_AppendTextString(text, string.data(), string.size()));
}

inline void Text::AppendString(std::string_view string)
{
  SDL::AppendTextString(m_resource, string);
}

/**
 * Delete UTF-8 text from a text object.
 *
 * This function may cause the internal text representation to be rebuilt.
 *
 * @param text the Text to modify.
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
 * @sa Text.AppendString
 * @sa Text.InsertString
 * @sa Text.SetString
 */
inline void DeleteTextString(TextParam text, int offset, int length)
{
  CheckError(TTF_DeleteTextString(text, offset, length));
}

inline void Text::DeleteString(int offset, int length)
{
  SDL::DeleteTextString(m_resource, offset, length);
}

/**
 * Get the size of a text object.
 *
 * The size of the text may change when the font or font style and size
 * change.
 *
 * @param text the Text to query.
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
inline void GetTextSize(TextConstParam text, int* w, int* h)
{
  CheckError(TTF_GetTextSize(text, w, h));
}

/**
 * Get the size of a text object.
 *
 * The size of the text may change when the font or font style and size
 * change.
 *
 * @param text the Text to query.
 * @returns Point containing the size on x and y axis on success or
 *          std::nullopt on failure; call GetError() for more information.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline Point GetTextSize(TextParam text)
{
  Point p;
  GetTextSize(text, &p.x, &p.y);
  return p;
}

inline void Text::GetSize(int* w, int* h) const
{
  SDL::GetTextSize(m_resource, w, h);
}

inline Point Text::GetSize() const { return SDL::GetTextSize(m_resource); }

/**
 * Get the substring of a text object that surrounds a text offset.
 *
 * If `offset` is less than 0, this will return a zero length substring at the
 * beginning of the text with the SUBSTRING_TEXT_START flag set. If
 * `offset` is greater than or equal to the length of the text string, this
 * will return a zero length substring at the end of the text with the
 * SUBSTRING_TEXT_END flag set.
 *
 * @param text the Text to query.
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
inline void GetTextSubString(TextConstParam text,
                             int offset,
                             SubString* substring)
{
  CheckError(TTF_GetTextSubString(text, offset, substring));
}

inline void Text::GetSubString(int offset, SubString* substring) const
{
  SDL::GetTextSubString(m_resource, offset, substring);
}

/**
 * Get the substring of a text object that contains the given line.
 *
 * If `line` is less than 0, this will return a zero length substring at the
 * beginning of the text with the SUBSTRING_TEXT_START flag set. If `line`
 * is greater than or equal to `text->num_lines` this will return a zero
 * length substring at the end of the text with the SUBSTRING_TEXT_END
 * flag set.
 *
 * @param text the Text to query.
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
inline void GetTextSubStringForLine(TextConstParam text,
                                    int line,
                                    SubString* substring)
{
  CheckError(TTF_GetTextSubStringForLine(text, line, substring));
}

inline void Text::GetSubStringForLine(int line, SubString* substring) const
{
  SDL::GetTextSubStringForLine(m_resource, line, substring);
}

/**
 * Get the substrings of a text object that contain a range of text.
 *
 * @param text the Text to query.
 * @param offset a byte offset into the text string.
 * @param length the length of the range being queried, in bytes, or -1 for
 *               the remainder of the string.
 * @returns a nullptr terminated array of substring pointers or nullptr on
 * failure; call GetError() for more information. This is a single allocation
 * that should be freed with free() when it is no longer needed.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline OwnArray<SubString*> GetTextSubStringsForRange(TextConstParam text,
                                                      int offset,
                                                      int length)
{
  int count = 0;
  auto data = TTF_GetTextSubStringsForRange(text, offset, length, &count);
  return OwnArray<SubString*>{data, size_t(count)};
}

inline OwnArray<SubString*> Text::GetSubStringsForRange(int offset,
                                                        int length) const
{
  return SDL::GetTextSubStringsForRange(m_resource, offset, length);
}

/**
 * Get the portion of a text string that is closest to a point.
 *
 * This will return the closest substring of text to the given point.
 *
 * @param text the Text to query.
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
inline void GetTextSubStringForPoint(TextConstParam text,
                                     Point p,
                                     SubString* substring)
{
  CheckError(TTF_GetTextSubStringForPoint(text, p.x, p.y, substring));
}

inline void Text::GetSubStringForPoint(Point p, SubString* substring) const
{
  SDL::GetTextSubStringForPoint(m_resource, p, substring);
}

/**
 * Get the previous substring in a text object
 *
 * If called at the start of the text, this will return a zero length
 * substring with the SUBSTRING_TEXT_START flag set.
 *
 * @param text the Text to query.
 * @param substring the SubString to query.
 * @param previous a pointer filled in with the previous substring in the text
 *                 object.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void GetPreviousTextSubString(TextConstParam text,
                                     const SubString& substring,
                                     SubString* previous)
{
  CheckError(TTF_GetPreviousTextSubString(text, &substring, previous));
}

inline void Text::GetPreviousSubString(const SubString& substring,
                                       SubString* previous) const
{
  SDL::GetPreviousTextSubString(m_resource, substring, previous);
}

/**
 * Get the next substring in a text object
 *
 * If called at the end of the text, this will return a zero length substring
 * with the SUBSTRING_TEXT_END flag set.
 *
 * @param text the Text to query.
 * @param substring the SubString to query.
 * @param next a pointer filled in with the next substring.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void GetNextTextSubString(TextConstParam text,
                                 const SubString& substring,
                                 SubString* next)
{
  CheckError(TTF_GetNextTextSubString(text, &substring, next));
}

inline void Text::GetNextSubString(const SubString& substring,
                                   SubString* next) const
{
  SDL::GetNextTextSubString(m_resource, substring, next);
}

/**
 * Update the layout of a text object.
 *
 * This is automatically done when the layout is requested or the text is
 * rendered, but you can call this if you need more control over the timing of
 * when the layout and text engine representation are updated.
 *
 * @param text the Text to update.
 * @throws Error on failure.
 *
 * @threadsafety This function should be called on the thread that created the
 *               text.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 */
inline void UpdateText(TextParam text) { CheckError(TTF_UpdateText(text)); }

inline void Text::Update() { SDL::UpdateText(m_resource); }

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
 * @sa RendererTextEngine.RendererTextEngine
 */
inline void DestroyText(TextRaw text) { TTF_DestroyText(text); }

inline void Text::Destroy() { DestroyText(release()); }

/**
 * Dispose of a previously-created font.
 *
 * Call this when done with a font. This function will free any resources
 * associated with it. It is safe to call this function on nullptr, for example
 * on the result of a failed call to Font.Font().
 *
 * The font is not valid after being passed to this function. String pointers
 * from functions that return information on this font, such as
 * Font.GetFamilyName() and Font.GetStyleName(), are no longer valid
 * after this call, as well.
 *
 * @param font the font to dispose of.
 *
 * @threadsafety This function should not be called while any other thread is
 *               using the font.
 *
 * @since This function is available since SDL_ttf 3.0.0.
 *
 * @sa Font.Font
 * @sa Font.Font
 */
inline void CloseFont(FontRaw font) { TTF_CloseFont(font); }

inline void Font::Close() { CloseFont(release()); }

/// @}

} // namespace SDL

#endif // defined(SDL3PP_ENABLE_TTF) || defined(SDL3PP_DOC)

#endif /* SDL3PP_TTF_H_ */
