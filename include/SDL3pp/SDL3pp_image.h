#ifndef SDL3PP_IMAGE_H_
#define SDL3PP_IMAGE_H_

#include "SDL3pp_error.h"
#include "SDL3pp_render.h"
#include "SDL3pp_surface.h"
#include "SDL3pp_version.h"
#include "SDL3pp_video.h"

#if !defined(SDL3PP_DISABLE_IMAGE) && !defined(SDL3PP_ENABLE_IMAGE) &&         \
  __has_include(<SDL3_image/SDL_image.h>)
#define SDL3PP_ENABLE_IMAGE
#endif

#if defined(SDL3PP_ENABLE_IMAGE) || defined(SDL3PP_DOC)

#include <SDL3_image/SDL_image.h>

namespace SDL {

/**
 * @defgroup CategorySDLImage Load images from several formats
 *
 * Header file for SDL_image library
 *
 * A simple library to load images of various formats as SDL surfaces
 * @{
 */

// Forward decl
struct AnimationRef;

// Forward decl
struct Animation;

#ifdef SDL3PP_DOC

/**
 * @name Image version
 * @{
 * Printable format: "%d.%d.%d", MAJOR, MINOR, MICRO
 */
#define SDL_IMAGE_MAJOR_VERSION

#define SDL_IMAGE_MINOR_VERSION

#define SDL_IMAGE_MICRO_VERSION

/// @}

/**
 * This is the version number macro for the current SDL_image version.
 */
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

#endif // SDL3PP_DOC

/**
 * This function gets the version of the dynamically linked SDL_image library.
 *
 * @returns SDL_image version.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline int IMG_Version() { return ::IMG_Version(); }

/**
 * Load an image from an SDL data source into a software surface.
 *
 * An SurfaceRef is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new SurfaceRef data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an SurfaceRef for
 * its specifics, and use SurfaceRef.Convert to then migrate to any supported
 * format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: SurfaceRef.SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * Even though this function accepts a file type, SDL_image may still try
 * other decoders that are capable of detecting file type from the contents of
 * the image data, but may rely on the caller-provided type string for formats
 * that it cannot autodetect. If `type` is nullptr, SDL_image will rely solely
 * on its ability to guess the format.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStreamRef: `LoadSurface("filename.jpg")` will call this function and
 * manage those details for you, determining the file type from the filename's
 * extension.
 *
 * There is also LoadSurface(), which is equivalent to this function except
 * that it will rely on SDL_image to determine what type of data it is
 * loading, much like passing a nullptr for type.
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an TextureRef for use by the GPU without using a
 * software surface: call LoadTexture() instead.
 *
 * @param src an IOStreamRef that data will be read from.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadSurface
 * @sa LoadSurface
 */
inline Surface LoadSurface(IOStreamRef& src, StringParam type)
{
  return Surface{IMG_LoadTyped_IO(src.get(), false, type)};
}

/**
 * Load an image from a filesystem path into a software surface.
 *
 * An SurfaceRef is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new SDL_Surface data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an SurfaceRef for
 * its specifics, and use SurfaceRef::Convert() to then migrate to any
 * supported format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: SurfaceRef::SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * There is a separate function to read files from an SDL_IOStream, if you
 * need an i/o abstraction to provide data from anywhere instead of a simple
 * filesystem read; that function is Load(SDL_IOStream*,bool).
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an Texture for use by the GPU without using a
 * software surface: call LoadTexture() instead.
 *
 * @param file a path on the filesystem to load an image from.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Surface LoadSurface(StringParam file) { return Surface{IMG_Load(file)}; }

/**
 * Load an image from an SDL data source into a software surface.
 *
 * An SurfaceRef is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new SurfaceRef data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an SurfaceRef for
 * its specifics, and use SurfaceRef.Convert to then migrate to any supported
 * format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: SurfaceRef.SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStreamRef: `LoadSurface("filename.jpg")` will call this function and
 * manage those details for you, determining the file type from the filename's
 * extension.
 *
 * There is also LoadSurface(), which is equivalent to this function
 * except a file extension (like "BMP", "JPG", etc) can be specified, in case
 * SDL_image cannot autodetect the file format.
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an TextureRef for use by the GPU without using a
 * software surface: call LoadTexture() instead.
 *
 * @param src an IOStreamRef that data will be read from.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadSurface
 * @sa LoadSurface
 * @sa SurfaceRef.reset
 */
inline Surface LoadSurface(IOStreamRef& src)
{
  return Surface{IMG_Load_IO(src.get(), false)};
}

/**
 * Load an image from a filesystem path into a GPU texture.
 *
 * A Texture represents an image in GPU memory, usable by SDL's 2D Render
 * API. This can be significantly more efficient than using a CPU-bound
 * Surface if you don't need to manipulate the image directly after loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * Texture in the most format that most reasonably represents the image
 * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
 * RGBA).
 *
 * If you would rather decode an image to an Surface (a buffer of pixels in CPU
 * memory), call LoadSurface() instead.
 *
 * @param renderer the RendererRef to use to create the GPU texture.
 * @param file a path on the filesystem to load an image from.
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Texture LoadTexture(RendererRef& renderer, StringParam file)
{
  return Texture{IMG_LoadTexture(renderer.get(), file)};
}

/**
 * Load an image from an SDL data source into a GPU texture.
 *
 * An TextureRef represents an image in GPU memory, usable by SDL's 2D Render
 * API. This can be significantly more efficient than using a CPU-bound
 * SurfaceRef if you don't need to manipulate the image directly after
 * loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * TextureRef in the most format that most reasonably represents the image
 * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
 * RGBA).
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStreamRef: `LoadTexture(renderer, "filename.jpg")` will call
 * this function and manage those details for you, determining the file type
 * from the filename's extension.
 *
 * There is also LoadTexture(), which is equivalent to this
 * function except a file extension (like "BMP", "JPG", etc) can be specified,
 * in case SDL_image cannot autodetect the file format.
 *
 * If you would rather decode an image to an SurfaceRef (a buffer of pixels
 * in CPU memory), call LoadSurface() instead.
 *
 * @param renderer the RendererRef to use to create the GPU texture.
 * @param src an IOStreamRef that data will be read from.
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Texture LoadTexture(RendererRef& renderer, IOStreamRef& src)
{
  return Texture{IMG_LoadTexture_IO(renderer.get(), src.get(), false)};
}

/**
 * Load an image from an SDL data source into a GPU texture.
 *
 * An TextureRef represents an image in GPU memory, usable by SDL's 2D Render
 * API. This can be significantly more efficient than using a CPU-bound
 * SurfaceRef if you don't need to manipulate the image directly after
 * loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * TextureRef in the most format that most reasonably represents the image
 * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
 * RGBA).
 *
 * Even though this function accepts a file type, SDL_image may still try
 * other decoders that are capable of detecting file type from the contents of
 * the image data, but may rely on the caller-provided type string for formats
 * that it cannot autodetect. If `type` is nullptr, SDL_image will rely solely
 * on its ability to guess the format.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStreamRef: `LoadTexture("filename.jpg")` will call this
 * function and manage those details for you, determining the file type from
 * the filename's extension.
 *
 * There is also LoadTexture(), which is equivalent to this function
 * except that it will rely on SDL_image to determine what type of data it is
 * loading, much like passing a nullptr for type.
 *
 * If you would rather decode an image to an SurfaceRef (a buffer of pixels
 * in CPU memory), call LoadSurface() instead.
 *
 * @param renderer the RendererRef to use to create the GPU texture.
 * @param src an IOStreamRef that data will be read from.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Texture LoadTexture(RendererRef& renderer,
                           IOStreamRef& src,
                           StringParam type)
{
  return Texture{
    IMG_LoadTextureTyped_IO(renderer.get(), src.get(), false, type)};
}

/**
 * Detect AVIF image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isAVIF(IOStreamRef& src) { return IMG_isAVIF(src.get()); }

/**
 * Detect ICO image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isICO(IOStreamRef& src) { return IMG_isICO(src.get()); }

/**
 * Detect CUR image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isCUR(IOStreamRef& src) { return IMG_isCUR(src.get()); }

/**
 * Detect BMP image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isBMP(IOStreamRef& src) { return IMG_isBMP(src.get()); }

/**
 * Detect GIF image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isGIF(IOStreamRef& src) { return IMG_isGIF(src.get()); }

/**
 * Detect JPG image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isJPG(IOStreamRef& src) { return IMG_isJPG(src.get()); }

/**
 * Detect JXL image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isJXL(IOStreamRef& src) { return IMG_isJXL(src.get()); }

/**
 * Detect LBM image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isLBM(IOStreamRef& src) { return IMG_isLBM(src.get()); }

/**
 * Detect PCX image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isPCX(IOStreamRef& src) { return IMG_isPCX(src.get()); }

/**
 * Detect PNG image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isPNG(IOStreamRef& src) { return IMG_isPNG(src.get()); }

/**
 * Detect PNM image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isPNM(IOStreamRef& src) { return IMG_isPNM(src.get()); }

/**
 * Detect SVG image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isSVG(IOStreamRef& src) { return IMG_isSVG(src.get()); }

/**
 * Detect QOI image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isQOI(IOStreamRef& src) { return IMG_isQOI(src.get()); }

/**
 * Detect TIFF image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isTIF(IOStreamRef& src) { return IMG_isTIF(src.get()); }

/**
 * Detect XCF image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isXCF(IOStreamRef& src) { return IMG_isXCF(src.get()); }

/**
 * Detect XPM image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isXPM(IOStreamRef& src) { return IMG_isXPM(src.get()); }

/**
 * Detect XV image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isXV(IOStreamRef& src) { return IMG_isXV(src.get()); }

/**
 * Detect WEBP image data on a readable/seekable IOStreamRef.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStreamRef (usually a few bytes).
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
 * @param src a seekable/readable IOStreamRef to provide image data.
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
inline bool isWEBP(IOStreamRef& src) { return IMG_isWEBP(src.get()); }

/**
 * Load a AVIF image directly.
 *
 * If you know you definitely have a AVIF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadAVIF(IOStreamRef& src)
{
  return Surface{IMG_LoadAVIF_IO(src.get())};
}

/**
 * Load a ICO image directly.
 *
 * If you know you definitely have a ICO image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadICO(IOStreamRef& src)
{
  return Surface{IMG_LoadICO_IO(src.get())};
}

/**
 * Load a CUR image directly.
 *
 * If you know you definitely have a CUR image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadCUR(IOStreamRef& src)
{
  return Surface{IMG_LoadCUR_IO(src.get())};
}

/**
 * Load a GIF image directly.
 *
 * If you know you definitely have a GIF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadGIF(IOStreamRef& src)
{
  return Surface{IMG_LoadGIF_IO(src.get())};
}

/**
 * Load a JPG image directly.
 *
 * If you know you definitely have a JPG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadJPG(IOStreamRef& src)
{
  return Surface{IMG_LoadJPG_IO(src.get())};
}

/**
 * Load a JXL image directly.
 *
 * If you know you definitely have a JXL image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadJXL(IOStreamRef& src)
{
  return Surface{IMG_LoadJXL_IO(src.get())};
}

/**
 * Load a LBM image directly.
 *
 * If you know you definitely have a LBM image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadLBM(IOStreamRef& src)
{
  return Surface{IMG_LoadLBM_IO(src.get())};
}

/**
 * Load a PCX image directly.
 *
 * If you know you definitely have a PCX image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadPCX(IOStreamRef& src)
{
  return Surface{IMG_LoadPCX_IO(src.get())};
}

/**
 * Load a PNG image directly.
 *
 * If you know you definitely have a PNG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadPNG(IOStreamRef& src)
{
  return Surface{IMG_LoadPNG_IO(src.get())};
}

/**
 * Load a PNM image directly.
 *
 * If you know you definitely have a PNM image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadPNM(IOStreamRef& src)
{
  return Surface{IMG_LoadPNM_IO(src.get())};
}

/**
 * Load a SVG image directly.
 *
 * If you know you definitely have a SVG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadSVG(IOStreamRef& src)
{
  return Surface{IMG_LoadSVG_IO(src.get())};
}

/**
 * Load a QOI image directly.
 *
 * If you know you definitely have a QOI image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadQOI(IOStreamRef& src)
{
  return Surface{IMG_LoadQOI_IO(src.get())};
}

/**
 * Load a TGA image directly.
 *
 * If you know you definitely have a TGA image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadTGA(IOStreamRef& src)
{
  return Surface{IMG_LoadTGA_IO(src.get())};
}

/**
 * Load a TIFF image directly.
 *
 * If you know you definitely have a TIFF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadTIF(IOStreamRef& src)
{
  return Surface{IMG_LoadTIF_IO(src.get())};
}

/**
 * Load a XCF image directly.
 *
 * If you know you definitely have a XCF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadXCF(IOStreamRef& src)
{
  return Surface{IMG_LoadXCF_IO(src.get())};
}

/**
 * Load a XPM image directly.
 *
 * If you know you definitely have a XPM image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadXPM(IOStreamRef& src)
{
  return Surface{IMG_LoadXPM_IO(src.get())};
}

/**
 * Load a XV image directly.
 *
 * If you know you definitely have a XV image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadXV(IOStreamRef& src)
{
  return Surface{IMG_LoadXV_IO(src.get())};
}

/**
 * Load a WEBP image directly.
 *
 * If you know you definitely have a WEBP image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef to load image data from.
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
inline Surface LoadWEBP(IOStreamRef& src)
{
  return Surface{IMG_LoadWEBP_IO(src.get())};
}

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
 * call to SurfaceRef.reset().
 *
 * @param src an IOStreamRef to load SVG data from.
 * @param width desired width of the generated surface, in pixels.
 * @param height desired height of the generated surface, in pixels.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Surface LoadSizedSVG(IOStreamRef& src, int width, int height)
{
  return Surface{IMG_LoadSizedSVG_IO(src.get(), width, height)};
}

/**
 * Load an XPM image from a memory array.
 *
 * The returned surface will be an 8bpp indexed surface, if possible,
 * otherwise it will be 32bpp. If you always want 32-bit data, use
 * IMG_ReadXPMFromArrayToRGB888() instead.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to SDL_DestroySurface().
 *
 * @param xpm a nullptr-terminated array of strings that comprise XPM data.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa ReadXPMFromArrayToRGB888
 */
inline Surface ReadXPMFromArray(char** xpm)
{
  return Surface{IMG_ReadXPMFromArray(xpm)};
}

/**
 * Load an XPM image from a memory array.
 *
 * The returned surface will always be a 32-bit RGB surface. If you want 8-bit
 * indexed colors (and the XPM data allows it), use IMG_ReadXPMFromArray()
 * instead.
 *
 * When done with the returned surface, the app should dispose of it with a
 * call to SDL_DestroySurface().
 *
 * @param xpm a nullptr-terminated array of strings that comprise XPM data.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa ReadXPMFromArray
 */
inline Surface ReadXPMFromArrayToRGB888(char** xpm)
{
  return Surface{IMG_ReadXPMFromArrayToRGB888(xpm)};
}

/**
 * Save an SurfaceRef into a AVIF image file.
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
 */
inline void SaveAVIF(SurfaceRef& surface, StringParam file, int quality)
{
  CheckError(IMG_SaveAVIF(surface.get(), file, quality));
}

/**
 * Save an SurfaceRef into AVIF image data, via an IOStreamRef.
 *
 * If you just want to save to a filename, you can use SaveAVIF() instead.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStreamRef to save the image data to.
 * @param quality the desired quality, ranging between 0 (lowest) and 100
 *                (highest).
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline void SaveAVIF(SurfaceRef surface, IOStreamRef& dst, int quality)
{
  CheckError(IMG_SaveAVIF_IO(surface.get(), dst.get(), false, quality));
}

/**
 * Save an SurfaceRef into a PNG image file.
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
inline void SavePNG(SurfaceRef& surface, StringParam file)
{
  CheckError(IMG_SavePNG(surface.get(), file));
}

/**
 * Save an SurfaceRef into PNG image data, via an IOStreamRef.
 *
 * If you just want to save to a filename, you can use SavePNG() instead.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStreamRef to save the image data to.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline void SavePNG(SurfaceRef surface, IOStreamRef& dst)
{
  CheckError(IMG_SavePNG_IO(surface.get(), dst.get(), false));
}

/**
 * Save an SurfaceRef into a JPEG image file.
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
 */
inline void SaveJPG(SurfaceRef& surface, StringParam file, int quality)
{
  CheckError(IMG_SaveJPG(surface.get(), file, quality));
}

/**
 * Save an SurfaceRef into JPEG image data, via an IOStreamRef.
 *
 * If you just want to save to a filename, you can use SaveJPG() instead.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStreamRef to save the image data to.
 * @param quality [0; 33] is Lowest quality, [34; 66] is Middle quality, [67;
 *                100] is Highest quality.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline void SaveJPG(SurfaceRef surface, IOStreamRef& dst, int quality)
{
  CheckError(IMG_SaveJPG_IO(surface.get(), dst.get(), false, quality));
}

/**
 * Animated image support
 *
 * Currently only animated GIFs and WEBP images are supported.
 *
 * @cat resource
 *
 * @sa Animation
 */
struct AnimationRef : Resource<IMG_Animation*>
{
  using Resource::Resource;

