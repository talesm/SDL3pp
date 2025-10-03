#ifndef SDL3PP_STORAGE_H_
#define SDL3PP_STORAGE_H_

#include <SDL3/SDL_storage.h>
#include "SDL3pp_filesystem.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryStorage Category Storage
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
 * ```c
 * void ReadGameData(void)
 * {
 *     extern char** fileNames;
 *     extern size_t numFiles;
 *
 *     Storage *title = Storage.Storage(nullptr, 0);
 *     if (title == nullptr) {
 *         // Something bad happened!
 *     }
 *     while (!Storage.Ready(title)) {
 *         Delay(1);
 *     }
 *
 *     for (size_t i = 0; i < numFiles; i += 1) {
 *         void* dst;
 *         Uint64 dstLen = 0;
 *
 *         if (Storage.GetFileSize(title, fileNames[i], &dstLen) && dstLen > 0)
 * { dst = malloc(dstLen); if (Storage.ReadFile(title, fileNames[i], dst,
 * dstLen)) {
 *                 // A bunch of stuff happens here
 *             } else {
 *                 // Something bad happened!
 *             }
 *             free(dst);
 *         } else {
 *             // Something bad happened!
 *         }
 *     }
 *
 *     Storage.Close(title);
 * }
 *
 * void ReadSave(void)
 * {
 *     Storage *user = Storage.Storage("libsdl", "Storage Example", 0);
 *     if (user == nullptr) {
 *         // Something bad happened!
 *     }
 *     while (!Storage.Ready(user)) {
 *         Delay(1);
 *     }
 *
 *     Uint64 saveLen = 0;
 *     if (Storage.GetFileSize(user, "save0.sav", &saveLen) && saveLen > 0) {
 *         void* dst = malloc(saveLen);
 *         if (Storage.ReadFile(user, "save0.sav", dst, saveLen)) {
 *             // A bunch of stuff happens here
 *         } else {
 *             // Something bad happened!
 *         }
 *         free(dst);
 *     } else {
 *         // Something bad happened!
 *     }
 *
 *     Storage.Close(user);
 * }
 *
 * void WriteSave(void)
 * {
 *     Storage *user = Storage.Storage("libsdl", "Storage Example", 0);
 *     if (user == nullptr) {
 *         // Something bad happened!
 *     }
 *     while (!Storage.Ready(user)) {
 *         Delay(1);
 *     }
 *
 *     extern void *saveData; // A bunch of stuff happened here...
 *     extern Uint64 saveLen;
 *     if (!Storage.WriteFile(user, "save0.sav", saveData, saveLen)) {
 *         // Something bad happened!
 *     }
 *
 *     Storage.Close(user);
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
struct Storage;

/// Alias to raw representation for Storage.
using StorageRaw = SDL_Storage*;

// Forward decl
struct StorageRef;

/// Safely wrap Storage for non owning parameters
struct StorageParam
{
  StorageRaw value; ///< parameter's StorageRaw

  /// Constructs from StorageRaw
  constexpr StorageParam(StorageRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr StorageParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const StorageParam& other) const = default;

  /// Converts to underlying StorageRaw
  constexpr operator StorageRaw() const { return value; }
};

/**
 * Function interface for Storage.
 *
 * Apps that want to supply a custom implementation of Storage will fill
 * in all the functions in this struct, and then pass it to Storage.Storage to
 * create a custom Storage object.
 *
 * It is not usually necessary to do this; SDL provides standard
 * implementations for many things you might expect to do with an Storage.
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
 * functions like Storage.Storage or Storage.Storage, etc, or create
 * an object with a custom implementation using Storage.Storage.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class Storage
{
  StorageRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Storage() = default;

  /**
   * Constructs from StorageParam.
   *
   * @param resource a StorageRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Storage(const StorageRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Storage(const Storage& other) = delete;

  /// Move constructor
  constexpr Storage(Storage&& other)
    : Storage(other.release())
  {
  }

  constexpr Storage(const StorageRef& other) = delete;

  constexpr Storage(StorageRef&& other) = delete;

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
   * @sa Storage.Close
   * @sa Storage.GetFileSize
   * @sa Storage.Storage
   * @sa Storage.ReadFile
   */
  Storage(StringParam override, PropertiesParam props)
    : m_resource(CheckError(SDL_OpenTitleStorage(override, props)))
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
   * @sa Storage.Close
   * @sa Storage.GetFileSize
   * @sa Storage.GetSpaceRemaining
   * @sa Storage.Storage
   * @sa Storage.ReadFile
   * @sa Storage.Ready
   * @sa Storage.WriteFile
   */
  Storage(StringParam org, StringParam app, PropertiesParam props)
    : m_resource(CheckError(SDL_OpenUserStorage(org, app, props)))
  {
  }

  /**
   * Opens up a container for local filesystem storage.
   *
   * This is provided for development and tools. Portable applications should
   * use Storage.Storage() for access to game data and
   * Storage.Storage() for access to user data.
   *
   * @param path the base path prepended to all storage paths, or nullptr for no
   *             base path.
   * @post a filesystem storage container on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.Close
   * @sa Storage.GetFileSize
   * @sa Storage.GetSpaceRemaining
   * @sa Storage.Storage
   * @sa Storage.Storage
   * @sa Storage.ReadFile
   * @sa Storage.WriteFile
   */
  Storage(StringParam path)
    : m_resource(CheckError(SDL_OpenFileStorage(path)))
  {
  }

  /**
   * Opens up a container using a client-provided storage interface.
   *
   * Applications do not need to use this function unless they are providing
   * their own Storage implementation. If you just need an Storage, you
   * should use the built-in implementations in SDL, like Storage.Storage()
   * or Storage.Storage().
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
   * @sa Storage.Close
   * @sa Storage.GetFileSize
   * @sa Storage.GetSpaceRemaining
   * @sa SDL_INIT_INTERFACE
   * @sa Storage.ReadFile
   * @sa Storage.Ready
   * @sa Storage.WriteFile
   */
  Storage(const StorageInterface& iface, void* userdata)
    : m_resource(CheckError(SDL_OpenStorage(&iface, userdata)))
  {
  }

  /// Destructor
  ~Storage() { SDL_CloseStorage(m_resource); }

  /// Assignment operator.
  Storage& operator=(Storage other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying StorageRaw.
  constexpr StorageRaw get() const { return m_resource; }

  /// Retrieves underlying StorageRaw and clear this.
  constexpr StorageRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Storage& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to StorageParam
  constexpr operator StorageParam() const { return {m_resource}; }

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
   * @sa Storage.Storage
   * @sa Storage.Storage
   * @sa Storage.Storage
   * @sa Storage.Storage
   */
  bool Close();

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
  bool Ready();

  /**
   * Query the size of a file within a storage container.
   *
   * @param path the relative path of the file to query.
   * @param length a pointer to be filled with the file's length.
   * @returns true if the file could be queried or false on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.ReadFile
   * @sa Storage.Ready
   */
  std::optional<Uint64> GetFileSize(StringParam path);

  /**
   * Synchronously read a file from a storage container into a client-provided
   * buffer.
   *
   * The value of `length` must match the length of the file exactly; call
   * Storage.GetFileSize() to get this value. This behavior may be relaxed in
   * a future release.
   *
   * @param path the relative path of the file to read.
   * @param destination a client-provided buffer to read the file into.
   * @param length the length of the destination buffer.
   * @returns true if the file was read or false on failure; call GetError()
   *          for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.GetFileSize
   * @sa Storage.Ready
   * @sa Storage.WriteFile
   */
  bool ReadFile(StringParam path, TargetBytes destination);

  /**
   * Synchronously read a file from a storage container into a client-provided
   * buffer.
   *
   * The value of `length` must match the length of the file exactly; call
   * Storage.GetFileSize() to get this value. This behavior may be relaxed in
   * a future release.
   *
   * @param storage a storage container to read from.
   * @param path the relative path of the file to read.
   * @param destination a client-provided buffer to read the file into.
   * @param length the length of the destination buffer.
   * @returns true if the file was read or false on failure; call GetError()
   *          for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.GetFileSize
   * @sa Storage.Ready
   * @sa Storage.WriteFile
   */
  std::string ReadFile(StringParam path);

  /**
   * Synchronously write a file from client memory into a storage container.
   *
   * @param path the relative path of the file to write.
   * @param source a client-provided buffer to write from.
   * @param length the length of the source buffer.
   * @returns true if the file was written or false on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.GetSpaceRemaining
   * @sa Storage.ReadFile
   * @sa Storage.Ready
   */
  void WriteFile(StringParam path, SourceBytes source);

  /**
   * Create a directory in a writable storage container.
   *
   * @param path the path of the directory to create.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.Ready
   */
  void CreateDirectory(StringParam path);

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
   * @sa Storage.Ready
   */
  void EnumerateDirectory(StringParam path,
                          EnumerateDirectoryCallback callback,
                          void* userdata);

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
   * @param storage a storage container.
   * @param path the path of the directory to enumerate, or nullptr for the
   * root.
   * @param callback a function that is called for each entry in the directory.
   * @param userdata a pointer that is passed to `callback`.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.Ready
   */
  void EnumerateDirectory(StringParam path, EnumerateDirectoryCB callback);

  /**
   * Remove a file or an empty directory in a writable storage container.
   *
   * @param path the path of the directory to enumerate.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.Ready
   */
  void RemovePath(StringParam path);

  /**
   * Rename a file or directory in a writable storage container.
   *
   * @param oldpath the old path.
   * @param newpath the new path.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.Ready
   */
  void RenamePath(StringParam oldpath, StringParam newpath);

  /**
   * Copy a file in a writable storage container.
   *
   * @param oldpath the old path.
   * @param newpath the new path.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.Ready
   */
  void CopyFile(StringParam oldpath, StringParam newpath);

  /**
   * Get information about a filesystem path in a storage container.
   *
   * @param path the path to query.
   * @param info a pointer filled in with information about the path, or nullptr
   * to check for the existence of a file.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.Ready
   */
  PathInfo GetPathInfo(StringParam path);

  /**
   * Queries the remaining space in a storage container.
   *
   * @returns the amount of remaining space, in bytes.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Storage.Ready
   * @sa Storage.WriteFile
   */
  Uint64 GetSpaceRemaining();

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
   * @param count on return, will be set to the number of items in the returned
   *              array. Can be nullptr.
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
                                GlobFlags flags);

  template<class T>
  std::vector<T> ReadFileAs(StringParam path);
};

/// Semi-safe reference for Storage.
struct StorageRef : Storage
{
  /**
   * Constructs from StorageParam.
   *
   * @param resource a StorageRaw or Storage.
   *
   * This does not takes ownership!
   */
  StorageRef(StorageParam resource)
    : Storage(resource.value)
  {
  }

  /// Copy constructor.
  StorageRef(const StorageRef& other)
    : Storage(other.get())
  {
  }

  /// Destructor
  ~StorageRef() { release(); }
};

/**
 * Opens up a read-only container for the application's filesystem.
 *
 * @param override a path to override the backend's default title root.
 * @param props a property list that may contain backend-specific information.
 * @returns a title storage container on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Close
 * @sa Storage.GetFileSize
 * @sa Storage.Storage
 * @sa Storage.ReadFile
 */
inline Storage OpenTitleStorage(StringParam override, PropertiesParam props)
{
  return Storage(std::move(override), props);
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
 * @returns a user storage container on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Close
 * @sa Storage.GetFileSize
 * @sa Storage.GetSpaceRemaining
 * @sa Storage.Storage
 * @sa Storage.ReadFile
 * @sa Storage.Ready
 * @sa Storage.WriteFile
 */
inline Storage OpenUserStorage(StringParam org,
                               StringParam app,
                               PropertiesParam props)
{
  return Storage(std::move(org), std::move(app), props);
}

/**
 * Opens up a container for local filesystem storage.
 *
 * This is provided for development and tools. Portable applications should
 * use Storage.Storage() for access to game data and
 * Storage.Storage() for access to user data.
 *
 * @param path the base path prepended to all storage paths, or nullptr for no
 *             base path.
 * @returns a filesystem storage container on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Close
 * @sa Storage.GetFileSize
 * @sa Storage.GetSpaceRemaining
 * @sa Storage.Storage
 * @sa Storage.Storage
 * @sa Storage.ReadFile
 * @sa Storage.WriteFile
 */
inline Storage OpenFileStorage(StringParam path)
{
  return Storage(std::move(path));
}

/**
 * Opens up a container using a client-provided storage interface.
 *
 * Applications do not need to use this function unless they are providing
 * their own Storage implementation. If you just need an Storage, you
 * should use the built-in implementations in SDL, like Storage.Storage()
 * or Storage.Storage().
 *
 * This function makes a copy of `iface` and the caller does not need to keep
 * it around after this call.
 *
 * @param iface the interface that implements this storage, initialized using
 *              SDL_INIT_INTERFACE().
 * @param userdata the pointer that will be passed to the interface functions.
 * @returns a storage container on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Close
 * @sa Storage.GetFileSize
 * @sa Storage.GetSpaceRemaining
 * @sa SDL_INIT_INTERFACE
 * @sa Storage.ReadFile
 * @sa Storage.Ready
 * @sa Storage.WriteFile
 */
inline Storage OpenStorage(const StorageInterface& iface, void* userdata)
{
  return Storage(iface, userdata);
}

/**
 * Closes and frees a storage container.
 *
 * @param storage a storage container to close.
 * @returns true if the container was freed with no errors, false otherwise;
 *          call GetError() for more information. Even if the function
 *          returns an error, the container data will be freed; the error is
 *          only for informational purposes.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Storage
 * @sa Storage.Storage
 * @sa Storage.Storage
 * @sa Storage.Storage
 */
inline bool CloseStorage(StorageRaw storage)
{
  return SDL_CloseStorage(storage);
}

inline bool Storage::Close() { return CloseStorage(release()); }

/**
 * Checks if the storage container is ready to use.
 *
 * This function should be called in regular intervals until it returns true -
 * however, it is not recommended to spinwait on this call, as the backend may
 * depend on a synchronous message loop. You might instead poll this in your
 * game's main loop while processing events and drawing a loading screen.
 *
 * @param storage a storage container to query.
 * @returns true if the container is ready, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool StorageReady(StorageParam storage)
{
  return SDL_StorageReady(storage);
}

inline bool Storage::Ready() { return SDL::StorageReady(m_resource); }

/**
 * Query the size of a file within a storage container.
 *
 * @param storage a storage container to query.
 * @param path the relative path of the file to query.
 * @param length a pointer to be filled with the file's length.
 * @returns true if the file could be queried or false on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.ReadFile
 * @sa Storage.Ready
 */
inline std::optional<Uint64> GetStorageFileSize(StorageParam storage,
                                                StringParam path)
{
  return SDL_GetStorageFileSize(storage, path);
}

inline std::optional<Uint64> Storage::GetFileSize(StringParam path)
{
  return SDL::GetStorageFileSize(m_resource, std::move(path));
}

/**
 * Synchronously read a file from a storage container into a client-provided
 * buffer.
 *
 * The value of `length` must match the length of the file exactly; call
 * Storage.GetFileSize() to get this value. This behavior may be relaxed in
 * a future release.
 *
 * @param storage a storage container to read from.
 * @param path the relative path of the file to read.
 * @param destination a client-provided buffer to read the file into.
 * @param length the length of the destination buffer.
 * @returns true if the file was read or false on failure; call GetError()
 *          for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.GetFileSize
 * @sa Storage.Ready
 * @sa Storage.WriteFile
 */
inline bool ReadStorageFile(StorageParam storage,
                            StringParam path,
                            TargetBytes destination)
{
  return SDL_ReadStorageFile(
    storage, path, destination.data, destination.size_bytes);
}

/**
 * Synchronously read a file from a storage container into a client-provided
 * buffer.
 *
 * The value of `length` must match the length of the file exactly; call
 * Storage.GetFileSize() to get this value. This behavior may be relaxed in
 * a future release.
 *
 * @param storage a storage container to read from.
 * @param path the relative path of the file to read.
 * @param destination a client-provided buffer to read the file into.
 * @param length the length of the destination buffer.
 * @returns true if the file was read or false on failure; call GetError()
 *          for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.GetFileSize
 * @sa Storage.Ready
 * @sa Storage.WriteFile
 */
inline std::string ReadStorageFile(StorageParam storage, StringParam path)
{
  static_assert(false, "Not implemented");
}

inline bool Storage::ReadFile(StringParam path, TargetBytes destination)
{
  return SDL::ReadStorageFile(
    m_resource, std::move(path), std::move(destination));
}

inline std::string Storage::ReadFile(StringParam path)
{
  return SDL::ReadStorageFile(m_resource, std::move(path));
}

template<class T>
inline std::vector<T> ReadStorageFileAs(StorageParam storage, StringParam path)
{
  static_assert(false, "Not implemented");
}

inline std::vector<T> Storage::ReadFileAs(StringParam path)
{
  return SDL::ReadStorageFileAs(m_resource, std::move(path));
}

/**
 * Synchronously write a file from client memory into a storage container.
 *
 * @param storage a storage container to write to.
 * @param path the relative path of the file to write.
 * @param source a client-provided buffer to write from.
 * @param length the length of the source buffer.
 * @returns true if the file was written or false on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.GetSpaceRemaining
 * @sa Storage.ReadFile
 * @sa Storage.Ready
 */
inline void WriteStorageFile(StorageParam storage,
                             StringParam path,
                             SourceBytes source)
{
  SDL_WriteStorageFile(storage, path, source.data, source.size_bytes);
}

inline void Storage::WriteFile(StringParam path, SourceBytes source)
{
  SDL::WriteStorageFile(m_resource, std::move(path), std::move(source));
}

/**
 * Create a directory in a writable storage container.
 *
 * @param storage a storage container.
 * @param path the path of the directory to create.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Ready
 */
inline void CreateStorageDirectory(StorageParam storage, StringParam path)
{
  CheckError(SDL_CreateStorageDirectory(storage, path));
}

inline void Storage::CreateDirectory(StringParam path)
{
  SDL::CreateStorageDirectory(m_resource, std::move(path));
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
 * @param storage a storage container.
 * @param path the path of the directory to enumerate, or nullptr for the root.
 * @param callback a function that is called for each entry in the directory.
 * @param userdata a pointer that is passed to `callback`.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Ready
 */
inline void EnumerateStorageDirectory(StorageParam storage,
                                      StringParam path,
                                      EnumerateDirectoryCallback callback,
                                      void* userdata)
{
  CheckError(SDL_EnumerateStorageDirectory(storage, path, callback, userdata));
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
 * @param storage a storage container.
 * @param path the path of the directory to enumerate, or nullptr for the root.
 * @param callback a function that is called for each entry in the directory.
 * @param userdata a pointer that is passed to `callback`.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Ready
 */
inline void EnumerateStorageDirectory(StorageParam storage,
                                      StringParam path,
                                      EnumerateDirectoryCB callback)
{
  static_assert(false, "Not implemented");
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
 * @param storage a storage container.
 * @param path the path of the directory to enumerate, or nullptr for the root.
 * @param callback a function that is called for each entry in the directory.
 * @param userdata a pointer that is passed to `callback`.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Ready
 */
inline std::vector<Path> EnumerateStorageDirectory(StorageParam storage,
                                                   StringParam path)
{
  static_assert(false, "Not implemented");
}

inline void Storage::EnumerateDirectory(StringParam path,
                                        EnumerateDirectoryCallback callback,
                                        void* userdata)
{
  SDL::EnumerateStorageDirectory(
    m_resource, std::move(path), callback, userdata);
}

inline void Storage::EnumerateDirectory(StringParam path,
                                        EnumerateDirectoryCB callback)
{
  SDL::EnumerateStorageDirectory(m_resource, std::move(path), callback);
}

/**
 * Remove a file or an empty directory in a writable storage container.
 *
 * @param storage a storage container.
 * @param path the path of the directory to enumerate.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Ready
 */
inline void RemoveStoragePath(StorageParam storage, StringParam path)
{
  CheckError(SDL_RemoveStoragePath(storage, path));
}

inline void Storage::RemovePath(StringParam path)
{
  SDL::RemoveStoragePath(m_resource, std::move(path));
}

/**
 * Rename a file or directory in a writable storage container.
 *
 * @param storage a storage container.
 * @param oldpath the old path.
 * @param newpath the new path.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Ready
 */
inline void RenameStoragePath(StorageParam storage,
                              StringParam oldpath,
                              StringParam newpath)
{
  CheckError(SDL_RenameStoragePath(storage, oldpath, newpath));
}

inline void Storage::RenamePath(StringParam oldpath, StringParam newpath)
{
  SDL::RenameStoragePath(m_resource, std::move(oldpath), std::move(newpath));
}

/**
 * Copy a file in a writable storage container.
 *
 * @param storage a storage container.
 * @param oldpath the old path.
 * @param newpath the new path.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Ready
 */
inline void CopyStorageFile(StorageParam storage,
                            StringParam oldpath,
                            StringParam newpath)
{
  CheckError(SDL_CopyStorageFile(storage, oldpath, newpath));
}

inline void Storage::CopyFile(StringParam oldpath, StringParam newpath)
{
  SDL::CopyStorageFile(m_resource, std::move(oldpath), std::move(newpath));
}

/**
 * Get information about a filesystem path in a storage container.
 *
 * @param storage a storage container.
 * @param path the path to query.
 * @param info a pointer filled in with information about the path, or nullptr
 * to check for the existence of a file.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Ready
 */
inline PathInfo GetStoragePathInfo(StorageParam storage, StringParam path)
{
  return CheckError(SDL_GetStoragePathInfo(storage, path));
}

inline PathInfo Storage::GetPathInfo(StringParam path)
{
  return SDL::GetStoragePathInfo(m_resource, std::move(path));
}

/**
 * Queries the remaining space in a storage container.
 *
 * @param storage a storage container to query.
 * @returns the amount of remaining space, in bytes.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Storage.Ready
 * @sa Storage.WriteFile
 */
inline Uint64 GetStorageSpaceRemaining(StorageParam storage)
{
  return SDL_GetStorageSpaceRemaining(storage);
}

inline Uint64 Storage::GetSpaceRemaining()
{
  return SDL::GetStorageSpaceRemaining(m_resource);
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
 * @param storage a storage container.
 * @param path the path of the directory to enumerate, or nullptr for the root.
 * @param pattern the pattern that files in the directory must match. Can be
 *                nullptr.
 * @param flags `SDL_GLOB_*` bitflags that affect this search.
 * @param count on return, will be set to the number of items in the returned
 *              array. Can be nullptr.
 * @returns an array of strings on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, assuming
 *               the `storage` object is thread-safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<char*> GlobStorageDirectory(StorageParam storage,
                                            StringParam path,
                                            StringParam pattern,
                                            GlobFlags flags)
{
  return CheckError(SDL_GlobStorageDirectory(storage, path, pattern, flags));
}

inline OwnArray<char*> Storage::GlobDirectory(StringParam path,
                                              StringParam pattern,
                                              GlobFlags flags)
{
  return SDL::GlobStorageDirectory(
    m_resource, std::move(path), std::move(pattern), flags);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_STORAGE_H_ */
