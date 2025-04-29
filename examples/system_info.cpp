#include <iostream>
#include <vector>
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  static constexpr SDL::Point LOG_SZ = {600, 400};

  void showInfo()
  {
    SDL::Point cursor{4, 4};
    auto rollDown = [&]() {
      cursor.y += SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE + 4;
    };
    auto show = [&](auto&&... args) {
      renderer.RenderDebugTextFormat(cursor, args...);
      rollDown();
    };
    {
      int seconds, percent;
      SDL::GetPowerInfo(&seconds, &percent);
      show("Power Status: {}", percent);
      show("  {} seconds remaining", seconds);
      rollDown();
    }
    {
      SDL::Time currTime = SDL::Time::Current();
      SDL::DateTime localTime{currTime};
      int offsetHours = localTime.utc_offset / 60 / 60;
      int offsetMinutes = std::abs(localTime.utc_offset / 60) % 60;
      show("Local Time:   {:04}-{:02}-{:02} {:02}:{:02}:{:02}{:+03}:{:02}",
           localTime.year,
           localTime.month,
           localTime.day,
           localTime.hour,
           localTime.minute,
           localTime.second,
           offsetHours,
           offsetMinutes);
      SDL::DateTime utcTime{currTime, false};
      show("UTC Time:     {:04}-{:02}-{:02} {:02}:{:02}:{:02}Z",
           utcTime.year,
           utcTime.month,
           utcTime.day,
           utcTime.hour,
           utcTime.minute,
           utcTime.second);
      rollDown();
    }
    {
      static const char* platform = SDL::GetPlatform();
      show("Platform:     {}", platform);
      show("Byte order:   {} endian",
           SDL_BYTEORDER == SDL_LIL_ENDIAN ? "little" : "big");
      show("Num log cores:{}", SDL::GetNumLogicalCPUCores());
      show("Cache line sz:{}", SDL::GetCPUCacheLineSize());
      show("System RAM:   {} MiB", SDL::GetSystemRAM());
      static SDL::SystemTheme systemTheme = SDL::GetSystemTheme();
      const char* systemThemeString;
      switch (systemTheme) {
      case SDL::SYSTEM_THEME_DARK: systemThemeString = "Dark"; break;
      case SDL::SYSTEM_THEME_LIGHT: systemThemeString = "Light"; break;
      default: systemThemeString = "Unknown"; break;
      }
      show("System Theme: {}", systemThemeString);

      rollDown();
    }
    {
      show("CPU Capabilities:");
      show("Has AltiVec:  {}", SDL::HasAltiVec());
      show("Has MMX:      {}", SDL::HasMMX());
      show("Has SSE:      {}", SDL::HasSSE());
      show("Has SSE2:     {}", SDL::HasSSE2());
      show("Has SSE3:     {}", SDL::HasSSE3());
      show("Has SSE41:    {}", SDL::HasSSE41());
      show("Has SSE42:    {}", SDL::HasSSE42());
      show("Has AVX:      {}", SDL::HasAVX());
      show("Has AVX2:     {}", SDL::HasAVX2());
      show("Has AVX512F:  {}", SDL::HasAVX512F());
      show("Has ARMSIMD:  {}", SDL::HasARMSIMD());
      show("Has NEON:     {}", SDL::HasNEON());
      show("Has LSX:      {}", SDL::HasLSX());
      show("Has LASX:     {}", SDL::HasLASX());
      rollDown();
    }
    {
      cursor.y = 4;
      cursor.x += 40 * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
      static auto locales{SDL::GetPreferredLocales()};
      show("Preferred Locales:");
      for (auto& locale : locales) {
        if (locale->country) {
          show("- {}/{}", locale->language, locale->country);
        } else {
          show("- {}", locale->language);
        }
      }
      rollDown();
    }
  }

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window;
  SDL::Renderer renderer;
  static constexpr SDL::Point WINDOW_SZ = LOG_SZ * 2;

  Main()
  {
    std::tie(window, renderer) =
      SDL::CreateWindowAndRenderer("Test", WINDOW_SZ);
    renderer.SetScale({2.f, 2.f});
  }

  SDL::AppResult Iterate()
  {
    renderer.SetDrawColor(SDL::FColor{.75f, .75f, .75f, 1.f});
    renderer.RenderClear();
    renderer.SetDrawColor(SDL::FColor{.0f, .0f, .0f, 1.f});
    showInfo();

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main,
                        "Example System info",
                        "1.0",
                        "com.example.system-info")
