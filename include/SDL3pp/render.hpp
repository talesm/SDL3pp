#ifndef SDL3PP_RENDER_HPP_
#define SDL3PP_RENDER_HPP_

/**
 * @file render.hpp
 * @brief Header file for SDL 2D rendering functions.
 *
 * This API supports the following features:
 *
 * - single pixel points
 * - single pixel lines
 * - filled rectangles
 * - texture images
 * - 2D polygons
 *
 * The primitives may be drawn in opaque, blended, or additive modes.
 *
 * The texture images may be drawn in opaque, blended, or additive modes. They
 * can have an additional color tint or alpha modulation applied to them, and
 * may also be stretched with linear interpolation.
 *
 * This API is designed to accelerate simple 2D operations. You may want more
 * functionality such as polygons and particle effects and in that case you
 * should use SDL's OpenGL/Direct3D support, the SDL3 GPU API, or one of the
 * many good 3D engines.
 *
 * These functions must be called from the main thread. See this bug for
 * details: https://github.com/libsdl-org/SDL/issues/986
 */

#include <SDL3/SDL_render.h>
#include "blendmode.hpp"
#include "error.hpp"
#include "pixels.hpp"
#include "properties.hpp"
#include "rect.hpp"
#include "stringParam.hpp"
#include "surface.hpp"
#include "video.hpp"

namespace SDL {

/**
 * @brief Vertex structure.
 *
 */
using Vertex = SDL_Vertex;

/**
 * @brief The access pattern allowed for a texture.
 *
 */
using TextureAccess = SDL_TextureAccess;

/**
 * @brief How the logical size is mapped to the output.
 *
 */
using RendererLogicalPresentation = SDL_RendererLogicalPresentation;

/**
 * @brief Get the number of 2D rendering drivers available for the current
 * display.
 *
 * A render driver is a set of code that handles rendering and texture
 * management on a particular display. Normally there is only one, but some
 * drivers may have several available with different capabilities.
 *
 * There may be none if SDL was compiled without render support.
 *
 * @returns the number of built in render drivers.
 *
 * @threadsafety It is safe to call this function from any thread.
 */
inline int GetNumRenderDrivers() { return SDL_GetNumRenderDrivers(); }

/**
 * @brief Use this function to get the name of a built in 2D rendering driver.
 *
 * The list of rendering drivers is given in the order that they are normally
 * initialized by default; the drivers that seem more reasonable to choose
 * first (as far as the SDL developers believe) are earlier in the list.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "opengl",
 * "direct3d12" or "metal". These never have Unicode characters, and are not
 * meant to be proper names.
 *
 * @param index the index of the rendering driver; the value ranges from 0 to
 *              SDL_GetNumRenderDrivers() - 1.
 * @returns the name of the rendering driver at the requested index, or NULL
 *          if an invalid index was specified.
 *
 * @threadsafety It is safe to call this function from any thread.
 */
inline const char* GetRenderDriver(int index)
{
  return SDL_GetRenderDriver(index);
}

template<template<class OBJ> class T>
struct RendererBase;

/// @brief Handle to a non owned renderer
using RendererRef = RendererBase<ObjectRef>;

template<>
struct ObjectDeleter<SDL_Renderer>
{
  void operator()(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); }
};

/// @brief Handle to an owned renderer
using Renderer = RendererBase<ObjectUnique>;
using RendererUnique = Renderer;

/**
 * @brief A structure representing rendering state
 *
 */
template<template<class OBJ> class T>
struct RendererBase : T<SDL_Renderer>
{
  using T<SDL_Renderer>::T;

