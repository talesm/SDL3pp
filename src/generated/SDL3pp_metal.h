#ifndef SDL3PP_METAL_H_
#define SDL3PP_METAL_H_

#include <SDL3/SDL_metal.h>
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategoryMetal Category Metal
 *
 * Functions to creating Metal layers and views on SDL windows.
 *
 * This provides some platform-specific glue for Apple platforms. Most macOS
 * and iOS apps can use SDL without these functions, but this API they can be
 * useful for specific OS-level integration tasks.
 *
 * @{
 */

// Forward decl
struct MetalView;

/// Alias to raw representation for MetalView.
using MetalViewRaw = SDL_MetalView;

// Forward decl
struct MetalViewRef;

/// Safely wrap MetalView for non owning parameters
struct MetalViewParam
{
  MetalViewRaw value; ///< parameter's MetalViewRaw

  /// Constructs from MetalViewRaw
  constexpr MetalViewParam(MetalViewRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr MetalViewParam(std::nullptr_t _ = nullptr)
    : value(0)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const MetalViewParam& other) const = default;

  /// Converts to underlying MetalViewRaw
  constexpr operator MetalViewRaw() const { return value; }
};

/**
 * A handle to a CAMetalLayer-backed NSView (macOS) or UIView (iOS/tvOS).
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 */
class MetalView
{
  MetalViewRaw m_resource = 0;

public:
  /// Default ctor
  constexpr MetalView() = default;

  /**
   * Constructs from MetalViewParam.
   *
   * @param resource a MetalViewRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit MetalView(const MetalViewRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr MetalView(const MetalView& other) = delete;

  /// Move constructor
  constexpr MetalView(MetalView&& other)
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
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MetalView.Metal_DestroyView
   * @sa MetalView.GetLayer
   */
  MetalView(WindowParam window)
    : m_resource(SDL_Metal_CreateView(window))
  {
  }

  /// Destructor
  ~MetalView() { SDL_Metal_DestroyView(m_resource); }

  /// Assignment operator.
  MetalView& operator=(MetalView other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying MetalViewRaw.
  constexpr MetalViewRaw get() const { return m_resource; }

  /// Retrieves underlying MetalViewRaw and clear this.
  constexpr MetalViewRaw release()
  {
    auto r = m_resource;
    m_resource = 0;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const MetalView& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to MetalViewParam
  constexpr operator MetalViewParam() const { return {m_resource}; }

  /**
   * Destroy an existing MetalView object.
   *
   * This should be called before Window.Destroy, if MetalView.MetalView was
   * called after Window.Window.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MetalView.MetalView
   */
  void Metal_DestroyView();

  /**
   * Get a pointer to the backing CAMetalLayer for the given view.
   *
   * @returns a pointer.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void* GetLayer();
};

/// Semi-safe reference for MetalView.
struct MetalViewRef : MetalView
{
  /**
   * Constructs from MetalViewParam.
   *
   * @param resource a MetalViewRaw or MetalView.
   *
   * This does not takes ownership!
   */
  MetalViewRef(MetalViewParam resource)
    : MetalView(resource.value)
  {
  }

  /// Copy constructor.
  MetalViewRef(const MetalViewRef& other)
    : MetalView(other.get())
  {
  }

  /// Destructor
  ~MetalViewRef() { release(); }
};

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
 * @returns handle NSView or UIView.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa MetalView.Metal_DestroyView
 * @sa MetalView.GetLayer
 */
inline MetalView Metal_CreateView(WindowParam window)
{
  return MetalView(window);
}

/**
 * Destroy an existing MetalView object.
 *
 * This should be called before Window.Destroy, if MetalView.MetalView was
 * called after Window.Window.
 *
 * @param view the MetalView object.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa MetalView.MetalView
 */
inline void Metal_DestroyView(MetalViewRaw view)
{
  SDL_Metal_DestroyView(view);
}

inline void MetalView::Metal_DestroyView() { Metal_DestroyView(release()); }

/**
 * Get a pointer to the backing CAMetalLayer for the given view.
 *
 * @param view the MetalView object.
 * @returns a pointer.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void* Metal_GetLayer(MetalViewParam view)
{
  return SDL_Metal_GetLayer(view);
}

inline void* MetalView::GetLayer() { return SDL::Metal_GetLayer(m_resource); }

/// @}

} // namespace SDL

#endif /* SDL3PP_METAL_H_ */
