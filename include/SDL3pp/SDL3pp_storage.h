#ifndef SDL3PP_STORAGE_H_
#define SDL3PP_STORAGE_H_

#include <SDL3/SDL_storage.h>
#include "SDL3pp_filesystem.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 *
 * @defgroup CategoryStorage Storage Abstraction
 *
 * The storage API is a high-level API designed to abstract away the
 * portability issues that come up when using something lower-level (in SDL's
 * case, this sits on top of the [Filesystem](CategoryFilesystem) and
 * [IOStream](CategoryIOStream) subsystems). It is significantly more
 * restrictive than a typical filesystem API, for a number of reasons:
 *
 * 1. **What to Access:** A common pitfall with existing filesystem APIs is
 * the assumption that all storage is monolithic. However, many other
 * platforms (game consoles in particular) are more strict about what _type_
 * of filesystem is being accessed; for example, game content and user data
 * are usually two separate storage devices with entirely different
 * characteristics (and possibly different low-level APIs altogether!).
 *
 * 2. **How to Access:** Another common mistake is applications assuming that
 * all storage is universally writeable - again, many platforms treat game
 * content and user data as two separate storage devices, and only user data
 * is writeable while game content is read-only.
 *
 * 3. **When to Access:** The most common portability issue with filesystem
 * access is _timing_ - you cannot always assume that the storage device is
 * always accessible all of the time, nor can you assume that there are no
 * limits to how long you have access to a particular device.
 *
 * Consider the following example:
 *
 * ```c
 * void ReadGameData(void)
 * {
 *     extern char** fileNames;
 *     extern size_t numFiles;
 *     for (size_t i = 0; i < numFiles; i += 1) {
 *         FILE *data = fopen(fileNames[i], "rwb");
 *         if (data == nullptr) {
 *             // Something bad happened!
 *         } else {
 *             // A bunch of stuff happens here
 *             fclose(data);
 *         }
 *     }
 * }
 *
 * void ReadSave(void)
 * {
 *     FILE *save = fopen("saves/save0.sav", "rb");
 *     if (save == nullptr) {
 *         // Something bad happened!
 *     } else {
 *         // A bunch of stuff happens here
 *         fclose(save);
 *     }
 * }
 *
 * void WriteSave(void)
 * {
 *     FILE *save = fopen("saves/save0.sav", "wb");
 *     if (save == nullptr) {
 *         // Something bad happened!
 *     } else {
 *         // A bunch of stuff happens here
 *         fclose(save);
 *     }
 * }
 * ```
 *
 * Going over the bullet points again:
 *
 * 1. **What to Access:** This code accesses a global filesystem; game data
 * and saves are all presumed to be in the current working directory (which
 * may or may not be the game's installation folder!).
 *
 * 2. **How to Access:** This code assumes that content paths are writeable,
 * and that save data is also writeable despite being in the same location as
 * the game data.
 *
 * 3. **When to Access:** This code assumes that they can be called at any
 * time, since the filesystem is always accessible and has no limits on how
 * long the filesystem is being accessed.
 *
 * Due to these assumptions, the filesystem code is not portable and will fail
 * under these common scenarios:
 *
 * - The game is installed on a device that is read-only, both content loading
 *   and game saves will fail or crash outright
 * - Game/User storage is not implicitly mounted, so no files will be found
 *   for either scenario when a platform requires explicitly mounting
 *   filesystems
 * - Save data may not be safe since the I/O is not being flushed or
 *   validated, so an error occurring elsewhere in the program may result in
 *   missing/corrupted save data
 *
 * When using Storage, these types of problems are virtually impossible to
 * trip over:
 *
 * ```cpp
 * void ReadGameData(void)
 * {
 *     extern char** fileNames;
 *     extern size_t numFiles;
 *
 *     Storage title(nullptr, 0);
 *     if (title == nullptr) {
 *         // Something bad happened!
 *     }
 *     while (!title.Ready()) {
 *         Delay(1);
 *     }
 *
 *     for (size_t i = 0; i < numFiles; i += 1) {
 *         void* dst;
 *         Uint64 dstLen = title.GetSize(fileNames[i]);
 *
 *         if (dstLen > 0) {
 *           dst = malloc(dstLen);
 *           if (title.ReadFile(fileNames[i], dst, dstLen)) {
 *                 // A bunch of stuff happens here
 *             } else {
 *                 // Something bad happened!
 *             }
 *             free(dst);
 *         } else {
 *             // Something bad happened!
 *         }
 *     }
 * }
 *
 * void ReadSave(void)
 * {
 *     Storage user("libsdl", "Storage Example", 0);
 *     if (user == nullptr) {
 *         // Something bad happened!
 *     }
 *     while (!user.Ready()) {
 *         Delay(1);
 *     }
 *
 *     Uint64 saveLen = user.GetFileSize();
 *     if (saveLen > 0) {
 *         void* dst = malloc(saveLen);
 *         if (user.ReadFile("save0.sav", dst, saveLen)) {
 *             // A bunch of stuff happens here
 *         } else {
 *             // Something bad happened!
 *         }
 *         free(dst);
 *     } else {
 *         // Something bad happened!
 *     }
 * }
 *
 * void WriteSave(void)
 * {
 *     Storage user("libsdl", "Storage Example", 0);
 *     if (user == nullptr) {
 *         // Something bad happened!
 *     }
 *     while (!user.Ready()) {
 *         Delay(1);
 *     }
 *
 *     extern void *saveData; // A bunch of stuff happened here...
 *     extern Uint64 saveLen;
 *     if (!user.WriteFile("save0.sav", saveData, saveLen)) {
 *         // Something bad happened!
 *     }
 * }
 * ```
 *
 * Note the improvements that Storage makes:
 *
 * 1. **What to Access:** This code explicitly reads from a title or user
 * storage device based on the context of the function.
 *
 * 2. **How to Access:** This code explicitly uses either a read or write
 * function based on the context of the function.
 *
 * 3. **When to Access:** This code explicitly opens the device when it needs
 * to, and closes it when it is finished working with the filesystem.
 *
 * The result is an application that is significantly more robust against the
 * increasing demands of platforms and their filesystems!
 *
 * A publicly available example of an Storage backend is the
 * [Steam Cloud](https://partner.steamgames.com/doc/features/cloud)
 * backend - you can initialize Steamworks when starting the program, and then
 * SDL will recognize that Steamworks is initialized and automatically use
 * ISteamRemoteStorage when the application opens user storage. More
 * importantly, when you _open_ storage it knows to begin a "batch" of
 * filesystem operations, and when you _close_ storage it knows to end and
 * flush the batch. This is used by Steam to support
 * [Dynamic Cloud
 * Sync](https://steamcommunity.com/groups/steamworks/announcements/detail/3142949576401813670)
 * ; users can save data on one PC, put the device to sleep, and then continue
 * playing on another PC (and vice versa) with the save data fully
 * synchronized across all devices, allowing for a seamless experience without
 * having to do full restarts of the program.
 *
 * ## Notes on valid paths
 *
 * All paths in the Storage API use Unix-style path separators ('/'). Using a
 * different path separator will not work, even if the underlying platform
 * would otherwise accept it. This is to keep code using the Storage API
 * portable between platforms and Storage implementations and simplify app
 * code.
 *
 * Paths with relative directories ("." and "..") are forbidden by the Storage
 * API.
 *
 * All valid UTF-8 strings (discounting the nullptr terminator character and the
 * '/' path separator) are usable for filenames, however, an underlying
 * Storage implementation may not support particularly strange sequences and
 * refuse to create files with those names, etc.
 *
 * @{
 */

// Forward decl
struct StorageRef;

// Forward decl
struct Storage;

/**
 * Function interface for StorageRef.
 *
 * Apps that want to supply a custom implementation of StorageRef will fill
 * in all the functions in this struct, and then pass it to
 * StorageRef.StorageRef to create a custom StorageRef object.
 *
 * It is not usually necessary to do this; SDL provides standard
 * implementations for many things you might expect to do with an StorageRef.
 *
 * This structure should be initialized using SDL_INIT_INTERFACE()
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa SDL_INIT_INTERFACE
 */
using StorageInterface = SDL_StorageInterface;

/**
 * An abstract interface for filesystem access.
 *
 * This is an opaque datatype. One can create this object using standard SDL
 * functions like StorageRef.StorageRef or StorageRef.StorageRef, etc, or
 * create an object with a custom implementation using StorageRef.StorageRef.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Storage
 */
struct StorageRef : Resource<SDL_Storage*>
{
  using Resource::Resource;

  /**
   * Copy constructor.
   */
  constexpr StorageRef(const StorageRef& other)
    : StorageRef(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr StorageRef(StorageRef&& other)
    : StorageRef(other.release())
  {
  }

  /**
   * Opens up a read-only container for the application's filesystem.
   *
   * @param override a path to override the backend's default title root.
   * @param props a property list that may contain backend-specific information.
   * @post a title storage container on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.Close
   * @sa StorageRef.GetFileSize
   * @sa StorageRef.StorageRef
   * @sa StorageRef.ReadFile
   */
  StorageRef(StringParam override, PropertiesRef& props)
    : Resource(CheckError(SDL_OpenTitleStorage(override, props.get())))
  {
  }

  /**
   * Opens up a container for a user's unique read/write filesystem.
   *
   * While title storage can generally be kept open throughout runtime, user
   * storage should only be opened when the client is ready to read/write files.
   * This allows the backend to properly batch file operations and flush them
   * when the container has been closed; ensuring safe and optimal save I/O.
   *
   * @param org the name of your organization.
   * @param app the name of your application.
   * @param props a property list that may contain backend-specific information.
   * @post a user storage container on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.GetFileSize
   * @sa StorageRef.GetSpaceRemaining
   * @sa StorageRef.StorageRef
   * @sa StorageRef.ReadFile
   * @sa StorageRef.Ready
   * @sa StorageRef.WriteFile
   */
  StorageRef(StringParam org, StringParam app, PropertiesRef& props)
    : Resource(CheckError(SDL_OpenUserStorage(org, app, props.get())))
  {
  }

  /**
   * Opens up a container for local filesystem storage.
   *
   * This is provided for development and tools. Portable applications should
   * use StorageRef.StorageRef() for access to game data and
   * StorageRef.StorageRef() for access to user data.
   *
   * @param path the base path prepended to all storage paths, or nullptr for no
   *             base path.
   * @post a filesystem storage container on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.GetFileSize
   * @sa StorageRef.GetSpaceRemaining
   * @sa StorageRef.StorageRef
   * @sa StorageRef.StorageRef
   * @sa StorageRef.ReadFile
   * @sa StorageRef.WriteFile
   */
  StorageRef(StringParam path)
    : Resource(CheckError(SDL_OpenFileStorage(path)))
  {
  }

  /**
   * Opens up a container using a client-provided storage interface.
   *
   * Applications do not need to use this function unless they are providing
   * their own StorageRef implementation. If you just need an StorageRef, you
   * should use the built-in implementations in SDL, like
   * StorageRef.StorageRef() or StorageRef.StorageRef().
   *
   * This function makes a copy of `iface` and the caller does not need to keep
   * it around after this call.
   *
   * @param iface the interface that implements this storage, initialized using
   *              SDL_INIT_INTERFACE().
   * @param userdata the pointer that will be passed to the interface functions.
   * @post a storage container on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.GetFileSize
   * @sa StorageRef.GetSpaceRemaining
   * @sa SDL_INIT_INTERFACE
   * @sa StorageRef.ReadFile
   * @sa StorageRef.Ready
   * @sa StorageRef.WriteFile
   */
  StorageRef(const StorageInterface& iface, void* userdata)
    : Resource(CheckError(SDL_OpenStorage(&iface, userdata)))
  {
  }

  /**
   * Assignment operator.
   */
  StorageRef& operator=(StorageRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Checks if the storage container is ready to use.
   *
   * This function should be called in regular intervals until it returns true -
   * however, it is not recommended to spinwait on this call, as the backend may
   * depend on a synchronous message loop. You might instead poll this in your
   * game's main loop while processing events and drawing a loading screen.
   *
   * @returns true if the container is ready, false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Ready() const { return SDL_StorageReady(get()); }

  /**
   * Query the size of a file within a storage container.
   *
   * @param path the relative path of the file to query.
   * @returns the file's length on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.ReadFile
   * @sa StorageRef.Ready
   */
  std::optional<Uint64> GetFileSize(StringParam path) const
  {
    if (Uint64 length; SDL_GetStorageFileSize(get(), path, &length)) {
      return length;
    }
    return 0;
  }

  /**
   * Synchronously read a file from a storage container into a client-provided
   * buffer.
   *
   * The value of `length` must match the length of the file exactly; call
   * StorageRef.GetFileSize() to get this value. This behavior may be relaxed
   * in a future release.
   *
   * @param path the relative path of the file to read.
   * @returns the content if the file was read or empty string on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.GetFileSize
   * @sa StorageRef.Ready
   * @sa StorageRef.WriteFile
   */
  std::string ReadFile(StringParam path) const
  {
    auto sz = GetFileSize(path.c_str());
    if (!sz || *sz == 0) return {};
    std::string buffer(*sz, 0);
    if (ReadFile(std::move(path), buffer)) return buffer;
    return {};
  }

  /**
   * Synchronously read a file from a storage container into a client-provided
   * buffer.
   *
   * The value of `length` must match the length of the file exactly; call
   * StorageRef.GetFileSize() to get this value. This behavior may be relaxed
   * in a future release.
   *
   * @param path the relative path of the file to read.
   * @param destination a client-provided buffer to read the file into.
   * @returns true if the file was read or false on failure; call GetError()
   *          for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.GetFileSize
   * @sa StorageRef.Ready
   * @sa StorageRef.WriteFile
   */
  bool ReadFile(StringParam path, TargetBytes destination) const
  {
    return SDL_ReadStorageFile(
      get(), path, destination.data, destination.size_bytes);
  }

  /**
   * Synchronously read a file from a storage container into a client-provided
   * buffer.
   *
   * The value of `length` must match the length of the file exactly; call
   * StorageRef.GetFileSize() to get this value. This behavior may be relaxed
   * in a future release.
   *
   * @param path the relative path of the file to read.
   * @returns the content if the file was read or empty string on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.GetFileSize
   * @sa StorageRef.Ready
   * @sa StorageRef.WriteFile
   */
  template<class T>
  std::vector<T> ReadFileAs(StringParam path) const
  {
    auto sz = GetFileSize(path.c_str());
    if (!sz || *sz == 0) return {};
    std::vector<T> buffer(*sz / sizeof(T) + (*sz % sizeof(T) ? 1 : 0), 0);
    if (ReadFile(std::move(path), {buffer.data(), *sz})) return buffer;
    return {};
  }

  /**
   * Synchronously write a file from client memory into a storage container.
   *
   * @param path the relative path of the file to write.
   * @param source a client-provided buffer to write from.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.GetSpaceRemaining
   * @sa StorageRef.ReadFile
   * @sa StorageRef.Ready
   */
  void WriteFile(StringParam path, SourceBytes source)
  {
    SDL_WriteStorageFile(get(), path, source.data, source.size_bytes);
  }

  /**
   * Create a directory in a writable storage container.
   *
   * @param path the path of the directory to create.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.Ready
   */
  void CreateDirectory(StringParam path)
  {
    CheckError(SDL_CreateStorageDirectory(get(), path));
  }

  /**
   * Enumerate a directory.
   *
   * @param path the path of the directory to enumerate.
   * @returns all the directory contents.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::vector<Path> EnumerateDirectory(StringParam path)
  {
    std::vector<Path> r;
    EnumerateDirectory(std::move(path), [&](const char*, const char* fname) {
      r.emplace_back(fname);
      return ENUM_CONTINUE;
    });
    return r;
  }

  /**
   * Enumerate a directory in a storage container through a callback function.
   *
   * This function provides every directory entry through an app-provided
   * callback, called once for each directory entry, until all results have been
   * provided or the callback returns either ENUM_SUCCESS or
   * ENUM_FAILURE.
   *
   * This will return false if there was a system problem in general, or if a
   * callback returns ENUM_FAILURE. A successful return means a callback
   * returned ENUM_SUCCESS to halt enumeration, or all directory entries
   * were enumerated.
   *
   * If `path` is nullptr, this is treated as a request to enumerate the root of
   * the storage container's tree. An empty string also works for this.
   *
   * @param path the path of the directory to enumerate, or nullptr for the
   * root.
   * @param callback a function that is called for each entry in the directory.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.Ready
   */
  void EnumerateDirectory(StringParam path, EnumerateDirectoryCB callback)
  {
    EnumerateDirectory(
      std::move(path),
      [](void* userdata, const char* dirname, const char* fname) {
        auto& cb = *static_cast<EnumerateDirectoryCB*>(userdata);
        return cb(dirname, fname);
      },
      &callback);
  }

  /**
   * Enumerate a directory in a storage container through a callback function.
   *
   * This function provides every directory entry through an app-provided
   * callback, called once for each directory entry, until all results have been
   * provided or the callback returns either ENUM_SUCCESS or
   * ENUM_FAILURE.
   *
   * This will return false if there was a system problem in general, or if a
   * callback returns ENUM_FAILURE. A successful return means a callback
   * returned ENUM_SUCCESS to halt enumeration, or all directory entries
   * were enumerated.
   *
   * If `path` is nullptr, this is treated as a request to enumerate the root of
   * the storage container's tree. An empty string also works for this.
   *
   * @param path the path of the directory to enumerate, or nullptr for the
   * root.
   * @param callback a function that is called for each entry in the directory.
   * @param userdata a pointer that is passed to `callback`.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.Ready
   */
  void EnumerateDirectory(StringParam path,
                          EnumerateDirectoryCallback callback,
                          void* userdata)
  {
    CheckError(SDL_EnumerateStorageDirectory(get(), path, callback, userdata));
  }

  /**
   * Remove a file or an empty directory in a writable storage container.
   *
   * @param path the path of the directory to enumerate.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.Ready
   */
  void RemovePath(StringParam path)
  {
    CheckError(SDL_RemoveStoragePath(get(), path));
  }

  /**
   * Rename a file or directory in a writable storage container.
   *
   * @param oldpath the old path.
   * @param newpath the new path.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.Ready
   */
  void RenamePath(StringParam oldpath, StringParam newpath)
  {
    CheckError(SDL_RenameStoragePath(get(), oldpath, newpath));
  }

  /**
   * Copy a file in a writable storage container.
   *
   * @param oldpath the old path.
   * @param newpath the new path.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.Ready
   */
  void CopyFile(StringParam oldpath, StringParam newpath)
  {
    CheckError(SDL_CopyStorageFile(get(), oldpath, newpath));
  }

  /**
   * Get information about a filesystem path in a storage container.
   *
   * @param path the path to query.
   * @returns the info on success or false if the file doesn't exist, or another
   *          failure; call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.Ready
   */
  PathInfo GetPathInfo(StringParam path) const
  {
    if (PathInfo info; SDL_GetStoragePathInfo(get(), path, &info)) {
      return info;
    }
    return {};
  }

  /**
   * Queries the remaining space in a storage container.
   *
   * @returns the amount of remaining space, in bytes.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.Ready
   * @sa StorageRef.WriteFile
   */
  Uint64 GetSpaceRemaining() const
  {
    return SDL_GetStorageSpaceRemaining(get());
  }

  /**
   * Enumerate a directory tree, filtered by pattern, and return a list.
   *
   * Files are filtered out if they don't match the string in `pattern`, which
   * may contain wildcard characters `*` (match everything) and `?` (match one
   * character). If pattern is nullptr, no filtering is done and all results are
   * returned. Subdirectories are permitted, and are specified with a path
   * separator of '/'. Wildcard characters `*` and `?` never match a path
   * separator.
   *
   * `flags` may be set to GLOB_CASEINSENSITIVE to make the pattern matching
   * case-insensitive.
   *
   * The returned array is always nullptr-terminated, for your iterating
   * convenience, but if `count` is non-nullptr, on return it will contain the
   * number of items in the array, not counting the nullptr terminator.
   *
   * If `path` is nullptr, this is treated as a request to enumerate the root of
   * the storage container's tree. An empty string also works for this.
   *
   * @param path the path of the directory to enumerate, or nullptr for the
   * root.
   * @param pattern the pattern that files in the directory must match. Can be
   *                nullptr.
   * @param flags `SDL_GLOB_*` bitflags that affect this search.
   * @returns an array of strings on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, assuming
   *               the `storage` object is thread-safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  OwnArray<char*> GlobDirectory(StringParam path,
                                StringParam pattern,
                                GlobFlags flags)
  {
    int count;
    auto data =
      CheckError(SDL_GlobStorageDirectory(get(), path, pattern, flags, &count));
    return OwnArray<char*>{data, size_t(count)};
  }

protected:
  /**
   * Closes and frees a storage container.
   *
   * @returns true if the container was freed with no errors, false otherwise;
   *          call GetError() for more information. Even if the function
   *          returns an error, the container data will be freed; the error is
   *          only for informational purposes.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.StorageRef
   */
  bool Close() { return reset(); }

  /**
   * Closes and frees a storage container.
   *
   * @returns true if the container was freed with no errors, false otherwise;
   *          call GetError() for more information. Even if the function
   *          returns an error, the container data will be freed; the error is
   *          only for informational purposes.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa StorageRef.StorageRef
   */
  bool reset(SDL_Storage* newResource = {})
  {
    return SDL_CloseStorage(release(newResource));
  }
};

/**
 * Unsafe Handle to storage
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa StorageRef
 */
struct StorageUnsafe : StorageRef
{
  using StorageRef::Close;

  using StorageRef::StorageRef;

  using StorageRef::reset;
};

/**
 * Handle to an owned storage
 *
 * @cat resource
 *
 * @sa StorageRef
 */
struct Storage : StorageUnsafe
{
  using StorageUnsafe::StorageUnsafe;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Storage(SDL_Storage* resource)
    : StorageUnsafe(resource)
  {
  }

  constexpr Storage(const Storage& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Storage(Storage&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Storage() { reset(); }

  /**
   * Assignment operator.
   */
  Storage& operator=(Storage other)
  {
    reset(other.release());
    return *this;
  }
};

/// @}
} // namespace SDL

#endif /* SDL3PP_STORAGE_H_ */
