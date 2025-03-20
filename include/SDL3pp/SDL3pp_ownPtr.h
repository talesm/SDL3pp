#ifndef SDL3PP_OWN_PTR_H_
#define SDL3PP_OWN_PTR_H_

#include "SDL3pp_objectWrapper.h"

namespace SDL {

namespace details {

template<class T>
struct PtrCommon : T
{
  using T::T;

  void free();
};

} // namespace details

/**
 * @defgroup CategoryOwnPtr Pointer wrapper to SDL::free()
 *
 * Wraps SDL generated pointers to automatically freeing them.
 * @{
 */

/**
 * Base class for SDL memory allocated pointer wrap
 *
 * @tparam T the wrapped type
 *
 * @cat resource
 */
template<class T>
struct PtrBase : details::PtrCommon<T>
{
  using details::PtrCommon<T>::PtrCommon;
};

/**
 * Handle to a non owned SDL memory allocated pointer
 *
 * @cat resource
 *
 * @sa resource
 * @sa SurfaceBase
 * @sa Surface
 */
template<class T>
using RefPtr = PtrBase<ObjectRef<T>>;

template<class T>
struct PtrDeleter
{
  void operator()(RefPtr<T> ptr) const { ptr.free(); }
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
using OwnPtr = PtrBase<ObjectUnique<T, PtrDeleter<T>>>;

/**
 * Base class for SDL memory allocated array wrap
 *
 * @tparam T the wrapped array type, without the []
 *
 * @cat resource
 */
template<class T>
class ArrayBase : public PtrBase<T>
{
  size_t m_size = 0;

public:
  using PtrBase<T>::PtrBase;

  constexpr explicit ArrayBase(PtrBase<T>::pointer ptr, size_t size)
    : PtrBase<T>{ptr}
    , m_size(size)
  {
  }

  constexpr explicit ArrayBase(PtrBase<T>::pointer ptr)
    : PtrBase<T>{ptr}
    , m_size(0)
  {
    if (ptr) {
      auto endPtr = ptr;
      while (*endPtr) ++endPtr;
      m_size = endPtr - ptr;
    }
  }

  constexpr size_t size() const { return m_size; }
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
using RefArray = ArrayBase<ObjectRef<T[]>>;

/**
 * Handle to an owned SDL memory allocated array
 *
 * @tparam T the wrapped array type, without the []
 *
 * @cat resource
 *
 * @sa resource
 * @sa ArrayBase
 * @sa RefArray
 * @sa OwnPtr
 */
template<class T>
using OwnArray = ArrayBase<ObjectUnique<T[], PtrDeleter<T[]>>>;

/// @}

} // namespace SDL
#endif /* SDL3PP_OWN_PTR_H_ */

#include "SDL3pp_stdinc.h"
