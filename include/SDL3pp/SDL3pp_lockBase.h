#ifndef SDL3PP_LOCK_BASE_H_
#define SDL3PP_LOCK_BASE_H_

#include <SDL3/SDL_assert.h>
#include "SDL3pp_error.h"

namespace SDL {

/**
 * Base class for locks
 *
 * @tparam RESOURCE
 *
 * @see AudioLock
 * @see SurfaceLock
 */
template<class RESOURCE>
class LockBase
{
  RESOURCE m_resource;

protected:
  /// Constructs initializing member
  constexpr LockBase(RESOURCE&& resource)
    : m_resource(std::move(resource))
  {
  }

public:
  /// Default ctor
  constexpr LockBase() = default;

  LockBase(const LockBase& other) = delete;

  /// Move ctor
  LockBase(LockBase&& other)
    : LockBase(std::move(other.m_resource))
  {
  }

  /// Dtor
  constexpr ~LockBase() { SDL_assert_paranoid(!m_resource); }

  LockBase& operator=(const LockBase& other) = delete;

  /// Move assignment
  LockBase& operator=(LockBase&& other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Release locked resource without unlocking it.
  RESOURCE release() { return m_resource.release(); }
};

} // namespace SDL

#endif /* SDL3PP_LOCK_BASE_H_ */
