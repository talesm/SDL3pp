#ifndef SDL3PP_IMAGE_H_
#define SDL3PP_IMAGE_H_

#include <SDL3/SDL_image.h>
#include "SDL3pp_error.h"
#include "SDL3pp_gpu.h"
#include "SDL3pp_mouse.h"
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

/// Alias to raw representation for Animation.
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
  constexpr AnimationParam(std::nullptr_t = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const AnimationParam& other) const = default;

  /// Converts to underlying AnimationRaw
  constexpr operator AnimationRaw() const { return value; }

  /// member access to underlying AnimationRaw.
  constexpr auto operator->() { return value; }
};

/// Safely wrap Animation for non owning const parameters
struct AnimationConstParam
{
  const AnimationRaw value; ///< parameter's const AnimationRaw

  /// Constructs from const AnimationRaw
  constexpr AnimationConstParam(const AnimationRaw value)
    : value(value)
  {
  }

  /// Constructs from AnimationParam
  constexpr AnimationConstParam(AnimationParam value)
    : value(value.value)
  {
  }

  /// Constructs null/invalid
  constexpr AnimationConstParam(std::nullptr_t = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const AnimationConstParam& other) const = default;

  /// Converts to underlying const AnimationRaw
  constexpr operator const AnimationRaw() const { return value; }

  /// member access to underlying AnimationRaw.
  constexpr auto operator->() { return value; }
};

// Forward decl
struct AnimationEncoder;

/// Alias to raw representation for AnimationEncoder.
using AnimationEncoderRaw = IMG_AnimationEncoder*;

// Forward decl
struct AnimationEncoderRef;

/// Safely wrap AnimationEncoder for non owning parameters
struct AnimationEncoderParam
{
  AnimationEncoderRaw value; ///< parameter's AnimationEncoderRaw

  /// Constructs from AnimationEncoderRaw
  constexpr AnimationEncoderParam(AnimationEncoderRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr AnimationEncoderParam(std::nullptr_t = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const AnimationEncoderParam& other) const =
    default;

  /// Converts to underlying AnimationEncoderRaw
  constexpr operator AnimationEncoderRaw() const { return value; }
};

// Forward decl
struct AnimationDecoder;

/// Alias to raw representation for AnimationDecoder.
using AnimationDecoderRaw = IMG_AnimationDecoder*;

// Forward decl
struct AnimationDecoderRef;

/// Safely wrap AnimationDecoder for non owning parameters
struct AnimationDecoderParam
{
  AnimationDecoderRaw value; ///< parameter's AnimationDecoderRaw

