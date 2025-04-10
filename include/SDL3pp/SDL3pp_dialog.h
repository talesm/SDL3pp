#ifndef SDL3PP_DIALOG_H_
#define SDL3PP_DIALOG_H_

#include <SDL3/SDL_dialog.h>
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 *
 * @defgroup CategoryDialog File Dialogs
 *
 * File dialog support.
 *
 * SDL offers file dialogs, to let users select files with native GUI
 * interfaces. There are "open" dialogs, "save" dialogs, and folder selection
 * dialogs. The app can control some details, such as filtering to specific
 * files, or whether multiple files can be selected by the user.
 *
 * Note that launching a file dialog is a non-blocking operation; control
 * returns to the app immediately, and a callback is called later (possibly in
 * another thread) when the user makes a choice.
 *
 * @{
 */

/**
 * An entry for filters for file dialogs.
 *
 * `name` is a user-readable label for the filter (for example, "Office
 * document").
 *
 * `pattern` is a semicolon-separated list of file extensions (for example,
 * "doc;docx"). File extensions may only contain alphanumeric characters,
 * hyphens, underscores and periods. Alternatively, the whole string can be a
 * single asterisk ("*"), which serves as an "All files" filter.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa DialogFileCallback
 * @sa ShowOpenFileDialog
 * @sa ShowSaveFileDialog
 * @sa ShowOpenFolderDialog
 * @sa ShowFileDialogWithProperties
 */
using DialogFileFilter = SDL_DialogFileFilter;

/**
 * Callback used by file dialog functions.
 *
 * The specific usage is described in each function.
 *
 * If `filelist` is:
 *
 * - nullptr, an error occurred. Details can be obtained with GetError().
 * - A pointer to nullptr, the user either didn't choose any file or canceled
 * the dialog.
 * - A pointer to non-`nullptr`, the user chose one or more files. The argument
 *   is a null-terminated array of pointers to UTF-8 encoded strings, each
 *   containing a path.
 *
 * The filelist argument should not be freed; it will automatically be freed
 * when the callback returns.
 *
 * The filter argument is the index of the filter that was selected, or -1 if
 * no filter was selected or if the platform or method doesn't support
 * fetching the selected filter.
 *
 * In Android, the `filelist` are `content://` URIs. They should be opened
 * using IOStreamBase.IOStreamBase() with appropriate modes. This applies both
 * to open and save file dialog.
 *
 * @param userdata an app-provided pointer, for the callback's use.
 * @param filelist the file(s) chosen by the user.
 * @param filter index of the selected filter.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa DialogFileFilter
 * @sa ShowOpenFileDialog
 * @sa ShowSaveFileDialog
 * @sa ShowOpenFolderDialog
 * @sa ShowFileDialogWithProperties
 * @sa DialogFileCB
 */
using DialogFileCallback = SDL_DialogFileCallback;

/**
 * Callback used by file dialog functions.
 *
 * The specific usage is described in each function.
 *
 * If `filelist` is:
 *
 * - nullptr, an error occurred. Details can be obtained with GetError().
 * - A pointer to nullptr, the user either didn't choose any file or canceled
 * the dialog.
 * - A pointer to non-`nullptr`, the user chose one or more files. The argument
 *   is a null-terminated array of pointers to UTF-8 encoded strings, each
 *   containing a path.
 *
 * The filelist argument should not be freed; it will automatically be freed
 * when the callback returns.
 *
 * The filter argument is the index of the filter that was selected, or -1 if
 * no filter was selected or if the platform or method doesn't support
 * fetching the selected filter.
 *
 * In Android, the `filelist` are `content://` URIs. They should be opened
 * using IOStreamBase.IOStreamBase() with appropriate modes. This applies both
 * to open and save file dialog.
 *
 * @param filelist the file(s) chosen by the user.
 * @param filter index of the selected filter.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa DialogFileFilter
 * @sa ShowOpenFileDialog
 * @sa ShowSaveFileDialog
 * @sa ShowOpenFolderDialog
 * @sa ShowFileDialogWithProperties
 * @sa DialogFileCallback
 */
using DialogFileCB = std::function<void(const char* const*, int)>;

/**
 * Displays a dialog that lets the user select a file on their filesystem.
 *
 * This is an asynchronous function; it will return immediately, and the
 * result will be passed to the callback.
 *
 * The callback will be invoked with a null-terminated list of files the user
 * chose. The list will be empty if the user canceled the dialog, and it will
 * be nullptr if an error occurred.
 *
 * Note that the callback may be called from a different thread than the one
 * the function was invoked on.
 *
 * Depending on the platform, the user may be allowed to input paths that
 * don't yet exist.
 *
 * On Linux, dialogs may require XDG Portals, which requires DBus, which
 * requires an event-handling loop. Apps that do not use SDL to handle events
 * should add a call to PumpEvents in their main loop.
 *
 * @param callback a function pointer to be invoked when the user selects a
 *                 file and accepts, or cancels the dialog, or an error
 *                 occurs.
 * @param userdata an optional pointer to pass extra data to the callback when
 *                 it will be invoked.
 * @param window the window that the dialog should be modal for, may be nullptr.
 *               Not all platforms support this option.
 * @param filters a list of filters, may be empty. Not all platforms support
 *                this option, and platforms that do support it may allow the
 *                user to ignore the filters. If non-nullptr, it must remain
 *                valid at least until the callback is invoked.
 * @param default_location the default folder or file to start the dialog at,
 *                         may be nullptr. Not all platforms support this
 *                         option.
 * @param allow_many if non-zero, the user will be allowed to select multiple
 *                   entries. Not all platforms support this option.
 *
 * @threadsafety This function should be called only from the main thread. The
 *               callback may be invoked from the same thread or from a
 *               different one, depending on the OS's constraints.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa DialogFileCallback
 * @sa DialogFileFilter
 * @sa ShowSaveFileDialog
 * @sa ShowOpenFolderDialog
 * @sa ShowFileDialogWithProperties
 */
inline void ShowOpenFileDialog(DialogFileCallback callback,
                               void* userdata,
                               WindowRef window = {},
                               std::span<const DialogFileFilter> filters = {},
                               StringParam default_location = {},
                               bool allow_many = false)
{
  SDL_ShowOpenFileDialog(callback,
                         userdata,
                         window.get(),
                         filters.data(),
                         filters.size(),
                         default_location,
                         allow_many);
}

/**
 * Displays a dialog that lets the user select a file on their filesystem.
 *
 * This is an asynchronous function; it will return immediately, and the
 * result will be passed to the callback.
 *
 * The callback will be invoked with a null-terminated list of files the user
 * chose. The list will be empty if the user canceled the dialog, and it will
 * be nullptr if an error occurred.
 *
 * Note that the callback may be called from a different thread than the one
 * the function was invoked on.
 *
 * Depending on the platform, the user may be allowed to input paths that
 * don't yet exist.
 *
 * On Linux, dialogs may require XDG Portals, which requires DBus, which
 * requires an event-handling loop. Apps that do not use SDL to handle events
 * should add a call to PumpEvents in their main loop.
 *
 * @param callback a function pointer to be invoked when the user selects a
 *                 file and accepts, or cancels the dialog, or an error
 *                 occurs.
 * @param window the window that the dialog should be modal for, may be nullptr.
 *               Not all platforms support this option.
 * @param filters a list of filters, may be nullptr. Not all platforms support
 *                this option, and platforms that do support it may allow the
 *                user to ignore the filters. If non-nullptr, it must remain
 *                valid at least until the callback is invoked.
 * @param default_location the default folder or file to start the dialog at,
 *                         may be nullptr. Not all platforms support this
 *                         option.
 * @param allow_many if non-zero, the user will be allowed to select multiple
 *                   entries. Not all platforms support this option.
 *
 * @threadsafety This function should be called only from the main thread. The
 *               callback may be invoked from the same thread or from a
 *               different one, depending on the OS's constraints.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa DialogFileCallback
 * @sa DialogFileFilter
 * @sa ShowSaveFileDialog
 * @sa ShowOpenFolderDialog
 * @sa ShowFileDialogWithProperties
 */
inline void ShowOpenFileDialog(DialogFileCB callback,
                               WindowRef window = {},
                               std::span<const DialogFileFilter> filters = {},
                               StringParam default_location = {},
                               bool allow_many = false)
{
  using Wrapper = CallbackWrapper<DialogFileCB>;
  ShowOpenFileDialog(&Wrapper::CallOnce,
                     Wrapper::Wrap(std::move(callback)),
                     window,
                     filters,
                     std::move(default_location),
                     allow_many);
}

/**
 * Displays a dialog that lets the user choose a new or existing file on their
 * filesystem.
 *
 * This is an asynchronous function; it will return immediately, and the
 * result will be passed to the callback.
 *
 * The callback will be invoked with a null-terminated list of files the user
 * chose. The list will be empty if the user canceled the dialog, and it will
 * be nullptr if an error occurred.
 *
 * Note that the callback may be called from a different thread than the one
 * the function was invoked on.
 *
 * The chosen file may or may not already exist.
 *
 * On Linux, dialogs may require XDG Portals, which requires DBus, which
 * requires an event-handling loop. Apps that do not use SDL to handle events
 * should add a call to PumpEvents in their main loop.
 *
 * @param callback a function pointer to be invoked when the user selects a
 *                 file and accepts, or cancels the dialog, or an error
 *                 occurs.
 * @param userdata an optional pointer to pass extra data to the callback when
 *                 it will be invoked.
 * @param window the window that the dialog should be modal for, may be nullptr.
 *               Not all platforms support this option.
 * @param filters a list of filters, may be empty. Not all platforms support
 *                this option, and platforms that do support it may allow the
 *                user to ignore the filters. If non-nullptr, it must remain
 *                valid at least until the callback is invoked.
 * @param default_location the default folder or file to start the dialog at,
 *                         may be nullptr. Not all platforms support this
 *                         option.
 *
 * @threadsafety This function should be called only from the main thread. The
 *               callback may be invoked from the same thread or from a
 *               different one, depending on the OS's constraints.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa DialogFileCallback
 * @sa DialogFileFilter
 * @sa ShowOpenFileDialog
 * @sa ShowOpenFolderDialog
 * @sa ShowFileDialogWithProperties
 */
inline void ShowSaveFileDialog(DialogFileCallback callback,
                               void* userdata,
                               WindowRef window = {},
                               std::span<const DialogFileFilter> filters = {},
                               StringParam default_location = {})
{
  SDL_ShowSaveFileDialog(callback,
                         userdata,
                         window.get(),
                         filters.data(),
                         filters.size(),
                         default_location);
}

/**
 * Displays a dialog that lets the user choose a new or existing file on their
 * filesystem.
 *
 * This is an asynchronous function; it will return immediately, and the
 * result will be passed to the callback.
 *
 * The callback will be invoked with a null-terminated list of files the user
 * chose. The list will be empty if the user canceled the dialog, and it will
 * be nullptr if an error occurred.
 *
 * Note that the callback may be called from a different thread than the one
 * the function was invoked on.
 *
 * The chosen file may or may not already exist.
 *
 * On Linux, dialogs may require XDG Portals, which requires DBus, which
 * requires an event-handling loop. Apps that do not use SDL to handle events
 * should add a call to PumpEvents in their main loop.
 *
 * @param callback a function pointer to be invoked when the user selects a
 *                 file and accepts, or cancels the dialog, or an error
 *                 occurs.
 * @param window the window that the dialog should be modal for, may be nullptr.
 *               Not all platforms support this option.
 * @param filters a list of filters, may be nullptr. Not all platforms support
 *                this option, and platforms that do support it may allow the
 *                user to ignore the filters. If non-nullptr, it must remain
 *                valid at least until the callback is invoked.
 * @param default_location the default folder or file to start the dialog at,
 *                         may be nullptr. Not all platforms support this
 *                         option.
 *
 * @threadsafety This function should be called only from the main thread. The
 *               callback may be invoked from the same thread or from a
 *               different one, depending on the OS's constraints.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa DialogFileCallback
 * @sa DialogFileFilter
 * @sa ShowOpenFileDialog
 * @sa ShowOpenFolderDialog
 * @sa ShowFileDialogWithProperties
 */
inline void ShowSaveFileDialog(DialogFileCB callback,
                               WindowRef window = {},
                               std::span<const DialogFileFilter> filters = {},
                               StringParam default_location = {})
{
  using Wrapper = CallbackWrapper<DialogFileCB>;
  ShowSaveFileDialog(&Wrapper::CallOnce,
                     Wrapper::Wrap(std::move(callback)),
                     window,
                     filters,
                     std::move(default_location));
}

/**
 * Displays a dialog that lets the user select a folder on their filesystem.
 *
 * This is an asynchronous function; it will return immediately, and the
 * result will be passed to the callback.
 *
 * The callback will be invoked with a null-terminated list of files the user
 * chose. The list will be empty if the user canceled the dialog, and it will
 * be nullptr if an error occurred.
 *
 * Note that the callback may be called from a different thread than the one
 * the function was invoked on.
 *
 * Depending on the platform, the user may be allowed to input paths that
 * don't yet exist.
 *
 * On Linux, dialogs may require XDG Portals, which requires DBus, which
 * requires an event-handling loop. Apps that do not use SDL to handle events
 * should add a call to PumpEvents in their main loop.
 *
 * @param callback a function pointer to be invoked when the user selects a
 *                 file and accepts, or cancels the dialog, or an error
 *                 occurs.
 * @param userdata an optional pointer to pass extra data to the callback when
 *                 it will be invoked.
 * @param window the window that the dialog should be modal for, may be nullptr.
 *               Not all platforms support this option.
 * @param default_location the default folder or file to start the dialog at,
 *                         may be nullptr. Not all platforms support this
 *                         option.
 * @param allow_many if non-zero, the user will be allowed to select multiple
 *                   entries. Not all platforms support this option.
 *
 * @threadsafety This function should be called only from the main thread. The
 *               callback may be invoked from the same thread or from a
 *               different one, depending on the OS's constraints.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa DialogFileCallback
 * @sa ShowOpenFileDialog
 * @sa ShowSaveFileDialog
 * @sa ShowFileDialogWithProperties
 */
inline void ShowOpenFolderDialog(DialogFileCallback callback,
                                 void* userdata,
                                 WindowRef window = {},
                                 StringParam default_location = {},
                                 bool allow_many = false)
{
  SDL_ShowOpenFolderDialog(
    callback, userdata, window.get(), default_location, allow_many);
}

/**
 * Displays a dialog that lets the user select a folder on their filesystem.
 *
 * This is an asynchronous function; it will return immediately, and the
 * result will be passed to the callback.
 *
 * The callback will be invoked with a null-terminated list of files the user
 * chose. The list will be empty if the user canceled the dialog, and it will
 * be nullptr if an error occurred.
 *
 * Note that the callback may be called from a different thread than the one
 * the function was invoked on.
 *
 * Depending on the platform, the user may be allowed to input paths that
 * don't yet exist.
 *
 * On Linux, dialogs may require XDG Portals, which requires DBus, which
 * requires an event-handling loop. Apps that do not use SDL to handle events
 * should add a call to PumpEvents in their main loop.
 *
 * @param callback a function pointer to be invoked when the user selects a
 *                 file and accepts, or cancels the dialog, or an error
 *                 occurs.
 * @param window the window that the dialog should be modal for, may be nullptr.
 *               Not all platforms support this option.
 * @param default_location the default folder or file to start the dialog at,
 *                         may be nullptr. Not all platforms support this
 *                         option.
 * @param allow_many if non-zero, the user will be allowed to select multiple
 *                   entries. Not all platforms support this option.
 *
 * @threadsafety This function should be called only from the main thread. The
 *               callback may be invoked from the same thread or from a
 *               different one, depending on the OS's constraints.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa DialogFileCallback
 * @sa ShowOpenFileDialog
 * @sa ShowSaveFileDialog
 * @sa ShowFileDialogWithProperties
 */
inline void ShowOpenFolderDialog(DialogFileCB callback,
                                 WindowRef window = {},
                                 StringParam default_location = {},
                                 bool allow_many = false)
{
  using Wrapper = CallbackWrapper<DialogFileCB>;
  ShowOpenFolderDialog(&Wrapper::CallOnce,
                       Wrapper::Wrap(std::move(callback)),
                       window,
                       std::move(default_location),
                       allow_many);
}

/**
 * Various types of file dialogs.
 *
 * This is used by ShowFileDialogWithProperties() to decide what kind of
 * dialog to present to the user.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa ShowFileDialogWithProperties
 */
using FileDialogType = SDL_FileDialogType;

constexpr FileDialogType FILEDIALOG_OPENFILE =
  SDL_FILEDIALOG_OPENFILE; ///< OPENFILE

constexpr FileDialogType FILEDIALOG_SAVEFILE =
  SDL_FILEDIALOG_SAVEFILE; ///< SAVEFILE

constexpr FileDialogType FILEDIALOG_OPENFOLDER =
  SDL_FILEDIALOG_OPENFOLDER; ///< OPENFOLDER

/**
 * Create and launch a file dialog with the specified properties.
 *
 * These are the supported properties:
 *
 * - `prop::FileDialog.FILTERS_POINTER`: a pointer to a list of
 *   DialogFileFilter structs, which will be used as filters for
 *   file-based selections. Ignored if the dialog is an "Open Folder" dialog.
 *   If non-nullptr, the array of filters must remain valid at least until the
 *   callback is invoked.
 * - `prop::FileDialog.NFILTERS_NUMBER`: the number of filters in the
 *   array of filters, if it exists.
 * - `prop::FileDialog.WINDOW_POINTER`: the window that the dialog should
 *   be modal for.
 * - `prop::FileDialog.LOCATION_STRING`: the default folder or file to
 *   start the dialog at.
 * - `prop::FileDialog.MANY_BOOLEAN`: true to allow the user to select
 *   more than one entry.
 * - `prop::FileDialog.TITLE_STRING`: the title for the dialog.
 * - `prop::FileDialog.ACCEPT_STRING`: the label that the accept button
 *   should have.
 * - `prop::FileDialog.CANCEL_STRING`: the label that the cancel button
 *   should have.
 *
 * Note that each platform may or may not support any of the properties.
 *
 * @param type the type of file dialog.
 * @param callback a function pointer to be invoked when the user selects a
 *                 file and accepts, or cancels the dialog, or an error
 *                 occurs.
 * @param userdata an optional pointer to pass extra data to the callback when
 *                 it will be invoked.
 * @param props the properties to use.
 *
 * @threadsafety This function should be called only from the main thread. The
 *               callback may be invoked from the same thread or from a
 *               different one, depending on the OS's constraints.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FileDialogType
 * @sa DialogFileCallback
 * @sa DialogFileFilter
 * @sa ShowOpenFileDialog
 * @sa ShowSaveFileDialog
 * @sa ShowOpenFolderDialog
 */
inline void ShowFileDialogWithProperties(FileDialogType type,
                                         DialogFileCallback callback,
                                         void* userdata,
                                         PropertiesBase& props)
{
  SDL_ShowFileDialogWithProperties(type, callback, userdata, props.get());
}

/**
 * Create and launch a file dialog with the specified properties.
 *
 * These are the supported properties:
 *
 * - `SDL_PROP_FILE_DIALOG_FILTERS_POINTER`: a pointer to a list of
 *   DialogFileFilter structs, which will be used as filters for
 *   file-based selections. Ignored if the dialog is an "Open Folder" dialog.
 *   If non-nullptr, the array of filters must remain valid at least until the
 *   callback is invoked.
 * - `SDL_PROP_FILE_DIALOG_NFILTERS_NUMBER`: the number of filters in the
 *   array of filters, if it exists.
 * - `SDL_PROP_FILE_DIALOG_WINDOW_POINTER`: the window that the dialog should
 *   be modal for.
 * - `SDL_PROP_FILE_DIALOG_LOCATION_STRING`: the default folder or file to
 *   start the dialog at.
 * - `SDL_PROP_FILE_DIALOG_MANY_BOOLEAN`: true to allow the user to select
 *   more than one entry.
 * - `SDL_PROP_FILE_DIALOG_TITLE_STRING`: the title for the dialog.
 * - `SDL_PROP_FILE_DIALOG_ACCEPT_STRING`: the label that the accept button
 *   should have.
 * - `SDL_PROP_FILE_DIALOG_CANCEL_STRING`: the label that the cancel button
 *   should have.
 *
 * Note that each platform may or may not support any of the properties.
 *
 * @param type the type of file dialog.
 * @param callback a function pointer to be invoked when the user selects a
 *                 file and accepts, or cancels the dialog, or an error
 *                 occurs.
 * @param props the properties to use.
 *
 * @threadsafety This function should be called only from the main thread. The
 *               callback may be invoked from the same thread or from a
 *               different one, depending on the OS's constraints.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FileDialogType
 * @sa DialogFileCallback
 * @sa DialogFileFilter
 * @sa ShowOpenFileDialog
 * @sa ShowSaveFileDialog
 * @sa ShowOpenFolderDialog
 */
inline void ShowFileDialogWithProperties(FileDialogType type,
                                         DialogFileCB callback,
                                         PropertiesRef props)
{

  using Wrapper = CallbackWrapper<DialogFileCB>;
  ShowFileDialogWithProperties(
    type, &Wrapper::CallOnce, Wrapper::Wrap(std::move(callback)), props);
}

namespace prop::FileDialog {

constexpr auto FILTERS_POINTER = SDL_PROP_FILE_DIALOG_FILTERS_POINTER;

constexpr auto NFILTERS_NUMBER = SDL_PROP_FILE_DIALOG_NFILTERS_NUMBER;

constexpr auto WINDOW_POINTER = SDL_PROP_FILE_DIALOG_WINDOW_POINTER;

constexpr auto LOCATION_STRING = SDL_PROP_FILE_DIALOG_LOCATION_STRING;

constexpr auto MANY_BOOLEAN = SDL_PROP_FILE_DIALOG_MANY_BOOLEAN;

constexpr auto TITLE_STRING = SDL_PROP_FILE_DIALOG_TITLE_STRING;

constexpr auto ACCEPT_STRING = SDL_PROP_FILE_DIALOG_ACCEPT_STRING;

constexpr auto CANCEL_STRING = SDL_PROP_FILE_DIALOG_CANCEL_STRING;

} // namespace prop::FileDialog

/// @}
} // namespace SDL

#endif /* SDL3PP_DIALOG_H_ */
