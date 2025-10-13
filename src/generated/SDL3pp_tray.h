#ifndef SDL3PP_TRAY_H_
#define SDL3PP_TRAY_H_

#include <SDL3/SDL_tray.h>
#include "SDL3pp_stdinc.h"
#include "SDL3pp_surface.h"

namespace SDL {

/**
 * @defgroup CategoryTray Category Tray
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
struct Tray;

/// Alias to raw representation for Tray.
using TrayRaw = SDL_Tray*;

// Forward decl
struct TrayRef;

/// Safely wrap Tray for non owning parameters
struct TrayParam
{
  TrayRaw value; ///< parameter's TrayRaw

  /// Constructs from TrayRaw
  constexpr TrayParam(TrayRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr TrayParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const TrayParam& other) const = default;

  /// Converts to underlying TrayRaw
  constexpr operator TrayRaw() const { return value; }
};

/// Alias to raw representation for TrayMenu.
using TrayMenuRaw = SDL_TrayMenu*;

// Forward decl
struct TrayEntry;

/// Alias to raw representation for TrayEntry.
using TrayEntryRaw = SDL_TrayEntry*;

// Forward decl
struct TrayEntryScoped;

/// Safely wrap TrayEntry for non owning parameters
struct TrayEntryParam
{
  TrayEntryRaw value; ///< parameter's TrayEntryRaw

  /// Constructs from TrayEntryRaw
  constexpr TrayEntryParam(TrayEntryRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr TrayEntryParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const TrayEntryParam& other) const = default;

  /// Converts to underlying TrayEntryRaw
  constexpr operator TrayEntryRaw() const { return value; }
};

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

/// Make the entry checked. This is valid only for checkboxes. Optional.
constexpr TrayEntryFlags TRAYENTRY_CHECKED = SDL_TRAYENTRY_CHECKED;

/**
 * A callback that is invoked when a tray entry is selected.
 *
 * @param userdata an optional pointer to pass extra data to the callback when
 *                 it will be invoked.
 * @param entry the tray entry that was selected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa TrayEntry.SetCallback
 */
using TrayCallback = SDL_TrayCallback;

using TrayCB = std::function<void(TrayEntryRaw)>;

/**
 * An opaque handle representing a toplevel system tray object.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class Tray
{
  TrayRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Tray() = default;

  /**
   * Constructs from TrayParam.
   *
   * @param resource a TrayRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Tray(const TrayRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Tray(const Tray& other) = delete;

  /// Move constructor
  constexpr Tray(Tray&& other)
    : Tray(other.release())
  {
  }

  constexpr Tray(const TrayRef& other) = delete;

  constexpr Tray(TrayRef&& other) = delete;

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
   * @sa Tray.CreateMenu
   * @sa Tray.GetMenu
   * @sa Tray.Destroy
   */
  Tray(SurfaceParam icon, StringParam tooltip)
    : m_resource(SDL_CreateTray(icon, tooltip))
  {
  }

  /// Destructor
  ~Tray() { SDL_DestroyTray(m_resource); }

  /// Assignment operator.
  Tray& operator=(Tray other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying TrayRaw.
  constexpr TrayRaw get() const { return m_resource; }

  /// Retrieves underlying TrayRaw and clear this.
  constexpr TrayRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Tray& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to TrayParam
  constexpr operator TrayParam() const { return {m_resource}; }

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
   * @sa Tray.Tray
   */
  void Destroy();

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
   * @sa Tray.Tray
   */
  void SetIcon(SurfaceParam icon);

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
   * @sa Tray.Tray
   */
  void SetTooltip(StringParam tooltip);

  /**
   * Create a menu for a system tray.
   *
   * This should be called at most once per tray icon.
   *
   * This function does the same thing as TrayEntry.CreateSubmenu(), except that
   * it takes a Tray instead of a TrayEntry.
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
   * @sa Tray.Tray
   * @sa Tray.GetMenu
   * @sa TrayMenu.GetParentTray
   */
  TrayMenu CreateMenu();

  /**
   * Gets a previously created tray menu.
   *
   * You should have called Tray.CreateMenu() on the tray object. This
   * function allows you to fetch it again later.
   *
   * This function does the same thing as TrayEntry.GetSubmenu(), except that it
   * takes a Tray instead of a TrayEntry.
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
   * @sa Tray.Tray
   * @sa Tray.CreateMenu
   */
  TrayMenu GetMenu() const;
};

/// Semi-safe reference for Tray.
struct TrayRef : Tray
{
  /**
   * Constructs from TrayParam.
   *
   * @param resource a TrayRaw or Tray.
   *
   * This does not takes ownership!
   */
  TrayRef(TrayParam resource)
    : Tray(resource.value)
  {
  }