  /// Constructs from AnimationDecoderRaw
  constexpr AnimationDecoderParam(AnimationDecoderRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr AnimationDecoderParam(std::nullptr_t = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const AnimationDecoderParam& other) const =
    default;

  /// Converts to underlying AnimationDecoderRaw
  constexpr operator AnimationDecoderRaw() const { return value; }
};

/// Printable format: "%d.%d.%d", MAJOR, MINOR, MICRO
#define SDL_IMAGE_MAJOR_VERSION

#define SDL_IMAGE_MINOR_VERSION

#define SDL_IMAGE_MICRO_VERSION

/// This is the version number macro for the current SDL_image version.
#define SDL_IMAGE_VERSION                                                      \
  SDL_VERSIONNUM(                                                              \
    SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION)

/// This macro will evaluate to true if compiled with SDL_image at least X.Y.Z.
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
 * Load an image from a filesystem path into a software surface.
 *
 * An Surface is a buffer of pixels in memory accessible by the CPU. Use this if
 * you plan to hand the data to something else or manipulate it further in code.
 *
 * There are no guarantees about what format the new Surface data will be; in
 * many cases, SDL_image will attempt to supply a surface that exactly matches
 * the provided image, but in others it might have to convert (either because
 * the image is in a format that SDL doesn't directly support or because it's
 * compressed data that could reasonably uncompress to various formats and
 * SDL_image had to pick one). You can inspect an Surface for its specifics, and
 * use Surface.Convert to then migrate to any supported format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards by
 * calling: Surface.SetColorKey(image, SDL_RLEACCEL, image->format->colorkey);
 *
 * There is a separate function to read files from an IOStream, if you need an
 * i/o abstraction to provide data from anywhere instead of a simple filesystem
 * read; that function is Surface.Surface().
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to load
 * images directly into an Texture for use by the GPU without using a software
 * surface: call Texture.Texture() instead.
 *
 * When done with the returned surface, the app should dispose of it with a call
 * to [Surface.Destroy](https://wiki.libsdl.org/SDL3/Surface.Destroy) ().
 *
 * @param file a path on the filesystem to load an image from.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadSurfaceTyped
 * @sa Surface.Surface
 */
inline Surface LoadSurface(StringParam file) { return IMG_Load(file); }

/**
 * Load an image from an SDL data source into a software surface.
 *
 * An Surface is a buffer of pixels in memory accessible by the CPU. Use this if
 * you plan to hand the data to something else or manipulate it further in code.
 *
 * There are no guarantees about what format the new Surface data will be; in
 * many cases, SDL_image will attempt to supply a surface that exactly matches
 * the provided image, but in others it might have to convert (either because
 * the image is in a format that SDL doesn't directly support or because it's
 * compressed data that could reasonably uncompress to various formats and
 * SDL_image had to pick one). You can inspect an Surface for its specifics, and
 * use Surface.Convert to then migrate to any supported format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards by
 * calling: Surface.SetColorKey(image, SDL_RLEACCEL, image->format->colorkey);
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
 * There is also LoadSurfaceTyped(), which is equivalent to this function except
 * a file extension (like "BMP", "JPG", etc) can be specified, in case SDL_image
 * cannot autodetect the file format.
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to load
 * images directly into an Texture for use by the GPU without using a software
 * surface: call Texture.Texture() instead.
 *
 * When done with the returned surface, the app should dispose of it with a call
 * to Surface.Destroy().
 *
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Surface.Surface
 * @sa LoadSurfaceTyped
 */
inline Surface LoadSurface(IOStreamParam src, bool closeio)
{
  return IMG_Load_IO(src, closeio);
}

inline Surface::Surface(StringParam file)
  : m_resource(IMG_Load(file))
{
}

inline Surface::Surface(IOStreamParam src, bool closeio)
  : m_resource(IMG_Load_IO(src, closeio))
{
}

/**
 * Load an image from an SDL data source into a software surface.
 *
 * An Surface is a buffer of pixels in memory accessible by the CPU. Use this if
 * you plan to hand the data to something else or manipulate it further in code.
 *
 * There are no guarantees about what format the new Surface data will be; in
 * many cases, SDL_image will attempt to supply a surface that exactly matches
 * the provided image, but in others it might have to convert (either because
 * the image is in a format that SDL doesn't directly support or because it's
 * compressed data that could reasonably uncompress to various formats and
 * SDL_image had to pick one). You can inspect an Surface for its specifics, and
 * use Surface.Convert to then migrate to any supported format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards by
 * calling: Surface.SetColorKey(image, SDL_RLEACCEL, image->format->colorkey);
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * Even though this function accepts a file type, SDL_image may still try other
 * decoders that are capable of detecting file type from the contents of the
 * image data, but may rely on the caller-provided type string for formats that
 * it cannot autodetect. If `type` is nullptr, SDL_image will rely solely on its
 * ability to guess the format.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `Surface.Surface("filename.jpg")` will call this function and
 * manage those details for you, determining the file type from the filename's
 * extension.
 *
 * There is also Surface.Surface(), which is equivalent to this function except
 * that it will rely on SDL_image to determine what type of data it is loading,
 * much like passing a nullptr for type.
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to load
 * images directly into an Texture for use by the GPU without using a software
 * surface: call LoadTextureTyped() instead.
 *
 * When done with the returned surface, the app should dispose of it with a call
 * to Surface.Destroy().
 *
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Surface.Surface
 * @sa Surface.Surface
 */
inline Surface LoadSurfaceTyped(IOStreamParam src,
                                StringParam type,
                                bool closeio = false)
{
  return IMG_LoadTyped_IO(src, type, closeio);
}

/**
 * Load an image from a filesystem path into a texture.
 *
 * An Texture represents an image in GPU memory, usable by SDL's 2D Render API.
 * This can be significantly more efficient than using a CPU-bound Surface if
 * you don't need to manipulate the image directly after loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * Texture in the most format that most reasonably represents the image data
 * (but in many cases, this will just end up being 32-bit RGB or 32-bit RGBA).
 *
 * There is a separate function to read files from an IOStream, if you need an
 * i/o abstraction to provide data from anywhere instead of a simple filesystem
 * read; that function is Texture.Texture().
 *
 * If you would rather decode an image to an Surface (a buffer of pixels in CPU
 * memory), call Surface.Surface() instead.
 *
 * When done with the returned texture, the app should dispose of it with a call
 * to Texture.Destroy().
 *
 * @param renderer the Renderer to use to create the texture.
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
 * Load an image from an SDL data source into a texture.
 *
 * An Texture represents an image in GPU memory, usable by SDL's 2D Render API.
 * This can be significantly more efficient than using a CPU-bound Surface if
 * you don't need to manipulate the image directly after loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * Texture in the most format that most reasonably represents the image data
 * (but in many cases, this will just end up being 32-bit RGB or 32-bit RGBA).
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `Texture.Texture(renderer, "filename.jpg")` will call this
 * function and manage those details for you, determining the file type from the
 * filename's extension.
 *
 * There is also LoadTextureTyped(), which is equivalent to this function except
 * a file extension (like "BMP", "JPG", etc) can be specified, in case SDL_image
 * cannot autodetect the file format.
 *
 * If you would rather decode an image to an Surface (a buffer of pixels in CPU
 * memory), call Surface.Surface() instead.
 *
 * When done with the returned texture, the app should dispose of it with a call
 * to Texture.Destroy().
 *
 * @param renderer the Renderer to use to create the texture.
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Texture.Texture
 * @sa LoadTextureTyped
 */
inline Texture LoadTexture(RendererParam renderer,
                           IOStreamParam src,
                           bool closeio)
{
  return IMG_LoadTexture_IO(renderer, src, closeio);
}

inline Texture::Texture(RendererParam renderer, StringParam file)
  : m_resource(IMG_LoadTexture(renderer, file))
{
}

inline Texture::Texture(RendererParam renderer, IOStreamParam src, bool closeio)
  : m_resource(IMG_LoadTexture_IO(renderer, src, closeio))
{
}

/**
 * Load an image from an SDL data source into a texture.
 *
 * An Texture represents an image in GPU memory, usable by SDL's 2D Render API.
 * This can be significantly more efficient than using a CPU-bound Surface if
 * you don't need to manipulate the image directly after loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * Texture in the most format that most reasonably represents the image data
 * (but in many cases, this will just end up being 32-bit RGB or 32-bit RGBA).
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * Even though this function accepts a file type, SDL_image may still try other
 * decoders that are capable of detecting file type from the contents of the
 * image data, but may rely on the caller-provided type string for formats that
 * it cannot autodetect. If `type` is nullptr, SDL_image will rely solely on its
 * ability to guess the format.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `Texture.Texture("filename.jpg")` will call this function and
 * manage those details for you, determining the file type from the filename's
 * extension.
 *
 * There is also Texture.Texture(), which is equivalent to this function except
 * that it will rely on SDL_image to determine what type of data it is loading,
 * much like passing a nullptr for type.
 *
 * If you would rather decode an image to an Surface (a buffer of pixels in CPU
 * memory), call LoadSurfaceTyped() instead.
 *
 * When done with the returned texture, the app should dispose of it with a call
 * to Texture.Destroy().
 *
 * @param renderer the Renderer to use to create the texture.
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Texture.Texture
 * @sa Texture.Texture
 */
inline Texture LoadTextureTyped(RendererParam renderer,
                                IOStreamParam src,
                                StringParam type,
                                bool closeio = false)
{
  return IMG_LoadTextureTyped_IO(renderer, src, type, closeio);
}

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Load an image from a filesystem path into a GPU texture.
 *
 * An GPUTexture represents an image in GPU memory, usable by SDL's GPU API.
 * Regardless of the source format of the image, this function will create a GPU
 * texture with the format GPU_TEXTUREFORMAT_R8G8B8A8_UNORM with no mip levels.
 * It can be bound as a sampled texture from a graphics or compute pipeline and
 * as a a readonly storage texture in a compute pipeline.
 *
 * There is a separate function to read files from an IOStream, if you need an
 * i/o abstraction to provide data from anywhere instead of a simple filesystem
 * read; that function is LoadGPUTexture().
 *
 * When done with the returned texture, the app should dispose of it with a call
 * to GPUDevice.ReleaseTexture().
 *
 * @param device the GPUDevice to use to create the GPU texture.
 * @param copy_pass the GPUCopyPass to use to upload the loaded image to the GPU
 *                  texture.
 * @param file a path on the filesystem to load an image from.
 * @param width a pointer filled in with the width of the GPU texture. may be
 *              nullptr.
 * @param height a pointer filled in with the width of the GPU texture. may be
 *               nullptr.
 * @returns a new GPU texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa LoadGPUTextureTyped
 * @sa LoadGPUTexture
 */
inline GPUTexture LoadGPUTexture(GPUDeviceParam device,
                                 GPUCopyPass copy_pass,
                                 StringParam file,
                                 int* width = nullptr,
                                 int* height = nullptr)
{
  return IMG_LoadGPUTexture(device, copy_pass, file, width, height);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Load an image from an SDL data source into a GPU texture.
 *
 * An GPUTexture represents an image in GPU memory, usable by SDL's GPU API.
 * Regardless of the source format of the image, this function will create a GPU
 * texture with the format GPU_TEXTUREFORMAT_R8G8B8A8_UNORM with no mip levels.
 * It can be bound as a sampled texture from a graphics or compute pipeline and
 * as a a readonly storage texture in a compute pipeline.
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `LoadGPUTexture(device, copy_pass, "filename.jpg", width,
 * height) will call this function and manage those details for you, determining
 * the file type from the filename's extension.
 *
 * There is also LoadGPUTextureTyped(), which is equivalent to this function
 * except a file extension (like "BMP", "JPG", etc) can be specified, in case
 * SDL_image cannot autodetect the file format.
 *
 * When done with the returned texture, the app should dispose of it with a call
 * to GPUDevice.ReleaseTexture().
 *
 * @param device the GPUDevice to use to create the GPU texture.
 * @param copy_pass the GPUCopyPass to use to upload the loaded image to the GPU
 *                  texture.
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param width a pointer filled in with the width of the GPU texture. may be
 *              nullptr.
 * @param height a pointer filled in with the width of the GPU texture. may be
 *               nullptr.
 * @returns a new GPU texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa LoadGPUTexture
 * @sa LoadGPUTextureTyped
 */
inline GPUTexture LoadGPUTexture(GPUDeviceParam device,
                                 GPUCopyPass copy_pass,
                                 IOStreamParam src,
                                 bool closeio = false,
                                 int* width = nullptr,
                                 int* height = nullptr)
{
  return IMG_LoadGPUTexture_IO(device, copy_pass, src, closeio, width, height);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Load an image from an SDL data source into a GPU texture.
 *
 * An GPUTexture represents an image in GPU memory, usable by SDL's GPU API.
 * Regardless of the source format of the image, this function will create a GPU
 * texture with the format GPU_TEXTUREFORMAT_R8G8B8A8_UNORM with no mip levels.
 * It can be bound as a sampled texture from a graphics or compute pipeline and
 * as a a readonly storage texture in a compute pipeline.
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * Even though this function accepts a file type, SDL_image may still try other
 * decoders that are capable of detecting file type from the contents of the
 * image data, but may rely on the caller-provided type string for formats that
 * it cannot autodetect. If `type` is nullptr, SDL_image will rely solely on its
 * ability to guess the format.
 *
 * There is a separate function to read files from disk without having to deal
 * with IOStream: `LoadGPUTexture(device, copy_pass, "filename.jpg", width,
 * height) will call this function and manage those details for you, determining
 * the file type from the filename's extension.
 *
 * There is also LoadGPUTexture(), which is equivalent to this function except
 * that it will rely on SDL_image to determine what type of data it is loading,
 * much like passing a nullptr for type.
 *
 * When done with the returned texture, the app should dispose of it with a call
 * to GPUDevice.ReleaseTexture().
 *
 * @param device the GPUDevice to use to create the GPU texture.
 * @param copy_pass the GPUCopyPass to use to upload the loaded image to the GPU
 *                  texture.
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @param width a pointer filled in with the width of the GPU texture. may be
 *              nullptr.
 * @param height a pointer filled in with the width of the GPU texture. may be
 *               nullptr.
 * @returns a new GPU texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa LoadGPUTexture
 * @sa LoadGPUTexture
 */
inline GPUTexture LoadGPUTextureTyped(GPUDeviceParam device,
                                      GPUCopyPass copy_pass,
                                      IOStreamParam src,
                                      StringParam type,
                                      bool closeio = false,
                                      int* width = nullptr,
                                      int* height = nullptr)
{
  return IMG_LoadGPUTextureTyped_IO(
    device, copy_pass, src, type, closeio, width, height);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Get the image currently in the clipboard.
 *
 * When done with the returned surface, the app should dispose of it with a call
 * to Surface.Destroy().
 *
 * @returns a new SDL surface, or nullptr if no supported image is available.
 *
 * @since This function is available since SDL_image 3.4.0.
 */
inline Surface GetClipboardImage() { return IMG_GetClipboardImage(); }

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Detect ANI animated cursor data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is ANI animated cursor data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isANI(IOStreamParam src) { return IMG_isANI(src); }

/**
 * Detect AVIF image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is AVIF data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isAVIF(IOStreamParam src) { return IMG_isAVIF(src); }

/**
 * Detect CUR image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is CUR data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isCUR(IOStreamParam src) { return IMG_isCUR(src); }

/**
 * Detect BMP image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is BMP data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isBMP(IOStreamParam src) { return IMG_isBMP(src); }

/**
 * Detect GIF image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is GIF data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isGIF(IOStreamParam src) { return IMG_isGIF(src); }

/**
 * Detect ICO image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is ICO data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isICO(IOStreamParam src) { return IMG_isICO(src); }

/**
 * Detect JPG image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is JPG data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isJPG(IOStreamParam src) { return IMG_isJPG(src); }

/**
 * Detect JXL image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is JXL data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isJXL(IOStreamParam src) { return IMG_isJXL(src); }

/**
 * Detect LBM image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is LBM data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isLBM(IOStreamParam src) { return IMG_isLBM(src); }

/**
 * Detect PCX image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is PCX data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isPCX(IOStreamParam src) { return IMG_isPCX(src); }

/**
 * Detect PNG image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is PNG data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isPNG(IOStreamParam src) { return IMG_isPNG(src); }

/**
 * Detect PNM image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is PNM data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isPNM(IOStreamParam src) { return IMG_isPNM(src); }

/**
 * Detect QOI image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is QOI data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isQOI(IOStreamParam src) { return IMG_isQOI(src); }

/**
 * Detect SVG image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is SVG data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isSVG(IOStreamParam src) { return IMG_isSVG(src); }

/**
 * Detect TIFF image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is TIFF data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isTIF(IOStreamParam src) { return IMG_isTIF(src); }

/**
 * Detect WEBP image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is WEBP data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isXCF
 * @sa isXPM
 * @sa isXV
 */
inline bool isWEBP(IOStreamParam src) { return IMG_isWEBP(src); }

/**
 * Detect XCF image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is XCF data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXPM
 * @sa isXV
 */
inline bool isXCF(IOStreamParam src) { return IMG_isXCF(src); }

/**
 * Detect XPM image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is XPM data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXV
 */
inline bool isXPM(IOStreamParam src) { return IMG_isXPM(src); }

/**
 * Detect XV image data on a readable/seekable IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the IOStream (usually a few bytes).
 *
 * There is no distinction made between "not the filetype in question" and basic
 * i/o errors.
 *
 * This function will always attempt to seek `src` back to where it started when
 * this function was called, but it will not report any errors in doing so, but
 * assuming seeking works, this means you can immediately use this with a
 * different IMG_isTYPE function, or load the image without further seeking.
 *
 * You do not need to call this function to load data; SDL_image can work to
 * determine file type in many cases in its standard load functions.
 *
 * @param src a seekable/readable IOStream to provide image data.
 * @returns true if this is XV data, false otherwise.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa isANI
 * @sa isAVIF
 * @sa isBMP
 * @sa isCUR
 * @sa isGIF
 * @sa isICO
 * @sa isJPG
 * @sa isJXL
 * @sa isLBM
 * @sa isPCX
 * @sa isPNG
 * @sa isPNM
 * @sa isQOI
 * @sa isSVG
 * @sa isTIF
 * @sa isWEBP
 * @sa isXCF
 * @sa isXPM
 */
inline bool isXV(IOStreamParam src) { return IMG_isXV(src); }

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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 */
inline Surface LoadAVIF(IOStreamParam src) { return IMG_LoadAVIF_IO(src); }

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
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 */
inline Surface LoadBMP(IOStreamParam src) { return IMG_LoadBMP_IO(src); }

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
 * @sa LoadBMP
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 */
inline Surface LoadCUR(IOStreamParam src) { return IMG_LoadCUR_IO(src); }

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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 */
inline Surface LoadGIF(IOStreamParam src) { return IMG_LoadGIF_IO(src); }

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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 */
inline Surface LoadICO(IOStreamParam src) { return IMG_LoadICO_IO(src); }

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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSizedSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 */
inline Surface LoadSVG(IOStreamParam src) { return IMG_LoadSVG_IO(src); }

/**
 * Load an SVG image, scaled to a specific size.
 *
 * Since SVG files are resolution-independent, you specify the size you would
 * like the output image to be and it will be generated at those dimensions.
 *
 * Either width or height may be 0 and the image will be auto-sized to preserve
 * aspect ratio.
 *
 * When done with the returned surface, the app should dispose of it with a call
 * to Surface.Destroy().
 *
 * @param src an IOStream to load SVG data from.
 * @param width desired width of the generated surface, in pixels.
 * @param height desired height of the generated surface, in pixels.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadSVG
 */
inline Surface LoadSizedSVG(IOStreamParam src, const PointRaw& size)
{
  return IMG_LoadSizedSVG_IO(src, size);
}

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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 */
inline Surface LoadTIF(IOStreamParam src) { return IMG_LoadTIF_IO(src); }

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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadXCF
 * @sa LoadXPM
 * @sa LoadXV
 */
inline Surface LoadWEBP(IOStreamParam src) { return IMG_LoadWEBP_IO(src); }

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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXPM
 * @sa LoadXV
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
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXV
 */
inline Surface LoadXPM(IOStreamParam src) { return IMG_LoadXPM_IO(src); }

/**
 * Load a XV image directly.
 *
 * If you know you definitely have a XV image, you can call this function, which
 * will skip SDL_image's file format detection routines. Generally it's better
 * to use the abstract interfaces; also, there is only an IOStream interface
 * available here.
 *
 * @param src an IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF
 * @sa LoadBMP
 * @sa LoadCUR
 * @sa LoadGIF
 * @sa LoadICO
 * @sa LoadJPG
 * @sa LoadJXL
 * @sa LoadLBM
 * @sa LoadPCX
 * @sa LoadPNG
 * @sa LoadPNM
 * @sa LoadQOI
 * @sa LoadSVG
 * @sa LoadTGA
 * @sa LoadTIF
 * @sa LoadWEBP
 * @sa LoadXCF
 * @sa LoadXPM
 */
inline Surface LoadXV(IOStreamParam src) { return IMG_LoadXV_IO(src); }

/**
 * Load an XPM image from a memory array.
 *
 * The returned surface will be an 8bpp indexed surface, if possible, otherwise
 * it will be 32bpp. If you always want 32-bit data, use
 * ReadXPMFromArrayToRGB888() instead.
 *
 * When done with the returned surface, the app should dispose of it with a call
 * to Surface.Destroy().
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
 * indexed colors (and the XPM data allows it), use ReadXPMFromArray() instead.
 *
 * When done with the returned surface, the app should dispose of it with a call
 * to Surface.Destroy().
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

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into an image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * For formats that accept a quality, a default quality of 90 will be used.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveTyped
 * @sa SaveAVIF
 * @sa SaveBMP
 * @sa SaveCUR
 * @sa SaveGIF
 * @sa SaveICO
 * @sa SaveJPG
 * @sa SavePNG
 * @sa SaveTGA
 * @sa SaveWEBP
 */
inline void Save(SurfaceParam surface, StringParam file)
{
  CheckError(IMG_Save(surface, file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into formatted image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use Save() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * For formats that accept a quality, a default quality of 90 will be used.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa Save
 * @sa SaveAVIF
 * @sa SaveBMP
 * @sa SaveCUR
 * @sa SaveGIF
 * @sa SaveICO
 * @sa SaveJPG
 * @sa SavePNG
 * @sa SaveTGA
 * @sa SaveWEBP
 */
inline void SaveTyped(SurfaceParam surface,
                      IOStreamParam dst,
                      StringParam type,
                      bool closeio = false)
{
  CheckError(IMG_SaveTyped_IO(surface, dst, type, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

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
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
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
                     int quality,
                     bool closeio = false)
{
  CheckError(IMG_SaveAVIF_IO(surface, dst, quality, closeio));
}

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into a BMP image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveBMP
 */
inline void SaveBMP(SurfaceParam surface, StringParam file)
{
  CheckError(IMG_SaveBMP(surface, file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into BMP image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use SaveBMP() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveBMP
 */
inline void SaveBMP(SurfaceParam surface,
                    IOStreamParam dst,
                    bool closeio = false)
{
  CheckError(IMG_SaveBMP_IO(surface, dst, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into a CUR image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveCUR
 */
inline void SaveCUR(SurfaceParam surface, StringParam file)
{
  CheckError(IMG_SaveCUR(surface, file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into CUR image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use SaveCUR() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveCUR
 */
inline void SaveCUR(SurfaceParam surface,
                    IOStreamParam dst,
                    bool closeio = false)
{
  CheckError(IMG_SaveCUR_IO(surface, dst, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into a GIF image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveGIF
 */
inline void SaveGIF(SurfaceParam surface, StringParam file)
{
  CheckError(IMG_SaveGIF(surface, file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into GIF image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use SaveGIF() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveGIF
 */
inline void SaveGIF(SurfaceParam surface,
                    IOStreamParam dst,
                    bool closeio = false)
{
  CheckError(IMG_SaveGIF_IO(surface, dst, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into a ICO image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveICO
 */
inline void SaveICO(SurfaceParam surface, StringParam file)
{
  CheckError(IMG_SaveICO(surface, file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into ICO image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use SaveICO() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveICO
 */
inline void SaveICO(SurfaceParam surface,
                    IOStreamParam dst,
                    bool closeio = false)
{
  CheckError(IMG_SaveICO_IO(surface, dst, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

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
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
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
                    int quality,
                    bool closeio = false)
{
  CheckError(IMG_SaveJPG_IO(surface, dst, quality, closeio));
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
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SavePNG
 */
inline void SavePNG(SurfaceParam surface,
                    IOStreamParam dst,
                    bool closeio = false)
{
  CheckError(IMG_SavePNG_IO(surface, dst, closeio));
}

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into a TGA image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveTGA
 */
inline void SaveTGA(SurfaceParam surface, StringParam file)
{
  CheckError(IMG_SaveTGA(surface, file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into TGA image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use SaveTGA() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveTGA
 */
inline void SaveTGA(SurfaceParam surface,
                    IOStreamParam dst,
                    bool closeio = false)
{
  CheckError(IMG_SaveTGA_IO(surface, dst, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into a WEBP image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write the new file to.
 * @param quality between 0 and 100. For lossy, 0 gives the smallest size and
 *                100 the largest. For lossless, this parameter is the amount of
 *                effort put into the compression: 0 is the fastest but gives
 *                larger files compared to the slowest, but best, 100.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveWEBP
 */
inline void SaveWEBP(SurfaceParam surface, StringParam file, float quality)
{
  CheckError(IMG_SaveWEBP(surface, file, quality));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an Surface into WEBP image data, via an IOStream.
 *
 * If you just want to save to a filename, you can use SaveWEBP() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the IOStream to save the image data to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param quality between 0 and 100. For lossy, 0 gives the smallest size and
 *                100 the largest. For lossless, this parameter is the amount of
 *                effort put into the compression: 0 is the fastest but gives
 *                larger files compared to the slowest, but best, 100.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa SaveWEBP
 */
inline void SaveWEBP(SurfaceParam surface,
                     IOStreamParam dst,
                     float quality,
                     bool closeio = false)
{
  CheckError(IMG_SaveWEBP_IO(surface, dst, quality, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Animated image support
 *
 * @cat resource
 */
class Animation
{
  AnimationRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Animation(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from AnimationParam.
   *
   * @param resource a AnimationRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Animation(const AnimationRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Animation(const Animation& other) noexcept = default;

public:
  /// Move constructor
  constexpr Animation(Animation&& other) noexcept
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
   * @sa Animation.CreateCursor
   * @sa Animation.Animation
   * @sa LoadAnimationTyped
   * @sa LoadANIAnimation
   * @sa LoadAPNGAnimation
   * @sa LoadAVIFAnimation
   * @sa LoadGIFAnimation
   * @sa LoadWEBPAnimation
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
   * @param closeio true to close/free the IOStream before returning, false to
   *                leave it open.
   * @post a new Animation, or nullptr on error.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa Animation.CreateCursor
   * @sa Animation.Animation
   * @sa LoadAnimationTyped
   * @sa LoadANIAnimation
   * @sa LoadAPNGAnimation
   * @sa LoadAVIFAnimation
   * @sa LoadGIFAnimation
   * @sa LoadWEBPAnimation
   * @sa Animation.Free
   */
  Animation(IOStreamParam src, bool closeio)
    : m_resource(IMG_LoadAnimation_IO(src, closeio))
  {
  }

  /// member access to underlying AnimationRaw.
  constexpr const AnimationRaw operator->() const noexcept
  {
    return m_resource;
  }

  /// member access to underlying AnimationRaw.
  constexpr AnimationRaw operator->() noexcept { return m_resource; }

  /// Destructor
  ~Animation() { IMG_FreeAnimation(m_resource); }

  /// Assignment operator.
  constexpr Animation& operator=(Animation&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Animation& operator=(const Animation& other) noexcept = default;

public:
  /// Retrieves underlying AnimationRaw.
  constexpr AnimationRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying AnimationRaw and clear this.
  constexpr AnimationRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Animation& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to AnimationParam
  constexpr operator AnimationParam() const noexcept { return {m_resource}; }

  /**
   * Dispose of an Animation and free its resources.
   *
   * The provided `anim` pointer is not valid once this call returns.
   *
   * @since This function is available since SDL_image 3.0.0.
   *
   * @sa Animation.Animation
   * @sa Animation.Animation
   * @sa LoadAnimationTyped
   * @sa LoadANIAnimation
   * @sa LoadAPNGAnimation
   * @sa LoadAVIFAnimation
   * @sa LoadGIFAnimation
   * @sa LoadWEBPAnimation
   */
  void Free();

  int GetWidth() const;

  int GetHeight() const;

  Point GetSize() const;

  int GetCount() const;

  Surface GetFrame(int index) const;

  int GetDelay(int index) const;

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Save an animation to a file.
   *
   * For formats that accept a quality, a default quality of 90 will be used.
   *
   * @param file path on the filesystem containing an animated image.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa Animation.SaveTyped
   * @sa Animation.SaveANI
   * @sa Animation.SaveAPNG
   * @sa Animation.SaveAVIF
   * @sa Animation.SaveGIF
   * @sa Animation.SaveWEBP
   */
  void Save(StringParam file);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Save an animation to an IOStream.
   *
   * If you just want to save to a filename, you can use Animation.Save()
   * instead.
   *
   * If `closeio` is true, `dst` will be closed before returning, whether this
   * function succeeds or not.
   *
   * For formats that accept a quality, a default quality of 90 will be used.
   *
   * @param dst an IOStream that data will be written to.
   * @param closeio true to close/free the IOStream before returning, false to
   *                leave it open.
   * @param type a filename extension that represent this data ("GIF", etc).
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa Animation.Save
   * @sa Animation.SaveANI
   * @sa Animation.SaveAPNG
   * @sa Animation.SaveAVIF
   * @sa Animation.SaveGIF
   * @sa Animation.SaveWEBP
   */
  void SaveTyped(IOStreamParam dst, StringParam type, bool closeio = false);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Save an animation in ANI format to an IOStream.
   *
   * If `closeio` is true, `dst` will be closed before returning, whether this
   * function succeeds or not.
   *
   * @param dst an IOStream from which data will be written to.
   * @param closeio true to close/free the IOStream before returning, false to
   *                leave it open.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa Animation.Save
   * @sa Animation.SaveTyped
   * @sa Animation.SaveAPNG
   * @sa Animation.SaveAVIF
   * @sa Animation.SaveGIF
   * @sa Animation.SaveWEBP
   */
  void SaveANI(IOStreamParam dst, bool closeio = false);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Save an animation in APNG format to an IOStream.
   *
   * If `closeio` is true, `dst` will be closed before returning, whether this
   * function succeeds or not.
   *
   * @param dst an IOStream from which data will be written to.
   * @param closeio true to close/free the IOStream before returning, false to
   *                leave it open.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa Animation.Save
   * @sa Animation.SaveTyped
   * @sa Animation.SaveANI
   * @sa Animation.SaveAVIF
   * @sa Animation.SaveGIF
   * @sa Animation.SaveWEBP
   */
  void SaveAPNG(IOStreamParam dst, bool closeio = false);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Save an animation in AVIF format to an IOStream.
   *
   * If `closeio` is true, `dst` will be closed before returning, whether this
   * function succeeds or not.
   *
   * @param dst an IOStream from which data will be written to.
   * @param closeio true to close/free the IOStream before returning, false to
   *                leave it open.
   * @param quality the desired quality, ranging between 0 (lowest) and 100
   *                (highest).
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa Animation.Save
   * @sa Animation.SaveTyped
   * @sa Animation.SaveANI
   * @sa Animation.SaveAPNG
   * @sa Animation.SaveGIF
   * @sa Animation.SaveWEBP
   */
  void SaveAVIF(IOStreamParam dst, int quality, bool closeio = false);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Save an animation in GIF format to an IOStream.
   *
   * If `closeio` is true, `dst` will be closed before returning, whether this
   * function succeeds or not.
   *
   * @param dst an IOStream from which data will be written to.
   * @param closeio true to close/free the IOStream before returning, false to
   *                leave it open.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa Animation.Save
   * @sa Animation.SaveTyped
   * @sa Animation.SaveANI
   * @sa Animation.SaveAPNG
   * @sa Animation.SaveAVIF
   * @sa Animation.SaveWEBP
   */
  void SaveGIF(IOStreamParam dst, bool closeio = false);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Save an animation in WEBP format to an IOStream.
   *
   * If `closeio` is true, `dst` will be closed before returning, whether this
   * function succeeds or not.
   *
   * @param dst an IOStream from which data will be written to.
   * @param closeio true to close/free the IOStream before returning, false to
   *                leave it open.
   * @param quality between 0 and 100. For lossy, 0 gives the smallest size and
   *                100 the largest. For lossless, this parameter is the amount
   *                of effort put into the compression: 0 is the fastest but
   *                gives larger files compared to the slowest, but best, 100.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa Animation.Save
   * @sa Animation.SaveTyped
   * @sa Animation.SaveANI
   * @sa Animation.SaveAPNG
   * @sa Animation.SaveAVIF
   * @sa Animation.SaveGIF
   */
  void SaveWEBP(IOStreamParam dst, int quality, bool closeio = false);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Create an animated cursor from an animation.
   *
   * @param hot_x the x position of the cursor hot spot.
   * @param hot_y the y position of the cursor hot spot.
   * @returns the new cursor on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa Animation.Animation
   * @sa Animation.Animation
   * @sa LoadAnimationTyped
   */
  Cursor CreateCursor(const PointRaw& hotspot);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)
};

/// Semi-safe reference for Animation.
struct AnimationRef : Animation
{
  using Animation::Animation;

  /**
   * Constructs from AnimationParam.
   *
   * @param resource a AnimationRaw or Animation.
   *
   * This does not takes ownership!
   */
  AnimationRef(AnimationParam resource) noexcept
    : Animation(resource.value)
  {
  }

  /**
   * Constructs from AnimationParam.
   *
   * @param resource a AnimationRaw or Animation.
   *
   * This does not takes ownership!
   */
  AnimationRef(AnimationRaw resource) noexcept
    : Animation(resource)
  {
  }

  /// Copy constructor.
  constexpr AnimationRef(const AnimationRef& other) noexcept = default;

  /// Destructor
  ~AnimationRef() { release(); }
};

inline int GetAnimationWidth(AnimationConstParam anim)
{
  static_assert(false, "Not implemented");
}

inline int Animation::GetWidth() const
{
  return SDL::GetAnimationWidth(m_resource);
}

inline int GetAnimationHeight(AnimationConstParam anim)
{
  static_assert(false, "Not implemented");
}

inline int Animation::GetHeight() const
{
  return SDL::GetAnimationHeight(m_resource);
}

inline Point GetAnimationSize(AnimationConstParam anim)
{
  static_assert(false, "Not implemented");
}

inline Point Animation::GetSize() const
{
  return SDL::GetAnimationSize(m_resource);
}

inline int GetAnimationCount(AnimationConstParam anim)
{
  static_assert(false, "Not implemented");
}

inline int Animation::GetCount() const
{
  return SDL::GetAnimationCount(m_resource);
}

inline Surface GetAnimationFrame(AnimationConstParam anim, int index)
{
  static_assert(false, "Not implemented");
}

inline Surface Animation::GetFrame(int index) const
{
  return SDL::GetAnimationFrame(m_resource, index);
}

inline int GetAnimationDelay(AnimationConstParam anim, int index)
{
  static_assert(false, "Not implemented");
}

inline int Animation::GetDelay(int index) const
{
  return SDL::GetAnimationDelay(m_resource, index);
}

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
 * @sa Animation.CreateCursor
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 * @sa LoadANIAnimation
 * @sa LoadAPNGAnimation
 * @sa LoadAVIFAnimation
 * @sa LoadGIFAnimation
 * @sa LoadWEBPAnimation
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
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Animation.CreateCursor
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 * @sa LoadANIAnimation
 * @sa LoadAPNGAnimation
 * @sa LoadAVIFAnimation
 * @sa LoadGIFAnimation
 * @sa LoadWEBPAnimation
 * @sa Animation.Free
 */
inline Animation LoadAnimation(IOStreamParam src, bool closeio)
{
  return Animation(src, closeio);
}

/**
 * Load an animation from an IOStream.
 *
 * Even though this function accepts a file type, SDL_image may still try other
 * decoders that are capable of detecting file type from the contents of the
 * image data, but may rely on the caller-provided type string for formats that
 * it cannot autodetect. If `type` is nullptr, SDL_image will rely solely on its
 * ability to guess the format.
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * When done with the returned animation, the app should dispose of it with a
 * call to Animation.Free().
 *
 * @param src an IOStream that data will be read from.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param type a filename extension that represent this data ("GIF", etc).
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa Animation.CreateCursor
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadANIAnimation
 * @sa LoadAPNGAnimation
 * @sa LoadAVIFAnimation
 * @sa LoadGIFAnimation
 * @sa LoadWEBPAnimation
 * @sa Animation.Free
 */
inline Animation LoadAnimationTyped(IOStreamParam src,
                                    StringParam type,
                                    bool closeio = false)
{
  return IMG_LoadAnimationTyped_IO(src, type, closeio);
}

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Load an ANI animation directly from an IOStream.
 *
 * If you know you definitely have an ANI image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally, it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * When done with the returned animation, the app should dispose of it with a
 * call to Animation.Free().
 *
 * @param src an IOStream from which data will be read.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa isANI
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 * @sa LoadAPNGAnimation
 * @sa LoadAVIFAnimation
 * @sa LoadGIFAnimation
 * @sa LoadWEBPAnimation
 * @sa Animation.Free
 */
inline Animation LoadANIAnimation(IOStreamParam src)
{
  return IMG_LoadANIAnimation_IO(src);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Load an APNG animation directly from an IOStream.
 *
 * If you know you definitely have an APNG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally, it's
 * better to use the abstract interfaces; also, there is only an IOStream
 * interface available here.
 *
 * When done with the returned animation, the app should dispose of it with a
 * call to Animation.Free().
 *
 * @param src an IOStream from which data will be read.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa isPNG
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 * @sa LoadANIAnimation
 * @sa LoadAVIFAnimation
 * @sa LoadGIFAnimation
 * @sa LoadWEBPAnimation
 * @sa Animation.Free
 */
inline Animation LoadAPNGAnimation(IOStreamParam src)
{
  return IMG_LoadAPNGAnimation_IO(src);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Load an AVIF animation directly from an IOStream.
 *
 * If you know you definitely have an AVIF animation, you can call this
 * function, which will skip SDL_image's file format detection routines.
 * Generally it's better to use the abstract interfaces; also, there is only an
 * IOStream interface available here.
 *
 * When done with the returned animation, the app should dispose of it with a
 * call to Animation.Free().
 *
 * @param src an IOStream that data will be read from.
 * @returns a new Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa isAVIF
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 * @sa LoadANIAnimation
 * @sa LoadAPNGAnimation
 * @sa LoadGIFAnimation
 * @sa LoadWEBPAnimation
 * @sa Animation.Free
 */
inline Animation LoadAVIFAnimation(IOStreamParam src)
{
  return IMG_LoadAVIFAnimation_IO(src);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

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
 * @sa isGIF
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 * @sa LoadANIAnimation
 * @sa LoadAPNGAnimation
 * @sa LoadAVIFAnimation
 * @sa LoadWEBPAnimation
 * @sa Animation.Free
 */
inline Animation LoadGIFAnimation(IOStreamParam src)
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
 * @sa isWEBP
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 * @sa LoadANIAnimation
 * @sa LoadAPNGAnimation
 * @sa LoadAVIFAnimation
 * @sa LoadGIFAnimation
 * @sa Animation.Free
 */
inline Animation LoadWEBPAnimation(IOStreamParam src)
{
  return IMG_LoadWEBPAnimation_IO(src);
}

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an animation to a file.
 *
 * For formats that accept a quality, a default quality of 90 will be used.
 *
 * @param anim the animation to save.
 * @param file path on the filesystem containing an animated image.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa Animation.SaveTyped
 * @sa Animation.SaveANI
 * @sa Animation.SaveAPNG
 * @sa Animation.SaveAVIF
 * @sa Animation.SaveGIF
 * @sa Animation.SaveWEBP
 */
inline void SaveAnimation(AnimationParam anim, StringParam file)
{
  CheckError(IMG_SaveAnimation(anim, file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void Animation::Save(StringParam file)
{
  SDL::SaveAnimation(m_resource, std::move(file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an animation to an IOStream.
 *
 * If you just want to save to a filename, you can use Animation.Save() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * For formats that accept a quality, a default quality of 90 will be used.
 *
 * @param anim the animation to save.
 * @param dst an IOStream that data will be written to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param type a filename extension that represent this data ("GIF", etc).
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa Animation.Save
 * @sa Animation.SaveANI
 * @sa Animation.SaveAPNG
 * @sa Animation.SaveAVIF
 * @sa Animation.SaveGIF
 * @sa Animation.SaveWEBP
 */
inline void SaveAnimationTyped(AnimationParam anim,
                               IOStreamParam dst,
                               StringParam type,
                               bool closeio = false)
{
  CheckError(IMG_SaveAnimationTyped_IO(anim, dst, type, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void Animation::SaveTyped(IOStreamParam dst,
                                 StringParam type,
                                 bool closeio)
{
  SDL::SaveAnimationTyped(m_resource, dst, std::move(type), closeio);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an animation in ANI format to an IOStream.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param anim the animation to save.
 * @param dst an IOStream from which data will be written to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa Animation.Save
 * @sa Animation.SaveTyped
 * @sa Animation.SaveAPNG
 * @sa Animation.SaveAVIF
 * @sa Animation.SaveGIF
 * @sa Animation.SaveWEBP
 */
inline void SaveANIAnimation(AnimationParam anim,
                             IOStreamParam dst,
                             bool closeio = false)
{
  CheckError(IMG_SaveANIAnimation_IO(anim, dst, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void Animation::SaveANI(IOStreamParam dst, bool closeio)
{
  SDL::SaveANIAnimation(m_resource, dst, closeio);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an animation in APNG format to an IOStream.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param anim the animation to save.
 * @param dst an IOStream from which data will be written to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa Animation.Save
 * @sa Animation.SaveTyped
 * @sa Animation.SaveANI
 * @sa Animation.SaveAVIF
 * @sa Animation.SaveGIF
 * @sa Animation.SaveWEBP
 */
inline void SaveAPNGAnimation(AnimationParam anim,
                              IOStreamParam dst,
                              bool closeio = false)
{
  CheckError(IMG_SaveAPNGAnimation_IO(anim, dst, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void Animation::SaveAPNG(IOStreamParam dst, bool closeio)
{
  SDL::SaveAPNGAnimation(m_resource, dst, closeio);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an animation in AVIF format to an IOStream.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param anim the animation to save.
 * @param dst an IOStream from which data will be written to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param quality the desired quality, ranging between 0 (lowest) and 100
 *                (highest).
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa Animation.Save
 * @sa Animation.SaveTyped
 * @sa Animation.SaveANI
 * @sa Animation.SaveAPNG
 * @sa Animation.SaveGIF
 * @sa Animation.SaveWEBP
 */
inline void SaveAVIFAnimation(AnimationParam anim,
                              IOStreamParam dst,
                              int quality,
                              bool closeio = false)
{
  CheckError(IMG_SaveAVIFAnimation_IO(anim, dst, quality, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void Animation::SaveAVIF(IOStreamParam dst, int quality, bool closeio)
{
  SDL::SaveAVIFAnimation(m_resource, dst, quality, closeio);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an animation in GIF format to an IOStream.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param anim the animation to save.
 * @param dst an IOStream from which data will be written to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa Animation.Save
 * @sa Animation.SaveTyped
 * @sa Animation.SaveANI
 * @sa Animation.SaveAPNG
 * @sa Animation.SaveAVIF
 * @sa Animation.SaveWEBP
 */
inline void SaveGIFAnimation(AnimationParam anim,
                             IOStreamParam dst,
                             bool closeio = false)
{
  CheckError(IMG_SaveGIFAnimation_IO(anim, dst, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void Animation::SaveGIF(IOStreamParam dst, bool closeio)
{
  SDL::SaveGIFAnimation(m_resource, dst, closeio);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Save an animation in WEBP format to an IOStream.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param anim the animation to save.
 * @param dst an IOStream from which data will be written to.
 * @param closeio true to close/free the IOStream before returning, false to
 *                leave it open.
 * @param quality between 0 and 100. For lossy, 0 gives the smallest size and
 *                100 the largest. For lossless, this parameter is the amount of
 *                effort put into the compression: 0 is the fastest but gives
 *                larger files compared to the slowest, but best, 100.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa Animation.Save
 * @sa Animation.SaveTyped
 * @sa Animation.SaveANI
 * @sa Animation.SaveAPNG
 * @sa Animation.SaveAVIF
 * @sa Animation.SaveGIF
 */
inline void SaveWEBPAnimation(AnimationParam anim,
                              IOStreamParam dst,
                              int quality,
                              bool closeio = false)
{
  CheckError(IMG_SaveWEBPAnimation_IO(anim, dst, quality, closeio));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void Animation::SaveWEBP(IOStreamParam dst, int quality, bool closeio)
{
  SDL::SaveWEBPAnimation(m_resource, dst, quality, closeio);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Create an animated cursor from an animation.
 *
 * @param anim an animation to use to create an animated cursor.
 * @param hot_x the x position of the cursor hot spot.
 * @param hot_y the y position of the cursor hot spot.
 * @returns the new cursor on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa Animation.Animation
 * @sa Animation.Animation
 * @sa LoadAnimationTyped
 */
inline Cursor CreateAnimatedCursor(AnimationParam anim, const PointRaw& hotspot)
{
  return CheckError(IMG_CreateAnimatedCursor(anim, hotspot));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline Cursor Animation::CreateCursor(const PointRaw& hotspot)
{
  return SDL::CreateAnimatedCursor(m_resource, hotspot);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

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
 * @sa LoadANIAnimation
 * @sa LoadAPNGAnimation
 * @sa LoadAVIFAnimation
 * @sa LoadGIFAnimation
 * @sa LoadWEBPAnimation
 */
inline void FreeAnimation(AnimationRaw anim) { IMG_FreeAnimation(anim); }

inline void Animation::Free() { FreeAnimation(release()); }

/**
 * An object representing the encoder context.
 *
 * @cat resource
 */
class AnimationEncoder
{
  AnimationEncoderRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr AnimationEncoder(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from AnimationEncoderParam.
   *
   * @param resource a AnimationEncoderRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit AnimationEncoder(
    const AnimationEncoderRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr AnimationEncoder(const AnimationEncoder& other) noexcept = default;

public:
  /// Move constructor
  constexpr AnimationEncoder(AnimationEncoder&& other) noexcept
    : AnimationEncoder(other.release())
  {
  }

  constexpr AnimationEncoder(const AnimationEncoderRef& other) = delete;

  constexpr AnimationEncoder(AnimationEncoderRef&& other) = delete;

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Create an encoder to save a series of images to a file.
   *
   * These animation types are currently supported:
   *
   * - ANI
   * - APNG
   * - AVIFS
   * - GIF
   * - WEBP
   *
   * The file type is determined from the file extension, e.g. "file.webp" will
   * be encoded using WEBP.
   *
   * @param file the file where the animation will be saved.
   * @post a new AnimationEncoder, or nullptr on failure; call GetError() for
   *       more information.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AddFrame
   * @sa AnimationEncoder.Close
   */
  AnimationEncoder(StringParam file)
    : m_resource(IMG_CreateAnimationEncoder(file))
  {
  }

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Create an encoder to save a series of images to an IOStream.
   *
   * These animation types are currently supported:
   *
   * - ANI
   * - APNG
   * - AVIFS
   * - GIF
   * - WEBP
   *
   * If `closeio` is true, `dst` will be closed before returning if this
   * function fails, or when the animation encoder is closed if this function
   * succeeds.
   *
   * @param dst an IOStream that will be used to save the stream.
   * @param closeio true to close the IOStream when done, false to leave it
   *                open.
   * @param type a filename extension that represent this data ("WEBP", etc).
   * @post a new AnimationEncoder, or nullptr on failure; call GetError() for
   *       more information.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AddFrame
   * @sa AnimationEncoder.Close
   */
  AnimationEncoder(IOStreamParam dst, StringParam type, bool closeio = false)
    : m_resource(IMG_CreateAnimationEncoder_IO(dst, type, closeio))
  {
  }

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Create an animation encoder with the specified properties.
   *
   * These animation types are currently supported:
   *
   * - ANI
   * - APNG
   * - AVIFS
   * - GIF
   * - WEBP
   *
   * These are the supported properties:
   *
   * - `prop::AnimationEncoder.CREATE_FILENAME_STRING`: the file to save, if an
   *   IOStream isn't being used. This is required if
   *   `prop::AnimationEncoder.CREATE_IOSTREAM_POINTER` isn't set.
   * - `prop::AnimationEncoder.CREATE_IOSTREAM_POINTER`: an IOStream that will
   *   be used to save the stream. This should not be closed until the animation
   *   encoder is closed. This is required if
   *   `prop::AnimationEncoder.CREATE_FILENAME_STRING` isn't set.
   * - `prop::AnimationEncoder.CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN`: true if
   *   closing the animation encoder should also close the associated IOStream.
   * - `prop::AnimationEncoder.CREATE_TYPE_STRING`: the output file type, e.g.
   *   "webp", defaults to the file extension if
   *   `prop::AnimationEncoder.CREATE_FILENAME_STRING` is set.
   * - `prop::AnimationEncoder.CREATE_QUALITY_NUMBER`: the compression quality,
   *   in the range of 0 to 100. The higher the number, the higher the quality
   *   and file size. This defaults to a balanced value for compression and
   *   quality.
   * - `prop::AnimationEncoder.CREATE_TIMEBASE_NUMERATOR_NUMBER`: the numerator
   *   of the fraction used to multiply the pts to convert it to seconds. This
   *   defaults to 1.
   * - `prop::AnimationEncoder.CREATE_TIMEBASE_DENOMINATOR_NUMBER`: the
   *   denominator of the fraction used to multiply the pts to convert it to
   *   seconds. This defaults to 1000.
   *
   * @param props the properties of the animation encoder.
   * @post a new AnimationEncoder, or nullptr on failure; call GetError() for
   *       more information.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AddFrame
   * @sa AnimationEncoder.Close
   */
  AnimationEncoder(PropertiesParam props)
    : m_resource(IMG_CreateAnimationEncoderWithProperties(props))
  {
  }

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /// Destructor
  ~AnimationEncoder() { IMG_CloseAnimationEncoder(m_resource); }

  /// Assignment operator.
  constexpr AnimationEncoder& operator=(AnimationEncoder&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr AnimationEncoder& operator=(
    const AnimationEncoder& other) noexcept = default;

public:
  /// Retrieves underlying AnimationEncoderRaw.
  constexpr AnimationEncoderRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying AnimationEncoderRaw and clear this.
  constexpr AnimationEncoderRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const AnimationEncoder& other) const noexcept =
    default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to AnimationEncoderParam
  constexpr operator AnimationEncoderParam() const noexcept
  {
    return {m_resource};
  }

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Close an animation encoder, finishing any encoding.
   *
   * Calling this function frees the animation encoder, and returns the final
   * status of the encoding process.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AnimationEncoder
   */
  bool Close();

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Add a frame to an animation encoder.
   *
   * @param surface the surface to add as the next frame in the animation.
   * @param duration the duration of the frame, usually in milliseconds but can
   *                 be other units if the
   *                 `prop::AnimationEncoder.CREATE_TIMEBASE_DENOMINATOR_NUMBER`
   *                 property is set when creating the encoder.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.AnimationEncoder
   * @sa AnimationEncoder.Close
   */
  void AddFrame(SurfaceParam surface, Uint64 duration);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)
};

/// Semi-safe reference for AnimationEncoder.
struct AnimationEncoderRef : AnimationEncoder
{
  using AnimationEncoder::AnimationEncoder;

  /**
   * Constructs from AnimationEncoderParam.
   *
   * @param resource a AnimationEncoderRaw or AnimationEncoder.
   *
   * This does not takes ownership!
   */
  AnimationEncoderRef(AnimationEncoderParam resource) noexcept
    : AnimationEncoder(resource.value)
  {
  }

  /**
   * Constructs from AnimationEncoderParam.
   *
   * @param resource a AnimationEncoderRaw or AnimationEncoder.
   *
   * This does not takes ownership!
   */
  AnimationEncoderRef(AnimationEncoderRaw resource) noexcept
    : AnimationEncoder(resource)
  {
  }

  /// Copy constructor.
  constexpr AnimationEncoderRef(const AnimationEncoderRef& other) noexcept =
    default;

  /// Destructor
  ~AnimationEncoderRef() { release(); }
};

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Create an encoder to save a series of images to a file.
 *
 * These animation types are currently supported:
 *
 * - ANI
 * - APNG
 * - AVIFS
 * - GIF
 * - WEBP
 *
 * The file type is determined from the file extension, e.g. "file.webp" will be
 * encoded using WEBP.
 *
 * @param file the file where the animation will be saved.
 * @returns a new AnimationEncoder, or nullptr on failure; call GetError() for
 *          more information.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AddFrame
 * @sa AnimationEncoder.Close
 */
inline AnimationEncoder CreateAnimationEncoder(StringParam file)
{
  return AnimationEncoder(std::move(file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Create an encoder to save a series of images to an IOStream.
 *
 * These animation types are currently supported:
 *
 * - ANI
 * - APNG
 * - AVIFS
 * - GIF
 * - WEBP
 *
 * If `closeio` is true, `dst` will be closed before returning if this function
 * fails, or when the animation encoder is closed if this function succeeds.
 *
 * @param dst an IOStream that will be used to save the stream.
 * @param closeio true to close the IOStream when done, false to leave it open.
 * @param type a filename extension that represent this data ("WEBP", etc).
 * @returns a new AnimationEncoder, or nullptr on failure; call GetError() for
 *          more information.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AddFrame
 * @sa AnimationEncoder.Close
 */
inline AnimationEncoder CreateAnimationEncoder(IOStreamParam dst,
                                               StringParam type,
                                               bool closeio = false)
{
  return AnimationEncoder(dst, std::move(type), closeio);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Create an animation encoder with the specified properties.
 *
 * These animation types are currently supported:
 *
 * - ANI
 * - APNG
 * - AVIFS
 * - GIF
 * - WEBP
 *
 * These are the supported properties:
 *
 * - `prop::AnimationEncoder.CREATE_FILENAME_STRING`: the file to save, if an
 *   IOStream isn't being used. This is required if
 *   `prop::AnimationEncoder.CREATE_IOSTREAM_POINTER` isn't set.
 * - `prop::AnimationEncoder.CREATE_IOSTREAM_POINTER`: an IOStream that will be
 *   used to save the stream. This should not be closed until the animation
 *   encoder is closed. This is required if
 *   `prop::AnimationEncoder.CREATE_FILENAME_STRING` isn't set.
 * - `prop::AnimationEncoder.CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN`: true if closing
 *   the animation encoder should also close the associated IOStream.
 * - `prop::AnimationEncoder.CREATE_TYPE_STRING`: the output file type, e.g.
 *   "webp", defaults to the file extension if
 *   `prop::AnimationEncoder.CREATE_FILENAME_STRING` is set.
 * - `prop::AnimationEncoder.CREATE_QUALITY_NUMBER`: the compression quality, in
 *   the range of 0 to 100. The higher the number, the higher the quality and
 *   file size. This defaults to a balanced value for compression and quality.
 * - `prop::AnimationEncoder.CREATE_TIMEBASE_NUMERATOR_NUMBER`: the numerator of
 *   the fraction used to multiply the pts to convert it to seconds. This
 *   defaults to 1.
 * - `prop::AnimationEncoder.CREATE_TIMEBASE_DENOMINATOR_NUMBER`: the
 *   denominator of the fraction used to multiply the pts to convert it to
 *   seconds. This defaults to 1000.
 *
 * @param props the properties of the animation encoder.
 * @returns a new AnimationEncoder, or nullptr on failure; call GetError() for
 *          more information.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AddFrame
 * @sa AnimationEncoder.Close
 */
inline AnimationEncoder CreateAnimationEncoderWithProperties(
  PropertiesParam props)
{
  return AnimationEncoder(props);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

namespace prop::AnimationEncoder {

constexpr auto CREATE_FILENAME_STRING =
  IMG_PROP_ANIMATION_ENCODER_CREATE_FILENAME_STRING;

constexpr auto CREATE_IOSTREAM_POINTER =
  IMG_PROP_ANIMATION_ENCODER_CREATE_IOSTREAM_POINTER;

constexpr auto CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN =
  IMG_PROP_ANIMATION_ENCODER_CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN;

constexpr auto CREATE_TYPE_STRING =
  IMG_PROP_ANIMATION_ENCODER_CREATE_TYPE_STRING;

constexpr auto CREATE_QUALITY_NUMBER =
  IMG_PROP_ANIMATION_ENCODER_CREATE_QUALITY_NUMBER;

constexpr auto CREATE_TIMEBASE_NUMERATOR_NUMBER =
  IMG_PROP_ANIMATION_ENCODER_CREATE_TIMEBASE_NUMERATOR_NUMBER;

constexpr auto CREATE_TIMEBASE_DENOMINATOR_NUMBER =
  IMG_PROP_ANIMATION_ENCODER_CREATE_TIMEBASE_DENOMINATOR_NUMBER;

constexpr auto CREATE_AVIF_MAX_THREADS_NUMBER =
  IMG_PROP_ANIMATION_ENCODER_CREATE_AVIF_MAX_THREADS_NUMBER;

constexpr auto CREATE_AVIF_KEYFRAME_INTERVAL_NUMBER =
  IMG_PROP_ANIMATION_ENCODER_CREATE_AVIF_KEYFRAME_INTERVAL_NUMBER;

constexpr auto CREATE_GIF_USE_LUT_BOOLEAN =
  IMG_PROP_ANIMATION_ENCODER_CREATE_GIF_USE_LUT_BOOLEAN;

} // namespace prop::AnimationEncoder

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Add a frame to an animation encoder.
 *
 * @param encoder the receiving images.
 * @param surface the surface to add as the next frame in the animation.
 * @param duration the duration of the frame, usually in milliseconds but can be
 *                 other units if the
 *                 `prop::AnimationEncoder.CREATE_TIMEBASE_DENOMINATOR_NUMBER`
 *                 property is set when creating the encoder.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.Close
 */
inline void AddAnimationEncoderFrame(AnimationEncoderParam encoder,
                                     SurfaceParam surface,
                                     Uint64 duration)
{
  CheckError(IMG_AddAnimationEncoderFrame(encoder, surface, duration));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void AnimationEncoder::AddFrame(SurfaceParam surface, Uint64 duration)
{
  SDL::AddAnimationEncoderFrame(m_resource, surface, duration);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Close an animation encoder, finishing any encoding.
 *
 * Calling this function frees the animation encoder, and returns the final
 * status of the encoding process.
 *
 * @param encoder the encoder to close.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AnimationEncoder
 * @sa AnimationEncoder.AnimationEncoder
 */
inline void CloseAnimationEncoder(AnimationEncoderRaw encoder)
{
  CheckError(IMG_CloseAnimationEncoder(encoder));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline bool AnimationEncoder::Close()
{
  return CloseAnimationEncoder(release());
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * An enum representing the status of an animation decoder.
 *
 * @since This enum is available since SDL_image 3.4.0.
 */
using AnimationDecoderStatus = IMG_AnimationDecoderStatus;

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

constexpr AnimationDecoderStatus DECODER_STATUS_INVALID =
  IMG_DECODER_STATUS_INVALID; ///< The decoder is invalid

constexpr AnimationDecoderStatus DECODER_STATUS_OK =
  IMG_DECODER_STATUS_OK; ///< The decoder is ready to decode the next frame

/// The decoder failed to decode a frame, call GetError() for more information.
constexpr AnimationDecoderStatus DECODER_STATUS_FAILED =
  IMG_DECODER_STATUS_FAILED;

constexpr AnimationDecoderStatus DECODER_STATUS_COMPLETE =
  IMG_DECODER_STATUS_COMPLETE; ///< No more frames available

/**
 * An object representing animation decoder.
 *
 * @cat resource
 */
class AnimationDecoder
{
  AnimationDecoderRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr AnimationDecoder(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from AnimationDecoderParam.
   *
   * @param resource a AnimationDecoderRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit AnimationDecoder(
    const AnimationDecoderRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr AnimationDecoder(const AnimationDecoder& other) noexcept = default;

public:
  /// Move constructor
  constexpr AnimationDecoder(AnimationDecoder&& other) noexcept
    : AnimationDecoder(other.release())
  {
  }

  constexpr AnimationDecoder(const AnimationDecoderRef& other) = delete;

  constexpr AnimationDecoder(AnimationDecoderRef&& other) = delete;

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Create a decoder to read a series of images from a file.
   *
   * These animation types are currently supported:
   *
   * - ANI
   * - APNG
   * - AVIFS
   * - GIF
   * - WEBP
   *
   * The file type is determined from the file extension, e.g. "file.webp" will
   * be decoded using WEBP.
   *
   * @param file the file containing a series of images.
   * @post a new AnimationDecoder, or nullptr on failure; call GetError() for
   *       more information.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.GetFrame
   * @sa AnimationDecoder.Reset
   * @sa AnimationDecoder.Close
   */
  AnimationDecoder(StringParam file)
    : m_resource(IMG_CreateAnimationDecoder(file))
  {
  }

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Create a decoder to read a series of images from an IOStream.
   *
   * These animation types are currently supported:
   *
   * - ANI
   * - APNG
   * - AVIFS
   * - GIF
   * - WEBP
   *
   * If `closeio` is true, `src` will be closed before returning if this
   * function fails, or when the animation decoder is closed if this function
   * succeeds.
   *
   * @param src an IOStream containing a series of images.
   * @param closeio true to close the IOStream when done, false to leave it
   *                open.
   * @param type a filename extension that represent this data ("WEBP", etc).
   * @post a new AnimationDecoder, or nullptr on failure; call GetError() for
   *       more information.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.GetFrame
   * @sa AnimationDecoder.Reset
   * @sa AnimationDecoder.Close
   */
  AnimationDecoder(IOStreamParam src, StringParam type, bool closeio = false)
    : m_resource(IMG_CreateAnimationDecoder_IO(src, type, closeio))
  {
  }

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Create an animation decoder with the specified properties.
   *
   * These animation types are currently supported:
   *
   * - ANI
   * - APNG
   * - AVIFS
   * - GIF
   * - WEBP
   *
   * These are the supported properties:
   *
   * - `prop::AnimationDecoder.CREATE_FILENAME_STRING`: the file to load, if an
   *   IOStream isn't being used. This is required if
   *   `prop::AnimationDecoder.CREATE_IOSTREAM_POINTER` isn't set.
   * - `prop::AnimationDecoder.CREATE_IOSTREAM_POINTER`: an IOStream containing
   *   a series of images. This should not be closed until the animation decoder
   *   is closed. This is required if
   *   `prop::AnimationDecoder.CREATE_FILENAME_STRING` isn't set.
   * - `prop::AnimationDecoder.CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN`: true if
   *   closing the animation decoder should also close the associated IOStream.
   * - `prop::AnimationDecoder.CREATE_TYPE_STRING`: the input file type, e.g.
   *   "webp", defaults to the file extension if
   *   `prop::AnimationDecoder.CREATE_FILENAME_STRING` is set.
   *
   * @param props the properties of the animation decoder.
   * @post a new AnimationDecoder, or nullptr on failure; call GetError() for
   *       more information.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.GetFrame
   * @sa AnimationDecoder.Reset
   * @sa AnimationDecoder.Close
   */
  AnimationDecoder(PropertiesParam props)
    : m_resource(IMG_CreateAnimationDecoderWithProperties(props))
  {
  }

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /// Destructor
  ~AnimationDecoder() { IMG_CloseAnimationDecoder(m_resource); }

  /// Assignment operator.
  constexpr AnimationDecoder& operator=(AnimationDecoder&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr AnimationDecoder& operator=(
    const AnimationDecoder& other) noexcept = default;

public:
  /// Retrieves underlying AnimationDecoderRaw.
  constexpr AnimationDecoderRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying AnimationDecoderRaw and clear this.
  constexpr AnimationDecoderRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const AnimationDecoder& other) const noexcept =
    default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to AnimationDecoderParam
  constexpr operator AnimationDecoderParam() const noexcept
  {
    return {m_resource};
  }

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Close an animation decoder, finishing any decoding.
   *
   * Calling this function frees the animation decoder, and returns the final
   * status of the decoding process.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   */
  bool Close();

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Get the properties of an animation decoder.
   *
   * This function returns the properties of the animation decoder, which holds
   * information about the underlying image such as description, copyright text
   * and loop count.
   *
   * @returns the properties ID of the animation decoder, or 0 if there are no
   *          properties; call GetError() for more information.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   */
  PropertiesRef GetProperties();

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Get the next frame in an animation decoder.
   *
   * This function decodes the next frame in the animation decoder, returning it
   * as an Surface. The returned surface should be freed with SDL_FreeSurface()
   * when no longer needed.
   *
   * If the animation decoder has no more frames or an error occurred while
   * decoding the frame, this function returns false. In that case, please call
   * GetError() for more information. If GetError() returns an empty string,
   * that means there are no more available frames. If GetError() returns a
   * valid string, that means the decoding failed.
   *
   * @param frame a pointer filled in with the Surface for the next frame in the
   *              animation.
   * @param duration the duration of the frame, usually in milliseconds but can
   *                 be other units if the
   *                 `prop::AnimationDecoder.CREATE_TIMEBASE_DENOMINATOR_NUMBER`
   *                 property is set when creating the decoder.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.GetStatus
   * @sa AnimationDecoder.Reset
   * @sa AnimationDecoder.Close
   */
  void GetFrame(SDL_Surface** frame, Uint64* duration);

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Get the decoder status indicating the current state of the decoder.
   *
   * @returns the status of the underlying decoder, or DECODER_STATUS_INVALID if
   *          the given decoder is invalid.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationDecoder.GetFrame
   */
  AnimationDecoderStatus GetStatus();

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

  /**
   * Reset an animation decoder.
   *
   * Calling this function resets the animation decoder, allowing it to start
   * from the beginning again. This is useful if you want to decode the frame
   * sequence again without creating a new decoder.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL_image 3.4.0.
   *
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.AnimationDecoder
   * @sa AnimationDecoder.GetFrame
   * @sa AnimationDecoder.Close
   */
  void Reset();

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)
};

/// Semi-safe reference for AnimationDecoder.
struct AnimationDecoderRef : AnimationDecoder
{
  using AnimationDecoder::AnimationDecoder;

  /**
   * Constructs from AnimationDecoderParam.
   *
   * @param resource a AnimationDecoderRaw or AnimationDecoder.
   *
   * This does not takes ownership!
   */
  AnimationDecoderRef(AnimationDecoderParam resource) noexcept
    : AnimationDecoder(resource.value)
  {
  }

  /**
   * Constructs from AnimationDecoderParam.
   *
   * @param resource a AnimationDecoderRaw or AnimationDecoder.
   *
   * This does not takes ownership!
   */
  AnimationDecoderRef(AnimationDecoderRaw resource) noexcept
    : AnimationDecoder(resource)
  {
  }

  /// Copy constructor.
  constexpr AnimationDecoderRef(const AnimationDecoderRef& other) noexcept =
    default;

  /// Destructor
  ~AnimationDecoderRef() { release(); }
};

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Create a decoder to read a series of images from a file.
 *
 * These animation types are currently supported:
 *
 * - ANI
 * - APNG
 * - AVIFS
 * - GIF
 * - WEBP
 *
 * The file type is determined from the file extension, e.g. "file.webp" will be
 * decoded using WEBP.
 *
 * @param file the file containing a series of images.
 * @returns a new AnimationDecoder, or nullptr on failure; call GetError() for
 *          more information.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.GetFrame
 * @sa AnimationDecoder.Reset
 * @sa AnimationDecoder.Close
 */
inline AnimationDecoder CreateAnimationDecoder(StringParam file)
{
  return AnimationDecoder(std::move(file));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Create a decoder to read a series of images from an IOStream.
 *
 * These animation types are currently supported:
 *
 * - ANI
 * - APNG
 * - AVIFS
 * - GIF
 * - WEBP
 *
 * If `closeio` is true, `src` will be closed before returning if this function
 * fails, or when the animation decoder is closed if this function succeeds.
 *
 * @param src an IOStream containing a series of images.
 * @param closeio true to close the IOStream when done, false to leave it open.
 * @param type a filename extension that represent this data ("WEBP", etc).
 * @returns a new AnimationDecoder, or nullptr on failure; call GetError() for
 *          more information.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.GetFrame
 * @sa AnimationDecoder.Reset
 * @sa AnimationDecoder.Close
 */
inline AnimationDecoder CreateAnimationDecoder(IOStreamParam src,
                                               StringParam type,
                                               bool closeio = false)
{
  return AnimationDecoder(src, std::move(type), closeio);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Create an animation decoder with the specified properties.
 *
 * These animation types are currently supported:
 *
 * - ANI
 * - APNG
 * - AVIFS
 * - GIF
 * - WEBP
 *
 * These are the supported properties:
 *
 * - `prop::AnimationDecoder.CREATE_FILENAME_STRING`: the file to load, if an
 *   IOStream isn't being used. This is required if
 *   `prop::AnimationDecoder.CREATE_IOSTREAM_POINTER` isn't set.
 * - `prop::AnimationDecoder.CREATE_IOSTREAM_POINTER`: an IOStream containing a
 *   series of images. This should not be closed until the animation decoder is
 *   closed. This is required if `prop::AnimationDecoder.CREATE_FILENAME_STRING`
 *   isn't set.
 * - `prop::AnimationDecoder.CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN`: true if closing
 *   the animation decoder should also close the associated IOStream.
 * - `prop::AnimationDecoder.CREATE_TYPE_STRING`: the input file type, e.g.
 *   "webp", defaults to the file extension if
 *   `prop::AnimationDecoder.CREATE_FILENAME_STRING` is set.
 *
 * @param props the properties of the animation decoder.
 * @returns a new AnimationDecoder, or nullptr on failure; call GetError() for
 *          more information.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.GetFrame
 * @sa AnimationDecoder.Reset
 * @sa AnimationDecoder.Close
 */
inline AnimationDecoder CreateAnimationDecoderWithProperties(
  PropertiesParam props)
{
  return AnimationDecoder(props);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

namespace prop::AnimationDecoder {

constexpr auto CREATE_FILENAME_STRING =
  IMG_PROP_ANIMATION_DECODER_CREATE_FILENAME_STRING;

constexpr auto CREATE_IOSTREAM_POINTER =
  IMG_PROP_ANIMATION_DECODER_CREATE_IOSTREAM_POINTER;

constexpr auto CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN =
  IMG_PROP_ANIMATION_DECODER_CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN;

constexpr auto CREATE_TYPE_STRING =
  IMG_PROP_ANIMATION_DECODER_CREATE_TYPE_STRING;

constexpr auto CREATE_TIMEBASE_NUMERATOR_NUMBER =
  IMG_PROP_ANIMATION_DECODER_CREATE_TIMEBASE_NUMERATOR_NUMBER;

constexpr auto CREATE_TIMEBASE_DENOMINATOR_NUMBER =
  IMG_PROP_ANIMATION_DECODER_CREATE_TIMEBASE_DENOMINATOR_NUMBER;

constexpr auto CREATE_AVIF_MAX_THREADS_NUMBER =
  IMG_PROP_ANIMATION_DECODER_CREATE_AVIF_MAX_THREADS_NUMBER;

constexpr auto CREATE_AVIF_ALLOW_INCREMENTAL_BOOLEAN =
  IMG_PROP_ANIMATION_DECODER_CREATE_AVIF_ALLOW_INCREMENTAL_BOOLEAN;

constexpr auto CREATE_AVIF_ALLOW_PROGRESSIVE_BOOLEAN =
  IMG_PROP_ANIMATION_DECODER_CREATE_AVIF_ALLOW_PROGRESSIVE_BOOLEAN;

constexpr auto CREATE_GIF_TRANSPARENT_COLOR_INDEX_NUMBER =
  IMG_PROP_ANIMATION_DECODER_CREATE_GIF_TRANSPARENT_COLOR_INDEX_NUMBER;

constexpr auto CREATE_GIF_NUM_COLORS_NUMBER =
  IMG_PROP_ANIMATION_DECODER_CREATE_GIF_NUM_COLORS_NUMBER;

} // namespace prop::AnimationDecoder

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Get the properties of an animation decoder.
 *
 * This function returns the properties of the animation decoder, which holds
 * information about the underlying image such as description, copyright text
 * and loop count.
 *
 * @param decoder the animation decoder.
 * @returns the properties ID of the animation decoder, or 0 if there are no
 *          properties; call GetError() for more information.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 */
inline PropertiesRef GetAnimationDecoderProperties(
  AnimationDecoderParam decoder)
{
  return IMG_GetAnimationDecoderProperties(decoder);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline PropertiesRef AnimationDecoder::GetProperties()
{
  return SDL::GetAnimationDecoderProperties(m_resource);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

namespace prop::Metadata {

constexpr auto IGNORE_PROPS_BOOLEAN = IMG_PROP_METADATA_IGNORE_PROPS_BOOLEAN;

constexpr auto DESCRIPTION_STRING = IMG_PROP_METADATA_DESCRIPTION_STRING;

constexpr auto COPYRIGHT_STRING = IMG_PROP_METADATA_COPYRIGHT_STRING;

constexpr auto TITLE_STRING = IMG_PROP_METADATA_TITLE_STRING;

constexpr auto AUTHOR_STRING = IMG_PROP_METADATA_AUTHOR_STRING;

constexpr auto CREATION_TIME_STRING = IMG_PROP_METADATA_CREATION_TIME_STRING;

constexpr auto FRAME_COUNT_NUMBER = IMG_PROP_METADATA_FRAME_COUNT_NUMBER;

constexpr auto LOOP_COUNT_NUMBER = IMG_PROP_METADATA_LOOP_COUNT_NUMBER;

} // namespace prop::Metadata

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Get the next frame in an animation decoder.
 *
 * This function decodes the next frame in the animation decoder, returning it
 * as an Surface. The returned surface should be freed with SDL_FreeSurface()
 * when no longer needed.
 *
 * If the animation decoder has no more frames or an error occurred while
 * decoding the frame, this function returns false. In that case, please call
 * GetError() for more information. If GetError() returns an empty string, that
 * means there are no more available frames. If GetError() returns a valid
 * string, that means the decoding failed.
 *
 * @param decoder the animation decoder.
 * @param frame a pointer filled in with the Surface for the next frame in the
 *              animation.
 * @param duration the duration of the frame, usually in milliseconds but can be
 *                 other units if the
 *                 `prop::AnimationDecoder.CREATE_TIMEBASE_DENOMINATOR_NUMBER`
 *                 property is set when creating the decoder.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.GetStatus
 * @sa AnimationDecoder.Reset
 * @sa AnimationDecoder.Close
 */
inline void GetAnimationDecoderFrame(AnimationDecoderParam decoder,
                                     SDL_Surface** frame,
                                     Uint64* duration)
{
  CheckError(IMG_GetAnimationDecoderFrame(decoder, frame, duration));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void AnimationDecoder::GetFrame(SDL_Surface** frame, Uint64* duration)
{
  SDL::GetAnimationDecoderFrame(m_resource, frame, duration);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Get the decoder status indicating the current state of the decoder.
 *
 * @param decoder the decoder to get the status of.
 * @returns the status of the underlying decoder, or DECODER_STATUS_INVALID if
 *          the given decoder is invalid.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationDecoder.GetFrame
 */
inline AnimationDecoderStatus GetAnimationDecoderStatus(
  AnimationDecoderParam decoder)
{
  return IMG_GetAnimationDecoderStatus(decoder);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline AnimationDecoderStatus AnimationDecoder::GetStatus()
{
  return SDL::GetAnimationDecoderStatus(m_resource);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Reset an animation decoder.
 *
 * Calling this function resets the animation decoder, allowing it to start from
 * the beginning again. This is useful if you want to decode the frame sequence
 * again without creating a new decoder.
 *
 * @param decoder the decoder to reset.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.GetFrame
 * @sa AnimationDecoder.Close
 */
inline void ResetAnimationDecoder(AnimationDecoderParam decoder)
{
  CheckError(IMG_ResetAnimationDecoder(decoder));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline void AnimationDecoder::Reset()
{
  SDL::ResetAnimationDecoder(m_resource);
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/**
 * Close an animation decoder, finishing any decoding.
 *
 * Calling this function frees the animation decoder, and returns the final
 * status of the decoding process.
 *
 * @param decoder the decoder to close.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_image 3.4.0.
 *
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 * @sa AnimationDecoder.AnimationDecoder
 */
inline void CloseAnimationDecoder(AnimationDecoderRaw decoder)
{
  CheckError(IMG_CloseAnimationDecoder(decoder));
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

#if SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

inline bool AnimationDecoder::Close()
{
  return CloseAnimationDecoder(release());
}

#endif // SDL_IMAGE_VERSION_ATLEAST(3, 4, 0)

/// @}

} // namespace SDL

#endif /* SDL3PP_IMAGE_H_ */