  /**
   * Copy constructor.
   */
  constexpr AnimationRef(const AnimationRef& other)
    : AnimationRef(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr AnimationRef(AnimationRef&& other)
    : AnimationRef(other.release())
  {
  }

  /**
   * Load an animation from a file.
   *
   * @param file path on the filesystem containing an animated image.
   * @post a new Animation, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa AnimationRef.reset
   */
  AnimationRef(StringParam file)
    : Resource(IMG_LoadAnimation(file))
  {
  }

  /**
   * Load an animation from an IOStreamRef.
   *
   * @param src an IOStreamRef that data will be read from.
   * @post a new AnimationRef, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa AnimationRef.reset
   */
  AnimationRef(IOStreamRef& src)
    : Resource(IMG_LoadAnimation_IO(src.get(), false))
  {
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
   * @param src an SDL_IOStream that data will be read from.
   * @param type a filename extension that represent this data ("GIF", etc).
   * @post a new AnimationRef, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa AnimationRef.reset
   */
  AnimationRef(IOStreamRef& src, StringParam type)
    : Resource(IMG_LoadAnimationTyped_IO(src.get(), false, type))
  {
  }

  /**
   * Assignment operator.
   */
  AnimationRef& operator=(AnimationRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Get the width in pixels.
   */
  int GetWidth() const { return get()->w; }

  /**
   * Get the height in pixels.
   */
  int GetHeight() const { return get()->h; }

  /**
   * Get the size in pixels.
   */
  Point GetSize() const { return Point(GetWidth(), GetHeight()); }

  /**
   * Return the number of frames.
   */
  int GetCount() const { return get()->count; }

  /**
   * Return the frame image under given index.
   *
   * @param index the index to get frame, within [0, GetCount() - 1]
   */
  SurfaceRef GetFrame(int index) const { return get()->frames[index]; }

  /**
   * Return the frame delay under given index.
   *
   * @param index the index to get frame, within [0, GetCount() - 1]
   */
  int GetDelay(int index) const { return get()->delays[index]; }

  /**
   * Dispose of an AnimationRef and free its resources.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa AnimationRef.AnimationRef
   */
  void Free() { reset(); }

  /**
   * Dispose of an AnimationRef and free its resources.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa AnimationRef.AnimationRef
   */
  void reset(IMG_Animation* newResource = {})
  {
    IMG_FreeAnimation(release(newResource));
  }
};
/**
 * Handle to an owned animation
 *
 * @cat resource
 *
 * @sa AnimationRef
 */
struct Animation : AnimationRef
{
  using AnimationRef::AnimationRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Animation(IMG_Animation* resource = {}) {}

  constexpr Animation(const Animation& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Animation(Animation&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Animation() { reset(); }

  /**
   * Assignment operator.
   */
  Animation& operator=(Animation other)
  {
    AnimationRef::operator=(other.release());
    return *this;
  }
};

/**
 * Load a GIF animation directly.
 *
 * If you know you definitely have a GIF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef that data will be read from.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa AnimationRef.AnimationRef
 * @sa AnimationRef.reset
 */
inline Animation LoadGIFAnimation(IOStreamRef& src)
{
  return Animation{IMG_LoadGIFAnimation_IO(src.get())};
}

/**
 * Load a WEBP animation directly.
 *
 * If you know you definitely have a WEBP image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an IOStreamRef
 * interface available here.
 *
 * @param src an IOStreamRef that data will be read from.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa AnimationRef.AnimationRef
 */
inline Animation LoadWEBPAnimation(IOStreamRef& src)
{
  return Animation{IMG_LoadWEBPAnimation_IO(src.get())};
}

#pragma region impl

/// @}

inline SurfaceRef::SurfaceRef(StringParam file)
  : Resource(CheckError(IMG_Load(file)))
{
}

inline SurfaceRef::SurfaceRef(IOStreamRef& src)
  : Resource(CheckError(IMG_Load_IO(src.get(), false)))
{
}

inline TextureRef::TextureRef(RendererRef& renderer, StringParam file)
  : Resource(CheckError(IMG_LoadTexture(renderer.get(), file)))
{
}

inline TextureRef::TextureRef(RendererRef& renderer, IOStream& src)
  : Resource(CheckError(IMG_LoadTexture_IO(renderer.get(), src.get(), false)))
{
}

#pragma endregion impl

} // namespace SDL

#else // defined(SDL3PP_ENABLE_IMAGE) || defined(SDL3PP_DOC)

namespace SDL {

inline SurfaceRef::SurfaceRef(StringParam file)
  : Resource(CheckError(SDL_LoadBMP(file)))
{
}

inline SurfaceRef::SurfaceRef(IOStreamRef& src)
  : Resource(CheckError(SDL_LoadBMP_IO(src.get(), false)))
{
}

inline TextureRef::TextureRef(RendererRef& renderer, StringParam file)
  : Resource(CheckError(LoadTextureBMP(renderer, std::move(file)).release()))
{
}

inline TextureRef::TextureRef(RendererRef& renderer, IOStream& src)
  : Resource(CheckError(LoadTextureBMP(renderer, src).release()))
{
}

}

#endif // defined(SDL3PP_ENABLE_IMAGE) || defined(SDL3PP_DOC)

#endif /* SDL3PP_IMAGE_H_ */
