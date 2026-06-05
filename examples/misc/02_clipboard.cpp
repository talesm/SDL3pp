/**
 * @file 02_clipboard.cpp
 *
 * Based on SDL3's clipboard.c
 *
 * This example code lets the user copy and paste with the system clipboard.
 *
 * This only handles text, but SDL supports other data types, too.
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

  SDL::Window window{SDL::CreateWindowAndRenderer("examples/misc/clipboard",
                                                  windowSz,
                                                  0,
                                                  nullptr)};
  SDL::RendererRef renderer{window.GetRenderer()};

  static constexpr std::string_view copybuttonstr = "Click here to copy!";
  static constexpr std::string_view pastebuttonstr = "Click here to paste!";
  SDL::FRect currenttimerect;
  SDL::FRect copybuttonrect;
  SDL::FRect pastetextrect;
  SDL::FRect pastebuttonrect;
  bool copy_pressed = false;
  bool paste_pressed = false;
  std::string current_time;
  std::string pasted_str;

  void CalculateCurrentTimeString()
  {
    try {
      SDL::Time ticks = SDL::GetCurrentTime();
      SDL::DateTime dt(ticks, true);

      static const char* month[12] = {"January",
                                      "February",
                                      "March",
                                      "April",
                                      "May",
                                      "June",
                                      "July",
                                      "August",
                                      "September",
                                      "October",
                                      "November",
                                      "December"};
      static const char* day[7] = {"Sunday",
                                   "Monday",
                                   "Tuesday",
                                   "Wednesday",
                                   "Thursday",
                                   "Friday",
                                   "Saturday"};
      current_time = std::format("{}, {} {}, {}   {:02}:{:02}:{:02}",
                                 day[dt.day_of_week],
                                 month[dt.month - 1],
                                 dt.day,
                                 dt.year,
                                 dt.hour,
                                 dt.minute,
                                 dt.second);
    } catch (const SDL::Error&) {
      current_time = "(Don't know the current time, sorry.)";
      return;
    }
  }

  Main()
  {
    CalculateCurrentTimeString();

    currenttimerect.x = 30;
    currenttimerect.y = 10;
    currenttimerect.w = 390;
    currenttimerect.h = SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE + 10;

    copybuttonrect.x = currenttimerect.x + currenttimerect.w + 30;
    copybuttonrect.y = currenttimerect.y;
    copybuttonrect.w =
      float((SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE * copybuttonstr.size()) + 10);
    copybuttonrect.h = currenttimerect.h;

    pastetextrect.x = 10;
    pastetextrect.y = currenttimerect.y + currenttimerect.h + 10;
    pastetextrect.w = 620;
    pastetextrect.h = ((480 - pastetextrect.y) - copybuttonrect.h) - 20;

    pastebuttonrect.w =
      float((SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE * pastebuttonstr.size()) + 10);
    pastebuttonrect.x = (640 - pastebuttonrect.w) / 2.0f;
    pastebuttonrect.y = pastetextrect.y + pastetextrect.h + 10;
    pastebuttonrect.h = copybuttonrect.h;
  }

  SDL::AppResult Event(const SDL::Event& event) final
  {
    if (event.type == SDL::EVENT_QUIT) {
      return SDL::APP_SUCCESS; // end the program, reporting success to the OS.
    } else if (event.type == SDL::EVENT_MOUSE_BUTTON_DOWN) {
      if (event.button.button == SDL::BUTTON_LEFT) {
        const SDL::FPoint p = {event.button.x, event.button.y};
        copy_pressed = SDL::PointInRectFloat(p, copybuttonrect);
        paste_pressed = SDL::PointInRectFloat(p, pastebuttonrect);
      }
    } else if (event.type == SDL::EVENT_MOUSE_BUTTON_UP) {
      if (event.button.button == SDL::BUTTON_LEFT) {
        if (const SDL::FPoint p{event.button.x, event.button.y};
            copy_pressed && SDL::PointInRectFloat(p, copybuttonrect)) {
          SDL::SetClipboardText(current_time);
        } else if (paste_pressed && SDL::PointInRectFloat(p, pastebuttonrect)) {
          pasted_str = SDL::GetClipboardText();
        }
        copy_pressed = paste_pressed = false;
      }
    }
    return SDL::APP_CONTINUE; // carry on with the program!
  }

  void RenderPastedText()
  {
    if (pasted_str.empty()) return;
    SDL::FPoint p{pastetextrect.x + 5, pastetextrect.y + 5};
    const float w = pastetextrect.w - 10;
    const float h = pastetextrect.h;
    const auto max_chars_per_line =
      size_t(w / SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE);
    char* str = pasted_str.data();
    char* newline;

    /* this doesn't wordwrap, or deal with Unicode....this is just a simple
     * example app! */
    while ((newline = SDL::strchr(str, '\n')) != nullptr) {
      const bool ignore_cr = ((newline > str) && (newline[-1] == '\r'));

      if (ignore_cr) newline[-1] = '\0';
      *newline = '\0';

      // length to end of line.
      size_t slen = SDL::min(SDL::strlen(str), max_chars_per_line);
      char ch = str[slen];
      str[slen] = '\0';
      renderer.RenderDebugText(p, str);
      str[slen] = ch;

      if (ignore_cr) newline[-1] = '\r';
      *newline = '\n';

      str = newline + 1;
      p.y += SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE + 2;
      if ((h - p.y) < SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE) {
        break; // no space for another line, stop here.
      }
    }
    // last text after newline, if there's room.
    if ((h - p.y) >= SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE) {
      size_t slen = SDL_strlen(str); // length to end of line.
      slen = SDL_min(slen, max_chars_per_line);
      char ch = str[slen];
      str[slen] = '\0';
      renderer.RenderDebugText(p, str);
      str[slen] = ch;
    }
  }

  SDL::AppResult Iterate() final
  {
    CalculateCurrentTimeString();

    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();

    // draw a frame around the current time.
    renderer.SetDrawColor(SDL::Color{0, 0, 255});
    renderer.RenderFillRect(currenttimerect);
    renderer.SetDrawColor(SDL::Color{255, 255, 255});
    renderer.RenderRect(currenttimerect);

    // draw the current time inside the frame.
    SDL::FPoint p{
      currenttimerect.x +
        ((currenttimerect.w -
          float(SDL::DEBUG_TEXT_FONT_CHARACTER_SIZE * current_time.size())) /
         2.0f),
      currenttimerect.y + 5};
    renderer.SetDrawColor(SDL::Color{255, 255, 0});
    renderer.RenderDebugText(p, current_time);

    // draw a frame for the "copy this text" button.
    if (copy_pressed) {
      renderer.SetDrawColor(SDL::Color{0, 255, 0});
    } else {
      renderer.SetDrawColor(SDL::Color{255, 0, 0});
    }
    renderer.RenderFillRect(copybuttonrect);
    renderer.SetDrawColor(SDL::Color{255, 255, 255});
    renderer.RenderRect(copybuttonrect);

    // draw the "copy this text" button string.
    renderer.SetDrawColor(SDL::Color{255, 255, 255});
    renderer.RenderDebugText({copybuttonrect.x + 5, copybuttonrect.y + 5},
                             copybuttonstr);

    // draw a frame for the pasted text area.
    renderer.SetDrawColor(SDL::Color{0, 53, 25});
    renderer.RenderFillRect(pastetextrect);
    renderer.SetDrawColor(SDL::Color{255, 255, 255});
    renderer.RenderRect(pastetextrect);

    // draw pasted text.
    renderer.SetDrawColor(SDL::Color{0, 219, 107});
    RenderPastedText();

    // draw a frame for the "paste from the clipboard" button.
    if (paste_pressed) {
      renderer.SetDrawColor(SDL::Color{0, 255, 0});
    } else {
      renderer.SetDrawColor(SDL::Color{255, 0, 0});
    }
    renderer.RenderFillRect(pastebuttonrect);
    renderer.SetDrawColor(SDL::Color{255, 255, 255});
    renderer.RenderRect(pastebuttonrect);

    // draw the "paste some text" button string.
    renderer.SetDrawColor(SDL::Color{255, 255, 255});
    renderer.RenderDebugText({pastebuttonrect.x + 5, pastebuttonrect.y + 5},
                             pastebuttonstr);

    // put the new rendering on the screen.
    renderer.Present();

    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

SDL3PP_DEFINE_CLASS_CALLBACKS(Main,
                              SDL::INIT_VIDEO,
                              "Example Misc Clipboard",
                              "1.0",
                              "com.example.misc-clipboard")
