/**
 * @file 01_power.cpp
 *
 * Based on SDL3's power.c
 *
 * This example code reports power status (plugged in, battery level, etc).
 *
 * This code is public domain. Feel free to use it for any purpose!
 */
#define SDL3PP_MAIN_USE_CLASS_CALLBACKS
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

struct Main : SDL::AppInterface
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::Window window{
    SDL::CreateWindowAndRenderer("examples/misc/power", windowSz, 0, nullptr)};
  SDL::RendererRef renderer{window.GetRenderer()};

  SDL::AppResult Iterate() final
  {
    constexpr SDL::FRect frame{100, 200, 440, 80};

    /* Query for battery info */
    int seconds = 0;
    int percent = 0;
    const SDL::PowerState state = SDL::GetPowerInfo(&seconds, &percent);

    /* We set up different drawing details for each power state, then
      run it all through the same drawing code. */
    SDL::Color clearc{0, 0, 0};      // clear window to this color.
    SDL::Color textc{255, 255, 255}; // draw messages in this color.
    SDL::Color framec{
      255, 255, 255};         // draw a percentage bar frame in this color.
    SDL::Color barc{0, 0, 0}; // draw a percentage bar in this color.
    std::string_view msg;
    std::string_view msg2;

    switch (state) {
    case SDL_POWERSTATE_ERROR:
      msg2 = "ERROR GETTING POWER STATE";
      msg = SDL_GetError();
      clearc.r = 255; /* red background */
      break;

    case SDL_POWERSTATE_ON_BATTERY:
      msg = "Running on battery.";
      barc.r = 255; /* draw in red */
      break;

    case SDL_POWERSTATE_NO_BATTERY:
      msg = "Plugged in, no battery available.";
      clearc.g = 50; /* green background */
      break;

    case SDL_POWERSTATE_CHARGING:
      msg = "Charging.";
      barc.b = barc.g = 255; /* draw in cyan */
      break;

    case SDL_POWERSTATE_CHARGED:
      msg = "Charged.";
      barc.g = 255; /* draw in green */
      break;

    // case SDL_POWERSTATE_UNKNOWN:
    default: /* in case this does something unexpected later, treat it as
                unknown. */
      msg = "Power state is unknown.";
      clearc.r = clearc.b = clearc.g = 50; /* grey background */
      break;
    }

    renderer.SetDrawColor(clearc);
    renderer.RenderClear();

    if (percent >= 0) {
      SDL::FRect pctrect = frame;
      pctrect.w *= float(percent) / 100.0f;

      std::string remainstr;
      if (seconds < 0) {
        remainstr = "unknown time";
      } else {
        int hours = seconds / (60 * 60);
        seconds -= hours * (60 * 60);
        int minutes = seconds / 60;
        seconds -= minutes * 60;
        remainstr = std::format("{:02}:{:02}:{:02}", hours, minutes, seconds);
      }

      std::string msgbuf =
        std::format("Battery: {:3} percent, {} remaining", percent, remainstr);
      SDL::FPoint p(frame.x +
                      ((frame.w - float(SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE *
                                        msgbuf.size())) /
                       2.0f),
                    frame.y + frame.h + SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE);

      renderer.SetDrawColor(barc);
      renderer.RenderFillRect(pctrect);
      renderer.SetDrawColor(framec);
      renderer.RenderRect(frame);
      renderer.SetDrawColor(textc);
      renderer.RenderDebugText(p, msgbuf);
    }

    if (!msg.empty()) {
      SDL::FPoint p{
        frame.x +
          ((frame.w - float(SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE * msg.size())) /
           2.0f),
        frame.y - (SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE * 2)};
      renderer.SetDrawColor(textc);
      renderer.RenderDebugText(p, msg);
    }

    if (!msg2.empty()) {
      SDL::FPoint p{
        frame.x + ((frame.w -
                    float(SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE * msg2.size())) /
                   2.0f),
        frame.y - SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE * 4,
      };
      renderer.SetDrawColor(textc);
      renderer.RenderDebugText(p, msg2);
    }

    // put the new rendering on the screen.
    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CLASS_CALLBACKS(Main,
                              SDL::INIT_VIDEO,
                              "Example Misc Power",
                              "1.0",
                              "com.example.misc-power")
