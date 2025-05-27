#ifndef SDL3PP_TRAY_H_
#define SDL3PP_TRAY_H_

#include <SDL3/SDL_tray.h>
#include "SDL3pp_stdinc.h"
#include "SDL3pp_surface.h"

namespace SDL {

/**
 *
 * @defgroup CategoryTray System Tray
 *
 * SDL offers a way to add items to the "system tray" (more correctly called
 * the "notification area" on Windows). On platforms that offer this concept,
 * an SDL app can add a tray icon, submenus, checkboxes, and clickable
 * entries, and register a callback that is fired when the user clicks on
 * these pieces.
 *
 * @{
 */

// Forward decl
struct TrayMenu;

// Forward decl
struct TrayRef;

// Forward decl
struct Tray;

// Forward decl
struct TrayEntryRef;

// Forward decl
struct TrayEntry;

/**
 * A trayEntry result that will be owned only if assigned to a TrayEntry.
 *
 * This is designed as resource types to cases where ownership might not be
 * required.
 */
using DetachedTrayEntry = DetachedResource<TrayEntryRef, TrayEntry>;

/**
 * Flags that control the creation of system tray entries.
 *
 * Some of these flags are required; exactly one of them must be specified at
 * the time a tray entry is created. Other flags are optional; zero or more of
 * those can be OR'ed together with the required flag.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa TrayMenu.InsertEntry
 */
using TrayEntryFlags = Uint32;

constexpr TrayEntryFlags TRAYENTRY_BUTTON =
  SDL_TRAYENTRY_BUTTON; ///< Make the entry a simple button. Required.

constexpr TrayEntryFlags TRAYENTRY_CHECKBOX =
  SDL_TRAYENTRY_CHECKBOX; ///< Make the entry a checkbox. Required.

constexpr TrayEntryFlags TRAYENTRY_SUBMENU =
  SDL_TRAYENTRY_SUBMENU; ///< Prepare the entry to have a submenu. Required

constexpr TrayEntryFlags TRAYENTRY_DISABLED =
  SDL_TRAYENTRY_DISABLED; ///< Make the entry disabled. Optional.

/**
 * Make the entry checked. This is valid only for checkboxes. Optional.
 */
constexpr TrayEntryFlags TRAYENTRY_CHECKED = SDL_TRAYENTRY_CHECKED;

/**
 * An opaque handle representing a toplevel system tray object.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Tray
 */
struct TrayRef : Resource<SDL_Tray*>
{
  using Resource::Resource;