  /// Copy constructor.
  TrayRef(const TrayRef& other)
    : Tray(other.get())
  {
  }

  /// Destructor
  ~TrayRef() { release(); }
};

/**
 * An opaque handle representing a menu/submenu on a system tray object.
 *
 * @since This struct is available since SDL 3.2.0.
 */
class TrayMenu
{
  TrayMenuRaw m_trayMenu;

public:
  /**
   * Wraps TrayMenu.
   *
   * @param trayMenu the value to be wrapped
   */
  constexpr TrayMenu(TrayMenuRaw trayMenu = {})
    : m_trayMenu(trayMenu)
  {
  }

  /**
   * Unwraps to the underlying TrayMenu.
   *
   * @returns the underlying TrayMenuRaw.
   */
  constexpr operator TrayMenuRaw() const { return m_trayMenu; }

  /**
   * Returns a list of entries in the menu, in order.
   *
   * @param count An optional pointer to obtain the number of entries in the
   *              menu.
   * @returns a nullptr-terminated list of entries within the given menu. The
   *          pointer becomes invalid when any function that inserts or deletes
   *          entries in the menu is called.
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayEntry.Remove
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
   * @sa TrayEntryFlags
   * @sa TrayMenu.GetEntries
   * @sa TrayEntry.Remove
   * @sa TrayEntry.GetParent
   */
  TrayEntry InsertEntry(int pos, StringParam label, TrayEntryFlags flags);