  /**
   * @brief Create a 2D rendering context for a window.
   *
   * By default the rendering size matches the window size in pixels, but you
   * can call SDL_SetRenderLogicalPresentation() to change the content size and
   * scaling options.
   *
   * It renderer creation fails for any reason this object is falsy; call
   * GetError() for more information.
   *
   * @param window the window where rendering is displayed.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  RendererBase(WindowRef window)
    : T<SDL_Renderer>(SDL_CreateRenderer(window.get(), nullptr))
  {
  }

  /**
   * @brief Create a 2D rendering context for a window.
   *
   * If you want a specific renderer, you can specify its name here. A list of
   * available renderers can be obtained by calling SDL_GetRenderDriver()
   * multiple times, with indices from 0 to SDL_GetNumRenderDrivers()-1. If you
   * don't need a specific renderer, specify NULL and SDL will attempt to choose
   * the best option for you, based on what is available on the user's system.
   *
   * If `name` is a comma-separated list, SDL will try each name, in the order
   * listed, until one succeeds or all of them fail.
   *
   * By default the rendering size matches the window size in pixels, but you
   * can call SDL_SetRenderLogicalPresentation() to change the content size and
   * scaling options.
   *
   * It renderer creation fails for any reason this object is falsy; call
   * GetError() for more information.
   *
   * @param window the window where rendering is displayed.
   * @param name the name of the rendering driver to initialize
   *
   * @threadsafety This function should only be called on the main thread.
   */
  RendererBase(WindowRef window, StringParam name)
    : T<SDL_Renderer>(SDL_CreateRenderer(window.get(), name))
  {
  }

  /**
   * @brief Create a 2D rendering context for a window, with the specified
   * properties.
   *
   * These are the supported properties:
   *
   * - `SDL_PROP_RENDERER_CREATE_NAME_STRING`: the name of the rendering driver
   *   to use, if a specific one is desired
   * - `SDL_PROP_RENDERER_CREATE_WINDOW_POINTER`: the window where rendering is
   *   displayed, required if this isn't a software renderer using a surface
   * - `SDL_PROP_RENDERER_CREATE_SURFACE_POINTER`: the surface where rendering
   *   is displayed, if you want a software renderer without a window
   * - `SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER`: an SDL_Colorspace
   *   value describing the colorspace for output to the display, defaults to
   *   SDL_COLORSPACE_SRGB. The direct3d11, direct3d12, and metal renderers
   *   support SDL_COLORSPACE_SRGB_LINEAR, which is a linear color space and
   *   supports HDR output. If you select SDL_COLORSPACE_SRGB_LINEAR, drawing
   *   still uses the sRGB colorspace, but values can go beyond 1.0 and float
   *   (linear) format textures can be used for HDR content.
   * - `SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER`: non-zero if you want
   *   present synchronized with the refresh rate. This property can take any
   *   value that is supported by SDL_SetRenderVSync() for the renderer.
   *
   * With the vulkan renderer:
   *
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_INSTANCE_POINTER`: the VkInstance to use
   *   with the renderer, optional.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_SURFACE_NUMBER`: the VkSurfaceKHR to use
   *   with the renderer, optional.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_PHYSICAL_DEVICE_POINTER`: the
   *   VkPhysicalDevice to use with the renderer, optional.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_DEVICE_POINTER`: the VkDevice to use
   *   with the renderer, optional.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER`: the
   *   queue family index used for rendering.
   * - `SDL_PROP_RENDERER_CREATE_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER`: the
   *   queue family index used for presentation.
   *
   * It renderer creation fails for any reason this object is falsy; call
   * GetError() for more information.
   *
   * @param props the properties to use.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  RendererBase(PropertiesRef props)
    : T<SDL_Renderer>(SDL_CreateRendererWithProperties(props.get()))
  {
  }

  /**
   * @brief Create a 2D software rendering context for a surface.
   *
   * Two other API which can be used to create Renderer:
   * RendererBase(WindowRef) and SDL_CreateWindowAndRenderer(WindowRef,
   * StringParam). These can _also_ create a software renderer, but they are
   * intended to be used with an Window as the final destination and not an
   * Surface.
   *
   * It renderer creation fails for any reason this object is falsy; call
   * GetError() for more information.
   *
   * @param surface the SDL_Surface structure representing the surface where
   *                rendering is done.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  RendererBase(SurfaceRef surface)
    : T<SDL_Renderer>(SDL_CreateSoftwareRenderer(surface.get()))
  {
  }

  // TODO SDL_GetRenderWindow
};

inline RendererRef GetRenderer(WindowRef window)
{
  return {SDL_GetRenderer(window.get())};
}
// TODO Renderer
// TODO Texture

} // namespace SDL

#endif /* SDL3PP_RENDER_HPP_ */
