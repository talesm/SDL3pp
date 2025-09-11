// Amalgamated SDL3pp
#ifndef SDL3PP_H_
#define SDL3PP_H_

#include <atomic>
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
 * LogCategory.SetLogPriority() or with SDL_SetHint(SDL_HINT_LOGGING, ...), or
 * with the "SDL_LOGGING" environment variable. This variable is a comma
 * separated set of category=level tokens that define the default logging levels
 * for SDL applications.
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
  constexpr LogCategory(SDL_LogCategory category = SDL_LOG_CATEGORY_APPLICATION)
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
  constexpr auto operator<=>(SDL_LogCategory category) const
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

/**
 * A macro to retrieve the flags of an PixelFormat.
 *
 * This macro is generally not needed directly by an app, which should use
 * specific tests, like SDL_ISPIXELFORMAT_FOURCC, instead.
 *
 * @param format an PixelFormat to check.
 * @returns the flags of `format`.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_PIXELFLAG(format) (((format) >> 28) & 0x0F)

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
  constexpr PixelFormat(SDL_PixelFormat format = {})
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
  constexpr PixelFormat(SDL_PixelType type,
                        int order,
                        SDL_PackedLayout layout,
                        int bits,
                        int bytes)
    : m_format(SDL_PixelFormat(
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
  constexpr bool operator==(SDL_PixelFormat format) const
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
   * This is usually a value from the BitmapOrder, PackedOrder, or ArrayOrder
   * enumerations, depending on the format type.
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
#define SDL_ISPIXELFORMAT_PACKED(format)                                       \
  (!SDL_ISPIXELFORMAT_FOURCC(format) &&                                        \
   ((SDL_PIXELTYPE(format) == SDL_PIXELTYPE_PACKED8) ||                        \
    (SDL_PIXELTYPE(format) == SDL_PIXELTYPE_PACKED16) ||                       \
    (SDL_PIXELTYPE(format) == SDL_PIXELTYPE_PACKED32)))

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
   * @sa SDL_SetSurfacePalette
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
   * Set a range of colors in a palette.
   *
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
  void SetColors(SpanRef<const SDL_Color> colors, int firstcolor = 0)
  {
    CheckError(SDL_SetPaletteColors(
      m_resource, colors.data(), firstcolor, colors.size()));
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
 * @sa SDL_SetSurfacePalette
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
                             SpanRef<const SDL_Color> colors,
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
 * @sa SDL_MapSurfaceRGB
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
 * @sa SDL_MapSurfaceRGBA
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
  static Rect GetEnclosingPoints(
    SpanRef<const SDL_Point> points,
    OptionalRef<const SDL_Rect> clip = std::nullopt)
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
  constexpr std::optional<Rect> GetIntersection(const RectRaw& other) const
  {
    if (Rect result; SDL_GetRectIntersection(this, &other, &result)) {
      return result;
    }
    return std::nullopt;
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
    SpanRef<const SDL_FPoint> points,
    OptionalRef<const SDL_FRect> clip = std::nullopt)
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
inline bool RectEmpty(const RectRaw& r) { return SDL_RectEmpty(&r); }

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
inline bool HasRectIntersection(const RectRaw& A, const RectRaw& B)
{
  return SDL_HasRectIntersection(&A, &B);
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
inline bool GetRectIntersection(const RectRaw& A,
                                const RectRaw& B,
                                RectRaw* result)
{
  return SDL_GetRectIntersection(&A, &B, result);
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
inline void GetRectUnion(const RectRaw& A, const RectRaw& B, RectRaw* result)
{
  CheckError(SDL_GetRectUnion(&A, &B, result));
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
static inline bool GetRectEnclosingPoints(SpanRef<const SDL_Point> points,
                                          OptionalRef<const SDL_Rect> clip,
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
inline bool HasRectIntersectionFloat(const FRectRaw& A, const FRectRaw& B)
{
  return SDL_HasRectIntersectionFloat(&A, &B);
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
inline bool GetRectIntersectionFloat(const FRectRaw& A,
                                     const FRectRaw& B,
                                     FRectRaw* result)
{
  return SDL_GetRectIntersectionFloat(&A, &B, result);
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
inline void GetRectUnionFloat(const FRectRaw& A,
                              const FRectRaw& B,
                              FRectRaw* result)
{
  CheckError(SDL_GetRectUnionFloat(&A, &B, result));
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
static inline bool GetRectEnclosingPointsFloat(
  SpanRef<const SDL_FPoint> points,
  OptionalRef<const SDL_FRect> clip,
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
 * The file I/O (for example: SDL_IOFromFile) and threading (SDL_CreateThread)
 * subsystems are initialized by default. Message boxes
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

} // namespace SDL
#endif // SDL3PP_H_
