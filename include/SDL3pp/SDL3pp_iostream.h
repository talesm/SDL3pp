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

// Forward decl
template<ObjectBox<SDL_IOStream*> T>
struct IOStreamBase;

/**
 * Handle to a non owned stream
 *
 * @cat resource
 *
 * @sa resource
 * @sa IOStreamBase
 * @sa IOStream
 */
using IOStreamRef = IOStreamBase<ObjectRef<SDL_IOStream>>;

/**
 * Handle to an owned stream
 *
 * @cat resource
 *
 * @sa resource
 * @sa IOStreamBase
 * @sa IOStreamRef
 */
using IOStream = IOStreamBase<ObjectUnique<SDL_IOStream>>;

/**
 * @cat constructor-tag
 */
constexpr struct IOFromDynamicMem_CtorTag
{
} IOFromDynamicMem;

/**
 * SDL_IOStream status, set by a read or write operation.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using IOStatus = SDL_IOStatus;

/**
 * Everything is ready (no errors and not EOF).
 */
constexpr IOStatus IO_STATUS_READY = SDL_IO_STATUS_READY;

/**
 * Read or write I/O error
 */
constexpr IOStatus IO_STATUS_ERROR = SDL_IO_STATUS_ERROR;

/**
 * End of file
 */
constexpr IOStatus IO_STATUS_EOF = SDL_IO_STATUS_EOF;

/**
 * Non blocking I/O, not ready
 */
constexpr IOStatus IO_STATUS_NOT_READY = SDL_IO_STATUS_NOT_READY;

/**
 * Tried to write a read-only buffer
 */
constexpr IOStatus IO_STATUS_READONLY = SDL_IO_STATUS_READONLY;

/**
 * Tried to read a write-only buffer
 */
constexpr IOStatus IO_STATUS_WRITEONLY = SDL_IO_STATUS_WRITEONLY;

/**
 * Possible `whence` values for SDL_IOStream seeking.
 *
 * These map to the same "whence" concept that `fseek` or `lseek` use in the
 * standard C runtime.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using IOWhence = SDL_IOWhence;

/**
 * Seek from the beginning of data
 */
constexpr IOWhence IO_SEEK_SET = SDL_IO_SEEK_SET;

/**
 * Seek relative to current read point
 */
constexpr IOWhence IO_SEEK_CUR = SDL_IO_SEEK_CUR;

/**
 * Seek relative to the end of data
 */
constexpr IOWhence IO_SEEK_END = SDL_IO_SEEK_END;