  TrayEntry AppendEntry(StringParam label, TrayEntryFlags flags);

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
   * @sa TrayEntry.CreateSubmenu
   * @sa TrayMenu.GetParentTray
   */
  TrayEntryParam GetParentEntry() const;

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
   * @sa Tray.CreateMenu
   * @sa TrayMenu.GetParentEntry
   */
  TrayParam GetParentTray() const;
};

/**
 * An opaque handle representing an entry on a system tray object.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class TrayEntry
{
  TrayEntryRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr TrayEntry() = default;

  /**
   * Constructs from TrayEntryParam.
   *
   * @param resource a TrayEntryRaw to be wrapped.
   */
  constexpr TrayEntry(const TrayEntryRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr TrayEntry(const TrayEntry& other) = default;

  /// Move constructor
  constexpr TrayEntry(TrayEntry&& other)
    : TrayEntry(other.release())
  {
  }

  /// Destructor
  ~TrayEntry() {}

  /// Assignment operator.
  TrayEntry& operator=(TrayEntry other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying TrayEntryRaw.
  constexpr TrayEntryRaw get() const { return m_resource; }

  /// Retrieves underlying TrayEntryRaw and clear this.
  constexpr TrayEntryRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const TrayEntry& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to TrayEntryParam
  constexpr operator TrayEntryParam() const { return {m_resource}; }

  /**
   * Removes a tray entry.
   *
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa TrayMenu.GetEntries
   * @sa TrayMenu.InsertEntry
   */
  void Remove();

  /**
   * Create a submenu for a system tray entry.
   *
   * This should be called at most once per tray entry.
   *
   * This function does the same thing as Tray.CreateMenu, except that it
   * takes a TrayEntry instead of a Tray.
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
   * @sa TrayEntry.GetSubmenu
   * @sa TrayMenu.GetParentEntry
   */
  TrayMenu CreateSubmenu();

  /**
   * Gets a previously created tray entry submenu.
   *
   * You should have called TrayEntry.CreateSubmenu() on the entry object. This
   * function allows you to fetch it again later.
   *
   * This function does the same thing as Tray.GetMenu(), except that it
   * takes a TrayEntry instead of a Tray.
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
   * @sa TrayEntry.CreateSubmenu
   */
  TrayMenu GetSubmenu();

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
   * @sa TrayEntry.GetLabel
   */
  void SetLabel(StringParam label);

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
   * @sa TrayEntry.SetLabel
   */
  const char* GetLabel() const;

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
   * @sa TrayEntry.GetChecked
   */
  void SetChecked(bool checked);

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
   * @sa TrayEntry.SetChecked
   */
  bool GetChecked() const;

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
   * @sa TrayEntry.GetEnabled
   */
  void SetEnabled(bool enabled);

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
   * @sa TrayEntry.SetEnabled
   */
  bool GetEnabled() const;

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
  void SetCallback(TrayCallback callback, void* userdata);

  /**
   * Simulate a click on a tray entry.
   *
   *
   * @threadsafety This function should be called on the thread that created the
   *               tray.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Click();

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
  TrayMenu GetParent();
};

/// RAII owning version TrayEntry.
struct TrayEntryScoped : TrayEntry
{
  using TrayEntry::TrayEntry;

  constexpr TrayEntryScoped(const TrayEntry& other) = delete;

  /// Move constructor
  constexpr TrayEntryScoped(TrayEntry&& other)
    : TrayEntry(other.release())
  {
  }

  /// Destructor
  ~TrayEntryScoped() { Remove(); }
};

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
 * @returns The newly created system tray icon.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Tray.CreateMenu
 * @sa Tray.GetMenu
 * @sa Tray.Destroy
 */
inline Tray CreateTray(SurfaceParam icon, StringParam tooltip)
{
  return Tray(icon, std::move(tooltip));
}

/**
 * Updates the system tray icon's icon.
 *
 * @param tray the tray icon to be updated.
 * @param icon the new icon. May be nullptr.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Tray.Tray
 */
inline void SetTrayIcon(TrayParam tray, SurfaceParam icon)
{
  SDL_SetTrayIcon(tray, icon);
}

inline void Tray::SetIcon(SurfaceParam icon)
{
  SDL::SetTrayIcon(m_resource, icon);
}

/**
 * Updates the system tray icon's tooltip.
 *
 * @param tray the tray icon to be updated.
 * @param tooltip the new tooltip in UTF-8 encoding. May be nullptr.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Tray.Tray
 */
inline void SetTrayTooltip(TrayParam tray, StringParam tooltip)
{
  SDL_SetTrayTooltip(tray, tooltip);
}

inline void Tray::SetTooltip(StringParam tooltip)
{
  SDL::SetTrayTooltip(m_resource, std::move(tooltip));
}

/**
 * Create a menu for a system tray.
 *
 * This should be called at most once per tray icon.
 *
 * This function does the same thing as TrayEntry.CreateSubmenu(), except that
 * it takes a Tray instead of a TrayEntry.
 *
 * A menu does not need to be destroyed; it will be destroyed with the tray.
 *
 * @param tray the tray to bind the menu to.
 * @returns the newly created menu.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Tray.Tray
 * @sa Tray.GetMenu
 * @sa TrayMenu.GetParentTray
 */
inline TrayMenu CreateTrayMenu(TrayParam tray)
{
  return SDL_CreateTrayMenu(tray);
}

inline TrayMenu Tray::CreateMenu() { return SDL::CreateTrayMenu(m_resource); }

/**
 * Create a submenu for a system tray entry.
 *
 * This should be called at most once per tray entry.
 *
 * This function does the same thing as Tray.CreateMenu, except that it
 * takes a TrayEntry instead of a Tray.
 *
 * A menu does not need to be destroyed; it will be destroyed with the tray.
 *
 * @param entry the tray entry to bind the menu to.
 * @returns the newly created menu.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.InsertEntry
 * @sa TrayEntry.GetSubmenu
 * @sa TrayMenu.GetParentEntry
 */
inline TrayMenu CreateTraySubmenu(TrayEntryParam entry)
{
  return SDL_CreateTraySubmenu(entry);
}

inline TrayMenu TrayEntry::CreateSubmenu()
{
  return SDL::CreateTraySubmenu(m_resource);
}

/**
 * Gets a previously created tray menu.
 *
 * You should have called Tray.CreateMenu() on the tray object. This
 * function allows you to fetch it again later.
 *
 * This function does the same thing as TrayEntry.GetSubmenu(), except that it
 * takes a Tray instead of a TrayEntry.
 *
 * A menu does not need to be destroyed; it will be destroyed with the tray.
 *
 * @param tray the tray entry to bind the menu to.
 * @returns the newly created menu.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Tray.Tray
 * @sa Tray.CreateMenu
 */
inline TrayMenu GetTrayMenu(TrayParam tray) { return SDL_GetTrayMenu(tray); }

inline TrayMenu Tray::GetMenu() const { return SDL::GetTrayMenu(m_resource); }

/**
 * Gets a previously created tray entry submenu.
 *
 * You should have called TrayEntry.CreateSubmenu() on the entry object. This
 * function allows you to fetch it again later.
 *
 * This function does the same thing as Tray.GetMenu(), except that it
 * takes a TrayEntry instead of a Tray.
 *
 * A menu does not need to be destroyed; it will be destroyed with the tray.
 *
 * @param entry the tray entry to bind the menu to.
 * @returns the newly created menu.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.InsertEntry
 * @sa TrayEntry.CreateSubmenu
 */
inline TrayMenu GetTraySubmenu(TrayEntryParam entry)
{
  return SDL_GetTraySubmenu(entry);
}

inline TrayMenu TrayEntry::GetSubmenu()
{
  return SDL::GetTraySubmenu(m_resource);
}

/**
 * Returns a list of entries in the menu, in order.
 *
 * @param menu The menu to get entries from.
 * @param count An optional pointer to obtain the number of entries in the
 *              menu.
 * @returns a nullptr-terminated list of entries within the given menu. The
 *          pointer becomes invalid when any function that inserts or deletes
 *          entries in the menu is called.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayEntry.Remove
 * @sa TrayMenu.InsertEntry
 */
inline std::span<TrayEntry> GetTrayEntries(TrayMenu menu)
{
  return SDL_GetTrayEntries(menu);
}

inline std::span<TrayEntry> TrayMenu::GetEntries()
{
  return SDL::GetTrayEntries(m_trayMenu);
}

/**
 * Removes a tray entry.
 *
 * @param entry The entry to be deleted.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.GetEntries
 * @sa TrayMenu.InsertEntry
 */
inline void RemoveTrayEntry(TrayEntryRaw entry) { SDL_RemoveTrayEntry(entry); }

inline void TrayEntry::Remove() { RemoveTrayEntry(release()); }

/**
 * Insert a tray entry at a given position.
 *
 * If label is nullptr, the entry will be a separator. Many functions won't work
 * for an entry that is a separator.
 *
 * An entry does not need to be destroyed; it will be destroyed with the tray.
 *
 * @param menu the menu to append the entry to.
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
 * @sa TrayEntryFlags
 * @sa TrayMenu.GetEntries
 * @sa TrayEntry.Remove
 * @sa TrayEntry.GetParent
 */
inline TrayEntry InsertTrayEntryAt(TrayMenu menu,
                                   int pos,
                                   StringParam label,
                                   TrayEntryFlags flags)
{
  return SDL_InsertTrayEntryAt(menu, pos, label, flags);
}

inline TrayEntry TrayMenu::InsertEntry(int pos,
                                       StringParam label,
                                       TrayEntryFlags flags)
{
  return SDL::InsertTrayEntryAt(m_trayMenu, pos, std::move(label), flags);
}

/**
 * Sets the label of an entry.
 *
 * An entry cannot change between a separator and an ordinary entry; that is,
 * it is not possible to set a non-nullptr label on an entry that has a nullptr
 * label (separators), or to set a nullptr label to an entry that has a
 * non-nullptr label. The function will silently fail if that happens.
 *
 * @param entry the entry to be updated.
 * @param label the new label for the entry in UTF-8 encoding.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.GetEntries
 * @sa TrayMenu.InsertEntry
 * @sa TrayEntry.GetLabel
 */
inline void SetTrayEntryLabel(TrayEntryParam entry, StringParam label)
{
  SDL_SetTrayEntryLabel(entry, label);
}

inline void TrayEntry::SetLabel(StringParam label)
{
  SDL::SetTrayEntryLabel(m_resource, std::move(label));
}

/**
 * Gets the label of an entry.
 *
 * If the returned value is nullptr, the entry is a separator.
 *
 * @param entry the entry to be read.
 * @returns the label of the entry in UTF-8 encoding.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.GetEntries
 * @sa TrayMenu.InsertEntry
 * @sa TrayEntry.SetLabel
 */
inline const char* GetTrayEntryLabel(TrayEntryParam entry)
{
  return SDL_GetTrayEntryLabel(entry);
}

inline const char* TrayEntry::GetLabel() const
{
  return SDL::GetTrayEntryLabel(m_resource);
}

/**
 * Sets whether or not an entry is checked.
 *
 * The entry must have been created with the TRAYENTRY_CHECKBOX flag.
 *
 * @param entry the entry to be updated.
 * @param checked true if the entry should be checked; false otherwise.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.GetEntries
 * @sa TrayMenu.InsertEntry
 * @sa TrayEntry.GetChecked
 */
inline void SetTrayEntryChecked(TrayEntryParam entry, bool checked)
{
  SDL_SetTrayEntryChecked(entry, checked);
}

inline void TrayEntry::SetChecked(bool checked)
{
  SDL::SetTrayEntryChecked(m_resource, checked);
}

/**
 * Gets whether or not an entry is checked.
 *
 * The entry must have been created with the TRAYENTRY_CHECKBOX flag.
 *
 * @param entry the entry to be read.
 * @returns true if the entry is checked; false otherwise.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.GetEntries
 * @sa TrayMenu.InsertEntry
 * @sa TrayEntry.SetChecked
 */
inline bool GetTrayEntryChecked(TrayEntryParam entry)
{
  return SDL_GetTrayEntryChecked(entry);
}

inline bool TrayEntry::GetChecked() const
{
  return SDL::GetTrayEntryChecked(m_resource);
}

/**
 * Sets whether or not an entry is enabled.
 *
 * @param entry the entry to be updated.
 * @param enabled true if the entry should be enabled; false otherwise.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.GetEntries
 * @sa TrayMenu.InsertEntry
 * @sa TrayEntry.GetEnabled
 */
inline void SetTrayEntryEnabled(TrayEntryParam entry, bool enabled)
{
  SDL_SetTrayEntryEnabled(entry, enabled);
}

inline void TrayEntry::SetEnabled(bool enabled)
{
  SDL::SetTrayEntryEnabled(m_resource, enabled);
}

/**
 * Gets whether or not an entry is enabled.
 *
 * @param entry the entry to be read.
 * @returns true if the entry is enabled; false otherwise.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.GetEntries
 * @sa TrayMenu.InsertEntry
 * @sa TrayEntry.SetEnabled
 */
inline bool GetTrayEntryEnabled(TrayEntryParam entry)
{
  return SDL_GetTrayEntryEnabled(entry);
}

inline bool TrayEntry::GetEnabled() const
{
  return SDL::GetTrayEntryEnabled(m_resource);
}

/**
 * Sets a callback to be invoked when the entry is selected.
 *
 * @param entry the entry to be updated.
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
inline void SetTrayEntryCallback(TrayEntryParam entry,
                                 TrayCallback callback,
                                 void* userdata)
{
  SDL_SetTrayEntryCallback(entry, callback, userdata);
}

inline void TrayEntry::SetCallback(TrayCallback callback, void* userdata)
{
  SDL::SetTrayEntryCallback(m_resource, callback, userdata);
}

/**
 * Simulate a click on a tray entry.
 *
 * @param entry The entry to activate.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ClickTrayEntry(TrayEntryParam entry) { SDL_ClickTrayEntry(entry); }

inline void TrayEntry::Click() { SDL::ClickTrayEntry(m_resource); }

/**
 * Destroys a tray object.
 *
 * This also destroys all associated menus and entries.
 *
 * @param tray the tray icon to be destroyed.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Tray.Tray
 */
inline void DestroyTray(TrayRaw tray) { SDL_DestroyTray(tray); }

inline void Tray::Destroy() { DestroyTray(release()); }

/**
 * Gets the menu containing a certain tray entry.
 *
 * @param entry the entry for which to get the parent menu.
 * @returns the parent menu.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayMenu.InsertEntry
 */
inline TrayMenu GetTrayEntryParent(TrayEntryParam entry)
{
  return SDL_GetTrayEntryParent(entry);
}

inline TrayMenu TrayEntry::GetParent()
{
  return SDL::GetTrayEntryParent(m_resource);
}

/**
 * Gets the entry for which the menu is a submenu, if the current menu is a
 * submenu.
 *
 * Either this function or TrayMenu.GetParentTray() will return non-nullptr
 * for any given menu.
 *
 * @param menu the menu for which to get the parent entry.
 * @returns the parent entry, or nullptr if this menu is not a submenu.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa TrayEntry.CreateSubmenu
 * @sa TrayMenu.GetParentTray
 */
inline TrayEntryParam GetTrayMenuParentEntry(TrayMenuRaw menu)
{
  return SDL_GetTrayMenuParentEntry(menu);
}

inline TrayEntryParam TrayMenu::GetParentEntry() const
{
  return SDL::GetTrayMenuParentEntry(m_trayMenu);
}

/**
 * Gets the tray for which this menu is the first-level menu, if the current
 * menu isn't a submenu.
 *
 * Either this function or TrayMenu.GetParentEntry() will return non-nullptr
 * for any given menu.
 *
 * @param menu the menu for which to get the parent enttrayry.
 * @returns the parent tray, or nullptr if this menu is a submenu.
 *
 * @threadsafety This function should be called on the thread that created the
 *               tray.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Tray.CreateMenu
 * @sa TrayMenu.GetParentEntry
 */
inline TrayParam GetTrayMenuParentTray(TrayMenuRaw menu)
{
  return SDL_GetTrayMenuParentTray(menu);
}

inline TrayParam TrayMenu::GetParentTray() const
{
  return SDL::GetTrayMenuParentTray(m_trayMenu);
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

/// @}

} // namespace SDL

#endif /* SDL3PP_TRAY_H_ */
