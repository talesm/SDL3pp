#ifndef SDL3PP_IOSTREAM_H_
#define SDL3PP_IOSTREAM_H_

#include <SDL3/SDL_iostream.h>
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryIOStream I/O Streams
 *
 * SDL provides an abstract interface for reading and writing data streams. It
 * offers implementations for files, memory, etc, and the app can provide
 * their own implementations, too.
 *
 * SDL_IOStream is not related to the standard C++ iostream class, other than
 * both are abstract interfaces to read/write data.
 *
 * @{
 */

/**
 * @cat constructor-tag
 */
constexpr struct IOFromDynamicMem_CtorTag
{
} IOFromDynamicMem;

// Forward decl
struct IOStreamBase;

// Forward decl
struct IOStreamRef;

// Forward decl
struct IOStream;

/**
 * IOStreamBase status, set by a read or write operation.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using IOStatus = SDL_IOStatus;

constexpr IOStatus IO_STATUS_READY =
  SDL_IO_STATUS_READY; ///< Everything is ready (no errors and not EOF).

constexpr IOStatus IO_STATUS_ERROR =
  SDL_IO_STATUS_ERROR; ///< Read or write I/O error.

constexpr IOStatus IO_STATUS_EOF = SDL_IO_STATUS_EOF; ///< End of file.

constexpr IOStatus IO_STATUS_NOT_READY =
  SDL_IO_STATUS_NOT_READY; ///< Non blocking I/O, not ready.

constexpr IOStatus IO_STATUS_READONLY =
  SDL_IO_STATUS_READONLY; ///< Tried to write a read-only buffer.

constexpr IOStatus IO_STATUS_WRITEONLY =
  SDL_IO_STATUS_WRITEONLY; ///< Tried to read a write-only buffer.

/**
 * Possible `whence` values for IOStreamBase seeking.
 *
 * These map to the same "whence" concept that `fseek` or `lseek` use in the
 * standard C runtime.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using IOWhence = SDL_IOWhence;

constexpr IOWhence IO_SEEK_SET =
  SDL_IO_SEEK_SET; ///< Seek from the beginning of data.

constexpr IOWhence IO_SEEK_CUR =
  SDL_IO_SEEK_CUR; ///< Seek relative to current read point.

constexpr IOWhence IO_SEEK_END =
  SDL_IO_SEEK_END; ///< Seek relative to the end of data.

/**
 * The function pointers that drive an IOStreamBase.
 *
 * Applications can provide this struct to IOStreamBase.IOStreamBase() to create
 * their own implementation of IOStreamBase. This is not necessarily required,
 * as SDL already offers several common types of I/O streams, via
 * IOStreamBase.IOStreamBase().
 *
 * This structure should be initialized using SDL_INIT_INTERFACE()
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa SDL_INIT_INTERFACE
 */
using IOStreamInterface = SDL_IOStreamInterface;

/**
 * The read/write operation structure.
 *
 * This operates as an opaque handle. There are several APIs to create various
 * types of I/O streams, or an app can supply an IOStreamInterface to
 * IOStreamBase.IOStreamBase() to provide their own stream implementation behind
 * this struct's abstract interface.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa IOStream
 * @sa IOStreamRef
 */
struct IOStreamBase : Resource<SDL_IOStream*>
{
  using Resource::Resource;

  /**
   * Use this function to create a new SDL_IOStream structure for reading from
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
   * In Android, IOStreamBase.IOStreamBase() can be used to open content://
   * URIs. As a fallback, IOStreamBase.IOStreamBase() will transparently open a
   * matching filename in the app's `assets`.
   *
   * Closing the IOStreamBase will close SDL's internal file handle.
   *
   * The following properties may be set at creation time by SDL:
   *
   * - `prop::IOStream.WINDOWS_HANDLE_POINTER`: a pointer, that can be cast
   *   to a win32 `HANDLE`, that this IOStreamBase is using to access the
   *   filesystem. If the program isn't running on Windows, or SDL used some
   *   other method to access the filesystem, this property will not be set.
   * - `prop::IOStream.STDIO_FILE_POINTER`: a pointer, that can be cast to a
   *   stdio `FILE *`, that this IOStreamBase is using to access the filesystem.
   *   If SDL used some other method to access the filesystem, this property
   *   will not be set. PLEASE NOTE that if SDL is using a different C runtime
   *   than your app, trying to use this pointer will almost certainly result in
   *   a crash! This is mostly a problem on Windows; make sure you build SDL and
   *   your app with the same compiler and settings to avoid it.
   * - `prop::IOStream.FILE_DESCRIPTOR_NUMBER`: a file descriptor that this
   *   IOStreamBase is using to access the filesystem.
   * - `prop::IOStream.ANDROID_AASSET_POINTER`: a pointer, that can be cast
   *   to an Android NDK `AAsset *`, that this IOStreamBase is using to access
   *   the filesystem. If SDL used some other method to access the filesystem,
   *   this property will not be set.
   *
   * @param file a UTF-8 string representing the filename to open.
   * @param mode an ASCII string representing the mode to be used for opening
   *             the file.
   * @post the object is convertible to true if valid or false on failure; call
   *       GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamRef.Close
   * @sa IOStreamBase.Flush
   * @sa IOStreamBase.Read
   * @sa IOStreamBase.Seek
   * @sa IOStreamBase.Tell
   * @sa IOStreamBase.Write
   */
  IOStreamBase(StringParam file, StringParam mode)
    : Resource(SDL_IOFromFile(file, mode))
  {
  }

  /**
   * Use this function to prepare a read-write memory buffer for use with
   * IOStreamBase.
   *
   * This function sets up an IOStreamBase struct based on a memory area of a
   * certain size, for both read and write access.
   *
   * This memory buffer is not copied by the IOStreamBase; the pointer you
   * provide must remain valid until you close the stream. Closing the stream
   * will not free the original buffer.
   *
   * If you need to make sure the IOStreamBase never writes to the memory
   * buffer, you should use IOStreamBase.IOStreamBase() with a read-only buffer
   * of memory instead.
   *
   * The following properties will be set at creation time by SDL:
   *
   * - `prop::IOStream.MEMORY_POINTER`: this will be the `mem` parameter that
   *   was passed to this function.
   * - `prop::IOStream.MEMORY_SIZE_NUMBER`: this will be the `size` parameter
   *   that was passed to this function.
   *
   * @param mem a pointer to a buffer to feed an IOStreamBase stream.
   * @param size the buffer size, in bytes.
   * @post the object is convertible to true if valid or false on failure; call
   *       GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.IOStreamBase
   * @sa IOStreamRef.Close
   * @sa IOStreamBase.Flush
   * @sa IOStreamBase.Read
   * @sa IOStreamBase.Seek
   * @sa IOStreamBase.Tell
   * @sa IOStreamBase.Write
   */
  IOStreamBase(void* mem, size_t size)
    : Resource(SDL_IOFromMem(mem, size))
  {
  }

  /**
   * Use this function to prepare a read-only memory buffer for use with
   * IOStreamBase.
   *
   * This function sets up an IOStreamBase struct based on a memory area of a
   * certain size. It assumes the memory area is not writable.
   *
   * Attempting to write to this IOStreamBase stream will report an error
   * without writing to the memory buffer.
   *
   * This memory buffer is not copied by the IOStreamBase; the pointer you
   * provide must remain valid until you close the stream. Closing the stream
   * will not free the original buffer.
   *
   * If you need to write to a memory buffer, you should use
   * IOStreamBase.IOStreamBase() with a writable buffer of memory instead.
   *
   * The following properties will be set at creation time by SDL:
   *
   * - `prop::IOStream.MEMORY_POINTER`: this will be the `mem` parameter that
   *   was passed to this function.
   * - `prop::IOStream.MEMORY_SIZE_NUMBER`: this will be the `size` parameter
   *   that was passed to this function.
   *
   * @param mem a pointer to a read-only buffer to feed an IOStreamBase stream.
   * @param size the buffer size, in bytes.
   * @post the object is convertible to true if valid or false on failure; call
   *       GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.IOStreamBase
   * @sa IOStreamRef.Close
   * @sa IOStreamBase.Read
   * @sa IOStreamBase.Seek
   * @sa IOStreamBase.Tell
   */
  IOStreamBase(const void* mem, size_t size)
    : Resource(SDL_IOFromConstMem(mem, size))
  {
  }

  /**
   * Use this function to create an IOStreamBase that is backed by dynamically
   * allocated memory.
   *
   * This supports the following properties to provide access to the memory and
   * control over allocations:
   *
   * - `prop::IOStream.DYNAMIC_MEMORY_POINTER`: a pointer to the internal
   *   memory of the stream. This can be set to nullptr to transfer ownership of
   *   the memory to the application, which should free the memory with
   *   free(). If this is done, the next operation on the stream must be
   *   IOStreamRef.Close().
   * - `prop::IOStream.DYNAMIC_CHUNKSIZE_NUMBER`: memory will be allocated in
   *   multiples of this size, defaulting to 1024.
   *
   * @post a pointer to a new IOStreamBase structure or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamRef.Close
   * @sa IOStreamBase.Read
   * @sa IOStreamBase.Seek
   * @sa IOStreamBase.Tell
   * @sa IOStreamBase.Write
   */
  IOStreamBase(IOFromDynamicMem_CtorTag)
    : Resource(SDL_IOFromDynamicMem())
  {
  }

  /**
   * Create a custom IOStreamBase.
   *
   * Applications do not need to use this function unless they are providing
   * their own IOStreamBase implementation. If you just need an IOStreamBase to
   * read/write a common data source, you should use the built-in
   * implementations in SDL, like IOStreamBase.IOStreamBase() or
   * IOStreamBase.IOStreamBase(), etc.
   *
   * This function makes a copy of `iface` and the caller does not need to keep
   * it around after this call.
   *
   * @param iface the interface that implements this IOStreamBase, initialized
   *              using SDL_INIT_INTERFACE().
   * @param userdata the pointer that will be passed to the interface functions.
   * @post the object is convertible to true if valid or false on failure; call
   *       GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamRef.Close
   * @sa SDL_INIT_INTERFACE
   * @sa IOStreamBase.IOStreamBase
   * @sa IOStreamBase.IOStreamBase
   * @sa IOStreamBase.IOStreamBase
   */
  IOStreamBase(const IOStreamInterface& iface, void* userdata)
    : Resource(SDL_OpenIO(&iface, userdata))
  {
  }

  /**
   * Use this function to prepare a memory buffer for use with IOStreamBase.
   *
   * @tparam U
   * @param mem the span of memory to use as buffer. If const we get read-only,
   * otherwise we get a read-write buffer.
   *
   * @post the object is convertible to true if valid or false on failure; call
   *       GetError() for more information.
   */
  template<class U>
  IOStreamBase(std::span<U> mem)
    : IOStreamBase(mem.data(), mem.size_bytes())
  {
  }

  /**
   * Get the properties associated with an IOStreamBase.
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return PropertiesRef{SDL_GetIOProperties(get())};
  }

  /**
   * Query the stream status of an IOStreamBase.
   *
   * This information can be useful to decide if a short read or write was due
   * to an error, an EOF, or a non-blocking operation that isn't yet ready to
   * complete.
   *
   * An IOStreamBase's status is only expected to change after a
   * IOStreamBase.Read or IOStreamBase.Write call; don't expect it to change if
   * you just call this query function in a tight loop.
   *
   * @returns an IOStatus enum with the current state.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  IOStatus GetStatus() const { return SDL_GetIOStatus(get()); }

  /**
   * Use this function to get the size of the data stream in an IOStreamBase.
   *
   * @returns the size of the data stream in the IOStreamBase on success or a
   *          negative error code on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint64 GetSize() const { return SDL_GetIOSize(get()); }

  /**
   * Seek within an IOStreamBase data stream.
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
   * @param whence any of `IO_SEEK_SET`, `IO_SEEK_CUR`,
   *               `IO_SEEK_END`.
   * @returns the final offset in the data stream after the seek or -1 on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.Tell
   */
  Sint64 Seek(Sint64 offset, IOWhence whence)
  {
    return SDL_SeekIO(get(), offset, whence);
  }

  /**
   * Determine the current read/write offset in an IOStreamBase data stream.
   *
   * This is actually a wrapper function that calls the IOStreamBase's `seek`
   * method, with an offset of 0 bytes from `IO_SEEK_CUR`, to simplify
   * application development.
   *
   * @returns the current offset in the stream, or -1 if the information can not
   *          be determined.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.Seek
   */
  Sint64 Tell() const { return SDL_TellIO(get()); }

  /**
   * Read from a data source.
   *
   * This function reads up `size` bytes from the data source to the area
   * pointed at by `ptr`. This function may read less bytes than requested.
   *
   * This function will return zero when the data stream is completely read, and
   * IOStreamBase.GetStatus() will return IO_STATUS_EOF. If zero is returned and
   * the stream is not at EOF, IOStreamBase.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
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
   * @sa IOStreamBase.Write
   * @sa IOStreamBase.GetStatus
   */
  size_t Read(void* ptr, size_t size) { return SDL_ReadIO(get(), ptr, size); }

  /**
   * Write to an IOStreamBase data stream.
   *
   * This function writes exactly `size` bytes from the area pointed at by `ptr`
   * to the stream. If this fails for any reason, it'll return less than `size`
   * to demonstrate how far the write progressed. On success, it returns `size`.
   *
   * On error, this function still attempts to write as much as possible, so it
   * might return a positive value less than the requested write size.
   *
   * The caller can use IOStreamBase.GetStatus() to determine if the problem is
   * recoverable, such as a non-blocking write that can simply be retried later,
   * or a fatal error.
   *
   * @param data the bytes to write to
   * @returns the number of bytes written, which will be less than `size` on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.printf
   * @sa IOStreamBase.Read
   * @sa IOStreamBase.Seek
   * @sa IOStreamBase.Flush
   * @sa IOStreamBase.GetStatus
   */
  template<class U>
  size_t Write(std::span<U> data)
  {
    return Write(data.data(), data.size_bytes());
  }

  /**
   * Write to an IOStreamBase data stream.
   *
   * This function writes exactly `size` bytes from the area pointed at by `ptr`
   * to the stream. If this fails for any reason, it'll return less than `size`
   * to demonstrate how far the write progressed. On success, it returns `size`.
   *
   * On error, this function still attempts to write as much as possible, so it
   * might return a positive value less than the requested write size.
   *
   * The caller can use IOStreamBase.GetStatus() to determine if the problem is
   * recoverable, such as a non-blocking write that can simply be retried later,
   * or a fatal error.
   *
   * @param str the bytes to write to
   * @returns the number of bytes written, which will be less than `size` on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.printf
   * @sa IOStreamBase.Read
   * @sa IOStreamBase.Seek
   * @sa IOStreamBase.Flush
   * @sa IOStreamBase.GetStatus
   */
  size_t Write(std::string_view str) { return Write(str.data(), str.size()); }

  /**
   * Write to an IOStreamBase data stream.
   *
   * This function writes exactly `size` bytes from the area pointed at by `ptr`
   * to the stream. If this fails for any reason, it'll return less than `size`
   * to demonstrate how far the write progressed. On success, it returns `size`.
   *
   * On error, this function still attempts to write as much as possible, so it
   * might return a positive value less than the requested write size.
   *
   * The caller can use IOStreamBase.GetStatus() to determine if the problem is
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
   * @sa IOStreamBase.printf
   * @sa IOStreamBase.Read
   * @sa IOStreamBase.Seek
   * @sa IOStreamBase.Flush
   * @sa IOStreamBase.GetStatus
   */
  size_t Write(const void* ptr, size_t size)
  {
    return SDL_WriteIO(get(), ptr, size);
  }

  /**
   * @cat formatted-string
   */
  size_t print(std::string_view fmt, auto... args)
  {
    return Write(std::vformat(fmt, std::make_format_args(args...)));
  }

  /**
   * @cat formatted-string
   */
  size_t println(std::string_view fmt, auto... args)
  {
    std::string result =
      std::vformat(fmt, std::make_format_args(args...)) + "\n";
    return Write(result);
  }

  /**
   * Print to an IOStreamBase data stream.
   *
   * @warning this is not typesafe! Prefer using print() and println()
   *
   * This function does formatted printing to the stream.
   *
   * @param fmt a printf() style format string.
   * @param ... additional parameters matching % tokens in the `fmt` string, if
   *            any.
   * @returns the number of bytes written or 0 on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.vprintf
   * @sa IOStreamBase.Write
   */
  size_t printf(SDL_PRINTF_FORMAT_STRING const char* fmt, ...)
  {
    va_list ap;
    size_t result;

    va_start(ap, fmt);
    result = vprintf(fmt, ap);
    va_end(ap);

    return result;
  }

  /**
   * Print to an IOStreamBase data stream.
   *
   * @warning this is not typesafe! Prefer using print() and println()
   *
   * This function does formatted printing to the stream.
   *
   * @param fmt a printf() style format string.
   * @param ap a variable argument list.
   * @returns the number of bytes written or 0 on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.printf
   * @sa IOStreamBase.Write
   */
  size_t vprintf(SDL_PRINTF_FORMAT_STRING const char* fmt, va_list ap)
  {
    return SDL_IOvprintf(get(), fmt, ap);
  }

  /**
   * Flush any buffered data in the stream.
   *
   * This function makes sure that any buffered data is written to the stream.
   * Normally this isn't necessary but if the stream is a pipe or socket it
   * guarantees that any pending data is sent.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.IOStreamBase
   * @sa IOStreamBase.Write
   */
  bool Flush() { return SDL_FlushIO(get()); }

  /**
   * Load all the data from an SDL data stream.
   *
   * The data is allocated with a zero byte at the end (null terminated) for
   * convenience. This extra byte is not included in the value reported via
   * `datasize`.
   *
   * @returns the data or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LoadFile
   * @sa IOStreamBase.SaveFile
   */
  OwnArray<std::byte> LoadFile()
  {
    size_t datasize = 0;
    auto data =
      static_cast<std::byte*>(SDL_LoadFile_IO(get(), &datasize, false));
    return OwnArray<std::byte>{data, datasize};
  }

  /**
   * Save all the data into an SDL data stream.
   *
   * @param data the data to be written. If datasize is 0, may be nullptr or a
   *             invalid pointer.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SaveFile
   * @sa IOStreamBase.LoadFile
   */
  template<class U>
  bool SaveFile(std::span<U> data)
  {
    return SaveFile(data.data(), data.size_bytes());
  }

  /**
   * Save all the data into an SDL data stream.
   *
   * @param str the data to be written. If datasize is 0, may be nullptr or a
   *            invalid pointer.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SaveFile
   * @sa IOStreamBase.LoadFile
   */
  bool SaveFile(std::string_view str)
  {
    return SaveFile(str.data(), str.size());
  }

  /**
   * Save all the data into an SDL data stream.
   *
   * @param data the data to be written. If datasize is 0, may be nullptr or a
   *             invalid pointer.
   * @param datasize the number of bytes to be written.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SaveFile
   * @sa IOStreamBase.LoadFile
   */
  bool SaveFile(const void* data, size_t datasize)
  {
    return SDL_SaveFile_IO(get(), data, datasize, false);
  }

  /**
   * Use this function to read a byte from an IOStreamBase.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on success or false on failure or EOF; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU8(Uint8* value) { return SDL_ReadU8(get(), value); }

  /**
   * Use this function to read a signed byte from an IOStreamBase.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS8(Sint8* value) { return SDL_ReadS8(get(), value); }

  /**
   * Use this function to read 16 bits of little-endian data from an
   * IOStreamBase and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU16LE(Uint16* value) { return SDL_ReadU16LE(get(), value); }

  /**
   * Use this function to read 16 bits of little-endian data from an
   * IOStreamBase and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS16LE(Sint16* value) { return SDL_ReadS16LE(get(), value); }

  /**
   * Use this function to read 16 bits of big-endian data from an IOStreamBase
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU16BE(Uint16* value) { return SDL_ReadU16BE(get(), value); }

  /**
   * Use this function to read 16 bits of big-endian data from an IOStreamBase
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS16BE(Sint16* value) { return SDL_ReadS16BE(get(), value); }

  /**
   * Use this function to read 32 bits of little-endian data from an
   * IOStreamBase and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU32LE(Uint32* value) { return SDL_ReadU32LE(get(), value); }

  /**
   * Use this function to read 32 bits of little-endian data from an
   * IOStreamBase and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS32LE(Sint32* value) { return SDL_ReadS32LE(get(), value); }

  /**
   * Use this function to read 32 bits of big-endian data from an IOStreamBase
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU32BE(Uint32* value) { return SDL_ReadU32BE(get(), value); }

  /**
   * Use this function to read 32 bits of big-endian data from an IOStreamBase
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS32BE(Sint32* value) { return SDL_ReadS32BE(get(), value); }

  /**
   * Use this function to read 64 bits of little-endian data from an
   * IOStreamBase and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU64LE(Uint64* value) { return SDL_ReadU64LE(get(), value); }

  /**
   * Use this function to read 64 bits of little-endian data from an
   * IOStreamBase and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS64LE(Sint64* value) { return SDL_ReadS64LE(get(), value); }

  /**
   * Use this function to read 64 bits of big-endian data from an IOStreamBase
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU64BE(Uint64* value) { return SDL_ReadU64BE(get(), value); }

  /**
   * Use this function to read 64 bits of big-endian data from an IOStreamBase
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamBase.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamBase.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS64BE(Sint64* value) { return SDL_ReadS64BE(get(), value); }

  /**
   * Use this function to write a byte to an IOStreamBase.
   *
   * @param value the byte value to write.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU8(Uint8 value) { return SDL_WriteU8(get(), value); }

  /**
   * Use this function to write a signed byte to an IOStreamBase.
   *
   * @param value the byte value to write.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS8(Sint8 value) { return SDL_WriteS8(get(), value); }

  /**
   * Use this function to write 16 bits in native format to an IOStreamBase as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU16LE(Uint16 value) { return SDL_WriteU16LE(get(), value); }

  /**
   * Use this function to write 16 bits in native format to an IOStreamBase as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS16LE(Sint16 value) { return SDL_WriteS16LE(get(), value); }

  /**
   * Use this function to write 16 bits in native format to an IOStreamBase as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU16BE(Uint16 value) { return SDL_WriteU16BE(get(), value); }

  /**
   * Use this function to write 16 bits in native format to an IOStreamBase as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS16BE(Sint16 value) { return SDL_WriteS16BE(get(), value); }

  /**
   * Use this function to write 32 bits in native format to an IOStreamBase as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU32LE(Uint32 value) { return SDL_WriteU32LE(get(), value); }

  /**
   * Use this function to write 32 bits in native format to an IOStreamBase as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS32LE(Sint32 value) { return SDL_WriteS32LE(get(), value); }

  /**
   * Use this function to write 32 bits in native format to an IOStreamBase as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU32BE(Uint32 value) { return SDL_WriteU32BE(get(), value); }

  /**
   * Use this function to write 32 bits in native format to an IOStreamBase as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS32BE(Sint32 value) { return SDL_WriteS32BE(get(), value); }

  /**
   * Use this function to write 64 bits in native format to an IOStreamBase as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU64LE(Uint64 value) { return SDL_WriteU64LE(get(), value); }

  /**
   * Use this function to write 64 bits in native format to an IOStreamBase as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS64LE(Sint64 value) { return SDL_WriteS64LE(get(), value); }

  /**
   * Use this function to write 64 bits in native format to an IOStreamBase as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU64BE(Uint64 value) { return SDL_WriteU64BE(get(), value); }

  /**
   * Use this function to write 64 bits in native format to an IOStreamBase as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS64BE(Sint64 value) { return SDL_WriteS64BE(get(), value); }
};

/**
 * Handle to a non owned iOStream
 *
 * @cat resource
 *
 * @sa IOStreamBase
 * @sa IOStream
 */
struct IOStreamRef : IOStreamBase
{
  using IOStreamBase::IOStreamBase;

  /**
   * Copy constructor.
   */
  constexpr IOStreamRef(const IOStreamRef& other)
    : IOStreamBase(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr IOStreamRef(IOStreamRef&& other)
    : IOStreamBase(other.release())
  {
  }

  /**
   * Default constructor
   */
  constexpr ~IOStreamRef() = default;

  /**
   * Assignment operator.
   */
  IOStreamRef& operator=(IOStreamRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Close and free an allocated IOStreamBase structure.
   *
   * IOStreamRef.Close() closes and cleans up the IOStreamBase stream. It
   * releases any resources used by the stream and frees the IOStreamBase
   * itself. This returns true on success, or false if the stream failed to
   * flush to its output (e.g. to disk).
   *
   * Note that if this fails to flush the stream for any reason, this function
   * reports an error, but the IOStreamBase is still invalid once this function
   * returns.
   *
   * This call flushes any buffered writes to the operating system, but there
   * are no guarantees that those writes have gone to physical media; they might
   * be in the OS's file cache, waiting to go to disk later. If it's absolutely
   * crucial that writes go to disk immediately, so they are definitely stored
   * even if the power fails before the file cache would have caught up, one
   * should call IOStreamBase.Flush() before closing. Note that flushing takes
   * time and makes the system and your app operate less efficiently, so do so
   * sparingly.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.IOStreamBase
   */
  bool reset(SDL_IOStream* newResource = {})
  {
    return SDL_CloseIO(release(newResource));
  }

  /**
   * Close and free an allocated IOStreamBase structure.
   *
   * IOStreamRef.Close() closes and cleans up the IOStreamBase stream. It
   * releases any resources used by the stream and frees the IOStreamBase
   * itself. This returns true on success, or false if the stream failed to
   * flush to its output (e.g. to disk).
   *
   * Note that if this fails to flush the stream for any reason, this function
   * reports an error, but the IOStreamBase is still invalid once this function
   * returns.
   *
   * This call flushes any buffered writes to the operating system, but there
   * are no guarantees that those writes have gone to physical media; they might
   * be in the OS's file cache, waiting to go to disk later. If it's absolutely
   * crucial that writes go to disk immediately, so they are definitely stored
   * even if the power fails before the file cache would have caught up, one
   * should call IOStreamBase.Flush() before closing. Note that flushing takes
   * time and makes the system and your app operate less efficiently, so do so
   * sparingly.
   *
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamBase.IOStreamBase
   */
  bool Close() { return reset(); }
};

/**
 * Handle to an owned iOStream
 *
 * @cat resource
 *
 * @sa IOStreamBase
 * @sa IOStreamRef
 */
struct IOStream : IOStreamRef
{
  using IOStreamRef::IOStreamRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit IOStream(SDL_IOStream* resource = {})
    : IOStreamRef(resource)
  {
  }

  constexpr IOStream(const IOStream& other) = delete;

  /**
   * Move constructor.
   */
  constexpr IOStream(IOStream&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~IOStream() { reset(); }

  /**
   * Assignment operator.
   */
  IOStream& operator=(IOStream other)
  {
    reset(other.release());
    return *this;
  }
};

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
 * Load all the data from a file path.
 *
 * The data is allocated with a zero byte at the end (null terminated) for
 * convenience. This extra byte is not included in the value reported via
 * `datasize`.
 *
 * @param file the path to read all available data from.
 * @returns the data or nullptr on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStreamBase.LoadFile
 * @sa SaveFile
 */
inline OwnArray<std::byte> LoadFile(StringParam file)
{
  size_t datasize = 0;
  auto data = static_cast<std::byte*>(SDL_LoadFile(file, &datasize));
  return OwnArray<std::byte>{data, datasize};
}

/**
 * Save all the data into a file path.
 *
 * @param file the path to write all available data into.
 * @param data the data to be written. If datasize is 0, may be nullptr or a
 *             invalid pointer.
 * @param datasize the number of bytes to be written.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStreamBase.SaveFile
 * @sa LoadFile
 */
inline bool SaveFile(StringParam file, const void* data, size_t datasize)
{
  return SDL_SaveFile(file, data, datasize);
}

/**
 * Save all the data into a file path.
 *
 * @param file the path to write all available data into.
 * @param data the data to be written.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStreamBase.SaveFile
 * @sa LoadFile
 */
template<class T>
inline bool SaveFile(StringParam file, std::span<T> data)
{
  return SaveFile(file, data.data(), data.size_bytes());
}

/**
 * Save all the data into a file path.
 *
 * @param file the path to write all available data into.
 * @param str the data to be written.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStreamBase.SaveFile
 * @sa LoadFile
 */
inline bool SaveFile(StringParam file, std::string_view str)
{
  return SaveFile(std::move(file), str.data(), str.size());
}

#pragma region impl
/// @}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_IOSTREAM_H_ */