/**
 * The function pointers that drive an SDL_IOStream.
 *
 * Applications can provide this struct to SDL_OpenIO() to create their own
 * implementation of SDL_IOStream. This is not necessarily required, as SDL
 * already offers several common types of I/O streams, via functions like
 * SDL_IOFromFile() and SDL_IOFromMem().
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
 * types of I/O streams, or an app can supply an SDL_IOStreamInterface to
 * SDL_OpenIO() to provide their own stream implementation behind this
 * struct's abstract interface.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
template<ObjectBox<SDL_IOStream*> T>
struct IOStreamBase : T
{
  using T::T;

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
   * In Android, SDL_IOFromFile() can be used to open content:// URIs. As a
   * fallback, SDL_IOFromFile() will transparently open a matching filename in
   * the app's `assets`.
   *
   * Closing the SDL_IOStream will close SDL's internal file handle.
   *
   * The following properties may be set at creation time by SDL:
   *
   * - `SDL_PROP_IOSTREAM_WINDOWS_HANDLE_POINTER`: a pointer, that can be cast
   *   to a win32 `HANDLE`, that this SDL_IOStream is using to access the
   *   filesystem. If the program isn't running on Windows, or SDL used some
   *   other method to access the filesystem, this property will not be set.
   * - `SDL_PROP_IOSTREAM_STDIO_FILE_POINTER`: a pointer, that can be cast to a
   *   stdio `FILE *`, that this SDL_IOStream is using to access the filesystem.
   *   If SDL used some other method to access the filesystem, this property
   *   will not be set. PLEASE NOTE that if SDL is using a different C runtime
   *   than your app, trying to use this pointer will almost certainly result in
   *   a crash! This is mostly a problem on Windows; make sure you build SDL and
   *   your app with the same compiler and settings to avoid it.
   * - `SDL_PROP_IOSTREAM_FILE_DESCRIPTOR_NUMBER`: a file descriptor that this
   *   SDL_IOStream is using to access the filesystem.
   * - `SDL_PROP_IOSTREAM_ANDROID_AASSET_POINTER`: a pointer, that can be cast
   *   to an Android NDK `AAsset *`, that this SDL_IOStream is using to access
   *   the filesystem. If SDL used some other method to access the filesystem,
   *   this property will not be set.
   *
   * @param file a UTF-8 string representing the filename to open.
   * @param mode an ASCII string representing the mode to be used for opening
   *             the file.
   * @post the object is convertible to true if valid or false on failure; call
   * GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CloseIO
   * @sa SDL_FlushIO
   * @sa SDL_ReadIO
   * @sa SDL_SeekIO
   * @sa SDL_TellIO
   * @sa SDL_WriteIO
   */
  IOStreamBase(StringParam file, StringParam mode)
    : T(SDL_IOFromFile(file, mode))
  {
  }

  /**
   * Use this function to prepare a read-write memory buffer for use with
   * SDL_IOStream.
   *
   * This function sets up an SDL_IOStream struct based on a memory area of a
   * certain size, for both read and write access.
   *
   * This memory buffer is not copied by the SDL_IOStream; the pointer you
   * provide must remain valid until you close the stream. Closing the stream
   * will not free the original buffer.
   *
   * If you need to make sure the SDL_IOStream never writes to the memory
   * buffer, you should use SDL_IOFromConstMem() with a read-only buffer of
   * memory instead.
   *
   * The following properties will be set at creation time by SDL:
   *
   * - `SDL_PROP_IOSTREAM_MEMORY_POINTER`: this will be the `mem` parameter that
   *   was passed to this function.
   * - `SDL_PROP_IOSTREAM_MEMORY_SIZE_NUMBER`: this will be the `size` parameter
   *   that was passed to this function.
   *
   * @param mem a pointer to a buffer to feed an SDL_IOStream stream.
   * @param size the buffer size, in bytes.
   * @post the object is convertible to true if valid or false on failure; call
   * GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_IOFromConstMem
   * @sa SDL_CloseIO
   * @sa SDL_FlushIO
   * @sa SDL_ReadIO
   * @sa SDL_SeekIO
   * @sa SDL_TellIO
   * @sa SDL_WriteIO
   */
  IOStreamBase(void* mem, size_t size)
    : T(SDL_IOFromMem(mem, size))
  {
  }

  /**
   * Use this function to prepare a read-only memory buffer for use with
   * SDL_IOStream.
   *
   * This function sets up an SDL_IOStream struct based on a memory area of a
   * certain size. It assumes the memory area is not writable.
   *
   * Attempting to write to this SDL_IOStream stream will report an error
   * without writing to the memory buffer.
   *
   * This memory buffer is not copied by the SDL_IOStream; the pointer you
   * provide must remain valid until you close the stream. Closing the stream
   * will not free the original buffer.
   *
   * If you need to write to a memory buffer, you should use SDL_IOFromMem()
   * with a writable buffer of memory instead.
   *
   * The following properties will be set at creation time by SDL:
   *
   * - `SDL_PROP_IOSTREAM_MEMORY_POINTER`: this will be the `mem` parameter that
   *   was passed to this function.
   * - `SDL_PROP_IOSTREAM_MEMORY_SIZE_NUMBER`: this will be the `size` parameter
   *   that was passed to this function.
   *
   * @param mem a pointer to a read-only buffer to feed an SDL_IOStream stream.
   * @param size the buffer size, in bytes.
   * @post the object is convertible to true if valid or false on failure; call
   * GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_IOFromMem
   * @sa SDL_CloseIO
   * @sa SDL_ReadIO
   * @sa SDL_SeekIO
   * @sa SDL_TellIO
   */
  IOStreamBase(const void* mem, size_t size)
    : T(SDL_IOFromConstMem(mem, size))
  {
  }

  /**
   * Use this function to create an SDL_IOStream that is backed by dynamically
   * allocated memory.
   *
   * This supports the following properties to provide access to the memory and
   * control over allocations:
   *
   * - `SDL_PROP_IOSTREAM_DYNAMIC_MEMORY_POINTER`: a pointer to the internal
   *   memory of the stream. This can be set to NULL to transfer ownership of
   *   the memory to the application, which should free the memory with
   *   SDL_free(). If this is done, the next operation on the stream must be
   *   Close().
   * - `SDL_PROP_IOSTREAM_DYNAMIC_CHUNKSIZE_NUMBER`: memory will be allocated in
   *   multiples of this size, defaulting to 1024.
   *
   * @post the object is convertible to true if valid or false on failure; call
   * GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CloseIO
   * @sa SDL_ReadIO
   * @sa SDL_SeekIO
   * @sa SDL_TellIO
   * @sa SDL_WriteIO
   */
  IOStreamBase(IOFromDynamicMem_CtorTag)
    : T(SDL_IOFromDynamicMem())
  {
  }

  /**
   * Create a custom SDL_IOStream.
   *
   * Applications do not need to use this function unless they are providing
   * their own SDL_IOStream implementation. If you just need an SDL_IOStream to
   * read/write a common data source, you should use the built-in
   * implementations in SDL, like SDL_IOFromFile() or SDL_IOFromMem(), etc.
   *
   * This function makes a copy of `iface` and the caller does not need to keep
   * it around after this call.
   *
   * @param iface the interface that implements this SDL_IOStream, initialized
   *              using SDL_INIT_INTERFACE().
   * @param userdata the pointer that will be passed to the interface functions.
   * @post the object is convertible to true if valid or false on failure; call
   * GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CloseIO
   * @sa SDL_INIT_INTERFACE
   * @sa SDL_IOFromConstMem
   * @sa SDL_IOFromFile
   * @sa SDL_IOFromMem
   */
  IOStreamBase(const IOStreamInterface* iface, void* userdata)
    : T(SDL_OpenIO(iface, userdata))
  {
  }

  template<class U>
  IOStreamBase(std::span<U> mem)
    : IOStreamBase(mem.data(), mem.size_bytes())
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Get the properties associated with an SDL_IOStream.
   *
   * @returns a valid property ID on success or 0 on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const { return SDL_GetIOProperties(T::get()); }

  /**
   * Query the stream status of an SDL_IOStream.
   *
   * This information can be useful to decide if a short read or write was due
   * to an error, an EOF, or a non-blocking operation that isn't yet ready to
   * complete.
   *
   * An SDL_IOStream's status is only expected to change after a SDL_ReadIO or
   * SDL_WriteIO call; don't expect it to change if you just call this query
   * function in a tight loop.
   *
   * @returns an SDL_IOStatus enum with the current state.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  IOStatus GetStatus() const { return SDL_GetIOStatus(T::get()); }

  /**
   * Use this function to get the size of the data stream in an SDL_IOStream.
   *
   * @returns the size of the data stream in the SDL_IOStream on success or a
   *          negative error code on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint64 GetSize() const { return SDL_GetIOSize(T::get()); }

  /**
   * Seek within an SDL_IOStream data stream.
   *
   * This function seeks to byte `offset`, relative to `whence`.
   *
   * `whence` may be any of the following values:
   *
   * - `SDL_IO_SEEK_SET`: seek from the beginning of data
   * - `SDL_IO_SEEK_CUR`: seek relative to current read point
   * - `SDL_IO_SEEK_END`: seek relative to the end of data
   *
   * If this stream can not seek, it will return -1.
   *
   * @param offset an offset in bytes, relative to `whence` location; can be
   *               negative.
   * @param whence any of `SDL_IO_SEEK_SET`, `SDL_IO_SEEK_CUR`,
   *               `SDL_IO_SEEK_END`.
   * @returns the final offset in the data stream after the seek or -1 on
   *          failure; call SDL_GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_TellIO
   */
  Sint64 Seek(Sint64 offset, IOWhence whence)
  {
    return SDL_SeekIO(T::get(), offset, whence);
  }

  /**
   * Determine the current read/write offset in an SDL_IOStream data stream.
   *
   * SDL_TellIO is actually a wrapper function that calls the SDL_IOStream's
   * `seek` method, with an offset of 0 bytes from `SDL_IO_SEEK_CUR`, to
   * simplify application development.
   *
   * @returns the current offset in the stream, or -1 if the information can not
   *          be determined.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_SeekIO
   */
  Sint64 Tell() const { return SDL_TellIO(T::get()); }

  /**
   * Read from a data source.
   *
   * This function reads up `size` bytes from the data source to the area
   * pointed at by `ptr`. This function may read less bytes than requested.
   *
   * This function will return zero when the data stream is completely read, and
   * SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If zero is returned and
   * the stream is not at EOF, SDL_GetIOStatus() will return a different error
   * value and SDL_GetError() will offer a human-readable message.
   *
   * @param ptr a pointer to a buffer to read data into.
   * @param size the number of bytes to read from the data source.
   * @returns the number of bytes read, or 0 on end of file or other failure;
   *          call SDL_GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_WriteIO
   * @sa SDL_GetIOStatus
   */
  size_t Read(void* ptr, size_t size)
  {
    return SDL_ReadIO(T::get(), ptr, size);
  }

  template<class U>
  size_t Write(std::span<U> data)
  {
    return Write(data.data(), data.size_bytes());
  }

  size_t Write(std::string_view str) { return Write(str.data(), str.size()); }

  /**
   * Write to an SDL_IOStream data stream.
   *
   * This function writes exactly `size` bytes from the area pointed at by `ptr`
   * to the stream. If this fails for any reason, it'll return less than `size`
   * to demonstrate how far the write progressed. On success, it returns `size`.
   *
   * On error, this function still attempts to write as much as possible, so it
   * might return a positive value less than the requested write size.
   *
   * The caller can use SDL_GetIOStatus() to determine if the problem is
   * recoverable, such as a non-blocking write that can simply be retried later,
   * or a fatal error.
   *
   * @param ptr a pointer to a buffer containing data to write.
   * @param size the number of bytes to write.
   * @returns the number of bytes written, which will be less than `size` on
   *          failure; call SDL_GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_IOprintf
   * @sa SDL_ReadIO
   * @sa SDL_SeekIO
   * @sa SDL_FlushIO
   * @sa SDL_GetIOStatus
   */
  size_t Write(const void* ptr, size_t size)
  {
    return SDL_WriteIO(T::get(), ptr, size);
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
   * Print to an SDL_IOStream data stream.
   *
   * @warning this is not typesafe! Prefer using print() and println()
   *
   * This function does formatted printing to the stream.
   *
   * @param fmt a printf() style format string.
   * @param ... additional parameters matching % tokens in the `fmt` string, if
   *            any.
   * @returns the number of bytes written or 0 on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_IOvprintf
   * @sa SDL_WriteIO
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
   * Print to an SDL_IOStream data stream.
   *
   * @warning this is not typesafe! Prefer using print() and println()
   *
   * This function does formatted printing to the stream.
   *
   * @param fmt a printf() style format string.
   * @param ap a variable argument list.
   * @returns the number of bytes written or 0 on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_IOprintf
   * @sa SDL_WriteIO
   */
  size_t vprintf(SDL_PRINTF_FORMAT_STRING const char* fmt, va_list ap)
  {
    return SDL_IOvprintf(T::get(), fmt, ap);
  }

  /**
   * Flush any buffered data in the stream.
   *
   * This function makes sure that any buffered data is written to the stream.
   * Normally this isn't necessary but if the stream is a pipe or socket it
   * guarantees that any pending data is sent.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_OpenIO
   * @sa SDL_WriteIO
   */
  bool Flush() { return SDL_FlushIO(T::get()); }

  /**
   * Load all the data from an SDL data stream.
   *
   * The data is allocated with a zero byte at the end (null terminated) for
   * convenience. This extra byte is not included in the value reported via
   * `datasize`.
   *
   * @returns the data or NULL on failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_LoadFile
   * @sa SDL_SaveFile_IO
   */
  OwnArray<std::byte> LoadFile()
  {
    size_t datasize = 0;
    auto data =
      static_cast<std::byte*>(SDL_LoadFile_IO(T::get(), &datasize, false));
    return OwnArray<std::byte>{data, datasize};
  }

  template<class U>
  bool SaveFile(std::span<U> data)
  {
    return SaveFile(data.data(), data.size_bytes());
  }

  bool SaveFile(std::string_view str)
  {
    return SaveFile(str.data(), str.size());
  }

  /**
   * Save all the data into an SDL data stream.
   *
   * @param data the data to be written. If datasize is 0, may be NULL or a
   *             invalid pointer.
   * @param datasize the number of bytes to be written.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_SaveFile
   * @sa SDL_LoadFile_IO
   */
  bool SaveFile(const void* data, size_t datasize)
  {
    return SDL_SaveFile_IO(T::get(), data, datasize);
  }
  /**
   * Use this function to read a byte from an SDL_IOStream.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on success or false on failure or EOF; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU8(Uint8* value) { return SDL_ReadU8(T::get(), value); }

  /**
   * Use this function to read a signed byte from an SDL_IOStream.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS8(Sint8* value) { return SDL_ReadS8(T::get(), value); }

  /**
   * Use this function to read 16 bits of little-endian data from an
   * SDL_IOStream and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU16LE(Uint16* value) { return SDL_ReadU16LE(T::get(), value); }

  /**
   * Use this function to read 16 bits of little-endian data from an
   * SDL_IOStream and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS16LE(Sint16* value) { return SDL_ReadS16LE(T::get(), value); }

  /**
   * Use this function to read 16 bits of big-endian data from an SDL_IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU16BE(Uint16* value) { return SDL_ReadU16BE(T::get(), value); }

  /**
   * Use this function to read 16 bits of big-endian data from an SDL_IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS16BE(Sint16* value) { return SDL_ReadS16BE(T::get(), value); }

  /**
   * Use this function to read 32 bits of little-endian data from an
   * SDL_IOStream and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU32LE(Uint32* value) { return SDL_ReadU32LE(T::get(), value); }

  /**
   * Use this function to read 32 bits of little-endian data from an
   * SDL_IOStream and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS32LE(Sint32* value) { return SDL_ReadS32LE(T::get(), value); }

  /**
   * Use this function to read 32 bits of big-endian data from an SDL_IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU32BE(Uint32* value) { return SDL_ReadU32BE(T::get(), value); }

  /**
   * Use this function to read 32 bits of big-endian data from an SDL_IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS32BE(Sint32* value) { return SDL_ReadS32BE(T::get(), value); }

  /**
   * Use this function to read 64 bits of little-endian data from an
   * SDL_IOStream and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU64LE(Uint64* value) { return SDL_ReadU64LE(T::get(), value); }

  /**
   * Use this function to read 64 bits of little-endian data from an
   * SDL_IOStream and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS64LE(Sint64* value) { return SDL_ReadS64LE(T::get(), value); }

  /**
   * Use this function to read 64 bits of big-endian data from an SDL_IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadU64BE(Uint64* value) { return SDL_ReadU64BE(T::get(), value); }

  /**
   * Use this function to read 64 bits of big-endian data from an SDL_IOStream
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and SDL_GetIOStatus() will return SDL_IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, SDL_GetIOStatus() will return a different
   * error value and SDL_GetError() will offer a human-readable message.
   *
   * @param value a pointer filled in with the data read.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool ReadS64BE(Sint64* value) { return SDL_ReadS64BE(T::get(), value); }

  /**
   * Use this function to write a byte to an SDL_IOStream.
   *
   * @param value the byte value to write.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU8(Uint8 value) { return SDL_WriteU8(T::get(), value); }

  /**
   * Use this function to write a signed byte to an SDL_IOStream.
   *
   * @param value the byte value to write.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS8(Sint8 value) { return SDL_WriteS8(T::get(), value); }

  /**
   * Use this function to write 16 bits in native format to an SDL_IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU16LE(Uint16 value) { return SDL_WriteU16LE(T::get(), value); }

  /**
   * Use this function to write 16 bits in native format to an SDL_IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS16LE(Sint16 value) { return SDL_WriteS16LE(T::get(), value); }

  /**
   * Use this function to write 16 bits in native format to an SDL_IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU16BE(Uint16 value) { return SDL_WriteU16BE(T::get(), value); }

  /**
   * Use this function to write 16 bits in native format to an SDL_IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS16BE(Sint16 value) { return SDL_WriteS16BE(T::get(), value); }

  /**
   * Use this function to write 32 bits in native format to an SDL_IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU32LE(Uint32 value) { return SDL_WriteU32LE(T::get(), value); }

  /**
   * Use this function to write 32 bits in native format to an SDL_IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS32LE(Sint32 value) { return SDL_WriteS32LE(T::get(), value); }

  /**
   * Use this function to write 32 bits in native format to an SDL_IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU32BE(Uint32 value) { return SDL_WriteU32BE(T::get(), value); }

  /**
   * Use this function to write 32 bits in native format to an SDL_IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS32BE(Sint32 value) { return SDL_WriteS32BE(T::get(), value); }

  /**
   * Use this function to write 64 bits in native format to an SDL_IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU64LE(Uint64 value) { return SDL_WriteU64LE(T::get(), value); }

  /**
   * Use this function to write 64 bits in native format to an SDL_IOStream as
   * little-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in little-endian
   * format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS64LE(Sint64 value) { return SDL_WriteS64LE(T::get(), value); }

  /**
   * Use this function to write 64 bits in native format to an SDL_IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteU64BE(Uint64 value) { return SDL_WriteU64BE(T::get(), value); }

  /**
   * Use this function to write 64 bits in native format to an SDL_IOStream as
   * big-endian data.
   *
   * SDL byteswaps the data only if necessary, so the application always
   * specifies native format, and the data written will be in big-endian format.
   *
   * @param value the data to be written, in native format.
   * @returns true on successful write or false on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool WriteS64BE(Sint64 value) { return SDL_WriteS64BE(T::get(), value); }

  /**
   * Close and free an allocated SDL_IOStream structure.
   *
   * SDL_CloseIO() closes and cleans up the SDL_IOStream stream. It releases any
   * resources used by the stream and frees the SDL_IOStream itself. This
   * returns true on success, or false if the stream failed to flush to its
   * output (e.g. to disk).
   *
   * Note that if this fails to flush the stream for any reason, this function
   * reports an error, but the SDL_IOStream is still invalid once this function
   * returns.
   *
   * This call flushes any buffered writes to the operating system, but there
   * are no guarantees that those writes have gone to physical media; they might
   * be in the OS's file cache, waiting to go to disk later. If it's absolutely
   * crucial that writes go to disk immediately, so they are definitely stored
   * even if the power fails before the file cache would have caught up, one
   * should call SDL_FlushIO() before closing. Note that flushing takes time and
   * makes the system and your app operate less efficiently, so do so sparingly.
   *
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_OpenIO
   */
  bool Close() { return SDL_CloseIO(T::release()); }
};

