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

// TODO category

/**
 * Base class for pointer wrap
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
 * Handle to a non owned pointer
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
 * Handle to an owned pointer
 *
 * @cat resource
 *
 * @sa resource
 * @sa PtrBase
 * @sa RefPtr
 */
template<class T>
using OwnPtr = PtrBase<ObjectUnique<T, PtrDeleter<T>>>;

} // namespace SDL
#endif /* SDL3PP_OWN_PTR_H_ */

#include "SDL3pp_stdinc.h"

namespace SDL::details {

template<class T>
void PtrCommon<T>::free()
{
  SDL::free(T::release());
}

} // namespace SDL
