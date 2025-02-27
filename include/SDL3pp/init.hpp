#ifndef SDL3PP_INIT_HPP_
#define SDL3PP_INIT_HPP_

#include <SDL3/SDL_init.h>
#include "callbackWrapper.hpp"
#include "stringParam.hpp"

/**
 * @namespace SDL
 * @brief the main namespace where all SDL3pp public functions and types live
 */
namespace SDL {

/**
 * @brief Initialization flags for SDL
 */
using InitFlags = SDL_InitFlags;
using AppResult = SDL_AppResult;

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
class Init
{
public:
  /**
   * @brief Init given subsystems
   * @param flags
   * @param autoQuit if true automatically quits when destroyed.
   *
   * The autoQuit mechanism has a refCount internally, so it is safe to call
   * this multiple times, the quit will be called only on the last call.
   * Only the topmost object instance affects autoQuit, inner ones just ignore
   * this flag, so you can use this to initialize subSystems as well.
   */

  Init(InitFlags flags = 0);

  // Copy ctor
  Init(const Init& rhs);

  // Move ctor
  Init(Init&& rhs);

  // Dtor
  ~Init();

  Init& operator=(Init rhs);

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
  bool release();

  /**
   * @brief reset the value of this instance, acts like it was destroyed and
   * then newly instantiated
   * @return false if there are still other locks, true if this was last one
   */
  bool reset();

  /// @brief returns true if active and has no errors
  operator bool() const { return active; }

  InitFlags GetCurrentFlags() const { return flags; }

private:
  InitFlags flags = 0;
  bool active = true;

  int refCount(int delta, bool autoQuit = true);
};

inline bool IsMainThread() { return SDL_IsMainThread(); }

inline bool RunOnMainThread(SDL_MainThreadCallback callback,
                            void* userData,
                            bool waitComplete = false)
{
  return SDL_RunOnMainThread(callback, userData, waitComplete);
}

using MainThreadCallback = void();

inline bool RunOnMainThread(std::function<MainThreadCallback> callback,
                            bool waitComplete = false)
{
  using Wrapper = CallbackWrapper<MainThreadCallback>;
  return RunOnMainThread(
    &Wrapper::CallOnce, Wrapper::Wrap(std::move(callback)), waitComplete);
}

inline bool SetAppMetadata(StringParam appName,
                           StringParam appVersion,
                           StringParam appIdentifier)
{
  return SDL_SetAppMetadata(appName, appVersion, appIdentifier);
}

inline bool SetAppMetadataProperty(StringParam name, StringParam value)
{
  return SDL_SetAppMetadataProperty(name, value);
}

inline const char* GetAppMetadataProperty(StringParam name)
{
  return SDL_GetAppMetadataProperty(name);
}

inline Init::Init(InitFlags flags)
  : flags(flags)
{
  refCount(+1, true);
}

// Copy ctor
inline Init::Init(const Init& rhs)
  : flags(rhs.flags)
  , active(rhs.active)
{
  if (active) refCount(+1);
}

// Move ctor
inline Init::Init(Init&& rhs)
  : flags(rhs.flags)
  , active(rhs.active)
{
  rhs.active = false;
}

inline Init::~Init()
{
  if (active) refCount(-1);
}

inline Init& Init::operator=(Init rhs)
{
  std::swap(active, rhs.active);
  std::swap(flags, rhs.flags);
  return *this;
}

inline bool Init::release()
{
  flags = 0;
  return refCount(-1, false) == 0;
}

inline bool Init::reset() { return refCount(-1) == 0; }

inline int Init::refCount(int delta, bool autoQuit)
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

} // namespace SDL

#endif /* SDL3PP_INIT_HPP_ */
