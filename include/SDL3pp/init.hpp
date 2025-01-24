#ifndef SDL3PP_INIT_HPP_
#define SDL3PP_INIT_HPP_

#include <SDL3/SDL_init.h>
#include "stringWrapper.hpp"

/// @namespace SDL
/// @brief the main namespace where all SDL3pp public functions and types live
namespace SDL {

using InitFlags = SDL_InitFlags;
using AppResult = SDL_AppResult;

/// @brief Inits SDL and/or deinit SDL
class Init
{
public:
  /// @brief Create object but don't initialize any subSystem
  Init(bool autoQuit = true);

  /// @brief Init given subsystems
  /// @param flags
  /// @param autoQuit if true automatically quits when destroyed.
  ///
  /// The autoQuit mechanism has a refCount internally, so it is safe to call
  /// this multiple times, the quit will be called only on the last call.
  /// Only the topmost object instance affects autoQuit, inner ones just ignore
  /// this flag, so you can use this to initialize subSystems as well.
  Init(InitFlags flags, bool autoQuit = true);

  // Copy ctor
  Init(const Init& rhs);

  // Move ctor
  Init(Init&& rhs);

  // Dtor
  ~Init();

  Init& operator=(Init rhs);

  /// @brief Check if given subSystems are initialized
  /// @param flags the flags to test or 0 to test all
  /// @return Which subsystem are active
  static InitFlags WasInit(InitFlags flags = 0) { return SDL_WasInit(flags); }

  /// @brief Release locking such as Reset() does, but never calls SDL_Quit() or
  /// SDL_QuitSubSystem()
  /// @return false if there are still other locks, true if this was last one
  ///
  /// When this returns true it is safe to call SDL_Quit()
  bool Release();

  /// @brief Reset the value of this instance, acts like it was destroyed and
  /// then newly instantiated
  /// @return false if there are still other locks, true if this was last one
  bool Reset();

  /// @brief returns true if active and has no errors
  operator bool() const { return active && successful; }

  bool IsQuitAtZero();

  InitFlags GetCurrentFlags() const { return flags; }

private:
  InitFlags flags = 0;
  bool active = true;
  bool successful = true;

  // Update refCount and quit if necessary
  int refCount(int delta, bool autoQuit = true, bool* autoQuitOut = nullptr);
};

inline bool
SetAppMetadata(StringWrapper appName,
               StringWrapper appVersion,
               StringWrapper appIdentifier)
{
  return SDL_SetAppMetadata(
    appName.Get(), appVersion.Get(), appIdentifier.Get());
}

inline bool
SetAppMetadataProperty(StringWrapper name, StringWrapper value)
{
  return SetAppMetadataProperty(name.Get(), value.Get());
}

inline const char*
GetAppMetadataProperty(StringWrapper name)
{
  return SDL_GetAppMetadataProperty(name.Get());
}

inline Init::Init(InitFlags flags, bool autoQuit)
  : flags(flags)
{
  refCount(+1, autoQuit);
}

// Copy ctor
inline Init::Init(const Init& rhs)
  : flags(rhs.flags)
  , active(rhs.active)
  , successful(rhs.successful)
{
  if (active) refCount(+1);
}

// Move ctor
inline Init::Init(Init&& rhs)
  : flags(rhs.flags)
  , active(rhs.active)
  , successful(rhs.successful)
{
  rhs.active = false;
}

inline Init::~Init()
{
  if (active) refCount(-1);
}

inline Init&
Init::operator=(Init rhs)
{
  std::swap(active, rhs.active);
  std::swap(flags, rhs.flags);
  std::swap(successful, rhs.successful);
  return *this;
}

inline bool
Init::Release()
{
  flags = 0;
  return refCount(-1, false) == 0;
}

inline bool
Init::Reset()
{
  return refCount(-1) == 0;
}

inline bool
Init::IsQuitAtZero()
{
  bool result;
  refCount(0, false, &result);
  return result;
}

inline int
Init::refCount(int delta, bool autoQuit, bool* autoQuitOut)
{
  // TODO Locking these?
  static int refCount = 0;
  static bool quitAtZero = false;
  if (delta && active) {
    if (delta > 0) {
      if (refCount++ == 0) quitAtZero = autoQuit;
      if (flags) successful = SDL_InitSubSystem(flags);
    } else {
      SDL_assert_always(refCount > 0);
      active = false;
      if (--refCount <= 0 && quitAtZero && autoQuit) {
        SDL_Quit();
      } else if (flags) {
        SDL_QuitSubSystem(flags);
      }
    }
  }
  if (autoQuitOut) *autoQuitOut = quitAtZero;
  return refCount;
}

} // namespace SDL

#endif // SDL3PP_INIT_HPP_
