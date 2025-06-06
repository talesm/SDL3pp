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
 * offers implementations for files, memory, etc, and the app can provide their
 * own implementations, too.
 *
 * IOStream is not related to the standard C++ iostream class, other than both
 * are abstract interfaces to read/write data.
 *
 * @{
 */

// Forward decl
struct IOStreamRef;

// Forward decl
struct IOStream;

/**
 * Handle to a shared iOStream.
 *
 * @cat resource
 *
 * @sa IOStreamRef
 * @sa IOStream
 */
using IOStreamShared = ResourceShared<IOStream>;

/**
 * Weak handle to a shared iOStream.
 *
 * @cat resource
 *
 * @sa IOStreamShared
 * @sa IOStreamRef
 */
using IOStreamWeak = ResourceWeak<IOStream>;

/**
 * IOStream status, set by a read or write operation.
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
 * Possible `whence` values for IOStream seeking.
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
 * The function pointers that drive an IOStream.
 *
 * Applications can provide this struct to IOStream.Open() to create their own
 * implementation of IOStream. This is not necessarily required, as SDL already
 * offers several common types of I/O streams, via functions like
 * IOStream.FromFile() and IOStream.FromMem().
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
 * IOStream.Open() to provide their own stream implementation behind this
 * struct's abstract interface.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa IOStream
 */
struct IOStreamRef : Resource<SDL_IOStream*>
{
  using Resource::Resource;

  /**
   * Get the properties associated with an IOStreamRef.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return CheckError(SDL_GetIOProperties(get()));
  }

  /**
   * Query the stream status of an IOStreamRef.
   *
   * This information can be useful to decide if a short read or write was due
   * to an error, an EOF, or a non-blocking operation that isn't yet ready to
   * complete.
   *
   * An IOStreamRef's status is only expected to change after a IOStreamRef.Read
   * or IOStreamRef.Write call; don't expect it to change if you just call this
   * query function in a tight loop.
   *
   * @returns an IOStatus enum with the current state.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  IOStatus GetStatus() const { return SDL_GetIOStatus(get()); }

  /**
   * Use this function to get the size of the data stream in an IOStreamRef.
   *
   * @returns the size of the data stream in the IOStreamRef on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint64 GetSize() const
  {
    auto size = SDL_GetIOSize(get());
    if (size < 0) throw Error{};
    return size;
  }

  /**
   * Seek within an IOStreamRef data stream.
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
   * @sa IOStreamRef.Tell
   */
  Sint64 Seek(Sint64 offset, IOWhence whence)
  {
    return SDL_SeekIO(get(), offset, whence);
  }

  /**
   * Determine the current read/write offset in an IOStreamRef data stream.
   *
   * IOStreamRef.Tell is actually a wrapper function that calls the
   * IOStreamRef's `seek` method, with an offset of 0 bytes from `IO_SEEK_CUR`,
   * to simplify application development.
   *
   * @returns the current offset in the stream, or -1 if the information can not
   *          be determined.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamRef.Seek
   */
  Sint64 Tell() const { return SDL_TellIO(get()); }

  /**
   * Read from a data source.
   *
   * This function reads up `size` bytes from the data source to the area
   * pointed at by `ptr`. This function may read less bytes than requested.
   *
   * This function will return zero when the data stream is completely read, and
   * IOStreamRef.GetStatus() will return IO_STATUS_EOF. If zero is returned and
   * the stream is not at EOF, IOStreamRef.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param size the number of bytes to read from the data source.
   * @returns the bytes, or empty string on end of file or other failure;
   *          call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamRef.Write
   * @sa IOStreamRef.GetStatus
   */
  std::string Read(size_t size = -1)
  {
    Sint64 pos = Tell();
    auto curSize = SDL_GetIOSize(get());
    if ((curSize < 0 || pos < 0)) {
      if (size == size_t(-1)) return {};
    } else if (curSize - pos <= 0) {
      return {};
    } else if (curSize - pos < size) {
      size = curSize - pos;
    }
    std::string result(size, 0);
    auto actualSize = Read(result);
    if (actualSize < size) result.resize(actualSize);
    return result;
  }

  /**
   * Read from a data source.
   *
   * This function reads up `size` bytes from the data source to the area
   * pointed at by `ptr`. This function may read less bytes than requested.
   *
   * This function will return zero when the data stream is completely read, and
   * IOStreamRef.GetStatus() will return IO_STATUS_EOF. If zero is returned and
   * the stream is not at EOF, IOStreamRef.GetStatus() will return a different
   * error value and GetError() will offer a human-readable message.
   *
   * @param buf the buffer to read data into.
   * @returns the number of bytes read, or 0 on end of file or other failure;
   *          call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamRef.Write
   * @sa IOStreamRef.GetStatus
   */
  size_t Read(TargetBytes buf)
  {
    return SDL_ReadIO(get(), buf.data, buf.size_bytes);
  }

  /**
   * Write to an IOStreamRef data stream.
   *
   * This function writes exactly `size` bytes from the area pointed at by `ptr`
   * to the stream. If this fails for any reason, it'll return less than `size`
   * to demonstrate how far the write progressed. On success, it returns `size`.
   *
   * On error, this function still attempts to write as much as possible, so it
   * might return a positive value less than the requested write size.
   *
   * The caller can use IOStreamRef.GetStatus() to determine if the problem is
   * recoverable, such as a non-blocking write that can simply be retried later,
   * or a fatal error.
   *
   * @param buf the bytes to write to
   * @returns the number of bytes written, which will be less than `size` on
   *          failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStreamRef.printf
   * @sa IOStreamRef.Read
   * @sa IOStreamRef.Seek
   * @sa IOStreamRef.Flush
   * @sa IOStreamRef.GetStatus
   */
  size_t Write(SourceBytes buf)
  {
    return SDL_WriteIO(get(), buf.data, buf.size_bytes);
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
   * Print to an IOStreamRef data stream.
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
   * @sa IOStreamRef.vprintf
   * @sa IOStreamRef.Write
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
   * Print to an IOStreamRef data stream.
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
   * @sa IOStreamRef.printf
   * @sa IOStreamRef.Write
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
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.FromFile
   * @sa IOStreamRef.Write
   */
  void Flush() { CheckError(SDL_FlushIO(get())); }

  /**
   * Load all the data from an SDL data stream.
   *
   * The data is allocated with a zero byte at the end (null terminated) for
   * convenience. This extra byte is not included in the value reported via
   * `datasize`.
   *
   * @returns the data in bytes
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LoadFile
   * @sa IOStreamRef.SaveFile
   */
  StringResult LoadFile()
  {
    size_t datasize = 0;
    auto data = static_cast<char*>(SDL_LoadFile_IO(get(), &datasize, false));
    return StringResult{CheckError(data), datasize};
  }

  /**
   * Load all the data from an SDL data stream.
   *
   * The data is allocated with a zero byte at the end (null terminated) for
   * convenience. This extra byte is not included in the value reported via
   * `datasize`.
   *
   * @returns the data in bytes
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LoadFile
   * @sa IOStreamRef.SaveFile
   */
  template<class T>
  OwnArray<T> LoadFileAs()
  {
    size_t datasize = 0;
    auto data = static_cast<T*>(SDL_LoadFile_IO(get(), &datasize, false));
    return OwnArray<T>{CheckError(data), datasize / sizeof(T)};
  }

  /**
   * Save all the data into an SDL data stream.
   *
   * @param data the buf to be written. If datasize is 0, may be nullptr or a
   *            invalid pointer.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SaveFile
   * @sa IOStreamRef.LoadFile
   */
  void SaveFile(SourceBytes data)
  {
    CheckError(SDL_SaveFile_IO(get(), data.data, data.size_bytes, false));
  }

  /**
   * Use this function to read a byte from an IOStreamRef.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint8 ReadU8()
  {
    Uint8 value;
    CheckError(SDL_ReadU8(get(), &value));
    return value;
  }

  /**
   * Use this function to read a signed byte from an IOStreamRef.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint8 ReadS8()
  {
    Sint8 value;
    CheckError(SDL_ReadS8(get(), &value));
    return value;
  }

  /**
   * Use this function to read 16 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint16 ReadU16LE()
  {
    Uint16 value;
    CheckError(SDL_ReadU16LE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 16 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint16 ReadS16LE()
  {
    Sint16 value;
    CheckError(SDL_ReadS16LE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 16 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint16 ReadU16BE()
  {
    Uint16 value;
    CheckError(SDL_ReadU16BE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 16 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint16 ReadS16BE()
  {
    Sint16 value;
    CheckError(SDL_ReadS16BE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 32 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint32 ReadU32LE()
  {
    Uint32 value;
    CheckError(SDL_ReadU32LE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 32 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint32 ReadS32LE()
  {
    Sint32 value;
    CheckError(SDL_ReadS32LE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 32 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint32 ReadU32BE()
  {
    Uint32 value;
    CheckError(SDL_ReadU32BE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 32 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint32 ReadS32BE()
  {
    Sint32 value;
    CheckError(SDL_ReadS32BE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 64 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint64 ReadU64LE()
  {
    Uint64 value;
    CheckError(SDL_ReadU64LE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 64 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint64 ReadS64LE()
  {
    Sint64 value;
    CheckError(SDL_ReadS64LE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 64 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint64 ReadU64BE()
  {
    Uint64 value;
    CheckError(SDL_ReadU64BE(get(), &value));
    return value;
  }

  /**
   * Use this function to read 64 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is returned
   * and the stream is not at EOF, IOStreamRef.GetStatus() will return a
   * different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint64 ReadS64BE()
  {
    Sint64 value;
    CheckError(SDL_ReadS64BE(get(), &value));
    return value;
  }

  /**
   * Use this function to write a byte to an IOStreamRef.
   *
   * @param value the byte value to write.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteU8(Uint8 value) { CheckError(SDL_WriteU8(get(), value)); }

  /**
   * Use this function to write a signed byte to an IOStreamRef.
   *
   * @param value the byte value to write.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void WriteS8(Sint8 value) { CheckError(SDL_WriteS8(get(), value)); }

  /**
   * Use this function to write 16 bits in native format to an IOStreamRef as
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
  void WriteU16LE(Uint16 value) { CheckError(SDL_WriteU16LE(get(), value)); }

  /**
   * Use this function to write 16 bits in native format to an IOStreamRef as
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
  void WriteS16LE(Sint16 value) { CheckError(SDL_WriteS16LE(get(), value)); }

  /**
   * Use this function to write 16 bits in native format to an IOStreamRef as
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
  void WriteU16BE(Uint16 value) { CheckError(SDL_WriteU16BE(get(), value)); }

  /**
   * Use this function to write 16 bits in native format to an IOStreamRef as
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
  void WriteS16BE(Sint16 value) { CheckError(SDL_WriteS16BE(get(), value)); }

  /**
   * Use this function to write 32 bits in native format to an IOStreamRef as
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
  void WriteU32LE(Uint32 value) { CheckError(SDL_WriteU32LE(get(), value)); }

  /**
   * Use this function to write 32 bits in native format to an IOStreamRef as
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
  void WriteS32LE(Sint32 value) { CheckError(SDL_WriteS32LE(get(), value)); }

  /**
   * Use this function to write 32 bits in native format to an IOStreamRef as
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
  void WriteU32BE(Uint32 value) { CheckError(SDL_WriteU32BE(get(), value)); }

  /**
   * Use this function to write 32 bits in native format to an IOStreamRef as
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
  void WriteS32BE(Sint32 value) { CheckError(SDL_WriteS32BE(get(), value)); }

  /**
   * Use this function to write 64 bits in native format to an IOStreamRef as
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
  void WriteU64LE(Uint64 value) { CheckError(SDL_WriteU64LE(get(), value)); }

  /**
   * Use this function to write 64 bits in native format to an IOStreamRef as
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
  void WriteS64LE(Sint64 value) { CheckError(SDL_WriteS64LE(get(), value)); }

  /**
   * Use this function to write 64 bits in native format to an IOStreamRef as
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
  void WriteU64BE(Uint64 value) { CheckError(SDL_WriteU64BE(get(), value)); }

  /**
   * Use this function to write 64 bits in native format to an IOStreamRef as
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
  void WriteS64BE(Sint64 value) { CheckError(SDL_WriteS64BE(get(), value)); }

  /**
   * Use this function to read a byte from an IOStreamRef.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Uint8> TryReadU8()
  {
    if (Uint8 value; SDL_ReadU8(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read a byte from an IOStreamRef.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Sint8> TryReadS8()
  {
    if (Sint8 value; SDL_ReadS8(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 16 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Uint16> TryReadU16LE()
  {
    if (Uint16 value; SDL_ReadU16LE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 16 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Sint16> TryReadS16LE()
  {
    if (Sint16 value; SDL_ReadS16LE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 16 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Uint16> TryReadU16BE()
  {
    if (Uint16 value; SDL_ReadU16BE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 16 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Sint16> TryReadS16BE()
  {
    if (Sint16 value; SDL_ReadS16BE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 32 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Uint32> TryReadU32LE()
  {
    if (Uint32 value; SDL_ReadU32LE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 32 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Sint32> TryReadS32LE()
  {
    if (Sint32 value; SDL_ReadS32LE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 32 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Uint32> TryReadU32BE()
  {
    if (Uint32 value; SDL_ReadU32BE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 32 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Sint32> TryReadS32BE()
  {
    if (Sint32 value; SDL_ReadS32BE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 64 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Uint64> TryReadU64LE()
  {
    if (Uint64 value; SDL_ReadU64LE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 64 bits of little-endian data from an
   * IOStreamRef and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Sint64> TryReadS64LE()
  {
    if (Sint64 value; SDL_ReadS64LE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 64 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Uint64> TryReadU64BE()
  {
    if (Uint64 value; SDL_ReadU64BE(get(), &value)) return value;
    return {};
  }

  /**
   * Use this function to read 64 bits of big-endian data from an IOStreamRef
   * and return in native format.
   *
   * SDL byteswaps the data only if necessary, so the data returned will be in
   * the native byte order.
   *
   * This function will return false when the data stream is completely read,
   * and IOStreamRef.GetStatus() will return IO_STATUS_EOF. If false is
   * returned and the stream is not at EOF, IOStreamRef.GetStatus() will return
   * a different error value and GetError() will offer a human-readable message.
   *
   * @returns the data read on success, std::nullopt on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   */
  std::optional<Sint64> TryReadS64BE()
  {
    if (Sint64 value; SDL_ReadS64BE(get(), &value)) return value;
    return {};
  }

  /**
   * Close and free an allocated IOStreamRef structure.
   *
   * IOStream.Close() closes and cleans up the IOStreamRef stream. It releases
   * any resources used by the stream and frees the IOStreamRef itself. This
   * returns true on success, or false if the stream failed to flush to its
   * output (e.g. to disk).
   *
   * Note that if this fails to flush the stream for any reason, this function
   * reports an error, but the IOStreamRef is still invalid once this function
   * returns.
   *
   * This call flushes any buffered writes to the operating system, but there
   * are no guarantees that those writes have gone to physical media; they might
   * be in the OS's file cache, waiting to go to disk later. If it's absolutely
   * crucial that writes go to disk immediately, so they are definitely stored
   * even if the power fails before the file cache would have caught up, one
   * should call IOStreamRef.Flush() before closing. Note that flushing takes
   * time and makes the system and your app operate less efficiently, so do so
   * sparingly.
   *
   * @param resource IOStreamRef structure to close.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Open
   */
  static void reset(SDL_IOStream* resource)
  {
    CheckError(SDL_CloseIO(resource));
  }
};

/**
 * Handle to an owned iOStream
 *
 * @cat resource
 *
 * @sa IOStreamRef
 */
struct IOStream : ResourceUnique<IOStreamRef>
{
  using ResourceUnique::ResourceUnique;

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
   * Closing the IOStreamRef will close SDL's internal file handle.
   *
   * The following properties may be set at creation time by SDL:
   *
   * - `prop::IOStream.WINDOWS_HANDLE_POINTER`: a pointer, that can be cast
   *   to a win32 `HANDLE`, that this IOStreamRef is using to access the
   *   filesystem. If the program isn't running on Windows, or SDL used some
   *   other method to access the filesystem, this property will not be set.
   * - `prop::IOStream.STDIO_FILE_POINTER`: a pointer, that can be cast to a
   *   stdio `FILE *`, that this IOStreamRef is using to access the filesystem.
   *   If SDL used some other method to access the filesystem, this property
   *   will not be set. PLEASE NOTE that if SDL is using a different C runtime
   *   than your app, trying to use this pointer will almost certainly result in
   *   a crash! This is mostly a problem on Windows; make sure you build SDL and
   *   your app with the same compiler and settings to avoid it.
   * - `prop::IOStream.FILE_DESCRIPTOR_NUMBER`: a file descriptor that this
   *   IOStreamRef is using to access the filesystem.
   * - `prop::IOStream.ANDROID_AASSET_POINTER`: a pointer, that can be cast
   *   to an Android NDK `AAsset *`, that this IOStreamRef is using to access
   *   the filesystem. If SDL used some other method to access the filesystem,
   *   this property will not be set.
   *
   * @param file a UTF-8 string representing the filename to open.
   * @param mode an ASCII string representing the mode to be used for opening
   *             the file.
   * @returns a pointer to the IOStreamRef structure that is created or nullptr
   * on failure; call GetError() for more information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Close
   * @sa IOStreamRef.Flush
   * @sa IOStreamRef.Read
   * @sa IOStreamRef.Seek
   * @sa IOStreamRef.Tell
   * @sa IOStreamRef.Write
   */
  static IOStream FromFile(StringParam file, StringParam mode)
  {
    return IOStream(SDL_IOFromFile(file, mode));
  }

  /**
   * Use this function to prepare a read-write memory buffer for use with
   * IOStream.
   *
   * This function sets up an IOStreamRef struct based on a memory area of a
   * certain size, for both read and write access.
   *
   * This memory buffer is not copied by the IOStreamRef; the pointer you
   * provide must remain valid until you close the stream. Closing the stream
   * will not free the original buffer.
   *
   * If you need to make sure the IOStreamRef never writes to the memory
   * buffer, you should use IOStream.FromConstMem() with a read-only buffer of
   * memory instead.
   *
   * The following properties will be set at creation time by SDL:
   *
   * - `prop::IOStream.MEMORY_POINTER`: this will be the `mem` parameter that
   *   was passed to this function.
   * - `prop::IOStream.MEMORY_SIZE_NUMBER`: this will be the `size` parameter
   *   that was passed to this function.
   *
   * @param mem a buffer to feed an IOStreamRef stream.
   * @returns a valid IOStream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.FromConstMem
   * @sa IOStream.Close
   * @sa IOStreamRef.Flush
   * @sa IOStreamRef.Read
   * @sa IOStreamRef.Seek
   * @sa IOStreamRef.Tell
   * @sa IOStreamRef.Write
   */
  static IOStream FromMem(TargetBytes mem)
  {
    return IOStream{SDL_IOFromMem(mem.data, mem.size_bytes)};
  }

  /**
   * Use this function to prepare a read-only memory buffer for use with
   * IOStreamRef.
   *
   * This function sets up an IOStreamRef struct based on a memory area of a
   * certain size. It assumes the memory area is not writable.
   *
   * Attempting to write to this IOStreamRef stream will report an error
   * without writing to the memory buffer.
   *
   * This memory buffer is not copied by the IOStreamRef; the pointer you
   * provide must remain valid until you close the stream. Closing the stream
   * will not free the original buffer.
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
   * @param mem a read-only buffer to feed an IOStreamRef stream.
   * @returns a valid IOStream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.FromMem
   * @sa IOStream.Close
   * @sa IOStreamRef.Read
   * @sa IOStreamRef.Seek
   * @sa IOStreamRef.Tell
   */
  static IOStream FromConstMem(SourceBytes mem)
  {
    return IOStream{SDL_IOFromConstMem(mem.data, mem.size_bytes)};
  }

  /**
   * Use this function to create an IOStreamRef that is backed by dynamically
   * allocated memory.
   *
   * This supports the following properties to provide access to the memory and
   * control over allocations:
   *
   * - `prop::IOStream.DYNAMIC_MEMORY_POINTER`: a pointer to the internal
   *   memory of the stream. This can be set to nullptr to transfer ownership of
   *   the memory to the application, which should free the memory with
   *   free(). If this is done, the next operation on the stream must be
   *   IOStream.Close().
   * - `prop::IOStream.DYNAMIC_CHUNKSIZE_NUMBER`: memory will be allocated in
   *   multiples of this size, defaulting to 1024.
   *
   * @returns a valid IOStream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Close
   * @sa IOStreamRef.Read
   * @sa IOStreamRef.Seek
   * @sa IOStreamRef.Tell
   * @sa IOStreamRef.Write
   */
  static IOStream FromDynamicMem() { return IOStream{SDL_IOFromDynamicMem()}; }

  /**
   * Create a custom IOStreamRef.
   *
   * Applications do not need to use this function unless they are providing
   * their own IOStreamRef implementation. If you just need an IOStreamRef to
   * read/write a common data source, you should use the built-in
   * implementations in SDL, like IOStream.FromFile() or IOStream.FromMem(),
   * etc.
   *
   * This function makes a copy of `iface` and the caller does not need to keep
   * it around after this call.
   *
   * @param iface the interface that implements this IOStreamRef, initialized
   *              using SDL_INIT_INTERFACE().
   * @param userdata the pointer that will be passed to the interface functions.
   * @returns a valid stream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Close
   * @sa SDL_INIT_INTERFACE
   * @sa IOStream.FromConstMem
   * @sa IOStream.FromFile
   * @sa IOStream.FromMem
   */
  static IOStream Open(const IOStreamInterface& iface, void* userdata)
  {
    return IOStream(CheckError(SDL_OpenIO(&iface, userdata)));
  }

  /**
   * Close and free an allocated IOStreamRef structure.
   *
   * IOStream.Close() closes and cleans up the IOStreamRef stream. It releases
   * any resources used by the stream and frees the IOStreamRef itself. This
   * returns true on success, or false if the stream failed to flush to its
   * output (e.g. to disk).
   *
   * Note that if this fails to flush the stream for any reason, this function
   * reports an error, but the IOStreamRef is still invalid once this function
   * returns.
   *
   * This call flushes any buffered writes to the operating system, but there
   * are no guarantees that those writes have gone to physical media; they might
   * be in the OS's file cache, waiting to go to disk later. If it's absolutely
   * crucial that writes go to disk immediately, so they are definitely stored
   * even if the power fails before the file cache would have caught up, one
   * should call IOStreamRef.Flush() before closing. Note that flushing takes
   * time and makes the system and your app operate less efficiently, so do so
   * sparingly.
   *
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa IOStream.Open
   */
  void Close() { reset(); }
  /**
   * Move this iOStream into a IOStreamShared.
   */
  IOStreamShared share();

};


inline IOStreamShared IOStream::share()
{
  return IOStreamShared(std::move(*this));
}

/**
 * Unsafe Handle to iOStream
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa IOStreamRef
 */
struct IOStreamUnsafe : ResourceUnsafe<IOStreamRef>
{
  using ResourceUnsafe::ResourceUnsafe;

  /**
   * Constructs IOStreamUnsafe from IOStream.
   */
  constexpr explicit IOStreamUnsafe(IOStream&& other)
    : IOStreamUnsafe(other.release())
  {
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
 * @returns the data.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStreamRef.LoadFile
 * @sa SaveFile
 */
inline StringResult LoadFile(StringParam file)
{
  size_t datasize = 0;
  auto data = static_cast<char*>(SDL_LoadFile(file, &datasize));
  return StringResult{CheckError(data), datasize};
}

/**
 * Load all the data from a file path.
 *
 * The data is allocated with a zero byte at the end (null terminated) for
 * convenience. This extra byte is not included in the value reported via
 * `datasize`.
 *
 * @param file the path to read all available data from.
 * @returns the data.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStreamRef.LoadFile
 * @sa SaveFile
 */
template<class T>
inline OwnArray<T> LoadFileAs(StringParam file)
{
  size_t datasize = 0;
  auto data = static_cast<T*>(SDL_LoadFile(file, &datasize));
  return OwnArray<T>{CheckError(data), datasize / sizeof(T)};
}

/**
 * Save all the data into a file path.
 *
 * @param file the path to write all available data into.
 * @param data the data to be written. If datasize is 0, may be nullptr or a
 *             invalid pointer.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStreamRef.SaveFile
 * @sa LoadFile
 */
inline void SaveFile(StringParam file, SourceBytes data)
{
  CheckError(SDL_SaveFile(file, data.data, data.size_bytes));
}

#pragma region impl
/// @}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_IOSTREAM_H_ */