  /**
   * Copy constructor.
   */
  constexpr TrayRef(const TrayRef& other)
    : TrayRef(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr TrayRef(TrayRef&& other)
    : TrayRef(other.release())
  {
  }

  TrayRef(Tray&& other) = delete;

  /**
   * Assignment operator.
   */
  TrayRef& operator=(TrayRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Updates the system tray icon's icon.
   *
   * @param icon the new icon. May be nullptr.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayRef.TrayRef
   */
  void SetIcon(SurfaceRef& icon) { SDL_SetTrayIcon(get(), icon.get()); }

  /**
   * Updates the system tray icon's tooltip.
   *
   * @param tooltip the new tooltip in UTF-8 encoding. May be nullptr.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayRef.TrayRef
   */
  void SetTooltip(StringParam tooltip) { SDL_SetTrayTooltip(get(), tooltip); }

  /**
   * Create a menu for a system tray.
   *
   * This should be called at most once per tray icon.
   *
   * This function does the same thing as TrayEntryRef.CreateSubmenu(), except
   * that it takes a TrayRef instead of a TrayEntryRef.
   *
   * A menu does not need to be destroyed; it will be destroyed with the tray.
   *
   * @returns the newly created menu.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayRef.TrayRef
   * @sa TrayRef.GetMenu
   * @sa TrayMenu.GetParentTray
   */
  TrayMenu CreateMenu();

  /**
   * Gets a previously created tray menu.
   *
   * You should have called TrayRef.CreateMenu() on the tray object. This
   * function allows you to fetch it again later.
   *
   * This function does the same thing as TrayEntryRef.GetSubmenu(), except
   * that it takes a TrayRef instead of a TrayEntryRef.
   *
   * A menu does not need to be destroyed; it will be destroyed with the tray.
   *
   * @returns the newly created menu.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayRef.TrayRef
   * @sa TrayRef.CreateMenu
   */
  TrayMenu GetMenu() const;

protected:
  /**
   * Destroys a tray object.
   *
   * This also destroys all associated menus and entries.
   *
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayRef.TrayRef
   */
  void Destroy() { reset(); }

  /**
   * Destroys a tray object.
   *
   * This also destroys all associated menus and entries.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayRef.TrayRef
   */
  void reset(SDL_Tray* newResource = {})
  {
    SDL_DestroyTray(release(newResource));
  }
};

/**
 * Unsafe Handle to tray
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa TrayRef
 */
struct TrayUnsafe : TrayRef
{
  using TrayRef::Destroy;

  using TrayRef::TrayRef;

  using TrayRef::reset;

  TrayUnsafe(const Tray& other) = delete;

  /**
   * Constructs TrayUnsafe from Tray.
   */
  explicit TrayUnsafe(Tray&& other);
};

/**
 * Handle to an owned tray
 *
 * @cat resource
 *
 * @sa TrayRef
 */
struct Tray : TrayUnsafe
{
  using TrayUnsafe::TrayUnsafe;

  /**
   * Constructs an empty Tray.
   */
  constexpr Tray()
    : TrayUnsafe(nullptr)
  {
  }

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Tray(SDL_Tray* resource)
    : TrayUnsafe(resource)
  {
  }

  constexpr Tray(const Tray& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Tray(Tray&& other) = default;

  /**
   * Create an icon to be placed in the operating system's tray, or equivalent.
   *
   * Many platforms advise not using a system tray unless persistence is a
   * necessary feature. Avoid needlessly creating a tray icon, as the user may
   * feel like it clutters their interface.
   *
   * Using tray icons require the video subsystem.
   *
   * @param icon a surface to be used as icon. May be nullptr.
   * @param tooltip a tooltip to be displayed when the mouse hovers the icon in
   *                UTF-8 encoding. Not supported on all platforms. May be
   * nullptr.
   * @post The newly created system tray icon.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayRef.CreateMenu
   * @sa TrayRef.GetMenu
   * @sa TrayRef.Destroy
   */
  Tray(SurfaceRef& icon, StringParam tooltip)
    : Tray(SDL_CreateTray(icon.get(), tooltip))
  {
  }

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Tray() { reset(); }

  /**
   * Assignment operator.
   */
  Tray& operator=(Tray other)
  {
    reset(other.release());
    return *this;
  }
};

/**
 * A callback that is invoked when a tray entry is selected.
 *
 * @param userdata an optional pointer to pass extra data to the callback when
 *                 it will be invoked.
 * @param entry the tray entry that was selected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa TrayEntryRef.SetCallback
 */
using TrayCallback = SDL_TrayCallback;

/**
 * A callback that is invoked when a tray entry is selected.
 *
 * @param entry the tray entry that was selected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa TrayEntryRef.SetCallback
 */
using TrayCB = std::function<void(TrayEntryRef)>;

inline TrayUnsafe::TrayUnsafe(Tray&& other)
  : TrayUnsafe(other.release())
{
}

/**
 * An opaque handle representing a menu/submenu on a system tray object.
 *
 * @since This struct is available since SDL 3.2.0.
 */
class TrayMenu
{
  SDL_TrayMenu* m_trayMenu;

public:
  /**
   * Wraps TrayMenu.
   *
   * @param trayMenu the value to be wrapped
   */
  constexpr TrayMenu(SDL_TrayMenu* trayMenu = {})
    : m_trayMenu(trayMenu)
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const TrayMenu& other) const = default;

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(SDL_TrayMenu* trayMenu) const
  {
    return operator==(TrayMenu(trayMenu));
  }

  /**
   * Unwraps to the underlying TrayMenu.
   *
   * @returns the underlying TrayMenu *.
   */
  constexpr operator SDL_TrayMenu*() const { return m_trayMenu; }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return m_trayMenu != nullptr; }

  /**
   * Returns a list of entries in the menu, in order.
   *
   * @returns a nullptr-terminated list of entries within the given menu. The
   *          pointer becomes invalid when any function that inserts or deletes
   *          entries in the menu is called.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayEntryRef.Remove
   * @sa TrayMenu.InsertEntry
   */
  std::span<TrayEntry> GetEntries();

  /**
   * Insert a tray entry at a given position.
   *
   * If label is nullptr, the entry will be a separator. Many functions won't
   * work for an entry that is a separator.
   *
   * An entry does not need to be destroyed; it will be destroyed with the tray.
   *
   * @param pos the desired position for the new entry. Entries at or following
   *            this place will be moved. If pos is -1, the entry is appended.
   * @param label the text to be displayed on the entry, in UTF-8 encoding, or
   *              nullptr for a separator.
   * @param flags a combination of flags, some of which are mandatory.
   * @returns the newly created entry, or nullptr if pos is out of bounds.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.AppendEntry
   * @sa TrayEntryFlags
   * @sa TrayMenu.GetEntries
   * @sa TrayEntryRef.Remove
   * @sa TrayEntryRef.GetParent
   */
  DetachedTrayEntry InsertEntry(int pos,
                                StringParam label,
                                TrayEntryFlags flags);

  /**
   * Appends a tray entry.
   *
   * If label is nullptr, the entry will be a separator. Many functions won't
   * work for an entry that is a separator.
   *
   * An entry does not need to be destroyed; it will be destroyed with the tray.
   *
   * @param label the text to be displayed on the entry, in UTF-8 encoding, or
   *              nullptr for a separator.
   * @param flags a combination of flags, some of which are mandatory.
   * @returns the newly created entry, or nullptr if pos is out of bounds.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.InsertEntry
   * @sa TrayEntryFlags
   * @sa TrayMenu.GetEntries
   * @sa TrayEntryRef.Remove
   * @sa TrayEntryRef.GetParent
   */
  DetachedTrayEntry AppendEntry(StringParam label, TrayEntryFlags flags);

  /**
   * Gets the entry for which the menu is a submenu, if the current menu is a
   * submenu.
   *
   * Either this function or TrayMenu.GetParentTray() will return non-nullptr
   * for any given menu.
   *
   * @returns the parent entry, or nullptr if this menu is not a submenu.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayEntryRef.CreateSubmenu
   * @sa TrayMenu.GetParentTray
   */
  TrayEntryRef GetParentEntry() const;

  /**
   * Gets the tray for which this menu is the first-level menu, if the current
   * menu isn't a submenu.
   *
   * Either this function or TrayMenu.GetParentEntry() will return non-nullptr
   * for any given menu.
   *
   * @returns the parent tray, or nullptr if this menu is a submenu.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayRef.CreateMenu
   * @sa TrayMenu.GetParentEntry
   */
  TrayRef GetParentTray() const;
};

/**
 * An opaque handle representing an entry on a system tray object.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa TrayEntry
 * @sa TrayEntryRef
 */
struct TrayEntryRef : Resource<SDL_TrayEntry*>
{
  using Resource::Resource;

