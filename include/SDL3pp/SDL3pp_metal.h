#ifndef SDL3PP_METAL_H_
#define SDL3PP_METAL_H_

#include <SDL3/SDL_metal.h>
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategoryMetal Metal Support
 *
 * Functions to creating Metal layers and views on SDL windows.
 *
 * This provides some platform-specific glue for Apple platforms. Most macOS and
 * iOS apps can use SDL without these functions, but this API they can be useful
 * for specific OS-level integration tasks.
 *
 * @{
 */

// Forward decl
struct MetalView;

/// Alias to raw representation for MetalView.
using MetalViewRaw = SDL_MetalView;

/**
 * Reference for MetalView.
 *
 * This does not take ownership!
 */
using MetalViewRef = ResourceRef<MetalView>;

/**
 * A handle to a CAMetalLayer-backed NSView (macOS) or UIView (iOS/tvOS).
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 */
struct MetalView : ResourceBase<MetalViewRaw>
{
  using ResourceBase::ResourceBase;

  /**
   * Constructs from raw MetalView.
   *
   * @param resource a MetalViewRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit MetalView(MetalViewRaw resource) noexcept
    : ResourceBase(resource)
  {
  }

  /// Copy constructor
  constexpr MetalView(const MetalView& other) = delete;

  /// Move constructor
  constexpr MetalView(MetalView&& other) noexcept
    : MetalView(other.release())
  {
  }

  constexpr MetalView(const MetalViewRef& other) = delete;

  constexpr MetalView(MetalViewRef&& other) = delete;

  /**
   * Create a CAMetalLayer-backed NSView/UIView and attach it to the specified
   * window.
   *
   * On macOS, this does *not* associate a MTLDevice with the CAMetalLayer on
   * its own. It is up to user code to do that.
   *
   * The returned handle can be casted directly to a NSView or UIView. To access
   * the backing CAMetalLayer, call MetalView.GetLayer().
   *
   * @param window the window.
   * @post handle NSView or UIView.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MetalView.Destroy
   * @sa MetalView.GetLayer
   */
  MetalView(WindowRef window);

  /// Destructor
  ~MetalView() { SDL_Metal_DestroyView(get()); }

  /// Assignment operator.
  constexpr MetalView& operator=(MetalView&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }

  /// Assignment operator.
  MetalView& operator=(const MetalView& other) = delete;

  /**
   * Destroy an existing MetalView object.
   *
   * This should be called before Window.Destroy, if Metal_CreateView was called
   * after CreateWindow.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Metal_CreateView
   */
  void Destroy();

  /**
   * Get a pointer to the backing CAMetalLayer for the given view.
   *
   * @returns a pointer.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void* GetLayer();
};

/**
 * Create a CAMetalLayer-backed NSView/UIView and attach it to the specified
 * window.
 *
 * On macOS, this does *not* associate a MTLDevice with the CAMetalLayer on its
 * own. It is up to user code to do that.
 *
 * The returned handle can be casted directly to a NSView or UIView. To access
 * the backing CAMetalLayer, call MetalView.GetLayer().
 *
 * @param window the window.
 * @returns handle NSView or UIView.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa MetalView.Destroy
 * @sa MetalView.GetLayer
 */
inline MetalView Metal_CreateView(WindowRef window)
{
  return MetalView(window);
}

inline MetalView::MetalView(WindowRef window)
  : MetalView(SDL_Metal_CreateView(window))
{
}

/**
 * Destroy an existing MetalView object.
 *
 * This should be called before Window.Destroy, if Metal_CreateView was called
 * after CreateWindow.
 *
 * @param view the MetalView object.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Metal_CreateView
 */
inline void Metal_DestroyView(MetalViewRaw view)
{
  SDL_Metal_DestroyView(view);
}

inline void MetalView::Destroy() { SDL::Metal_DestroyView(release()); }

/**
 * Get a pointer to the backing CAMetalLayer for the given view.
 *
 * @param view the MetalView object.
 * @returns a pointer.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void* Metal_GetLayer(MetalViewRef view)
{
  return SDL_Metal_GetLayer(view);
}

inline void* MetalView::GetLayer() { return SDL::Metal_GetLayer(get()); }

/// @}

} // namespace SDL

#endif /* SDL3PP_METAL_H_ */
