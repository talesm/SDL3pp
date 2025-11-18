#ifndef SDL3PP_VULKAN_H_
#define SDL3PP_VULKAN_H_

#include <SDL3/SDL_vulkan.h>
#include "SDL3pp.h"

namespace SDL {

/**
 * @defgroup CategoryVulkan Vulkan Support
 *
 * Functions for creating Vulkan surfaces on SDL windows.
 *
 * For the most part, Vulkan operates independent of SDL, but it benefits from a
 * little support during setup.
 *
 * Use Vulkan_GetInstanceExtensions() to get platform-specific bits for creating
 * a VkInstance, then Vulkan_GetVkGetInstanceProcAddr() to get the appropriate
 * function for querying Vulkan entry points. Then Vulkan_CreateSurface() will
 * get you the final pieces you need to prepare for rendering into an Window
 * with Vulkan.
 *
 * Unlike OpenGL, most of the details of "context" creation and window buffer
 * swapping are handled by the Vulkan API directly, so SDL doesn't provide
 * Vulkan equivalents of Window.GL_Swap(), etc; they aren't necessary.
 *
 * @{
 */

/**
 * Dynamically load the Vulkan loader library.
 *
 * This should be called after initializing the video driver, but before
 * creating any Vulkan windows. If no Vulkan loader library is loaded, the
 * default library will be loaded upon creation of the first Vulkan window.
 *
 * SDL keeps a counter of how many times this function has been successfully
 * called, so it is safe to call this function multiple times, so long as it is
 * eventually paired with an equivalent number of calls to Vulkan_UnloadLibrary.
 * The `path` argument is ignored unless there is no library currently loaded,
 * and and the library isn't actually unloaded until there have been an
 * equivalent number of calls to Vulkan_UnloadLibrary.
 *
 * It is fairly common for Vulkan applications to link with libvulkan instead of
 * explicitly loading it at run time. This will work with SDL provided the
 * application links to a dynamic library and both it and SDL use the same
 * search path.
 *
 * If you specify a non-nullptr `path`, an application should retrieve all of
 * the Vulkan functions it uses from the dynamic library using
 * Vulkan_GetVkGetInstanceProcAddr unless you can guarantee `path` points to the
 * same vulkan loader library the application linked to.
 *
 * On Apple devices, if `path` is nullptr, SDL will attempt to find the
 * `vkGetInstanceProcAddr` address within all the Mach-O images of the current
 * process. This is because it is fairly common for Vulkan applications to link
 * with libvulkan (and historically MoltenVK was provided as a static library).
 * If it is not found, on macOS, SDL will attempt to load
 * `vulkan.framework/vulkan`, `libvulkan.1.dylib`,
 * `MoltenVK.framework/MoltenVK`, and `libMoltenVK.dylib`, in that order. On
 * iOS, SDL will attempt to load `libMoltenVK.dylib`. Applications using a
 * dynamic framework or .dylib must ensure it is included in its application
 * bundle.
 *
 * On non-Apple devices, application linking with a static libvulkan is not
 * supported. Either do not link to the Vulkan loader or link to a dynamic
 * library version.
 *
 * @param path the platform dependent Vulkan loader library name or nullptr.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Vulkan_GetVkGetInstanceProcAddr
 * @sa Vulkan_UnloadLibrary
 */
inline void Vulkan_LoadLibrary(StringParam path)
{
  CheckError(SDL_Vulkan_LoadLibrary(path));
}

/**
 * Get the address of the `vkGetInstanceProcAddr` function.
 *
 * This should be called after either calling Vulkan_LoadLibrary() or creating
 * an Window with the `WINDOW_VULKAN` flag.
 *
 * The actual type of the returned function pointer is
 * PFN_vkGetInstanceProcAddr, but that isn't available because the Vulkan
 * headers are not included here. You should cast the return value of this
 * function to that type, e.g.
 *
 * `vkGetInstanceProcAddr =
 * (PFN_vkGetInstanceProcAddr)Vulkan_GetVkGetInstanceProcAddr();`
 *
 * @returns the function pointer for `vkGetInstanceProcAddr` or nullptr on
 *          failure; call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline FunctionPointer Vulkan_GetVkGetInstanceProcAddr()
{
  return SDL_Vulkan_GetVkGetInstanceProcAddr();
}

/**
 * Unload the Vulkan library previously loaded by Vulkan_LoadLibrary().
 *
 * SDL keeps a counter of how many times this function has been called, so it is
 * safe to call this function multiple times, so long as it is paired with an
 * equivalent number of calls to Vulkan_LoadLibrary. The library isn't actually
 * unloaded until there have been an equivalent number of calls to
 * Vulkan_UnloadLibrary.
 *
 * Once the library has actually been unloaded, if any Vulkan instances remain,
 * they will likely crash the program. Clean up any existing Vulkan resources,
 * and destroy appropriate windows, renderers and GPU devices before calling
 * this function.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Vulkan_LoadLibrary
 */
inline void Vulkan_UnloadLibrary() { SDL_Vulkan_UnloadLibrary(); }

/**
 * Get the Vulkan instance extensions needed for vkCreateInstance.
 *
 * This should be called after either calling Vulkan_LoadLibrary() or creating
 * an Window with the `WINDOW_VULKAN` flag.
 *
 * On return, the variable pointed to by `count` will be set to the number of
 * elements returned, suitable for using with
 * VkInstanceCreateInfo::enabledExtensionCount, and the returned array can be
 * used with VkInstanceCreateInfo::ppEnabledExtensionNames, for calling Vulkan's
 * vkCreateInstance API.
 *
 * @returns an array of extension name strings on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Vulkan_CreateSurface
 */
inline std::span<char const* const> Vulkan_GetInstanceExtensions()
{
  Uint32 count;
  auto data = CheckError(SDL_Vulkan_GetInstanceExtensions(&count));
  return std::span<char const* const>(data, size_t(count));
}

/**
 * Create a Vulkan rendering surface for a window.
 *
 * The `window` must have been created with the `WINDOW_VULKAN` flag and
 * `instance` must have been created with extensions returned by
 * Vulkan_GetInstanceExtensions() enabled.
 *
 * If `allocator` is nullptr, Vulkan will use the system default allocator. This
 * argument is passed directly to Vulkan and isn't used by SDL itself.
 *
 * @param window the window to which to attach the Vulkan surface.
 * @param instance the Vulkan instance handle.
 * @param allocator a VkAllocationCallbacks struct, which lets the app set the
 *                  allocator that creates the surface. Can be nullptr.
 * @param surface a pointer to a VkSurfaceKHR handle to output the newly created
 *                surface.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Vulkan_GetInstanceExtensions
 * @sa Vulkan_DestroySurface
 */
inline void Vulkan_CreateSurface(WindowParam window,
                                 VkInstance instance,
                                 const struct VkAllocationCallbacks* allocator,
                                 VkSurfaceKHR* surface)
{
  CheckError(SDL_Vulkan_CreateSurface(window, instance, allocator, surface));
}

/**
 * Destroy the Vulkan rendering surface of a window.
 *
 * This should be called before Window.Destroy, if Vulkan_CreateSurface was
 * called after Window.Window.
 *
 * The `instance` must have been created with extensions returned by
 * Vulkan_GetInstanceExtensions() enabled and `surface` must have been created
 * successfully by an Vulkan_CreateSurface() call.
 *
 * If `allocator` is nullptr, Vulkan will use the system default allocator. This
 * argument is passed directly to Vulkan and isn't used by SDL itself.
 *
 * @param instance the Vulkan instance handle.
 * @param surface vkSurfaceKHR handle to destroy.
 * @param allocator a VkAllocationCallbacks struct, which lets the app set the
 *                  allocator that destroys the surface. Can be nullptr.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Vulkan_GetInstanceExtensions
 * @sa Vulkan_CreateSurface
 */
inline void Vulkan_DestroySurface(VkInstance instance,
                                  VkSurfaceKHR surface,
                                  const struct VkAllocationCallbacks* allocator)
{
  SDL_Vulkan_DestroySurface(instance, surface, allocator);
}

/**
 * Query support for presentation via a given physical device and queue family.
 *
 * The `instance` must have been created with extensions returned by
 * Vulkan_GetInstanceExtensions() enabled.
 *
 * @param instance the Vulkan instance handle.
 * @param physicalDevice a valid Vulkan physical device handle.
 * @param queueFamilyIndex a valid queue family index for the given physical
 *                         device.
 * @returns true if supported, false if unsupported or an error occurred.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Vulkan_GetInstanceExtensions
 */
inline bool Vulkan_GetPresentationSupport(VkInstance instance,
                                          VkPhysicalDevice physicalDevice,
                                          Uint32 queueFamilyIndex)
{
  return SDL_Vulkan_GetPresentationSupport(
    instance, physicalDevice, queueFamilyIndex);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_VULKAN_H_ */
