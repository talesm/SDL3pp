

// begin --- SDL3pp.h --- 

/**
 * Main include header for the SDL3pp library
 *
 * It is almost always best to include just this one header instead of
 * picking out individual headers included here. There are exceptions to
 * this rule--SDL3pp_main.h is special and not included here--but usually
 * letting SDL3pp.h include the kitchen sink for you is the correct approach.
 */
#ifndef SDL3PP_H_
#define SDL3PP_H_

#include <SDL3/SDL.h>
//
//
//
//
//

// begin --- SDL3pp_blendmode.h --- 

#ifndef SDL3PP_BLENDMODE_H_
#define SDL3PP_BLENDMODE_H_

#include <SDL3/SDL_blendmode.h>

namespace SDL {
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
 * @brief  A set of blend modes used in drawing operations.
 *
 * These predefined blend modes are supported everywhere.
 *
 * Additional values may be obtained from ComposeCustomBlendMode().
 */
using BlendMode = SDL_BlendMode;

/**
 * @brief The blend operation used when combining source and destination pixel
 * components.
 *
 */
using BlendOperation = SDL_BlendOperation;

/**
 * dst + src: supported by all renderers
 */
constexpr BlendOperation BLENDOPERATION_ADD = SDL_BLENDOPERATION_ADD;

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
 * @brief The normalized factor used to multiply pixel components.
 *
 * The blend factors are multiplied with the pixels from a drawing operation
 * (src) and the pixels from the render target (dst) before the blend
 * operation. The comma-separated factors listed above are always applied in
 * the component order red, green, blue, and alpha.
 *
 */
using BlendFactor = SDL_BlendFactor;

/**
 * 0, 0, 0, 0
 */
constexpr BlendFactor BLENDFACTOR_ZERO = SDL_BLENDFACTOR_ZERO;

/**
 * 1, 1, 1, 1
 */
constexpr BlendFactor BLENDFACTOR_ONE = SDL_BLENDFACTOR_ONE;

/**
 * srcR, srcG, srcB, srcA
 */
constexpr BlendFactor BLENDFACTOR_SRC_COLOR = SDL_BLENDFACTOR_SRC_COLOR;

/**
 * 1-srcR, 1-srcG, 1-srcB, 1-srcA
 */
constexpr BlendFactor BLENDFACTOR_ONE_MINUS_SRC_COLOR =
  SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR;

/**
 * srcA, srcA, srcA, srcA
 */
constexpr BlendFactor BLENDFACTOR_SRC_ALPHA = SDL_BLENDFACTOR_SRC_ALPHA;

/**
 * 1-srcA, 1-srcA, 1-srcA, 1-srcA
 */
constexpr BlendFactor BLENDFACTOR_ONE_MINUS_SRC_ALPHA =
  SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

/**
 * dstR, dstG, dstB, dstA
 */
constexpr BlendFactor BLENDFACTOR_DST_COLOR = SDL_BLENDFACTOR_DST_COLOR;

/**
 * 1-dstR, 1-dstG, 1-dstB, 1-dstA
 */
constexpr BlendFactor BLENDFACTOR_ONE_MINUS_DST_COLOR =
  SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR;

/**
 * dstA, dstA, dstA, dstA
 */
constexpr BlendFactor BLENDFACTOR_DST_ALPHA = SDL_BLENDFACTOR_DST_ALPHA;

/**
 * 1-dstA, 1-dstA, 1-dstA, 1-dstA
 */
constexpr BlendFactor BLENDFACTOR_ONE_MINUS_DST_ALPHA =
  SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA;

/**
 * @brief Compose a custom blend mode for renderers.
 *
 * The functions SDL_SetRenderDrawBlendMode and SDL_SetTextureBlendMode accept
 * the SDL_BlendMode returned by this function if the renderer supports it.
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
 * second, and third components of the SDL_BlendFactor, respectively. The
 * fourth component is not used.
 *
 * The alpha component is always multiplied with the fourth component of the
 * SDL_BlendFactor. The other components are not used in the alpha
 * calculation.
 *
 * Support for these blend modes varies for each renderer. To check if a
 * specific SDL_BlendMode is supported, create a renderer and pass it to
 * either SDL_SetRenderDrawBlendMode or SDL_SetTextureBlendMode. They will
 * return with an error if the blend mode is not supported.
 *
 * This list describes the support of custom blend modes for each renderer.
 * All renderers support the four blend modes listed in the SDL_BlendMode
 * enumeration.
 *
 * - **direct3d**: Supports all operations with all factors. However, some
 *   factors produce unexpected results with `SDL_BLENDOPERATION_MINIMUM` and
 *   `SDL_BLENDOPERATION_MAXIMUM`.
 * - **direct3d11**: Same as Direct3D 9.
 * - **opengl**: Supports the `SDL_BLENDOPERATION_ADD` operation with all
 *   factors. OpenGL versions 1.1, 1.2, and 1.3 do not work correctly here.
 * - **opengles2**: Supports the `SDL_BLENDOPERATION_ADD`,
 *   `SDL_BLENDOPERATION_SUBTRACT`, `SDL_BLENDOPERATION_REV_SUBTRACT`
 *   operations with all factors.
 * - **psp**: No custom blend mode support.
 * - **software**: No custom blend mode support.
 *
 * Some renderers do not provide an alpha component for the default render
 * target. The `SDL_BLENDFACTOR_DST_ALPHA` and
 * `SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA` factors do not have an effect in this
 * case.
 *
 * @param srcColorFactor the SDL_BlendFactor applied to the red, green, and
 *                       blue components of the source pixels.
 * @param dstColorFactor the SDL_BlendFactor applied to the red, green, and
 *                       blue components of the destination pixels.
 * @param colorOperation the SDL_BlendOperation used to combine the red,
 *                       green, and blue components of the source and
 *                       destination pixels.
 * @param srcAlphaFactor the SDL_BlendFactor applied to the alpha component of
 *                       the source pixels.
 * @param dstAlphaFactor the SDL_BlendFactor applied to the alpha component of
 *                       the destination pixels.
 * @param alphaOperation the SDL_BlendOperation used to combine the alpha
 *                       component of the source and destination pixels.
 * @returns an SDL_BlendMode that represents the chosen factors and
 *          operations.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
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
/** @} */

} // namespace SDL

#endif /* SDL3PP_BLENDMODE_H_ */


// end --- SDL3pp_blendmode.h --- 


//
//
//
//
//

// begin --- SDL3pp_error.h --- 

#ifndef SDL3PP_ERROR_H_
#define SDL3PP_ERROR_H_

#include <SDL3/SDL_error.h>

namespace SDL {

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
 * other apps to consume. These strings are set by calling SDL_SetError().
 *
 * A common usage pattern is to have a function that returns true for success
 * and false for failure, and do this when something fails:
 *
 * ```c
 * if (something_went_wrong) {
 *    return SetError("The thing broke in this specific way: %d", errcode);
 * }
 * ```
 *
 * It's also common to just return `false` in this case if the failing thing
 * is known to call SetError(), so errors simply propagate through.
 *
 * @{
 */

/**
 * @brief Retrieve a message about the last error that occurred on the current
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
 * @return a message with information about the specific error that occurred,
 *          or an empty string if there hasn't been an error message set since
 *          the last call to ClearError().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @sa ClearError()
 */
inline const char* GetError() { return SDL_GetError(); }

/**
 * @brief Clear any previous error message for this thread.
 *
 * @returns true.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @sa GetError()
 */
inline bool ClearError() { return SDL_ClearError(); }

/** @} */

} // namespace SDL

#endif /* SDL3PP_ERROR_H_ */


// end --- SDL3pp_error.h --- 



// begin --- SDL3pp_events.h --- 

#ifndef SDL3PP_EVENTS_H_
#define SDL3PP_EVENTS_H_

#include <chrono>
#include <SDL3/SDL_events.h>

// begin --- SDL3pp_callbackWrapper.h --- 

#ifndef SDL3PP_CALLBACK_WRAPPER_H_
#define SDL3PP_CALLBACK_WRAPPER_H_

#include <functional>
#include <map>
#include <SDL3/SDL_assert.h>

namespace SDL {

template<class F>
struct CallbackWrapper;

template<typename Result, typename... Args>
struct CallbackWrapper<Result(Args...)>
{
  CallbackWrapper() = delete;
  using FunctionType = std::function<Result(Args...)>;

  static void* Wrap(FunctionType cb)
  {
    // TODO Protect against concurrency
    auto id = NextId();
    Values().insert_or_assign(id, std::move(cb));
    return (void*)id;
  }

  static Result Call(void* handle, Args... args)
  {
    auto& f = at(handle);
    return f(args...);
  }
  static Result CallSuffixed(Args... args, void* handle)
  {
    auto& f = at(handle);
    return f(args...);
  }
  static Result CallOnce(void* handle, Args... args)
  {
    auto f = release(handle);
    return f(args...);
  }
  static Result CallOnceSuffixed(Args... args, void* handle)
  {
    auto& f = release(handle);
    return f(args...);
  }

  static bool contains(void* handle)
  {
    return Values().contains((size_t)handle);
  }

  static const FunctionType& at(void* handle)
  {
    return Values().at((size_t)(handle));
  }

  static FunctionType release(void* handle)
  {
    auto& values = Values();
    auto value = std::move(values.at((size_t)(handle)));
    Erase(handle);
    return value;
  }

  static bool Erase(void* handle) { return Values().erase((size_t)handle); }

  static size_t NextId()
  {
    static size_t lastId = 0;
    SDL_assert_paranoid(lastId < SDL_SIZE_MAX);
    // TODO Some strategy on the odd case we get to SIZE_MAX
    ++lastId;
    return lastId;
  }

  static std::map<size_t, FunctionType>& Values()
  {
    static std::map<size_t, FunctionType> values;
    return values;
  }
};

} // namespace SDL

#endif /* SDL3PP_CALLBACK_WRAPPER_H_ */


// end --- SDL3pp_callbackWrapper.h --- 



// begin --- SDL3pp_stdinc.h --- 

#ifndef SDL3PP_STDINC_H_
#define SDL3PP_STDINC_H_

#include <SDL3/SDL_stdinc.h>

// begin --- SDL3pp_freeWrapper.h --- 

#ifndef SDL3PP_FREE_WRAPPER_H_
#define SDL3PP_FREE_WRAPPER_H_

#include <memory>
#include <SDL3/SDL_stdinc.h>

namespace SDL {

/**
 * @brief Wraps around SDL alloced pointers to automatically free them
 *
 */
struct FreeWrapperDeleter
{
  void operator()(void* mem) const { SDL_free(mem); }
};

template<class T>
using FreeWrapper = std::unique_ptr<T, FreeWrapperDeleter>;

template<class T>
FreeWrapper<T[]> wrapArray(T* array)
{
  return FreeWrapper<T[]>{array};
}
} // namespace SDL

#endif /* SDL3PP_FREE_WRAPPER_H_ */


// end --- SDL3pp_freeWrapper.h --- 



// begin --- SDL3pp_objectWrapper.h --- 

#ifndef SDL3PP_OBJECT_WRAPPER_H_
#define SDL3PP_OBJECT_WRAPPER_H_

#include <concepts>
#include <memory>

namespace SDL {

template<class T, class POINTER>
concept ObjectBox = requires(const T a, T b) {
  { a.get() } -> std::convertible_to<POINTER>;
  { b.release() } -> std::convertible_to<POINTER>;
};

template<class T, class POINTER = T*>
class ObjectRef
{
  POINTER value;

public:
  using pointer = POINTER;

  constexpr ObjectRef(pointer value = nullptr)
    : value(value)
  {
  }

  template<ObjectBox<POINTER> BOX>
  ObjectRef(BOX&& box)
    : value(std::is_reference_v<BOX> ? box.get() : box.release())
  {
  }

  constexpr ObjectRef(std::nullptr_t)
    : value{0}
  {
  }

  constexpr pointer get() const { return value; }

  constexpr operator bool() const { return bool(get()); }

  const pointer operator->() const { return get(); }
  pointer operator->() { return get(); }
  const T& operator*() const { return *get(); }
  T& operator*() { return *get(); }

  pointer release()
  {
    pointer p;
    std::swap(value, p);
    return p;
  }
  void reset(pointer other = nullptr) { return std::swap(value, other); }
  void swap(ObjectRef& other) { return std::swap(value, other.value); }
};

template<class T>
struct ObjectDeleter;

template<class T, class DELETER = ObjectDeleter<T>>
class ObjectUnique
{
  std::unique_ptr<T, DELETER> value;

public:
  using pointer = std::unique_ptr<T, DELETER>::pointer;

  explicit ObjectUnique(pointer value = nullptr)
    : value(value)
  {
  }

  constexpr ObjectUnique(std::nullptr_t)
    : value{}
  {
  }

  pointer get() const { return value.get(); }
  operator bool() const { return bool(get()); }

  const pointer operator->() const { return get(); }
  pointer operator->() { return get(); }
  const T& operator*() const { return *value; }
  T& operator*() { return *value; }

  pointer release() { return value.release(); }
  void reset(pointer other = nullptr) { return value.reset(other); }
  void swap(ObjectUnique& other) { return std::swap(value, other.value); }
};

template<class T, T defaultValue = 0>
class FancyPointer
{
  T value;

public:
  constexpr FancyPointer(T value = defaultValue)
    : value(value)
  {
  }

  constexpr auto operator<=>(const FancyPointer& other) const = default;

  constexpr operator bool() const { return value != defaultValue; }

  bool operator==(nullptr_t) const { return bool(*this); }

  constexpr operator T() const { return value; }

  T operator*() const { return value; }
  T& operator*() { return value; }

  T* operator->() { return &value; }
};

} // namespace SDL

template<class T, class POINTER>
void std::swap(SDL::ObjectRef<T, POINTER>& left,
               SDL::ObjectRef<T, POINTER>& right)
{
  left.swap(right);
}

template<class T, class DELETER>
void std::swap(SDL::ObjectUnique<T, DELETER> left,
               SDL::ObjectUnique<T, DELETER>& right)
{
  left.swap(right);
}

#endif /* SDL3PP_OBJECT_WRAPPER_H_ */


// end --- SDL3pp_objectWrapper.h --- 



// begin --- SDL3pp_stringParam.h --- 

#ifndef SDL3PP_STRING_PARAM_H_
#define SDL3PP_STRING_PARAM_H_

#include <string>
#include <string_view>
#include <variant>

namespace SDL {

#ifndef SDLPP_ENABLE_STRING_PARAM

#ifndef SDLPP_DISABLE_STRING_PARAM
#define SDLPP_ENABLE_STRING_PARAM
#endif // SDLPP_DISABLE_STRING_PARAM

#endif // SDLPP_ENABLE_STRING_PARAM''

/**
 * @brief A safe and mostly efficient wrapper to std::string and
 * std::string_view parameters
 *
 * This should only be declared in [function
 * parameters](https://en.cppreference.com/w/cpp/language/expressions#Full-expressions),
 * using it otherwise is to ask for undefined behavior
 */
class StringParamImpl
{
  std::variant<std::monostate, const char*, std::string> data;

public:
  constexpr StringParamImpl(std::nullptr_t = nullptr) {}
  constexpr StringParamImpl(const char* str)
    : data(str)
  {
  }

  StringParamImpl(const std::string& str)
    : StringParamImpl(str.c_str())
  {
  }

  StringParamImpl(std::string&& str)
    : data(std::move(str))
  {
  }

  StringParamImpl(std::string_view str)
    : StringParamImpl(std::string{str})
  {
  }

  StringParamImpl(const StringParamImpl&) = delete;
  StringParamImpl(StringParamImpl&&) = default;
  StringParamImpl& operator=(const StringParamImpl&) = delete;
  StringParamImpl& operator=(StringParamImpl&&) = default;

  operator const char*() const
  {
    struct Visitor
    {
      const char* operator()(const char* a) const { return a; }
      const char* operator()(const std::string& s) const { return s.c_str(); }
      const char* operator()(std::monostate) const { return ""; }
    };
    return std::visit(Visitor{}, data);
  }
};
#ifdef SDLPP_ENABLE_STRING_PARAM
using StringParam = StringParamImpl;
#else  // SDLPP_ENABLE_STRING_PARAM
using StringParam = const char*;
#endif // SDLPP_ENABLE_STRING_PARAM
} // namespace SDL

#endif /* SDL3PP_STRING_PARAM_H_ */


// end --- SDL3pp_stringParam.h --- 



namespace SDL {

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
 * For many of the most common functions, like SDL_memcpy, SDL might just call
 * through to the usual C runtime behind the scenes, if it makes sense to do
 * so (if it's faster and always available/reliable on a given platform),
 * reducing library size and offering the most optimized option.
 *
 * SDL also offers other C-runtime-adjacent functionality in this header that
 * either isn't, strictly speaking, part of any C runtime standards, like
 * SDL_crc32() and SDL_reinterpret_cast, etc. It also offers a few better
 * options, like SDL_strlcpy(), which functions as a safer form of strcpy().
 *
 * @{
 */

// Forward decl
template<ObjectBox<SDL_Environment*> T>
struct EnvironmentBase;

using EnvironmentRef = EnvironmentBase<ObjectRef<SDL_Environment>>;

template<>
struct ObjectDeleter<SDL_Environment>
{
  inline void operator()(EnvironmentRef environment) const;
};

using Environment = EnvironmentBase<ObjectUnique<SDL_Environment>>;

// Forward decl
template<ObjectBox<SDL_iconv_t> T>
struct IConvBase;

using IConvRef = IConvBase<ObjectRef<SDL_iconv_data_t>>;

template<>
struct ObjectDeleter<SDL_iconv_data_t>
{
  inline void operator()(IConvRef iconv) const;
};

using IConv = IConvBase<ObjectUnique<SDL_iconv_data_t>>;

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
 * @since This macro is available since SDL 3.2.0.
 **/
constexpr Uint32 FourCC(Uint8 a, Uint8 b, Uint8 c, Uint8 d)
{
  return SDL_FOURCC(a, b, c, d);
}

/**
 * SDL times are signed, 64-bit integers representing nanoseconds since the
 * Unix epoch (Jan 1, 1970).
 *
 * They can be converted between POSIX time_t values with SDL_NS_TO_SECONDS()
 * and SDL_SECONDS_TO_NS(), and between Windows FILETIME values with
 * SDL_TimeToWindows() and SDL_TimeFromWindows().
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_MAX_SINT64
 * @sa SDL_MIN_SINT64
 **/
using Time = SDL_Time;

/**
 * Allocate uninitialized memory.
 *
 * The allocated memory returned by this function must be freed with
 * SDL_free().
 *
 * If `size` is 0, it will be set to 1.
 *
 * If you want to allocate memory aligned to a specific alignment, consider
 * using SDL_aligned_alloc().
 *
 * @param size the size to allocate.
 * @returns a pointer to the allocated memory, or NULL if allocation failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_free
 * @sa SDL_calloc
 * @sa SDL_realloc
 * @sa SDL_aligned_alloc
 **/
inline void* malloc(size_t size) { return SDL_malloc(size); }

/**
 * Allocate a zero-initialized array.
 *
 * The memory returned by this function must be freed with SDL_free().
 *
 * If either of `nmemb` or `size` is 0, they will both be set to 1.
 *
 * @param nmemb the number of elements in the array.
 * @param size the size of each element of the array.
 * @returns a pointer to the allocated array, or NULL if allocation failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_free
 * @sa SDL_malloc
 * @sa SDL_realloc
 **/
inline void* calloc(size_t nmemb, size_t size)
{
  return SDL_calloc(nmemb, size);
}

/**
 * Change the size of allocated memory.
 *
 * The memory returned by this function must be freed with SDL_free().
 *
 * If `size` is 0, it will be set to 1. Note that this is unlike some other C
 * runtime `realloc` implementations, which may treat `realloc(mem, 0)` the
 * same way as `free(mem)`.
 *
 * If `mem` is NULL, the behavior of this function is equivalent to
 * SDL_malloc(). Otherwise, the function can have one of three possible
 * outcomes:
 *
 * - If it returns the same pointer as `mem`, it means that `mem` was resized
 *   in place without freeing.
 * - If it returns a different non-NULL pointer, it means that `mem` was freed
 *   and cannot be dereferenced anymore.
 * - If it returns NULL (indicating failure), then `mem` will remain valid and
 *   must still be freed with SDL_free().
 *
 * @param mem a pointer to allocated memory to reallocate, or NULL.
 * @param size the new size of the memory.
 * @returns a pointer to the newly allocated memory, or NULL if allocation
 *          failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_free
 * @sa SDL_malloc
 * @sa SDL_calloc
 **/
inline void* realloc(void* mem, size_t size) { return SDL_realloc(mem, size); }

/**
 * Free allocated memory.
 *
 * The pointer is no longer valid after this call and cannot be dereferenced
 * anymore.
 *
 * If `mem` is NULL, this function does nothing.
 *
 * @param mem a pointer to allocated memory, or NULL.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_malloc
 * @sa SDL_calloc
 * @sa SDL_realloc
 **/
inline void free(void* mem) { SDL_free(mem); }

/**
 * A callback used to implement SDL_malloc().
 *
 * SDL will always ensure that the passed `size` is greater than 0.
 *
 * @param size the size to allocate.
 * @returns a pointer to the allocated memory, or NULL if allocation failed.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SDL_malloc
 * @sa SDL_GetOriginalMemoryFunctions
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_SetMemoryFunctions
 **/
using malloc_func = SDL_malloc_func;

/**
 * A callback used to implement SDL_calloc().
 *
 * SDL will always ensure that the passed `nmemb` and `size` are both greater
 * than 0.
 *
 * @param nmemb the number of elements in the array.
 * @param size the size of each element of the array.
 * @returns a pointer to the allocated array, or NULL if allocation failed.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SDL_calloc
 * @sa SDL_GetOriginalMemoryFunctions
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_SetMemoryFunctions
 **/
using calloc_func = SDL_calloc_func;

/**
 * A callback used to implement SDL_realloc().
 *
 * SDL will always ensure that the passed `size` is greater than 0.
 *
 * @param mem a pointer to allocated memory to reallocate, or NULL.
 * @param size the new size of the memory.
 * @returns a pointer to the newly allocated memory, or NULL if allocation
 *          failed.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SDL_realloc
 * @sa SDL_GetOriginalMemoryFunctions
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_SetMemoryFunctions
 **/
using realloc_func = SDL_realloc_func;

/**
 * A callback used to implement SDL_free().
 *
 * SDL will always ensure that the passed `mem` is a non-NULL pointer.
 *
 * @param mem a pointer to allocated memory.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SDL_free
 * @sa SDL_GetOriginalMemoryFunctions
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_SetMemoryFunctions
 **/
using free_func = SDL_free_func;

/**
 * Get the original set of SDL memory functions.
 *
 * This is what SDL_malloc and friends will use by default, if there has been
 * no call to SDL_SetMemoryFunctions. This is not necessarily using the C
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
 **/
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
 *               SDL_SetMemoryFunctions simultaneously.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_SetMemoryFunctions
 * @sa SDL_GetOriginalMemoryFunctions
 **/
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
 * as future calls to SDL_free will use the new allocator, even if they came
 * from an SDL_malloc made with the old one!
 *
 * If used, usually this needs to be the first call made into the SDL library,
 * if not the very first thing done at program startup time.
 *
 * @param malloc_func custom malloc function.
 * @param calloc_func custom calloc function.
 * @param realloc_func custom realloc function.
 * @param free_func custom free function.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread, but one
 *               should not replace the memory functions once any allocations
 *               are made!
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_GetOriginalMemoryFunctions
 **/
inline bool SetMemoryFunctions(malloc_func malloc_func,
                               calloc_func calloc_func,
                               realloc_func realloc_func,
                               free_func free_func)
{
  return SDL_SetMemoryFunctions(
    malloc_func, calloc_func, realloc_func, free_func);
}

/**
 * Allocate memory aligned to a specific alignment.
 *
 * The memory returned by this function must be freed with SDL_aligned_free(),
 * _not_ SDL_free().
 *
 * If `alignment` is less than the size of `void *`, it will be increased to
 * match that.
 *
 * The returned memory address will be a multiple of the alignment value, and
 * the size of the memory allocated will be a multiple of the alignment value.
 *
 * @param alignment the alignment of the memory.
 * @param size the size to allocate.
 * @returns a pointer to the aligned memory, or NULL if allocation failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_aligned_free
 **/
inline void* aligned_alloc(size_t alignment, size_t size)
{
  return SDL_aligned_alloc(alignment, size);
}

/**
 * Free memory allocated by SDL_aligned_alloc().
 *
 * The pointer is no longer valid after this call and cannot be dereferenced
 * anymore.
 *
 * If `mem` is NULL, this function does nothing.
 *
 * @param mem a pointer previously returned by SDL_aligned_alloc(), or NULL.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_aligned_alloc
 **/
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
 **/
inline int GetNumAllocations() { return SDL_GetNumAllocations(); }

/**
 * A thread-safe set of environment variables
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @ingroup resource
 **/
template<ObjectBox<SDL_Environment*> T>
struct EnvironmentBase : T
{
  using T::T;

  /**
   * Create a set of environment variables
   *
   * @param populated true to initialize it from the C runtime environment,
   *                  false to create an empty environment.
   * @post the new environment (convertible to true) on success or convertible
   * to false on failure; call GetError() for more information.
   *
   * @threadsafety If `populated` is false, it is safe to call this function
   *               from any thread, otherwise it is safe if no other threads are
   *               calling setenv() or unsetenv()
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetEnvironmentVariable
   * @sa SDL_GetEnvironmentVariables
   * @sa SDL_SetEnvironmentVariable
   * @sa SDL_UnsetEnvironmentVariable
   * @sa SDL_DestroyEnvironment
   **/
  inline EnvironmentBase(bool populated)
    : T(SDL_CreateEnvironment(populated))
  {
  }

  /**
   * Get the value of a variable in the environment.
   *
   * @param name the name of the variable to get.
   * @returns a pointer to the value of the variable or NULL if it can't be
   *          found.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetEnvironment
   * @sa SDL_CreateEnvironment
   * @sa SDL_GetEnvironmentVariables
   * @sa SDL_SetEnvironmentVariable
   * @sa SDL_UnsetEnvironmentVariable
   **/
  inline const char* GetVariable(StringParam name)
  {
    return SDL_GetEnvironmentVariable(T::get(), name);
  }

  /**
   * Get all variables in the environment.
   *
   * @returns a NULL terminated array of pointers to environment variables in
   *          the form "variable=value" or NULL on failure; call SDL_GetError()
   *          for more information. This is wrapped to be auto-deleted, use
   *          FreeWrapper.release() if you want to manage manually.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetEnvironment
   * @sa SDL_CreateEnvironment
   * @sa SDL_GetEnvironmentVariables
   * @sa SDL_SetEnvironmentVariable
   * @sa SDL_UnsetEnvironmentVariable
   **/
  inline FreeWrapper<char*[]> GetVariables()
  {
    return wrapArray(SDL_GetEnvironmentVariables(T::get()));
  }

  /**
   * @brief Get the Variables count
   *
   * @return Uint64
   */
  inline Uint64 GetVariableCount()
  {
    Uint64 count = 0;
    for (auto vars = GetVariables(); vars != nullptr; ++vars) count += 1;
    return count;
  }

  /**
   * Set the value of a variable in the environment.
   *
   * @param name the name of the variable to set.
   * @param value the value of the variable to set.
   * @param overwrite true to overwrite the variable if it exists, false to
   *                  return success without setting the variable if it already
   *                  exists.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetEnvironment
   * @sa SDL_CreateEnvironment
   * @sa SDL_GetEnvironmentVariable
   * @sa SDL_GetEnvironmentVariables
   * @sa SDL_UnsetEnvironmentVariable
   **/
  inline bool SetVariable(StringParam name, StringParam value, bool overwrite)
  {
    return SDL_SetEnvironmentVariable(T::get(), name, value, overwrite);
  }

  /**
   * Clear a variable from the environment.
   *
   * @param name the name of the variable to unset.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetEnvironment()
   * @sa EnvironmentBase()
   * @sa GetVariable()
   * @sa GetVariables()
   * @sa SetVariable()
   * @sa UnsetVariable()
   **/
  inline bool UnsetVariable(StringParam name)
  {
    return SDL_UnsetEnvironmentVariable(T::get(), name);
  }

  /**
   * Destroy a set of environment variables.
   *
   * This object becomes empty after the call.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the environment is no longer in use.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CreateEnvironment
   */
  void Destroy() { return SDL_DestroyEnvironment(T::release()); }
};
/**
 * Get the process environment.
 *
 * This is initialized at application start and is not affected by setenv()
 * and unsetenv() calls after that point. Use SDL_SetEnvironmentVariable() and
 * SDL_UnsetEnvironmentVariable() if you want to modify this environment, or
 * SDL_setenv_unsafe() or SDL_unsetenv_unsafe() if you want changes to persist
 * in the C runtime environment after SDL_Quit().
 *
 * @returns a pointer to the environment for the process or NULL on failure;
 *          call SDL_GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetVariable()
 * @sa GetVariables()
 * @sa SetVariable()
 * @sa UnsetVariable()
 **/
inline EnvironmentRef GetEnvironment() { return SDL_GetEnvironment(); }

/**
 * Get the value of a variable in the environment.
 *
 * This function uses SDL's cached copy of the environment and is thread-safe.
 *
 * @param name the name of the variable to get.
 * @returns a pointer to the value of the variable or NULL if it can't be
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline const char* getenv(StringParam name) { return SDL_getenv(name); }

/**
 * Get the value of a variable in the environment.
 *
 * This function bypasses SDL's cached copy of the environment and is not
 * thread-safe.
 *
 * @param name the name of the variable to get.
 * @returns a pointer to the value of the variable or NULL if it can't be
 *          found.
 *
 * @threadsafety This function is not thread safe, consider using SDL_getenv()
 *               instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_getenv
 **/
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
 *               SDL_SetEnvironmentVariable() instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_SetEnvironmentVariable
 **/
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
 *               SDL_UnsetEnvironmentVariable() instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_UnsetEnvironmentVariable
 **/
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
 * @sa SDL_bsearch
 * @sa SDL_qsort
 **/
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
 * SDL_qsort(values, SDL_arraysize(values), sizeof(values[0]), compare);
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
 * @sa SDL_bsearch
 * @sa SDL_qsort_r
 **/
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
 * data key = { 2, NULL };
 *
 * data *result = SDL_bsearch(&key, values, SDL_arraysize(values),
 *sizeof(values[0]), compare);
 * ```
 *
 * @param key a pointer to a key equal to the element being searched for.
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 * @returns a pointer to the matching element in the array, or NULL if not
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_bsearch_r
 * @sa SDL_qsort
 **/
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
 * @sa SDL_qsort_r
 * @sa SDL_bsearch_r
 **/
using CompareCallback_r = SDL_CompareCallback_r;

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
 * SDL_qsort_r(values, SDL_arraysize(values), sizeof(values[0]), compare, (const
 *void *)(uintptr_t)sort_increasing);
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
 * @sa SDL_bsearch_r
 * @sa SDL_qsort
 **/
inline void qsort_r(void* base,
                    size_t nmemb,
                    size_t size,
                    CompareCallback_r compare,
                    void* userdata)
{
  SDL_qsort_r(base, nmemb, size, compare, userdata);
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
 * data key = { 2, NULL };
 *
 * data *result = SDL_bsearch_r(&key, values, SDL_arraysize(values),
 *sizeof(values[0]), compare, (const void *)(uintptr_t)sort_increasing);
 * ```
 *
 * @param key a pointer to a key equal to the element being searched for.
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 * @param userdata a pointer to pass to the compare function.
 * @returns a pointer to the matching element in the array, or NULL if not
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_bsearch
 * @sa SDL_qsort_r
 **/
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
 * Compute the absolute value of `x`.
 *
 * @param x an integer value.
 * @returns the absolute value of x.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int abs(int x) { return SDL_abs(x); }

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
 **/
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
 **/
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
 **/
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
 **/
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
 **/
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
 **/
inline int isxdigit(int x) { return SDL_isxdigit(x); }

/**
 * Report if a character is a punctuation mark.
 *
 * **WARNING**: Regardless of system locale, this is equivalent to
 * `((SDL_isgraph(x)) && (!SDL_isalnum(x)))`.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_isgraph
 * @sa SDL_isalnum
 **/
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
 **/
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
 **/
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
 **/
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
 **/
inline int isprint(int x) { return SDL_isprint(x); }

/**
 * Report if a character is any "printable" except space.
 *
 * Be advised that "printable" has a definition that goes back to text
 * terminals from the dawn of computing, making this a sort of special case
 * function that is not suitable for Unicode (or most any) text management.
 *
 * **WARNING**: Regardless of system locale, this is equivalent to
 * `(SDL_isprint(x)) && ((x) != ' ')`.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_isprint
 **/
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
 **/
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
 **/
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
 **/
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
 **/
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
 * this does not work like SDL_crc16 and SDL_crc32: you can't feed a previous
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
 **/
inline Uint32 murmur3_32(const void* data, size_t len, Uint32 seed)
{
  return SDL_murmur3_32(data, len, seed);
}

/**
 * Copy non-overlapping memory.
 *
 * The memory regions must not overlap. If they do, use SDL_memmove() instead.
 *
 * @param dst The destination memory region. Must not be NULL, and must not
 *            overlap with `src`.
 * @param src The source memory region. Must not be NULL, and must not overlap
 *            with `dst`.
 * @param len The length in bytes of both `dst` and `src`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_memmove
 **/
inline void* memcpy(void* dst, const void* src, size_t len)
{
  return SDL_memcpy(dst, src, len);
}

/**
 * Copy memory ranges that might overlap.
 *
 * It is okay for the memory regions to overlap. If you are confident that the
 * regions never overlap, using SDL_memcpy() may improve performance.
 *
 * @param dst The destination memory region. Must not be NULL.
 * @param src The source memory region. Must not be NULL.
 * @param len The length in bytes of both `dst` and `src`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_memcpy
 **/
inline void* memmove(void* dst, const void* src, size_t len)
{
  return SDL_memmove(dst, src, len);
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
 * @param dst the destination memory region. Must not be NULL.
 * @param c the byte value to set.
 * @param len the length, in bytes, to set in `dst`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline void* memset(void* dst, int c, size_t len)
{
  return SDL_memset(dst, c, len);
}

/**
 * Initialize all 32-bit words of buffer of memory to a specific value.
 *
 * This function will set a buffer of `dwords` Uint32 values, pointed to by
 * `dst`, to the value specified in `val`.
 *
 * Unlike SDL_memset, this sets 32-bit values, not bytes, so it's not limited
 * to a range of 0-255.
 *
 * @param dst the destination memory region. Must not be NULL.
 * @param val the Uint32 value to set.
 * @param dwords the number of Uint32 values to set in `dst`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline void* memset4(void* dst, Uint32 val, size_t dwords)
{
  return SDL_memset4(dst, val, dwords);
}

/**
 * Compare two buffers of memory.
 *
 * @param s1 the first buffer to compare. NULL is not permitted!
 * @param s2 the second buffer to compare. NULL is not permitted!
 * @param len the number of bytes to compare between the buffers.
 * @returns less than zero if s1 is "less than" s2, greater than zero if s1 is
 *          "greater than" s2, and zero if the buffers match exactly for `len`
 *          bytes.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * Like SDL_strlen only counts bytes and not codepoints in a UTF-8 string,
 * this counts wchar_t values in a string, even if the string's encoding is of
 * variable width, like UTF-16.
 *
 * Also be aware that wchar_t is different sizes on different platforms (4
 * bytes on Linux, 2 on Windows, etc).
 *
 * @param wstr The null-terminated wide string to read. Must not be NULL.
 * @returns the length (in wchar_t values, excluding the null terminator) of
 *          `wstr`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_wcsnlen
 * @sa SDL_utf8strlen
 * @sa SDL_utf8strnlen
 **/
inline size_t wcslen(const wchar_t* wstr) { return SDL_wcslen(wstr); }

/**
 * This works exactly like wcsnlen() but doesn't require access to a C
 * runtime.
 *
 * Counts up to a maximum of `maxlen` wchar_t values in `wstr`, excluding the
 * null terminator.
 *
 * Like SDL_strnlen only counts bytes and not codepoints in a UTF-8 string,
 * this counts wchar_t values in a string, even if the string's encoding is of
 * variable width, like UTF-16.
 *
 * Also be aware that wchar_t is different sizes on different platforms (4
 * bytes on Linux, 2 on Windows, etc).
 *
 * Also, `maxlen` is a count of wide characters, not bytes!
 *
 * @param wstr The null-terminated wide string to read. Must not be NULL.
 * @param maxlen The maximum amount of wide characters to count.
 * @returns the length (in wide characters, excluding the null terminator) of
 *          `wstr` but never more than `maxlen`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_wcslen
 * @sa SDL_utf8strlen
 * @sa SDL_utf8strnlen
 **/
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
 * @param dst The destination buffer. Must not be NULL, and must not overlap
 *            with `src`.
 * @param src The null-terminated wide string to copy. Must not be NULL, and
 *            must not overlap with `dst`.
 * @param maxlen The length (in wide characters) of the destination buffer.
 * @returns the length (in wide characters, excluding the null terminator) of
 *          `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_wcslcat
 **/
inline size_t wcslcpy(wchar_t* dst, const wchar_t* src, size_t maxlen)
{
  return SDL_wcslcpy(dst, src, maxlen);
}

/**
 * Concatenate wide strings.
 *
 * This function appends up to `maxlen` - SDL_wcslen(dst) - 1 wide characters
 * from `src` to the end of the wide string in `dst`, then appends a null
 * terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * If `maxlen` - SDL_wcslen(dst) - 1 is less than or equal to 0, then `dst` is
 * unmodified.
 *
 * @param dst The destination buffer already containing the first
 *            null-terminated wide string. Must not be NULL and must not
 *            overlap with `src`.
 * @param src The second null-terminated wide string. Must not be NULL, and
 *            must not overlap with `dst`.
 * @param maxlen The length (in wide characters) of the destination buffer.
 * @returns the length (in wide characters, excluding the null terminator) of
 *          the string in `dst` plus the length of `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_wcslcpy
 **/
inline size_t wcslcat(wchar_t* dst, const wchar_t* src, size_t maxlen)
{
  return SDL_wcslcat(dst, src, maxlen);
}

/**
 * Allocate a copy of a wide string.
 *
 * This allocates enough space for a null-terminated copy of `wstr`, using
 * SDL_malloc, and then makes a copy of the string into this space.
 *
 * The returned string is owned by the caller, and should be passed to
 * SDL_free when no longer needed.
 *
 * @param wstr the string to copy.
 * @returns a pointer to the newly-allocated wide string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param haystack the wide string to search. Must not be NULL.
 * @param needle the wide string to search for. Must not be NULL.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param haystack the wide string to search. Must not be NULL.
 * @param needle the wide string to search for. Must not be NULL.
 * @param maxlen the maximum number of wide characters to search in
 *               `haystack`.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @param maxlen the maximum number of wchar_t to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @param maxlen the maximum number of wchar_t values to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param str The null-terminated wide string to read. Must not be NULL.
 * @param endp If not NULL, the address of the first invalid wide character
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
 * @sa SDL_strtol
 **/
inline long wcstol(const wchar_t* str, wchar_t** endp, int base)
{
  return SDL_wcstol(str, endp, base);
}

/**
 * This works exactly like strlen() but doesn't require access to a C runtime.
 *
 * Counts the bytes in `str`, excluding the null terminator.
 *
 * If you need the length of a UTF-8 string, consider using SDL_utf8strlen().
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @returns the length (in bytes, excluding the null terminator) of `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strnlen
 * @sa SDL_utf8strlen
 * @sa SDL_utf8strnlen
 **/
inline size_t strlen(StringParam str) { return SDL_strlen(str); }

/**
 * This works exactly like strnlen() but doesn't require access to a C
 * runtime.
 *
 * Counts up to a maximum of `maxlen` bytes in `str`, excluding the null
 * terminator.
 *
 * If you need the length of a UTF-8 string, consider using SDL_utf8strnlen().
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @param maxlen The maximum amount of bytes to count.
 * @returns the length (in bytes, excluding the null terminator) of `src` but
 *          never more than `maxlen`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlen
 * @sa SDL_utf8strlen
 * @sa SDL_utf8strnlen
 **/
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
 * sequences are not truncated, consider using SDL_utf8strlcpy().
 *
 * @param dst The destination buffer. Must not be NULL, and must not overlap
 *            with `src`.
 * @param src The null-terminated string to copy. Must not be NULL, and must
 *            not overlap with `dst`.
 * @param maxlen The length (in characters) of the destination buffer.
 * @returns the length (in characters, excluding the null terminator) of
 *          `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlcat
 * @sa SDL_utf8strlcpy
 **/
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
 * Note that unlike SDL_strlcpy(), this function returns the number of bytes
 * written, not the length of `src`.
 *
 * @param dst The destination buffer. Must not be NULL, and must not overlap
 *            with `src`.
 * @param src The null-terminated UTF-8 string to copy. Must not be NULL, and
 *            must not overlap with `dst`.
 * @param dst_bytes The length (in bytes) of the destination buffer. Must not
 *                  be 0.
 * @returns the number of bytes written, excluding the null terminator.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlcpy
 **/
inline size_t utf8strlcpy(char* dst, StringParam src, size_t dst_bytes)
{
  return SDL_utf8strlcpy(dst, src, dst_bytes);
}

/**
 * Concatenate strings.
 *
 * This function appends up to `maxlen` - SDL_strlen(dst) - 1 characters from
 * `src` to the end of the string in `dst`, then appends a null terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * If `maxlen` - SDL_strlen(dst) - 1 is less than or equal to 0, then `dst` is
 * unmodified.
 *
 * @param dst The destination buffer already containing the first
 *            null-terminated string. Must not be NULL and must not overlap
 *            with `src`.
 * @param src The second null-terminated string. Must not be NULL, and must
 *            not overlap with `dst`.
 * @param maxlen The length (in characters) of the destination buffer.
 * @returns the length (in characters, excluding the null terminator) of the
 *          string in `dst` plus the length of `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlcpy
 **/
inline size_t strlcat(char* dst, StringParam src, size_t maxlen)
{
  return SDL_strlcat(dst, src, maxlen);
}

/**
 * Allocate a copy of a string.
 *
 * This allocates enough space for a null-terminated copy of `str`, using
 * SDL_malloc, and then makes a copy of the string into this space.
 *
 * The returned string is owned by the caller, and should be passed to
 * SDL_free when no longer needed.
 *
 * @param str the string to copy.
 * @returns a pointer to the newly-allocated string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strdup(StringParam str) { return SDL_strdup(str); }

/**
 * Allocate a copy of a string, up to n characters.
 *
 * This allocates enough space for a null-terminated copy of `str`, up to
 * `maxlen` bytes, using SDL_malloc, and then makes a copy of the string into
 * this space.
 *
 * If the string is longer than `maxlen` bytes, the returned string will be
 * `maxlen` bytes long, plus a null-terminator character that isn't included
 * in the count.
 *
 * The returned string is owned by the caller, and should be passed to
 * SDL_free when no longer needed.
 *
 * @param str the string to copy.
 * @param maxlen the maximum length of the copied string, not counting the
 *               null-terminator character.
 * @returns a pointer to the newly-allocated string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 **/
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
 * @param str the string to convert in-place. Can not be NULL.
 * @returns the `str` pointer passed into this function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlwr
 **/
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
 * @param str the string to convert in-place. Can not be NULL.
 * @returns the `str` pointer passed into this function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strupr
 **/
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
 * @param str the string to search. Must not be NULL.
 * @param c the byte value to search for.
 * @returns a pointer to the first instance of `c` in the string, or NULL if
 *          not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strchr(StringParam str, int c) { return SDL_strchr(str, c); }

/**
 * Search a string for the last instance of a specific byte.
 *
 * The search must go until it finds a null terminator byte to end the string.
 *
 * Note that this looks for _bytes_, not _characters_, so you cannot match
 * against a Unicode codepoint > 255, regardless of character encoding.
 *
 * @param str the string to search. Must not be NULL.
 * @param c the byte value to search for.
 * @returns a pointer to the last instance of `c` in the string, or NULL if
 *          not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param haystack the string to search. Must not be NULL.
 * @param needle the string to search for. Must not be NULL.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param haystack the string to search. Must not be NULL.
 * @param needle the string to search for. Must not be NULL.
 * @param maxlen the maximum number of bytes to search in `haystack`.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param haystack the string to search. Must not be NULL.
 * @param needle the string to search for. Must not be NULL.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * To start tokenizing a new string, `str` should be the non-NULL address of
 * the string to start tokenizing. Future calls to get the next token from the
 * same string should specify a NULL.
 *
 * Note that this function will overwrite pieces of `str` with null chars to
 * split it into tokens. This function cannot be used with const/read-only
 * strings!
 *
 * `saveptr` just needs to point to a `char *` that can be overwritten; SDL
 * will use this to save tokenizing state between calls. It is initialized if
 * `str` is non-NULL, and used to resume tokenizing when `str` is NULL.
 *
 * @param str the string to tokenize, or NULL to continue tokenizing.
 * @param delim the delimiter string that separates tokens.
 * @param saveptr pointer to a char *, used for ongoing state.
 * @returns A pointer to the next token, or NULL if no tokens remain.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * SDL_strlen().
 *
 * Since this handles Unicode, it expects the strings to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), so a malformed or incomplete UTF-8 sequence might increase the
 * count by several replacement characters.
 *
 * @param str The null-terminated UTF-8 string to read. Must not be NULL.
 * @returns The length (in codepoints, excluding the null terminator) of
 *          `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_utf8strnlen
 * @sa SDL_strlen
 **/
inline size_t utf8strlen(StringParam str) { return SDL_utf8strlen(str); }

/**
 * Count the number of codepoints in a UTF-8 string, up to n bytes.
 *
 * Counts the _codepoints_, not _bytes_, in `str`, excluding the null
 * terminator.
 *
 * If you need to count the bytes in a string instead, consider using
 * SDL_strnlen().
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
 * @param str The null-terminated UTF-8 string to read. Must not be NULL.
 * @param bytes The maximum amount of bytes to count.
 * @returns The length (in codepoints, excluding the null terminator) of `src`
 *          but never more than `maxlen`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_utf8strlen
 * @sa SDL_strnlen
 **/
inline size_t utf8strnlen(StringParam str, size_t bytes)
{
  return SDL_utf8strnlen(str, bytes);
}

/**
 * Convert an integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
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
 * @sa SDL_uitoa
 * @sa SDL_ltoa
 * @sa SDL_lltoa
 **/
inline char* itoa(int value, char* str, int radix)
{
  return SDL_itoa(value, str, radix);
}

/**
 * Convert an unsigned integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
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
 * @sa SDL_itoa
 * @sa SDL_ultoa
 * @sa SDL_ulltoa
 **/
inline char* uitoa(unsigned int value, char* str, int radix)
{
  return SDL_uitoa(value, str, radix);
}

/**
 * Convert a long integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
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
 * @sa SDL_ultoa
 * @sa SDL_itoa
 * @sa SDL_lltoa
 **/
inline char* ltoa(long value, char* str, int radix)
{
  return SDL_ltoa(value, str, radix);
}

/**
 * Convert an unsigned long integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
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
 * @sa SDL_ltoa
 * @sa SDL_uitoa
 * @sa SDL_ulltoa
 **/
inline char* ultoa(unsigned long value, char* str, int radix)
{
  return SDL_ultoa(value, str, radix);
}

/**
 * Convert a long long integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget possible negative
 * signs, null terminator bytes, etc).
 *
 * @param value the long long integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_ulltoa
 * @sa SDL_itoa
 * @sa SDL_ltoa
 **/
inline char* lltoa(long long value, char* str, int radix)
{
  return SDL_lltoa(value, str, radix);
}

/**
 * Convert an unsigned long long integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget null terminator
 * bytes, etc).
 *
 * @param value the unsigned long long integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_lltoa
 * @sa SDL_uitoa
 * @sa SDL_ultoa
 **/
inline char* ulltoa(unsigned long long value, char* str, int radix)
{
  return SDL_ulltoa(value, str, radix);
}

/**
 * Parse an `int` from a string.
 *
 * The result of calling `SDL_atoi(str)` is equivalent to
 * `(int)SDL_strtol(str, NULL, 10)`.
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @returns the parsed `int`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atof
 * @sa SDL_strtol
 * @sa SDL_strtoul
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa SDL_strtod
 * @sa SDL_itoa
 **/
inline int atoi(StringParam str) { return SDL_atoi(str); }

/**
 * Parse a `double` from a string.
 *
 * The result of calling `SDL_atof(str)` is equivalent to `SDL_strtod(str,
 * NULL)`.
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @returns the parsed `double`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atoi
 * @sa SDL_strtol
 * @sa SDL_strtoul
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa SDL_strtod
 **/
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
 * @param str The null-terminated string to read. Must not be NULL.
 * @param endp If not NULL, the address of the first invalid character (i.e.
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
 * @sa SDL_atoi
 * @sa SDL_atof
 * @sa SDL_strtoul
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa SDL_strtod
 * @sa SDL_ltoa
 * @sa SDL_wcstol
 **/
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
 * @param str The null-terminated string to read. Must not be NULL.
 * @param endp If not NULL, the address of the first invalid character (i.e.
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
 * @sa SDL_atoi
 * @sa SDL_atof
 * @sa SDL_strtol
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa SDL_strtod
 * @sa SDL_ultoa
 **/
inline unsigned long strtoul(StringParam str, char** endp, int base)
{
  return SDL_strtoul(str, endp, base);
}

/**
 * Parse a `long long` from a string.
 *
 * If `str` starts with whitespace, then those whitespace characters are
 * skipped before attempting to parse the number.
 *
 * If the parsed number does not fit inside a `long long`, the result is
 * clamped to the minimum and maximum representable `long long` values.
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @param endp If not NULL, the address of the first invalid character (i.e.
 *             the next character after the parsed number) will be written to
 *             this pointer.
 * @param base The base of the integer to read. Supported values are 0 and 2
 *             to 36 inclusive. If 0, the base will be inferred from the
 *             number's prefix (0x for hexadecimal, 0 for octal, decimal
 *             otherwise).
 * @returns the parsed `long long`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atoi
 * @sa SDL_atof
 * @sa SDL_strtol
 * @sa SDL_strtoul
 * @sa SDL_strtoull
 * @sa SDL_strtod
 * @sa SDL_lltoa
 **/
inline long long strtoll(StringParam str, char** endp, int base)
{
  return SDL_strtoll(str, endp, base);
}

/**
 * Parse an `unsigned long long` from a string.
 *
 * If `str` starts with whitespace, then those whitespace characters are
 * skipped before attempting to parse the number.
 *
 * If the parsed number does not fit inside an `unsigned long long`, the
 * result is clamped to the maximum representable `unsigned long long` value.
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @param endp If not NULL, the address of the first invalid character (i.e.
 *             the next character after the parsed number) will be written to
 *             this pointer.
 * @param base The base of the integer to read. Supported values are 0 and 2
 *             to 36 inclusive. If 0, the base will be inferred from the
 *             number's prefix (0x for hexadecimal, 0 for octal, decimal
 *             otherwise).
 * @returns the parsed `unsigned long long`, or 0 if no number could be
 *          parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atoi
 * @sa SDL_atof
 * @sa SDL_strtol
 * @sa SDL_strtoll
 * @sa SDL_strtoul
 * @sa SDL_strtod
 * @sa SDL_ulltoa
 **/
inline unsigned long long strtoull(StringParam str, char** endp, int base)
{
  return SDL_strtoull(str, endp, base);
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
 * @param str the null-terminated string to read. Must not be NULL.
 * @param endp if not NULL, the address of the first invalid character (i.e.
 *             the next character after the parsed number) will be written to
 *             this pointer.
 * @returns the parsed `double`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atoi
 * @sa SDL_atof
 * @sa SDL_strtol
 * @sa SDL_strtoll
 * @sa SDL_strtoul
 * @sa SDL_strtoull
 **/
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
 * used with SDL_qsort() to put strings in (roughly) alphabetical order.
 *
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * used with SDL_qsort() to put strings in (roughly) alphabetical order.
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
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @param maxlen the maximum number of _bytes_ to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @param maxlen the maximum number of bytes to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int strncasecmp(StringParam str1, StringParam str2, size_t maxlen)
{
  return SDL_strncasecmp(str1, str2, maxlen);
}

/**
 * Searches a string for the first occurence of any character contained in a
 * breakset, and returns a pointer from the string to that character.
 *
 * @param str The null-terminated string to be searched. Must not be NULL, and
 *            must not overlap with `breakset`.
 * @param breakset A null-terminated string containing the list of characters
 *                 to look for. Must not be NULL, and must not overlap with
 *                 `str`.
 * @returns A pointer to the location, in str, of the first occurence of a
 *          character present in the breakset, or NULL if none is found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strpbrk(StringParam str, StringParam breakset)
{
  return SDL_strpbrk(str, breakset);
}

/**
 * Decode a UTF-8 string, one Unicode codepoint at a time.
 *
 * This will return the first Unicode codepoint in the UTF-8 encoded string in
 * `*pstr`, and then advance `*pstr` past any consumed bytes before returning.
 *
 * It will not access more than `*pslen` bytes from the string. `*pslen` will
 * be adjusted, as well, subtracting the number of bytes consumed.
 *
 * `pslen` is allowed to be NULL, in which case the string _must_ be
 * NULL-terminated, as the function will blindly read until it sees the NULL
 * char.
 *
 * if `*pslen` is zero, it assumes the end of string is reached and returns a
 * zero codepoint regardless of the contents of the string buffer.
 *
 * If the resulting codepoint is zero (a NULL terminator), or `*pslen` is
 * zero, it will not advance `*pstr` or `*pslen` at all.
 *
 * Generally this function is called in a loop until it returns zero,
 * adjusting its parameters each iteration.
 *
 * If an invalid UTF-8 sequence is encountered, this function returns
 * SDL_INVALID_UNICODE_CODEPOINT and advances the string/length by one byte
 * (which is to say, a multibyte sequence might produce several
 * SDL_INVALID_UNICODE_CODEPOINT returns before it syncs to the next valid
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
 *              adjusted. NULL is allowed.
 * @returns the first Unicode codepoint in the string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 * SDL_INVALID_UNICODE_CODEPOINT.
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
 **/
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
 * knowing where to write a NULL-terminator character to end the string (in
 * either case, plan to have a buffer of _more_ than 4 bytes!).
 *
 * If `codepoint` is an invalid value (outside the Unicode range, or a UTF-16
 * surrogate value, etc), this will use U+FFFD (REPLACEMENT CHARACTER) for the
 * codepoint instead, and not set an error.
 *
 * If `dst` is NULL, this returns NULL immediately without writing to the
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
 **/
inline char* UCS4ToUTF8(Uint32 codepoint, char* dst)
{
  return SDL_UCS4ToUTF8(codepoint, dst);
}

/**
 * Seeds the pseudo-random number generator.
 *
 * Reusing the seed number will cause SDL_rand_*() to repeat the same stream
 * of 'random' numbers.
 *
 * @param seed the value to use as a random number seed, or 0 to use
 *             SDL_GetPerformanceCounter().
 *
 * @threadsafety This should be called on the same thread that calls
 *               SDL_rand*()
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_rand
 * @sa SDL_rand_bits
 * @sa SDL_randf
 **/
inline void srand(Uint64 seed) { SDL_srand(seed); }

/**
 * Generate a pseudo-random number less than n for positive n
 *
 * The method used is faster and of better quality than `rand() % n`. Odds are
 * roughly 99.9% even for n = 1 million. Evenness is better for smaller n, and
 * much worse as n gets bigger.
 *
 * Example: to simulate a d6 use `SDL_rand(6) + 1` The +1 converts 0..5 to
 * 1..6
 *
 * If you want to generate a pseudo-random number in the full range of Sint32,
 * you should use: (Sint32)SDL_rand_bits()
 *
 * If you want reproducible output, be sure to initialize with SDL_srand()
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
 * @sa SDL_srand
 * @sa SDL_randf
 **/
inline Sint32 rand(Sint32 n) { return SDL_rand(n); }

/**
 * Generate a uniform pseudo-random floating point number less than 1.0
 *
 * If you want reproducible output, be sure to initialize with SDL_srand()
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
 * @sa SDL_srand
 * @sa SDL_rand
 **/
inline float randf() { return SDL_randf(); }

/**
 * Generate 32 pseudo-random bits.
 *
 * You likely want to use SDL_rand() to get a psuedo-random number instead.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @returns a random value in the range of [0-SDL_MAX_UINT32].
 *
 * @threadsafety All calls should be made from a single thread
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_rand
 * @sa SDL_randf
 * @sa SDL_srand
 **/
inline Uint32 rand_bits() { return SDL_rand_bits(); }

/**
 * Generate a pseudo-random number less than n for positive n
 *
 * The method used is faster and of better quality than `rand() % n`. Odds are
 * roughly 99.9% even for n = 1 million. Evenness is better for smaller n, and
 * much worse as n gets bigger.
 *
 * Example: to simulate a d6 use `SDL_rand_r(state, 6) + 1` The +1 converts
 * 0..5 to 1..6
 *
 * If you want to generate a pseudo-random number in the full range of Sint32,
 * you should use: (Sint32)SDL_rand_bits_r(state)
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @param state a pointer to the current random number state, this may not be
 *              NULL.
 * @param n the number of possible outcomes. n must be positive.
 * @returns a random value in the range of [0 .. n-1].
 *
 * @threadsafety This function is thread-safe, as long as the state pointer
 *               isn't shared between threads.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_rand
 * @sa SDL_rand_bits_r
 * @sa SDL_randf_r
 **/
inline Sint32 rand_r(Uint64* state, Sint32 n) { return SDL_rand_r(state, n); }

/**
 * Generate a uniform pseudo-random floating point number less than 1.0
 *
 * If you want reproducible output, be sure to initialize with SDL_srand()
 * first.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @param state a pointer to the current random number state, this may not be
 *              NULL.
 * @returns a random value in the range of [0.0, 1.0).
 *
 * @threadsafety This function is thread-safe, as long as the state pointer
 *               isn't shared between threads.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_rand_bits_r
 * @sa SDL_rand_r
 * @sa SDL_randf
 **/
inline float randf_r(Uint64* state) { return SDL_randf_r(state); }

/**
 * Generate 32 pseudo-random bits.
 *
 * You likely want to use SDL_rand_r() to get a psuedo-random number instead.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @param state a pointer to the current random number state, this may not be
 *              NULL.
 * @returns a random value in the range of [0-SDL_MAX_UINT32].
 *
 * @threadsafety This function is thread-safe, as long as the state pointer
 *               isn't shared between threads.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_rand_r
 * @sa SDL_randf_r
 **/
inline Uint32 rand_bits_r(Uint64* state) { return SDL_rand_bits_r(state); }

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
 * SDL_acosf for single-precision floats.
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
 * @sa SDL_acosf
 * @sa SDL_asin
 * @sa SDL_cos
 **/
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
 * SDL_acos for double-precision floats.
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
 * @sa SDL_acos
 * @sa SDL_asinf
 * @sa SDL_cosf
 **/
inline float acosf(float x) { return SDL_acosf(x); }

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
 * SDL_asinf for single-precision floats.
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
 * @sa SDL_asinf
 * @sa SDL_acos
 * @sa SDL_sin
 **/
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
 * SDL_asin for double-precision floats.
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
 * @sa SDL_asin
 * @sa SDL_acosf
 * @sa SDL_sinf
 **/
inline float asinf(float x) { return SDL_asinf(x); }

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
 * SDL_atanf for single-precision floats.
 *
 * To calculate the arc tangent of y / x, use SDL_atan2.
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
 * @sa SDL_atanf
 * @sa SDL_atan2
 * @sa SDL_tan
 **/
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
 * SDL_atan for dboule-precision floats.
 *
 * To calculate the arc tangent of y / x, use SDL_atan2f.
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
 * @sa SDL_atan
 * @sa SDL_atan2f
 * @sa SDL_tanf
 **/
inline float atanf(float x) { return SDL_atanf(x); }

/**
 * Compute the arc tangent of `y / x`, using the signs of x and y to adjust
 * the result's quadrant.
 *
 * The definition of `z = atan2(x, y)` is `y = x tan(z)`, where the quadrant
 * of z is determined based on the signs of x and y.
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on double-precision floating point values, use
 * SDL_atan2f for single-precision floats.
 *
 * To calculate the arc tangent of a single value, use SDL_atan.
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
 * @sa SDL_atan2f
 * @sa SDL_atan
 * @sa SDL_tan
 **/
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
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on single-precision floating point values, use
 * SDL_atan2 for double-precision floats.
 *
 * To calculate the arc tangent of a single value, use SDL_atanf.
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
 * @sa SDL_atan2f
 * @sa SDL_atan
 * @sa SDL_tan
 **/
inline float atan2f(float y, float x) { return SDL_atan2f(y, x); }

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
 * SDL_ceilf for single-precision floats.
 *
 * @param x floating point value.
 * @returns the ceiling of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_ceilf
 * @sa SDL_floor
 * @sa SDL_trunc
 * @sa SDL_round
 * @sa SDL_lround
 **/
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
 * SDL_ceil for double-precision floats.
 *
 * @param x floating point value.
 * @returns the ceiling of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_ceil
 * @sa SDL_floorf
 * @sa SDL_truncf
 * @sa SDL_roundf
 * @sa SDL_lroundf
 **/
inline float ceilf(float x) { return SDL_ceilf(x); }

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
 * SDL_copysignf for single-precision floats.
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
 * @sa SDL_copysignf
 * @sa SDL_fabs
 **/
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
 * SDL_copysign for double-precision floats.
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
 * @sa SDL_copysignf
 * @sa SDL_fabsf
 **/
inline float copysignf(float x, float y) { return SDL_copysignf(x, y); }

/**
 * Compute the cosine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on double-precision floating point values, use
 * SDL_cosf for single-precision floats.
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
 * @sa SDL_cosf
 * @sa SDL_acos
 * @sa SDL_sin
 **/
inline double cos(double x) { return SDL_cos(x); }

/**
 * Compute the cosine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on single-precision floating point values, use
 * SDL_cos for double-precision floats.
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
 * @sa SDL_cos
 * @sa SDL_acosf
 * @sa SDL_sinf
 **/
inline float cosf(float x) { return SDL_cosf(x); }

/**
 * Compute the exponential of `x`.
 *
 * The definition of `y = exp(x)` is `y = e^x`, where `e` is the base of the
 * natural logarithm. The inverse is the natural logarithm, SDL_log.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * The output will overflow if `exp(x)` is too large to be represented.
 *
 * This function operates on double-precision floating point values, use
 * SDL_expf for single-precision floats.
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
 * @sa SDL_expf
 * @sa SDL_log
 **/
inline double exp(double x) { return SDL_exp(x); }

/**
 * Compute the exponential of `x`.
 *
 * The definition of `y = exp(x)` is `y = e^x`, where `e` is the base of the
 * natural logarithm. The inverse is the natural logarithm, SDL_logf.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * The output will overflow if `exp(x)` is too large to be represented.
 *
 * This function operates on single-precision floating point values, use
 * SDL_exp for double-precision floats.
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
 * @sa SDL_exp
 * @sa SDL_logf
 **/
inline float expf(float x) { return SDL_expf(x); }

/**
 * Compute the absolute value of `x`
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * SDL_copysignf for single-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @returns the absolute value of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_fabsf
 **/
inline double fabs(double x) { return SDL_fabs(x); }

/**
 * Compute the absolute value of `x`
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * SDL_copysignf for double-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @returns the absolute value of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_fabs
 **/
inline float fabsf(float x) { return SDL_fabsf(x); }

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
 * SDL_floorf for single-precision floats.
 *
 * @param x floating point value.
 * @returns the floor of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_floorf
 * @sa SDL_ceil
 * @sa SDL_trunc
 * @sa SDL_round
 * @sa SDL_lround
 **/
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
 * SDL_floorf for double-precision floats.
 *
 * @param x floating point value.
 * @returns the floor of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_floor
 * @sa SDL_ceilf
 * @sa SDL_truncf
 * @sa SDL_roundf
 * @sa SDL_lroundf
 **/
inline float floorf(float x) { return SDL_floorf(x); }

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
 * SDL_truncf for single-precision floats.
 *
 * @param x floating point value.
 * @returns `x` truncated to an integer.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_truncf
 * @sa SDL_fmod
 * @sa SDL_ceil
 * @sa SDL_floor
 * @sa SDL_round
 * @sa SDL_lround
 **/
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
 * SDL_truncf for double-precision floats.
 *
 * @param x floating point value.
 * @returns `x` truncated to an integer.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_trunc
 * @sa SDL_fmodf
 * @sa SDL_ceilf
 * @sa SDL_floorf
 * @sa SDL_roundf
 * @sa SDL_lroundf
 **/
inline float truncf(float x) { return SDL_truncf(x); }

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
 * SDL_fmodf for single-precision floats.
 *
 * @param x the numerator.
 * @param y the denominator. Must not be 0.
 * @returns the remainder of `x / y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_fmodf
 * @sa SDL_modf
 * @sa SDL_trunc
 * @sa SDL_ceil
 * @sa SDL_floor
 * @sa SDL_round
 * @sa SDL_lround
 **/
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
 * SDL_fmod for single-precision floats.
 *
 * @param x the numerator.
 * @param y the denominator. Must not be 0.
 * @returns the remainder of `x / y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_fmod
 * @sa SDL_truncf
 * @sa SDL_modff
 * @sa SDL_ceilf
 * @sa SDL_floorf
 * @sa SDL_roundf
 * @sa SDL_lroundf
 **/
inline float fmodf(float x, float y) { return SDL_fmodf(x, y); }

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
 * @sa SDL_isinff
 **/
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
 * @sa SDL_isinf
 **/
inline int isinff(float x) { return SDL_isinff(x); }

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
 * @sa SDL_isnanf
 **/
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
 * @sa SDL_isnan
 **/
inline int isnanf(float x) { return SDL_isnanf(x); }

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
 * SDL_logf for single-precision floats.
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
 * @sa SDL_logf
 * @sa SDL_log10
 * @sa SDL_exp
 **/
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
 * SDL_log for double-precision floats.
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
 * @sa SDL_log
 * @sa SDL_expf
 **/
inline float logf(float x) { return SDL_logf(x); }

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
 * SDL_log10f for single-precision floats.
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
 * @sa SDL_log10f
 * @sa SDL_log
 * @sa SDL_pow
 **/
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
 * SDL_log10 for double-precision floats.
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
 * @sa SDL_log10
 * @sa SDL_logf
 * @sa SDL_powf
 **/
inline float log10f(float x) { return SDL_log10f(x); }

/**
 * Split `x` into integer and fractional parts
 *
 * This function operates on double-precision floating point values, use
 * SDL_modff for single-precision floats.
 *
 * @param x floating point value.
 * @param y output pointer to store the integer part of `x`.
 * @returns the fractional part of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_modff
 * @sa SDL_trunc
 * @sa SDL_fmod
 **/
inline double modf(double x, double* y) { return SDL_modf(x, y); }

/**
 * Split `x` into integer and fractional parts
 *
 * This function operates on single-precision floating point values, use
 * SDL_modf for double-precision floats.
 *
 * @param x floating point value.
 * @param y output pointer to store the integer part of `x`.
 * @returns the fractional part of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_modf
 * @sa SDL_truncf
 * @sa SDL_fmodf
 **/
inline float modff(float x, float* y) { return SDL_modff(x, y); }

/**
 * Raise `x` to the power `y`
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-INF <= z <= INF`
 *
 * If `y` is the base of the natural logarithm (e), consider using SDL_exp
 * instead.
 *
 * This function operates on double-precision floating point values, use
 * SDL_powf for single-precision floats.
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
 * @sa SDL_powf
 * @sa SDL_exp
 * @sa SDL_log
 **/
inline double pow(double x, double y) { return SDL_pow(x, y); }

/**
 * Raise `x` to the power `y`
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-INF <= z <= INF`
 *
 * If `y` is the base of the natural logarithm (e), consider using SDL_exp
 * instead.
 *
 * This function operates on single-precision floating point values, use
 * SDL_powf for double-precision floats.
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
 * @sa SDL_pow
 * @sa SDL_expf
 * @sa SDL_logf
 **/
inline float powf(float x, float y) { return SDL_powf(x, y); }

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
 * SDL_roundf for single-precision floats. To get the result as an integer
 * type, use SDL_lround.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_roundf
 * @sa SDL_lround
 * @sa SDL_floor
 * @sa SDL_ceil
 * @sa SDL_trunc
 **/
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
 * This function operates on double-precision floating point values, use
 * SDL_roundf for single-precision floats. To get the result as an integer
 * type, use SDL_lroundf.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_round
 * @sa SDL_lroundf
 * @sa SDL_floorf
 * @sa SDL_ceilf
 * @sa SDL_truncf
 **/
inline float roundf(float x) { return SDL_roundf(x); }

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
 * SDL_lround for single-precision floats. To get the result as a
 * floating-point type, use SDL_round.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_lroundf
 * @sa SDL_round
 * @sa SDL_floor
 * @sa SDL_ceil
 * @sa SDL_trunc
 **/
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
 * SDL_lroundf for double-precision floats. To get the result as a
 * floating-point type, use SDL_roundf,
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_lround
 * @sa SDL_roundf
 * @sa SDL_floorf
 * @sa SDL_ceilf
 * @sa SDL_truncf
 **/
inline long lroundf(float x) { return SDL_lroundf(x); }

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
 * SDL_scalbnf for single-precision floats.
 *
 * @param x floating point value to be scaled.
 * @param n integer exponent.
 * @returns `x * 2^n`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_scalbnf
 * @sa SDL_pow
 **/
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
 * SDL_scalbn for double-precision floats.
 *
 * @param x floating point value to be scaled.
 * @param n integer exponent.
 * @returns `x * 2^n`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_scalbn
 * @sa SDL_powf
 **/
inline float scalbnf(float x, int n) { return SDL_scalbnf(x, n); }

/**
 * Compute the sine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on double-precision floating point values, use
 * SDL_sinf for single-precision floats.
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
 * @sa SDL_sinf
 * @sa SDL_asin
 * @sa SDL_cos
 **/
inline double sin(double x) { return SDL_sin(x); }

/**
 * Compute the sine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on single-precision floating point values, use
 * SDL_sin for double-precision floats.
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
 * @sa SDL_sin
 * @sa SDL_asinf
 * @sa SDL_cosf
 **/
inline float sinf(float x) { return SDL_sinf(x); }

/**
 * Compute the square root of `x`.
 *
 * Domain: `0 <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * SDL_sqrtf for single-precision floats.
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
 * @sa SDL_sqrtf
 **/
inline double sqrt(double x) { return SDL_sqrt(x); }

/**
 * Compute the square root of `x`.
 *
 * Domain: `0 <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * SDL_sqrt for double-precision floats.
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
 * @sa SDL_sqrt
 **/
inline float sqrtf(float x) { return SDL_sqrtf(x); }

/**
 * Compute the tangent of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * SDL_tanf for single-precision floats.
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
 * @sa SDL_tanf
 * @sa SDL_sin
 * @sa SDL_cos
 * @sa SDL_atan
 * @sa SDL_atan2
 **/
inline double tan(double x) { return SDL_tan(x); }

/**
 * Compute the tangent of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * SDL_tanf for double-precision floats.
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
 * @sa SDL_tan
 * @sa SDL_sinf
 * @sa SDL_cosf
 * @sa SDL_atanf
 * @sa SDL_atan2f
 **/
inline float tanf(float x) { return SDL_tanf(x); }

/**
 * An opaque handle representing string encoding conversion state.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @ingroup resource
 */
template<ObjectBox<SDL_iconv_t> T>
struct IConvBase : T
{
  using T::T;

  /**
   * This function allocates a context for the specified character set
   * conversion.
   *
   * @param tocode The target character encoding, must not be NULL.
   * @param fromcode The source character encoding, must not be NULL.
   * @post this becomes a valid handle convertible to true on success, or
   *          convertible to false on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa iconv()
   * @sa iconv_close()
   * @sa iconv_string()
   **/
  IConvBase(StringParam tocode, StringParam fromcode)
    : T(SDL_iconv_open(tocode, fromcode))
  {
  }

  /**
   * This function converts text between encodings, reading from and writing to
   * a buffer.
   *
   * It returns the number of succesful conversions on success. On error,
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
   * @returns the number of conversions on success, or a negative error code.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa iconv_string
   */
  size_t iconv(const char** inbuf,
               size_t* inbytesleft,
               char** outbuf,
               size_t* outbytesleft)
  {
    return SDL_iconv(T::get(), inbuf, inbytesleft, outbuf, outbytesleft);
  }

  /**
   * This function frees a context used for character set conversion.
   *
   * This object becomes empty after the call.
   * @returns 0 on success, or -1 on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_iconv
   * @sa SDL_iconv_open
   * @sa SDL_iconv_string
   */
  int close() { return SDL_iconv_close(T::release()); }
};

/**
 * Helper function to convert a string's encoding in one call.
 *
 * This function converts a buffer or string between encodings in one pass.
 *
 * The string does not need to be NULL-terminated; this function operates on
 * the number of bytes specified in `inbytesleft` whether there is a NULL
 * character anywhere in the buffer.
 *
 * The returned string is owned by the caller, and should be passed to
 * SDL_free when no longer needed.
 *
 * @param tocode the character encoding of the output string. Examples are
 *               "UTF-8", "UCS-4", etc.
 * @param fromcode the character encoding of data in `inbuf`.
 * @param inbuf the string to convert to a different encoding.
 * @param inbytesleft the size of the input string _in bytes_.
 * @returns a new string, converted to the new encoding, or NULL on error.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_iconv_open
 * @sa SDL_iconv_close
 * @sa SDL_iconv
 **/
inline char* iconv_string(StringParam tocode,
                          StringParam fromcode,
                          StringParam inbuf,
                          size_t inbytesleft)
{
  return SDL_iconv_string(tocode, fromcode, inbuf, inbytesleft);
}

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
 *            not be NULL.
 * @returns false on overflow, true if result is multiplied without overflow.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 *            NULL.
 * @returns false on overflow, true if result is added without overflow.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
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
 **/
using FunctionPointer = SDL_FunctionPointer;

#pragma region impl
/// @}

inline void ObjectDeleter<SDL_Environment>::operator()(
  EnvironmentRef environment) const
{
  environment.Destroy();
}

inline void ObjectDeleter<SDL_iconv_data_t>::operator()(IConvRef iconv) const
{
  iconv.close();
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_STDINC_H_ */


// end --- SDL3pp_stdinc.h --- 



// begin --- SDL3pp_video.h --- 

#ifndef SDL3PP_VIDEO_H_
#define SDL3PP_VIDEO_H_

#include <memory>
#include <optional>
#include <vector>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

// begin --- SDL3pp_properties.h --- 

#ifndef SDL3PP_PROPERTIES_H_
#define SDL3PP_PROPERTIES_H_

#include <SDL3/SDL_properties.h>

namespace SDL {

/**
 * @defgroup CategoryProperties Object Properties
 *
 * A property is a variable that can be created and retrieved by name at
 * runtime.
 *
 * All properties are part of a property group (SDL_PropertiesID). A property
 * group can be created with the SDL_CreateProperties function and destroyed
 * with the SDL_DestroyProperties function.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - PropertiesBase.SetPointer() and PropertiesBase.GetPointer() operate on
 * `void*` pointer types.
 * - PropertiesBase.SetString() and PropertiesBase.GetString() operate on string
 * types.
 * - PropertiesBase.SetNumber() and PropertiesBase.GetNumber() operate on signed
 * 64-bit integer types.
 * - PropertiesBase.SetFloat() and PropertiesBase.GetFloat() operate on floating
 * point types.
 * - PropertiesBase.SetBoolean() and PropertiesBase.GetBoolean() operate on
 * boolean types.
 *
 * Properties can be removed from a group by using PropertiesBase.Clear().
 * @{
 */

// Forward decl
template<ObjectBox<FancyPointer<SDL_PropertiesID>> T>
struct PropertiesBase;

/**
 * @brief Handle to a non owned properties
 */
using PropertiesRef =
  PropertiesBase<ObjectRef<SDL_PropertiesID, FancyPointer<SDL_PropertiesID>>>;

struct PropertiesDeleter
{
  using pointer = FancyPointer<SDL_PropertiesID>;
  inline void operator()(PropertiesRef props) const;
};

/**
 * @brief Handle to an owned surface
 */
using Properties =
  PropertiesBase<ObjectUnique<SDL_PropertiesID, PropertiesDeleter>>;

/**
 * SDL property type
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PropertyType = SDL_PropertyType;

// Forward decl
struct PropertiesLock;

/**
 * @brief Wrap properties id
 *
 * A property is a variable that can be created and retrieved by name at
 * runtime.
 *
 * All properties are part of a property group (Properties). A property
 * group can be created with the Properties constructor and destroyed
 * with this goes out of scope.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - SetPointer() and GetPointer() operate on `void*`
 *   pointer types.
 * - SetString() and GetString() operate on string types.
 * - SetNumber() and GetNumber() operate on signed 64-bit
 *   integer types.
 * - SetFloat() and GetFloat() operate on floating point
 *   types.
 * - SetBoolean() and GetBoolean() operate on boolean
 *   types.
 *
 * Properties can be removed from a group by using SDL_ClearProperty.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa CreateProperties()
 *
 * @ingroup resource
 */
template<ObjectBox<FancyPointer<SDL_PropertiesID>> T>
struct PropertiesBase : T
{
  using T::T;

  /**
   * Copy a group of properties.
   *
   * Copy all the properties from one group of properties to another, with the
   * exception of properties requiring cleanup (set using
   * SetPointerWithCleanup()), which will not be copied. Any
   * property that already exists on `dst` will be overwritten.
   *
   * @param dst the destination properties.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool CopyPropertiesTo(PropertiesRef dst) const
  {
    return SDL_CopyProperties(T::get(), dst.get());
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
   * @returns PropertiesLock on success or false on failure; call GetError() for
   * more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesLock.UnlockProperties()
   */
  PropertiesLock Lock() &;

  /**
   * A callback used to free resources when a property is deleted.
   *
   * This should release any resources associated with `value` that are no
   * longer needed.
   *
   * This callback is set per-property. Different properties in the same group
   * can have different cleanup callbacks.
   *
   * This callback will be called _during_ SetPointerWithCleanup() if
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
   * @sa SetPointerWithCleanup
   */
  using CleanupCallback = SDL_CleanupPropertyCallback;

  /**
   * @sa PropertiesRef.CleanupCallback
   */
  using CleanupFunction = std::function<void(void*)>;

  /**
   * Set a pointer property in a group of properties with a cleanup function
   * that is called when the property is deleted.
   *
   * The cleanup function is also called if setting the property fails for any
   * reason.
   *
   * For simply setting basic data types, like numbers, bools, or strings, use
   * SetNumber(), SetBoolean(), or SetString()
   * instead, as those functions will handle cleanup on your behalf. This
   * function is only for more complex, custom data.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @param cleanup the function to call when this property is deleted, or NULL
   *                if no cleanup is necessary.
   * @param userdata a pointer that is passed to the cleanup function.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetPointer
   * @sa PropertiesRef.SetPointer
   * @sa PropertiesRef.CleanupCallback
   */
  bool SetPointerWithCleanup(StringParam name,
                             void* value,
                             CleanupCallback cleanup,
                             void* userdata)
  {
    return SDL_SetPointerPropertyWithCleanup(
      T::get(), name, value, cleanup, userdata);
  }

  /**
   * @brief Set a pointer property in a group of properties with a cleanup
   * function that is called when the property is deleted.
   *
   * The cleanup function is also called if setting the property fails for any
   * reason.
   *
   * For simply setting basic data types, like numbers, bools, or strings, use
   * SetNumber(), SetBoolean(), or SetString()
   * instead, as those functions will handle cleanup on your behalf. This
   * function is only for more complex, custom data.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @param cleanup the function to call when this property is deleted.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetPointerWithCleanup(StringParam name,
                             void* value,
                             CleanupFunction cleanup)
  {
    using Wrapper = CallbackWrapper<void(void* value)>;

    return SetPointerWithCleanup(std::move(name),
                                 value,
                                 &Wrapper::CallOnce,
                                 Wrapper::Wrap(std::move(cleanup)));
  }

  /**
   * Set a pointer property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetPointer()
   * @sa Has()
   * @sa SetBoolean()
   * @sa SetFloat()
   * @sa SetNumber()
   * @sa SetPointerWithCleanup()
   * @sa SetString()
   */
  bool SetPointer(StringParam name, void* value)
  {
    return SDL_SetPointerProperty(T::get(), name, value);
  }

  /**
   * Set a string property in a group of properties.
   *
   * This function makes a copy of the string; the caller does not have to
   * preserve the data after this call completes.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetString()
   */
  bool SetString(StringParam name, StringParam value)
  {
    return SDL_SetStringProperty(T::get(), name, value);
  }

  /**
   * Set an integer property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetNumber()
   */
  bool SetNumber(StringParam name, Sint64 value)
  {
    return SDL_SetNumberProperty(T::get(), name, value);
  }

  /**
   * Set a floating point property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFloat()
   */
  bool SetFloat(StringParam name, float value)
  {
    return SDL_SetFloatProperty(T::get(), name, value);
  }

  /**
   * Set a boolean property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBoolean()
   */
  bool SetBoolean(StringParam name, bool value)
  {
    return SDL_SetBooleanProperty(T::get(), name, value);
  }

  /**
   * Return whether a property exists.
   *
   * @param name the name of the property to query.
   * @returns true if the property exists, or false if it doesn't.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetType()
   */
  bool Has(StringParam name) const { return SDL_HasProperty(T::get(), name); }

  /**
   * Get the type of a property.
   *
   * @param name the name of the property to query.
   * @returns the type of the property, or SDL_PROPERTY_TYPE_INVALID if it is
   *          not set.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Has()
   */
  PropertyType GetType(StringParam name) const
  {
    return SDL_GetPropertyType(T::get(), name);
  }

  /**
   * Get a pointer property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a pointer property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a pointer property.
   *
   * @threadsafety It is safe to call this function from any thread, although
   *               the data returned is not protected and could potentially be
   *               freed if you call SetProperty() or
   *               ClearProperty() on these properties from another thread.
   *               If you need to avoid this, use SDL_LockProperties() and
   *               SDL_UnlockProperties().
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBoolean()
   * @sa GetFloat()
   * @sa GetNumber()
   * @sa GetType()
   * @sa GetString()
   * @sa Has()
   * @sa SetPointer()
   */
  void* GetPointer(StringParam name, void* default_value) const
  {
    return SDL_GetPointerProperty(T::get(), name, default_value);
  }

  /**
   * Get a string property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a string property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a string property.
   *
   * @threadsafety It is safe to call this function from any thread, although
   *               the data returned is not protected and could potentially be
   *               freed if you call SetString() or
   *               ClearProperty() on these properties from another thread.
   *               If you need to avoid this, use SDL_LockProperties() and
   *               SDL_UnlockProperties().
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetType()
   * @sa PropertiesRef.Has()
   * @sa PropertiesRef.SetString()
   */
  const char* GetString(StringParam name, StringParam default_value) const
  {
    return SDL_GetStringProperty(T::get(), name, default_value);
  }

  /**
   * Get a number property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a number property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a number property.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetType()
   * @sa Has()
   * @sa SetNumber()
   */
  Sint64 GetNumber(StringParam name, Sint64 default_value) const
  {
    return SDL_GetNumberProperty(T::get(), name, default_value);
  }

  /**
   * Get a floating point property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a floating point property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a float property.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetType()
   * @sa Has()
   * @sa SetFloat()
   */
  float GetFloat(StringParam name, float default_value) const
  {
    return SDL_GetFloatProperty(T::get(), name, default_value);
  }

  /**
   * Get a boolean property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a boolean property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a boolean property.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetType()
   * @sa Has()
   * @sa SetBoolean()
   */
  bool GetBoolean(StringParam name, bool default_value) const
  {
    return SDL_GetBooleanProperty(T::get(), name, default_value);
  }

  /**
   * Clear a property from a group of properties.
   *
   * @param name the name of the property to clear.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Clear(StringParam name) { return SDL_ClearProperty(T::get(), name); }

  /**
   * A callback used to enumerate all the properties in a group of properties.
   *
   * This callback is called from SDL_EnumerateProperties(), and is called once
   * per property in the set.
   *
   * @param userdata an app-defined pointer passed to the callback.
   * @param props the SDL_PropertiesID that is being enumerated.
   * @param name the next property name in the enumeration.
   *
   * @threadsafety SDL_EnumerateProperties holds a lock on `props` during this
   *               callback.
   *
   * @since This datatype is available since SDL 3.2.0.
   *
   * @sa Enumerate()
   */
  using EnumerateCallback = SDL_EnumeratePropertiesCallback;

  /**
   * @sa EnumerateCallback()
   */
  using EnumerateFunction = std::function<void(SDL_PropertiesID, const char*)>;

  /**
   * Enumerate the properties contained in a group of properties.
   *
   * The callback function is called for each property in the group of
   * properties. The properties are locked during enumeration.
   *
   * @param callback the function to call for each property.
   * @param userdata a pointer that is passed to `callback`.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Enumerate(EnumerateCallback callback, void* userdata) const
  {
    return SDL_EnumerateProperties(T::get(), callback, userdata);
  }

  /**
   * @brief Enumerate the properties contained in a group of properties.
   *
   * The callback function is called for each property in the group of
   * properties. The properties are locked during enumeration.
   *
   * @param callback the function to call for each property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool Enumerate(EnumerateFunction callback) const
  {
    using Wrapper =
      CallbackWrapper<void(SDL_PropertiesID props, const char* name)>;
    void* cbHandle = Wrapper::Wrap(std::move(callback));
    return Enumerate(&Wrapper::CallOnce, cbHandle);
  }

  /**
   * @brief Enumerate the properties contained in a group of properties.
   *
   * @param outputIter an output iterator to be assigned to each property name
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  template<std::output_iterator<const char*> IT>
  bool Enumerate(IT outputIter) const
  {
    return Enumerate(
      [&outputIter](auto props, const char name) { *outputIter++ = name; });
  }

  /**
   * Destroy a group of properties.
   *
   * All properties are deleted and their cleanup functions will be called, if
   * any.
   *
   * @post This object becomes empty after the call.
   *
   * @threadsafety This function should not be called while these properties are
   *               locked or other threads might be setting or getting values
   *               from these properties.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Destroy() { return SDL_DestroyProperties(T::release()); }

  /**
   * Returns the number of properties this has
   *
   * This uses Enumerate() internally, so might not be so fast
   */
  Uint64 GetCount() const
  {
    Uint64 count = 0;
    if (Enumerate([&](SDL_PropertiesID, const char*) { count++; })) {
      return count;
    }
    return 0;
  }
};

/**
 * @brief Wrap the lock state for PropertiesBase
 *
 */
class PropertiesLock
{
  PropertiesRef properties;

  /**
   * @sa PropertiesBase.Lock()
   */
  explicit PropertiesLock(PropertiesRef properties)
    : properties(properties)
  {
  }

public:
  /// Default ctor
  PropertiesLock()
    : properties(nullptr)
  {
  }

  PropertiesLock(const PropertiesLock& other) = delete;

  /// Move ctor
  PropertiesLock(PropertiesLock&& other)
    : properties(std::move(other.properties))
  {
  }

  /**
   * @sa Unlock()
   */
  ~PropertiesLock() { Unlock(); }

  PropertiesLock& operator=(PropertiesLock other)
  {
    std::swap(properties, other.properties);
    return *this;
  }

  /**
   * @brief Returns true if lock is active
   */
  constexpr operator bool() const { return bool(properties); }

  /**
   * Unlock a group of properties.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesBase.Lock()
   */
  void Unlock() { return SDL_UnlockProperties(properties.release()); }

  template<ObjectBox<FancyPointer<SDL_PropertiesID>> T>
  friend class PropertiesBase;
};

constexpr PropertyType PROPERTY_TYPE_INVALID = SDL_PROPERTY_TYPE_INVALID;

constexpr PropertyType PROPERTY_TYPE_POINTER = SDL_PROPERTY_TYPE_POINTER;

constexpr PropertyType PROPERTY_TYPE_STRING = SDL_PROPERTY_TYPE_STRING;

constexpr PropertyType PROPERTY_TYPE_NUMBER = SDL_PROPERTY_TYPE_NUMBER;

constexpr PropertyType PROPERTY_TYPE_FLOAT = SDL_PROPERTY_TYPE_FLOAT;

constexpr PropertyType PROPERTY_TYPE_BOOLEAN = SDL_PROPERTY_TYPE_BOOLEAN;

/**
 * Get the global SDL properties.
 *
 * @returns a valid property ID on success or 0 on failure; call
 *          GetError() for more information.
 */
inline PropertiesRef GetGlobalProperties()
{
  return FancyPointer{SDL_GetGlobalProperties()};
}

/**
 * Create a group of properties.
 *
 * All properties are automatically destroyed when Quit() is called.
 *
 * @returns a valid Properties for a new group of properties, or false on
 * failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties
 */
inline Properties CreateProperties()
{
  return Properties{SDL_CreateProperties()};
}

#pragma region impl
/// @}

inline void PropertiesDeleter::operator()(PropertiesRef props) const
{
  props.Destroy();
}

template<ObjectBox<FancyPointer<SDL_PropertiesID>> T>
PropertiesLock PropertiesBase<T>::Lock() &
{
  if (SDL_LockProperties(T::get())) return {*this};
  return {nullptr};
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_PROPERTIES_H_ */


// end --- SDL3pp_properties.h --- 



// begin --- SDL3pp_rect.h --- 

#ifndef SDL3PP_RECT_H_
#define SDL3PP_RECT_H_

#include <cmath>
#include <optional>
#include <span>
#include <SDL3/SDL_rect.h>

// begin --- SDL3pp_optionalRef.h --- 

#ifndef SDL3PP_OPTIONAL_REF_H_
#define SDL3PP_OPTIONAL_REF_H_

#include <optional>

namespace SDL {

/**
 * @brief Shim to get optional-like interface for references
 *
 * @tparam T
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

} // namespace SDL

#endif /* SDL3PP_OPTIONAL_REF_H_ */


// end --- SDL3pp_optionalRef.h --- 



// begin --- SDL3pp_spanRef.h --- 

#ifndef SDL3PP_SPAN_REF_H_
#define SDL3PP_SPAN_REF_H_

#include <concepts>
#include <ranges>
#include <span>

namespace SDL {

template<class T, class BASE>
concept DerivedWrapper =
  std::derived_from<T, BASE> && sizeof(T) == sizeof(BASE);

/**
 * A wrapper around span that works for out derived-wrapper pattern
 * (eg, Rect, Color)
 *
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

} // namespace SDL

#endif /* SDL3PP_SPAN_REF_H_ */


// end --- SDL3pp_spanRef.h --- 



namespace SDL {

/**
 * @defgroup CategoryRect Rectangle Functions
 *
 * Some helper functions for managing rectangles and 2D points, in both
 * integer and floating point versions.
 * @{
 */

// Forward decl
struct FPoint;

// Forward decl
struct Rect;

// Forward decl
struct FRect;

/**
 * @brief The structure that defines a point (using integers)
 *
 * Based on https://github.com/libSDL2pp/libSDL2pp/blob/master/SDL2pp/Point.hh
 */
struct Point : SDL_Point
{
  constexpr Point(const SDL_Point& point)
    : SDL_Point(point)
  {
  }
  constexpr Point()
    : Point({0})
  {
  }

  constexpr Point(int x, int y)
    : SDL_Point{x, y}
  {
  }

  /**
   * @brief Get X coordinate of the point
   *
   * @returns X coordinate of the point
   *
   */
  constexpr int GetX() const { return x; }

  /**
   * @brief Set X coordinate of the point
   *
   * @param[in] nx New X coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr Point& SetX(int nx)
  {
    x = nx;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the point
   *
   * @returns Y coordinate of the point
   *
   */
  constexpr int GetY() const { return y; }

  /**
   * @brief Set Y coordinate of the point
   *
   * @param[in] ny New Y coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr Point& SetY(int ny)
  {
    y = ny;
    return *this;
  }

  /**
   * Determine whether a point resides inside a rectangle.
   *
   * A point is considered part of a rectangle if both `p` and `r` are not NULL,
   * and `p`'s x and y coordinates are >= to the rectangle's top left corner,
   * and < the rectangle's x+w and y+h. So a 1x1 rectangle considers point (0,0)
   * as "inside" and (0,1) as not.
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
  constexpr bool IsInRect(const Rect& r) const;

  /**
   * @brief Get point's memberwise negation
   *
   * @returns New Point representing memberwise negation
   *
   */
  constexpr Point operator-() const { return Point(-x, -y); }

  /**
   * @brief Get point's memberwise addition with another point
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
   * @brief Get point's memberwise subtraction with another point
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
   * @brief Get point's memberwise division by an integer
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
   * @brief Get point's memberwise division by another point
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
   * @brief Get point's memberwise remainder from division
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
   * @brief Get point's memberwise remainder from division
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
   * @brief Get point's memberwise multiplication by an
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
   * @brief Get point's memberwise multiplication by another
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
   * @brief Memberwise add another point
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
   * @brief Memberwise subtract another point
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
   * @brief Memberwise divide by an integer
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
   * @brief Memberwise divide by another point
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
   * @brief Memberwise remainder from division by an integer
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
   * @brief Memberwise remainder from division by another
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
   * @brief Memberwise multiply by an integer
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
   * @brief Memberwise multiply by another point
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
   * @brief Get a point with coordinates modified so it fits
   *        into a given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Clamped point
   *
   */
  constexpr Point GetClamped(const Rect& rect) const;

  /**
   * @brief Clamp point coordinates to make it fit into a
   *        given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Reference to self
   *
   */
  constexpr Point& Clamp(const Rect& rect);

  /**
   * @brief Get a point wrapped within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Wrapped point
   *
   */
  constexpr Point GetWrapped(const Rect& rect) const;

  /**
   * @brief Wrap point coordinates within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Reference to self
   *
   */
  constexpr Point& Wrap(const Rect& rect);

  // Auto comparison operator
  constexpr auto operator<=>(const Point&) const = default;

  /**
   * @brief Converts to FPoint
   *
   * @return FPoint
   */
  constexpr operator FPoint() const;
};

/**
 * @brief The structure that defines a point (using floating point values).
 */
struct FPoint : SDL_FPoint
{
  constexpr FPoint(const SDL_FPoint& point)
    : SDL_FPoint(point)
  {
  }
  constexpr FPoint()
    : FPoint({0})
  {
  }

  constexpr FPoint(float x, float y)
    : SDL_FPoint{x, y}
  {
  }

  /**
   * @brief Get X coordinate of the point
   *
   * @returns X coordinate of the point
   *
   */
  constexpr int GetX() const { return x; }

  /**
   * @brief Set X coordinate of the point
   *
   * @param[in] nx New X coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& SetX(int nx)
  {
    x = nx;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the point
   *
   * @returns Y coordinate of the point
   *
   */
  constexpr int GetY() const { return y; }

  /**
   * @brief Set Y coordinate of the point
   *
   * @param[in] ny New Y coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& SetY(int ny)
  {
    y = ny;
    return *this;
  }

  /**
   * Determine whether a point resides inside a floating point rectangle.
   *
   * A point is considered part of a rectangle if both `p` and `r` are not NULL,
   * and `p`'s x and y coordinates are >= to the rectangle's top left corner,
   * and <= the rectangle's x+w and y+h. So a 1x1 rectangle considers point
   * (0,0) and (0,1) as "inside" and (0,2) as not.
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
  constexpr bool IsInRect(const FRect& r) const;

  /**
   * @brief Get point's memberwise negation
   *
   * @returns New Point representing memberwise negation
   *
   */
  constexpr FPoint operator-() const { return FPoint(-x, -y); }

  /**
   * @brief Get point's memberwise addition with another point
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
   * @brief Get point's memberwise subtraction with another point
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
   * @brief Get point's memberwise division by an float
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
   * @brief Get point's memberwise division by another point
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
   * @brief Get point's memberwise multiplication by an
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
   * @brief Get point's memberwise multiplication by another
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
   * @brief Memberwise add another point
   *
   * @param[in] other Point to add to the current one
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator+=(const Point& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * @brief Memberwise subtract another point
   *
   * @param[in] other Point to subtract from the current one
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator-=(const Point& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * @brief Memberwise divide by an float
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
   * @brief Memberwise divide by another point
   *
   * @param[in] other Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator/=(const Point& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  /**
   * @brief Memberwise multiply by an float
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
   * @brief Memberwise multiply by another point
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
   * @brief Get a point with coordinates modified so it fits
   *        into a given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Clamped point
   *
   */
  constexpr FPoint GetClamped(const FRect& rect) const;

  /**
   * @brief Clamp point coordinates to make it fit into a
   *        given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& Clamp(const FRect& rect);

  /**
   * @brief Get a point wrapped within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Wrapped point
   *
   */
  constexpr FPoint GetWrapped(const FRect& rect) const;

  /**
   * @brief Wrap point coordinates within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& Wrap(const FRect& rect);

  // Auto comparison operator
  constexpr auto operator<=>(const FPoint&) const = default;
};

/**
 * @brief A rectangle, with the origin at the upper left (using integers).
 */
struct Rect : SDL_Rect
{
  constexpr Rect(const SDL_Rect& rect = {0})
    : SDL_Rect(rect)
  {
  }

  constexpr Rect(int x, int y, int w, int h)
    : SDL_Rect({x, y, w, h})
  {
  }

  constexpr Rect(const SDL_Point& corner, const SDL_Point& size)
    : Rect{corner.x, corner.y, size.x, size.y}
  {
  }

  /**
   * Calculate a minimal rectangle enclosing a set of points.
   *
   * If `clip` is not NULL then only points inside of the clipping rectangle are
   * considered.
   *
   * @param points a span of SDL_Point structures representing points to be
   *               enclosed.
   * @param clip an SDL_Rect used for clipping or std::nullopt to enclose all
   *             points.
   * @returns a SDL_Rect structure filled in with the minimal enclosing
   * rectangle or std::nullopt if all the points were outside of the
   * clipping rectangle.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static std::optional<Rect> GetEnclosingPoints(
    SpanRef<const SDL_Point> points,
    OptionalRef<const SDL_Rect> clip = std::nullopt)
  {
    Rect result;
    if (SDL_GetRectEnclosingPoints(
          points.data(), points.size(), clip, &result)) {
      return result;
    }
    return std::nullopt;
  }

  /**
   * @brief Construct the rect from given center coordinates, width and height
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
   * @brief Construct the rect from given center coordinates and size
   *
   * @param[in] center Coordinates of the rectangle center
   * @param[in] size Dimensions of the rectangle
   *
   */
  static constexpr Rect FromCenter(const Point& center, const Point& size)
  {
    return Rect(center - size / 2, size);
  }

  /**
   * @brief Construct the rect from given corners coordinates
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
   * @brief Construct the rect from given centers coordinates
   *
   * @param[in] p1 Coordinates of the top left rectangle corner
   * @param[in] p2 Coordinates of the bottom right rectangle corner
   *
   */
  static constexpr Rect FromCorners(const Point& p1, const Point& p2)
  {
    return Rect(p1, p2 - p1 + Point(1, 1));
  }

  /**
   * @brief Get X coordinate of the rect corner
   *
   * @returns X coordinate of the rect corner
   *
   */
  constexpr int GetX() const { return x; }

  /**
   * @brief Set X coordinate of the rect corner
   *
   * @param[in] nx New X coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetX(int nx)
  {
    x = nx;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the rect corner
   *
   * @returns Y coordinate of the rect corner
   *
   */
  constexpr int GetY() const { return y; }

  /**
   * @brief Set Y coordinate of the rect corner
   *
   * @param[in] ny New Y coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetY(int ny)
  {
    y = ny;
    return *this;
  }

  /**
   * @brief Get width of the rect
   *
   * @returns Width of the rect
   *
   */
  constexpr int GetW() const { return w; }

  /**
   * @brief Set width of the rect
   *
   * @param[in] nw New width of the rect
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetW(int nw)
  {
    w = nw;
    return *this;
  }

  /**
   * @brief Get height of the rect
   *
   * @returns Height of the rect
   *
   */
  constexpr int GetH() const { return h; }

  /**
   * @brief Set height of the rect
   *
   * @param[in] nh New height of the rect
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetH(int nh)
  {
    h = nh;
    return *this;
  }

  /**
   * @brief Get X coordinate of the rect second corner
   *
   * @returns X coordinate of the rect second corner
   *
   */
  constexpr int GetX2() const { return x + w - 1; }

  /**
   * @brief Set X coordinate of the rect second corner
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
   * @brief Get Y coordinate of the rect second corner
   *
   * @returns Y coordinate of the rect second corner
   *
   */
  constexpr int GetY2() const { return y + h - 1; }

  /**
   * @brief Set Y coordinate of the rect second corner
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
   * @brief Get top left corner of the rect
   *
   * @returns Top left corner of the rect
   *
   */
  constexpr Point GetTopLeft() const { return Point(x, y); }

  /**
   * @brief Get top right corner of the rect
   *
   * @returns Top right corner of the rect
   *
   */
  constexpr Point GetTopRight() const { return Point(GetX2(), y); }

  /**
   * @brief Get bottom left corner of the rect
   *
   * @returns bottom left corner of the rect
   *
   */
  constexpr Point GetBottomLeft() const { return Point(x, GetY2()); }

  /**
   * @brief Get bottom right corner of the rect
   *
   * @returns Bottom right corner of the rect
   *
   */
  constexpr Point GetBottomRight() const { return Point(GetX2(), GetY2()); }

  /**
   * @brief Get size of the rect
   *
   * @returns Size of the rect
   *
   */
  constexpr Point GetSize() const { return Point(w, h); }

  /**
   * @brief Get centroid of the rect
   *
   * @returns Centroid of the rect
   *
   */
  constexpr Point GetCentroid() const { return Point(x + w / 2, y + h / 2); }

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
  bool IntersectLine(int* X1, int* Y1, int* X2, int* Y2) const
  {
    return SDL_GetRectAndLineIntersection(this, X1, Y1, X2, Y2);
  }

  /**
   * @brief Calculate the intersection of a rectangle and line segment
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
  bool IntersectLine(Point* p1, Point* p2) const
  {
    return IntersectLine(p1 ? &p1->x : nullptr,
                         p1 ? &p1->y : nullptr,
                         p2 ? &p2->x : nullptr,
                         p2 ? &p2->y : nullptr);
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
  constexpr operator FRect() const;

  /**
   * @sa operator FRect()
   */
  constexpr operator SDL_FRect() const { return operator SDL_FRect(); }

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
  constexpr bool Empty() const { return SDL_RectEmpty(this); }

  /**
   * @sa Empty()
   */
  constexpr operator bool() const { return !Empty(); }

  /**
   * Determine whether two rectangles are equal.
   *
   * Rectangles are considered equal if both are not NULL and each of their x,
   * y, width and height match.
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
  constexpr bool Equal(const Rect& other) const
  {
    return SDL_RectsEqual(this, &other);
  }

  /**
   * @sa Equal()
   */
  constexpr bool operator==(const Rect& other) const { return Equal(other); }

  /**
   * @brief Check whether the rect contains given point
   *
   * @param point Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const Point& point) const
  {
    return SDL_PointInRect(&point, this);
  }

  /**
   * @brief Check whether the rect contains given point
   *
   * @param other Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const Rect& other) const
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
   * @sa GetIntersection()
   */
  bool HasIntersection(const Rect& other) const
  {
    return SDL_HasRectIntersection(this, &other);
  }

  /**
   * Calculate the intersection of two rectangles.
   *
   * If `result` is NULL then this function will return false.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns an SDL_Rect structure filled in with the intersection of
   *               if there is intersection, std::nullopt otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa HasIntersection()
   */
  constexpr std::optional<Rect> GetIntersection(const Rect& other) const
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
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr std::optional<Rect> GetUnion(const Rect& other) const
  {
    if (Rect result; SDL_GetRectUnion(this, &other, &result)) return result;
    return std::nullopt;
  }

  /**
   * @brief Get a rect extended by specified amount of pixels
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
   * @brief Get a rect extended by specified amount of pixels
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
   * @brief Extend a rect by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& Extend(unsigned int amount) { return Extend(amount, amount); }

  /**
   * @brief Extend a rect by specified amount of pixels
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
   * @brief Get rectangle moved by a given offset
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
   * @brief Get rectangle moved by an opposite of given offset
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
   * @brief Move by then given offset
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
   * @brief Move by an opposite of the given offset
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
 * @brief A rectangle, with the origin at the upper left (using floats).
 */
struct FRect : SDL_FRect
{
  constexpr FRect(const SDL_FRect& rect = {0})
    : SDL_FRect(rect)
  {
  }

  constexpr FRect(float x, float y, float w, float h)
    : SDL_FRect({x, y, w, h})
  {
  }

  constexpr FRect(const SDL_FPoint& corner, const SDL_FPoint& size)
    : FRect{corner.x, corner.y, size.x, size.y}
  {
  }

  /**
   * Calculate a minimal rectangle enclosing a set of points.
   *
   * If `clip` is not NULL then only points inside of the clipping rectangle are
   * considered.
   *
   * @param points a span of SDL_Point structures representing points to be
   *               enclosed.
   * @param clip an SDL_Rect used for clipping or std::nullopt to enclose all
   *             points.
   * @returns a FRect structure filled in with the minimal enclosing
   *          rectangle or std::nullopt if all the points were outside of
   * the clipping rectangle.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static std::optional<FRect> GetEnclosingPoints(
    SpanRef<const SDL_FPoint> points,
    OptionalRef<const SDL_FRect> clip = std::nullopt)
  {
    FRect result;
    if (SDL_GetRectEnclosingPointsFloat(
          points.data(), points.size(), clip, &result)) {
      return result;
    }
    return std::nullopt;
  }

  /**
   * @brief Construct the rect from given center coordinates, width and height
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
   * @brief Construct the rect from given center coordinates and size
   *
   * @param[in] center Coordinates of the rectangle center
   * @param[in] size Dimensions of the rectangle
   *
   */
  static constexpr FRect FromCenter(const FPoint& center, const FPoint& size)
  {
    return FRect(center - size / 2, size);
  }

  /**
   * @brief Construct the rect from given corners coordinates
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
   * @brief Construct the rect from given centers coordinates
   *
   * @param[in] p1 Coordinates of the top left rectangle corner
   * @param[in] p2 Coordinates of the bottom right rectangle corner
   *
   */
  static constexpr FRect FromCorners(const FPoint& p1, const FPoint& p2)
  {
    return FRect(p1, p2 - p1 + FPoint(1, 1));
  }

  /**
   * @brief Get X coordinate of the rect corner
   *
   * @returns X coordinate of the rect corner
   *
   */
  constexpr float GetX() const { return x; }

  /**
   * @brief Set X coordinate of the rect corner
   *
   * @param[in] nx New X coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetX(float nx)
  {
    x = nx;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the rect corner
   *
   * @returns Y coordinate of the rect corner
   *
   */
  constexpr float GetY() const { return y; }

  /**
   * @brief Set Y coordinate of the rect corner
   *
   * @param[in] ny New Y coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetY(float ny)
  {
    y = ny;
    return *this;
  }

  /**
   * @brief Get width of the rect
   *
   * @returns Width of the rect
   *
   */
  constexpr float GetW() const { return w; }

  /**
   * @brief Set width of the rect
   *
   * @param[in] nw New width of the rect
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetW(float nw)
  {
    w = nw;
    return *this;
  }

  /**
   * @brief Get height of the rect
   *
   * @returns Height of the rect
   *
   */
  constexpr float GetH() const { return h; }

  /**
   * @brief Set height of the rect
   *
   * @param[in] nh New height of the rect
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetH(float nh)
  {
    h = nh;
    return *this;
  }

  /**
   * @brief Get X coordinate of the rect second corner
   *
   * @returns X coordinate of the rect second corner
   *
   */
  constexpr float GetX2() const { return x + w - 1; }

  /**
   * @brief Set X coordinate of the rect second corner
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
   * @brief Get Y coordinate of the rect second corner
   *
   * @returns Y coordinate of the rect second corner
   *
   */
  constexpr float GetY2() const { return y + h - 1; }

  /**
   * @brief Set Y coordinate of the rect second corner
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
   * @brief Get top left corner of the rect
   *
   * @returns Top left corner of the rect
   *
   */
  constexpr FPoint GetTopLeft() const { return FPoint(x, y); }

  /**
   * @brief Get top right corner of the rect
   *
   * @returns Top right corner of the rect
   *
   */
  constexpr FPoint GetTopRight() const { return FPoint(GetX2(), y); }

  /**
   * @brief Get bottom left corner of the rect
   *
   * @returns bottom left corner of the rect
   *
   */
  constexpr FPoint GetBottomLeft() const { return FPoint(x, GetY2()); }

  /**
   * @brief Get bottom right corner of the rect
   *
   * @returns Bottom right corner of the rect
   *
   */
  constexpr FPoint GetBottomRight() const { return FPoint(GetX2(), GetY2()); }

  /**
   * @brief Get size of the rect
   *
   * @returns Size of the rect
   *
   */
  constexpr FPoint GetSize() const { return FPoint(w, h); }

  /**
   * @brief Get centroid of the rect
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
  bool IntersectLine(float* X1, float* Y1, float* X2, float* Y2) const
  {
    return SDL_GetRectAndLineIntersectionFloat(this, X1, Y1, X2, Y2);
  }

  /**
   * @brief Calculate the intersection of a rectangle and line segment
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
  bool IntersectLine(FPoint* p1, FPoint* p2) const
  {
    return IntersectLine(p1 ? &p1->x : nullptr,
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
   * @sa Empty()
   */
  constexpr operator bool() const { return !Empty(); }

  /**
   * Determine whether two floating point rectangles are equal, within some
   * given epsilon.
   *
   * Rectangles are considered equal if both are not NULL and each of their x,
   * y, width and height are within `epsilon` of each other. If you don't know
   * what value to use for `epsilon`, you should call the SDL_RectsEqualFloat
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
   * @sa Equal()
   */
  constexpr bool EqualEpsilon(const FRect& other, const float epsilon) const
  {
    return SDL_RectsEqualEpsilon(this, &other, epsilon);
  }

  /**
   * Determine whether two rectangles are equal.
   *
   * Rectangles are considered equal if both are not NULL and each of their x,
   * y, width and height match.
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
  constexpr bool Equal(const FRect& other) const
  {
    return SDL_RectsEqualFloat(this, &other);
  }

  /**
   * @sa Equal()
   */
  constexpr bool operator==(const FRect& other) const { return Equal(other); }

  /**
   * @brief Check whether the rect contains given point
   *
   * @param point Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const FPoint& point) const
  {
    return SDL_PointInRectFloat(&point, this);
  }

  /**
   * @brief Check whether the rect contains given point
   *
   * @param other Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const FRect& other) const
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
   * @sa GetIntersection()
   */
  bool HasIntersection(const FRect& other) const
  {
    return SDL_HasRectIntersectionFloat(this, &other);
  }

  /**
   * Calculate the intersection of two rectangles.
   *
   * If `result` is NULL then this function will return false.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns an SDL_Rect structure filled in with the intersection of
   *               if there is intersection, std::nullopt otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa HasIntersection()
   */
  constexpr std::optional<FRect> GetIntersection(const FRect& other) const
  {
    if (FRect result; SDL_GetRectIntersectionFloat(this, &other, &result)) {
      return result;
    }
    return std::nullopt;
  }

  /**
   * Calculate the union of two rectangles with float precision.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns Rect representing union of two rectangles
   *
   * @since This function is available since SDL 3.2.0.
   */
  inline std::optional<FRect> GetUnion(const FRect& other) const
  {
    if (FRect result; SDL_GetRectUnionFloat(this, &other, &result))
      return result;
    return std::nullopt;
  }

  /**
   * @brief Get a rect extended by specified amount of pixels
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
   * @brief Get a rect extended by specified amount of pixels
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
   * @brief Extend a rect by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& Extend(float amount) { return Extend(amount, amount); }

  /**
   * @brief Extend a rect by specified amount of pixels
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
   * @brief Get rectangle moved by a given offset
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
   * @brief Get rectangle moved by an opposite of given offset
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
   * @brief Move by then given offset
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
   * @brief Move by an opposite of the given offset
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

#pragma region impl
/// @}

constexpr bool Point::IsInRect(const Rect& r) const
{
  return r.Contains(*this);
}

constexpr Point::operator FPoint() const { return {float(x), float(y)}; }

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

constexpr bool FPoint::IsInRect(const FRect& r) const
{
  return r.Contains(*this);
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
    x = rect.x + rect.w - 1 - fmod(rect.x - x + rect.w - 1, rect.w);
  else if (x >= rect.x + rect.w)
    x = rect.x + fmod(x - rect.x - rect.w, rect.w);

  if (y < rect.y)
    y = rect.y + rect.h - 1 - fmod(rect.y - y + rect.h - 1, rect.h);
  else if (y >= rect.y + rect.h)
    y = rect.y + fmod(y - rect.y - rect.h, rect.h);

  return *this;
}

constexpr Rect::operator FRect() const
{
  return {float(x), float(y), float(w), float(h)};
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_RECT_H_ */


// end --- SDL3pp_rect.h --- 



// begin --- SDL3pp_surface.h --- 

#ifndef SDL3PP_SURFACE_H_
#define SDL3PP_SURFACE_H_

#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_version.h>

// begin --- SDL3pp_pixels.h --- 

#ifndef SDL3PP_PIXELS_H_
#define SDL3PP_PIXELS_H_

#include <span>
#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_version.h>

namespace SDL {

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
template<class T>
struct PaletteBase;

/**
 * @brief Handle to a non owned surface
 */
using PaletteRef = PaletteBase<ObjectRef<SDL_Palette>>;

template<>
struct ObjectDeleter<SDL_Palette>
{
  void operator()(PaletteRef palette) const;
};

/**
 * @brief Handle to an owned surface
 */
using Palette = PaletteBase<ObjectUnique<SDL_Palette>>;

// Forward decl
struct Color;

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

constexpr PixelType PIXELTYPE_UNKNOWN = SDL_PIXELTYPE_UNKNOWN;

constexpr PixelType PIXELTYPE_INDEX1 = SDL_PIXELTYPE_INDEX1;

constexpr PixelType PIXELTYPE_INDEX4 = SDL_PIXELTYPE_INDEX4;

constexpr PixelType PIXELTYPE_INDEX8 = SDL_PIXELTYPE_INDEX8;

constexpr PixelType PIXELTYPE_PACKED8 = SDL_PIXELTYPE_PACKED8;

constexpr PixelType PIXELTYPE_PACKED16 = SDL_PIXELTYPE_PACKED16;

constexpr PixelType PIXELTYPE_PACKED32 = SDL_PIXELTYPE_PACKED32;

constexpr PixelType PIXELTYPE_ARRAYU8 = SDL_PIXELTYPE_ARRAYU8;

constexpr PixelType PIXELTYPE_ARRAYU16 = SDL_PIXELTYPE_ARRAYU16;

constexpr PixelType PIXELTYPE_ARRAYU32 = SDL_PIXELTYPE_ARRAYU32;

constexpr PixelType PIXELTYPE_ARRAYF16 = SDL_PIXELTYPE_ARRAYF16;

constexpr PixelType PIXELTYPE_ARRAYF32 = SDL_PIXELTYPE_ARRAYF32;

constexpr PixelType PIXELTYPE_INDEX2 = SDL_PIXELTYPE_INDEX2;

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

constexpr BitmapOrder BITMAPORDER_NONE = SDL_BITMAPORDER_NONE;

constexpr BitmapOrder BITMAPORDER_4321 = SDL_BITMAPORDER_4321;

constexpr BitmapOrder BITMAPORDER_1234 = SDL_BITMAPORDER_1234;

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

constexpr PackedOrder PACKEDORDER_NONE = SDL_PACKEDORDER_NONE;

constexpr PackedOrder PACKEDORDER_XRGB = SDL_PACKEDORDER_XRGB;

constexpr PackedOrder PACKEDORDER_RGBX = SDL_PACKEDORDER_RGBX;

constexpr PackedOrder PACKEDORDER_ARGB = SDL_PACKEDORDER_ARGB;

constexpr PackedOrder PACKEDORDER_RGBA = SDL_PACKEDORDER_RGBA;

constexpr PackedOrder PACKEDORDER_XBGR = SDL_PACKEDORDER_XBGR;

constexpr PackedOrder PACKEDORDER_BGRX = SDL_PACKEDORDER_BGRX;

constexpr PackedOrder PACKEDORDER_ABGR = SDL_PACKEDORDER_ABGR;

constexpr PackedOrder PACKEDORDER_BGRA = SDL_PACKEDORDER_BGRA;

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

constexpr ArrayOrder ARRAYORDER_NONE = SDL_ARRAYORDER_NONE;

constexpr ArrayOrder ARRAYORDER_RGB = SDL_ARRAYORDER_RGB;

constexpr ArrayOrder ARRAYORDER_RGBA = SDL_ARRAYORDER_RGBA;

constexpr ArrayOrder ARRAYORDER_ARGB = SDL_ARRAYORDER_ARGB;

constexpr ArrayOrder ARRAYORDER_BGR = SDL_ARRAYORDER_BGR;

constexpr ArrayOrder ARRAYORDER_BGRA = SDL_ARRAYORDER_BGRA;

constexpr ArrayOrder ARRAYORDER_ABGR = SDL_ARRAYORDER_ABGR;

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

constexpr PackedLayout PACKEDLAYOUT_NONE = SDL_PACKEDLAYOUT_NONE;

constexpr PackedLayout PACKEDLAYOUT_332 = SDL_PACKEDLAYOUT_332;

constexpr PackedLayout PACKEDLAYOUT_4444 = SDL_PACKEDLAYOUT_4444;

constexpr PackedLayout PACKEDLAYOUT_1555 = SDL_PACKEDLAYOUT_1555;

constexpr PackedLayout PACKEDLAYOUT_5551 = SDL_PACKEDLAYOUT_5551;

constexpr PackedLayout PACKEDLAYOUT_565 = SDL_PACKEDLAYOUT_565;

constexpr PackedLayout PACKEDLAYOUT_8888 = SDL_PACKEDLAYOUT_8888;

constexpr PackedLayout PACKEDLAYOUT_2101010 = SDL_PACKEDLAYOUT_2101010;

constexpr PackedLayout PACKEDLAYOUT_1010102 = SDL_PACKEDLAYOUT_1010102;

/// @}

/**
 * Details about the format of a pixel.
 */
using PixelFormatDetails = SDL_PixelFormatDetails;

/**
 * Pixel format.
 *
 * SDL's pixel formats have the following naming convention:
 *
 * - Names with a list of components and a single bit count, such as RGB24 and
 *   ABGR32, define a platform-independent encoding into bytes in the order
 *   specified. For example, in RGB24 data, each pixel is encoded in 3 bytes
 *   (red, green, blue) in that order, and in ABGR32 data, each pixel is
 *   encoded in 4 bytes alpha, blue, green, red) in that order. Use these
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
 * @sa PixelFormats
 */
struct PixelFormat
{
  SDL_PixelFormat format;

  constexpr PixelFormat(SDL_PixelFormat format = SDL_PIXELFORMAT_UNKNOWN)
    : format(format)
  {
  }

  /**
   * Defining custom non-FourCC pixel formats.
   *
   * For example, defining SDL_PIXELFORMAT_RGBA8888 looks like this:
   *
   * ```c
   * PixelFormat format(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_RGBA,
   *   SDL_PACKEDLAYOUT_8888, 32, 4);
   * ```
   *
   * @param type the type of the new format, probably a SDL_PixelType value.
   * @param order the order of the new format, probably a SDL_BitmapOrder,
   *              SDL_PackedOrder, or SDL_ArrayOrder value.
   * @param layout the layout of the new format, probably an SDL_PackedLayout
   *               value or zero.
   * @param bits the number of bits per pixel of the new format.
   * @param bytes the number of bytes per pixel of the new format.
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
    : format(SDL_PixelFormat(
        SDL_DEFINE_PIXELFORMAT(type, order, layout, bits, bytes)))
  {
  }

  constexpr operator bool() const { return format != SDL_PIXELFORMAT_UNKNOWN; }

  constexpr operator SDL_PixelFormat() const { return format; }

  /**
   * Retrieve the type.
   *
   * @returns the type as PixelType.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr PixelType GetType() const
  {
    return PixelType(SDL_PIXELTYPE(format));
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
   */
  constexpr int GetOrder() const { return SDL_PIXELORDER(format); }

  /**
   * Retrieve the layout.
   *
   * This is usually a value from the SDL_PackedLayout enumeration, or zero if a
   * layout doesn't make sense for the format type.
   *
   * @returns the layout
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr PackedLayout GetLayout() const
  {
    return PackedLayout(SDL_PIXELLAYOUT(format));
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
   * @sa PixelFormat.GetBytesPerPixel
   */
  constexpr int GetBitsPerPixel() const { return SDL_BITSPERPIXEL(format); }

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
   * @sa PixelFormat.GetBitsPerPixel
   */
  constexpr int GetBytesPerPixel() const { return SDL_BYTESPERPIXEL(format); }

  /**
   * Determine if this is an indexed format.
   *
   * @returns true if the format is indexed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsIndexed() const { return SDL_ISPIXELFORMAT_INDEXED(format); }

  /**
   * Determine if this is a packed format.
   *
   * @returns true if the format is packed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsPacked() const { return SDL_ISPIXELFORMAT_PACKED(format); }

  /**
   * @brief Determine if this is an array format.
   *
   * @returns true if the format is an array, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsArray() const { return SDL_ISPIXELFORMAT_ARRAY(format); }

  /**
   * Determine if this is a 10-bit format.
   *
   * @returns true if the format is 10-bit, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool Is10Bit() const { return SDL_ISPIXELFORMAT_10BIT(format); }

  /**
   * Determine if this is a floating point format.
   *
   * @returns true if the format is 10-bit, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsFloat() const { return SDL_ISPIXELFORMAT_FLOAT(format); }

  /**
   * Determine if this has an alpha channel.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsAlpha() const { return SDL_ISPIXELFORMAT_ALPHA(format); }

  /**
   * Determine if this is a "FourCC" format.
   *
   * This covers custom and other unusual formats.
   *
   * @returns true if the format has alpha, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  constexpr bool IsFourCC() const { return SDL_ISPIXELFORMAT_FOURCC(format); }

  /**
   * Get the human readable name of a pixel format.
   *
   * @returns the human readable name of the specified pixel format or
   *          "SDL_PIXELFORMAT_UNKNOWN" if the format isn't recognized.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  inline const char* GetName() const { return SDL_GetPixelFormatName(format); }

  /**
   * Convert one of the enumerated pixel formats to a bpp value and RGBA masks.
   *
   * @param bpp a bits per pixel value; usually 15, 16, or 32.
   * @param Rmask a pointer filled in with the red mask for the format.
   * @param Gmask a pointer filled in with the green mask for the format.
   * @param Bmask a pointer filled in with the blue mask for the format.
   * @param Amask a pointer filled in with the alpha mask for the format.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ForMasks()
   */
  inline bool GetMasks(int* bpp,
                       Uint32* Rmask,
                       Uint32* Gmask,
                       Uint32* Bmask,
                       Uint32* Amask) const
  {
    return SDL_GetMasksForPixelFormat(format, bpp, Rmask, Gmask, Bmask, Amask);
  }

  /**
   * Convert a bpp value and RGBA masks to an enumerated pixel format.
   *
   * This will return `SDL_PIXELFORMAT_UNKNOWN` if the conversion wasn't
   * possible.
   *
   * @param bpp a bits per pixel value; usually 15, 16, or 32.
   * @param Rmask the red mask for the format.
   * @param Gmask the green mask for the format.
   * @param Bmask the blue mask for the format.
   * @param Amask the alpha mask for the format.
   * @returns the SDL_PixelFormat value corresponding to the format masks, or
   *          SDL_PIXELFORMAT_UNKNOWN if there isn't a match.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMasks()
   */
  static inline PixelFormat ForMasks(int bpp,
                                     Uint32 Rmask,
                                     Uint32 Gmask,
                                     Uint32 Bmask,
                                     Uint32 Amask)
  {
    return {SDL_GetPixelFormatForMasks(bpp, Rmask, Gmask, Bmask, Amask)};
  }

  /**
   * Create an SDL_PixelFormatDetails structure corresponding to a pixel format.
   *
   * Returned structure may come from a shared global cache (i.e. not newly
   * allocated), and hence should not be modified, especially the palette. Weird
   * errors such as `Blit combination not supported` may occur.
   *
   * @returns a pointer to a SDL_PixelFormatDetails structure or NULL on
   *          failure; call SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  inline const PixelFormatDetails* GetDetails() const
  {
    return SDL_GetPixelFormatDetails(format);
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
  inline Uint32 Map(Color color, PaletteRef palette) const;

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
  inline Color Get(Uint32 pixel, PaletteRef palette) const;
};

/**
 * @defgroup PixelFormats Pixel Formats
 *
 * Pixel formats
 * @{
 */
constexpr PixelFormat PIXELFORMAT_UNKNOWN = SDL_PIXELFORMAT_UNKNOWN;

constexpr PixelFormat PIXELFORMAT_INDEX1LSB = SDL_PIXELFORMAT_INDEX1LSB;

constexpr PixelFormat PIXELFORMAT_INDEX1MSB = SDL_PIXELFORMAT_INDEX1MSB;

constexpr PixelFormat PIXELFORMAT_INDEX2LSB = SDL_PIXELFORMAT_INDEX2LSB;

constexpr PixelFormat PIXELFORMAT_INDEX2MSB = SDL_PIXELFORMAT_INDEX2MSB;

constexpr PixelFormat PIXELFORMAT_INDEX4LSB = SDL_PIXELFORMAT_INDEX4LSB;

constexpr PixelFormat PIXELFORMAT_INDEX4MSB = SDL_PIXELFORMAT_INDEX4MSB;

constexpr PixelFormat PIXELFORMAT_INDEX8 = SDL_PIXELFORMAT_INDEX8;

constexpr PixelFormat PIXELFORMAT_RGB332 = SDL_PIXELFORMAT_RGB332;

constexpr PixelFormat PIXELFORMAT_XRGB4444 = SDL_PIXELFORMAT_XRGB4444;

constexpr PixelFormat PIXELFORMAT_XBGR4444 = SDL_PIXELFORMAT_XBGR4444;

constexpr PixelFormat PIXELFORMAT_XRGB1555 = SDL_PIXELFORMAT_XRGB1555;

constexpr PixelFormat PIXELFORMAT_XBGR1555 = SDL_PIXELFORMAT_XBGR1555;

constexpr PixelFormat PIXELFORMAT_ARGB4444 = SDL_PIXELFORMAT_ARGB4444;

constexpr PixelFormat PIXELFORMAT_RGBA4444 = SDL_PIXELFORMAT_RGBA4444;

constexpr PixelFormat PIXELFORMAT_ABGR4444 = SDL_PIXELFORMAT_ABGR4444;

constexpr PixelFormat PIXELFORMAT_BGRA4444 = SDL_PIXELFORMAT_BGRA4444;

constexpr PixelFormat PIXELFORMAT_ARGB1555 = SDL_PIXELFORMAT_ARGB1555;

constexpr PixelFormat PIXELFORMAT_RGBA5551 = SDL_PIXELFORMAT_RGBA5551;

constexpr PixelFormat PIXELFORMAT_ABGR1555 = SDL_PIXELFORMAT_ABGR1555;

constexpr PixelFormat PIXELFORMAT_BGRA5551 = SDL_PIXELFORMAT_BGRA5551;

constexpr PixelFormat PIXELFORMAT_RGB565 = SDL_PIXELFORMAT_RGB565;

constexpr PixelFormat PIXELFORMAT_BGR565 = SDL_PIXELFORMAT_BGR565;

constexpr PixelFormat PIXELFORMAT_RGB24 = SDL_PIXELFORMAT_RGB24;

constexpr PixelFormat PIXELFORMAT_BGR24 = SDL_PIXELFORMAT_BGR24;

constexpr PixelFormat PIXELFORMAT_XRGB8888 = SDL_PIXELFORMAT_XRGB8888;

constexpr PixelFormat PIXELFORMAT_RGBX8888 = SDL_PIXELFORMAT_RGBX8888;

constexpr PixelFormat PIXELFORMAT_XBGR8888 = SDL_PIXELFORMAT_XBGR8888;

constexpr PixelFormat PIXELFORMAT_BGRX8888 = SDL_PIXELFORMAT_BGRX8888;

constexpr PixelFormat PIXELFORMAT_ARGB8888 = SDL_PIXELFORMAT_ARGB8888;

constexpr PixelFormat PIXELFORMAT_RGBA8888 = SDL_PIXELFORMAT_RGBA8888;

constexpr PixelFormat PIXELFORMAT_ABGR8888 = SDL_PIXELFORMAT_ABGR8888;

constexpr PixelFormat PIXELFORMAT_BGRA8888 = SDL_PIXELFORMAT_BGRA8888;

constexpr PixelFormat PIXELFORMAT_XRGB2101010 = SDL_PIXELFORMAT_XRGB2101010;

constexpr PixelFormat PIXELFORMAT_XBGR2101010 = SDL_PIXELFORMAT_XBGR2101010;

constexpr PixelFormat PIXELFORMAT_ARGB2101010 = SDL_PIXELFORMAT_ARGB2101010;

constexpr PixelFormat PIXELFORMAT_ABGR2101010 = SDL_PIXELFORMAT_ABGR2101010;

constexpr PixelFormat PIXELFORMAT_RGB48 = SDL_PIXELFORMAT_RGB48;

constexpr PixelFormat PIXELFORMAT_BGR48 = SDL_PIXELFORMAT_BGR48;

constexpr PixelFormat PIXELFORMAT_RGBA64 = SDL_PIXELFORMAT_RGBA64;

constexpr PixelFormat PIXELFORMAT_ARGB64 = SDL_PIXELFORMAT_ARGB64;

constexpr PixelFormat PIXELFORMAT_BGRA64 = SDL_PIXELFORMAT_BGRA64;

constexpr PixelFormat PIXELFORMAT_ABGR64 = SDL_PIXELFORMAT_ABGR64;

constexpr PixelFormat PIXELFORMAT_RGB48_FLOAT = SDL_PIXELFORMAT_RGB48_FLOAT;

constexpr PixelFormat PIXELFORMAT_BGR48_FLOAT = SDL_PIXELFORMAT_BGR48_FLOAT;

constexpr PixelFormat PIXELFORMAT_RGBA64_FLOAT = SDL_PIXELFORMAT_RGBA64_FLOAT;

constexpr PixelFormat PIXELFORMAT_ARGB64_FLOAT = SDL_PIXELFORMAT_ARGB64_FLOAT;

constexpr PixelFormat PIXELFORMAT_BGRA64_FLOAT = SDL_PIXELFORMAT_BGRA64_FLOAT;

constexpr PixelFormat PIXELFORMAT_ABGR64_FLOAT = SDL_PIXELFORMAT_ABGR64_FLOAT;

constexpr PixelFormat PIXELFORMAT_RGB96_FLOAT = SDL_PIXELFORMAT_RGB96_FLOAT;

constexpr PixelFormat PIXELFORMAT_BGR96_FLOAT = SDL_PIXELFORMAT_BGR96_FLOAT;

constexpr PixelFormat PIXELFORMAT_RGBA128_FLOAT = SDL_PIXELFORMAT_RGBA128_FLOAT;

constexpr PixelFormat PIXELFORMAT_ARGB128_FLOAT = SDL_PIXELFORMAT_ARGB128_FLOAT;

constexpr PixelFormat PIXELFORMAT_BGRA128_FLOAT = SDL_PIXELFORMAT_BGRA128_FLOAT;

constexpr PixelFormat PIXELFORMAT_ABGR128_FLOAT = SDL_PIXELFORMAT_ABGR128_FLOAT;

/**
 * Planar mode: Y + V + U  (3 planes)
 */
constexpr PixelFormat PIXELFORMAT_YV12 = SDL_PIXELFORMAT_YV12;

/**
 * Planar mode: Y + U + V  (3 planes)
 */
constexpr PixelFormat PIXELFORMAT_IYUV = SDL_PIXELFORMAT_IYUV;

/**
 * Packed mode: Y0+U0+Y1+V0 (1 plane)
 */
constexpr PixelFormat PIXELFORMAT_YUY2 = SDL_PIXELFORMAT_YUY2;

/**
 * Packed mode: U0+Y0+V0+Y1 (1 plane)
 */
constexpr PixelFormat PIXELFORMAT_UYVY = SDL_PIXELFORMAT_UYVY;

/**
 * Packed mode: Y0+V0+Y1+U0 (1 plane)
 */
constexpr PixelFormat PIXELFORMAT_YVYU = SDL_PIXELFORMAT_YVYU;

/**
 * Planar mode: Y + U/V interleaved  (2 planes)
 */
constexpr PixelFormat PIXELFORMAT_NV12 = SDL_PIXELFORMAT_NV12;

/**
 * Planar mode: Y + V/U interleaved  (2 planes)
 */
constexpr PixelFormat PIXELFORMAT_NV21 = SDL_PIXELFORMAT_NV21;

/**
 * Planar mode: Y + U/V interleaved  (2 planes)
 */
constexpr PixelFormat PIXELFORMAT_P010 = SDL_PIXELFORMAT_P010;

/**
 * Android video texture format
 */
constexpr PixelFormat PIXELFORMAT_EXTERNAL_OES = SDL_PIXELFORMAT_EXTERNAL_OES;

#if SDL_VERSION_ATLEAST(3, 2, 9)
/**
 * Motion JPEG
 * @since SDL 3.2.10
 */
constexpr PixelFormat PIXELFORMAT_MJPG = SDL_PIXELFORMAT_MJPG;

#endif

constexpr PixelFormat PIXELFORMAT_RGBA32 = SDL_PIXELFORMAT_RGBA32;

constexpr PixelFormat PIXELFORMAT_ARGB32 = SDL_PIXELFORMAT_ARGB32;

constexpr PixelFormat PIXELFORMAT_BGRA32 = SDL_PIXELFORMAT_BGRA32;

constexpr PixelFormat PIXELFORMAT_ABGR32 = SDL_PIXELFORMAT_ABGR32;

constexpr PixelFormat PIXELFORMAT_RGBX32 = SDL_PIXELFORMAT_RGBX32;

constexpr PixelFormat PIXELFORMAT_XRGB32 = SDL_PIXELFORMAT_XRGB32;

constexpr PixelFormat PIXELFORMAT_BGRX32 = SDL_PIXELFORMAT_BGRX32;

constexpr PixelFormat PIXELFORMAT_XBGR32 = SDL_PIXELFORMAT_XBGR32;

/// @}

/**
 * @name Colorspaces
 * @{
 */

/**
 * @brief Colorspace color type.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ColorType = SDL_ColorType;

constexpr ColorType COLOR_TYPE_UNKNOWN = SDL_COLOR_TYPE_UNKNOWN;

constexpr ColorType COLOR_TYPE_RGB = SDL_COLOR_TYPE_RGB;

constexpr ColorType COLOR_TYPE_YCBCR = SDL_COLOR_TYPE_YCBCR;

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

constexpr ColorRange COLOR_RANGE_UNKNOWN = SDL_COLOR_RANGE_UNKNOWN;

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

constexpr ColorPrimaries COLOR_PRIMARIES_UNKNOWN = SDL_COLOR_PRIMARIES_UNKNOWN;

/**
 * ITU-R BT.709-6
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT709 = SDL_COLOR_PRIMARIES_BT709;

constexpr ColorPrimaries COLOR_PRIMARIES_UNSPECIFIED =
  SDL_COLOR_PRIMARIES_UNSPECIFIED;

/**
 * ITU-R BT.470-6 System M
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT470M = SDL_COLOR_PRIMARIES_BT470M;

/**
 * ITU-R BT.470-6 System B, G / ITU-R BT.601-7 625
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT470BG = SDL_COLOR_PRIMARIES_BT470BG;

/**
 * ITU-R BT.601-7 525, SMPTE 170M
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT601 = SDL_COLOR_PRIMARIES_BT601;

/**
 * SMPTE 240M, functionally the same as SDL_COLOR_PRIMARIES_BT601
 */
constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE240 =
  SDL_COLOR_PRIMARIES_SMPTE240;

/**
 * Generic film (color filters using Illuminant C)
 */
constexpr ColorPrimaries COLOR_PRIMARIES_GENERIC_FILM =
  SDL_COLOR_PRIMARIES_GENERIC_FILM;

/**
 * ITU-R BT.2020-2 / ITU-R BT.2100-0
 */
constexpr ColorPrimaries COLOR_PRIMARIES_BT2020 = SDL_COLOR_PRIMARIES_BT2020;

/**
 * SMPTE ST 428-1
 */
constexpr ColorPrimaries COLOR_PRIMARIES_XYZ = SDL_COLOR_PRIMARIES_XYZ;

/**
 * SMPTE RP 431-2
 */
constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE431 =
  SDL_COLOR_PRIMARIES_SMPTE431;

/**
 * SMPTE EG 432-1 / DCI P3
 */
constexpr ColorPrimaries COLOR_PRIMARIES_SMPTE432 =
  SDL_COLOR_PRIMARIES_SMPTE432;

/**
 * EBU Tech. 3213-E
 */
constexpr ColorPrimaries COLOR_PRIMARIES_EBU3213 = SDL_COLOR_PRIMARIES_EBU3213;

constexpr ColorPrimaries COLOR_PRIMARIES_CUSTOM = SDL_COLOR_PRIMARIES_CUSTOM;

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
  SDL_TRANSFER_CHARACTERISTICS_UNKNOWN;

/**
 * Rec. ITU-R BT.709-6 / ITU-R BT1361
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT709 =
  SDL_TRANSFER_CHARACTERISTICS_BT709;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_UNSPECIFIED =
  SDL_TRANSFER_CHARACTERISTICS_UNSPECIFIED;

/**
 * ITU-R BT.470-6 System M / ITU-R BT1700 625 PAL & SECAM
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA22 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA22;

/**
 * ITU-R BT.470-6 System B, G
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_GAMMA28 =
  SDL_TRANSFER_CHARACTERISTICS_GAMMA28;

/**
 * SMPTE ST 170M / ITU-R BT.601-7 525 or 625
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT601 =
  SDL_TRANSFER_CHARACTERISTICS_BT601;

/**
 * SMPTE ST 240M
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE240 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE240;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LINEAR =
  SDL_TRANSFER_CHARACTERISTICS_LINEAR;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_LOG100_SQRT10 =
  SDL_TRANSFER_CHARACTERISTICS_LOG100_SQRT10;

/**
 * IEC 61966-2-4
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_IEC61966 =
  SDL_TRANSFER_CHARACTERISTICS_IEC61966;

/**
 * ITU-R BT1361 Extended Colour Gamut
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT1361 =
  SDL_TRANSFER_CHARACTERISTICS_BT1361;

/**
 * IEC 61966-2-1 (sRGB or sYCC)
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SRGB =
  SDL_TRANSFER_CHARACTERISTICS_SRGB;

/**
 * ITU-R BT2020 for 10-bit system
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_10BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_10BIT;

/**
 * ITU-R BT2020 for 12-bit system
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_BT2020_12BIT =
  SDL_TRANSFER_CHARACTERISTICS_BT2020_12BIT;

/**
 * SMPTE ST 2084 for 10-, 12-, 14- and 16-bit systems
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_PQ =
  SDL_TRANSFER_CHARACTERISTICS_PQ;

/**
 * SMPTE ST 428-1
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_SMPTE428 =
  SDL_TRANSFER_CHARACTERISTICS_SMPTE428;

/**
 * ARIB STD-B67, known as "hybrid log-gamma" (HLG)
 */
constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_HLG =
  SDL_TRANSFER_CHARACTERISTICS_HLG;

constexpr TransferCharacteristics TRANSFER_CHARACTERISTICS_CUSTOM =
  SDL_TRANSFER_CHARACTERISTICS_CUSTOM;

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
  SDL_MATRIX_COEFFICIENTS_IDENTITY;

/**
 * ITU-R BT.709-6
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT709 =
  SDL_MATRIX_COEFFICIENTS_BT709;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_UNSPECIFIED =
  SDL_MATRIX_COEFFICIENTS_UNSPECIFIED;

/**
 * US FCC Title 47
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_FCC =
  SDL_MATRIX_COEFFICIENTS_FCC;

/**
 * ITU-R BT.470-6 System B, G / ITU-R BT.601-7 625, functionally the same as
 * SDL_MATRIX_COEFFICIENTS_BT601
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT470BG =
  SDL_MATRIX_COEFFICIENTS_BT470BG;

/**
 * ITU-R BT.601-7 525
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT601 =
  SDL_MATRIX_COEFFICIENTS_BT601;

/**
 * SMPTE 240M
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE240 =
  SDL_MATRIX_COEFFICIENTS_SMPTE240;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_YCGCO =
  SDL_MATRIX_COEFFICIENTS_YCGCO;

/**
 * ITU-R BT.2020-2 non-constant luminance
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_NCL =
  SDL_MATRIX_COEFFICIENTS_BT2020_NCL;

/**
 * ITU-R BT.2020-2 constant luminance
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_BT2020_CL =
  SDL_MATRIX_COEFFICIENTS_BT2020_CL;

/**
 * SMPTE ST 2085
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_SMPTE2085 =
  SDL_MATRIX_COEFFICIENTS_SMPTE2085;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_NCL;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL =
  SDL_MATRIX_COEFFICIENTS_CHROMA_DERIVED_CL;

/**
 * ITU-R BT.2100-0 ICTCP
 */
constexpr MatrixCoefficients MATRIX_COEFFICIENTS_ICTCP =
  SDL_MATRIX_COEFFICIENTS_ICTCP;

constexpr MatrixCoefficients MATRIX_COEFFICIENTS_CUSTOM =
  SDL_MATRIX_COEFFICIENTS_CUSTOM;

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

/**
 * RGB, no chroma sampling
 */
constexpr ChromaLocation CHROMA_LOCATION_NONE = SDL_CHROMA_LOCATION_NONE;

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
 * Colorspace definitions.
 *
 * Since similar colorspaces may vary in their details (matrix, transfer
 * function, etc.), this is not an exhaustive list, but rather a
 * representative sample of the kinds of colorspaces supported in SDL.
 *
 * @since This enum is available since SDL 3.2.0.
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
  SDL_Colorspace colorspace;

public:
  /**
   * Wrap a SDL_Colorspace
   *
   */
  constexpr Colorspace(SDL_Colorspace colorspace = SDL_COLORSPACE_UNKNOWN)
    : colorspace(colorspace)
  {
  }

  /**
   * Define custom Colorspace formats.
   *
   * For example, defining SDL_COLORSPACE_SRGB looks like this:
   *
   * ```cpp
   * Colorspace colorspace(SDL_COLOR_TYPE_RGB,
   *                       SDL_COLOR_RANGE_FULL,
   *                       SDL_COLOR_PRIMARIES_BT709,
   *                       SDL_TRANSFER_CHARACTERISTICS_SRGB,
   *                       SDL_MATRIX_COEFFICIENTS_IDENTITY,
   *                       SDL_CHROMA_LOCATION_NONE)
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
    : colorspace(SDL_Colorspace(SDL_DEFINE_COLORSPACE(type,
                                                      range,
                                                      primaries,
                                                      transfer,
                                                      matrix,
                                                      chroma)))
  {
  }

  constexpr operator bool() const
  {
    return colorspace != SDL_COLORSPACE_UNKNOWN;
  }

  constexpr operator SDL_Colorspace() const { return colorspace; }

  /**
   * A macro to retrieve the type of a Colorspace.
   *
   * @returns the ColorType for `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr ColorType GetType() const { return SDL_COLORSPACETYPE(colorspace); }

  /**
   * A macro to retrieve the range of a Colorspace.
   *
   * @returns the ColorRange of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr ColorRange GetRange() const
  {
    return SDL_COLORSPACERANGE(colorspace);
  }

  /**
   * A macro to retrieve the chroma sample location of a Colorspace.
   *
   * @returns the ChromaLocation of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr ChromaLocation GetChroma() const
  {
    return SDL_COLORSPACECHROMA(colorspace);
  }

  /**
   * A macro to retrieve the primaries of a Colorspace.
   *
   * @returns the ColorPrimaries of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr ColorPrimaries GetPrimaries() const
  {
    return SDL_COLORSPACEPRIMARIES(colorspace);
  }

  /**
   * A macro to retrieve the transfer characteristics of a Colorspace.
   *
   * @returns the TransferCharacteristics of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr TransferCharacteristics GetTransfer() const
  {
    return SDL_COLORSPACETRANSFER(colorspace);
  }

  /**
   * A macro to retrieve the matrix coefficients of a Colorspace.
   *
   * @returns the MatrixCoefficients of `cspace`.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr MatrixCoefficients GetMatrix() const
  {
    return SDL_COLORSPACEMATRIX(colorspace);
  }

  /**
   * A macro to determine if a Colorspace uses BT601 (or BT470BG) matrix
   * coefficients.
   *
   * Note that this macro double-evaluates its parameter, so do not use
   * expressions with side-effects here.
   *
   * @returns true if BT601 or BT470BG, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT601() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT601(colorspace);
  }

  /**
   * A macro to determine if a Colorspace uses BT709 matrix coefficients.
   *
   * @returns true if BT709, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT709() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT709(colorspace);
  }

  /**
   * A macro to determine if a Colorspace uses BT2020_NCL matrix
   * coefficients.
   *
   * @returns true if BT2020_NCL, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsMatrixBT2020_NCL() const
  {
    return SDL_ISCOLORSPACE_MATRIX_BT2020_NCL(colorspace);
  }

  /**
   * A macro to determine if a Colorspace has a limited range.
   *
   * @returns true if limited range, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsLimitedRange() const
  {
    return SDL_ISCOLORSPACE_LIMITED_RANGE(colorspace);
  }

  /**
   * A macro to determine if a Colorspace has a full range.
   *
   * @returns true if full range, false otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool IsFullRange() const
  {
    return SDL_ISCOLORSPACE_FULL_RANGE(colorspace);
  }
};

/**
 * @defgroup Colorspaces Colorspaces
 * @{
 */

constexpr Colorspace COLORSPACE_UNKNOWN = SDL_COLORSPACE_UNKNOWN;

/**
 * Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709
 */
constexpr Colorspace COLORSPACE_SRGB = SDL_COLORSPACE_SRGB;

/**
 * Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709
 */
constexpr Colorspace COLORSPACE_SRGB_LINEAR = SDL_COLORSPACE_SRGB_LINEAR;

/**
 * Equivalent to DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020
 */
constexpr Colorspace COLORSPACE_HDR10 = SDL_COLORSPACE_HDR10;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_NONE_P709_X601
 */
constexpr Colorspace COLORSPACE_JPEG = SDL_COLORSPACE_JPEG;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601
 */
constexpr Colorspace COLORSPACE_BT601_LIMITED = SDL_COLORSPACE_BT601_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601
 */
constexpr Colorspace COLORSPACE_BT601_FULL = SDL_COLORSPACE_BT601_FULL;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709
 */
constexpr Colorspace COLORSPACE_BT709_LIMITED = SDL_COLORSPACE_BT709_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709
 */
constexpr Colorspace COLORSPACE_BT709_FULL = SDL_COLORSPACE_BT709_FULL;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P2020
 */
constexpr Colorspace COLORSPACE_BT2020_LIMITED = SDL_COLORSPACE_BT2020_LIMITED;

/**
 * Equivalent to DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P2020
 */
constexpr Colorspace COLORSPACE_BT2020_FULL = SDL_COLORSPACE_BT2020_FULL;

/**
 * The default colorspace for RGB surfaces if no colorspace is specified
 */
constexpr Colorspace COLORSPACE_RGB_DEFAULT = SDL_COLORSPACE_RGB_DEFAULT;

/**
 * The default colorspace for YUV surfaces if no colorspace is specified
 */
constexpr Colorspace COLORSPACE_YUV_DEFAULT = SDL_COLORSPACE_YUV_DEFAULT;

/// @}

/**
 * A structure that represents a color as RGBA components.
 *
 * The bits of this structure can be directly reinterpreted as an
 * integer-packed color which uses the SDL_PIXELFORMAT_RGBA32 format
 * (SDL_PIXELFORMAT_ABGR8888 on little-endian systems and
 * SDL_PIXELFORMAT_RGBA8888 on big-endian systems).
 *
 * @since This struct is available since SDL 3.2.0.
 */
struct Color : SDL_Color
{
  constexpr Color(SDL_Color color = {0})
    : SDL_Color(color)
  {
  }

  constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
    : SDL_Color{r, g, b, a}
  {
  }

  // Auto comparison operator
  auto operator<=>(const Color& other) const = default;

  /**
   * @brief Get the red component from the color
   *
   * @returns The red component from the color
   *
   */
  constexpr Uint8 GetRed() const { return r; }

  /**
   * @brief Set the red component from the color
   *
   * @param[in] nr New red component value
   *
   * @returns Reference to self
   *
   */
  constexpr Color& SetRed(Uint8 nr)
  {
    r = nr;
    return *this;
  }

  /**
   * @brief Get the green component from the color
   *
   * @returns The green component from the color
   *
   */
  constexpr Uint8 GetGreen() const { return g; }

  /**
   * @brief Set the green component from the color
   *
   * @param[in] ng New green component value
   *
   * @returns Reference to self
   *
   */
  constexpr Color& SetGreen(Uint8 ng)
  {
    g = ng;
    return *this;
  }

  /**
   * @brief Get the blue component from the color
   *
   * @returns The blue component from the color
   *
   */
  constexpr Uint8 GetBlue() const { return b; }

  /**
   * @brief Set the blue component from the color
   *
   * @param[in] nb New blue component value
   *
   * @returns Reference to self
   *
   */
  constexpr Color& SetBlue(Uint8 nb)
  {
    b = nb;
    return *this;
  }

  /**
   * @brief Get the alpha component from the color
   *
   * @returns The alpha component from the color
   *
   */
  constexpr Uint8 GetAlpha() const { return a; }

  /**
   * @brief Set the alpha component from the color
   *
   * @param[in] na New alpha component value
   *
   * @returns Reference to self
   *
   */
  constexpr Color& SetAlpha(Uint8 na)
  {
    a = na;
    return *this;
  }

  /**
   * @brief Map an RGBA quadruple to a pixel value for a given pixel format.
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
  Uint32 Map(const PixelFormatDetails* format, PaletteRef palette) const;

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
                   const PixelFormatDetails* format,
                   PaletteRef palette);
};

/**
 * The bits of this structure can be directly reinterpreted as a float-packed
 * color which uses the SDL_PIXELFORMAT_RGBA128_FLOAT format
 *
 * @since This struct is available since SDL 3.2.0.
 */
struct FColor : SDL_FColor
{
  constexpr FColor(SDL_FColor color = {0})
    : SDL_FColor(color)
  {
  }

  constexpr FColor(float r, float g, float b, float a = 1)
    : SDL_FColor{r, g, b, a}
  {
  }

  // Auto comparison operator
  auto operator<=>(const FColor& other) const = default;

  /**
   * @brief Get the red component from the color
   *
   * @returns The red component from the color
   *
   */
  constexpr float GetRed() const { return r; }

  /**
   * @brief Set the red component from the color
   *
   * @param[in] nr New red component value
   *
   * @returns Reference to self
   *
   */
  constexpr FColor& SetRed(float nr)
  {
    r = nr;
    return *this;
  }

  /**
   * @brief Get the green component from the color
   *
   * @returns The green component from the color
   *
   */
  constexpr float GetGreen() const { return g; }

  /**
   * @brief Set the green component from the color
   *
   * @param[in] ng New green component value
   *
   * @returns Reference to self
   *
   */
  constexpr FColor& SetGreen(float ng)
  {
    g = ng;
    return *this;
  }

  /**
   * @brief Get the blue component from the color
   *
   * @returns The blue component from the color
   *
   */
  constexpr float GetBlue() const { return b; }

  /**
   * @brief Set the blue component from the color
   *
   * @param[in] nb New blue component value
   *
   * @returns Reference to self
   *
   */
  constexpr FColor& SetBlue(float nb)
  {
    b = nb;
    return *this;
  }

  /**
   * @brief Get the alpha component from the color
   *
   * @returns The alpha component from the color
   *
   */
  constexpr float GetAlpha() const { return a; }

  /**
   * @brief Set the alpha component from the color
   *
   * @param[in] na New alpha component value
   *
   * @returns Reference to self
   *
   */
  constexpr FColor& SetAlpha(float na)
  {
    a = na;
    return *this;
  }
};

/**
 * @brief A set of indexed colors representing a palette.
 *
 * @ingroup resource
 */
template<class T>
struct PaletteBase : T
{
  // Make default ctors available
  using T::T;

  /**
   * Create a palette structure with the specified number of color entries.
   *
   * The palette entries are initialized to white.
   *
   * @param ncolors represents the number of color entries in the color palette.
   * @post this represents a new Palette structure convertible to true on
   * success or converts to false on failure (e.g. if there wasn't enough
   * memory); call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColors()
   * @sa Surface.SetPalette()
   */
  inline PaletteBase(int ncolors)
    : T(SDL_CreatePalette(ncolors))
  {
  }

  constexpr int GetSize() const { return this->ncolors; }

  constexpr Color operator[](int index) const { return this->colors[index]; }

  /**
   * Set a range of colors in a palette.
   *
   * @param colors an array of SDL_Color structures to copy into the palette.
   * @param firstcolor the index of the first palette entry to modify.
   * @param ncolors the number of entries to modify.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  inline bool SetColors(const Color* colors, int firstcolor, int ncolors)
  {
    return SDL_SetPaletteColors(T::get(), colors, firstcolor, ncolors);
  }

  /**
   * Set a range of colors in a palette.
   *
   * @param colors a span of SDL_Color structures to copy into the palette.
   * @param firstcolor the index of the first palette entry to modify.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   */
  bool SetColors(SpanRef<const SDL_Color> colors, int firstcolor = 0)
  {
    SDL_assert_paranoid(colors.size() < SDL_MAX_SINT32);
    return SetColors(colors.data(), firstcolor, colors.size());
  }

  /**
   * Free a palette
   *
   * After calling, this object becomes empty.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the palette is not modified or destroyed in another thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Destroy() { return SDL_DestroyPalette(T::release()); }
};

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
 * @param format a pointer to SDL_PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be NULL.
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
 * @sa GetPixelFormatDetails()
 * @sa GetRGB()
 * @sa MapRGBA()
 * @sa Surface.MapColor()
 */
inline Uint32 MapRGB(const PixelFormatDetails* format,
                     PaletteRef palette,
                     Uint8 r,
                     Uint8 g,
                     Uint8 b)
{
  return SDL_MapRGB(format, palette.get(), r, g, b);
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
 * @param format a pointer to SDL_PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be NULL.
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
 * @sa PixelFormat.GetDetails()
 * @sa GetRGBA()
 * @sa MapRGB()
 * @sa Surface.MapColor()
 */
inline Uint32 MapRGBA(const PixelFormatDetails* format,
                      PaletteRef palette,
                      Uint8 r,
                      Uint8 g,
                      Uint8 b,
                      Uint8 a)
{
  return SDL_MapRGBA(format, palette.get(), r, g, b, a);
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
 * @param format a pointer to SDL_PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be NULL.
 * @param r a pointer filled in with the red component, may be NULL.
 * @param g a pointer filled in with the green component, may be NULL.
 * @param b a pointer filled in with the blue component, may be NULL.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails()
 * @sa GetRGBA()
 * @sa MapRGB()
 * @sa MapRGBA()
 */
inline void GetRGB(Uint32 pixel,
                   const PixelFormatDetails* format,
                   PaletteRef palette,
                   Uint8* r,
                   Uint8* g,
                   Uint8* b)
{
  SDL_GetRGB(pixel, format, palette.get(), r, g, b);
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
 * @param format a pointer to SDL_PixelFormatDetails describing the pixel
 *               format.
 * @param palette an optional palette for indexed formats, may be NULL.
 * @param r a pointer filled in with the red component, may be NULL.
 * @param g a pointer filled in with the green component, may be NULL.
 * @param b a pointer filled in with the blue component, may be NULL.
 * @param a a pointer filled in with the alpha component, may be NULL.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the palette is not modified.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PixelFormat.GetDetails()
 * @sa GetRGB()
 * @sa MapRGB()
 * @sa MapRGBA()
 */
inline void GetRGBA(Uint32 pixel,
                    const PixelFormatDetails* format,
                    PaletteRef palette,
                    Uint8* r,
                    Uint8* g,
                    Uint8* b,
                    Uint8* a)
{
  SDL_GetRGBA(pixel, format, palette.get(), r, g, b, a);
}

/** @} */

#pragma region impl

inline void ObjectDeleter<SDL_Palette>::operator()(PaletteRef palette) const
{
  palette.Destroy();
}

inline Uint32 Color::Map(const PixelFormatDetails* format,
                         PaletteRef palette = nullptr) const
{
  return MapRGBA(format, palette.get(), r, g, b, a);
}

inline Color Get(Uint32 pixel,
                 const PixelFormatDetails* format,
                 PaletteRef palette = nullptr)
{
  Color c;
  GetRGBA(pixel, format, palette, &c.r, &c.g, &c.b, &c.a);
  return c;
}

inline Uint32 PixelFormat::Map(Color color, PaletteRef palette = nullptr) const
{
  return color.Map(GetDetails(), palette);
}

inline Color PixelFormat::Get(Uint32 pixel, PaletteRef palette = nullptr) const
{
  return Color::Get(pixel, GetDetails(), palette);
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_PIXELS_H_ */


// end --- SDL3pp_pixels.h --- 



namespace SDL {

/**
 * @defgroup CategorySurface Surface Creation and Simple Drawing
 *
 * SDL surfaces are buffers of pixels in system RAM. These are useful for
 * passing around and manipulating images that are not stored in GPU memory.
 *
 * SDL_Surface makes serious efforts to manage images in various formats, and
 * provides a reasonable toolbox for transforming the data, including copying
 * between surfaces, filling rectangles in the image data, etc.
 *
 * There is also a simple .bmp loader, SDL::LoadBMP(). SDL itself does not
 * provide loaders for various other file formats, but there are several
 * excellent external libraries that do, including its own satellite library,
 * SDL_image:
 *
 * https://github.com/libsdl-org/SDL_image
 * @{
 */

// Forward decl
template<ObjectBox<SDL_Surface*> T>
struct SurfaceBase;

/**
 * @brief Handle to a non owned surface
 */
using SurfaceRef = SurfaceBase<ObjectRef<SDL_Surface>>;

template<>
struct ObjectDeleter<SDL_Surface>
{
  void operator()(SurfaceRef Surface) const;
};

/**
 * @brief Handle to an owned surface
 */
using Surface = SurfaceBase<ObjectUnique<SDL_Surface>>;

// Forward decl
struct SurfaceLock;

/**
 * @brief The flags on an SDL_Surface.
 *
 * These are generally considered read-only.
 *
 */
using SurfaceFlags = SDL_SurfaceFlags;

/**
 * @brief The scaling mode.
 *
 */
using ScaleMode = SDL_ScaleMode;

#if SDL_VERSION_ATLEAST(3, 2, 9)

/**
 * @since SDL 3.2.10
 */
constexpr ScaleMode SCALEMODE_INVALID = SDL_SCALEMODE_INVALID;
#endif

/**
 * nearest pixel sampling
 */
constexpr ScaleMode SCALEMODE_NEAREST = SDL_SCALEMODE_NEAREST;

/**
 * linear filtering
 */
constexpr ScaleMode SCALEMODE_LINEAR = SDL_SCALEMODE_LINEAR;

/**
 * @brief The flip mode.
 *
 */
using FlipMode = SDL_FlipMode;

/**
 * Do not flip
 */
constexpr FlipMode FLIP_NONE = SDL_FLIP_NONE;

/**
 * flip horizontally
 */
constexpr FlipMode FLIP_HORIZONTAL = SDL_FLIP_HORIZONTAL;

/**
 * flip vertically
 */
constexpr FlipMode FLIP_VERTICAL = SDL_FLIP_VERTICAL;

/**
 * A collection of pixels used in software blitting.
 *
 * Pixels are arranged in memory in rows, with the top row first. Each row
 * occupies an amount of memory given by the pitch (sometimes known as the row
 * stride in non-SDL APIs).
 *
 * Within each row, pixels are arranged from left to right until the width is
 * reached. Each pixel occupies a number of bits appropriate for its format,
 * with most formats representing each pixel as one or more whole bytes (in
 * some indexed formats, instead multiple pixels are packed into each byte),
 * and a byte order given by the format. After encoding all pixels, any
 * remaining bytes to reach the pitch are used as padding to reach a desired
 * alignment, and have undefined contents.
 *
 * When a surface holds YUV format data, the planes are assumed to be
 * contiguous without padding between them, e.g. a 32x32 surface in NV12
 * format with a pitch of 32 would consist of 32x32 bytes of Y plane followed
 * by 32x16 bytes of UV plane.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa SurfaceBase::SurfaceBase()
 *
 * @ingroup resource
 */
template<ObjectBox<SDL_Surface*> T>
struct SurfaceBase : T
{
  using T::T;

  /**
   * Allocate a new surface with a specific pixel format.
   *
   * The pixels of the new surface are initialized to zero.
   *
   * @param width the width of the surface.
   * @param height the height of the surface.
   * @param format the PixelFormat for the new surface's pixel format.
   * @post the new structure that is created and convertible to true on success
   * or convertible to false on failure; call SDL_GetError() for more
   * information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  SurfaceBase(int width, int height, PixelFormat format)
    : T(SDL_CreateSurface(width, height, format))
  {
  }

  /**
   * @brief Allocate a new surface with a specific pixel format and existing
   * pixel data.
   *
   * No copy is made of the pixel data. Pixel data is not managed automatically;
   * you must free the surface before you free the pixel data.
   *
   * Pitch is the offset in bytes from one row of pixels to the next, e.g.
   * `width*4` for `SDL_PIXELFORMAT_RGBA8888`.
   *
   * You may pass NULL for pixels and 0 for pitch to create a surface that you
   * will fill in with valid values later.
   *
   * @param width the width of the surface.
   * @param height the height of the surface.
   * @param format the PixelFormat for the new surface's pixel format.
   * @param pixels a pointer to existing pixel data.
   * @param pitch the number of bytes between each row, including padding.
   * @post the new structure that is created and convertible to true on success
   * or convertible to false on failure; call SDL_GetError() for more
   * information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  SurfaceBase(int width,
              int height,
              PixelFormat format,
              void* pixels,
              int pitch)
    : T(SDL_CreateSurfaceFrom(width, height, format, pixels, pitch))
  {
  }

  /**
   * Free a surface.
   *
   * This makes this object empty
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Destroy() { return SDL_DestroySurface(T::release()); }

  /**
   * Get the properties associated with a surface.
   *
   * The following properties are understood by SDL:
   *
   * - `SDL_PROP_SURFACE_SDR_WHITE_POINT_FLOAT`: for HDR10 and floating point
   *   surfaces, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 203 for HDR10 surfaces and 1.0 for floating point surfaces.
   * - `SDL_PROP_SURFACE_HDR_HEADROOM_FLOAT`: for HDR10 and floating point
   *   surfaces, this defines the maximum dynamic range used by the content, in
   *   terms of the SDR white point. This defaults to 0.0, which disables tone
   *   mapping.
   * - `SDL_PROP_SURFACE_TONEMAP_OPERATOR_STRING`: the tone mapping operator
   *   used when compressing from a surface with high dynamic range to another
   *   with lower dynamic range. Currently this supports "chrome", which uses
   *   the same tone mapping that Chrome uses for HDR content, the form "*=N",
   *   where N is a floating point scale factor applied in linear space, and
   *   "none", which disables tone mapping. This defaults to "chrome".
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return SDL_GetSurfaceProperties(T::get());
  }

  /**
   * Set the colorspace used by a surface.
   *
   * Setting the colorspace doesn't change the pixels, only how they are
   * interpreted in color operations.
   *
   * @param colorspace a Colorspace value describing the surface colorspace.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorspace()
   */
  bool SetColorspace(Colorspace colorspace)
  {
    return SDL_SetSurfaceColorspace(T::get(), colorspace);
  }

  /**
   * @brief Get the colorspace used by a surface.
   *
   * The colorspace defaults to SDL_COLORSPACE_SRGB_LINEAR for floating point
   * formats, SDL_COLORSPACE_HDR10 for 10-bit formats, SDL_COLORSPACE_SRGB for
   * other RGB surfaces and SDL_COLORSPACE_BT709_FULL for YUV textures.
   *
   * @returns the colorspace used by the surface, or SDL_COLORSPACE_UNKNOWN if
   *          the surface is NULL.
   */
  Colorspace GetColorspace() const
  {
    return SDL_GetSurfaceColorspace(T::get());
  }

  /**
   * Create a palette and associate it with a surface.
   *
   * This function creates a palette compatible with the provided surface. The
   * palette is then returned for you to modify, and the surface will
   * automatically use the new palette in future operations. You do not need to
   * destroy the returned palette, it will be freed when the reference count
   * reaches 0, usually when the surface is destroyed.
   *
   * Bitmap surfaces (with format SDL_PIXELFORMAT_INDEX1LSB or
   * SDL_PIXELFORMAT_INDEX1MSB) will have the palette initialized with 0 as
   * white and 1 as black. Other surfaces will get a palette initialized with
   * white in every entry.
   *
   * If this function is called for a surface that already has a palette, a new
   * palette will be created to replace it.
   *
   * @returns a new SDL_Palette structure on success or NULL on failure (e.g. if
   *          the surface didn't have an index format); call GetError() for
   *          more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette.SetColors()
   */
  PaletteRef CreatePalette() { return SDL_CreateSurfacePalette(T::get()); }

  /**
   * Set the palette used by a surface.
   *
   * A single palette can be shared with many surfaces.
   *
   * @param palette the Palette to use.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Palette::Palette()
   * @sa GetPalette()
   */
  bool SetPalette(PaletteRef palette)
  {
    return SDL_SetSurfacePalette(T::get(), palette.get());
  }

  /**
   * Get the palette used by a surface.
   *
   * @returns a pointer to the palette used by the surface, or NULL if there is
   *          no palette used.
   */
  PaletteRef GetPalette() const { return SDL_GetSurfacePalette(T::get()); }

  /**
   * Add an alternate version of a surface.
   *
   * This function adds an alternate version of this surface, usually used for
   * content with high DPI representations like cursors or icons. The size,
   * format, and content do not need to match the original surface, and these
   * alternate versions will not be updated when the original surface changes.
   *
   * This function adds a reference to the alternate version, so you should call
   * SDL_DestroySurface() on the image after this call.
   *
   * @param image a pointer to an alternate SDL_Surface to associate with this
   *              surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RemoveAlternateImages()
   * @sa GetImages()
   * @sa HasAlternateImages()
   */
  bool AddAlternateImage(SurfaceRef image)
  {
    return SDL_AddSurfaceAlternateImage(T::get(), image);
  }

  /**
   * Return whether a surface has alternate versions available.
   *
   * @returns true if alternate versions are available or false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AddAlternateImage()
   * @sa RemoveAlternateImages()
   * @sa GetImages()
   */
  bool HasAlternateImages() const
  {
    return SDL_SurfaceHasAlternateImages(T::get());
  }

  /**
   * Get an array including all versions of a surface.
   *
   * This returns all versions of a surface, with the surface being queried as
   * the first element in the returned array.
   *
   * Freeing the array of surfaces does not affect the surfaces in the array.
   * They are still referenced by the surface being queried and will be cleaned
   * up normally.
   *
   * @param count a pointer filled in with the number of surface pointers
   *              returned, may be NULL.
   * @returns a NULL terminated array of SDL_Surface pointers or NULL on
   *          failure; call SDL_GetError() for more information. This should be
   *          freed with SDL_free() when it is no longer needed.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AddAlternateImage()
   * @sa RemoveAlternateImages()
   * @sa HasAlternateImages()
   */
  FreeWrapper<SurfaceRef*[]> GetImages(int* count = nullptr) const
  {
    return SDL_GetSurfaceImages(T::get(), count);
  }

  /**
   * Remove all alternate versions of a surface.
   *
   * This function removes a reference from all the alternative versions,
   * destroying them if this is the last reference to them.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AddAlternateImage()
   * @sa GetImages()
   * @sa HasAlternateImages()
   */
  void RemoveAlternateImages() { SDL_RemoveSurfaceAlternateImages(T::get()); }

  /**
   * Evaluates to true if the surface needs to be locked before access.
   *
   * @since This macro is available since SDL 3.2.0.
   */
  constexpr bool MustLock() const { return SDL_MUSTLOCK(T::get()); }

  /**
   * Set up a surface for directly accessing the pixels.
   *
   * Between calls to SurfaceBase.Lock() / Unlock(), you can write
   * to and read from `GetPixels()`, using the pixel format stored in
   * `GetFormat()`. Once you are done accessing the surface, you should use
   * Unlock() to release it or let the destructor take care of this
   * for you.
   *
   * Not all surfaces require locking. If `SurfaceBase.MustLock(surface)`
   * evaluates to false, then you can read and write to the surface at any time,
   * and the pixel format of the surface will not change.
   *
   * @returns SurfaceLock object that converts to true on success or false on
   * failure; call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MustLock()
   * @sa SurfaceLock.Unlock()
   */
  SurfaceLock Lock() &;

  /**
   * @brief Set the RLE acceleration hint for a surface.
   *
   * If RLE is enabled, color key and alpha blending blits are much faster, but
   * the surface must be locked before directly accessing the pixels.
   *
   * @param enabled true to enable RLE acceleration, false to disable it.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool SetRLE(bool enabled) { return SDL_SetSurfaceRLE(T::get(), enabled); }

  /**
   * @brief Returns whether the surface is RLE enabled.
   *
   * @returns true if the surface is RLE enabled, false otherwise.
   */
  bool HasRLE() const { return SDL_SurfaceHasRLE(T::get()); }

  /**
   * Set the color key (transparent pixel) in a surface.
   *
   * The color key defines a pixel value that will be treated as transparent in
   * a blit. For example, one can use this to specify that cyan pixels should be
   * considered transparent, and therefore not rendered.
   *
   * It is a pixel of the format used by the surface, as generated by
   * SDL_MapRGB().
   *
   * @param key the transparent pixel color.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorKey()
   * @sa SetRLE()
   * @sa HasColorKey()
   */
  bool SetColorKey(Color key) { return SetColorKey(MapColor(key)); }

  /**
   * Set the color key (transparent pixel) in a surface.
   *
   * The color key defines a pixel value that will be treated as transparent in
   * a blit. For example, one can use this to specify that cyan pixels should be
   * considered transparent, and therefore not rendered.
   *
   * It is a pixel of the format used by the surface, as generated by
   * SDL_MapRGB().
   *
   * @param key the transparent pixel or std::nullopt to disable it.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorKey()
   * @sa SetRLE()
   * @sa HasColorKey()
   */
  bool SetColorKey(std::optional<Uint32> key)
  {
    return SDL_SetSurfaceColorKey(T::get(), key.has_value(), key.value_or(0));
  }

  /**
   * Unset the color key (transparent pixel) in a surface.
   *
   * The color key defines a pixel value that will be treated as transparent in
   * a blit. For example, one can use this to specify that cyan pixels should be
   * considered transparent, and therefore not rendered.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool ClearColorKey() { return SDL_SetSurfaceColorKey(T::get(), false, 0); }

  /**
   * @brief Returns whether the surface has a color key.
   *
   * @returns true if the surface has a color key, false otherwise.
   */
  bool HasColorKey() const { return SDL_SurfaceHasColorKey(T::get()); }

  /**
   * @brief Get the color key (transparent pixel) for a surface.
   *
   * The color key is a pixel of the format used by the surface, as generated by
   * MapRGB().
   *
   * If the surface doesn't have color key enabled this function returns
   * std::nullopt.
   *
   * @returns the color key as Color or std::nullopt if it does not have one.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorKey()
   * @sa HasColorKey()
   */
  std::optional<Color> GetColorKey() const
  {
    if (Color color; GetColorKey(&color)) return color;
    return std::nullopt;
  }

  /**
   * Get the color key (transparent pixel) for a surface.
   *
   * The color key is a pixel of the format used by the surface, as generated by
   * SDL_MapRGB().
   *
   * If the surface doesn't have color key enabled this function returns false.
   *
   * @param key a pointer filled in with the transparent pixel.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorKey()
   * @sa HasColorKey()
   */
  bool GetColorKey(Color* key) const
  {
    if (Uint32 color; GetColorKey(&color)) {
      *key = MapColor(color);
      return true;
    }
    return false;
  }

  /**
   * Get the color key (transparent pixel) for a surface.
   *
   * The color key is a pixel of the format used by the surface, as generated by
   * SDL_MapRGB().
   *
   * If the surface doesn't have color key enabled this function returns false.
   *
   * @param key a pointer filled in with the transparent pixel.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorKey()
   * @sa HasColorKey()
   */
  bool GetColorKey(Uint32* key) const
  {
    return SDL_GetSurfaceColorKey(T::get(), key);
  }

  /**
   * @brief Set an additional color value multiplied into blit operations.
   *
   * When this surface is blitted, during the blit operation each source color
   * channel is modulated by the appropriate color value according to the
   * following formula:
   *
   * `srcC = srcC * (color / 255)`
   *
   * @param r the red color value multiplied into blit operations.
   * @param g the green color value multiplied into blit operations.
   * @param b the blue color value multiplied into blit operations.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool SetColorMod(Uint8 r, Uint8 g, Uint8 b)
  {
    return SDL_SetSurfaceColorMod(T::get(), r, g, b);
  }

  /**
   * @brief Get the additional color value multiplied into blit operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool GetColorMod(Uint8* r, Uint8* g, Uint8* b) const
  {
    return SDL_GetSurfaceColorMod(T::get(), r, g, b);
  }

  /**
   * @brief Set an additional alpha value used in blit operations.
   *
   * When this surface is blitted, during the blit operation the source alpha
   * value is modulated by this alpha value according to the following formula:
   *
   * `srcA = srcA * (alpha / 255)`
   *
   * @param alpha the alpha value multiplied into blit operations.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool SetAlphaMod(Uint8 alpha)
  {
    return SDL_SetSurfaceAlphaMod(T::get(), alpha);
  }

  /**
   * @brief Get the additional alpha value used in blit operations.
   *
   * @returns alpha on success or std::nullopt on failure; call GetError()
   * for more information.
   */
  std::optional<Uint8> GetAlphaMod() const
  {
    if (Uint8 alpha; SDL_GetSurfaceAlphaMod(T::get(), &alpha)) return alpha;
    return std::nullopt;
  }

  /**
   * @brief Set an additional color and alpha value multiplied into blit
   * operations.
   *
   * When this surface is blitted, during the blit operation each source color
   * channel is modulated by the appropriate color value according to the
   * following formula:
   *
   * `srcC = srcC * (color / 255)`
   * `srcA = srcA * (alpha / 255)`
   *
   * @param color the color to be multiplied in blit operations
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool SetColorAndAlphaMod(Color color)
  {
    return SetColorMod(color.r, color.g, color.b) && SetAlphaMod(color.a);
  }

  /**
   * @brief Get the additional color and alpha value multiplied into blit
   * operations.
   *
   * @returns a Color containing RGBA value on success or std::nullopt on
   * failure; call GetError() for more information.
   */
  std::optional<Color> GetColorAndAlphaMod() const
  {
    if (Color c; GetColorMod(&c.r, &c.g, &c.b) &&
                 SDL_GetSurfaceAlphaMod(T::get(), &c.a)) {
      return c;
    }
    return std::nullopt;
  }

  /**
   * Set the blend mode used for blit operations.
   *
   * To copy a surface to another surface (or texture) without blending with the
   * existing data, the blendmode of the SOURCE surface should be set to
   * `SDL_BLENDMODE_NONE`.
   *
   * @param blendMode the SDL_BlendMode to use for blit blending.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBlendMode()
   */
  bool SetBlendMode(BlendMode blendMode)
  {
    return SDL_SetSurfaceBlendMode(T::get(), blendMode);
  }

  /**
   * Get the blend mode used for blit operations.
   *
   * @return the blendMode on success or std::nullopt on failure; call
   * GetError() for more information.
   */
  std::optional<BlendMode> GetBlendMode() const
  {
    if (BlendMode blendMode; SDL_GetSurfaceBlendMode(T::get(), &blendMode)) {
      return blendMode;
    }
    return std::nullopt;
  }

  /**
   * Set the clipping rectangle for a surface.
   *
   * When `surface` is the destination of a blit, only the area within the clip
   * rectangle is drawn into.
   *
   * Note that blits are automatically clipped to the edges of the source and
   * destination surfaces.
   *
   * @param rect the SDL_Rect structure representing the clipping rectangle or
   *        nullopt to disable it
   * @returns true if the rectangle intersects the surface, otherwise false and
   *          blits will be completely clipped.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ResetClipRect()
   */
  bool SetClipRect(OptionalRef<const SDL_Rect> rect)
  {
    return SDL_SetSurfaceClipRect(T::get(), rect);
  }

  /**
   * @brief Disable the clipping rectangle for a surface.
   *
   * @returns true if clips was reset successfully
   *
   * @sa SetClipRect()
   */
  bool ResetClipRect() { return SDL_SetSurfaceClipRect(T::get(), nullptr); }

  /**
   * @brief Get the clipping rectangle for a surface.
   *
   * When `surface` is the destination of a blit, only the area within the clip
   * rectangle is drawn into.
   *
   * @returns the Rect structure filled in with the clipping rectangle for the
   * surface on success, or std::nullopt on failure; call GetError() for
   * more information.
   */
  std::optional<Rect> GetClipRect() const
  {
    if (Rect r; SDL_GetSurfaceClipRect(T::get(), &r)) { return r; }
    return std::nullopt;
  }

  /**
   * Flip a surface vertically or horizontally.
   *
   * @param flip the direction to flip.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Flip(FlipMode flip) { return SDL_FlipSurface(T::get(), flip); }

  /**
   * @brief Creates a new surface identical to the existing surface.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * The returned surface  automatically calls SDL_free after result is out of
   * scope.
   *
   * @returns a copy of the surface or NULL on failure; call GetError() for
   *          more information.
   */
  Surface Duplicate() const { return {SDL_DuplicateSurface(T::get())}; }

  /**
   * @brief Creates a new surface identical to the existing surface, scaled to
   * the desired size.
   *
   * The returned surface  automatically calls SDL_free after result is out of
   * scope.
   *
   * @param width the width of the new surface.
   * @param height the height of the new surface.
   * @param scaleMode the ScaleMode to be used.
   * @returns a copy of the surface or NULL on failure; call GetError() for
   *          more information.
   */
  Surface Scale(int width, int height, ScaleMode scaleMode) const
  {
    return {SDL_ScaleSurface(T::get(), width, height, scaleMode)};
  }

  /**
   * Copy an existing surface to a new surface of the specified format.
   *
   * This function is used to optimize images for faster *repeat* blitting. This
   * is accomplished by converting the original and storing the result as a new
   * surface. The new, optimized surface can then be used as the source for
   * future blits, making them faster.
   *
   * If you are converting to an indexed surface and want to map colors to a
   * palette, you can use SDL_ConvertSurfaceAndColorspace() instead.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * @param format the new pixel format.
   * @returns the new Surface that is created or NULL on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Surface Convert(PixelFormat format) const
  {
    return {SDL_ConvertSurface(T::get(), format)};
  }

  /**
   * Copy an existing surface to a new surface of the specified format and
   * colorspace.
   *
   * This function converts an existing surface to a new format and colorspace
   * and returns the new surface. This will perform any pixel format and
   * colorspace conversion needed.
   *
   * If the original surface has alternate images, the new surface will have a
   * reference to them as well.
   *
   * @param format the new pixel format.
   * @param palette an optional palette to use for indexed formats, may be NULL.
   * @param colorspace the new colorspace.
   * @param props an SDL_PropertiesID with additional color properties, or 0.
   * @returns the new SDL_Surface structure that is created or NULL on failure;
   *          call SDL_GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Surface Convert(PixelFormat format,
                  PaletteRef palette,
                  Colorspace colorspace,
                  PropertiesRef props) const
  {
    return SDL_ConvertSurfaceAndColorspace(
      T::get(), format, palette, colorspace, props);
  }

  /**
   * Premultiply the alpha in a surface.
   *
   * This is safe to use with src == dst, but not for other overlapping areas.
   *
   * @param linear true to convert from sRGB to linear space for the alpha
   *               multiplication, false to do multiplication in sRGB space.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool PremultiplyAlpha(bool linear)
  {
    return SDL_PremultiplySurfaceAlpha(T::get(), linear);
  }

  // TODO SDL_ConvertSurfaceAndColorspace

  /**
   * Clear a surface with a specific color, with floating point precision.
   *
   * This function handles all surface formats, and ignores any clip rectangle.
   *
   * If the surface is YUV, the color is assumed to be in the sRGB colorspace,
   * otherwise the color is assumed to be in the colorspace of the surface.
   *
   * @param color the color of the pixel, normally in the range 0-1.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Clear(SDL_FColor color)
  {
    return SDL_ClearSurface(T::get(), color.r, color.g, color.b, color.a);
  }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by SDL_MapRGB() or SDL_MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * the clip rectangle.
   *
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool Fill(SDL_Color color) { return Fill(MapColor(color)); }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by SDL_MapRGB() or SDL_MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * the clip rectangle.
   *
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool Fill(Uint32 color)
  {
    return SDL_FillSurfaceRect(T::get(), nullptr, color);
  }

  /**
   * Perform a fast fill of a rectangle with a specific color.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rect the SDL_Rect structure representing the rectangle to fill.
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool FillRect(const SDL_Rect& rect, SDL_Color color)
  {
    return FillRect(rect, MapColor(color));
  }

  /**
   * @brief Perform a fast fill of a rectangle with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by SDL_MapRGB() or SDL_MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rect the SDL_Rect structure representing the rectangle to fill.
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool FillRect(const SDL_Rect& rect, Uint32 color)
  {
    return SDL_FillSurfaceRect(T::get(), &rect, color);
  }

  /**
   * Perform a fast fill of a set of rectangles with a specific color.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rects an array of SDL_Rects representing the rectangles to fill.
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FillRect()
   */
  bool FillRects(SpanRef<const SDL_Rect> rects, SDL_Color color)
  {
    return FillRects(rects, MapColor(color));
  }

  /**
   * Perform a fast fill of a set of rectangles with a specific color.
   *
   * `color` should be a pixel of the format used by the surface, and can be
   * generated by SDL_MapRGB() or SDL_MapRGBA(). If the color value contains an
   * alpha component then the destination is simply filled with that alpha
   * information, no blending takes place.
   *
   * If there is a clip rectangle set on the destination (set via
   * SDL_SetSurfaceClipRect()), then this function will fill based on the
   * intersection of the clip rectangle and `rect`.
   *
   * @param rects an array of SDL_Rects representing the rectangles to fill.
   * @param color the color to fill with.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FillRect()
   */
  bool FillRects(SpanRef<const SDL_Rect> rects, Uint32 color)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_UINT32);
    return SDL_FillSurfaceRects(T::get(), rects.data(), rects.size(), color);
  }

  /**
   * Performs a fast blit from the source surface to the destination surface
   * with clipping.
   *
   * If either `srcrect` or `dstrect` are NULL, the entire surface (`src` or
   * `dst`) is copied while ensuring clipping to `dst->clip_rect`.
   *
   * The final blit rectangles are saved in `srcrect` and `dstrect` after all
   * clipping is performed.
   *
   * The blit function should not be called on a locked surface.
   *
   * The blit semantics for surfaces with and without blending and colorkey are
   * defined as follows:
   *
   * ```
   *    RGBA->RGB:
   *      Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGBA:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB, set destination alpha to source per-surface alpha value.
   *     both:
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       source color key.
   *
   *   RGBA->RGBA:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy all of RGBA to the destination.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGB:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB.
   *     both:
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       source color key.
   * ```
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param dstpos  the SDL_Point structure representing the x and y position in
   *                the destination surface, or NULL for (0,0). The width and
   *                height are ignored, and are copied from `srcrect`. If you
   *                want a specific width and height, you should use
   *                SDL_BlitSurfaceScaled().
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa BlitScaled()
   */
  bool Blit(SurfaceRef src,
            OptionalRef<const SDL_Rect> srcrect,
            const SDL_Point& dstpos)
  {
    return Blit(src, srcrect, Rect{dstpos, {}});
  }

  /**
   * Performs a fast blit from the source surface to the destination surface
   * with clipping.
   *
   * If either `srcrect` or `dstrect` are NULL, the entire surface (`src` or
   * `dst`) is copied while ensuring clipping to `dst->clip_rect`.
   *
   * The final blit rectangles are saved in `srcrect` and `dstrect` after all
   * clipping is performed.
   *
   * The blit function should not be called on a locked surface.
   *
   * The blit semantics for surfaces with and without blending and colorkey are
   * defined as follows:
   *
   * ```
   *    RGBA->RGB:
   *      Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGBA:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB, set destination alpha to source per-surface alpha value.
   *     both:
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       source color key.
   *
   *   RGBA->RGBA:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source alpha-channel and per-surface alpha)
   *       SDL_SRCCOLORKEY ignored.
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy all of RGBA to the destination.
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       RGB values of the source color key, ignoring alpha in the
   *       comparison.
   *
   *   RGB->RGB:
   *     Source surface blend mode set to SDL_BLENDMODE_BLEND:
   *       alpha-blend (using the source per-surface alpha)
   *     Source surface blend mode set to SDL_BLENDMODE_NONE:
   *       copy RGB.
   *     both:
   *       if SDL_SRCCOLORKEY set, only copy the pixels that do not match the
   *       source color key.
   * ```
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param dstrect the SDL_Point structure representing the x and y position in
   *                the destination surface, or NULL for (0,0). The width and
   *                height are ignored, and are copied from `srcrect`. If you
   *                want a specific width and height, you should use
   *                SDL_BlitSurfaceScaled().
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa BlitScaled()
   */
  bool Blit(SurfaceRef src,
            OptionalRef<const SDL_Rect> srcrect,
            OptionalRef<const SDL_Rect> dstrect)
  {
    return SDL_BlitSurface(src.get(), srcrect, T::get(), dstrect);
  }

  /**
   * Perform low-level surface blitting only.
   *
   * This is a semi-private blit function and it performs low-level surface
   * blitting, assuming the input rectangles have already been clipped.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, may not be NULL.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, may not be NULL.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool BlitUnchecked(SurfaceRef src,
                     const SDL_Rect& srcrect,
                     const SDL_Rect& dstrect)
  {
    return SDL_BlitSurfaceUnchecked(src.get(), srcrect, T::get(), dstrect);
  }

  /**
   * Perform a scaled blit to a destination surface, which may be of a different
   * format.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire
   *                destination surface.
   * @param scaleMode the SDL_ScaleMode to be used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool BlitScaled(SurfaceRef src,
                  OptionalRef<const SDL_Rect> srcrect,
                  OptionalRef<const SDL_Rect> dstrect,
                  ScaleMode scaleMode)
  {
    return SDL_BlitSurfaceScaled(
      src.get(), srcrect, T::get(), dstrect, scaleMode);
  }

  /**
   * Perform low-level surface scaled blitting only.
   *
   * This is a semi-private function and it performs low-level surface blitting,
   * assuming the input rectangles have already been clipped.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, may not be NULL.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, may not be NULL.
   * @param scaleMode the SDL_ScaleMode to be used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa BlitScaled()
   */
  bool BlitUncheckedScaled(SurfaceRef src,
                           const SDL_Rect& srcrect,
                           const SDL_Rect& dstrect,
                           ScaleMode scaleMode)
  {
    return SDL_BlitSurfaceScaled(
      src.get(), srcrect, T::get(), dstrect, scaleMode);
  }

#if SDL_VERSION_ATLEAST(3, 2, 4)
  /**
   * Perform a stretched pixel copy from one surface to another.
   *
   * @param src the Surface structure to be copied from.
   * @param srcrect the Rect structure representing the rectangle to be
   *                copied.
   * @param dstrect the Rect structure representing the target rectangle in
   *                the destination surface.
   * @param scaleMode the ScaleMode to be used.
   * @returns true on success or false on failure; GetError() for more
   *          information.
   *
   * @threadsafety Only one thread should be using the `src` and `dst` surfaces
   *               at any given time.
   *
   * @since This function is available since SDL 3.2.4.
   *
   * @sa BlitScaled()
   */
  bool Stretch(SurfaceRef src,
               const SDL_Rect& srcrect,
               const SDL_Rect& dstrect,
               ScaleMode scaleMode)
  {
    return SDL_StretchSurface(
      src.get(), &srcrect, T::get(), &dstrect, scaleMode);
  }
#endif

  /**
   * Perform a tiled blit to a destination surface, which may be of a different
   * format.
   *
   * The pixels in `srcrect` will be repeated as many times as needed to
   * completely fill `dstrect`.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool BlitTiled(SurfaceRef src,
                 OptionalRef<const SDL_Rect> srcrect,
                 OptionalRef<const SDL_Rect> dstrect)
  {
    return SDL_BlitSurfaceTiled(src.get(), srcrect, T::get(), dstrect);
  }

  /**
   * Perform a scaled and tiled blit to a destination surface, which may be of a
   * different format.
   *
   * The pixels in `srcrect` will be scaled and repeated as many times as needed
   * to completely fill `dstrect`.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param srcrect the SDL_Rect structure representing the rectangle to be
   *                copied, or NULL to copy the entire surface.
   * @param scale the scale used to transform srcrect into the destination
   *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
   *              64x64 tiles.
   * @param scaleMode scale algorithm to be used.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool BlitTiledWithScale(SurfaceRef src,
                          OptionalRef<const SDL_Rect> srcrect,
                          float scale,
                          SDL_ScaleMode scaleMode,
                          OptionalRef<const SDL_Rect> dstrect)
  {
    return SDL_BlitSurfaceTiledWithScale(
      src.get(), srcrect, scale, scaleMode, T::get(), dstrect);
  }

  /**
   * Perform a scaled blit using the 9-grid algorithm to a destination surface,
   * which may be of a different format.
   *
   * The pixels in the source surface are split into a 3x3 grid, using the
   * different corner sizes for each corner, and the sides and center making up
   * the remaining pixels. The corners are then scaled using `scale` and fit
   * into the corners of the destination rectangle. The sides and center are
   * then stretched into place to cover the remaining destination rectangle.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param left_width the width, in pixels, of the left corners in `srcrect`.
   * @param right_width the width, in pixels, of the right corners in `srcrect`.
   * @param top_height the height, in pixels, of the top corners in `srcrect`.
   * @param bottom_height the height, in pixels, of the bottom corners in
   *                      `srcrect`.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire surface.
   * @param srcrect the SDL_Rect structure representing the rectangle to be used
   *                for the 9-grid, or NULL to use the entire surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool Blit9Grid(SurfaceRef src,
                 OptionalRef<const SDL_Rect> srcrect,
                 int left_width,
                 int right_width,
                 int top_height,
                 int bottom_height,
                 OptionalRef<const SDL_Rect> dstrect)
  {
    return Blit9GridWithScale(src,
                              srcrect,
                              left_width,
                              right_width,
                              top_height,
                              bottom_height,
                              0.0,
                              SDL_SCALEMODE_NEAREST,
                              dstrect);
  }

  /**
   * Perform a scaled blit using the 9-grid algorithm to a destination surface,
   * which may be of a different format.
   *
   * The pixels in the source surface are split into a 3x3 grid, using the
   * different corner sizes for each corner, and the sides and center making up
   * the remaining pixels. The corners are then scaled using `scale` and fit
   * into the corners of the destination rectangle. The sides and center are
   * then stretched into place to cover the remaining destination rectangle.
   *
   * @param src the SDL_Surface structure to be copied from.
   * @param left_width the width, in pixels, of the left corners in `srcrect`.
   * @param right_width the width, in pixels, of the right corners in `srcrect`.
   * @param top_height the height, in pixels, of the top corners in `srcrect`.
   * @param bottom_height the height, in pixels, of the bottom corners in
   *                      `srcrect`.
   * @param scale the scale used to transform the corner of `srcrect` into the
   *              corner of `dstrect`, or 0.0f for an unscaled blit.
   * @param scaleMode scale algorithm to be used.
   * @param dstrect the SDL_Rect structure representing the target rectangle in
   *                the destination surface, or NULL to fill the entire surface.
   * @param srcrect the SDL_Rect structure representing the rectangle to be used
   *                for the 9-grid, or NULL to use the entire surface.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety The same destination surface should not be used from two
   *               threads at once. It is safe to use the same source surface
   *               from multiple threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Blit()
   */
  bool Blit9GridWithScale(SurfaceRef src,
                          OptionalRef<const SDL_Rect> srcrect,
                          int left_width,
                          int right_width,
                          int top_height,
                          int bottom_height,
                          float scale,
                          SDL_ScaleMode scaleMode,
                          OptionalRef<const SDL_Rect> dstrect)
  {
    return SDL_BlitSurface9Grid(src.get(),
                                srcrect,
                                left_width,
                                right_width,
                                top_height,
                                bottom_height,
                                scale,
                                scaleMode,
                                T::get(),
                                dstrect);
  }

  /**
   * Map an RGBA quadruple to a pixel value for a surface.
   *
   * This function maps the RGBA color value to the specified pixel format and
   * returns the pixel value best approximating the given RGBA color value for
   * the given pixel format.
   *
   * If the surface pixel format has no alpha component the alpha value will be
   * ignored (as it will be in formats with a palette).
   *
   * If the surface has a palette, the index of the closest matching color in
   * the palette will be returned.
   *
   * If the pixel format bpp (color depth) is less than 32-bpp then the unused
   * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
   * format the return value can be assigned to a Uint16, and similarly a Uint8
   * for an 8-bpp format).
   *
   * @param color the color components
   * @return a pixel value.
   */
  Uint32 MapColor(SDL_Color color) const
  {
    return MapColor(color.r, color.g, color.b, color.a);
  }

  /**
   * Map an RGB triple to an opaque pixel value for a surface.
   *
   * This function maps the RGB color value to the specified pixel format and
   * returns the pixel value best approximating the given RGB color value for
   * the given pixel format.
   *
   * If the surface has a palette, the index of the closest matching color in
   * the palette will be returned.
   *
   * If the surface pixel format has an alpha component it will be returned as
   * all 1 bits (fully opaque).
   *
   * If the pixel format bpp (color depth) is less than 32-bpp then the unused
   * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
   * format the return value can be assigned to a Uint16, and similarly a Uint8
   * for an 8-bpp format).
   *
   * @param r the red component of the pixel in the range 0-255.
   * @param g the green component of the pixel in the range 0-255.
   * @param b the blue component of the pixel in the range 0-255.
   * @returns a pixel value.
   */
  Uint32 MapColor(Uint8 r, Uint8 g, Uint8 b) const
  {
    return SDL_MapSurfaceRGB(T::get(), r, g, b);
  }

  /**
   * Map an RGBA quadruple to a pixel value for a surface.
   *
   * This function maps the RGBA color value to the specified pixel format and
   * returns the pixel value best approximating the given RGBA color value for
   * the given pixel format.
   *
   * If the surface pixel format has no alpha component the alpha value will be
   * ignored (as it will be in formats with a palette).
   *
   * If the surface has a palette, the index of the closest matching color in
   * the palette will be returned.
   *
   * If the pixel format bpp (color depth) is less than 32-bpp then the unused
   * upper bits of the return value can safely be ignored (e.g., with a 16-bpp
   * format the return value can be assigned to a Uint16, and similarly a Uint8
   * for an 8-bpp format).
   *
   * @param r the red component of the pixel in the range 0-255.
   * @param g the green component of the pixel in the range 0-255.
   * @param b the blue component of the pixel in the range 0-255.
   * @param a the alpha component of the pixel in the range 0-255.
   * @return a pixel value.
   */
  Uint32 MapColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
  {
    return SDL_MapSurfaceRGBA(T::get(), r, g, b, a);
  }

  /**
   * @brief This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like SDL_GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @returns color  on success or std::nullopt on failure; call GetError() for
   * more information.
   */
  std::optional<Color> ReadPixel(int x, int y) const
  {
    if (Color c; ReadPixel(x, y, &c)) return c;
    return std::nullopt;
  }

  /**
   * @brief This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like SDL_GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param c a color pointer to be filled with the color information. Must not
   *          be NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, SDL_Color* c) const
  {
    return ReadPixel(x, y, &c->r, &c->g, &c->b, &c->a);
  }

  /**
   * @brief Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param c a color pointer to be filled with the color information. Must not
   *          be NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, SDL_FColor* c) const
  {
    return ReadPixel(x, y, &c->r, &c->g, &c->b, &c->a);
  }

  /**
   * Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * Like SDL_GetRGBA, this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r a pointer filled in with the red channel, 0-255, or NULL to ignore
   *          this channel.
   * @param g a pointer filled in with the green channel, 0-255, or NULL to
   *          ignore this channel.
   * @param b a pointer filled in with the blue channel, 0-255, or NULL to
   *          ignore this channel.
   * @param a a pointer filled in with the alpha channel, 0-255, or NULL to
   *          ignore this channel.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadPixel(int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a) const
  {
    return SDL_ReadSurfacePixel(T::get(), x, y, r, g, b, a);
  }

  /**
   * @brief Retrieves a single pixel from a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r a pointer filled in with the red channel, normally in the range
   *          0-1, or NULL to ignore this channel.
   * @param g a pointer filled in with the green channel, normally in the range
   *          0-1, or NULL to ignore this channel.
   * @param b a pointer filled in with the blue channel, normally in the range
   *          0-1, or NULL to ignore this channel.
   * @param a a pointer filled in with the alpha channel, normally in the range
   *          0-1, or NULL to ignore this channel.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool ReadPixel(int x, int y, float* r, float* g, float* b, float* a) const
  {
    return SDL_ReadSurfacePixelFloat(T::get(), x, y, r, g, b, a);
  }

  /**
   * @brief This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like MapRGBA(), this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param c the color values, 0-255.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool WritePixel(int x, int y, SDL_Color c)
  {
    return WritePixel(x, y, c.r, c.g, c.b, c.a);
  }

  /**
   * @brief Writes a single pixel to a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param c the color values, normally in the range 0-1.
   * @returns true on success or false on failure; call GetError() for
   * more information.
   */
  bool WritePixel(int x, int y, SDL_FColor c)
  {
    return WritePixel(x, y, c.r, c.g, c.b, c.a);
  }

  /**
   * @brief This function prioritizes correctness over speed: it is suitable for
   * unit tests, but is not intended for use in a game engine.
   *
   * Like MapRGBA(), this uses the entire 0..255 range when converting color
   * components from pixel formats with less than 8 bits per RGB component.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r the red channel value, 0-255.
   * @param g the green channel value, 0-255.
   * @param b the blue channel value, 0-255.
   * @param a the alpha channel value, 0-255.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool WritePixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
  {
    return SDL_WriteSurfacePixel(T::get(), x, y, r, g, b, a);
  }

  /**
   * @brief Writes a single pixel to a surface.
   *
   * This function prioritizes correctness over speed: it is suitable for unit
   * tests, but is not intended for use in a game engine.
   *
   * @param x the horizontal coordinate, 0 <= x < width.
   * @param y the vertical coordinate, 0 <= y < height.
   * @param r the red channel value, normally in the range 0-1.
   * @param g the green channel value, normally in the range 0-1.
   * @param b the blue channel value, normally in the range 0-1.
   * @param a the alpha channel value, normally in the range 0-1.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   */
  bool WritePixel(int x, int y, float r, float g, float b, float a)
  {
    return SDL_WriteSurfacePixelFloat(T::get(), x, y, r, g, b, a);
  }

  // Convenience functions to avoid dereferencing
  int GetWidth() const { return T::get()->w; }

  int GetHeight() const { return T::get()->w; }

  Point GetSize() const { return Point(GetWidth(), GetHeight()); }

  PixelFormat GetFormat() const { return T::get()->format; }
};

/**
 * @brief Locks a Surface for access to its pixels
 *
 * Only really necessary if Surface.MustLock() returns t
 */
class SurfaceLock
{
  SurfaceRef surface;

  /**
   * @sa SurfaceBase.Lock()
   */
  explicit SurfaceLock(SurfaceRef surface)
    : surface(std::move(surface))
  {
    if (!SDL_LockSurface(this->surface.get())) this->surface.reset();
  }

public:
  // default ctor
  SurfaceLock()
    : surface(nullptr)
  {
  }

  // Copy ctor
  SurfaceLock(const SurfaceLock& other) = delete;

  // Move ctor
  SurfaceLock(SurfaceLock&& other)
    : surface(other.surface.release())
  {
  }

  /**
   * destructor
   * @sa Unlock()
   */
  ~SurfaceLock() { Unlock(); }

  // Assignment operator
  SurfaceLock& operator=(SurfaceLock other)
  {
    std::swap(surface, other.surface);
    return *this;
  }

  /**
   * @brief Returns true if lock is active
   */
  constexpr operator bool() const { return bool(surface); }

  /**
   * Release the locked surface after directly accessing the pixels.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Surface.Lock()
   */
  void Unlock() { return SDL_UnlockSurface(surface.release()); }

  /**
   * Get the pixels
   */
  void* GetPixels() const { return surface->pixels; }

  /**
   * Get pitch (the number of bytes between the start of one row the next)
   */
  int GetPitch() const { return surface->pitch; }

  /**
   * @brief Get the pixel format
   */
  PixelFormat GetFormat() const { return surface->format; }

  template<ObjectBox<SDL_Surface*> T>
  friend class SurfaceBase;
};

/**
 * Load a BMP image from a seekable SDL data stream.
 *
 * @param src the data stream for the surface.
 * @param closeio if true, calls SDL_CloseIO() on `src` before returning, even
 *                in the case of an error.
 * @returns a Surface with the loaded content or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SaveBMP()
 */
inline Surface LoadBMP(SDL_IOStream* src, bool closeio)
{
  return Surface{SDL_LoadBMP_IO(src, closeio)};
}

/**
 * Load a BMP image from a file.
 *
 * @param file the BMP file to load.
 * @returns a Surface with the loaded content or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SaveBMP()
 */
inline Surface LoadBMP(StringParam file) { return Surface{SDL_LoadBMP(file)}; }

/**
 * Save a surface to a seekable SDL data stream in BMP format.
 *
 * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
 * BMP directly. Other RGB formats with 8-bit or higher get converted to a
 * 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
 * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
 * not supported.
 *
 * @param surface the SDL_Surface structure containing the image to be saved.
 * @param dst a data stream to save to.
 * @param closeio if true, calls SDL_CloseIO() on `dst` before returning, even
 *                in the case of an error.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SaveBMP()
 */
inline bool SaveBMP(SurfaceRef surface, SDL_IOStream* dst, bool closeio)
{
  return SDL_SaveBMP_IO(surface.get(), dst, closeio);
}

/**
 * Save a surface to a file.
 *
 * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
 * BMP directly. Other RGB formats with 8-bit or higher get converted to a
 * 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
 * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
 * not supported.
 *
 * @param surface the SDL_Surface structure containing the image to be saved.
 * @param file a file to save to.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LoadBMP()
 */
inline bool SaveBMP(SurfaceRef surface, StringParam file)
{
  return SDL_SaveBMP(surface.get(), file);
}

/**
 * Copy a block of pixels of one format to another format.
 *
 * @param width the width of the block to copy, in pixels.
 * @param height the height of the block to copy, in pixels.
 * @param src_format an SDL_PixelFormat value of the `src` pixels format.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an SDL_PixelFormat value of the `dst` pixels format.
 * @param dst a pointer to be filled in with new pixel data.
 * @param dst_pitch the pitch of the destination pixels, in bytes.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ConvertPixelsAndColorspace()
 */
inline bool ConvertPixels(int width,
                          int height,
                          PixelFormat src_format,
                          const void* src,
                          int src_pitch,
                          PixelFormat dst_format,
                          void* dst,
                          int dst_pitch)
{
  return SDL_ConvertPixels(
    width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch);
}

/**
 * Copy a block of pixels of one format and colorspace to another format and
 * colorspace.
 *
 * @param width the width of the block to copy, in pixels.
 * @param height the height of the block to copy, in pixels.
 * @param src_format an SDL_PixelFormat value of the `src` pixels format.
 * @param src_colorspace an SDL_Colorspace value describing the colorspace of
 *                       the `src` pixels.
 * @param src_properties an SDL_PropertiesID with additional source color
 *                       properties, or 0.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an SDL_PixelFormat value of the `dst` pixels format.
 * @param dst_colorspace an SDL_Colorspace value describing the colorspace of
 *                       the `dst` pixels.
 * @param dst_properties an SDL_PropertiesID with additional destination color
 *                       properties, or 0.
 * @param dst a pointer to be filled in with new pixel data.
 * @param dst_pitch the pitch of the destination pixels, in bytes.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ConvertPixels()
 */
inline bool ConvertPixelsAndColorspace(int width,
                                       int height,
                                       PixelFormat src_format,
                                       Colorspace src_colorspace,
                                       PropertiesRef src_properties,
                                       const void* src,
                                       int src_pitch,
                                       PixelFormat dst_format,
                                       Colorspace dst_colorspace,
                                       PropertiesRef dst_properties,
                                       void* dst,
                                       int dst_pitch)
{
  return SDL_ConvertPixelsAndColorspace(width,
                                        height,
                                        src_format,
                                        src_colorspace,
                                        src_properties,
                                        src,
                                        src_pitch,
                                        dst_format,
                                        dst_colorspace,
                                        dst_properties,
                                        dst,
                                        dst_pitch);
}

/**
 * Premultiply the alpha on a block of pixels.
 *
 * This is safe to use with src == dst, but not for other overlapping areas.
 *
 * @param width the width of the block to convert, in pixels.
 * @param height the height of the block to convert, in pixels.
 * @param src_format an SDL_PixelFormat value of the `src` pixels format.
 * @param src a pointer to the source pixels.
 * @param src_pitch the pitch of the source pixels, in bytes.
 * @param dst_format an SDL_PixelFormat value of the `dst` pixels format.
 * @param dst a pointer to be filled in with premultiplied pixel data.
 * @param dst_pitch the pitch of the destination pixels, in bytes.
 * @param linear true to convert from sRGB to linear space for the alpha
 *               multiplication, false to do multiplication in sRGB space.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool PremultiplyAlpha(int width,
                             int height,
                             PixelFormat src_format,
                             const void* src,
                             int src_pitch,
                             PixelFormat dst_format,
                             void* dst,
                             int dst_pitch,
                             bool linear)
{
  return SDL_PremultiplyAlpha(width,
                              height,
                              src_format,
                              src,
                              src_pitch,
                              dst_format,
                              dst,
                              dst_pitch,
                              linear);
}

#pragma region impl
/// @}

inline void ObjectDeleter<SDL_Surface>::operator()(SurfaceRef surface) const
{
  surface.Destroy();
}

template<ObjectBox<SDL_Surface*> T>
SurfaceLock SurfaceBase<T>::Lock() &
{
  return SurfaceLock{*this};
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_SURFACE_H_ */


// end --- SDL3pp_surface.h --- 



namespace SDL {
/**
 * @defgroup CategoryVideo Display and Window Management
 *
 * SDL's video subsystem is largely interested in abstracting window
 * management from the underlying operating system. You can create windows,
 * manage them in various ways, set them fullscreen, and get events when
 * interesting things happen with them, such as the mouse or keyboard
 * interacting with a window.
 *
 * The video subsystem is also interested in abstracting away some
 * platform-specific differences in OpenGL: context creation, swapping
 * buffers, etc. This may be crucial to your app, but also you are not
 * required to use OpenGL at all. In fact, SDL can provide rendering to those
 * windows as well, either with an easy-to-use
 * [2D API](https://wiki.libsdl.org/SDL3/CategoryRender)
 * or with a more-powerful
 * [GPU API](https://wiki.libsdl.org/SDL3/CategoryGPU)
 * . Of course, it can simply get out of your way and give you the window
 * handles you need to use Vulkan, Direct3D, Metal, or whatever else you like
 * directly, too.
 *
 * The video subsystem covers a lot of functionality, out of necessity, so it
 * is worth perusing the list of functions just to see what's available, but
 * most apps can get by with simply creating a window and listening for
 * events, so start with SDL_CreateWindow() and SDL_PollEvent().
 * @{
 */

// Forward decl
template<ObjectBox<SDL_Window*> T>
struct WindowBase;

/**
 * @brief Handle to a non owned window
 */
using WindowRef = WindowBase<ObjectRef<SDL_Window>>;

template<>
struct ObjectDeleter<SDL_Window>
{
  void operator()(WindowRef window) const;
};

/**
 * @brief Handle to an owned window
 */
using Window = WindowBase<ObjectUnique<SDL_Window>>;

// Forward decl
template<ObjectBox<SDL_Renderer*> T>
struct RendererBase;

using RendererRef = RendererBase<ObjectRef<SDL_Renderer>>;

/**
 * @name DisplayOrientations
 * @{
 */

/**
 * Display orientation values; the way a display is rotated.
 *
 * @since This enum is available since SDL 3.2.0.
 * @sa ORIENTATION_LANDSCAPE
 * @sa ORIENTATION_PORTRAIT
 * @sa ORIENTATION_UNKNOWN
 */
using DisplayOrientation = SDL_DisplayOrientation;

/**
 * The display orientation can't be determined
 */
constexpr DisplayOrientation ORIENTATION_UNKNOWN = SDL_ORIENTATION_UNKNOWN;

/**
 * The display is in landscape mode, with the right side up, relative to
 * portrait mode
 */
constexpr DisplayOrientation ORIENTATION_LANDSCAPE = SDL_ORIENTATION_LANDSCAPE;

/**
 * The display is in landscape mode, with the left side up, relative to portrait
 * mode
 */
constexpr DisplayOrientation ORIENTATION_LANDSCAPE_FLIPPED =
  SDL_ORIENTATION_LANDSCAPE_FLIPPED;

/**
 * The display is in portrait mode
 */
constexpr DisplayOrientation ORIENTATION_PORTRAIT = SDL_ORIENTATION_PORTRAIT;

/**
 * The display is in portrait mode, upside down
 */
constexpr DisplayOrientation ORIENTATION_PORTRAIT_FLIPPED =
  SDL_ORIENTATION_PORTRAIT_FLIPPED;

/// @}

/**
 * Internal display mode data.
 *
 * This lives as a field in SDL_DisplayMode, as opaque data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa DisplayMode()
 */
using DisplayModeData = SDL_DisplayModeData;

/**
 * The structure that defines a display mode.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Display.GetFullscreenModes()
 * @sa Display.GetDesktopMode()
 * @sa Display.GetCurrentMode()
 * @sa WindowBase.SetFullscreenMode()
 * @sa WindowBase.GetFullscreenMode()
 */
using DisplayMode = SDL_DisplayMode;

/**
 * The flags on a window.
 *
 * These cover a lot of true/false, or on/off, window state. Some of it is
 * immutable after being set through Window::Window(), some of it can be
 * changed on existing windows by the app, and some of it might be altered by
 * the user or system outside of the app's control.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa WindowBase.GetFlags()
 */
using WindowFlags = SDL_WindowFlags;

/**
 * @name FlashOperations
 * @{
 */

/**
 * Window flash operation.
 *
 * @since This enum is available since SDL 3.2.0.
 * @sa FLASH_CANCEL
 * @sa FLASH_BRIEFLY
 * @sa FLASH_UNTIL_FOCUSED
 */
using FlashOperation = SDL_FlashOperation;

/**
 * Cancel any window flash state
 */
constexpr FlashOperation FLASH_CANCEL = SDL_FLASH_CANCEL;

/**
 * Flash the window briefly to get attention
 */
constexpr FlashOperation FLASH_BRIEFLY = SDL_FLASH_BRIEFLY;

/**
 * Flash the window until it gets focus
 */
constexpr FlashOperation FLASH_UNTIL_FOCUSED = SDL_FLASH_UNTIL_FOCUSED;

/// @}

/**
 * @name HitTestResults
 * Possible return values from the HitTest callback
 *
 * @{
 */

/**
 * Possible return values from the HitTest callback.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa HitTest
 * @sa HITTEST_NORMAL
 */
using HitTestResult = SDL_HitTestResult;

/**
 * Region is normal. No special properties.
 */
constexpr HitTestResult HITTEST_NORMAL = SDL_HITTEST_NORMAL;

/**
 * Region can drag entire window.
 */
constexpr HitTestResult HITTEST_DRAGGABLE = SDL_HITTEST_DRAGGABLE;

/**
 * Region is the resizable top-left corner border.
 */
constexpr HitTestResult HITTEST_RESIZE_TOPLEFT = SDL_HITTEST_RESIZE_TOPLEFT;

/**
 * Region is the resizable top border.
 */
constexpr HitTestResult HITTEST_RESIZE_TOP = SDL_HITTEST_RESIZE_TOP;

/**
 * Region is the resizable top-right corner border.
 */
constexpr HitTestResult HITTEST_RESIZE_TOPRIGHT = SDL_HITTEST_RESIZE_TOPRIGHT;

/**
 * Region is the resizable right border.
 */
constexpr HitTestResult HITTEST_RESIZE_RIGHT = SDL_HITTEST_RESIZE_RIGHT;

/**
 * Region is the resizable bottom-right corner border.
 */
constexpr HitTestResult HITTEST_RESIZE_BOTTOMRIGHT =
  SDL_HITTEST_RESIZE_BOTTOMRIGHT;

/**
 * Region is the resizable bottom border.
 */
constexpr HitTestResult HITTEST_RESIZE_BOTTOM = SDL_HITTEST_RESIZE_BOTTOM;

/**
 * Region is the resizable bottom-left corner border.
 */
constexpr HitTestResult HITTEST_RESIZE_BOTTOMLEFT =
  SDL_HITTEST_RESIZE_BOTTOMLEFT;

/**
 * Region is the resizable left border.
 */
constexpr HitTestResult HITTEST_RESIZE_LEFT = SDL_HITTEST_RESIZE_LEFT;

/// @}

/**
 * Callback used for hit-testing.
 *
 * @param win the SDL_Window where hit-testing was set on.
 * @param area an SDL_Point which should be hit-tested.
 * @param data what was passed as `callback_data` to WindowBase::SetHitTest().
 * @returns an SDL::HitTestResult value.
 *
 * @sa WindowBase::SetHitTest()
 */
using HitTest = SDL_HitTest;

/**
 * This is a unique ID for a display for the time it is connected to the
 * system, and is never reused for the lifetime of the application.
 *
 * If the display is disconnected and reconnected, it will get a new ID.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
struct Display
{
  SDL_DisplayID displayID;

  constexpr Display(SDL_DisplayID displayID = 0)
    : displayID(displayID)
  {
  }

  constexpr operator bool() const { return displayID != 0; }

  /**
   * Get a list of currently connected displays.
   *
   * @param count a pointer filled in with the number of displays returned, may
   *              be NULL.
   * @returns a 0 terminated array of display instance IDs or NULL on failure;
   *          call GetError() for more information. This should be freed
   *          with SDL_free() when it is no longer needed.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static FreeWrapper<Display[]> GetAll(int* count)
  {
    return wrapArray(reinterpret_cast<Display*>(SDL_GetDisplays(count)));
  }

  /**
   * Return the primary display.
   *
   * @returns the instance ID of the primary display on success or 0 on failure;
   *          call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDisplays()
   */
  static Display GetPrimary() { return {SDL_GetPrimaryDisplay()}; }

  /**
   * Get the properties associated with a display.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN`: true if the display has HDR
   *   headroom above the SDR white point. This is for informational and
   *   diagnostic purposes only, as not all platforms provide this information
   *   at the display level.
   *
   * On KMS/DRM:
   *
   * - `SDL_PROP_DISPLAY_KMSDRM_PANEL_ORIENTATION_NUMBER`: the "panel
   *   orientation" property for the display in degrees of clockwise rotation.
   *   Note that this is provided only as a hint, and the application is
   *   responsible for any coordinate transformations needed to conform to the
   *   requested display orientation.
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return PropertiesRef{SDL_GetDisplayProperties(displayID)};
  }

  /**
   * @brief Get the name of a display in UTF-8 encoding.
   *
   * @returns the name of a display or NULL on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  const char* GetName() const { return SDL_GetDisplayName(displayID); }

  /**
   * @brief Get the desktop area represented by a display.
   *
   * The primary display is often located at (0,0), but may be placed at a
   * different location depending on monitor layout.
   *
   * @returns the Rect structure filled in with the display bounds on success
   * or std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  std::optional<Rect> GetBounds() const
  {
    Rect bounds;
    if (SDL_GetDisplayBounds(displayID, &bounds)) return bounds;
    return std::nullopt;
  }

  /**
   * @brief Get the usable desktop area represented by a display, in screen
   * coordinates.
   *
   * This is the same area as Display.GetBounds() reports, but with portions
   * reserved by the system removed. For example, on Apple's macOS, this
   * subtracts the area occupied by the menu bar and dock.
   *
   * Setting a window to be fullscreen generally bypasses these unusable areas,
   * so these are good guidelines for the maximum space available to a
   * non-fullscreen window.
   *
   * @returns he Rect structure filled in with the display bounds on success
   * or std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  std::optional<Rect> GetUsableBounds() const
  {
    Rect bounds;
    if (SDL_GetDisplayUsableBounds(displayID, &bounds)) return bounds;
    return {};
  }

  /**
   * @brief Get the orientation of a display when it is unrotated.
   *
   * @returns the SDL_DisplayOrientation enum value of the display, or
   *          `SDL_ORIENTATION_UNKNOWN` if it isn't available.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa GetDisplays()
   * @sa Display.GetAll()
   */
  DisplayOrientation GetNaturalOrientation() const
  {
    return SDL_GetNaturalDisplayOrientation(displayID);
  }

  /**
   * @brief  Get the orientation of a display.
   *
   * @returns the SDL_DisplayOrientation enum value of the display, or
   *          `SDL_ORIENTATION_UNKNOWN` if it isn't available.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa Display.GetAll()
   * @sa GetDisplays()
   */
  DisplayOrientation GetCurrentOrientation() const
  {
    return SDL_GetCurrentDisplayOrientation(displayID);
  }

  /**
   * @brief Get the content scale of a display.
   *
   * @returns the content scale of the display, or 0.0f on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa Display.GetAll()
   * @sa WindowBase.GetDisplayScale()
   * @sa GetDisplays()
   */
  float GetContentScale() const
  {
    return SDL_GetDisplayContentScale(displayID);
  }

  /**
   * @brief Get a list of fullscreen display modes available on a display.
   *
   * The display modes are sorted in this priority:
   *
   * - w -> largest to smallest
   * - h -> largest to smallest
   * - bits per pixel -> more colors to fewer colors
   * - packed pixel layout -> largest to smallest
   * - refresh rate -> highest to lowest
   * - pixel density -> lowest to highest
   *
   * @param count a pointer filled in with the number of display modes returned,
   *              may be NULL.
   * @returns a NULL terminated array of display mode pointers or NULL on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * This automatically calls SDL_free after result is out of scope.
   *
   * @sa Display.GetAll()
   * @sa GetDisplays()
   */
  FreeWrapper<DisplayMode*[]> GetFullscreenModes(int* count = nullptr) const
  {
    return wrapArray(SDL_GetFullscreenDisplayModes(displayID, count));
  }

  /**
   * Get the closest match to the requested display mode.
   *
   * The available display modes are scanned and `closest` is filled in with the
   * closest mode matching the requested mode and returned. The mode format and
   * refresh rate default to the desktop mode if they are set to 0. The modes
   * are scanned with size being first priority, format being second priority,
   * and finally checking the refresh rate. If all the available modes are too
   * small, then false is returned.
   *
   * @param w the width in pixels of the desired display mode.
   * @param h the height in pixels of the desired display mode.
   * @param refresh_rate the refresh rate of the desired display mode, or 0.0f
   *                     for the desktop refresh rate.
   * @param include_high_density_modes boolean to include high density modes in
   *                                   the search.
   * @returns the closest display mode equal to or larger than the desired mode
   * on success or std::nullopt on failure; call GetError() for more
   * information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Display.GetAll()
   * @sa Display.GetFullscreenModes()
   */
  std::optional<DisplayMode> GetClosestFullscreenMode(
    int w,
    int h,
    float refresh_rate,
    bool include_high_density_modes) const
  {
    if (SDL_DisplayMode closest;
        SDL_GetClosestFullscreenDisplayMode(displayID,
                                            w,
                                            h,
                                            refresh_rate,
                                            include_high_density_modes,
                                            &closest)) {
      return closest;
    }
    return {};
  }

  /**
   * @brief Get information about the desktop's display mode.
   *
   * There's a difference between this function and SDL_GetCurrentDisplayMode()
   * when SDL runs fullscreen and has changed the resolution. In that case this
   * function will return the previous native display mode, and not the current
   * display mode.
   *
   * @returns a pointer to the desktop display mode or NULL on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa Display.GetCurrentMode()
   * @sa Display.GetAll()
   * @sa GetDisplays()
   */
  const DisplayMode* GetDesktopMode() const
  {
    return SDL_GetDesktopDisplayMode(displayID);
  }

  /**
   * @brief Get information about the current display mode.
   *
   * There's a difference between this function and SDL_GetDesktopDisplayMode()
   * when SDL runs fullscreen and has changed the resolution. In that case this
   * function will return the current display mode, and not the previous native
   * display mode.
   *
   * @returns a pointer to the desktop display mode or NULL on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa Display.GetDesktopMode()
   * @sa Display.GetAll()
   * @sa GetDisplays()
   */
  const DisplayMode* GetCurrentMode() const
  {
    return SDL_GetCurrentDisplayMode(displayID);
  }

  /**
   * Get the display containing a point.
   *
   * @param point the point to query.
   * @returns the instance ID of the display containing the point or 0 on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBounds()
   * @sa GetAll()
   */
  static Display GetForPoint(const SDL_Point& point)
  {
    return {SDL_GetDisplayForPoint(&point)};
  }

  /**
   * @brief Get the display primarily containing a rect.
   * @param rect the rect to query.
   * @returns the instance ID of the display entirely containing the rect or
   *          closest to the center of the rect on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  static Display GetForRect(const SDL_Rect& rect)
  {
    return {SDL_GetDisplayForRect(&rect)};
  }

  /**
   * @brief Get the display associated with a window.
   * @param window the window to query.
   * @returns the instance ID of the display containing the center of the window
   *          on success or 0 on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  static Display GetForWindow(WindowRef window);
};

/**
 * This is a unique ID for a window.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using WindowID = SDL_WindowID;

/**
 * @name SystemThemes
 * @{
 */

/**
 * System theme.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using SystemTheme = SDL_SystemTheme;

/**
 * Unknown system theme
 */
constexpr SystemTheme SYSTEM_THEME_UNKNOWN = SDL_SYSTEM_THEME_UNKNOWN;

/**
 * Light colored system theme
 */
constexpr SystemTheme SYSTEM_THEME_LIGHT = SDL_SYSTEM_THEME_LIGHT;

/**
 * Dark colored system theme
 */
constexpr SystemTheme SYSTEM_THEME_DARK = SDL_SYSTEM_THEME_DARK;

/// @}

/**
 * @brief Represents a handle to a window
 * @ingroup resource
 */
template<ObjectBox<SDL_Window*> T>
struct WindowBase : T
{
  using T::T;

  /**
   * Create a window with the specified dimensions and flags.
   *
   * `flags` may be any of the following OR'd together:
   *
   * - `SDL_WINDOW_FULLSCREEN`: fullscreen window at desktop resolution
   * - `SDL_WINDOW_OPENGL`: window usable with an OpenGL context
   * - `SDL_WINDOW_OCCLUDED`: window partially or completely obscured by another
   *   window
   * - `SDL_WINDOW_HIDDEN`: window is not visible
   * - `SDL_WINDOW_BORDERLESS`: no window decoration
   * - `SDL_WINDOW_RESIZABLE`: window can be resized
   * - `SDL_WINDOW_MINIMIZED`: window is minimized
   * - `SDL_WINDOW_MAXIMIZED`: window is maximized
   * - `SDL_WINDOW_MOUSE_GRABBED`: window has grabbed mouse focus
   * - `SDL_WINDOW_INPUT_FOCUS`: window has input focus
   * - `SDL_WINDOW_MOUSE_FOCUS`: window has mouse focus
   * - `SDL_WINDOW_EXTERNAL`: window not created by SDL
   * - `SDL_WINDOW_MODAL`: window is modal
   * - `SDL_WINDOW_HIGH_PIXEL_DENSITY`: window uses high pixel density back
   *   buffer if possible
   * - `SDL_WINDOW_MOUSE_CAPTURE`: window has mouse captured (unrelated to
   *   MOUSE_GRABBED)
   * - `SDL_WINDOW_ALWAYS_ON_TOP`: window should always be above others
   * - `SDL_WINDOW_UTILITY`: window should be treated as a utility window, not
   *   showing in the task bar and window list
   * - `SDL_WINDOW_TOOLTIP`: window should be treated as a tooltip and does not
   *   get mouse or keyboard focus, requires a parent window
   * - `SDL_WINDOW_POPUP_MENU`: window should be treated as a popup menu,
   *   requires a parent window
   * - `SDL_WINDOW_KEYBOARD_GRABBED`: window has grabbed keyboard input
   * - `SDL_WINDOW_VULKAN`: window usable with a Vulkan instance
   * - `SDL_WINDOW_METAL`: window usable with a Metal instance
   * - `SDL_WINDOW_TRANSPARENT`: window with transparent buffer
   * - `SDL_WINDOW_NOT_FOCUSABLE`: window should not be focusable
   *
   * The SDL_Window is implicitly shown if SDL_WINDOW_HIDDEN is not set.
   *
   * On Apple's macOS, you **must** set the NSHighResolutionCapable Info.plist
   * property to YES, otherwise you will not receive a High-DPI OpenGL canvas.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use SDL_GetWindowSize() to query
   * the client area's size in window coordinates, and
   * SDL_GetWindowSizeInPixels() or SDL_GetRenderOutputSize() to query the
   * drawable size in pixels. Note that the drawable size can vary after the
   * window is created and should be queried again if you get an
   * SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED event.
   *
   * If the window is created with any of the SDL_WINDOW_OPENGL or
   * SDL_WINDOW_VULKAN flags, then the corresponding LoadLibrary function
   * (SDL_GL_LoadLibrary or SDL_Vulkan_LoadLibrary) is called and the
   * corresponding UnloadLibrary function is called by SDL_DestroyWindow().
   *
   * If SDL_WINDOW_VULKAN is specified and there isn't a working Vulkan driver,
   * this will fail, because SDL_Vulkan_LoadLibrary() will fail.
   *
   * If SDL_WINDOW_METAL is specified on an OS that does not support Metal,
   * this will fail.
   *
   * If you intend to use this window with an SDL_Renderer, you should use
   * CreateWindowAndRenderer() instead of this function, to avoid window
   * flicker.
   *
   * On non-Apple devices, SDL requires you to either not link to the Vulkan
   * loader or link to a dynamic library version. This limitation may be removed
   * in a future version of SDL.
   *
   * @param title the title of the window, in UTF-8 encoding.
   * @param size the width and height of the window.
   * @param flags 0, or one or more WindowFlags OR'd together.
   * @post the window that was created, convertible to true on success or
   * convertible to false on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa CreateWindowAndRenderer()
   */
  WindowBase(StringParam title, SDL_Point size, WindowFlags flags = 0)
    : T(SDL_CreateWindow(title, size.x, size.y, flags))
  {
  }

  /**
   * Create a child popup window of the specified parent window.
   *
   * The flags parameter **must** contain at least one of the following:
   *
   * - `SDL_WINDOW_TOOLTIP`: The popup window is a tooltip and will not pass any
   *   input events.
   * - `SDL_WINDOW_POPUP_MENU`: The popup window is a popup menu. The topmost
   *   popup menu will implicitly gain the keyboard focus.
   *
   * The following flags are not relevant to popup window creation and will be
   * ignored:
   *
   * - `SDL_WINDOW_MINIMIZED`
   * - `SDL_WINDOW_MAXIMIZED`
   * - `SDL_WINDOW_FULLSCREEN`
   * - `SDL_WINDOW_BORDERLESS`
   *
   * The following flags are incompatible with popup window creation and will
   * cause it to fail:
   *
   * - `SDL_WINDOW_UTILITY`
   * - `SDL_WINDOW_MODAL`
   *
   * The parent parameter **must** be non-null and a valid window. The parent of
   * a popup window can be either a regular, toplevel window, or another popup
   * window.
   *
   * Popup windows cannot be minimized, maximized, made fullscreen, raised,
   * flash, be made a modal window, be the parent of a toplevel window, or grab
   * the mouse and/or keyboard. Attempts to do so will fail.
   *
   * Popup windows implicitly do not have a border/decorations and do not appear
   * on the taskbar/dock or in lists of windows such as alt-tab menus.
   *
   * If a parent window is hidden or destroyed, any child popup windows will be
   * recursively hidden or destroyed as well. Child popup windows not explicitly
   * hidden will be restored when the parent is shown.
   *
   * @param parent the parent of the window, must not be NULL.
   * @param offset the x, y position of the popup window relative to the origin
   *               of the parent.
   * @param size the width and height of the window.
   * @param flags SDL_WINDOW_TOOLTIP or SDL_WINDOW_POPUP_MENU, and zero or more
   *              additional SDL_WindowFlags OR'd together.
   * @post the window that was created, convertible to true on success or
   * convertible to false on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetParent()
   */
  WindowBase(WindowRef parent,
             SDL_Point offset,
             SDL_Point size,
             WindowFlags flags = 0)
    : T(SDL_CreatePopupWindow(parent.get(),
                              offset.x,
                              offset.y,
                              size.x,
                              size.y,
                              flags))
  {
  }

  /**
   * Create a window with the specified properties.
   *
   * These are the supported properties:
   *
   * - `SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN`: true if the window should
   *   be always on top
   * - `SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN`: true if the window has no
   *   window decoration
   * - `SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN`: true if the
   *   window will be used with an externally managed graphics context.
   * - `SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN`: true if the window should
   *   accept keyboard input (defaults true)
   * - `SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN`: true if the window should
   *   start in fullscreen mode at desktop resolution
   * - `SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER`: the height of the window
   * - `SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN`: true if the window should start
   *   hidden
   * - `SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN`: true if the window
   *   uses a high pixel density buffer if possible
   * - `SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN`: true if the window should
   *   start maximized
   * - `SDL_PROP_WINDOW_CREATE_MENU_BOOLEAN`: true if the window is a popup menu
   * - `SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN`: true if the window will be used
   *   with Metal rendering
   * - `SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN`: true if the window should
   *   start minimized
   * - `SDL_PROP_WINDOW_CREATE_MODAL_BOOLEAN`: true if the window is modal to
   *   its parent
   * - `SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN`: true if the window starts
   *   with grabbed mouse focus
   * - `SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN`: true if the window will be used
   *   with OpenGL rendering
   * - `SDL_PROP_WINDOW_CREATE_PARENT_POINTER`: an SDL_Window that will be the
   *   parent of this window, required for windows with the "tooltip", "menu",
   *   and "modal" properties
   * - `SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN`: true if the window should be
   *   resizable
   * - `SDL_PROP_WINDOW_CREATE_TITLE_STRING`: the title of the window, in UTF-8
   *   encoding
   * - `SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN`: true if the window show
   *   transparent in the areas with alpha of 0
   * - `SDL_PROP_WINDOW_CREATE_TOOLTIP_BOOLEAN`: true if the window is a tooltip
   * - `SDL_PROP_WINDOW_CREATE_UTILITY_BOOLEAN`: true if the window is a utility
   *   window, not showing in the task bar and window list
   * - `SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN`: true if the window will be used
   *   with Vulkan rendering
   * - `SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER`: the width of the window
   * - `SDL_PROP_WINDOW_CREATE_X_NUMBER`: the x position of the window, or
   *   `SDL_WINDOWPOS_CENTERED`, defaults to `SDL_WINDOWPOS_UNDEFINED`. This is
   *   relative to the parent for windows with the "tooltip" or "menu" property
   *   set.
   * - `SDL_PROP_WINDOW_CREATE_Y_NUMBER`: the y position of the window, or
   *   `SDL_WINDOWPOS_CENTERED`, defaults to `SDL_WINDOWPOS_UNDEFINED`. This is
   *   relative to the parent for windows with the "tooltip" or "menu" property
   *   set.
   *
   * These are additional supported properties on macOS:
   *
   * - `SDL_PROP_WINDOW_CREATE_COCOA_WINDOW_POINTER`: the
   *   `(__unsafe_unretained)` NSWindow associated with the window, if you want
   *   to wrap an existing window.
   * - `SDL_PROP_WINDOW_CREATE_COCOA_VIEW_POINTER`: the `(__unsafe_unretained)`
   *   NSView associated with the window, defaults to `[window contentView]`
   *
   * These are additional supported properties on Wayland:
   *
   * - `SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN` - true if
   *   the application wants to use the Wayland surface for a custom role and
   *   does not want it attached to an XDG toplevel window. See
   *   [README/wayland](README/wayland) for more information on using custom
   *   surfaces.
   * - `SDL_PROP_WINDOW_CREATE_WAYLAND_CREATE_EGL_WINDOW_BOOLEAN` - true if the
   *   application wants an associated `wl_egl_window` object to be created and
   *   attached to the window, even if the window does not have the OpenGL
   *   property or `SDL_WINDOW_OPENGL` flag set.
   * - `SDL_PROP_WINDOW_CREATE_WAYLAND_WL_SURFACE_POINTER` - the wl_surface
   *   associated with the window, if you want to wrap an existing window. See
   *   [README/wayland](README/wayland) for more information.
   *
   * These are additional supported properties on Windows:
   *
   * - `SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER`: the HWND associated with the
   *   window, if you want to wrap an existing window.
   * - `SDL_PROP_WINDOW_CREATE_WIN32_PIXEL_FORMAT_HWND_POINTER`: optional,
   *   another window to share pixel format with, useful for OpenGL windows
   *
   * These are additional supported properties with X11:
   *
   * - `SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER`: the X11 Window associated
   *   with the window, if you want to wrap an existing window.
   *
   * The window is implicitly shown if the "hidden" property is not set.
   *
   * Windows with the "tooltip" and "menu" properties are popup windows and have
   * the behaviors and guidelines outlined in SDL_CreatePopupWindow().
   *
   * If this window is being created to be used with an SDL_Renderer, you should
   * not add a graphics API specific property
   * (`SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN`, etc), as SDL will handle that
   * internally when it chooses a renderer. However, SDL might need to recreate
   * your window at that point, which may cause the window to appear briefly,
   * and then flicker as it is recreated. The correct approach to this is to
   * create the window with the `SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN` property
   * set to true, then create the renderer, then show the window with
   * SDL_ShowWindow().
   *
   * @param props the properties to use.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef
   * @sa Properties
   */
  WindowBase(PropertiesRef props)
    : T(SDL_CreateWindowWithProperties(props.get()))
  {
  }

  /**
   * @brief Get the display associated with a window.
   *
   * @returns the instance ID of the display containing the center of the window
   *          on success or 0 on failure; call GetError() for more
   *          information.
   */
  Display GetDisplay() const { return Display::GetForWindow(T::get()); }

  /**
   * @brief Get the pixel density of a window.
   *
   * This is a ratio of pixel size to window size. For example, if the window is
   * 1920x1080 and it has a high density back buffer of 3840x2160 pixels, it
   * would have a pixel density of 2.0.
   *
   * @returns the pixel density or 0.0f on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa WindowBase.GetDisplayScale()
   */
  float GetPixelDensity() const { return SDL_GetWindowPixelDensity(T::get()); }

  /**
   * @brief Get the content display scale relative to a window's pixel size.
   *
   * This is a combination of the window pixel density and the display content
   * scale, and is the expected scale for displaying content in this window. For
   * example, if a 3840x2160 window had a display scale of 2.0, the user expects
   * the content to take twice as many pixels and be the same physical size as
   * if it were being displayed in a 1920x1080 window with a display scale of
   * 1.0.
   *
   * Conceptually this value corresponds to the scale display setting, and is
   * updated when that setting is changed, or the window moves to a display with
   * a different scale setting.
   *
   * @returns the display scale, or 0.0f on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  float GetDisplayScale() const { return SDL_GetWindowDisplayScale(T::get()); }

  /**
   * Set the display mode to use when a window is visible and fullscreen.
   *
   * This only affects the display mode used when the window is fullscreen. To
   * change the window size when the window is not fullscreen, use
   * SDL_SetWindowSize().
   *
   * If the window is currently in the fullscreen state, this request is
   * asynchronous on some windowing systems and the new mode dimensions may not
   * be applied immediately upon the return of this function. If an immediate
   * change is required, call SDL_SyncWindow() to block until the changes have
   * taken effect.
   *
   * When the new mode takes effect, an SDL_EVENT_WINDOW_RESIZED and/or an
   * SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED event will be emitted with the new
   * mode dimensions.
   *
   * @param mode a pointer to the display mode to use, which can be NULL for
   *             borderless fullscreen desktop mode, or one of the fullscreen
   *             modes returned by SDL_GetFullscreenDisplayModes() to set an
   *             exclusive fullscreen mode.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFullscreenMode()
   * @sa SetFullscreen()
   * @sa Sync()
   */
  bool SetFullscreenMode(OptionalRef<const DisplayMode> mode)
  {
    return SDL_SetWindowFullscreenMode(T::get(), mode);
  }

  /**
   * Query the display mode to use when a window is visible at fullscreen.
   *
   * @returns a pointer to the exclusive fullscreen mode to use or NULL for
   *          borderless fullscreen desktop mode.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetFullscreenMode()
   * @sa SetFullscreen()
   */
  const DisplayMode* GetFullscreenMode() const
  {
    return SDL_GetWindowFullscreenMode(T::get());
  }

  /**
   * Get the raw ICC profile data for the screen the window is currently on.
   *
   * @param size the size of the ICC profile.
   * @returns the raw ICC profile data on success or NULL on failure; call
   *          GetError() for more information. This should be freed with
   *          SDL_free() when it is no longer needed.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  FreeWrapper<void*> GetICCProfile(size_t* size) const
  {
    return {SDL_GetWindowICCProfile(T::get(), size)};
  }

  /**
   * Get the pixel format associated with the window.
   *
   * @returns the pixel format of the window on success or
   *          SDL_PIXELFORMAT_UNKNOWN on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PixelFormat GetPixelFormat() const
  {
    return SDL_GetWindowPixelFormat(T::get());
  }

  /**
   * @brief Get a window from a stored ID.
   *
   * The numeric ID is what SDL_WindowEvent references, and is necessary to map
   * these events to specific SDL_Window objects.
   *
   * @returns the ID of the window on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  WindowID GetID() const { return SDL_GetWindowID(T::get()); }

  /**
   * Get parent of a window.
   *
   * @returns the parent of the window on success or NULL if the window has no
   *          parent.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  WindowRef GetParent() const { return SDL_GetWindowParent(T::get()); }

  /**
   * @brief Get the properties associated with a window.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `SDL_PROP_WINDOW_SHAPE_POINTER`: the surface associated with a shaped
   *   window
   * - `SDL_PROP_WINDOW_HDR_ENABLED_BOOLEAN`: true if the window has HDR
   *   headroom above the SDR white point. This property can change dynamically
   *   when SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `SDL_PROP_WINDOW_SDR_WHITE_LEVEL_FLOAT`: the value of SDR white in the
   *   SDL_COLORSPACE_SRGB_LINEAR colorspace. On Windows this corresponds to
   * the SDR white level in scRGB colorspace, and on Apple platforms this is
   *   always 1.0 for EDR content. This property can change dynamically when
   *   SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `SDL_PROP_WINDOW_HDR_HEADROOM_FLOAT`: the additional high dynamic range
   *   that can be displayed, in terms of the SDR white point. When HDR is not
   *   enabled, this will be 1.0. This property can change dynamically when
   *   SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   *
   * On Android:
   *
   * - `SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER`: the ANativeWindow associated
   *   with the window
   * - `SDL_PROP_WINDOW_ANDROID_SURFACE_POINTER`: the EGLSurface associated
   * with the window
   *
   * On iOS:
   *
   * - `SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER`: the `(__unsafe_unretained)`
   *   UIWindow associated with the window
   * - `SDL_PROP_WINDOW_UIKIT_METAL_VIEW_TAG_NUMBER`: the NSInteger tag
   *   associated with metal views on the window
   * - `SDL_PROP_WINDOW_UIKIT_OPENGL_FRAMEBUFFER_NUMBER`: the OpenGL view's
   *   framebuffer object. It must be bound when rendering to the screen using
   *   OpenGL.
   * - `SDL_PROP_WINDOW_UIKIT_OPENGL_RENDERBUFFER_NUMBER`: the OpenGL view's
   *   renderbuffer object. It must be bound when SDL_GL_SwapWindow is called.
   * - `SDL_PROP_WINDOW_UIKIT_OPENGL_RESOLVE_FRAMEBUFFER_NUMBER`: the OpenGL
   *   view's resolve framebuffer, when MSAA is used.
   *
   * On KMS/DRM:
   *
   * - `SDL_PROP_WINDOW_KMSDRM_DEVICE_INDEX_NUMBER`: the device index
   * associated with the window (e.g. the X in /dev/dri/cardX)
   * - `SDL_PROP_WINDOW_KMSDRM_DRM_FD_NUMBER`: the DRM FD associated with the
   *   window
   * - `SDL_PROP_WINDOW_KMSDRM_GBM_DEVICE_POINTER`: the GBM device associated
   *   with the window
   *
   * On macOS:
   *
   * - `SDL_PROP_WINDOW_COCOA_WINDOW_POINTER`: the `(__unsafe_unretained)`
   *   NSWindow associated with the window
   * - `SDL_PROP_WINDOW_COCOA_METAL_VIEW_TAG_NUMBER`: the NSInteger tag
   *   assocated with metal views on the window
   *
   * On OpenVR:
   *
   * - `SDL_PROP_WINDOW_OPENVR_OVERLAY_ID`: the OpenVR Overlay Handle ID for
   * the associated overlay window.
   *
   * On Vivante:
   *
   * - `SDL_PROP_WINDOW_VIVANTE_DISPLAY_POINTER`: the EGLNativeDisplayType
   *   associated with the window
   * - `SDL_PROP_WINDOW_VIVANTE_WINDOW_POINTER`: the EGLNativeWindowType
   *   associated with the window
   * - `SDL_PROP_WINDOW_VIVANTE_SURFACE_POINTER`: the EGLSurface associated
   * with the window
   *
   * On Windows:
   *
   * - `SDL_PROP_WINDOW_WIN32_HWND_POINTER`: the HWND associated with the
   * window
   * - `SDL_PROP_WINDOW_WIN32_HDC_POINTER`: the HDC associated with the window
   * - `SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER`: the HINSTANCE associated with
   *   the window
   *
   * On Wayland:
   *
   * Note: The `xdg_*` window objects do not internally persist across window
   * show/hide calls. They will be null if the window is hidden and must be
   * queried each time it is shown.
   *
   * - `SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER`: the wl_display associated
   * with the window
   * - `SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER`: the wl_surface associated
   * with the window
   * - `SDL_PROP_WINDOW_WAYLAND_VIEWPORT_POINTER`: the wp_viewport associated
   *   with the window
   * - `SDL_PROP_WINDOW_WAYLAND_EGL_WINDOW_POINTER`: the wl_egl_window
   *   associated with the window
   * - `SDL_PROP_WINDOW_WAYLAND_XDG_SURFACE_POINTER`: the xdg_surface
   * associated with the window
   * - `SDL_PROP_WINDOW_WAYLAND_XDG_TOPLEVEL_POINTER`: the xdg_toplevel role
   *   associated with the window
   * - 'SDL_PROP_WINDOW_WAYLAND_XDG_TOPLEVEL_EXPORT_HANDLE_STRING': the export
   *   handle associated with the window
   * - `SDL_PROP_WINDOW_WAYLAND_XDG_POPUP_POINTER`: the xdg_popup role
   *   associated with the window
   * - `SDL_PROP_WINDOW_WAYLAND_XDG_POSITIONER_POINTER`: the xdg_positioner
   *   associated with the window, in popup mode
   *
   * On X11:
   *
   * - `SDL_PROP_WINDOW_X11_DISPLAY_POINTER`: the X11 Display associated with
   *   the window
   * - `SDL_PROP_WINDOW_X11_SCREEN_NUMBER`: the screen number associated with
   *   the window
   * - `SDL_PROP_WINDOW_X11_WINDOW_NUMBER`: the X11 Window associated with the
   *   window
   *
   * @returns a valid property ID on success or falsy one on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   */
  PropertiesRef GetProperties() const
  {
    return {SDL_GetWindowProperties(T::get())};
  }

  /**
   * Get the window flags.
   *
   * @returns a mask of the SDL_WindowFlags associated with `window`.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Hide()
   * @sa Maximize()
   * @sa Minimize()
   * @sa SetFullscreen()
   * @sa SetMouseGrab()
   * @sa Show()
   */
  WindowFlags GetFlags() const { return SDL_GetWindowFlags(T::get()); }

  /**
   * Set the title of a window.
   *
   * This string is expected to be in UTF-8 encoding.
   *
   * @param title the desired window title in UTF-8 format.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetTitle()
   */
  bool SetTitle(StringParam title)
  {
    return SDL_SetWindowTitle(T::get(), title);
  }

  /**
   * Get the title of a window.
   *
   * @returns the title of the window in UTF-8 format or "" if there is no
   *          title.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetTitle()
   */
  const char* GetTitle() const { return SDL_GetWindowTitle(T::get()); }

  /**
   * Set the icon for a window.
   *
   * If this function is passed a surface with alternate representations, the
   * surface will be interpreted as the content to be used for 100% display
   * scale, and the alternate representations will be used for high DPI
   * situations. For example, if the original surface is 32x32, then on a 2x
   * macOS display or 200% display scale on Windows, a 64x64 version of the
   * image will be used, if available. If a matching version of the image isn't
   * available, the closest larger size image will be downscaled to the
   * appropriate size and be used instead, if available. Otherwise, the closest
   * smaller image will be upscaled and be used instead.
   *
   * @param icon an SDL_Surface structure containing the icon for the window.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetIcon(SurfaceRef icon) { return SDL_SetWindowIcon(T::get(), icon); }

  /**
   * @brief Request the window's position and size to be set.
   *
   * @param rect the rect containing the new coordinates
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa SetPosition()
   * @sa SetSize()
   */
  bool SetRect(Rect rect)
  {
    return SetPosition(rect.GetTopLeft()) && SetSize(rect.GetSize());
  }

  /**
   * Get the position and client size of a window.
   *
   * This is the current position of the window as last reported by the
   * windowing system.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use SDL_GetWindowSizeInPixels()
   * or SDL_GetRenderOutputSize() to get the real client area size in pixels.
   *
   * @return std::optional<Rect>
   */
  std::optional<Rect> GetRect() const
  {
    if (Rect rect; GetPosition(rect.x, rect.y) && GetSize(rect.w, rect.h))
      return rect;
    return std::nullopt;
  }

  /**
   * Request that the window's position be set.
   *
   * If the window is in an exclusive fullscreen or maximized state, this
   * request has no effect.
   *
   * This can be used to reposition fullscreen-desktop windows onto a different
   * display, however, as exclusive fullscreen windows are locked to a specific
   * display, they can only be repositioned programmatically via
   * SetFullscreenMode().
   *
   * On some windowing systems this request is asynchronous and the new
   * coordinates may not have have been applied immediately upon the return of
   * this function. If an immediate change is required, call Sync() to block
   * until the changes have taken effect.
   *
   * When the window position changes, an SDL_EVENT_WINDOW_MOVED event will be
   * emitted with the window's new coordinates. Note that the new coordinates
   * may not match the exact coordinates requested, as some windowing systems
   * can restrict the position of the window in certain scenarios (e.g.
   * constraining the position so the window is always within desktop bounds).
   * Additionally, as this is just a request, it can be denied by the windowing
   * system.
   *
   * @param p the x, y coordinate of the window, or `SDL_WINDOWPOS_CENTERED` or
   *          `SDL_WINDOWPOS_UNDEFINED`.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetPosition()
   * @sa Sync()
   */
  bool SetPosition(SDL_Point p)
  {
    return SDL_SetWindowPosition(T::get(), p.x, p.y);
  }

  /**
   * Get the position of a window.
   *
   * This is the current position of the window as last reported by the
   * windowing system.
   *
   * @returns the position on success or false on std::nullopt; call
   * GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetPosition()
   * @sa SetPosition(int *, int *)
   */
  std::optional<Point> GetPosition() const
  {
    if (Point p; GetPosition(&p.x, &p.y)) return p;
    return std::nullopt;
  }

  /**
   * Get the position of a window.
   *
   * This is the current position of the window as last reported by the
   * windowing system.
   *
   * If you do not need the value for one of the positions a NULL may be passed
   * in the `x` or `y` parameter.
   *
   * @param x a pointer filled in with the x position of the window, may be
   *          NULL.
   * @param y a pointer filled in with the y position of the window, may be
   *          NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetPosition()
   */
  bool GetPosition(int* x, int* y) const
  {
    return SDL_GetWindowPosition(T::get(), x, y);
  }

  /**
   * Request that the size of a window's client area be set.
   *
   * If the window is in a fullscreen or maximized state, this request has no
   * effect.
   *
   * To change the exclusive fullscreen mode of a window, use
   * SDL_SetWindowFullscreenMode().
   *
   * On some windowing systems, this request is asynchronous and the new window
   * size may not have have been applied immediately upon the return of this
   * function. If an immediate change is required, call SDL_SyncWindow() to
   * block until the changes have taken effect.
   *
   * When the window size changes, an SDL_EVENT_WINDOW_RESIZED event will be
   * emitted with the new window dimensions. Note that the new dimensions may
   * not match the exact size requested, as some windowing systems can restrict
   * the window size in certain scenarios (e.g. constraining the size of the
   * content area to remain within the usable desktop bounds). Additionally, as
   * this is just a request, it can be denied by the windowing system.
   *
   * @param p the width, height of the window, both must be > 0.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   * @sa SetFullscreenMode()
   * @sa Sync()
   */
  bool SetSize(SDL_Point p) { return SDL_SetWindowSize(T::get(), p.x, p.y); }

  /**
   * Get the size of a window's client area.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use GetSizeInPixels()
   * or Renderer.GetOutputSize() to get the real client area size in pixels.
   *
   * @returns a point with width and height on success or std::nullopt on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetOutputSize()
   * @sa GetSizeInPixels()
   * @sa SetSize()
   * @sa GetSize(int *, int *)
   */
  std::optional<Point> GetSize() const
  {
    if (Point p; GetSize(&p.x, &p.y)) return p;
    return std::nullopt;
  }

  /**
   * Get the size of a window's client area.
   *
   * The window pixel size may differ from its window coordinate size if the
   * window is on a high pixel density display. Use GetSizeInPixels()
   * or Renderer.GetOutputSize() to get the real client area size in pixels.
   *
   * @param w a pointer filled in with the width of the window, may be NULL.
   * @param h a pointer filled in with the height of the window, may be NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Renderer.GetOutputSize()
   * @sa GetSizeInPixels()
   * @sa SetSize()
   */
  bool GetSize(int* w, int* h) const
  {
    return SDL_GetWindowSize(T::get(), w, h);
  }

  /**
   * Get the safe area for this window.
   *
   * Some devices have portions of the screen which are partially obscured or
   * not interactive, possibly due to on-screen controls, curved edges, camera
   * notches, TV overscan, etc. This function provides the area of the window
   * which is safe to have interactable content. You should continue rendering
   * into the rest of the window, but it should not contain visually important
   * or interactible content.
   *
   * @returns the client area that is safe for interactive content on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Rect> GetSafeArea() const
  {
    if (Rect rect; SDL_GetWindowSafeArea(T::get(), &rect)) return rect;
    return std::nullopt;
  }

  /**
   * Request that the aspect ratio of a window's client area be set.
   *
   * The aspect ratio is the ratio of width divided by height, e.g. 2560x1600
   * would be 1.6. Larger aspect ratios are wider and smaller aspect ratios are
   * narrower.
   *
   * If, at the time of this request, the window in a fixed-size state, such as
   * maximized or fullscreen, the request will be deferred until the window
   * exits this state and becomes resizable again.
   *
   * On some windowing systems, this request is asynchronous and the new window
   * aspect ratio may not have have been applied immediately upon the return of
   * this function. If an immediate change is required, call SDL_SyncWindow()
   * to block until the changes have taken effect.
   *
   * When the window size changes, an SDL_EVENT_WINDOW_RESIZED event will be
   * emitted with the new window dimensions. Note that the new dimensions may
   * not match the exact aspect ratio requested, as some windowing systems can
   * restrict the window size in certain scenarios (e.g. constraining the size
   * of the content area to remain within the usable desktop bounds).
   * Additionally, as this is just a request, it can be denied by the windowing
   * system.
   *
   * @param min_aspect the minimum aspect ratio of the window, or 0.0f for no
   *                   limit.
   * @param max_aspect the maximum aspect ratio of the window, or 0.0f for no
   *                   limit.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetAspectRatio()
   * @sa Sync()
   */
  bool SetAspectRatio(float min_aspect, float max_aspect)
  {
    return SDL_SetWindowAspectRatio(T::get(), min_aspect, max_aspect);
  }

  /**
   * Get the size of a window's client area.
   *
   * @param min_aspect a pointer filled in with the minimum aspect ratio of the
   *                   window, may be NULL.
   * @param max_aspect a pointer filled in with the maximum aspect ratio of the
   *                   window, may be NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetspectRatio()
   */
  bool GetAspectRatio(float* min_aspect, float* max_aspect) const
  {
    return SDL_GetWindowAspectRatio(T::get(), min_aspect, max_aspect);
  }

  /**
   * Get the size of a window's borders (decorations) around the client area.
   *
   * Note: If this function fails (returns false), the size values will be
   * initialized to 0, 0, 0, 0 (if a non-NULL pointer is provided), as if the
   * window in question was borderless.
   *
   * Note: This function may fail on systems where the window has not yet been
   * decorated by the display server (for example, immediately after calling
   * WindowBase::WindowBase()). It is recommended that you wait at least until
   * the window has been presented and composited, so that the window system has
   * a chance to decorate the window and provide the border dimensions to SDL.
   *
   * This function also returns false if getting the information is not
   * supported.
   *
   * @param top pointer to variable for storing the size of the top border;
   * NULL is permitted.
   * @param left pointer to variable for storing the size of the left border;
   *             NULL is permitted.
   * @param bottom pointer to variable for storing the size of the bottom
   *               border; NULL is permitted.
   * @param right pointer to variable for storing the size of the right border;
   *              NULL is permitted.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   */
  bool GetBordersSize(int* top, int* left, int* bottom, int* right) const
  {
    return SDL_GetWindowBordersSize(T::get(), top, left, bottom, right);
  }

  /**
   * Get the size of a window's client area, in pixels.
   *
   * @returns the size on success or std::nullopt on failure; call GetError()
   * for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   * @sa GetSizeInPixels(int*, int*)
   */
  std::optional<Point> GetSizeInPixels() const
  {
    if (Point p; GetSizeInPixels(&p.x, &p.y)) return p;
    return std::nullopt;
  }

  /**
   * Get the size of a window's client area, in pixels.
   *
   * @param w a pointer to variable for storing the width in pixels, may be
   *          NULL.
   * @param h a pointer to variable for storing the height in pixels, may be
   *          NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   */
  bool GetSizeInPixels(int* w, int* h) const
  {
    return SDL_GetWindowSizeInPixels(T::get(), w, h);
  }

  /**
   * Set the minimum size of a window's client area.
   *
   * @param p the minimum width and height of the window, or 0 for no limit.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMinimumSize()
   * @sa SetMaximumSize()
   */
  bool SetMinimumSize(SDL_Point p)
  {
    return SDL_SetWindowMinimumSize(T::get(), p.x, p.y);
  }

  /**
   * Get the minimum size of a window's client area.
   *
   * @param w a pointer filled in with the minimum width of the window, may be
   *          NULL.
   * @param h a pointer filled in with the minimum height of the window, may be
   *          NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMaximumSize()
   * @sa SetMinimumSize()
   */
  bool GetMinimumSize(int* w, int* h) const
  {
    return SDL_GetWindowMinimumSize(T::get(), w, h);
  }

  /**
   * Set the maximum size of a window's client area.
   *
   * @param p the maximum width and height of the window, or 0 for no limit.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMaximumSize()
   * @sa SetMinimumSize()
   */
  bool SetMaximumSize(SDL_Point p)
  {
    return SDL_SetWindowMaximumSize(T::get(), p.x, p.y);
  }

  /**
   * Get the maximum size of a window's client area.
   *
   * @param w a pointer filled in with the maximum width of the window, may be
   *          NULL.
   * @param h a pointer filled in with the maximum height of the window, may be
   *          NULL.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMinimumSize()
   * @sa SetMaximumSize()
   */
  bool GetMaximumSize(int* w, int* h) const
  {
    return SDL_GetWindowMaximumSize(T::get(), w, h);
  }

  /**
   * Set the border state of a window.
   *
   * This will add or remove the window's `SDL_WINDOW_BORDERLESS` flag and add
   * or remove the border from the actual window. This is a no-op if the
   * window's border already matches the requested state.
   *
   * You can't change the border state of a fullscreen window.
   *
   * @param bordered false to remove border, true to add border.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFlags()
   */
  bool SetBordered(bool bordered)
  {
    return SDL_SetWindowBordered(T::get(), bordered);
  }

  /**
   * Set the user-resizable state of a window.
   *
   * This will add or remove the window's `SDL_WINDOW_RESIZABLE` flag and
   * allow/disallow user resizing of the window. This is a no-op if the
   * window's resizable state already matches the requested state.
   *
   * You can't change the resizable state of a fullscreen window.
   *
   * @param resizable true to allow resizing, false to disallow.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetWindowFlags()
   */
  bool SetResizable(bool resizable)
  {
    return SDL_SetWindowResizable(T::get(), resizable);
  }

  /**
   * Set the window to always be above the others.
   *
   * This will add or remove the window's `SDL_WINDOW_ALWAYS_ON_TOP` flag. This
   * will bring the window to the front and keep the window above the rest.
   *
   * @param on_top true to set the window always on top, false to disable.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFlags()
   */
  bool SetAlwaysOnTop(bool on_top)
  {
    return SDL_SetWindowAlwaysOnTop(T::get(), on_top);
  }

  /**
   * Show a window.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Hide()
   * @sa Raise()
   */
  bool Show() { return SDL_ShowWindow(T::get()); }

  /**
   * Hide a window.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Show()
   * @sa SDL_WINDOW_HIDDEN
   */
  bool Hide() { return SDL_HideWindow(T::get()); }

  /**
   * Request that a window be raised above other windows and gain the input
   * focus.
   *
   * The result of this request is subject to desktop window manager policy,
   * particularly if raising the requested window would result in stealing
   * focus from another application. If the window is successfully raised and
   * gains input focus, an SDL_EVENT_WINDOW_FOCUS_GAINED event will be emitted,
   * and the window will have the SDL_WINDOW_INPUT_FOCUS flag set.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Raise() { return SDL_RaiseWindow(T::get()); }

  /**
   * Request that the window be made as large as possible.
   *
   * Non-resizable windows can't be maximized. The window must have the
   * SDL_WINDOW_RESIZABLE flag set, or this will have no effect.
   *
   * On some windowing systems this request is asynchronous and the new window
   * state may not have have been applied immediately upon the return of this
   * function. If an immediate change is required, call SDL_SyncWindow() to
   * block until the changes have taken effect.
   *
   * When the window state changes, an SDL_EVENT_WINDOW_MAXIMIZED event will be
   * emitted. Note that, as this is just a request, the windowing system can
   * deny the state change.
   *
   * When maximizing a window, whether the constraints set via
   * SDL_SetWindowMaximumSize() are honored depends on the policy of the window
   * manager. Win32 and macOS enforce the constraints when maximizing, while
   * X11 and Wayland window managers may vary.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Minimize()
   * @sa Restore()
   * @sa Sync()
   */
  bool Maximize() { return SDL_MaximizeWindow(T::get()); }

  /**
   * Request that the window be minimized to an iconic representation.
   *
   * If the window is in a fullscreen state, this request has no direct effect.
   * It may alter the state the window is returned to when leaving fullscreen.
   *
   * On some windowing systems this request is asynchronous and the new window
   * state may not have been applied immediately upon the return of this
   * function. If an immediate change is required, call SDL_SyncWindow() to
   * block until the changes have taken effect.
   *
   * When the window state changes, an SDL_EVENT_WINDOW_MINIMIZED event will be
   * emitted. Note that, as this is just a request, the windowing system can
   * deny the state change.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Maximize()
   * @sa Restore()
   * @sa Sync()
   */
  bool Minimize() { return SDL_MinimizeWindow(T::get()); }

  /**
   * Request that the size and position of a minimized or maximized window be
   * restored.
   *
   * If the window is in a fullscreen state, this request has no direct effect.
   * It may alter the state the window is returned to when leaving fullscreen.
   *
   * On some windowing systems this request is asynchronous and the new window
   * state may not have have been applied immediately upon the return of this
   * function. If an immediate change is required, call SDL_SyncWindow() to
   * block until the changes have taken effect.
   *
   * When the window state changes, an SDL_EVENT_WINDOW_RESTORED event will be
   * emitted. Note that, as this is just a request, the windowing system can
   * deny the state change.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Maximize()
   * @sa Minimize()
   * @sa Sync()
   */
  bool Restore() { return SDL_RestoreWindow(T::get()); }

  /**
   * Request that the window's fullscreen state be changed.
   *
   * By default a window in fullscreen state uses borderless fullscreen desktop
   * mode, but a specific exclusive display mode can be set using
   * SDL_SetWindowFullscreenMode().
   *
   * On some windowing systems this request is asynchronous and the new
   * fullscreen state may not have have been applied immediately upon the
   * return of this function. If an immediate change is required, call
   * SDL_SyncWindow() to block until the changes have taken effect.
   *
   * When the window state changes, an SDL_EVENT_WINDOW_ENTER_FULLSCREEN or
   * SDL_EVENT_WINDOW_LEAVE_FULLSCREEN event will be emitted. Note that, as
   * this is just a request, it can be denied by the windowing system.
   *
   * @param fullscreen true for fullscreen mode, false for windowed mode.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFullscreenMode()
   * @sa SetFullscreenMode()
   * @sa Sync()
   * @sa SDL_WINDOW_FULLSCREEN
   */
  bool SetFullscreen(bool fullscreen)
  {
    return SDL_SetWindowFullscreen(T::get(), fullscreen);
  }

  /**
   * Block until any pending window state is finalized.
   *
   * On asynchronous windowing systems, this acts as a synchronization barrier
   * for pending window state. It will attempt to wait until any pending window
   * state has been applied and is guaranteed to return within finite time.
   * Note that for how long it can potentially block depends on the underlying
   * window system, as window state changes may involve somewhat lengthy
   * animations that must complete before the window is in its final requested
   * state.
   *
   * On windowing systems where changes are immediate, this does nothing.
   *
   * @returns true on success or false if the operation timed out before the
   *          window was in the requested state.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetSize()
   * @sa SetPosition()
   * @sa SetFullscreen()
   * @sa Minimize()
   * @sa Maximize()
   * @sa Restore()
   * @sa SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS
   */
  bool Sync() { return SDL_SyncWindow(T::get()); }

  /**
   * Get the renderer associated with a window.
   *
   * @returns the rendering context on success or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  RendererRef GetRenderer() const;

  /**
   * @brief Return whether the window has a surface associated with it.
   * @returns true if there is a surface associated with the window, or false
   *          otherwise.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa WindowBase.GetSurface()
   */
  bool HasSurface() const { return SDL_WindowHasSurface(T::get()); }

  /**
   * @brief Get the SDL surface associated with the window.
   *
   * A new surface will be created with the optimal format for the window, if
   * necessary. This surface will be freed when the window is destroyed. Do not
   * free this surface.
   *
   * This surface will be invalidated if the window is resized. After resizing
   * a window this function must be called again to return a valid surface.
   *
   * You may not combine this with 3D or the rendering API on this window.
   *
   * This function is affected by `SDL_HINT_FRAMEBUFFER_ACCELERATION`.
   * @return the surface associated with the window, or NULL on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa DestroySurface()
   * @sa HasSurface()
   * @sa UpdateSurface()
   * @sa UpdateSurfaceRects()
   */
  SurfaceRef GetSurface() { return SDL_GetWindowSurface(T::get()); }

  /**
   * Toggle VSync for the window surface.
   *
   * When a window surface is created, vsync defaults to
   * SDL_WINDOW_SURFACE_VSYNC_DISABLED.
   *
   * The `vsync` parameter can be 1 to synchronize present with every vertical
   * refresh, 2 to synchronize present with every second vertical refresh,
   * etc., SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE for late swap tearing (adaptive
   * vsync), or SDL_WINDOW_SURFACE_VSYNC_DISABLED to disable. Not every value
   * is supported by every driver, so you should check the return value to see
   * whether the requested setting is supported.
   *
   * @param vsync the vertical refresh sync interval.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSurfaceVSync()
   */
  bool SetSurfaceVSync(int vsync)
  {
    return SDL_SetWindowSurfaceVSync(T::get(), vsync);
  }

  /**
   * Get VSync for the window surface.
   *
   * @returns the current vertical refresh sync interval on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetSurfaceVSync()
   */
  std::optional<int> GetSurfaceVSync() const
  {
    if (int vsync; SDL_GetWindowSurfaceVSync(T::get(), vsync)) return vsync;
    return std::nullopt;
  }

  // TODO SDL_SetWindowSurfaceVSync

  /**
   * @brief Copy the window surface to the screen.
   *
   * This is the function you use to reflect any changes to the surface on the
   * screen.
   *
   * This function is equivalent to the SDL 1.2 API SDL_Flip().
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @sa GetSurface()
   * @sa UpdateSurfaceRects()
   */
  bool UpdateSurface() { return SDL_UpdateWindowSurface(T::get()); }

  /**
   * Copy areas of the window surface to the screen.
   *
   * This is the function you use to reflect changes to portions of the surface
   * on the screen.
   *
   * This function is equivalent to the SDL 1.2 API SDL_UpdateRects().
   *
   * Note that this function will update _at least_ the rectangles specified,
   * but this is only intended as an optimization; in practice, this might
   * update more of the screen (or all of the screen!), depending on what method
   * SDL uses to send pixels to the system.
   *
   * @param rects an array of SDL_Rect structures representing areas of the
   *              surface to copy, in pixels.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSurface()
   * @sa UpdateSurface()
   */
  bool UpdateSurfaceRects(SpanRef<const SDL_Rect> rects)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_SINT32);
    return SDL_UpdateWindowSurfaceRects(T::get(), rects.data(), rects.size());
  }

  /**
   * Destroy the surface associated with the window.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSurface()
   * @sa HasSurface()
   */
  bool DestroySurface() { return SDL_DestroyWindowSurface(T::get()); }

  /**
   * Set a window's keyboard grab mode.
   *
   * Keyboard grab enables capture of system keyboard shortcuts like Alt+Tab or
   * the Meta/Super key. Note that not all system keyboard shortcuts can be
   * captured by applications (one example is Ctrl+Alt+Del on Windows).
   *
   * This is primarily intended for specialized applications such as VNC
   * clients or VM frontends. Normal games should not use keyboard grab.
   *
   * When keyboard grab is enabled, SDL will continue to handle Alt+Tab when
   * the window is full-screen to ensure the user is not trapped in your
   * application. If you have a custom keyboard shortcut to exit fullscreen
   * mode, you may suppress this behavior with
   * `SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED`.
   *
   * If the caller enables a grab while another window is currently grabbed,
   * the other window loses its grab in favor of the caller's window.
   *
   * @param grabbed this is true to grab keyboard, and false to release.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetKeyboardGrab()
   * @sa SetMouseGrab()
   */
  bool SetKeyboardGrab(bool grabbed)
  {
    return SDL_SetWindowKeyboardGrab(T::get(), grabbed);
  }

  /**
   * Set a window's mouse grab mode.
   *
   * Mouse grab confines the mouse cursor to the window.
   *
   * @param grabbed this is true to grab mouse, and false to release.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMouseRect()
   * @sa SetMouseRect()
   * @sa SetMouseGrab()
   * @sa SetKeyboardGrab()
   */
  bool SetMouseGrab(bool grabbed)
  {
    return SDL_SetWindowMouseGrab(T::get(), grabbed);
  }

  /**
   * Get a window's keyboard grab mode.
   *
   * @returns true if keyboard is grabbed, and false otherwise.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetKeyboardGrab()
   */
  bool GetKeyboardGrab() const { return SDL_GetWindowKeyboardGrab(T::get()); }

  /**
   * Get a window's mouse grab mode.
   *
   * @returns true if mouse is grabbed, and false otherwise.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMouseRect()
   * @sa SetMouseRect()
   * @sa SetMouseGrab()
   * @sa SetKeyboardGrab()
   */
  bool GetMouseGrab() const { return SDL_GetWindowMouseGrab(T::get()); }

  /**
   * Confines the cursor to the specified area of a window.
   *
   * Note that this does NOT grab the cursor, it only defines the area a cursor
   * is restricted to when the window has mouse focus.
   *
   * @param rect a rectangle area in window-relative coordinates. If NULL the
   *             barrier for the specified window will be destroyed.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetMouseRect()
   * @sa GetMouseGrab()
   * @sa SetMouseGrab()
   */
  bool SetMouseRect(const SDL_Rect& rect)
  {
    return SDL_SetWindowMouseRect(T::get(), rect);
  }

  /**
   * Get the mouse confinement rectangle of a window.
   *
   * @returns a pointer to the mouse confinement rectangle of a window, or NULL
   *          if there isn't one.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetMouseRect()
   * @sa GetMouseGrab()
   * @sa SetMouseGrab()
   */
  const Rect* GetMouseRect() const { return SDL_GetWindowMouseRect(T::get()); }

  /**
   * Set the opacity for a window.
   *
   * The parameter `opacity` will be clamped internally between 0.0f
   * (transparent) and 1.0f (opaque).
   *
   * This function also returns false if setting the opacity isn't supported.
   *
   * @param opacity the opacity value (0.0f - transparent, 1.0f - opaque).
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetOpacity()
   */
  bool SetOpacity(float opacity)
  {
    return SDL_SetWindowOpacity(T::get(), opacity);
  }

  /**
   * Get the opacity of a window.
   *
   * If transparency isn't supported on this platform, opacity will be returned
   * as 1.0f without error.
   *
   * @returns the opacity, (0.0f - transparent, 1.0f - opaque), or -1.0f on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetOpacity()
   */
  float GetOpacity() const { return SDL_GetWindowOpacity(T::get()); }

  /**
   * Set the window as a child of a parent window.
   *
   * If the window is already the child of an existing window, it will be
   * reparented to the new owner. Setting the parent window to NULL unparents
   * the window and removes child window status.
   *
   * If a parent window is hidden or destroyed, the operation will be
   * recursively applied to child windows. Child windows hidden with the parent
   * that did not have their hidden status explicitly set will be restored when
   * the parent is shown.
   *
   * Attempting to set the parent of a window that is currently in the modal
   * state will fail. Use SDL_SetWindowModal() to cancel the modal status
   * before attempting to change the parent.
   *
   * Popup windows cannot change parents and attempts to do so will fail.
   *
   * Setting a parent window that is currently the sibling or descendent of the
   * child window results in undefined behavior.
   *
   * @param parent the new parent window for the child window.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetModal()
   */
  bool SetParent(WindowRef parent)
  {
    return SDL_SetWindowParent(T::get(), parent.get());
  }

  /**
   * Toggle the state of the window as modal.
   *
   * To enable modal status on a window, the window must currently be the child
   * window of a parent, or toggling modal status on will fail.
   *
   * @param modal true to toggle modal status on, false to toggle it off.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetParent()
   * @sa SDL_WINDOW_MODAL
   */
  bool SetModal(bool modal) { return SDL_SetWindowModal(T::get(), modal); }

  /**
   * Set whether the window may have input focus.
   *
   * @param focusable true to allow input focus, false to not allow input
   * focus.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetFocusable(bool focusable)
  {
    return SDL_SetWindowFocusable(T::get(), focusable);
  }

  /**
   * Display the system-level window menu.
   *
   * This default window menu is provided by the system and on some platforms
   * provides functionality for setting or changing privileged state on the
   * window, such as moving it between workspaces or displays, or toggling the
   * always-on-top property.
   *
   * On platforms or desktops where this is unsupported, this function does
   * nothing.
   *
   * @param p the x, y coordinate of the menu, relative to the origin (top-left)
   * of the client area.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ShowSystemMenu(SDL_Point p)
  {
    return SDL_ShowWindowSystemMenu(T::get(), p.x, p.y);
  }

  /**
   * @sa HitTest
   */
  using HitTestFunction =
    std::function<HitTestResult(SDL_Window* window, const SDL_Point* area)>;

  /**
   * Provide a callback that decides if a window region has special properties.
   *
   * Normally windows are dragged and resized by decorations provided by the
   * system window manager (a title bar, borders, etc), but for some apps, it
   * makes sense to drag them from somewhere else inside the window itself; for
   * example, one might have a borderless window that wants to be draggable from
   * any part, or simulate its own title bar, etc.
   *
   * This function lets the app provide a callback that designates pieces of a
   * given window as special. This callback is run during event processing if we
   * need to tell the OS to treat a region of the window specially; the use of
   * this callback is known as "hit testing."
   *
   * Mouse input may not be delivered to your application if it is within a
   * special area; the OS will often apply that input to moving the window or
   * resizing the window and not deliver it to the application.
   *
   * Specifying NULL for a callback disables hit-testing. Hit-testing is
   * disabled by default.
   *
   * Platforms that don't support this functionality will return false
   * unconditionally, even if you're attempting to disable hit-testing.
   *
   * Your callback may fire at any time, and its firing does not indicate any
   * specific behavior (for example, on Windows, this certainly might fire when
   * the OS is deciding whether to drag your window, but it fires for lots of
   * other reasons, too, some unrelated to anything you probably care about _and
   * when the mouse isn't actually at the location it is testing_). Since this
   * can fire at any time, you should try to keep your callback efficient,
   * devoid of allocations, etc.
   *
   * @param callback the function to call when doing a hit-test.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetHitTest(HitTestFunction callback)
  {
    using Wrapper = CallbackWrapper<HitTestResult(SDL_Window * window,
                                                  const SDL_Point* area)>;
    void* cbHandle = Wrapper::Wrap(std::move(callback));
    return SetHitTest(&Wrapper::CallSuffixed, cbHandle);
  }

  /**
   * Provide a callback that decides if a window region has special properties.
   *
   * Normally windows are dragged and resized by decorations provided by the
   * system window manager (a title bar, borders, etc), but for some apps, it
   * makes sense to drag them from somewhere else inside the window itself; for
   * example, one might have a borderless window that wants to be draggable from
   * any part, or simulate its own title bar, etc.
   *
   * This function lets the app provide a callback that designates pieces of a
   * given window as special. This callback is run during event processing if we
   * need to tell the OS to treat a region of the window specially; the use of
   * this callback is known as "hit testing."
   *
   * Mouse input may not be delivered to your application if it is within a
   * special area; the OS will often apply that input to moving the window or
   * resizing the window and not deliver it to the application.
   *
   * Specifying NULL for a callback disables hit-testing. Hit-testing is
   * disabled by default.
   *
   * Platforms that don't support this functionality will return false
   * unconditionally, even if you're attempting to disable hit-testing.
   *
   * Your callback may fire at any time, and its firing does not indicate any
   * specific behavior (for example, on Windows, this certainly might fire when
   * the OS is deciding whether to drag your window, but it fires for lots of
   * other reasons, too, some unrelated to anything you probably care about _and
   * when the mouse isn't actually at the location it is testing_). Since this
   * can fire at any time, you should try to keep your callback efficient,
   * devoid of allocations, etc.
   *
   * @param callback the function to call when doing a hit-test.
   * @param callback_data an app-defined void pointer passed to **callback**.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetHitTest(HitTest callback, void* callback_data)
  {
    return SDL_SetWindowHitTest(T::get(), callback, callback_data);
  }

  /**
   * Set the shape of a transparent window.
   *
   * This sets the alpha channel of a transparent window and any fully
   * transparent areas are also transparent to mouse clicks. If you are using
   * something besides the SDL render API, then you are responsible for drawing
   * the alpha channel of the window to match the shape alpha channel to get
   * consistent cross-platform results.
   *
   * The shape is copied inside this function, so you can free it afterwards.
   * If your shape surface changes, you should call SDL_SetWindowShape() again
   * to update the window. This is an expensive operation, so should be done
   * sparingly.
   *
   * The window must have been created with the SDL_WINDOW_TRANSPARENT flag.
   *
   * @param shape the surface representing the shape of the window, or NULL to
   *              remove any current shape.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetShape(SurfaceRef shape)
  {
    return SDL_SetWindowShape(T::get(), shape);
  }

  /**
   * Request a window to demand attention from the user.
   *
   * @param operation the operation to perform.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Flash(FlashOperation operation)
  {
    return SDL_FlashWindow(T::get(), operation);
  }

  /**
   * Destroy a window.
   *
   * Any child windows owned by the window will be recursively destroyed as
   * well.
   *
   * Note that on some platforms, the visible window may not actually be removed
   * from the screen until the SDL event loop is pumped again, even though the
   * SDL_Window is no longer valid after this call.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Destroy() { return SDL_DestroyWindow(T::release()); }
};

/**
 * An opaque handle to an OpenGL context.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GL_CreateContext()
 */
using GLContext = SDL_GLContext;

/**
 * Opaque type for an EGL display.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLDisplay = SDL_EGLDisplay;

/**
 * Opaque type for an EGL config.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLConfig = SDL_EGLConfig;

/**
 * Opaque type for an EGL surface.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLSurface = SDL_EGLSurface;

/**
 * An EGL attribute, used when creating an EGL context.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLAttrib = SDL_EGLAttrib;

/**
 * An EGL integer attribute, used when creating an EGL surface.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using EGLint = SDL_EGLint;

/**
 * EGL platform attribute initialization callback.
 *
 * This is called when SDL is attempting to create an EGL context, to let the
 * app add extra attributes to its eglGetPlatformDisplay() call.
 *
 * The callback should return a pointer to an EGL attribute array terminated
 * with `EGL_NONE`. If this function returns NULL, the SDL_CreateWindow
 * process will fail gracefully.
 *
 * The returned pointer should be allocated with SDL_malloc() and will be
 * passed to SDL_free().
 *
 * The arrays returned by each callback will be appended to the existing
 * attribute arrays defined by SDL.
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 * @returns a newly-allocated array of attributes, terminated with `EGL_NONE`.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EGL_SetAttributeCallbacks()
 */
using EGLAttribArrayCallback = SDL_EGLAttribArrayCallback;

/**
 * EGL surface/context attribute initialization callback types.
 *
 * This is called when SDL is attempting to create an EGL surface, to let the
 * app add extra attributes to its eglCreateWindowSurface() or
 * eglCreateContext calls.
 *
 * For convenience, the EGLDisplay and EGLConfig to use are provided to the
 * callback.
 *
 * The callback should return a pointer to an EGL attribute array terminated
 * with `EGL_NONE`. If this function returns NULL, the SDL_CreateWindow
 * process will fail gracefully.
 *
 * The returned pointer should be allocated with SDL_malloc() and will be
 * passed to SDL_free().
 *
 * The arrays returned by each callback will be appended to the existing
 * attribute arrays defined by SDL.
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 * @param display the EGL display to be used.
 * @param config the EGL config to be used.
 * @returns a newly-allocated array of attributes, terminated with `EGL_NONE`.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EGL_SetAttributeCallbacks()
 */
using EGLIntArrayCallback = SDL_EGLIntArrayCallback;

/**
 * @name GLAttrs
 * @{
 */

/**
 * An enumeration of OpenGL configuration attributes.
 *
 * While you can set most OpenGL attributes normally, the attributes listed
 * above must be known before SDL creates the window that will be used with
 * the OpenGL context. These attributes are set and read with
 * SDL_GL_SetAttribute() and SDL_GL_GetAttribute().
 *
 * In some cases, these attributes are minimum requests; the GL does not
 * promise to give you exactly what you asked for. It's possible to ask for a
 * 16-bit depth buffer and get a 24-bit one instead, for example, or to ask
 * for no stencil buffer and still have one available. Context creation should
 * fail if the GL can't provide your requested attributes at a minimum, but
 * you should check to see exactly what you got.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using GLAttr = SDL_GLAttr;

/**
 * the minimum number of bits for the red channel of the color buffer; defaults
 * to 3.
 */
constexpr GLAttr GL_RED_SIZE = SDL_GL_RED_SIZE;

/**
 * the minimum number of bits for the green channel of the color buffer;
 * defaults to 3.
 */
constexpr GLAttr GL_GREEN_SIZE = SDL_GL_GREEN_SIZE;

/**
 * the minimum number of bits for the blue channel of the color buffer; defaults
 * to 2.
 */
constexpr GLAttr GL_BLUE_SIZE = SDL_GL_BLUE_SIZE;

/**
 * the minimum number of bits for the alpha channel of the color buffer;
 * defaults to 0.
 */
constexpr GLAttr GL_ALPHA_SIZE = SDL_GL_ALPHA_SIZE;

/**
 * the minimum number of bits for frame buffer size; defaults to 0.
 */
constexpr GLAttr GL_BUFFER_SIZE = SDL_GL_BUFFER_SIZE;

/**
 * whether the output is single or double buffered; defaults to double buffering
 * on.
 */
constexpr GLAttr GL_DOUBLEBUFFER = SDL_GL_DOUBLEBUFFER;

/**
 * the minimum number of bits in the depth buffer; defaults to 16.
 */
constexpr GLAttr GL_DEPTH_SIZE = SDL_GL_DEPTH_SIZE;

/**
 * the minimum number of bits in the stencil buffer; defaults to 0.
 */
constexpr GLAttr GL_STENCIL_SIZE = SDL_GL_STENCIL_SIZE;

/**
 * the minimum number of bits for the red channel of the accumulation buffer;
 * defaults to 0.
 */
constexpr GLAttr GL_ACCUM_RED_SIZE = SDL_GL_ACCUM_RED_SIZE;

/**
 * the minimum number of bits for the green channel of the accumulation buffer;
 * defaults to 0.
 */
constexpr GLAttr GL_ACCUM_GREEN_SIZE = SDL_GL_ACCUM_GREEN_SIZE;

/**
 * the minimum number of bits for the blue channel of the accumulation buffer;
 * defaults to 0.
 */
constexpr GLAttr GL_ACCUM_BLUE_SIZE = SDL_GL_ACCUM_BLUE_SIZE;

/**
 * the minimum number of bits for the alpha channel of the accumulation buffer;
 * defaults to 0.
 */
constexpr GLAttr GL_ACCUM_ALPHA_SIZE = SDL_GL_ACCUM_ALPHA_SIZE;

/**
 * whether the output is stereo 3D; defaults to off.
 */
constexpr GLAttr GL_STEREO = SDL_GL_STEREO;

/**
 * the number of buffers used for multisample anti-aliasing; defaults to 0.
 */
constexpr GLAttr GL_MULTISAMPLEBUFFERS = SDL_GL_MULTISAMPLEBUFFERS;

/**
 * the number of samples used around the current pixel used for multisample
 * anti-aliasing.
 */
constexpr GLAttr GL_MULTISAMPLESAMPLES = SDL_GL_MULTISAMPLESAMPLES;

/**
 * set to 1 to require hardware acceleration, set to 0 to force software
 * rendering; defaults to allow either.
 */
constexpr GLAttr GL_ACCELERATED_VISUAL = SDL_GL_ACCELERATED_VISUAL;

/**
 * not used (deprecated).
 */
constexpr GLAttr GL_RETAINED_BACKING = SDL_GL_RETAINED_BACKING;

/**
 * OpenGL context major version.
 */
constexpr GLAttr GL_CONTEXT_MAJOR_VERSION = SDL_GL_CONTEXT_MAJOR_VERSION;

/**
 * OpenGL context minor version.
 */
constexpr GLAttr GL_CONTEXT_MINOR_VERSION = SDL_GL_CONTEXT_MINOR_VERSION;

/**
 * some combination of 0 or more of elements of the SDL_GLContextFlag
 * enumeration; defaults to 0.
 */
constexpr GLAttr GL_CONTEXT_FLAGS = SDL_GL_CONTEXT_FLAGS;

/**
 * type of GL context (Core, Compatibility, ES). See SDL_GLProfile; default
 * value depends on platform.
 */
constexpr GLAttr GL_CONTEXT_PROFILE_MASK = SDL_GL_CONTEXT_PROFILE_MASK;

/**
 * OpenGL context sharing; defaults to 0.
 */
constexpr GLAttr GL_SHARE_WITH_CURRENT_CONTEXT =
  SDL_GL_SHARE_WITH_CURRENT_CONTEXT;

/**
 * requests sRGB capable visual; defaults to 0.
 */
constexpr GLAttr GL_FRAMEBUFFER_SRGB_CAPABLE = SDL_GL_FRAMEBUFFER_SRGB_CAPABLE;

/**
 * sets context the release behavior. See SDL_GLContextReleaseFlag; defaults to
 * FLUSH.
 */
constexpr GLAttr GL_CONTEXT_RELEASE_BEHAVIOR = SDL_GL_CONTEXT_RELEASE_BEHAVIOR;

/**
 * set context reset notification. See SDL_GLContextResetNotification; defaults
 * to NO_NOTIFICATION.
 */
constexpr GLAttr GL_CONTEXT_RESET_NOTIFICATION =
  SDL_GL_CONTEXT_RESET_NOTIFICATION;

constexpr GLAttr GL_CONTEXT_NO_ERROR = SDL_GL_CONTEXT_NO_ERROR;

constexpr GLAttr GL_FLOATBUFFERS = SDL_GL_FLOATBUFFERS;

constexpr GLAttr GL_EGL_PLATFORM = SDL_GL_EGL_PLATFORM;

/// @}

/**
 * Possible values to be set for the SDL_GL_CONTEXT_PROFILE_MASK attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLProfile = SDL_GLProfile;

/**
 * Possible flags to be set for the SDL_GL_CONTEXT_FLAGS attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLContextFlag = SDL_GLContextFlag;

/**
 * Possible values to be set for the SDL_GL_CONTEXT_RELEASE_BEHAVIOR
 * attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLContextReleaseFlag = SDL_GLContextReleaseFlag;

/**
 * Possible values to be set SDL_GL_CONTEXT_RESET_NOTIFICATION attribute.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using GLContextResetNotification = SDL_GLContextResetNotification;

/**
 * Get the number of video drivers compiled into SDL.
 *
 * @returns the number of built in video drivers.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetVideoDriver()
 */
inline int GetNumVideoDrivers() { return SDL_GetNumVideoDrivers(); }

/**
 * Get the name of a built in video driver.
 *
 * The video drivers are presented in the order in which they are normally
 * checked during initialization.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "cocoa",
 * "x11" or "windows". These never have Unicode characters, and are not meant
 * to be proper names.
 *
 * @param index the index of a video driver.
 * @returns the name of the video driver with the given **index**.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumVideoDrivers()
 */
inline const char* GetVideoDriver(int index)
{
  return SDL_GetVideoDriver(index);
}

/**
 * Get the name of the currently initialized video driver.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "cocoa",
 * "x11" or "windows". These never have Unicode characters, and are not meant
 * to be proper names.
 *
 * @returns the name of the current video driver or NULL if no driver has been
 *          initialized.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumVideoDrivers()
 * @sa GetVideoDriver()
 */
inline const char* GetCurrentVideoDriver()
{
  return SDL_GetCurrentVideoDriver();
}

/**
 * Get the current system theme.
 *
 * @returns the current system theme, light, dark, or unknown.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline SystemTheme GetSystemTheme() { return SDL_GetSystemTheme(); }

/**
 * Get a list of valid windows.
 *
 * @param count a pointer filled in with the number of windows returned, may
 *              be NULL.
 * @returns a NULL terminated array of SDL_Window pointers or NULL on failure;
 *          call GetError() for more information. This is a single
 *          allocation that should be freed with SDL_free() when it is no
 *          longer needed.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline FreeWrapper<WindowRef[]> GetWindows(int* count)
{
  return wrapArray(reinterpret_cast<WindowRef*>(SDL_GetWindows(count)));
}

/**
 * Get a window from a stored ID.
 *
 * The numeric ID is what SDL_WindowEvent references, and is necessary to map
 * these events to specific SDL_Window objects.
 *
 * @param id the ID of the window.
 * @returns the window associated with `id` or NULL if it doesn't exist; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetID()
 */
inline WindowRef GetWindowFromID(WindowID id)
{
  return SDL_GetWindowFromID(id);
}

/**
 * Get the window that currently has an input grab enabled.
 *
 * @returns the window if input is grabbed or NULL otherwise.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetMouseGrab()
 * @sa SetKeyboardGrab()
 */
inline WindowRef GetGrabbedWindow() { return SDL_GetGrabbedWindow(); }

/**
 * @brief  Check whether the screensaver is currently enabled.
 *
 * The screensaver is disabled by default.
 *
 * The default can also be changed using `SDL_HINT_VIDEO_ALLOW_SCREENSAVER`.
 *
 * @returns true if the screensaver is enabled, false if it is disabled.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @sa DisableScreenSaver()
 * @sa EnableScreenSaver()
 */
inline bool ScreenSaverEnabled() { return SDL_ScreenSaverEnabled(); }

/**
 * @brief Allow the screen to be blanked by a screen saver.
 *
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @sa DisableScreenSaver()
 * @sa ScreenSaverEnabled()
 */
inline bool EnableScreenSaver() { return SDL_EnableScreenSaver(); }

/**
 * @brief Prevent the screen from being blanked by a screen saver.
 *
 * If you disable the screensaver, it is automatically re-enabled when SDL
 * quits.
 *
 * The screensaver is disabled by default, but this may by changed by
 * SDL_HINT_VIDEO_ALLOW_SCREENSAVER.
 *
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @sa EnableScreenSaver()
 * @sa ScreenSaverEnabled()
 */
inline bool DisableScreenSaver() { return SDL_DisableScreenSaver(); }

/**
 * Dynamically load an OpenGL library.
 *
 * This should be done after initializing the video driver, but before
 * creating any OpenGL windows. If no OpenGL library is loaded, the default
 * library will be loaded upon creation of the first OpenGL window.
 *
 * If you do this, you need to retrieve all of the GL functions used in your
 * program from the dynamic library using SDL_GL_GetProcAddress().
 *
 * @param path the platform dependent OpenGL library name, or NULL to open the
 *             default OpenGL library.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetProcAddress()
 * @sa GL_UnloadLibrary()
 */
inline bool GL_LoadLibrary(StringParam path)
{
  return SDL_GL_LoadLibrary(path);
}

/**
 * Get an OpenGL function by name.
 *
 * If the GL library is loaded at runtime with SDL_GL_LoadLibrary(), then all
 * GL functions must be retrieved this way. Usually this is used to retrieve
 * function pointers to OpenGL extensions.
 *
 * There are some quirks to looking up OpenGL functions that require some
 * extra care from the application. If you code carefully, you can handle
 * these quirks without any platform-specific code, though:
 *
 * - On Windows, function pointers are specific to the current GL context;
 *   this means you need to have created a GL context and made it current
 *   before calling SDL_GL_GetProcAddress(). If you recreate your context or
 *   create a second context, you should assume that any existing function
 *   pointers aren't valid to use with it. This is (currently) a
 *   Windows-specific limitation, and in practice lots of drivers don't suffer
 *   this limitation, but it is still the way the wgl API is documented to
 *   work and you should expect crashes if you don't respect it. Store a copy
 *   of the function pointers that comes and goes with context lifespan.
 * - On X11, function pointers returned by this function are valid for any
 *   context, and can even be looked up before a context is created at all.
 *   This means that, for at least some common OpenGL implementations, if you
 *   look up a function that doesn't exist, you'll get a non-NULL result that
 *   is _NOT_ safe to call. You must always make sure the function is actually
 *   available for a given GL context before calling it, by checking for the
 *   existence of the appropriate extension with SDL_GL_ExtensionSupported(),
 *   or verifying that the version of OpenGL you're using offers the function
 *   as core functionality.
 * - Some OpenGL drivers, on all platforms, *will* return NULL if a function
 *   isn't supported, but you can't count on this behavior. Check for
 *   extensions you use, and if you get a NULL anyway, act as if that
 *   extension wasn't available. This is probably a bug in the driver, but you
 *   can code defensively for this scenario anyhow.
 * - Just because you're on Linux/Unix, don't assume you'll be using X11.
 *   Next-gen display servers are waiting to replace it, and may or may not
 *   make the same promises about function pointers.
 * - OpenGL function pointers must be declared `APIENTRY` as in the example
 *   code. This will ensure the proper calling convention is followed on
 *   platforms where this matters (Win32) thereby avoiding stack corruption.
 *
 * @param proc the name of an OpenGL function.
 * @returns a pointer to the named OpenGL function. The returned pointer
 *          should be cast to the appropriate function signature.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_ExtensionSupported()
 * @sa GL_LoadLibrary()
 * @sa GL_UnloadLibrary()
 */
inline FunctionPointer GL_GetProcAddress(StringParam proc)
{
  return SDL_GL_GetProcAddress(proc);
}

/**
 * Get an EGL library function by name.
 *
 * If an EGL library is loaded, this function allows applications to get entry
 * points for EGL functions. This is useful to provide to an EGL API and
 * extension loader.
 *
 * @param proc the name of an EGL function.
 * @returns a pointer to the named EGL function. The returned pointer should
 *          be cast to the appropriate function signature.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa EGL_GetCurrentDisplay()
 */
inline FunctionPointer EGL_GetProcAddress(StringParam proc)
{
  return SDL_EGL_GetProcAddress(proc);
}

/**
 * Unload the OpenGL library previously loaded by SDL_GL_LoadLibrary().
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_LoadLibrary()
 */
inline void GL_UnloadLibrary() { SDL_GL_UnloadLibrary(); }

/**
 * Check if an OpenGL extension is supported for the current context.
 *
 * This function operates on the current GL context; you must have created a
 * context and it must be current before calling this function. Do not assume
 * that all contexts you create will have the same set of extensions
 * available, or that recreating an existing context will offer the same
 * extensions again.
 *
 * While it's probably not a massive overhead, this function is not an O(1)
 * operation. Check the extensions you care about after creating the GL
 * context and save that information somewhere instead of calling the function
 * every time you need to know.
 *
 * @param extension the name of the extension to check.
 * @returns true if the extension is supported, false otherwise.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GL_ExtensionSupported(StringParam extension)
{
  return SDL_GL_ExtensionSupported(extension);
}

/**
 * Reset all previously set OpenGL context attributes to their default values.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetAttribute()
 * @sa GL_SetAttribute()
 */
inline void GL_ResetAttributes() { SDL_GL_ResetAttributes(); }

/**
 * Set an OpenGL window attribute before window creation.
 *
 * This function sets the OpenGL attribute `attr` to `value`. The requested
 * attributes should be set before creating an OpenGL window. You should use
 * SDL_GL_GetAttribute() to check the values after creating the OpenGL
 * context, since the values obtained can differ from the requested ones.
 *
 * @param attr an SDL_GLAttr enum value specifying the OpenGL attribute to
 *             set.
 * @param value the desired value for the attribute.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetAttribute()
 * @sa GL_ResetAttributes()
 */
inline bool GL_SetAttribute(GLAttr attr, int value)
{
  return SDL_GL_SetAttribute(attr, value);
}

/**
 * Get the actual value for an attribute from the current context.
 *
 * @param attr an SDL_GLAttr enum value specifying the OpenGL attribute to
 *             get.
 * @param value a pointer filled in with the current value of `attr`.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_ResetAttributes()
 * @sa GL_SetAttribute()
 */
inline bool GL_GetAttribute(GLAttr attr, int* value)
{
  return SDL_GL_GetAttribute(attr, value);
}

/**
 * Create an OpenGL context for an OpenGL window, and make it current.
 *
 * Windows users new to OpenGL should note that, for historical reasons, GL
 * functions added after OpenGL version 1.1 are not available by default.
 * Those functions must be loaded at run-time, either with an OpenGL
 * extension-handling library or with SDL_GL_GetProcAddress() and its related
 * functions.
 *
 * SDL_GLContext is opaque to the application.
 *
 * @param window the window to associate with the context.
 * @returns the OpenGL context associated with `window` or NULL on failure;
 *          call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_DestroyContext()
 * @sa GL_MakeCurrent()
 */
inline GLContext GL_CreateContext(WindowRef window)
{
  return SDL_GL_CreateContext(window.get());
}

/**
 * Set up an OpenGL context for rendering into an OpenGL window.
 *
 * The context must have been created with a compatible window.
 *
 * @param window the window to associate with the context.
 * @param context the OpenGL context to associate with the window.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_CreateContext()
 */
inline bool GL_MakeCurrent(WindowRef window, GLContext context)
{
  return SDL_GL_MakeCurrent(window.get(), context);
}

/**
 * Get the currently active OpenGL window.
 *
 * @returns the currently active OpenGL window on success or NULL on failure;
 *          call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline WindowRef GL_GetCurrentWindow() { return SDL_GL_GetCurrentWindow(); }

/**
 * Get the currently active OpenGL context.
 *
 * @returns the currently active OpenGL context or NULL on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_MakeCurrent()
 */
inline GLContext GL_GetCurrentContext() { return SDL_GL_GetCurrentContext(); }

/**
 * Get the currently active EGL display.
 *
 * @returns the currently active EGL display or NULL on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline EGLDisplay EGL_GetCurrentDisplay()
{
  return SDL_EGL_GetCurrentDisplay();
}

/**
 * Get the currently active EGL config.
 *
 * @returns the currently active EGL config or NULL on failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline EGLConfig EGL_GetCurrentConfig() { return SDL_EGL_GetCurrentConfig(); }

/**
 * Get the EGL surface associated with the window.
 *
 * @param window the window to query.
 * @returns the EGLSurface pointer associated with the window, or NULL on
 *          failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline EGLSurface EGL_GetWindowSurface(WindowRef window)
{
  return SDL_EGL_GetWindowSurface(window.get());
}

/**
 * Sets the callbacks for defining custom EGLAttrib arrays for EGL
 * initialization.
 *
 * Callbacks that aren't needed can be set to NULL.
 *
 * NOTE: These callback pointers will be reset after SDL_GL_ResetAttributes.
 *
 * @param platformAttribCallback callback for attributes to pass to
 *                               eglGetPlatformDisplay. May be NULL.
 * @param surfaceAttribCallback callback for attributes to pass to
 *                              eglCreateSurface. May be NULL.
 * @param contextAttribCallback callback for attributes to pass to
 *                              eglCreateContext. May be NULL.
 * @param userdata a pointer that is passed to the callbacks.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EGL_SetAttributeCallbacks(
  EGLAttribArrayCallback platformAttribCallback,
  EGLIntArrayCallback surfaceAttribCallback,
  EGLIntArrayCallback contextAttribCallback,
  void* userdata)
{
  SDL_EGL_SetAttributeCallbacks(platformAttribCallback,
                                surfaceAttribCallback,
                                contextAttribCallback,
                                userdata);
}

/**
 * Set the swap interval for the current OpenGL context.
 *
 * Some systems allow specifying -1 for the interval, to enable adaptive
 * vsync. Adaptive vsync works the same as vsync, but if you've already missed
 * the vertical retrace for a given frame, it swaps buffers immediately, which
 * might be less jarring for the user during occasional framerate drops. If an
 * application requests adaptive vsync and the system does not support it,
 * this function will fail and return false. In such a case, you should
 * probably retry the call with 1 for the interval.
 *
 * Adaptive vsync is implemented for some glX drivers with
 * GLX_EXT_swap_control_tear, and for some Windows drivers with
 * WGL_EXT_swap_control_tear.
 *
 * Read more on the Khronos wiki:
 * https://www.khronos.org/opengl/wiki/Swap_Interval#Adaptive_Vsync
 *
 * @param interval 0 for immediate updates, 1 for updates synchronized with
 *                 the vertical retrace, -1 for adaptive vsync.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_GetSwapInterval()
 */
inline bool GL_SetSwapInterval(int interval)
{
  return SDL_GL_SetSwapInterval(interval);
}

/**
 * Get the swap interval for the current OpenGL context.
 *
 * If the system can't determine the swap interval, or there isn't a valid
 * current context, this function will set *interval to 0 as a safe default.
 *
 * @param interval output interval value. 0 if there is no vertical retrace
 *                 synchronization, 1 if the buffer swap is synchronized with
 *                 the vertical retrace, and -1 if late swaps happen
 *                 immediately instead of waiting for the next retrace.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_SetSwapInterval()
 */
inline bool GL_GetSwapInterval(int* interval)
{
  return SDL_GL_GetSwapInterval(interval);
}

/**
 * Update a window with OpenGL rendering.
 *
 * This is used with double-buffered OpenGL contexts, which are the default.
 *
 * On macOS, make sure you bind 0 to the draw framebuffer before swapping the
 * window, otherwise nothing will happen. If you aren't using
 * glBindFramebuffer(), this is the default and you won't have to do anything
 * extra.
 *
 * @param window the window to change.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GL_SwapWindow(WindowRef window)
{
  return SDL_GL_SwapWindow(window.get());
}

/**
 * Delete an OpenGL context.
 *
 * @param context the OpenGL context to be deleted.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GL_CreateContext()
 */
inline bool GL_DestroyContext(GLContext context)
{
  return SDL_GL_DestroyContext(context);
}

#pragma region impl

/// @}

inline void ObjectDeleter<SDL_Window>::operator()(WindowRef window) const
{
  window.Destroy();
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_VIDEO_H_ */


// end --- SDL3pp_video.h --- 



namespace SDL {

/**
 * @defgroup CategoryEvents Category Events
 *
 * Event queue management.
 *
 * It's extremely common--often required--that an app deal with SDL's event
 * queue. Almost all useful information about interactions with the real world
 * flow through here: the user interacting with the computer and app, hardware
 * coming and going, the system changing in some way, etc.
 *
 * An app generally takes a moment, perhaps at the start of a new frame, to
 * examine any events that have occurred since the last time and process or
 * ignore them. This is generally done by calling SDL_PollEvent() in a loop
 * until it returns false (or, if using the main callbacks, events are
 * provided one at a time in calls to SDL_AppEvent() before the next call to
 * SDL_AppIterate(); in this scenario, the app does not call PollEvent()
 * at all).
 *
 * There is other forms of control, too: PeepEvents() has more
 * functionality at the cost of more complexity, and WaitEvent() can block
 * the process until something interesting happens, which might be beneficial
 * for certain types of programs on low-power hardware. One may also call
 * AddEventWatch() to set a callback when new events arrive.
 *
 * The app is free to generate their own events, too: PushEvent() allows the
 * app to put events onto the queue for later retrieval; RegisterEvents()
 * can guarantee that these events have a type that isn't in use by other
 * parts of the system.
 *
 * @{
 */

/**
 * The types of events that can be delivered.
 *
 * @since This enum is available since SDL 3.2.0.
 * @sa EventTypes
 */
using EventType = SDL_EventType;

/**
 * @defgroup EventTypes Event Types
 *
 * Event types for Event.
 * @{
 */

/**
 * Unused (do not remove)
 */
constexpr EventType EVENT_FIRST = SDL_EVENT_FIRST;

/**
 * User-requested quit
 */
constexpr EventType EVENT_QUIT = SDL_EVENT_QUIT;

constexpr EventType EVENT_TERMINATING = SDL_EVENT_TERMINATING;

constexpr EventType EVENT_LOW_MEMORY = SDL_EVENT_LOW_MEMORY;

constexpr EventType EVENT_WILL_ENTER_BACKGROUND =
  SDL_EVENT_WILL_ENTER_BACKGROUND;

constexpr EventType EVENT_DID_ENTER_BACKGROUND = SDL_EVENT_DID_ENTER_BACKGROUND;

constexpr EventType EVENT_WILL_ENTER_FOREGROUND =
  SDL_EVENT_WILL_ENTER_FOREGROUND;

constexpr EventType EVENT_DID_ENTER_FOREGROUND = SDL_EVENT_DID_ENTER_FOREGROUND;

/**
 * The user's locale preferences have changed.
 */
constexpr EventType EVENT_LOCALE_CHANGED = SDL_EVENT_LOCALE_CHANGED;

/**
 * The system theme changed
 */
constexpr EventType EVENT_SYSTEM_THEME_CHANGED = SDL_EVENT_SYSTEM_THEME_CHANGED;

/**
 * Display orientation has changed to data1
 */
constexpr EventType EVENT_DISPLAY_ORIENTATION = SDL_EVENT_DISPLAY_ORIENTATION;

/**
 * Display has been added to the system
 */
constexpr EventType EVENT_DISPLAY_ADDED = SDL_EVENT_DISPLAY_ADDED;

/**
 * Display has been removed from the system
 */
constexpr EventType EVENT_DISPLAY_REMOVED = SDL_EVENT_DISPLAY_REMOVED;

/**
 * Display has changed position
 */
constexpr EventType EVENT_DISPLAY_MOVED = SDL_EVENT_DISPLAY_MOVED;

/**
 * Display has changed desktop mode
 */
constexpr EventType EVENT_DISPLAY_DESKTOP_MODE_CHANGED =
  SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED;

/**
 * Display has changed current mode
 */
constexpr EventType EVENT_DISPLAY_CURRENT_MODE_CHANGED =
  SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED;

/**
 * Display has changed content scale
 */
constexpr EventType EVENT_DISPLAY_CONTENT_SCALE_CHANGED =
  SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED;

constexpr EventType EVENT_DISPLAY_FIRST = SDL_EVENT_DISPLAY_FIRST;

constexpr EventType EVENT_DISPLAY_LAST = SDL_EVENT_DISPLAY_LAST;

/**
 * Window has been shown
 */
constexpr EventType EVENT_WINDOW_SHOWN = SDL_EVENT_WINDOW_SHOWN;

/**
 * Window has been hidden
 */
constexpr EventType EVENT_WINDOW_HIDDEN = SDL_EVENT_WINDOW_HIDDEN;

/**
 * Window has been exposed and should be redrawn, and can be redrawn directly
 * from event watchers for this event
 */
constexpr EventType EVENT_WINDOW_EXPOSED = SDL_EVENT_WINDOW_EXPOSED;

/**
 * Window has been moved to data1, data2
 */
constexpr EventType EVENT_WINDOW_MOVED = SDL_EVENT_WINDOW_MOVED;

/**
 * Window has been resized to data1xdata2
 */
constexpr EventType EVENT_WINDOW_RESIZED = SDL_EVENT_WINDOW_RESIZED;

/**
 * The pixel size of the window has changed to data1xdata2
 */
constexpr EventType EVENT_WINDOW_PIXEL_SIZE_CHANGED =
  SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED;

/**
 * The pixel size of a Metal view associated with the window has changed
 */
constexpr EventType EVENT_WINDOW_METAL_VIEW_RESIZED =
  SDL_EVENT_WINDOW_METAL_VIEW_RESIZED;

/**
 * Window has been minimized
 */
constexpr EventType EVENT_WINDOW_MINIMIZED = SDL_EVENT_WINDOW_MINIMIZED;

/**
 * Window has been maximized
 */
constexpr EventType EVENT_WINDOW_MAXIMIZED = SDL_EVENT_WINDOW_MAXIMIZED;

/**
 * Window has been restored to normal size and position
 */
constexpr EventType EVENT_WINDOW_RESTORED = SDL_EVENT_WINDOW_RESTORED;

/**
 * Window has gained mouse focus
 */
constexpr EventType EVENT_WINDOW_MOUSE_ENTER = SDL_EVENT_WINDOW_MOUSE_ENTER;

/**
 * Window has lost mouse focus
 */
constexpr EventType EVENT_WINDOW_MOUSE_LEAVE = SDL_EVENT_WINDOW_MOUSE_LEAVE;

/**
 * Window has gained keyboard focus
 */
constexpr EventType EVENT_WINDOW_FOCUS_GAINED = SDL_EVENT_WINDOW_FOCUS_GAINED;

/**
 * Window has lost keyboard focus
 */
constexpr EventType EVENT_WINDOW_FOCUS_LOST = SDL_EVENT_WINDOW_FOCUS_LOST;

/**
 * The window manager requests that the window be closed
 */
constexpr EventType EVENT_WINDOW_CLOSE_REQUESTED =
  SDL_EVENT_WINDOW_CLOSE_REQUESTED;

/**
 * Window had a hit test that wasn't SDL_HITTEST_NORMAL
 */
constexpr EventType EVENT_WINDOW_HIT_TEST = SDL_EVENT_WINDOW_HIT_TEST;

/**
 * The ICC profile of the window's display has changed
 */
constexpr EventType EVENT_WINDOW_ICCPROF_CHANGED =
  SDL_EVENT_WINDOW_ICCPROF_CHANGED;

/**
 * Window has been moved to display data1
 */
constexpr EventType EVENT_WINDOW_DISPLAY_CHANGED =
  SDL_EVENT_WINDOW_DISPLAY_CHANGED;

/**
 * Window display scale has been changed
 */
constexpr EventType EVENT_WINDOW_DISPLAY_SCALE_CHANGED =
  SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED;

/**
 * The window safe area has been changed
 */
constexpr EventType EVENT_WINDOW_SAFE_AREA_CHANGED =
  SDL_EVENT_WINDOW_SAFE_AREA_CHANGED;

/**
 * The window has been occluded
 */
constexpr EventType EVENT_WINDOW_OCCLUDED = SDL_EVENT_WINDOW_OCCLUDED;

/**
 * The window has entered fullscreen mode
 */
constexpr EventType EVENT_WINDOW_ENTER_FULLSCREEN =
  SDL_EVENT_WINDOW_ENTER_FULLSCREEN;

/**
 * The window has left fullscreen mode
 */
constexpr EventType EVENT_WINDOW_LEAVE_FULLSCREEN =
  SDL_EVENT_WINDOW_LEAVE_FULLSCREEN;

constexpr EventType EVENT_WINDOW_DESTROYED = SDL_EVENT_WINDOW_DESTROYED;

/**
 * Window HDR properties have changed
 */
constexpr EventType EVENT_WINDOW_HDR_STATE_CHANGED =
  SDL_EVENT_WINDOW_HDR_STATE_CHANGED;

constexpr EventType EVENT_WINDOW_FIRST = SDL_EVENT_WINDOW_FIRST;

constexpr EventType EVENT_WINDOW_LAST = SDL_EVENT_WINDOW_LAST;

/**
 * Key pressed
 */
constexpr EventType EVENT_KEY_DOWN = SDL_EVENT_KEY_DOWN;

/**
 * Key released
 */
constexpr EventType EVENT_KEY_UP = SDL_EVENT_KEY_UP;

/**
 * Keyboard text editing (composition)
 */
constexpr EventType EVENT_TEXT_EDITING = SDL_EVENT_TEXT_EDITING;

/**
 * Keyboard text input
 */
constexpr EventType EVENT_TEXT_INPUT = SDL_EVENT_TEXT_INPUT;

constexpr EventType EVENT_KEYMAP_CHANGED = SDL_EVENT_KEYMAP_CHANGED;

/**
 * A new keyboard has been inserted into the system
 */
constexpr EventType EVENT_KEYBOARD_ADDED = SDL_EVENT_KEYBOARD_ADDED;

/**
 * A keyboard has been removed
 */
constexpr EventType EVENT_KEYBOARD_REMOVED = SDL_EVENT_KEYBOARD_REMOVED;

/**
 * Keyboard text editing candidates
 */
constexpr EventType EVENT_TEXT_EDITING_CANDIDATES =
  SDL_EVENT_TEXT_EDITING_CANDIDATES;

/**
 * Mouse moved
 */
constexpr EventType EVENT_MOUSE_MOTION = SDL_EVENT_MOUSE_MOTION;

/**
 * Mouse button pressed
 */
constexpr EventType EVENT_MOUSE_BUTTON_DOWN = SDL_EVENT_MOUSE_BUTTON_DOWN;

/**
 * Mouse button released
 */
constexpr EventType EVENT_MOUSE_BUTTON_UP = SDL_EVENT_MOUSE_BUTTON_UP;

/**
 * Mouse wheel motion
 */
constexpr EventType EVENT_MOUSE_WHEEL = SDL_EVENT_MOUSE_WHEEL;

/**
 * A new mouse has been inserted into the system
 */
constexpr EventType EVENT_MOUSE_ADDED = SDL_EVENT_MOUSE_ADDED;

/**
 * A mouse has been removed
 */
constexpr EventType EVENT_MOUSE_REMOVED = SDL_EVENT_MOUSE_REMOVED;

/**
 * Joystick axis motion
 */
constexpr EventType EVENT_JOYSTICK_AXIS_MOTION = SDL_EVENT_JOYSTICK_AXIS_MOTION;

/**
 * Joystick trackball motion
 */
constexpr EventType EVENT_JOYSTICK_BALL_MOTION = SDL_EVENT_JOYSTICK_BALL_MOTION;

/**
 * Joystick hat position change
 */
constexpr EventType EVENT_JOYSTICK_HAT_MOTION = SDL_EVENT_JOYSTICK_HAT_MOTION;

/**
 * Joystick button pressed
 */
constexpr EventType EVENT_JOYSTICK_BUTTON_DOWN = SDL_EVENT_JOYSTICK_BUTTON_DOWN;

/**
 * Joystick button released
 */
constexpr EventType EVENT_JOYSTICK_BUTTON_UP = SDL_EVENT_JOYSTICK_BUTTON_UP;

/**
 * A new joystick has been inserted into the system
 */
constexpr EventType EVENT_JOYSTICK_ADDED = SDL_EVENT_JOYSTICK_ADDED;

/**
 * An opened joystick has been removed
 */
constexpr EventType EVENT_JOYSTICK_REMOVED = SDL_EVENT_JOYSTICK_REMOVED;

/**
 * Joystick battery level change
 */
constexpr EventType EVENT_JOYSTICK_BATTERY_UPDATED =
  SDL_EVENT_JOYSTICK_BATTERY_UPDATED;

/**
 * Joystick update is complete
 */
constexpr EventType EVENT_JOYSTICK_UPDATE_COMPLETE =
  SDL_EVENT_JOYSTICK_UPDATE_COMPLETE;

/**
 * Gamepad axis motion
 */
constexpr EventType EVENT_GAMEPAD_AXIS_MOTION = SDL_EVENT_GAMEPAD_AXIS_MOTION;

/**
 * Gamepad button pressed
 */
constexpr EventType EVENT_GAMEPAD_BUTTON_DOWN = SDL_EVENT_GAMEPAD_BUTTON_DOWN;

/**
 * Gamepad button released
 */
constexpr EventType EVENT_GAMEPAD_BUTTON_UP = SDL_EVENT_GAMEPAD_BUTTON_UP;

/**
 * A new gamepad has been inserted into the system
 */
constexpr EventType EVENT_GAMEPAD_ADDED = SDL_EVENT_GAMEPAD_ADDED;

/**
 * A gamepad has been removed
 */
constexpr EventType EVENT_GAMEPAD_REMOVED = SDL_EVENT_GAMEPAD_REMOVED;

/**
 * The gamepad mapping was updated
 */
constexpr EventType EVENT_GAMEPAD_REMAPPED = SDL_EVENT_GAMEPAD_REMAPPED;

/**
 * Gamepad touchpad was touched
 */
constexpr EventType EVENT_GAMEPAD_TOUCHPAD_DOWN =
  SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN;

/**
 * Gamepad touchpad finger was moved
 */
constexpr EventType EVENT_GAMEPAD_TOUCHPAD_MOTION =
  SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION;

/**
 * Gamepad touchpad finger was lifted
 */
constexpr EventType EVENT_GAMEPAD_TOUCHPAD_UP = SDL_EVENT_GAMEPAD_TOUCHPAD_UP;

/**
 * Gamepad sensor was updated
 */
constexpr EventType EVENT_GAMEPAD_SENSOR_UPDATE =
  SDL_EVENT_GAMEPAD_SENSOR_UPDATE;

/**
 * Gamepad update is complete
 */
constexpr EventType EVENT_GAMEPAD_UPDATE_COMPLETE =
  SDL_EVENT_GAMEPAD_UPDATE_COMPLETE;

/**
 * Gamepad Steam handle has changed
 */
constexpr EventType EVENT_GAMEPAD_STEAM_HANDLE_UPDATED =
  SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED;

constexpr EventType EVENT_FINGER_DOWN = SDL_EVENT_FINGER_DOWN;

constexpr EventType EVENT_FINGER_UP = SDL_EVENT_FINGER_UP;

constexpr EventType EVENT_FINGER_MOTION = SDL_EVENT_FINGER_MOTION;

constexpr EventType EVENT_FINGER_CANCELED = SDL_EVENT_FINGER_CANCELED;

/**
 * The clipboard or primary selection changed
 */
constexpr EventType EVENT_CLIPBOARD_UPDATE = SDL_EVENT_CLIPBOARD_UPDATE;

/**
 * The system requests a file open
 */
constexpr EventType EVENT_DROP_FILE = SDL_EVENT_DROP_FILE;

/**
 * text/plain drag-and-drop event
 */
constexpr EventType EVENT_DROP_TEXT = SDL_EVENT_DROP_TEXT;

/**
 * A new set of drops is beginning (NULL filename)
 */
constexpr EventType EVENT_DROP_BEGIN = SDL_EVENT_DROP_BEGIN;

/**
 * Current set of drops is now complete (NULL filename)
 */
constexpr EventType EVENT_DROP_COMPLETE = SDL_EVENT_DROP_COMPLETE;

/**
 * Position while moving over the window
 */
constexpr EventType EVENT_DROP_POSITION = SDL_EVENT_DROP_POSITION;

/**
 * A new audio device is available
 */
constexpr EventType EVENT_AUDIO_DEVICE_ADDED = SDL_EVENT_AUDIO_DEVICE_ADDED;

/**
 * An audio device has been removed.
 */
constexpr EventType EVENT_AUDIO_DEVICE_REMOVED = SDL_EVENT_AUDIO_DEVICE_REMOVED;

/**
 * An audio device's format has been changed by the system.
 */
constexpr EventType EVENT_AUDIO_DEVICE_FORMAT_CHANGED =
  SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED;

/**
 * A sensor was updated
 */
constexpr EventType EVENT_SENSOR_UPDATE = SDL_EVENT_SENSOR_UPDATE;

/**
 * Pressure-sensitive pen has become available
 */
constexpr EventType EVENT_PEN_PROXIMITY_IN = SDL_EVENT_PEN_PROXIMITY_IN;

/**
 * Pressure-sensitive pen has become unavailable
 */
constexpr EventType EVENT_PEN_PROXIMITY_OUT = SDL_EVENT_PEN_PROXIMITY_OUT;

/**
 * Pressure-sensitive pen touched drawing surface
 */
constexpr EventType EVENT_PEN_DOWN = SDL_EVENT_PEN_DOWN;

/**
 * Pressure-sensitive pen stopped touching drawing surface
 */
constexpr EventType EVENT_PEN_UP = SDL_EVENT_PEN_UP;

/**
 * Pressure-sensitive pen button pressed
 */
constexpr EventType EVENT_PEN_BUTTON_DOWN = SDL_EVENT_PEN_BUTTON_DOWN;

/**
 * Pressure-sensitive pen button released
 */
constexpr EventType EVENT_PEN_BUTTON_UP = SDL_EVENT_PEN_BUTTON_UP;

/**
 * Pressure-sensitive pen is moving on the tablet
 */
constexpr EventType EVENT_PEN_MOTION = SDL_EVENT_PEN_MOTION;

/**
 * Pressure-sensitive pen angle/pressure/etc changed
 */
constexpr EventType EVENT_PEN_AXIS = SDL_EVENT_PEN_AXIS;

/**
 * A new camera device is available
 */
constexpr EventType EVENT_CAMERA_DEVICE_ADDED = SDL_EVENT_CAMERA_DEVICE_ADDED;

/**
 * A camera device has been removed.
 */
constexpr EventType EVENT_CAMERA_DEVICE_REMOVED =
  SDL_EVENT_CAMERA_DEVICE_REMOVED;

/**
 * A camera device has been approved for use by the user.
 */
constexpr EventType EVENT_CAMERA_DEVICE_APPROVED =
  SDL_EVENT_CAMERA_DEVICE_APPROVED;

/**
 * A camera device has been denied for use by the user.
 */
constexpr EventType EVENT_CAMERA_DEVICE_DENIED = SDL_EVENT_CAMERA_DEVICE_DENIED;

/**
 * The render targets have been reset and their contents need to be updated
 */
constexpr EventType EVENT_RENDER_TARGETS_RESET = SDL_EVENT_RENDER_TARGETS_RESET;

/**
 * The device has been reset and all textures need to be recreated
 */
constexpr EventType EVENT_RENDER_DEVICE_RESET = SDL_EVENT_RENDER_DEVICE_RESET;

/**
 * The device has been lost and can't be recovered.
 */
constexpr EventType EVENT_RENDER_DEVICE_LOST = SDL_EVENT_RENDER_DEVICE_LOST;

constexpr EventType EVENT_PRIVATE0 = SDL_EVENT_PRIVATE0;

constexpr EventType EVENT_PRIVATE1 = SDL_EVENT_PRIVATE1;

constexpr EventType EVENT_PRIVATE2 = SDL_EVENT_PRIVATE2;

constexpr EventType EVENT_PRIVATE3 = SDL_EVENT_PRIVATE3;

/**
 * Signals the end of an event poll cycle
 */
constexpr EventType EVENT_POLL_SENTINEL = SDL_EVENT_POLL_SENTINEL;

/**
 * Events SDL_EVENT_USER through SDL_EVENT_LAST are for your use,
 *  and should be allocated with SDL_RegisterEvents()
 */
constexpr EventType EVENT_USER = SDL_EVENT_USER;

/**
 * This last event is only for bounding internal arrays
 */
constexpr EventType EVENT_LAST = SDL_EVENT_LAST;

constexpr EventType EVENT_ENUM_PADDING = SDL_EVENT_ENUM_PADDING;

/// @}

/**
 * Fields shared by every event
 *
 * @since This struct is available since SDL 3.2.0.
 */
using CommonEvent = SDL_CommonEvent;

/**
 * Display state change event data (event.display.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using DisplayEvent = SDL_DisplayEvent;

/**
 * Window state change event data (event.window.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using WindowEvent = SDL_WindowEvent;

/**
 * Keyboard device event structure (event.kdevice.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using KeyboardDeviceEvent = SDL_KeyboardDeviceEvent;

/**
 * Keyboard button event structure (event.key.*)
 *
 * The `key` is the base SDL_Keycode generated by pressing the `scancode`
 * using the current keyboard layout, applying any options specified in
 * SDL_HINT_KEYCODE_OPTIONS. You can get the SDL_Keycode corresponding to the
 * event scancode and modifiers directly from the keyboard layout, bypassing
 * SDL_HINT_KEYCODE_OPTIONS, by calling SDL_GetKeyFromScancode().
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GetKeyFromScancode()
 * @sa HINT_KEYCODE_OPTIONS
 */
using KeyboardEvent = SDL_KeyboardEvent;

/**
 * Keyboard text editing event structure (event.edit.*)
 *
 * The start cursor is the position, in UTF-8 characters, where new typing
 * will be inserted into the editing text. The length is the number of UTF-8
 * characters that will be replaced by new typing.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using TextEditingEvent = SDL_TextEditingEvent;

/**
 * Keyboard IME candidates event structure (event.edit_candidates.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using TextEditingCandidatesEvent = SDL_TextEditingCandidatesEvent;

/**
 * Keyboard text input event structure (event.text.*)
 *
 * This event will never be delivered unless text input is enabled by calling
 * SDL_StartTextInput(). Text input is disabled by default!
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa StartTextInput()
 * @sa StopTextInput()
 */
using TextInputEvent = SDL_TextInputEvent;

/**
 * Mouse device event structure (event.mdevice.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MouseDeviceEvent = SDL_MouseDeviceEvent;

/**
 * Mouse motion event structure (event.motion.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MouseMotionEvent = SDL_MouseMotionEvent;

/**
 * Mouse button event structure (event.button.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MouseButtonEvent = SDL_MouseButtonEvent;

/**
 * Mouse wheel event structure (event.wheel.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MouseWheelEvent = SDL_MouseWheelEvent;

/**
 * Joystick axis motion event structure (event.jaxis.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyAxisEvent = SDL_JoyAxisEvent;

/**
 * Joystick trackball motion event structure (event.jball.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyBallEvent = SDL_JoyBallEvent;

/**
 * Joystick hat position change event structure (event.jhat.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyHatEvent = SDL_JoyHatEvent;

/**
 * Joystick button event structure (event.jbutton.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyButtonEvent = SDL_JoyButtonEvent;

/**
 * Joystick device event structure (event.jdevice.*)
 *
 * SDL will send JOYSTICK_ADDED events for devices that are already plugged in
 * during SDL_Init.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GamepadDeviceEvent
 */
using JoyDeviceEvent = SDL_JoyDeviceEvent;

/**
 * Joystick battery level change event structure (event.jbattery.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyBatteryEvent = SDL_JoyBatteryEvent;

/**
 * Gamepad axis motion event structure (event.gaxis.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using GamepadAxisEvent = SDL_GamepadAxisEvent;

/**
 * Gamepad button event structure (event.gbutton.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using GamepadButtonEvent = SDL_GamepadButtonEvent;

/**
 * Gamepad device event structure (event.gdevice.*)
 *
 * Joysticks that are supported gamepads receive both an SDL_JoyDeviceEvent
 * and an SDL_GamepadDeviceEvent.
 *
 * SDL will send GAMEPAD_ADDED events for joysticks that are already plugged
 * in during SDL_Init() and are recognized as gamepads. It will also send
 * events for joysticks that get gamepad mappings at runtime.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa JoyDeviceEvent
 */
using GamepadDeviceEvent = SDL_GamepadDeviceEvent;

/**
 * Gamepad touchpad event structure (event.gtouchpad.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using GamepadTouchpadEvent = SDL_GamepadTouchpadEvent;

/**
 * Gamepad sensor event structure (event.gsensor.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using GamepadSensorEvent = SDL_GamepadSensorEvent;

/**
 * Audio device event structure (event.adevice.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using AudioDeviceEvent = SDL_AudioDeviceEvent;

/**
 * Camera device event structure (event.cdevice.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using CameraDeviceEvent = SDL_CameraDeviceEvent;

/**
 * Renderer event structure (event.render.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using RenderEvent = SDL_RenderEvent;

/**
 * Touch finger event structure (event.tfinger.*)
 *
 * Coordinates in this event are normalized. `x` and `y` are normalized to a
 * range between 0.0f and 1.0f, relative to the window, so (0,0) is the top
 * left and (1,1) is the bottom right. Delta coordinates `dx` and `dy` are
 * normalized in the ranges of -1.0f (traversed all the way from the bottom or
 * right to all the way up or left) to 1.0f (traversed all the way from the
 * top or left to all the way down or right).
 *
 * Note that while the coordinates are _normalized_, they are not _clamped_,
 * which means in some circumstances you can get a value outside of this
 * range. For example, a renderer using logical presentation might give a
 * negative value when the touch is in the letterboxing. Some platforms might
 * report a touch outside of the window, which will also be outside of the
 * range.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using TouchFingerEvent = SDL_TouchFingerEvent;

/**
 * Pressure-sensitive pen proximity event structure (event.pmotion.*)
 *
 * When a pen becomes visible to the system (it is close enough to a tablet,
 * etc), SDL will send an SDL_EVENT_PEN_PROXIMITY_IN event with the new pen's
 * ID. This ID is valid until the pen leaves proximity again (has been removed
 * from the tablet's area, the tablet has been unplugged, etc). If the same
 * pen reenters proximity again, it will be given a new ID.
 *
 * Note that "proximity" means "close enough for the tablet to know the tool
 * is there." The pen touching and lifting off from the tablet while not
 * leaving the area are handled by SDL_EVENT_PEN_DOWN and SDL_EVENT_PEN_UP.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenProximityEvent = SDL_PenProximityEvent;

/**
 * Pressure-sensitive pen motion event structure (event.pmotion.*)
 *
 * Depending on the hardware, you may get motion events when the pen is not
 * touching a tablet, for tracking a pen even when it isn't drawing. You
 * should listen for SDL_EVENT_PEN_DOWN and SDL_EVENT_PEN_UP events, or check
 * `pen_state & SDL_PEN_INPUT_DOWN` to decide if a pen is "drawing" when
 * dealing with pen motion.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenMotionEvent = SDL_PenMotionEvent;

/**
 * Pressure-sensitive pen touched event structure (event.ptouch.*)
 *
 * These events come when a pen touches a surface (a tablet, etc), or lifts
 * off from one.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenTouchEvent = SDL_PenTouchEvent;

/**
 * Pressure-sensitive pen button event structure (event.pbutton.*)
 *
 * This is for buttons on the pen itself that the user might click. The pen
 * itself pressing down to draw triggers a SDL_EVENT_PEN_DOWN event instead.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenButtonEvent = SDL_PenButtonEvent;

/**
 * Pressure-sensitive pen pressure / angle event structure (event.paxis.*)
 *
 * You might get some of these events even if the pen isn't touching the
 * tablet.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenAxisEvent = SDL_PenAxisEvent;

/**
 * An event used to drop text or request a file open by the system
 * (event.drop.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using DropEvent = SDL_DropEvent;

/**
 * An event triggered when the clipboard contents have changed
 * (event.clipboard.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using ClipboardEvent = SDL_ClipboardEvent;

/**
 * Sensor event structure (event.sensor.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using SensorEvent = SDL_SensorEvent;

/**
 * The "quit requested" event
 *
 * @since This struct is available since SDL 3.2.0.
 */
using QuitEvent = SDL_QuitEvent;

/**
 * A user-defined event type (event.user.*)
 *
 * This event is unique; it is never created by SDL, but only by the
 * application. The event can be pushed onto the event queue using
 * SDL_PushEvent(). The contents of the structure members are completely up to
 * the programmer; the only requirement is that '''type''' is a value obtained
 * from SDL_RegisterEvents().
 *
 * @since This struct is available since SDL 3.2.0.
 */
using UserEvent = SDL_UserEvent;

/**
 * The structure for all events in SDL.
 *
 * The SDL_Event structure is the core of all event handling in SDL. SDL_Event
 * is a union of all event structures used in SDL.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using Event = SDL_Event;

/**
 * Pump the event loop, gathering events from the input devices.
 *
 * This function updates the event queue and internal input device state.
 *
 * SDL_PumpEvents() gathers all the pending input information from devices and
 * places it in the event queue. Without calls to SDL_PumpEvents() no events
 * would ever be placed on the queue. Often the need for calls to
 * SDL_PumpEvents() is hidden from the user since SDL_PollEvent() and
 * SDL_WaitEvent() implicitly call SDL_PumpEvents(). However, if you are not
 * polling or waiting for events (e.g. you are filtering them), then you must
 * call SDL_PumpEvents() to force an event queue update.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa WaitEvent()
 */
inline void PumpEvents() { return SDL_PumpEvents(); }

/**
 * @name EventActions
 * The type of action to request from PeepEvents().
 * @{
 */

/**
 * The type of action to request from PeepEvents().
 *
 * @since This enum is available since SDL 3.2.0.
 */
using EventAction = SDL_EventAction;

/**
 * Add events to the back of the queue.
 */
constexpr EventAction ADDEVENT = SDL_ADDEVENT;

/**
 * Check but don't remove events from the queue front.
 */
constexpr EventAction PEEKEVENT = SDL_PEEKEVENT;

/**
 * Retrieve/remove events from the front of the queue.
 */
constexpr EventAction GETEVENT = SDL_GETEVENT;

/// @}

/**
 * Check the event queue for messages and optionally return them.
 *
 * `action` may be any of the following:
 *
 * ## remarks
 *
 * - `SDL_ADDEVENT`: up to `numevents` events will be added to the back of the
 *   event queue.
 * - `SDL_PEEKEVENT`: `numevents` events at the front of the event queue,
 *   within the specified minimum and maximum type, will be returned to the
 *   caller and will _not_ be removed from the queue. If you pass NULL for
 *   `events`, then `numevents` is ignored and the total number of matching
 *   events will be returned.
 * - `SDL_GETEVENT`: up to `numevents` events at the front of the event queue,
 *   within the specified minimum and maximum type, will be returned to the
 *   caller and will be removed from the queue.
 *
 * You may have to call SDL_PumpEvents() before calling this function.
 * Otherwise, the events may not be ready to be filtered when you call
 * SDL_PeepEvents().
 *
 * @param events destination buffer for the retrieved events, may be NULL to
 *               leave the events in the queue and return the number of events
 *               that would have been stored.
 * @param numevents if action is SDL_ADDEVENT, the number of events to add
 *                  back to the event queue; if action is SDL_PEEKEVENT or
 *                  SDL_GETEVENT, the maximum number of events to retrieve.
 * @param action action to take; see [Remarks](#remarks) for details.
 * @param minType minimum value of the event type to be considered;
 *                SDL_EVENT_FIRST is a safe choice.
 * @param maxType maximum value of the event type to be considered;
 *                SDL_EVENT_LAST is a safe choice.
 * @returns the number of events actually stored or -1 on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PumpEvents()
 * @sa PushEvent()
 */
inline int PeepEvents(Event* events,
                      int numevents,
                      EventAction action,
                      Uint32 minType = EVENT_FIRST,
                      Uint32 maxType = EVENT_LAST)
{
  return SDL_PeepEvents(events, numevents, action, minType, maxType);
}

/**
 * Check for the existence of a certain event type in the event queue.
 *
 * If you need to check for a range of event types, use SDL_HasEvents()
 * instead.
 *
 * @param type the type of event to be queried; see @ref EventTypes for
 * details.
 * @returns true if events matching `type` are present, or false if events
 *          matching `type` are not present.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasEvents()
 */
inline bool HasEvent(Uint32 type) { return SDL_HasEvent(type); }

/**
 * Check for the existence of certain event types in the event queue.
 *
 * If you need to check for a single event type, use SDL_HasEvent() instead.
 *
 * @param minType the low end of event type to be queried, inclusive; see
 *                SDL_EventType for details.
 * @param maxType the high end of event type to be queried, inclusive; see
 *                SDL_EventType for details.
 * @returns true if events with type >= `minType` and <= `maxType` are
 *          present, or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasEvent()
 */
inline bool HasEvents(Uint32 minType = EVENT_FIRST, Uint32 maxType = EVENT_LAST)
{
  return SDL_HasEvents(minType, maxType);
}

/**
 * Clear events of a specific type from the event queue.
 *
 * This will unconditionally remove any events from the queue that match
 * `type`. If you need to remove a range of event types, use SDL_FlushEvents()
 * instead.
 *
 * It's also normal to just ignore events you don't care about in your event
 * loop without calling this function.
 *
 * This function only affects currently queued events. If you want to make
 * sure that all pending OS events are flushed, you can call SDL_PumpEvents()
 * on the main thread immediately before the flush call.
 *
 * If you have user events with custom data that needs to be freed, you should
 * use SDL_PeepEvents() to remove and clean up those events before calling
 * this function.
 *
 * @param type the type of event to be cleared; see SDL_EventType for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FlushEvents()
 */
inline void FlushEvent(Uint32 type) { return SDL_FlushEvent(type); }

/**
 * Clear events of a range of types from the event queue.
 *
 * This will unconditionally remove any events from the queue that are in the
 * range of `minType` to `maxType`, inclusive. If you need to remove a single
 * event type, use SDL_FlushEvent() instead.
 *
 * It's also normal to just ignore events you don't care about in your event
 * loop without calling this function.
 *
 * This function only affects currently queued events. If you want to make
 * sure that all pending OS events are flushed, you can call SDL_PumpEvents()
 * on the main thread immediately before the flush call.
 *
 * @param minType the low end of event type to be cleared, inclusive; see
 *                SDL_EventType for details.
 * @param maxType the high end of event type to be cleared, inclusive; see
 *                SDL_EventType for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FlushEvent()
 */
inline void FlushEvents(Uint32 minType = EVENT_FIRST,
                        Uint32 maxType = EVENT_LAST)
{
  return SDL_FlushEvents(minType, maxType);
}

/**
 * Poll for currently pending events.
 *
 * If `event` is not nullptr, the next event is removed from the queue and
 * stored in the Event structure pointed to by `event`. The true returned
 * refers to this event, immediately stored in the SDL Event structure -- not an
 * event to follow.
 *
 * If `event` is nullptr, it simply returns true if there is an event in the
 * queue, but will not remove it from the queue.
 *
 * As this function may implicitly call PumpEvents(), you can only call
 * this function in the thread that set the video mode.
 *
 * PollEvent() is the favored way of receiving system events since it can
 * be done from the main loop and does not suspend the main loop while waiting
 * on an event to be posted.
 *
 * The common practice is to fully process the event queue once every frame,
 * usually as a first step before updating the game's state:
 *
 * ```c
 * while (game_is_still_running) {
 *     SDL::Event event;
 *     while (SDL::PollEvent(&event)) {  // poll until all events are handled!
 *         // decide what to do with this event.
 *     }
 *
 *     // update game state, draw the current frame
 * }
 * ```
 *
 * @param event the SDL_Event structure to be filled with the next event from
 *              the queue, or nullptr.
 * @returns true if this got an event or false if there are none available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PushEvent()
 * @sa WaitEvent()
 * @sa WaitEventTimeout()
 */
inline bool PollEvent(Event* event) { return SDL_PollEvent(event); }

/**
 * Poll for currently pending events.
 *
 * The next event is removed from the queue and returned.
 *
 * As this function may implicitly call PumpEvents(), you can only call
 * this function in the thread that set the video mode.
 *
 * PollEvent() is the favored way of receiving system events since it can
 * be done from the main loop and does not suspend the main loop while waiting
 * on an event to be posted.
 *
 * The common practice is to fully process the event queue once every frame,
 * usually as a first step before updating the game's state:
 *
 * ```c
 * while (game_is_still_running) {
 *     while (auto event = SDL::PollEvent()) {  // poll until all events are
 * handled!
 *         // decide what to do with this event.
 *     }
 *
 *     // update game state, draw the current frame
 * }
 * ```
 *
 * @returns Event if this got an event or std::nullopt if there are none
 * available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PushEvent()
 * @sa WaitEvent()
 * @sa WaitEventTimeout()
 */
inline std::optional<Event> PollEvent()
{
  if (Event event; PollEvent(&event)) return event;
  return std::nullopt;
}

/**
 * Wait indefinitely for the next available event.
 *
 * If `event` is not nullptr, the next event is removed from the queue and
 * stored in the Event structure pointed to by `event`.
 *
 * As this function may implicitly call PumpEvents(), you can only call
 * this function in the thread that initialized the video subsystem.
 *
 * @param event the Event structure to be filled in with the next event
 *              from the queue, or nullptr.
 * @returns true on success or false if there was an error while waiting for
 *          events; call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEventTimeout()
 */
inline bool WaitEvent(Event* event) { return SDL_WaitEvent(event); }

/**
 * Wait indefinitely for the next available event.
 *
 * The next event is removed from the queue and returned.
 *
 * As this function may implicitly call PumpEvents(), you can only call
 * this function in the thread that initialized the video subsystem.
 *
 * @returns Event on success or std::nullopt if there was an error while waiting
 * for events; call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEventTimeout()
 */
inline std::optional<Event> WaitEvent()
{
  if (Event event; WaitEvent(&event)) return event;
  return std::nullopt;
}

/**
 * Wait until the specified timeout (in milliseconds) for the next available
 * event.
 *
 * If `event` is not nullptr, the next event is removed from the queue and
 * stored in the Event structure pointed to by `event`.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * The timeout is not guaranteed, the actual wait time could be longer due to
 * system scheduling.
 *
 * @param event the Event structure to be filled in with the next event from the
 * queue, or nullptr.
 * @param timeoutMS the maximum number of milliseconds to wait for the next
 *                  available event.
 * @returns true if this got an event or false if the timeout elapsed without
 *          any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEvent()
 */
inline bool WaitEventTimeout(Event* event, Sint32 timeoutMS)
{
  return SDL_WaitEventTimeout(event, timeoutMS);
}

/**
 * Wait until the specified timeout (in milliseconds) for the next available
 * event.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * The timeout is not guaranteed, the actual wait time could be longer due to
 * system scheduling.
 *
 * @param timeoutMS the maximum number of milliseconds to wait for the next
 *                  available event.
 * @returns the Event if this got an event or std::nullopt if the timeout
 * elapsed without any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEvent()
 */
inline std::optional<Event> WaitEventTimeout(Sint32 timeoutMS)
{
  if (Event event; WaitEventTimeout(&event, timeoutMS)) return event;
  return std::nullopt;
}

/**
 * Wait until the specified timeout (with milliseconds precision) for the next
 * available event.
 *
 * If `event` is not nullptr, the next event is removed from the queue and
 * stored in the Event structure pointed to by `event`.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * The timeout is not guaranteed, the actual wait time could be longer due to
 * system scheduling.
 *
 * @param event the Event structure to be filled in with the next event from the
 * queue, or nullptr.
 * @param timeoutDuration the duration to wait for the next available event,
 * with millisecond precision
 * @returns true if this got an event or false if the timeout elapsed without
 *          any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEvent()
 */
inline bool WaitEventTimeout(Event* event,
                             std::chrono::milliseconds timeoutDuration)
{
  return WaitEventTimeout(event, std::max(timeoutDuration.count(), 1l));
}
/**
 * Wait until the specified timeout (with milliseconds precision) for the next
 * available event.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * The timeout is not guaranteed, the actual wait time could be longer due to
 * system scheduling.
 *
 * @param timeoutDuration the duration to wait for the next available event,
 * with millisecond precision.
 * @returns the Event if this got an event or std::nullopt if the timeout
 * elapsed without any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEvent()
 */
inline std::optional<Event> WaitEventTimeout(
  std::chrono::milliseconds timeoutDuration)
{
  if (Event event; WaitEventTimeout(&event, timeoutDuration)) return event;
  return std::nullopt;
}
/**
 * Add an event to the event queue.
 *
 * The event queue can actually be used as a two way communication channel.
 * Not only can events be read from the queue, but the user can also push
 * their own events onto it. `event` is a pointer to the event structure you
 * wish to push onto the queue. The event is copied into the queue, and the
 * caller may dispose of the memory pointed to after SDL_PushEvent() returns.
 *
 * Note: Pushing device input events onto the queue doesn't modify the state
 * of the device within SDL.
 *
 * Note: Events pushed onto the queue with SDL_PushEvent() get passed through
 * the event filter but events added with SDL_PeepEvents() do not.
 *
 * For pushing application-specific events, please use SDL_RegisterEvents() to
 * get an event type that does not conflict with other code that also wants
 * its own custom event types.
 *
 * @param event the SDL_Event to be added to the queue.
 * @returns true on success, false if the event was filtered or on failure;
 *          call GetError() for more information. A common reason for
 *          error is the event queue being full.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PeepEvents()
 * @sa PollEvent()
 * @sa RegisterEvents()
 */
inline bool PushEvent(Event* event) { return SDL_PushEvent(event); }

inline bool PushEvent(const Event& event)
{
  return PushEvent(const_cast<Event*>(&event));
}

/**
 * A function pointer used for callbacks that watch the event queue.
 *
 * @param userdata what was passed as `userdata` to SetEventFilter() or
 *                 AddEventWatch(), etc.
 * @param event the event that triggered the callback.
 * @returns true to permit event to be added to the queue, and false to
 *          disallow it. When used with AddEventWatch(), the return value is
 *          ignored.
 *
 * @threadsafety SDL may call this callback at any time from any thread; the
 *               application is responsible for locking resources the callback
 *               touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SetEventFilter()
 * @sa AddEventWatch()
 */
using EventFilter = SDL_EventFilter;

/**
 * Set up a filter to process all events before they are added to the internal
 * event queue.
 *
 * If you just want to see events without modifying them or preventing them
 * from being queued, you should use SDL_AddEventWatch() instead.
 *
 * If the filter function returns true when called, then the event will be
 * added to the internal queue. If it returns false, then the event will be
 * dropped from the queue, but the internal state will still be updated. This
 * allows selective filtering of dynamically arriving events.
 *
 * **WARNING**: Be very careful of what you do in the event filter function,
 * as it may run in a different thread!
 *
 * On platforms that support it, if the quit event is generated by an
 * interrupt signal (e.g. pressing Ctrl-C), it will be delivered to the
 * application at the next event poll.
 *
 * Note: Disabled events never make it to the event filter function; see
 * SDL_SetEventEnabled().
 *
 * Note: Events pushed onto the queue with SDL_PushEvent() get passed through
 * the event filter, but events pushed onto the queue with SDL_PeepEvents() do
 * not.
 *
 * @param filter an SDL_EventFilter function to call when an event happens.
 * @param userdata a pointer that is passed to `filter`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddEventWatch()
 * @sa SetEventEnabled()
 * @sa GetEventFilter()
 * @sa PeepEvents()
 * @sa PushEvent()
 */
inline void SetEventFilter(EventFilter filter, void* userdata)
{
  return SDL_SetEventFilter(filter, userdata);
}

/**
 * Query the current event filter.
 *
 * This function can be used to "chain" filters, by saving the existing filter
 * before replacing it with a function that will call that saved filter.
 *
 * @param filter the current callback function will be stored here.
 * @param userdata the pointer that is passed to the current event filter will
 *                 be stored here.
 * @returns true on success or false if there is no event filter set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetEventFilter()
 */
inline bool GetEventFilter(EventFilter* filter, void** userdata)
{
  return SDL_GetEventFilter(filter, userdata);
}

/**
 * Add a callback to be triggered when an event is added to the event queue.
 *
 * `filter` will be called when an event happens, and its return value is
 * ignored.
 *
 * **WARNING**: Be very careful of what you do in the event filter function,
 * as it may run in a different thread!
 *
 * If the quit event is generated by a signal (e.g. SIGINT), it will bypass
 * the internal queue and be delivered to the watch callback immediately, and
 * arrive at the next event poll.
 *
 * Note: the callback is called for events posted by the user through
 * SDL_PushEvent(), but not for disabled events, nor for events by a filter
 * callback set with SDL_SetEventFilter(), nor for events posted by the user
 * through SDL_PeepEvents().
 *
 * @param filter an SDL_EventFilter function to call when an event happens.
 * @param userdata a pointer that is passed to `filter`.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RemoveEventWatch()
 * @sa SetEventFilter()
 */
inline bool AddEventWatch(EventFilter filter, void* userdata)
{
  return SDL_AddEventWatch(filter, userdata);
}

/**
 * Remove an event watch callback added with SDL_AddEventWatch().
 *
 * This function takes the same input as SDL_AddEventWatch() to identify and
 * delete the corresponding callback.
 *
 * @param filter the function originally passed to SDL_AddEventWatch().
 * @param userdata the pointer originally passed to SDL_AddEventWatch().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddEventWatch()
 */
inline void RemoveEventWatch(EventFilter filter, void* userdata)
{
  return SDL_RemoveEventWatch(filter, userdata);
}

/**
 * Run a specific filter function on the current event queue, removing any
 * events for which the filter returns false.
 *
 * See SDL_SetEventFilter() for more information. Unlike SDL_SetEventFilter(),
 * this function does not change the filter permanently, it only uses the
 * supplied filter until this function returns.
 *
 * @param filter the SDL_EventFilter function to call when an event happens.
 * @param userdata a pointer that is passed to `filter`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetEventFilter()
 * @sa SetEventFilter()
 */
inline void FilterEvents(EventFilter filter, void* userdata)
{
  return SDL_FilterEvents(filter, userdata);
}

/**
 * Set the state of processing events by type.
 *
 * @param type the type of event; see SDL_EventType for details.
 * @param enabled whether to process the event or not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa EventEnabled()
 */
inline void SetEventEnabled(Uint32 type, bool enabled)
{
  return SDL_SetEventEnabled(type, enabled);
}

/**
 * Query the state of processing events by type.
 *
 * @param type the type of event; see SDL_EventType for details.
 * @returns true if the event is being processed, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetEventEnabled()
 */
inline bool EventEnabled(Uint32 type) { return SDL_EventEnabled(type); }

/**
 * Allocate a set of user-defined events, and return the beginning event
 * number for that set of events.
 *
 * @param numevents the number of events to be allocated.
 * @returns the beginning event number, or 0 if numevents is invalid or if
 *          there are not enough user-defined events left.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PushEvent()
 */
inline Uint32 RegisterEvents(int numevents)
{
  return SDL_RegisterEvents(numevents);
}

/**
 * Get window associated with an event.
 *
 * @param event an event containing a `windowID`.
 * @returns the associated window on success or NULL if there is none.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa WaitEvent()
 * @sa WaitEventTimeout()
 */
inline WindowRef GetWindowFromEvent(const Event* event)
{
  return SDL_GetWindowFromEvent(event);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_EVENTS_H_ */


// end --- SDL3pp_events.h --- 


//
//
//
//
//
//
//

// begin --- SDL3pp_init.h --- 

#ifndef SDL3PP_INIT_H_
#define SDL3PP_INIT_H_

#include <SDL3/SDL_init.h>

/**
 * @namespace SDL
 * @brief the main namespace where all SDL3pp public functions and types live
 */
namespace SDL {

/**
 * @defgroup CategoryInit Initialization and Shutdown
 *
 * All SDL programs need to initialize the library before starting to work
 * with it.
 *
 * Almost everything can simply call SDL_Init() near startup, with a handful
 * of flags to specify subsystems to touch. These are here to make sure SDL
 * does not even attempt to touch low-level pieces of the operating system
 * that you don't intend to use. For example, you might be using SDL for video
 * and input but chose an external library for audio, and in this case you
 * would just need to leave off the `SDL_INIT_AUDIO` flag to make sure that
 * external library has complete control.
 *
 * Most apps, when terminating, should call SDL_Quit(). This will clean up
 * (nearly) everything that SDL might have allocated, and crucially, it'll
 * make sure that the display's resolution is back to what the user expects if
 * you had previously changed it for your game.
 *
 * SDL3 apps are strongly encouraged to call SDL_SetAppMetadata() at startup
 * to fill in details about the program. This is completely optional, but it
 * helps in small ways (we can provide an About dialog box for the macOS menu,
 * we can name the app in the system's audio mixer, etc). Those that want to
 * provide a _lot_ of information should look at the more-detailed
 * SDL_SetAppMetadataProperty().
 *
 * @{
 */

/**
 * @name InitFlags
 *
 * Initialization flags
 */

/**
 * @brief Initialization flags for SDL
 */
using InitFlags = SDL_InitFlags;

/**
 * `SDL_INIT_AUDIO` implies `SDL_INIT_EVENTS`
 */
constexpr inline InitFlags INIT_AUDIO = SDL_INIT_AUDIO;

/**
 * `SDL_INIT_VIDEO` implies `SDL_INIT_EVENTS`, should be initialized on the main
 * thread
 */
constexpr inline InitFlags INIT_VIDEO = SDL_INIT_VIDEO;

/**
 * `SDL_INIT_JOYSTICK` implies `SDL_INIT_EVENTS`, should be initialized on the
 * same thread as SDL_INIT_VIDEO on Windows if you don't set
 * SDL_HINT_JOYSTICK_THREAD
 */
constexpr inline InitFlags INIT_JOYSTICK = SDL_INIT_JOYSTICK;

constexpr inline InitFlags INIT_HAPTIC = SDL_INIT_HAPTIC;

/**
 * `SDL_INIT_GAMEPAD` implies `SDL_INIT_JOYSTICK`
 */
constexpr inline InitFlags INIT_GAMEPAD = SDL_INIT_GAMEPAD;

constexpr inline InitFlags INIT_EVENTS = SDL_INIT_EVENTS;

/**
 * `SDL_INIT_SENSOR` implies `SDL_INIT_EVENTS`
 */
constexpr inline InitFlags INIT_SENSOR = SDL_INIT_SENSOR;

/**
 * `SDL_INIT_CAMERA` implies `SDL_INIT_EVENTS`
 */
constexpr inline InitFlags INIT_CAMERA = SDL_INIT_CAMERA;

/// @}

/**
 * @name AppResult
 * App result for Main callback
 * @{
 */

/**
 * Return values for optional main callbacks.
 *
 * Returning SDL_APP_SUCCESS or SDL_APP_FAILURE from SDL_AppInit,
 * SDL_AppEvent, or SDL_AppIterate will terminate the program and report
 * success/failure to the operating system. What that means is
 * platform-dependent. On Unix, for example, on success, the process error
 * code will be zero, and on failure it will be 1. This interface doesn't
 * allow you to return specific exit codes, just whether there was an error
 * generally or not.
 *
 * Returning SDL_APP_CONTINUE from these functions will let the app continue
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
 * Function pointer typedef for SDL_AppInit.
 *
 * These are used by SDL_EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppInit directly.
 *
 * @param appstate a place where the app can optionally store a pointer for
 *                 future use.
 * @param argc the standard ANSI C main's argc; number of elements in `argv`.
 * @param argv the standard ANSI C main's argv; array of command line
 *             arguments.
 * @returns SDL_APP_FAILURE to terminate with an error, SDL_APP_SUCCESS to
 *          terminate with success, SDL_APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppInit_func = SDL_AppInit_func;

/**
 * Function pointer typedef for SDL_AppIterate.
 *
 * These are used by SDL_EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppIterate directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @returns SDL_APP_FAILURE to terminate with an error, SDL_APP_SUCCESS to
 *          terminate with success, SDL_APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppIterate_func = SDL_AppIterate_func;

/**
 * Function pointer typedef for SDL_AppEvent.
 *
 * These are used by SDL_EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppEvent directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @param event the new event for the app to examine.
 * @returns SDL_APP_FAILURE to terminate with an error, SDL_APP_SUCCESS to
 *          terminate with success, SDL_APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppEvent_func = SDL_AppEvent_func;

/**
 * Function pointer typedef for SDL_AppQuit.
 *
 * These are used by SDL_EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppEvent directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @param result the result code that terminated the app (success or failure).
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppQuit_func = SDL_AppQuit_func;

/**
 * Initialize the SDL library.
 *
 * SDL_Init() simply forwards to calling SDL_InitSubSystem(). Therefore, the
 * two may be used interchangeably. Though for readability of your code
 * SDL_InitSubSystem() might be preferred.
 *
 * The file I/O (for example: SDL_IOFromFile) and threading (SDL_CreateThread)
 * subsystems are initialized by default. Message boxes
 * (SDL_ShowSimpleMessageBox) also attempt to work without initializing the
 * video subsystem, in hopes of being useful in showing an error dialog when
 * SDL_Init fails. You must specifically initialize other subsystems if you
 * use them in your application.
 *
 * Logging (such as SDL_Log) works without initialization, too.
 *
 * `flags` may be any of the following OR'd together:
 *
 * - `SDL_INIT_AUDIO`: audio subsystem; automatically initializes the events
 *   subsystem
 * - `SDL_INIT_VIDEO`: video subsystem; automatically initializes the events
 *   subsystem, should be initialized on the main thread.
 * - `SDL_INIT_JOYSTICK`: joystick subsystem; automatically initializes the
 *   events subsystem
 * - `SDL_INIT_HAPTIC`: haptic (force feedback) subsystem
 * - `SDL_INIT_GAMEPAD`: gamepad subsystem; automatically initializes the
 *   joystick subsystem
 * - `SDL_INIT_EVENTS`: events subsystem
 * - `SDL_INIT_SENSOR`: sensor subsystem; automatically initializes the events
 *   subsystem
 * - `SDL_INIT_CAMERA`: camera subsystem; automatically initializes the events
 *   subsystem
 *
 * Subsystem initialization is ref-counted, you must call SDL_QuitSubSystem()
 * for each SDL_InitSubSystem() to correctly shutdown a subsystem manually (or
 * call SDL_Quit() to force shutdown). If a subsystem is already loaded then
 * this call will increase the ref-count and return.
 *
 * Consider reporting some basic metadata about your application before
 * calling SDL_Init, using either SDL_SetAppMetadata() or
 * SDL_SetAppMetadataProperty().
 *
 * @param flags subsystem initialization flags.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadata()
 * @sa SetAppMetadataProperty()
 * @sa InitSubSystem()
 * @sa Quit()
 * @sa SetMainReady()
 * @sa WasInit()
 */
inline bool Init(InitFlags flags) { return SDL_Init(flags); }

/**
 * Compatibility function to initialize the SDL library.
 *
 * This function and SDL_Init() are interchangeable.
 *
 * @param flags any of the flags used by SDL_Init(); see SDL_Init for details.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init()
 * @sa Quit()
 * @sa QuitSubSystem()
 */
inline bool InitSubSystem(InitFlags flags) { return SDL_InitSubSystem(flags); }

/**
 * Shut down specific SDL subsystems.
 *
 * You still need to call SDL_Quit() even if you close all open subsystems
 * with SDL_QuitSubSystem().
 *
 * @param flags any of the flags used by SDL_Init(); see SDL_Init for details.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa InitSubSystem()
 * @sa Quit()
 */
inline void QuitSubSystem(InitFlags flags) { return SDL_QuitSubSystem(flags); }

/**
 * Get a mask of the specified subsystems which are currently initialized.
 *
 * @param flags any of the flags used by SDL_Init(); see SDL_Init for details.
 * @returns a mask of all initialized subsystems if `flags` is 0, otherwise it
 *          returns the initialization status of the specified subsystems.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init()
 * @sa InitSubSystem()
 */
inline InitFlags WasInit(InitFlags flags) { return SDL_WasInit(flags); }

/**
 * Clean up all initialized subsystems.
 *
 * You should call this function even if you have already shutdown each
 * initialized subsystem with SDL_QuitSubSystem(). It is safe to call this
 * function even in the case of errors in initialization.
 *
 * You can use this function with atexit() to ensure that it is run when your
 * application is shutdown, but it is not wise to do this from a library or
 * other dynamically loaded code.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init()
 * @sa QuitSubSystem()
 */
inline void Quit() { return SDL_Quit(); }

/**
 * @brief Initialize the SDL library.
 *
 * Also init any subsystem passed as InitFlags
 *
 * This might be called multiple times, it keeps a ref count and calls SDL_Quit
 * only on the last one.
 *
 * The SubSystems are out of the refCount, as SDL itself already keep track
 * internally.
 */
struct SDL
{

  /**
   * @brief Init given subsystems
   * @param flags
   *
   * This uses a refCount internally, so it is safe to call
   * this multiple times, the quit will be called only on the last call.
   */
  SDL(InitFlags flags)
    : flags(flags)
  {
    refCount(+1, true);
  }

  // Copy ctor
  SDL(const SDL& other)
    : flags(other.flags)
    , active(other.active)
  {
    if (active) refCount(+1);
  }

  // Move ctor
  SDL(SDL&& other)
    : flags(other.flags)
    , active(other.active)
  {
    other.active = false;
  }

  // Dtor
  ~SDL()
  {
    if (active) refCount(-1);
  }

  SDL& operator=(SDL rhs)
  {
    std::swap(active, rhs.active);
    std::swap(flags, rhs.flags);
    return *this;
  }

  /**
   * @brief Check if given subSystems are initialized
   * @param flags the flags to test or 0 to test all
   * @return Which subsystem are active
   */
  static InitFlags WasInit(InitFlags flags = 0) { return SDL_WasInit(flags); }

  /**
   * @brief release locking such as reset() does, but never calls SDL_Quit() or
   * SDL_QuitSubSystem()
   * @return false if there are still other locks, true if this was last one
   *
   * When this returns true it is safe to call SDL_Quit()
   */
  bool release()
  {
    flags = 0;
    return refCount(-1, false) == 0;
  }

  /**
   * @brief reset the value of this instance, acts like it was destroyed and
   * then newly instantiated
   * @return false if there are still other locks, true if this was last one
   */
  bool reset() { return refCount(-1) == 0; }

  /// @brief returns true if active and has no errors
  operator bool() const { return active; }

  InitFlags GetCurrentFlags() const { return flags; }

private:
  InitFlags flags = 0;
  bool active = true;

  int refCount(int delta, bool autoQuit = true);
};

/**
 * Return whether this is the main thread.
 *
 * On Apple platforms, the main thread is the thread that runs your program's
 * main() entry point. On other platforms, the main thread is the one that
 * calls SDL_Init(SDL_INIT_VIDEO), which should usually be the one that runs
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
 * @sa RunOnMainThread()
 */
inline bool IsMainThread() { return SDL_IsMainThread(); }

/**
 * Callback run on the main thread.
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa RunOnMainThread()
 */
using MainThreadCallback = SDL_MainThreadCallback;

/**
 * @sa PropertiesRef.MainThreadCallback
 */
using MainThreadFunction = std::function<void()>;

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
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IsMainThread()
 */
inline bool RunOnMainThread(MainThreadCallback callback,
                            void* userdata,
                            bool wait_complete)
{
  return SDL_RunOnMainThread(callback, userdata, wait_complete);
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
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IsMainThread()
 */
inline bool RunOnMainThread(MainThreadFunction callback, bool wait_complete)
{
  using Wrapper = CallbackWrapper<void()>;
  return RunOnMainThread(
    &Wrapper::CallOnce, Wrapper::Wrap(std::move(callback)), wait_complete);
}

/**
 * Specify basic metadata about your app.
 *
 * You can optionally provide metadata about your app to SDL. This is not
 * required, but strongly encouraged.
 *
 * There are several locations where SDL can make use of metadata (an "About"
 * box in the macOS menu bar, the name of the app can be shown on some audio
 * mixers, etc). Any piece of metadata can be left as NULL, if a specific
 * detail doesn't make sense for the app.
 *
 * This function should be called as early as possible, before SDL_Init.
 * Multiple calls to this function are allowed, but various state might not
 * change once it has been set up with a previous call to this function.
 *
 * Passing a NULL removes any previous metadata.
 *
 * This is a simplified interface for the most important information. You can
 * supply significantly more detailed metadata with
 * SDL_SetAppMetadataProperty().
 *
 * @param appname The name of the application ("My Game 2: Bad Guy's
 *                Revenge!").
 * @param appversion The version of the application ("1.0.0beta5" or a git
 *                   hash, or whatever makes sense).
 * @param appidentifier A unique string in reverse-domain format that
 *                      identifies this app ("com.example.mygame2").
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadataProperty()
 */
inline bool SetAppMetadata(StringParam appname,
                           StringParam appversion,
                           StringParam appidentifier)
{
  return SDL_SetAppMetadata(appname, appversion, appidentifier);
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
 * This function should be called as early as possible, before SDL_Init.
 * Multiple calls to this function are allowed, but various state might not
 * change once it has been set up with a previous call to this function.
 *
 * Once set, this metadata can be read using SDL_GetAppMetadataProperty().
 *
 * These are the supported properties:
 *
 * - `SDL_PROP_APP_METADATA_NAME_STRING`: The human-readable name of the
 *   application, like "My Game 2: Bad Guy's Revenge!". This will show up
 *   anywhere the OS shows the name of the application separately from window
 *   titles, such as volume control applets, etc. This defaults to "SDL
 *   Application".
 * - `SDL_PROP_APP_METADATA_VERSION_STRING`: The version of the app that is
 *   running; there are no rules on format, so "1.0.3beta2" and "April 22nd,
 *   2024" and a git hash are all valid options. This has no default.
 * - `SDL_PROP_APP_METADATA_IDENTIFIER_STRING`: A unique string that
 *   identifies this app. This must be in reverse-domain format, like
 *   "com.example.mygame2". This string is used by desktop compositors to
 *   identify and group windows together, as well as match applications with
 *   associated desktop settings and icons. If you plan to package your
 *   application in a container such as Flatpak, the app ID should match the
 *   name of your Flatpak container as well. This has no default.
 * - `SDL_PROP_APP_METADATA_CREATOR_STRING`: The human-readable name of the
 *   creator/developer/maker of this app, like "MojoWorkshop, LLC"
 * - `SDL_PROP_APP_METADATA_COPYRIGHT_STRING`: The human-readable copyright
 *   notice, like "Copyright (c) 2024 MojoWorkshop, LLC" or whatnot. Keep this
 *   to one line, don't paste a copy of a whole software license in here. This
 *   has no default.
 * - `SDL_PROP_APP_METADATA_URL_STRING`: A URL to the app on the web. Maybe a
 *   product page, or a storefront, or even a GitHub repository, for user's
 *   further information This has no default.
 * - `SDL_PROP_APP_METADATA_TYPE_STRING`: The type of application this is.
 *   Currently this string can be "game" for a video game, "mediaplayer" for a
 *   media player, or generically "application" if nothing else applies.
 *   Future versions of SDL might add new types. This defaults to
 *   "application".
 *
 * @param name the name of the metadata property to set.
 * @param value the value of the property, or NULL to remove that property.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAppMetadataProperty()
 * @sa SetAppMetadata()
 */
inline bool SetAppMetadataProperty(StringParam name, StringParam value)
{
  return SDL_SetAppMetadataProperty(name, value);
}

/**
 * Get metadata about your app.
 *
 * This returns metadata previously set using SDL_SetAppMetadata() or
 * SDL_SetAppMetadataProperty(). See SDL_SetAppMetadataProperty() for the list
 * of available properties and their meanings.
 *
 * @param name the name of the metadata property to get.
 * @returns the current value of the metadata property, or the default if it
 *          is not set, NULL for properties with no default.
 *
 * @threadsafety It is safe to call this function from any thread, although
 *               the string returned is not protected and could potentially be
 *               freed if you call SDL_SetAppMetadataProperty() to set that
 *               property from another thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadata()
 * @sa SetAppMetadataProperty()
 */
inline const char* GetAppMetadataProperty(StringParam name)
{
  return SDL_GetAppMetadataProperty(name);
}

/** @} */

#pragma region impl

inline int SDL::refCount(int delta, bool autoQuit)
{
  // TODO Locking these?
  static int refCount = 0;
  if (delta && active) {
    if (delta > 0) {
      refCount += 1;
      if (flags) active = SDL_InitSubSystem(flags);
    } else {
      SDL_assert_always(refCount > 0);
      active = false;
      refCount -= 1;

      if (autoQuit) {
        if (refCount <= 0) {
          // TODO Make this under FLAG
          SDL_Quit();
        } else if (flags) {
          SDL_QuitSubSystem(flags);
        }
      }
    }
  }
  return refCount;
}

#pragma endregion

} // namespace SDL

#endif /* SDL3PP_INIT_H_ */


// end --- SDL3pp_init.h --- 


//
//
//
//
//
//
//
//
//
//

// begin --- SDL3pp_misc.h --- 


#ifndef SDL3PP_MISC_H_
#define SDL3PP_MISC_H_

#include <SDL3/SDL_misc.h>

namespace SDL {

/**
 * @defgroup CategoryMisc Miscellaneous
 *
 * SDL API functions that don't fit elsewhere.
 *
 * @{
 */

/**
 * Open a URL/URI in the browser or other appropriate external application.
 *
 * Open a URL in a separate, system-provided application. How this works will
 * vary wildly depending on the platform. This will likely launch what makes
 * sense to handle a specific URL's protocol (a web browser for `http://`,
 * etc), but it might also be able to launch file managers for directories and
 * other things.
 *
 * What happens when you open a URL varies wildly as well: your game window
 * may lose focus (and may or may not lose focus if your game was fullscreen
 * or grabbing input at the time). On mobile devices, your app will likely
 * move to the background or your process might be paused. Any given platform
 * may or may not handle a given URL.
 *
 * If this is unimplemented (or simply unavailable) for a platform, this will
 * fail with an error. A successful result does not mean the URL loaded, just
 * that we launched _something_ to handle it (or at least believe we did).
 *
 * All this to say: this function can be useful, but you should definitely
 * test it on every platform you target.
 *
 * @param url a valid URL/URI to open. Use `file:///full/path/to/file` for
 *            local files, if supported.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool OpenURL(StringParam url) { return SDL_OpenURL(url); }

/// @}

} // namespace SDL

#endif /* SDL3PP_MISC_H_ */


// end --- SDL3pp_misc.h --- 


//
//
//
//
//
//

// begin --- SDL3pp_render.h --- 

#ifndef SDL3PP_RENDER_H_
#define SDL3PP_RENDER_H_

#include <SDL3/SDL_render.h>

namespace SDL {

/**
 * @defgroup CategoryRender 2D Accelerated Rendering
 *
 * Header file for SDL 2D rendering functions.
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
 *
 * @{
 */

/**  Deleter */
template<>
struct ObjectDeleter<SDL_Renderer>
{
  void operator()(RendererRef renderer) const;
};

/// @brief Handle to an owned renderer
using Renderer = RendererBase<ObjectUnique<SDL_Renderer>>;

// Forward decl
template<ObjectBox<SDL_Texture*> T>
struct TextureBase;

using TextureRef = TextureBase<ObjectRef<SDL_Texture>>;

template<>
struct ObjectDeleter<SDL_Texture>
{
  void operator()(TextureRef texture) const;
};

using Texture = TextureBase<ObjectUnique<SDL_Texture>>;

// Forward decl
struct TextureLock;

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
 * Changes rarely, not lockable
 */
constexpr TextureAccess TEXTUREACCESS_STATIC = SDL_TEXTUREACCESS_STATIC;

/**
 * Changes frequently, lockable
 */
constexpr TextureAccess TEXTUREACCESS_STREAMING = SDL_TEXTUREACCESS_STREAMING;

/**
 * Texture can be used as a render target
 */
constexpr TextureAccess TEXTUREACCESS_TARGET = SDL_TEXTUREACCESS_TARGET;

/**
 * @brief How the logical size is mapped to the output.
 *
 */
using RendererLogicalPresentation = SDL_RendererLogicalPresentation;

/**
 * There is no logical size in effect
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_DISABLED =
  SDL_LOGICAL_PRESENTATION_DISABLED;

/**
 * The rendered content is stretched to the output resolution
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_STRETCH =
  SDL_LOGICAL_PRESENTATION_STRETCH;

/**
 * The rendered content is fit to the largest dimension and the other dimension
 * is letterboxed with black bars
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_LETTERBOX =
  SDL_LOGICAL_PRESENTATION_LETTERBOX;

/**
 * The rendered content is fit to the smallest dimension and the other dimension
 * extends beyond the output bounds
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_OVERSCAN =
  SDL_LOGICAL_PRESENTATION_OVERSCAN;

/**
 * The rendered content is scaled up by integer multiples to fit the output
 * resolution
 */
constexpr RendererLogicalPresentation LOGICAL_PRESENTATION_INTEGER_SCALE =
  SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;

/**
 * @brief A structure representing rendering state
 *
 */
template<ObjectBox<SDL_Renderer*> T>
struct RendererBase : T
{
  using T::T;

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
   * @ingroup resource
   */
  RendererBase(WindowRef window)
    : T(SDL_CreateRenderer(window.get(), nullptr))
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
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetNumRenderDrivers()
   * @sa GetRenderDriver()
   * @sa GetRendererName()
   */
  RendererBase(WindowRef window, StringParam name)
    : T(SDL_CreateRenderer(window.get(), name))
  {
  }

  /**
   * Create a 2D rendering context for a window, with the specified properties.
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
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties
   * @sa GetName()
   */
  RendererBase(PropertiesRef props)
    : T(SDL_CreateRendererWithProperties(props.get()))
  {
  }

  /**
   * Create a 2D software rendering context for a surface.
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
   *
   * @since This function is available since SDL 3.2.0.
   */
  RendererBase(SurfaceRef surface)
    : T(SDL_CreateSoftwareRenderer(surface.get()))
  {
  }

  /**
   * Get the window associated with a renderer.
   *
   * @returns the window on success or NULL on failure; call SDL_GetError() for
   *          more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  WindowRef GetWindow() { return {SDL_GetRenderWindow(T::get())}; }

  /**
   * Get the name of a renderer.
   *
   * @returns the name of the selected renderer, or NULL on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   */
  const char* GetName() const { return SDL_GetRendererName(T::get()); }

  std::optional<Point> GetOutputSize() const
  {
    if (Point size; GetOutputSize(&size)) return size;
    return std::nullopt;
  }

  bool GetOutputSize(SDL_Point* size) const
  {
    if (!size) return GetOutputSize(nullptr, nullptr);
    return GetOutputSize(&size->x, &size->y);
  }

  /**
   * Get the output size in pixels of a rendering context.
   *
   * This returns the true output size in pixels, ignoring any render targets or
   * logical size and presentation.
   *
   * @param w a pointer filled in with the width in pixels.
   * @param h a pointer filled in with the height in pixels.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetCurrentOutputSize()
   */
  bool GetOutputSize(int* w, int* h) const
  {
    return SDL_GetRenderOutputSize(T::get(), w, h);
  }

  std::optional<Point> GetCurrentOutputSize() const
  {
    if (Point size; GetCurrentOutputSize(&size)) return size;
    return std::nullopt;
  }

  bool GetCurrentOutputSize(SDL_Point* size) const
  {
    if (!size) return GetCurrentOutputSize(nullptr, nullptr);
    return GetCurrentOutputSize(&size->x, &size->y);
  }

  /**
   * Get the current output size in pixels of a rendering context.
   *
   * If a rendering target is active, this will return the size of the rendering
   * target in pixels, otherwise if a logical size is set, it will return the
   * logical size, otherwise it will return the value of
   * SDL_GetRenderOutputSize().
   *
   * @param w a pointer filled in with the current width.
   * @param h a pointer filled in with the current height.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetOutputSize()
   */
  bool GetCurrentOutputSize(int* w, int* h) const
  {
    return SDL_GetCurrentRenderOutputSize(T::get(), w, h);
  }

  /**
   * Get the properties associated with a renderer.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `SDL_PROP_RENDERER_NAME_STRING`: the name of the rendering driver
   * - `SDL_PROP_RENDERER_WINDOW_POINTER`: the window where rendering is
   *   displayed, if any
   * - `SDL_PROP_RENDERER_SURFACE_POINTER`: the surface where rendering is
   *   displayed, if this is a software renderer without a window
   * - `SDL_PROP_RENDERER_VSYNC_NUMBER`: the current vsync setting
   * - `SDL_PROP_RENDERER_MAX_TEXTURE_SIZE_NUMBER`: the maximum texture width
   *   and height
   * - `SDL_PROP_RENDERER_TEXTURE_FORMATS_POINTER`: a (const SDL_PixelFormat *)
   *   array of pixel formats, terminated with SDL_PIXELFORMAT_UNKNOWN,
   *   representing the available texture formats for this renderer.
   * - `SDL_PROP_RENDERER_OUTPUT_COLORSPACE_NUMBER`: an SDL_Colorspace value
   *   describing the colorspace for output to the display, defaults to
   *   SDL_COLORSPACE_SRGB.
   * - `SDL_PROP_RENDERER_HDR_ENABLED_BOOLEAN`: true if the output colorspace is
   *   SDL_COLORSPACE_SRGB_LINEAR and the renderer is showing on a display with
   *   HDR enabled. This property can change dynamically when
   *   SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `SDL_PROP_RENDERER_SDR_WHITE_POINT_FLOAT`: the value of SDR white in the
   *   SDL_COLORSPACE_SRGB_LINEAR colorspace. When HDR is enabled, this value is
   *   automatically multiplied into the color scale. This property can change
   *   dynamically when SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   * - `SDL_PROP_RENDERER_HDR_HEADROOM_FLOAT`: the additional high dynamic range
   *   that can be displayed, in terms of the SDR white point. When HDR is not
   *   enabled, this will be 1.0. This property can change dynamically when
   *   SDL_EVENT_WINDOW_HDR_STATE_CHANGED is sent.
   *
   * With the direct3d renderer:
   *
   * - `SDL_PROP_RENDERER_D3D9_DEVICE_POINTER`: the IDirect3DDevice9 associated
   *   with the renderer
   *
   * With the direct3d11 renderer:
   *
   * - `SDL_PROP_RENDERER_D3D11_DEVICE_POINTER`: the ID3D11Device associated
   *   with the renderer
   * - `SDL_PROP_RENDERER_D3D11_SWAPCHAIN_POINTER`: the IDXGISwapChain1
   *   associated with the renderer. This may change when the window is resized.
   *
   * With the direct3d12 renderer:
   *
   * - `SDL_PROP_RENDERER_D3D12_DEVICE_POINTER`: the ID3D12Device associated
   *   with the renderer
   * - `SDL_PROP_RENDERER_D3D12_SWAPCHAIN_POINTER`: the IDXGISwapChain4
   *   associated with the renderer.
   * - `SDL_PROP_RENDERER_D3D12_COMMAND_QUEUE_POINTER`: the ID3D12CommandQueue
   *   associated with the renderer
   *
   * With the vulkan renderer:
   *
   * - `SDL_PROP_RENDERER_VULKAN_INSTANCE_POINTER`: the VkInstance associated
   *   with the renderer
   * - `SDL_PROP_RENDERER_VULKAN_SURFACE_NUMBER`: the VkSurfaceKHR associated
   *   with the renderer
   * - `SDL_PROP_RENDERER_VULKAN_PHYSICAL_DEVICE_POINTER`: the VkPhysicalDevice
   *   associated with the renderer
   * - `SDL_PROP_RENDERER_VULKAN_DEVICE_POINTER`: the VkDevice associated with
   *   the renderer
   * - `SDL_PROP_RENDERER_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER`: the queue
   *   family index used for rendering
   * - `SDL_PROP_RENDERER_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER`: the queue
   *   family index used for presentation
   * - `SDL_PROP_RENDERER_VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER`: the number of
   *   swapchain images, or potential frames in flight, used by the Vulkan
   *   renderer
   *
   * With the gpu renderer:
   *
   * - `SDL_PROP_RENDERER_GPU_DEVICE_POINTER`: the SDL_GPUDevice associated with
   *   the renderer
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return PropertiesRef{SDL_GetRendererProperties(T::get())};
  }

  bool ResetTarget() { return SetTarget({}); }

  /**
   * Set a texture as the current rendering target.
   *
   * The default render target is the window for which the renderer was created.
   * To stop rendering to a texture and render to the window again, call this
   * function with a NULL `texture`.
   *
   * @param texture the targeted texture, which must be created with the
   *                `SDL_TEXTUREACCESS_TARGET` flag, or NULL to render to the
   *                window instead of a texture.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetTarget()
   */
  bool SetTarget(TextureRef texture);

  /**
   * Get the current render target.
   *
   * The default render target is the window for which the renderer was created,
   * and is reported a NULL here.
   *
   * @returns the current render target or NULL for the default render target.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetTarget()
   */
  TextureRef GetTarget() const;

  /**
   * Set a device independent resolution and presentation mode for rendering.
   *
   * This function sets the width and height of the logical rendering output.
   * The renderer will act as if the window is always the requested dimensions,
   * scaling to the actual window resolution as necessary.
   *
   * This can be useful for games that expect a fixed size, but would like to
   * scale the output to whatever is available, regardless of how a user resizes
   * a window, or if the display is high DPI.
   *
   * You can disable logical coordinates by setting the mode to
   * SDL_LOGICAL_PRESENTATION_DISABLED, and in that case you get the full pixel
   * resolution of the output window; it is safe to toggle logical presentation
   * during the rendering of a frame: perhaps most of the rendering is done to
   * specific dimensions but to make fonts look sharp, the app turns off logical
   * presentation while drawing text.
   *
   * Letterboxing will only happen if logical presentation is enabled during
   * SDL_RenderPresent; be sure to reenable it first if you were using it.
   *
   * You can convert coordinates in an event into rendering coordinates using
   * SDL_ConvertEventToRenderCoordinates().
   *
   * @param size the width and height of the logical resolution.
   * @param mode the presentation mode used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ConvertEventToRenderCoordinates()
   * @sa GetLogicalPresentation()
   * @sa GetLogicalPresentationRect()
   */
  bool SetLogicalPresentation(SDL_Point size, RendererLogicalPresentation mode)
  {
    return SDL_SetRenderLogicalPresentation(T::get(), size.x, size.y, mode);
  }

  /**
   * Get device independent resolution and presentation mode for rendering.
   *
   * This function gets the width and height of the logical rendering output, or
   * the output size in pixels if a logical resolution is not enabled.
   *
   * @param size a Point to be filled with the width and height.
   * @param mode the presentation mode used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   */
  bool GetLogicalPresentation(SDL_Point* size,
                              RendererLogicalPresentation* mode)
  {
    if (!size) return GetLogicalPresentation(nullptr, nullptr, mode);
    return GetLogicalPresentation(&size->x, &size->y, mode);
  }

  /**
   * Get device independent resolution and presentation mode for rendering.
   *
   * This function gets the width and height of the logical rendering output, or
   * the output size in pixels if a logical resolution is not enabled.
   *
   * @param w an int to be filled with the width.
   * @param h an int to be filled with the height.
   * @param mode the presentation mode used.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   */
  bool GetLogicalPresentation(int* w,
                              int* h,
                              RendererLogicalPresentation* mode) const
  {
    return SDL_GetRenderLogicalPresentation(T::get(), w, h, mode);
  }

  /**
   * Get the final presentation rectangle for rendering.
   *
   * This function returns the calculated rectangle used for logical
   * presentation, based on the presentation mode and output size. If logical
   * presentation is disabled, it will fill the rectangle with the output size,
   * in pixels.
   *
   * @returns the rect with the final presentation rectangle on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   */
  std::optional<FRect> GetLogicalPresentationRect() const
  {
    if (FRect rect; SDL_GetRenderLogicalPresentationRect(T::get(), &rect)) {
      return rect;
    }
    return std::nullopt;
  }

  /**
   * Get a point in render coordinates when given a point in window coordinates.
   *
   * This takes into account several states:
   *
   * - The window dimensions.
   * - The logical presentation settings (SDL_SetRenderLogicalPresentation)
   * - The scale (SDL_SetRenderScale)
   * - The viewport (SDL_SetRenderViewport)
   *
   * @param window_coord the x, y coordinate in window coordinates.
   * @returns a FPoint containing ther render coordinates on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   * @sa SetScale()
   */
  std::optional<FPoint> RenderCoordinatesFromWindow(
    const SDL_FPoint& window_coord) const
  {
    return SDL_RenderCoordinatesFromWindow(T::get(), window_coord);
  }

  /**
   * Get a point in window coordinates when given a point in render coordinates.
   *
   * This takes into account several states:
   *
   * - The window dimensions.
   * - The logical presentation settings (SDL_SetRenderLogicalPresentation)
   * - The scale (SDL_SetRenderScale)
   * - The viewport (SDL_SetRenderViewport)
   *
   * @param coord the x, y coordinate in render coordinates.
   * @returns a FPoint filled with window coordinates on success or std::nullopt
   * on failure; call SDL_GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogicalPresentation()
   * @sa SetScale()
   * @sa SetViewport()
   */
  std::optional<FPoint> RenderCoordinatesToWindow(const SDL_FPoint& coord) const
  {
    return SDL_RenderCoordinatesToWindow(T::get(), coord);
  }

  /**
   * Convert the coordinates in an event to render coordinates.
   *
   * This takes into account several states:
   *
   * - The window dimensions.
   * - The logical presentation settings (SDL_SetRenderLogicalPresentation)
   * - The scale (SDL_SetRenderScale)
   * - The viewport (SDL_SetRenderViewport)
   *
   * Various event types are converted with this function: mouse, touch, pen,
   * etc.
   *
   * Touch coordinates are converted from normalized coordinates in the window
   * to non-normalized rendering coordinates.
   *
   * Relative mouse coordinates (xrel and yrel event fields) are _also_
   * converted. Applications that do not want these fields converted should use
   * SDL_RenderCoordinatesFromWindow() on the specific event fields instead of
   * converting the entire event structure.
   *
   * Once converted, coordinates may be outside the rendering area.
   *
   * @param event the event to modify.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderCoordinatesFromWindow
   */
  bool ConvertEventToRenderCoordinates(SDL_Event* event) const
  {
    return SDL_ConvertEventToRenderCoordinates(T::get(), event);
  }

  bool ResetViewport() { return SetViewport({}); }

  /**
   * Set the drawing area for rendering on the current target.
   *
   * Drawing will clip to this area (separately from any clipping done with
   * SDL_SetRenderClipRect), and the top left of the area will become coordinate
   * (0, 0) for future drawing commands.
   *
   * The area's width and height must be >= 0.
   *
   * Each render target has its own viewport. This function sets the viewport
   * for the current render target.
   *
   * @param rect the SDL_Rect structure representing the drawing area, or
   *             std::nullopt to set the viewport to the entire target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetViewport()
   * @sa ViewportSet()
   */
  bool SetViewport(OptionalRef<const SDL_Rect> rect)
  {
    return SDL_SetRenderViewport(T::get(), rect);
  }

  /**
   * Get the drawing area for the current target.
   *
   * Each render target has its own viewport. This function gets the viewport
   * for the current render target.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IsViewportSet()
   * @sa SetViewport()
   */
  std::optional<Rect> GetViewport() const
  {
    if (Rect rect; SDL_GetRenderViewport(T::get(), &rect)) return rect;
    return {};
  }

  /**
   * Return whether an explicit rectangle was set as the viewport.
   *
   * This is useful if you're saving and restoring the viewport and want to know
   * whether you should restore a specific rectangle or NULL. Note that the
   * viewport is always reset when changing rendering targets.
   *
   * Each render target has its own viewport. This function checks the viewport
   * for the current render target.
   *
   * @returns true if the viewport was set to a specific rectangle, or false if
   *          it was set to NULL (the entire target).
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetViewport()
   * @sa SetViewport()
   */
  bool IsViewportSet() const { return SDL_RenderViewportSet(T::get()); }

  /**
   * Get the safe area for rendering within the current viewport.
   *
   * Some devices have portions of the screen which are partially obscured or
   * not interactive, possibly due to on-screen controls, curved edges, camera
   * notches, TV overscan, etc. This function provides the area of the current
   * viewport which is safe to have interactible content. You should continue
   * rendering into the rest of the render target, but it should not contain
   * visually important or interactible content.
   *
   * @returns the rect filled the area that is safe for interactive content on
   * success or std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Rect> GetSafeArea() const
  {
    if (Rect rect; SDL_GetRenderSafeArea(T::get(), &rect)) return rect;
    return {};
  }

  bool ResetClipRect() { return SetClipRect({}); }

  /**
   * Set the clip rectangle for rendering on the specified target.
   *
   * @param rect an SDL_Rect structure representing the clip area, relative to
   *             the viewport, or NULL to disable clipping.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetClipRect()
   * @sa ClipEnabled()
   */
  bool SetClipRect(OptionalRef<const SDL_Rect> rect)
  {
    return SDL_SetRenderClipRect(T::get(), rect);
  }

  /**
   * Get the clip rectangle for the current target.
   *
   * @returns the rect filled in with the current clipping area or an empty
   * rectangle if clipping is disabled on success; std::nullopt on failure, call
   * GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ClipEnabled()
   * @sa SetClipRect()
   */
  std::optional<Rect> GetClipRect() const
  {
    if (Rect rect; SDL_GetRenderClipRect(T::get(), &rect)) return rect;
    return {};
  }

  /**
   * Get whether clipping is enabled on the given renderer.
   *
   * @returns true if clipping is enabled or false if not; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetClipRect()
   * @sa SetClipRect()
   */
  bool IsClipEnabled() const { return SDL_RenderClipEnabled(T::get()); }

  /**
   * Set the drawing scale for rendering on the current target.
   *
   * The drawing coordinates are scaled by the x/y scaling factors before they
   * are used by the renderer. This allows resolution independent drawing with a
   * single coordinate system.
   *
   * If this results in scaling or subpixel drawing by the rendering backend, it
   * will be handled using the appropriate quality hints. For best results use
   * integer scaling factors.
   *
   * @param scale the x, y scaling factors.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetScale()
   */
  bool SetScale(SDL_FPoint scale)
  {
    return SDL_SetRenderScale(T::get(), scale);
  }

  std::optional<FPoint> GetScale() const
  {
    if (FPoint scale; GetScale(&scale)) return scale;
    return std::nullopt;
  }

  bool GetScale(FPoint* scale) const
  {
    if (!scale) return GetScale(nullptr, nullptr);
    return GetScale(&scale->x, &scale->y);
  }

  /**
   * Get the drawing scale for the current target.
   *
   * @param scaleX a pointer filled in with the horizontal scaling factor.
   * @param scaleY a pointer filled in with the vertical scaling factor.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetScale()
   */
  bool GetScale(float* scaleX, float* scaleY) const
  {
    return SDL_GetRenderScale(T::get(), scaleX, scaleY);
  }

  /**
   * Set the color used for drawing operations.
   *
   * Set the color for drawing or filling rectangles, lines, and points, and for
   * SDL_RenderClear().
   *
   * @param c the color value used to draw on the rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColor()
   * @sa SetDrawColorFloat()
   */
  bool SetDrawColor(SDL_Color c)
  {
    return SDL_SetRenderDrawColor(T::get(), c.r, c.g, c.b, c.a);
  }

  /**
   * Set the color used for drawing operations (Rect, Line and Clear).
   *
   * Set the color for drawing or filling rectangles, lines, and points, and for
   * SDL_RenderClear().
   *
   * @param c the RGBA values used to draw on the rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColorFloat()
   * @sa SetDrawColor()
   */
  bool SetDrawColor(SDL_FColor c)
  {
    return SDL_SetRenderDrawColorFloat(T::get(), c.r, c.g, c.b, c.a);
  }

  std::optional<FColor> GetDrawColor() const
  {
    if (FColor color; GetDrawColor(&color)) return color;
    return std::nullopt;
  }

  bool GetDrawColor(SDL_Color* c) const
  {
    if (!c) {
      return SDL_GetRenderDrawColor(
        T::get(), nullptr, nullptr, nullptr, nullptr);
    }
    return GetDrawColor(&c->r, &c->g, &c->b, &c->a);
  }

  bool GetDrawColor(SDL_FColor* c) const
  {
    if (!c) {
      return SDL_GetRenderDrawColorFloat(
        T::get(), nullptr, nullptr, nullptr, nullptr);
    }
    return GetDrawColor(&c->r, &c->g, &c->b, &c->a);
  }

  /**
   * Get the color used for drawing operations (Rect, Line and Clear).
   *
   * @param r a pointer filled in with the red value used to draw on the
   *          rendering target.
   * @param g a pointer filled in with the green value used to draw on the
   *          rendering target.
   * @param b a pointer filled in with the blue value used to draw on the
   *          rendering target.
   * @param a a pointer filled in with the alpha value used to draw on the
   *          rendering target; usually `SDL_ALPHA_OPAQUE` (255).
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawColorFloat()
   * @sa SetDrawColor()
   */
  bool GetDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a) const
  {
    return SDL_GetRenderDrawColor(T::get(), r, g, b, a);
  }

  /**
   * Get the color used for drawing operations (Rect, Line and Clear).
   *
   * @param r a pointer filled in with the red value used to draw on the
   *          rendering target.
   * @param g a pointer filled in with the green value used to draw on the
   *          rendering target.
   * @param b a pointer filled in with the blue value used to draw on the
   *          rendering target.
   * @param a a pointer filled in with the alpha value used to draw on the
   *          rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetDrawColorFloat()
   * @sa GetDrawColor()
   */
  bool GetDrawColor(float* r, float* g, float* b, float* a) const
  {
    return SDL_GetRenderDrawColorFloat(T::get(), r, g, b, a);
  }

  /**
   * Set the color scale used for render operations.
   *
   * The color scale is an additional scale multiplied into the pixel color
   * value while rendering. This can be used to adjust the brightness of colors
   * during HDR rendering, or changing HDR video brightness when playing on an
   * SDR display.
   *
   * The color scale does not affect the alpha channel, only the color
   * brightness.
   *
   * @param scale the color scale value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetColorScale()
   */
  bool SetColorScale(float scale)
  {
    return SDL_SetRenderColorScale(T::get(), scale);
  }

  /**
   * Get the color scale used for render operations.
   *
   * @returns a float representing the current color scale on success or
   * std::nullopt on failure; call SDL_GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetColorScale()
   */
  std::optional<float> GetColorScale() const
  {
    if (float scale; SDL_GetRenderColorScale(T::get(), &scale)) return scale;
    return std::nullopt;
  }

  /**
   * Set the blend mode used for drawing operations (Fill and Line).
   *
   * If the blend mode is not supported, the closest supported mode is chosen.
   *
   * @param blendMode the SDL_BlendMode to use for blending.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetDrawBlendMode()
   */
  bool SetDrawBlendMode(BlendMode blendMode)
  {
    return SDL_SetRenderDrawBlendMode(T::get(), blendMode);
  }

  /**
   * Get the blend mode used for drawing operations.
   *
   * @returns the current BlendMode on success or std::nullopt on failure; call
   * SDL_GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetDrawBlendMode()
   */
  std::optional<BlendMode> GetDrawBlendMode() const
  {
    if (BlendMode blendMode; SDL_GetRenderDrawBlendMode(T::get(), &blendMode)) {
      return blendMode;
    }
    return std::nullopt;
  }

  /**
   * Clear the current rendering target with the drawing color.
   *
   * This function clears the entire rendering target, ignoring the viewport and
   * the clip rectangle. Note, that clearing will also set/fill all pixels of
   * the rendering target to current renderer draw color, so make sure to invoke
   * SDL_SetRenderDrawColor() when needed.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetDrawColor()
   */
  bool RenderClear() { return SDL_RenderClear(T::get()); }

  /**
   * Draw a point on the current rendering target at subpixel precision.
   *
   * @param p the x, y coordinates of the point.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderPoints
   */
  bool RenderPoint(SDL_FPoint p) { return SDL_RenderPoint(T::get(), p); }

  /**
   * Draw multiple points on the current rendering target at subpixel precision.
   *
   * @param points the points to draw.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderPoint
   */
  bool RenderPoints(SpanRef<const SDL_FPoint> points)
  {
    SDL_assert_paranoid(points.size() < SDL_MAX_SINT32);
    return SDL_RenderPoints(T::get(), points.data(), points.size());
  }

  /**
   * Draw a line on the current rendering target at subpixel precision.
   *
   * @param p1 the x,y coordinate of the start point.
   * @param p2 the x,y coordinate of the end point.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderLines
   */
  bool RenderLine(SDL_FPoint p1, SDL_FPoint p2)
  {
    return SDL_RenderLine(T::get(), p1, p2);
  }

  /**
   * Draw a series of connected lines on the current rendering target at
   * subpixel precision.
   *
   * @param points the points along the lines, drawing count-1 lines.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderLine
   */
  bool RenderLines(SpanRef<const SDL_FPoint> points)
  {
    SDL_assert_paranoid(points.size() < SDL_MAX_SINT32);
    return SDL_RenderLines(T::get(), points.data(), points.size());
  }

  /**
   * Draw a rectangle on the current rendering target at subpixel precision.
   *
   * @param rect a pointer to the destination rectangle, or std::nullopt to
   *             outline the entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderRects
   */
  bool RenderRect(OptionalRef<const SDL_FRect> rect)
  {
    return SDL_RenderRect(T::get(), rect);
  }

  /**
   * Draw some number of rectangles on the current rendering target at subpixel
   * precision.
   *
   * @param rects a pointer to an array of destination rectangles.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderRect
   */
  bool RenderRects(SpanRef<const SDL_FRect> rects)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_SINT32);
    return SDL_RenderRects(T::get(), rects.data(), rects.size());
  }

  /**
   * Fill a rectangle on the current rendering target with the drawing color at
   * subpixel precision.
   *
   * @param rect a pointer to the destination rectangle, or std::nullopt for the
   *             entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderFillRects
   */
  bool RenderFillRect(OptionalRef<const SDL_FRect> rect)
  {
    return SDL_RenderFillRect(T::get(), rect);
  }

  /**
   * Fill some number of rectangles on the current rendering target with the
   * drawing color at subpixel precision.
   *
   * @param rects a pointer to an array of destination rectangles.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderFillRect
   */
  bool RenderFillRects(SpanRef<const SDL_FRect> rects)
  {
    SDL_assert_paranoid(rects.size() < SDL_MAX_SINT32);
    return SDL_RenderFillRects(T::get(), rects.data(), rects.size());
  }

  /**
   * Copy a portion of the texture to the current rendering target at subpixel
   * precision.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or NULL for the entire
   *                texture.
   * @param dstrect a pointer to the destination rectangle, or NULL for the
   *                entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTextureRotated
   * @sa SDL_RenderTextureTiled
   */
  bool RenderTexture(TextureRef texture,
                     OptionalRef<const SDL_FRect> srcrect,
                     OptionalRef<const SDL_FRect> dstrect);

  /**
   * Copy a portion of the source texture to the current rendering target, with
   * rotation and flipping, at subpixel precision.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or NULL for the entire
   *                texture.
   * @param dstrect a pointer to the destination rectangle, or NULL for the
   *                entire rendering target.
   * @param angle an angle in degrees that indicates the rotation that will be
   *              applied to dstrect, rotating it in a clockwise direction.
   * @param center a pointer to a point indicating the point around which
   *               dstrect will be rotated (if NULL, rotation will be done
   *               around dstrect.w/2, dstrect.h/2).
   * @param flip an SDL_FlipMode value stating which flipping actions should be
   *             performed on the texture.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTexture
   */
  bool RenderTextureRotated(TextureRef texture,
                            OptionalRef<const SDL_FRect> srcrect,
                            OptionalRef<const SDL_FRect> dstrect,
                            double angle,
                            OptionalRef<const SDL_FPoint> center = {},
                            FlipMode flip = SDL_FLIP_NONE);

  /**
   * Copy a portion of the source texture to the current rendering target, with
   * affine transform, at subpixel precision.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or NULL for the entire
   *                texture.
   * @param origin a pointer to a point indicating where the top-left corner of
   *               srcrect should be mapped to, or NULL for the rendering
   *               target's origin.
   * @param right a pointer to a point indicating where the top-right corner of
   *              srcrect should be mapped to, or NULL for the rendering
   *              target's top-right corner.
   * @param down a pointer to a point indicating where the bottom-left corner of
   *             srcrect should be mapped to, or NULL for the rendering target's
   *             bottom-left corner.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety You may only call this function from the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTexture
   */
  bool RenderTextureAffine(TextureRef texture,
                           OptionalRef<const SDL_FRect> srcrect,
                           OptionalRef<const SDL_FPoint> origin,
                           OptionalRef<const SDL_FPoint> right,
                           OptionalRef<const SDL_FPoint> down);

  /**
   * Tile a portion of the texture to the current rendering target at subpixel
   * precision.
   *
   * The pixels in `srcrect` will be repeated as many times as needed to
   * completely fill `dstrect`.
   *
   * @param texture the source texture.
   * @param srcrect a pointer to the source rectangle, or NULL for the entire
   *                texture.
   * @param scale the scale used to transform srcrect into the destination
   *              rectangle, e.g. a 32x32 texture with a scale of 2 would fill
   *              64x64 tiles.
   * @param dstrect a pointer to the destination rectangle, or NULL for the
   *                entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTexture
   */
  bool RenderTextureTiled(TextureRef texture,
                          OptionalRef<const SDL_FRect> srcrect,
                          float scale,
                          OptionalRef<const SDL_FRect> dstrect);

  /**
   * Perform a scaled copy using the 9-grid algorithm to the current rendering
   * target at subpixel precision.
   *
   * The pixels in the texture are split into a 3x3 grid, using the different
   * corner sizes for each corner, and the sides and center making up the
   * remaining pixels. The corners are then scaled using `scale` and fit into
   * the corners of the destination rectangle. The sides and center are then
   * stretched into place to cover the remaining destination rectangle.
   *
   * @param texture the source texture.
   * @param srcrect the SDL_Rect structure representing the rectangle to be used
   *                for the 9-grid, or NULL to use the entire texture.
   * @param left_width the width, in pixels, of the left corners in `srcrect`.
   * @param right_width the width, in pixels, of the right corners in `srcrect`.
   * @param top_height the height, in pixels, of the top corners in `srcrect`.
   * @param bottom_height the height, in pixels, of the bottom corners in
   *                      `srcrect`.
   * @param scale the scale used to transform the corner of `srcrect` into the
   *              corner of `dstrect`, or 0.0f for an unscaled copy.
   * @param dstrect a pointer to the destination rectangle, or NULL for the
   *                entire rendering target.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderTexture
   */
  bool RenderTexture9Grid(TextureRef texture,
                          OptionalRef<const SDL_FRect> srcrect,
                          float left_width,
                          float right_width,
                          float top_height,
                          float bottom_height,
                          float scale,
                          OptionalRef<const SDL_FRect> dstrect);

  /**
   * Render a list of triangles, optionally using a texture and indices into the
   * vertex array Color and alpha modulation is done per vertex
   * (SDL_SetTextureColorMod and SDL_SetTextureAlphaMod are ignored).
   *
   * @param texture (optional) The SDL texture to use.
   * @param vertices vertices.
   * @param indices (optional) An array of integer indices into the 'vertices'
   *                array, if NULL all vertices will be rendered in sequential
   *                order.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderGeometryRaw
   */
  bool RenderGeometry(TextureRef texture,
                      std::span<const Vertex> vertices,
                      std::span<const int> indices = {});

  /**
   * Render a list of triangles, optionally using a texture and indices into the
   * vertex arrays Color and alpha modulation is done per vertex
   * (SDL_SetTextureColorMod and SDL_SetTextureAlphaMod are ignored).
   *
   * @param texture (optional) The SDL texture to use.
   * @param xy vertex positions.
   * @param xy_stride byte size to move from one element to the next element.
   * @param color vertex colors (as SDL_FColor).
   * @param color_stride byte size to move from one element to the next element.
   * @param uv vertex normalized texture coordinates.
   * @param uv_stride byte size to move from one element to the next element.
   * @param num_vertices number of vertices.
   * @param indices (optional) An array of indices into the 'vertices' arrays,
   *                if NULL all vertices will be rendered in sequential order.
   * @param num_indices number of indices.
   * @param size_indices index size: 1 (byte), 2 (short), 4 (int).
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_RenderGeometry
   */
  bool RenderGeometryRaw(TextureRef texture,
                         const float* xy,
                         int xy_stride,
                         const FColor* color,
                         int color_stride,
                         const float* uv,
                         int uv_stride,
                         int num_vertices,
                         const void* indices,
                         int num_indices,
                         int size_indices);

  /**
   * Read pixels from the current rendering target.
   *
   * **WARNING**: This is a very slow operation, and should not be used
   * frequently. If you're using this on the main rendering target, it should be
   * called after rendering and before SDL_RenderPresent().
   *
   * @param rect an SDL_Rect structure representing the area in pixels relative
   *             to the to current viewport, or std::nullopt for the entire
   *             viewport.
   * @returns a new SDL_Surface on success or NULL on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Surface RenderReadPixels(OptionalRef<const SDL_Rect> rect) const
  {
    return {SDL_RenderReadPixels(T::get(), rect)};
  }

  /**
   * Update the screen with any rendering performed since the previous call.
   *
   * SDL's rendering functions operate on a backbuffer; that is, calling a
   * rendering function such as SDL_RenderLine() does not directly put a line on
   * the screen, but rather updates the backbuffer. As such, you compose your
   * entire scene and *present* the composed backbuffer to the screen as a
   * complete picture.
   *
   * Therefore, when using SDL's rendering API, one does all drawing intended
   * for the frame, and then calls this function once per frame to present the
   * final drawing to the user.
   *
   * The backbuffer should be considered invalidated after each present; do not
   * assume that previous contents will exist between frames. You are strongly
   * encouraged to call SDL_RenderClear() to initialize the backbuffer before
   * starting each new frame's drawing, even if you plan to overwrite every
   * pixel.
   *
   * Please note, that in case of rendering to a texture - there is **no need**
   * to call `SDL_RenderPresent` after drawing needed objects to a texture, and
   * should not be done; you are only required to change back the rendering
   * target to default via `SDL_SetRenderTarget(renderer, NULL)` afterwards, as
   * textures by themselves do not have a concept of backbuffers. Calling
   * SDL_RenderPresent while rendering to a texture will still update the screen
   * with any current drawing that has been done _to the window itself_.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RenderClear()
   * @sa RenderFillRect()
   * @sa RenderFillRects()
   * @sa RenderLine()
   * @sa RenderLines()
   * @sa RenderPoint()
   * @sa RenderPoints()
   * @sa RenderRect()
   * @sa RenderRects()
   * @sa SetDrawBlendMode()
   * @sa SetDrawColor()
   */
  bool Present() { return SDL_RenderPresent(T::get()); }

  /**
   * Destroy the rendering context for a window and free all associated
   * textures.
   *
   * This object becomes empty after the call.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CreateRenderer
   */
  void Destroy() { return SDL_DestroyRenderer(T::release()); }

  /**
   * Force the rendering context to flush any pending commands and state.
   *
   * You do not need to (and in fact, shouldn't) call this function unless you
   * are planning to call into OpenGL/Direct3D/Metal/whatever directly, in
   * addition to using an SDL_Renderer.
   *
   * This is for a very-specific case: if you are using SDL's render API, and
   * you plan to make OpenGL/D3D/whatever calls in addition to SDL render API
   * calls. If this applies, you should call this function between calls to
   * SDL's render API and the low-level API you're using in cooperation.
   *
   * In all other cases, you can ignore this function.
   *
   * This call makes SDL flush any pending rendering work it was queueing up to
   * do later in a single batch, and marks any internal cached state as invalid,
   * so it'll prepare all its state again later, from scratch.
   *
   * This means you do not need to save state in your rendering code to protect
   * the SDL renderer. However, there lots of arbitrary pieces of Direct3D and
   * OpenGL state that can confuse things; you should use your best judgment and
   * be prepared to make changes if specific state needs to be protected.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Flush() { return SDL_FlushRenderer(T::get()); }

  /**
   * Toggle VSync of the given renderer.
   *
   * When a renderer is created, vsync defaults to SDL_RENDERER_VSYNC_DISABLED.
   *
   * The `vsync` parameter can be 1 to synchronize present with every vertical
   * refresh, 2 to synchronize present with every second vertical refresh, etc.,
   * SDL_RENDERER_VSYNC_ADAPTIVE for late swap tearing (adaptive vsync), or
   * SDL_RENDERER_VSYNC_DISABLED to disable. Not every value is supported by
   * every driver, so you should check the return value to see whether the
   * requested setting is supported.
   *
   * @param vsync the vertical refresh sync interval.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetVSync()
   */
  bool SetVSync(int vsync) { return SDL_SetRenderVSync(T::get(), vsync); }

  /**
   * Get VSync of the given renderer.
   *
   * @returns the current vertical refresh sync interval on success or
   * std::nullopt on failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetVSync()
   */
  std::optional<int> GetVSync() const
  {
    if (int vsync; SDL_GetRenderVSync(T::get(), &vsync)) return vsync;
    return std::nullopt;
  }

  /**
   * Draw debug text to an SDL_Renderer.
   *
   * This function will render a string of text to an SDL_Renderer. Note that
   * this is a convenience function for debugging, with severe limitations, and
   * not intended to be used for production apps and games.
   *
   * Among these limitations:
   *
   * - It accepts UTF-8 strings, but will only renders ASCII characters.
   * - It has a single, tiny size (8x8 pixels). One can use logical presentation
   *   or scaling to adjust it, but it will be blurry.
   * - It uses a simple, hardcoded bitmap font. It does not allow different font
   *   selections and it does not support truetype, for proper scaling.
   * - It does no word-wrapping and does not treat newline characters as a line
   *   break. If the text goes out of the window, it's gone.
   *
   * For serious text rendering, there are several good options, such as
   * SDL_ttf, stb_truetype, or other external libraries.
   *
   * On first use, this will create an internal texture for rendering glyphs.
   * This texture will live until the renderer is destroyed.
   *
   * The text is drawn in the color specified by SDL_SetRenderDrawColor().
   *
   * @param p the x, y coordinates where the top-left corner of the text will
   * draw.
   * @param str the string to render.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa DebugTextFormat()
   * @sa SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE
   */
  bool RenderDebugText(FPoint p, StringParam str)
  {
    return SDL_RenderDebugText(T::get(), p.x, p.y, str);
  }

  // TODO RenderDebugTextFormat
};

/**
 * An efficient driver-specific representation of pixel data
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @ingroup resource
 */
template<ObjectBox<SDL_Texture*> T>
struct TextureBase : T
{
  using T::T;

  /**
   * Create a texture for a rendering context.
   *
   * The contents of a texture when first created are not defined.
   *
   * @param renderer the rendering context.
   * @param format one of the enumerated values in SDL_PixelFormat.
   * @param access one of the enumerated values in SDL_TextureAccess.
   * @param w the width of the texture in pixels.
   * @param h the height of the texture in pixels.
   * @post the created texture is convertible to true on success or false on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   * @sa Update()
   */
  TextureBase(RendererRef renderer,
              PixelFormat format,
              TextureAccess access,
              int w,
              int h)
    : T(SDL_CreateTexture(renderer.get(), format, access, w, h))
  {
  }

  /**
   * Create a texture from an existing surface.
   *
   * The surface is not modified or freed by this function.
   *
   * The SDL_TextureAccess hint for the created texture is
   * `SDL_TEXTUREACCESS_STATIC`.
   *
   * The pixel format of the created texture may be different from the pixel
   * format of the surface, and can be queried using the
   * SDL_PROP_TEXTURE_FORMAT_NUMBER property.
   *
   * @param renderer the rendering context.
   * @param surface the SDL_Surface structure containing pixel data used to fill
   *                the texture.
   * @post the created texture is convertible to true on success or false on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetSize()
   * @sa Update()
   */
  TextureBase(RendererRef renderer, SurfaceRef surface)
    : T(SDL_CreateTextureFromSurface(renderer.get(), surface.get()))
  {
  }

  /**
   * Create a texture for a rendering context with the specified properties.
   *
   * These are the supported properties:
   *
   * - `SDL_PROP_TEXTURE_CREATE_COLORSPACE_NUMBER`: an SDL_Colorspace value
   *   describing the texture colorspace, defaults to SDL_COLORSPACE_SRGB_LINEAR
   *   for floating point textures, SDL_COLORSPACE_HDR10 for 10-bit textures,
   *   SDL_COLORSPACE_SRGB for other RGB textures and SDL_COLORSPACE_JPEG for
   *   YUV textures.
   * - `SDL_PROP_TEXTURE_CREATE_FORMAT_NUMBER`: one of the enumerated values in
   *   SDL_PixelFormat, defaults to the best RGBA format for the renderer
   * - `SDL_PROP_TEXTURE_CREATE_ACCESS_NUMBER`: one of the enumerated values in
   *   SDL_TextureAccess, defaults to SDL_TEXTUREACCESS_STATIC
   * - `SDL_PROP_TEXTURE_CREATE_WIDTH_NUMBER`: the width of the texture in
   *   pixels, required
   * - `SDL_PROP_TEXTURE_CREATE_HEIGHT_NUMBER`: the height of the texture in
   *   pixels, required
   * - `SDL_PROP_TEXTURE_CREATE_SDR_WHITE_POINT_FLOAT`: for HDR10 and floating
   *   point textures, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 100 for HDR10 textures and 1.0 for floating point textures.
   * - `SDL_PROP_TEXTURE_CREATE_HDR_HEADROOM_FLOAT`: for HDR10 and floating
   *   point textures, this defines the maximum dynamic range used by the
   *   content, in terms of the SDR white point. This would be equivalent to
   *   maxCLL / SDL_PROP_TEXTURE_CREATE_SDR_WHITE_POINT_FLOAT for HDR10 content.
   *   If this is defined, any values outside the range supported by the display
   *   will be scaled into the available HDR headroom, otherwise they are
   *   clipped.
   *
   * With the direct3d11 renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_POINTER`: the ID3D11Texture2D
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_U_POINTER`: the ID3D11Texture2D
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_V_POINTER`: the ID3D11Texture2D
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the direct3d12 renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_POINTER`: the ID3D12Resource
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_U_POINTER`: the ID3D12Resource
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_V_POINTER`: the ID3D12Resource
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the metal renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_METAL_PIXELBUFFER_POINTER`: the CVPixelBufferRef
   *   associated with the texture, if you want to create a texture from an
   *   existing pixel buffer.
   *
   * With the opengl renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the opengles2 renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture, if you want to wrap an existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture, if you want to wrap an
   *   existing texture.
   * - `SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture, if you want to wrap an
   *   existing texture.
   *
   * With the vulkan renderer:
   *
   * - `SDL_PROP_TEXTURE_CREATE_VULKAN_TEXTURE_NUMBER`: the VkImage with layout
   *   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL associated with the texture, if
   *   you want to wrap an existing texture.
   *
   * @param renderer the rendering context.
   * @param props the properties to use.
   * @post the created texture is convertible to true on success or false on
   * failure; call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CreateProperties
   * @sa SDL_CreateTexture
   * @sa SDL_CreateTextureFromSurface
   * @sa SDL_DestroyTexture
   * @sa SDL_GetTextureSize
   * @sa SDL_UpdateTexture
   */
  TextureBase(RendererRef renderer, PropertiesRef props)
    : T(SDL_CreateTextureWithProperties(renderer.get(), props))
  {
  }

  /**
   * Get the properties associated with a texture.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `SDL_PROP_TEXTURE_COLORSPACE_NUMBER`: an SDL_Colorspace value describing
   *   the texture colorspace.
   * - `SDL_PROP_TEXTURE_FORMAT_NUMBER`: one of the enumerated values in
   *   SDL_PixelFormat.
   * - `SDL_PROP_TEXTURE_ACCESS_NUMBER`: one of the enumerated values in
   *   SDL_TextureAccess.
   * - `SDL_PROP_TEXTURE_WIDTH_NUMBER`: the width of the texture in pixels.
   * - `SDL_PROP_TEXTURE_HEIGHT_NUMBER`: the height of the texture in pixels.
   * - `SDL_PROP_TEXTURE_SDR_WHITE_POINT_FLOAT`: for HDR10 and floating point
   *   textures, this defines the value of 100% diffuse white, with higher
   *   values being displayed in the High Dynamic Range headroom. This defaults
   *   to 100 for HDR10 textures and 1.0 for other textures.
   * - `SDL_PROP_TEXTURE_HDR_HEADROOM_FLOAT`: for HDR10 and floating point
   *   textures, this defines the maximum dynamic range used by the content, in
   *   terms of the SDR white point. If this is defined, any values outside the
   *   range supported by the display will be scaled into the available HDR
   *   headroom, otherwise they are clipped. This defaults to 1.0 for SDR
   *   textures, 4.0 for HDR10 textures, and no default for floating point
   *   textures.
   *
   * With the direct3d11 renderer:
   *
   * - `SDL_PROP_TEXTURE_D3D11_TEXTURE_POINTER`: the ID3D11Texture2D associated
   *   with the texture
   * - `SDL_PROP_TEXTURE_D3D11_TEXTURE_U_POINTER`: the ID3D11Texture2D
   *   associated with the U plane of a YUV texture
   * - `SDL_PROP_TEXTURE_D3D11_TEXTURE_V_POINTER`: the ID3D11Texture2D
   *   associated with the V plane of a YUV texture
   *
   * With the direct3d12 renderer:
   *
   * - `SDL_PROP_TEXTURE_D3D12_TEXTURE_POINTER`: the ID3D12Resource associated
   *   with the texture
   * - `SDL_PROP_TEXTURE_D3D12_TEXTURE_U_POINTER`: the ID3D12Resource associated
   *   with the U plane of a YUV texture
   * - `SDL_PROP_TEXTURE_D3D12_TEXTURE_V_POINTER`: the ID3D12Resource associated
   *   with the V plane of a YUV texture
   *
   * With the vulkan renderer:
   *
   * - `SDL_PROP_TEXTURE_VULKAN_TEXTURE_NUMBER`: the VkImage associated with the
   *   texture
   *
   * With the opengl renderer:
   *
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_NUMBER`: the GLuint texture associated
   *   with the texture
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_U_NUMBER`: the GLuint texture associated
   *   with the U plane of a YUV texture
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_V_NUMBER`: the GLuint texture associated
   *   with the V plane of a YUV texture
   * - `SDL_PROP_TEXTURE_OPENGL_TEXTURE_TARGET_NUMBER`: the GLenum for the
   *   texture target (`GL_TEXTURE_2D`, `GL_TEXTURE_RECTANGLE_ARB`, etc)
   * - `SDL_PROP_TEXTURE_OPENGL_TEX_W_FLOAT`: the texture coordinate width of
   *   the texture (0.0 - 1.0)
   * - `SDL_PROP_TEXTURE_OPENGL_TEX_H_FLOAT`: the texture coordinate height of
   *   the texture (0.0 - 1.0)
   *
   * With the opengles2 renderer:
   *
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_NUMBER`: the GLuint texture
   *   associated with the texture
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_UV_NUMBER`: the GLuint texture
   *   associated with the UV plane of an NV12 texture
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_U_NUMBER`: the GLuint texture
   *   associated with the U plane of a YUV texture
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_V_NUMBER`: the GLuint texture
   *   associated with the V plane of a YUV texture
   * - `SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_TARGET_NUMBER`: the GLenum for the
   *   texture target (`GL_TEXTURE_2D`, `GL_TEXTURE_EXTERNAL_OES`, etc)
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return PropertiesRef{SDL_GetTextureProperties(T::get())};
  }

  /**
   * Get the renderer that created an SDL_Texture.
   *
   * @returns a pointer to the SDL_Renderer that created the texture, or NULL on
   *          failure; call SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  RendererRef GetRenderer() const
  {
    return SDL_GetRendererFromTexture(T::get());
  }

  std::optional<FPoint> GetSize() const
  {
    if (FPoint size; GetSize(&size)) return size;
    return std::nullopt;
  }

  bool GetSize(FPoint* size) const
  {
    if (!size) return GetSize(nullptr, nullptr);
    return GetSize(&size->x, &size->y);
  }

  /**
   * Get the size of a texture, as floating point values.
   *
   * @param w a pointer filled in with the width of the texture in pixels. This
   *          argument can be NULL if you don't need this information.
   * @param h a pointer filled in with the height of the texture in pixels. This
   *          argument can be NULL if you don't need this information.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetSize(float* w, float* h) const
  {
    return SDL_GetTextureSize(T::get(), w, h);
  }

  bool SetColorAndAlphaMod(Color c)
  {
    return SetColorMod(c.r, c.g, c.b) && SetAlphaMod(c.a);
  }

  bool SetColorAndAlphaMod(FColor c)
  {
    return SetColorMod(c.r, c.g, c.b) && SetAlphaMod(c.a);
  }

  std::optional<FColor> GetColorAndAlphaMod() const
  {
    if (FColor color; GetColorAndAlphaMod(&color)) return color;
    return std::nullopt;
  }

  bool GetColorAndAlphaMod(Color* c) const
  {
    SDL_assert(c != nullptr);
    return GetColorMod(&c->r, &c->g, &c->b) && GetAlphaMod(&c->a);
  }

  bool GetColorAndAlphaMod(FColor* c) const
  {
    SDL_assert(c != nullptr);
    return GetColorMod(&c->r, &c->g, &c->b) && GetAlphaMod(&c->a);
  }

  /**
   * Set an additional color value multiplied into render copy operations.
   *
   * When this texture is rendered, during the copy operation each source color
   * channel is modulated by the appropriate color value according to the
   * following formula:
   *
   * `srcC = srcC * (color / 255)`
   *
   * Color modulation is not always supported by the renderer; it will return
   * false if color modulation is not supported.
   *
   * @param r the red color value multiplied into copy operations.
   * @param g the green color value multiplied into copy operations.
   * @param b the blue color value multiplied into copy operations.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureColorMod
   * @sa SDL_SetTextureAlphaMod
   * @sa SDL_SetTextureColorModFloat
   */
  bool SetColorMod(Uint8 r, Uint8 g, Uint8 b)
  {
    return SDL_SetTextureColorMod(T::get(), r, g, b);
  }

  /**
   * Set an additional color value multiplied into render copy operations.
   *
   * When this texture is rendered, during the copy operation each source color
   * channel is modulated by the appropriate color value according to the
   * following formula:
   *
   * `srcC = srcC * color`
   *
   * Color modulation is not always supported by the renderer; it will return
   * false if color modulation is not supported.
   *
   * @param r the red color value multiplied into copy operations.
   * @param g the green color value multiplied into copy operations.
   * @param b the blue color value multiplied into copy operations.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureColorModFloat
   * @sa SDL_SetTextureAlphaModFloat
   * @sa SDL_SetTextureColorMod
   */
  bool SetColorMod(float r, float g, float b)
  {
    return SDL_SetTextureColorModFloat(T::get(), r, g, b);
  }

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureAlphaMod
   * @sa SDL_GetTextureColorModFloat
   * @sa SDL_SetTextureColorMod
   */
  bool GetColorMod(Uint8* r, Uint8* g, Uint8* b) const
  {
    return SDL_GetTextureColorMod(T::get(), r, g, b);
  }

  /**
   * Get the additional color value multiplied into render copy operations.
   *
   * @param r a pointer filled in with the current red color value.
   * @param g a pointer filled in with the current green color value.
   * @param b a pointer filled in with the current blue color value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureAlphaModFloat
   * @sa SDL_GetTextureColorMod
   * @sa SDL_SetTextureColorModFloat
   */
  bool GetColorMod(float* r, float* g, float* b) const
  {
    return SDL_GetTextureColorModFloat(T::get(), r, g, b);
  }

  /**
   * Set an additional alpha value multiplied into render copy operations.
   *
   * When this texture is rendered, during the copy operation the source alpha
   * value is modulated by this alpha value according to the following formula:
   *
   * `srcA = srcA * (alpha / 255)`
   *
   * Alpha modulation is not always supported by the renderer; it will return
   * false if alpha modulation is not supported.
   *
   * @param alpha the source alpha value multiplied into copy operations.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureAlphaMod
   * @sa SDL_SetTextureAlphaModFloat
   * @sa SDL_SetTextureColorMod
   */
  bool SetAlphaMod(Uint8 alpha)
  {
    return SDL_SetTextureAlphaMod(T::get(), alpha);
  }

  /**
   * Set an additional alpha value multiplied into render copy operations.
   *
   * When this texture is rendered, during the copy operation the source alpha
   * value is modulated by this alpha value according to the following formula:
   *
   * `srcA = srcA * alpha`
   *
   * Alpha modulation is not always supported by the renderer; it will return
   * false if alpha modulation is not supported.
   *
   * @param alpha the source alpha value multiplied into copy operations.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureAlphaModFloat
   * @sa SDL_SetTextureAlphaMod
   * @sa SDL_SetTextureColorModFloat
   */
  bool SetAlphaMod(float alpha)
  {
    return SDL_SetTextureAlphaModFloat(T::get(), alpha);
  }

  std::optional<float> GetAlphaMod() const
  {
    if (float alpha; GetAlphaMod(&alpha)) return alpha;
    return std::nullopt;
  }

  /**
   * Get the additional alpha value multiplied into render copy operations.
   *
   * @param alpha a pointer filled in with the current alpha value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetAlphaMod(Uint8* alpha) const
  {
    return SDL_GetTextureAlphaMod(T::get(), alpha);
  }

  /**
   * Get the additional alpha value multiplied into render copy operations.
   *
   * @param alpha a pointer filled in with the current alpha value.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetAlphaMod(float* alpha) const
  {
    return SDL_GetTextureAlphaModFloat(T::get(), alpha);
  }

  /**
   * Set the blend mode for a texture, used by SDL_RenderTexture().
   *
   * If the blend mode is not supported, the closest supported mode is chosen
   * and this function returns false.
   *
   * @param blendMode the SDL_BlendMode to use for texture blending.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureBlendMode
   */
  bool SetBlendMode(BlendMode blendMode)
  {
    return SDL_SetTextureBlendMode(T::get(), blendMode);
  }

  /**
   * Get the blend mode used for texture copy operations.
   *
   * @returns the current SDL_BlendMode on success or std::nullopt on failure;
   * call GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_SetTextureBlendMode
   */
  std::optional<BlendMode> GetBlendMode() const
  {
    if (BlendMode blendMode; SDL_GetTextureBlendMode(T::get(), &blendMode)) {
      return blendMode;
    }
    return std::nullopt;
  }

  /**
   * Set the scale mode used for texture scale operations.
   *
   * The default texture scale mode is SDL_SCALEMODE_LINEAR.
   *
   * If the scale mode is not supported, the closest supported mode is chosen.
   *
   * @param scaleMode the SDL_ScaleMode to use for texture scaling.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetTextureScaleMode
   */
  bool SetScaleMode(ScaleMode scaleMode)
  {
    return SDL_SetTextureScaleMode(T::get(), scaleMode);
  }

  /**
   * Get the scale mode used for texture scale operations.
   *
   * @returns the current scale mode on success or std::nullopt on failure; call
   * SDL_GetError() for more information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_SetTextureScaleMode
   */
  std::optional<ScaleMode> GetScaleMode() const
  {
    if (ScaleMode scaleMode; SDL_GetTextureScaleMode(T::get(), &scaleMode)) {
      return scaleMode;
    }
    return std::nullopt;
  }

  /**
   * Update the given texture rectangle with new pixel data.
   *
   * The pixel data must be in the pixel format of the texture, which can be
   * queried using the SDL_PROP_TEXTURE_FORMAT_NUMBER property.
   *
   * This is a fairly slow function, intended for use with static textures that
   * do not change often.
   *
   * If the texture is intended to be updated often, it is preferred to create
   * the texture as streaming and use the locking functions referenced below.
   * While this function will work with streaming textures, for optimization
   * reasons you may not get the pixels back if you lock the texture afterward.
   *
   * @param rect an SDL_Rect structure representing the area to update, or NULL
   *             to update the entire texture.
   * @param pixels the raw pixel data in the format of the texture.
   * @param pitch the number of bytes in a row of pixel data, including padding
   *              between lines.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_LockTexture
   * @sa SDL_UnlockTexture
   * @sa SDL_UpdateNVTexture
   * @sa SDL_UpdateYUVTexture
   */
  bool Update(OptionalRef<const SDL_Rect> rect, const void* pixels, int pitch)
  {
    return SDL_UpdateTexture(T::get(), rect, pixels, pitch);
  }

  /**
   * Update a rectangle within a planar YV12 or IYUV texture with new pixel
   * data.
   *
   * You can use SDL_UpdateTexture() as long as your pixel data is a contiguous
   * block of Y and U/V planes in the proper order, but this function is
   * available if your pixel data is not contiguous.
   *
   * @param rect a pointer to the rectangle of pixels to update, or NULL to
   *             update the entire texture.
   * @param Yplane the raw pixel data for the Y plane.
   * @param Ypitch the number of bytes between rows of pixel data for the Y
   *               plane.
   * @param Uplane the raw pixel data for the U plane.
   * @param Upitch the number of bytes between rows of pixel data for the U
   *               plane.
   * @param Vplane the raw pixel data for the V plane.
   * @param Vpitch the number of bytes between rows of pixel data for the V
   *               plane.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_UpdateNVTexture
   * @sa SDL_UpdateTexture
   */
  bool UpdateYUV(OptionalRef<const SDL_Rect> rect,
                 const Uint8* Yplane,
                 int Ypitch,
                 const Uint8* Uplane,
                 int Upitch,
                 const Uint8* Vplane,
                 int Vpitch)
  {
    return SDL_UpdateYUVTexture(
      T::get(), rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
  }

  /**
   * Update a rectangle within a planar NV12 or NV21 texture with new pixels.
   *
   * You can use SDL_UpdateTexture() as long as your pixel data is a contiguous
   * block of NV12/21 planes in the proper order, but this function is available
   * if your pixel data is not contiguous.
   *
   * @param rect a pointer to the rectangle of pixels to update, or NULL to
   *             update the entire texture.
   * @param Yplane the raw pixel data for the Y plane.
   * @param Ypitch the number of bytes between rows of pixel data for the Y
   *               plane.
   * @param UVplane the raw pixel data for the UV plane.
   * @param UVpitch the number of bytes between rows of pixel data for the UV
   *                plane.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_UpdateTexture
   * @sa SDL_UpdateYUVTexture
   */
  bool UpdateNV(OptionalRef<const SDL_Rect> rect,
                const Uint8* Yplane,
                int Ypitch,
                const Uint8* UVplane,
                int UVpitch)
  {
    return SDL_UpdateNVTexture(
      T::get(), rect, Yplane, Ypitch, UVplane, UVpitch);
  }

  /**
   * Lock a portion of the texture for **write-only** pixel access.
   *
   * As an optimization, the pixels made available for editing don't necessarily
   * contain the old texture data. This is a write-only operation, and if you
   * need to keep a copy of the texture data you should do that at the
   * application level.
   *
   * You must use SDL_UnlockTexture() to unlock the pixels and apply any
   * changes.
   *
   * @param rect an SDL_Rect structure representing the area to lock for access;
   *             NULL to lock the entire texture.
   * @returns TextureLock on success or false if the texture is not valid or was
   * not created with `SDL_TEXTUREACCESS_STREAMING`; call GetError() for more
   * information.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TextureLock.UnlockTexture
   */
  TextureLock Lock(OptionalRef<const SDL_Rect> rect) &;

  /**
   * Destroy the texture.
   *
   * This object becomes empty after the call.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CreateTexture
   * @sa SDL_CreateTextureFromSurface
   */
  void Destroy() { return SDL_DestroyTexture(T::release()); }
};

/**
 * @brief Locks a Texture for access to its pixels
 */
class TextureLock
{
  TextureRef texture;
  SurfaceRef surface;

  /**
   * @sa TextureBase.Lock()
   */
  explicit TextureLock(TextureRef texture, OptionalRef<const SDL_Rect> rect)
    : texture(std::move(texture))
    , surface(nullptr)
  {
    SDL_Surface* maybeLock;
    if (SDL_LockTextureToSurface(this->texture.get(), rect, &maybeLock)) {
      surface = maybeLock;
    } else {
      texture.release();
    }
  }

public:
  // default ctor
  TextureLock()
    : texture(nullptr)
    , surface(nullptr)
  {
  }

  // Copy ctor
  TextureLock(const TextureLock& other) = delete;

  // Move ctor
  TextureLock(TextureLock&& other)
    : texture(other.texture.release())
    , surface(other.surface.release())
  {
  }

  /**
   * destructor
   * @sa Unlock()
   */
  ~TextureLock() { Unlock(); }

  // Assignment operator
  TextureLock& operator=(TextureLock other)
  {
    std::swap(texture, other.texture);
    std::swap(surface, other.surface);
    return *this;
  }

  /**
   * @brief Returns true if lock is active
   */
  constexpr operator bool() const { return bool(texture); }

  /**
   * Unlock a texture, uploading the changes to video memory, if needed.
   *
   * **Warning**: Please note that SDL_LockTexture() is intended to be
   * write-only; it will not guarantee the previous contents of the texture will
   * be provided. You must fully initialize any area of a texture that you lock
   * before unlocking it, as the pixels might otherwise be uninitialized memory.
   *
   * Which is to say: locking and immediately unlocking a texture can result in
   * corrupted textures, depending on the renderer in use.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_LockTexture
   */
  void Unlock()
  {
    surface.release();
    return SDL_UnlockTexture(texture.release());
  }

  /**
   * Get the pixels
   */
  void* GetPixels() const { return surface->pixels; }

  /**
   * Get pitch (the number of bytes between the start of one row the next)
   */
  int GetPitch() const { return surface->pitch; }

  /**
   * @brief Get the pixel format
   */
  PixelFormat GetFormat() const { return surface->format; }

  template<ObjectBox<SDL_Texture*> T>
  friend class TextureBase;
};

/**
 * Get the number of 2D rendering drivers available for the current display.
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
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDriver()
 */
inline int GetNumRenderDrivers() { return SDL_GetNumRenderDrivers(); }

/**
 * Use this function to get the name of a built in 2D rendering driver.
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
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetNumRenderDrivers
 */
inline const char* GetRenderDriver(int index)
{
  return SDL_GetRenderDriver(index);
}

/**
 * Create a window and default renderer.
 *
 * @param title the title of the window, in UTF-8 encoding.
 * @param size the width and height of the window.
 * @param window_flags the flags used to create the window (see
 *                     Window::Window()).
 * @returns a pair with Window and Renderer on success or a pair of std::nullptr
 * on failure; call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Renderer
 * @sa Window
 */
inline std::pair<Window, Renderer> CreateWindowAndRenderer(
  StringParam title,
  SDL_Point size,
  WindowFlags window_flags = 0)
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_CreateWindowAndRenderer(
    title, size.x, size.y, window_flags, &window, &renderer);
  return {Window{window}, Renderer{renderer}};
}

/**
 * Get the CAMetalLayer associated with the given Metal renderer.
 *
 * This function returns `void *`, so SDL doesn't have to include Metal's
 * headers, but it can be safely cast to a `CAMetalLayer *`.
 *
 * @param renderer the renderer to query.
 * @returns a `CAMetalLayer *` on success, or NULL if the renderer isn't a
 *          Metal renderer.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetRenderMetalCommandEncoder
 */
inline void* GetRenderMetalLayer(RendererRef renderer)
{
  return SDL_GetRenderMetalLayer(renderer.get());
}

/**
 * Get the Metal command encoder for the current frame.
 *
 * This function returns `void *`, so SDL doesn't have to include Metal's
 * headers, but it can be safely cast to an `id<MTLRenderCommandEncoder>`.
 *
 * This will return NULL if Metal refuses to give SDL a drawable to render to,
 * which might happen if the window is hidden/minimized/offscreen. This
 * doesn't apply to command encoders for render targets, just the window's
 * backbuffer. Check your return values!
 *
 * @param renderer the renderer to query.
 * @returns an `id<MTLRenderCommandEncoder>` on success, or NULL if the
 *          renderer isn't a Metal renderer or there was an error.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetRenderMetalLayer
 */
inline void* GetRenderMetalCommandEncoder(RendererRef renderer)
{
  return SDL_GetRenderMetalCommandEncoder(renderer.get());
}

/**
 * Add a set of synchronization semaphores for the current frame.
 *
 * The Vulkan renderer will wait for `wait_semaphore` before submitting
 * rendering commands and signal `signal_semaphore` after rendering commands
 * are complete for this frame.
 *
 * This should be called each frame that you want semaphore synchronization.
 * The Vulkan renderer may have multiple frames in flight on the GPU, so you
 * should have multiple semaphores that are used for synchronization. Querying
 * SDL_PROP_RENDERER_VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER will give you the
 * maximum number of semaphores you'll need.
 *
 * @param renderer the rendering context.
 * @param wait_stage_mask the VkPipelineStageFlags for the wait.
 * @param wait_semaphore a VkSempahore to wait on before rendering the current
 *                       frame, or 0 if not needed.
 * @param signal_semaphore a VkSempahore that SDL will signal when rendering
 *                         for the current frame is complete, or 0 if not
 *                         needed.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is **NOT** safe to call this function from two threads at
 *               once.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool AddVulkanRenderSemaphores(RendererRef renderer,
                                      Uint32 wait_stage_mask,
                                      Sint64 wait_semaphore,
                                      Sint64 signal_semaphore)
{
  return SDL_AddVulkanRenderSemaphores(
    renderer.get(), wait_stage_mask, wait_semaphore, signal_semaphore);
}

/**
 * Load a BMP texture from a seekable SDL data stream.
 *
 * @param renderer the renderer to create texture
 * @param src the data stream for the surface.
 * @param closeio if true, calls SDL_CloseIO() on `src` before returning, even
 *                in the case of an error.
 * @returns a Texture with loaded content or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 */
inline Texture LoadTextureBMP(RendererRef renderer,
                              SDL_IOStream* src,
                              bool closeio)
{
  Surface surface{LoadBMP(src, closeio)};
  return Texture(renderer, surface);
}

/**
 * Load a BMP texture from a file.
 *
 * @param renderer the renderer to create texture
 * @param file the BMP file to load.
 * @returns a Texture with loaded content or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Texture LoadTextureBMP(RendererRef renderer, StringParam file)
{
  Surface surface{SDL_LoadBMP(file)};
  return Texture(renderer, surface);
}

#pragma region impl
/// @}

template<ObjectBox<SDL_Window*> T>
inline RendererRef WindowBase<T>::GetRenderer() const
{
  return {SDL_GetRenderer(T::get())};
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::SetTarget(TextureRef texture)
{
  return SDL_SetRenderTarget(T::get(), texture);
}

template<ObjectBox<SDL_Renderer*> T>
TextureRef RendererBase<T>::GetTarget() const
{
  return SDL_GetRenderTarget(T::get());
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTexture(TextureRef texture,
                                    OptionalRef<const SDL_FRect> srcrect,
                                    OptionalRef<const SDL_FRect> dstrect)
{
  return SDL_RenderTexture(T::get(), texture, srcrect, dstrect);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTextureRotated(TextureRef texture,
                                           OptionalRef<const SDL_FRect> srcrect,
                                           OptionalRef<const SDL_FRect> dstrect,
                                           double angle,
                                           OptionalRef<const SDL_FPoint> center,
                                           FlipMode flip)
{
  return SDL_RenderTextureRotated(
    T::get(), texture, srcrect, dstrect, angle, center, flip);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTextureAffine(TextureRef texture,
                                          OptionalRef<const SDL_FRect> srcrect,
                                          OptionalRef<const SDL_FPoint> origin,
                                          OptionalRef<const SDL_FPoint> right,
                                          OptionalRef<const SDL_FPoint> down)
{
  return SDL_RenderTextureAffine(
    T::get(), texture, srcrect, origin, right, down);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTextureTiled(TextureRef texture,
                                         OptionalRef<const SDL_FRect> srcrect,
                                         float scale,
                                         OptionalRef<const SDL_FRect> dstrect)
{
  return SDL_RenderTextureTiled(T::get(), texture, srcrect, scale, dstrect);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderTexture9Grid(TextureRef texture,
                                         OptionalRef<const SDL_FRect> srcrect,
                                         float left_width,
                                         float right_width,
                                         float top_height,
                                         float bottom_height,
                                         float scale,
                                         OptionalRef<const SDL_FRect> dstrect)
{
  return SDL_RenderTexture9Grid(T::get(),
                                texture,
                                srcrect,
                                left_width,
                                right_width,
                                top_height,
                                bottom_height,
                                scale,
                                dstrect);
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderGeometry(TextureRef texture,
                                     std::span<const Vertex> vertices,
                                     std::span<const int> indices)
{
  if (indices.empty()) {
    return SDL_RenderGeometry(
      T::get(), texture, vertices.data(), vertices.size(), nullptr, 0);
  }
  return SDL_RenderGeometry(T::get(),
                            texture,
                            vertices.data(),
                            vertices.size(),
                            indices.data(),
                            indices.size());
}

template<ObjectBox<SDL_Renderer*> T>
bool RendererBase<T>::RenderGeometryRaw(TextureRef texture,
                                        const float* xy,
                                        int xy_stride,
                                        const FColor* color,
                                        int color_stride,
                                        const float* uv,
                                        int uv_stride,
                                        int num_vertices,
                                        const void* indices,
                                        int num_indices,
                                        int size_indices)
{
  return SDL_RenderGeometryRaw(T::get(),
                               texture,
                               xy,
                               xy_stride,
                               color,
                               color_stride,
                               uv,
                               uv_stride,
                               num_vertices,
                               indices,
                               num_indices,
                               size_indices);
}

inline void ObjectDeleter<SDL_Renderer>::operator()(RendererRef renderer) const
{
  renderer.Destroy();
}

inline void ObjectDeleter<SDL_Texture>::operator()(TextureRef texture) const
{
  texture.Destroy();
}

template<ObjectBox<SDL_Texture*> T>
TextureLock TextureBase<T>::Lock(OptionalRef<const SDL_Rect> rect) &
{
  return TextureLock{*this, rect};
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_RENDER_H_ */


// end --- SDL3pp_render.h --- 


//
//
//
//
//
//

// begin --- SDL3pp_timer.h --- 

#ifndef SDL3PP_TIMER_H_
#define SDL3PP_TIMER_H_

#include <chrono>
#include <SDL3/SDL_timer.h>

namespace SDL {

/**
 * @defgroup CategoryTimer Timer Support
 *
 * SDL provides time management functionality. It is useful for dealing with
 * (usually) small durations of time.
 *
 * This is not to be confused with _calendar time_ management, which is
 * provided by [CategoryTime](#CategoryTime).
 *
 * This category covers measuring time elapsed (GetTicks(),
 * GetPerformanceCounter()), putting a thread to sleep for a certain
 * amount of time (SDL_Delay(), SDL_DelayNS(), SDL_DelayPrecise()), and firing
 * a callback function after a certain amount of time has elapsed
 * (AddTimer(), etc).
 *
 * @{
 */

/**
 * Get the number of milliseconds since SDL library initialization.
 *
 * @returns an unsigned 64-bit value representing the number of milliseconds
 *          since the SDL library initialized.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint64 GetTicks() { return SDL_GetTicks(); }

/**
 * Get the number of nanoseconds since SDL library initialization.
 *
 * @returns an unsigned 64-bit value representing the number of nanoseconds
 *          since the SDL library initialized.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint64 GetTicksNS() { return SDL_GetTicksNS(); }

/**
 * Get the current value of the high resolution counter.
 *
 * This function is typically used for profiling.
 *
 * The counter values are only meaningful relative to each other. Differences
 * between values can be converted to times by using
 * SDL_GetPerformanceFrequency().
 *
 * @returns the current counter value.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPerformanceFrequency()
 */
inline Uint64 GetPerformanceCounter() { return SDL_GetPerformanceCounter(); }

/**
 * Get the count per second of the high resolution counter.
 *
 * @returns a platform-specific count per second.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPerformanceCounter()
 */
inline Uint64 GetPerformanceFrequency()
{
  return SDL_GetPerformanceFrequency();
}

/**
 * Wait a specified number of milliseconds before returning.
 *
 * This function waits a specified number of milliseconds before returning. It
 * waits at least the specified time, but possibly longer due to OS
 * scheduling.
 *
 * @param ms the number of milliseconds to delay.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 */
inline void Delay(Uint32 ms) { SDL_Delay(ms); }

/**
 * Wait a specified duration before returning.
 *
 * This function waits a specified duration before returning. It
 * waits at least the specified time, but possibly longer due to OS
 * scheduling.
 *
 * @param duration the duration to delay, with max precision in ns.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 */
inline void Delay(std::chrono::nanoseconds duration)
{
  SDL_DelayNS(std::max(duration.count(), 1l));
}

/**
 * @brief Wait a specified number of nanoseconds before returning.
 *
 * This function waits a specified number of nanoseconds before returning. It
 * will attempt to wait as close to the requested time as possible, busy
 * waiting if necessary, but could return later due to OS scheduling.
 *
 * @param ns the number of nanoseconds to delay.
 *
 * @threadsafety It is safe to call this function from any thread.
 */
inline void DelayNS(Uint64 ns) { SDL_DelayNS(ns); }

/**
 * Wait a specified number of nanoseconds before returning.
 *
 * This function waits a specified number of nanoseconds before returning. It
 * will attempt to wait as close to the requested time as possible, busy
 * waiting if necessary, but could return later due to OS scheduling.
 *
 * @param ns the number of nanoseconds to delay.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Delay()
 * @sa DelayNS()
 */
inline void DelayPrecise(Uint64 ns) { return SDL_DelayPrecise(ns); }

/**
 * Wait a specified duration before returning.
 *
 * This function waits a specified duration before returning. It
 * will attempt to wait as close to the requested time as possible, busy
 * waiting if necessary, but could return later due to OS scheduling.
 *
 * @param duration the duration to delay.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Delay()
 * @sa DelayNS()
 */
inline void DelayPrecise(std::chrono::nanoseconds duration)
{
  SDL_DelayPrecise(std::max(duration.count(), 1l));
}
/**
 * Definition of the timer ID type.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using TimerID = SDL_TimerID;

/**
 * Function prototype for the millisecond timer callback function.
 *
 * The callback function is passed the current timer interval and returns the
 * next timer interval, in milliseconds. If the returned value is the same as
 * the one passed in, the periodic alarm continues, otherwise a new alarm is
 * scheduled. If the callback returns 0, the periodic alarm is canceled and
 * will be removed.
 *
 * @param userdata an arbitrary pointer provided by the app through
 *                 SDL_AddTimer, for its own use.
 * @param timerID the current timer being processed.
 * @param interval the current callback time interval.
 * @returns the new callback time interval, or 0 to disable further runs of
 *          the callback.
 *
 * @threadsafety SDL may call this callback at any time from a background
 *               thread; the application is responsible for locking resources
 *               the callback touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AddTimer()
 */
using TimerCallback = SDL_TimerCallback;

/**
 * Call a callback function at a future time.
 *
 * The callback function is passed the current timer interval and the user
 * supplied parameter from the AddTimer() call and should return the next
 * timer interval. If the value returned from the callback is 0, the timer is
 * canceled and will be removed.
 *
 * The callback is run on a separate thread, and for short timeouts can
 * potentially be called before this function returns.
 *
 * Timers take into account the amount of time it took to execute the
 * callback. For example, if the callback took 250 ms to execute and returned
 * 1000 (ms), the timer would only wait another 750 ms before its next
 * iteration.
 *
 * Timing may be inexact due to OS scheduling. Be sure to note the current
 * time with GetTicksNS() or GetPerformanceCounter() in case your
 * callback needs to adjust for variances.
 *
 * @param interval the timer delay, in milliseconds, passed to `callback`.
 * @param callback the TimerCallback function to call when the specified
 *                 `interval` elapses.
 * @param userdata a pointer that is passed to `callback`.
 * @returns a timer ID or 0 on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddTimerNS()
 * @sa RemoveTimer()
 */
inline TimerID AddTimer(Uint32 interval, TimerCallback callback, void* userdata)
{
  return SDL_AddTimer(interval, callback, userdata);
}

/**
 * Function prototype for the nanosecond timer callback function.
 *
 * The callback function is passed the current timer interval and returns the
 * next timer interval, in nanoseconds. If the returned value is the same as
 * the one passed in, the periodic alarm continues, otherwise a new alarm is
 * scheduled. If the callback returns 0, the periodic alarm is canceled and
 * will be removed.
 *
 * @param userdata an arbitrary pointer provided by the app through
 *                 AddTimer(), for its own use.
 * @param timerID the current timer being processed.
 * @param interval the current callback time interval.
 * @returns the new callback time interval, or 0 to disable further runs of
 *          the callback.
 *
 * @threadsafety SDL may call this callback at any time from a background
 *               thread; the application is responsible for locking resources
 *               the callback touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AddTimerNS()
 */
using NSTimerCallback = SDL_NSTimerCallback;

/**
 * Call a callback function at a future time.
 *
 * The callback function is passed the current timer interval and the user
 * supplied parameter from the AddTimerNS() call and should return the
 * next timer interval. If the value returned from the callback is 0, the
 * timer is canceled and will be removed.
 *
 * The callback is run on a separate thread, and for short timeouts can
 * potentially be called before this function returns.
 *
 * Timers take into account the amount of time it took to execute the
 * callback. For example, if the callback took 250 ns to execute and returned
 * 1000 (ns), the timer would only wait another 750 ns before its next
 * iteration.
 *
 * Timing may be inexact due to OS scheduling. Be sure to note the current
 * time with GetTicksNS() or GetPerformanceCounter() in case your
 * callback needs to adjust for variances.
 *
 * @param interval the timer delay, in nanoseconds, passed to `callback`.
 * @param callback the NSTimerCallback function to call when the specified
 *                 `interval` elapses.
 * @param userdata a pointer that is passed to `callback`.
 * @returns a timer ID or 0 on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddTimer()
 * @sa RemoveTimer()
 */
inline TimerID AddTimerNS(Uint64 interval,
                          NSTimerCallback callback,
                          void* userdata)
{
  return SDL_AddTimerNS(interval, callback, userdata);
}

/**
 * Remove a timer created with AddTimer().
 *
 * @param id the ID of the timer to remove.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddTimer()
 */
inline bool RemoveTimer(TimerID id) { return SDL_RemoveTimer(id); }

// TODO TimerID

/// @}

} // namespace SDL

#endif /* SDL3PP_TIMER_H_ */


// end --- SDL3pp_timer.h --- 


//
//

// begin --- SDL3pp_version.h --- 

#ifndef SDL3PP_VERSION_H_
#define SDL3PP_VERSION_H_

#include <SDL3/SDL_version.h>

namespace SDL {

/**
 * @defgroup CategoryVersion Querying SDL Version
 *
 * Functionality to query the current SDL version, both as headers the app was
 * compiled against, and a library the app is linked to.
 *
 * @{
 */

/**
 * Get the version of SDL that is linked against your program.
 *
 * If you are linking to SDL dynamically, then it is possible that the current
 * version will be different than the version you compiled against. This
 * function returns the current version, while SDL_VERSION is the version you
 * compiled with.
 *
 * This function may be called safely at any time, even before SDL_Init().
 *
 * @returns the version of the linked library.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetRevision
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
 * @sa SDL_GetVersion
 */
inline const char* GetRevision() { return SDL_GetRevision(); }

/// @}

} // namespace SDL

#endif /* SDL3PP_VERSION_H_ */


// end --- SDL3pp_version.h --- 



// Here we have additional "satellite" libraries

// begin --- SDL3pp_image.h --- 

#ifndef SDL3PP_IMAGE_H_
#define SDL3PP_IMAGE_H_

#if __has_include(<SDL3_image/SDL_image.h>) || defined(SDL3PP_DOC)

#include <SDL3_image/SDL_image.h>
#define SDL3PP_HAS_IMAGE

namespace SDL::IMG {

/**
 * @defgroup CategorySDLImage Load images from several formats
 *
 * Header file for SDL_image library
 *
 * A simple library to load images of various formats as SDL surfaces
 * @{
 */

/**
 * This function gets the version of the dynamically linked SDL_image library.
 *
 * @returns SDL_image version.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline int Version() { return IMG_Version(); }

/**
 * Load an image from an SDL data source into a software surface.
 *
 * An SDL_Surface is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new SDL_Surface data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an SDL_Surface for
 * its specifics, and use SDL_ConvertSurface to then migrate to any supported
 * format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: SurfaceBase::SetColorKey(image, SDL_RLEACCEL,
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
 * with SDL_IOStream: `IMG_Load("filename.jpg")` will call this function and
 * manage those details for you, determining the file type from the filename's
 * extension.
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an Texture for use by the GPU without using a
 * software surface: call LoadTexture() instead.
 *
 * @param src an SDL_IOStream that data will be read from.
 * @param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Surface Load(SDL_IOStream* src, bool closeio, StringParam type)
{
  return Surface{IMG_LoadTyped_IO(src, closeio, type)};
}

/**
 * Load an image from a filesystem path into a software surface.
 *
 * An SDL_Surface is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new SDL_Surface data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an SurfaceBase for
 * its specifics, and use SurfaceBase::Convert() to then migrate to any
 * supported format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: SurfaceBase::SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * There is a separate function to read files from an SDL_IOStream, if you
 * need an i/o abstraction to provide data from anywhere instead of a simple
 * filesystem read; that function is Load().
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
inline Surface Load(StringParam file) { return Surface{IMG_Load(file)}; }

/**
 * Load an image from an SDL data source into a software surface.
 *
 * An SDL_Surface is a buffer of pixels in memory accessible by the CPU. Use
 * this if you plan to hand the data to something else or manipulate it
 * further in code.
 *
 * There are no guarantees about what format the new SDL_Surface data will be;
 * in many cases, SDL_image will attempt to supply a surface that exactly
 * matches the provided image, but in others it might have to convert (either
 * because the image is in a format that SDL doesn't directly support or
 * because it's compressed data that could reasonably uncompress to various
 * formats and SDL_image had to pick one). You can inspect an SurfaceBase for
 * its specifics, and use SurfaceBase::Convert() to then migrate to any
 * supported format.
 *
 * If the image format supports a transparent pixel, SDL will set the colorkey
 * for the surface. You can enable RLE acceleration on the surface afterwards
 * by calling: SurfaceBase::SetColorKey(image, SDL_RLEACCEL,
 * image->format->colorkey);
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * There is a separate function to read files from disk without having to deal
 * with SDL_IOStream: `Load("filename.jpg")` will call this function and
 * manage those details for you, determining the file type from the filename's
 * extension.
 *
 * There is also IMG_LoadTyped_IO(), which is equivalent to this function
 * except a file extension (like "BMP", "JPG", etc) can be specified, in case
 * SDL_image cannot autodetect the file format.
 *
 * If you are using SDL's 2D rendering API, there is an equivalent call to
 * load images directly into an Texture for use by the GPU without using a
 * software surface: call LoadTexture() instead.
 *
 * @param src an SDL_IOStream that data will be read from.
 * @param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Surface Load(SDL_IOStream* src, bool closeio)
{
  return Surface{IMG_Load_IO(src, closeio)};
}

/**
 * Load an image from a filesystem path into a GPU texture.
 *
 * A Texture represents an image in GPU memory, usable by SDL's 2D Render
 * API. This can be significantly more efficient than using a CPU-bound
 * SDL_Surface if you don't need to manipulate the image directly after
 * loading it.
 *
 * If the loaded image has transparency or a colorkey, a texture with an alpha
 * channel will be created. Otherwise, SDL_image will attempt to create an
 * SDL_Texture in the most format that most reasonably represents the image
 * data (but in many cases, this will just end up being 32-bit RGB or 32-bit
 * RGBA).
 *
 * If you would rather decode an image to an Surface (a buffer of pixels in CPU
 * memory), call Load() instead.
 *
 * @param renderer the SDL_Renderer to use to create the GPU texture.
 * @param file a path on the filesystem to load an image from.
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Texture LoadTexture(RendererRef renderer, StringParam file)
{
  return Texture{IMG_LoadTexture(renderer.get(), file)};
}

/**
 * Load an image from an SDL data source into a GPU texture.
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
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * There is a separate function to read files from disk without having to deal
 * with SDL_IOStream: `LoadTexture(renderer, "filename.jpg")` will call
 * this function and manage those details for you, determining the file type
 * from the filename's extension.
 *
 * If you would rather decode an image to an Surface (a buffer of pixels
 * in CPU memory), call Load() instead.
 *
 * @param renderer the SDL_Renderer to use to create the GPU texture.
 * @param src an SDL_IOStream that data will be read from.
 * @param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Texture LoadTexture(RendererRef renderer,
                           SDL_IOStream* src,
                           bool closeio)
{
  return Texture{IMG_LoadTexture_IO(renderer.get(), src, closeio)};
}

/**
 * Load an image from an SDL data source into a GPU texture.
 *
 * A Texture represents an image in GPU memory, usable by SDL's 2D Render API.
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
 * Even though this function accepts a file type, SDL_image may still try
 * other decoders that are capable of detecting file type from the contents of
 * the image data, but may rely on the caller-provided type string for formats
 * that it cannot autodetect. If `type` is nullptr, SDL_image will rely solely
 * on its ability to guess the format.
 *
 * There is a separate function to read files from disk without having to deal
 * with SDL_IOStream: `IMG_LoadTexture("filename.jpg")` will call this
 * function and manage those details for you, determining the file type from
 * the filename's extension.
 *
 * If you would rather decode an image to an SDL_Surface (a buffer of pixels
 * in CPU memory), call IMG_Load() instead.
 *
 * @param renderer the SDL_Renderer to use to create the GPU texture.
 * @param src an SDL_IOStream that data will be read from.
 * @param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * @param type a filename extension that represent this data ("BMP", "GIF",
 *             "PNG", etc).
 * @returns a new texture, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Texture LoadTexture(RendererRef renderer,
                           SDL_IOStream* src,
                           bool closeio,
                           StringParam type)
{
  return Texture{IMG_LoadTextureTyped_IO(renderer.get(), src, closeio, type)};
}

/**
 * Detect AVIF image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isAVIF(SDL_IOStream* src) { return IMG_isAVIF(src); }

/**
 * Detect ICO image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isICO(SDL_IOStream* src) { return IMG_isICO(src); }

/**
 * Detect CUR image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isCUR(SDL_IOStream* src) { return IMG_isCUR(src); }

/**
 * Detect BMP image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isBMP(SDL_IOStream* src) { return IMG_isBMP(src); }

/**
 * Detect GIF image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isGIF(SDL_IOStream* src) { return IMG_isGIF(src); }

/**
 * Detect JPG image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isJPG(SDL_IOStream* src) { return IMG_isJPG(src); }

/**
 * Detect JXL image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isJXL(SDL_IOStream* src) { return IMG_isJXL(src); }

/**
 * Detect LBM image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isLBM(SDL_IOStream* src) { return IMG_isLBM(src); }

/**
 * Detect PCX image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isPCX(SDL_IOStream* src) { return IMG_isPCX(src); }

/**
 * Detect PNG image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isPNG(SDL_IOStream* src) { return IMG_isPNG(src); }

/**
 * Detect PNM image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isPNM(SDL_IOStream* src) { return IMG_isPNM(src); }

/**
 * Detect SVG image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isSVG(SDL_IOStream* src) { return IMG_isSVG(src); }

/**
 * Detect QOI image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isQOI(SDL_IOStream* src) { return IMG_isQOI(src); }

/**
 * Detect TIFF image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isTIF(SDL_IOStream* src) { return IMG_isTIF(src); }

/**
 * Detect XCF image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isXCF(SDL_IOStream* src) { return IMG_isXCF(src); }

/**
 * Detect XPM image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isXPM(SDL_IOStream* src) { return IMG_isXPM(src); }

/**
 * Detect XV image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isXV(SDL_IOStream* src) { return IMG_isXV(src); }

/**
 * Detect WEBP image data on a readable/seekable SDL_IOStream.
 *
 * This function attempts to determine if a file is a given filetype, reading
 * the least amount possible from the SDL_IOStream (usually a few bytes).
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
 * @param src a seekable/readable SDL_IOStream to provide image data.
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
inline bool isWEBP(SDL_IOStream* src) { return IMG_isWEBP(src); }

/**
 * Load a AVIF image directly.
 *
 * If you know you definitely have a AVIF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadAVIF(SDL_IOStream* src)
{
  return Surface{IMG_LoadAVIF_IO(src)};
}

/**
 * Load a ICO image directly.
 *
 * If you know you definitely have a ICO image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadICO(SDL_IOStream* src)
{
  return Surface{IMG_LoadICO_IO(src)};
}

/**
 * Load a CUR image directly.
 *
 * If you know you definitely have a CUR image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadCUR(SDL_IOStream* src)
{
  return Surface{IMG_LoadCUR_IO(src)};
}

/**
 * Load a BMP image directly.
 *
 * If you know you definitely have a BMP image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadBMP(SDL_IOStream* src)
{
  return Surface{IMG_LoadBMP_IO(src)};
}

/**
 * Load a GIF image directly.
 *
 * If you know you definitely have a GIF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadGIF(SDL_IOStream* src)
{
  return Surface{IMG_LoadGIF_IO(src)};
}

/**
 * Load a JPG image directly.
 *
 * If you know you definitely have a JPG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadJPG(SDL_IOStream* src)
{
  return Surface{IMG_LoadJPG_IO(src)};
}

/**
 * Load a JXL image directly.
 *
 * If you know you definitely have a JXL image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadJXL(SDL_IOStream* src)
{
  return Surface{IMG_LoadJXL_IO(src)};
}

/**
 * Load a LBM image directly.
 *
 * If you know you definitely have a LBM image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadLBM(SDL_IOStream* src)
{
  return Surface{IMG_LoadLBM_IO(src)};
}

/**
 * Load a PCX image directly.
 *
 * If you know you definitely have a PCX image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadPCX(SDL_IOStream* src)
{
  return Surface{IMG_LoadPCX_IO(src)};
}

/**
 * Load a PNG image directly.
 *
 * If you know you definitely have a PNG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadPNG(SDL_IOStream* src)
{
  return Surface{IMG_LoadPNG_IO(src)};
}

/**
 * Load a PNM image directly.
 *
 * If you know you definitely have a PNM image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadPNM(SDL_IOStream* src)
{
  return Surface{IMG_LoadPNM_IO(src)};
}

/**
 * Load a SVG image directly.
 *
 * If you know you definitely have a SVG image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadSVG(SDL_IOStream* src)
{
  return Surface{IMG_LoadSVG_IO(src)};
}

/**
 * Load a QOI image directly.
 *
 * If you know you definitely have a QOI image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadQOI(SDL_IOStream* src)
{
  return Surface{IMG_LoadQOI_IO(src)};
}

/**
 * Load a TGA image directly.
 *
 * If you know you definitely have a TGA image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadTGA(SDL_IOStream* src)
{
  return Surface{IMG_LoadTGA_IO(src)};
}

/**
 * Load a TIFF image directly.
 *
 * If you know you definitely have a TIFF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadTIF(SDL_IOStream* src)
{
  return Surface{IMG_LoadTIF_IO(src)};
}

/**
 * Load a XCF image directly.
 *
 * If you know you definitely have a XCF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXPM()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadXCF(SDL_IOStream* src)
{
  return Surface{IMG_LoadXCF_IO(src)};
}

/**
 * Load a XPM image directly.
 *
 * If you know you definitely have a XPM image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXV()
 * @sa LoadWEBP()
 */
inline Surface LoadXPM(SDL_IOStream* src)
{
  return Surface{IMG_LoadXPM_IO(src)};
}

/**
 * Load a XV image directly.
 *
 * If you know you definitely have a XV image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadWEBP()
 */
inline Surface LoadXV(SDL_IOStream* src) { return Surface{IMG_LoadXV_IO(src)}; }

/**
 * Load a WEBP image directly.
 *
 * If you know you definitely have a WEBP image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream to load image data from.
 * @returns SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAVIF()
 * @sa LoadICO()
 * @sa LoadCUR()
 * @sa LoadBMP()
 * @sa LoadGIF()
 * @sa LoadJPG()
 * @sa LoadJXL()
 * @sa LoadLBM()
 * @sa LoadPCX()
 * @sa LoadPNG()
 * @sa LoadPNM()
 * @sa LoadSVG()
 * @sa LoadQOI()
 * @sa LoadTGA()
 * @sa LoadTIF()
 * @sa LoadXCF()
 * @sa LoadXPM()
 * @sa LoadXV()
 */
inline Surface LoadWEBP(SDL_IOStream* src)
{
  return Surface{IMG_LoadWEBP_IO(src)};
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
 * call to SDL_DestroySurface().
 *
 * @param src an SDL_IOStream to load SVG data from.
 * @param width desired width of the generated surface, in pixels.
 * @param height desired height of the generated surface, in pixels.
 * @returns a new SDL surface, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 */
inline Surface LoadSizedSVG(SDL_IOStream* src, int width, int height)
{
  return Surface{IMG_LoadSizedSVG_IO(src, width, height)};
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
 * Save an SDL_Surface into a AVIF image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @param quality the desired quality, ranging between 0 (lowest) and 100
 *                (highest).
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SaveAVIF_IO
 */
inline bool SaveAVIF(SurfaceRef surface, StringParam file, int quality)
{
  return IMG_SaveAVIF(surface.get(), file, quality);
}

/**
 * Save an SDL_Surface into AVIF image data, via an SDL_IOStream.
 *
 * If you just want to save to a filename, you can use IMG_SaveAVIF() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the SDL_IOStream to save the image data to.
 * @param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * @param quality the desired quality, ranging between 0 (lowest) and 100
 *                (highest).
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SaveAVIF
 */
inline bool SaveAVIF(SurfaceRef surface,
                     SDL_IOStream* dst,
                     bool closeio,
                     int quality)
{
  return IMG_SaveAVIF_IO(surface.get(), dst, closeio, quality);
}

/**
 * Save an SDL_Surface into a PNG image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SavePNG_IO
 */
inline bool SavePNG(SurfaceRef surface, StringParam file)
{
  return IMG_SavePNG(surface.get(), file);
}

/**
 * Save an SDL_Surface into PNG image data, via an SDL_IOStream.
 *
 * If you just want to save to a filename, you can use IMG_SavePNG() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the SDL_IOStream to save the image data to.
 * @param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SavePNG
 */
inline bool SavePNG(SurfaceRef surface, SDL_IOStream* dst, bool closeio)
{
  return IMG_SavePNG_IO(surface.get(), dst, closeio);
}

/**
 * Save an SDL_Surface into a JPEG image file.
 *
 * If the file already exists, it will be overwritten.
 *
 * @param surface the SDL surface to save.
 * @param file path on the filesystem to write new file to.
 * @param quality [0; 33] is Lowest quality, [34; 66] is Middle quality, [67;
 *                100] is Highest quality.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SaveJPG_IO
 */
inline bool SaveJPG(SurfaceRef surface, StringParam file, int quality)
{
  return IMG_SaveJPG(surface.get(), file, quality);
}

/**
 * Save an SDL_Surface into JPEG image data, via an SDL_IOStream.
 *
 * If you just want to save to a filename, you can use IMG_SaveJPG() instead.
 *
 * If `closeio` is true, `dst` will be closed before returning, whether this
 * function succeeds or not.
 *
 * @param surface the SDL surface to save.
 * @param dst the SDL_IOStream to save the image data to.
 * @param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * @param quality [0; 33] is Lowest quality, [34; 66] is Middle quality, [67;
 *                100] is Highest quality.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa SaveJPG
 */
inline bool SaveJPG(SurfaceRef surface,
                    SDL_IOStream* dst,
                    bool closeio,
                    int quality)
{
  return IMG_SaveJPG_IO(surface.get(), dst, closeio, quality);
}

/**
 * Animated image support
 *
 * Currently only animated GIFs and WEBP images are supported.
 */
using Animation = IMG_Animation;

/**
 * Load an animation from a file.
 *
 * When done with the returned animation, the app should dispose of it with a
 * call to IMG_FreeAnimation().
 *
 * @param file path on the filesystem containing an animated image.
 * @returns a new IMG_Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa FreeAnimation
 */
inline Animation* LoadAnimation(StringParam file)
{
  return IMG_LoadAnimation(file);
}

/**
 * Load an animation from an SDL_IOStream.
 *
 * If `closeio` is true, `src` will be closed before returning, whether this
 * function succeeds or not. SDL_image reads everything it needs from `src`
 * during this call in any case.
 *
 * When done with the returned animation, the app should dispose of it with a
 * call to IMG_FreeAnimation().
 *
 * @param src an SDL_IOStream that data will be read from.
 * @param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * @returns a new IMG_Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa FreeAnimation
 */
inline Animation* LoadAnimation_IO(SDL_IOStream* src, bool closeio)
{
  return IMG_LoadAnimation_IO(src, closeio);
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
 * call to IMG_FreeAnimation().
 *
 * @param src an SDL_IOStream that data will be read from.
 * @param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * @param type a filename extension that represent this data ("GIF", etc).
 * @returns a new IMG_Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAnimation
 * @sa LoadAnimation()
 * @sa FreeAnimation
 */
inline Animation* LoadAnimationTyped_IO(SDL_IOStream* src,
                                        bool closeio,
                                        StringParam type)
{
  return IMG_LoadAnimationTyped_IO(src, closeio, type);
}

/**
 * Dispose of an IMG_Animation and free its resources.
 *
 * The provided `anim` pointer is not valid once this call returns.
 *
 * @param anim IMG_Animation to dispose of.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAnimation
 * @sa LoadAnimation()
 * @sa LoadAnimationTyped()
 */
inline void FreeAnimation(Animation* anim) { return IMG_FreeAnimation(anim); }

/**
 * Load a GIF animation directly.
 *
 * If you know you definitely have a GIF image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream that data will be read from.
 * @returns a new IMG_Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAnimation
 * @sa LoadAnimation()
 * @sa LoadAnimationTyped()
 * @sa FreeAnimation
 */
inline Animation* LoadGIFAnimation_IO(SDL_IOStream* src)
{
  return IMG_LoadGIFAnimation_IO(src);
}

/**
 * Load a WEBP animation directly.
 *
 * If you know you definitely have a WEBP image, you can call this function,
 * which will skip SDL_image's file format detection routines. Generally it's
 * better to use the abstract interfaces; also, there is only an SDL_IOStream
 * interface available here.
 *
 * @param src an SDL_IOStream that data will be read from.
 * @returns a new IMG_Animation, or nullptr on error.
 *
 * @since This function is available since SDL_image 3.0.0.
 *
 * @sa LoadAnimation
 * @sa LoadAnimation()
 * @sa LoadAnimationTyped()
 * @sa FreeAnimation
 */
inline Animation* LoadWEBPAnimation_IO(SDL_IOStream* src)
{
  return IMG_LoadWEBPAnimation_IO(src);
}

/// @}
} // namespace SDL::IMG

#endif // __has_include(<SDL3/SDL_image.h>)

#endif /* SDL3PP_IMAGE_H_ */


// end --- SDL3pp_image.h --- 



#endif /* SDL3PP_H_ */


// end --- SDL3pp.h --- 

