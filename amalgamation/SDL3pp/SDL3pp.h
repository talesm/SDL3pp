// Amalgamated SDL3pp
#ifndef SDL3PP_H_
#define SDL3PP_H_

#include <atomic>
#include <chrono>
#include <concepts>
#include <exception>
#include <format>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <SDL3/SDL.h>

namespace SDL {

/**
 * @defgroup CategoryBits Bit Manipulation
 *
 * Functions for fiddling with bits and bitmasks.
 *
 * @{
 */

/**
 * Get the index of the most significant (set) bit in a 32-bit number.
 *
 * Result is undefined when called with 0. This operation can also be stated
 * as "count leading zeroes" and "log base 2".
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param x the 32-bit value to examine.
 * @returns the index of the most significant bit, or -1 if the value is 0.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr int MostSignificantBitIndex32(Uint32 x)
{
  return SDL_MostSignificantBitIndex32(x);
}

/**
 * Determine if a unsigned 32-bit value has exactly one bit set.
 *
 * If there are no bits set (`x` is zero), or more than one bit set, this
 * returns false. If any one bit is exclusively set, this returns true.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param x the 32-bit value to examine.
 * @returns true if exactly one bit is set in `x`, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool HasExactlyOneBitSet32(Uint32 x)
{
  return SDL_HasExactlyOneBitSet32(x);
}

/// @}

/**
 * @defgroup CategoryBlendmode Blend modes
 *
 * Blend modes decide how two colors will mix together. There are both
 * standard modes for basic needs and a means to create custom modes,
 * dictating what sort of math to do on what color components.
 *
 * @{
 */

/**
 * @name BlendModes
 *
 * @{
 */

/**
 * A set of blend modes used in drawing operations.
 *
 * These predefined blend modes are supported everywhere.
 *
 * Additional values may be obtained from ComposeCustomBlendMode.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa ComposeCustomBlendMode
 */
using BlendMode = Uint32;

constexpr BlendMode BLENDMODE_NONE =
  SDL_BLENDMODE_NONE; ///< no blending: dstRGBA = srcRGBA

/**
 * alpha blending: dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA)), dstA = srcA +
 * (dstA * (1-srcA))
 */
constexpr BlendMode BLENDMODE_BLEND = SDL_BLENDMODE_BLEND;

/**
 * pre-multiplied alpha blending: dstRGBA = srcRGBA + (dstRGBA * (1-srcA))
 */
constexpr BlendMode BLENDMODE_BLEND_PREMULTIPLIED =
  SDL_BLENDMODE_BLEND_PREMULTIPLIED;

/**
 * additive blending: dstRGB = (srcRGB * srcA) + dstRGB, dstA = dstA
 */
constexpr BlendMode BLENDMODE_ADD = SDL_BLENDMODE_ADD;

/**
 * pre-multiplied additive blending: dstRGB = srcRGB + dstRGB, dstA = dstA
 */
constexpr BlendMode BLENDMODE_ADD_PREMULTIPLIED =
  SDL_BLENDMODE_ADD_PREMULTIPLIED;

/**
 * color modulate: dstRGB = srcRGB * dstRGB, dstA = dstA
 */
constexpr BlendMode BLENDMODE_MOD = SDL_BLENDMODE_MOD;

/**
 * color multiply: dstRGB = (srcRGB * dstRGB) + (dstRGB * (1-srcA)), dstA = dstA
 */
constexpr BlendMode BLENDMODE_MUL = SDL_BLENDMODE_MUL;

constexpr BlendMode BLENDMODE_INVALID = SDL_BLENDMODE_INVALID; ///< INVALID

///@}

/**
 * The blend operation used when combining source and destination pixel
 * components.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using BlendOperation = SDL_BlendOperation;

constexpr BlendOperation BLENDOPERATION_ADD =
  SDL_BLENDOPERATION_ADD; ///< dst + src: supported by all renderers

/**
 * src - dst : supported by D3D, OpenGL, OpenGLES, and Vulkan
 */
constexpr BlendOperation BLENDOPERATION_SUBTRACT = SDL_BLENDOPERATION_SUBTRACT;

/**
 * dst - src : supported by D3D, OpenGL, OpenGLES, and Vulkan
 */
constexpr BlendOperation BLENDOPERATION_REV_SUBTRACT =
  SDL_BLENDOPERATION_REV_SUBTRACT;

/**
 * min(dst, src) : supported by D3D, OpenGL, OpenGLES, and Vulkan
 */
constexpr BlendOperation BLENDOPERATION_MINIMUM = SDL_BLENDOPERATION_MINIMUM;

/**
 * max(dst, src) : supported by D3D, OpenGL, OpenGLES, and Vulkan
 */
constexpr BlendOperation BLENDOPERATION_MAXIMUM = SDL_BLENDOPERATION_MAXIMUM;

/**
 * The normalized factor used to multiply pixel components.
 *
 * The blend factors are multiplied with the pixels from a drawing operation
 * (src) and the pixels from the render target (dst) before the blend
 * operation. The comma-separated factors listed above are always applied in
 * the component order red, green, blue, and alpha.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using BlendFactor = SDL_BlendFactor;

constexpr BlendFactor BLENDFACTOR_ZERO = SDL_BLENDFACTOR_ZERO; ///< 0, 0, 0, 0

constexpr BlendFactor BLENDFACTOR_ONE = SDL_BLENDFACTOR_ONE; ///< 1, 1, 1, 1

constexpr BlendFactor BLENDFACTOR_SRC_COLOR =
  SDL_BLENDFACTOR_SRC_COLOR; ///< srcR, srcG, srcB, srcA

constexpr BlendFactor BLENDFACTOR_ONE_MINUS_SRC_COLOR =
  SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR; ///< 1-srcR, 1-srcG, 1-srcB, 1-srcA

constexpr BlendFactor BLENDFACTOR_SRC_ALPHA =
  SDL_BLENDFACTOR_SRC_ALPHA; ///< srcA, srcA, srcA, srcA

constexpr BlendFactor BLENDFACTOR_ONE_MINUS_SRC_ALPHA =
  SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA; ///< 1-srcA, 1-srcA, 1-srcA, 1-srcA

constexpr BlendFactor BLENDFACTOR_DST_COLOR =
  SDL_BLENDFACTOR_DST_COLOR; ///< dstR, dstG, dstB, dstA

constexpr BlendFactor BLENDFACTOR_ONE_MINUS_DST_COLOR =
  SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR; ///< 1-dstR, 1-dstG, 1-dstB, 1-dstA

constexpr BlendFactor BLENDFACTOR_DST_ALPHA =
  SDL_BLENDFACTOR_DST_ALPHA; ///< dstA, dstA, dstA, dstA

constexpr BlendFactor BLENDFACTOR_ONE_MINUS_DST_ALPHA =
  SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA; ///< 1-dstA, 1-dstA, 1-dstA, 1-dstA

/**
 * Compose a custom blend mode for renderers.
 *
 * The functions SDL_SetRenderDrawBlendMode and SDL_SetTextureBlendMode accept
 * the BlendMode returned by this function if the renderer supports it.
 *
 * A blend mode controls how the pixels from a drawing operation (source) get
 * combined with the pixels from the render target (destination). First, the
 * components of the source and destination pixels get multiplied with their
 * blend factors. Then, the blend operation takes the two products and
 * calculates the result that will get stored in the render target.
 *
 * Expressed in pseudocode, it would look like this:
 *
 * ```c
 * dstRGB = colorOperation(srcRGB * srcColorFactor, dstRGB * dstColorFactor);
 * dstA = alphaOperation(srcA * srcAlphaFactor, dstA * dstAlphaFactor);
 * ```
 *
 * Where the functions `colorOperation(src, dst)` and `alphaOperation(src,
 * dst)` can return one of the following:
 *
 * - `src + dst`
 * - `src - dst`
 * - `dst - src`
 * - `min(src, dst)`
 * - `max(src, dst)`
 *
 * The red, green, and blue components are always multiplied with the first,
 * second, and third components of the BlendFactor, respectively. The
 * fourth component is not used.
 *
 * The alpha component is always multiplied with the fourth component of the
 * BlendFactor. The other components are not used in the alpha
 * calculation.
 *
 * Support for these blend modes varies for each renderer. To check if a
 * specific BlendMode is supported, create a renderer and pass it to
 * either SDL_SetRenderDrawBlendMode or SDL_SetTextureBlendMode. They will
 * return with an error if the blend mode is not supported.
 *
 * This list describes the support of custom blend modes for each renderer.
 * All renderers support the four blend modes listed in the BlendMode
 * enumeration.
 *
 * - **direct3d**: Supports all operations with all factors. However, some
 *   factors produce unexpected results with `BLENDOPERATION_MINIMUM` and
 *   `BLENDOPERATION_MAXIMUM`.
 * - **direct3d11**: Same as Direct3D 9.
 * - **opengl**: Supports the `BLENDOPERATION_ADD` operation with all
 *   factors. OpenGL versions 1.1, 1.2, and 1.3 do not work correctly here.
 * - **opengles2**: Supports the `BLENDOPERATION_ADD`,
 *   `BLENDOPERATION_SUBTRACT`, `BLENDOPERATION_REV_SUBTRACT`
 *   operations with all factors.
 * - **psp**: No custom blend mode support.
 * - **software**: No custom blend mode support.
 *
 * Some renderers do not provide an alpha component for the default render
 * target. The `BLENDFACTOR_DST_ALPHA` and
 * `BLENDFACTOR_ONE_MINUS_DST_ALPHA` factors do not have an effect in this
 * case.
 *
 * @param srcColorFactor the BlendFactor applied to the red, green, and
 *                       blue components of the source pixels.
 * @param dstColorFactor the BlendFactor applied to the red, green, and
 *                       blue components of the destination pixels.
 * @param colorOperation the BlendOperation used to combine the red,
 *                       green, and blue components of the source and
 *                       destination pixels.
 * @param srcAlphaFactor the BlendFactor applied to the alpha component of
 *                       the source pixels.
 * @param dstAlphaFactor the BlendFactor applied to the alpha component of
 *                       the destination pixels.
 * @param alphaOperation the BlendOperation used to combine the alpha
 *                       component of the source and destination pixels.
 * @returns an BlendMode that represents the chosen factors and
 *          operations.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_SetRenderDrawBlendMode
 * @sa SDL_GetRenderDrawBlendMode
 * @sa SDL_SetTextureBlendMode
 * @sa SDL_GetTextureBlendMode
 */
inline BlendMode ComposeCustomBlendMode(BlendFactor srcColorFactor,
                                        BlendFactor dstColorFactor,
                                        BlendOperation colorOperation,
                                        BlendFactor srcAlphaFactor,
                                        BlendFactor dstAlphaFactor,
                                        BlendOperation alphaOperation)
{
  return SDL_ComposeCustomBlendMode(srcColorFactor,
                                    dstColorFactor,
                                    colorOperation,
                                    srcAlphaFactor,
                                    dstAlphaFactor,
                                    alphaOperation);
}

/// @}

/**
 * @defgroup CategoryCallbackWrapper Async callback helpers
 *
 * Async callback wrapper helper functions and types.
 *
 * @{
 */

/** A typesafe handle for callback */
class CallbackHandle
{
  void* id;

public:
  /// @private
  constexpr explicit CallbackHandle(void* id = nullptr)
    : id(id)
  {
  }
  /// Get Internal id
  constexpr void* get() const { return id; }

  /// True if has a valid id
  constexpr operator bool() const { return id != 0; }
};

/// Base class for callback wrappers
template<class Result, class... Args>
struct CallbackWrapperBase
{
  /// The wrapped std::function type
  using ValueType = std::function<Result(Args...)>;

  /// Return unwrapped value of handle.
  static const ValueType& Unwrap(void* handle)
  {
    return *static_cast<ValueType*>(handle);
  }

  /// Call
  static Result Call(void* handle, Args... args)
  {
    auto& f = Unwrap(handle);
    return f(args...);
  }

  /// Call with suffix handle.
  static Result CallSuffixed(Args... args, void* handle)
  {
    auto& f = Unwrap(handle);
    return f(args...);
  }
};

template<class F>
struct CallbackWrapper;

/**
 * @brief Wrapper [result callbacks](#result-callback).
 *
 * @tparam F the function type.
 *
 * For the simpler case, where no transformation is done on the parameters, you
 * can just pass CallOnce() or CallOnceSuffixed(). Otherwise use release().
 *
 * In all cases, use Wrap to change the callback into a void* pointer.
 */
template<class Result, class... Args>
struct CallbackWrapper<std::function<Result(Args...)>>
  : CallbackWrapperBase<Result, Args...>
{
  CallbackWrapper() = delete;

  /// The wrapped std::function type
  using ValueType = std::function<Result(Args...)>;

  /**
   * @brief Change the callback into a void* pointer.
   *
   * @param cb
   * @return void*
   */
  static ValueType* Wrap(ValueType&& cb)
  {
    return new ValueType(std::move(cb));
  }

  /// Call once and release.
  static Result CallOnce(void* handle, Args... args)
  {
    auto f = release(handle);
    return f(args...);
  }

  /// Call once and release with suffix handle.
  static Result CallOnceSuffixed(Args... args, void* handle)
  {
    auto f = release(handle);
    return f(args...);
  }

  /**
   * @brief Transfer ownership from the function and delete handle.
   *
   * @param handle the handle to be released.
   *
   * @return the callback ready to be invoked.
   */
  static ValueType release(void* handle)
  {
    if (handle == nullptr) return {};
    auto ptr = static_cast<ValueType*>(handle);
    ValueType value{std::move(*ptr)};
    delete ptr;
    return value;
  }

  /// Return unwrapped value of handle.
  static const ValueType release(CallbackHandle handle)
  {
    return release(handle.get());
  }
};

/**
 * @brief Wrapper key to value [result callbacks](#result-callback).
 *
 * @tparam KEY the key type.
 * @tparam VALUE the value type.
 * @tparam VARIANT the variant, if more than one listener type is associated.
 *
 */
template<class KEY, class VALUE, size_t VARIANT = 0>
struct KeyValueWrapper
{
  static_assert(sizeof(KEY) <= sizeof(void*));
  KeyValueWrapper() = delete;

  /// Key type
  using KeyType = KEY;

  /// Value type.
  using ValueType = VALUE;

  /**
   * @brief Change the value into a void* pointer held by key.
   *
   * @param key
   * @param value
   * @return void*
   */
  static ValueType* Wrap(KeyType key, ValueType&& value)
  {
    auto lockGuard = lock();
    return &Values().insert_or_assign(key, std::move(value)).first->second;
  }

  /// True if handle is stored.
  static bool contains(KeyType handle)
  {
    auto lockGuard = lock();
    return Values().contains(handle);
  }

  /// Return unwrapped value of handle.
  static const ValueType& at(KeyType handle)
  {
    auto lockGuard = lock();
    return Values().at(handle);
  }

  /// Return unwrapped value associated by key and remove association.
  static ValueType release(KeyType key)
  {
    auto lockGuard = lock();
    auto& values = Values();
    auto it = values.find(key);
    if (it == values.end()) return {};
    ValueType value{std::move(it->second)};
    values.erase(it);
    return value;
  }

  /**
   * Remove association.
   *
   * @param key the key associated.
   * @return true if the key was associated and was erased, false otherwise.
   */
  static bool erase(KeyType key)
  {
    auto lockGuard = lock();
    return Values().erase(key);
  }

private:
  static std::map<KeyType, ValueType>& Values()
  {
    static std::map<KeyType, ValueType> values;
    return values;
  }

  static std::lock_guard<std::mutex> lock()
  {
    static std::mutex uniqueMutex;
    return std::lock_guard{uniqueMutex};
  }
};

/// Store callbacks by key
template<class KEY, class VALUE, size_t VARIANT = 0>
struct KeyValueCallbackWrapper;

/// Store callbacks by key
template<class KEY, class Result, class... Args, size_t VARIANT>
struct KeyValueCallbackWrapper<KEY, std::function<Result(Args...)>, VARIANT>
  : CallbackWrapperBase<Result, Args...>
  , KeyValueWrapper<KEY, std::function<Result(Args...)>, VARIANT>
{
  KeyValueCallbackWrapper() = delete;

  /// Wrapped type.
  using ValueType = std::function<Result(Args...)>;
};

/**
 * @brief Stored Wrapper unique by type [result callbacks](#result-callback).
 *
 * @tparam VALUE the function type.
 */
template<class VALUE>
struct UniqueCallbackWrapper;

/**
 * @brief Stored Wrapper unique by type [result callbacks](#result-callback).
 */
template<class Result, class... Args>
struct UniqueCallbackWrapper<std::function<Result(Args...)>>
  : CallbackWrapperBase<Result, Args...>
{
  UniqueCallbackWrapper() = delete;

  /// Wrapped type.
  using ValueType = std::function<Result(Args...)>;

  /**
   * @brief Change the value into a void* pointer held uniquely by this type.
   *
   * @param value
   * @return void*
   */
  static ValueType* Wrap(ValueType&& value)
  {
    auto lockGuard = lock();
    auto& v = Value();
    v = std::move(value);
    return &v;
  }

  /// True if handle equals to wrapped value.
  static bool contains(void* handle)
  {
    auto lockGuard = lock();
    auto& v = Value();
    return bool(v) && &v == handle;
  }

  /// Return wrapped type, if handle is contained.
  static ValueType at(void* handle)
  {
    if (&get() == handle) {
      return CallbackWrapperBase<Result, Args...>::Unwrap(handle);
    }
    return {};
  }

  /// Return wrapped type, if handle is contained.
  static const ValueType& get()
  {
    auto lockGuard = lock();
    return Value();
  }

  /// Return wrapped type and erase it from store.
  static ValueType release()
  {
    auto lockGuard = lock();
    ValueType value{std::move(Value())};
    return value;
  }

  /// Return wrapped type and erase it from store.
  static ValueType release(void* handle)
  {
    SDL_assert_paranoid(&get() == handle);
    return release();
  }

  /// Erase value from store.
  static void erase()
  {
    auto lockGuard = lock();
    Value() = {};
  }

private:
  static ValueType& Value()
  {
    static ValueType value;
    return value;
  }

  static std::lock_guard<std::mutex> lock()
  {
    static std::mutex uniqueMutex;
    return std::lock_guard{uniqueMutex};
  }
};

/// @}

/**
 * @brief Optional-like shim for references
 *
 * This allows us explicitly annotate optional parameters that would otherwise
 * be references.
 *
 * @tparam T the underlying type
 *
 * @ingroup CategoriesCppSupport
 */
template<class T>
class OptionalRef
{
  T* ptr;

public:
  constexpr OptionalRef(std::nullopt_t = std::nullopt)
    : ptr(nullptr)
  {
  }

  constexpr OptionalRef(T& value)
    : ptr(&value)
  {
  }

  constexpr auto operator<=>(const OptionalRef&) const = default;

  constexpr bool has_value() const { return ptr != nullptr; }

  constexpr const T& value() const { return *ptr; }
  constexpr T& value() { return *ptr; }

  constexpr operator T*() const { return ptr; }

  constexpr bool operator==(std::nullopt_t) const { return ptr == nullptr; }

  constexpr T& operator*() { return *ptr; }
  constexpr const T& operator*() const { return *ptr; }
  constexpr T* operator->() { return ptr; }
  constexpr const T* operator->() const { return ptr; }
};

template<class T>
constexpr OptionalRef<T> fromNullable(T* ptr)
{
  if (ptr) return {*ptr};
  return std::nullopt;
}

/**
 * @defgroup CategoryOwnPtr Pointer wrapper to SDL::free()
 *
 * Wraps SDL generated pointers to automatically freeing them.
 * @{
 */

/// Calls SDL_free()
struct PtrDeleter
{
  void operator()(void* ptr) const;
};

/**
 * Handle to an owned SDL memory allocated pointer
 *
 * @cat resource
 *
 * @sa resource
 * @sa PtrBase
 * @sa RefPtr
 */
template<class T>
using OwnPtr = std::unique_ptr<T, PtrDeleter>;

/**
 * Base class for SDL memory allocated array wrap
 *
 * @tparam T the wrapped array type, without the []
 *
 * @cat resource
 */
template<class T>
class OwnArray
{
  OwnPtr<T> m_ptr;
  size_t m_size = 0;

public:
  constexpr OwnArray(std::nullptr_t = nullptr) {}

  constexpr explicit OwnArray(T* ptr, size_t size)
    : m_ptr(ptr)
    , m_size(size)
  {
  }

  constexpr explicit OwnArray(T* ptr)
    : m_ptr(ptr)
  {
    if (ptr) {
      auto endPtr = ptr;
      while (*endPtr) ++endPtr;
      m_size = endPtr - ptr;
    }
  }

  /// True if not empty
  constexpr explicit operator bool() const { return bool(m_ptr); }

  /// True if size() == 0
  constexpr bool empty() const { return !m_ptr; }

  /// Data
  constexpr T* data() { return m_ptr.get(); }

  /// Data
  constexpr const T* data() const { return m_ptr.get(); }

  /// Size
  constexpr size_t size() const { return m_size; }

  /**
   * Release control on object.
   *
   * You are responsible to call free() on the returned value, if its different
   * than nullptr.
   */
  T* release() { return m_ptr.release(); }

  /**
   * Reset object.
   *
   * You are responsible to call free() on the returned value, if its different
   * than nullptr.
   */
  void reset(T* newValue = nullptr)
  {
    m_ptr.reset(newValue);
    if (newValue) {
      auto endPtr = newValue;
      while (*endPtr) ++endPtr;
      m_size = endPtr - newValue;
    }
  }

  /**
   * Reset object.
   *
   * You are responsible to call free() on the returned value, if its different
   * than nullptr.
   */
  void reset(T* newValue, size_t size)
  {
    m_ptr.reset(newValue);
    m_size = size;
  }

  /// Access index
  constexpr T& operator[](size_t i) { return m_ptr.get()[i]; }

  /// Access index
  constexpr const T& operator[](size_t i) const { return m_ptr.get()[i]; }

  /**
   * @{
   *
   * Get iterator to first element
   */
  T* begin() { return data(); }
  const T* begin() const { return data(); }
  const T* cbegin() const { return begin(); }
  /// @}

  /**
   * @{
   *
   * Get iterator to one past end element
   */
  T* end() { return begin() + size(); }
  const T* end() const { return begin() + size(); }
  const T* cend() const { return begin() + size(); }
  /// @}

  /// Return first element.
  T& front() { return *data(); }

  /// Return last element
  T& back() { return begin()[size()]; }
};

/**
 * Handle to an owned SDL memory allocated array
 *
 * @tparam T the wrapped array type, without the []
 *
 * @cat resource
 *
 * @sa resource
 * @sa ArrayBase
 * @sa OwnArray
 * @sa RefPtr
 */
template<class T>
using RefArray = std::span<T>;

/// @}

template<class T, class BASE>
concept DerivedWrapper =
  std::derived_from<T, BASE> && sizeof(T) == sizeof(BASE);

/**
 * span-like for empty-derived structs
 *
 * This wraps around std::span that works for out derived-wrapper pattern
 * (eg, Rect, Color)
 *
 * @ingroup CategoriesCppSupport
 */
template<class T>
class SpanRef
{
  std::span<T> value;

public:
  constexpr SpanRef() = default;

  template<DerivedWrapper<T> U, size_t N>
  constexpr SpanRef(U (&other)[N])
    : value(static_cast<T*>(other), N)
  {
  }

  template<DerivedWrapper<T> U>
  constexpr SpanRef(const std::span<U>& other)
    : value(other.data(), other.size())
  {
  }

  template<std::contiguous_iterator It>
    requires DerivedWrapper<std::iter_value_t<It>, T>
  constexpr SpanRef(It first, size_t count)
    : value((T*)(&*first), count)
  {
  }

  template<std::contiguous_iterator It, std::sized_sentinel_for<It> End>
    requires DerivedWrapper<std::iter_value_t<It>, T>
  constexpr SpanRef(It first, End last)
    : value((T*)(&*first), size_t(last - first))
  {
  }
  template<std::ranges::contiguous_range R>
    requires DerivedWrapper<std::iter_value_t<std::ranges::iterator_t<R>>, T>
  constexpr SpanRef(R&& range)
    : SpanRef(std::begin(range), std::end(range))
  {
  }

  constexpr size_t size() const { return value.size(); }

  constexpr T* data() const { return value.data(); }
};

/**
 * @defgroup CategoryVersion Querying SDL Version
 *
 * Functionality to query the current SDL version, both as headers the app was
 * compiled against, and a library the app is linked to.
 *
 * @{
 */

#ifdef SDL3PP_DOC

/**
 * The current major version of SDL headers.
 *
 * If this were SDL version 3.2.1, this value would be 3.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_MAJOR_VERSION 3

/**
 * The current minor version of the SDL headers.
 *
 * If this were SDL version 3.2.1, this value would be 2.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_MINOR_VERSION 2

/**
 * The current micro (or patchlevel) version of the SDL headers.
 *
 * If this were SDL version 3.2.1, this value would be 1.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_MICRO_VERSION 23

/**
 * This macro turns the version numbers into a numeric value.
 *
 * (1,2,3) becomes 1002003.
 *
 * @param major the major version number.
 * @param minor the minorversion number.
 * @param patch the patch version number.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSIONNUM(major, minor, patch)                                    \
  ((major) * 1000000 + (minor) * 1000 + (patch))

/**
 * This macro extracts the major version from a version number
 *
 * 1002003 becomes 1.
 *
 * @param version the version number.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSIONNUM_MAJOR(version) ((version) / 1000000)

/**
 * This macro extracts the minor version from a version number
 *
 * 1002003 becomes 2.
 *
 * @param version the version number.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSIONNUM_MINOR(version) (((version) / 1000) % 1000)

/**
 * This macro extracts the micro version from a version number
 *
 * 1002003 becomes 3.
 *
 * @param version the version number.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSIONNUM_MICRO(version) ((version) % 1000)

/**
 * This is the version number macro for the current SDL version.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa GetVersion
 */
#define SDL_VERSION                                                            \
  SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION)

/**
 * This macro will evaluate to true if compiled with SDL at least X.Y.Z.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSION_ATLEAST(X, Y, Z) (SDL_VERSION >= SDL_VERSIONNUM(X, Y, Z))

#endif // SDL3PP_DOC

/**
 * Get the version of SDL that is linked against your program.
 *
 * If you are linking to SDL dynamically, then it is possible that the current
 * version will be different than the version you compiled against. This
 * function returns the current version, while SDL_VERSION is the version you
 * compiled with.
 *
 * This function may be called safely at any time, even before Init().
 *
 * @returns the version of the linked library.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetRevision
 */
inline int GetVersion() { return SDL_GetVersion(); }

/**
 * Get the code revision of SDL that is linked against your program.
 *
 * This value is the revision of the code you are linked with and may be
 * different from the code you are compiling with, which is found in the
 * constant SDL_REVISION.
 *
 * The revision is arbitrary string (a hash value) uniquely identifying the
 * exact revision of the SDL library in use, and is only useful in comparing
 * against other revisions. It is NOT an incrementing number.
 *
 * If SDL wasn't built from a git repository with the appropriate tools, this
 * will return an empty string.
 *
 * You shouldn't use this function for anything but logging it for debugging
 * purposes. The string is not intended to be reliable in any way.
 *
 * @returns an arbitrary string, uniquely identifying the exact revision of
 *          the SDL library in use.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetVersion
 */
inline const char* GetRevision() { return SDL_GetRevision(); }

/// @}

/**
 * @defgroup CategoryStrings Helpers to use C++ strings and byte arrays.
 *
 * It has StringParam to use on parameters and StringResult to have
 * a simple std::string-like interface for SDL allocated strings.
 * @{
 */

#ifndef SDL3PP_ENABLE_STRING_PARAM

#ifndef SDL3PP_DISABLE_STRING_PARAM
#define SDL3PP_ENABLE_STRING_PARAM
#endif // SDL3PP_DISABLE_STRING_PARAM

#endif // SDL3PP_ENABLE_STRING_PARAM

#ifdef SDL3PP_ENABLE_STRING_PARAM

/**
 * @brief Helpers to use C++ strings parameters
 *
 * This wraps std::string and std::string_view to the SDL's C interfaces.
 *
 * This should only be declared in [function
 * parameters](https://en.cppreference.com/w/cpp/language/expressions#Full-expressions),
 * using it otherwise is to ask for undefined behavior
 *
 * @ingroup CategoriesCppSupport
 */
class StringParam
{
  std::variant<const char*, std::string> data;

public:
  /// Default ctor
  constexpr StringParam(std::nullptr_t = nullptr)
    : data("")
  {
  }

  /**
   * Constructs from a C string.
   *
   * This assumes the parameter object lifetime is larger than this object,
   * which is always true on parameters, so it just stores the pointer, no copy
   * is done.
   *
   * @param str the string to store. This parameter must outlive this object.
   */
  constexpr StringParam(const char* str)
    : data(str ?: "")
  {
  }

  /**
   * Constructs from std::string reference.
   *
   * This assumes the parameter object lifetime is larger than this object,
   * which is always true on parameters, so it just stores the pointer to
   * internal string data, no copy is done.
   *
   * @param str the reference to a string. This parameter must outlive this
   *            object.
   */
  constexpr StringParam(const std::string& str)
    : StringParam(str.c_str())
  {
  }

  /**
   * Constructs from std::string object.
   *
   * This case we assume the ownership for the string and will properly call
   * destructor automatically.
   *
   * @param str the string to store
   */
  constexpr StringParam(std::string&& str)
    : data(std::move(str))
  {
  }

  /**
   * Constructs from std::string_view object
   *
   * String view are very usefull on C++, but they don't have the null
   * terminator expected by most string SDL APIs, so we always copy its content
   * to a stored std::string.
   *
   * @param str the string_view to store
   */
  StringParam(std::string_view str)
    : StringParam(std::string{str})
  {
  }

  StringParam(const StringParam&) = delete;
  StringParam& operator=(const StringParam&) = delete;

  /// Move ctor
  StringParam(StringParam&&) = default;

  /// Move assignment
  StringParam& operator=(StringParam&&) = default;

  /**
   * Converts to a null terminated C string.
   *
   * @return the C string representation. We guarantee it to be null terminated
   * unless the objects it was constructed from are corrupted.
   */
  constexpr const char* c_str() const
  {
    struct Visitor
    {
      const char* operator()(const char* a) const { return a; }
      const char* operator()(const std::string& s) const { return s.c_str(); }
    };
    return std::visit(Visitor{}, data);
  }

  /**
   * Converts to a null terminated C string.
   *
   * @return the C string representation. We guarantee it to be null terminated
   * unless the objects it was constructed from are corrupted.
   */
  constexpr operator const char*() const { return c_str(); }
};

#else // SDL3PP_ENABLE_STRING_PARAM

using StringParam = const char*;

#endif // SDL3PP_ENABLE_STRING_PARAM

/**
 * A simple std::string-like interface for SDL allocated strings.
 */
struct StringResult : OwnArray<char>
{

  /// Use parent ctors
  using OwnArray::OwnArray;

  /// Copy ctor
  StringResult(const StringResult& other)
    : StringResult(std::string_view(other))
  {
  }

  /// Move ctor
  constexpr StringResult(StringResult&& other)
    : OwnArray(other.release(), other.size())
  {
  }

  /// Constructs from string view
  StringResult(std::string_view other)
    : OwnArray(other.empty() ? nullptr
                             : SDL_strndup(other.data(), other.size()))
  {
  }

  /// Convert to StringParam
  constexpr operator StringParam() const { return std::string_view{*this}; }

  /// Convert to std::string_view
  constexpr operator std::string_view() const
  {
    return std::string_view{data(), size()};
  }

  /// Append string.
  StringResult& operator+=(std::string_view other)
  {
    if (empty()) {
      reset(StringResult(other).release());
    } else if (!other.empty()) {
      size_t lhsSz = size();
      size_t rhsSz = other.size();
      size_t finalSize = lhsSz + lhsSz + 1;
      auto newBuf = static_cast<char*>(SDL_realloc(data(), finalSize));
      newBuf += lhsSz;
      SDL_memcpy(newBuf, other.data(), rhsSz);
      newBuf += rhsSz;
      *newBuf = 0;
      reset(newBuf, finalSize - 1);
    }
    return *this;
  }

  /// Append char.
  StringResult& operator+=(char ch)
  {
    return (*this) += std::string_view{&ch, 1};
  }

  /// Append string.
  StringResult operator+(std::string_view other) const
  {
    StringResult t{*this};
    t += other;
    return t;
  }

  /// Append char.
  StringResult operator+(char ch) { return (*this) + std::string_view{&ch, 1}; }

  /// Convert to string.
  std::string str() const { return std::string{data(), size()}; }

  /// Convert to c-string.
  const char* c_str() const
  {
    if (empty()) return "";
    return data();
  }
};

/**
 * Source byte stream.
 *
 * Source byte stream, tipically used as source where bytes are copied from.
 */
struct SourceBytes
{
  const void* data;  ///< The data copied from
  size_t size_bytes; ///< The size in bytes

  /// Default ctor
  constexpr SourceBytes() = default;

  /// Disambiguate between multiple pointer types.
  constexpr SourceBytes(std::nullptr_t)
    : SourceBytes()
  {
  }

  /**
   * From span
   *
   * @param span the data.
   *
   */
  template<class T, size_t N>
  constexpr SourceBytes(std::span<T, N> span)
    : SourceBytes(span.data(), span.size_bytes())
  {
  }

  /**
   * From container-like
   *
   * @param data the data.
   */
  template<class T>
  constexpr SourceBytes(const T& data)
    : SourceBytes(std::span{data.data(), data.size()})
  {
  }
  /**
   * From array
   *
   * @param data the data.
   */
  template<class T, size_t N>
  constexpr SourceBytes(T (&data)[N])
    : SourceBytes(std::span<T, N>{static_cast<T*>(data), N})
  {
  }

  /**
   * From data + size in bytes.
   *
   * @param data the data.
   * @param size_bytes size in @b bytes!
   *
   */
  constexpr SourceBytes(const void* data, size_t size_bytes)
    : data(size_bytes > 0 ? data : nullptr)
    , size_bytes(data != nullptr ? size_bytes : 0)
  {
  }
};

/**
 * Target byte stream.
 *
 * Target byte stream, tipically used as target where bytes are copied to.
 */
struct TargetBytes
{
  void* data;        ///< The address to have data copied to it
  size_t size_bytes; ///< The size in bytes

  /// Default ctor
  constexpr TargetBytes() = default;

  /// Disambiguate between multiple pointer types.
  constexpr TargetBytes(std::nullptr_t)
    : TargetBytes()
  {
  }

  /// Just to have better error message
  template<class T, size_t N>
  constexpr TargetBytes(std::span<const T, N> span)
  {
    static_assert(false, "Non-const type is required");
  }

  /**
   * From span
   *
   * @param span the data.
   *
   */
  template<class T, size_t N>
  constexpr TargetBytes(std::span<T, N> span)
    : TargetBytes(span.data(), span.size_bytes())
  {
  }

  /**
   * From container-like
   *
   * @param data the data.
   */
  template<class T>
  constexpr TargetBytes(T& data)
    : TargetBytes(std::span{data.data(), data.size()})
  {
  }
  /**
   * From array
   *
   * @param data the data.
   */
  template<class T, size_t N>
  constexpr TargetBytes(T (&data)[N])
    : TargetBytes(std::span<T, N>{static_cast<T*>(data), N})
  {
  }

  /**
   * From data + size in bytes.
   *
   * @param data the data.
   * @param size_bytes size in @b bytes!
   *
   */
  constexpr TargetBytes(void* data, size_t size_bytes)
    : data(size_bytes > 0 ? data : nullptr)
    , size_bytes(data != nullptr ? size_bytes : 0)
  {
  }
};

/// @}

/**
 * @defgroup CategoryAssert Assertions
 *
 * A helpful assertion macro!
 *
 * SDL assertions operate like your usual `assert` macro, but with some added
 * features:
 *
 * - It uses a trick with the `sizeof` operator, so disabled assertions
 *   vaporize out of the compiled code, but variables only referenced in the
 *   assertion won't trigger compiler warnings about being unused.
 * - It is safe to use with a dangling-else: `if (x) SDL_assert(y); else
 *   do_something();`
 * - It works the same everywhere, instead of counting on various platforms'
 *   compiler and C runtime to behave.
 * - It provides multiple levels of assertion (SDL_assert, SDL_assert_release,
 *   SDL_assert_paranoid) instead of a single all-or-nothing option.
 * - It offers a variety of responses when an assertion fails (retry, trigger
 *   the debugger, abort the program, ignore the failure once, ignore it for
 *   the rest of the program's run).
 * - It tries to show the user a dialog by default, if possible, but the app
 *   can provide a callback to handle assertion failures however they like.
 * - It lets failed assertions be retried. Perhaps you had a network failure
 *   and just want to retry the test after plugging your network cable back
 *   in? You can.
 * - It lets the user ignore an assertion failure, if there's a harmless
 *   problem that one can continue past.
 * - It lets the user mark an assertion as ignored for the rest of the
 *   program's run; if there's a harmless problem that keeps popping up.
 * - It provides statistics and data on all failed assertions to the app.
 * - It allows the default assertion handler to be controlled with environment
 *   variables, in case an automated script needs to control it.
 * - It can be used as an aid to Clang's static analysis; it will treat SDL
 *   assertions as universally true (under the assumption that you are serious
 *   about the asserted claims and that your debug builds will detect when
 *   these claims were wrong). This can help the analyzer avoid false
 *   positives.
 *
 * To use it: compile a debug build and just sprinkle around tests to check
 * your code!
 *
 * @{
 */

#ifdef SDL3PP_DOC

/**
 * The level of assertion aggressiveness.
 *
 * This value changes depending on compiler options and other preprocessor
 * defines.
 *
 * It is currently one of the following values, but future SDL releases might
 * add more:
 *
 * - 0: All SDL assertion macros are disabled.
 * - 1: Release settings: SDL_assert disabled, SDL_assert_release enabled.
 * - 2: Debug settings: SDL_assert and SDL_assert_release enabled.
 * - 3: Paranoid settings: All SDL assertion macros enabled, including
 *   SDL_assert_paranoid.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_ASSERT_LEVEL SomeNumberBasedOnVariousFactors

/**
 * Attempt to tell an attached debugger to pause.
 *
 * This allows an app to programmatically halt ("break") the debugger as if it
 * had hit a breakpoint, allowing the developer to examine program state, etc.
 *
 * This is a macro--not a function--so that the debugger breaks on the source
 * code line that used SDL_TriggerBreakpoint and not in some random guts of
 * SDL. SDL_assert uses this macro for the same reason.
 *
 * If the program is not running under a debugger, SDL_TriggerBreakpoint will
 * likely terminate the app, possibly without warning. If the current platform
 * isn't supported, this macro is left undefined.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_TriggerBreakpoint() TriggerABreakpointInAPlatformSpecificManner

/**
 * A macro that reports the current function being compiled.
 *
 * If SDL can't figure how the compiler reports this, it will use "???".
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_FUNCTION __FUNCTION__

/**
 * A macro that reports the current file being compiled.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_FILE __FILE__

/**
 * A macro that reports the current line number of the file being compiled.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_LINE __LINE__

/**
 * A macro for wrapping code in `do {} while (0);` without compiler warnings.
 *
 * Visual Studio with really aggressive warnings enabled needs this to avoid
 * compiler complaints.
 *
 * the `do {} while (0);` trick is useful for wrapping code in a macro that
 * may or may not be a single statement, to avoid various C language
 * accidents.
 *
 * To use:
 *
 * ```c
 * do { SomethingOnce(); } while (SDL_NULL_WHILE_LOOP_CONDITION (0));
 * ```
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_NULL_WHILE_LOOP_CONDITION (0)

/**
 * The macro used when an assertion is disabled.
 *
 * This isn't for direct use by apps, but this is the code that is inserted
 * when an SDL_assert is disabled (perhaps in a release build).
 *
 * The code does nothing, but wraps `condition` in a sizeof operator, which
 * generates no code and has no side effects, but avoid compiler warnings
 * about unused variables.
 *
 * @param condition the condition to assert (but not actually run here).
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_disabled_assert(condition)                                         \
  do {                                                                         \
    (void)sizeof((condition));                                                 \
  } while (SDL_NULL_WHILE_LOOP_CONDITION)

#endif // SDL3PP_DOC

/**
 * Possible outcomes from a triggered assertion.
 *
 * When an enabled assertion triggers, it may call the assertion handler
 * (possibly one provided by the app via SetAssertionHandler), which will
 * return one of these values, possibly after asking the user.
 *
 * Then SDL will respond based on this outcome (loop around to retry the
 * condition, try to break in a debugger, kill the program, or ignore the
 * problem).
 *
 * @since This enum is available since SDL 3.2.0.
 */
using AssertState = SDL_AssertState;

constexpr AssertState ASSERTION_RETRY =
  SDL_ASSERTION_RETRY; ///< Retry the assert immediately.

constexpr AssertState ASSERTION_BREAK =
  SDL_ASSERTION_BREAK; ///< Make the debugger trigger a breakpoint.

constexpr AssertState ASSERTION_ABORT =
  SDL_ASSERTION_ABORT; ///< Terminate the program.

constexpr AssertState ASSERTION_IGNORE =
  SDL_ASSERTION_IGNORE; ///< Ignore the assert.

constexpr AssertState ASSERTION_ALWAYS_IGNORE =
  SDL_ASSERTION_ALWAYS_IGNORE; ///< Ignore the assert from now on.

/**
 * Information about an assertion failure.
 *
 * This structure is filled in with information about a triggered assertion,
 * used by the assertion handler, then added to the assertion report. This is
 * returned as a linked list from GetAssertionReport().
 *
 * @since This struct is available since SDL 3.2.0.
 */
using AssertData = SDL_AssertData;

/**
 * Never call this directly.
 *
 * Use the SDL_assert macros instead.
 *
 * @param data assert data structure.
 * @param func function name.
 * @param file file name.
 * @param line line number.
 * @returns assert state.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline AssertState ReportAssertion(AssertData* data,
                                   StringParam func,
                                   StringParam file,
                                   int line)
{
  return SDL_ReportAssertion(data, func, file, line);
}

#ifdef SDL3PP_DOC

/**
 * The macro used when an assertion triggers a breakpoint.
 *
 * This isn't for direct use by apps; use SDL_assert or SDL_TriggerBreakpoint
 * instead.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_AssertBreakpoint() SDL_TriggerBreakpoint()

/**
 * The macro used when an assertion is enabled.
 *
 * This isn't for direct use by apps, but this is the code that is inserted
 * when an SDL_assert is enabled.
 *
 * The `do {} while(0)` avoids dangling else problems:
 *
 * ```c
 * if (x) SDL_assert(y); else blah();
 * ```
 *
 * ... without the do/while, the "else" could attach to this macro's "if". We
 * try to handle just the minimum we need here in a macro...the loop, the
 * static vars, and break points. The heavy lifting is handled in
 * ReportAssertion().
 *
 * @param condition the condition to assert.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_enabled_assert(condition)                                          \
  do {                                                                         \
    while (!(condition)) {                                                     \
      static struct SDL_AssertData sdl_assert_data = {                         \
        0, 0, #condition, 0, 0, 0, 0};                                         \
      const SDL_AssertState sdl_assert_state = SDL_ReportAssertion(            \
        &sdl_assert_data, SDL_FUNCTION, SDL_FILE, SDL_LINE);                   \
      if (sdl_assert_state == SDL_ASSERTION_RETRY) {                           \
        continue; /* go again. */                                              \
      } else if (sdl_assert_state == SDL_ASSERTION_BREAK) {                    \
        SDL_AssertBreakpoint();                                                \
      }                                                                        \
      break; /* not retrying. */                                               \
    }                                                                          \
  } while (SDL_NULL_WHILE_LOOP_CONDITION)

/**
 * An assertion test that is normally performed only in debug builds.
 *
 * This macro is enabled when the SDL_ASSERT_LEVEL is >= 2, otherwise it is
 * disabled. This is meant to only do these tests in debug builds, so they can
 * tend to be more expensive, and they are meant to bring everything to a halt
 * when they fail, with the programmer there to assess the problem.
 *
 * In short: you can sprinkle these around liberally and assume they will
 * evaporate out of the build when building for end-users.
 *
 * When assertions are disabled, this wraps `condition` in a `sizeof`
 * operator, which means any function calls and side effects will not run, but
 * the compiler will not complain about any otherwise-unused variables that
 * are only referenced in the assertion.
 *
 * One can set the environment variable "SDL_ASSERT" to one of several strings
 * ("abort", "break", "retry", "ignore", "always_ignore") to force a default
 * behavior, which may be desirable for automation purposes. If your platform
 * requires GUI interfaces to happen on the main thread but you're debugging
 * an assertion in a background thread, it might be desirable to set this to
 * "break" so that your debugger takes control as soon as assert is triggered,
 * instead of risking a bad UI interaction (deadlock, etc) in the application.
 *
 * @param condition boolean value to test.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_assert(condition)                                                  \
  if (assertion_enabled && (condition)) { trigger_assertion; }

/**
 * An assertion test that is performed even in release builds.
 *
 * This macro is enabled when the SDL_ASSERT_LEVEL is >= 1, otherwise it is
 * disabled. This is meant to be for tests that are cheap to make and
 * extremely unlikely to fail; generally it is frowned upon to have an
 * assertion failure in a release build, so these assertions generally need to
 * be of more than life-and-death importance if there's a chance they might
 * trigger. You should almost always consider handling these cases more
 * gracefully than an assert allows.
 *
 * When assertions are disabled, this wraps `condition` in a `sizeof`
 * operator, which means any function calls and side effects will not run, but
 * the compiler will not complain about any otherwise-unused variables that
 * are only referenced in the assertion.
 *
 * One can set the environment variable "SDL_ASSERT" to one of several strings
 * ("abort", "break", "retry", "ignore", "always_ignore") to force a default
 * behavior, which may be desirable for automation purposes. If your platform
 * requires GUI interfaces to happen on the main thread but you're debugging
 * an assertion in a background thread, it might be desirable to set this to
 * "break" so that your debugger takes control as soon as assert is triggered,
 * instead of risking a bad UI interaction (deadlock, etc) in the application.
 * *
 *
 * @param condition boolean value to test.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_assert_release(condition) SDL_disabled_assert(condition)

/**
 * An assertion test that is performed only when built with paranoid settings.
 *
 * This macro is enabled when the SDL_ASSERT_LEVEL is >= 3, otherwise it is
 * disabled. This is a higher level than both release and debug, so these
 * tests are meant to be expensive and only run when specifically looking for
 * extremely unexpected failure cases in a special build.
 *
 * When assertions are disabled, this wraps `condition` in a `sizeof`
 * operator, which means any function calls and side effects will not run, but
 * the compiler will not complain about any otherwise-unused variables that
 * are only referenced in the assertion.
 *
 * One can set the environment variable "SDL_ASSERT" to one of several strings
 * ("abort", "break", "retry", "ignore", "always_ignore") to force a default
 * behavior, which may be desirable for automation purposes. If your platform
 * requires GUI interfaces to happen on the main thread but you're debugging
 * an assertion in a background thread, it might be desirable to set this to
 * "break" so that your debugger takes control as soon as assert is triggered,
 * instead of risking a bad UI interaction (deadlock, etc) in the application.
 *
 * @param condition boolean value to test.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_assert_paranoid(condition) SDL_disabled_assert(condition)

/**
 * An assertion test that is always performed.
 *
 * This macro is always enabled no matter what SDL_ASSERT_LEVEL is set to. You
 * almost never want to use this, as it could trigger on an end-user's system,
 * crashing your program.
 *
 * One can set the environment variable "SDL_ASSERT" to one of several strings
 * ("abort", "break", "retry", "ignore", "always_ignore") to force a default
 * behavior, which may be desirable for automation purposes. If your platform
 * requires GUI interfaces to happen on the main thread but you're debugging
 * an assertion in a background thread, it might be desirable to set this to
 * "break" so that your debugger takes control as soon as assert is triggered,
 * instead of risking a bad UI interaction (deadlock, etc) in the application.
 *
 * @param condition boolean value to test.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_assert_always(condition) SDL_enabled_assert(condition)

#endif // SDL3PP_DOC

/**
 * A @ref callback that fires when an SDL assertion fails.
 *
 * @param data a pointer to the AssertData structure corresponding to the
 *             current assertion.
 * @param userdata what was passed as `userdata` to SetAssertionHandler().
 * @returns an AssertState value indicating how to handle the failure.
 *
 * @threadsafety This callback may be called from any thread that triggers an
 *               assert at any time.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AssertionHandler = SDL_AssertionHandler;

/**
 * A callback that fires when an SDL assertion fails.
 *
 * @param data a pointer to the AssertData structure corresponding to the
 *             current assertion.
 * @returns an AssertState value indicating how to handle the failure.
 *
 * @threadsafety This callback may be called from any thread that triggers an
 *               assert at any time.
 *
 * @since This datatype is available since SDL 3.2.0.
 * @sa AssertionHandler
 */
using AssertionHandlerCB =
  std::function<SDL_AssertState(const SDL_AssertData*)>;

/**
 * Set an application-defined assertion handler.
 *
 * This function allows an application to show its own assertion UI and/or
 * force the response to an assertion failure. If the application doesn't
 * provide this, SDL will try to do the right thing, popping up a
 * system-specific GUI dialog, and probably minimizing any fullscreen windows.
 *
 * This callback may fire from any thread, but it runs wrapped in a mutex, so
 * it will only fire from one thread at a time.
 *
 * This callback is NOT reset to SDL's internal handler upon Quit()!
 *
 * @param handler the AssertionHandler function to call when an assertion
 *                fails or nullptr for the default handler.
 * @param userdata a pointer that is passed to `handler`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAssertionHandler
 */
inline void SetAssertionHandler(AssertionHandler handler, void* userdata)
{
  UniqueCallbackWrapper<AssertionHandlerCB>::erase();
  return SDL_SetAssertionHandler(handler, userdata);
}

/**
 * Set an application-defined assertion handler.
 *
 * This function allows an application to show its own assertion UI and/or
 * force the response to an assertion failure. If the application doesn't
 * provide this, SDL will try to do the right thing, popping up a
 * system-specific GUI dialog, and probably minimizing any fullscreen windows.
 *
 * This callback may fire from any thread, but it runs wrapped in a mutex, so
 * it will only fire from one thread at a time.
 *
 * This callback is NOT reset to SDL's internal handler upon Quit()!
 *
 * @param handler the SDL_AssertionHandler function to call when an assertion
 *                fails.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAssertionHandler
 */
inline void SetAssertionHandler(AssertionHandlerCB handler)
{
  using Wrapper = UniqueCallbackWrapper<AssertionHandlerCB>;
  SetAssertionHandler(&Wrapper::CallSuffixed,
                      Wrapper::Wrap(std::move(handler)));
}
/**
 * Get the default assertion handler.
 *
 * This returns the function pointer that is called by default when an
 * assertion is triggered. This is an internal function provided by SDL, that
 * is used for assertions when SetAssertionHandler() hasn't been used to
 * provide a different function.
 *
 * @returns the default AssertionHandler that is called when an assert triggers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAssertionHandler
 */
inline AssertionHandler GetDefaultAssertionHandler()
{
  return SDL_GetDefaultAssertionHandler();
}

/**
 * Get the current assertion handler.
 *
 * This returns the function pointer that is called when an assertion is
 * triggered. This is either the value last passed to SetAssertionHandler(), or
 * if no application-specified function is set, is equivalent to calling
 * GetDefaultAssertionHandler().
 *
 * The parameter `puserdata` is a pointer to a void*, which will store the
 * "userdata" pointer that was passed to SetAssertionHandler(). This value
 * will always be nullptr for the default handler. If you don't care about this
 * data, it is safe to pass a nullptr pointer to this function to ignore it.
 *
 * @param puserdata pointer which is filled with the "userdata" pointer that
 *                  was passed to SetAssertionHandler().
 * @returns the AssertionHandler that is called when an assert triggers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAssertionHandler
 */
inline AssertionHandler GetAssertionHandler(void** puserdata)
{
  return SDL_GetAssertionHandler(puserdata);
}

/**
 * Get the current assertion handler.
 *
 * This returns the function pointer that is called when an assertion is
 * triggered. This is either the value last passed to
 * SDL_SetAssertionHandler(), or if no application-specified function is set,
 * is equivalent to calling SDL_GetDefaultAssertionHandler().
 *
 * The parameter `puserdata` is a pointer to a void*, which will store the
 * "userdata" pointer that was passed to SDL_SetAssertionHandler(). This value
 * will always be nullptr for the default handler. If you don't care about this
 * data, it is safe to pass a nullptr pointer to this function to ignore it.
 *
 * @returns the AssertionHandlerCB that is called when an assert triggers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAssertionHandler
 */
inline AssertionHandlerCB GetAssertionHandler()
{
  using Wrapper = UniqueCallbackWrapper<AssertionHandlerCB>;
  void* userdata = nullptr;
  auto cb = GetAssertionHandler(&userdata);
  if (Wrapper::contains(userdata)) return Wrapper::Unwrap(userdata);
  return [cb, userdata](const AssertData* data) { return cb(data, userdata); };
}

/**
 * Get a list of all assertion failures.
 *
 * This function gets all assertions triggered since the last call to
 * ResetAssertionReport(), or the start of the program.
 *
 * The proper way to examine this data looks something like this:
 *
 * ```cpp
 * const AssertData *item = GetAssertionReport();
 * while (item) {
 *    printf("'%s', %s (%s:%d), triggered %u times, always ignore: %s.\@n",
 *           item->condition, item->function, item->filename,
 *           item->linenum, item->trigger_count,
 *           item->always_ignore ? "yes" : "no");
 *    item = item->next;
 * }
 * ```
 *
 * @returns a list of all failed assertions or nullptr if the list is empty.
 * This memory should not be modified or freed by the application. This pointer
 * remains valid until the next call to Quit() or ResetAssertionReport().
 *
 * @threadsafety This function is not thread safe. Other threads calling
 *               ResetAssertionReport() simultaneously, may render the
 *               returned pointer invalid.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ResetAssertionReport
 */
inline const AssertData* GetAssertionReport()
{
  return SDL_GetAssertionReport();
}

/**
 * Clear the list of all assertion failures.
 *
 * This function will clear the list of all assertions triggered up to that
 * point. Immediately following this call, GetAssertionReport will return
 * no items. In addition, any previously-triggered assertions will be reset to
 * a trigger_count of zero, and their always_ignore state will be false.
 *
 * @threadsafety This function is not thread safe. Other threads triggering an
 *               assertion, or simultaneously calling this function may cause
 *               memory leaks or crashes.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAssertionReport
 */
inline void ResetAssertionReport() { SDL_ResetAssertionReport(); }

/// @}

/**
 * @defgroup CategoryError Error Handling
 *
 * Simple error message routines for SDL.
 *
 * Most apps will interface with these APIs in exactly one function: when
 * almost any SDL function call reports failure, you can get a human-readable
 * string of the problem from GetError().
 *
 * These strings are maintained per-thread, and apps are welcome to set their
 * own errors, which is popular when building libraries on top of SDL for
 * other apps to consume. These strings are set by calling SetError().
 *
 * A common usage pattern is to have a function that returns true for success
 * and false for failure, and do this when something fails:
 *
 * ```cpp
 * if (something_went_wrong) {
 *    return SDL::SetError("The thing broke in this specific way: {}", errcode);
 * }
 * ```
 *
 * It's also common to just return `false` in this case if the failing thing
 * is known to call SetError(), so errors simply propagate through.
 *
 * @{
 */

/**
 * Set the SDL error message for the current thread.
 *
 * Calling this function will replace any previous error message that was set.
 *
 * This function always returns false, since SDL frequently uses false to
 * signify a failing result, leading to this idiom:
 *
 * ```c
 * if (error_code) {
 *     return SDL::SetError("This operation has failed: {}", error_code);
 * }
 * ```
 *
 * @param message the error message
 * @returns false.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ClearError
 * @sa GetError
 * @sa SetError
 */
inline bool SetErrorUnformatted(StringParam message)
{
  return SDL_SetError("%s", static_cast<const char*>(message));
}

/**
 * Set the SDL error message for the current thread.
 *
 * Calling this function will replace any previous error message that was set.
 *
 * This function always returns false, since SDL frequently uses false to
 * signify a failing result, leading to this idiom:
 *
 * ```c
 * if (error_code) {
 *     return SetError("This operation has failed: {}", error_code);
 * }
 * ```
 *
 * @tparam ARGS the formatting parameters
 * @param fmt a
 * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
 * style message format string
 * @param args additional parameters matching the `{}` tokens in the format
 * string, if any.
 * @returns false.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat formatted-string
 *
 * @sa formatted-string
 * @sa ClearError
 * @sa GetError
 * @sa SetError
 * @return false
 */
template<class... ARGS>
inline bool SetError(std::string_view fmt, ARGS... args)
{
  return SetError(
    std::vformat(fmt, std::make_format_args(std::forward<ARGS>(args)...)));
}

/**
 * Set an error indicating that memory allocation failed.
 *
 * This function does not do any memory allocation.
 *
 * @returns false.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool OutOfMemory() { return SDL_OutOfMemory(); }

/**
 * Retrieve a message about the last error that occurred on the current
 * thread.
 *
 * It is possible for multiple errors to occur before calling GetError().
 * Only the last error is returned.
 *
 * The message is only applicable when an SDL function has signaled an error.
 * You must check the return values of SDL function calls to determine when to
 * appropriately call GetError(). You should *not* use the results of
 * GetError() to decide if an error has occurred! Sometimes SDL will set
 * an error string even when reporting success.
 *
 * SDL will *not* clear the error string for successful API calls. You *must*
 * check return values for failure cases before you can assume the error
 * string applies.
 *
 * Error strings are set per-thread, so an error set in a different thread
 * will not interfere with the current thread's operation.
 *
 * The returned value is a thread-local string which will remain valid until
 * the current thread's error string is changed. The caller should make a copy
 * if the value is needed after the next SDL API call.
 *
 * @returns a message with information about the specific error that occurred,
 *          or an empty string if there hasn't been an error message set since
 *          the last call to ClearError().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ClearError
 * @sa SetErrorUnformatted
 */
inline const char* GetError() { return SDL_GetError(); }

/**
 * An exception that returns GetError()
 *
 */
class Error : public std::exception
{
  std::string m_message;

public:
  /**
   * Default ctor.
   */
  Error()
    : m_message(SDL_GetError())
  {
  }

  /**
   * Constructs from string
   */
  Error(std::string message)
    : m_message(std::move(message))
  {
  }

  /**
   * Returns the explanatory string.
   */
  constexpr const char* what() const noexcept { return m_message.c_str(); }

  /**
   * Returns the explanatory string.
   */
  constexpr const std::string& str() const noexcept { return m_message; }
};

/**
 * Check and throw if returned value from SDL is an error.
 *
 * This should be called only for things that may set SetError(). If the
 * parameter is false it will throw Error.
 *
 * @param result the result returned
 */
constexpr void CheckError(bool result)
{
  if (!result) throw Error();
}

/**
 * Check and throw if returned value from SDL is an error.
 *
 * This should be called only for things that may set SetError(). If the
 * parameter is false it will throw Error.
 *
 * @param result the result returned
 */
template<class T>
constexpr T CheckError(T result)
{
  if (!result) throw Error();
  return result;
}

/**
 * Check and throw if returned value from SDL is an error.
 *
 * This should be called only for things that may set SetError(). If the result
 * parameter is equals to invalidValue it will throw Error.
 *
 * @param result       the result returned
 * @param invalidValue the value that if equal to result indicates this is
 *                     invalid.
 */
template<class T>
constexpr T CheckError(T result, T invalidValue)
{
  if (result == invalidValue) throw Error();
  return result;
}

/**
 * Clear any previous error message for this thread.
 *
 * @returns true.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetError
 * @sa SetErrorUnformatted
 */
inline bool ClearError() { return SDL_ClearError(); }

#ifdef SDL3PP_DOC

/**
 * A macro to standardize error reporting on unsupported operations.
 *
 * This simply calls SetError() with a standardized error string, for
 * convenience, consistency, and clarity.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_Unsupported() SDL_SetError("That operation is not supported")

/**
 * A macro to standardize error reporting on unsupported operations.
 *
 * This simply calls SetError() with a standardized error string, for
 * convenience, consistency, and clarity.
 *
 * A common usage pattern inside SDL is this:
 *
 * ```c
 * bool MyFunction(const char *str) {
 *     if (!str) {
 *         return SDL_InvalidParamError("str");  // returns false.
 *     }
 *     DoSomething(str);
 *     return true;
 * }
 * ```
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_InvalidParamError(param)                                           \
  SDL_SetError("Parameter '%s' is invalid", (param))

#endif // SDL3PP_DOC

/// @}

/**
 * @defgroup CategoryHints Configuration Variables
 *
 * This file contains functions to set and get configuration hints, as well as
 * listing each of them alphabetically.
 *
 * The convention for naming hints is SDL_HINT_X, where "SDL_X" is the
 * environment variable that can be used to override the default.
 *
 * In general these hints are just that - they may or may not be supported or
 * applicable on any given platform, but they provide a way for an application
 * or user to give the library a hint as to how they would like the library to
 * work.
 *
 * @{
 */

#ifdef SDL3PP_DOC

/**
 * Specify the behavior of Alt+Tab while the keyboard is grabbed.
 *
 * By default, SDL emulates Alt+Tab functionality while the keyboard is
 * grabbed and your window is full-screen. This prevents the user from getting
 * stuck in your application if you've enabled keyboard grab.
 *
 * The variable can be set to the following values:
 *
 * - "0": SDL will not handle Alt+Tab. Your application is responsible for
 *   handling Alt+Tab while the keyboard is grabbed.
 * - "1": SDL will minimize your window when Alt+Tab is pressed (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED "SDL_ALLOW_ALT_TAB_WHILE_GRABBED"

/**
 * A variable to control whether the SDL activity is allowed to be re-created.
 *
 * If this hint is true, the activity can be recreated on demand by the OS,
 * and Java static data and C++ static data remain with their current values.
 * If this hint is false, then SDL will call exit() when you return from your
 * main function and the application will be terminated and then started fresh
 * each time.
 *
 * The variable can be set to the following values:
 *
 * - "0": The application starts fresh at each launch. (default)
 * - "1": The application activity can be recreated by the OS.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_ANDROID_ALLOW_RECREATE_ACTIVITY                               \
  "SDL_ANDROID_ALLOW_RECREATE_ACTIVITY"

/**
 * A variable to control whether the event loop will block itself when the app
 * is paused.
 *
 * The variable can be set to the following values:
 *
 * - "0": Non blocking.
 * - "1": Blocking. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_ANDROID_BLOCK_ON_PAUSE "SDL_ANDROID_BLOCK_ON_PAUSE"

/**
 * A variable to control whether low latency audio should be enabled.
 *
 * Some devices have poor quality output when this is enabled, but this is
 * usually an improvement in audio latency.
 *
 * The variable can be set to the following values:
 *
 * - "0": Low latency audio is not enabled.
 * - "1": Low latency audio is enabled. (default)
 *
 * This hint should be set before SDL audio is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_ANDROID_LOW_LATENCY_AUDIO "SDL_ANDROID_LOW_LATENCY_AUDIO"

/**
 * A variable to control whether we trap the Android back button to handle it
 * manually.
 *
 * This is necessary for the right mouse button to work on some Android
 * devices, or to be able to trap the back button for use in your code
 * reliably. If this hint is true, the back button will show up as an
 * SDL_EVENT_KEY_DOWN / SDL_EVENT_KEY_UP pair with a keycode of
 * SDL_SCANCODE_AC_BACK.
 *
 * The variable can be set to the following values:
 *
 * - "0": Back button will be handled as usual for system. (default)
 * - "1": Back button will be trapped, allowing you to handle the key press
 *   manually. (This will also let right mouse click work on systems where the
 *   right mouse button functions as back.)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_ANDROID_TRAP_BACK_BUTTON "SDL_ANDROID_TRAP_BACK_BUTTON"

/**
 * A variable setting the app ID string.
 *
 * This string is used by desktop compositors to identify and group windows
 * together, as well as match applications with associated desktop settings
 * and icons.
 *
 * This will override prop::appMetaData.IDENTIFIER_STRING, if set by the
 * application.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_APP_ID "SDL_APP_ID"

/**
 * A variable setting the application name.
 *
 * This hint lets you specify the application name sent to the OS when
 * required. For example, this will often appear in volume control applets for
 * audio streams, and in lists of applications which are inhibiting the
 * screensaver. You should use a string that describes your program ("My Game
 * 2: The Revenge")
 *
 * This will override prop::appMetaData.NAME_STRING, if set by the
 * application.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_APP_NAME "SDL_APP_NAME"

/**
 * A variable controlling whether controllers used with the Apple TV generate
 * UI events.
 *
 * When UI events are generated by controller input, the app will be
 * backgrounded when the Apple TV remote's menu button is pressed, and when
 * the pause or B buttons on gamepads are pressed.
 *
 * More information about properly making use of controllers for the Apple TV
 * can be found here:
 * https://developer.apple.com/tvos/human-interface-guidelines/remote-and-controllers/
 *
 * The variable can be set to the following values:
 *
 * - "0": Controller input does not generate UI events. (default)
 * - "1": Controller input generates UI events.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_APPLE_TV_CONTROLLER_UI_EVENTS                                 \
  "SDL_APPLE_TV_CONTROLLER_UI_EVENTS"

/**
 * A variable controlling whether the Apple TV remote's joystick axes will
 * automatically match the rotation of the remote.
 *
 * The variable can be set to the following values:
 *
 * - "0": Remote orientation does not affect joystick axes. (default)
 * - "1": Joystick axes are based on the orientation of the remote.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_APPLE_TV_REMOTE_ALLOW_ROTATION                                \
  "SDL_APPLE_TV_REMOTE_ALLOW_ROTATION"

/**
 * Specify the default ALSA audio device name.
 *
 * This variable is a specific audio device to open when the "default" audio
 * device is used.
 *
 * This hint will be ignored when opening the default playback device if
 * SDL_HINT_AUDIO_ALSA_DEFAULT_PLAYBACK_DEVICE is set, or when opening the
 * default recording device if SDL_HINT_AUDIO_ALSA_DEFAULT_RECORDING_DEVICE is
 * set.
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 *
 * @sa SDL_HINT_AUDIO_ALSA_DEFAULT_PLAYBACK_DEVICE
 * @sa SDL_HINT_AUDIO_ALSA_DEFAULT_RECORDING_DEVICE
 */
#define SDL_HINT_AUDIO_ALSA_DEFAULT_DEVICE "SDL_AUDIO_ALSA_DEFAULT_DEVICE"

/**
 * Specify the default ALSA audio playback device name.
 *
 * This variable is a specific audio device to open for playback, when the
 * "default" audio device is used.
 *
 * If this hint isn't set, SDL will check SDL_HINT_AUDIO_ALSA_DEFAULT_DEVICE
 * before choosing a reasonable default.
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 *
 * @sa SDL_HINT_AUDIO_ALSA_DEFAULT_RECORDING_DEVICE
 * @sa SDL_HINT_AUDIO_ALSA_DEFAULT_DEVICE
 */
#define SDL_HINT_AUDIO_ALSA_DEFAULT_PLAYBACK_DEVICE                            \
  "SDL_AUDIO_ALSA_DEFAULT_PLAYBACK_DEVICE"

/**
 * Specify the default ALSA audio recording device name.
 *
 * This variable is a specific audio device to open for recording, when the
 * "default" audio device is used.
 *
 * If this hint isn't set, SDL will check SDL_HINT_AUDIO_ALSA_DEFAULT_DEVICE
 * before choosing a reasonable default.
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 *
 * @sa SDL_HINT_AUDIO_ALSA_DEFAULT_PLAYBACK_DEVICE
 * @sa SDL_HINT_AUDIO_ALSA_DEFAULT_DEVICE
 */
#define SDL_HINT_AUDIO_ALSA_DEFAULT_RECORDING_DEVICE                           \
  "SDL_AUDIO_ALSA_DEFAULT_RECORDING_DEVICE"

/**
 * A variable controlling the audio category on iOS and macOS.
 *
 * The variable can be set to the following values:
 *
 * - "ambient": Use the AVAudioSessionCategoryAmbient audio category, will be
 *   muted by the phone mute switch (default)
 * - "playback": Use the AVAudioSessionCategoryPlayback category.
 *
 * For more information, see Apple's documentation:
 * https://developer.apple.com/library/content/documentation/Audio/Conceptual/AudioSessionProgrammingGuide/AudioSessionCategoriesandModes/AudioSessionCategoriesandModes.html
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_CATEGORY "SDL_AUDIO_CATEGORY"

/**
 * A variable controlling the default audio channel count.
 *
 * If the application doesn't specify the audio channel count when opening the
 * device, this hint can be used to specify a default channel count that will
 * be used. This defaults to "1" for recording and "2" for playback devices.
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_CHANNELS "SDL_AUDIO_CHANNELS"

/**
 * Specify an application icon name for an audio device.
 *
 * Some audio backends (such as Pulseaudio and Pipewire) allow you to set an
 * XDG icon name for your application. Among other things, this icon might
 * show up in a system control panel that lets the user adjust the volume on
 * specific audio streams instead of using one giant master volume slider.
 * Note that this is unrelated to the icon used by the windowing system, which
 * may be set with SDL_SetWindowIcon (or via desktop file on Wayland).
 *
 * Setting this to "" or leaving it unset will have SDL use a reasonable
 * default, "applications-games", which is likely to be installed. See
 * https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html
 * and
 * https://specifications.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
 * for the relevant XDG icon specs.
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_DEVICE_APP_ICON_NAME "SDL_AUDIO_DEVICE_APP_ICON_NAME"

/**
 * A variable controlling device buffer size.
 *
 * This hint is an integer > 0, that represents the size of the device's
 * buffer in sample frames (stereo audio data in 16-bit format is 4 bytes per
 * sample frame, for example).
 *
 * SDL3 generally decides this value on behalf of the app, but if for some
 * reason the app needs to dictate this (because they want either lower
 * latency or higher throughput AND ARE WILLING TO DEAL WITH what that might
 * require of the app), they can specify it.
 *
 * SDL will try to accommodate this value, but there is no promise you'll get
 * the buffer size requested. Many platforms won't honor this request at all,
 * or might adjust it.
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES "SDL_AUDIO_DEVICE_SAMPLE_FRAMES"

/**
 * Specify an audio stream name for an audio device.
 *
 * Some audio backends (such as PulseAudio) allow you to describe your audio
 * stream. Among other things, this description might show up in a system
 * control panel that lets the user adjust the volume on specific audio
 * streams instead of using one giant master volume slider.
 *
 * This hints lets you transmit that information to the OS. The contents of
 * this hint are used while opening an audio device. You should use a string
 * that describes your what your program is playing ("audio stream" is
 * probably sufficient in many cases, but this could be useful for something
 * like "team chat" if you have a headset playing VoIP audio separately).
 *
 * Setting this to "" or leaving it unset will have SDL use a reasonable
 * default: "audio stream" or something similar.
 *
 * Note that while this talks about audio streams, this is an OS-level
 * concept, so it applies to a physical audio device in this case, and not an
 * SDL_AudioStream, nor an SDL logical audio device.
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_DEVICE_STREAM_NAME "SDL_AUDIO_DEVICE_STREAM_NAME"

/**
 * Specify an application role for an audio device.
 *
 * Some audio backends (such as Pipewire) allow you to describe the role of
 * your audio stream. Among other things, this description might show up in a
 * system control panel or software for displaying and manipulating media
 * playback/recording graphs.
 *
 * This hints lets you transmit that information to the OS. The contents of
 * this hint are used while opening an audio device. You should use a string
 * that describes your what your program is playing (Game, Music, Movie,
 * etc...).
 *
 * Setting this to "" or leaving it unset will have SDL use a reasonable
 * default: "Game" or something similar.
 *
 * Note that while this talks about audio streams, this is an OS-level
 * concept, so it applies to a physical audio device in this case, and not an
 * SDL_AudioStream, nor an SDL logical audio device.
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_DEVICE_STREAM_ROLE "SDL_AUDIO_DEVICE_STREAM_ROLE"

/**
 * Specify the input file when recording audio using the disk audio driver.
 *
 * This defaults to "sdlaudio-in.raw"
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_DISK_INPUT_FILE "SDL_AUDIO_DISK_INPUT_FILE"

/**
 * Specify the output file when playing audio using the disk audio driver.
 *
 * This defaults to "sdlaudio.raw"
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_DISK_OUTPUT_FILE "SDL_AUDIO_DISK_OUTPUT_FILE"

/**
 * A variable controlling the audio rate when using the disk audio driver.
 *
 * The disk audio driver normally simulates real-time for the audio rate that
 * was specified, but you can use this variable to adjust this rate higher or
 * lower down to 0. The default value is "1.0".
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_DISK_TIMESCALE "SDL_AUDIO_DISK_TIMESCALE"

/**
 * A variable that specifies an audio backend to use.
 *
 * By default, SDL will try all available audio backends in a reasonable order
 * until it finds one that can work, but this hint allows the app or user to
 * force a specific driver, such as "pipewire" if, say, you are on PulseAudio
 * but want to try talking to the lower level instead.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_DRIVER "SDL_AUDIO_DRIVER"

/**
 * A variable controlling the audio rate when using the dummy audio driver.
 *
 * The dummy audio driver normally simulates real-time for the audio rate that
 * was specified, but you can use this variable to adjust this rate higher or
 * lower down to 0. The default value is "1.0".
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_DUMMY_TIMESCALE "SDL_AUDIO_DUMMY_TIMESCALE"

/**
 * A variable controlling the default audio format.
 *
 * If the application doesn't specify the audio format when opening the
 * device, this hint can be used to specify a default format that will be
 * used.
 *
 * The variable can be set to the following values:
 *
 * - "U8": Unsigned 8-bit audio
 * - "S8": Signed 8-bit audio
 * - "S16LE": Signed 16-bit little-endian audio
 * - "S16BE": Signed 16-bit big-endian audio
 * - "S16": Signed 16-bit native-endian audio (default)
 * - "S32LE": Signed 32-bit little-endian audio
 * - "S32BE": Signed 32-bit big-endian audio
 * - "S32": Signed 32-bit native-endian audio
 * - "F32LE": Floating point little-endian audio
 * - "F32BE": Floating point big-endian audio
 * - "F32": Floating point native-endian audio
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_FORMAT "SDL_AUDIO_FORMAT"

/**
 * A variable controlling the default audio frequency.
 *
 * If the application doesn't specify the audio frequency when opening the
 * device, this hint can be used to specify a default frequency that will be
 * used. This defaults to "44100".
 *
 * This hint should be set before an audio device is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_FREQUENCY "SDL_AUDIO_FREQUENCY"

/**
 * A variable that causes SDL to not ignore audio "monitors".
 *
 * This is currently only used by the PulseAudio driver.
 *
 * By default, SDL ignores audio devices that aren't associated with physical
 * hardware. Changing this hint to "1" will expose anything SDL sees that
 * appears to be an audio source or sink. This will add "devices" to the list
 * that the user probably doesn't want or need, but it can be useful in
 * scenarios where you want to hook up SDL to some sort of virtual device,
 * etc.
 *
 * The variable can be set to the following values:
 *
 * - "0": Audio monitor devices will be ignored. (default)
 * - "1": Audio monitor devices will show up in the device list.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUDIO_INCLUDE_MONITORS "SDL_AUDIO_INCLUDE_MONITORS"

/**
 * A variable controlling whether SDL updates joystick state when getting
 * input events.
 *
 * The variable can be set to the following values:
 *
 * - "0": You'll call SDL_UpdateJoysticks() manually.
 * - "1": SDL will automatically call SDL_UpdateJoysticks(). (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUTO_UPDATE_JOYSTICKS "SDL_AUTO_UPDATE_JOYSTICKS"

/**
 * A variable controlling whether SDL updates sensor state when getting input
 * events.
 *
 * The variable can be set to the following values:
 *
 * - "0": You'll call SDL_UpdateSensors() manually.
 * - "1": SDL will automatically call SDL_UpdateSensors(). (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_AUTO_UPDATE_SENSORS "SDL_AUTO_UPDATE_SENSORS"

/**
 * Prevent SDL from using version 4 of the bitmap header when saving BMPs.
 *
 * The bitmap header version 4 is required for proper alpha channel support
 * and SDL will use it when required. Should this not be desired, this hint
 * can force the use of the 40 byte header version which is supported
 * everywhere.
 *
 * The variable can be set to the following values:
 *
 * - "0": Surfaces with a colorkey or an alpha channel are saved to a 32-bit
 *   BMP file with an alpha mask. SDL will use the bitmap header version 4 and
 *   set the alpha mask accordingly. (default)
 * - "1": Surfaces with a colorkey or an alpha channel are saved to a 32-bit
 *   BMP file without an alpha mask. The alpha channel data will be in the
 *   file, but applications are going to ignore it.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_BMP_SAVE_LEGACY_FORMAT "SDL_BMP_SAVE_LEGACY_FORMAT"

/**
 * A variable that decides what camera backend to use.
 *
 * By default, SDL will try all available camera backends in a reasonable
 * order until it finds one that can work, but this hint allows the app or
 * user to force a specific target, such as "directshow" if, say, you are on
 * Windows Media Foundations but want to try DirectShow instead.
 *
 * The default value is unset, in which case SDL will try to figure out the
 * best camera backend on your behalf. This hint needs to be set before
 * Init() is called to be useful.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_CAMERA_DRIVER "SDL_CAMERA_DRIVER"

/**
 * A variable that limits what CPU features are available.
 *
 * By default, SDL marks all features the current CPU supports as available.
 * This hint allows the enabled features to be limited to a subset.
 *
 * When the hint is unset, or empty, SDL will enable all detected CPU
 * features.
 *
 * The variable can be set to a comma separated list containing the following
 * items:
 *
 * - "all"
 * - "altivec"
 * - "sse"
 * - "sse2"
 * - "sse3"
 * - "sse41"
 * - "sse42"
 * - "avx"
 * - "avx2"
 * - "avx512f"
 * - "arm-simd"
 * - "neon"
 * - "lsx"
 * - "lasx"
 *
 * The items can be prefixed by '+'/'-' to add/remove features.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_CPU_FEATURE_MASK "SDL_CPU_FEATURE_MASK"

/**
 * A variable controlling whether DirectInput should be used for controllers.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable DirectInput detection.
 * - "1": Enable DirectInput detection. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_DIRECTINPUT "SDL_JOYSTICK_DIRECTINPUT"

/**
 * A variable that specifies a dialog backend to use.
 *
 * By default, SDL will try all available dialog backends in a reasonable
 * order until it finds one that can work, but this hint allows the app or
 * user to force a specific target.
 *
 * If the specified target does not exist or is not available, the
 * dialog-related function calls will fail.
 *
 * This hint currently only applies to platforms using the generic "Unix"
 * dialog implementation, but may be extended to more platforms in the future.
 * Note that some Unix and Unix-like platforms have their own implementation,
 * such as macOS and Haiku.
 *
 * The variable can be set to the following values:
 *
 * - nullptr: Select automatically (default, all platforms)
 * - "portal": Use XDG Portals through DBus (Unix only)
 * - "zenity": Use the Zenity program (Unix only)
 *
 * More options may be added in the future.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_FILE_DIALOG_DRIVER "SDL_FILE_DIALOG_DRIVER"

/**
 * Override for SDL_GetDisplayUsableBounds().
 *
 * If set, this hint will override the expected results for
 * SDL_GetDisplayUsableBounds() for display index 0. Generally you don't want
 * to do this, but this allows an embedded system to request that some of the
 * screen be reserved for other uses when paired with a well-behaved
 * application.
 *
 * The contents of this hint must be 4 comma-separated integers, the first is
 * the bounds x, then y, width and height, in that order.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_DISPLAY_USABLE_BOUNDS "SDL_DISPLAY_USABLE_BOUNDS"

/**
 * Disable giving back control to the browser automatically when running with
 * asyncify.
 *
 * With -s ASYNCIFY, SDL calls emscripten_sleep during operations such as
 * refreshing the screen or polling events.
 *
 * This hint only applies to the emscripten platform.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable emscripten_sleep calls (if you give back browser control
 *   manually or use asyncify for other purposes).
 * - "1": Enable emscripten_sleep calls. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_EMSCRIPTEN_ASYNCIFY "SDL_EMSCRIPTEN_ASYNCIFY"

/**
 * Specify the CSS selector used for the "default" window/canvas.
 *
 * This hint only applies to the emscripten platform.
 *
 * The default value is "#canvas"
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_EMSCRIPTEN_CANVAS_SELECTOR "SDL_EMSCRIPTEN_CANVAS_SELECTOR"

/**
 * Override the binding element for keyboard inputs for Emscripten builds.
 *
 * This hint only applies to the emscripten platform.
 *
 * The variable can be one of:
 *
 * - "#window": the javascript window object (default)
 * - "#document": the javascript document object
 * - "#screen": the javascript window.screen object
 * - "#canvas": the WebGL canvas element
 * - "#none": Don't bind anything at all
 * - any other string without a leading # sign applies to the element on the
 *   page with that ID.
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT "SDL_EMSCRIPTEN_KEYBOARD_ELEMENT"

/**
 * A variable that controls whether the on-screen keyboard should be shown
 * when text input is active.
 *
 * The variable can be set to the following values:
 *
 * - "auto": The on-screen keyboard will be shown if there is no physical
 *   keyboard attached. (default)
 * - "0": Do not show the on-screen keyboard.
 * - "1": Show the on-screen keyboard, if available.
 *
 * This hint must be set before SDL_StartTextInput() is called
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_ENABLE_SCREEN_KEYBOARD "SDL_ENABLE_SCREEN_KEYBOARD"

/**
 * A variable containing a list of evdev devices to use if udev is not
 * available.
 *
 * The list of devices is in the form:
 *
 * deviceclass:path[,deviceclass:path[,...]]
 *
 * where device class is an integer representing the SDL_UDEV_deviceclass and
 * path is the full path to the event device.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_EVDEV_DEVICES "SDL_EVDEV_DEVICES"

/**
 * A variable controlling verbosity of the logging of SDL events pushed onto
 * the internal queue.
 *
 * The variable can be set to the following values, from least to most
 * verbose:
 *
 * - "0": Don't log any events. (default)
 * - "1": Log most events (other than the really spammy ones).
 * - "2": Include mouse and finger motion events.
 *
 * This is generally meant to be used to debug SDL itself, but can be useful
 * for application developers that need better visibility into what is going
 * on in the event queue. Logged events are sent through Log(), which
 * means by default they appear on stdout on most platforms or maybe
 * OutputDebugString() on Windows, and can be funneled by the app with
 * SetLogOutputFunction(), etc.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_EVENT_LOGGING "SDL_EVENT_LOGGING"

/**
 * A variable controlling whether raising the window should be done more
 * forcefully.
 *
 * The variable can be set to the following values:
 *
 * - "0": Honor the OS policy for raising windows. (default)
 * - "1": Force the window to be raised, overriding any OS policy.
 *
 * At present, this is only an issue under MS Windows, which makes it nearly
 * impossible to programmatically move a window to the foreground, for
 * "security" reasons. See http://stackoverflow.com/a/34414846 for a
 * discussion.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_FORCE_RAISEWINDOW "SDL_FORCE_RAISEWINDOW"

/**
 * A variable controlling how 3D acceleration is used to accelerate the SDL
 * screen surface.
 *
 * SDL can try to accelerate the SDL screen surface by using streaming
 * textures with a 3D rendering engine. This variable controls whether and how
 * this is done.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable 3D acceleration
 * - "1": Enable 3D acceleration, using the default renderer. (default)
 * - "X": Enable 3D acceleration, using X where X is one of the valid
 *   rendering drivers. (e.g. "direct3d", "opengl", etc.)
 *
 * This hint should be set before calling SDL_GetWindowSurface()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_FRAMEBUFFER_ACCELERATION "SDL_FRAMEBUFFER_ACCELERATION"

/**
 * A variable that lets you manually hint extra gamecontroller db entries.
 *
 * The variable should be newline delimited rows of gamecontroller config
 * data, see SDL_gamepad.h
 *
 * You can update mappings after SDL is initialized with
 * SDL_GetGamepadMappingForGUID() and SDL_AddGamepadMapping()
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GAMECONTROLLERCONFIG "SDL_GAMECONTROLLERCONFIG"

/**
 * A variable that lets you provide a file with extra gamecontroller db
 * entries.
 *
 * The file should contain lines of gamecontroller config data, see
 * SDL_gamepad.h
 *
 * You can update mappings after SDL is initialized with
 * SDL_GetGamepadMappingForGUID() and SDL_AddGamepadMapping()
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GAMECONTROLLERCONFIG_FILE "SDL_GAMECONTROLLERCONFIG_FILE"

/**
 * A variable that overrides the automatic controller type detection.
 *
 * The variable should be comma separated entries, in the form: VID/PID=type
 *
 * The VID and PID should be hexadecimal with exactly 4 digits, e.g. 0x00fd
 *
 * This hint affects what low level protocol is used with the HIDAPI driver.
 *
 * The variable can be set to the following values:
 *
 * - "Xbox360"
 * - "XboxOne"
 * - "PS3"
 * - "PS4"
 * - "PS5"
 * - "SwitchPro"
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GAMECONTROLLERTYPE "SDL_GAMECONTROLLERTYPE"

/**
 * A variable containing a list of devices to skip when scanning for game
 * controllers.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * 0xAAAA/0xBBBB,0xCCCC/0xDDDD
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GAMECONTROLLER_IGNORE_DEVICES                                 \
  "SDL_GAMECONTROLLER_IGNORE_DEVICES"

/**
 * If set, all devices will be skipped when scanning for game controllers
 * except for the ones listed in this variable.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * 0xAAAA/0xBBBB,0xCCCC/0xDDDD
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GAMECONTROLLER_IGNORE_DEVICES_EXCEPT                          \
  "SDL_GAMECONTROLLER_IGNORE_DEVICES_EXCEPT"

/**
 * A variable that controls whether the device's built-in accelerometer and
 * gyro should be used as sensors for gamepads.
 *
 * The variable can be set to the following values:
 *
 * - "0": Sensor fusion is disabled
 * - "1": Sensor fusion is enabled for all controllers that lack sensors
 *
 * Or the variable can be a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * 0xAAAA/0xBBBB,0xCCCC/0xDDDD
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint should be set before a gamepad is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GAMECONTROLLER_SENSOR_FUSION "SDL_GAMECONTROLLER_SENSOR_FUSION"

/**
 * This variable sets the default text of the TextInput window on GDK
 * platforms.
 *
 * This hint is available only if SDL_GDK_TEXTINPUT defined.
 *
 * This hint should be set before calling SDL_StartTextInput()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GDK_TEXTINPUT_DEFAULT_TEXT "SDL_GDK_TEXTINPUT_DEFAULT_TEXT"

/**
 * This variable sets the description of the TextInput window on GDK
 * platforms.
 *
 * This hint is available only if SDL_GDK_TEXTINPUT defined.
 *
 * This hint should be set before calling SDL_StartTextInput()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GDK_TEXTINPUT_DESCRIPTION "SDL_GDK_TEXTINPUT_DESCRIPTION"

/**
 * This variable sets the maximum input length of the TextInput window on GDK
 * platforms.
 *
 * The value must be a stringified integer, for example "10" to allow for up
 * to 10 characters of text input.
 *
 * This hint is available only if SDL_GDK_TEXTINPUT defined.
 *
 * This hint should be set before calling SDL_StartTextInput()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GDK_TEXTINPUT_MAX_LENGTH "SDL_GDK_TEXTINPUT_MAX_LENGTH"

/**
 * This variable sets the input scope of the TextInput window on GDK
 * platforms.
 *
 * Set this hint to change the XGameUiTextEntryInputScope value that will be
 * passed to the window creation function. The value must be a stringified
 * integer, for example "0" for XGameUiTextEntryInputScope::Default.
 *
 * This hint is available only if SDL_GDK_TEXTINPUT defined.
 *
 * This hint should be set before calling SDL_StartTextInput()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GDK_TEXTINPUT_SCOPE "SDL_GDK_TEXTINPUT_SCOPE"

/**
 * This variable sets the title of the TextInput window on GDK platforms.
 *
 * This hint is available only if SDL_GDK_TEXTINPUT defined.
 *
 * This hint should be set before calling SDL_StartTextInput()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GDK_TEXTINPUT_TITLE "SDL_GDK_TEXTINPUT_TITLE"

/**
 * A variable to control whether HIDAPI uses libusb for device access.
 *
 * By default libusb will only be used for a few devices that require direct
 * USB access, and this can be controlled with
 * SDL_HINT_HIDAPI_LIBUSB_WHITELIST.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI will not use libusb for device access.
 * - "1": HIDAPI will use libusb for device access if available. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_HIDAPI_LIBUSB "SDL_HIDAPI_LIBUSB"

/**
 * A variable to control whether HIDAPI uses libusb only for whitelisted
 * devices.
 *
 * By default libusb will only be used for a few devices that require direct
 * USB access.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI will use libusb for all device access.
 * - "1": HIDAPI will use libusb only for whitelisted devices. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_HIDAPI_LIBUSB_WHITELIST "SDL_HIDAPI_LIBUSB_WHITELIST"

/**
 * A variable to control whether HIDAPI uses udev for device detection.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI will poll for device changes.
 * - "1": HIDAPI will use udev for device detection. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_HIDAPI_UDEV "SDL_HIDAPI_UDEV"

/**
 * A variable that specifies a GPU backend to use.
 *
 * By default, SDL will try all available GPU backends in a reasonable order
 * until it finds one that can work, but this hint allows the app or user to
 * force a specific target, such as "direct3d12" if, say, your hardware
 * supports Vulkan but you want to try using D3D12 instead.
 *
 * This hint should be set before any GPU functions are called.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_GPU_DRIVER "SDL_GPU_DRIVER"

/**
 * A variable to control whether SDL_hid_enumerate() enumerates all HID
 * devices or only controllers.
 *
 * The variable can be set to the following values:
 *
 * - "0": SDL_hid_enumerate() will enumerate all HID devices.
 * - "1": SDL_hid_enumerate() will only enumerate controllers. (default)
 *
 * By default SDL will only enumerate controllers, to reduce risk of hanging
 * or crashing on devices with bad drivers and avoiding macOS keyboard capture
 * permission prompts.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_HIDAPI_ENUMERATE_ONLY_CONTROLLERS                             \
  "SDL_HIDAPI_ENUMERATE_ONLY_CONTROLLERS"

/**
 * A variable containing a list of devices to ignore in SDL_hid_enumerate().
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * For example, to ignore the Shanwan DS3 controller and any Valve controller,
 * you might use the string "0x2563/0x0523,0x28de/0x0000"
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_HIDAPI_IGNORE_DEVICES "SDL_HIDAPI_IGNORE_DEVICES"

/**
 * A variable describing what IME UI elements the application can display.
 *
 * By default IME UI is handled using native components by the OS where
 * possible, however this can interfere with or not be visible when exclusive
 * fullscreen mode is used.
 *
 * The variable can be set to a comma separated list containing the following
 * items:
 *
 * - "none" or "0": The application can't render any IME elements, and native
 *   UI should be used. (default)
 * - "composition": The application handles SDL_EVENT_TEXT_EDITING events and
 *   can render the composition text.
 * - "candidates": The application handles SDL_EVENT_TEXT_EDITING_CANDIDATES
 *   and can render the candidate list.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_IME_IMPLEMENTED_UI "SDL_IME_IMPLEMENTED_UI"

/**
 * A variable controlling whether the home indicator bar on iPhone X should be
 * hidden.
 *
 * The variable can be set to the following values:
 *
 * - "0": The indicator bar is not hidden. (default for windowed applications)
 * - "1": The indicator bar is hidden and is shown when the screen is touched
 *   (useful for movie playback applications).
 * - "2": The indicator bar is dim and the first swipe makes it visible and
 *   the second swipe performs the "home" action. (default for fullscreen
 *   applications)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_IOS_HIDE_HOME_INDICATOR "SDL_IOS_HIDE_HOME_INDICATOR"

/**
 * A variable that lets you enable joystick (and gamecontroller) events even
 * when your app is in the background.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable joystick & gamecontroller input events when the application
 *   is in the background. (default)
 * - "1": Enable joystick & gamecontroller input events when the application
 *   is in the background.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS                              \
  "SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS"

/**
 * A variable containing a list of arcade stick style controllers.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_ARCADESTICK_DEVICES "SDL_JOYSTICK_ARCADESTICK_DEVICES"

/**
 * A variable containing a list of devices that are not arcade stick style
 * controllers.
 *
 * This will override SDL_HINT_JOYSTICK_ARCADESTICK_DEVICES and the built in
 * device list.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_ARCADESTICK_DEVICES_EXCLUDED                         \
  "SDL_JOYSTICK_ARCADESTICK_DEVICES_EXCLUDED"

/**
 * A variable containing a list of devices that should not be considered
 * joysticks.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_BLACKLIST_DEVICES "SDL_JOYSTICK_BLACKLIST_DEVICES"

/**
 * A variable containing a list of devices that should be considered
 * joysticks.
 *
 * This will override SDL_HINT_JOYSTICK_BLACKLIST_DEVICES and the built in
 * device list.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_BLACKLIST_DEVICES_EXCLUDED                           \
  "SDL_JOYSTICK_BLACKLIST_DEVICES_EXCLUDED"

/**
 * A variable containing a comma separated list of devices to open as
 * joysticks.
 *
 * This variable is currently only used by the Linux joystick driver.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_DEVICE "SDL_JOYSTICK_DEVICE"

/**
 * A variable controlling whether enhanced reports should be used for
 * controllers when using the HIDAPI driver.
 *
 * Enhanced reports allow rumble and effects on Bluetooth PlayStation
 * controllers and gyro on Nintendo Switch controllers, but break Windows
 * DirectInput for other applications that don't use SDL.
 *
 * Once enhanced reports are enabled, they can't be disabled on PlayStation
 * controllers without power cycling the controller.
 *
 * The variable can be set to the following values:
 *
 * - "0": enhanced reports are not enabled.
 * - "1": enhanced reports are enabled. (default)
 * - "auto": enhanced features are advertised to the application, but SDL
 *   doesn't change the controller report mode unless the application uses
 *   them.
 *
 * This hint can be enabled anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_ENHANCED_REPORTS "SDL_JOYSTICK_ENHANCED_REPORTS"

/**
 * A variable containing a list of flightstick style controllers.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of file, in which case the named file
 * will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_FLIGHTSTICK_DEVICES "SDL_JOYSTICK_FLIGHTSTICK_DEVICES"

/**
 * A variable containing a list of devices that are not flightstick style
 * controllers.
 *
 * This will override SDL_HINT_JOYSTICK_FLIGHTSTICK_DEVICES and the built in
 * device list.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_FLIGHTSTICK_DEVICES_EXCLUDED                         \
  "SDL_JOYSTICK_FLIGHTSTICK_DEVICES_EXCLUDED"

/**
 * A variable controlling whether GameInput should be used for controller
 * handling on Windows.
 *
 * The variable can be set to the following values:
 *
 * - "0": GameInput is not used.
 * - "1": GameInput is used.
 *
 * The default is "1" on GDK platforms, and "0" otherwise.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_GAMEINPUT "SDL_JOYSTICK_GAMEINPUT"

/**
 * A variable containing a list of devices known to have a GameCube form
 * factor.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_GAMECUBE_DEVICES "SDL_JOYSTICK_GAMECUBE_DEVICES"

/**
 * A variable containing a list of devices known not to have a GameCube form
 * factor.
 *
 * This will override SDL_HINT_JOYSTICK_GAMECUBE_DEVICES and the built in
 * device list.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_GAMECUBE_DEVICES_EXCLUDED                            \
  "SDL_JOYSTICK_GAMECUBE_DEVICES_EXCLUDED"

/**
 * A variable controlling whether the HIDAPI joystick drivers should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI drivers are not used.
 * - "1": HIDAPI drivers are used. (default)
 *
 * This variable is the default for all drivers, but can be overridden by the
 * hints for specific drivers below.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI "SDL_JOYSTICK_HIDAPI"

/**
 * A variable controlling whether Nintendo Switch Joy-Con controllers will be
 * combined into a single Pro-like controller when using the HIDAPI driver.
 *
 * The variable can be set to the following values:
 *
 * - "0": Left and right Joy-Con controllers will not be combined and each
 *   will be a mini-gamepad.
 * - "1": Left and right Joy-Con controllers will be combined into a single
 *   controller. (default)
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_COMBINE_JOY_CONS                              \
  "SDL_JOYSTICK_HIDAPI_COMBINE_JOY_CONS"

/**
 * A variable controlling whether the HIDAPI driver for Nintendo GameCube
 * controllers should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_GAMECUBE "SDL_JOYSTICK_HIDAPI_GAMECUBE"

/**
 * A variable controlling whether rumble is used to implement the GameCube
 * controller's 3 rumble modes, Stop(0), Rumble(1), and StopHard(2).
 *
 * This is useful for applications that need full compatibility for things
 * like ADSR envelopes. - Stop is implemented by setting low_frequency_rumble
 * to 0 and high_frequency_rumble >0 - Rumble is both at any arbitrary value -
 * StopHard is implemented by setting both low_frequency_rumble and
 * high_frequency_rumble to 0
 *
 * The variable can be set to the following values:
 *
 * - "0": Normal rumble behavior is behavior is used. (default)
 * - "1": Proper GameCube controller rumble behavior is used.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_GAMECUBE_RUMBLE_BRAKE                         \
  "SDL_JOYSTICK_HIDAPI_GAMECUBE_RUMBLE_BRAKE"

/**
 * A variable controlling whether the HIDAPI driver for Nintendo Switch
 * Joy-Cons should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_JOY_CONS "SDL_JOYSTICK_HIDAPI_JOY_CONS"

/**
 * A variable controlling whether the Home button LED should be turned on when
 * a Nintendo Switch Joy-Con controller is opened.
 *
 * The variable can be set to the following values:
 *
 * - "0": home button LED is turned off
 * - "1": home button LED is turned on
 *
 * By default the Home button LED state is not changed. This hint can also be
 * set to a floating point value between 0.0 and 1.0 which controls the
 * brightness of the Home button LED.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_JOYCON_HOME_LED                               \
  "SDL_JOYSTICK_HIDAPI_JOYCON_HOME_LED"

/**
 * A variable controlling whether the HIDAPI driver for Amazon Luna
 * controllers connected via Bluetooth should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_LUNA "SDL_JOYSTICK_HIDAPI_LUNA"

/**
 * A variable controlling whether the HIDAPI driver for Nintendo Online
 * classic controllers should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_NINTENDO_CLASSIC                              \
  "SDL_JOYSTICK_HIDAPI_NINTENDO_CLASSIC"

/**
 * A variable controlling whether the HIDAPI driver for PS3 controllers should
 * be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI on macOS, and "0" on
 * other platforms.
 *
 * For official Sony driver (sixaxis.sys) use
 * SDL_HINT_JOYSTICK_HIDAPI_PS3_SIXAXIS_DRIVER. See
 * https://github.com/ViGEm/DsHidMini for an alternative driver on Windows.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_PS3 "SDL_JOYSTICK_HIDAPI_PS3"

/**
 * A variable controlling whether the Sony driver (sixaxis.sys) for PS3
 * controllers (Sixaxis/DualShock 3) should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": Sony driver (sixaxis.sys) is not used.
 * - "1": Sony driver (sixaxis.sys) is used.
 *
 * The default value is 0.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_PS3_SIXAXIS_DRIVER                            \
  "SDL_JOYSTICK_HIDAPI_PS3_SIXAXIS_DRIVER"

/**
 * A variable controlling whether the HIDAPI driver for PS4 controllers should
 * be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_PS4 "SDL_JOYSTICK_HIDAPI_PS4"

/**
 * A variable controlling the update rate of the PS4 controller over Bluetooth
 * when using the HIDAPI driver.
 *
 * This defaults to 4 ms, to match the behavior over USB, and to be more
 * friendly to other Bluetooth devices and older Bluetooth hardware on the
 * computer. It can be set to "1" (1000Hz), "2" (500Hz) and "4" (250Hz)
 *
 * This hint can be set anytime, but only takes effect when extended input
 * reports are enabled.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_PS4_REPORT_INTERVAL                           \
  "SDL_JOYSTICK_HIDAPI_PS4_REPORT_INTERVAL"

/**
 * A variable controlling whether the HIDAPI driver for PS5 controllers should
 * be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_PS5 "SDL_JOYSTICK_HIDAPI_PS5"

/**
 * A variable controlling whether the player LEDs should be lit to indicate
 * which player is associated with a PS5 controller.
 *
 * The variable can be set to the following values:
 *
 * - "0": player LEDs are not enabled.
 * - "1": player LEDs are enabled. (default)
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_PS5_PLAYER_LED                                \
  "SDL_JOYSTICK_HIDAPI_PS5_PLAYER_LED"

/**
 * A variable controlling whether the HIDAPI driver for NVIDIA SHIELD
 * controllers should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_SHIELD "SDL_JOYSTICK_HIDAPI_SHIELD"

/**
 * A variable controlling whether the HIDAPI driver for Google Stadia
 * controllers should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_STADIA "SDL_JOYSTICK_HIDAPI_STADIA"

/**
 * A variable controlling whether the HIDAPI driver for Bluetooth Steam
 * Controllers should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used. (default)
 * - "1": HIDAPI driver is used for Steam Controllers, which requires
 *   Bluetooth access and may prompt the user for permission on iOS and
 *   Android.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_STEAM "SDL_JOYSTICK_HIDAPI_STEAM"

/**
 * A variable controlling whether the Steam button LED should be turned on
 * when a Steam controller is opened.
 *
 * The variable can be set to the following values:
 *
 * - "0": Steam button LED is turned off.
 * - "1": Steam button LED is turned on.
 *
 * By default the Steam button LED state is not changed. This hint can also be
 * set to a floating point value between 0.0 and 1.0 which controls the
 * brightness of the Steam button LED.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_STEAM_HOME_LED                                \
  "SDL_JOYSTICK_HIDAPI_STEAM_HOME_LED"

/**
 * A variable controlling whether the HIDAPI driver for the Steam Deck builtin
 * controller should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_STEAMDECK "SDL_JOYSTICK_HIDAPI_STEAMDECK"

/**
 * A variable controlling whether the HIDAPI driver for HORI licensed Steam
 * controllers should be used.
 *
 * This variable can be set to the following values: "0" - HIDAPI driver is
 * not used "1" - HIDAPI driver is used
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI
 */
#define SDL_HINT_JOYSTICK_HIDAPI_STEAM_HORI "SDL_JOYSTICK_HIDAPI_STEAM_HORI"

/**
 * A variable controlling whether the HIDAPI driver for Nintendo Switch
 * controllers should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_SWITCH "SDL_JOYSTICK_HIDAPI_SWITCH"

/**
 * A variable controlling whether the Home button LED should be turned on when
 * a Nintendo Switch Pro controller is opened.
 *
 * The variable can be set to the following values:
 *
 * - "0": Home button LED is turned off.
 * - "1": Home button LED is turned on.
 *
 * By default the Home button LED state is not changed. This hint can also be
 * set to a floating point value between 0.0 and 1.0 which controls the
 * brightness of the Home button LED.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_SWITCH_HOME_LED                               \
  "SDL_JOYSTICK_HIDAPI_SWITCH_HOME_LED"

/**
 * A variable controlling whether the player LEDs should be lit to indicate
 * which player is associated with a Nintendo Switch controller.
 *
 * The variable can be set to the following values:
 *
 * - "0": Player LEDs are not enabled.
 * - "1": Player LEDs are enabled. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_SWITCH_PLAYER_LED                             \
  "SDL_JOYSTICK_HIDAPI_SWITCH_PLAYER_LED"

/**
 * A variable controlling whether Nintendo Switch Joy-Con controllers will be
 * in vertical mode when using the HIDAPI driver.
 *
 * The variable can be set to the following values:
 *
 * - "0": Left and right Joy-Con controllers will not be in vertical mode.
 *   (default)
 * - "1": Left and right Joy-Con controllers will be in vertical mode.
 *
 * This hint should be set before opening a Joy-Con controller.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_VERTICAL_JOY_CONS                             \
  "SDL_JOYSTICK_HIDAPI_VERTICAL_JOY_CONS"

/**
 * A variable controlling whether the HIDAPI driver for Nintendo Wii and Wii U
 * controllers should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * This driver doesn't work with the dolphinbar, so the default is false for
 * now.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_WII "SDL_JOYSTICK_HIDAPI_WII"

/**
 * A variable controlling whether the player LEDs should be lit to indicate
 * which player is associated with a Wii controller.
 *
 * The variable can be set to the following values:
 *
 * - "0": Player LEDs are not enabled.
 * - "1": Player LEDs are enabled. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_WII_PLAYER_LED                                \
  "SDL_JOYSTICK_HIDAPI_WII_PLAYER_LED"

/**
 * A variable controlling whether the HIDAPI driver for XBox controllers
 * should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is "0" on Windows, otherwise the value of
 * SDL_HINT_JOYSTICK_HIDAPI
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_XBOX "SDL_JOYSTICK_HIDAPI_XBOX"

/**
 * A variable controlling whether the HIDAPI driver for XBox 360 controllers
 * should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI_XBOX
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_XBOX_360 "SDL_JOYSTICK_HIDAPI_XBOX_360"

/**
 * A variable controlling whether the player LEDs should be lit to indicate
 * which player is associated with an Xbox 360 controller.
 *
 * The variable can be set to the following values:
 *
 * - "0": Player LEDs are not enabled.
 * - "1": Player LEDs are enabled. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_XBOX_360_PLAYER_LED                           \
  "SDL_JOYSTICK_HIDAPI_XBOX_360_PLAYER_LED"

/**
 * A variable controlling whether the HIDAPI driver for XBox 360 wireless
 * controllers should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI_XBOX_360
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_XBOX_360_WIRELESS                             \
  "SDL_JOYSTICK_HIDAPI_XBOX_360_WIRELESS"

/**
 * A variable controlling whether the HIDAPI driver for XBox One controllers
 * should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": HIDAPI driver is not used.
 * - "1": HIDAPI driver is used.
 *
 * The default is the value of SDL_HINT_JOYSTICK_HIDAPI_XBOX.
 *
 * This hint should be set before initializing joysticks and gamepads.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_XBOX_ONE "SDL_JOYSTICK_HIDAPI_XBOX_ONE"

/**
 * A variable controlling whether the Home button LED should be turned on when
 * an Xbox One controller is opened.
 *
 * The variable can be set to the following values:
 *
 * - "0": Home button LED is turned off.
 * - "1": Home button LED is turned on.
 *
 * By default the Home button LED state is not changed. This hint can also be
 * set to a floating point value between 0.0 and 1.0 which controls the
 * brightness of the Home button LED. The default brightness is 0.4.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_HIDAPI_XBOX_ONE_HOME_LED                             \
  "SDL_JOYSTICK_HIDAPI_XBOX_ONE_HOME_LED"

/**
 * A variable controlling whether IOKit should be used for controller
 * handling.
 *
 * The variable can be set to the following values:
 *
 * - "0": IOKit is not used.
 * - "1": IOKit is used. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_IOKIT "SDL_JOYSTICK_IOKIT"

/**
 * A variable controlling whether to use the classic /dev/input/js* joystick
 * interface or the newer /dev/input/event* joystick interface on Linux.
 *
 * The variable can be set to the following values:
 *
 * - "0": Use /dev/input/event* (default)
 * - "1": Use /dev/input/js*
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_LINUX_CLASSIC "SDL_JOYSTICK_LINUX_CLASSIC"

/**
 * A variable controlling whether joysticks on Linux adhere to their
 * HID-defined deadzones or return unfiltered values.
 *
 * The variable can be set to the following values:
 *
 * - "0": Return unfiltered joystick axis values. (default)
 * - "1": Return axis values with deadzones taken into account.
 *
 * This hint should be set before a controller is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_LINUX_DEADZONES "SDL_JOYSTICK_LINUX_DEADZONES"

/**
 * A variable controlling whether joysticks on Linux will always treat 'hat'
 * axis inputs (ABS_HAT0X - ABS_HAT3Y) as 8-way digital hats without checking
 * whether they may be analog.
 *
 * The variable can be set to the following values:
 *
 * - "0": Only map hat axis inputs to digital hat outputs if the input axes
 *   appear to actually be digital. (default)
 * - "1": Always handle the input axes numbered ABS_HAT0X to ABS_HAT3Y as
 *   digital hats.
 *
 * This hint should be set before a controller is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_LINUX_DIGITAL_HATS "SDL_JOYSTICK_LINUX_DIGITAL_HATS"

/**
 * A variable controlling whether digital hats on Linux will apply deadzones
 * to their underlying input axes or use unfiltered values.
 *
 * The variable can be set to the following values:
 *
 * - "0": Return digital hat values based on unfiltered input axis values.
 * - "1": Return digital hat values with deadzones on the input axes taken
 *   into account. (default)
 *
 * This hint should be set before a controller is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_LINUX_HAT_DEADZONES "SDL_JOYSTICK_LINUX_HAT_DEADZONES"

/**
 * A variable controlling whether GCController should be used for controller
 * handling.
 *
 * The variable can be set to the following values:
 *
 * - "0": GCController is not used.
 * - "1": GCController is used. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_MFI "SDL_JOYSTICK_MFI"

/**
 * A variable controlling whether the RAWINPUT joystick drivers should be used
 * for better handling XInput-capable devices.
 *
 * The variable can be set to the following values:
 *
 * - "0": RAWINPUT drivers are not used. (default)
 * - "1": RAWINPUT drivers are used.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_RAWINPUT "SDL_JOYSTICK_RAWINPUT"

/**
 * A variable controlling whether the RAWINPUT driver should pull correlated
 * data from XInput.
 *
 * The variable can be set to the following values:
 *
 * - "0": RAWINPUT driver will only use data from raw input APIs.
 * - "1": RAWINPUT driver will also pull data from XInput and
 *   Windows.Gaming.Input, providing better trigger axes, guide button
 *   presses, and rumble support for Xbox controllers. (default)
 *
 * This hint should be set before a gamepad is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT                            \
  "SDL_JOYSTICK_RAWINPUT_CORRELATE_XINPUT"

/**
 * A variable controlling whether the ROG Chakram mice should show up as
 * joysticks.
 *
 * The variable can be set to the following values:
 *
 * - "0": ROG Chakram mice do not show up as joysticks. (default)
 * - "1": ROG Chakram mice show up as joysticks.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_ROG_CHAKRAM "SDL_JOYSTICK_ROG_CHAKRAM"

/**
 * A variable controlling whether a separate thread should be used for
 * handling joystick detection and raw input messages on Windows.
 *
 * The variable can be set to the following values:
 *
 * - "0": A separate thread is not used.
 * - "1": A separate thread is used for handling raw input messages. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_THREAD "SDL_JOYSTICK_THREAD"

/**
 * A variable containing a list of throttle style controllers.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_THROTTLE_DEVICES "SDL_JOYSTICK_THROTTLE_DEVICES"

/**
 * A variable containing a list of devices that are not throttle style
 * controllers.
 *
 * This will override SDL_HINT_JOYSTICK_THROTTLE_DEVICES and the built in
 * device list.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_THROTTLE_DEVICES_EXCLUDED                            \
  "SDL_JOYSTICK_THROTTLE_DEVICES_EXCLUDED"

/**
 * A variable controlling whether Windows.Gaming.Input should be used for
 * controller handling.
 *
 * The variable can be set to the following values:
 *
 * - "0": WGI is not used. (default)
 * - "1": WGI is used.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_WGI "SDL_JOYSTICK_WGI"

/**
 * A variable containing a list of wheel style controllers.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_WHEEL_DEVICES "SDL_JOYSTICK_WHEEL_DEVICES"

/**
 * A variable containing a list of devices that are not wheel style
 * controllers.
 *
 * This will override SDL_HINT_JOYSTICK_WHEEL_DEVICES and the built in device
 * list.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_WHEEL_DEVICES_EXCLUDED                               \
  "SDL_JOYSTICK_WHEEL_DEVICES_EXCLUDED"

/**
 * A variable containing a list of devices known to have all axes centered at
 * zero.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint should be set before a controller is opened.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_JOYSTICK_ZERO_CENTERED_DEVICES                                \
  "SDL_JOYSTICK_ZERO_CENTERED_DEVICES"

#if SDL_VERSION_ATLEAST(3, 2, 5)

/**
 * A variable containing a list of devices and their desired number of haptic
 * (force feedback) enabled axis.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form plus the number of desired axes, e.g.
 *
 * `0xAAAA/0xBBBB/1,0xCCCC/0xDDDD/3`
 *
 * This hint supports a "wildcard" device that will set the number of haptic
 * axes on all initialized haptic devices which were not defined explicitly in
 * this hint.
 *
 * `0xFFFF/0xFFFF/1`
 *
 * This hint should be set before a controller is opened. The number of haptic
 * axes won't exceed the number of real axes found on the device.
 *
 * @since This hint is available since SDL 3.2.5.
 */
#define SDL_HINT_JOYSTICK_HAPTIC_AXES "SDL_JOYSTICK_HAPTIC_AXES"

#endif // SDL_VERSION_ATLEAST(3, 2, 5)

/**
 * A variable that controls keycode representation in keyboard events.
 *
 * This variable is a comma separated set of options for translating keycodes
 * in events:
 *
 * - "none": Keycode options are cleared, this overrides other options.
 * - "hide_numpad": The numpad keysyms will be translated into their
 *   non-numpad versions based on the current NumLock state. For example,
 *   SDLK_KP_4 would become SDLK_4 if SDL_KMOD_NUM is set in the event
 *   modifiers, and SDLK_LEFT if it is unset.
 * - "french_numbers": The number row on French keyboards is inverted, so
 *   pressing the 1 key would yield the keycode SDLK_1, or '1', instead of
 *   SDLK_AMPERSAND, or '&'
 * - "latin_letters": For keyboards using non-Latin letters, such as Russian
 *   or Thai, the letter keys generate keycodes as though it had an en_US
 *   layout. e.g. pressing the key associated with SDL_SCANCODE_A on a Russian
 *   keyboard would yield 'a' instead of a Cyrillic letter.
 *
 * The default value for this hint is "french_numbers,latin_letters"
 *
 * Some platforms like Emscripten only provide modified keycodes and the
 * options are not used.
 *
 * These options do not affect the return value of SDL_GetKeyFromScancode() or
 * SDL_GetScancodeFromKey(), they just apply to the keycode included in key
 * events.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_KEYCODE_OPTIONS "SDL_KEYCODE_OPTIONS"

/**
 * A variable that controls what KMSDRM device to use.
 *
 * SDL might open something like "/dev/dri/cardNN" to access KMSDRM
 * functionality, where "NN" is a device index number. SDL makes a guess at
 * the best index to use (usually zero), but the app or user can set this hint
 * to a number between 0 and 99 to force selection.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_KMSDRM_DEVICE_INDEX "SDL_KMSDRM_DEVICE_INDEX"

/**
 * A variable that controls whether SDL requires DRM master access in order to
 * initialize the KMSDRM video backend.
 *
 * The DRM subsystem has a concept of a "DRM master" which is a DRM client
 * that has the ability to set planes, set cursor, etc. When SDL is DRM
 * master, it can draw to the screen using the SDL rendering APIs. Without DRM
 * master, SDL is still able to process input and query attributes of attached
 * displays, but it cannot change display state or draw to the screen
 * directly.
 *
 * In some cases, it can be useful to have the KMSDRM backend even if it
 * cannot be used for rendering. An app may want to use SDL for input
 * processing while using another rendering API (such as an MMAL overlay on
 * Raspberry Pi) or using its own code to render to DRM overlays that SDL
 * doesn't support.
 *
 * The variable can be set to the following values:
 *
 * - "0": SDL will allow usage of the KMSDRM backend without DRM master.
 * - "1": SDL Will require DRM master to use the KMSDRM backend. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_KMSDRM_REQUIRE_DRM_MASTER "SDL_KMSDRM_REQUIRE_DRM_MASTER"

/**
 * A variable controlling the default SDL log levels.
 *
 * This variable is a comma separated set of category=level tokens that define
 * the default logging levels for SDL applications.
 *
 * The category can be a numeric category, one of "app", "error", "assert",
 * "system", "audio", "video", "render", "input", "test", or `*` for any
 * unspecified category.
 *
 * The level can be a numeric level, one of "verbose", "debug", "info",
 * "warn", "error", "critical", or "quiet" to disable that category.
 *
 * You can omit the category if you want to set the logging level for all
 * categories.
 *
 * If this hint isn't set, the default log levels are equivalent to:
 *
 * `app=info,assert=warn,test=verbose,*=error`
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_LOGGING "SDL_LOGGING"

/**
 * A variable controlling whether to force the application to become the
 * foreground process when launched on macOS.
 *
 * The variable can be set to the following values:
 *
 * - "0": The application is brought to the foreground when launched.
 *   (default)
 * - "1": The application may remain in the background when launched.
 *
 * This hint needs to be set before Init().
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MAC_BACKGROUND_APP "SDL_MAC_BACKGROUND_APP"

/**
 * A variable that determines whether Ctrl+Click should generate a right-click
 * event on macOS.
 *
 * The variable can be set to the following values:
 *
 * - "0": Ctrl+Click does not generate a right mouse button click event.
 *   (default)
 * - "1": Ctrl+Click generated a right mouse button click event.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK                            \
  "SDL_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK"

/**
 * A variable controlling whether dispatching OpenGL context updates should
 * block the dispatching thread until the main thread finishes processing on
 * macOS.
 *
 * The variable can be set to the following values:
 *
 * - "0": Dispatching OpenGL context updates will block the dispatching thread
 *   until the main thread finishes processing. (default)
 * - "1": Dispatching OpenGL context updates will allow the dispatching thread
 *   to continue execution.
 *
 * Generally you want the default, but if you have OpenGL code in a background
 * thread on a Mac, and the main thread hangs because it's waiting for that
 * background thread, but that background thread is also hanging because it's
 * waiting for the main thread to do an update, this might fix your issue.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MAC_OPENGL_ASYNC_DISPATCH "SDL_MAC_OPENGL_ASYNC_DISPATCH"

/**
 * A variable controlling whether the Option key on macOS should be remapped
 * to act as the Alt key.
 *
 * The variable can be set to the following values:
 *
 * - "none": The Option key is not remapped to Alt. (default)
 * - "only_left": Only the left Option key is remapped to Alt.
 * - "only_right": Only the right Option key is remapped to Alt.
 * - "both": Both Option keys are remapped to Alt.
 *
 * This will prevent the triggering of key compositions that rely on the
 * Option key, but will still send the Alt modifier for keyboard events. In
 * the case that both Alt and Option are pressed, the Option key will be
 * ignored. This is particularly useful for applications like terminal
 * emulators and graphical user interfaces (GUIs) that rely on Alt key
 * functionality for shortcuts or navigation. This does not apply to
 * SDL_GetKeyFromScancode and only has an effect if IME is enabled.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MAC_OPTION_AS_ALT "SDL_MAC_OPTION_AS_ALT"

/**
 * A variable controlling whether SDL_EVENT_MOUSE_WHEEL event values will have
 * momentum on macOS.
 *
 * The variable can be set to the following values:
 *
 * - "0": The mouse wheel events will have no momentum. (default)
 * - "1": The mouse wheel events will have momentum.
 *
 * This hint needs to be set before Init().
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MAC_SCROLL_MOMENTUM "SDL_MAC_SCROLL_MOMENTUM"

/**
 * Request SDL_AppIterate() be called at a specific rate.
 *
 * If this is set to a number, it represents Hz, so "60" means try to iterate
 * 60 times per second. "0" means to iterate as fast as possible. Negative
 * values are illegal, but reserved, in case they are useful in a future
 * revision of SDL.
 *
 * There are other strings that have special meaning. If set to "waitevent",
 * SDL_AppIterate will not be called until new event(s) have arrived (and been
 * processed by SDL_AppEvent). This can be useful for apps that are completely
 * idle except in response to input.
 *
 * On some platforms, or if you are using SDL_main instead of SDL_AppIterate,
 * this hint is ignored. When the hint can be used, it is allowed to be
 * changed at any time.
 *
 * This defaults to 0, and specifying nullptr for the hint's value will restore
 * the default.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MAIN_CALLBACK_RATE "SDL_MAIN_CALLBACK_RATE"

/**
 * A variable controlling whether the mouse is captured while mouse buttons
 * are pressed.
 *
 * The variable can be set to the following values:
 *
 * - "0": The mouse is not captured while mouse buttons are pressed.
 * - "1": The mouse is captured while mouse buttons are pressed.
 *
 * By default the mouse is captured while mouse buttons are pressed so if the
 * mouse is dragged outside the window, the application continues to receive
 * mouse events until the button is released.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_AUTO_CAPTURE "SDL_MOUSE_AUTO_CAPTURE"

/**
 * A variable setting the double click radius, in pixels.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_DOUBLE_CLICK_RADIUS "SDL_MOUSE_DOUBLE_CLICK_RADIUS"

/**
 * A variable setting the double click time, in milliseconds.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_DOUBLE_CLICK_TIME "SDL_MOUSE_DOUBLE_CLICK_TIME"

/**
 * A variable setting which system cursor to use as the default cursor.
 *
 * This should be an integer corresponding to the SDL_SystemCursor enum. The
 * default value is zero (SDL_SYSTEM_CURSOR_DEFAULT).
 *
 * This hint needs to be set before Init().
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_DEFAULT_SYSTEM_CURSOR "SDL_MOUSE_DEFAULT_SYSTEM_CURSOR"

/**
 * A variable controlling whether warping a hidden mouse cursor will activate
 * relative mouse mode.
 *
 * When this hint is set, the mouse cursor is hidden, and multiple warps to
 * the window center occur within a short time period, SDL will emulate mouse
 * warps using relative mouse mode. This can provide smoother and more
 * reliable mouse motion for some older games, which continuously calculate
 * the distance travelled by the mouse pointer and warp it back to the center
 * of the window, rather than using relative mouse motion.
 *
 * Note that relative mouse mode may have different mouse acceleration
 * behavior than pointer warps.
 *
 * If your application needs to repeatedly warp the hidden mouse cursor at a
 * high-frequency for other purposes, it should disable this hint.
 *
 * The variable can be set to the following values:
 *
 * - "0": Attempts to warp the mouse will always be made.
 * - "1": Some mouse warps will be emulated by forcing relative mouse mode.
 *   (default)
 *
 * If not set, this is automatically enabled unless an application uses
 * relative mouse mode directly.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_EMULATE_WARP_WITH_RELATIVE                              \
  "SDL_MOUSE_EMULATE_WARP_WITH_RELATIVE"

/**
 * Allow mouse click events when clicking to focus an SDL window.
 *
 * The variable can be set to the following values:
 *
 * - "0": Ignore mouse clicks that activate a window. (default)
 * - "1": Generate events for mouse clicks that activate a window.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH "SDL_MOUSE_FOCUS_CLICKTHROUGH"

/**
 * A variable setting the speed scale for mouse motion, in floating point,
 * when the mouse is not in relative mode.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_NORMAL_SPEED_SCALE "SDL_MOUSE_NORMAL_SPEED_SCALE"

/**
 * A variable controlling whether relative mouse mode constrains the mouse to
 * the center of the window.
 *
 * Constraining to the center of the window works better for FPS games and
 * when the application is running over RDP. Constraining to the whole window
 * works better for 2D games and increases the chance that the mouse will be
 * in the correct position when using high DPI mice.
 *
 * The variable can be set to the following values:
 *
 * - "0": Relative mouse mode constrains the mouse to the window.
 * - "1": Relative mouse mode constrains the mouse to the center of the
 *   window. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_RELATIVE_MODE_CENTER "SDL_MOUSE_RELATIVE_MODE_CENTER"

/**
 * A variable setting the scale for mouse motion, in floating point, when the
 * mouse is in relative mode.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_RELATIVE_SPEED_SCALE "SDL_MOUSE_RELATIVE_SPEED_SCALE"

/**
 * A variable controlling whether the system mouse acceleration curve is used
 * for relative mouse motion.
 *
 * The variable can be set to the following values:
 *
 * - "0": Relative mouse motion will be unscaled. (default)
 * - "1": Relative mouse motion will be scaled using the system mouse
 *   acceleration curve.
 *
 * If SDL_HINT_MOUSE_RELATIVE_SPEED_SCALE is set, that will be applied after
 * system speed scale.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_RELATIVE_SYSTEM_SCALE "SDL_MOUSE_RELATIVE_SYSTEM_SCALE"

/**
 * A variable controlling whether a motion event should be generated for mouse
 * warping in relative mode.
 *
 * The variable can be set to the following values:
 *
 * - "0": Warping the mouse will not generate a motion event in relative mode
 * - "1": Warping the mouse will generate a motion event in relative mode
 *
 * By default warping the mouse will not generate motion events in relative
 * mode. This avoids the application having to filter out large relative
 * motion due to warping.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_RELATIVE_WARP_MOTION "SDL_MOUSE_RELATIVE_WARP_MOTION"

/**
 * A variable controlling whether the hardware cursor stays visible when
 * relative mode is active.
 *
 * This variable can be set to the following values:
 *
 * - "0": The cursor will be hidden while relative mode is active (default)
 * - "1": The cursor will remain visible while relative mode is active
 *
 * Note that for systems without raw hardware inputs, relative mode is
 * implemented using warping, so the hardware cursor will visibly warp between
 * frames if this is enabled on those systems.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_RELATIVE_CURSOR_VISIBLE                                 \
  "SDL_MOUSE_RELATIVE_CURSOR_VISIBLE"

/**
 * A variable controlling whether mouse events should generate synthetic touch
 * events.
 *
 * The variable can be set to the following values:
 *
 * - "0": Mouse events will not generate touch events. (default for desktop
 *   platforms)
 * - "1": Mouse events will generate touch events. (default for mobile
 *   platforms, such as Android and iOS)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MOUSE_TOUCH_EVENTS "SDL_MOUSE_TOUCH_EVENTS"

/**
 * A variable controlling whether the keyboard should be muted on the console.
 *
 * Normally the keyboard is muted while SDL applications are running so that
 * keyboard input doesn't show up as key strokes on the console. This hint
 * allows you to turn that off for debugging purposes.
 *
 * The variable can be set to the following values:
 *
 * - "0": Allow keystrokes to go through to the console.
 * - "1": Mute keyboard input so it doesn't show up on the console. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_MUTE_CONSOLE_KEYBOARD "SDL_MUTE_CONSOLE_KEYBOARD"

/**
 * Tell SDL not to catch the SIGINT or SIGTERM signals on POSIX platforms.
 *
 * The variable can be set to the following values:
 *
 * - "0": SDL will install a SIGINT and SIGTERM handler, and when it catches a
 *   signal, convert it into an SDL_EVENT_QUIT event. (default)
 * - "1": SDL will not install a signal handler at all.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_NO_SIGNAL_HANDLERS "SDL_NO_SIGNAL_HANDLERS"

/**
 * Specify the OpenGL library to load.
 *
 * This hint should be set before creating an OpenGL window or creating an
 * OpenGL context. If this hint isn't set, SDL will choose a reasonable
 * default.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_OPENGL_LIBRARY "SDL_OPENGL_LIBRARY"

/**
 * Specify the EGL library to load.
 *
 * This hint should be set before creating an OpenGL window or creating an
 * OpenGL context. This hint is only considered if SDL is using EGL to manage
 * OpenGL contexts. If this hint isn't set, SDL will choose a reasonable
 * default.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_EGL_LIBRARY "SDL_EGL_LIBRARY"

/**
 * A variable controlling what driver to use for OpenGL ES contexts.
 *
 * On some platforms, currently Windows and X11, OpenGL drivers may support
 * creating contexts with an OpenGL ES profile. By default SDL uses these
 * profiles, when available, otherwise it attempts to load an OpenGL ES
 * library, e.g. that provided by the ANGLE project. This variable controls
 * whether SDL follows this default behaviour or will always load an OpenGL ES
 * library.
 *
 * Circumstances where this is useful include - Testing an app with a
 * particular OpenGL ES implementation, e.g ANGLE, or emulator, e.g. those
 * from ARM, Imagination or Qualcomm. - Resolving OpenGL ES function addresses
 * at link time by linking with the OpenGL ES library instead of querying them
 * at run time with SDL_GL_GetProcAddress().
 *
 * Caution: for an application to work with the default behaviour across
 * different OpenGL drivers it must query the OpenGL ES function addresses at
 * run time using SDL_GL_GetProcAddress().
 *
 * This variable is ignored on most platforms because OpenGL ES is native or
 * not supported.
 *
 * The variable can be set to the following values:
 *
 * - "0": Use ES profile of OpenGL, if available. (default)
 * - "1": Load OpenGL ES library using the default library names.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_OPENGL_ES_DRIVER "SDL_OPENGL_ES_DRIVER"

/**
 * Mechanism to specify openvr_api library location
 *
 * By default, when using the OpenVR driver, it will search for the API
 * library in the current folder. But, if you wish to use a system API you can
 * specify that by using this hint. This should be the full or relative path
 * to a .dll on Windows or .so on Linux.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_OPENVR_LIBRARY "SDL_OPENVR_LIBRARY"

/**
 * A variable controlling which orientations are allowed on iOS/Android.
 *
 * In some circumstances it is necessary to be able to explicitly control
 * which UI orientations are allowed.
 *
 * This variable is a space delimited list of the following values:
 *
 * - "LandscapeLeft"
 * - "LandscapeRight"
 * - "Portrait"
 * - "PortraitUpsideDown"
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_ORIENTATIONS "SDL_ORIENTATIONS"

/**
 * A variable controlling the use of a sentinel event when polling the event
 * queue.
 *
 * When polling for events, SDL_PumpEvents is used to gather new events from
 * devices. If a device keeps producing new events between calls to
 * SDL_PumpEvents, a poll loop will become stuck until the new events stop.
 * This is most noticeable when moving a high frequency mouse.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable poll sentinels.
 * - "1": Enable poll sentinels. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_POLL_SENTINEL "SDL_POLL_SENTINEL"

/**
 * Override for SDL_GetPreferredLocales().
 *
 * If set, this will be favored over anything the OS might report for the
 * user's preferred locales. Changing this hint at runtime will not generate a
 * SDL_EVENT_LOCALE_CHANGED event (but if you can change the hint, you can
 * push your own event, if you want).
 *
 * The format of this hint is a comma-separated list of language and locale,
 * combined with an underscore, as is a common format: "en_GB". Locale is
 * optional: "en". So you might have a list like this: "en_GB,jp,es_PT"
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_PREFERRED_LOCALES "SDL_PREFERRED_LOCALES"

/**
 * A variable that decides whether to send SDL_EVENT_QUIT when closing the
 * last window.
 *
 * The variable can be set to the following values:
 *
 * - "0": SDL will not send an SDL_EVENT_QUIT event when the last window is
 *   requesting to close. Note that in this case, there are still other
 *   legitimate reasons one might get an SDL_EVENT_QUIT event: choosing "Quit"
 *   from the macOS menu bar, sending a SIGINT (ctrl-c) on Unix, etc.
 * - "1": SDL will send a quit event when the last window is requesting to
 *   close. (default)
 *
 * If there is at least one active system tray icon, SDL_EVENT_QUIT will
 * instead be sent when both the last window will be closed and the last tray
 * icon will be destroyed.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_QUIT_ON_LAST_WINDOW_CLOSE "SDL_QUIT_ON_LAST_WINDOW_CLOSE"

/**
 * A variable controlling whether the Direct3D device is initialized for
 * thread-safe operations.
 *
 * The variable can be set to the following values:
 *
 * - "0": Thread-safety is not enabled. (default)
 * - "1": Thread-safety is enabled.
 *
 * This hint should be set before creating a renderer.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RENDER_DIRECT3D_THREADSAFE "SDL_RENDER_DIRECT3D_THREADSAFE"

/**
 * A variable controlling whether to enable Direct3D 11+'s Debug Layer.
 *
 * This variable does not have any effect on the Direct3D 9 based renderer.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable Debug Layer use. (default)
 * - "1": Enable Debug Layer use.
 *
 * This hint should be set before creating a renderer.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RENDER_DIRECT3D11_DEBUG "SDL_RENDER_DIRECT3D11_DEBUG"

/**
 * A variable controlling whether to enable Vulkan Validation Layers.
 *
 * This variable can be set to the following values:
 *
 * - "0": Disable Validation Layer use
 * - "1": Enable Validation Layer use
 *
 * By default, SDL does not use Vulkan Validation Layers.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RENDER_VULKAN_DEBUG "SDL_RENDER_VULKAN_DEBUG"

/**
 * A variable controlling whether to create the GPU device in debug mode.
 *
 * This variable can be set to the following values:
 *
 * - "0": Disable debug mode use (default)
 * - "1": Enable debug mode use
 *
 * This hint should be set before creating a renderer.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RENDER_GPU_DEBUG "SDL_RENDER_GPU_DEBUG"

/**
 * A variable controlling whether to prefer a low-power GPU on multi-GPU
 * systems.
 *
 * This variable can be set to the following values:
 *
 * - "0": Prefer high-performance GPU (default)
 * - "1": Prefer low-power GPU
 *
 * This hint should be set before creating a renderer.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RENDER_GPU_LOW_POWER "SDL_RENDER_GPU_LOW_POWER"

/**
 * A variable specifying which render driver to use.
 *
 * If the application doesn't pick a specific renderer to use, this variable
 * specifies the name of the preferred renderer. If the preferred renderer
 * can't be initialized, creating a renderer will fail.
 *
 * This variable is case insensitive and can be set to the following values:
 *
 * - "direct3d"
 * - "direct3d11"
 * - "direct3d12"
 * - "opengl"
 * - "opengles2"
 * - "opengles"
 * - "metal"
 * - "vulkan"
 * - "gpu"
 * - "software"
 *
 * This hint accepts a comma-separated list of driver names, and each will be
 * tried in the order listed when creating a renderer until one succeeds or
 * all of them fail.
 *
 * The default varies by platform, but it's the first one in the list that is
 * available on the current platform.
 *
 * This hint should be set before creating a renderer.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RENDER_DRIVER "SDL_RENDER_DRIVER"

/**
 * A variable controlling how the 2D render API renders lines.
 *
 * The variable can be set to the following values:
 *
 * - "0": Use the default line drawing method (Bresenham's line algorithm)
 * - "1": Use the driver point API using Bresenham's line algorithm (correct,
 *   draws many points)
 * - "2": Use the driver line API (occasionally misses line endpoints based on
 *   hardware driver quirks
 * - "3": Use the driver geometry API (correct, draws thicker diagonal lines)
 *
 * This hint should be set before creating a renderer.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RENDER_LINE_METHOD "SDL_RENDER_LINE_METHOD"

/**
 * A variable controlling whether the Metal render driver select low power
 * device over default one.
 *
 * The variable can be set to the following values:
 *
 * - "0": Use the preferred OS device. (default)
 * - "1": Select a low power device.
 *
 * This hint should be set before creating a renderer.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RENDER_METAL_PREFER_LOW_POWER_DEVICE                          \
  "SDL_RENDER_METAL_PREFER_LOW_POWER_DEVICE"

/**
 * A variable controlling whether updates to the SDL screen surface should be
 * synchronized with the vertical refresh, to avoid tearing.
 *
 * This hint overrides the application preference when creating a renderer.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable vsync. (default)
 * - "1": Enable vsync.
 *
 * This hint should be set before creating a renderer.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RENDER_VSYNC "SDL_RENDER_VSYNC"

/**
 * A variable to control whether the return key on the soft keyboard should
 * hide the soft keyboard on Android and iOS.
 *
 * This hint sets the default value of SDL_PROP_TEXTINPUT_MULTILINE_BOOLEAN.
 *
 * The variable can be set to the following values:
 *
 * - "0": The return key will be handled as a key event. (default)
 * - "1": The return key will hide the keyboard.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RETURN_KEY_HIDES_IME "SDL_RETURN_KEY_HIDES_IME"

/**
 * A variable containing a list of ROG gamepad capable mice.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 *
 * @sa SDL_HINT_ROG_GAMEPAD_MICE_EXCLUDED
 */
#define SDL_HINT_ROG_GAMEPAD_MICE "SDL_ROG_GAMEPAD_MICE"

/**
 * A variable containing a list of devices that are not ROG gamepad capable
 * mice.
 *
 * This will override SDL_HINT_ROG_GAMEPAD_MICE and the built in device list.
 *
 * The format of the string is a comma separated list of USB VID/PID pairs in
 * hexadecimal form, e.g.
 *
 * `0xAAAA/0xBBBB,0xCCCC/0xDDDD`
 *
 * The variable can also take the form of "@file", in which case the named
 * file will be loaded and interpreted as the value of the variable.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_ROG_GAMEPAD_MICE_EXCLUDED "SDL_ROG_GAMEPAD_MICE_EXCLUDED"

/**
 * A variable controlling which Dispmanx layer to use on a Raspberry PI.
 *
 * Also known as Z-order. The variable can take a negative or positive value.
 * The default is 10000.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_RPI_VIDEO_LAYER "SDL_RPI_VIDEO_LAYER"

/**
 * Specify an "activity name" for screensaver inhibition.
 *
 * Some platforms, notably Linux desktops, list the applications which are
 * inhibiting the screensaver or other power-saving features.
 *
 * This hint lets you specify the "activity name" sent to the OS when
 * SDL_DisableScreenSaver() is used (or the screensaver is automatically
 * disabled). The contents of this hint are used when the screensaver is
 * disabled. You should use a string that describes what your program is doing
 * (and, therefore, why the screensaver is disabled). For example, "Playing a
 * game" or "Watching a video".
 *
 * Setting this to "" or leaving it unset will have SDL use a reasonable
 * default: "Playing a game" or something similar.
 *
 * This hint should be set before calling SDL_DisableScreenSaver()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_SCREENSAVER_INHIBIT_ACTIVITY_NAME                             \
  "SDL_SCREENSAVER_INHIBIT_ACTIVITY_NAME"

/**
 * A variable controlling whether SDL calls dbus_shutdown() on quit.
 *
 * This is useful as a debug tool to validate memory leaks, but shouldn't ever
 * be set in production applications, as other libraries used by the
 * application might use dbus under the hood and this can cause crashes if
 * they continue after Quit().
 *
 * The variable can be set to the following values:
 *
 * - "0": SDL will not call dbus_shutdown() on quit. (default)
 * - "1": SDL will call dbus_shutdown() on quit.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_SHUTDOWN_DBUS_ON_QUIT "SDL_SHUTDOWN_DBUS_ON_QUIT"

/**
 * A variable that specifies a backend to use for title storage.
 *
 * By default, SDL will try all available storage backends in a reasonable
 * order until it finds one that can work, but this hint allows the app or
 * user to force a specific target, such as "pc" if, say, you are on Steam but
 * want to avoid SteamRemoteStorage for title data.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_STORAGE_TITLE_DRIVER "SDL_STORAGE_TITLE_DRIVER"

/**
 * A variable that specifies a backend to use for user storage.
 *
 * By default, SDL will try all available storage backends in a reasonable
 * order until it finds one that can work, but this hint allows the app or
 * user to force a specific target, such as "pc" if, say, you are on Steam but
 * want to avoid SteamRemoteStorage for user data.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_STORAGE_USER_DRIVER "SDL_STORAGE_USER_DRIVER"

/**
 * Specifies whether SDL_THREAD_PRIORITY_TIME_CRITICAL should be treated as
 * realtime.
 *
 * On some platforms, like Linux, a realtime priority thread may be subject to
 * restrictions that require special handling by the application. This hint
 * exists to let SDL know that the app is prepared to handle said
 * restrictions.
 *
 * On Linux, SDL will apply the following configuration to any thread that
 * becomes realtime:
 *
 * - The SCHED_RESET_ON_FORK bit will be set on the scheduling policy,
 * - An RLIMIT_RTTIME budget will be configured to the rtkit specified limit.
 * - Exceeding this limit will result in the kernel sending SIGKILL to the
 *   app, refer to the man pages for more information.
 *
 * The variable can be set to the following values:
 *
 * - "0": default platform specific behaviour
 * - "1": Force SDL_THREAD_PRIORITY_TIME_CRITICAL to a realtime scheduling
 *   policy
 *
 * This hint should be set before calling SDL_SetCurrentThreadPriority()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_THREAD_FORCE_REALTIME_TIME_CRITICAL                           \
  "SDL_THREAD_FORCE_REALTIME_TIME_CRITICAL"

/**
 * A string specifying additional information to use with
 * SDL_SetCurrentThreadPriority.
 *
 * By default SDL_SetCurrentThreadPriority will make appropriate system
 * changes in order to apply a thread priority. For example on systems using
 * pthreads the scheduler policy is changed automatically to a policy that
 * works well with a given priority. Code which has specific requirements can
 * override SDL's default behavior with this hint.
 *
 * pthread hint values are "current", "other", "fifo" and "rr". Currently no
 * other platform hint values are defined but may be in the future.
 *
 * On Linux, the kernel may send SIGKILL to realtime tasks which exceed the
 * distro configured execution budget for rtkit. This budget can be queried
 * through RLIMIT_RTTIME after calling SDL_SetCurrentThreadPriority().
 *
 * This hint should be set before calling SDL_SetCurrentThreadPriority()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_THREAD_PRIORITY_POLICY "SDL_THREAD_PRIORITY_POLICY"

/**
 * A variable that controls the timer resolution, in milliseconds.
 *
 * The higher resolution the timer, the more frequently the CPU services timer
 * interrupts, and the more precise delays are, but this takes up power and
 * CPU time. This hint is only used on Windows.
 *
 * See this blog post for more information:
 * http://randomascii.wordpress.com/2013/07/08/windows-timer-resolution-megawatts-wasted/
 *
 * The default value is "1".
 *
 * If this variable is set to "0", the system timer resolution is not set.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_TIMER_RESOLUTION "SDL_TIMER_RESOLUTION"

/**
 * A variable controlling whether touch events should generate synthetic mouse
 * events.
 *
 * The variable can be set to the following values:
 *
 * - "0": Touch events will not generate mouse events.
 * - "1": Touch events will generate mouse events. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_TOUCH_MOUSE_EVENTS "SDL_TOUCH_MOUSE_EVENTS"

/**
 * A variable controlling whether trackpads should be treated as touch
 * devices.
 *
 * On macOS (and possibly other platforms in the future), SDL will report
 * touches on a trackpad as mouse input, which is generally what users expect
 * from this device; however, these are often actually full multitouch-capable
 * touch devices, so it might be preferable to some apps to treat them as
 * such.
 *
 * The variable can be set to the following values:
 *
 * - "0": Trackpad will send mouse events. (default)
 * - "1": Trackpad will send touch events.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_TRACKPAD_IS_TOUCH_ONLY "SDL_TRACKPAD_IS_TOUCH_ONLY"

/**
 * A variable controlling whether the Android / tvOS remotes should be listed
 * as joystick devices, instead of sending keyboard events.
 *
 * The variable can be set to the following values:
 *
 * - "0": Remotes send enter/escape/arrow key events.
 * - "1": Remotes are available as 2 axis, 2 button joysticks. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_TV_REMOTE_AS_JOYSTICK "SDL_TV_REMOTE_AS_JOYSTICK"

/**
 * A variable controlling whether the screensaver is enabled.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable screensaver. (default)
 * - "1": Enable screensaver.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_ALLOW_SCREENSAVER "SDL_VIDEO_ALLOW_SCREENSAVER"

/**
 * A comma separated list containing the names of the displays that SDL should
 * sort to the front of the display list.
 *
 * When this hint is set, displays with matching name strings will be
 * prioritized in the list of displays, as exposed by calling
 * SDL_GetDisplays(), with the first listed becoming the primary display. The
 * naming convention can vary depending on the environment, but it is usually
 * a connector name (e.g. 'DP-1', 'DP-2', 'HDMI-A-1',etc...).
 *
 * On Wayland and X11 desktops, the connector names associated with displays
 * can typically be found by using the `xrandr` utility.
 *
 * This hint is currently supported on the following drivers:
 *
 * - KMSDRM (kmsdrm)
 * - Wayland (wayland)
 * - X11 (x11)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_DISPLAY_PRIORITY "SDL_VIDEO_DISPLAY_PRIORITY"

/**
 * Tell the video driver that we only want a double buffer.
 *
 * By default, most lowlevel 2D APIs will use a triple buffer scheme that
 * wastes no CPU time on waiting for vsync after issuing a flip, but
 * introduces a frame of latency. On the other hand, using a double buffer
 * scheme instead is recommended for cases where low latency is an important
 * factor because we save a whole frame of latency.
 *
 * We do so by waiting for vsync immediately after issuing a flip, usually
 * just after eglSwapBuffers call in the backend's *_SwapWindow function.
 *
 * This hint is currently supported on the following drivers:
 *
 * - Raspberry Pi (raspberrypi)
 * - Wayland (wayland)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_DOUBLE_BUFFER "SDL_VIDEO_DOUBLE_BUFFER"

/**
 * A variable that specifies a video backend to use.
 *
 * By default, SDL will try all available video backends in a reasonable order
 * until it finds one that can work, but this hint allows the app or user to
 * force a specific target, such as "x11" if, say, you are on Wayland but want
 * to try talking to the X server instead.
 *
 * This hint accepts a comma-separated list of driver names, and each will be
 * tried in the order listed during init, until one succeeds or all of them
 * fail.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_DRIVER "SDL_VIDEO_DRIVER"

/**
 * A variable controlling whether the dummy video driver saves output frames.
 *
 * - "0": Video frames are not saved to disk. (default)
 * - "1": Video frames are saved to files in the format "SDL_windowX-Y.bmp",
 *   where X is the window ID, and Y is the frame number.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_DUMMY_SAVE_FRAMES "SDL_VIDEO_DUMMY_SAVE_FRAMES"

/**
 * If eglGetPlatformDisplay fails, fall back to calling eglGetDisplay.
 *
 * The variable can be set to one of the following values:
 *
 * - "0": Do not fall back to eglGetDisplay.
 * - "1": Fall back to eglGetDisplay if eglGetPlatformDisplay fails. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_EGL_ALLOW_GETDISPLAY_FALLBACK                           \
  "SDL_VIDEO_EGL_ALLOW_GETDISPLAY_FALLBACK"

/**
 * A variable controlling whether the OpenGL context should be created with
 * EGL.
 *
 * The variable can be set to the following values:
 *
 * - "0": Use platform-specific GL context creation API (GLX, WGL, CGL, etc).
 *   (default)
 * - "1": Use EGL
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_FORCE_EGL "SDL_VIDEO_FORCE_EGL"

/**
 * A variable that specifies the policy for fullscreen Spaces on macOS.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable Spaces support (FULLSCREEN_DESKTOP won't use them and
 *   SDL_WINDOW_RESIZABLE windows won't offer the "fullscreen" button on their
 *   titlebars).
 * - "1": Enable Spaces support (FULLSCREEN_DESKTOP will use them and
 *   SDL_WINDOW_RESIZABLE windows will offer the "fullscreen" button on their
 *   titlebars). (default)
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES "SDL_VIDEO_MAC_FULLSCREEN_SPACES"

/**
 * A variable that specifies the menu visibility when a window is fullscreen
 * in Spaces on macOS.
 *
 * The variable can be set to the following values:
 *
 * - "0": The menu will be hidden when the window is in a fullscreen space,
 *   and not accessible by moving the mouse to the top of the screen.
 * - "1": The menu will be accessible when the window is in a fullscreen
 *   space.
 * - "auto": The menu will be hidden if fullscreen mode was toggled on
 *   programmatically via `SDL_SetWindowFullscreen()`, and accessible if
 *   fullscreen was entered via the "fullscreen" button on the window title
 *   bar. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_MAC_FULLSCREEN_MENU_VISIBILITY                          \
  "SDL_VIDEO_MAC_FULLSCREEN_MENU_VISIBILITY"

/**
 * A variable controlling whether fullscreen windows are minimized when they
 * lose focus.
 *
 * The variable can be set to the following values:
 *
 * - "0": Fullscreen windows will not be minimized when they lose focus.
 *   (default)
 * - "1": Fullscreen windows are minimized when they lose focus.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS "SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS"

/**
 * A variable controlling whether the offscreen video driver saves output
 * frames.
 *
 * This only saves frames that are generated using software rendering, not
 * accelerated OpenGL rendering.
 *
 * - "0": Video frames are not saved to disk. (default)
 * - "1": Video frames are saved to files in the format "SDL_windowX-Y.bmp",
 *   where X is the window ID, and Y is the frame number.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_OFFSCREEN_SAVE_FRAMES "SDL_VIDEO_OFFSCREEN_SAVE_FRAMES"

/**
 * A variable controlling whether all window operations will block until
 * complete.
 *
 * Window systems that run asynchronously may not have the results of window
 * operations that resize or move the window applied immediately upon the
 * return of the requesting function. Setting this hint will cause such
 * operations to block after every call until the pending operation has
 * completed. Setting this to '1' is the equivalent of calling
 * SDL_SyncWindow() after every function call.
 *
 * Be aware that amount of time spent blocking while waiting for window
 * operations to complete can be quite lengthy, as animations may have to
 * complete, which can take upwards of multiple seconds in some cases.
 *
 * The variable can be set to the following values:
 *
 * - "0": Window operations are non-blocking. (default)
 * - "1": Window operations will block until completed.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS "SDL_VIDEO_SYNC_WINDOW_OPERATIONS"

/**
 * A variable controlling whether the libdecor Wayland backend is allowed to
 * be used.
 *
 * libdecor is used over xdg-shell when xdg-decoration protocol is
 * unavailable.
 *
 * The variable can be set to the following values:
 *
 * - "0": libdecor use is disabled.
 * - "1": libdecor use is enabled. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR "SDL_VIDEO_WAYLAND_ALLOW_LIBDECOR"

/**
 * A variable controlling whether video mode emulation is enabled under
 * Wayland.
 *
 * When this hint is set, a standard set of emulated CVT video modes will be
 * exposed for use by the application. If it is disabled, the only modes
 * exposed will be the logical desktop size and, in the case of a scaled
 * desktop, the native display resolution.
 *
 * The variable can be set to the following values:
 *
 * - "0": Video mode emulation is disabled.
 * - "1": Video mode emulation is enabled. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_WAYLAND_MODE_EMULATION "SDL_VIDEO_WAYLAND_MODE_EMULATION"

/**
 * A variable controlling how modes with a non-native aspect ratio are
 * displayed under Wayland.
 *
 * When this hint is set, the requested scaling will be used when displaying
 * fullscreen video modes that don't match the display's native aspect ratio.
 * This is contingent on compositor viewport support.
 *
 * The variable can be set to the following values:
 *
 * - "aspect" - Video modes will be displayed scaled, in their proper aspect
 *   ratio, with black bars.
 * - "stretch" - Video modes will be scaled to fill the entire display.
 *   (default)
 * - "none" - Video modes will be displayed as 1:1 with no scaling.
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_WAYLAND_MODE_SCALING "SDL_VIDEO_WAYLAND_MODE_SCALING"

/**
 * A variable controlling whether the libdecor Wayland backend is preferred
 * over native decorations.
 *
 * When this hint is set, libdecor will be used to provide window decorations,
 * even if xdg-decoration is available. (Note that, by default, libdecor will
 * use xdg-decoration itself if available).
 *
 * The variable can be set to the following values:
 *
 * - "0": libdecor is enabled only if server-side decorations are unavailable.
 *   (default)
 * - "1": libdecor is always enabled if available.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR                                 \
  "SDL_VIDEO_WAYLAND_PREFER_LIBDECOR"

/**
 * A variable forcing non-DPI-aware Wayland windows to output at 1:1 scaling.
 *
 * This must be set before initializing the video subsystem.
 *
 * When this hint is set, Wayland windows that are not flagged as being
 * DPI-aware will be output with scaling designed to force 1:1 pixel mapping.
 *
 * This is intended to allow legacy applications to be displayed without
 * desktop scaling being applied, and has issues with certain display
 * configurations, as this forces the window to behave in a way that Wayland
 * desktops were not designed to accommodate:
 *
 * - Rounding errors can result with odd window sizes and/or desktop scales,
 *   which can cause the window contents to appear slightly blurry.
 * - Positioning the window may be imprecise due to unit conversions and
 *   rounding.
 * - The window may be unusably small on scaled desktops.
 * - The window may jump in size when moving between displays of different
 *   scale factors.
 * - Displays may appear to overlap when using a multi-monitor setup with
 *   scaling enabled.
 * - Possible loss of cursor precision due to the logical size of the window
 *   being reduced.
 *
 * New applications should be designed with proper DPI awareness handling
 * instead of enabling this.
 *
 * The variable can be set to the following values:
 *
 * - "0": Windows will be scaled normally.
 * - "1": Windows will be forced to scale to achieve 1:1 output.
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_WAYLAND_SCALE_TO_DISPLAY                                \
  "SDL_VIDEO_WAYLAND_SCALE_TO_DISPLAY"

/**
 * A variable specifying which shader compiler to preload when using the
 * Chrome ANGLE binaries.
 *
 * SDL has EGL and OpenGL ES2 support on Windows via the ANGLE project. It can
 * use two different sets of binaries, those compiled by the user from source
 * or those provided by the Chrome browser. In the later case, these binaries
 * require that SDL loads a DLL providing the shader compiler.
 *
 * The variable can be set to the following values:
 *
 * - "d3dcompiler_46.dll" - best for Vista or later. (default)
 * - "d3dcompiler_43.dll" - for XP support.
 * - "none" - do not load any library, useful if you compiled ANGLE from
 *   source and included the compiler in your binaries.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_WIN_D3DCOMPILER "SDL_VIDEO_WIN_D3DCOMPILER"

#if SDL_VERSION_ATLEAST(3, 2, 10)

/**
 * A variable controlling whether SDL should call XSelectInput() to enable
 * input events on X11 windows wrapped by SDL windows.
 *
 * The variable can be set to the following values:
 *
 * - "0": Don't call XSelectInput(), assuming the native window code has done
 *   it already.
 * - "1": Call XSelectInput() to enable input events. (default)
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.10.
 */
#define SDL_HINT_VIDEO_X11_EXTERNAL_WINDOW_INPUT                               \
  "SDL_VIDEO_X11_EXTERNAL_WINDOW_INPUT"

#endif // SDL_VERSION_ATLEAST(3, 2, 10)

/**
 * A variable controlling whether the X11 _NET_WM_BYPASS_COMPOSITOR hint
 * should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable _NET_WM_BYPASS_COMPOSITOR.
 * - "1": Enable _NET_WM_BYPASS_COMPOSITOR. (default)
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR                            \
  "SDL_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR"

/**
 * A variable controlling whether the X11 _NET_WM_PING protocol should be
 * supported.
 *
 * By default SDL will use _NET_WM_PING, but for applications that know they
 * will not always be able to respond to ping requests in a timely manner they
 * can turn it off to avoid the window manager thinking the app is hung.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable _NET_WM_PING.
 * - "1": Enable _NET_WM_PING. (default)
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_X11_NET_WM_PING "SDL_VIDEO_X11_NET_WM_PING"

/**
 * A variable controlling whether SDL uses DirectColor visuals.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable DirectColor visuals.
 * - "1": Enable DirectColor visuals. (default)
 *
 * This hint should be set before initializing the video subsystem.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_X11_NODIRECTCOLOR "SDL_VIDEO_X11_NODIRECTCOLOR"

/**
 * A variable forcing the content scaling factor for X11 displays.
 *
 * The variable can be set to a floating point value in the range 1.0-10.0f
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_X11_SCALING_FACTOR "SDL_VIDEO_X11_SCALING_FACTOR"

/**
 * A variable forcing the visual ID used for X11 display modes.
 *
 * This hint should be set before initializing the video subsystem.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_X11_VISUALID "SDL_VIDEO_X11_VISUALID"

/**
 * A variable forcing the visual ID chosen for new X11 windows.
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_X11_WINDOW_VISUALID "SDL_VIDEO_X11_WINDOW_VISUALID"

/**
 * A variable controlling whether the X11 XRandR extension should be used.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable XRandR.
 * - "1": Enable XRandR. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VIDEO_X11_XRANDR "SDL_VIDEO_X11_XRANDR"

/**
 * A variable controlling whether touch should be enabled on the back panel of
 * the PlayStation Vita.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable touch on the back panel.
 * - "1": Enable touch on the back panel. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VITA_ENABLE_BACK_TOUCH "SDL_VITA_ENABLE_BACK_TOUCH"

/**
 * A variable controlling whether touch should be enabled on the front panel
 * of the PlayStation Vita.
 *
 * The variable can be set to the following values:
 *
 * - "0": Disable touch on the front panel.
 * - "1": Enable touch on the front panel. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VITA_ENABLE_FRONT_TOUCH "SDL_VITA_ENABLE_FRONT_TOUCH"

/**
 * A variable controlling the module path on the PlayStation Vita.
 *
 * This hint defaults to "app0:module"
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VITA_MODULE_PATH "SDL_VITA_MODULE_PATH"

/**
 * A variable controlling whether to perform PVR initialization on the
 * PlayStation Vita.
 *
 * - "0": Skip PVR initialization.
 * - "1": Perform the normal PVR initialization. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VITA_PVR_INIT "SDL_VITA_PVR_INIT"

/**
 * A variable overriding the resolution reported on the PlayStation Vita.
 *
 * The variable can be set to the following values:
 *
 * - "544": 544p (default)
 * - "720": 725p for PSTV
 * - "1080": 1088i for PSTV
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VITA_RESOLUTION "SDL_VITA_RESOLUTION"

/**
 * A variable controlling whether OpenGL should be used instead of OpenGL ES
 * on the PlayStation Vita.
 *
 * The variable can be set to the following values:
 *
 * - "0": Use OpenGL ES. (default)
 * - "1": Use OpenGL.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VITA_PVR_OPENGL "SDL_VITA_PVR_OPENGL"

/**
 * A variable controlling which touchpad should generate synthetic mouse
 * events.
 *
 * The variable can be set to the following values:
 *
 * - "0": Only front touchpad should generate mouse events. (default)
 * - "1": Only back touchpad should generate mouse events.
 * - "2": Both touchpads should generate mouse events.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VITA_TOUCH_MOUSE_DEVICE "SDL_VITA_TOUCH_MOUSE_DEVICE"

/**
 * A variable overriding the display index used in SDL_Vulkan_CreateSurface()
 *
 * The display index starts at 0, which is the default.
 *
 * This hint should be set before calling SDL_Vulkan_CreateSurface()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VULKAN_DISPLAY "SDL_VULKAN_DISPLAY"

/**
 * Specify the Vulkan library to load.
 *
 * This hint should be set before creating a Vulkan window or calling
 * SDL_Vulkan_LoadLibrary().
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_VULKAN_LIBRARY "SDL_VULKAN_LIBRARY"

/**
 * A variable controlling how the fact chunk affects the loading of a WAVE
 * file.
 *
 * The fact chunk stores information about the number of samples of a WAVE
 * file. The Standards Update from Microsoft notes that this value can be used
 * to 'determine the length of the data in seconds'. This is especially useful
 * for compressed formats (for which this is a mandatory chunk) if they
 * produce multiple sample frames per block and truncating the block is not
 * allowed. The fact chunk can exactly specify how many sample frames there
 * should be in this case.
 *
 * Unfortunately, most application seem to ignore the fact chunk and so SDL
 * ignores it by default as well.
 *
 * The variable can be set to the following values:
 *
 * - "truncate" - Use the number of samples to truncate the wave data if the
 *   fact chunk is present and valid.
 * - "strict" - Like "truncate", but raise an error if the fact chunk is
 *   invalid, not present for non-PCM formats, or if the data chunk doesn't
 *   have that many samples.
 * - "ignorezero" - Like "truncate", but ignore fact chunk if the number of
 *   samples is zero.
 * - "ignore" - Ignore fact chunk entirely. (default)
 *
 * This hint should be set before calling SDL_LoadWAV() or SDL_LoadWAV_IO()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WAVE_FACT_CHUNK "SDL_WAVE_FACT_CHUNK"

/**
 * A variable controlling the maximum number of chunks in a WAVE file.
 *
 * This sets an upper bound on the number of chunks in a WAVE file to avoid
 * wasting time on malformed or corrupt WAVE files. This defaults to "10000".
 *
 * This hint should be set before calling SDL_LoadWAV() or SDL_LoadWAV_IO()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WAVE_CHUNK_LIMIT "SDL_WAVE_CHUNK_LIMIT"

/**
 * A variable controlling how the size of the RIFF chunk affects the loading
 * of a WAVE file.
 *
 * The size of the RIFF chunk (which includes all the sub-chunks of the WAVE
 * file) is not always reliable. In case the size is wrong, it's possible to
 * just ignore it and step through the chunks until a fixed limit is reached.
 *
 * Note that files that have trailing data unrelated to the WAVE file or
 * corrupt files may slow down the loading process without a reliable
 * boundary. By default, SDL stops after 10000 chunks to prevent wasting time.
 * Use SDL_HINT_WAVE_CHUNK_LIMIT to adjust this value.
 *
 * The variable can be set to the following values:
 *
 * - "force" - Always use the RIFF chunk size as a boundary for the chunk
 *   search.
 * - "ignorezero" - Like "force", but a zero size searches up to 4 GiB.
 *   (default)
 * - "ignore" - Ignore the RIFF chunk size and always search up to 4 GiB.
 * - "maximum" - Search for chunks until the end of file. (not recommended)
 *
 * This hint should be set before calling SDL_LoadWAV() or SDL_LoadWAV_IO()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WAVE_RIFF_CHUNK_SIZE "SDL_WAVE_RIFF_CHUNK_SIZE"

/**
 * A variable controlling how a truncated WAVE file is handled.
 *
 * A WAVE file is considered truncated if any of the chunks are incomplete or
 * the data chunk size is not a multiple of the block size. By default, SDL
 * decodes until the first incomplete block, as most applications seem to do.
 *
 * The variable can be set to the following values:
 *
 * - "verystrict" - Raise an error if the file is truncated.
 * - "strict" - Like "verystrict", but the size of the RIFF chunk is ignored.
 * - "dropframe" - Decode until the first incomplete sample frame.
 * - "dropblock" - Decode until the first incomplete block. (default)
 *
 * This hint should be set before calling SDL_LoadWAV() or SDL_LoadWAV_IO()
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WAVE_TRUNCATION "SDL_WAVE_TRUNCATION"

/**
 * A variable controlling whether the window is activated when the
 * SDL_RaiseWindow function is called.
 *
 * The variable can be set to the following values:
 *
 * - "0": The window is not activated when the SDL_RaiseWindow function is
 *   called.
 * - "1": The window is activated when the SDL_RaiseWindow function is called.
 *   (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOW_ACTIVATE_WHEN_RAISED "SDL_WINDOW_ACTIVATE_WHEN_RAISED"

/**
 * A variable controlling whether the window is activated when the
 * SDL_ShowWindow function is called.
 *
 * The variable can be set to the following values:
 *
 * - "0": The window is not activated when the SDL_ShowWindow function is
 *   called.
 * - "1": The window is activated when the SDL_ShowWindow function is called.
 *   (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOW_ACTIVATE_WHEN_SHOWN "SDL_WINDOW_ACTIVATE_WHEN_SHOWN"

/**
 * If set to "0" then never set the top-most flag on an SDL Window even if the
 * application requests it.
 *
 * This is a debugging aid for developers and not expected to be used by end
 * users.
 *
 * The variable can be set to the following values:
 *
 * - "0": don't allow topmost
 * - "1": allow topmost (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOW_ALLOW_TOPMOST "SDL_WINDOW_ALLOW_TOPMOST"

/**
 * A variable controlling whether the window frame and title bar are
 * interactive when the cursor is hidden.
 *
 * The variable can be set to the following values:
 *
 * - "0": The window frame is not interactive when the cursor is hidden (no
 *   move, resize, etc).
 * - "1": The window frame is interactive when the cursor is hidden. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOW_FRAME_USABLE_WHILE_CURSOR_HIDDEN                       \
  "SDL_WINDOW_FRAME_USABLE_WHILE_CURSOR_HIDDEN"

/**
 * A variable controlling whether SDL generates window-close events for Alt+F4
 * on Windows.
 *
 * The variable can be set to the following values:
 *
 * - "0": SDL will only do normal key handling for Alt+F4.
 * - "1": SDL will generate a window-close event when it sees Alt+F4.
 *   (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_CLOSE_ON_ALT_F4 "SDL_WINDOWS_CLOSE_ON_ALT_F4"

/**
 * A variable controlling whether menus can be opened with their keyboard
 * shortcut (Alt+mnemonic).
 *
 * If the mnemonics are enabled, then menus can be opened by pressing the Alt
 * key and the corresponding mnemonic (for example, Alt+F opens the File
 * menu). However, in case an invalid mnemonic is pressed, Windows makes an
 * audible beep to convey that nothing happened. This is true even if the
 * window has no menu at all!
 *
 * Because most SDL applications don't have menus, and some want to use the
 * Alt key for other purposes, SDL disables mnemonics (and the beeping) by
 * default.
 *
 * Note: This also affects keyboard events: with mnemonics enabled, when a
 * menu is opened from the keyboard, you will not receive a KEYUP event for
 * the mnemonic key, and *might* not receive one for Alt.
 *
 * The variable can be set to the following values:
 *
 * - "0": Alt+mnemonic does nothing, no beeping. (default)
 * - "1": Alt+mnemonic opens menus, invalid mnemonics produce a beep.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_ENABLE_MENU_MNEMONICS                                 \
  "SDL_WINDOWS_ENABLE_MENU_MNEMONICS"

/**
 * A variable controlling whether the windows message loop is processed by
 * SDL.
 *
 * The variable can be set to the following values:
 *
 * - "0": The window message loop is not run.
 * - "1": The window message loop is processed in SDL_PumpEvents(). (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_ENABLE_MESSAGELOOP "SDL_WINDOWS_ENABLE_MESSAGELOOP"

/**
 * A variable controlling whether GameInput is used for raw keyboard and mouse
 * on Windows.
 *
 * The variable can be set to the following values:
 *
 * - "0": GameInput is not used for raw keyboard and mouse events.
 * - "1": GameInput is used for raw keyboard and mouse events, if available.
 *   (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_GAMEINPUT "SDL_WINDOWS_GAMEINPUT"

/**
 * A variable controlling whether raw keyboard events are used on Windows.
 *
 * The variable can be set to the following values:
 *
 * - "0": The Windows message loop is used for keyboard events. (default)
 * - "1": Low latency raw keyboard events are used.
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_RAW_KEYBOARD "SDL_WINDOWS_RAW_KEYBOARD"

/**
 * A variable controlling whether SDL uses Kernel Semaphores on Windows.
 *
 * Kernel Semaphores are inter-process and require a context switch on every
 * interaction. On Windows 8 and newer, the WaitOnAddress API is available.
 * Using that and atomics to implement semaphores increases performance. SDL
 * will fall back to Kernel Objects on older OS versions or if forced to by
 * this hint.
 *
 * The variable can be set to the following values:
 *
 * - "0": Use Atomics and WaitOnAddress API when available, otherwise fall
 *   back to Kernel Objects. (default)
 * - "1": Force the use of Kernel Objects in all cases.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_FORCE_SEMAPHORE_KERNEL                                \
  "SDL_WINDOWS_FORCE_SEMAPHORE_KERNEL"

/**
 * A variable to specify custom icon resource id from RC file on Windows
 * platform.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_INTRESOURCE_ICON "SDL_WINDOWS_INTRESOURCE_ICON"

/**
 * A variable to specify custom icon resource id from RC file on Windows
 * platform.
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_INTRESOURCE_ICON_SMALL                                \
  "SDL_WINDOWS_INTRESOURCE_ICON_SMALL"

/**
 * A variable controlling whether SDL uses the D3D9Ex API introduced in
 * Windows Vista, instead of normal D3D9.
 *
 * Direct3D 9Ex contains changes to state management that can eliminate device
 * loss errors during scenarios like Alt+Tab or UAC prompts. D3D9Ex may
 * require some changes to your application to cope with the new behavior, so
 * this is disabled by default.
 *
 * For more information on Direct3D 9Ex, see:
 *
 * -
 * https://docs.microsoft.com/en-us/windows/win32/direct3darticles/graphics-apis-in-windows-vista#direct3d-9ex
 * -
 * https://docs.microsoft.com/en-us/windows/win32/direct3darticles/direct3d-9ex-improvements
 *
 * The variable can be set to the following values:
 *
 * - "0": Use the original Direct3D 9 API. (default)
 * - "1": Use the Direct3D 9Ex API on Vista and later (and fall back if D3D9Ex
 *   is unavailable)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_USE_D3D9EX "SDL_WINDOWS_USE_D3D9EX"

/**
 * A variable controlling whether SDL will clear the window contents when the
 * WM_ERASEBKGND message is received.
 *
 * The variable can be set to the following values:
 *
 * - "0"/"never": Never clear the window.
 * - "1"/"initial": Clear the window when the first WM_ERASEBKGND event fires.
 *   (default)
 * - "2"/"always": Clear the window on every WM_ERASEBKGND event.
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_WINDOWS_ERASE_BACKGROUND_MODE                                 \
  "SDL_WINDOWS_ERASE_BACKGROUND_MODE"

/**
 * A variable controlling whether X11 windows are marked as override-redirect.
 *
 * If set, this _might_ increase framerate at the expense of the desktop not
 * working as expected. Override-redirect windows aren't noticed by the window
 * manager at all.
 *
 * You should probably only use this for fullscreen windows, and you probably
 * shouldn't even use it for that. But it's here if you want to try!
 *
 * The variable can be set to the following values:
 *
 * - "0": Do not mark the window as override-redirect. (default)
 * - "1": Mark the window as override-redirect.
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_X11_FORCE_OVERRIDE_REDIRECT "SDL_X11_FORCE_OVERRIDE_REDIRECT"

/**
 * A variable specifying the type of an X11 window.
 *
 * During SDL_CreateWindow, SDL uses the _NET_WM_WINDOW_TYPE X11 property to
 * report to the window manager the type of window it wants to create. This
 * might be set to various things if SDL_WINDOW_TOOLTIP or
 * SDL_WINDOW_POPUP_MENU, etc, were specified. For "normal" windows that
 * haven't set a specific type, this hint can be used to specify a custom
 * type. For example, a dock window might set this to
 * "_NET_WM_WINDOW_TYPE_DOCK".
 *
 * This hint should be set before creating a window.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_X11_WINDOW_TYPE "SDL_X11_WINDOW_TYPE"

/**
 * Specify the XCB library to load for the X11 driver.
 *
 * The default is platform-specific, often "libX11-xcb.so.1".
 *
 * This hint should be set before initializing the video subsystem.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_X11_XCB_LIBRARY "SDL_X11_XCB_LIBRARY"

/**
 * A variable controlling whether XInput should be used for controller
 * handling.
 *
 * The variable can be set to the following values:
 *
 * - "0": XInput is not enabled.
 * - "1": XInput is enabled. (default)
 *
 * This hint should be set before SDL is initialized.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_XINPUT_ENABLED "SDL_XINPUT_ENABLED"

/**
 * A variable controlling response to SDL_assert failures.
 *
 * The variable can be set to the following case-sensitive values:
 *
 * - "abort": Program terminates immediately.
 * - "break": Program triggers a debugger breakpoint.
 * - "retry": Program reruns the SDL_assert's test again.
 * - "ignore": Program continues on, ignoring this assertion failure this
 *   time.
 * - "always_ignore": Program continues on, ignoring this assertion failure
 *   for the rest of the run.
 *
 * Note that SetAssertionHandler offers a programmatic means to deal with
 * assertion failures through a callback, and this hint is largely intended to
 * be used via environment variables by end users and automated tools.
 *
 * This hint should be set before an assertion failure is triggered and can be
 * changed at any time.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_ASSERT "SDL_ASSERT"

/**
 * A variable controlling whether pen events should generate synthetic mouse
 * events.
 *
 * The variable can be set to the following values:
 *
 * - "0": Pen events will not generate mouse events.
 * - "1": Pen events will generate mouse events. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_PEN_MOUSE_EVENTS "SDL_PEN_MOUSE_EVENTS"

/**
 * A variable controlling whether pen events should generate synthetic touch
 * events.
 *
 * The variable can be set to the following values:
 *
 * - "0": Pen events will not generate touch events.
 * - "1": Pen events will generate touch events. (default)
 *
 * This hint can be set anytime.
 *
 * @since This hint is available since SDL 3.2.0.
 */
#define SDL_HINT_PEN_TOUCH_EVENTS "SDL_PEN_TOUCH_EVENTS"

#endif // SDL3PP_DOC

/**
 * An enumeration of hint priorities.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using HintPriority = SDL_HintPriority;

constexpr HintPriority HINT_DEFAULT = SDL_HINT_DEFAULT; ///< DEFAULT

constexpr HintPriority HINT_NORMAL = SDL_HINT_NORMAL; ///< NORMAL

constexpr HintPriority HINT_OVERRIDE = SDL_HINT_OVERRIDE; ///< OVERRIDE

/**
 * Set a hint with a specific priority.
 *
 * The priority controls the behavior when setting a hint that already has a
 * value. Hints will replace existing hints of their priority and lower.
 * Environment variables are considered to have override priority.
 *
 * @param name the hint to set.
 * @param value the value of the hint variable.
 * @param priority the HintPriority level for the hint.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetHint
 * @sa ResetHint
 * @sa SetHint
 */
inline void SetHintWithPriority(StringParam name,
                                StringParam value,
                                HintPriority priority)
{
  CheckError(SDL_SetHintWithPriority(name, value, priority));
}

/**
 * Set a hint with normal priority.
 *
 * Hints will not be set if there is an existing override hint or environment
 * variable that takes precedence. You can use SetHintWithPriority() to
 * set the hint with override priority instead.
 *
 * @param name the hint to set.
 * @param value the value of the hint variable.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetHint
 * @sa ResetHint
 * @sa SetHintWithPriority
 */
inline void SetHint(StringParam name, StringParam value)
{
  CheckError(SDL_SetHint(name, value));
}

/**
 * Reset a hint to the default value.
 *
 * This will reset a hint to the value of the environment variable, or nullptr
 * if the environment isn't set. Callbacks will be called normally with this
 * change.
 *
 * @param name the hint to set.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetHint
 * @sa ResetHints
 */
inline void ResetHint(StringParam name) { CheckError(SDL_ResetHint(name)); }

/**
 * Reset all hints to the default values.
 *
 * This will reset all hints to the value of the associated environment
 * variable, or nullptr if the environment isn't set. Callbacks will be called
 * normally with this change.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ResetHint
 */
inline void ResetHints() { SDL_ResetHints(); }

/**
 * Get the value of a hint.
 *
 * @param name the hint to query.
 * @returns the string value of a hint or nullptr if the hint isn't set.
 *
 * @threadsafety It is safe to call this function from any thread, however the
 *               return value only remains valid until the hint is changed; if
 *               another thread might do so, the app should supply locks
 *               and/or make a copy of the string. Note that using a hint
 *               callback instead is always thread-safe, as SDL holds a lock
 *               on the thread subsystem during the callback.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetHint
 * @sa SetHintWithPriority
 */
inline const char* GetHint(StringParam name) { return SDL_GetHint(name); }

/**
 * Get the boolean value of a hint variable.
 *
 * @param name the name of the hint to get the boolean value from.
 * @param default_value the value to return if the hint does not exist.
 * @returns the boolean value of a hint or the provided default value if the
 *          hint does not exist.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetHint
 * @sa SetHint
 */
inline bool GetHintBoolean(StringParam name, bool default_value)
{
  return SDL_GetHintBoolean(name, default_value);
}

/**
 * A callback used to send notifications of hint value changes.
 *
 * This is called an initial time during AddHintCallback with the hint's
 * current value, and then again each time the hint's value changes.
 *
 * @param userdata what was passed as `userdata` to AddHintCallback().
 * @param name what was passed as `name` to AddHintCallback().
 * @param oldValue the previous hint value.
 * @param newValue the new value hint is to be set to.
 *
 * @threadsafety This callback is fired from whatever thread is setting a new
 *               hint value. SDL holds a lock on the hint subsystem when
 *               calling this callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AddHintCallback
 */
using HintCallback = SDL_HintCallback;

/**
 * A callback used to send notifications of hint value changes.
 *
 * This is called an initial time during AddHintCallback with the hint's
 * current value, and then again each time the hint's value changes.
 *
 * @param userdata what was passed as `userdata` to AddHintCallback().
 * @param name what was passed as `name` to AddHintCallback().
 * @param oldValue the previous hint value.
 * @param newValue the new value hint is to be set to.
 *
 * @threadsafety This callback is fired from whatever thread is setting a new
 *               hint value. SDL holds a lock on the hint subsystem when
 *               calling this callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AddHintCallback
 * @sa HintCallback
 */
using HintCB = std::function<void(const char*, const char*, const char*)>;

/**
 * Handle returned by AddHintCallback()
 */
struct HintCallbackHandle : CallbackHandle
{
  using CallbackHandle::CallbackHandle;
};

/**
 * Add a function to watch a particular hint.
 *
 * The callback function is called _during_ this function, to provide it an
 * initial value, and again each time the hint's value changes.
 *
 * @param name the hint to watch.
 * @param callback An HintCallback function that will be called when the
 *                 hint value changes.
 * @param userdata a pointer to pass to the callback function.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RemoveHintCallback
 */
inline void AddHintCallback(StringParam name,
                            HintCallback callback,
                            void* userdata)
{
  CheckError(SDL_AddHintCallback(name, callback, userdata));
}

/**
 * Add a function to watch a particular hint.
 *
 * The callback function is called _during_ this function, to provide it an
 * initial value, and again each time the hint's value changes.
 *
 * @param name the hint to watch.
 * @param callback An HintCallback function that will be called when the
 *                 hint value changes.
 * @returns a handle to be used on RemoveHintCallback()
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RemoveHintCallback
 */
inline HintCallbackHandle AddHintCallback(StringParam name, HintCB callback)
{
  using Wrapper = CallbackWrapper<HintCB>;
  auto cb = Wrapper::Wrap(std::move(callback));
  if (!SDL_AddHintCallback(name, &Wrapper::Call, cb)) {
    Wrapper::release(cb);
    throw Error{};
  }
  return HintCallbackHandle{cb};
}

/**
 * Remove a function watching a particular hint.
 *
 * @param name the hint being watched.
 * @param callback an HintCallback function that will be called when the
 *                 hint value changes.
 * @param userdata a pointer being passed to the callback function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddHintCallback
 */
inline void RemoveHintCallback(StringParam name,
                               HintCallback callback,
                               void* userdata)
{
  SDL_RemoveHintCallback(name, callback, userdata);
}

/**
 * Remove a function watching a particular hint.
 *
 * @param name the hint being watched.
 * @param handle the handle for the HintCallback function to be removed
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddHintCallback
 */
inline void RemoveHintCallback(StringParam name, HintCallbackHandle handle)
{
  CallbackWrapper<HintCB>::release(handle);
}

/// @}

/**
 * @defgroup CategoryLog Log Handling
 *
 * Simple log messages with priorities and categories. A message's
 * LogPriority signifies how important the message is. A message's
 * LogCategory signifies from what domain it belongs to. Every category
 * has a minimum priority specified: when a message belongs to that category,
 * it will only be sent out if it has that minimum priority or higher.
 *
 * SDL's own logs are sent below the default priority threshold, so they are
 * quiet by default.
 *
 * You can change the log verbosity programmatically using
 * LogCategory.SetLogPriority() or with SetHint(SDL_HINT_LOGGING, ...), or with
 * the "SDL_LOGGING" environment variable. This variable is a comma separated
 * set of category=level tokens that define the default logging levels for SDL
 * applications.
 *
 * The category can be a numeric category, one of "app", "error", "assert",
 * "system", "audio", "video", "render", "input", "test", or `*` for any
 * unspecified category.
 *
 * The level can be a numeric level, one of "trace", "verbose", "debug",
 * "info", "warn", "error", "critical", or "quiet" to disable that category.
 *
 * You can omit the category if you want to set the logging level for all
 * categories.
 *
 * If this hint isn't set, the default log levels are equivalent to:
 *
 * `app=info,assert=warn,test=verbose,*=error`
 *
 * Here's where the messages go on different platforms:
 *
 * - Windows: debug output stream
 * - Android: log output
 * - Others: standard error output (stderr)
 *
 * You don't need to have a newline (`@n`) on the end of messages, the
 * functions will do that for you. For consistent behavior cross-platform, you
 * shouldn't have any newlines in messages, such as to log multiple lines in
 * one call; unusual platform-specific behavior can be observed in such usage.
 * Do one log call per line instead, with no newlines in messages.
 *
 * Each log call is atomic, so you won't see log messages cut off one another
 * when logging from multiple threads.
 *
 * @{
 */

using LogCategoryRaw = SDL_LogCategory;

/**
 * @name LogPriorities
 *
 * The priorities assignable for a LogCategory.
 *
 * @{
 */

/**
 * The predefined log priorities
 *
 * @since This enum is available since SDL 3.2.0.
 */
using LogPriority = SDL_LogPriority;

constexpr LogPriority LOG_PRIORITY_INVALID =
  SDL_LOG_PRIORITY_INVALID; ///< INVALID

constexpr LogPriority LOG_PRIORITY_TRACE = SDL_LOG_PRIORITY_TRACE; ///< TRACE

constexpr LogPriority LOG_PRIORITY_VERBOSE =
  SDL_LOG_PRIORITY_VERBOSE; ///< VERBOSE

constexpr LogPriority LOG_PRIORITY_DEBUG = SDL_LOG_PRIORITY_DEBUG; ///< DEBUG

constexpr LogPriority LOG_PRIORITY_INFO = SDL_LOG_PRIORITY_INFO; ///< INFO

constexpr LogPriority LOG_PRIORITY_WARN = SDL_LOG_PRIORITY_WARN; ///< WARN

constexpr LogPriority LOG_PRIORITY_ERROR = SDL_LOG_PRIORITY_ERROR; ///< ERROR

constexpr LogPriority LOG_PRIORITY_CRITICAL =
  SDL_LOG_PRIORITY_CRITICAL; ///< CRITICAL

constexpr LogPriority LOG_PRIORITY_COUNT = SDL_LOG_PRIORITY_COUNT; ///< COUNT

/// @}

/**
 * @name LogCategories
 *
 * The logging categories. see LogCategory for more info
 *
 * @{
 */

/**
 * The predefined log categories
 *
 * By default the application and gpu categories are enabled at the INFO
 * level, the assert category is enabled at the WARN level, test is enabled at
 * the VERBOSE level and all other categories are enabled at the ERROR level.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @cat wrap-state
 *
 * @sa wrap-state
 */
class LogCategory
{
  SDL_LogCategory m_category;

public:
  /**
   * Wraps LogCategory.
   *
   * @param category the value to be wrapped
   */
  constexpr LogCategory(LogCategoryRaw category = SDL_LOG_CATEGORY_APPLICATION)
    : m_category(category)
  {
  }

  /**
   * Wraps LogCategory.
   *
   * @param category the value to be wrapped
   */
  constexpr explicit LogCategory(int category)
    : m_category(SDL_LogCategory(category))
  {
  }

  /**
   * Default comparison operator
   */
  constexpr auto operator<=>(const LogCategory& other) const = default;

  /**
   * Compares with the underlying type
   */
  constexpr auto operator<=>(LogCategoryRaw category) const
  {
    return operator<=>(LogCategory(category));
  }

  /**
   * Unwraps to the underlying LogCategory.
   *
   * @returns the underlying LogCategory.
   */
  constexpr operator SDL_LogCategory() const { return m_category; }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return m_category != 0; }

  /**
   * Set the priority of a particular log category.
   *
   * @param priority the LogPriority to assign.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LogCategory.GetLogPriority
   * @sa ResetLogPriorities
   * @sa SetLogPriorities
   */
  void SetLogPriority(LogPriority priority) const
  {
    SDL_SetLogPriority(m_category, priority);
  }

  /**
   * Get the priority of a particular log category.
   *
   * @param category the category to query.
   * @returns the LogPriority for the requested category.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LogCategory.SetLogPriority
   */
  LogPriority GetLogPriority() const { return SDL_GetLogPriority(m_category); }

  /**
   * Log an unformatted message with the specified priority.
   *
   * @param priority the priority of the message.
   * @param message string to output.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL::Log()
   * @sa Log()
   * @sa LogCritical()
   * @sa LogDebug()
   * @sa LogError()
   * @sa LogInfo()
   * @sa LogTrace()
   * @sa LogVerbose()
   * @sa LogWarn()
   */
  void LogUnformatted(LogPriority priority, StringParam message) const
  {
    SDL_LogMessage(m_category, priority, "%s", (const char*)(message));
  }

  /**
   * Log a message with the specified priority.
   *
   * @param priority the priority of the message.
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogMessage(LogPriority priority,
                  std::string_view fmt,
                  ARGS... args) const
  {
    LogUnformatted(priority, std::vformat(fmt, std::make_format_args(args))...);
  }

  /**
   * Log a message with LOG_PRIORITY_TRACE.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogTrace(std::string_view fmt, ARGS&&... args) const
  {
    LogMessage(SDL_LOG_PRIORITY_TRACE, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with LOG_PRIORITY_VERBOSE.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogVerbose(std::string_view fmt, ARGS&&... args) const
  {
    LogMessage(SDL_LOG_PRIORITY_VERBOSE, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with LOG_PRIORITY_DEBUG.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogDebug(std::string_view fmt, ARGS&&... args) const
  {
    LogMessage(SDL_LOG_PRIORITY_DEBUG, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with LOG_PRIORITY_INFO.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogInfo(std::string_view fmt, ARGS&&... args) const
  {
    LogMessage(SDL_LOG_PRIORITY_INFO, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with LOG_PRIORITY_WARN.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   */
  template<class... ARGS>
  void LogWarn(std::string_view fmt, ARGS&&... args) const
  {
    LogMessage(SDL_LOG_PRIORITY_WARN, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with LOG_PRIORITY_ERROR.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogError(std::string_view fmt, ARGS&&... args) const
  {
    LogMessage(SDL_LOG_PRIORITY_ERROR, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with LOG_PRIORITY_CRITICAL.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa formatted-string
   * @sa LogCategory.LogDebug()
   * @sa LogCategory.LogError()
   * @sa LogCategory.LogInfo()
   * @sa LogUnformatted()
   * @sa LogCategory.LogUnformatted()
   * @sa LogCategory.LogTrace()
   * @sa LogCategory.LogVerbose()
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogCritical(std::string_view fmt, ARGS&&... args) const
  {
    LogMessage(SDL_LOG_PRIORITY_CRITICAL, fmt, std::forward<ARGS>(args)...);
  }
};

constexpr LogCategory LOG_CATEGORY_APPLICATION =
  SDL_LOG_CATEGORY_APPLICATION; ///< APPLICATION

constexpr LogCategory LOG_CATEGORY_ERROR = SDL_LOG_CATEGORY_ERROR; ///< ERROR

constexpr LogCategory LOG_CATEGORY_ASSERT = SDL_LOG_CATEGORY_ASSERT; ///< ASSERT

constexpr LogCategory LOG_CATEGORY_SYSTEM = SDL_LOG_CATEGORY_SYSTEM; ///< SYSTEM

constexpr LogCategory LOG_CATEGORY_AUDIO = SDL_LOG_CATEGORY_AUDIO; ///< AUDIO

constexpr LogCategory LOG_CATEGORY_VIDEO = SDL_LOG_CATEGORY_VIDEO; ///< VIDEO

constexpr LogCategory LOG_CATEGORY_RENDER = SDL_LOG_CATEGORY_RENDER; ///< RENDER

constexpr LogCategory LOG_CATEGORY_INPUT = SDL_LOG_CATEGORY_INPUT; ///< INPUT

constexpr LogCategory LOG_CATEGORY_TEST = SDL_LOG_CATEGORY_TEST; ///< TEST

constexpr LogCategory LOG_CATEGORY_GPU = SDL_LOG_CATEGORY_GPU; ///< GPU

constexpr LogCategory LOG_CATEGORY_RESERVED2 =
  SDL_LOG_CATEGORY_RESERVED2; ///< RESERVED2

constexpr LogCategory LOG_CATEGORY_RESERVED3 =
  SDL_LOG_CATEGORY_RESERVED3; ///< RESERVED3

constexpr LogCategory LOG_CATEGORY_RESERVED4 =
  SDL_LOG_CATEGORY_RESERVED4; ///< RESERVED4

constexpr LogCategory LOG_CATEGORY_RESERVED5 =
  SDL_LOG_CATEGORY_RESERVED5; ///< RESERVED5

constexpr LogCategory LOG_CATEGORY_RESERVED6 =
  SDL_LOG_CATEGORY_RESERVED6; ///< RESERVED6

constexpr LogCategory LOG_CATEGORY_RESERVED7 =
  SDL_LOG_CATEGORY_RESERVED7; ///< RESERVED7

constexpr LogCategory LOG_CATEGORY_RESERVED8 =
  SDL_LOG_CATEGORY_RESERVED8; ///< RESERVED8

constexpr LogCategory LOG_CATEGORY_RESERVED9 =
  SDL_LOG_CATEGORY_RESERVED9; ///< RESERVED9

constexpr LogCategory LOG_CATEGORY_RESERVED10 =
  SDL_LOG_CATEGORY_RESERVED10; ///< RESERVED10

constexpr LogCategory LOG_CATEGORY_CUSTOM = SDL_LOG_CATEGORY_CUSTOM; ///< CUSTOM

/**
 * Set the priority of all log categories.
 *
 * @param priority the LogPriority to assign.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ResetLogPriorities
 * @sa LogCategory.SetLogPriority
 */
inline void SetLogPriorities(LogPriority priority)
{
  SDL_SetLogPriorities(priority);
}

/**
 * Set the priority of a particular log category.
 *
 * @param category the category to assign a priority to.
 * @param priority the LogPriority to assign.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LogCategory.GetLogPriority
 * @sa ResetLogPriorities
 * @sa SetLogPriorities
 */
inline void SetLogPriority(int category, LogPriority priority)
{
  SDL_SetLogPriority(category, priority);
}

/**
 * Get the priority of a particular log category.
 *
 * @param category the category to query.
 * @returns the LogPriority for the requested category.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LogCategory.SetLogPriority
 */
inline LogPriority GetLogPriority(int category)
{
  return SDL_GetLogPriority(category);
}

/**
 * Reset all priorities to default.
 *
 * This is called by Quit().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetLogPriorities
 * @sa LogCategory.SetLogPriority
 */
inline void ResetLogPriorities() { SDL_ResetLogPriorities(); }

/**
 * Set the text prepended to log messages of a given priority.
 *
 * By default LOG_PRIORITY_INFO and below have no prefix, and
 * LOG_PRIORITY_WARN and higher have a prefix showing their priority, e.g.
 * "WARNING: ".
 *
 * @param priority the LogPriority to modify.
 * @param prefix the prefix to use for that log priority, or nullptr to use no
 *               prefix.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LogCategory.SetLogPriorities
 * @sa LogCategory.SetLogPriority
 */
inline void SetLogPriorityPrefix(LogPriority priority, StringParam prefix)
{
  CheckError(SDL_SetLogPriorityPrefix(priority, prefix));
}

/**
 * Log a message with LOG_CATEGORY_APPLICATION and LOG_PRIORITY_INFO.
 *
 * @param fmt a
 *            [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
 *            style message format string.
 * @param args additional parameters matching the `{}` tokens in the format
 *             string, if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat formatted-string
 *
 * @sa formatted-string
 * @sa LogUnformatted()
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void Log(std::string_view fmt, ARGS&&... args)
{
  LOG_CATEGORY_APPLICATION.LogInfo(fmt, std::forward<ARGS>(args)...);
}

/**
 * Log a message with LOG_PRIORITY_TRACE.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param ... additional parameters matching % tokens in the **fmt** string,
 *            if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogTrace(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  category.LogTrace(fmt, std::forward<ARGS>(args)...);
}

/**
 * Log a message with LOG_PRIORITY_VERBOSE.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param ... additional parameters matching % tokens in the **fmt** string,
 *            if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogVerbose(LogCategory category,
                       std::string_view fmt,
                       ARGS&&... args)
{
  category.LogVerbose(fmt, std::forward<ARGS>(args)...);
}

/**
 * Log a message with LOG_PRIORITY_DEBUG.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param ... additional parameters matching % tokens in the **fmt** string,
 *            if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogDebug(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  category.LogDebug(fmt, std::forward<ARGS>(args)...);
}

/**
 * Log a message with LOG_PRIORITY_INFO.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param ... additional parameters matching % tokens in the **fmt** string,
 *            if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogInfo(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  category.LogInfo(fmt, std::forward<ARGS>(args)...);
}

/**
 * Log a message with LOG_PRIORITY_WARN.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param ... additional parameters matching % tokens in the **fmt** string,
 *            if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 */
template<class... ARGS>
inline void LogWarn(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  category.LogWarn(fmt, std::forward<ARGS>(args)...);
}

/**
 * Log a message with LOG_PRIORITY_ERROR.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param ... additional parameters matching % tokens in the **fmt** string,
 *            if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogError(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  category.LogError(fmt, std::forward<ARGS>(args)...);
}

/**
 * Log a message with LOG_PRIORITY_CRITICAL.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param ... additional parameters matching % tokens in the **fmt** string,
 *            if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogCritical(LogCategory category,
                        std::string_view fmt,
                        ARGS&&... args)
{
  category.LogCritical(fmt, std::forward<ARGS>(args)...);
}

/**
 * Log a message with the specified category and priority.
 *
 * @param category the category of the message.
 * @param priority the priority of the message.
 * @param fmt a printf() style message format string.
 * @param ... additional parameters matching % tokens in the **fmt** string,
 *            if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogMessage(LogCategory category,
                       LogPriority priority,
                       std::string_view fmt,
                       ARGS... args)
{
  category.LogMessage(priority, fmt, std::forward<ARGS>(args)...);
}

/**
 * Log an unformatted message with LOG_CATEGORY_APPLICATION and
 * LOG_PRIORITY_INFO.
 *
 * @param message string to output.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
inline void LogUnformatted(StringParam message)
{
  SDL_Log("%s", static_cast<const char*>(message));
}

/**
 * The prototype for the log output callback function.
 *
 * This function is called by SDL when there is new text to be logged. A mutex
 * is held so that this function is never called by more than one thread at
 * once.
 *
 * @param userdata what was passed as `userdata` to
 *                 SetLogOutputFunction().
 * @param category the category of the message.
 * @param priority the priority of the message.
 * @param message the message being output.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using LogOutputFunction = SDL_LogOutputFunction;

/**
 * The prototype for the log output callback function.
 *
 * This function is called by SDL when there is new text to be logged. A mutex
 * is held so that this function is never called by more than one thread at
 * once.
 *
 * @param category the category of the message.
 * @param priority the priority of the message.
 * @param message the message being output.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa LogOutputFunction
 */
using LogOutputCB = std::function<void(LogCategory, LogPriority, const char*)>;

/**
 * Get the default log output function.
 *
 * @returns the default log output callback.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetLogOutputFunction()
 * @sa GetLogOutputFunction()
 */
inline LogOutputFunction GetDefaultLogOutputFunction()
{
  return SDL_GetDefaultLogOutputFunction();
}

/**
 * Get the current log output function.
 *
 * @param callback an LogOutputFunction filled in with the current log callback.
 * @param userdata a pointer filled in with the pointer that is passed to
 *                 `callback`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDefaultLogOutputFunction()
 * @sa SetLogOutputFunction()
 */
inline void GetLogOutputFunction(LogOutputFunction* callback, void** userdata)
{
  SDL_GetLogOutputFunction(callback, userdata);
}

/**
 * Get the current log output function.
 *
 * @returns the LogOutputCB currently set
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa listener-callback
 * @sa GetDefaultLogOutputFunction
 * @sa SetLogOutputFunction
 */
inline LogOutputCB GetLogOutputFunction()
{
  using Wrapper = UniqueCallbackWrapper<LogOutputCB>;
  LogOutputFunction cb;
  void* userdata;
  GetLogOutputFunction(&cb, &userdata);
  if (userdata == nullptr) {
    return [cb](LogCategory c, LogPriority p, StringParam m) {
      cb(nullptr, c, p, m);
    };
  }
  if (auto cb = Wrapper::at(userdata)) return cb;
  return [cb, userdata](LogCategory c, LogPriority p, StringParam m) {
    cb(userdata, c, p, m);
  };
}

/**
 * Replace the default log output function with one of your own.
 *
 * @param callback an LogOutputFunction to call instead of the default.
 * @param userdata a pointer that is passed to `callback`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDefaultLogOutputFunction
 * @sa GetLogOutputFunction
 * @sa ResetLogOutputFunction
 */
inline void SetLogOutputFunction(LogOutputFunction callback, void* userdata)
{
  UniqueCallbackWrapper<LogOutputCB>::erase();
  return SDL_SetLogOutputFunction(callback, userdata);
}

/**
 * Replace the default log output function with one of your own.
 *
 * @param callback an LogOutputFunction to call instead of the default.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa listener-callback
 * @sa GetDefaultLogOutputFunction
 * @sa GetLogOutputFunction
 * @sa ResetLogOutputFunction
 */
inline void SetLogOutputFunction(LogOutputCB callback)
{
  using Wrapper = UniqueCallbackWrapper<LogOutputCB>;
  SDL_SetLogOutputFunction(
    [](
      void* userdata, int category, LogPriority priority, const char* message) {
      return Wrapper::Call(userdata, LogCategory{category}, priority, message);
    },
    Wrapper::Wrap(std::move(callback)));
}

/**
 * Replace the current log output function with the default one
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDefaultLogOutputFunction
 * @sa GetLogOutputFunction
 */
inline void ResetLogOutputFunction()
{
  return SetLogOutputFunction(GetDefaultLogOutputFunction(), nullptr);
}

/// @}

/**
 * @defgroup CategoryPixels Pixel Formats and Conversion Routines
 *
 * SDL offers facilities for pixel management.
 *
 * Largely these facilities deal with pixel _format_: what does this set of
 * bits represent?
 *
 * If you mostly want to think of a pixel as some combination of red, green,
 * blue, and maybe alpha intensities, this is all pretty straightforward, and
 * in many cases, is enough information to build a perfectly fine game.
 *
 * However, the actual definition of a pixel is more complex than that:
 *
 * Pixels are a representation of a color in a particular color space.
 *
 * The first characteristic of a color space is the color type. SDL
 * understands two different color types, RGB and YCbCr, or in SDL also
 * referred to as YUV.
 *
 * RGB colors consist of red, green, and blue channels of color that are added
 * together to represent the colors we see on the screen.
 *
 * https://en.wikipedia.org/wiki/RGB_color_model
 *
 * YCbCr colors represent colors as a Y luma brightness component and red and
 * blue chroma color offsets. This color representation takes advantage of the
 * fact that the human eye is more sensitive to brightness than the color in
 * an image. The Cb and Cr components are often compressed and have lower
 * resolution than the luma component.
 *
 * https://en.wikipedia.org/wiki/YCbCr
 *
 * When the color information in YCbCr is compressed, the Y pixels are left at
 * full resolution and each Cr and Cb pixel represents an average of the color
 * information in a block of Y pixels. The chroma location determines where in
 * that block of pixels the color information is coming from.
 *
 * The color range defines how much of the pixel to use when converting a
 * pixel into a color on the display. When the full color range is used, the
 * entire numeric range of the pixel bits is significant. When narrow color
 * range is used, for historical reasons, the pixel uses only a portion of the
 * numeric range to represent colors.
 *
 * The color primaries and white point are a definition of the colors in the
 * color space relative to the standard XYZ color space.
 *
 * https://en.wikipedia.org/wiki/CIE_1931_color_space
 *
 * The transfer characteristic, or opto-electrical transfer function (OETF),
 * is the way a color is converted from mathematically linear space into a
 * non-linear output signals.
 *
 * https://en.wikipedia.org/wiki/Rec._709#Transfer_characteristics
 *
 * The matrix coefficients are used to convert between YCbCr and RGB colors.
 *
 * @{
 */

// Forward decl
struct Palette;

using PaletteRaw = SDL_Palette*;

/**
 * Safely wrap Palette for non owning parameters
 */
struct PaletteParam
{
  PaletteRaw value;

  constexpr PaletteParam(PaletteRaw value)
    : value(value)
  {
  }

  constexpr PaletteParam(std::nullptr_t _)
    : value(nullptr)
  {
  }

  constexpr operator PaletteRaw() const { return value; }
};

/**
 * Safely wrap Palette for non owning const parameters
 */
struct PaletteConstParam
{
  const PaletteRaw value;

  constexpr PaletteConstParam(const PaletteRaw value)
    : value(value)
  {
  }

  constexpr PaletteConstParam(PaletteParam value)
    : value(value.value)
  {
  }

  constexpr PaletteConstParam(std::nullptr_t _)
    : value(nullptr)
  {
  }

  constexpr operator const PaletteRaw() const { return value; }
};

using PixelFormatRaw = SDL_PixelFormat;

using ColorRaw = SDL_Color;

using FColorRaw = SDL_FColor;

// Forward decl
struct Color;

/**
 * Details about the format of a pixel.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PixelFormatDetails = SDL_PixelFormatDetails;

#ifdef SDL3PP_DOC

/**
 * A fully opaque 8-bit alpha value.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_ALPHA_TRANSPARENT
 */
#define SDL_ALPHA_OPAQUE 255

/**
 * A fully opaque floating point alpha value.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_ALPHA_TRANSPARENT_FLOAT
 */
#define SDL_ALPHA_OPAQUE_FLOAT 1.0f

/**
 * A fully transparent 8-bit alpha value.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_ALPHA_OPAQUE
 */
#define SDL_ALPHA_TRANSPARENT 0

/**
 * A fully transparent floating point alpha value.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_ALPHA_OPAQUE_FLOAT
 */
#define SDL_ALPHA_TRANSPARENT_FLOAT 0.0f

#endif // SDL3PP_DOC

/**
 * @name PixelTypes
 * @{
 */

/**
 * Pixel type.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PixelType = SDL_PixelType;

constexpr PixelType PIXELTYPE_UNKNOWN = SDL_PIXELTYPE_UNKNOWN; ///< UNKNOWN

constexpr PixelType PIXELTYPE_INDEX1 = SDL_PIXELTYPE_INDEX1; ///< INDEX1

constexpr PixelType PIXELTYPE_INDEX4 = SDL_PIXELTYPE_INDEX4; ///< INDEX4

constexpr PixelType PIXELTYPE_INDEX8 = SDL_PIXELTYPE_INDEX8; ///< INDEX8

constexpr PixelType PIXELTYPE_PACKED8 = SDL_PIXELTYPE_PACKED8; ///< PACKED8

constexpr PixelType PIXELTYPE_PACKED16 = SDL_PIXELTYPE_PACKED16; ///< PACKED16

constexpr PixelType PIXELTYPE_PACKED32 = SDL_PIXELTYPE_PACKED32; ///< PACKED32

constexpr PixelType PIXELTYPE_ARRAYU8 = SDL_PIXELTYPE_ARRAYU8; ///< ARRAYU8

constexpr PixelType PIXELTYPE_ARRAYU16 = SDL_PIXELTYPE_ARRAYU16; ///< ARRAYU16

constexpr PixelType PIXELTYPE_ARRAYU32 = SDL_PIXELTYPE_ARRAYU32; ///< ARRAYU32

constexpr PixelType PIXELTYPE_ARRAYF16 = SDL_PIXELTYPE_ARRAYF16; ///< ARRAYF16

constexpr PixelType PIXELTYPE_ARRAYF32 = SDL_PIXELTYPE_ARRAYF32; ///< ARRAYF32

constexpr PixelType PIXELTYPE_INDEX2 = SDL_PIXELTYPE_INDEX2; ///< INDEX2

/// @}

/**
 * @name BitmapOrders
 * @{
 */

/**
 * Bitmap pixel order, high bit -> low bit.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using BitmapOrder = SDL_BitmapOrder;

constexpr BitmapOrder BITMAPORDER_NONE = SDL_BITMAPORDER_NONE; ///< NONE

constexpr BitmapOrder BITMAPORDER_4321 = SDL_BITMAPORDER_4321; ///< 4321

constexpr BitmapOrder BITMAPORDER_1234 = SDL_BITMAPORDER_1234; ///< 1234

/// @}

/**
 * @name PackedOrders
 * @{
 */

/**
 * Packed component order, high bit -> low bit.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PackedOrder = SDL_PackedOrder;

constexpr PackedOrder PACKEDORDER_NONE = SDL_PACKEDORDER_NONE; ///< NONE

constexpr PackedOrder PACKEDORDER_XRGB = SDL_PACKEDORDER_XRGB; ///< XRGB

constexpr PackedOrder PACKEDORDER_RGBX = SDL_PACKEDORDER_RGBX; ///< RGBX

constexpr PackedOrder PACKEDORDER_ARGB = SDL_PACKEDORDER_ARGB; ///< ARGB

constexpr PackedOrder PACKEDORDER_RGBA = SDL_PACKEDORDER_RGBA; ///< RGBA

constexpr PackedOrder PACKEDORDER_XBGR = SDL_PACKEDORDER_XBGR; ///< XBGR

constexpr PackedOrder PACKEDORDER_BGRX = SDL_PACKEDORDER_BGRX; ///< BGRX

constexpr PackedOrder PACKEDORDER_ABGR = SDL_PACKEDORDER_ABGR; ///< ABGR

constexpr PackedOrder PACKEDORDER_BGRA = SDL_PACKEDORDER_BGRA; ///< BGRA

/// @}

/**
 * @name ArrayOrders
 * @{
 */

/**
 * Array component order, low byte -> high byte.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ArrayOrder = SDL_ArrayOrder;

constexpr ArrayOrder ARRAYORDER_NONE = SDL_ARRAYORDER_NONE; ///< NONE

constexpr ArrayOrder ARRAYORDER_RGB = SDL_ARRAYORDER_RGB; ///< RGB

constexpr ArrayOrder ARRAYORDER_RGBA = SDL_ARRAYORDER_RGBA; ///< RGBA

constexpr ArrayOrder ARRAYORDER_ARGB = SDL_ARRAYORDER_ARGB; ///< ARGB

constexpr ArrayOrder ARRAYORDER_BGR = SDL_ARRAYORDER_BGR; ///< BGR

constexpr ArrayOrder ARRAYORDER_BGRA = SDL_ARRAYORDER_BGRA; ///< BGRA

constexpr ArrayOrder ARRAYORDER_ABGR = SDL_ARRAYORDER_ABGR; ///< ABGR

/// @}

/**
 * @name PackedLayouts
 * @{
 */

/**
 * Packed component layout.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PackedLayout = SDL_PackedLayout;

constexpr PackedLayout PACKEDLAYOUT_NONE = SDL_PACKEDLAYOUT_NONE; ///< NONE

constexpr PackedLayout PACKEDLAYOUT_332 = SDL_PACKEDLAYOUT_332; ///< 332

constexpr PackedLayout PACKEDLAYOUT_4444 = SDL_PACKEDLAYOUT_4444; ///< 4444

constexpr PackedLayout PACKEDLAYOUT_1555 = SDL_PACKEDLAYOUT_1555; ///< 1555

constexpr PackedLayout PACKEDLAYOUT_5551 = SDL_PACKEDLAYOUT_5551; ///< 5551

constexpr PackedLayout PACKEDLAYOUT_565 = SDL_PACKEDLAYOUT_565; ///< 565

constexpr PackedLayout PACKEDLAYOUT_8888 = SDL_PACKEDLAYOUT_8888; ///< 8888

constexpr PackedLayout PACKEDLAYOUT_2101010 =
  SDL_PACKEDLAYOUT_2101010; ///< 2101010

constexpr PackedLayout PACKEDLAYOUT_1010102 =
  SDL_PACKEDLAYOUT_1010102; ///< 1010102

/// @}

#ifdef SDL3PP_DOC

/**
 * A macro for defining custom FourCC pixel formats.
 *
 * For example, defining PIXELFORMAT_YV12 looks like this:
 *
 * ```c
 * SDL_DEFINE_PIXELFOURCC('Y', 'V', '1', '2')
 * ```
 *
 * @param A the first character of the FourCC code.
 * @param B the second character of the FourCC code.
 * @param C the third character of the FourCC code.
 * @param D the fourth character of the FourCC code.
 * @returns a format value in the style of PixelFormat.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_DEFINE_PIXELFOURCC(A, B, C, D) SDL_FOURCC(A, B, C, D)

#endif // SDL3PP_DOC

/**
 * @name PixelFormats
 * @{
 */

/**
 * Pixel format.
 *
 * SDL's pixel formats have the following naming convention:
 *
 * - Names with a list of components and a single bit count, such as RGB24 and
 *   ABGR32, define a platform-independent encoding into bytes in the order
 *   specified. For example, in RGB24 data, each pixel is encoded in 3 bytes
 *   (red, green, blue) in that order, and in ABGR32 data, each pixel is
 *   encoded in 4 bytes (alpha, blue, green, red) in that order. Use these
 *   names if the property of a format that is important to you is the order
 *   of the bytes in memory or on disk.
 * - Names with a bit count per component, such as ARGB8888 and XRGB1555, are
 *   "packed" into an appropriately-sized integer in the platform's native
 *   endianness. For example, ARGB8888 is a sequence of 32-bit integers; in
 *   each integer, the most significant bits are alpha, and the least
 *   significant bits are blue. On a little-endian CPU such as x86, the least
 *   significant bits of each integer are arranged first in memory, but on a
 *   big-endian CPU such as s390x, the most significant bits are arranged
 *   first. Use these names if the property of a format that is important to
 *   you is the meaning of each bit position within a native-endianness
 *   integer.
 * - In indexed formats such as INDEX4LSB, each pixel is represented by
 *   encoding an index into the palette into the indicated number of bits,
 *   with multiple pixels packed into each byte if appropriate. In LSB
 *   formats, the first (leftmost) pixel is stored in the least-significant
 *   bits of the byte; in MSB formats, it's stored in the most-significant
 *   bits. INDEX8 does not need LSB/MSB variants, because each pixel exactly
 *   fills one byte.
 *
 * The 32-bit byte-array encodings such as RGBA32 are aliases for the
 * appropriate 8888 encoding for the current platform. For example, RGBA32 is
 * an alias for ABGR8888 on little-endian CPUs like x86, or an alias for
 * RGBA8888 on big-endian CPUs.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @cat wrap-state
 *
 * @sa wrap-state
 * @sa PixelFormats
 */
class PixelFormat
{
  SDL_PixelFormat m_format;

public:
  /**
   * Wraps PixelFormat.
   *
   * @param format the value to be wrapped
   */
  constexpr PixelFormat(PixelFormatRaw format = {})
    : m_format(format)
  {
  }

  /**
   * Defining custom non-FourCC pixel formats.
   *
   * For example, defining PIXELFORMAT_RGBA8888 looks like this:
   *
   * ```c
   * PixelFormat(PIXELTYPE_PACKED32, PACKEDORDER_RGBA,
   * PACKEDLAYOUT_8888, 32, 4)
   * ```
   *
   * @param type the type of the new format, probably a PixelType value.
   * @param order the order of the new format, probably a BitmapOrder,
   *              PackedOrder, or ArrayOrder value.
   * @param layout the layout of the new format, probably an PackedLayout
   *               value or zero.
   * @param bits the number of bits per pixel of the new format.
   * @param bytes the number of bytes per pixel of the new format.
   * @post a format value in the style of PixelFormat.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr PixelFormat(PixelType type,
                        int order,
                        PackedLayout layout,
                        int bits,
                        int bytes)
    : m_format(PixelFormatRaw(
        SDL_DEFINE_PIXELFORMAT(type, order, layout, bits, bytes)))
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const PixelFormat& other) const = default;

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(PixelFormatRaw format) const
  {
    return operator==(PixelFormat(format));
  }

  /**
   * Unwraps to the underlying PixelFormat.
   *
   * @returns the underlying PixelFormat.
   */
  constexpr operator SDL_PixelFormat() const { return m_format; }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return m_format != 0; }

  /**
   * Retrieve the type.
   *
   * @returns the type as PixelType.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr PixelType GetType() const
  {
    return PixelType(SDL_PIXELTYPE(m_format));
  }

  /**
   * Retrieve the order.
   *
   * This is usually a value from the BitmapOrder, PackedOrder, or
   * ArrayOrder enumerations, depending on the format type.
   *
   * @returns the order.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr int GetOrder() const { return SDL_PIXELORDER(m_format); }

  /**
   * Retrieve the layout.
   *
   * This is usually a value from the PackedLayout enumeration, or zero if a
   * layout doesn't make sense for the format type.
   *
   * @returns the layout
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr PackedLayout GetLayout() const
  {
    return PackedLayout(SDL_PIXELLAYOUT(m_format));
  }

  /**
   * Determine this's bits per pixel.
   *
   * FourCC formats will report zero here, as it rarely makes sense to measure
   * them per-pixel.
   *
   * @returns the bits-per-pixel.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBytesPerPixel
   */
  constexpr int GetBitsPerPixel() const { return SDL_BITSPERPIXEL(m_format); }

  /**
   * Determine this's bytes per pixel.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * FourCC formats do their best here, but many of them don't have a meaningful
   * measurement of bytes per pixel.
   *
   * @return the bytes-per-pixel.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBitsPerPixel
   */
  constexpr int GetBytesPerPixel() const { return SDL_BYTESPERPIXEL(m_format); }

  /**
   * Determine if this is an indexed format.
   *
   * @returns true if the format is indexed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsIndexed() const
  {
    return SDL_ISPIXELFORMAT_INDEXED(m_format);
  }

  /**
   * Determine if this is a packed format.
   *
   * @returns true if the format is packed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsPacked() const { return SDL_ISPIXELFORMAT_PACKED(m_format); }

  /**
   * Determine if this is an array format.
   *
   * @returns true if the format is an array, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsArray() const { return SDL_ISPIXELFORMAT_ARRAY(m_format); }

  /**
   * Determine if this is a 10-bit format.
   *
   * @returns true if the format is 10-bit, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Is10Bit() const { return SDL_ISPIXELFORMAT_10BIT(m_format); }

  /**
   * Determine if this is a floating point format.
   *
   * @returns true if the format is 10-bit, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFloat() const { return SDL_ISPIXELFORMAT_FLOAT(m_format); }

  /**
   * Determine if this has an alpha channel.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsAlpha() const { return SDL_ISPIXELFORMAT_ALPHA(m_format); }

  /**
   * Determine if this is a "FourCC" format.
   *
   * This covers custom and other unusual formats.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFourCC() const { return SDL_ISPIXELFORMAT_FOURCC(m_format); }

  /**
   * Get the human readable name of a pixel format.
   *
   * @returns the human readable name of the specified pixel format or
   *          "PIXELFORMAT_UNKNOWN" if the format isn't recognized.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const char* GetName() const { return SDL_GetPixelFormatName(m_format); }

  /**
   * Convert one of the enumerated pixel formats to a bpp value and RGBA masks.
   *
   * @param bpp a bits per pixel value; usually 15, 16, or 32.
   * @param Rmask a pointer filled in with the red mask for the format.
   * @param Gmask a pointer filled in with the green mask for the format.
   * @param Bmask a pointer filled in with the blue mask for the format.
   * @param Amask a pointer filled in with the alpha mask for the format.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PixelFormat.ForMasks
   */
  void GetMasks(int* bpp,
                Uint32* Rmask,
                Uint32* Gmask,
                Uint32* Bmask,
                Uint32* Amask) const
  {
    CheckError(
      SDL_GetMasksForPixelFormat(m_format, bpp, Rmask, Gmask, Bmask, Amask));
  }

  /**
   * Convert a bpp value and RGBA masks to an enumerated pixel format.
   *
   * This will return `PIXELFORMAT_UNKNOWN` if the conversion wasn't
   * possible.
   *
   * @param bpp a bits per pixel value; usually 15, 16, or 32.
   * @param Rmask the red mask for the format.
   * @param Gmask the green mask for the format.
   * @param Bmask the blue mask for the format.
   * @param Amask the alpha mask for the format.
   * @returns the PixelFormat value corresponding to the format masks, or
   *          PIXELFORMAT_UNKNOWN if there isn't a match.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PixelFormat.GetMasks
   */
  static PixelFormat ForMasks(int bpp,
                              Uint32 Rmask,
                              Uint32 Gmask,
                              Uint32 Bmask,
                              Uint32 Amask)
  {
    return SDL_GetPixelFormatForMasks(bpp, Rmask, Gmask, Bmask, Amask);
  }

  /**
   * Create an PixelFormatDetails structure corresponding to a pixel format.
   *
   * Returned structure may come from a shared global cache (i.e. not newly
   * allocated), and hence should not be modified, especially the palette. Weird
   * errors such as `Blit combination not supported` may occur.
   *
   * @returns a pointer to a PixelFormatDetails structure or nullptr on
   *          failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const PixelFormatDetails* GetDetails() const
  {
    return SDL_GetPixelFormatDetails(m_format);
  }

  /**
   * Map an RGBA quadruple to a pixel value for a given pixel format.
   *
   * This function maps the RGBA color value to the specified pixel format and
   * returns the pixel value best approximating the given RGBA color value for
   * the given pixel format.
   *
   * If the specified pixel format has no alpha component the alpha value will
   * be ignored (as it will be in formats with a palette).
   *
   * If the format has a palette (8-bit) the index of the closest matching color
   * in the palette will be returned.
   *
   * If the pixel format bpp (color depth) is less than 32-bpp then the unused
   * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
   * format the return value can be assigned to a Uint16, and similarly a Uint8
   * for an 8-bpp format).
   *
   * @param color the color components of the pixel in the range 0-255.
   * @param palette an optional palette for indexed formats, may be NULL.
   * @returns a pixel value.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetPixelFormatDetails()
   * @sa Get()
   * @sa MapRGBA()
   * @sa Surface.MapColor()
   */
  Uint32 Map(Color color, PaletteConstParam palette) const;

  /**
   * Get RGBA values from a pixel in the specified format.
   *
   * This function uses the entire 8-bit [0..255] range when converting color
   * components from pixel formats with less than 8-bits per RGB component
   * (e.g., a completely white pixel in 16-bit RGB565 format would return [0xff,
   * 0xff, 0xff] not [0xf8, 0xfc, 0xf8]).
   *
   * If the surface has no alpha component, the alpha will be returned as 0xff
   * (100% opaque).
   *
   * @param pixel a pixel value.
   * @param palette an optional palette for indexed formats, may be NULL.
   * @returns a color value.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetPixelFormatDetails()
   * @sa GetRGBA()
   * @sa Map()
   */
  Color Get(Uint32 pixel, PaletteConstParam palette) const;
};

constexpr SDL_PixelFormat PIXELFORMAT_UNKNOWN =
  SDL_PIXELFORMAT_UNKNOWN; ///< UNKNOWN

constexpr SDL_PixelFormat PIXELFORMAT_INDEX1LSB =
  SDL_PIXELFORMAT_INDEX1LSB; ///< INDEX1LSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX1MSB =
  SDL_PIXELFORMAT_INDEX1MSB; ///< INDEX1MSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX2LSB =
  SDL_PIXELFORMAT_INDEX2LSB; ///< INDEX2LSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX2MSB =
  SDL_PIXELFORMAT_INDEX2MSB; ///< INDEX2MSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX4LSB =
  SDL_PIXELFORMAT_INDEX4LSB; ///< INDEX4LSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX4MSB =
  SDL_PIXELFORMAT_INDEX4MSB; ///< INDEX4MSB

constexpr SDL_PixelFormat PIXELFORMAT_INDEX8 =
  SDL_PIXELFORMAT_INDEX8; ///< INDEX8

constexpr SDL_PixelFormat PIXELFORMAT_RGB332 =
  SDL_PIXELFORMAT_RGB332; ///< RGB332

constexpr SDL_PixelFormat PIXELFORMAT_XRGB4444 =
  SDL_PIXELFORMAT_XRGB4444; ///< XRGB4444

constexpr SDL_PixelFormat PIXELFORMAT_XBGR4444 =
  SDL_PIXELFORMAT_XBGR4444; ///< XBGR4444

constexpr SDL_PixelFormat PIXELFORMAT_XRGB1555 =
  SDL_PIXELFORMAT_XRGB1555; ///< XRGB1555

constexpr SDL_PixelFormat PIXELFORMAT_XBGR1555 =
  SDL_PIXELFORMAT_XBGR1555; ///< XBGR1555

constexpr SDL_PixelFormat PIXELFORMAT_ARGB4444 =
  SDL_PIXELFORMAT_ARGB4444; ///< ARGB4444

constexpr SDL_PixelFormat PIXELFORMAT_RGBA4444 =
  SDL_PIXELFORMAT_RGBA4444; ///< RGBA4444

constexpr SDL_PixelFormat PIXELFORMAT_ABGR4444 =
  SDL_PIXELFORMAT_ABGR4444; ///< ABGR4444

constexpr SDL_PixelFormat PIXELFORMAT_BGRA4444 =
  SDL_PIXELFORMAT_BGRA4444; ///< BGRA4444

constexpr SDL_PixelFormat PIXELFORMAT_ARGB1555 =
  SDL_PIXELFORMAT_ARGB1555; ///< ARGB1555

constexpr SDL_PixelFormat PIXELFORMAT_RGBA5551 =
  SDL_PIXELFORMAT_RGBA5551; ///< RGBA5551

constexpr SDL_PixelFormat PIXELFORMAT_ABGR1555 =
  SDL_PIXELFORMAT_ABGR1555; ///< ABGR1555

constexpr SDL_PixelFormat PIXELFORMAT_BGRA5551 =
  SDL_PIXELFORMAT_BGRA5551; ///< BGRA5551

constexpr SDL_PixelFormat PIXELFORMAT_RGB565 =
  SDL_PIXELFORMAT_RGB565; ///< RGB565

constexpr SDL_PixelFormat PIXELFORMAT_BGR565 =
  SDL_PIXELFORMAT_BGR565; ///< BGR565

constexpr SDL_PixelFormat PIXELFORMAT_RGB24 = SDL_PIXELFORMAT_RGB24; ///< RGB24

constexpr SDL_PixelFormat PIXELFORMAT_BGR24 = SDL_PIXELFORMAT_BGR24; ///< BGR24

constexpr SDL_PixelFormat PIXELFORMAT_XRGB8888 =
  SDL_PIXELFORMAT_XRGB8888; ///< XRGB8888

constexpr SDL_PixelFormat PIXELFORMAT_RGBX8888 =
  SDL_PIXELFORMAT_RGBX8888; ///< RGBX8888

constexpr SDL_PixelFormat PIXELFORMAT_XBGR8888 =
  SDL_PIXELFORMAT_XBGR8888; ///< XBGR8888

constexpr SDL_PixelFormat PIXELFORMAT_BGRX8888 =
  SDL_PIXELFORMAT_BGRX8888; ///< BGRX8888

constexpr SDL_PixelFormat PIXELFORMAT_ARGB8888 =
  SDL_PIXELFORMAT_ARGB8888; ///< ARGB8888

constexpr SDL_PixelFormat PIXELFORMAT_RGBA8888 =
  SDL_PIXELFORMAT_RGBA8888; ///< RGBA8888

constexpr SDL_PixelFormat PIXELFORMAT_ABGR8888 =
  SDL_PIXELFORMAT_ABGR8888; ///< ABGR8888

constexpr SDL_PixelFormat PIXELFORMAT_BGRA8888 =
  SDL_PIXELFORMAT_BGRA8888; ///< BGRA8888

constexpr SDL_PixelFormat PIXELFORMAT_XRGB2101010 =
  SDL_PIXELFORMAT_XRGB2101010; ///< XRGB2101010

constexpr SDL_PixelFormat PIXELFORMAT_XBGR2101010 =
  SDL_PIXELFORMAT_XBGR2101010; ///< XBGR2101010

constexpr SDL_PixelFormat PIXELFORMAT_ARGB2101010 =
  SDL_PIXELFORMAT_ARGB2101010; ///< ARGB2101010

constexpr SDL_PixelFormat PIXELFORMAT_ABGR2101010 =
  SDL_PIXELFORMAT_ABGR2101010; ///< ABGR2101010

constexpr SDL_PixelFormat PIXELFORMAT_RGB48 = SDL_PIXELFORMAT_RGB48; ///< RGB48

constexpr SDL_PixelFormat PIXELFORMAT_BGR48 = SDL_PIXELFORMAT_BGR48; ///< BGR48

constexpr SDL_PixelFormat PIXELFORMAT_RGBA64 =
  SDL_PIXELFORMAT_RGBA64; ///< RGBA64

constexpr SDL_PixelFormat PIXELFORMAT_ARGB64 =
  SDL_PIXELFORMAT_ARGB64; ///< ARGB64

constexpr SDL_PixelFormat PIXELFORMAT_BGRA64 =
  SDL_PIXELFORMAT_BGRA64; ///< BGRA64

constexpr SDL_PixelFormat PIXELFORMAT_ABGR64 =
  SDL_PIXELFORMAT_ABGR64; ///< ABGR64

constexpr SDL_PixelFormat PIXELFORMAT_RGB48_FLOAT =
  SDL_PIXELFORMAT_RGB48_FLOAT; ///< RGB48_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_BGR48_FLOAT =
  SDL_PIXELFORMAT_BGR48_FLOAT; ///< BGR48_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_RGBA64_FLOAT =
  SDL_PIXELFORMAT_RGBA64_FLOAT; ///< RGBA64_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_ARGB64_FLOAT =
  SDL_PIXELFORMAT_ARGB64_FLOAT; ///< ARGB64_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_BGRA64_FLOAT =
  SDL_PIXELFORMAT_BGRA64_FLOAT; ///< BGRA64_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_ABGR64_FLOAT =
  SDL_PIXELFORMAT_ABGR64_FLOAT; ///< ABGR64_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_RGB96_FLOAT =
  SDL_PIXELFORMAT_RGB96_FLOAT; ///< RGB96_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_BGR96_FLOAT =
  SDL_PIXELFORMAT_BGR96_FLOAT; ///< BGR96_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_RGBA128_FLOAT =
  SDL_PIXELFORMAT_RGBA128_FLOAT; ///< RGBA128_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_ARGB128_FLOAT =
  SDL_PIXELFORMAT_ARGB128_FLOAT; ///< ARGB128_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_BGRA128_FLOAT =
  SDL_PIXELFORMAT_BGRA128_FLOAT; ///< BGRA128_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_ABGR128_FLOAT =
  SDL_PIXELFORMAT_ABGR128_FLOAT; ///< ABGR128_FLOAT

constexpr SDL_PixelFormat PIXELFORMAT_YV12 =
  SDL_PIXELFORMAT_YV12; ///< Planar mode: Y + V + U (3 planes)

constexpr SDL_PixelFormat PIXELFORMAT_IYUV =
  SDL_PIXELFORMAT_IYUV; ///< Planar mode: Y + U + V (3 planes)

constexpr SDL_PixelFormat PIXELFORMAT_YUY2 =
  SDL_PIXELFORMAT_YUY2; ///< Packed mode: Y0+U0+Y1+V0 (1 plane)

constexpr SDL_PixelFormat PIXELFORMAT_UYVY =
  SDL_PIXELFORMAT_UYVY; ///< Packed mode: U0+Y0+V0+Y1 (1 plane)

constexpr SDL_PixelFormat PIXELFORMAT_YVYU =
  SDL_PIXELFORMAT_YVYU; ///< Packed mode: Y0+V0+Y1+U0 (1 plane)

constexpr SDL_PixelFormat PIXELFORMAT_NV12 =
  SDL_PIXELFORMAT_NV12; ///< Planar mode: Y + U/V interleaved (2 planes)

constexpr SDL_PixelFormat PIXELFORMAT_NV21 =
  SDL_PIXELFORMAT_NV21; ///< Planar mode: Y + V/U interleaved (2 planes)

constexpr SDL_PixelFormat PIXELFORMAT_P010 =
  SDL_PIXELFORMAT_P010; ///< Planar mode: Y + U/V interleaved (2 planes)

constexpr SDL_PixelFormat PIXELFORMAT_EXTERNAL_OES =
  SDL_PIXELFORMAT_EXTERNAL_OES; ///< Android video texture format.

#if SDL_VERSION_ATLEAST(3, 2, 10)

constexpr SDL_PixelFormat PIXELFORMAT_MJPG =
  SDL_PIXELFORMAT_MJPG; ///< Motion JPEG.

#endif // SDL_VERSION_ATLEAST(3, 2, 10)

constexpr SDL_PixelFormat PIXELFORMAT_RGBA32 =
  SDL_PIXELFORMAT_RGBA32; ///< RGBA32

constexpr SDL_PixelFormat PIXELFORMAT_ARGB32 =
  SDL_PIXELFORMAT_ARGB32; ///< ARGB32

constexpr SDL_PixelFormat PIXELFORMAT_BGRA32 =
  SDL_PIXELFORMAT_BGRA32; ///< BGRA32

constexpr SDL_PixelFormat PIXELFORMAT_ABGR32 =
  SDL_PIXELFORMAT_ABGR32; ///< ABGR32

constexpr SDL_PixelFormat PIXELFORMAT_RGBX32 =
  SDL_PIXELFORMAT_RGBX32; ///< RGBX32

constexpr SDL_PixelFormat PIXELFORMAT_XRGB32 =
  SDL_PIXELFORMAT_XRGB32; ///< XRGB32

constexpr SDL_PixelFormat PIXELFORMAT_BGRX32 =
  SDL_PIXELFORMAT_BGRX32; ///< BGRX32

constexpr SDL_PixelFormat PIXELFORMAT_XBGR32 =
  SDL_PIXELFORMAT_XBGR32; ///< XBGR32

/// @}

/**
 * A macro for defining custom non-FourCC pixel formats.
 *
 * For example, defining PIXELFORMAT_RGBA8888 looks like this:
 *
 * ```c
 * PixelFormat.PixelFormat(PIXELTYPE_PACKED32, PACKEDORDER_RGBA,
 * PACKEDLAYOUT_8888, 32, 4)
 * ```
 *
 * @param type the type of the new format, probably a PixelType value.
 * @param order the order of the new format, probably a BitmapOrder,
 *              PackedOrder, or ArrayOrder value.
 * @param layout the layout of the new format, probably an PackedLayout
 *               value or zero.
 * @param bits the number of bits per pixel of the new format.
 * @param bytes the number of bytes per pixel of the new format.
 * @returns a format value in the style of PixelFormat.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr PixelFormat DEFINE_PIXELFORMAT(PixelType type,
                                         int order,
                                         PackedLayout layout,
                                         int bits,
                                         int bytes)
{
  return PixelFormatRaw(
    SDL_DEFINE_PIXELFORMAT(type, order, layout, bits, bytes));
}

/**
 * A macro to retrieve the flags of an PixelFormat.
 *
 * This macro is generally not needed directly by an app, which should use
 * specific tests, like PixelFormat.IsFourCC, instead.
 *
 * @param format an PixelFormat to check.
 * @returns the flags of `format`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_PIXELFLAG(format) (((format) >> 28) & 0x0F)

/**
 * A macro to retrieve the type of an PixelFormat.
 *
 * This is usually a value from the PixelType enumeration.
 *
 * @param format an PixelFormat to check.
 * @returns the type of `format`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr PixelType PIXELTYPE(PixelFormatRaw format)
{
  return PixelType(SDL_PIXELTYPE(format));
}

/**
 * A macro to retrieve the order of an PixelFormat.
 *
 * This is usually a value from the BitmapOrder, PackedOrder, or
 * ArrayOrder enumerations, depending on the format type.
 *
 * @param format an PixelFormat to check.
 * @returns the order of `format`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr int PIXELORDER(PixelFormatRaw format)
{
  return SDL_PIXELORDER(format);
}

/**
 * A macro to retrieve the layout of an PixelFormat.
 *
 * This is usually a value from the PackedLayout enumeration, or zero if a
 * layout doesn't make sense for the format type.
 *
 * @param format an PixelFormat to check.
 * @returns the layout of `format`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr PackedLayout PIXELLAYOUT(PixelFormatRaw format)
{
  return PackedLayout(SDL_PIXELLAYOUT(format));
}

/**
 * A macro to determine an PixelFormat's bits per pixel.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * FourCC formats will report zero here, as it rarely makes sense to measure
 * them per-pixel.
 *
 * @param format an PixelFormat to check.
 * @returns the bits-per-pixel of `format`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetBytesPerPixel
 */
constexpr int BITSPERPIXEL(PixelFormatRaw format)
{
  return SDL_BITSPERPIXEL(format);
}

/**
 * A macro to determine an PixelFormat's bytes per pixel.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * FourCC formats do their best here, but many of them don't have a meaningful
 * measurement of bytes per pixel.
 *
 * @param format an PixelFormat to check.
 * @returns the bytes-per-pixel of `format`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetBitsPerPixel
 */
constexpr int BYTESPERPIXEL(PixelFormatRaw format)
{
  return SDL_BYTESPERPIXEL(format);
}

/**
 * A macro to determine if an PixelFormat is an indexed format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is indexed, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr bool ISPIXELFORMAT_INDEXED(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_INDEXED(format);
}

/**
 * A macro to determine if an PixelFormat is a packed format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is packed, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr bool ISPIXELFORMAT_PACKED(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_PACKED(format);
}

/**
 * A macro to determine if an PixelFormat is an array format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is an array, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr bool ISPIXELFORMAT_ARRAY(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_ARRAY(format);
}

/**
 * A macro to determine if an PixelFormat is a 10-bit format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is 10-bit, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr bool ISPIXELFORMAT_10BIT(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_10BIT(format);
}

/**
 * A macro to determine if an PixelFormat is a floating point format.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format is 10-bit, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr bool ISPIXELFORMAT_FLOAT(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_FLOAT(format);
}

/**
 * A macro to determine if an PixelFormat has an alpha channel.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format has alpha, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr bool ISPIXELFORMAT_ALPHA(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_ALPHA(format);
}

/**
 * A macro to determine if an PixelFormat is a "FourCC" format.
 *
 * This covers custom and other unusual formats.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param format an PixelFormat to check.
 * @returns true if the format has alpha, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr bool ISPIXELFORMAT_FOURCC(PixelFormatRaw format)
{
  return SDL_ISPIXELFORMAT_FOURCC(format);
}

/**
 * @name ColorTypes
 * @{
 */

/**
 * Colorspace color type.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ColorType = SDL_ColorType;

constexpr ColorType COLOR_TYPE_UNKNOWN =
  SDL_COLOR_TYPE_UNKNOWN; ///< COLOR_TYPE_UNKNOWN

constexpr ColorType COLOR_TYPE_RGB = SDL_COLOR_TYPE_RGB; ///< COLOR_TYPE_RGB

constexpr ColorType COLOR_TYPE_YCBCR =
  SDL_COLOR_TYPE_YCBCR; ///< COLOR_TYPE_YCBCR

/// @}

/**
 * @name ColorRanges
 * @{
 */

/**
 * Colorspace color range, as described by
 * https://www.itu.int/rec/R-REC-BT.2100-2-201807-I/en
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ColorRange = SDL_ColorRange;

constexpr ColorRange COLOR_RANGE_UNKNOWN =
  SDL_COLOR_RANGE_UNKNOWN; ///< COLOR_RANGE_UNKNOWN

/**
 * Narrow range, e.g. 16-235 for 8-bit RGB and luma, and 16-240 for 8-bit chroma
 */
constexpr ColorRange COLOR_RANGE_LIMITED = SDL_COLOR_RANGE_LIMITED;

/**
 * Full range, e.g. 0-255 for 8-bit RGB and luma, and 1-255 for 8-bit chroma
 */
constexpr ColorRange COLOR_RANGE_FULL = SDL_COLOR_RANGE_FULL;

/// @}

/**
 * @name ColorPrimaries
 * @{
 */

/**
 * Colorspace color primaries, as described by
 * https://www.itu.int/rec/T-REC-H.273-201612-S/en
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ColorPrimaries = SDL_ColorPrimaries;

constexpr ColorPrimaries COLOR_PRIMARIES_UNKNOWN =
  SDL_COLOR_PRIMARIES_UNKNOWN; ///< COLOR_PRIMARIES_UNKNOWN

constexpr ColorPrimaries COLOR_PRIMARIES_BT709 =
  SDL_COLOR_PRIMARIES_BT709; ///< ITU-R BT.709-6.

constexpr ColorPrimaries COLOR_PRIMARIES_UNSPECIFIED =
  SDL_COLOR_PRIMARIES_UNSPECIFIED; ///< COLOR_PRIMARIES_UNSPECIFIED

constexpr ColorPrimaries COLOR_PRIMARIES_BT470M =
  SDL_COLOR_PRIMARIES_BT470M; ///< ITU-R BT.470-6 System M.

constexpr ColorPrimaries COLOR_PRIMARIES_BT470BG =
  SDL_COLOR_PRIMARIES_BT470BG; ///< ITU-R BT.470-6 System B, G / ITU-R BT.601-7
                               ///< 625.

constexpr ColorPrimaries COLOR_PRIMARIES_BT601 =
  SDL_COLOR_PRIMARIES_BT601; ///< ITU-R BT.601-7 525, SMPTE 170M.

/**
 * SMPTE 240M, functionally the same as COLOR_PRIMARIES_BT601.
 */
constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE240 =
  SDL_COLOR_PRIMARIES_SMPTE240;

constexpr ColorPrimaries COLOR_PRIMARIES_GENERIC_FILM =
  SDL_COLOR_PRIMARIES_GENERIC_FILM; ///< Generic film (color filters using
                                    ///< Illuminant C)

constexpr ColorPrimaries COLOR_PRIMARIES_BT2020 =
  SDL_COLOR_PRIMARIES_BT2020; ///< ITU-R BT.2020-2 / ITU-R BT.2100-0.

constexpr ColorPrimaries COLOR_PRIMARIES_XYZ =
  SDL_COLOR_PRIMARIES_XYZ; ///< SMPTE ST 428-1.

constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE431 =
  SDL_COLOR_PRIMARIES_SMPTE431; ///< SMPTE RP 431-2.

constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE432 =
  SDL_COLOR_PRIMARIES_SMPTE432; ///< SMPTE EG 432-1 / DCI P3.

constexpr ColorPrimaries COLOR_PRIMARIES_EBU3213 =
  SDL_COLOR_PRIMARIES_EBU3213; ///< EBU Tech.  3213-E

constexpr ColorPrimaries COLOR_PRIMARIES_CUSTOM =
  SDL_COLOR_PRIMARIES_CUSTOM; ///< COLOR_PRIMARIES_CUSTOM

/// @}

/**
 * @name TransferCharacteristics
 * @{
 */

/**
 * Colorspace transfer characteristics.
 *
 * These are as described by https://www.itu.int/rec/T-REC-H.273-201612-S/en
 *
 * @since This enum is available since SDL 3.2.0.
 */
using TransferCharacteristics = SDL_TransferCharacteristics;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_UNKNOWN =
  SDL_TRANSFER_CHARACTERISTICS_UNKNOWN; ///< TRANSFER_CHARACTERISTICS_UNKNOWN

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT709 =
  SDL_TRANSFER_CHARACTERISTICS_BT709; ///< Rec.  ITU-R BT.709-6 / ITU-R BT1361

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_UNSPECIFIED =
  SDL_TRANSFER_CHARACTERISTICS_UNSPECIFIED; ///< TRANSFER_CHARACTERISTICS_UNSPECIFIED

/**
 * ITU-R BT.470-6 System M / ITU-R BT1700 625 PAL & SECAM.
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA22 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA22;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA28 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA28; ///< ITU-R BT.470-6 System B, G.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT601 =
  SDL_TRANSFER_CHARACTERISTICS_BT601; ///< SMPTE ST 170M / ITU-R BT.601-7 525 or
                                      ///< 625.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE240 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE240; ///< SMPTE ST 240M.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LINEAR =
  SDL_TRANSFER_CHARACTERISTICS_LINEAR; ///< TRANSFER_CHARACTERISTICS_LINEAR

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100; ///< TRANSFER_CHARACTERISTICS_LOG100

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100_SQRT10 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100_SQRT10; ///< TRANSFER_CHARACTERISTICS_LOG100_SQRT10

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_IEC61966 =
  SDL_TRANSFER_CHARACTERISTICS_IEC61966; ///< IEC 61966-2-4.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT1361 =
  SDL_TRANSFER_CHARACTERISTICS_BT1361; ///< ITU-R BT1361 Extended Colour Gamut.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SRGB =
  SDL_TRANSFER_CHARACTERISTICS_SRGB; ///< IEC 61966-2-1 (sRGB or sYCC)

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_10BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_10BIT; ///< ITU-R BT2020 for 10-bit
                                             ///< system.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_12BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_12BIT; ///< ITU-R BT2020 for 12-bit
                                             ///< system.

/**
 * SMPTE ST 2084 for 10-, 12-, 14- and 16-bit systems.
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_PQ =
  SDL_TRANSFER_CHARACTERISTICS_PQ;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE428 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE428; ///< SMPTE ST 428-1.

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_HLG =
  SDL_TRANSFER_CHARACTERISTICS_HLG; ///< ARIB STD-B67, known as "hybrid
                                    ///< log-gamma" (HLG)

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_CUSTOM =
  SDL_TRANSFER_CHARACTERISTICS_CUSTOM; ///< TRANSFER_CHARACTERISTICS_CUSTOM

/// @}

/**
 * @name MatrixCoefficients
 * @{
 */

/**
 * Colorspace matrix coefficients.
 *
 * These are as described by https://www.itu.int/rec/T-REC-H.273-201612-S/en
 *
 * @since This enum is available since SDL 3.2.0.
 */
using MatrixCoefficients = SDL_MatrixCoefficients;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_IDENTITY =
  SDL_MATRIX_COEFFICIENTS_IDENTITY; ///< MATRIX_COEFFICIENTS_IDENTITY

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT709 =
  SDL_MATRIX_COEFFICIENTS_BT709; ///< ITU-R BT.709-6.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_UNSPECIFIED =
  SDL_MATRIX_COEFFICIENTS_UNSPECIFIED; ///< MATRIX_COEFFICIENTS_UNSPECIFIED

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_FCC =
  SDL_MATRIX_COEFFICIENTS_FCC; ///< US FCC Title 47.

/**
 * ITU-R BT.470-6 System B, G / ITU-R BT.601-7 625, functionally the same as
 * MATRIX_COEFFICIENTS_BT601.
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT470BG =
  SDL_MATRIX_COEFFICIENTS_BT470BG;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT601 =
  SDL_MATRIX_COEFFICIENTS_BT601; ///< ITU-R BT.601-7 525.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE240 =
  SDL_MATRIX_COEFFICIENTS_SMPTE240; ///< SMPTE 240M.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_YCGCO =
  SDL_MATRIX_COEFFICIENTS_YCGCO; ///< MATRIX_COEFFICIENTS_YCGCO

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_NCL =
  SDL_MATRIX_COEFFICIENTS_BT2020_NCL; ///< ITU-R BT.2020-2 non-constant
                                      ///< luminance.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_CL =
  SDL_MATRIX_COEFFICIENTS_BT2020_CL; ///< ITU-R BT.2020-2 constant luminance.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE2085 =
  SDL_MATRIX_COEFFICIENTS_SMPTE2085; ///< SMPTE ST 2085.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL; ///< MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL; ///< MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_ICTCP =
  SDL_MATRIX_COEFFICIENTS_ICTCP; ///< ITU-R BT.2100-0 ICTCP.

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CUSTOM =
  SDL_MATRIX_COEFFICIENTS_CUSTOM; ///< MATRIX_COEFFICIENTS_CUSTOM

/// @}

/**
 * @name ChromaLocations
 * @{
 */

/**
 * Colorspace chroma sample location.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ChromaLocation = SDL_ChromaLocation;

constexpr ChromaLocation CHROMA_LOCATION_NONE =
  SDL_CHROMA_LOCATION_NONE; ///< RGB, no chroma sampling.

/**
 * In MPEG-2, MPEG-4, and AVC, Cb and Cr are taken on midpoint of the left-edge
 * of the 2x2 square. In other words, they have the same horizontal location as
 * the top-left pixel, but is shifted one-half pixel down vertically.
 */
constexpr ChromaLocation CHROMA_LOCATION_LEFT = SDL_CHROMA_LOCATION_LEFT;

/**
 * In JPEG/JFIF, H.261, and MPEG-1, Cb and Cr are taken at the center of the 2x2
 * square. In other words, they are offset one-half pixel to the right and
 * one-half pixel down compared to the top-left pixel.
 */
constexpr ChromaLocation CHROMA_LOCATION_CENTER = SDL_CHROMA_LOCATION_CENTER;

/**
 * In HEVC for BT.2020 and BT.2100 content (in particular on Blu-rays), Cb and
 * Cr are sampled at the same location as the group's top-left Y pixel
 * ("co-sited", "co-located").
 */
constexpr ChromaLocation CHROMA_LOCATION_TOPLEFT = SDL_CHROMA_LOCATION_TOPLEFT;

/// @}

#ifdef SDL3PP_DOC

/**
 * A macro for defining custom Colorspace formats.
 *
 * For example, defining COLORSPACE_SRGB looks like this:
 *
 * ```c
 * SDL_DEFINE_COLORSPACE(COLOR_TYPE_RGB,
 *                       COLOR_RANGE_FULL,
 *                       COLOR_PRIMARIES_BT709,
 *                       TRANSFER_CHARACTERISTICS_SRGB,
 *                       MATRIX_COEFFICIENTS_IDENTITY,
 *                       CHROMA_LOCATION_NONE)
 * ```
 *
 * @param type the type of the new format, probably an ColorType value.
 * @param range the range of the new format, probably a ColorRange value.
 * @param primaries the primaries of the new format, probably an
 *                  ColorPrimaries value.
 * @param transfer the transfer characteristics of the new format, probably an
 *                 TransferCharacteristics value.
 * @param matrix the matrix coefficients of the new format, probably an
 *               MatrixCoefficients value.
 * @param chroma the chroma sample location of the new format, probably an
 *               ChromaLocation value.
 * @returns a format value in the style of Colorspace.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_DEFINE_COLORSPACE(                                                 \
  type, range, primaries, transfer, matrix, chroma)                            \
  (((Uint32)(type) << 28) | ((Uint32)(range) << 24) |                          \
   ((Uint32)(chroma) << 20) | ((Uint32)(primaries) << 10) |                    \
   ((Uint32)(transfer) << 5) | ((Uint32)(matrix) << 0))

/**
 * A macro to retrieve the type of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the ColorType for `cspace`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_COLORSPACETYPE(cspace) (SDL_ColorType)(((cspace) >> 28) & 0x0F)

/**
 * A macro to retrieve the range of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the ColorRange of `cspace`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_COLORSPACERANGE(cspace) (SDL_ColorRange)(((cspace) >> 24) & 0x0F)

/**
 * A macro to retrieve the chroma sample location of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the ChromaLocation of `cspace`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_COLORSPACECHROMA(cspace)                                           \
  (SDL_ChromaLocation)(((cspace) >> 20) & 0x0F)

/**
 * A macro to retrieve the primaries of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the ColorPrimaries of `cspace`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_COLORSPACEPRIMARIES(cspace)                                        \
  (SDL_ColorPrimaries)(((cspace) >> 10) & 0x1F)

/**
 * A macro to retrieve the transfer characteristics of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the TransferCharacteristics of `cspace`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_COLORSPACETRANSFER(cspace)                                         \
  (SDL_TransferCharacteristics)(((cspace) >> 5) & 0x1F)

/**
 * A macro to retrieve the matrix coefficients of an Colorspace.
 *
 * @param cspace an Colorspace to check.
 * @returns the MatrixCoefficients of `cspace`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_COLORSPACEMATRIX(cspace) (SDL_MatrixCoefficients)((cspace) & 0x1F)

/**
 * A macro to determine if an Colorspace uses BT601 (or BT470BG) matrix
 * coefficients.
 *
 * Note that this macro double-evaluates its parameter, so do not use
 * expressions with side-effects here.
 *
 * @param cspace an Colorspace to check.
 * @returns true if BT601 or BT470BG, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_ISCOLORSPACE_MATRIX_BT601(cspace)                                  \
  (SDL_COLORSPACEMATRIX(cspace) == SDL_MATRIX_COEFFICIENTS_BT601 ||            \
   SDL_COLORSPACEMATRIX(cspace) == SDL_MATRIX_COEFFICIENTS_BT470BG)

/**
 * A macro to determine if an Colorspace uses BT709 matrix coefficients.
 *
 * @param cspace an Colorspace to check.
 * @returns true if BT709, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_ISCOLORSPACE_MATRIX_BT709(cspace)                                  \
  (SDL_COLORSPACEMATRIX(cspace) == SDL_MATRIX_COEFFICIENTS_BT709)

/**
 * A macro to determine if an Colorspace uses BT2020_NCL matrix
 * coefficients.
 *
 * @param cspace an Colorspace to check.
 * @returns true if BT2020_NCL, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_ISCOLORSPACE_MATRIX_BT2020_NCL(cspace)                             \
  (SDL_COLORSPACEMATRIX(cspace) == SDL_MATRIX_COEFFICIENTS_BT2020_NCL)

/**
 * A macro to determine if an Colorspace has a limited range.
 *
 * @param cspace an Colorspace to check.
 * @returns true if limited range, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_ISCOLORSPACE_LIMITED_RANGE(cspace)                                 \
  (SDL_COLORSPACERANGE(cspace) != SDL_COLOR_RANGE_FULL)

/**
 * A macro to determine if an Colorspace has a full range.
 *
 * @param cspace an Colorspace to check.
 * @returns true if full range, false otherwise.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_ISCOLORSPACE_FULL_RANGE(cspace)                                    \
  (SDL_COLORSPACERANGE(cspace) == SDL_COLOR_RANGE_FULL)

#endif // SDL3PP_DOC
/**
 * @name Colorspaces
 * @{
 */

/**
 * Colorspace definitions.
 *
 * Since similar colorspaces may vary in their details (matrix, transfer
 * function, etc.), this is not an exhaustive list, but rather a
 * representative sample of the kinds of colorspaces supported in SDL.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @cat wrap-state
 *
 * @sa Colorspaces
 * @sa ColorPrimaries
 * @sa ColorRange
 * @sa ColorType
 * @sa MatrixCoefficients
 * @sa TransferCharacteristics
 */
class Colorspace
{
  SDL_Colorspace m_colorspace;

public:
  /**
   * Wraps Colorspace.
   *
   * @param colorspace the value to be wrapped
   */
  constexpr Colorspace(SDL_Colorspace colorspace = {})
    : m_colorspace(colorspace)
  {
  }

  /**
   * Define custom Colorspace formats.
   *
   * For example, defining COLORSPACE_SRGB looks like this:
   *
   * ```cpp
   * Colorspace colorspace(COLOR_TYPE_RGB,
   *                       COLOR_RANGE_FULL,
   *                       COLOR_PRIMARIES_BT709,
   *                       TRANSFER_CHARACTERISTICS_SRGB,
   *                       MATRIX_COEFFICIENTS_IDENTITY,
   *                       CHROMA_LOCATION_NONE)
   * ```
   *
   * @param type the type of the new format, probably an ColorType value.
   * @param range the range of the new format, probably a ColorRange value.
   * @param primaries the primaries of the new format, probably an
   *                  ColorPrimaries value.
   * @param transfer the transfer characteristics of the new format, probably an
   *                 TransferCharacteristics value.
   * @param matrix the matrix coefficients of the new format, probably an
   *               MatrixCoefficients value.
   * @param chroma the chroma sample location of the new format, probably an
   *               ChromaLocation value.
   * @post a format value in the style of Colorspace.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr Colorspace(ColorType type,
                       ColorRange range,
                       ColorPrimaries primaries,
                       TransferCharacteristics transfer,
                       MatrixCoefficients matrix,
                       ChromaLocation chroma)
    : m_colorspace(SDL_Colorspace(SDL_DEFINE_COLORSPACE(type,
                                                        range,
                                                        primaries,
                                                        transfer,
                                                        matrix,
                                                        chroma)))
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const Colorspace& other) const = default;

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(SDL_Colorspace colorspace) const
  {
    return operator==(Colorspace(colorspace));
  }

  /// True if a valid colorspace
  /**
   * Unwraps to the underlying Colorspace.
   *
   * @returns the underlying Colorspace.
   */
  constexpr operator SDL_Colorspace() const { return m_colorspace; }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return m_colorspace != 0; }

  /**
   * Retrieve the type of a Colorspace.
   *
   * @returns the ColorType for `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ColorType GetType() const
  {
    return SDL_COLORSPACETYPE(m_colorspace);
  }

  /**
   * Retrieve the range of a Colorspace.
   *
   * @returns the ColorRange of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ColorRange GetRange() const
  {
    return SDL_COLORSPACERANGE(m_colorspace);
  }

  /**
   * Retrieve the chroma sample location of an Colorspace.
   *
   * @returns the ChromaLocation of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ChromaLocation GetChroma() const
  {
    return SDL_COLORSPACECHROMA(m_colorspace);
  }

  /**
   * Retrieve the primaries of an Colorspace.
   *
   * @returns the ColorPrimaries of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr ColorPrimaries GetPrimaries() const
  {
    return SDL_COLORSPACEPRIMARIES(m_colorspace);
  }

  /**
   * Retrieve the transfer characteristics of an Colorspace.
   *
   * @returns the TransferCharacteristics of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr TransferCharacteristics GetTransfer() const
  {
    return SDL_COLORSPACETRANSFER(m_colorspace);
  }

  /**
   * Retrieve the matrix coefficients of an Colorspace.
   *
   * @returns the MatrixCoefficients of `cspace`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr MatrixCoefficients GetMatrix() const
  {
    return SDL_COLORSPACEMATRIX(m_colorspace);
  }

  /**
   * Determine if a Colorspace uses BT601 (or BT470BG) matrix coefficients.
   *
   * @returns true if BT601 or BT470BG, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT601() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT601(m_colorspace);
  }

  /**
   * Determine if an Colorspace uses BT709 matrix coefficients.
   *
   * @returns true if BT709, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT709() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT709(m_colorspace);
  }

  /**
   * Determine if an Colorspace uses BT2020_NCL matrix coefficients.
   *
   * @returns true if BT2020_NCL, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT2020_NCL() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT2020_NCL(m_colorspace);
  }

  /**
   * A function to determine if an Colorspace has a limited range.
   *
   * @returns true if limited range, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsLimitedRange() const
  {
    return SDL_ISCOLORSPACE_LIMITED_RANGE(m_colorspace);
  }

  /**
   * A function to determine if an Colorspace has a full range.
   *
   * @returns true if full range, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFullRange() const
  {
    return SDL_ISCOLORSPACE_FULL_RANGE(m_colorspace);
  }
};

constexpr SDL_Colorspace COLORSPACE_UNKNOWN =
  SDL_COLORSPACE_UNKNOWN; ///< UNKNOWN

/**
 * Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709.
 */
constexpr SDL_Colorspace COLORSPACE_SRGB = SDL_COLORSPACE_SRGB;

constexpr SDL_Colorspace COLORSPACE_SRGB_LINEAR =
  SDL_COLORSPACE_SRGB_LINEAR; ///< [object Object]

constexpr SDL_Colorspace COLORSPACE_HDR10 =
  SDL_COLORSPACE_HDR10; ///< [object Object]

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_NONE_P709_X601.
 */
constexpr SDL_Colorspace COLORSPACE_JPEG = SDL_COLORSPACE_JPEG;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601.
 */
constexpr SDL_Colorspace COLORSPACE_BT601_LIMITED =
  SDL_COLORSPACE_BT601_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601.
 */
constexpr SDL_Colorspace COLORSPACE_BT601_FULL = SDL_COLORSPACE_BT601_FULL;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709.
 */
constexpr SDL_Colorspace COLORSPACE_BT709_LIMITED =
  SDL_COLORSPACE_BT709_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709.
 */
constexpr SDL_Colorspace COLORSPACE_BT709_FULL = SDL_COLORSPACE_BT709_FULL;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P2020.
 */
constexpr SDL_Colorspace COLORSPACE_BT2020_LIMITED =
  SDL_COLORSPACE_BT2020_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P2020.
 */
constexpr SDL_Colorspace COLORSPACE_BT2020_FULL = SDL_COLORSPACE_BT2020_FULL;

/**
 * The default colorspace for RGB surfaces if no colorspace is specified.
 */
constexpr SDL_Colorspace COLORSPACE_RGB_DEFAULT = SDL_COLORSPACE_RGB_DEFAULT;

/**
 * The default colorspace for YUV surfaces if no colorspace is specified.
 */
constexpr SDL_Colorspace COLORSPACE_YUV_DEFAULT = SDL_COLORSPACE_YUV_DEFAULT;

/// @}

/**
 * A structure that represents a color as RGBA components.
 *
 * The bits of this structure can be directly reinterpreted as an
 * integer-packed color which uses the PIXELFORMAT_RGBA32 format
 * (PIXELFORMAT_ABGR8888 on little-endian systems and
 * PIXELFORMAT_RGBA8888 on big-endian systems).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 * @sa FColor
 */
struct Color : SDL_Color
{
  /**
   * Wraps Color.
   *
   * @param color the value to be wrapped
   */
  constexpr Color(ColorRaw color = {})
    : SDL_Color(color)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param r the value for channel r.
   * @param g the value for channel g.
   * @param b the value for channel b.
   * @param a the value for channel a.
   */
  constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
    : SDL_Color{r, g, b, a}
  {
  }

  /**
   * Default comparison operator
   */
  constexpr auto operator<=>(const Color& other) const
  {
    auto c = r <=> other.r;
    if (c != std::strong_ordering::equal) return c;
    c = g <=> other.g;
    if (c != std::strong_ordering::equal) return c;
    c = b <=> other.b;
    if (c != std::strong_ordering::equal) return c;
    return a <=> other.a;
  }

  /**
   * Compares with the underlying type
   */
  constexpr auto operator<=>(ColorRaw color) const
  {
    return operator<=>(Color(color));
  }

  /**
   * Get the red component from the color.
   *
   * @returns The red component from the color.
   */
  constexpr Uint8 GetR() const { return r; }

  /**
   * Set the red component from the color.
   *
   * @param newR New red component value.
   * @returns Reference to self.
   */
  constexpr Color& SetR(Uint8 newR)
  {
    r = newR;
    return *this;
  }

  /**
   * Get the green component from the color.
   *
   * @returns The green component from the color.
   *
   */
  constexpr Uint8 GetG() const { return g; }

  /**
   * Set the green component from the color.
   *
   * @param newG New green component value.
   * @returns Reference to self.
   */
  constexpr Color& SetG(Uint8 newG)
  {
    g = newG;
    return *this;
  }

  /**
   * Get the blue component from the color.
   *
   * @returns The blue component from the color.
   *
   */
  constexpr Uint8 GetB() const { return b; }

  /**
   * Set the blue component from the color.
   *
   * @param newB New blue component value.
   * @returns Reference to self.
   */
  constexpr Color& SetB(Uint8 newB)
  {
    b = newB;
    return *this;
  }

  /**
   * Get the alpha component from the color.
   *
   * @returns The alpha component from the color.
   *
   */
  constexpr Uint8 GetA() const { return a; }

  /**
   * Set the alpha component from the color.
   *
   * @param newA New alpha component value.
   * @returns Reference to self.
   */
  constexpr Color& SetA(Uint8 newA)
  {
    a = newA;
    return *this;
  }

  /**
   * Map an RGBA quadruple to a pixel value for a given pixel format.
   *
   * This function maps the RGBA color value to the specified pixel format and
   * returns the pixel value best approximating the given RGBA color value for
   * the given pixel format.
   *
   * If the specified pixel format has no alpha component the alpha value will
   * be ignored (as it will be in formats with a palette).
   *
   * If the format has a palette (8-bit) the index of the closest matching color
   * in the palette will be returned.
   *
   * If the pixel format bpp (color depth) is less than 32-bpp then the unused
   * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
   * format the return value can be assigned to a Uint16, and similarly a Uint8
   * for an 8-bpp format).
   *
   * @param format a pointer to PixelFormatDetails describing the pixel
   *               format.
   * @param palette an optional palette for indexed formats, may be NULL.
   * @returns a pixel value.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified.
   */
  Uint32 Map(const PixelFormatDetails& format, PaletteConstParam palette) const;

  /**
   * Get RGBA values from a pixel in the specified format.
   *
   * This function uses the entire 8-bit [0..255] range when converting color
   * components from pixel formats with less than 8-bits per RGB component
   * (e.g., a completely white pixel in 16-bit RGB565 format would return [0xff,
   * 0xff, 0xff] not [0xf8, 0xfc, 0xf8]).
   *
   * If the surface has no alpha component, the alpha will be returned as 0xff
   * (100% opaque).
   *
   * @param pixel a pixel value.
   * @param format a pointer to SDL_PixelFormatDetails describing the pixel
   *               format.
   * @param palette an optional palette for indexed formats, may be NULL.
   * @returns a color value.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PixelFormat.GetDetails()
   * @sa GetRGBA()
   * @sa Map()
   */
  static Color Get(Uint32 pixel,
                   const PixelFormatDetails& format,
                   PaletteConstParam palette);
};

/**
 * The bits of this structure can be directly reinterpreted as a float-packed
 * color which uses the PIXELFORMAT_RGBA128_FLOAT format
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa Color
 */
struct FColor : SDL_FColor
{
  /**
   * Wraps SDL_FColor.
   *
   * @param color the value to be wrapped
   */
  constexpr FColor(const FColorRaw& color = {})
    : SDL_FColor(color)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param r the value for r.
   * @param g the value for g.
   * @param b the value for b.
   * @param a the value for a.
   */
  constexpr FColor(float r, float g, float b, float a = 1)
    : SDL_FColor{r, g, b, a}
  {
  }

  /**
   * Default comparison operator
   */
  constexpr auto operator<=>(const FColor& other) const
  {
    auto c = r <=> other.r;
    if (c != std::partial_ordering::equivalent) return c;
    c = g <=> other.g;
    if (c != std::partial_ordering::equivalent) return c;
    c = b <=> other.b;
    if (c != std::partial_ordering::equivalent) return c;
    return a <=> other.a;
  }

  /**
   * Compares with the underlying type
   */
  constexpr auto operator<=>(const FColorRaw& color) const
  {
    return operator<=>(FColor(color));
  }

  /**
   * Get the red component from the color.
   *
   * @returns The red component from the color.
   */
  constexpr float GetR() const { return r; }

  /**
   * Set the red component from the color.
   *
   * @param newR New red component value.
   * @returns Reference to self.
   */
  constexpr FColor& SetR(float newR)
  {
    r = newR;
    return *this;
  }

  /**
   * Get the green component from the color.
   *
   * @returns The green component from the color.
   *
   */
  constexpr float GetG() const { return g; }

  /**
   * Set the green component from the color.
   *
   * @param newG New green component value.
   * @returns Reference to self.
   */
  constexpr FColor& SetG(float newG)
  {
    g = newG;
    return *this;
  }

  /**
   * Get the blue component from the color.
   *
   * @returns The blue component from the color.
   *
   */
  constexpr float GetB() const { return b; }

  /**
   * Set the blue component from the color.
   *
   * @param newB New blue component value.
   * @returns Reference to self.
   */
  constexpr FColor& SetB(float newB)
  {
    b = newB;
    return *this;
  }

  /**
   * Get the alpha component from the color.
   *
   * @returns The alpha component from the color.
   *
   */
  constexpr float GetA() const { return a; }

  /**
   * Set the alpha component from the color.
   *
   * @param newA New alpha component value.
   * @returns Reference to self.
   */
  constexpr FColor& SetA(float newA)
  {
    a = newA;
    return *this;
  }
};

/**
 * A set of indexed colors representing a palette.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Palette.SetColors
 *
 * @cat resource
 */
class Palette
{

  PaletteRaw m_resource = nullptr;

public:
  constexpr Palette() = default;

  constexpr explicit Palette(const PaletteRaw resource)
    : m_resource(resource)
  {
  }

  Palette(const Palette& other) { ++m_resource->refcount; }

  constexpr Palette(Palette&& other) { other.m_resource = nullptr; }

  /**
   * Create a palette structure with the specified number of color entries.
   *
   * The palette entries are initialized to white.
   *
   * @param ncolors represents the number of color entries in the color palette.
   * @post a new Palette structure on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.Destroy
   * @sa Palette.SetColors
   * @sa Surface.SetPalette
   */
  Palette(int ncolors)
    : m_resource(CheckError(SDL_CreatePalette(ncolors)))
  {
  }

  ~Palette() { SDL_DestroyPalette(m_resource); }

  Palette& operator=(Palette other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  constexpr PaletteRaw get() const { return m_resource; }

  constexpr PaletteRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  constexpr operator PaletteParam() const { return {m_resource}; }

  constexpr int GetSize() const { return m_resource->ncolors; }

  constexpr Color operator[](int index) const
  {
    return m_resource->colors[index];
  }

  /**
   * Free a palette created with Palette.Palette().
   *
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.Palette
   */
  void Destroy()
  {
    SDL_DestroyPalette(m_resource);
    m_resource = nullptr;
  }

  /**
   * Set a range of colors in a palette.
   *
   * @param colors an array of Color structures to copy into the palette.
   * @param firstcolor the index of the first palette entry to modify.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.Palette
   */
  void SetColors(SpanRef<const ColorRaw> colors, int firstcolor = 0)
  {
    CheckError(SDL_SetPaletteColors(
      m_resource, colors.data(), firstcolor, colors.size()));
  }
};

/**
 * Get the human readable name of a pixel format.
 *
 * @param format the pixel format to query.
 * @returns the human readable name of the specified pixel format or
 *          "PIXELFORMAT_UNKNOWN" if the format isn't recognized.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetPixelFormatName(PixelFormat format)
{
  return SDL_GetPixelFormatName(format);
}

/**
 * Convert one of the enumerated pixel formats to a bpp value and RGBA masks.
 *
 * @param format one of the PixelFormat values.
 * @param bpp a bits per pixel value; usually 15, 16, or 32.
 * @param Rmask a pointer filled in with the red mask for the format.
 * @param Gmask a pointer filled in with the green mask for the format.
 * @param Bmask a pointer filled in with the blue mask for the format.
 * @param Amask a pointer filled in with the alpha mask for the format.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.ForMasks
 */
inline void GetMasksForPixelFormat(PixelFormat format,
                                   int* bpp,
                                   Uint32* Rmask,
                                   Uint32* Gmask,
                                   Uint32* Bmask,
                                   Uint32* Amask)
{
  CheckError(
    SDL_GetMasksForPixelFormat(format, bpp, Rmask, Gmask, Bmask, Amask));
}

/**
 * Convert a bpp value and RGBA masks to an enumerated pixel format.
 *
 * This will return `PIXELFORMAT_UNKNOWN` if the conversion wasn't
 * possible.
 *
 * @param bpp a bits per pixel value; usually 15, 16, or 32.
 * @param Rmask the red mask for the format.
 * @param Gmask the green mask for the format.
 * @param Bmask the blue mask for the format.
 * @param Amask the alpha mask for the format.
 * @returns the PixelFormat value corresponding to the format masks, or
 *          PIXELFORMAT_UNKNOWN if there isn't a match.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetMasks
 */
inline PixelFormat GetPixelFormatForMasks(int bpp,
                                          Uint32 Rmask,
                                          Uint32 Gmask,
                                          Uint32 Bmask,
                                          Uint32 Amask)
{
  return SDL_GetPixelFormatForMasks(bpp, Rmask, Gmask, Bmask, Amask);
}

/**
 * Create an PixelFormatDetails structure corresponding to a pixel format.
 *
 * Returned structure may come from a shared global cache (i.e. not newly
 * allocated), and hence should not be modified, especially the palette. Weird
 * errors such as `Blit combination not supported` may occur.
 *
 * @param format one of the PixelFormat values.
 * @returns a pointer to a PixelFormatDetails structure or nullptr on
 *          failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const PixelFormatDetails* GetPixelFormatDetails(PixelFormat format)
{
  return SDL_GetPixelFormatDetails(format);
}

/**
 * Create a palette structure with the specified number of color entries.
 *
 * The palette entries are initialized to white.
 *
 * @param ncolors represents the number of color entries in the color palette.
 * @returns a new Palette structure on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Palette.Destroy
 * @sa Palette.SetColors
 * @sa Surface.SetPalette
 */
inline Palette CreatePalette(int ncolors)
{
  return Palette(CheckError(SDL_CreatePalette(ncolors)));
}

/**
 * Set a range of colors in a palette.
 *
 * @param palette the Palette structure to modify.
 * @param colors an array of Color structures to copy into the palette.
 * @param firstcolor the index of the first palette entry to modify.
 * @param ncolors the number of entries to modify.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified or destroyed in another thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetPaletteColors(PaletteParam palette,
                             SpanRef<const ColorRaw> colors,
                             int firstcolor = 0)
{
  CheckError(
    SDL_SetPaletteColors(palette, colors.data(), firstcolor, colors.size()));
}

/**
 * Free a palette created with Palette.Palette().
 *
 * @param palette the Palette structure to be freed.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified or destroyed in another thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Palette.Palette
 */
inline void DestroyPalette(PaletteRaw palette) { SDL_DestroyPalette(palette); }

/**
 * Map an RGB triple to an opaque pixel value for a given pixel format.
 *
 * This function maps the RGB color value to the specified pixel format and
 * returns the pixel value best approximating the given RGB color value for
 * the given pixel format.
 *
 * If the format has a palette (8-bit) the index of the closest matching color
 * in the palette will be returned.
 *
 * If the specified pixel format has an alpha component it will be returned as
 * all 1 bits (fully opaque).
 *
 * If the pixel format bpp (color depth) is less than 32-bpp then the unused
 * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
 * format the return value can be assigned to a Uint16, and similarly a Uint8
 * for an 8-bpp format).
 *
 * @param format a pointer to PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be nullptr.
 * @param r the red component of the pixel in the range 0-255.
 * @param g the green component of the pixel in the range 0-255.
 * @param b the blue component of the pixel in the range 0-255.
 * @returns a pixel value.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails
 * @sa GetRGB
 * @sa MapRGBA
 * @sa Surface.MapColor
 */
inline Uint32 MapRGB(const PixelFormatDetails* format,
                     PaletteConstParam palette,
                     Uint8 r,
                     Uint8 g,
                     Uint8 b)
{
  return SDL_MapRGB(format, palette, r, g, b);
}

/**
 * Map an RGBA quadruple to a pixel value for a given pixel format.
 *
 * This function maps the RGBA color value to the specified pixel format and
 * returns the pixel value best approximating the given RGBA color value for
 * the given pixel format.
 *
 * If the specified pixel format has no alpha component the alpha value will
 * be ignored (as it will be in formats with a palette).
 *
 * If the format has a palette (8-bit) the index of the closest matching color
 * in the palette will be returned.
 *
 * If the pixel format bpp (color depth) is less than 32-bpp then the unused
 * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
 * format the return value can be assigned to a Uint16, and similarly a Uint8
 * for an 8-bpp format).
 *
 * @param format a pointer to PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be nullptr.
 * @param r the red component of the pixel in the range 0-255.
 * @param g the green component of the pixel in the range 0-255.
 * @param b the blue component of the pixel in the range 0-255.
 * @param a the alpha component of the pixel in the range 0-255.
 * @returns a pixel value.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails
 * @sa GetRGBA
 * @sa MapRGB
 * @sa Surface.MapColor
 */
inline Uint32 MapRGBA(const PixelFormatDetails* format,
                      PaletteConstParam palette,
                      Uint8 r,
                      Uint8 g,
                      Uint8 b,
                      Uint8 a)
{
  return SDL_MapRGBA(format, palette, r, g, b, a);
}

/**
 * Get RGB values from a pixel in the specified format.
 *
 * This function uses the entire 8-bit [0..255] range when converting color
 * components from pixel formats with less than 8-bits per RGB component
 * (e.g., a completely white pixel in 16-bit RGB565 format would return [0xff,
 * 0xff, 0xff] not [0xf8, 0xfc, 0xf8]).
 *
 * @param pixel a pixel value.
 * @param format a pointer to PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be nullptr.
 * @param r a pointer filled in with the red component, may be nullptr.
 * @param g a pointer filled in with the green component, may be nullptr.
 * @param b a pointer filled in with the blue component, may be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails
 * @sa GetRGBA
 * @sa MapRGB
 * @sa MapRGBA
 */
inline void GetRGB(Uint32 pixel,
                   const PixelFormatDetails* format,
                   PaletteConstParam palette,
                   Uint8* r,
                   Uint8* g,
                   Uint8* b)
{
  SDL_GetRGB(pixel, format, palette, r, g, b);
}

/**
 * Get RGBA values from a pixel in the specified format.
 *
 * This function uses the entire 8-bit [0..255] range when converting color
 * components from pixel formats with less than 8-bits per RGB component
 * (e.g., a completely white pixel in 16-bit RGB565 format would return [0xff,
 * 0xff, 0xff] not [0xf8, 0xfc, 0xf8]).
 *
 * If the surface has no alpha component, the alpha will be returned as 0xff
 * (100% opaque).
 *
 * @param pixel a pixel value.
 * @param format a pointer to PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be nullptr.
 * @param r a pointer filled in with the red component, may be nullptr.
 * @param g a pointer filled in with the green component, may be nullptr.
 * @param b a pointer filled in with the blue component, may be nullptr.
 * @param a a pointer filled in with the alpha component, may be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails
 * @sa GetRGB
 * @sa MapRGB
 * @sa MapRGBA
 */
inline void GetRGBA(Uint32 pixel,
                    const PixelFormatDetails* format,
                    PaletteConstParam palette,
                    Uint8* r,
                    Uint8* g,
                    Uint8* b,
                    Uint8* a)
{
  SDL_GetRGBA(pixel, format, palette, r, g, b, a);
}

/// @}

inline Uint32 Color::Map(const PixelFormatDetails& format,
                         PaletteConstParam palette = nullptr) const
{
  return MapRGBA(&format, palette, r, g, b, a);
}

inline Color Color::Get(Uint32 pixel,
                        const PixelFormatDetails& format,
                        PaletteConstParam palette = nullptr)
{
  Color c;
  GetRGBA(pixel, &format, palette, &c.r, &c.g, &c.b, &c.a);
  return c;
}

inline Uint32 PixelFormat::Map(Color color,
                               PaletteConstParam palette = nullptr) const
{
  return color.Map(*GetDetails(), palette);
}

inline Color PixelFormat::Get(Uint32 pixel,
                              PaletteConstParam palette = nullptr) const
{
  return Color::Get(pixel, *GetDetails(), palette);
}

/**
 * @defgroup CategoryProperties Object Properties
 *
 * A property is a variable that can be created and retrieved by name at
 * runtime.
 *
 * All properties are part of a property group (Properties). A property group
 * can be created with the Properties.Create function and destroyed with the
 * Properties.Destroy function, but the Properties destructor probably will do
 * what you want to, automatically.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - SetPointerProperty and GetPointerProperty operate on `void*`
 *   pointer types.
 * - SetStringProperty and GetStringProperty operate on string types.
 * - SetNumberProperty and GetNumberProperty operate on signed 64-bit
 *   integer types.
 * - SetFloatProperty and GetFloatProperty operate on floating point
 *   types.
 * - SetBooleanProperty and GetBooleanProperty operate on boolean
 *   types.
 *
 * Properties can be removed from a group by using ClearProperty.
 *
 * @{
 */

// Forward decl
struct Properties;

using PropertiesID = SDL_PropertiesID;

/**
 * Safely wrap Properties for non owning parameters
 */
struct PropertiesParam
{

  PropertiesID value;

  constexpr PropertiesParam(PropertiesID value)
    : value(value)
  {
  }

  constexpr PropertiesParam(std::nullptr_t _)
    : value(0)
  {
  }

  constexpr operator PropertiesID() const { return value; }
};

/**
 * SDL properties ID
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Properties.Create
 */
class Properties
{
  PropertiesID m_resource = 0;

public:
  constexpr Properties() = default;

  constexpr explicit Properties(const PropertiesID resource)
    : m_resource(resource)
  {
  }

  Properties(const Properties& other) {}

  constexpr Properties(Properties&& other) { other.m_resource = 0; }

  /**
   * Create a group of properties.
   *
   * All properties are automatically destroyed when Quit() is called.
   *
   * @returns a valid ID for a new group of properties on success;
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Destroy
   */
  static Properties Create()
  {
    return Properties(CheckError(SDL_CreateProperties()));
  }

  ~Properties() { SDL_DestroyProperties(m_resource); }

  Properties& operator=(Properties other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  constexpr PropertiesID get() const { return m_resource; }

  constexpr PropertiesID release()
  {
    auto r = m_resource;
    m_resource = 0;
    return r;
  }

  constexpr operator PropertiesParam() const { return {m_resource}; }

  /**
   * Destroy a group of properties.
   *
   * All properties are deleted and their cleanup functions will be called, if
   * any.
   *
   * @threadsafety This function should not be called while these properties are
   *               locked or other threads might be setting or getting values
   *               from these properties.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Create
   */
  void Destroy()
  {
    SDL_DestroyProperties(m_resource);
    m_resource = 0;
  }
};

/**
 * SDL property type
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PropertyType = SDL_PropertyType;

constexpr PropertyType PROPERTY_TYPE_INVALID =
  SDL_PROPERTY_TYPE_INVALID; ///< INVALID

constexpr PropertyType PROPERTY_TYPE_POINTER =
  SDL_PROPERTY_TYPE_POINTER; ///< POINTER

constexpr PropertyType PROPERTY_TYPE_STRING =
  SDL_PROPERTY_TYPE_STRING; ///< STRING

constexpr PropertyType PROPERTY_TYPE_NUMBER =
  SDL_PROPERTY_TYPE_NUMBER; ///< NUMBER

constexpr PropertyType PROPERTY_TYPE_FLOAT = SDL_PROPERTY_TYPE_FLOAT; ///< FLOAT

constexpr PropertyType PROPERTY_TYPE_BOOLEAN =
  SDL_PROPERTY_TYPE_BOOLEAN; ///< BOOLEAN

/**
 * Get the global SDL properties.
 *
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesID GetGlobalProperties()
{
  return CheckError(SDL_GetGlobalProperties());
}

/**
 * Create a group of properties.
 *
 * All properties are automatically destroyed when Quit() is called.
 *
 * @returns a valid ID for a new group of properties on success;
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Destroy
 */
inline Properties CreateProperties()
{
  return Properties(CheckError(SDL_CreateProperties()));
}

/**
 * Copy a group of properties.
 *
 * Copy all the properties from one group of properties to another, with the
 * exception of properties requiring cleanup (set using
 * SetPointerPropertyWithCleanup()), which will not be copied. Any
 * property that already exists on `dst` will be overwritten.
 *
 * @param src the properties to copy.
 * @param dst the destination properties.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void CopyProperties(PropertiesParam src, PropertiesParam dst)
{
  CheckError(SDL_CopyProperties(src, dst));
}

/**
 * Lock a group of properties.
 *
 * Obtain a multi-threaded lock for these properties. Other threads will wait
 * while trying to lock these properties until they are unlocked. Properties
 * must be unlocked before they are destroyed.
 *
 * The lock is automatically taken when setting individual properties, this
 * function is only needed when you want to set several properties atomically
 * or want to guarantee that properties being queried aren't freed in another
 * thread.
 *
 * @param props the properties to lock.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa UnlockProperties
 */
inline void LockProperties(PropertiesParam props)
{
  CheckError(SDL_LockProperties(props));
}

/**
 * Unlock a group of properties.
 *
 * @param props the properties to unlock.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LockProperties
 */
inline void UnlockProperties(PropertiesParam props)
{
  SDL_UnlockProperties(props);
}

/**
 * A callback used to free resources when a property is deleted.
 *
 * This should release any resources associated with `value` that are no
 * longer needed.
 *
 * This callback is set per-property. Different properties in the same group
 * can have different cleanup callbacks.
 *
 * This callback will be called _during_ SetPointerPropertyWithCleanup if
 * the function fails for any reason.
 *
 * @param userdata an app-defined pointer passed to the callback.
 * @param value the pointer assigned to the property to clean up.
 *
 * @threadsafety This callback may fire without any locks held; if this is a
 *               concern, the app should provide its own locking.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SetPointerPropertyWithCleanup
 */
using CleanupPropertyCallback = SDL_CleanupPropertyCallback;

/**
 * A callback used to free resources when a property is deleted.
 *
 * This should release any resources associated with `value` that are no
 * longer needed.
 *
 * This callback is set per-property. Different properties in the same group
 * can have different cleanup callbacks.
 *
 * This callback will be called _during_ SetPointerPropertyWithCleanup if
 * the function fails for any reason.
 *
 * @param value the pointer assigned to the property to clean up.
 *
 * @threadsafety This callback may fire without any locks held; if this is a
 *               concern, the app should provide its own locking.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SetPointerPropertyWithCleanup
 * @sa CleanupPropertyCallback
 */
using CleanupPropertyCB = std::function<void(void*)>;

/**
 * Set a pointer property in a group of properties with a cleanup function
 * that is called when the property is deleted.
 *
 * The cleanup function is also called if setting the property fails for any
 * reason.
 *
 * For simply setting basic data types, like numbers, bools, or strings, use
 * SetNumberProperty, SetBooleanProperty, or SetStringProperty
 * instead, as those functions will handle cleanup on your behalf. This
 * function is only for more complex, custom data.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 * property.
 * @param cleanup the function to call when this property is deleted, or nullptr
 *                if no cleanup is necessary.
 * @param userdata a pointer that is passed to the cleanup function.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPointerProperty
 * @sa SetPointerProperty
 * @sa CleanupPropertyCallback
 */
inline void SetPointerPropertyWithCleanup(PropertiesParam props,
                                          StringParam name,
                                          void* value,
                                          CleanupPropertyCallback cleanup,
                                          void* userdata)
{
  CheckError(
    SDL_SetPointerPropertyWithCleanup(props, name, value, cleanup, userdata));
}

/**
 * Set a pointer property in a group of properties with a cleanup function
 * that is called when the property is deleted.
 *
 * The cleanup function is also called if setting the property fails for any
 * reason.
 *
 * For simply setting basic data types, like numbers, bools, or strings, use
 * SetNumberProperty, SetBooleanProperty, or SetStringProperty
 * instead, as those functions will handle cleanup on your behalf. This
 * function is only for more complex, custom data.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 * property.
 * @param cleanup the function to call when this property is deleted, or nullptr
 *                if no cleanup is necessary.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPointerProperty
 * @sa SetPointerProperty
 * @sa CleanupPropertyCallback
 */
inline void SetPointerPropertyWithCleanup(PropertiesParam props,
                                          StringParam name,
                                          void* value,
                                          CleanupPropertyCB cleanup)
{
  using Wrapper = CallbackWrapper<CleanupPropertyCB>;
  SDL_SetPointerPropertyWithCleanup(props,
                                    std::move(name),
                                    value,
                                    &Wrapper::CallOnce,
                                    Wrapper::Wrap(std::move(cleanup)));
}

/**
 * Set a pointer property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 * property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPointerProperty
 * @sa HasProperty
 * @sa SetBooleanProperty
 * @sa SetFloatProperty
 * @sa SetNumberProperty
 * @sa SetPointerPropertyWithCleanup
 * @sa SetStringProperty
 */
inline void SetPointerProperty(PropertiesParam props,
                               StringParam name,
                               void* value)
{
  CheckError(SDL_SetPointerProperty(props, name, value));
}

/**
 * Set a string property in a group of properties.
 *
 * This function makes a copy of the string; the caller does not have to
 * preserve the data after this call completes.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 * property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetStringProperty
 */
inline void SetStringProperty(PropertiesParam props,
                              StringParam name,
                              StringParam value)
{
  CheckError(SDL_SetStringProperty(props, name, value));
}

/**
 * Set an integer property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumberProperty
 */
inline void SetNumberProperty(PropertiesParam props,
                              StringParam name,
                              Sint64 value)
{
  CheckError(SDL_SetNumberProperty(props, name, value));
}

/**
 * Set a floating point property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetFloatProperty
 */
inline void SetFloatProperty(PropertiesParam props,
                             StringParam name,
                             float value)
{
  CheckError(SDL_SetFloatProperty(props, name, value));
}

/**
 * Set a boolean property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetBooleanProperty
 */
inline void SetBooleanProperty(PropertiesParam props,
                               StringParam name,
                               bool value)
{
  CheckError(SDL_SetBooleanProperty(props, name, value));
}

/**
 * Return whether a property exists in a group of properties.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @returns true if the property exists, or false if it doesn't.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 */
inline bool HasProperty(PropertiesParam props, StringParam name)
{
  return SDL_HasProperty(props, name);
}

/**
 * Get the type of a property in a group of properties.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @returns the type of the property, or PROPERTY_TYPE_INVALID if it is
 *          not set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasProperty
 */
inline PropertyType GetPropertyType(PropertiesParam props, StringParam name)
{
  return SDL_GetPropertyType(props, name);
}

/**
 * Get a pointer property from a group of properties.
 *
 * By convention, the names of properties that SDL exposes on objects will
 * start with "SDL.", and properties that SDL uses internally will start with
 * "SDL.internal.". These should be considered read-only and should not be
 * modified by applications.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a pointer property.
 *
 * @threadsafety It is safe to call this function from any thread, although
 *               the data returned is not protected and could potentially be
 *               freed if you call SetPointerProperty() or
 *               ClearProperty() on these properties from another thread.
 *               If you need to avoid this, use LockProperties() and
 *               UnlockProperties().
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetBooleanProperty
 * @sa GetFloatProperty
 * @sa GetNumberProperty
 * @sa GetPropertyType
 * @sa GetStringProperty
 * @sa HasProperty
 * @sa SetPointerProperty
 */
inline void* GetPointerProperty(PropertiesParam props,
                                StringParam name,
                                void* default_value)
{
  return SDL_GetPointerProperty(props, name, default_value);
}

/**
 * Get a string property from a group of properties.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a string property.
 *
 * @threadsafety It is safe to call this function from any thread, although
 *               the data returned is not protected and could potentially be
 *               freed if you call SetStringProperty() or
 *               ClearProperty() on these properties from another thread.
 *               If you need to avoid this, use LockProperties() and
 *               UnlockProperties().
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 * @sa HasProperty
 * @sa SetStringProperty
 */
inline const char* GetStringProperty(PropertiesParam props,
                                     StringParam name,
                                     StringParam default_value)
{
  return SDL_GetStringProperty(props, name, default_value);
}

/**
 * Get a number property from a group of properties.
 *
 * You can use GetPropertyType() to query whether the property exists and
 * is a number property.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a number property.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 * @sa HasProperty
 * @sa SetNumberProperty
 */
inline Sint64 GetNumberProperty(PropertiesParam props,
                                StringParam name,
                                Sint64 default_value)
{
  return SDL_GetNumberProperty(props, name, default_value);
}

/**
 * Get a floating point property from a group of properties.
 *
 * You can use GetPropertyType() to query whether the property exists and
 * is a floating point property.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a float property.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 * @sa HasProperty
 * @sa SetFloatProperty
 */
inline float GetFloatProperty(PropertiesParam props,
                              StringParam name,
                              float default_value)
{
  return SDL_GetFloatProperty(props, name, default_value);
}

/**
 * Get a boolean property from a group of properties.
 *
 * You can use GetPropertyType() to query whether the property exists and
 * is a boolean property.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a boolean property.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 * @sa HasProperty
 * @sa SetBooleanProperty
 */
inline bool GetBooleanProperty(PropertiesParam props,
                               StringParam name,
                               bool default_value)
{
  return SDL_GetBooleanProperty(props, name, default_value);
}

/**
 * Clear a property from a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to clear.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ClearProperty(PropertiesParam props, StringParam name)
{
  CheckError(SDL_ClearProperty(props, name));
}

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from EnumerateProperties(), and is called once
 * per property in the set.
 *
 * @param userdata an app-defined pointer passed to the callback.
 * @param props the Properties that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety EnumerateProperties holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EnumerateProperties
 */
using EnumeratePropertiesCallback = SDL_EnumeratePropertiesCallback;

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from EnumerateProperties(), and is called once
 * per property in the set.
 *
 * @param props the Properties that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety EnumerateProperties holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EnumerateProperties
 * @sa EnumeratePropertiesCallback
 */
using EnumeratePropertiesCB =
  std::function<void(PropertiesID props, const char* name)>;

/**
 * Enumerate the properties contained in a group of properties.
 *
 * The callback function is called for each property in the group of
 * properties. The properties are locked during enumeration.
 *
 * @param props the properties to query.
 * @param callback the function to call for each property.
 * @param userdata a pointer that is passed to `callback`.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EnumerateProperties(PropertiesParam props,
                                EnumeratePropertiesCallback callback,
                                void* userdata)
{
  CheckError(SDL_EnumerateProperties(props, callback, userdata));
}

/**
 * Enumerate the properties contained in a group of properties.
 *
 * The callback function is called for each property in the group of
 * properties. The properties are locked during enumeration.
 *
 * @param props the properties to query.
 * @param callback the function to call for each property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EnumerateProperties(PropertiesParam props,
                                EnumeratePropertiesCB callback)
{
  return EnumerateProperties(
    props,
    [](void* userdata, PropertiesID props, const char* name) {
      auto& f = *static_cast<EnumeratePropertiesCB*>(userdata);
      f(props, name);
    },
    &callback);
}

/**
 * Returns the number of properties this has
 *
 * This uses Enumerate() internally, so might not be so fast
 */
inline Uint64 CountProperties(PropertiesParam props)
{
  Uint64 count = 0;
  EnumerateProperties(props, [&](auto, const char*) { count++; });
  return count;
}

/**
 * Destroy a group of properties.
 *
 * All properties are deleted and their cleanup functions will be called, if
 * any.
 *
 * @param props the properties to destroy.
 *
 * @threadsafety This function should not be called while these properties are
 *               locked or other threads might be setting or getting values
 *               from these properties.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Create
 */
inline void DestroyProperties(PropertiesID props)
{
  SDL_DestroyProperties(props);
}

/// @}

/**
 * @defgroup CategoryStdinc Standard Library Functionality
 *
 * SDL provides its own implementation of some of the most important C runtime
 * functions.
 *
 * Using these functions allows an app to have access to common C
 * functionality without depending on a specific C runtime (or a C runtime at
 * all). More importantly, the SDL implementations work identically across
 * platforms, so apps can avoid surprises like snprintf() behaving differently
 * between Windows and Linux builds, or itoa() only existing on some
 * platforms.
 *
 * For many of the most common functions, like memcpy, SDL might just call
 * through to the usual C runtime behind the scenes, if it makes sense to do
 * so (if it's faster and always available/reliable on a given platform),
 * reducing library size and offering the most optimized option.
 *
 * SDL also offers other C-runtime-adjacent functionality in this header that
 * either isn't, strictly speaking, part of any C runtime standards, like
 * crc32() and SDL_reinterpret_cast, etc. It also offers a few better
 * options, like strlcpy(), which functions as a safer form of strcpy().
 *
 * @{
 */

// Forward decl
struct Environment;

using EnvironmentRaw = SDL_Environment*;

/**
 * Safely wrap Environment for non owning parameters
 */
struct EnvironmentParam
{

  EnvironmentRaw value;

  constexpr EnvironmentParam(EnvironmentRaw value)
    : value(value)
  {
  }

  constexpr EnvironmentParam(std::nullptr_t _)
    : value(nullptr)
  {
  }

  constexpr operator EnvironmentRaw() const { return value; }
};

// Forward decl
struct IConv;

using IConvRaw = SDL_iconv_t;

/**
 * Safely wrap IConv for non owning parameters
 */
struct IConvParam
{

  IConvRaw value;

  constexpr IConvParam(IConvRaw value)
    : value(value)
  {
  }

  constexpr IConvParam(std::nullptr_t _)
    : value(nullptr)
  {
  }

  constexpr operator IConvRaw() const { return value; }
};

#ifdef SDL3PP_DOC

/**
 * Don't let SDL use "long long" C types.
 *
 * SDL will define this if it believes the compiler doesn't understand the
 * "long long" syntax for C datatypes. This can happen on older compilers.
 *
 * If _your_ compiler doesn't support "long long" but SDL doesn't know it, it
 * is safe to define this yourself to build against the SDL headers.
 *
 * If this is defined, it will remove access to some C runtime support
 * functions, like SDL_ulltoa and SDL_strtoll that refer to this datatype
 * explicitly. The rest of SDL will still be available.
 *
 * SDL's own source code cannot be built with a compiler that has this
 * defined, for various technical reasons.
 */
#define SDL_NOLONGLONG 1

/**
 * The largest value that a `size_t` can hold for the target platform.
 *
 * `size_t` is generally the same size as a pointer in modern times, but this
 * can get weird on very old and very esoteric machines. For example, on a
 * 16-bit Intel 286, you might have a 32-bit "far" pointer (16-bit segment
 * plus 16-bit offset), but `size_t` is 16 bits, because it can only deal with
 * the offset into an individual segment.
 *
 * In modern times, it's generally expected to cover an entire linear address
 * space. But be careful!
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_SIZE_MAX SIZE_MAX

#endif // SDL3PP_DOC

/**
 * The number of elements in a static array.
 *
 * This will compile but return incorrect results for a pointer to an array;
 * it has to be an array the compiler knows the size of.
 *
 * @since This function is available since SDL 3.2.0.
 */
template<class T, std::size_t N>
constexpr std::size_t arraysize(const T (&array)[N])
{
  return SDL_arraysize(array);
}

#ifdef SDL3PP_DOC

/**
 * Macro useful for building other macros with strings in them.
 *
 * For example:
 *
 * ```c
 * #define LOG_ERROR(X) OutputDebugString(SDL_STRINGIFY_ARG(__FUNCTION__) ": " X
 * "@n")`
 * ```
 *
 * @param arg the text to turn into a string literal.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_STRINGIFY_ARG(arg) #arg

#endif // SDL3PP_DOC

/**
 * Define a four character code as a Uint32.
 *
 * @param a the first ASCII character.
 * @param b the second ASCII character.
 * @param c the third ASCII character.
 * @param d the fourth ASCII character.
 * @returns the four characters converted into a Uint32, one character
 *          per-byte.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint32 FourCC(Uint8 a, Uint8 b, Uint8 c, Uint8 d)
{
  return SDL_FOURCC(a, b, c, d);
}

#ifdef SDL3PP_DOC

/**
 * Append the 64 bit integer suffix to a signed integer literal.
 *
 * This helps compilers that might believe a integer literal larger than
 * 0xFFFFFFFF is overflowing a 32-bit value. Use `SDL_SINT64_C(0xFFFFFFFF1)`
 * instead of `0xFFFFFFFF1` by itself.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_UINT64_C
 */
#define SDL_SINT64_C(c) c##LL /* or whatever the current compiler uses. */

/**
 * Append the 64 bit integer suffix to an unsigned integer literal.
 *
 * This helps compilers that might believe a integer literal larger than
 * 0xFFFFFFFF is overflowing a 32-bit value. Use `SDL_UINT64_C(0xFFFFFFFF1)`
 * instead of `0xFFFFFFFF1` by itself.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_SINT64_C
 */
#define SDL_UINT64_C(c) c##ULL /* or whatever the current compiler uses. */

/**
 * A signed 8-bit integer type.
 *
 * @since This macro is available since SDL 3.2.0.
 */
using Sint8 = Sint8;

#endif // SDL3PP_DOC

constexpr Sint8 MAX_SINT8 = SDL_MAX_SINT8;

constexpr Sint8 MIN_SINT8 = SDL_MIN_SINT8;

#ifdef SDL3PP_DOC

/**
 * An unsigned 8-bit integer type.
 *
 * @since This macro is available since SDL 3.2.0.
 */
using Uint8 = Uint8;

#endif // SDL3PP_DOC

constexpr Uint8 MAX_UINT8 = SDL_MAX_UINT8;

constexpr Uint8 MIN_UINT8 = SDL_MIN_UINT8;

#ifdef SDL3PP_DOC

/**
 * A signed 16-bit integer type.
 *
 * @since This macro is available since SDL 3.2.0.
 */
using Sint16 = Sint16;

#endif // SDL3PP_DOC

constexpr Sint16 MAX_SINT16 = SDL_MAX_SINT16;

constexpr Sint16 MIN_SINT16 = SDL_MIN_SINT16;

#ifdef SDL3PP_DOC

/**
 * An unsigned 16-bit integer type.
 *
 * @since This macro is available since SDL 3.2.0.
 */
using Uint16 = Uint16;

#endif // SDL3PP_DOC

constexpr Uint16 MAX_UINT16 = SDL_MAX_UINT16;

constexpr Uint16 MIN_UINT16 = SDL_MIN_UINT16;

#ifdef SDL3PP_DOC

/**
 * A signed 32-bit integer type.
 *
 * @since This macro is available since SDL 3.2.0.
 */
using Sint32 = Sint32;

#endif // SDL3PP_DOC

constexpr Sint32 MAX_SINT32 = SDL_MAX_SINT32;

constexpr Sint32 MIN_SINT32 = SDL_MIN_SINT32;

#ifdef SDL3PP_DOC

/**
 * An unsigned 32-bit integer type.
 *
 * @since This macro is available since SDL 3.2.0.
 */
using Uint32 = Uint32;

#endif // SDL3PP_DOC

constexpr Uint32 MAX_UINT32 = SDL_MAX_UINT32;

constexpr Uint8 MIN_UINT32 = SDL_MIN_UINT32;

#ifdef SDL3PP_DOC

/**
 * A signed 64-bit integer type.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_SINT64_C
 */
using Sint64 = Sint64;

#endif // SDL3PP_DOC

constexpr Sint64 MAX_SINT64 = SDL_MAX_SINT64;

constexpr Sint64 MIN_SINT64 = SDL_MIN_SINT64;

#ifdef SDL3PP_DOC

/**
 * An unsigned 64-bit integer type.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_UINT64_C
 */
using Uint64 = Uint64;

#endif // SDL3PP_DOC

constexpr Uint64 MAX_UINT64 = SDL_MAX_UINT64;

constexpr Uint8 MIN_UINT64 = SDL_MIN_UINT64;

/**
 * Duration in seconds (float).
 */
using Seconds = std::chrono::duration<float>;

/**
 * Duration in Nanoseconds (Uint64).
 */
using Nanoseconds = std::chrono::nanoseconds;

/**
 * Converts a time duration to seconds (float).
 */
constexpr float ToSeconds(Seconds duration) { return duration.count(); }

/**
 * Converts a float to seconds representation.
 */
constexpr Seconds FromSeconds(float duration) { return Seconds(duration); }

/**
 * Converts a time duration to nanoseconds (Sint64);
 */
constexpr Sint64 ToNS(std::chrono::nanoseconds duration)
{
  return duration.count();
}

/**
 * Converts a Sint64 to nanoseconds representation.
 */
constexpr Nanoseconds FromNS(Sint64 duration) { return Nanoseconds{duration}; }

/**
 * SDL times are signed, 64-bit integers representing nanoseconds since the
 * Unix epoch (Jan 1, 1970).
 *
 * They can be converted between POSIX time_t values with Time.ToPosix()
 * and Time.FromPosix(), and between Windows FILETIME values with
 * Time.ToWindows() and Time.FromWindows().
 *
 * @since This type is available since SDL 3.2.0.
 *
 * @sa MAX_SINT64
 * @sa MIN_SINT64
 */
class Time
{
  std::chrono::nanoseconds m_value;

public:
  constexpr Time() = default;

  /// Constructs from a nanoseconds period.
  constexpr Time(std::chrono::nanoseconds time)
    : m_value(time)
  {
  }

  /// Constructs from SDL_Time
  constexpr explicit Time(SDL_Time time)
    : m_value(FromNS(time))
  {
  }

  /// True if not zero
  constexpr explicit operator bool() const
  {
    return m_value != std::chrono::nanoseconds{};
  }

  /// Converts to nanoseconds period
  constexpr operator std::chrono::nanoseconds() const { return m_value; }

  static Time Current();

  /// Create from a nanoseconds Sint64.
  static constexpr Time FromNS(Sint64 time)
  {
    return Time{std::chrono::nanoseconds{time}};
  }

  /// Converts to nanoseconds Sint64
  constexpr Sint64 ToNS() const { return m_value.count(); }

  static constexpr Time FromPosix(Sint64 time);

  constexpr Sint64 ToPosix() const;

  static Time FromWindows(Uint32 dwLowDateTime, Uint32 dwHighDateTime);

  void ToWindows(Uint32* dwLowDateTime, Uint32* dwHighDateTime) const;

  /**
   * Converts a time to seconds (float) since epoch.
   */
  constexpr float ToSeconds() const { return Seconds(m_value).count(); }

  /**
   * Converts a time to seconds (float) since epoch.
   */
  static constexpr Time FromSeconds(float interval)
  {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
      Seconds(interval));
  }

  /// Increment time
  constexpr Time& operator+=(std::chrono::nanoseconds interval)
  {
    m_value += interval;
    return *this;
  }

  /// Decrement
  constexpr Time& operator-=(std::chrono::nanoseconds interval)
  {
    m_value -= interval;
    return *this;
  }
};

constexpr Time MAX_TIME = Time::FromNS(SDL_MAX_TIME);

constexpr Time MIN_TIME = Time::FromNS(SDL_MIN_TIME);

#ifdef SDL3PP_DOC

/**
 * Epsilon constant, used for comparing floating-point numbers.
 *
 * Equals by default to platform-defined `FLT_EPSILON`, or
 * `1.1920928955078125e-07F` if that's not available.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_FLT_EPSILON 1.1920928955078125e-07F /* 0x0.000002p0 */

/**
 * A macro to initialize an SDL interface.
 *
 * This macro will initialize an SDL interface structure and should be called
 * before you fill out the fields with your implementation.
 *
 * You can use it like this:
 *
 * ```cpp
 * IOStreamInterface iface;
 *
 * SDL_INIT_INTERFACE(&iface);
 *
 * // Fill in the interface function pointers with your implementation
 * iface.seek = ...
 *
 * stream = IOStream.Open(&iface, nullptr);
 * ```
 *
 * If you are using designated initializers, you can use the size of the
 * interface as the version, e.g.
 *
 * ```cpp
 * IOStreamInterface iface = {
 *     .version = sizeof(iface),
 *     .seek = ...
 * };
 * stream = IOStream.Open(&iface, nullptr);
 * ```
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa IOStreamInterface
 * @sa SDL_StorageInterface
 * @sa SDL_VirtualJoystickDesc
 */
#define SDL_INIT_INTERFACE(iface)                                              \
  do {                                                                         \
    SDL_zerop(iface);                                                          \
    (iface)->version = sizeof(*(iface));                                       \
  } while (0)

#endif // SDL3PP_DOC

/**
 * Allocate uninitialized memory.
 *
 * The allocated memory returned by this function must be freed with
 * free().
 *
 * If `size` is 0, it will be set to 1.
 *
 * If the allocation is successful, the returned pointer is guaranteed to be
 * aligned to either the *fundamental alignment* (`alignof(max_align_t)` in
 * C11 and later) or `2 * sizeof(void *)`, whichever is smaller. Use
 * aligned_alloc() if you need to allocate memory aligned to an alignment
 * greater than this guarantee.
 *
 * @param size the size to allocate.
 * @returns a pointer to the allocated memory, or nullptr if allocation failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa free
 * @sa calloc
 * @sa realloc
 * @sa aligned_alloc
 */
inline void* malloc(size_t size) { return SDL_malloc(size); }

/**
 * Allocate a zero-initialized array.
 *
 * The memory returned by this function must be freed with free().
 *
 * If either of `nmemb` or `size` is 0, they will both be set to 1.
 *
 * If the allocation is successful, the returned pointer is guaranteed to be
 * aligned to either the *fundamental alignment* (`alignof(max_align_t)` in
 * C11 and later) or `2 * sizeof(void *)`, whichever is smaller.
 *
 * @param nmemb the number of elements in the array.
 * @param size the size of each element of the array.
 * @returns a pointer to the allocated array, or nullptr if allocation failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa free
 * @sa malloc
 * @sa realloc
 */
inline void* calloc(size_t nmemb, size_t size)
{
  return SDL_calloc(nmemb, size);
}

/**
 * Change the size of allocated memory.
 *
 * The memory returned by this function must be freed with free().
 *
 * If `size` is 0, it will be set to 1. Note that this is unlike some other C
 * runtime `realloc` implementations, which may treat `realloc(mem, 0)` the
 * same way as `free(mem)`.
 *
 * If `mem` is nullptr, the behavior of this function is equivalent to
 * malloc(). Otherwise, the function can have one of three possible
 * outcomes:
 *
 * - If it returns the same pointer as `mem`, it means that `mem` was resized
 *   in place without freeing.
 * - If it returns a different non-nullptr pointer, it means that `mem` was
 * freed and cannot be dereferenced anymore.
 * - If it returns nullptr (indicating failure), then `mem` will remain valid
 * and must still be freed with free().
 *
 * If the allocation is successfully resized, the returned pointer is
 * guaranteed to be aligned to either the *fundamental alignment*
 * (`alignof(max_align_t)` in C11 and later) or `2 * sizeof(void *)`,
 * whichever is smaller.
 *
 * @param mem a pointer to allocated memory to reallocate, or nullptr.
 * @param size the new size of the memory.
 * @returns a pointer to the newly allocated memory, or nullptr if allocation
 *          failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa free
 * @sa malloc
 * @sa calloc
 */
inline void* realloc(void* mem, size_t size) { return SDL_realloc(mem, size); }

/**
 * Free allocated memory.
 *
 * The pointer is no longer valid after this call and cannot be dereferenced
 * anymore.
 *
 * If `mem` is nullptr, this function does nothing.
 *
 * @param mem a pointer to allocated memory, or nullptr.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa malloc
 * @sa calloc
 * @sa realloc
 */
inline void free(void* mem) { SDL_free(mem); }

/**
 * A callback used to implement malloc().
 *
 * SDL will always ensure that the passed `size` is greater than 0.
 *
 * @param size the size to allocate.
 * @returns a pointer to the allocated memory, or nullptr if allocation failed.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa malloc
 * @sa GetOriginalMemoryFunctions
 * @sa GetMemoryFunctions
 * @sa SetMemoryFunctions
 */
using malloc_func = SDL_malloc_func;

/**
 * A callback used to implement calloc().
 *
 * SDL will always ensure that the passed `nmemb` and `size` are both greater
 * than 0.
 *
 * @param nmemb the number of elements in the array.
 * @param size the size of each element of the array.
 * @returns a pointer to the allocated array, or nullptr if allocation failed.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa calloc
 * @sa GetOriginalMemoryFunctions
 * @sa GetMemoryFunctions
 * @sa SetMemoryFunctions
 */
using calloc_func = SDL_calloc_func;

/**
 * A callback used to implement realloc().
 *
 * SDL will always ensure that the passed `size` is greater than 0.
 *
 * @param mem a pointer to allocated memory to reallocate, or nullptr.
 * @param size the new size of the memory.
 * @returns a pointer to the newly allocated memory, or nullptr if allocation
 *          failed.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa realloc
 * @sa GetOriginalMemoryFunctions
 * @sa GetMemoryFunctions
 * @sa SetMemoryFunctions
 */
using realloc_func = SDL_realloc_func;

/**
 * A callback used to implement free().
 *
 * SDL will always ensure that the passed `mem` is a non-nullptr pointer.
 *
 * @param mem a pointer to allocated memory.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa free
 * @sa GetOriginalMemoryFunctions
 * @sa GetMemoryFunctions
 * @sa SetMemoryFunctions
 */
using free_func = SDL_free_func;

/**
 * Get the original set of SDL memory functions.
 *
 * This is what malloc and friends will use by default, if there has been
 * no call to SetMemoryFunctions. This is not necessarily using the C
 * runtime's `malloc` functions behind the scenes! Different platforms and
 * build configurations might do any number of unexpected things.
 *
 * @param malloc_func filled with malloc function.
 * @param calloc_func filled with calloc function.
 * @param realloc_func filled with realloc function.
 * @param free_func filled with free function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GetOriginalMemoryFunctions(malloc_func* malloc_func,
                                       calloc_func* calloc_func,
                                       realloc_func* realloc_func,
                                       free_func* free_func)
{
  SDL_GetOriginalMemoryFunctions(
    malloc_func, calloc_func, realloc_func, free_func);
}

/**
 * Get the current set of SDL memory functions.
 *
 * @param malloc_func filled with malloc function.
 * @param calloc_func filled with calloc function.
 * @param realloc_func filled with realloc function.
 * @param free_func filled with free function.
 *
 * @threadsafety This does not hold a lock, so do not call this in the
 *               unlikely event of a background thread calling
 *               SetMemoryFunctions simultaneously.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetMemoryFunctions
 * @sa GetOriginalMemoryFunctions
 */
inline void GetMemoryFunctions(malloc_func* malloc_func,
                               calloc_func* calloc_func,
                               realloc_func* realloc_func,
                               free_func* free_func)
{
  SDL_GetMemoryFunctions(malloc_func, calloc_func, realloc_func, free_func);
}

/**
 * Replace SDL's memory allocation functions with a custom set.
 *
 * It is not safe to call this function once any allocations have been made,
 * as future calls to free will use the new allocator, even if they came
 * from an malloc made with the old one!
 *
 * If used, usually this needs to be the first call made into the SDL library,
 * if not the very first thing done at program startup time.
 *
 * @param malloc_func custom malloc function.
 * @param calloc_func custom calloc function.
 * @param realloc_func custom realloc function.
 * @param free_func custom free function.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, but one
 *               should not replace the memory functions once any allocations
 *               are made!
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetMemoryFunctions
 * @sa GetOriginalMemoryFunctions
 */
inline void SetMemoryFunctions(malloc_func malloc_func,
                               calloc_func calloc_func,
                               realloc_func realloc_func,
                               free_func free_func)
{
  CheckError(
    SDL_SetMemoryFunctions(malloc_func, calloc_func, realloc_func, free_func));
}

/**
 * Allocate memory aligned to a specific alignment.
 *
 * The memory returned by this function must be freed with aligned_free(),
 * _not_ free().
 *
 * If `alignment` is less than the size of `void *`, it will be increased to
 * match that.
 *
 * The returned memory address will be a multiple of the alignment value, and
 * the size of the memory allocated will be a multiple of the alignment value.
 *
 * @param alignment the alignment of the memory.
 * @param size the size to allocate.
 * @returns a pointer to the aligned memory, or nullptr if allocation failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa aligned_free
 */
inline void* aligned_alloc(size_t alignment, size_t size)
{
  return SDL_aligned_alloc(alignment, size);
}

/**
 * Free memory allocated by aligned_alloc().
 *
 * The pointer is no longer valid after this call and cannot be dereferenced
 * anymore.
 *
 * If `mem` is nullptr, this function does nothing.
 *
 * @param mem a pointer previously returned by aligned_alloc(), or nullptr.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa aligned_alloc
 */
inline void aligned_free(void* mem) { SDL_aligned_free(mem); }

/**
 * Get the number of outstanding (unfreed) allocations.
 *
 * @returns the number of allocations or -1 if allocation counting is
 *          disabled.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetNumAllocations() { return SDL_GetNumAllocations(); }

/**
 * A thread-safe set of environment variables
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa GetEnvironment
 * @sa Environment.Environment
 * @sa Environment.GetVariable
 * @sa Environment.GetVariables
 * @sa Environment.SetVariable
 * @sa Environment.UnsetVariable
 * @sa Environment.Destroy
 */
class Environment
{

  EnvironmentRaw m_resource = nullptr;

public:
  constexpr Environment() = default;

  constexpr explicit Environment(const EnvironmentRaw resource)
    : m_resource(resource)
  {
  }

  Environment(const Environment& other) = delete;

  constexpr Environment(Environment&& other) { other.m_resource = nullptr; }

  /**
   * Create a set of environment variables
   *
   * @param populated true to initialize it from the C runtime environment,
   *                  false to create an empty environment.
   * @post a pointer to the new environment or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety If `populated` is false, it is safe to call this function
   *               from any thread, otherwise it is safe if no other threads are
   *               calling setenv() or unsetenv()
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Environment.GetVariable
   * @sa Environment.GetVariables
   * @sa Environment.SetVariable
   * @sa Environment.UnsetVariable
   * @sa Environment.Destroy
   */
  Environment(bool populated)
    : m_resource(SDL_CreateEnvironment(populated))
  {
  }

  ~Environment() { SDL_DestroyEnvironment(m_resource); }

  Environment& operator=(Environment other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  constexpr EnvironmentRaw get() const { return m_resource; }

  constexpr EnvironmentRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  constexpr operator EnvironmentParam() const { return {m_resource}; }

  /**
   * Get all variables in the environment.
   *
   * @returns a nullptr terminated array of pointers to environment variables in
   *          the form "variable=value" on success. This is wrapped to be
   *          auto-deleted, use FreeWrapper.release() if you want to manage
   *          manually.
   * @throws Error on failure
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetEnvironment
   * @sa Environment.Environment
   * @sa Environment.GetVariables
   * @sa Environment.SetVariable
   * @sa Environment.UnsetVariable
   */
  OwnArray<char*> GetVariables()
  {
    return OwnArray<char*>{CheckError(SDL_GetEnvironmentVariables(m_resource))};
  }

  /**
   * Get the Variables count.
   *
   * @return the number of existing environment variables
   *
   * This might be slow.
   */
  Uint64 GetVariableCount()
  {
    Uint64 count = 0;
    for (auto& var : GetVariables()) count += 1;
    return count;
  }

  /**
   * Destroy a set of environment variables.
   *
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the environment is no longer in use.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Environment.Environment
   */
  void Destroy()
  {
    SDL_DestroyEnvironment(m_resource);
    m_resource = nullptr;
  }

  /**
   * Get the value of a variable in the environment.
   *
   * @param name the name of the variable to get.
   * @returns a pointer to the value of the variable or nullptr if it can't be
   *          found.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetEnvironment
   * @sa Environment.Environment
   * @sa Environment.GetVariables
   * @sa Environment.SetVariable
   * @sa Environment.UnsetVariable
   */
  const char* GetVariable(StringParam name)
  {
    return SDL_GetEnvironmentVariable(m_resource, name);
  }

  /**
   * Set the value of a variable in the environment.
   *
   * @param name the name of the variable to set.
   * @param value the value of the variable to set.
   * @param overwrite true to overwrite the variable if it exists, false to
   *                  return success without setting the variable if it already
   *                  exists.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetEnvironment
   * @sa Environment.Environment
   * @sa Environment.GetVariable
   * @sa Environment.GetVariables
   * @sa Environment.UnsetVariable
   */
  void SetVariable(StringParam name, StringParam value, bool overwrite)
  {
    CheckError(SDL_SetEnvironmentVariable(m_resource, name, value, overwrite));
  }

  /**
   * Clear a variable from the environment.
   *
   * @param name the name of the variable to unset.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetEnvironment
   * @sa Environment.Environment
   * @sa Environment.GetVariable
   * @sa Environment.GetVariables
   * @sa Environment.SetVariable
   * @sa Environment.UnsetVariable
   */
  void UnsetVariable(StringParam name)
  {
    CheckError(SDL_UnsetEnvironmentVariable(m_resource, name));
  }
};

/**
 * Get the process environment.
 *
 * This is initialized at application start and is not affected by setenv()
 * and unsetenv() calls after that point. Use Environment.SetVariable() and
 * Environment.UnsetVariable() if you want to modify this environment, or
 * setenv_unsafe() or unsetenv_unsafe() if you want changes to persist
 * in the C runtime environment after Quit().
 *
 * @returns a pointer to the environment for the process or nullptr on failure;
 *          call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Environment.GetVariable
 * @sa Environment.GetVariables
 * @sa Environment.SetVariable
 * @sa Environment.UnsetVariable
 */
inline EnvironmentRaw GetEnvironment() { return SDL_GetEnvironment(); }

/**
 * Create a set of environment variables
 *
 * @param populated true to initialize it from the C runtime environment,
 *                  false to create an empty environment.
 * @returns a pointer to the new environment or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety If `populated` is false, it is safe to call this function
 *               from any thread, otherwise it is safe if no other threads are
 *               calling setenv() or unsetenv()
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Environment.GetVariable
 * @sa Environment.GetVariables
 * @sa Environment.SetVariable
 * @sa Environment.UnsetVariable
 * @sa Environment.Destroy
 */
inline Environment CreateEnvironment(bool populated)
{
  return Environment(SDL_CreateEnvironment(populated));
}

/**
 * Get the value of a variable in the environment.
 *
 * @param env the environment to query.
 * @param name the name of the variable to get.
 * @returns a pointer to the value of the variable or nullptr if it can't be
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetEnvironment
 * @sa Environment.Environment
 * @sa Environment.GetVariables
 * @sa Environment.SetVariable
 * @sa Environment.UnsetVariable
 */
inline const char* GetEnvironmentVariable(EnvironmentParam env,
                                          StringParam name)
{
  return SDL_GetEnvironmentVariable(env, name);
}

/**
 * Get all variables in the environment.
 *
 * @param env the environment to query.
 * @returns a nullptr terminated array of pointers to environment variables in
 *          the form "variable=value" or nullptr on failure; call GetError()
 *          for more information. This is a single allocation that should be
 *          freed with free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetEnvironment
 * @sa Environment.Environment
 * @sa Environment.GetVariables
 * @sa Environment.SetVariable
 * @sa Environment.UnsetVariable
 */
inline OwnArray<char*> GetEnvironmentVariables(EnvironmentParam env)
{
  return OwnArray<char*>{CheckError(SDL_GetEnvironmentVariables(env))};
}

/**
 * Set the value of a variable in the environment.
 *
 * @param env the environment to modify.
 * @param name the name of the variable to set.
 * @param value the value of the variable to set.
 * @param overwrite true to overwrite the variable if it exists, false to
 *                  return success without setting the variable if it already
 *                  exists.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetEnvironment
 * @sa Environment.Environment
 * @sa Environment.GetVariable
 * @sa Environment.GetVariables
 * @sa Environment.UnsetVariable
 */
inline void SetEnvironmentVariable(EnvironmentParam env,
                                   StringParam name,
                                   StringParam value,
                                   bool overwrite)
{
  return CheckError(SDL_SetEnvironmentVariable(env, name, value, overwrite));
}

/**
 * Clear a variable from the environment.
 *
 * @param env the environment to modify.
 * @param name the name of the variable to unset.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetEnvironment
 * @sa Environment.Environment
 * @sa Environment.GetVariable
 * @sa Environment.GetVariables
 * @sa Environment.SetVariable
 * @sa Environment.UnsetVariable
 */
inline void UnsetEnvironmentVariable(EnvironmentParam env, StringParam name)
{
  return CheckError(SDL_UnsetEnvironmentVariable(env, name));
}

/**
 * Destroy a set of environment variables.
 *
 * @param env the environment to destroy.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the environment is no longer in use.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Environment.Environment
 */
inline void DestroyEnvironment(EnvironmentRaw env)
{
  SDL_DestroyEnvironment(env);
}

/**
 * Get the value of a variable in the environment.
 *
 * This function uses SDL's cached copy of the environment and is thread-safe.
 *
 * @param name the name of the variable to get.
 * @returns a pointer to the value of the variable or nullptr if it can't be
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* getenv(StringParam name) { return SDL_getenv(name); }

/**
 * Get the value of a variable in the environment.
 *
 * This function bypasses SDL's cached copy of the environment and is not
 * thread-safe.
 *
 * @param name the name of the variable to get.
 * @returns a pointer to the value of the variable or nullptr if it can't be
 *          found.
 *
 * @threadsafety This function is not thread safe, consider using getenv()
 *               instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa getenv
 */
inline const char* getenv_unsafe(StringParam name)
{
  return SDL_getenv_unsafe(name);
}

/**
 * Set the value of a variable in the environment.
 *
 * @param name the name of the variable to set.
 * @param value the value of the variable to set.
 * @param overwrite 1 to overwrite the variable if it exists, 0 to return
 *                  success without setting the variable if it already exists.
 * @returns 0 on success, -1 on error.
 *
 * @threadsafety This function is not thread safe, consider using
 *               Environment.SetVariable() instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Environment.SetVariable
 */
inline int setenv_unsafe(StringParam name, StringParam value, int overwrite)
{
  return SDL_setenv_unsafe(name, value, overwrite);
}

/**
 * Clear a variable from the environment.
 *
 * @param name the name of the variable to unset.
 * @returns 0 on success, -1 on error.
 *
 * @threadsafety This function is not thread safe, consider using
 *               Environment.UnsetVariable() instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Environment.UnsetVariable
 */
inline int unsetenv_unsafe(StringParam name)
{
  return SDL_unsetenv_unsafe(name);
}

/**
 * A callback used with SDL sorting and binary search functions.
 *
 * @param a a pointer to the first element being compared.
 * @param b a pointer to the second element being compared.
 * @returns -1 if `a` should be sorted before `b`, 1 if `b` should be sorted
 *          before `a`, 0 if they are equal. If two elements are equal, their
 *          order in the sorted array is undefined.
 *
 * @since This callback is available since SDL 3.2.0.
 *
 * @sa bsearch
 * @sa qsort
 */
using CompareCallback = SDL_CompareCallback;

/**
 * Sort an array.
 *
 * For example:
 *
 * ```c
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *a, const void *b)
 * {
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->n < B->n) {
 *         return -1;
 *     } else if (B->n < A->n) {
 *         return 1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 3, "third" }, { 1, "first" }, { 2, "second" }
 * };
 *
 * qsort(values, arraysize(values), sizeof(values[0]), compare);
 * ```
 *
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa bsearch
 * @sa qsort_r
 */
inline void qsort(void* base,
                  size_t nmemb,
                  size_t size,
                  CompareCallback compare)
{
  SDL_qsort(base, nmemb, size, compare);
}

/**
 * Perform a binary search on a previously sorted array.
 *
 * For example:
 *
 * ```c
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *a, const void *b)
 * {
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->n < B->n) {
 *         return -1;
 *     } else if (B->n < A->n) {
 *         return 1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 1, "first" }, { 2, "second" }, { 3, "third" }
 * };
 * data key = { 2, nullptr };
 *
 * data *result = bsearch(&key, values, arraysize(values), sizeof(values[0]),
 * compare);
 * ```
 *
 * @param key a pointer to a key equal to the element being searched for.
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 * @returns a pointer to the matching element in the array, or nullptr if not
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa bsearch_r
 * @sa qsort
 */
inline void* bsearch(const void* key,
                     const void* base,
                     size_t nmemb,
                     size_t size,
                     CompareCallback compare)
{
  return SDL_bsearch(key, base, nmemb, size, compare);
}

/**
 * A callback used with SDL sorting and binary search functions.
 *
 * @param userdata the `userdata` pointer passed to the sort function.
 * @param a a pointer to the first element being compared.
 * @param b a pointer to the second element being compared.
 * @returns -1 if `a` should be sorted before `b`, 1 if `b` should be sorted
 *          before `a`, 0 if they are equal. If two elements are equal, their
 *          order in the sorted array is undefined.
 *
 * @since This callback is available since SDL 3.2.0.
 *
 * @sa qsort_r
 * @sa bsearch_r
 */
using CompareCallback_r = SDL_CompareCallback_r;

/**
 * A callback used with SDL sorting and binary search functions.
 *
 * @param a a pointer to the first element being compared.
 * @param b a pointer to the second element being compared.
 * @returns -1 if `a` should be sorted before `b`, 1 if `b` should be sorted
 *          before `a`, 0 if they are equal. If two elements are equal, their
 *          order in the sorted array is undefined.
 *
 * @since This callback is available since SDL 3.2.0.
 *
 * @sa qsort_r
 * @sa bsearch_r
 * @sa CompareCallback_r
 */
using CompareCB = std::function<int(const void*, const void*)>;

/**
 * Sort an array, passing a userdata pointer to the compare function.
 *
 * For example:
 *
 * ```c
 * typedef enum {
 *     sort_increasing,
 *     sort_decreasing,
 * } sort_method;
 *
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *userdata, const void *a, const void *b)
 * {
 *     sort_method method = (sort_method)(uintptr_t)userdata;
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->key < B->key) {
 *         return (method == sort_increasing) ? -1 : 1;
 *     } else if (B->key < A->key) {
 *         return (method == sort_increasing) ? 1 : -1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 3, "third" }, { 1, "first" }, { 2, "second" }
 * };
 *
 * qsort_r(values, arraysize(values), sizeof(values[0]), compare, (const void
 * *)(uintptr_t)sort_increasing);
 * ```
 *
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 * @param userdata a pointer to pass to the compare function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa bsearch_r
 * @sa qsort
 */
inline void qsort_r(void* base,
                    size_t nmemb,
                    size_t size,
                    CompareCallback_r compare,
                    void* userdata)
{
  SDL_qsort_r(base, nmemb, size, compare, userdata);
}

/**
 * Sort an array, passing a userdata pointer to the compare function.
 *
 * For example:
 *
 * ```c
 * typedef enum {
 *     sort_increasing,
 *     sort_decreasing,
 * } sort_method;
 *
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *userdata, const void *a, const void *b)
 * {
 *     sort_method method = (sort_method)(uintptr_t)userdata;
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->key < B->key) {
 *         return (method == sort_increasing) ? -1 : 1;
 *     } else if (B->key < A->key) {
 *         return (method == sort_increasing) ? 1 : -1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 3, "third" }, { 1, "first" }, { 2, "second" }
 * };
 *
 * qsort_r(values, arraysize(values), sizeof(values[0]), compare, (const void
 * *)(uintptr_t)sort_increasing);
 * ```
 *
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa bsearch_r
 * @sa qsort
 */
inline void qsort_r(void* base, size_t nmemb, size_t size, CompareCB compare)
{
  return qsort_r(
    base,
    nmemb,
    size,
    [](void* userdata, const void* a, const void* b) {
      auto& cb = *static_cast<CompareCB*>(userdata);
      return cb(a, b);
    },
    &compare);
}

/**
 * Perform a binary search on a previously sorted array, passing a userdata
 * pointer to the compare function.
 *
 * For example:
 *
 * ```c
 * typedef enum {
 *     sort_increasing,
 *     sort_decreasing,
 * } sort_method;
 *
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *userdata, const void *a, const void *b)
 * {
 *     sort_method method = (sort_method)(uintptr_t)userdata;
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->key < B->key) {
 *         return (method == sort_increasing) ? -1 : 1;
 *     } else if (B->key < A->key) {
 *         return (method == sort_increasing) ? 1 : -1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 1, "first" }, { 2, "second" }, { 3, "third" }
 * };
 * data key = { 2, nullptr };
 *
 * data *result = bsearch_r(&key, values, arraysize(values), sizeof(values[0]),
 * compare, (const void *)(uintptr_t)sort_increasing);
 * ```
 *
 * @param key a pointer to a key equal to the element being searched for.
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 * @param userdata a pointer to pass to the compare function.
 * @returns a pointer to the matching element in the array, or nullptr if not
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa bsearch
 * @sa qsort_r
 */
inline void* bsearch_r(const void* key,
                       const void* base,
                       size_t nmemb,
                       size_t size,
                       CompareCallback_r compare,
                       void* userdata)
{
  return SDL_bsearch_r(key, base, nmemb, size, compare, userdata);
}

/**
 * Perform a binary search on a previously sorted array, passing a userdata
 * pointer to the compare function.
 *
 * For example:
 *
 * ```c
 * typedef enum {
 *     sort_increasing,
 *     sort_decreasing,
 * } sort_method;
 *
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *userdata, const void *a, const void *b)
 * {
 *     sort_method method = (sort_method)(uintptr_t)userdata;
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->key < B->key) {
 *         return (method == sort_increasing) ? -1 : 1;
 *     } else if (B->key < A->key) {
 *         return (method == sort_increasing) ? 1 : -1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 1, "first" }, { 2, "second" }, { 3, "third" }
 * };
 * data key = { 2, nullptr };
 *
 * data *result = bsearch_r(&key, values, arraysize(values), sizeof(values[0]),
 * compare, (const void *)(uintptr_t)sort_increasing);
 * ```
 *
 * @param key a pointer to a key equal to the element being searched for.
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 * @returns a pointer to the matching element in the array, or nullptr if not
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa bsearch
 * @sa qsort_r
 */
inline void* bsearch_r(const void* key,
                       const void* base,
                       size_t nmemb,
                       size_t size,
                       CompareCB compare)
{
  return bsearch_r(
    key,
    base,
    nmemb,
    size,
    [](void* userdata, const void* a, const void* b) {
      auto& cb = *static_cast<CompareCB*>(userdata);
      return cb(a, b);
    },
    &compare);
}

/**
 * Compute the absolute value of `x`.
 *
 * @param x an integer value.
 * @returns the absolute value of x.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int abs(int x) { return SDL_abs(x); }

/**
 * Compute the absolute value of `x`
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * abs for single-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @returns the absolute value of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa abs
 */
inline double abs(double x) { return SDL_fabs(x); }

/**
 * Compute the absolute value of `x`
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * abs for double-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @returns the absolute value of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa abs
 */
inline float abs(float x) { return SDL_fabsf(x); }

/**
 * Return the lesser of two values.
 *
 * This is a helper macro that might be more clear than writing out the
 * comparisons directly, and works with any type that can be compared with the
 * `<` operator. However, it double-evaluates both its parameters, so do not
 * use expressions with side-effects here.
 *
 * @param x the first value to compare.
 * @param y the second value to compare.
 * @returns the lesser of `x` and `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
template<class T, class U>
constexpr T min(T x, U y)
{
  return SDL_min(x, y);
}

/**
 * Return the greater of two values.
 *
 * This is a helper function that might be more clear than writing out the
 * comparisons directly, and works with any type that can be compared with the
 * `>` operator. However, it double-evaluates both its parameters, so do not
 * use expressions with side-effects here.
 *
 * @param x the first value to compare.
 * @param y the second value to compare.
 * @returns the lesser of `x` and `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
template<class T, class U>
constexpr T max(T x, U y)
{
  return SDL_max(x, y);
}

/**
 * Return a value clamped to a range.
 *
 * If `x` is outside the range a values between `a` and `b`, the returned
 * value will be `a` or `b` as appropriate. Otherwise, `x` is returned.
 *
 * This function will produce incorrect results if `b` is less than `a`.
 *
 * This is a helper function that might be more clear than writing out the
 * comparisons directly, and works with any type that can be compared with the
 * `<` and `>` operators. However, it double-evaluates all its parameters, so
 * do not use expressions with side-effects here.
 *
 * @param x the value to compare.
 * @param a the low end value.
 * @param b the high end value.
 * @returns x, clamped between a and b.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
template<class T, class U, class V>
constexpr T clamp(T x, U a, V b)
{
  return SDL_clamp(x, a, b);
}

/**
 * Query if a character is alphabetic (a letter).
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * for English 'a-z' and 'A-Z' as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int isalpha(int x) { return SDL_isalpha(x); }

/**
 * Query if a character is alphabetic (a letter) or a number.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * for English 'a-z', 'A-Z', and '0-9' as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int isalnum(int x) { return SDL_isalnum(x); }

/**
 * Report if a character is blank (a space or tab).
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 0x20 (space) or 0x9 (tab) as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int isblank(int x) { return SDL_isblank(x); }

/**
 * Report if a character is a control character.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 0 through 0x1F, and 0x7F, as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int iscntrl(int x) { return SDL_iscntrl(x); }

/**
 * Report if a character is a numeric digit.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * '0' (0x30) through '9' (0x39), as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int isdigit(int x) { return SDL_isdigit(x); }

/**
 * Report if a character is a hexadecimal digit.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 'A' through 'F', 'a' through 'f', and '0' through '9', as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int isxdigit(int x) { return SDL_isxdigit(x); }

/**
 * Report if a character is a punctuation mark.
 *
 * **WARNING**: Regardless of system locale, this is equivalent to
 * `((isgraph(x)) && (!isalnum(x)))`.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa isgraph
 * @sa isalnum
 */
inline int ispunct(int x) { return SDL_ispunct(x); }

/**
 * Report if a character is whitespace.
 *
 * **WARNING**: Regardless of system locale, this will only treat the
 * following ASCII values as true:
 *
 * - space (0x20)
 * - tab (0x09)
 * - newline (0x0A)
 * - vertical tab (0x0B)
 * - form feed (0x0C)
 * - return (0x0D)
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int isspace(int x) { return SDL_isspace(x); }

/**
 * Report if a character is upper case.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 'A' through 'Z' as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int isupper(int x) { return SDL_isupper(x); }

/**
 * Report if a character is lower case.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 'a' through 'z' as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int islower(int x) { return SDL_islower(x); }

/**
 * Report if a character is "printable".
 *
 * Be advised that "printable" has a definition that goes back to text
 * terminals from the dawn of computing, making this a sort of special case
 * function that is not suitable for Unicode (or most any) text management.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * ' ' (0x20) through '~' (0x7E) as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int isprint(int x) { return SDL_isprint(x); }

/**
 * Report if a character is any "printable" except space.
 *
 * Be advised that "printable" has a definition that goes back to text
 * terminals from the dawn of computing, making this a sort of special case
 * function that is not suitable for Unicode (or most any) text management.
 *
 * **WARNING**: Regardless of system locale, this is equivalent to
 * `(isprint(x)) && ((x) != ' ')`.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa isprint
 */
inline int isgraph(int x) { return SDL_isgraph(x); }

/**
 * Convert low-ASCII English letters to uppercase.
 *
 * **WARNING**: Regardless of system locale, this will only convert ASCII
 * values 'a' through 'z' to uppercase.
 *
 * This function returns the uppercase equivalent of `x`. If a character
 * cannot be converted, or is already uppercase, this function returns `x`.
 *
 * @param x character value to check.
 * @returns capitalized version of x, or x if no conversion available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int toupper(int x) { return SDL_toupper(x); }

/**
 * Convert low-ASCII English letters to lowercase.
 *
 * **WARNING**: Regardless of system locale, this will only convert ASCII
 * values 'A' through 'Z' to lowercase.
 *
 * This function returns the lowercase equivalent of `x`. If a character
 * cannot be converted, or is already lowercase, this function returns `x`.
 *
 * @param x character value to check.
 * @returns lowercase version of x, or x if no conversion available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int tolower(int x) { return SDL_tolower(x); }

/**
 * Calculate a CRC-16 value.
 *
 * https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 *
 * This function can be called multiple times, to stream data to be
 * checksummed in blocks. Each call must provide the previous CRC-16 return
 * value to be updated with the next block. The first call to this function
 * for a set of blocks should pass in a zero CRC value.
 *
 * @param crc the current checksum for this data set, or 0 for a new data set.
 * @param data a new block of data to add to the checksum.
 * @param len the size, in bytes, of the new block of data.
 * @returns a CRC-16 checksum value of all blocks in the data set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint16 crc16(Uint16 crc, const void* data, size_t len)
{
  return SDL_crc16(crc, data, len);
}

/**
 * Calculate a CRC-32 value.
 *
 * https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 *
 * This function can be called multiple times, to stream data to be
 * checksummed in blocks. Each call must provide the previous CRC-32 return
 * value to be updated with the next block. The first call to this function
 * for a set of blocks should pass in a zero CRC value.
 *
 * @param crc the current checksum for this data set, or 0 for a new data set.
 * @param data a new block of data to add to the checksum.
 * @param len the size, in bytes, of the new block of data.
 * @returns a CRC-32 checksum value of all blocks in the data set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint32 crc32(Uint32 crc, const void* data, size_t len)
{
  return SDL_crc32(crc, data, len);
}

/**
 * Calculate a 32-bit MurmurHash3 value for a block of data.
 *
 * https://en.wikipedia.org/wiki/MurmurHash
 *
 * A seed may be specified, which changes the final results consistently, but
 * this does not work like crc16 and crc32: you can't feed a previous
 * result from this function back into itself as the next seed value to
 * calculate a hash in chunks; it won't produce the same hash as it would if
 * the same data was provided in a single call.
 *
 * If you aren't sure what to provide for a seed, zero is fine. Murmur3 is not
 * cryptographically secure, so it shouldn't be used for hashing top-secret
 * data.
 *
 * @param data the data to be hashed.
 * @param len the size of data, in bytes.
 * @param seed a value that alters the final hash value.
 * @returns a Murmur3 32-bit hash value.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint32 murmur3_32(const void* data, size_t len, Uint32 seed)
{
  return SDL_murmur3_32(data, len, seed);
}

/**
 * Copy non-overlapping memory.
 *
 * The memory regions must not overlap. If they do, use memmove() instead.
 *
 * @param dst The destination memory region. Must not be nullptr, and must not
 *            overlap with `src`.
 * @param src The source memory region. Must not be nullptr, and must not
 * overlap with `dst`.
 * @param len The length in bytes of both `dst` and `src`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa memmove
 */
inline void* memcpy(void* dst, const void* src, size_t len)
{
#ifdef SDL_SLOW_MEMCPY
  return SDL_memcpy(dst, src, len);
#else
  return ::memcpy(dst, src, len);
#endif // SDL_SLOW_MEMCPY
}

#ifdef SDL3PP_DOC

/**
 * A macro to copy memory between objects, with basic type checking.
 *
 * memcpy and memmove do not care where you copy memory to and from,
 * which can lead to bugs. This macro aims to avoid most of those bugs by
 * making sure that the source and destination are both pointers to objects
 * that are the same size. It does not check that the objects are the same
 * _type_, just that the copy will not overflow either object.
 *
 * The size check happens at compile time, and the compiler will throw an
 * error if the objects are different sizes.
 *
 * Generally this is intended to copy a single object, not an array.
 *
 * This macro looks like it double-evaluates its parameters, but the extras
 * them are in `sizeof` sections, which generate no code nor side-effects.
 *
 * @param dst a pointer to the destination object. Must not be nullptr.
 * @param src a pointer to the source object. Must not be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
#define SDL_copyp(dst, src)                                                    \
  {                                                                            \
    SDL_COMPILE_TIME_ASSERT(SDL_copyp, sizeof(*(dst)) == sizeof(*(src)));      \
  }                                                                            \
  SDL_memcpy((dst), (src), sizeof(*(src)))

#endif // SDL3PP_DOC

/**
 * Copy memory ranges that might overlap.
 *
 * It is okay for the memory regions to overlap. If you are confident that the
 * regions never overlap, using memcpy() may improve performance.
 *
 * @param dst The destination memory region. Must not be nullptr.
 * @param src The source memory region. Must not be nullptr.
 * @param len The length in bytes of both `dst` and `src`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa memcpy
 */
inline void* memmove(void* dst, const void* src, size_t len)
{
#ifdef SDL_SLOW_MEMMOVE
  return SDL_memmove(dst, src, len);
#else
  return ::memmove(dst, src, len);
#endif // SDL_SLOW_MEMMOVE
}

/**
 * Initialize all bytes of buffer of memory to a specific value.
 *
 * This function will set `len` bytes, pointed to by `dst`, to the value
 * specified in `c`.
 *
 * Despite `c` being an `int` instead of a `char`, this only operates on
 * bytes; `c` must be a value between 0 and 255, inclusive.
 *
 * @param dst the destination memory region. Must not be nullptr.
 * @param c the byte value to set.
 * @param len the length, in bytes, to set in `dst`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void* memset(void* dst, int c, size_t len)
{
#ifdef SDL_SLOW_MEMSET
  return SDL_memset(dst, c, len);
#else
  return ::memset(dst, c, len);
#endif // SDL_SLOW_MEMSET
}

/**
 * Initialize all 32-bit words of buffer of memory to a specific value.
 *
 * This function will set a buffer of `dwords` Uint32 values, pointed to by
 * `dst`, to the value specified in `val`.
 *
 * Unlike memset, this sets 32-bit values, not bytes, so it's not limited
 * to a range of 0-255.
 *
 * @param dst the destination memory region. Must not be nullptr.
 * @param val the Uint32 value to set.
 * @param dwords the number of Uint32 values to set in `dst`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void* memset4(void* dst, Uint32 val, size_t dwords)
{
  return SDL_memset4(dst, val, dwords);
}

/**
 * Clear an object's memory to zero.
 *
 * This is wrapper over memset that handles calculating the object size,
 * so there's no chance of copy/paste errors, and the code is cleaner.
 *
 * This requires an object, not a pointer to an object, nor an array.
 *
 * @param x the object to clear.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa zerop
 * @sa zeroa
 */
template<class T>
inline void zero(T& x)
{
  SDL_zero(x);
}

/**
 * Clear an object's memory to zero, using a pointer.
 *
 * This is wrapper over memset that handles calculating the object size,
 * so there's no chance of copy/paste errors, and the code is cleaner.
 *
 * This requires a pointer to an object, not an object itself, nor an array.
 *
 * @param x a pointer to the object to clear.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa zero
 * @sa zeroa
 */
template<class T>
inline void zerop(T* x)
{
  SDL_zerop(x);
}

/**
 * Clear an array's memory to zero.
 *
 * This is wrapper over memset that handles calculating the array size, so
 * there's no chance of copy/paste errors, and the code is cleaner.
 *
 * This requires an array, not an object, nor a pointer to an object.
 *
 * @param x an array to clear.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa zero
 * @sa zeroa
 */
template<class T, std::size_t N>
inline void zeroa(T (&x)[N])
{
  SDL_zeroa(x);
}

/**
 * Compare two buffers of memory.
 *
 * @param s1 the first buffer to compare. nullptr is not permitted!
 * @param s2 the second buffer to compare. nullptr is not permitted!
 * @param len the number of bytes to compare between the buffers.
 * @returns less than zero if s1 is "less than" s2, greater than zero if s1 is
 *          "greater than" s2, and zero if the buffers match exactly for `len`
 *          bytes.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int memcmp(const void* s1, const void* s2, size_t len)
{
  return SDL_memcmp(s1, s2, len);
}

/**
 * This works exactly like wcslen() but doesn't require access to a C runtime.
 *
 * Counts the number of wchar_t values in `wstr`, excluding the null
 * terminator.
 *
 * Like strlen only counts bytes and not codepoints in a UTF-8 string,
 * this counts wchar_t values in a string, even if the string's encoding is of
 * variable width, like UTF-16.
 *
 * Also be aware that wchar_t is different sizes on different platforms (4
 * bytes on Linux, 2 on Windows, etc).
 *
 * @param wstr The null-terminated wide string to read. Must not be nullptr.
 * @returns the length (in wchar_t values, excluding the null terminator) of
 *          `wstr`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa wcsnlen
 * @sa utf8strlen
 * @sa utf8strnlen
 */
inline size_t wcslen(const wchar_t* wstr) { return SDL_wcslen(wstr); }

/**
 * This works exactly like wcsnlen() but doesn't require access to a C
 * runtime.
 *
 * Counts up to a maximum of `maxlen` wchar_t values in `wstr`, excluding the
 * null terminator.
 *
 * Like strnlen only counts bytes and not codepoints in a UTF-8 string,
 * this counts wchar_t values in a string, even if the string's encoding is of
 * variable width, like UTF-16.
 *
 * Also be aware that wchar_t is different sizes on different platforms (4
 * bytes on Linux, 2 on Windows, etc).
 *
 * Also, `maxlen` is a count of wide characters, not bytes!
 *
 * @param wstr The null-terminated wide string to read. Must not be nullptr.
 * @param maxlen The maximum amount of wide characters to count.
 * @returns the length (in wide characters, excluding the null terminator) of
 *          `wstr` but never more than `maxlen`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa wcslen
 * @sa utf8strlen
 * @sa utf8strnlen
 */
inline size_t wcsnlen(const wchar_t* wstr, size_t maxlen)
{
  return SDL_wcsnlen(wstr, maxlen);
}

/**
 * Copy a wide string.
 *
 * This function copies `maxlen` - 1 wide characters from `src` to `dst`, then
 * appends a null terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * If `maxlen` is 0, no wide characters are copied and no null terminator is
 * written.
 *
 * @param dst The destination buffer. Must not be nullptr, and must not overlap
 *            with `src`.
 * @param src The null-terminated wide string to copy. Must not be nullptr, and
 *            must not overlap with `dst`.
 * @param maxlen The length (in wide characters) of the destination buffer.
 * @returns the length (in wide characters, excluding the null terminator) of
 *          `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa wcslcat
 */
inline size_t wcslcpy(wchar_t* dst, const wchar_t* src, size_t maxlen)
{
  return SDL_wcslcpy(dst, src, maxlen);
}

/**
 * Concatenate wide strings.
 *
 * This function appends up to `maxlen` - wcslen(dst) - 1 wide characters
 * from `src` to the end of the wide string in `dst`, then appends a null
 * terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * If `maxlen` - wcslen(dst) - 1 is less than or equal to 0, then `dst` is
 * unmodified.
 *
 * @param dst The destination buffer already containing the first
 *            null-terminated wide string. Must not be nullptr and must not
 *            overlap with `src`.
 * @param src The second null-terminated wide string. Must not be nullptr, and
 *            must not overlap with `dst`.
 * @param maxlen The length (in wide characters) of the destination buffer.
 * @returns the length (in wide characters, excluding the null terminator) of
 *          the string in `dst` plus the length of `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa wcslcpy
 */
inline size_t wcslcat(wchar_t* dst, const wchar_t* src, size_t maxlen)
{
  return SDL_wcslcat(dst, src, maxlen);
}

/**
 * Allocate a copy of a wide string.
 *
 * This allocates enough space for a null-terminated copy of `wstr`, using
 * malloc, and then makes a copy of the string into this space.
 *
 * The returned string is owned by the caller, and should be passed to
 * free when no longer needed.
 *
 * @param wstr the string to copy.
 * @returns a pointer to the newly-allocated wide string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline wchar_t* wcsdup(const wchar_t* wstr) { return SDL_wcsdup(wstr); }

/**
 * Search a wide string for the first instance of a specific substring.
 *
 * The search ends once it finds the requested substring, or a null terminator
 * byte to end the string.
 *
 * Note that this looks for strings of _wide characters_, not _codepoints_, so
 * it's legal to search for malformed and incomplete UTF-16 sequences.
 *
 * @param haystack the wide string to search. Must not be nullptr.
 * @param needle the wide string to search for. Must not be nullptr.
 * @returns a pointer to the first instance of `needle` in the string, or
 * nullptr if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline wchar_t* wcsstr(const wchar_t* haystack, const wchar_t* needle)
{
  return SDL_wcsstr(haystack, needle);
}

/**
 * Search a wide string, up to n wide chars, for the first instance of a
 * specific substring.
 *
 * The search ends once it finds the requested substring, or a null terminator
 * value to end the string, or `maxlen` wide character have been examined. It
 * is possible to use this function on a wide string without a null
 * terminator.
 *
 * Note that this looks for strings of _wide characters_, not _codepoints_, so
 * it's legal to search for malformed and incomplete UTF-16 sequences.
 *
 * @param haystack the wide string to search. Must not be nullptr.
 * @param needle the wide string to search for. Must not be nullptr.
 * @param maxlen the maximum number of wide characters to search in
 *               `haystack`.
 * @returns a pointer to the first instance of `needle` in the string, or
 * nullptr if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline wchar_t* wcsnstr(const wchar_t* haystack,
                        const wchar_t* needle,
                        size_t maxlen)
{
  return SDL_wcsnstr(haystack, needle, maxlen);
}

/**
 * Compare two null-terminated wide strings.
 *
 * This only compares wchar_t values until it hits a null-terminating
 * character; it does not care if the string is well-formed UTF-16 (or UTF-32,
 * depending on your platform's wchar_t size), or uses valid Unicode values.
 *
 * @param str1 the first string to compare. nullptr is not permitted!
 * @param str2 the second string to compare. nullptr is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int wcscmp(const wchar_t* str1, const wchar_t* str2)
{
  return SDL_wcscmp(str1, str2);
}

/**
 * Compare two wide strings up to a number of wchar_t values.
 *
 * This only compares wchar_t values; it does not care if the string is
 * well-formed UTF-16 (or UTF-32, depending on your platform's wchar_t size),
 * or uses valid Unicode values.
 *
 * Note that while this function is intended to be used with UTF-16 (or
 * UTF-32, depending on your platform's definition of wchar_t), it is
 * comparing raw wchar_t values and not Unicode codepoints: `maxlen` specifies
 * a wchar_t limit! If the limit lands in the middle of a multi-wchar UTF-16
 * sequence, it will only compare a portion of the final character.
 *
 * `maxlen` specifies a maximum number of wchar_t to compare; if the strings
 * match to this number of wide chars (or both have matched to a
 * null-terminator character before this count), they will be considered
 * equal.
 *
 * @param str1 the first string to compare. nullptr is not permitted!
 * @param str2 the second string to compare. nullptr is not permitted!
 * @param maxlen the maximum number of wchar_t to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int wcsncmp(const wchar_t* str1, const wchar_t* str2, size_t maxlen)
{
  return SDL_wcsncmp(str1, str2, maxlen);
}

/**
 * Compare two null-terminated wide strings, case-insensitively.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Depending on your platform, "wchar_t" might be 2 bytes, and expected to be
 * UTF-16 encoded (like Windows), or 4 bytes in UTF-32 format. Since this
 * handles Unicode, it expects the string to be well-formed and not a
 * null-terminated string of arbitrary bytes. Characters that are not valid
 * UTF-16 (or UTF-32) are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * @param str1 the first string to compare. nullptr is not permitted!
 * @param str2 the second string to compare. nullptr is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int wcscasecmp(const wchar_t* str1, const wchar_t* str2)
{
  return SDL_wcscasecmp(str1, str2);
}

/**
 * Compare two wide strings, case-insensitively, up to a number of wchar_t.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Depending on your platform, "wchar_t" might be 2 bytes, and expected to be
 * UTF-16 encoded (like Windows), or 4 bytes in UTF-32 format. Since this
 * handles Unicode, it expects the string to be well-formed and not a
 * null-terminated string of arbitrary bytes. Characters that are not valid
 * UTF-16 (or UTF-32) are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * Note that while this function might deal with variable-sized characters,
 * `maxlen` specifies a _wchar_ limit! If the limit lands in the middle of a
 * multi-byte UTF-16 sequence, it may convert a portion of the final character
 * to one or more Unicode character U+FFFD (REPLACEMENT CHARACTER) so as not
 * to overflow a buffer.
 *
 * `maxlen` specifies a maximum number of wchar_t values to compare; if the
 * strings match to this number of wchar_t (or both have matched to a
 * null-terminator character before this number of bytes), they will be
 * considered equal.
 *
 * @param str1 the first string to compare. nullptr is not permitted!
 * @param str2 the second string to compare. nullptr is not permitted!
 * @param maxlen the maximum number of wchar_t values to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int wcsncasecmp(const wchar_t* str1, const wchar_t* str2, size_t maxlen)
{
  return SDL_wcsncasecmp(str1, str2, maxlen);
}

/**
 * Parse a `long` from a wide string.
 *
 * If `str` starts with whitespace, then those whitespace characters are
 * skipped before attempting to parse the number.
 *
 * If the parsed number does not fit inside a `long`, the result is clamped to
 * the minimum and maximum representable `long` values.
 *
 * @param str The null-terminated wide string to read. Must not be nullptr.
 * @param endp If not nullptr, the address of the first invalid wide character
 *             (i.e. the next character after the parsed number) will be
 *             written to this pointer.
 * @param base The base of the integer to read. Supported values are 0 and 2
 *             to 36 inclusive. If 0, the base will be inferred from the
 *             number's prefix (0x for hexadecimal, 0 for octal, decimal
 *             otherwise).
 * @returns the parsed `long`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa strtol
 */
inline long wcstol(const wchar_t* str, wchar_t** endp, int base)
{
  return SDL_wcstol(str, endp, base);
}

/**
 * This works exactly like strlen() but doesn't require access to a C runtime.
 *
 * Counts the bytes in `str`, excluding the null terminator.
 *
 * If you need the length of a UTF-8 string, consider using utf8strlen().
 *
 * @param str The null-terminated string to read. Must not be nullptr.
 * @returns the length (in bytes, excluding the null terminator) of `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa strnlen
 * @sa utf8strlen
 * @sa utf8strnlen
 */
inline size_t strlen(StringParam str) { return SDL_strlen(str); }

/**
 * This works exactly like strnlen() but doesn't require access to a C
 * runtime.
 *
 * Counts up to a maximum of `maxlen` bytes in `str`, excluding the null
 * terminator.
 *
 * If you need the length of a UTF-8 string, consider using utf8strnlen().
 *
 * @param str The null-terminated string to read. Must not be nullptr.
 * @param maxlen The maximum amount of bytes to count.
 * @returns the length (in bytes, excluding the null terminator) of `src` but
 *          never more than `maxlen`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa strlen
 * @sa utf8strlen
 * @sa utf8strnlen
 */
inline size_t strnlen(StringParam str, size_t maxlen)
{
  return SDL_strnlen(str, maxlen);
}

/**
 * Copy a string.
 *
 * This function copies up to `maxlen` - 1 characters from `src` to `dst`,
 * then appends a null terminator.
 *
 * If `maxlen` is 0, no characters are copied and no null terminator is
 * written.
 *
 * If you want to copy an UTF-8 string but need to ensure that multi-byte
 * sequences are not truncated, consider using utf8strlcpy().
 *
 * @param dst The destination buffer. Must not be nullptr, and must not overlap
 *            with `src`.
 * @param src The null-terminated string to copy. Must not be nullptr, and must
 *            not overlap with `dst`.
 * @param maxlen The length (in characters) of the destination buffer.
 * @returns the length (in characters, excluding the null terminator) of
 *          `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa strlcat
 * @sa utf8strlcpy
 */
inline size_t strlcpy(char* dst, StringParam src, size_t maxlen)
{
  return SDL_strlcpy(dst, src, maxlen);
}

/**
 * Copy an UTF-8 string.
 *
 * This function copies up to `dst_bytes` - 1 bytes from `src` to `dst` while
 * also ensuring that the string written to `dst` does not end in a truncated
 * multi-byte sequence. Finally, it appends a null terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * Note that unlike strlcpy(), this function returns the number of bytes
 * written, not the length of `src`.
 *
 * @param dst The destination buffer. Must not be nullptr, and must not overlap
 *            with `src`.
 * @param src The null-terminated UTF-8 string to copy. Must not be nullptr, and
 *            must not overlap with `dst`.
 * @param dst_bytes The length (in bytes) of the destination buffer. Must not
 *                  be 0.
 * @returns the number of bytes written, excluding the null terminator.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa strlcpy
 */
inline size_t utf8strlcpy(char* dst, StringParam src, size_t dst_bytes)
{
  return SDL_utf8strlcpy(dst, src, dst_bytes);
}

/**
 * Concatenate strings.
 *
 * This function appends up to `maxlen` - strlen(dst) - 1 characters from
 * `src` to the end of the string in `dst`, then appends a null terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * If `maxlen` - strlen(dst) - 1 is less than or equal to 0, then `dst` is
 * unmodified.
 *
 * @param dst The destination buffer already containing the first
 *            null-terminated string. Must not be nullptr and must not overlap
 *            with `src`.
 * @param src The second null-terminated string. Must not be nullptr, and must
 *            not overlap with `dst`.
 * @param maxlen The length (in characters) of the destination buffer.
 * @returns the length (in characters, excluding the null terminator) of the
 *          string in `dst` plus the length of `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa strlcpy
 */
inline size_t strlcat(char* dst, StringParam src, size_t maxlen)
{
  return SDL_strlcat(dst, src, maxlen);
}

/**
 * Allocate a copy of a string.
 *
 * This allocates enough space for a null-terminated copy of `str`, using
 * malloc, and then makes a copy of the string into this space.
 *
 * The returned string is owned by the caller, and should be passed to
 * free when no longer needed.
 *
 * @param str the string to copy.
 * @returns a pointer to the newly-allocated string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strdup(StringParam str) { return SDL_strdup(str); }

/**
 * Allocate a copy of a string, up to n characters.
 *
 * This allocates enough space for a null-terminated copy of `str`, up to
 * `maxlen` bytes, using malloc, and then makes a copy of the string into
 * this space.
 *
 * If the string is longer than `maxlen` bytes, the returned string will be
 * `maxlen` bytes long, plus a null-terminator character that isn't included
 * in the count.
 *
 * The returned string is owned by the caller, and should be passed to
 * free when no longer needed.
 *
 * @param str the string to copy.
 * @param maxlen the maximum length of the copied string, not counting the
 *               null-terminator character.
 * @returns a pointer to the newly-allocated string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strndup(StringParam str, size_t maxlen)
{
  return SDL_strndup(str, maxlen);
}

/**
 * Reverse a string's contents.
 *
 * This reverses a null-terminated string in-place. Only the content of the
 * string is reversed; the null-terminator character remains at the end of the
 * reversed string.
 *
 * **WARNING**: This function reverses the _bytes_ of the string, not the
 * codepoints. If `str` is a UTF-8 string with Unicode codepoints > 127, this
 * will ruin the string data. You should only use this function on strings
 * that are completely comprised of low ASCII characters.
 *
 * @param str the string to reverse.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strrev(char* str) { return SDL_strrev(str); }

/**
 * Convert a string to uppercase.
 *
 * **WARNING**: Regardless of system locale, this will only convert ASCII
 * values 'A' through 'Z' to uppercase.
 *
 * This function operates on a null-terminated string of bytes--even if it is
 * malformed UTF-8!--and converts ASCII characters 'a' through 'z' to their
 * uppercase equivalents in-place, returning the original `str` pointer.
 *
 * @param str the string to convert in-place. Can not be nullptr.
 * @returns the `str` pointer passed into this function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa strlwr
 */
inline char* strupr(char* str) { return SDL_strupr(str); }

/**
 * Convert a string to lowercase.
 *
 * **WARNING**: Regardless of system locale, this will only convert ASCII
 * values 'A' through 'Z' to lowercase.
 *
 * This function operates on a null-terminated string of bytes--even if it is
 * malformed UTF-8!--and converts ASCII characters 'A' through 'Z' to their
 * lowercase equivalents in-place, returning the original `str` pointer.
 *
 * @param str the string to convert in-place. Can not be nullptr.
 * @returns the `str` pointer passed into this function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa strupr
 */
inline char* strlwr(char* str) { return SDL_strlwr(str); }

/**
 * Search a string for the first instance of a specific byte.
 *
 * The search ends once it finds the requested byte value, or a null
 * terminator byte to end the string.
 *
 * Note that this looks for _bytes_, not _characters_, so you cannot match
 * against a Unicode codepoint > 255, regardless of character encoding.
 *
 * @param str the string to search. Must not be nullptr.
 * @param c the byte value to search for.
 * @returns a pointer to the first instance of `c` in the string, or nullptr if
 *          not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strchr(StringParam str, int c) { return SDL_strchr(str, c); }

/**
 * Search a string for the last instance of a specific byte.
 *
 * The search must go until it finds a null terminator byte to end the string.
 *
 * Note that this looks for _bytes_, not _characters_, so you cannot match
 * against a Unicode codepoint > 255, regardless of character encoding.
 *
 * @param str the string to search. Must not be nullptr.
 * @param c the byte value to search for.
 * @returns a pointer to the last instance of `c` in the string, or nullptr if
 *          not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strrchr(StringParam str, int c) { return SDL_strrchr(str, c); }

/**
 * Search a string for the first instance of a specific substring.
 *
 * The search ends once it finds the requested substring, or a null terminator
 * byte to end the string.
 *
 * Note that this looks for strings of _bytes_, not _characters_, so it's
 * legal to search for malformed and incomplete UTF-8 sequences.
 *
 * @param haystack the string to search. Must not be nullptr.
 * @param needle the string to search for. Must not be nullptr.
 * @returns a pointer to the first instance of `needle` in the string, or
 * nullptr if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strstr(StringParam haystack, StringParam needle)
{
  return SDL_strstr(haystack, needle);
}

/**
 * Search a string, up to n bytes, for the first instance of a specific
 * substring.
 *
 * The search ends once it finds the requested substring, or a null terminator
 * byte to end the string, or `maxlen` bytes have been examined. It is
 * possible to use this function on a string without a null terminator.
 *
 * Note that this looks for strings of _bytes_, not _characters_, so it's
 * legal to search for malformed and incomplete UTF-8 sequences.
 *
 * @param haystack the string to search. Must not be nullptr.
 * @param needle the string to search for. Must not be nullptr.
 * @param maxlen the maximum number of bytes to search in `haystack`.
 * @returns a pointer to the first instance of `needle` in the string, or
 * nullptr if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strnstr(StringParam haystack, StringParam needle, size_t maxlen)
{
  return SDL_strnstr(haystack, needle, maxlen);
}

/**
 * Search a UTF-8 string for the first instance of a specific substring,
 * case-insensitively.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Since this handles Unicode, it expects the strings to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * @param haystack the string to search. Must not be nullptr.
 * @param needle the string to search for. Must not be nullptr.
 * @returns a pointer to the first instance of `needle` in the string, or
 * nullptr if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strcasestr(StringParam haystack, StringParam needle)
{
  return SDL_strcasestr(haystack, needle);
}

/**
 * This works exactly like strtok_r() but doesn't require access to a C
 * runtime.
 *
 * Break a string up into a series of tokens.
 *
 * To start tokenizing a new string, `str` should be the non-nullptr address of
 * the string to start tokenizing. Future calls to get the next token from the
 * same string should specify a nullptr.
 *
 * Note that this function will overwrite pieces of `str` with null chars to
 * split it into tokens. This function cannot be used with const/read-only
 * strings!
 *
 * `saveptr` just needs to point to a `char *` that can be overwritten; SDL
 * will use this to save tokenizing state between calls. It is initialized if
 * `str` is non-nullptr, and used to resume tokenizing when `str` is nullptr.
 *
 * @param str the string to tokenize, or nullptr to continue tokenizing.
 * @param delim the delimiter string that separates tokens.
 * @param saveptr pointer to a char *, used for ongoing state.
 * @returns A pointer to the next token, or nullptr if no tokens remain.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strtok_r(char* str, StringParam delim, char** saveptr)
{
  return SDL_strtok_r(str, delim, saveptr);
}

/**
 * Count the number of codepoints in a UTF-8 string.
 *
 * Counts the _codepoints_, not _bytes_, in `str`, excluding the null
 * terminator.
 *
 * If you need to count the bytes in a string instead, consider using
 * strlen().
 *
 * Since this handles Unicode, it expects the strings to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), so a malformed or incomplete UTF-8 sequence might increase the
 * count by several replacement characters.
 *
 * @param str The null-terminated UTF-8 string to read. Must not be nullptr.
 * @returns The length (in codepoints, excluding the null terminator) of
 *          `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa utf8strnlen
 * @sa strlen
 */
inline size_t utf8strlen(StringParam str) { return SDL_utf8strlen(str); }

/**
 * Count the number of codepoints in a UTF-8 string, up to n bytes.
 *
 * Counts the _codepoints_, not _bytes_, in `str`, excluding the null
 * terminator.
 *
 * If you need to count the bytes in a string instead, consider using
 * strnlen().
 *
 * The counting stops at `bytes` bytes (not codepoints!). This seems
 * counterintuitive, but makes it easy to express the total size of the
 * string's buffer.
 *
 * Since this handles Unicode, it expects the strings to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), so a malformed or incomplete UTF-8 sequence might increase the
 * count by several replacement characters.
 *
 * @param str The null-terminated UTF-8 string to read. Must not be nullptr.
 * @param bytes The maximum amount of bytes to count.
 * @returns The length (in codepoints, excluding the null terminator) of `src`
 *          but never more than `maxlen`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa utf8strlen
 * @sa strnlen
 */
inline size_t utf8strnlen(StringParam str, size_t bytes)
{
  return SDL_utf8strnlen(str, bytes);
}

/**
 * Convert an integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexadecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use snprintf to clamp output, or
 * asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget possible negative
 * signs, null terminator bytes, etc).
 *
 * @param value the integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa uitoa
 * @sa ltoa
 * @sa SDL_lltoa
 */
inline char* itoa(int value, char* str, int radix)
{
  return SDL_itoa(value, str, radix);
}

/**
 * Convert an unsigned integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexadecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use snprintf to clamp output, or
 * asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget null terminator
 * bytes, etc).
 *
 * @param value the unsigned integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa itoa
 * @sa ultoa
 * @sa SDL_ulltoa
 */
inline char* uitoa(unsigned int value, char* str, int radix)
{
  return SDL_uitoa(value, str, radix);
}

/**
 * Convert a long integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexadecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use snprintf to clamp output, or
 * asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget possible negative
 * signs, null terminator bytes, etc).
 *
 * @param value the long integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ultoa
 * @sa itoa
 * @sa SDL_lltoa
 */
inline char* ltoa(long value, char* str, int radix)
{
  return SDL_ltoa(value, str, radix);
}

/**
 * Convert an unsigned long integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexadecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use snprintf to clamp output, or
 * asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget null terminator
 * bytes, etc).
 *
 * @param value the unsigned long integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ltoa
 * @sa uitoa
 * @sa SDL_ulltoa
 */
inline char* ultoa(unsigned long value, char* str, int radix)
{
  return SDL_ultoa(value, str, radix);
}

/**
 * Parse an `int` from a string.
 *
 * The result of calling `atoi(str)` is equivalent to
 * `(int)strtol(str, nullptr, 10)`.
 *
 * @param str The null-terminated string to read. Must not be nullptr.
 * @returns the parsed `int`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa atof
 * @sa strtol
 * @sa strtoul
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa strtod
 * @sa itoa
 */
inline int atoi(StringParam str) { return SDL_atoi(str); }

/**
 * Parse a `double` from a string.
 *
 * The result of calling `atof(str)` is equivalent to `strtod(str,
 * nullptr)`.
 *
 * @param str The null-terminated string to read. Must not be nullptr.
 * @returns the parsed `double`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa atoi
 * @sa strtol
 * @sa strtoul
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa strtod
 */
inline double atof(StringParam str) { return SDL_atof(str); }

/**
 * Parse a `long` from a string.
 *
 * If `str` starts with whitespace, then those whitespace characters are
 * skipped before attempting to parse the number.
 *
 * If the parsed number does not fit inside a `long`, the result is clamped to
 * the minimum and maximum representable `long` values.
 *
 * @param str The null-terminated string to read. Must not be nullptr.
 * @param endp If not nullptr, the address of the first invalid character (i.e.
 *             the next character after the parsed number) will be written to
 *             this pointer.
 * @param base The base of the integer to read. Supported values are 0 and 2
 *             to 36 inclusive. If 0, the base will be inferred from the
 *             number's prefix (0x for hexadecimal, 0 for octal, decimal
 *             otherwise).
 * @returns the parsed `long`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa atoi
 * @sa atof
 * @sa strtoul
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa strtod
 * @sa ltoa
 * @sa wcstol
 */
inline long strtol(StringParam str, char** endp, int base)
{
  return SDL_strtol(str, endp, base);
}

/**
 * Parse an `unsigned long` from a string.
 *
 * If `str` starts with whitespace, then those whitespace characters are
 * skipped before attempting to parse the number.
 *
 * If the parsed number does not fit inside an `unsigned long`, the result is
 * clamped to the maximum representable `unsigned long` value.
 *
 * @param str The null-terminated string to read. Must not be nullptr.
 * @param endp If not nullptr, the address of the first invalid character (i.e.
 *             the next character after the parsed number) will be written to
 *             this pointer.
 * @param base The base of the integer to read. Supported values are 0 and 2
 *             to 36 inclusive. If 0, the base will be inferred from the
 *             number's prefix (0x for hexadecimal, 0 for octal, decimal
 *             otherwise).
 * @returns the parsed `unsigned long`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa atoi
 * @sa atof
 * @sa strtol
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa strtod
 * @sa ultoa
 */
inline unsigned long strtoul(StringParam str, char** endp, int base)
{
  return SDL_strtoul(str, endp, base);
}

/**
 * Parse a `double` from a string.
 *
 * This function makes fewer guarantees than the C runtime `strtod`:
 *
 * - Only decimal notation is guaranteed to be supported. The handling of
 *   scientific and hexadecimal notation is unspecified.
 * - Whether or not INF and NAN can be parsed is unspecified.
 * - The precision of the result is unspecified.
 *
 * @param str the null-terminated string to read. Must not be nullptr.
 * @param endp if not nullptr, the address of the first invalid character (i.e.
 *             the next character after the parsed number) will be written to
 *             this pointer.
 * @returns the parsed `double`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa atoi
 * @sa atof
 * @sa strtol
 * @sa SDL_strtoll
 * @sa strtoul
 * @sa SDL_strtoull
 */
inline double strtod(StringParam str, char** endp)
{
  return SDL_strtod(str, endp);
}

/**
 * Compare two null-terminated UTF-8 strings.
 *
 * Due to the nature of UTF-8 encoding, this will work with Unicode strings,
 * since effectively this function just compares bytes until it hits a
 * null-terminating character. Also due to the nature of UTF-8, this can be
 * used with qsort() to put strings in (roughly) alphabetical order.
 *
 * @param str1 the first string to compare. nullptr is not permitted!
 * @param str2 the second string to compare. nullptr is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int strcmp(StringParam str1, StringParam str2)
{
  return SDL_strcmp(str1, str2);
}

/**
 * Compare two UTF-8 strings up to a number of bytes.
 *
 * Due to the nature of UTF-8 encoding, this will work with Unicode strings,
 * since effectively this function just compares bytes until it hits a
 * null-terminating character. Also due to the nature of UTF-8, this can be
 * used with qsort() to put strings in (roughly) alphabetical order.
 *
 * Note that while this function is intended to be used with UTF-8, it is
 * doing a bytewise comparison, and `maxlen` specifies a _byte_ limit! If the
 * limit lands in the middle of a multi-byte UTF-8 sequence, it will only
 * compare a portion of the final character.
 *
 * `maxlen` specifies a maximum number of bytes to compare; if the strings
 * match to this number of bytes (or both have matched to a null-terminator
 * character before this number of bytes), they will be considered equal.
 *
 * @param str1 the first string to compare. nullptr is not permitted!
 * @param str2 the second string to compare. nullptr is not permitted!
 * @param maxlen the maximum number of _bytes_ to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int strncmp(StringParam str1, StringParam str2, size_t maxlen)
{
  return SDL_strncmp(str1, str2, maxlen);
}

/**
 * Compare two null-terminated UTF-8 strings, case-insensitively.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Since this handles Unicode, it expects the string to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * @param str1 the first string to compare. nullptr is not permitted!
 * @param str2 the second string to compare. nullptr is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int strcasecmp(StringParam str1, StringParam str2)
{
  return SDL_strcasecmp(str1, str2);
}

/**
 * Compare two UTF-8 strings, case-insensitively, up to a number of bytes.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Since this handles Unicode, it expects the string to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * Note that while this function is intended to be used with UTF-8, `maxlen`
 * specifies a _byte_ limit! If the limit lands in the middle of a multi-byte
 * UTF-8 sequence, it may convert a portion of the final character to one or
 * more Unicode character U+FFFD (REPLACEMENT CHARACTER) so as not to overflow
 * a buffer.
 *
 * `maxlen` specifies a maximum number of bytes to compare; if the strings
 * match to this number of bytes (or both have matched to a null-terminator
 * character before this number of bytes), they will be considered equal.
 *
 * @param str1 the first string to compare. nullptr is not permitted!
 * @param str2 the second string to compare. nullptr is not permitted!
 * @param maxlen the maximum number of bytes to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int strncasecmp(StringParam str1, StringParam str2, size_t maxlen)
{
  return SDL_strncasecmp(str1, str2, maxlen);
}

/**
 * Searches a string for the first occurrence of any character contained in a
 * breakset, and returns a pointer from the string to that character.
 *
 * @param str The null-terminated string to be searched. Must not be nullptr,
 * and must not overlap with `breakset`.
 * @param breakset A null-terminated string containing the list of characters
 *                 to look for. Must not be nullptr, and must not overlap with
 *                 `str`.
 * @returns A pointer to the location, in str, of the first occurrence of a
 *          character present in the breakset, or nullptr if none is found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* strpbrk(StringParam str, StringParam breakset)
{
  return SDL_strpbrk(str, breakset);
}

/**
 * The Unicode REPLACEMENT CHARACTER codepoint.
 *
 * StepUTF8() and StepBackUTF8() report this codepoint when they
 * encounter a UTF-8 string with encoding errors.
 *
 * This tends to render as something like a question mark in most places.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa StepBackUTF8
 * @sa StepUTF8
 */
constexpr Uint32 INVALID_UNICODE_CODEPOINT = SDL_INVALID_UNICODE_CODEPOINT;

/**
 * Decode a UTF-8 string, one Unicode codepoint at a time.
 *
 * This will return the first Unicode codepoint in the UTF-8 encoded string in
 * `*pstr`, and then advance `*pstr` past any consumed bytes before returning.
 *
 * It will not access more than `*pslen` bytes from the string. `*pslen` will
 * be adjusted, as well, subtracting the number of bytes consumed.
 *
 * `pslen` is allowed to be nullptr, in which case the string _must_ be
 * nullptr-terminated, as the function will blindly read until it sees the
 * nullptr char.
 *
 * if `*pslen` is zero, it assumes the end of string is reached and returns a
 * zero codepoint regardless of the contents of the string buffer.
 *
 * If the resulting codepoint is zero (a nullptr terminator), or `*pslen` is
 * zero, it will not advance `*pstr` or `*pslen` at all.
 *
 * Generally this function is called in a loop until it returns zero,
 * adjusting its parameters each iteration.
 *
 * If an invalid UTF-8 sequence is encountered, this function returns
 * INVALID_UNICODE_CODEPOINT and advances the string/length by one byte
 * (which is to say, a multibyte sequence might produce several
 * INVALID_UNICODE_CODEPOINT returns before it syncs to the next valid
 * UTF-8 sequence).
 *
 * Several things can generate invalid UTF-8 sequences, including overlong
 * encodings, the use of UTF-16 surrogate values, and truncated data. Please
 * refer to
 * [RFC3629](https://www.ietf.org/rfc/rfc3629.txt)
 * for details.
 *
 * @param pstr a pointer to a UTF-8 string pointer to be read and adjusted.
 * @param pslen a pointer to the number of bytes in the string, to be read and
 *              adjusted. nullptr is allowed.
 * @returns the first Unicode codepoint in the string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint32 StepUTF8(const char** pstr, size_t* pslen)
{
  return SDL_StepUTF8(pstr, pslen);
}

/**
 * Decode a UTF-8 string in reverse, one Unicode codepoint at a time.
 *
 * This will go to the start of the previous Unicode codepoint in the string,
 * move `*pstr` to that location and return that codepoint.
 *
 * If `*pstr` is already at the start of the string), it will not advance
 * `*pstr` at all.
 *
 * Generally this function is called in a loop until it returns zero,
 * adjusting its parameter each iteration.
 *
 * If an invalid UTF-8 sequence is encountered, this function returns
 * INVALID_UNICODE_CODEPOINT.
 *
 * Several things can generate invalid UTF-8 sequences, including overlong
 * encodings, the use of UTF-16 surrogate values, and truncated data. Please
 * refer to
 * [RFC3629](https://www.ietf.org/rfc/rfc3629.txt)
 * for details.
 *
 * @param start a pointer to the beginning of the UTF-8 string.
 * @param pstr a pointer to a UTF-8 string pointer to be read and adjusted.
 * @returns the previous Unicode codepoint in the string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint32 StepBackUTF8(StringParam start, const char** pstr)
{
  return SDL_StepBackUTF8(start, pstr);
}

/**
 * Convert a single Unicode codepoint to UTF-8.
 *
 * The buffer pointed to by `dst` must be at least 4 bytes long, as this
 * function may generate between 1 and 4 bytes of output.
 *
 * This function returns the first byte _after_ the newly-written UTF-8
 * sequence, which is useful for encoding multiple codepoints in a loop, or
 * knowing where to write a nullptr-terminator character to end the string (in
 * either case, plan to have a buffer of _more_ than 4 bytes!).
 *
 * If `codepoint` is an invalid value (outside the Unicode range, or a UTF-16
 * surrogate value, etc), this will use U+FFFD (REPLACEMENT CHARACTER) for the
 * codepoint instead, and not set an error.
 *
 * If `dst` is nullptr, this returns nullptr immediately without writing to the
 * pointer and without setting an error.
 *
 * @param codepoint a Unicode codepoint to convert to UTF-8.
 * @param dst the location to write the encoded UTF-8. Must point to at least
 *            4 bytes!
 * @returns the first byte past the newly-written UTF-8 sequence.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline char* UCS4ToUTF8(Uint32 codepoint, char* dst)
{
  return SDL_UCS4ToUTF8(codepoint, dst);
}

/**
 * This works exactly like sscanf() but doesn't require access to a C runtime.
 *
 * Scan a string, matching a format string, converting each '%' item and
 * storing it to pointers provided through variable arguments.
 *
 * @param text the string to scan. Must not be nullptr.
 * @param fmt a printf-style format string. Must not be nullptr.
 * @param ... a list of pointers to values to be filled in with scanned items.
 * @returns the number of items that matched the format string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int sscanf(StringParam text,
                  SDL_SCANF_FORMAT_STRING const char* fmt,
                  ...)
{
  int rc;
  va_list ap;
  va_start(ap, fmt);
  rc = SDL_vsscanf(text, fmt, ap);
  va_end(ap);
  return rc;
}

/**
 * This works exactly like vsscanf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to sscanf(), except it takes a `va_list` instead
 * of using `...` variable arguments.
 *
 * @param text the string to scan. Must not be nullptr.
 * @param fmt a printf-style format string. Must not be nullptr.
 * @param ap a `va_list` of pointers to values to be filled in with scanned
 *           items.
 * @returns the number of items that matched the format string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int vsscanf(StringParam text,
                   SDL_SCANF_FORMAT_STRING const char* fmt,
                   va_list ap)
{
  return SDL_vsscanf(text, fmt, ap);
}

/**
 * This works exactly like snprintf() but doesn't require access to a C
 * runtime.
 *
 * Format a string of up to `maxlen`-1 bytes, converting each '%' item with
 * values provided through variable arguments.
 *
 * While some C runtimes differ on how to deal with too-large strings, this
 * function null-terminates the output, by treating the null-terminator as
 * part of the `maxlen` count. Note that if `maxlen` is zero, however, no
 * bytes will be written at all.
 *
 * This function returns the number of _bytes_ (not _characters_) that should
 * be written, excluding the null-terminator character. If this returns a
 * number >= `maxlen`, it means the output string was truncated. A negative
 * return value means an error occurred.
 *
 * Referencing the output string's pointer with a format item is undefined
 * behavior.
 *
 * @param text the buffer to write the string into. Must not be nullptr.
 * @param maxlen the maximum bytes to write, including the null-terminator.
 * @param fmt a printf-style format string. Must not be nullptr.
 * @param ... a list of values to be used with the format string.
 * @returns the number of bytes that should be written, not counting the
 *          null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int snprintf(char* text,
                    size_t maxlen,
                    SDL_PRINTF_FORMAT_STRING const char* fmt,
                    ...)
{
  va_list ap;
  int result;

  va_start(ap, fmt);
  result = SDL_vsnprintf(text, maxlen, fmt, ap);
  va_end(ap);

  return result;
}

/**
 * This works exactly like swprintf() but doesn't require access to a C
 * runtime.
 *
 * Format a wide string of up to `maxlen`-1 wchar_t values, converting each
 * '%' item with values provided through variable arguments.
 *
 * While some C runtimes differ on how to deal with too-large strings, this
 * function null-terminates the output, by treating the null-terminator as
 * part of the `maxlen` count. Note that if `maxlen` is zero, however, no wide
 * characters will be written at all.
 *
 * This function returns the number of _wide characters_ (not _codepoints_)
 * that should be written, excluding the null-terminator character. If this
 * returns a number >= `maxlen`, it means the output string was truncated. A
 * negative return value means an error occurred.
 *
 * Referencing the output string's pointer with a format item is undefined
 * behavior.
 *
 * @param text the buffer to write the wide string into. Must not be nullptr.
 * @param maxlen the maximum wchar_t values to write, including the
 *               null-terminator.
 * @param fmt a printf-style format string. Must not be nullptr.
 * @param ... a list of values to be used with the format string.
 * @returns the number of wide characters that should be written, not counting
 *          the null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int swprintf(wchar_t* text,
                    size_t maxlen,
                    SDL_PRINTF_FORMAT_STRING const wchar_t* fmt,
                    ...)
{
  va_list ap;
  int result;

  va_start(ap, fmt);
  result = SDL_vswprintf(text, maxlen, fmt, ap);
  va_end(ap);

  return result;
}

/**
 * This works exactly like vsnprintf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to snprintf(), except it takes a `va_list`
 * instead of using `...` variable arguments.
 *
 * @param text the buffer to write the string into. Must not be nullptr.
 * @param maxlen the maximum bytes to write, including the null-terminator.
 * @param fmt a printf-style format string. Must not be nullptr.
 * @param ap a `va_list` values to be used with the format string.
 * @returns the number of bytes that should be written, not counting the
 *          null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int vsnprintf(char* text,
                     size_t maxlen,
                     SDL_PRINTF_FORMAT_STRING const char* fmt,
                     va_list ap)
{
  return SDL_vsnprintf(text, maxlen, fmt, ap);
}

/**
 * This works exactly like vswprintf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to swprintf(), except it takes a `va_list`
 * instead of using `...` variable arguments.
 *
 * @param text the buffer to write the string into. Must not be nullptr.
 * @param maxlen the maximum wide characters to write, including the
 *               null-terminator.
 * @param fmt a printf-style format wide string. Must not be nullptr.
 * @param ap a `va_list` values to be used with the format string.
 * @returns the number of wide characters that should be written, not counting
 *          the null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int vswprintf(wchar_t* text,
                     size_t maxlen,
                     SDL_PRINTF_FORMAT_STRING const wchar_t* fmt,
                     va_list ap)
{
  return SDL_vswprintf(text, maxlen, fmt, ap);
}

/**
 * This works exactly like asprintf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to snprintf(), except it allocates a buffer large
 * enough to hold the output string on behalf of the caller.
 *
 * On success, this function returns the number of bytes (not characters)
 * comprising the output string, not counting the null-terminator character,
 * and sets `*strp` to the newly-allocated string.
 *
 * On error, this function returns a negative number, and the value of `*strp`
 * is undefined.
 *
 * The returned string is owned by the caller, and should be passed to
 * free when no longer needed.
 *
 * @param strp on output, is set to the new string. Must not be nullptr.
 * @param fmt a printf-style format string. Must not be nullptr.
 * @param ... a list of values to be used with the format string.
 * @returns the number of bytes in the newly-allocated string, not counting
 *          the null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int asprintf(char** strp, SDL_PRINTF_FORMAT_STRING const char* fmt, ...)
{
  va_list ap;
  int result;

  va_start(ap, fmt);
  result = SDL_vasprintf(strp, fmt, ap);
  va_end(ap);

  return result;
}

/**
 * This works exactly like vasprintf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to asprintf(), except it takes a `va_list`
 * instead of using `...` variable arguments.
 *
 * @param strp on output, is set to the new string. Must not be nullptr.
 * @param fmt a printf-style format string. Must not be nullptr.
 * @param ap a `va_list` values to be used with the format string.
 * @returns the number of bytes in the newly-allocated string, not counting
 *          the null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int vasprintf(char** strp,
                     SDL_PRINTF_FORMAT_STRING const char* fmt,
                     va_list ap)
{
  return SDL_vasprintf(strp, fmt, ap);
}

/**
 * Seeds the pseudo-random number generator.
 *
 * Reusing the seed number will cause rand() to repeat the same stream of
 * 'random' numbers.
 *
 * @param seed the value to use as a random number seed, or 0 to use
 *             SDL_GetPerformanceCounter().
 *
 * @threadsafety This should be called on the same thread that calls
 *               rand()
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa rand
 * @sa rand_bits
 * @sa randf
 */
inline void srand(Uint64 seed) { SDL_srand(seed); }

/**
 * Generate a pseudo-random number less than n for positive n
 *
 * The method used is faster and of better quality than `rand() % n`. Odds are
 * roughly 99.9% even for n = 1 million. Evenness is better for smaller n, and
 * much worse as n gets bigger.
 *
 * Example: to simulate a d6 use `rand(6) + 1` The +1 converts 0..5 to
 * 1..6
 *
 * If you want to generate a pseudo-random number in the full range of Sint32,
 * you should use: (Sint32)rand_bits()
 *
 * If you want reproducible output, be sure to initialize with srand()
 * first.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @param n the number of possible outcomes. n must be positive.
 * @returns a random value in the range of [0 .. n-1].
 *
 * @threadsafety All calls should be made from a single thread
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa srand
 * @sa randf
 */
inline Sint32 rand(Sint32 n) { return SDL_rand(n); }

/**
 * Generate a uniform pseudo-random floating point number less than 1.0
 *
 * If you want reproducible output, be sure to initialize with srand()
 * first.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @returns a random value in the range of [0.0, 1.0).
 *
 * @threadsafety All calls should be made from a single thread
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa srand
 * @sa rand
 */
inline float randf() { return SDL_randf(); }

/**
 * Generate 32 pseudo-random bits.
 *
 * You likely want to use rand() to get a pseudo-random number instead.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @returns a random value in the range of [0-MAX_UINT32].
 *
 * @threadsafety All calls should be made from a single thread
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa rand
 * @sa randf
 * @sa srand
 */
inline Uint32 rand_bits() { return SDL_rand_bits(); }

/**
 * A independent pseudo random state
 *
 * This can be instantiated in any thread and as long as it is not shared with
 * another thread all members are safe to call.
 *
 * @cat wrap-state
 *
 * @sa wrap-state
 */
class Random
{
  Uint64 m_state;

public:
  constexpr Random()
    : m_state(0)
  {
  }

  /**
   * Init state with the given value
   */
  constexpr explicit Random(Uint64 state)
    : m_state(state)
  {
  }

  /// Convert to the underlying type
  constexpr operator Uint64() { return m_state; }

  /**
   * Generate a pseudo-random number less than n for positive n
   *
   * The method used is faster and of better quality than `rand() % n`. Odds are
   * roughly 99.9% even for n = 1 million. Evenness is better for smaller n, and
   * much worse as n gets bigger.
   *
   * Example: to simulate a d6 use `state.rand(6) + 1` The +1 converts
   * 0..5 to 1..6
   *
   * If you want to generate a pseudo-random number in the full range of Sint32,
   * you should use: (Sint32)state.rand_bits()
   *
   * There are no guarantees as to the quality of the random sequence produced,
   * and this should not be used for security (cryptography, passwords) or where
   * money is on the line (loot-boxes, casinos). There are many random number
   * libraries available with different characteristics and you should pick one
   * of those to meet any serious needs.
   *
   * @param n the number of possible outcomes. n must be positive.
   * @returns a random value in the range of [0 .. n-1].
   *
   * @threadsafety This function is thread-safe, as long as this object
   *               isn't shared between threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa rand
   * @sa Random.rand_bits
   * @sa Random.randf
   */
  Sint32 rand(Sint32 n) { return SDL_rand_r(&m_state, n); }

  /**
   * Generate a uniform pseudo-random floating point number less than 1.0
   *
   * If you want reproducible output, be sure to initialize with srand()
   * first.
   *
   * There are no guarantees as to the quality of the random sequence produced,
   * and this should not be used for security (cryptography, passwords) or where
   * money is on the line (loot-boxes, casinos). There are many random number
   * libraries available with different characteristics and you should pick one
   * of those to meet any serious needs.
   *
   * @returns a random value in the range of [0.0, 1.0).
   *
   * @threadsafety This function is thread-safe, as long as this object
   *               isn't shared between threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Random.rand_bits
   * @sa Random.rand
   * @sa randf
   */
  float randf() { return SDL_randf_r(&m_state); }

  /**
   * Generate 32 pseudo-random bits.
   *
   * You likely want to use Random.rand() to get a pseudo-random number instead.
   *
   * There are no guarantees as to the quality of the random sequence produced,
   * and this should not be used for security (cryptography, passwords) or where
   * money is on the line (loot-boxes, casinos). There are many random number
   * libraries available with different characteristics and you should pick one
   * of those to meet any serious needs.
   *
   * @returns a random value in the range of [0-MAX_UINT32].
   *
   * @threadsafety This function is thread-safe, as long as this object
   *               isn't shared between threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Random.rand
   * @sa Random.randf
   */
  Uint32 rand_bits() { return SDL_rand_bits_r(&m_state); }
};

/**
 * Generate a pseudo-random number less than n for positive n
 *
 * The method used is faster and of better quality than `rand() % n`. Odds are
 * roughly 99.9% even for n = 1 million. Evenness is better for smaller n, and
 * much worse as n gets bigger.
 *
 * Example: to simulate a d6 use `Random.rand(state, 6) + 1` The +1 converts
 * 0..5 to 1..6
 *
 * If you want to generate a pseudo-random number in the full range of Sint32,
 * you should use: (Sint32)Random.rand_bits(state)
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @param state a pointer to the current random number state, this may not be
 *              nullptr.
 * @param n the number of possible outcomes. n must be positive.
 * @returns a random value in the range of [0 .. n-1].
 *
 * @threadsafety This function is thread-safe, as long as the state pointer
 *               isn't shared between threads.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa rand
 * @sa Random.rand_bits
 * @sa Random.randf
 */
inline Sint32 rand_r(Uint64* state, Sint32 n) { return SDL_rand_r(state, n); }

/**
 * Generate a uniform pseudo-random floating point number less than 1.0
 *
 * If you want reproducible output, be sure to initialize with srand()
 * first.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @param state a pointer to the current random number state, this may not be
 *              nullptr.
 * @returns a random value in the range of [0.0, 1.0).
 *
 * @threadsafety This function is thread-safe, as long as the state pointer
 *               isn't shared between threads.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Random.rand_bits
 * @sa Random.rand
 * @sa randf
 */
inline float randf_r(Uint64* state) { return SDL_randf_r(state); }

/**
 * Generate 32 pseudo-random bits.
 *
 * You likely want to use Random.rand() to get a psuedo-random number instead.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @param state a pointer to the current random number state, this may not be
 *              nullptr.
 * @returns a random value in the range of [0-MAX_UINT32].
 *
 * @threadsafety This function is thread-safe, as long as the state pointer
 *               isn't shared between threads.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Random.rand
 * @sa Random.randf
 */
inline Uint32 rand_bits_r(Uint64* state) { return SDL_rand_bits_r(state); }

/**
 * The value of Pi, as a double-precision floating point literal.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa PI_F
 */
constexpr double PI_D = SDL_PI_D;

/**
 * The value of Pi, as a single-precision floating point literal.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa PI_D
 */
constexpr float PI_F = SDL_PI_F;

/**
 * Compute the arc cosine of `x`.
 *
 * The definition of `y = acos(x)` is `x = cos(y)`.
 *
 * Domain: `-1 <= x <= 1`
 *
 * Range: `0 <= y <= Pi`
 *
 * This function operates on double-precision floating point values, use
 * acos for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc cosine of `x`, in radians.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa acos
 * @sa asin
 * @sa cos
 */
inline double acos(double x) { return SDL_acos(x); }

/**
 * Compute the arc cosine of `x`.
 *
 * The definition of `y = acos(x)` is `x = cos(y)`.
 *
 * Domain: `-1 <= x <= 1`
 *
 * Range: `0 <= y <= Pi`
 *
 * This function operates on single-precision floating point values, use
 * acos for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc cosine of `x`, in radians.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa acos
 * @sa asin
 * @sa cos
 */
inline float acos(float x) { return SDL_acosf(x); }

/**
 * Compute the arc sine of `x`.
 *
 * The definition of `y = asin(x)` is `x = sin(y)`.
 *
 * Domain: `-1 <= x <= 1`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on double-precision floating point values, use
 * asin for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc sine of `x`, in radians.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa asin
 * @sa acos
 * @sa sin
 */
inline double asin(double x) { return SDL_asin(x); }

/**
 * Compute the arc sine of `x`.
 *
 * The definition of `y = asin(x)` is `x = sin(y)`.
 *
 * Domain: `-1 <= x <= 1`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on single-precision floating point values, use
 * asin for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc sine of `x`, in radians.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa asin
 * @sa acos
 * @sa sin
 */
inline float asin(float x) { return SDL_asinf(x); }

/**
 * Compute the arc tangent of `x`.
 *
 * The definition of `y = atan(x)` is `x = tan(y)`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on double-precision floating point values, use
 * atan for single-precision floats.
 *
 * To calculate the arc tangent of y / x, use atan2.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc tangent of of `x` in radians, or 0 if `x = 0`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa atan
 * @sa atan2
 * @sa tan
 */
inline double atan(double x) { return SDL_atan(x); }

/**
 * Compute the arc tangent of `x`.
 *
 * The definition of `y = atan(x)` is `x = tan(y)`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on single-precision floating point values, use
 * atan for dboule-precision floats.
 *
 * To calculate the arc tangent of y / x, use atan2.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc tangent of of `x` in radians, or 0 if `x = 0`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa atan
 * @sa atan2
 * @sa tan
 */
inline float atan(float x) { return SDL_atanf(x); }

/**
 * Compute the arc tangent of `y / x`, using the signs of x and y to adjust
 * the result's quadrant.
 *
 * The definition of `z = atan2(x, y)` is `y = x tan(z)`, where the quadrant
 * of z is determined based on the signs of x and y.
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-Pi <= y <= Pi`
 *
 * This function operates on double-precision floating point values, use
 * atan2 for single-precision floats.
 *
 * To calculate the arc tangent of a single value, use atan.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param y floating point value of the numerator (y coordinate).
 * @param x floating point value of the denominator (x coordinate).
 * @returns arc tangent of of `y / x` in radians, or, if `x = 0`, either
 *          `-Pi/2`, `0`, or `Pi/2`, depending on the value of `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa atan
 * @sa tan
 */
inline double atan2(double y, double x) { return SDL_atan2(y, x); }

/**
 * Compute the arc tangent of `y / x`, using the signs of x and y to adjust
 * the result's quadrant.
 *
 * The definition of `z = atan2(x, y)` is `y = x tan(z)`, where the quadrant
 * of z is determined based on the signs of x and y.
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-Pi <= y <= Pi`
 *
 * This function operates on single-precision floating point values, use
 * atan2 for double-precision floats.
 *
 * To calculate the arc tangent of a single value, use atan.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param y floating point value of the numerator (y coordinate).
 * @param x floating point value of the denominator (x coordinate).
 * @returns arc tangent of of `y / x` in radians, or, if `x = 0`, either
 *          `-Pi/2`, `0`, or `Pi/2`, depending on the value of `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa atan2
 * @sa atan
 * @sa tan
 */
inline float atan2(float y, float x) { return SDL_atan2f(y, x); }

/**
 * Compute the ceiling of `x`.
 *
 * The ceiling of `x` is the smallest integer `y` such that `y > x`, i.e `x`
 * rounded up to the nearest integer.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on double-precision floating point values, use
 * ceil for single-precision floats.
 *
 * @param x floating point value.
 * @returns the ceiling of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ceil
 * @sa floor
 * @sa trunc
 * @sa round
 * @sa lround
 */
inline double ceil(double x) { return SDL_ceil(x); }

/**
 * Compute the ceiling of `x`.
 *
 * The ceiling of `x` is the smallest integer `y` such that `y > x`, i.e `x`
 * rounded up to the nearest integer.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on single-precision floating point values, use
 * ceil for double-precision floats.
 *
 * @param x floating point value.
 * @returns the ceiling of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ceil
 * @sa floor
 * @sa trunc
 * @sa round
 * @sa lround
 */
inline float ceil(float x) { return SDL_ceilf(x); }

/**
 * Copy the sign of one floating-point value to another.
 *
 * The definition of copysign is that ``copysign(x, y) = abs(x) * sign(y)``.
 *
 * Domain: `-INF <= x <= INF`, ``-INF <= y <= f``
 *
 * Range: `-INF <= z <= INF`
 *
 * This function operates on double-precision floating point values, use
 * copysign for single-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @param y floating point value to use as the sign.
 * @returns the floating point value with the sign of y and the magnitude of
 *          x.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa copysign
 * @sa abs
 */
inline double copysign(double x, double y) { return SDL_copysign(x, y); }

/**
 * Copy the sign of one floating-point value to another.
 *
 * The definition of copysign is that ``copysign(x, y) = abs(x) * sign(y)``.
 *
 * Domain: `-INF <= x <= INF`, ``-INF <= y <= f``
 *
 * Range: `-INF <= z <= INF`
 *
 * This function operates on single-precision floating point values, use
 * copysign for double-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @param y floating point value to use as the sign.
 * @returns the floating point value with the sign of y and the magnitude of
 *          x.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa copysign
 * @sa abs
 */
inline float copysign(float x, float y) { return SDL_copysignf(x, y); }

/**
 * Compute the cosine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on double-precision floating point values, use
 * cos for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns cosine of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa cos
 * @sa acos
 * @sa sin
 */
inline double cos(double x) { return SDL_cos(x); }

/**
 * Compute the cosine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on single-precision floating point values, use
 * cos for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns cosine of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa cos
 * @sa acos
 * @sa sin
 */
inline float cos(float x) { return SDL_cosf(x); }

/**
 * Compute the exponential of `x`.
 *
 * The definition of `y = exp(x)` is `y = e^x`, where `e` is the base of the
 * natural logarithm. The inverse is the natural logarithm, log.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * The output will overflow if `exp(x)` is too large to be represented.
 *
 * This function operates on double-precision floating point values, use
 * exp for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns value of `e^x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa exp
 * @sa log
 */
inline double exp(double x) { return SDL_exp(x); }

/**
 * Compute the exponential of `x`.
 *
 * The definition of `y = exp(x)` is `y = e^x`, where `e` is the base of the
 * natural logarithm. The inverse is the natural logarithm, log.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * The output will overflow if `exp(x)` is too large to be represented.
 *
 * This function operates on single-precision floating point values, use
 * exp for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns value of `e^x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa exp
 * @sa log
 */
inline float exp(float x) { return SDL_expf(x); }

/**
 * Compute the floor of `x`.
 *
 * The floor of `x` is the largest integer `y` such that `y > x`, i.e `x`
 * rounded down to the nearest integer.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on double-precision floating point values, use
 * floor for single-precision floats.
 *
 * @param x floating point value.
 * @returns the floor of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa floor
 * @sa ceil
 * @sa trunc
 * @sa round
 * @sa lround
 */
inline double floor(double x) { return SDL_floor(x); }

/**
 * Compute the floor of `x`.
 *
 * The floor of `x` is the largest integer `y` such that `y > x`, i.e `x`
 * rounded down to the nearest integer.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on single-precision floating point values, use
 * floor for double-precision floats.
 *
 * @param x floating point value.
 * @returns the floor of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa floor
 * @sa ceil
 * @sa trunc
 * @sa round
 * @sa lround
 */
inline float floor(float x) { return SDL_floorf(x); }

/**
 * Truncate `x` to an integer.
 *
 * Rounds `x` to the next closest integer to 0. This is equivalent to removing
 * the fractional part of `x`, leaving only the integer part.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on double-precision floating point values, use
 * trunc for single-precision floats.
 *
 * @param x floating point value.
 * @returns `x` truncated to an integer.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa trunc
 * @sa fmod
 * @sa ceil
 * @sa floor
 * @sa round
 * @sa lround
 */
inline double trunc(double x) { return SDL_trunc(x); }

/**
 * Truncate `x` to an integer.
 *
 * Rounds `x` to the next closest integer to 0. This is equivalent to removing
 * the fractional part of `x`, leaving only the integer part.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on single-precision floating point values, use
 * trunc for double-precision floats.
 *
 * @param x floating point value.
 * @returns `x` truncated to an integer.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa trunc
 * @sa fmod
 * @sa ceil
 * @sa floor
 * @sa round
 * @sa lround
 */
inline float trunc(float x) { return SDL_truncf(x); }

/**
 * Return the floating-point remainder of `x / y`
 *
 * Divides `x` by `y`, and returns the remainder.
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`, `y != 0`
 *
 * Range: `-y <= z <= y`
 *
 * This function operates on double-precision floating point values, use
 * fmod for single-precision floats.
 *
 * @param x the numerator.
 * @param y the denominator. Must not be 0.
 * @returns the remainder of `x / y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa fmod
 * @sa modf
 * @sa trunc
 * @sa ceil
 * @sa floor
 * @sa round
 * @sa lround
 */
inline double fmod(double x, double y) { return SDL_fmod(x, y); }

/**
 * Return the floating-point remainder of `x / y`
 *
 * Divides `x` by `y`, and returns the remainder.
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`, `y != 0`
 *
 * Range: `-y <= z <= y`
 *
 * This function operates on single-precision floating point values, use
 * fmod for double-precision floats.
 *
 * @param x the numerator.
 * @param y the denominator. Must not be 0.
 * @returns the remainder of `x / y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa fmod
 * @sa trunc
 * @sa modf
 * @sa ceil
 * @sa floor
 * @sa round
 * @sa lround
 */
inline float fmod(float x, float y) { return SDL_fmodf(x, y); }

/**
 * Return whether the value is infinity.
 *
 * @param x double-precision floating point value.
 * @returns non-zero if the value is infinity, 0 otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa isinf
 */
inline int isinf(double x) { return SDL_isinf(x); }

/**
 * Return whether the value is infinity.
 *
 * @param x floating point value.
 * @returns non-zero if the value is infinity, 0 otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa isinf
 */
inline int isinf(float x) { return SDL_isinff(x); }

/**
 * Return whether the value is NaN.
 *
 * @param x double-precision floating point value.
 * @returns non-zero if the value is NaN, 0 otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa isnan
 */
inline int isnan(double x) { return SDL_isnan(x); }

/**
 * Return whether the value is NaN.
 *
 * @param x floating point value.
 * @returns non-zero if the value is NaN, 0 otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa isnan
 */
inline int isnan(float x) { return SDL_isnanf(x); }

/**
 * Compute the natural logarithm of `x`.
 *
 * Domain: `0 < x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * It is an error for `x` to be less than or equal to 0.
 *
 * This function operates on double-precision floating point values, use
 * log for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than 0.
 * @returns the natural logarithm of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa log
 * @sa log10
 * @sa exp
 */
inline double log(double x) { return SDL_log(x); }

/**
 * Compute the natural logarithm of `x`.
 *
 * Domain: `0 < x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * It is an error for `x` to be less than or equal to 0.
 *
 * This function operates on single-precision floating point values, use
 * log for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than 0.
 * @returns the natural logarithm of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa log
 * @sa exp
 */
inline float log(float x) { return SDL_logf(x); }

/**
 * Compute the base-10 logarithm of `x`.
 *
 * Domain: `0 < x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * It is an error for `x` to be less than or equal to 0.
 *
 * This function operates on double-precision floating point values, use
 * log10 for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than 0.
 * @returns the logarithm of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa log10
 * @sa log
 * @sa pow
 */
inline double log10(double x) { return SDL_log10(x); }

/**
 * Compute the base-10 logarithm of `x`.
 *
 * Domain: `0 < x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * It is an error for `x` to be less than or equal to 0.
 *
 * This function operates on single-precision floating point values, use
 * log10 for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than 0.
 * @returns the logarithm of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa log10
 * @sa log
 * @sa pow
 */
inline float log10(float x) { return SDL_log10f(x); }

/**
 * Split `x` into integer and fractional parts
 *
 * This function operates on double-precision floating point values, use
 * modf for single-precision floats.
 *
 * @param x floating point value.
 * @param y output pointer to store the integer part of `x`.
 * @returns the fractional part of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa modf
 * @sa trunc
 * @sa fmod
 */
inline double modf(double x, double* y) { return SDL_modf(x, y); }

/**
 * Split `x` into integer and fractional parts
 *
 * This function operates on single-precision floating point values, use
 * modf for double-precision floats.
 *
 * @param x floating point value.
 * @param y output pointer to store the integer part of `x`.
 * @returns the fractional part of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa modf
 * @sa trunc
 * @sa fmod
 */
inline float modf(float x, float* y) { return SDL_modff(x, y); }

/**
 * Raise `x` to the power `y`
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-INF <= z <= INF`
 *
 * If `y` is the base of the natural logarithm (e), consider using exp
 * instead.
 *
 * This function operates on double-precision floating point values, use
 * pow for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x the base.
 * @param y the exponent.
 * @returns `x` raised to the power `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa pow
 * @sa exp
 * @sa log
 */
inline double pow(double x, double y) { return SDL_pow(x, y); }

/**
 * Raise `x` to the power `y`
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-INF <= z <= INF`
 *
 * If `y` is the base of the natural logarithm (e), consider using exp
 * instead.
 *
 * This function operates on single-precision floating point values, use
 * pow for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x the base.
 * @param y the exponent.
 * @returns `x` raised to the power `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa pow
 * @sa exp
 * @sa log
 */
inline float pow(float x, float y) { return SDL_powf(x, y); }

/**
 * Round `x` to the nearest integer.
 *
 * Rounds `x` to the nearest integer. Values halfway between integers will be
 * rounded away from zero.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on double-precision floating point values, use
 * round for single-precision floats. To get the result as an integer
 * type, use lround.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa round
 * @sa lround
 * @sa floor
 * @sa ceil
 * @sa trunc
 */
inline double round(double x) { return SDL_round(x); }

/**
 * Round `x` to the nearest integer.
 *
 * Rounds `x` to the nearest integer. Values halfway between integers will be
 * rounded away from zero.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on single-precision floating point values, use
 * round for double-precision floats. To get the result as an integer
 * type, use lround.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa round
 * @sa lround
 * @sa floor
 * @sa ceil
 * @sa trunc
 */
inline float round(float x) { return SDL_roundf(x); }

/**
 * Round `x` to the nearest integer representable as a long
 *
 * Rounds `x` to the nearest integer. Values halfway between integers will be
 * rounded away from zero.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `MIN_LONG <= y <= MAX_LONG`
 *
 * This function operates on double-precision floating point values, use
 * lround for single-precision floats. To get the result as a
 * floating-point type, use round.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa lround
 * @sa round
 * @sa floor
 * @sa ceil
 * @sa trunc
 */
inline long lround(double x) { return SDL_lround(x); }

/**
 * Round `x` to the nearest integer representable as a long
 *
 * Rounds `x` to the nearest integer. Values halfway between integers will be
 * rounded away from zero.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `MIN_LONG <= y <= MAX_LONG`
 *
 * This function operates on single-precision floating point values, use
 * lround for double-precision floats. To get the result as a
 * floating-point type, use round.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa lround
 * @sa round
 * @sa floor
 * @sa ceil
 * @sa trunc
 */
inline long lround(float x) { return SDL_lroundf(x); }

/**
 * Scale `x` by an integer power of two.
 *
 * Multiplies `x` by the `n`th power of the floating point radix (always 2).
 *
 * Domain: `-INF <= x <= INF`, `n` integer
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * scalbn for single-precision floats.
 *
 * @param x floating point value to be scaled.
 * @param n integer exponent.
 * @returns `x * 2^n`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa scalbn
 * @sa pow
 */
inline double scalbn(double x, int n) { return SDL_scalbn(x, n); }

/**
 * Scale `x` by an integer power of two.
 *
 * Multiplies `x` by the `n`th power of the floating point radix (always 2).
 *
 * Domain: `-INF <= x <= INF`, `n` integer
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * scalbn for double-precision floats.
 *
 * @param x floating point value to be scaled.
 * @param n integer exponent.
 * @returns `x * 2^n`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa scalbn
 * @sa pow
 */
inline float scalbn(float x, int n) { return SDL_scalbnf(x, n); }

/**
 * Compute the sine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on double-precision floating point values, use
 * sin for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns sine of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa sin
 * @sa asin
 * @sa cos
 */
inline double sin(double x) { return SDL_sin(x); }

/**
 * Compute the sine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on single-precision floating point values, use
 * sin for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns sine of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa sin
 * @sa asin
 * @sa cos
 */
inline float sin(float x) { return SDL_sinf(x); }

/**
 * Compute the square root of `x`.
 *
 * Domain: `0 <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * sqrt for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than or equal to 0.
 * @returns square root of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa sqrt
 */
inline double sqrt(double x) { return SDL_sqrt(x); }

/**
 * Compute the square root of `x`.
 *
 * Domain: `0 <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * sqrt for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than or equal to 0.
 * @returns square root of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa sqrt
 */
inline float sqrt(float x) { return SDL_sqrtf(x); }

/**
 * Compute the tangent of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * tan for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns tangent of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa tan
 * @sa sin
 * @sa cos
 * @sa atan
 * @sa atan2
 */
inline double tan(double x) { return SDL_tan(x); }

/**
 * Compute the tangent of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * tan for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns tangent of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa tan
 * @sa sin
 * @sa cos
 * @sa atan
 * @sa atan2
 */
inline float tan(float x) { return SDL_tanf(x); }

/**
 * An opaque handle representing string encoding conversion state.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa IConv.open
 * @sa IConv
 */
class IConv
{
  IConvRaw m_resource = nullptr;

public:
  constexpr IConv() = default;

  constexpr explicit IConv(const IConvRaw resource)
    : m_resource(resource)
  {
  }

  IConv(const IConv& other) = delete;

  constexpr IConv(IConv&& other) { other.m_resource = nullptr; }

  /**
   * This function converts text between encodings, reading from and writing to
   * a buffer.
   *
   * It returns the number of successful conversions on success. On error,
   * SDL_ICONV_E2BIG is returned when the output buffer is too small, or
   * SDL_ICONV_EILSEQ is returned when an invalid input sequence is encountered,
   * or SDL_ICONV_EINVAL is returned when an incomplete input sequence is
   * encountered.
   *
   * On exit:
   *
   * - inbuf will point to the beginning of the next multibyte sequence. On
   *   error, this is the location of the problematic input sequence. On
   *   success, this is the end of the input sequence.
   * - inbytesleft will be set to the number of bytes left to convert, which
   *   will be 0 on success.
   * - outbuf will point to the location where to store the next output byte.
   * - outbytesleft will be set to the number of bytes left in the output
   *   buffer.
   *
   * @param inbuf Address of variable that points to the first character of the
   *              input sequence.
   * @param inbytesleft The number of bytes in the input buffer.
   * @param outbuf Address of variable that points to the output buffer.
   * @param outbytesleft The number of bytes in the output buffer.
   * @returns the number of conversions on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IConv.open
   * @sa IConv.close
   * @sa iconv_string
   */
  size_t iconv(const char** inbuf,
               size_t* inbytesleft,
               char** outbuf,
               size_t* outbytesleft)
  {
    return CheckError(
      SDL_iconv(m_resource, inbuf, inbytesleft, outbuf, outbytesleft));
  }
};

/**
 * This function allocates a context for the specified character set
 * conversion.
 *
 * @param tocode The target character encoding, must not be nullptr.
 * @param fromcode The source character encoding, must not be nullptr.
 * @returns a handle that must be freed with IConv.close, or
 *          SDL_ICONV_ERROR on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IConv.iconv
 * @sa IConv.close
 * @sa iconv_string
 */
inline IConv iconv_open(StringParam tocode, StringParam fromcode)
{
  return IConv(SDL_iconv_open(tocode, fromcode));
}

/**
 * This function frees a context used for character set conversion.
 *
 * @param cd The character set conversion handle.
 * @returns 0 on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IConv.iconv
 * @sa IConv.open
 * @sa iconv_string
 */
inline int iconv_close(IConvRaw cd) { return CheckError(SDL_iconv_close(cd)); }

/**
 * This function converts text between encodings, reading from and writing to
 * a buffer.
 *
 * It returns the number of successful conversions on success. On error,
 * SDL_ICONV_E2BIG is returned when the output buffer is too small, or
 * SDL_ICONV_EILSEQ is returned when an invalid input sequence is encountered,
 * or SDL_ICONV_EINVAL is returned when an incomplete input sequence is
 * encountered.
 *
 * On exit:
 *
 * - inbuf will point to the beginning of the next multibyte sequence. On
 *   error, this is the location of the problematic input sequence. On
 *   success, this is the end of the input sequence.
 * - inbytesleft will be set to the number of bytes left to convert, which
 *   will be 0 on success.
 * - outbuf will point to the location where to store the next output byte.
 * - outbytesleft will be set to the number of bytes left in the output
 *   buffer.
 *
 * @param cd The character set conversion context, created in
 *           IConv.open().
 * @param inbuf Address of variable that points to the first character of the
 *              input sequence.
 * @param inbytesleft The number of bytes in the input buffer.
 * @param outbuf Address of variable that points to the output buffer.
 * @param outbytesleft The number of bytes in the output buffer.
 * @returns the number of conversions on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IConv.open
 * @sa IConv.close
 * @sa iconv_string
 */
inline size_t iconv(IConvRaw cd,
                    const char** inbuf,
                    size_t* inbytesleft,
                    char** outbuf,
                    size_t* outbytesleft)
{
  return CheckError(SDL_iconv(cd, inbuf, inbytesleft, outbuf, outbytesleft));
}

#ifdef SDL3PP_DOC

/**
 * Generic error. Check GetError()?
 */
#define SDL_ICONV_ERROR (size_t)-1

/**
 * Output buffer was too small.
 */
#define SDL_ICONV_E2BIG (size_t)-2

/**
 * Invalid input sequence was encountered.
 */
#define SDL_ICONV_EILSEQ (size_t)-3

/**
 * Incomplete input sequence was encountered.
 */
#define SDL_ICONV_EINVAL (size_t)-4

#endif // SDL3PP_DOC

/**
 * Helper function to convert a string's encoding in one call.
 *
 * This function converts a buffer or string between encodings in one pass.
 *
 * The string does not need to be nullptr-terminated; this function operates on
 * the number of bytes specified in `inbytesleft` whether there is a nullptr
 * character anywhere in the buffer.
 *
 * @param tocode the character encoding of the output string. Examples are
 *               "UTF-8", "UCS-4", etc.
 * @param fromcode the character encoding of data in `inbuf`.
 * @param inbuf the string to convert to a different encoding.
 * @param inbytesleft the size of the input string _in bytes_.
 * @returns a new string, converted to the new encoding, or nullptr on error.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IConv.open
 * @sa IConv.close
 * @sa IConv.iconv
 */
inline OwnPtr<char> iconv_string(StringParam tocode,
                                 StringParam fromcode,
                                 StringParam inbuf,
                                 size_t inbytesleft)
{
  return OwnPtr<char>{SDL_iconv_string(tocode, fromcode, inbuf, inbytesleft)};
}

#ifdef SDL3PP_DOC

/**
 * Convert a UTF-8 string to the current locale's character encoding.
 *
 * This is a helper macro that might be more clear than calling
 * iconv_string directly. However, it double-evaluates its parameter, so
 * do not use an expression with side-effects here.
 *
 * @param S the string to convert.
 * @returns a new string, converted to the new encoding, or nullptr on error.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_iconv_utf8_locale(S)                                               \
  SDL_iconv_string("", "UTF-8", S, SDL_strlen(S) + 1)

/**
 * Convert a UTF-8 string to UCS-2.
 *
 * This is a helper macro that might be more clear than calling
 * iconv_string directly. However, it double-evaluates its parameter, so
 * do not use an expression with side-effects here.
 *
 * @param S the string to convert.
 * @returns a new string, converted to the new encoding, or nullptr on error.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_iconv_utf8_ucs2(S)                                                 \
  (Uint16*)SDL_iconv_string("UCS-2", "UTF-8", S, SDL_strlen(S) + 1)

/**
 * Convert a UTF-8 string to UCS-4.
 *
 * This is a helper macro that might be more clear than calling
 * iconv_string directly. However, it double-evaluates its parameter, so
 * do not use an expression with side-effects here.
 *
 * @param S the string to convert.
 * @returns a new string, converted to the new encoding, or nullptr on error.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_iconv_utf8_ucs4(S)                                                 \
  (Uint32*)SDL_iconv_string("UCS-4", "UTF-8", S, SDL_strlen(S) + 1)

/**
 * Convert a wchar_t string to UTF-8.
 *
 * This is a helper macro that might be more clear than calling
 * iconv_string directly. However, it double-evaluates its parameter, so
 * do not use an expression with side-effects here.
 *
 * @param S the string to convert.
 * @returns a new string, converted to the new encoding, or nullptr on error.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_iconv_wchar_utf8(S)                                                \
  SDL_iconv_string(                                                            \
    "UTF-8", "WCHAR_T", (char*)S, (SDL_wcslen(S) + 1) * sizeof(wchar_t))

#endif // SDL3PP_DOC

/**
 * Multiply two integers, checking for overflow.
 *
 * If `a * b` would overflow, return false.
 *
 * Otherwise store `a * b` via ret and return true.
 *
 * @param a the multiplicand.
 * @param b the multiplier.
 * @param ret on non-overflow output, stores the multiplication result, may
 *            not be nullptr.
 * @returns false on overflow, true if result is multiplied without overflow.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool size_mul_check_overflow(size_t a, size_t b, size_t* ret)
{
  return SDL_size_mul_check_overflow(a, b, ret);
}

/**
 * Add two integers, checking for overflow.
 *
 * If `a + b` would overflow, return false.
 *
 * Otherwise store `a + b` via ret and return true.
 *
 * @param a the first addend.
 * @param b the second addend.
 * @param ret on non-overflow output, stores the addition result, may not be
 *            nullptr.
 * @returns false on overflow, true if result is added without overflow.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool size_add_check_overflow(size_t a, size_t b, size_t* ret)
{
  return SDL_size_add_check_overflow(a, b, ret);
}

/**
 * A generic function pointer.
 *
 * In theory, generic function pointers should use this, instead of `void *`,
 * since some platforms could treat code addresses differently than data
 * addresses. Although in current times no popular platforms make this
 * distinction, it is more correct and portable to use the correct type for a
 * generic pointer.
 *
 * If for some reason you need to force this typedef to be an actual `void *`,
 * perhaps to work around a compiler or existing code, you can define
 * `SDL_FUNCTION_POINTER_IS_VOID_POINTER` before including any SDL headers.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using FunctionPointer = SDL_FunctionPointer;

/// @}

inline void PtrDeleter::operator()(void* ptr) const { SDL_free(ptr); }

/**
 * @defgroup CategoryInit Initialization and Shutdown
 *
 * All SDL programs need to initialize the library before starting to work
 * with it.
 *
 * Almost everything can simply call Init() near startup, with a handful
 * of flags to specify subsystems to touch. These are here to make sure SDL
 * does not even attempt to touch low-level pieces of the operating system
 * that you don't intend to use. For example, you might be using SDL for video
 * and input but chose an external library for audio, and in this case you
 * would just need to leave off the `INIT_AUDIO` flag to make sure that
 * external library has complete control.
 *
 * Most apps, when terminating, should call Quit(). This will clean up
 * (nearly) everything that SDL might have allocated, and crucially, it'll
 * make sure that the display's resolution is back to what the user expects if
 * you had previously changed it for your game.
 *
 * SDL3 apps are strongly encouraged to call SetAppMetadata() at startup
 * to fill in details about the program. This is completely optional, but it
 * helps in small ways (we can provide an About dialog box for the macOS menu,
 * we can name the app in the system's audio mixer, etc). Those that want to
 * provide a _lot_ of information should look at the more-detailed
 * SetAppMetadataProperty().
 *
 * @{
 */

/**
 * @defgroup InitFlags Initialization flags
 *
 * @{
 */

/**
 * Initialization flags for Init and/or InitSubSystem
 *
 * These are the flags which may be passed to Init(). You should specify
 * the subsystems which you will be using in your application.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa Init
 * @sa Quit
 * @sa InitSubSystem
 * @sa QuitSubSystem
 * @sa WasInit
 */
using InitFlags = Uint32;

constexpr InitFlags INIT_AUDIO =
  SDL_INIT_AUDIO; ///< `INIT_AUDIO` implies `INIT_EVENTS`

/**
 * `INIT_VIDEO` implies `INIT_EVENTS`, should be initialized on the main thread
 */
constexpr InitFlags INIT_VIDEO = SDL_INIT_VIDEO;

constexpr InitFlags INIT_JOYSTICK =
  SDL_INIT_JOYSTICK; ///< `INIT_JOYSTICK` implies `INIT_EVENTS`

constexpr InitFlags INIT_HAPTIC = SDL_INIT_HAPTIC; ///< HAPTIC

constexpr InitFlags INIT_GAMEPAD =
  SDL_INIT_GAMEPAD; ///< `INIT_GAMEPAD` implies `INIT_JOYSTICK`

constexpr InitFlags INIT_EVENTS = SDL_INIT_EVENTS; ///< EVENTS

constexpr InitFlags INIT_SENSOR =
  SDL_INIT_SENSOR; ///< `INIT_SENSOR` implies `INIT_EVENTS`

constexpr InitFlags INIT_CAMERA =
  SDL_INIT_CAMERA; ///< `INIT_CAMERA` implies `INIT_EVENTS`

/// @}

/**
 * @name AppResult
 * App result for Main callback
 * @{
 */

/**
 * Return values for optional main callbacks.
 *
 * Returning APP_SUCCESS or APP_FAILURE from SDL_AppInit,
 * SDL_AppEvent, or SDL_AppIterate will terminate the program and report
 * success/failure to the operating system. What that means is
 * platform-dependent. On Unix, for example, on success, the process error
 * code will be zero, and on failure it will be 1. This interface doesn't
 * allow you to return specific exit codes, just whether there was an error
 * generally or not.
 *
 * Returning APP_CONTINUE from these functions will let the app continue
 * to run.
 *
 * See
 * [Main callbacks in
 * SDL3](https://wiki.libsdl.org/SDL3/README/main-functions#main-callbacks-in-sdl3)
 * for complete details.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using AppResult = SDL_AppResult;

/**
 * Value that requests that the app continue from the main callbacks.
 */
constexpr AppResult APP_CONTINUE = SDL_APP_CONTINUE;

/**
 * Value that requests termination with success from the main callbacks.
 */
constexpr AppResult APP_SUCCESS = SDL_APP_SUCCESS;

/**
 * Value that requests termination with error from the main callbacks.
 */
constexpr AppResult APP_FAILURE = SDL_APP_FAILURE;

/// @}

/**
 * @name Callbacks for EnterAppMainCallbacks()
 *
 * @{
 */

/**
 * Function pointer typedef for SDL_AppInit.
 *
 * These are used by EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppInit directly.
 *
 * @param appstate a place where the app can optionally store a pointer for
 *                 future use.
 * @param argc the standard ANSI C main's argc; number of elements in `argv`.
 * @param argv the standard ANSI C main's argv; array of command line
 *             arguments.
 * @returns APP_FAILURE to terminate with an error, APP_SUCCESS to
 *          terminate with success, APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppInit_func = SDL_AppInit_func;

/**
 * Function pointer typedef for SDL_AppIterate.
 *
 * These are used by EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppIterate directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @returns APP_FAILURE to terminate with an error, APP_SUCCESS to
 *          terminate with success, APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppIterate_func = SDL_AppIterate_func;

/**
 * Function pointer typedef for SDL_AppEvent.
 *
 * These are used by EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppEvent directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @param event the new event for the app to examine.
 * @returns APP_FAILURE to terminate with an error, APP_SUCCESS to
 *          terminate with success, APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppEvent_func = SDL_AppEvent_func;

/**
 * Function pointer typedef for SDL_AppQuit.
 *
 * These are used by EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppEvent directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @param result the result code that terminated the app (success or failure).
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppQuit_func = SDL_AppQuit_func;

/// @}

/**
 * Initialize the SDL library.
 *
 * Init() simply forwards to calling InitSubSystem(). Therefore, the
 * two may be used interchangeably. Though for readability of your code
 * InitSubSystem() might be preferred.
 *
 * The file I/O (for example: IOStream.FromFile) and threading
 * (SDL_CreateThread) subsystems are initialized by default. Message boxes
 * (SDL_ShowSimpleMessageBox) also attempt to work without initializing the
 * video subsystem, in hopes of being useful in showing an error dialog when
 * Init fails. You must specifically initialize other subsystems if you
 * use them in your application.
 *
 * Logging (such as Log) works without initialization, too.
 *
 * `flags` may be any of the following OR'd together:
 *
 * - `INIT_AUDIO`: audio subsystem; automatically initializes the events
 *   subsystem
 * - `INIT_VIDEO`: video subsystem; automatically initializes the events
 *   subsystem, should be initialized on the main thread.
 * - `INIT_JOYSTICK`: joystick subsystem; automatically initializes the
 *   events subsystem
 * - `INIT_HAPTIC`: haptic (force feedback) subsystem
 * - `INIT_GAMEPAD`: gamepad subsystem; automatically initializes the
 *   joystick subsystem
 * - `INIT_EVENTS`: events subsystem
 * - `INIT_SENSOR`: sensor subsystem; automatically initializes the events
 *   subsystem
 * - `INIT_CAMERA`: camera subsystem; automatically initializes the events
 *   subsystem
 *
 * Subsystem initialization is ref-counted, you must call QuitSubSystem()
 * for each InitSubSystem() to correctly shutdown a subsystem manually (or
 * call Quit() to force shutdown). If a subsystem is already loaded then
 * this call will increase the ref-count and return.
 *
 * Consider reporting some basic metadata about your application before
 * calling Init, using either SetAppMetadata() or
 * SetAppMetadataProperty().
 *
 * @param flags subsystem initialization flags.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadata
 * @sa SetAppMetadataProperty
 * @sa InitSubSystem
 * @sa Quit
 * @sa SetMainReady
 * @sa WasInit
 */
inline void Init(InitFlags flags) { CheckError(SDL_Init(flags)); }

/**
 * Compatibility function to initialize the SDL library.
 *
 * This function and Init() are interchangeable.
 *
 * @param flags any of the flags used by Init(); see Init for details.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init
 * @sa Quit
 * @sa QuitSubSystem
 */
inline void InitSubSystem(InitFlags flags)
{
  CheckError(SDL_InitSubSystem(flags));
}

/**
 * Shut down specific SDL subsystems.
 *
 * You still need to call Quit() even if you close all open subsystems
 * with QuitSubSystem().
 *
 * @param flags any of the flags used by Init(); see Init for details.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa InitSubSystem
 * @sa Quit
 */
inline void QuitSubSystem(InitFlags flags) { SDL_QuitSubSystem(flags); }

/**
 * Get a mask of the specified subsystems which are currently initialized.
 *
 * @param flags any of the flags used by Init(); see Init for details.
 * @returns a mask of all initialized subsystems if `flags` is 0, otherwise it
 *          returns the initialization status of the specified subsystems.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init
 * @sa InitSubSystem
 */
inline InitFlags WasInit(InitFlags flags) { return SDL_WasInit(flags); }

/**
 * Clean up all initialized subsystems.
 *
 * You should call this function even if you have already shutdown each
 * initialized subsystem with QuitSubSystem(). It is safe to call this
 * function even in the case of errors in initialization.
 *
 * You can use this function with atexit() to ensure that it is run when your
 * application is shutdown, but it is not wise to do this from a library or
 * other dynamically loaded code.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init
 * @sa QuitSubSystem
 */
inline void Quit() { SDL_Quit(); }

/**
 * Return whether this is the main thread.
 *
 * On Apple platforms, the main thread is the thread that runs your program's
 * main() entry point. On other platforms, the main thread is the one that
 * calls Init(INIT_VIDEO), which should usually be the one that runs
 * your program's main() entry point. If you are using the main callbacks,
 * SDL_AppInit(), SDL_AppIterate(), and SDL_AppQuit() are all called on the
 * main thread.
 *
 * @returns true if this thread is the main thread, or false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RunOnMainThread
 */
inline bool IsMainThread() { return SDL_IsMainThread(); }

/**
 * @name Callbacks for RunOnMainThread()
 * @{
 */

/**
 * Callback run on the main thread.
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa RunOnMainThread
 */
using MainThreadCallback = SDL_MainThreadCallback;

/**
 * Callback run on the main thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa RunOnMainThread
 * @sa MainThreadCallback
 *
 * @cat result-callback
 *
 */
using MainThreadCB = std::function<void()>;

/// @}

/**
 * Call a function on the main thread during event processing.
 *
 * If this is called on the main thread, the callback is executed immediately.
 * If this is called on another thread, this callback is queued for execution
 * on the main thread during event processing.
 *
 * Be careful of deadlocks when using this functionality. You should not have
 * the main thread wait for the current thread while this function is being
 * called with `wait_complete` true.
 *
 * @param callback the callback to call on the main thread.
 * @param userdata a pointer that is passed to `callback`.
 * @param wait_complete true to wait for the callback to complete, false to
 *                      return immediately.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IsMainThread
 */
inline void RunOnMainThread(MainThreadCallback callback,
                            void* userdata,
                            bool wait_complete)
{
  CheckError(SDL_RunOnMainThread(callback, userdata, wait_complete));
}

/**
 * Call a function on the main thread during event processing.
 *
 * If this is called on the main thread, the callback is executed immediately.
 * If this is called on another thread, this callback is queued for execution
 * on the main thread during event processing.
 *
 * Be careful of deadlocks when using this functionality. You should not have
 * the main thread wait for the current thread while this function is being
 * called with `wait_complete` true.
 *
 * @param callback the callback to call on the main thread.
 * @param wait_complete true to wait for the callback to complete, false to
 *                      return immediately.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IsMainThread
 * @sa result-callback
 *
 * @cat result-callback
 */
inline void RunOnMainThread(MainThreadCB callback, bool wait_complete)
{
  using Wrapper = CallbackWrapper<MainThreadCB>;
  void* wrapped = Wrapper::Wrap(std::move(callback));
  RunOnMainThread(&Wrapper::CallOnce, wrapped, wait_complete);
}

/**
 * Specify basic metadata about your app.
 *
 * You can optionally provide metadata about your app to SDL. This is not
 * required, but strongly encouraged.
 *
 * There are several locations where SDL can make use of metadata (an "About"
 * box in the macOS menu bar, the name of the app can be shown on some audio
 * mixers, etc). Any piece of metadata can be left as nullptr, if a specific
 * detail doesn't make sense for the app.
 *
 * This function should be called as early as possible, before Init.
 * Multiple calls to this function are allowed, but various state might not
 * change once it has been set up with a previous call to this function.
 *
 * Passing a nullptr removes any previous metadata.
 *
 * This is a simplified interface for the most important information. You can
 * supply significantly more detailed metadata with
 * SetAppMetadataProperty().
 *
 * @param appname The name of the application ("My Game 2: Bad Guy's
 *                Revenge!").
 * @param appversion The version of the application ("1.0.0beta5" or a git
 *                   hash, or whatever makes sense).
 * @param appidentifier A unique string in reverse-domain format that
 *                      identifies this app ("com.example.mygame2").
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadataProperty
 */
inline void SetAppMetadata(StringParam appname,
                           StringParam appversion,
                           StringParam appidentifier)
{
  CheckError(SDL_SetAppMetadata(appname, appversion, appidentifier));
}

/**
 * Specify metadata about your app through a set of properties.
 *
 * You can optionally provide metadata about your app to SDL. This is not
 * required, but strongly encouraged.
 *
 * There are several locations where SDL can make use of metadata (an "About"
 * box in the macOS menu bar, the name of the app can be shown on some audio
 * mixers, etc). Any piece of metadata can be left out, if a specific detail
 * doesn't make sense for the app.
 *
 * This function should be called as early as possible, before Init.
 * Multiple calls to this function are allowed, but various state might not
 * change once it has been set up with a previous call to this function.
 *
 * Once set, this metadata can be read using GetAppMetadataProperty().
 *
 * These are the supported properties:
 *
 * - `prop::appMetaData.NAME_STRING`: The human-readable name of the
 *   application, like "My Game 2: Bad Guy's Revenge!". This will show up
 *   anywhere the OS shows the name of the application separately from window
 *   titles, such as volume control applets, etc. This defaults to "SDL
 *   Application".
 * - `prop::appMetaData.VERSION_STRING`: The version of the app that is
 *   running; there are no rules on format, so "1.0.3beta2" and "April 22nd,
 *   2024" and a git hash are all valid options. This has no default.
 * - `prop::appMetaData.IDENTIFIER_STRING`: A unique string that
 *   identifies this app. This must be in reverse-domain format, like
 *   "com.example.mygame2". This string is used by desktop compositors to
 *   identify and group windows together, as well as match applications with
 *   associated desktop settings and icons. If you plan to package your
 *   application in a container such as Flatpak, the app ID should match the
 *   name of your Flatpak container as well. This has no default.
 * - `prop::appMetaData.CREATOR_STRING`: The human-readable name of the
 *   creator/developer/maker of this app, like "MojoWorkshop, LLC"
 * - `prop::appMetaData.COPYRIGHT_STRING`: The human-readable copyright
 *   notice, like "Copyright (c) 2024 MojoWorkshop, LLC" or whatnot. Keep this
 *   to one line, don't paste a copy of a whole software license in here. This
 *   has no default.
 * - `prop::appMetaData.URL_STRING`: A URL to the app on the web. Maybe a
 *   product page, or a storefront, or even a GitHub repository, for user's
 *   further information This has no default.
 * - `prop::appMetaData.TYPE_STRING`: The type of application this is.
 *   Currently this string can be "game" for a video game, "mediaplayer" for a
 *   media player, or generically "application" if nothing else applies.
 *   Future versions of SDL might add new types. This defaults to
 *   "application".
 *
 * @param name the name of the metadata property to set.
 * @param value the value of the property, or nullptr to remove that property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAppMetadataProperty
 * @sa SetAppMetadata
 */
inline void SetAppMetadataProperty(StringParam name, StringParam value)
{
  CheckError(SDL_SetAppMetadataProperty(name, value));
}

namespace prop::appMetaData {

constexpr auto NAME_STRING = SDL_PROP_APP_METADATA_NAME_STRING;

constexpr auto VERSION_STRING = SDL_PROP_APP_METADATA_VERSION_STRING;

constexpr auto IDENTIFIER_STRING = SDL_PROP_APP_METADATA_IDENTIFIER_STRING;

constexpr auto CREATOR_STRING = SDL_PROP_APP_METADATA_CREATOR_STRING;

constexpr auto COPYRIGHT_STRING = SDL_PROP_APP_METADATA_COPYRIGHT_STRING;

constexpr auto URL_STRING = SDL_PROP_APP_METADATA_URL_STRING;

constexpr auto TYPE_STRING = SDL_PROP_APP_METADATA_TYPE_STRING;

} // namespace prop::appMetaData

/**
 * Get metadata about your app.
 *
 * This returns metadata previously set using SetAppMetadata() or
 * SetAppMetadataProperty(). See SetAppMetadataProperty() for the list
 * of available properties and their meanings.
 *
 * @param name the name of the metadata property to get.
 * @returns the current value of the metadata property, or the default if it
 *          is not set, nullptr for properties with no default.
 *
 * @threadsafety It is safe to call this function from any thread, although
 *               the string returned is not protected and could potentially be
 *               freed if you call SetAppMetadataProperty() to set that
 *               property from another thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadata
 * @sa SetAppMetadataProperty
 */
inline const char* GetAppMetadataProperty(StringParam name)
{
  return SDL_GetAppMetadataProperty(name);
}

#ifndef SDL3PP_APPCLASS_LOG_PRIORITY
/**
 * The default log priority for app class.
 */
#define SDL3PP_APPCLASS_LOG_PRIORITY LOG_PRIORITY_CRITICAL
#endif // SDL3PP_APPCLASS_LOG_PRIORITY

/**
 * Represents application parameters
 */
using AppArgs = std::span<char const* const>;

/**
 * @{
 *
 * Allocate and initialize state with new.
 *
 * If possible, pass the args to constructor, otherwise expects a default ctor;
 *
 * @tparam T the state class
 * @param state the state to initialize
 * @param args the program arguments
 * @return the app status
 */
template<class T>
inline AppResult DefaultCreateClass(T** state, AppArgs args)
{
  static_assert(std::is_default_constructible_v<T>);
  *state = new T{};
  return APP_CONTINUE;
}

template<class T>
  requires std::convertible_to<AppArgs, T>
inline AppResult DefaultCreateClass(T** state, AppArgs args)
{
  *state = new T{args};
  return APP_CONTINUE;
}
/// @}

/// @private
template<class T>
concept HasInitFunction = requires(T** state) {
  { T::Init(state, AppArgs{}) } -> std::convertible_to<AppResult>;
};

/**
 * @{
 *
 * Init state with arguments.
 *
 * This will call T::Init() if available, otherwise it delegates to
 * DefaultCreateClass().
 *
 * @tparam T the state class
 * @param state the state to initialize
 * @param args the program arguments
 * @return the app status
 */
template<class T>
inline AppResult InitClass(T** state, AppArgs args)
{
  try {
    return DefaultCreateClass(state, args);
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}

template<HasInitFunction T>
inline AppResult InitClass(T** state, AppArgs args)
{
  *state = nullptr;
  try {
    AppResult result = T::Init(state, args);
    if (*state == nullptr && result != APP_FAILURE) return APP_SUCCESS;
    return result;
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}
/// @}

/// @private
template<class T>
concept HasIterateFunction = requires(T* state) { state->Iterate(); };

/**
 * Iterate the state
 *
 * @tparam T the state class
 * @param state the state
 * @return the app status
 */
template<HasIterateFunction T>
inline AppResult IterateClass(T* state)
{
  try {
    return state->Iterate();
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}

/// @private
template<class T>
concept HasEventFunction =
  requires(T* state, const SDL_Event& event) { state->Event(event); };

/**
 * Default handle by finishing if QUIT is requested
 *
 * @tparam T the state class
 * @param state the state
 * @param event the event
 * @return APP_SUCCESS if event is QUIT_EVENT, APP_CONTINUE otherwise,
 */
template<class T>
inline AppResult DefaultEventClass(T* state, const SDL_Event& event)
{
  if (event.type == SDL_EVENT_QUIT) return APP_SUCCESS;
  return APP_CONTINUE;
}

/**
 * @{
 * Iterate the state
 *
 * @tparam T the state class
 * @param state the state
 * @param event the event to handle
 * @return the app status
 */
template<class T>
inline AppResult EventClass(T* state, const SDL_Event& event)
{
  try {
    return DefaultEventClass(state, event);
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}

template<HasEventFunction T>
inline AppResult EventClass(T* state, const SDL_Event& event)
{
  try {
    return state->Event(event);
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}

/// @}

/**
 * Destroy state with delete;
 *
 * @tparam T
 * @param state
 */
template<class T>
inline void DefaultClassDestroy(T* state)
{
  delete state;
}

/// @private
template<class T>
concept HasQuitFunction =
  requires(T* state, AppResult result) { T::Quit(state, result); };

/**
 * @{
 * Destroy state with given result
 *
 * This is responsible to destroy and deallocate the state. It tries to call
 * T::Quit() if available and delegates to it the duty of deleting. Otherwise it
 * calls delete directly.
 *
 * @tparam T the state class.
 * @param state the state to destroy.
 * @param result the app result.
 */
template<class T>
inline void QuitClass(T* state, AppResult result)
{
  DefaultClassDestroy(state);
}

template<HasQuitFunction T>
inline void QuitClass(T* state, AppResult result)
{
  T::Quit(state, result);
}
/// @}

/// @}

/**
 * @defgroup CategoryRect Rectangle Functions
 *
 * Some helper functions for managing rectangles and 2D points, in both
 * integer and floating point versions.
 *
 * @{
 */

using PointRaw = SDL_Point;

using FPointRaw = SDL_FPoint;

using RectRaw = SDL_Rect;

using FRectRaw = SDL_FRect;

// Forward decl
struct FPoint;

// Forward decl
struct Rect;

// Forward decl
struct FRect;

/**
 * The structure that defines a point (using integers).
 *
 * Inspired by
 * https://github.com/libSDL2pp/libSDL2pp/blob/master/SDL2pp/Point.hh
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa Rect.GetEnclosingPoints
 * @sa Point.InRect
 */
struct Point : SDL_Point
{
  /**
   * Wraps Point.
   *
   * @param p the value to be wrapped
   */
  constexpr Point(const PointRaw& p = {})
    : SDL_Point(p)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the value for x.
   * @param y the value for y.
   */
  constexpr Point(int x, int y)
    : SDL_Point{x, y}
  {
  }

  /**
   * Wraps Point.
   *
   * @param p the value to be wrapped
   */
  constexpr explicit Point(const FPointRaw& p)
    : SDL_Point{int(p.x), int(p.y)}
  {
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const PointRaw& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const Point& other) const
  {
    return *this == (const PointRaw&)(other);
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != SDL_Point{}; }

  /**
   * Get x coordinate
   *
   * @returns x coordinate
   */
  constexpr int GetX() const { return x; }

  /**
   * Set the x coordinate.
   *
   * @param newX the new x coordinate.
   * @returns Reference to self.
   */
  constexpr Point& SetX(int newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get y coordinate
   *
   * @returns y coordinate
   */
  constexpr int GetY() const { return y; }

  /**
   * Set the y coordinate.
   *
   * @param newY the new y coordinate.
   * @returns Reference to self.
   */
  constexpr Point& SetY(int newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Determine whether a point resides inside a rectangle.
   *
   * A point is considered part of a rectangle if both `p` and `r` are not
   * nullptr, and `p`'s x and y coordinates are >= to the rectangle's top left
   * corner, and < the rectangle's x+w and y+h. So a 1x1 rectangle considers
   * point (0,0) as "inside" and (0,1) as not.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param r the rectangle to test.
   * @returns true if this is contained by `r`, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool InRect(const RectRaw& r) const
  {
    return SDL_PointInRect(this, &r);
  }

  /**
   * Get point's memberwise negation
   *
   * @returns New Point representing memberwise negation
   *
   */
  constexpr Point operator-() const { return Point(-x, -y); }

  /**
   * Get point's memberwise addition with another point
   *
   * @param[in] other Point to add
   *
   * @returns New Point representing memberwise addition with another point
   *
   */
  constexpr Point operator+(const Point& other) const
  {
    return Point(x + other.x, y + other.y);
  }

  /**
   * Get point's memberwise subtraction with another point
   *
   * @param[in] other Point to subtract
   *
   * @returns New Point representing memberwise subtraction of another point
   *
   */
  constexpr Point operator-(const Point& other) const
  {
    return Point(x - other.x, y - other.y);
  }

  /**
   * Get point's memberwise division by an integer
   *
   * @param[in] value Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by an integer
   *
   */
  constexpr Point operator/(int value) const
  {
    return Point(x / value, y / value);
  }

  /**
   * Get point's memberwise division by an integer
   *
   * @param[in] value Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by an integer
   *
   */
  constexpr FPoint operator/(float value) const;

  /**
   * Get point's memberwise division by another point
   *
   * @param[in] other Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by another point
   *
   */
  constexpr Point operator/(const Point& other) const
  {
    return Point(x / other.x, y / other.y);
  }

  /**
   * Get point's memberwise remainder from division
   *        by an integer
   *
   * @param[in] value Divisor
   *
   * @returns New Point representing memberwise remainder
   *          from division by an integer
   *
   */
  constexpr Point operator%(int value) const
  {
    return Point(x % value, y % value);
  }

  /**
   * Get point's memberwise remainder from division
   *        by another point
   *
   * @param[in] other Divisor
   *
   * @returns New Point representing memberwise remainder
   *          from division by another point
   *
   */
  constexpr Point operator%(const Point& other) const
  {
    return Point(x % other.x, y % other.y);
  }

  /**
   * Get point's memberwise multiplication by an
   *        integer
   *
   * @param[in] value Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by an integer
   *
   */
  constexpr Point operator*(int value) const
  {
    return Point(x * value, y * value);
  }

  /**
   * Get point's memberwise multiplication by an
   *        integer
   *
   * @param[in] value Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by an integer
   *
   */
  constexpr FPoint operator*(float value) const;

  /**
   * Get point's memberwise multiplication by another
   *        point
   *
   * @param[in] other Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by another point
   *
   */
  constexpr Point operator*(const Point& other) const
  {
    return Point(x * other.x, y * other.y);
  }

  /**
   * Memberwise add another point
   *
   * @param[in] other Point to add to the current one
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator+=(const Point& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * Memberwise subtract another point
   *
   * @param[in] other Point to subtract from the current one
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator-=(const Point& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * Memberwise divide by an integer
   *
   * @param[in] value Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator/=(int value)
  {
    x /= value;
    y /= value;
    return *this;
  }

  /**
   * Memberwise divide by another point
   *
   * @param[in] other Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator/=(const Point& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  /**
   * Memberwise remainder from division by an integer
   *
   * @param[in] value Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator%=(int value)
  {
    x %= value;
    y %= value;
    return *this;
  }

  /**
   * Memberwise remainder from division by another
   *        point
   *
   * @param[in] other Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator%=(const Point& other)
  {
    x %= other.x;
    y %= other.y;
    return *this;
  }

  /**
   * Memberwise multiply by an integer
   *
   * @param[in] value Multiplier
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator*=(int value)
  {
    x *= value;
    y *= value;
    return *this;
  }

  /**
   * Memberwise multiply by another point
   *
   * @param[in] other Multiplier
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator*=(const Point& other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  /**
   * Get a point with coordinates modified so it fits
   *        into a given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Clamped point
   *
   */
  constexpr Point GetClamped(const Rect& rect) const;

  /**
   * Clamp point coordinates to make it fit into a
   *        given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Reference to self
   *
   */
  constexpr Point& Clamp(const Rect& rect);

  /**
   * Get a point wrapped within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Wrapped point
   *
   */
  constexpr Point GetWrapped(const Rect& rect) const;

  /**
   * Wrap point coordinates within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Reference to self
   *
   */
  constexpr Point& Wrap(const Rect& rect);

  /**
   * Converts to FPoint
   *
   * @return FPoint
   */
  constexpr operator FPoint() const;
};

/**
 * The structure that defines a point (using floating point values).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa FRect.GetEnclosingPoints
 * @sa FPoint.InRect
 */
struct FPoint : SDL_FPoint
{
  /**
   * Wraps FPoint.
   *
   * @param p the value to be wrapped
   */
  constexpr FPoint(const FPointRaw& p = {})
    : SDL_FPoint(p)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the value for x.
   * @param y the value for y.
   */
  constexpr FPoint(float x, float y)
    : SDL_FPoint{x, y}
  {
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FPointRaw& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FPoint& other) const
  {
    return *this == (const FPointRaw&)(other);
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != SDL_FPoint{}; }

  /**
   * Get the x coordinate.
   *
   * @returns current x value.
   */
  constexpr float GetX() const { return x; }

  /**
   * Set the x coordinate.
   *
   * @param newX the new x coordinate.
   * @returns Reference to self.
   */
  constexpr FPoint& SetX(float newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get the y coordinate.
   *
   * @returns current y coordinate.
   */
  constexpr float GetY() const { return y; }

  /**
   * Set the y coordinate.
   *
   * @param newY the new y coordinate.
   * @returns Reference to self.
   */
  constexpr FPoint& SetY(float newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Determine whether a point resides inside a floating point rectangle.
   *
   * A point is considered part of a rectangle if both `p` and `r` are not
   * nullptr, and `p`'s x and y coordinates are >= to the rectangle's top left
   * corner, and <= the rectangle's x+w and y+h. So a 1x1 rectangle considers
   * point (0,0) and (0,1) as "inside" and (0,2) as not.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param r the rectangle to test.
   * @returns true if this is contained by `r`, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool InRect(const FRectRaw& r) const;

  /**
   * Get point's memberwise negation
   *
   * @returns New Point representing memberwise negation
   *
   */
  constexpr FPoint operator-() const { return FPoint(-x, -y); }

  /**
   * Get point's memberwise addition with another point
   *
   * @param[in] other Point to add
   *
   * @returns New Point representing memberwise addition with another point
   *
   */
  constexpr FPoint operator+(const FPoint& other) const
  {
    return FPoint(x + other.x, y + other.y);
  }

  /**
   * Get point's memberwise subtraction with another point
   *
   * @param[in] other Point to subtract
   *
   * @returns New Point representing memberwise subtraction of another point
   *
   */
  constexpr FPoint operator-(const FPoint& other) const
  {
    return FPoint(x - other.x, y - other.y);
  }

  /**
   * Get point's memberwise division by an float
   *
   * @param[in] value Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by an float
   *
   */
  constexpr FPoint operator/(float value) const
  {
    return FPoint(x / value, y / value);
  }

  /**
   * Get point's memberwise division by another point
   *
   * @param[in] other Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by another point
   *
   */
  constexpr FPoint operator/(const FPoint& other) const
  {
    return FPoint(x / other.x, y / other.y);
  }

  /**
   * Get point's memberwise multiplication by an
   *        float
   *
   * @param[in] value Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by an float
   *
   */
  constexpr FPoint operator*(float value) const
  {
    return FPoint(x * value, y * value);
  }

  /**
   * Get point's memberwise multiplication by another
   *        point
   *
   * @param[in] other Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by another point
   *
   */
  constexpr FPoint operator*(const FPoint& other) const
  {
    return FPoint(x * other.x, y * other.y);
  }

  /**
   * Memberwise add another point
   *
   * @param[in] other Point to add to the current one
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator+=(const FPoint& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * Memberwise subtract another point
   *
   * @param[in] other Point to subtract from the current one
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator-=(const FPoint& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * Memberwise divide by an float
   *
   * @param[in] value Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator/=(float value)
  {
    x /= value;
    y /= value;
    return *this;
  }

  /**
   * Memberwise divide by another point
   *
   * @param[in] other Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator/=(const FPoint& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  /**
   * Memberwise multiply by an float
   *
   * @param[in] value Multiplier
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator*=(float value)
  {
    x *= value;
    y *= value;
    return *this;
  }

  /**
   * Memberwise multiply by another point
   *
   * @param[in] other Multiplier
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator*=(const FPoint& other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  /**
   * Get a point with coordinates modified so it fits
   *        into a given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Clamped point
   *
   */
  constexpr FPoint GetClamped(const FRect& rect) const;

  /**
   * Clamp point coordinates to make it fit into a
   *        given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& Clamp(const FRect& rect);

  /**
   * Get a point wrapped within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Wrapped point
   *
   */
  constexpr FPoint GetWrapped(const FRect& rect) const;

  /**
   * Wrap point coordinates within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& Wrap(const FRect& rect);
};

/**
 * A rectangle, with the origin at the upper left (using integers).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa Rect.Empty
 * @sa Rect.Equal
 * @sa Rect.HasIntersection
 * @sa Rect.GetIntersection
 * @sa Rect.GetLineIntersection
 * @sa Rect.GetUnion
 * @sa Rect.GetEnclosingPoints
 */
struct Rect : SDL_Rect
{
  /**
   * Wraps Rect.
   *
   * @param r the value to be wrapped
   */
  constexpr Rect(const RectRaw& r = {})
    : SDL_Rect(r)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the left x.
   * @param y the top y.
   * @param w the width.
   * @param h the height.
   */
  constexpr Rect(int x, int y, int w, int h)
    : SDL_Rect{x, y, w, h}
  {
  }

  /**
   * Construct from offset and size
   *
   * @param corner the top-left corner
   * @param size the size
   */
  constexpr Rect(const PointRaw& corner, const PointRaw& size)
    : Rect{corner.x, corner.y, size.x, size.y}
  {
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const RectRaw& other) const { return Equal(other); }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const Rect& other) const
  {
    return *this == (const RectRaw&)(other);
  }

  /**
   * @sa Empty()
   */
  constexpr explicit operator bool() const { return !Empty(); }

  /**
   * Get left x coordinate.
   *
   * @returns coordinate of the left x
   */
  constexpr int GetX() const { return x; }

  /**
   * Set the left x coordinate.
   *
   * @param newX the new left x.
   * @returns Reference to self.
   */
  constexpr Rect& SetX(int newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get top y coordinate.
   *
   * @returns coordinate of the top y.
   */
  constexpr int GetY() const { return y; }

  /**
   * Set the top y coordinate.
   *
   * @param newY the new top y.
   * @returns Reference to self.
   */
  constexpr Rect& SetY(int newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Get width of the rect
   *
   * @returns Width of the rect
   */
  constexpr int GetW() const { return w; }

  /**
   * Set the width of the rect.
   *
   * @param newW the new width.
   * @returns Reference to self.
   */
  constexpr Rect& SetW(int newW)
  {
    w = newW;
    return *this;
  }

  /**
   * Get height of the rect
   *
   * @returns Height of the rect
   */
  constexpr int GetH() const { return h; }

  /**
   * Set the height of the rect.
   *
   * @param newH the new height.
   * @returns Reference to self.
   */
  constexpr Rect& SetH(int newH)
  {
    h = newH;
    return *this;
  }

  /**
   * Calculate a minimal rectangle enclosing a set of points.
   *
   * If `clip` is not nullptr then only points inside of the clipping rectangle
   * are considered.
   *
   * @param points a span of SDL_Point structures representing points to be
   *               enclosed.
   * @param clip an SDL_Rect used for clipping or std::nullopt to enclose all
   *             points.
   * @returns a SDL_Rect structure filled in with the minimal enclosing
   *          rectangle or an empty rect if all the points were outside of the
   *          clipping rectangle.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static Rect GetEnclosingPoints(SpanRef<const PointRaw> points,
                                 OptionalRef<const RectRaw> clip = std::nullopt)
  {
    Rect result;
    if (SDL_GetRectEnclosingPoints(
          points.data(), points.size(), clip, &result)) {
      return result;
    }
    return {};
  }

  /**
   * Construct the rect from given center coordinates, width and height
   *
   * @param[in] cx X coordinate of the rectangle center
   * @param[in] cy Y coordinate of the rectangle center
   * @param[in] w Width of the rectangle
   * @param[in] h Height of the rectangle
   *
   */
  static constexpr Rect FromCenter(int cx, int cy, int w, int h)
  {
    return Rect(cx - w / 2, cy - h / 2, w, h);
  }

  /**
   * Construct the rect from given center coordinates and size
   *
   * @param[in] center Coordinates of the rectangle center
   * @param[in] size Dimensions of the rectangle
   *
   */
  static constexpr Rect FromCenter(Point center, Point size)
  {
    return Rect(center - size / 2, size);
  }

  /**
   * Construct the rect from given corners coordinates
   *
   * @param[in] x1 X coordinate of the top left rectangle corner
   * @param[in] y1 Y coordinate of the top left rectangle corner
   * @param[in] x2 X coordinate of the bottom right rectangle corner
   * @param[in] y2 Y coordinate of the bottom right rectangle corner
   *
   */
  static constexpr Rect FromCorners(int x1, int y1, int x2, int y2)
  {
    return Rect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
  }

  /**
   * Construct the rect from given centers coordinates
   *
   * @param[in] p1 Coordinates of the top left rectangle corner
   * @param[in] p2 Coordinates of the bottom right rectangle corner
   *
   */
  static constexpr Rect FromCorners(Point p1, Point p2)
  {
    return Rect(p1, p2 - p1 + Point(1, 1));
  }

  /**
   * Get X coordinate of the rect second corner
   *
   * @returns X coordinate of the rect second corner
   *
   */
  constexpr int GetX2() const { return x + w - 1; }

  /**
   * Set X coordinate of the rect second corner
   *
   * @param[in] x2 New X coordinate value
   *
   * This modifies rectangle width internally
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetX2(int x2)
  {
    w = x2 - x + 1;
    return *this;
  }

  /**
   * Get Y coordinate of the rect second corner
   *
   * @returns Y coordinate of the rect second corner
   *
   */
  constexpr int GetY2() const { return y + h - 1; }

  /**
   * Set Y coordinate of the rect second corner
   *
   * @param[in] y2 New Y coordinate value
   *
   * This modifies rectangle height internally
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetY2(int y2)
  {
    h = y2 - y + 1;
    return *this;
  }

  /**
   * Get top left corner of the rect
   *
   * @returns Top left corner of the rect
   *
   */
  constexpr Point GetTopLeft() const { return Point(x, y); }

  /**
   * Get top right corner of the rect
   *
   * @returns Top right corner of the rect
   *
   */
  constexpr Point GetTopRight() const { return Point(GetX2(), y); }

  /**
   * Get bottom left corner of the rect
   *
   * @returns bottom left corner of the rect
   *
   */
  constexpr Point GetBottomLeft() const { return Point(x, GetY2()); }

  /**
   * Get bottom right corner of the rect
   *
   * @returns Bottom right corner of the rect
   *
   */
  constexpr Point GetBottomRight() const { return Point(GetX2(), GetY2()); }

  /**
   * Get size of the rect
   *
   * @returns Size of the rect
   *
   */
  constexpr Point GetSize() const { return Point(w, h); }

  /**
   * Get centroid of the rect
   *
   * @returns Centroid of the rect
   *
   */
  constexpr Point GetCentroid() const { return Point(x + w / 2, y + h / 2); }

  /**
   * Calculate the intersection of a rectangle and line segment
   *
   * @param[in,out] p1 Starting coordinates of the line
   * @param[in,out] p2 Ending coordinates of the line
   *
   * @returns True if there is an intersection, false otherwise
   *
   * This function is used to clip a line segment to a
   * rectangle. A line segment contained entirely within the
   * rectangle or that does not intersect will remain unchanged.
   * A line segment that crosses the rectangle at either or both
   * ends will be clipped to the boundary of the rectangle and
   * the new coordinates saved in p1 and/or p2 as necessary.
   *
   */
  bool GetLineIntersection(PointRaw* p1, PointRaw* p2) const
  {
    return GetLineIntersection(p1 ? &p1->x : nullptr,
                               p1 ? &p1->y : nullptr,
                               p2 ? &p2->x : nullptr,
                               p2 ? &p2->y : nullptr);
  }

  /**
   * Calculate the intersection of a rectangle and line segment.
   *
   * This function is used to clip a line segment to a rectangle. A line segment
   * contained entirely within the rectangle or that does not intersect will
   * remain unchanged. A line segment that crosses the rectangle at either or
   * both ends will be clipped to the boundary of the rectangle and the new
   * coordinates saved in `X1`, `Y1`, `X2`, and/or `Y2` as necessary.
   *
   * @param X1 a pointer to the starting X-coordinate of the line.
   * @param Y1 a pointer to the starting Y-coordinate of the line.
   * @param X2 a pointer to the ending X-coordinate of the line.
   * @param Y2 a pointer to the ending Y-coordinate of the line.
   * @returns true if there is an intersection, false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetLineIntersection(int* X1, int* Y1, int* X2, int* Y2) const
  {
    return SDL_GetRectAndLineIntersection(this, X1, Y1, X2, Y2);
  }

  /**
   * Convert an SDL_Rect to SDL_FRect
   *
   * @return A FRect filled in with the floating point representation of
   *              `rect`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr operator SDL_FRect() const
  {
    SDL_FRect r;
    SDL_RectToFRect(this, &r);
    return r;
  }

  /**
   * @sa operator ToFRect()
   */
  constexpr operator FRect() const;

  /**
   * Determine whether a rectangle has no area.
   *
   * A rectangle is considered "empty" for this function if `r` is nullptr, or
   * if `r`'s width and/or height are <= 0.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @returns true if the rectangle is "empty", false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Empty() const { return SDL_RectEmpty(this); }

  /**
   * Determine whether two rectangles are equal.
   *
   * Rectangles are considered equal if both are not nullptr and each of their
   * x, y, width and height match.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param other the second rectangle to test.
   * @returns true if the rectangles are equal, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Equal(const RectRaw& other) const
  {
    return SDL_RectsEqual(this, &other);
  }

  /**
   * Check whether the rect contains given point
   *
   * @param p Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const PointRaw& p) const
  {
    return SDL_PointInRect(&p, this);
  }

  /**
   * Check whether the rect contains given point
   *
   * @param other Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const RectRaw& other) const
  {
    return GetUnion(other) == *this;
  }

  /**
   * Determine whether two rectangles intersect.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns true if there is an intersection, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Rect.GetIntersection
   */
  constexpr bool HasIntersection(const RectRaw& other) const
  {
    return SDL_HasRectIntersection(this, &other);
  }

  /**
   * Calculate the intersection of two rectangles.
   *
   * If `result` is nullptr then this function will return false.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns an SDL_Rect structure filled in with the intersection of
   *               if there is intersection, std::nullopt otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Rect.HasIntersection
   */
  constexpr Rect GetIntersection(const RectRaw& other) const
  {
    if (Rect result; SDL_GetRectIntersection(this, &other, &result)) {
      return result;
    }
    return {};
  }

  /**
   * Calculate the union of two rectangles.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns Rect representing union of two rectangles
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr Rect GetUnion(const RectRaw& other) const
  {
    Rect result;
    CheckError(SDL_GetRectUnion(this, &other, &result));
    return result;
  }

  /**
   * Get a rect extended by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Extended rect
   *
   */
  constexpr Rect GetExtension(unsigned int amount) const
  {
    Rect r = *this;
    r.Extend(amount);
    return r;
  }

  /**
   * Get a rect extended by specified amount of pixels
   *
   * @param[in] hAmount Number of pixels to extend by
   *                    in horizontal direction
   * @param[in] vAmount Number of pixels to extend by
   *                    in vertical direction
   *
   * @returns Extended rect
   *
   */
  constexpr Rect GetExtension(unsigned int hAmount, unsigned int vAmount) const
  {
    Rect r = *this;
    r.Extend(hAmount, vAmount);
    return r;
  }

  /**
   * Extend a rect by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& Extend(unsigned int amount) { return Extend(amount, amount); }

  /**
   * Extend a rect by specified amount of pixels
   *
   * @param[in] hAmount Number of pixels to extend by
   *                    in horizontal direction
   * @param[in] vAmount Number of pixels to extend by
   *                    in vertical direction
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& Extend(unsigned int hAmount, unsigned int vAmount)
  {
    x -= hAmount;
    y -= vAmount;
    w += hAmount * 2;
    h += vAmount * 2;
    return *this;
  }

  /**
   * Get rectangle moved by a given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Moved rectangle
   *
   */
  constexpr Rect operator+(const Point& offset) const
  {
    return Rect(x + offset.x, y + offset.y, w, h);
  }

  /**
   * Get rectangle moved by an opposite of given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Moved rectangle
   *
   */
  constexpr Rect operator-(const Point& offset) const
  {
    return Rect(x - offset.x, y - offset.y, w, h);
  }

  /**
   * Move by then given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& operator+=(const Point& offset)
  {
    x += offset.x;
    y += offset.y;
    return *this;
  }

  /**
   * Move by an opposite of the given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& operator-=(const Point& offset)
  {
    x -= offset.x;
    y -= offset.y;
    return *this;
  }
};

/**
 * A rectangle, with the origin at the upper left (using floating point
 * values).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa FRect.Empty
 * @sa FRect.Equal
 * @sa FRect.EqualEpsilon
 * @sa FRect.HasIntersection
 * @sa FRect.GetIntersection
 * @sa FRect.GetLineIntersection
 * @sa FRect.GetUnion
 * @sa FRect.GetEnclosingPoints
 * @sa FPoint.InRect
 */
struct FRect : SDL_FRect
{
  /**
   * Wraps FRect.
   *
   * @param r the value to be wrapped
   */
  constexpr FRect(const FRectRaw& r = {})
    : SDL_FRect(r)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the left x.
   * @param y the top y.
   * @param w the width.
   * @param h the height.
   */
  constexpr FRect(float x, float y, float w, float h)
    : SDL_FRect{x, y, w, h}
  {
  }

  /**
   * Constructs from top-left corner plus size
   */
  constexpr FRect(FPoint corner, FPoint size)
    : FRect{corner.x, corner.y, size.x, size.y}
  {
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FRectRaw& other) const
  {
    return Equal(other);
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FRect& other) const
  {
    return *this == (const FRectRaw&)(other);
  }

  /**
   * @sa Empty()
   */
  constexpr operator bool() const { return !Empty(); }

  /**
   * Get left x coordinate.
   *
   * @returns coordinate of the left x
   */
  constexpr float GetX() const { return x; }

  /**
   * Set the left x coordinate.
   *
   * @param newX the new left x.
   * @returns Reference to self.
   */
  constexpr FRect& SetX(float newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get top y coordinate.
   *
   * @returns coordinate of the top y.
   */
  constexpr float GetY() const { return y; }

  /**
   * Set the top y coordinate.
   *
   * @param newY the new top y.
   * @returns Reference to self.
   */
  constexpr FRect& SetY(float newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Get width of the rect
   *
   * @returns Width of the rect
   */
  constexpr float GetW() const { return w; }

  /**
   * Set the width of the rect.
   *
   * @param newW the new width.
   * @returns Reference to self.
   */
  constexpr FRect& SetW(float newW)
  {
    w = newW;
    return *this;
  }

  /**
   * Get height of the rect
   *
   * @returns Height of the rect
   */
  constexpr float GetH() const { return h; }

  /**
   * Set the height of the rect.
   *
   * @param newH the new height.
   * @returns Reference to self.
   */
  constexpr FRect& SetH(float newH)
  {
    h = newH;
    return *this;
  }

  /**
   * Calculate a minimal rectangle enclosing a set of points with float
   * precision.
   *
   * If `clip` is not nullptr then only points inside of the clipping rectangle
   * are considered.
   *
   * @param points a span of SDL_Point structures representing points to be
   *               enclosed.
   * @param clip an SDL_Rect used for clipping or std::nullopt to enclose all
   *             points.
   * @returns a FRect structure filled in with the minimal enclosing
   *          rectangle or an empty FRect if all the points were outside of
   *          the clipping rectangle.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static constexpr FRect GetEnclosingPoints(
    SpanRef<const FPointRaw> points,
    OptionalRef<const FRectRaw> clip = std::nullopt)
  {
    if (FRect result; SDL_GetRectEnclosingPointsFloat(
          points.data(), points.size(), clip, &result)) {
      return result;
    }
    return {};
  }

  /**
   * Construct the rect from given center coordinates, width and height
   *
   * @param[in] cx X coordinate of the rectangle center
   * @param[in] cy Y coordinate of the rectangle center
   * @param[in] w Width of the rectangle
   * @param[in] h Height of the rectangle
   *
   */
  static constexpr FRect FromCenter(float cx, float cy, float w, float h)
  {
    return FRect(cx - w / 2, cy - h / 2, w, h);
  }

  /**
   * Construct the rect from given center coordinates and size
   *
   * @param[in] center Coordinates of the rectangle center
   * @param[in] size Dimensions of the rectangle
   *
   */
  static constexpr FRect FromCenter(FPoint center, FPoint size)
  {
    return FRect(center - size / 2, size);
  }

  /**
   * Construct the rect from given corners coordinates
   *
   * @param[in] x1 X coordinate of the top left rectangle corner
   * @param[in] y1 Y coordinate of the top left rectangle corner
   * @param[in] x2 X coordinate of the bottom right rectangle corner
   * @param[in] y2 Y coordinate of the bottom right rectangle corner
   *
   */
  static constexpr FRect FromCorners(float x1, float y1, float x2, float y2)
  {
    return FRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
  }

  /**
   * Construct the rect from given centers coordinates
   *
   * @param[in] p1 Coordinates of the top left rectangle corner
   * @param[in] p2 Coordinates of the bottom right rectangle corner
   *
   */
  static constexpr FRect FromCorners(FPoint p1, FPoint p2)
  {
    return FRect(p1, p2 - p1 + FPoint(1, 1));
  }

  /**
   * Get X coordinate of the rect second corner
   *
   * @returns X coordinate of the rect second corner
   *
   */
  constexpr float GetX2() const { return x + w - 1; }

  /**
   * Set X coordinate of the rect second corner
   *
   * @param[in] x2 New X coordinate value
   *
   * This modifies rectangle width internally
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetX2(float x2)
  {
    w = x2 - x + 1;
    return *this;
  }

  /**
   * Get Y coordinate of the rect second corner
   *
   * @returns Y coordinate of the rect second corner
   *
   */
  constexpr float GetY2() const { return y + h - 1; }

  /**
   * Set Y coordinate of the rect second corner
   *
   * @param[in] y2 New Y coordinate value
   *
   * This modifies rectangle height internally
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetY2(float y2)
  {
    h = y2 - y + 1;
    return *this;
  }

  /**
   * Get top left corner of the rect
   *
   * @returns Top left corner of the rect
   *
   */
  constexpr FPoint GetTopLeft() const { return FPoint(x, y); }

  /**
   * Get top right corner of the rect
   *
   * @returns Top right corner of the rect
   *
   */
  constexpr FPoint GetTopRight() const { return FPoint(GetX2(), y); }

  /**
   * Get bottom left corner of the rect
   *
   * @returns bottom left corner of the rect
   *
   */
  constexpr FPoint GetBottomLeft() const { return FPoint(x, GetY2()); }

  /**
   * Get bottom right corner of the rect
   *
   * @returns Bottom right corner of the rect
   *
   */
  constexpr FPoint GetBottomRight() const { return FPoint(GetX2(), GetY2()); }

  /**
   * Get size of the rect
   *
   * @returns Size of the rect
   *
   */
  constexpr FPoint GetSize() const { return FPoint(w, h); }

  /**
   * Get centroid of the rect
   *
   * @returns Centroid of the rect
   *
   */
  constexpr FPoint GetCentroid() const { return FPoint(x + w / 2, y + h / 2); }

  /**
   * Calculate the intersection of a rectangle and line segment with float
   * precision.
   *
   * This function is used to clip a line segment to a rectangle. A line segment
   * contained entirely within the rectangle or that does not intersect will
   * remain unchanged. A line segment that crosses the rectangle at either or
   * both ends will be clipped to the boundary of the rectangle and the new
   * coordinates saved in `X1`, `Y1`, `X2`, and/or `Y2` as necessary.
   *
   * @param X1 a pointer to the starting X-coordinate of the line.
   * @param Y1 a pointer to the starting Y-coordinate of the line.
   * @param X2 a pointer to the ending X-coordinate of the line.
   * @param Y2 a pointer to the ending Y-coordinate of the line.
   * @returns true if there is an intersection, false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetLineIntersection(float* X1, float* Y1, float* X2, float* Y2) const
  {
    return SDL_GetRectAndLineIntersectionFloat(this, X1, Y1, X2, Y2);
  }

  /**
   * Calculate the intersection of a rectangle and line segment
   *
   * @param[in,out] p1 Starting coordinates of the line
   * @param[in,out] p2 Ending coordinates of the line
   *
   * @returns True if there is an intersection, false otherwise
   *
   * This function is used to clip a line segment to a
   * rectangle. A line segment contained entirely within the
   * rectangle or that does not intersect will remain unchanged.
   * A line segment that crosses the rectangle at either or both
   * ends will be clipped to the boundary of the rectangle and
   * the new coordinates saved in p1 and/or p2 as necessary.
   *
   */
  bool GetLineIntersection(FPoint* p1, FPoint* p2) const
  {
    return GetLineIntersection(p1 ? &p1->x : nullptr,
                               p1 ? &p1->y : nullptr,
                               p2 ? &p2->x : nullptr,
                               p2 ? &p2->y : nullptr);
  }

  /**
   * Determine whether a rectangle has no area.
   *
   * A rectangle is considered "empty" for this function if `r` is NULL, or if
   * `r`'s width and/or height are <= 0.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @returns true if the rectangle is "empty", false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Empty() const { return SDL_RectEmptyFloat(this); }

  /**
   * Determine whether two floating point rectangles are equal, within some
   * given epsilon.
   *
   * Rectangles are considered equal if both are not nullptr and each of their
   * x, y, width and height are within `epsilon` of each other. If you don't
   * know what value to use for `epsilon`, you should call the FRect.Equal
   * function instead.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param other the second rectangle to test.
   * @param epsilon the epsilon value for comparison.
   * @returns true if the rectangles are equal, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FRect.Equal
   */
  constexpr bool EqualEpsilon(const FRectRaw& other, const float epsilon) const
  {
    return SDL_RectsEqualEpsilon(this, &other, epsilon);
  }

  /**
   * Determine whether two rectangles are equal.
   *
   * Rectangles are considered equal if both are not nullptr and each of their
   * x, y, width and height are within SDL_FLT_EPSILON of each other. This is
   * often a reasonable way to compare two floating point rectangles and deal
   * with the slight precision variations in floating point calculations that
   * tend to pop up.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param other the second rectangle to test.
   * @returns true if the rectangles are equal, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FRect.EqualEpsilon
   */
  constexpr bool Equal(const FRectRaw& other) const
  {
    return SDL_RectsEqualFloat(this, &other);
  }

  /**
   * Check whether the rect contains given point
   *
   * @param p Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const FPointRaw& p) const
  {
    return SDL_PointInRectFloat(&p, this);
  }

  /**
   * Check whether the rect contains given point
   *
   * @param other Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const FRectRaw& other) const
  {
    return GetUnion(other) == *this;
  }

  /**
   * Determine whether two rectangles intersect.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns true if there is an intersection, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Rect.GetIntersection
   */
  constexpr bool HasIntersection(const FRectRaw& other) const
  {
    return SDL_HasRectIntersectionFloat(this, &other);
  }

  /**
   * Calculate the intersection of two rectangles with float precision.
   *
   * If `result` is nullptr then this function will return false.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns an SDL_Rect structure filled in with the intersection of
   *          if there is intersection, an empty FRect otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FRect.HasIntersection
   */
  constexpr FRect GetIntersection(const FRectRaw& other) const
  {
    if (FRect result; SDL_GetRectIntersectionFloat(this, &other, &result)) {
      return result;
    }
    return {};
  }

  /**
   * Calculate the union of two rectangles with float precision.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns Rect representing union of two rectangles
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr FRect GetUnion(const FRectRaw& other) const
  {
    FRect result;
    CheckError(SDL_GetRectUnionFloat(this, &other, &result));
    return result;
  }

  /**
   * Get a rect extended by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Extended rect
   *
   */
  constexpr FRect GetExtension(unsigned int amount) const
  {
    FRect r = *this;
    r.Extend(amount);
    return r;
  }

  /**
   * Get a rect extended by specified amount of pixels
   *
   * @param[in] hAmount Number of pixels to extend by
   *                    in horizontal direction
   * @param[in] vAmount Number of pixels to extend by
   *                    in vertical direction
   *
   * @returns Extended rect
   *
   */
  constexpr FRect GetExtension(float hAmount, float vAmount) const
  {
    FRect r = *this;
    r.Extend(hAmount, vAmount);
    return r;
  }

  /**
   * Extend a rect by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& Extend(float amount) { return Extend(amount, amount); }

  /**
   * Extend a rect by specified amount of pixels
   *
   * @param[in] hAmount Number of pixels to extend by
   *                    in horizontal direction
   * @param[in] vAmount Number of pixels to extend by
   *                    in vertical direction
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& Extend(float hAmount, float vAmount)
  {
    x -= hAmount;
    y -= vAmount;
    w += hAmount * 2;
    h += vAmount * 2;
    return *this;
  }

  /**
   * Get rectangle moved by a given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Moved rectangle
   *
   */
  constexpr FRect operator+(const FPoint& offset) const
  {
    return FRect(x + offset.x, y + offset.y, w, h);
  }

  /**
   * Get rectangle moved by an opposite of given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Moved rectangle
   *
   */
  constexpr FRect operator-(const FPoint& offset) const
  {
    return FRect(x - offset.x, y - offset.y, w, h);
  }

  /**
   * Move by then given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& operator+=(const FPoint& offset)
  {
    x += offset.x;
    y += offset.y;
    return *this;
  }

  /**
   * Move by an opposite of the given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& operator-=(const FPoint& offset)
  {
    x -= offset.x;
    y -= offset.y;
    return *this;
  }
};

/**
 * Convert an Rect to FRect
 *
 * @param rect a pointer to an Rect.
 * @param frect a pointer filled in with the floating point representation of
 *              `rect`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void RectToFRect(const RectRaw& rect, FRectRaw* frect)
{
  SDL_RectToFRect(&rect, frect);
}

/**
 * Determine whether a point resides inside a rectangle.
 *
 * A point is considered part of a rectangle if both `p` and `r` are not
 * nullptr, and `p`'s x and y coordinates are >= to the rectangle's top left
 * corner, and < the rectangle's x+w and y+h. So a 1x1 rectangle considers point
 * (0,0) as "inside" and (0,1) as not.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param p the point to test.
 * @param r the rectangle to test.
 * @returns true if `p` is contained by `r`, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool PointInRect(const PointRaw& p, const RectRaw& r)
{
  return SDL_PointInRect(&p, &r);
}

/**
 * Determine whether a rectangle has no area.
 *
 * A rectangle is considered "empty" for this function if `r` is nullptr, or if
 * `r`'s width and/or height are <= 0.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param r the rectangle to test.
 * @returns true if the rectangle is "empty", false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool RectEmpty(const RectRaw& r) { return SDL_RectEmpty(&r); }

/**
 * Determine whether two rectangles are equal.
 *
 * Rectangles are considered equal if both are not nullptr and each of their x,
 * y, width and height match.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param a the first rectangle to test.
 * @param b the second rectangle to test.
 * @returns true if the rectangles are equal, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool RectsEqual(const RectRaw& a, const RectRaw& b)
{
  return SDL_RectsEqual(&a, &b);
}

/**
 * Determine whether two rectangles intersect.
 *
 * If either pointer is nullptr the function will return false.
 *
 * @param A an Rect structure representing the first rectangle.
 * @param B an Rect structure representing the second rectangle.
 * @returns true if there is an intersection, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Rect.GetIntersection
 */
constexpr bool HasRectIntersection(const RectRaw& self, const RectRaw& other)
{
  return SDL_HasRectIntersection(&self, &other);
}

/**
 * Calculate the intersection of two rectangles.
 *
 * If `result` is nullptr then this function will return false.
 *
 * @param A an Rect structure representing the first rectangle.
 * @param B an Rect structure representing the second rectangle.
 * @param result an Rect structure filled in with the intersection of
 *               rectangles `A` and `B`.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Rect.HasIntersection
 */
constexpr Rect GetRectIntersection(const RectRaw& self, const RectRaw& other)
{
  if (Rect result; SDL_GetRectIntersection(&self, &other, &result)) {
    return result;
  }
  return {};
}

/**
 * Calculate the union of two rectangles.
 *
 * @param A an Rect structure representing the first rectangle.
 * @param B an Rect structure representing the second rectangle.
 * @param result an Rect structure filled in with the union of rectangles
 *               `A` and `B`.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Rect GetRectUnion(const RectRaw& self, const RectRaw& other)
{
  Rect r;
  CheckError(SDL_GetRectUnion(&self, &other, &r));
  return r;
}

/**
 * Calculate a minimal rectangle enclosing a set of points.
 *
 * If `clip` is not nullptr then only points inside of the clipping rectangle
 * are considered.
 *
 * @param points an array of Point structures representing points to be
 *               enclosed.
 * @param count the number of structures in the `points` array.
 * @param clip an Rect used for clipping or nullptr to enclose all points.
 * @param result an Rect structure filled in with the minimal enclosing
 *               rectangle.
 * @returns true if any points were enclosed or false if all the points were
 *          outside of the clipping rectangle.
 *
 * @since This function is available since SDL 3.2.0.
 */
static inline bool GetRectEnclosingPoints(SpanRef<const PointRaw> points,
                                          OptionalRef<const RectRaw> clip,
                                          RectRaw* result)
{
  return SDL_GetRectEnclosingPoints(points.data(), points.size(), clip, result);
}

/**
 * Calculate the intersection of a rectangle and line segment.
 *
 * This function is used to clip a line segment to a rectangle. A line segment
 * contained entirely within the rectangle or that does not intersect will
 * remain unchanged. A line segment that crosses the rectangle at either or
 * both ends will be clipped to the boundary of the rectangle and the new
 * coordinates saved in `X1`, `Y1`, `X2`, and/or `Y2` as necessary.
 *
 * @param rect an Rect structure representing the rectangle to intersect.
 * @param X1 a pointer to the starting X-coordinate of the line.
 * @param Y1 a pointer to the starting Y-coordinate of the line.
 * @param X2 a pointer to the ending X-coordinate of the line.
 * @param Y2 a pointer to the ending Y-coordinate of the line.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GetRectAndLineIntersection(const RectRaw& rect,
                                       int* X1,
                                       int* Y1,
                                       int* X2,
                                       int* Y2)
{
  return SDL_GetRectAndLineIntersection(&rect, X1, Y1, X2, Y2);
}

/**
 * Determine whether a point resides inside a floating point rectangle.
 *
 * A point is considered part of a rectangle if both `p` and `r` are not
 * nullptr, and `p`'s x and y coordinates are >= to the rectangle's top left
 * corner, and <= the rectangle's x+w and y+h. So a 1x1 rectangle considers
 * point (0,0) and (0,1) as "inside" and (0,2) as not.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param p the point to test.
 * @param r the rectangle to test.
 * @returns true if `p` is contained by `r`, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool PointInRectFloat(const FPointRaw& p, const FRectRaw& r)
{
  return SDL_PointInRectFloat(&p, &r);
}

/**
 * Determine whether a floating point rectangle can contain any point.
 *
 * A rectangle is considered "empty" for this function if `r` is nullptr, or if
 * `r`'s width and/or height are < 0.0f.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param r the rectangle to test.
 * @returns true if the rectangle is "empty", false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool RectEmptyFloat(const FRectRaw& r) { return SDL_RectEmptyFloat(&r); }

/**
 * Determine whether two floating point rectangles are equal, within some
 * given epsilon.
 *
 * Rectangles are considered equal if both are not nullptr and each of their x,
 * y, width and height are within `epsilon` of each other. If you don't know
 * what value to use for `epsilon`, you should call the FRect.Equal
 * function instead.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param a the first rectangle to test.
 * @param b the second rectangle to test.
 * @param epsilon the epsilon value for comparison.
 * @returns true if the rectangles are equal, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FRect.Equal
 */
inline bool RectsEqualEpsilon(const FRectRaw& a,
                              const FRectRaw& b,
                              float epsilon)
{
  return SDL_RectsEqualEpsilon(&a, &b, epsilon);
}

/**
 * Determine whether two floating point rectangles are equal, within a default
 * epsilon.
 *
 * Rectangles are considered equal if both are not nullptr and each of their x,
 * y, width and height are within SDL_FLT_EPSILON of each other. This is often
 * a reasonable way to compare two floating point rectangles and deal with the
 * slight precision variations in floating point calculations that tend to pop
 * up.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param a the first rectangle to test.
 * @param b the second rectangle to test.
 * @returns true if the rectangles are equal, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FRect.EqualEpsilon
 */
inline bool RectsEqualFloat(const FRectRaw& a, const FRectRaw& b)
{
  return SDL_RectsEqualFloat(&a, &b);
}

/**
 * Determine whether two rectangles intersect with float precision.
 *
 * If either pointer is nullptr the function will return false.
 *
 * @param A an FRect structure representing the first rectangle.
 * @param B an FRect structure representing the second rectangle.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Rect.GetIntersection
 */
constexpr bool HasRectIntersectionFloat(const FRectRaw& self,
                                        const FRectRaw& other)
{
  return SDL_HasRectIntersectionFloat(&self, &other);
}

/**
 * Calculate the intersection of two rectangles with float precision.
 *
 * If `result` is nullptr then this function will return false.
 *
 * @param A an FRect structure representing the first rectangle.
 * @param B an FRect structure representing the second rectangle.
 * @param result an FRect structure filled in with the intersection of
 *               rectangles `A` and `B`.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FRect.HasIntersection
 */
constexpr FRect GetRectIntersectionFloat(const FRectRaw& self,
                                         const FRectRaw& other)
{
  if (FRect r; SDL_GetRectIntersectionFloat(&self, &other, &r)) return r;
  return {};
}

/**
 * Calculate the union of two rectangles with float precision.
 *
 * @param A an FRect structure representing the first rectangle.
 * @param B an FRect structure representing the second rectangle.
 * @param result an FRect structure filled in with the union of rectangles
 *               `A` and `B`.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr FRect GetRectUnionFloat(const FRectRaw& self, const FRectRaw& other)
{
  FRect r;
  CheckError(SDL_GetRectUnionFloat(&self, &other, &r));
  return r;
}

/**
 * Calculate a minimal rectangle enclosing a set of points with float
 * precision.
 *
 * If `clip` is not nullptr then only points inside of the clipping rectangle
 * are considered.
 *
 * @param points an array of FPoint structures representing points to be
 *               enclosed.
 * @param count the number of structures in the `points` array.
 * @param clip an FRect used for clipping or nullptr to enclose all points.
 * @param result an FRect structure filled in with the minimal enclosing
 *               rectangle.
 * @returns true if any points were enclosed or false if all the points were
 *          outside of the clipping rectangle.
 *
 * @since This function is available since SDL 3.2.0.
 */
static inline bool GetRectEnclosingPointsFloat(SpanRef<const FPointRaw> points,
                                               OptionalRef<const FRectRaw> clip,
                                               FRectRaw* result)
{
  return SDL_GetRectEnclosingPointsFloat(
    points.data(), points.size(), clip, result);
}

/**
 * Calculate the intersection of a rectangle and line segment with float
 * precision.
 *
 * This function is used to clip a line segment to a rectangle. A line segment
 * contained entirely within the rectangle or that does not intersect will
 * remain unchanged. A line segment that crosses the rectangle at either or
 * both ends will be clipped to the boundary of the rectangle and the new
 * coordinates saved in `X1`, `Y1`, `X2`, and/or `Y2` as necessary.
 *
 * @param rect an FRect structure representing the rectangle to intersect.
 * @param X1 a pointer to the starting X-coordinate of the line.
 * @param Y1 a pointer to the starting Y-coordinate of the line.
 * @param X2 a pointer to the ending X-coordinate of the line.
 * @param Y2 a pointer to the ending Y-coordinate of the line.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GetRectAndLineIntersectionFloat(const FRectRaw& rect,
                                            float* X1,
                                            float* Y1,
                                            float* X2,
                                            float* Y2)
{
  return SDL_GetRectAndLineIntersectionFloat(&rect, X1, Y1, X2, Y2);
}

/// @}

constexpr Point::operator FPoint() const { return {float(x), float(y)}; }

constexpr FPoint Point::operator/(float value) const
{
  return FPoint(*this) / value;
}
constexpr FPoint Point::operator*(float value) const
{
  return FPoint(*this) * value;
}

constexpr Point Point::GetClamped(const Rect& rect) const
{
  Point p = *this;
  p.Clamp(rect);
  return p;
}

constexpr Point& Point::Clamp(const Rect& rect)
{
  if (x < rect.x) x = rect.x;
  if (x > rect.GetX2()) x = rect.GetX2();
  if (y < rect.y) y = rect.y;
  if (y > rect.GetY2()) y = rect.GetY2();
  return *this;
}

constexpr Point Point::GetWrapped(const Rect& rect) const
{
  Point p = *this;
  p.Wrap(rect);
  return p;
}

constexpr Point& Point::Wrap(const Rect& rect)
{
  if (x < rect.x)
    x = rect.x + rect.w - 1 - (rect.x - x + rect.w - 1) % rect.w;
  else if (x >= rect.x + rect.w)
    x = rect.x + (x - rect.x - rect.w) % rect.w;

  if (y < rect.y)
    y = rect.y + rect.h - 1 - (rect.y - y + rect.h - 1) % rect.h;
  else if (y >= rect.y + rect.h)
    y = rect.y + (y - rect.y - rect.h) % rect.h;

  return *this;
}

constexpr FPoint FPoint::GetClamped(const FRect& rect) const
{
  FPoint p = *this;
  p.Clamp(rect);
  return p;
}

constexpr FPoint& FPoint::Clamp(const FRect& rect)
{
  if (x < rect.x) x = rect.x;
  if (x > rect.GetX2()) x = rect.GetX2();
  if (y < rect.y) y = rect.y;
  if (y > rect.GetY2()) y = rect.GetY2();
  return *this;
}

constexpr FPoint FPoint::GetWrapped(const FRect& rect) const
{
  FPoint p = *this;
  p.Wrap(rect);
  return p;
}

constexpr FPoint& FPoint::Wrap(const FRect& rect)
{
  if (x < rect.x)
    x = rect.x + rect.w - 1 - SDL_fmod(rect.x - x + rect.w - 1, rect.w);
  else if (x >= rect.x + rect.w)
    x = rect.x + SDL_fmod(x - rect.x - rect.w, rect.w);

  if (y < rect.y)
    y = rect.y + rect.h - 1 - SDL_fmod(rect.y - y + rect.h - 1, rect.h);
  else if (y >= rect.y + rect.h)
    y = rect.y + SDL_fmod(y - rect.y - rect.h, rect.h);

  return *this;
}

constexpr Rect::operator FRect() const
{
  return {float(x), float(y), float(w), float(h)};
}

} // namespace SDL
#endif // SDL3PP_H_
