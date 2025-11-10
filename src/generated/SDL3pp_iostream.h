#ifndef SDL3PP_IOSTREAM_H_
#define SDL3PP_IOSTREAM_H_

#include <SDL3/SDL_iostream.h>
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryIOStream Category IOStream
 *
 * SDL provides an abstract interface for reading and writing data streams. It
 * offers implementations for files, memory, etc, and the app can provide their
 * own implementations, too.
 *
 * IOStream is not related to the standard C++ iostream class, other than both
 * are abstract interfaces to read/write data.
 *
 * @{
 */

// Forward decl
struct IOStream;

/// Alias to raw representation for IOStream.
using IOStreamRaw = SDL_IOStream*;

// Forward decl
struct IOStreamRef;

/// Safely wrap IOStream for non owning parameters
struct IOStreamParam
{
  IOStreamRaw value; ///< parameter's IOStreamRaw

  /// Constructs from IOStreamRaw
  constexpr IOStreamParam(IOStreamRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr IOStreamParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const IOStreamParam& other) const = default;

  /// Converts to underlying IOStreamRaw
  constexpr operator IOStreamRaw() const { return value; }
};

/**
 * IOStream status, set by a read or write operation.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using IOStatus = SDL_IOStatus;

constexpr IOStatus IO_STATUS_READY =
  SDL_IO_STATUS_READY; ///< Everything is ready (no errors and not EOF).

constexpr IOStatus IO_STATUS_ERROR =
  SDL_IO_STATUS_ERROR; ///< Read or write I/O error

constexpr IOStatus IO_STATUS_EOF = SDL_IO_STATUS_EOF; ///< End of file

constexpr IOStatus IO_STATUS_NOT_READY =
  SDL_IO_STATUS_NOT_READY; ///< Non blocking I/O, not ready

constexpr IOStatus IO_STATUS_READONLY =
  SDL_IO_STATUS_READONLY; ///< Tried to write a read-only buffer

constexpr IOStatus IO_STATUS_WRITEONLY =
  SDL_IO_STATUS_WRITEONLY; ///< Tried to read a write-only buffer

/**
 * Possible `whence` values for IOStream seeking.
 *
 * These map to the same "whence" concept that `fseek` or `lseek` use in the
 * standard C runtime.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using IOWhence = SDL_IOWhence;

constexpr IOWhence IO_SEEK_SET =
  SDL_IO_SEEK_SET; ///< Seek from the beginning of data

constexpr IOWhence IO_SEEK_CUR =
  SDL_IO_SEEK_CUR; ///< Seek relative to current read point

constexpr IOWhence IO_SEEK_END =
  SDL_IO_SEEK_END; ///< Seek relative to the end of data

/**
 * The function pointers that drive an IOStream.
 *
 * Applications can provide this struct to IOStream.Open() to create their own
 * implementation of IOStream. This is not necessarily required, as SDL already
 * offers several common types of I/O streams, via functions like
 * IOStream.FromFile() and IOStream.FromMem().
 *
 * This structure should be initialized using InitInterface()
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa InitInterface
 */
using IOStreamInterface = SDL_IOStreamInterface;

/**
 * The read/write operation structure.
 *
 * This operates as an opaque handle. There are several APIs to create various
 * types of I/O streams, or an app can supply an IOStreamInterface to
 * IOStream.Open() to provide their own stream implementation behind this
 * struct's abstract interface.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class IOStream
{
  IOStreamRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr IOStream() = default;

  /**
   * Constructs from IOStreamParam.
   *
   * @param resource a IOStreamRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit IOStream(const IOStreamRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr IOStream(const IOStream& other) = delete;

  /// Move constructor
  constexpr IOStream(IOStream&& other)
    : IOStream(other.release())
  {
  }

  constexpr IOStream(const IOStreamRef& other) = delete;

  constexpr IOStream(IOStreamRef&& other) = delete;

  /**
   * Use this function to create a new IOStream structure for reading from
   * and/or writing to a named file.
   *
   * The `mode` string is treated roughly the same as in a call to the C
   * library's fopen(), even if SDL doesn't happen to use fopen() behind the
   * scenes.
   *
   * Available `mode` strings:
   *
   * - "r": Open a file for reading. The file must exist.
   * - "w": Create an empty file for writing. If a file with the same name
   *   already exists its content is erased and the file is treated as a new
   *   empty file.
   * - "a": Append to a file. Writing operations append data at the end of the
   *   file. The file is created if it does not exist.
   * - "r+": Open a file for update both reading and writing. The file must
   *   exist.
   * - "w+": Create an empty file for both reading and writing. If a file with
   *   the same name already exists its content is erased and the file is
   *   treated as a new empty file.
   * - "a+": Open a file for reading and appending. All writing operations are
   *   performed at the end of the file, protecting the previous content to be
   *   overwritten. You can reposition (fseek, rewind) the internal pointer to
   *   anywhere in the file for reading, but writing operations will move it
   *   back to the end of file. The file is created if it does not exist.
   *
   * **NOTE**: In order to open a file as a binary file, a "b" character has to
   * be included in the `mode` string. This additional "b" character can either
   * be appended at the end of the string (thus making the following compound
   * modes: "rb", "wb", "ab", "r+b", "w+b", "a+b") or be inserted between the
   * letter and the "+" sign for the mixed modes ("rb+", "wb+", "ab+").
   * Additional characters may follow the sequence, although they should have no
   * effect. For example, "t" is sometimes appended to make explicit the file is
   * a text file.
   *
   * This function supports Unicode filenames, but they must be encoded in UTF-8
   * format, regardless of the underlying operating system.
   *
   * In Android, IOStream.FromFile() can be used to open content:// URIs. As a
   * fallback, IOStream.FromFile() will transparently open a matching filename
   * in the app's `assets`.
   *
   * Closing the IOStream will close SDL's internal file handle.
   *
   * The following properties may be set at creation time by SDL:
   *
   * - `prop::IOStream.WINDOWS_HANDLE_POINTER`: a pointer, that can be cast to a
   *   win32 `HANDLE`, that this IOStream is using to access the filesystem. If
   *   the program isn't running on Windows, or SDL used some other method to
   *   access the filesystem, this property will not be set.
   * - `prop::IOStream.STDIO_FILE_POINTER`: a pointer, that can be cast to a
   *   stdio `FILE *`, that this IOStream is using to access the filesystem. If
   *   SDL used some other method to access the filesystem, this property will
   *   not be set. PLEASE NOTE that if SDL is using a different C runtime than
   *   your app, trying to use this pointer will almost certainly result in a
   *   crash! This is mostly a problem on Windows; make sure you build SDL and
   *   your app with the same compiler and settings to avoid it.
   * - `prop::IOStream.FILE_DESCRIPTOR_NUMBER`: a file descriptor that this
   *   IOStream is using to access the filesystem.
   * - `prop::IOStream.ANDROID_AASSET_POINTER`: a pointer, that can be cast to
   *   an Android NDK `AAsset *`, that this IOStream is using to access the
   *   filesystem. If SDL used some other method to access the filesystem, this
   *   property will not be set.
   *
   * @param file a UTF-8 string representing the filename to open.
   * @param mode an ASCII string representing the mode to be used for opening
   *             the file.
   * @returns a pointer to the IOStream structure that is created or nullptr on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Close
   * @sa IOStream.Flush
   * @sa IOStream.Read
   * @sa IOStream.Seek
   * @sa IOStream.Tell
   * @sa IOStream.Write
   */
  static IOStream FromFile(StringParam file, StringParam mode);

  /**
   * Use this function to prepare a read-write memory buffer for use with
   * IOStream.
   *
   * This function sets up an IOStream struct based on a memory area of a
   * certain size, for both read and write access.
   *
   * This memory buffer is not copied by the IOStream; the pointer you provide
   * must remain valid until you close the stream. Closing the stream will not
   * free the original buffer.
   *
   * If you need to make sure the IOStream never writes to the memory buffer,
   * you should use IOStream.FromConstMem() with a read-only buffer of memory
   * instead.
   *
   * The following properties will be set at creation time by SDL:
   *
   * - `prop::IOStream.MEMORY_POINTER`: this will be the `mem` parameter that
   *   was passed to this function.
   * - `prop::IOStream.MEMORY_SIZE_NUMBER`: this will be the `size` parameter
   *   that was passed to this function.
   *
   * @param mem a pointer to a buffer to feed an IOStream stream.
   * @param size the buffer size, in bytes.
   * @returns a pointer to a new IOStream structure or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.FromConstMem
   * @sa IOStream.Close
   * @sa IOStream.Flush
   * @sa IOStream.Read
   * @sa IOStream.Seek
   * @sa IOStream.Tell
   * @sa IOStream.Write
   */
  static IOStream FromMem(TargetBytes mem);

  /**
   * Use this function to prepare a read-only memory buffer for use with
   * IOStream.
   *
   * This function sets up an IOStream struct based on a memory area of a
   * certain size. It assumes the memory area is not writable.
   *
   * Attempting to write to this IOStream stream will report an error without
   * writing to the memory buffer.
   *
   * This memory buffer is not copied by the IOStream; the pointer you provide
   * must remain valid until you close the stream. Closing the stream will not
   * free the original buffer.
   *
   * If you need to write to a memory buffer, you should use IOStream.FromMem()
   * with a writable buffer of memory instead.
   *
   * The following properties will be set at creation time by SDL:
   *
   * - `prop::IOStream.MEMORY_POINTER`: this will be the `mem` parameter that
   *   was passed to this function.
   * - `prop::IOStream.MEMORY_SIZE_NUMBER`: this will be the `size` parameter
   *   that was passed to this function.
   *
   * @param mem a pointer to a read-only buffer to feed an IOStream stream.
   * @param size the buffer size, in bytes.
   * @returns a pointer to a new IOStream structure or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.FromMem
   * @sa IOStream.Close
   * @sa IOStream.Read
   * @sa IOStream.Seek
   * @sa IOStream.Tell
   */
  static IOStream FromConstMem(SourceBytes mem);

  /**
   * Use this function to create an IOStream that is backed by dynamically
   * allocated memory.
   *
   * This supports the following properties to provide access to the memory and
   * control over allocations:
   *
   * - `prop::IOStream.DYNAMIC_MEMORY_POINTER`: a pointer to the internal memory
   *   of the stream. This can be set to nullptr to transfer ownership of the
   *   memory to the application, which should free the memory with free(). If
   *   this is done, the next operation on the stream must be IOStream.Close().
   * - `prop::IOStream.DYNAMIC_CHUNKSIZE_NUMBER`: memory will be allocated in
   *   multiples of this size, defaulting to 1024.
   *
   * @returns a pointer to a new IOStream structure or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Close
   * @sa IOStream.Read
   * @sa IOStream.Seek
   * @sa IOStream.Tell
   * @sa IOStream.Write
   */
  static IOStream FromDynamicMem();

  /**
   * Create a custom IOStream.
   *
   * Applications do not need to use this function unless they are providing
   * their own IOStream implementation. If you just need an IOStream to
   * read/write a common data source, you should use the built-in
   * implementations in SDL, like IOStream.FromFile() or IOStream.FromMem(),
   * etc.
   *
   * This function makes a copy of `iface` and the caller does not need to keep
   * it around after this call.
   *
   * @param iface the interface that implements this IOStream, initialized using
   *              InitInterface().
   * @param userdata the pointer that will be passed to the interface functions.
   * @returns a pointer to the allocated memory on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Close
   * @sa InitInterface
   * @sa IOStream.FromConstMem
   * @sa IOStream.FromFile
   * @sa IOStream.FromMem
   */
  static IOStream Open(const IOStreamInterface& iface, void* userdata);

  /// Destructor
  ~IOStream() { SDL_CloseIO(m_resource); }

  /// Assignment operator.
  IOStream& operator=(IOStream other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying IOStreamRaw.
  constexpr IOStreamRaw get() const { return m_resource; }

  /// Retrieves underlying IOStreamRaw and clear this.
  constexpr IOStreamRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const IOStream& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to IOStreamParam
  constexpr operator IOStreamParam() const { return {m_resource}; }

  /**
   * Close and free an allocated IOStream structure.
   *
   * IOStream.Close() closes and cleans up the IOStream stream. It releases any
   * resources used by the stream and frees the IOStream itself. This returns
   * true on success, or false if the stream failed to flush to its output (e.g.
   * to disk).
   *
   * Note that if this fails to flush the stream for any reason, this function
   * reports an error, but the IOStream is still invalid once this function
   * returns.
   *
   * This call flushes any buffered writes to the operating system, but there
   * are no guarantees that those writes have gone to physical media; they might
   * be in the OS's file cache, waiting to go to disk later. If it's absolutely
   * crucial that writes go to disk immediately, so they are definitely stored
   * even if the power fails before the file cache would have caught up, one
   * should call IOStream.Flush() before closing. Note that flushing takes time
   * and makes the system and your app operate less efficiently, so do so
   * sparingly.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Open
   */
  void Close();

  /**
   * Get the properties associated with an IOStream.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const;

  /**
   * Query the stream status of an IOStream.
   *
   * This information can be useful to decide if a short read or write was due
   * to an error, an EOF, or a non-blocking operation that isn't yet ready to
   * complete.
   *
   * An IOStream's status is only expected to change after a IOStream.Read or
   * IOStream.Write call; don't expect it to change if you just call this query
   * function in a tight loop.
   *
   * @returns an IOStatus enum with the current state.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  IOStatus GetStatus() const;

  /**
   * Use this function to get the size of the data stream in an IOStream.
   *
   * @returns the size of the data stream in the IOStream on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint64 GetSize() const;

  /**
   * Seek within an IOStream data stream.
   *
   * This function seeks to byte `offset`, relative to `whence`.
   *
   * `whence` may be any of the following values:
   *
   * - `IO_SEEK_SET`: seek from the beginning of data
   * - `IO_SEEK_CUR`: seek relative to current read point
   * - `IO_SEEK_END`: seek relative to the end of data
   *
   * If this stream can not seek, it will return -1.
   *
   * @param offset an offset in bytes, relative to `whence` location; can be
   *               negative.
   * @param whence any of `IO_SEEK_SET`, `IO_SEEK_CUR`, `IO_SEEK_END`.
   * @returns the final offset in the data stream after the seek or -1 on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Tell
   */
  Sint64 Seek(Sint64 offset, IOWhence whence);

  /**
   * Determine the current read/write offset in an IOStream data stream.
   *
   * IOStream.Tell is actually a wrapper function that calls the IOStream's
   * `seek` method, with an offset of 0 bytes from `IO_SEEK_CUR`, to simplify
   * application development.
   *
   * @returns the current offset in the stream, or -1 if the information can not
   *          be determined.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Seek
   */
  Sint64 Tell() const;

  std::string Read(size_t size = -1)
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Read from a data source.
   *
   * This function reads up `size` bytes from the data source to the area
   * pointed at by `ptr`. This function may read less bytes than requested.
   *
   * This function will return zero when the data stream is completely read, and
   * IOStream.GetStatus() will return IO_STATUS_EOF. If zero is returned and the
   * stream is not at EOF, IOStream.GetStatus() will return a different error
   * value and GetError() will offer a human-readable message.
   *
   * @param ptr a pointer to a buffer to read data into.
   * @param size the number of bytes to read from the data source.
   * @returns the number of bytes read, or 0 on end of file or other failure;
   *          call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Write
   * @sa IOStream.GetStatus
   */
  size_t Read(TargetBytes buf);

  /**
   * Write to an IOStream data stream.
   *
   * This function writes exactly `size` bytes from the area pointed at by `ptr`
   * to the stream. If this fails for any reason, it'll return less than `size`
   * to demonstrate how far the write progressed. On success, it returns `size`.
   *
   * On error, this function still attempts to write as much as possible, so it
   * might return a positive value less than the requested write size.
   *
   * The caller can use IOStream.GetStatus() to determine if the problem is
   * recoverable, such as a non-blocking write that can simply be retried later,
   * or a fatal error.
   *
   * @param ptr a pointer to a buffer containing data to write.
   * @param size the number of bytes to write.
   * @returns the number of bytes written, which will be less than `size` on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.printf
   * @sa IOStream.Read
   * @sa IOStream.Seek
   * @sa IOStream.Flush
   * @sa IOStream.GetStatus
   */
  size_t Write(SourceBytes buf);

  /// @cat formatted-string
  size_t print(std::string_view fmt, auto... args)
  {
    static_assert(false, "Not implemented");
  }

  /// @cat formatted-string
  size_t println(std::string_view fmt, auto... args)
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Print to an IOStream data stream.
   *
   * This function does formatted printing to the stream.
   *
   * @param fmt a printf() style format string.
   * @param ... additional parameters matching % tokens in the `fmt` string, if
   *            any.
   * @returns the number of bytes written or 0 on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.vprintf
   * @sa IOStream.Write
   */
  size_t printf(SDL_PRINTF_FORMAT_STRING const char* fmt, ...)
  {
    return SDL_IOprintf(m_resource, fmt, ...);
  }

  /**
   * Print to an IOStream data stream.
   *
   * This function does formatted printing to the stream.
   *
   * @param fmt a printf() style format string.
   * @param ap a variable argument list.
   * @returns the number of bytes written or 0 on failure; call GetError() for
   *          more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.printf
   * @sa IOStream.Write
   */
  size_t vprintf(SDL_PRINTF_FORMAT_STRING const char* fmt, va_list ap);

  /**
   * Flush any buffered data in the stream.
   *
   * This function makes sure that any buffered data is written to the stream.
   * Normally this isn't necessary but if the stream is a pipe or socket it
   * guarantees that any pending data is sent.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Open
   * @sa IOStream.Write
   */
  void Flush();

  /**
   * Load all the data from an SDL data stream.
   *
   * The data is allocated with a zero byte at the end (null terminated) for
   * convenience. This extra byte is not included in the value reported via
   * `datasize`.
   *
   * The data should be freed with free().
   *
   * @param datasize a pointer filled in with the number of bytes read, may be
   *                 nullptr.
   * @param closeio if true, calls IOStream.Close() on `src` before returning,
   *                even in the case of an error.
   * @returns the data or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LoadFile
   * @sa IOStream.SaveFile
   */
  StringResult LoadFile();

  template<class T>
  OwnArray<T> LoadFileAs()
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Save all the data into an SDL data stream.
   *
   * @param data the data to be written. If datasize is 0, may be nullptr or a
   *             invalid pointer.
   * @param datasize the number of bytes to be written.
   * @param closeio if true, calls IOStream.Close() on `src` before returning,
   *                even in the case of an error.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SaveFile
   * @sa IOStream.LoadFile
   */
  void SaveFile(SourceBytes data);

  /**
   * Use this function to read a byte from an IOStream.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the IOStream to read from.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint8 ReadU8();

  /**
   * Use this function to read a signed byte from an IOStream.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the IOStream to read from.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint8 ReadS8();

  /**
   * Use this function to read 16 bits of little-endian data from an IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint16 ReadU16LE();

  /**
   * Use this function to read 16 bits of little-endian data from an IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint16 ReadS16LE();

  /**
   * Use this function to read 16 bits of big-endian data from an IOStream and
   * return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint16 ReadU16BE();

  /**
   * Use this function to read 16 bits of big-endian data from an IOStream and
   * return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint16 ReadS16BE();

  /**
   * Use this function to read 32 bits of little-endian data from an IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint32 ReadU32LE();

  /**
   * Use this function to read 32 bits of little-endian data from an IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint32 ReadS32LE();

  /**
   * Use this function to read 32 bits of big-endian data from an IOStream and
   * return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint32 ReadU32BE();

  /**
   * Use this function to read 32 bits of big-endian data from an IOStream and
   * return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint32 ReadS32BE();

  /**
   * Use this function to read 64 bits of little-endian data from an IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint64 ReadU64LE();

  /**
   * Use this function to read 64 bits of little-endian data from an IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint64 ReadS64LE();

  /**
   * Use this function to read 64 bits of big-endian data from an IOStream and
   * return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint64 ReadU64BE();

  /**
   * Use this function to read 64 bits of big-endian data from an IOStream and
   * return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStream.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param src the stream from which to read data.
   * @param value a pointer filled in with the data read.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint64 ReadS64BE();

  std::optional<Uint8> TryReadU8() { static_assert(false, "Not implemented"); }

  std::optional<Sint8> TryReadS8() { static_assert(false, "Not implemented"); }

  std::optional<Uint16> TryReadU16LE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Sint16> TryReadS16LE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Uint16> TryReadU16BE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Sint16> TryReadS16BE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Uint32> TryReadU32LE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Sint32> TryReadS32LE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Uint32> TryReadU32BE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Sint32> TryReadS32BE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Uint64> TryReadU64LE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Sint64> TryReadS64LE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Uint64> TryReadU64BE()
  {
    static_assert(false, "Not implemented");
  }

  std::optional<Sint64> TryReadS64BE()
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Use this function to write a byte to an IOStream.
   *
   * @param value the byte value to write.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteU8(Uint8 value);

  /**
   * Use this function to write a signed byte to an IOStream.
   *
   * @param value the byte value to write.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteS8(Sint8 value);

  /**
   * Use this function to write 16 bits in native format to an IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteU16LE(Uint16 value);

  /**
   * Use this function to write 16 bits in native format to an IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteS16LE(Sint16 value);

  /**
   * Use this function to write 16 bits in native format to an IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteU16BE(Uint16 value);

  /**
   * Use this function to write 16 bits in native format to an IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteS16BE(Sint16 value);

  /**
   * Use this function to write 32 bits in native format to an IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteU32LE(Uint32 value);

  /**
   * Use this function to write 32 bits in native format to an IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteS32LE(Sint32 value);

  /**
   * Use this function to write 32 bits in native format to an IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteU32BE(Uint32 value);

  /**
   * Use this function to write 32 bits in native format to an IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteS32BE(Sint32 value);

  /**
   * Use this function to write 64 bits in native format to an IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteU64LE(Uint64 value);

  /**
   * Use this function to write 64 bits in native format to an IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteS64LE(Sint64 value);

  /**
   * Use this function to write 64 bits in native format to an IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteU64BE(Uint64 value);

  /**
   * Use this function to write 64 bits in native format to an IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteS64BE(Sint64 value);
};

/// Semi-safe reference for IOStream.
struct IOStreamRef : IOStream
{
  /**
   * Constructs from IOStreamParam.
   *
   * @param resource a IOStreamRaw or IOStream.
   *
   * This does not takes ownership!
   */
  IOStreamRef(IOStreamParam resource)
    : IOStream(resource.value)
  {
  }

  /// Copy constructor.
  IOStreamRef(const IOStreamRef& other)
    : IOStream(other.get())
  {
  }

  /// Destructor
  ~IOStreamRef() { release(); }
};

/**
 * Use this function to create a new IOStream structure for reading from and/or
 * writing to a named file.
 *
 * The `mode` string is treated roughly the same as in a call to the C library's
 * fopen(), even if SDL doesn't happen to use fopen() behind the scenes.
 *
 * Available `mode` strings:
 *
 * - "r": Open a file for reading. The file must exist.
 * - "w": Create an empty file for writing. If a file with the same name already
 *   exists its content is erased and the file is treated as a new empty file.
 * - "a": Append to a file. Writing operations append data at the end of the
 *   file. The file is created if it does not exist.
 * - "r+": Open a file for update both reading and writing. The file must exist.
 * - "w+": Create an empty file for both reading and writing. If a file with the
 *   same name already exists its content is erased and the file is treated as a
 *   new empty file.
 * - "a+": Open a file for reading and appending. All writing operations are
 *   performed at the end of the file, protecting the previous content to be
 *   overwritten. You can reposition (fseek, rewind) the internal pointer to
 *   anywhere in the file for reading, but writing operations will move it back
 *   to the end of file. The file is created if it does not exist.
 *
 * **NOTE**: In order to open a file as a binary file, a "b" character has to be
 * included in the `mode` string. This additional "b" character can either be
 * appended at the end of the string (thus making the following compound modes:
 * "rb", "wb", "ab", "r+b", "w+b", "a+b") or be inserted between the letter and
 * the "+" sign for the mixed modes ("rb+", "wb+", "ab+"). Additional characters
 * may follow the sequence, although they should have no effect. For example,
 * "t" is sometimes appended to make explicit the file is a text file.
 *
 * This function supports Unicode filenames, but they must be encoded in UTF-8
 * format, regardless of the underlying operating system.
 *
 * In Android, IOStream.FromFile() can be used to open content:// URIs. As a
 * fallback, IOStream.FromFile() will transparently open a matching filename in
 * the app's `assets`.
 *
 * Closing the IOStream will close SDL's internal file handle.
 *
 * The following properties may be set at creation time by SDL:
 *
 * - `prop::IOStream.WINDOWS_HANDLE_POINTER`: a pointer, that can be cast to a
 *   win32 `HANDLE`, that this IOStream is using to access the filesystem. If
 *   the program isn't running on Windows, or SDL used some other method to
 *   access the filesystem, this property will not be set.
 * - `prop::IOStream.STDIO_FILE_POINTER`: a pointer, that can be cast to a stdio
 *   `FILE *`, that this IOStream is using to access the filesystem. If SDL used
 *   some other method to access the filesystem, this property will not be set.
 *   PLEASE NOTE that if SDL is using a different C runtime than your app,
 *   trying to use this pointer will almost certainly result in a crash! This is
 *   mostly a problem on Windows; make sure you build SDL and your app with the
 *   same compiler and settings to avoid it.
 * - `prop::IOStream.FILE_DESCRIPTOR_NUMBER`: a file descriptor that this
 *   IOStream is using to access the filesystem.
 * - `prop::IOStream.ANDROID_AASSET_POINTER`: a pointer, that can be cast to an
 *   Android NDK `AAsset *`, that this IOStream is using to access the
 *   filesystem. If SDL used some other method to access the filesystem, this
 *   property will not be set.
 *
 * @param file a UTF-8 string representing the filename to open.
 * @param mode an ASCII string representing the mode to be used for opening the
 *             file.
 * @returns a pointer to the IOStream structure that is created or nullptr on
 *          failure; call GetError() for more information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.Close
 * @sa IOStream.Flush
 * @sa IOStream.Read
 * @sa IOStream.Seek
 * @sa IOStream.Tell
 * @sa IOStream.Write
 */
inline IOStream IOFromFile(StringParam file, StringParam mode)
{
  return IOStream(SDL_IOFromFile(file, mode));
}

inline IOStream IOStream::FromFile(StringParam file, StringParam mode)
{
  return SDL::IOFromFile(std::move(file), std::move(mode));
}

namespace prop::IOStream {

constexpr auto WINDOWS_HANDLE_POINTER =
  SDL_PROP_IOSTREAM_WINDOWS_HANDLE_POINTER;

constexpr auto STDIO_FILE_POINTER = SDL_PROP_IOSTREAM_STDIO_FILE_POINTER;

constexpr auto FILE_DESCRIPTOR_NUMBER =
  SDL_PROP_IOSTREAM_FILE_DESCRIPTOR_NUMBER;

constexpr auto ANDROID_AASSET_POINTER =
  SDL_PROP_IOSTREAM_ANDROID_AASSET_POINTER;

constexpr auto MEMORY_POINTER = SDL_PROP_IOSTREAM_MEMORY_POINTER;

constexpr auto MEMORY_SIZE_NUMBER = SDL_PROP_IOSTREAM_MEMORY_SIZE_NUMBER;

constexpr auto DYNAMIC_MEMORY_POINTER =
  SDL_PROP_IOSTREAM_DYNAMIC_MEMORY_POINTER;

constexpr auto DYNAMIC_CHUNKSIZE_NUMBER =
  SDL_PROP_IOSTREAM_DYNAMIC_CHUNKSIZE_NUMBER;

} // namespace prop::IOStream

/**
 * Use this function to prepare a read-write memory buffer for use with
 * IOStream.
 *
 * This function sets up an IOStream struct based on a memory area of a certain
 * size, for both read and write access.
 *
 * This memory buffer is not copied by the IOStream; the pointer you provide
 * must remain valid until you close the stream. Closing the stream will not
 * free the original buffer.
 *
 * If you need to make sure the IOStream never writes to the memory buffer, you
 * should use IOStream.FromConstMem() with a read-only buffer of memory instead.
 *
 * The following properties will be set at creation time by SDL:
 *
 * - `prop::IOStream.MEMORY_POINTER`: this will be the `mem` parameter that was
 *   passed to this function.
 * - `prop::IOStream.MEMORY_SIZE_NUMBER`: this will be the `size` parameter that
 *   was passed to this function.
 *
 * @param mem a pointer to a buffer to feed an IOStream stream.
 * @param size the buffer size, in bytes.
 * @returns a pointer to a new IOStream structure or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.FromConstMem
 * @sa IOStream.Close
 * @sa IOStream.Flush
 * @sa IOStream.Read
 * @sa IOStream.Seek
 * @sa IOStream.Tell
 * @sa IOStream.Write
 */
inline IOStream IOFromMem(void* mem, size_t size)
{
  return IOStream(SDL_IOFromMem(mem, size));
}

inline IOStream IOStream::FromMem(TargetBytes mem)
{
  return SDL::IOFromMem(std::move(mem));
}

/**
 * Use this function to prepare a read-only memory buffer for use with IOStream.
 *
 * This function sets up an IOStream struct based on a memory area of a certain
 * size. It assumes the memory area is not writable.
 *
 * Attempting to write to this IOStream stream will report an error without
 * writing to the memory buffer.
 *
 * This memory buffer is not copied by the IOStream; the pointer you provide
 * must remain valid until you close the stream. Closing the stream will not
 * free the original buffer.
 *
 * If you need to write to a memory buffer, you should use IOStream.FromMem()
 * with a writable buffer of memory instead.
 *
 * The following properties will be set at creation time by SDL:
 *
 * - `prop::IOStream.MEMORY_POINTER`: this will be the `mem` parameter that was
 *   passed to this function.
 * - `prop::IOStream.MEMORY_SIZE_NUMBER`: this will be the `size` parameter that
 *   was passed to this function.
 *
 * @param mem a pointer to a read-only buffer to feed an IOStream stream.
 * @param size the buffer size, in bytes.
 * @returns a pointer to a new IOStream structure or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.FromMem
 * @sa IOStream.Close
 * @sa IOStream.Read
 * @sa IOStream.Seek
 * @sa IOStream.Tell
 */
inline IOStream IOFromConstMem(const void* mem, size_t size)
{
  return IOStream(SDL_IOFromConstMem(mem, size));
}

inline IOStream IOStream::FromConstMem(SourceBytes mem)
{
  return SDL::IOFromConstMem(std::move(mem));
}

/**
 * Use this function to create an IOStream that is backed by dynamically
 * allocated memory.
 *
 * This supports the following properties to provide access to the memory and
 * control over allocations:
 *
 * - `prop::IOStream.DYNAMIC_MEMORY_POINTER`: a pointer to the internal memory
 *   of the stream. This can be set to nullptr to transfer ownership of the
 *   memory to the application, which should free the memory with free(). If
 *   this is done, the next operation on the stream must be IOStream.Close().
 * - `prop::IOStream.DYNAMIC_CHUNKSIZE_NUMBER`: memory will be allocated in
 *   multiples of this size, defaulting to 1024.
 *
 * @returns a pointer to a new IOStream structure or nullptr on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.Close
 * @sa IOStream.Read
 * @sa IOStream.Seek
 * @sa IOStream.Tell
 * @sa IOStream.Write
 */
inline IOStream IOFromDynamicMem() { return IOStream(SDL_IOFromDynamicMem()); }

inline IOStream IOStream::FromDynamicMem() { return SDL::IOFromDynamicMem(); }

/**
 * Create a custom IOStream.
 *
 * Applications do not need to use this function unless they are providing their
 * own IOStream implementation. If you just need an IOStream to read/write a
 * common data source, you should use the built-in implementations in SDL, like
 * IOStream.FromFile() or IOStream.FromMem(), etc.
 *
 * This function makes a copy of `iface` and the caller does not need to keep it
 * around after this call.
 *
 * @param iface the interface that implements this IOStream, initialized using
 *              InitInterface().
 * @param userdata the pointer that will be passed to the interface functions.
 * @returns a pointer to the allocated memory on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.Close
 * @sa InitInterface
 * @sa IOStream.FromConstMem
 * @sa IOStream.FromFile
 * @sa IOStream.FromMem
 */
inline IOStream OpenIO(const IOStreamInterface& iface, void* userdata)
{
  return IOStream(CheckError(SDL_OpenIO(&iface, userdata)));
}

inline IOStream IOStream::Open(const IOStreamInterface& iface, void* userdata)
{
  return SDL::OpenIO(iface, userdata);
}

/**
 * Close and free an allocated IOStream structure.
 *
 * IOStream.Close() closes and cleans up the IOStream stream. It releases any
 * resources used by the stream and frees the IOStream itself. This returns true
 * on success, or false if the stream failed to flush to its output (e.g. to
 * disk).
 *
 * Note that if this fails to flush the stream for any reason, this function
 * reports an error, but the IOStream is still invalid once this function
 * returns.
 *
 * This call flushes any buffered writes to the operating system, but there are
 * no guarantees that those writes have gone to physical media; they might be in
 * the OS's file cache, waiting to go to disk later. If it's absolutely crucial
 * that writes go to disk immediately, so they are definitely stored even if the
 * power fails before the file cache would have caught up, one should call
 * IOStream.Flush() before closing. Note that flushing takes time and makes the
 * system and your app operate less efficiently, so do so sparingly.
 *
 * @param context IOStream structure to close.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.Open
 */
inline void CloseIO(IOStreamRaw context) { CheckError(SDL_CloseIO(context)); }

inline void IOStream::Close() { CloseIO(release()); }

/**
 * Get the properties associated with an IOStream.
 *
 * @param context a pointer to an IOStream structure.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetIOProperties(IOStreamParam context)
{
  return CheckError(SDL_GetIOProperties(context));
}

inline PropertiesRef IOStream::GetProperties() const
{
  return SDL::GetIOProperties(m_resource);
}

/**
 * Query the stream status of an IOStream.
 *
 * This information can be useful to decide if a short read or write was due to
 * an error, an EOF, or a non-blocking operation that isn't yet ready to
 * complete.
 *
 * An IOStream's status is only expected to change after a IOStream.Read or
 * IOStream.Write call; don't expect it to change if you just call this query
 * function in a tight loop.
 *
 * @param context the IOStream to query.
 * @returns an IOStatus enum with the current state.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline IOStatus GetIOStatus(IOStreamParam context)
{
  return SDL_GetIOStatus(context);
}

inline IOStatus IOStream::GetStatus() const
{
  return SDL::GetIOStatus(m_resource);
}

/**
 * Use this function to get the size of the data stream in an IOStream.
 *
 * @param context the IOStream to get the size of the data stream from.
 * @returns the size of the data stream in the IOStream on success.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sint64 GetIOSize(IOStreamParam context)
{
  return CheckError(SDL_GetIOSize(context));
}

inline Sint64 IOStream::GetSize() const { return SDL::GetIOSize(m_resource); }

/**
 * Seek within an IOStream data stream.
 *
 * This function seeks to byte `offset`, relative to `whence`.
 *
 * `whence` may be any of the following values:
 *
 * - `IO_SEEK_SET`: seek from the beginning of data
 * - `IO_SEEK_CUR`: seek relative to current read point
 * - `IO_SEEK_END`: seek relative to the end of data
 *
 * If this stream can not seek, it will return -1.
 *
 * @param context a pointer to an IOStream structure.
 * @param offset an offset in bytes, relative to `whence` location; can be
 *               negative.
 * @param whence any of `IO_SEEK_SET`, `IO_SEEK_CUR`, `IO_SEEK_END`.
 * @returns the final offset in the data stream after the seek or -1 on failure;
 *          call GetError() for more information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.Tell
 */
inline Sint64 SeekIO(IOStreamParam context, Sint64 offset, IOWhence whence)
{
  return SDL_SeekIO(context, offset, whence);
}

inline Sint64 IOStream::Seek(Sint64 offset, IOWhence whence)
{
  return SDL::SeekIO(m_resource, offset, whence);
}

/**
 * Determine the current read/write offset in an IOStream data stream.
 *
 * IOStream.Tell is actually a wrapper function that calls the IOStream's `seek`
 * method, with an offset of 0 bytes from `IO_SEEK_CUR`, to simplify application
 * development.
 *
 * @param context an IOStream data stream object from which to get the current
 *                offset.
 * @returns the current offset in the stream, or -1 if the information can not
 *          be determined.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.Seek
 */
inline Sint64 TellIO(IOStreamParam context) { return SDL_TellIO(context); }

inline Sint64 IOStream::Tell() const { return SDL::TellIO(m_resource); }

/**
 * Read from a data source.
 *
 * This function reads up `size` bytes from the data source to the area pointed
 * at by `ptr`. This function may read less bytes than requested.
 *
 * This function will return zero when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If zero is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param context a pointer to an IOStream structure.
 * @param ptr a pointer to a buffer to read data into.
 * @param size the number of bytes to read from the data source.
 * @returns the number of bytes read, or 0 on end of file or other failure; call
 *          GetError() for more information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.Write
 * @sa IOStream.GetStatus
 */
inline size_t ReadIO(IOStreamParam context, TargetBytes buf)
{
  return SDL_ReadIO(context, buf.data(), buf.size_bytes());
}

inline size_t IOStream::Read(TargetBytes buf)
{
  return SDL::ReadIO(m_resource, std::move(buf));
}

/**
 * Write to an IOStream data stream.
 *
 * This function writes exactly `size` bytes from the area pointed at by `ptr`
 * to the stream. If this fails for any reason, it'll return less than `size` to
 * demonstrate how far the write progressed. On success, it returns `size`.
 *
 * On error, this function still attempts to write as much as possible, so it
 * might return a positive value less than the requested write size.
 *
 * The caller can use IOStream.GetStatus() to determine if the problem is
 * recoverable, such as a non-blocking write that can simply be retried later,
 * or a fatal error.
 *
 * @param context a pointer to an IOStream structure.
 * @param ptr a pointer to a buffer containing data to write.
 * @param size the number of bytes to write.
 * @returns the number of bytes written, which will be less than `size` on
 *          failure; call GetError() for more information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.printf
 * @sa IOStream.Read
 * @sa IOStream.Seek
 * @sa IOStream.Flush
 * @sa IOStream.GetStatus
 */
inline size_t WriteIO(IOStreamParam context, SourceBytes buf)
{
  return SDL_WriteIO(context, buf.data(), buf.size_bytes());
}

inline size_t IOStream::Write(SourceBytes buf)
{
  return SDL::WriteIO(m_resource, std::move(buf));
}

/**
 * Print to an IOStream data stream.
 *
 * This function does formatted printing to the stream.
 *
 * @param context a pointer to an IOStream structure.
 * @param fmt a printf() style format string.
 * @param ... additional parameters matching % tokens in the `fmt` string, if
 *            any.
 * @returns the number of bytes written or 0 on failure; call GetError() for
 *          more information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.vprintf
 * @sa IOStream.Write
 */
inline size_t IOprintf(IOStreamParam context,
                       SDL_PRINTF_FORMAT_STRING const char* fmt)
{
  return SDL_IOprintf(context, fmt);
}

/**
 * Print to an IOStream data stream.
 *
 * This function does formatted printing to the stream.
 *
 * @param context a pointer to an IOStream structure.
 * @param fmt a printf() style format string.
 * @param ap a variable argument list.
 * @returns the number of bytes written or 0 on failure; call GetError() for
 *          more information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.printf
 * @sa IOStream.Write
 */
inline size_t IOvprintf(IOStreamParam context,
                        SDL_PRINTF_FORMAT_STRING const char* fmt,
                        va_list ap)
{
  return SDL_IOvprintf(context, fmt, ap);
}

inline size_t IOStream::vprintf(SDL_PRINTF_FORMAT_STRING const char* fmt,
                                va_list ap)
{
  return SDL::IOvprintf(m_resource, fmt, ap);
}

/**
 * Flush any buffered data in the stream.
 *
 * This function makes sure that any buffered data is written to the stream.
 * Normally this isn't necessary but if the stream is a pipe or socket it
 * guarantees that any pending data is sent.
 *
 * @param context IOStream structure to flush.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.Open
 * @sa IOStream.Write
 */
inline void FlushIO(IOStreamParam context) { CheckError(SDL_FlushIO(context)); }

inline void IOStream::Flush() { SDL::FlushIO(m_resource); }

/**
 * Load all the data from an SDL data stream.
 *
 * The data is allocated with a zero byte at the end (null terminated) for
 * convenience. This extra byte is not included in the value reported via
 * `datasize`.
 *
 * The data should be freed with free().
 *
 * @param src the IOStream to read all available data from.
 * @param datasize a pointer filled in with the number of bytes read, may be
 *                 nullptr.
 * @param closeio if true, calls IOStream.Close() on `src` before returning,
 *                even in the case of an error.
 * @returns the data or nullptr on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LoadFile
 * @sa IOStream.SaveFile
 */
inline StringResult LoadFile(IOStreamParam src, bool closeio = true)
{
  return SDL_LoadFile_IO(src, closeio);
}

/**
 * Load all the data from a file path.
 *
 * The data is allocated with a zero byte at the end (null terminated) for
 * convenience. This extra byte is not included in the value reported via
 * `datasize`.
 *
 * The data should be freed with free().
 *
 * @param file the path to read all available data from.
 * @param datasize if not nullptr, will store the number of bytes read.
 * @returns the data or nullptr on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.LoadFile
 * @sa SaveFile
 */
inline StringResult LoadFile(StringParam file) { return SDL_LoadFile(file); }

inline StringResult IOStream::LoadFile() { return SDL::LoadFile(m_resource); }

template<class T>
inline OwnArray<T> LoadFileAs(StringParam file)
{
  static_assert(false, "Not implemented");
}

/**
 * Save all the data into an SDL data stream.
 *
 * @param src the IOStream to write all data to.
 * @param data the data to be written. If datasize is 0, may be nullptr or a
 *             invalid pointer.
 * @param datasize the number of bytes to be written.
 * @param closeio if true, calls IOStream.Close() on `src` before returning,
 *                even in the case of an error.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SaveFile
 * @sa IOStream.LoadFile
 */
inline void SaveFile(IOStreamParam src, SourceBytes data, bool closeio = true)
{
  CheckError(SDL_SaveFile_IO(src, data.data(), data.size_bytes(), closeio));
}

/**
 * Save all the data into a file path.
 *
 * @param file the path to write all available data into.
 * @param data the data to be written. If datasize is 0, may be nullptr or a
 *             invalid pointer.
 * @param datasize the number of bytes to be written.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.SaveFile
 * @sa LoadFile
 */
inline void SaveFile(StringParam file, SourceBytes data)
{
  CheckError(SDL_SaveFile(file, data.data(), data.size_bytes()));
}

inline void IOStream::SaveFile(SourceBytes data)
{
  SDL::SaveFile(m_resource, std::move(data));
}

/**
 * Use this function to read a byte from an IOStream.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the IOStream to read from.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint8 ReadU8(IOStreamParam src) { return CheckError(SDL_ReadU8(src)); }

inline Uint8 IOStream::ReadU8() { return SDL::ReadU8(m_resource); }

/**
 * Use this function to read a signed byte from an IOStream.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the IOStream to read from.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sint8 ReadS8(IOStreamParam src) { return CheckError(SDL_ReadS8(src)); }

inline Sint8 IOStream::ReadS8() { return SDL::ReadS8(m_resource); }

/**
 * Use this function to read 16 bits of little-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint16 ReadU16LE(IOStreamParam src)
{
  return CheckError(SDL_ReadU16LE(src));
}

inline Uint16 IOStream::ReadU16LE() { return SDL::ReadU16LE(m_resource); }

/**
 * Use this function to read 16 bits of little-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sint16 ReadS16LE(IOStreamParam src)
{
  return CheckError(SDL_ReadS16LE(src));
}

inline Sint16 IOStream::ReadS16LE() { return SDL::ReadS16LE(m_resource); }

/**
 * Use this function to read 16 bits of big-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint16 ReadU16BE(IOStreamParam src)
{
  return CheckError(SDL_ReadU16BE(src));
}

inline Uint16 IOStream::ReadU16BE() { return SDL::ReadU16BE(m_resource); }

/**
 * Use this function to read 16 bits of big-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sint16 ReadS16BE(IOStreamParam src)
{
  return CheckError(SDL_ReadS16BE(src));
}

inline Sint16 IOStream::ReadS16BE() { return SDL::ReadS16BE(m_resource); }

/**
 * Use this function to read 32 bits of little-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint32 ReadU32LE(IOStreamParam src)
{
  return CheckError(SDL_ReadU32LE(src));
}

inline Uint32 IOStream::ReadU32LE() { return SDL::ReadU32LE(m_resource); }

/**
 * Use this function to read 32 bits of little-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sint32 ReadS32LE(IOStreamParam src)
{
  return CheckError(SDL_ReadS32LE(src));
}

inline Sint32 IOStream::ReadS32LE() { return SDL::ReadS32LE(m_resource); }

/**
 * Use this function to read 32 bits of big-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint32 ReadU32BE(IOStreamParam src)
{
  return CheckError(SDL_ReadU32BE(src));
}

inline Uint32 IOStream::ReadU32BE() { return SDL::ReadU32BE(m_resource); }

/**
 * Use this function to read 32 bits of big-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sint32 ReadS32BE(IOStreamParam src)
{
  return CheckError(SDL_ReadS32BE(src));
}

inline Sint32 IOStream::ReadS32BE() { return SDL::ReadS32BE(m_resource); }

/**
 * Use this function to read 64 bits of little-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint64 ReadU64LE(IOStreamParam src)
{
  return CheckError(SDL_ReadU64LE(src));
}

inline Uint64 IOStream::ReadU64LE() { return SDL::ReadU64LE(m_resource); }

/**
 * Use this function to read 64 bits of little-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sint64 ReadS64LE(IOStreamParam src)
{
  return CheckError(SDL_ReadS64LE(src));
}

inline Sint64 IOStream::ReadS64LE() { return SDL::ReadS64LE(m_resource); }

/**
 * Use this function to read 64 bits of big-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint64 ReadU64BE(IOStreamParam src)
{
  return CheckError(SDL_ReadU64BE(src));
}

inline Uint64 IOStream::ReadU64BE() { return SDL::ReadU64BE(m_resource); }

/**
 * Use this function to read 64 bits of big-endian data from an IOStream and
 * return in native format.
 *
 * SDL byteswaps the data only if necessary, so the data returned will be in the
 * native byte order.
 *
 * This function will return false when the data stream is completely read, and
 * IOStream.GetStatus() will return IO_STATUS_EOF. If false is returned and the
 * stream is not at EOF, IOStream.GetStatus() will return a different error
 * value and GetError() will offer a human-readable message.
 *
 * @param src the stream from which to read data.
 * @param value a pointer filled in with the data read.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sint64 ReadS64BE(IOStreamParam src)
{
  return CheckError(SDL_ReadS64BE(src));
}

inline Sint64 IOStream::ReadS64BE() { return SDL::ReadS64BE(m_resource); }

/**
 * Use this function to write a byte to an IOStream.
 *
 * @param dst the IOStream to write to.
 * @param value the byte value to write.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteU8(IOStreamParam dst, Uint8 value)
{
  CheckError(SDL_WriteU8(dst, value));
}

inline void IOStream::WriteU8(Uint8 value) { SDL::WriteU8(m_resource, value); }

/**
 * Use this function to write a signed byte to an IOStream.
 *
 * @param dst the IOStream to write to.
 * @param value the byte value to write.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteS8(IOStreamParam dst, Sint8 value)
{
  CheckError(SDL_WriteS8(dst, value));
}

inline void IOStream::WriteS8(Sint8 value) { SDL::WriteS8(m_resource, value); }

/**
 * Use this function to write 16 bits in native format to an IOStream as
 * little-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in little-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteU16LE(IOStreamParam dst, Uint16 value)
{
  CheckError(SDL_WriteU16LE(dst, value));
}

inline void IOStream::WriteU16LE(Uint16 value)
{
  SDL::WriteU16LE(m_resource, value);
}

/**
 * Use this function to write 16 bits in native format to an IOStream as
 * little-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in little-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteS16LE(IOStreamParam dst, Sint16 value)
{
  CheckError(SDL_WriteS16LE(dst, value));
}

inline void IOStream::WriteS16LE(Sint16 value)
{
  SDL::WriteS16LE(m_resource, value);
}

/**
 * Use this function to write 16 bits in native format to an IOStream as
 * big-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in big-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteU16BE(IOStreamParam dst, Uint16 value)
{
  CheckError(SDL_WriteU16BE(dst, value));
}

inline void IOStream::WriteU16BE(Uint16 value)
{
  SDL::WriteU16BE(m_resource, value);
}

/**
 * Use this function to write 16 bits in native format to an IOStream as
 * big-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in big-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteS16BE(IOStreamParam dst, Sint16 value)
{
  CheckError(SDL_WriteS16BE(dst, value));
}

inline void IOStream::WriteS16BE(Sint16 value)
{
  SDL::WriteS16BE(m_resource, value);
}

/**
 * Use this function to write 32 bits in native format to an IOStream as
 * little-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in little-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteU32LE(IOStreamParam dst, Uint32 value)
{
  CheckError(SDL_WriteU32LE(dst, value));
}

inline void IOStream::WriteU32LE(Uint32 value)
{
  SDL::WriteU32LE(m_resource, value);
}

/**
 * Use this function to write 32 bits in native format to an IOStream as
 * little-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in little-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteS32LE(IOStreamParam dst, Sint32 value)
{
  CheckError(SDL_WriteS32LE(dst, value));
}

inline void IOStream::WriteS32LE(Sint32 value)
{
  SDL::WriteS32LE(m_resource, value);
}

/**
 * Use this function to write 32 bits in native format to an IOStream as
 * big-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in big-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteU32BE(IOStreamParam dst, Uint32 value)
{
  CheckError(SDL_WriteU32BE(dst, value));
}

inline void IOStream::WriteU32BE(Uint32 value)
{
  SDL::WriteU32BE(m_resource, value);
}

/**
 * Use this function to write 32 bits in native format to an IOStream as
 * big-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in big-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteS32BE(IOStreamParam dst, Sint32 value)
{
  CheckError(SDL_WriteS32BE(dst, value));
}

inline void IOStream::WriteS32BE(Sint32 value)
{
  SDL::WriteS32BE(m_resource, value);
}

/**
 * Use this function to write 64 bits in native format to an IOStream as
 * little-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in little-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteU64LE(IOStreamParam dst, Uint64 value)
{
  CheckError(SDL_WriteU64LE(dst, value));
}

inline void IOStream::WriteU64LE(Uint64 value)
{
  SDL::WriteU64LE(m_resource, value);
}

/**
 * Use this function to write 64 bits in native format to an IOStream as
 * little-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in little-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteS64LE(IOStreamParam dst, Sint64 value)
{
  CheckError(SDL_WriteS64LE(dst, value));
}

inline void IOStream::WriteS64LE(Sint64 value)
{
  SDL::WriteS64LE(m_resource, value);
}

/**
 * Use this function to write 64 bits in native format to an IOStream as
 * big-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in big-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteU64BE(IOStreamParam dst, Uint64 value)
{
  CheckError(SDL_WriteU64BE(dst, value));
}

inline void IOStream::WriteU64BE(Uint64 value)
{
  SDL::WriteU64BE(m_resource, value);
}

/**
 * Use this function to write 64 bits in native format to an IOStream as
 * big-endian data.
 *
 * SDL byteswaps the data only if necessary, so the application always specifies
 * native format, and the data written will be in big-endian format.
 *
 * @param dst the stream to which data will be written.
 * @param value the data to be written, in native format.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void WriteS64BE(IOStreamParam dst, Sint64 value)
{
  CheckError(SDL_WriteS64BE(dst, value));
}

inline void IOStream::WriteS64BE(Sint64 value)
{
  SDL::WriteS64BE(m_resource, value);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_IOSTREAM_H_ */
