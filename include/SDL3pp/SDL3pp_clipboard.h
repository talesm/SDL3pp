#ifndef SDL3PP_CLIPBOARD_H_
#define SDL3PP_CLIPBOARD_H_

#include <SDL3/SDL_clipboard.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryClipboard Clipboard Handling
 *
 * SDL provides access to the system clipboard, both for reading information
 * from other processes and publishing information of its own.
 *
 * This is not just text! SDL apps can access and publish data by mimetype.
 *
 * ## Basic use (text)
 *
 * Obtaining and publishing simple text to the system clipboard is as easy as
 * calling GetClipboardText() and SetClipboardText(), respectively.
 * These deal with C strings in UTF-8 encoding. Data transmission and encoding
 * conversion is completely managed by SDL.
 *
 * ## Clipboard callbacks (data other than text)
 *
 * Things get more complicated when the clipboard contains something other
 * than text. Not only can the system clipboard contain data of any type, in
 * some cases it can contain the same data in different formats! For example,
 * an image painting app might let the user copy a graphic to the clipboard,
 * and offers it in .BMP, .JPG, or .PNG format for other apps to consume.
 *
 * Obtaining clipboard data ("pasting") like this is a matter of calling
 * GetClipboardData() and telling it the mimetype of the data you want.
 * But how does one know if that format is available? HasClipboardData()
 * can report if a specific mimetype is offered, and
 * GetClipboardMimeTypes() can provide the entire list of mimetypes
 * available, so the app can decide what to do with the data and what formats
 * it can support.
 *
 * Setting the clipboard ("copying") to arbitrary data is done with
 * SetClipboardData. The app does not provide the data in this call, but
 * rather the mimetypes it is willing to provide and a callback function.
 * During the callback, the app will generate the data. This allows massive
 * data sets to be provided to the clipboard, without any data being copied
 * before it is explicitly requested. More specifically, it allows an app to
 * offer data in multiple formats without providing a copy of all of them
 * upfront. If the app has an image that it could provide in PNG or JPG
 * format, it doesn't have to encode it to either of those unless and until
 * something tries to paste it.
 *
 * ## Primary Selection
 *
 * The X11 and Wayland video targets have a concept of the "primary selection"
 * in addition to the usual clipboard. This is generally highlighted (but not
 * explicitly copied) text from various apps. SDL offers APIs for this through
 * GetPrimarySelectionText() and SetPrimarySelectionText(). SDL offers
 * these APIs on platforms without this concept, too, but only so far that it
 * will keep a copy of a string that the app sets for later retrieval; the
 * operating system will not ever attempt to change the string externally if
 * it doesn't support a primary selection.
 *
 * @{
 */

/**
 * Put UTF-8 text into the clipboard.
 *
 * @param text the text to store in the clipboard.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetClipboardText
 * @sa HasClipboardText
 */
inline void SetClipboardText(StringParam text)
{
  CheckError(SDL_SetClipboardText(text));
}

/**
 * Get UTF-8 text from the clipboard.
 *
 * This function returns an empty string if there is not enough memory left
 * for a copy of the clipboard's content.
 *
 * @returns the clipboard text on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasClipboardText
 * @sa SetClipboardText
 */
inline StringResult GetClipboardText()
{
  return StringResult{CheckError(SDL_GetClipboardText())};
}

/**
 * Query whether the clipboard exists and contains a non-empty text string.
 *
 * @returns true if the clipboard has text, or false if it does not.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetClipboardText
 * @sa SetClipboardText
 */
inline bool HasClipboardText() { return SDL_HasClipboardText(); }

/**
 * Put UTF-8 text into the primary selection.
 *
 * @param text the text to store in the primary selection.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPrimarySelectionText
 * @sa HasPrimarySelectionText
 */
inline void SetPrimarySelectionText(StringParam text)
{
  CheckError(SDL_SetPrimarySelectionText(text));
}

/**
 * Get UTF-8 text from the primary selection.
 *
 * This function returns an empty string if there is not enough memory left
 * for a copy of the primary selection's content.
 *
 * @returns the primary selection text on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasPrimarySelectionText
 * @sa SetPrimarySelectionText
 */
inline StringResult GetPrimarySelectionText()
{
  return StringResult{CheckError(SDL_GetPrimarySelectionText())};
}

/**
 * Query whether the primary selection exists and contains a non-empty text
 * string.
 *
 * @returns true if the primary selection has text, or false if it does not.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPrimarySelectionText
 * @sa SetPrimarySelectionText
 */
inline bool HasPrimarySelectionText() { return SDL_HasPrimarySelectionText(); }

/**
 * Callback function that will be called when data for the specified mime-type
 * is requested by the OS.
 *
 * The callback function is called with nullptr as the mime_type when the
 * clipboard is cleared or new data is set. The clipboard is automatically
 * cleared in Quit().
 *
 * @param userdata a pointer to the provided user data.
 * @param mime_type the requested mime-type.
 * @param size a pointer filled in with the length of the returned data.
 * @returns a pointer to the data for the provided mime-type. Returning nullptr
 *          or setting the length to 0 will cause zero length data to be sent
 *          to the "receiver", which should be able to handle this. The
 *          returned data will not be freed, so it needs to be retained and
 *          dealt with internally.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetClipboardData
 */
using ClipboardDataCallback = SDL_ClipboardDataCallback;

/**
 * Callback function that will be called when data for the specified mime-type
 * is requested by the OS.
 *
 * The callback function is called with nullptr as the mime_type when the
 * clipboard is cleared or new data is set. The clipboard is automatically
 * cleared in Quit().
 *
 * @param mime_type the requested mime-type.
 * @param size a pointer filled in with the length of the returned data.
 * @returns a pointer to the data for the provided mime-type. Returning nullptr
 *          or setting the length to 0 will cause zero length data to be sent
 *          to the "receiver", which should be able to handle this. The
 *          returned data will not be freed, so it needs to be retained and
 *          dealt with internally.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetClipboardData
 * @sa ClipboardDataCallback
 */
using ClipboardDataCB = std::function<SourceBytes(const char* mime_type)>;

/**
 * Callback function that will be called when the clipboard is cleared, or
 * when new data is set.
 *
 * @param userdata a pointer to the provided user data.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetClipboardData
 */
using ClipboardCleanupCallback = SDL_ClipboardCleanupCallback;

/**
 * Callback function that will be called when the clipboard is cleared, or
 * when new data is set.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetClipboardData
 * @sa ClipboardCleanupCallback
 */
using ClipboardCleanupCB = std::function<void()>;

/**
 * Offer clipboard data to the OS.
 *
 * Tell the operating system that the application is offering clipboard data
 * for each of the provided mime-types. Once another application requests the
 * data the callback function will be called, allowing it to generate and
 * respond with the data for the requested mime-type.
 *
 * The size of text data does not include any terminator, and the text does
 * not need to be null-terminated (e.g., you can directly copy a portion of a
 * document).
 *
 * @param callback a function pointer to the function that provides the
 *                 clipboard data.
 * @param cleanup a function pointer to the function that cleans up the
 *                clipboard data.
 * @param userdata an opaque pointer that will be forwarded to the callbacks.
 * @param mime_types a list of mime-types that are being offered. SDL copies
 *                   the given list.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ClearClipboardData
 * @sa GetClipboardData
 * @sa HasClipboardData
 */
inline void SetClipboardData(ClipboardDataCallback callback,
                             ClipboardCleanupCallback cleanup,
                             void* userdata,
                             std::span<const char*> mime_types)
{
  CheckError(SDL_SetClipboardData(
    callback, cleanup, userdata, mime_types.data(), mime_types.size()));
}

/**
 * Offer clipboard data to the OS.
 *
 * Tell the operating system that the application is offering clipboard data
 * for each of the provided mime-types. Once another application requests the
 * data the callback function will be called, allowing it to generate and
 * respond with the data for the requested mime-type.
 *
 * The size of text data does not include any terminator, and the text does
 * not need to be null-terminated (e.g., you can directly copy a portion of a
 * document).
 *
 * @param callback a function pointer to the function that provides the
 *                 clipboard data.
 * @param cleanup a function pointer to the function that cleans up the
 *                clipboard data.
 * @param mime_types a list of mime-types that are being offered. SDL copies
 *                   the given list.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ClearClipboardData
 * @sa GetClipboardData
 * @sa HasClipboardData
 */
inline void SetClipboardData(ClipboardDataCB callback,
                             ClipboardCleanupCB cleanup,
                             std::span<const char*> mime_types)
{
  static ClipboardDataCB s_callback;
  static ClipboardCleanupCB s_cleanup;
  CheckError(SDL_ClearClipboardData());
  s_callback = callback;
  s_cleanup = cleanup;
  SetClipboardData(
    [](void*, const char* mime_type, size_t* size) -> const void* {
      auto source = s_callback(mime_type);
      *size = source.size_bytes();
      return source.data();
    },
    [](void*) {
      if (s_cleanup) s_cleanup();
    },
    nullptr,
    mime_types);
}

/**
 * Clear the clipboard data.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetClipboardData
 */
inline void ClearClipboardData() { CheckError(SDL_ClearClipboardData()); }

/**
 * Get the data from the clipboard for a given mime type.
 *
 * The size of text data does not include the terminator, but the text is
 * guaranteed to be null-terminated.
 *
 * @param mime_type the mime type to read from the clipboard.
 * @returns the retrieved data buffer or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasClipboardData
 * @sa SetClipboardData
 */
inline StringResult GetClipboardData(StringParam mime_type)
{
  size_t count = 0;
  auto data = SDL_GetClipboardData(mime_type, &count);
  if (!data) return {};
  return StringResult{static_cast<char*>(data), count};
}

/**
 * Get the data from the clipboard for a given mime type.
 *
 * The size of text data does not include the terminator, but the text is
 * guaranteed to be null-terminated.
 *
 * @param mime_type the mime type to read from the clipboard.
 * @returns the retrieved data buffer or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasClipboardData
 * @sa SetClipboardData
 */
template<class T>
inline OwnArray<T> GetClipboardDataAs(StringParam mime_type)
{
  size_t count = 0;
  auto data = SDL_GetClipboardData(mime_type, &count);
  if (!data) return {};
  return OwnArray<T>{static_cast<T*>(data), count / sizeof(T)};
}

/**
 * Query whether there is data in the clipboard for the provided mime type.
 *
 * @param mime_type the mime type to check for data.
 * @returns true if data exists in the clipboard for the provided mime type,
 *          false if it does not.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetClipboardData
 * @sa GetClipboardData
 */
inline bool HasClipboardData(StringParam mime_type)
{
  return SDL_HasClipboardData(mime_type);
}

/**
 * Retrieve the list of mime types available in the clipboard.
 *
 * @returns a null terminated array of strings with mime types, or empty on
 *          failure; call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetClipboardData
 */
inline OwnArray<char*> GetClipboardMimeTypes()
{
  size_t count = 0;
  auto data = SDL_GetClipboardMimeTypes(&count);
  if (!data) return {};
  return OwnArray<char*>{data, count};
}

/// @}

} // namespace SDL

#endif /* SDL3PP_CLIPBOARD_H_ */
