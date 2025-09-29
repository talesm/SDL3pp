#ifndef SDL3PP_IMAGE_H_
#define SDL3PP_IMAGE_H_

#include <SDL3/SDL_image.h>
#include "SDL3pp_error.h"
#include "SDL3pp_render.h"
#include "SDL3pp_surface.h"
#include "SDL3pp_version.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategorySDLImage Category SDLImage
 *
 * Header file for SDL_image library
 *
 * A simple library to load images of various formats as SDL surfaces
 *
 * @{
 */

// Forward decl
struct Animation;

using AnimationRaw = IMG_Animation*;

// Forward decl
struct AnimationRef;

/// Safely wrap Animation for non owning parameters
struct AnimationParam
{
  AnimationRaw value; ///< parameter's AnimationRaw

  /// Constructs from AnimationRaw
  constexpr AnimationParam(AnimationRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr AnimationParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const AnimationParam& other) const = default;

  /// Converts to underlying AnimationRaw
  constexpr operator AnimationRaw() const { return value; }
};

/// Printable format: "%d.%d.%d", MAJOR, MINOR, MICRO
#define SDL_IMAGE_MAJOR_VERSION 3

#define SDL_IMAGE_MINOR_VERSION 2

#define SDL_IMAGE_MICRO_VERSION 5

/// This is the version number macro for the current SDL_image version.
#define SDL_IMAGE_VERSION                                                      \
  SDL_VERSIONNUM(                                                              \
    SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION)

/**
 * This macro will evaluate to true if compiled with SDL_image at least X.Y.Z.
 */
#define SDL_IMAGE_VERSION_ATLEAST(X, Y, Z)                                     \
  ((SDL_IMAGE_MAJOR_VERSION >= X) &&                                           \
   (SDL_IMAGE_MAJOR_VERSION > X || SDL_IMAGE_MINOR_VERSION >= Y) &&            \
   (SDL_IMAGE_MAJOR_VERSION > X || SDL_IMAGE_MINOR_VERSION > Y ||              \
    SDL_IMAGE_MICRO_VERSION >= Z))

namespace IMG {

/**
 * This function gets the version of the dynamically linked SDL_image library.
 *
 * @returns SDL_image version.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline int Version();

} // namespace IMG

inline int IMG::Version() { return SDL::Version(); }

/**
 * Load an image from an SDL data source into a software surface.
 *
 * An Surface is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new Surface data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an Surface for
 * its specifics, and use Surface.Convert to then migrate to any supported
 * format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: Surface.SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * Even though this function accepts a file type, SDL_image may still try
 * other decoders that are capable of detecting file type from the contents of
 * the image data, but may rely on the caller-provided type string for formats
 * that it cannot autodetect. If `type` is nullptr, SDL_image will rely solely
 * on its ability to guess the format.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `Surface.Surface("filename.jpg")` will call this function and
 * manage those details for you, determining the file type from the filename's
 * extension.
 *
 * There is also Surface.Surface(), which is equivalent to this function except
 * that it will rely on SDL_image to determine what type of data it is
 * loading, much like passing a nullptr for type.
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an Texture for use by the GPU without using a
 * software surface: call LoadTextureTyped() instead.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to Surface.Destroy().
 *
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Surface.Surface
 * @sa Surface.Surface
 * @sa Surface.Destroy
 */
inline Surface LoadSurfaceTyped(IOStreamParam src,
                                bool closeio,
                                StringParam type)
{
  return IMG_LoadTyped_IO(src, closeio, type);
}

/**
 * Load an image from a filesystem path into a software surface.
 *
 * An Surface is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new Surface data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an Surface for
 * its specifics, and use Surface.Convert to then migrate to any supported
 * format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: Surface.SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * There is a separate function to read files from an IOStream, if you
 * need an i/o abstraction to provide data from anywhere instead of a simple
 * filesystem read; that function is Surface.Surface().
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an Texture for use by the GPU without using a
 * software surface: call Texture.Texture() instead.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to
 * [Surface.Destroy](https://wiki.libsdl.org/SDL3/Surface.Destroy)
 * ().
 *
 * @param file a path on the filesystem to load an image from.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadSurfaceTyped
 * @sa Surface.Surface
 * @sa Surface.Destroy
 */
inline Surface LoadSurface(StringParam file) { return IMG_Load(file); }

/**
 * Load an image from an SDL data source into a software surface.
 *
 * An Surface is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new Surface data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an Surface for
 * its specifics, and use Surface.Convert to then migrate to any supported
 * format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: Surface.SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `Surface.Surface("filename.jpg")` will call this function and
 * manage those details for you, determining the file type from the filename's
 * extension.
 *
 * There is also LoadSurfaceTyped(), which is equivalent to this function
 * except a file extension (like "BMP", "JPG", etc) can be specified, in case
 * SDL_image cannot autodetect the file format.
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an Texture for use by the GPU without using a
 * software surface: call Texture.Texture() instead.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to Surface.Destroy().
 *
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Surface.Surface
 * @sa LoadSurfaceTyped
 * @sa Surface.Destroy
 */
inline Surface LoadSurface(IOStreamParam src, bool closeio)
{
  return IMG_Load_IO(src, closeio);
}

/**
 * Load an image from a filesystem path into a software surface.
 *
 * An Surface is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new Surface data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an Surface for
 * its specifics, and use Surface.Convert to then migrate to any supported
 * format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: Surface.SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * There is a separate function to read files from an IOStream, if you
 * need an i/o abstraction to provide data from anywhere instead of a simple
 * filesystem read; that function is Surface.Surface().
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an Texture for use by the GPU without using a
 * software surface: call Texture.Texture() instead.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to
 * [Surface.Destroy](https://wiki.libsdl.org/SDL3/Surface.Destroy)
 * ().
 *
 * @param file a path on the filesystem to load an image from.
 * @post a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadSurfaceTyped
 * @sa Surface.Surface
 * @sa Surface.Destroy
 */
inline Surface::Surface(StringParam file)
  : m_resource(IMG_Load(file))
{
}

/**
 * Load an image from an SDL data source into a software surface.
 *
 * An Surface is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new Surface data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an Surface for
 * its specifics, and use Surface.Convert to then migrate to any supported
 * format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: Surface.SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `Surface.Surface("filename.jpg")` will call this function and
 * manage those details for you, determining the file type from the filename's
 * extension.
 *
 * There is also LoadSurfaceTyped(), which is equivalent to this function
 * except a file extension (like "BMP", "JPG", etc) can be specified, in case
 * SDL_image cannot autodetect the file format.
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an Texture for use by the GPU without using a
 * software surface: call Texture.Texture() instead.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to Surface.Destroy().
 *
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @post a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Surface.Surface
 * @sa LoadSurfaceTyped
 * @sa Surface.Destroy
 */
inline Surface::Surface(IOStreamParam src, bool closeio)
  : m_resource(IMG_Load_IO(src, closeio))
{
}

/**
 * Load an image from a filesystem path into a GPU texture.
 *
 * An Texture represents an image in GPU memory, usable by SDL's 2D Render
 * API. This can be significantly more efficient than using a CPU-bound
 * Surface if you don't need to manipulate the image directly after
 * loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * Texture in the most format that most reasonably represents the image
 * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
 * RGBA).
 *
 * There is a separate function to read files from an IOStream, if you
 * need an i/o abstraction to provide data from anywhere instead of a simple
 * filesystem read; that function is Texture.Texture().
 *
 * If you would rather decode an image to an Surface (a buffer of pixels
 * in CPU memory), call Surface.Surface() instead.
 *
 * When done with the returned texture, the app should dispose of it with a
 * call to Texture.Destroy().
 *
 * @param renderer the Renderer to use to create the GPU texture.
 * @param file a path on the filesystem to load an image from.
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadTextureTyped
 * @sa Texture.Texture
 */
inline Texture LoadTexture(RendererParam renderer, StringParam file)
{
  return IMG_LoadTexture(renderer, file);
}

/**
 * Load an image from an SDL data source into a GPU texture.
 *
 * An Texture represents an image in GPU memory, usable by SDL's 2D Render
 * API. This can be significantly more efficient than using a CPU-bound
 * Surface if you don't need to manipulate the image directly after
 * loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * Texture in the most format that most reasonably represents the image
 * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
 * RGBA).
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `Texture.Texture(renderer, "filename.jpg")` will call
 * this function and manage those details for you, determining the file type
 * from the filename's extension.
 *
 * There is also LoadTextureTyped(), which is equivalent to this
 * function except a file extension (like "BMP", "JPG", etc) can be specified,
 * in case SDL_image cannot autodetect the file format.
 *
 * If you would rather decode an image to an Surface (a buffer of pixels
 * in CPU memory), call Surface.Surface() instead.
 *
 * When done with the returned texture, the app should dispose of it with a
 * call to Texture.Destroy().
 *
 * @param renderer the Renderer to use to create the GPU texture.
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Texture.Texture
 * @sa LoadTextureTyped
 * @sa Texture.Destroy
 */
inline Texture LoadTexture(RendererParam renderer,
                           IOStreamParam src,
                           bool closeio)
{
  return IMG_LoadTexture_IO(renderer, src, closeio);
}

/**
 * Load an image from a filesystem path into a GPU texture.
 *
 * An Texture represents an image in GPU memory, usable by SDL's 2D Render
 * API. This can be significantly more efficient than using a CPU-bound
 * Surface if you don't need to manipulate the image directly after
 * loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * Texture in the most format that most reasonably represents the image
 * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
 * RGBA).
 *
 * There is a separate function to read files from an IOStream, if you
 * need an i/o abstraction to provide data from anywhere instead of a simple
 * filesystem read; that function is Texture.Texture().
 *
 * If you would rather decode an image to an Surface (a buffer of pixels
 * in CPU memory), call Surface.Surface() instead.
 *
 * When done with the returned texture, the app should dispose of it with a
 * call to Texture.Destroy().
 *
 * @param renderer the Renderer to use to create the GPU texture.
 * @param file a path on the filesystem to load an image from.
 * @post a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadTextureTyped
 * @sa Texture.Texture
 */
inline Texture::Texture(RendererParam renderer, StringParam file)
  : m_resource(IMG_LoadTexture(renderer, file))
{
}

/**
 * Load an image from an SDL data source into a GPU texture.
 *
 * An Texture represents an image in GPU memory, usable by SDL's 2D Render
 * API. This can be significantly more efficient than using a CPU-bound
 * Surface if you don't need to manipulate the image directly after
 * loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * Texture in the most format that most reasonably represents the image
 * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
 * RGBA).
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `Texture.Texture(renderer, "filename.jpg")` will call
 * this function and manage those details for you, determining the file type
 * from the filename's extension.
 *
 * There is also LoadTextureTyped(), which is equivalent to this
 * function except a file extension (like "BMP", "JPG", etc) can be specified,
 * in case SDL_image cannot autodetect the file format.
 *
 * If you would rather decode an image to an Surface (a buffer of pixels
 * in CPU memory), call Surface.Surface() instead.
 *
 * When done with the returned texture, the app should dispose of it with a
 * call to Texture.Destroy().
 *
 * @param renderer the Renderer to use to create the GPU texture.
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @post a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Texture.Texture
 * @sa LoadTextureTyped
 * @sa Texture.Destroy
 */
inline Texture::Texture(RendererParam renderer, IOStreamParam src, bool closeio)
  : m_resource(IMG_LoadTexture_IO(renderer, src, closeio))
{
}

/**
 * Load an image from an SDL data source into a GPU texture.
 *
 * An Texture represents an image in GPU memory, usable by SDL's 2D Render
 * API. This can be significantly more efficient than using a CPU-bound
 * Surface if you don't need to manipulate the image directly after
 * loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * Texture in the most format that most reasonably represents the image
 * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
 * RGBA).
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * Even though this function accepts a file type, SDL_image may still try
 * other decoders that are capable of detecting file type from the contents of
 * the image data, but may rely on the caller-provided type string for formats
 * that it cannot autodetect. If `type` is nullptr, SDL_image will rely solely
 * on its ability to guess the format.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `Texture.Texture("filename.jpg")` will call this
 * function and manage those details for you, determining the file type from
 * the filename's extension.
 *
 * There is also Texture.Texture(), which is equivalent to this function
 * except that it will rely on SDL_image to determine what type of data it is
 * loading, much like passing a nullptr for type.
 *
 * If you would rather decode an image to an Surface (a buffer of pixels
 * in CPU memory), call LoadSurfaceTyped() instead.
 *
 * When done with the returned texture, the app should dispose of it with a
 * call to Texture.Destroy().
 *
 * @param renderer the Renderer to use to create the GPU texture.
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Texture.Texture
 * @sa Texture.Texture
 * @sa Texture.Destroy
 */
inline Texture LoadTextureTyped(RendererParam renderer,
                                IOStreamParam src,
                                bool closeio,
                                StringParam type)
{
  return IMG_LoadTextureTyped_IO(renderer, src, closeio, type);
}

/**
 * Detect AVIF image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is AVIF data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isAVIF(IOStreamParam src) { return IMG_isAVIF(src); }

/**
 * Detect ICO image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is ICO data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isICO(IOStreamParam src) { return IMG_isICO(src); }

/**
 * Detect CUR image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is CUR data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isCUR(IOStreamParam src) { return IMG_isCUR(src); }

/**
 * Detect BMP image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is BMP data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isBMP(IOStreamParam src) { return IMG_isBMP(src); }

/**
 * Detect GIF image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is GIF data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isGIF(IOStreamParam src) { return IMG_isGIF(src); }

/**
 * Detect JPG image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is JPG data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isJPG(IOStreamParam src) { return IMG_isJPG(src); }

/**
 * Detect JXL image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is JXL data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isJXL(IOStreamParam src) { return IMG_isJXL(src); }

/**
 * Detect LBM image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is LBM data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isLBM(IOStreamParam src) { return IMG_isLBM(src); }

/**
 * Detect PCX image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is PCX data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isPCX(IOStreamParam src) { return IMG_isPCX(src); }

/**
 * Detect PNG image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is PNG data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isPNG(IOStreamParam src) { return IMG_isPNG(src); }

/**
 * Detect PNM image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is PNM data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isPNM(IOStreamParam src) { return IMG_isPNM(src); }

/**
 * Detect SVG image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is SVG data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isSVG(IOStreamParam src) { return IMG_isSVG(src); }

/**
 * Detect QOI image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is QOI data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isQOI(IOStreamParam src) { return IMG_isQOI(src); }

/**
 * Detect TIFF image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is TIFF data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isTIF(IOStreamParam src) { return IMG_isTIF(src); }

/**
 * Detect XCF image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is XCF data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXPM
 * @sa isXV
 * @sa isWEBP
 */
inline bool isXCF(IOStreamParam src) { return IMG_isXCF(src); }

/**
 * Detect XPM image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is XPM data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXV
 * @sa isWEBP
 */
inline bool isXPM(IOStreamParam src) { return IMG_isXPM(src); }

/**
 * Detect XV image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is XV data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isWEBP
 */
inline bool isXV(IOStreamParam src) { return IMG_isXV(src); }

/**
 * Detect WEBP image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and
 * basic i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started
 * when this function was called, but it will not report any errors in doing
 * so, but assuming seeking works, this means you can immediately use this
 * with a different IMG_isTYPE function, or load the image without further
 * seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns non-zero if this is WEBP data, zero otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isICO
 * @sa isCUR
 * @sa isBMP
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isQOI
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isWEBP(IOStreamParam src) { return IMG_isWEBP(src); }

/**
 * Load a AVIF image directly.
 *
 * If you know you definitely have a AVIF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadAVIF(IOStreamParam src) { return IMG_LoadAVIF_IO(src); }

/**
 * Load a ICO image directly.
 *
 * If you know you definitely have a ICO image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadICO(IOStreamParam src) { return IMG_LoadICO_IO(src); }

/**
 * Load a CUR image directly.
 *
 * If you know you definitely have a CUR image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadCUR(IOStreamParam src) { return IMG_LoadCUR_IO(src); }

/**
 * Load a BMP image directly.
 *
 * If you know you definitely have a BMP image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadBMP(IOStreamParam src) { return IMG_LoadBMP_IO(src); }

/**
 * Load a GIF image directly.
 *
 * If you know you definitely have a GIF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadGIF(IOStreamParam src) { return IMG_LoadGIF_IO(src); }

/**
 * Load a JPG image directly.
 *
 * If you know you definitely have a JPG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadJPG(IOStreamParam src) { return IMG_LoadJPG_IO(src); }

/**
 * Load a JXL image directly.
 *
 * If you know you definitely have a JXL image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadJXL(IOStreamParam src) { return IMG_LoadJXL_IO(src); }

/**
 * Load a LBM image directly.
 *
 * If you know you definitely have a LBM image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadLBM(IOStreamParam src) { return IMG_LoadLBM_IO(src); }

/**
 * Load a PCX image directly.
 *
 * If you know you definitely have a PCX image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadPCX(IOStreamParam src) { return IMG_LoadPCX_IO(src); }

/**
 * Load a PNG image directly.
 *
 * If you know you definitely have a PNG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadPNG(IOStreamParam src) { return IMG_LoadPNG_IO(src); }

/**
 * Load a PNM image directly.
 *
 * If you know you definitely have a PNM image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadPNM(IOStreamParam src) { return IMG_LoadPNM_IO(src); }

/**
 * Load a SVG image directly.
 *
 * If you know you definitely have a SVG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadSVG(IOStreamParam src) { return IMG_LoadSVG_IO(src); }

/**
 * Load a QOI image directly.
 *
 * If you know you definitely have a QOI image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadQOI(IOStreamParam src) { return IMG_LoadQOI_IO(src); }

/**
 * Load a TGA image directly.
 *
 * If you know you definitely have a TGA image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadTGA(IOStreamParam src) { return IMG_LoadTGA_IO(src); }

/**
 * Load a TIFF image directly.
 *
 * If you know you definitely have a TIFF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadTIF(IOStreamParam src) { return IMG_LoadTIF_IO(src); }

/**
 * Load a XCF image directly.
 *
 * If you know you definitely have a XCF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXPM
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadXCF(IOStreamParam src) { return IMG_LoadXCF_IO(src); }

/**
 * Load a XPM image directly.
 *
 * If you know you definitely have a XPM image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXV
 * @sa LoadWEBP
 */
inline Surface LoadXPM(IOStreamParam src) { return IMG_LoadXPM_IO(src); }

/**
 * Load a XV image directly.
 *
 * If you know you definitely have a XV image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadWEBP
 */
inline Surface LoadXV(IOStreamParam src) { return IMG_LoadXV_IO(src); }

/**
 * Load a WEBP image directly.
 *
 * If you know you definitely have a WEBP image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadICO
 * @sa LoadCUR
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadQOI
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 */
inline Surface LoadWEBP(IOStreamParam src) { return IMG_LoadWEBP_IO(src); }

/**
 * Load an SVG image, scaled to a specific size.
 *
 * Since SVG files are resolution-independent, you specify the size you would
 * like the output image to be and it will be generated at those dimensions.
 *
 * Either width or height may be 0 and the image will be auto-sized to
 * preserve aspect ratio.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to Surface.Destroy().
 *
 * @param src an IOStream to load SVG data from.
 * @param width desired width of the generated surface, in pixels.
 * @param height desired height of the generated surface, in pixels.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Surface LoadSizedSVG(IOStreamParam src, const PointRaw& size)
{
  return IMG_LoadSizedSVG_IO(src, size);
}

/**
 * Load an XPM image from a memory array.
 *
 * The returned surface will be an 8bpp indexed surface, if possible,
 * otherwise it will be 32bpp. If you always want 32-bit data, use
 * ReadXPMFromArrayToRGB888() instead.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to Surface.Destroy().
 *
 * @param xpm a null-terminated array of strings that comprise XPM data.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa ReadXPMFromArrayToRGB888
 */
inline Surface ReadXPMFromArray(char** xpm)
{
  return IMG_ReadXPMFromArray(xpm);
}

/**
 * Load an XPM image from a memory array.
 *
 * The returned surface will always be a 32-bit RGB surface. If you want 8-bit
 * indexed colors (and the XPM data allows it), use ReadXPMFromArray()
 * instead.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to Surface.Destroy().
 *
 * @param xpm a null-terminated array of strings that comprise XPM data.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa ReadXPMFromArray
 */
inline Surface ReadXPMFromArrayToRGB888(char** xpm)
{
  return IMG_ReadXPMFromArrayToRGB888(xpm);
}

/**
 * Save an Surface into a AVIF image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @param quality the desired quality, ranging between 0 (lowest) and 100
 *                (highest).
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SaveAVIF
 */
inline void SaveAVIF(SurfaceParam surface, StringParam file, int quality)
{
  CheckError(IMG_SaveAVIF(surface, file, quality));
}

/**
 * Save an Surface into AVIF image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use SaveAVIF() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @param quality the desired quality, ranging between 0 (lowest) and 100
 *                (highest).
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SaveAVIF
 */
inline void SaveAVIF(SurfaceParam surface,
                     IOStreamParam dst,
                     bool closeio,
                     int quality)
{
  CheckError(IMG_SaveAVIF_IO(surface, dst, closeio, quality));
}

/**
 * Save an Surface into a PNG image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SavePNG
 */
inline void SavePNG(SurfaceParam surface, StringParam file)
{
  CheckError(IMG_SavePNG(surface, file));
}

/**
 * Save an Surface into PNG image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use SavePNG() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SavePNG
 */
inline void SavePNG(SurfaceParam surface, IOStreamParam dst, bool closeio)
{
  CheckError(IMG_SavePNG_IO(surface, dst, closeio));
}

/**
 * Save an Surface into a JPEG image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @param quality [0; 33] is Lowest quality, [34; 66] is Middle quality, [67;
 *                100] is Highest quality.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SaveJPG
 */
inline void SaveJPG(SurfaceParam surface, StringParam file, int quality)
{
  CheckError(IMG_SaveJPG(surface, file, quality));
}

/**
 * Save an Surface into JPEG image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use SaveJPG() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @param quality [0; 33] is Lowest quality, [34; 66] is Middle quality, [67;
 *                100] is Highest quality.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SaveJPG
 */
inline void SaveJPG(SurfaceParam surface,
                    IOStreamParam dst,
                    bool closeio,
                    int quality)
{
  CheckError(IMG_SaveJPG_IO(surface, dst, closeio, quality));
}

/**
 * Animated image support
 *
 * Currently only animated GIFs and WEBP images are supported.
 *
 * @cat resource
 */
class Animation
{
  AnimationRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Animation() = default;

  /**
   * Constructs from AnimationParam.
   *
   * @param resource a AnimationRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Animation(const AnimationRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Animation(const Animation& other) = delete;

  /// Move constructor
  constexpr Animation(Animation&& other)
    : Animation(other.release())
  {
  }

  constexpr Animation(const AnimationRef& other) = delete;

  constexpr Animation(AnimationRef&& other) = delete;

  /**
   * Load an animation from a file.
   *
   * When done with the returned animation, the app should dispose of it with a
   * call to Animation.Free().
   *
   * @param file path on the filesystem containing an animated image.
   * @post a new Animation, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa Animation.Free
   */
  Animation(StringParam file)
    : m_resource(IMG_LoadAnimation(file))
  {
  }

  /**
   * Load an animation from an IOStream.
   *
   * If `closeio` is true, `src` will be closed before returning, whether this
   * function succeeds or not. SDL_image reads everything it needs from `src`
   * during this call in any case.
   *
   * When done with the returned animation, the app should dispose of it with a
   * call to Animation.Free().
   *
   * @param src an IOStream that data will be read from.
   * @param closeio true to close/free the IOStream before returning, false
   *                to leave it open.
   * @post a new Animation, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa Animation.Free
   */
  Animation(IOStreamParam src, bool closeio)
    : m_resource(IMG_LoadAnimation_IO(src, closeio))
  {
  }

  /// Destructor
  ~Animation() { IMG_FreeAnimation(m_resource); }

  /// Assignment operator.
  Animation& operator=(Animation other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying AnimationRaw.
  constexpr AnimationRaw get() const { return m_resource; }

  /// Retrieves underlying AnimationRaw and clear this.
  constexpr AnimationRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Animation& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to AnimationParam
  constexpr operator AnimationParam() const { return {m_resource}; }

  /**
   * Dispose of an Animation and free its resources.
   *
   * The provided `anim` pointer is not valid once this call returns.
   *
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa Animation.Animation
   * @sa Animation.Animation
   * @sa LoadAnimationTyped
   */
  void Free();

  int GetWidth() const { static_assert(false, "Not implemented"); }

  int GetHeight() const { static_assert(false, "Not implemented"); }

  Point GetSize() const { static_assert(false, "Not implemented"); }

  int GetCount() const { static_assert(false, "Not implemented"); }

  Surface GetFrame(int index) const { static_assert(false, "Not implemented"); }

  int GetDelay(int index) const { static_assert(false, "Not implemented"); }
};

/// Semi-safe reference for Animation.
struct AnimationRef : Animation
{
  /**
   * Constructs from AnimationParam.
   *
   * @param resource a AnimationRaw or Animation.
   *
   * This does not takes ownership!
   */
  AnimationRef(AnimationParam resource)
    : Animation(resource.value)
  {
  }

  /// Destructor
  ~AnimationRef() { release(); }
};

/**
 * Load an animation from a file.
 *
 * When done with the returned animation, the app should dispose of it with a
 * call to Animation.Free().
 *
 * @param file path on the filesystem containing an animated image.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Animation.Free
 */
inline Animation LoadAnimation(StringParam file)
{
  return Animation(std::move(file));
}

/**
 * Load an animation from an IOStream.
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * When done with the returned animation, the app should dispose of it with a
 * call to Animation.Free().
 *
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Animation.Free
 */
inline Animation LoadAnimation(IOStreamParam src, bool closeio)
{
  return Animation(src, closeio);
}

/**
 * Load an animation from an SDL datasource
 *
 * Even though this function accepts a file type, SDL_image may still try
 * other decoders that are capable of detecting file type from the contents of
 * the image data, but may rely on the caller-provided type string for formats
 * that it cannot autodetect. If `type` is nullptr, SDL_image will rely solely
 * on its ability to guess the format.
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * When done with the returned animation, the app should dispose of it with a
 * call to Animation.Free().
 *
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false
 *                to leave it open.
 * @param type a filename extension that represent this data ("GIF", etc).
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa Animation.Free
 */
inline Animation LoadAnimationTyped(IOStreamParam src,
                                    bool closeio,
                                    StringParam type)
{
  return IMG_LoadAnimationTyped_IO(src, closeio, type);
}

/**
 * Dispose of an Animation and free its resources.
 *
 * The provided `anim` pointer is not valid once this call returns.
 *
 * @param anim Animation to dispose of.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 */
inline void FreeAnimation(AnimationRaw anim) { IMG_FreeAnimation(anim); }

inline void Animation::Free()
{
  IMG_FreeAnimation(m_resource);
  m_resource = nullptr;
}

/**
 * Load a GIF animation directly.
 *
 * If you know you definitely have a GIF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream that data will be read from.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 * @sa Animation.Free
 */
inline AnimationRef LoadGIFAnimation(IOStreamParam src)
{
  return IMG_LoadGIFAnimation_IO(src);
}

/**
 * Load a WEBP animation directly.
 *
 * If you know you definitely have a WEBP image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * @param src an IOStream that data will be read from.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 * @sa Animation.Free
 */
inline AnimationRef LoadWEBPAnimation(IOStreamParam src)
{
  return IMG_LoadWEBPAnimation_IO(src);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_IMAGE_H_ */