/**
 * Close and free an allocated SDL_IOStream structure.
 *
 * SDL_CloseIO() closes and cleans up the SDL_IOStream stream. It releases any
 * resources used by the stream and frees the SDL_IOStream itself. This
 * returns true on success, or false if the stream failed to flush to its
 * output (e.g. to disk).
 *
 * Note that if this fails to flush the stream for any reason, this function
 * reports an error, but the SDL_IOStream is still invalid once this function
 * returns.
 *
 * This call flushes any buffered writes to the operating system, but there
 * are no guarantees that those writes have gone to physical media; they might
 * be in the OS's file cache, waiting to go to disk later. If it's absolutely
 * crucial that writes go to disk immediately, so they are definitely stored
 * even if the power fails before the file cache would have caught up, one
 * should call SDL_FlushIO() before closing. Note that flushing takes time and
 * makes the system and your app operate less efficiently, so do so sparingly.
 *
 * @param resource SDL_IOStream structure to close.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_OpenIO
 */
template<>
inline void ObjectRef<SDL_IOStream>::doFree(SDL_IOStream* resource)
{
  SDL_CloseIO(resource);
}

/**
 * Load all the data from a file path.
 *
 * The data is allocated with a zero byte at the end (null terminated) for
 * convenience. This extra byte is not included in the value reported via
 * `datasize`.
 *
 * The data should be freed with SDL_free().
 *
 * @param file the path to read all available data from.
 * @returns the data or NULL on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_LoadFile_IO
 * @sa SDL_SaveFile
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
 * @param data the data to be written. If datasize is 0, may be NULL or a
 *             invalid pointer.
 * @param datasize the number of bytes to be written.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_SaveFile_IO
 * @sa SDL_LoadFile
 */
inline bool SaveFile(StringParam file, const void* data, size_t datasize)
{
  return SDL_SaveFile(file, data, datasize);
}

template<class T>
inline bool SaveFile(StringParam file, std::span<T> data)
{
  return SaveFile(file, data.data(), data.size_bytes());
}

inline bool SaveFile(StringParam file, std::string_view str)
{
  return SaveFile(std::move(file), str.data(), str.size());
}

#pragma region impl
/// @}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_IOSTREAM_H_ */
