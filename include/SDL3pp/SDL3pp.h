/**
 * Main include header for the SDL3pp library
 *
 * It is almost always best to include just this one header instead of
 * picking out individual headers included here. There are exceptions to
 * this rule--SDL3pp_main.h is special and not included here--but usually
 * letting SDL3pp.h include the kitchen sink for you is the correct approach.
 */
#ifndef SDL3PP_H_
#define SDL3PP_H_

#include <SDL3/SDL.h>
#include "SDL3pp_assert.h"
// #include "SDL3pp_asyncio.h"
// #include "SDL3pp_atomic.h"
// #include "SDL3pp_audio.h"
#include "SDL3pp_bits.h"
#include "SDL3pp_blendmode.h"
// #include "SDL3pp_camera.h"
// #include "SDL3pp_clipboard.h"
#include "SDL3pp_cpuinfo.h"
#include "SDL3pp_dialog.h"
#include "SDL3pp_endian.h"
#include "SDL3pp_error.h"
#include "SDL3pp_events.h"
// #include "SDL3pp_filesystem.h"
// #include "SDL3pp_gamepad.h"
// #include "SDL3pp_gpu.h"
#include "SDL3pp_guid.h"
// #include "SDL3pp_haptic.h"
// #include "SDL3pp_hidapi.h"
#include "SDL3pp_hints.h"
#include "SDL3pp_init.h"
#include "SDL3pp_intrin.h"
#include "SDL3pp_iostream.h"
// #include "SDL3pp_joystick.h"
#include "SDL3pp_keyboard.h"
#include "SDL3pp_keycode.h"
#include "SDL3pp_loadso.h"
#include "SDL3pp_locale.h"
#include "SDL3pp_log.h"
#include "SDL3pp_messagebox.h"
// #include "SDL3pp_metal.h"
#include "SDL3pp_misc.h"
// #include "SDL3pp_mouse.h"
// #include "SDL3pp_mutex.h"
// #include "SDL3pp_pen.h"
#include "SDL3pp_pixels.h"
#include "SDL3pp_platform.h"
#include "SDL3pp_power.h"
// #include "SDL3pp_process.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_render.h"
#include "SDL3pp_scancode.h"
// #include "SDL3pp_sensor.h"
#include "SDL3pp_stdinc.h"
// #include "SDL3pp_storage.h"
#include "SDL3pp_surface.h"
// #include "SDL3pp_system.h"
// #include "SDL3pp_thread.h"
#include "SDL3pp_time.h"
#include "SDL3pp_timer.h"
// #include "SDL3pp_touch.h"
#include "SDL3pp_tray.h"
#include "SDL3pp_version.h"
#include "SDL3pp_video.h"

// Here we have additional "satellite" libraries
#include "SDL3pp_image.h"
#include "SDL3pp_ttf.h"

#endif /* SDL3PP_H_ */
