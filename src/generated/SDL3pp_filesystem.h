#ifndef SDL3PP_FILESYSTEM_H_
#define SDL3PP_FILESYSTEM_H_

#include <SDL3/SDL_filesystem.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryFilesystem Category Filesystem
 *
 * SDL offers an API for examining and manipulating the system's filesystem.
 * This covers most things one would need to do with directories, except for
 * actual file I/O (which is covered by [CategoryIOStream](CategoryIOStream) and
 * [CategoryAsyncIO](CategoryAsyncIO) instead).
 *
 * There are functions to answer necessary path questions:
 *
 * - Where is my app's data? GetBasePath().
 * - Where can I safely write files? GetPrefPath().
 * - Where are paths like Downloads, Desktop, Music? GetUserFolder().
 * - What is this thing at this location? GetPathInfo().
 * - What items live in this folder? EnumerateDirectory().
 * - What items live in this folder by wildcard? GlobDirectory().
 * - What is my current working directory? GetCurrentDirectory().
 *
 * SDL also offers functions to manipulate the directory tree: renaming,
 * removing, copying files.
 *
 * @{
 */

/// Alias to raw representation for PathInfo.
using PathInfoRaw = SDL_PathInfo;

// Forward decl
struct PathInfo;

struct Path : StringResult
{
  using StringResult::StringResult;

  Path& operator+=(std::string_view other)
  {
    static_assert(false, "Not implemented");
  }

  Path& operator+=(char ch) { static_assert(false, "Not implemented"); }

  Path operator+(std::string_view other) const
  {
    static_assert(false, "Not implemented");
  }

  Path operator+(char ch) const { static_assert(false, "Not implemented"); }

  Path& operator/=(std::string_view other)
  {
    static_assert(false, "Not implemented");
  }

  Path operator/(std::string_view other) const
  {
    static_assert(false, "Not implemented");
  }
};

/**
 * Get the directory where the application was run from.
 *
 * SDL caches the result of this call internally, but the first call to this
 * function is not necessarily fast, so plan accordingly.
 *
 * **macOS and iOS Specific Functionality**: If the application is in a ".app"
 * bundle, this function returns the Resource directory (e.g.
 * MyApp.app/Contents/Resources/). This behaviour can be overridden by adding a
 * property to the Info.plist file. Adding a string key with the name
 * SDL_FILESYSTEM_BASE_DIR_TYPE with a supported value will change the
 * behaviour.
 *
 * Supported values for the SDL_FILESYSTEM_BASE_DIR_TYPE property (Given an
 * application in /Applications/SDLApp/MyApp.app):
 *
 * - `resource`: bundle resource directory (the default). For example:
 *   `/Applications/SDLApp/MyApp.app/Contents/Resources`
 * - `bundle`: the Bundle directory. For example:
 *   `/Applications/SDLApp/MyApp.app/`
 * - `parent`: the containing directory of the bundle. For example:
 *   `/Applications/SDLApp/`
 *
 * **Android Specific Functionality**: This function returns "./", which allows
 * filesystem operations to use internal storage and the asset system.
 *
 * **Nintendo 3DS Specific Functionality**: This function returns "romfs"
 * directory of the application as it is uncommon to store resources outside the
 * executable. As such it is not a writable directory.
 *
 * The returned path is guaranteed to end with a path separator ('\\' on
 * Windows, '/' on most other platforms).
 *
 * @returns an absolute path in UTF-8 encoding to the application data
 *          directory. nullptr will be returned on error or when the platform
 *          doesn't implement this functionality, call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPrefPath
 */
inline const char* GetBasePath() { return SDL_GetBasePath(); }

/**
 * Get the user-and-app-specific path where files can be written.
 *
 * Get the "pref dir". This is meant to be where users can write personal files
 * (preferences and save games, etc) that are specific to your application. This
 * directory is unique per user, per application.
 *
 * This function will decide the appropriate location in the native filesystem,
 * create the directory if necessary, and return a string of the absolute path
 * to the directory in UTF-8 encoding.
 *
 * On Windows, the string might look like:
 *
 * `C:\@Users\@bob\@AppData\@Roaming\@My Company\@My Program Name\\`
 *
 * On Linux, the string might look like:
 *
 * `/home/bob/.local/share/My Program Name/`
 *
 * On macOS, the string might look like:
 *
 * `/Users/bob/Library/Application Support/My Program Name/`
 *
 * You should assume the path returned by this function is the only safe place
 * to write files (and that GetBasePath(), while it might be writable, or even
 * the parent of the returned path, isn't where you should be writing things).
 *
 * Both the org and app strings may become part of a directory name, so please
 * follow these rules:
 *
 * - Try to use the same org string (_including case-sensitivity_) for all your
 *   applications that use this function.
 * - Always use a unique app string for each one, and make sure it never changes
 *   for an app once you've decided on it.
 * - Unicode characters are legal, as long as they are UTF-8 encoded, but...
 * - ...only use letters, numbers, and spaces. Avoid punctuation like "Game Name
 *   2: Bad Guy's Revenge!" ... "Game Name 2" is sufficient.
 *
 * Due to historical mistakes, `org` is allowed to be nullptr or "". In such
 * cases, SDL will omit the org subdirectory, including on platforms where it
 * shouldn't, and including on platforms where this would make your app fail
 * certification for an app store. New apps should definitely specify a real
 * string for `org`.
 *
 * The returned path is guaranteed to end with a path separator ('\\' on
 * Windows, '/' on most other platforms).
 *
 * @param org the name of your organization.
 * @param app the name of your application.
 * @returns a UTF-8 string of the user directory in platform-dependent notation.
 *          nullptr if there's a problem (creating directory failed, etc.). This
 *          should be freed with free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetBasePath
 */
inline Path GetPrefPath(StringParam org, StringParam app)
{
  return SDL_GetPrefPath(org, app);
}

/**
 * The type of the OS-provided default folder for a specific purpose.
 *
 * Note that the Trash folder isn't included here, because trashing files
 * usually involves extra OS-specific functionality to remember the file's
 * original location.
 *
 * The folders supported per platform are:
 *
 * |             | Windows | macOS/iOS | tvOS | Unix (XDG) | Haiku | Emscripten
 * | | ----------- | ------- | --------- | ---- | ---------- | ----- |
 * ---------- | | HOME        | X       | X         |      | X          | X | X
 * | | DESKTOP     | X       | X         |      | X          | X     | | |
 * DOCUMENTS   | X       | X         |      | X          |       |            |
 * | DOWNLOADS   | Vista+  | X         |      | X          |       | | | MUSIC
 * | X       | X         |      | X          |       |            | | PICTURES
 * | X       | X         |      | X          |       |            | |
 * PUBLICSHARE |         | X         |      | X          |       |            |
 * | SAVEDGAMES  | Vista+  |           |      |            |       | | |
 * SCREENSHOTS | Vista+  |           |      |            |       |            |
 * | TEMPLATES   | X       | X         |      | X          |       | | | VIDEOS
 * | X       | X*        |      | X          |       |            |
 *
 * Note that on macOS/iOS, the Videos folder is called "Movies".
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GetUserFolder
 */
using Folder = SDL_Folder;

/**
 * The folder which contains all of the current user's data, preferences, and
 * documents. It usually contains most of the other folders. If a requested
 * folder does not exist, the home folder can be considered a safe fallback to
 * store a user's documents.
 */
constexpr Folder FOLDER_HOME = SDL_FOLDER_HOME;

/**
 * The folder of files that are displayed on the desktop. Note that the
 * existence of a desktop folder does not guarantee that the system does show
 * icons on its desktop; certain GNU/Linux distros with a graphical environment
 * may not have desktop icons.
 */
constexpr Folder FOLDER_DESKTOP = SDL_FOLDER_DESKTOP;

/**
 * User document files, possibly application-specific. This is a good place to
 * save a user's projects.
 */
constexpr Folder FOLDER_DOCUMENTS = SDL_FOLDER_DOCUMENTS;

/// Standard folder for user files downloaded from the internet.
constexpr Folder FOLDER_DOWNLOADS = SDL_FOLDER_DOWNLOADS;

/// Music files that can be played using a standard music player (mp3, ogg...).
constexpr Folder FOLDER_MUSIC = SDL_FOLDER_MUSIC;

/// Image files that can be displayed using a standard viewer (png, jpg...).
constexpr Folder FOLDER_PICTURES = SDL_FOLDER_PICTURES;

/// Files that are meant to be shared with other users on the same computer.
constexpr Folder FOLDER_PUBLICSHARE = SDL_FOLDER_PUBLICSHARE;

constexpr Folder FOLDER_SAVEDGAMES =
  SDL_FOLDER_SAVEDGAMES; ///< Save files for games.

constexpr Folder FOLDER_SCREENSHOTS =
  SDL_FOLDER_SCREENSHOTS; ///< Application screenshots.

/**
 * Template files to be used when the user requests the desktop environment to
 * create a new file in a certain folder, such as "New Text File.txt". Any file
 * in the Templates folder can be used as a starting point for a new file.
 */
constexpr Folder FOLDER_TEMPLATES = SDL_FOLDER_TEMPLATES;

/**
 * Video files that can be played using a standard video player (mp4, webm...).
 */
constexpr Folder FOLDER_VIDEOS = SDL_FOLDER_VIDEOS;

/// Total number of types in this enum, not a folder type by itself.
constexpr Folder FOLDER_COUNT = SDL_FOLDER_COUNT;

/**
 * Finds the most suitable user folder for a specific purpose.
 *
 * Many OSes provide certain standard folders for certain purposes, such as
 * storing pictures, music or videos for a certain user. This function gives the
 * path for many of those special locations.
 *
 * This function is specifically for _user_ folders, which are meant for the
 * user to access and manage. For application-specific folders, meant to hold
 * data for the application to manage, see GetBasePath() and GetPrefPath().
 *
 * The returned path is guaranteed to end with a path separator ('\\' on
 * Windows, '/' on most other platforms).
 *
 * If nullptr is returned, the error may be obtained with GetError().
 *
 * @param folder the type of folder to find.
 * @returns either a null-terminated C string containing the full path to the
 *          folder, or nullptr if an error happened.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetUserFolder(Folder folder)
{
  return SDL_GetUserFolder(folder);
}

/**
 * Types of filesystem entries.
 *
 * Note that there may be other sorts of items on a filesystem: devices,
 * symlinks, named pipes, etc. They are currently reported as PATHTYPE_OTHER.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa PathInfo
 */
using PathType = SDL_PathType;

constexpr PathType PATHTYPE_NONE = SDL_PATHTYPE_NONE; ///< path does not exist

constexpr PathType PATHTYPE_FILE = SDL_PATHTYPE_FILE; ///< a normal file

constexpr PathType PATHTYPE_DIRECTORY = SDL_PATHTYPE_DIRECTORY; ///< a directory

/**
 * something completely different like a device node (not a symlink, those are
 * always followed)
 */
constexpr PathType PATHTYPE_OTHER = SDL_PATHTYPE_OTHER;

/**
 * Information about a path on the filesystem.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GetPathInfo
 * @sa Storage.GetPathInfo
 */
struct PathInfo : PathInfoRaw
{
  /**
   * Wraps PathInfo.
   *
   * @param pathInfo the value to be wrapped
   */
  constexpr PathInfo(const PathInfoRaw& pathInfo = {}) noexcept
    : PathInfoRaw(pathInfo)
  {
  }

  /**
   * Compare with nullptr.
   *
   * @returns True if invalid state, false otherwise.
   */
  constexpr bool operator==(std::nullptr_t _) const noexcept
  {
    return !bool(*this);
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const noexcept
  {
    return *this != PathInfoRaw{};
  }
};

/**
 * Flags for path matching.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GlobDirectory
 * @sa Storage.GlobDirectory
 */
using GlobFlags = Uint32;

constexpr GlobFlags GLOB_CASEINSENSITIVE =
  SDL_GLOB_CASEINSENSITIVE; ///< CASEINSENSITIVE

/**
 * Create a directory, and any missing parent directories.
 *
 * This reports success if `path` already exists as a directory.
 *
 * If parent directories are missing, it will also create them. Note that if
 * this fails, it will not remove any parent directories it already made.
 *
 * @param path the path of the directory to create.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void CreateDirectory(StringParam path)
{
  CheckError(SDL_CreateDirectory(path));
}

/**
 * Possible results from an enumeration callback.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa EnumerateDirectoryCallback
 */
using EnumerationResult = SDL_EnumerationResult;

constexpr EnumerationResult ENUM_CONTINUE =
  SDL_ENUM_CONTINUE; ///< Value that requests that enumeration continue.

/// Value that requests that enumeration stop, successfully.
constexpr EnumerationResult ENUM_SUCCESS = SDL_ENUM_SUCCESS;

/// Value that requests that enumeration stop, as a failure.
constexpr EnumerationResult ENUM_FAILURE = SDL_ENUM_FAILURE;

/**
 * Callback for directory enumeration.
 *
 * Enumeration of directory entries will continue until either all entries have
 * been provided to the callback, or the callback has requested a stop through
 * its return value.
 *
 * Returning ENUM_CONTINUE will let enumeration proceed, calling the callback
 * with further entries. ENUM_SUCCESS and ENUM_FAILURE will terminate the
 * enumeration early, and dictate the return value of the enumeration function
 * itself.
 *
 * `dirname` is guaranteed to end with a path separator ('\\' on Windows, '/' on
 * most other platforms).
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 * @param dirname the directory that is being enumerated.
 * @param fname the next entry in the enumeration.
 * @returns how the enumeration should proceed.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EnumerateDirectory
 */
using EnumerateDirectoryCallback = EnumerationResult(
  SDLCALL*)(void* userdata, const char* dirname, const char* fname);

/**
 * Callback for directory enumeration.
 *
 * Enumeration of directory entries will continue until either all entries have
 * been provided to the callback, or the callback has requested a stop through
 * its return value.
 *
 * Returning ENUM_CONTINUE will let enumeration proceed, calling the callback
 * with further entries. ENUM_SUCCESS and ENUM_FAILURE will terminate the
 * enumeration early, and dictate the return value of the enumeration function
 * itself.
 *
 * `dirname` is guaranteed to end with a path separator ('\\' on Windows, '/' on
 * most other platforms).
 *
 * @param dirname the directory that is being enumerated.
 * @param fname the next entry in the enumeration.
 * @returns how the enumeration should proceed.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EnumerateDirectory
 *
 * @sa EnumerateDirectoryCallback
 */
using EnumerateDirectoryCB =
  std::function<EnumerationResult(const char* dirname, const char* fname)>;

/**
 * Enumerate a directory through a callback function.
 *
 * This function provides every directory entry through an app-provided
 * callback, called once for each directory entry, until all results have been
 * provided or the callback returns either ENUM_SUCCESS or ENUM_FAILURE.
 *
 * This will return false if there was a system problem in general, or if a
 * callback returns ENUM_FAILURE. A successful return means a callback returned
 * ENUM_SUCCESS to halt enumeration, or all directory entries were enumerated.
 *
 * @param path the path of the directory to enumerate.
 * @param callback a function that is called for each entry in the directory.
 * @param userdata a pointer that is passed to `callback`.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EnumerateDirectory(StringParam path,
                               EnumerateDirectoryCallback callback,
                               void* userdata)
{
  CheckError(SDL_EnumerateDirectory(path, callback, userdata));
}

/**
 * Enumerate a directory through a callback function.
 *
 * This function provides every directory entry through an app-provided
 * callback, called once for each directory entry, until all results have been
 * provided or the callback returns either ENUM_SUCCESS or ENUM_FAILURE.
 *
 * This will return false if there was a system problem in general, or if a
 * callback returns ENUM_FAILURE. A successful return means a callback returned
 * ENUM_SUCCESS to halt enumeration, or all directory entries were enumerated.
 *
 * @param path the path of the directory to enumerate.
 * @param callback a function that is called for each entry in the directory.
 * @param userdata a pointer that is passed to `callback`.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EnumerateDirectory(StringParam path, EnumerateDirectoryCB callback)
{
  static_assert(false, "Not implemented");
}

/**
 * Enumerate a directory through a callback function.
 *
 * This function provides every directory entry through an app-provided
 * callback, called once for each directory entry, until all results have been
 * provided or the callback returns either ENUM_SUCCESS or ENUM_FAILURE.
 *
 * This will return false if there was a system problem in general, or if a
 * callback returns ENUM_FAILURE. A successful return means a callback returned
 * ENUM_SUCCESS to halt enumeration, or all directory entries were enumerated.
 *
 * @param path the path of the directory to enumerate.
 * @param callback a function that is called for each entry in the directory.
 * @param userdata a pointer that is passed to `callback`.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline std::vector<Path> EnumerateDirectory(StringParam path)
{
  static_assert(false, "Not implemented");
}

/**
 * Remove a file or an empty directory.
 *
 * Directories that are not empty will fail; this function will not recursely
 * delete directory trees.
 *
 * @param path the path to remove from the filesystem.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void RemovePath(StringParam path) { CheckError(SDL_RemovePath(path)); }

/**
 * Rename a file or directory.
 *
 * If the file at `newpath` already exists, it will be replaced.
 *
 * Note that this will not copy files across filesystems/drives/volumes, as that
 * is a much more complicated (and possibly time-consuming) operation.
 *
 * Which is to say, if this function fails, CopyFile() to a temporary file in
 * the same directory as `newpath`, then RenamePath() from the temporary file to
 * `newpath` and RemovePath() on `oldpath` might work for files. Renaming a
 * non-empty directory across filesystems is dramatically more complex, however.
 *
 * @param oldpath the old path.
 * @param newpath the new path.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void RenamePath(StringParam oldpath, StringParam newpath)
{
  CheckError(SDL_RenamePath(oldpath, newpath));
}

/**
 * Copy a file.
 *
 * If the file at `newpath` already exists, it will be overwritten with the
 * contents of the file at `oldpath`.
 *
 * This function will block until the copy is complete, which might be a
 * significant time for large files on slow disks. On some platforms, the copy
 * can be handed off to the OS itself, but on others SDL might just open both
 * paths, and read from one and write to the other.
 *
 * Note that this is not an atomic operation! If something tries to read from
 * `newpath` while the copy is in progress, it will see an incomplete copy of
 * the data, and if the calling thread terminates (or the power goes out) during
 * the copy, `newpath`'s previous contents will be gone, replaced with an
 * incomplete copy of the data. To avoid this risk, it is recommended that the
 * app copy to a temporary file in the same directory as `newpath`, and if the
 * copy is successful, use RenamePath() to replace `newpath` with the temporary
 * file. This will ensure that reads of `newpath` will either see a complete
 * copy of the data, or it will see the pre-copy state of `newpath`.
 *
 * This function attempts to synchronize the newly-copied data to disk before
 * returning, if the platform allows it, so that the renaming trick will not
 * have a problem in a system crash or power failure, where the file could be
 * renamed but the contents never made it from the system file cache to the
 * physical disk.
 *
 * If the copy fails for any reason, the state of `newpath` is undefined. It
 * might be half a copy, it might be the untouched data of what was already
 * there, or it might be a zero-byte file, etc.
 *
 * @param oldpath the old path.
 * @param newpath the new path.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, but this
 *               operation is not atomic, so the app might need to protect
 *               access to specific paths from other threads if appropriate.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void CopyFile(StringParam oldpath, StringParam newpath)
{
  CheckError(SDL_CopyFile(oldpath, newpath));
}

/**
 * Get information about a filesystem path.
 *
 * @param path the path to query.
 * @param info a pointer filled in with information about the path, or nullptr
 *             to check for the existence of a file.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PathInfo GetPathInfo(StringParam path)
{
  return CheckError(SDL_GetPathInfo(path));
}

/**
 * Enumerate a directory tree, filtered by pattern, and return a list.
 *
 * Files are filtered out if they don't match the string in `pattern`, which may
 * contain wildcard characters `*` (match everything) and `?` (match one
 * character). If pattern is nullptr, no filtering is done and all results are
 * returned. Subdirectories are permitted, and are specified with a path
 * separator of `/`. Wildcard characters `*` and `?` never match a path
 * separator.
 *
 * `flags` may be set to GLOB_CASEINSENSITIVE to make the pattern matching
 * case-insensitive.
 *
 * The returned array is always nullptr-terminated, for your iterating
 * convenience, but if `count` is non-nullptr, on return it will contain the
 * number of items in the array, not counting the nullptr terminator.
 *
 * @param path the path of the directory to enumerate.
 * @param pattern the pattern that files in the directory must match. Can be
 *                nullptr.
 * @param flags `SDL_GLOB_*` bitflags that affect this search.
 * @param count on return, will be set to the number of items in the returned
 *              array. Can be nullptr.
 * @returns an array of strings on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<char*> GlobDirectory(StringParam path,
                                     StringParam pattern,
                                     GlobFlags flags)
{
  return CheckError(SDL_GlobDirectory(path, pattern, flags));
}

/**
 * Get what the system believes is the "current working directory."
 *
 * For systems without a concept of a current working directory, this will still
 * attempt to provide something reasonable.
 *
 * SDL does not provide a means to _change_ the current working directory; for
 * platforms without this concept, this would cause surprises with file access
 * outside of SDL.
 *
 * The returned path is guaranteed to end with a path separator ('\\' on
 * Windows, '/' on most other platforms).
 *
 * @returns a UTF-8 string of the current working directory in
 *          platform-dependent notation. nullptr if there's a problem. This
 *          should be freed with free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Path GetCurrentDirectory() { return SDL_GetCurrentDirectory(); }

/// @}

} // namespace SDL

#endif /* SDL3PP_FILESYSTEM_H_ */