  /**
   * Copy constructor.
   */
  constexpr TrayEntryRef(const TrayEntryRef& other)
    : TrayEntryRef(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr TrayEntryRef(TrayEntryRef&& other)
    : TrayEntryRef(other.release())
  {
  }

  TrayEntryRef(TrayEntry&& other) = delete;

  /**
   * Assignment operator.
   */
  TrayEntryRef& operator=(TrayEntryRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Create a submenu for a system tray entry.
   *
   * This should be called at most once per tray entry.
   *
   * This function does the same thing as TrayRef.CreateMenu, except that it
   * takes a TrayEntryRef instead of a TrayRef.
   *
   * A menu does not need to be destroyed; it will be destroyed with the tray.
   *
   * @returns the newly created menu.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.InsertEntry
   * @sa TrayEntryRef.GetSubmenu
   * @sa TrayMenu.GetParentEntry
   */
  TrayMenu CreateSubmenu() { return SDL_CreateTraySubmenu(get()); }

  /**
   * Gets a previously created tray entry submenu.
   *
   * You should have called TrayEntryRef.CreateSubmenu() on the entry object.
   * This function allows you to fetch it again later.
   *
   * This function does the same thing as TrayRef.GetMenu(), except that it
   * takes a TrayEntryRef instead of a TrayRef.
   *
   * A menu does not need to be destroyed; it will be destroyed with the tray.
   *
   * @returns the newly created menu.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.InsertEntry
   * @sa TrayEntryRef.CreateSubmenu
   */
  TrayMenu GetSubmenu() { return SDL_GetTraySubmenu(get()); }

  /**
   * Sets the label of an entry.
   *
   * An entry cannot change between a separator and an ordinary entry; that is,
   * it is not possible to set a non-nullptr label on an entry that has a
   * nullptr label (separators), or to set a nullptr label to an entry that has
   * a non-nullptr label. The function will silently fail if that happens.
   *
   * @param label the new label for the entry in UTF-8 encoding.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   * @sa TrayEntryRef.GetLabel
   */
  void SetLabel(StringParam label) { SDL_SetTrayEntryLabel(get(), label); }

  /**
   * Gets the label of an entry.
   *
   * If the returned value is nullptr, the entry is a separator.
   *
   * @returns the label of the entry in UTF-8 encoding.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   * @sa TrayEntryRef.SetLabel
   */
  const char* GetLabel() const { return SDL_GetTrayEntryLabel(get()); }

  /**
   * Sets whether or not an entry is checked.
   *
   * The entry must have been created with the TRAYENTRY_CHECKBOX flag.
   *
   * @param checked true if the entry should be checked; false otherwise.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   * @sa TrayEntryRef.GetChecked
   */
  void SetChecked(bool checked) { SDL_SetTrayEntryChecked(get(), checked); }

  /**
   * Gets whether or not an entry is checked.
   *
   * The entry must have been created with the TRAYENTRY_CHECKBOX flag.
   *
   * @returns true if the entry is checked; false otherwise.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   * @sa TrayEntryRef.SetChecked
   */
  bool GetChecked() const { return SDL_GetTrayEntryChecked(get()); }

  /**
   * Sets whether or not an entry is enabled.
   *
   * @param enabled true if the entry should be enabled; false otherwise.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   * @sa TrayEntryRef.GetEnabled
   */
  void SetEnabled(bool enabled) { SDL_SetTrayEntryEnabled(get(), enabled); }

  /**
   * Gets whether or not an entry is enabled.
   *
   * @returns true if the entry is enabled; false otherwise.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   * @sa TrayEntryRef.SetEnabled
   */
  bool GetEnabled() const { return SDL_GetTrayEntryEnabled(get()); }

  /**
   * Sets a callback to be invoked when the entry is selected.
   *
   * @param callback a callback to be invoked when the entry is selected.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   */
  void SetCallback(TrayCB callback);

  /**
   * Sets a callback to be invoked when the entry is selected.
   *
   * @param callback a callback to be invoked when the entry is selected.
   * @param userdata an optional pointer to pass extra data to the callback when
   *                 it will be invoked.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   */
  void SetCallback(TrayCallback callback, void* userdata)
  {
    SDL_SetTrayEntryCallback(get(), callback, userdata);
  }

  /**
   * Simulate a click on a tray entry.
   *
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Click() { SDL_ClickTrayEntry(get()); }

  /**
   * Gets the menu containing a certain tray entry.
   *
   * @returns the parent menu.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.InsertEntry
   */
  TrayMenu GetParent() { return SDL_GetTrayEntryParent(get()); }

protected:
  /**
   * Removes a tray entry.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   */
  void Remove() { reset(); }

  /**
   * Removes a tray entry.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   */
  void reset(SDL_TrayEntry* newResource = {})
  {
    SDL_RemoveTrayEntry(release(newResource));
  }
};

/**
 * Unsafe Handle to trayEntry
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa TrayEntryRef
 */
struct TrayEntryUnsafe : TrayEntryRef
{
  using TrayEntryRef::Remove;

  using TrayEntryRef::TrayEntryRef;

  using TrayEntryRef::reset;

  TrayEntryUnsafe(const TrayEntry& other) = delete;

  /**
   * Constructs TrayEntryUnsafe from TrayEntry.
   */
  explicit TrayEntryUnsafe(TrayEntry&& other);
};

/**
 * Handle to an owned trayEntry
 *
 * @cat resource
 *
 * @sa TrayEntryRef
 */
struct TrayEntry : TrayEntryUnsafe
{
  using TrayEntryUnsafe::TrayEntryUnsafe;

  /**
   * Constructs an empty TrayEntry.
   */
  constexpr TrayEntry()
    : TrayEntryUnsafe(nullptr)
  {
  }

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit TrayEntry(SDL_TrayEntry* resource)
    : TrayEntryUnsafe(resource)
  {
  }

  constexpr TrayEntry(const TrayEntry& other) = delete;

  /**
   * Move constructor.
   */
  constexpr TrayEntry(TrayEntry&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~TrayEntry() { reset(); }

  /**
   * Assignment operator.
   */
  TrayEntry& operator=(TrayEntry other)
  {
    reset(other.release());
    return *this;
  }
};

inline TrayEntryUnsafe::TrayEntryUnsafe(TrayEntry&& other)
  : TrayEntryUnsafe(other.release())
{
}

inline TrayMenu TrayRef::CreateMenu() { return SDL_CreateTrayMenu(get()); }

inline TrayMenu TrayRef::GetMenu() const { return SDL_GetTrayMenu(get()); }

inline std::span<TrayEntry> TrayMenu::GetEntries()
{
  int count;
  auto entries = SDL_GetTrayEntries(m_trayMenu, &count);
  return std::span<TrayEntry>{reinterpret_cast<TrayEntry*>(entries),
                              size_t(count)};
}

inline DetachedTrayEntry TrayMenu::InsertEntry(int pos,
                                               StringParam label,
                                               TrayEntryFlags flags)
{
  return SDL_InsertTrayEntryAt(m_trayMenu, pos, label, flags);
}

inline TrayEntryRef TrayMenu::GetParentEntry() const
{
  return SDL_GetTrayMenuParentEntry(m_trayMenu);
}

inline TrayRef TrayMenu::GetParentTray() const
{
  return SDL_GetTrayMenuParentTray(m_trayMenu);
}

/**
 * Update the trays.
 *
 * This is called automatically by the event loop and is only needed if you're
 * using trays but aren't handling SDL events.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void UpdateTrays() { SDL_UpdateTrays(); }

#pragma region impl
/// @}

inline DetachedTrayEntry TrayMenu::AppendEntry(StringParam label,
                                               TrayEntryFlags flags)
{
  return InsertEntry(-1, std::move(label), flags);
}

inline void TrayEntryRef::SetCallback(TrayCB callback)
{
  using Wrapper = KeyValueCallbackWrapper<SDL_TrayEntry*, TrayCB>;
  SetCallback(
    [](void* userdata, SDL_TrayEntry* entry) {
      Wrapper::Call(userdata, TrayEntryRef{entry});
    },
    Wrapper::Wrap(get(), std::move(callback)));
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_TRAY_H_ */
