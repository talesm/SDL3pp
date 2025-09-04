/*
 * A quick and dirty hexadecimal editor.
 *
 * A left panel with the hexadecimal representation and the ascii on the right.
 *
 */
#define SDL3PP_MAIN_USE_CALLBACKS 1
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

namespace colors {

constexpr SDL::Color background{58, 58, 58};
constexpr SDL::Color text{255, 255, 180};
constexpr SDL::Color hex_text{255, 255, 255, 197};
constexpr SDL::Color placeholder_text{128, 128, 128};

constexpr SDL::Color selected_background{0, 0, 255};
constexpr SDL::Color selected_text{255, 255, 180};
constexpr SDL::Color selected_placeholder_text{128, 128, 128};

constexpr SDL::Color title_background{29, 29, 29};
constexpr SDL::Color title_text{255, 255, 255};
constexpr SDL::Color cursor{255, 255, 255};
constexpr SDL::Color mirror_cursor{128, 128, 128};
}

constexpr SDL::Point sourceCharSz{SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE,
                                  SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE};
constexpr char placeholder{'.'};
constexpr char hexDigits[] = "0123456789ABCDEF";

enum class ClickMode
{
  NONE,
  SCROLL_BAR,
};

constexpr SDL::Nanoseconds cursorHalfPeriod = 500ms;

struct Main
{
  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Point windowSz = {1024, 800};
  SDL::Window window = SDL::Window::Create("examples/demo/template", windowSz);
  SDL::Renderer renderer = SDL::Renderer::Create(window);
  SDL::FPoint charSz{16, 16};
  SDL::FPoint spacingSz{charSz / 2};
  SDL::Texture charTable{makeCharTable()};
  Uint64 address = 0;
  std::string content{"Hello World!"};
  std::string lastFilename;
  Uint64 addressRowsCount = 0;
  int rowsInScreen = 1;
  int addressDigits = 4;
  float addressAreaX = 0;
  float addressAreaW = 0;
  float hexAreaX = 0;
  float hexAreaW = 0;
  float asciiAreaX = 0;
  float asciiAreaW = 0;
  float scrollBarX = 0;
  float scrollBarW = 0;
  float headerH = 0;
  SDL::FRect barRect;
  SDL::Point cursor;
  std::optional<SDL::FPoint> clickPos;
  ClickMode clickMode{};
  SDL::Nanoseconds delta;
  SDL::Nanoseconds cursorCounter;
  bool longAddressBar = false;
  bool cursorOnRight = false;
  bool cursorShown = true;

  Main(SDL::AppArgs args)
  {
    if (args.size() > 1) loadFile(args.back());
  }

  SDL::AppResult Iterate()
  {
    setupFrame();

    renderer->SetDrawColor(colors::background);
    renderer->RenderClear();

    renderLabels();
    renderAscii();
    renderHex();

    renderer->Present();
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult Event(const SDL::Event& event)
  {
    switch (event.type) {
    case SDL::EVENT_QUIT: return SDL::APP_SUCCESS;
    case SDL::EVENT_KEY_DOWN: return KeyDown(event.key);
    case SDL::EVENT_DROP_TEXT:
      content = event.drop.data;
      address = 0;
      cursor = {0, 0};
      break;
    case SDL::EVENT_DROP_FILE: loadFile(event.drop.data); break;
    case SDL::EVENT_MOUSE_BUTTON_DOWN:
      if (event.button.button == SDL::BUTTON_LEFT) {
        SDL::FPoint p{event.button.x, event.button.y};
        if (p.x >= scrollBarX && p.x < scrollBarX + scrollBarW) {
          if (p.IsInRect(barRect)) {
            clickPos = p;
            clickMode = ClickMode::SCROLL_BAR;
          } else if (p.y > barRect.y) {
            address += rowsInScreen << 4;
          } else if (addressRow() < rowsInScreen) {
            address = 0;
          } else {
            address -= rowsInScreen << 4;
          }
        }
      }
      break;
    case SDL::EVENT_MOUSE_BUTTON_UP:
      if (clickPos) {
        MouseUp(event.button);
        clickPos.reset();
        clickMode = ClickMode::NONE;
      }
      break;
    case SDL::EVENT_MOUSE_MOTION:
      if (clickPos) MouseMotion(event.motion);
      break;

    default: break;
    }
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult KeyDown(const SDL::KeyboardEvent& event)
  {
    bool ctrl = event.mod & SDL::KMOD_CTRL;
    switch (event.key) {
    case SDL::KEYCODE_UP:
      if (ctrl) {
        cursor.y = 0;
      } else {
        cursor.y--;
      }
      break;
    case SDL::KEYCODE_DOWN:
      if (ctrl) {
        cursor.y = rowsInScreen - 1;
      } else {
        cursor.y++;
      }
      break;
    case SDL::KEYCODE_LEFT:
      if (ctrl) {
        cursorOnRight = false;
        cursor.x = 0;
      } else if (cursorOnRight == true) {
        cursorOnRight = false;
      } else {
        cursor.x--;
        cursorOnRight = true;
      }
      break;
    case SDL::KEYCODE_RIGHT:
      if (ctrl) {
        cursor.x = 15;
        cursorOnRight = true;
      } else if (cursorOnRight == false) {
        cursorOnRight = true;
      } else {
        cursorOnRight = false;
        cursor.x++;
      }
      break;
    case SDL::KEYCODE_PAGEUP:
      if (ctrl) break;
      if (address > rowsInScreen << 4) {
        address -= rowsInScreen << 4;
      } else {
        address = 0;
      }
      break;
    case SDL::KEYCODE_PAGEDOWN:
      if (ctrl) break;
      address += rowsInScreen << 4;
      break;

    case SDL::KEYCODE_HOME:
      cursorOnRight = false;
      cursor = {0, 0};
      if (ctrl) address = 0;
      break;
    case SDL::KEYCODE_END:
      cursorOnRight = true;
      cursor = {15, rowsInScreen - 1};
      if (ctrl) address = (addressRowsCount - rowsInScreen) << 4;
      break;
    case SDL::KEYCODE_S:
      if (ctrl) {
        SDL::ShowSaveFileDialog(
          [this](auto files, int index) {
            if (!files || !*files) return;
            saveFile(*files);
          },
          window,
          {},
          lastFilename);
      }
      break;
    case SDL::KEYCODE_O:
      if (ctrl) {
        SDL::ShowOpenFileDialog(
          [this](auto files, int index) {
            if (!files || !*files) return;
            loadFile(*files);
          },
          window,
          {},
          lastFilename);
      }
    default:
      if (event.key >= SDL::KEYCODE_0 && event.key <= SDL::KEYCODE_9) {
        enterNimble(event.key - int(SDL::KEYCODE_0));
      } else if (event.key >= SDL::KEYCODE_A && event.key <= SDL::KEYCODE_F) {
        enterNimble(event.key - int(SDL::KEYCODE_A) + 10);

      } else if (event.key == SDL::KEYCODE_KP_0) {
        enterNimble(0);
      } else if (event.key >= SDL::KEYCODE_KP_1 &&
                 event.key <= SDL::KEYCODE_KP_9) {
        enterNimble(event.key - int(SDL::KEYCODE_KP_1) + 1);
      }
      break;
    }
    return SDL::APP_CONTINUE;
  }

  void enterNimble(int nimble)
  {
    size_t index = address + cursor.y * 16 + cursor.x;
    if (index < content.size()) {
      char& ch = content[index];
      if (cursorOnRight) {
        ch = (ch & 0xF0) | nimble;
        cursorOnRight = false;
        cursor.x++;
      } else {
        ch = (ch & 0x0F) | (nimble << 4);
        cursorOnRight = true;
      }
    } else {
      content.push_back(nimble << 4);
    }
  }

  SDL::AppResult MouseUp(const SDL::MouseButtonEvent& event)
  {
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult MouseMotion(const SDL::MouseMotionEvent& event)
  {
    switch (clickMode) {
    case ClickMode::SCROLL_BAR: {
      float offset = (event.y - clickPos->y) * addressRowsCount / windowSz.y;
      if (offset > -1.f && offset < 1.f) break;
      clickPos = SDL::FPoint(event.x, event.y);
      if (offset < 0) {
        Uint64 offsetAddress = SDL::ceil(-offset * 16);
        if (offsetAddress > address)
          address = 0;
        else
          address -= offsetAddress;

      } else if (offset > 0) {
        Uint64 offsetAddress = SDL::ceil(offset * 16);
        if (content.size() - address <= offsetAddress)
          address = (addressRowsCount - 1) << 4;
        else
          address += offsetAddress;
      }
      break;
    }

    default: break;
    }
    return SDL::APP_CONTINUE;
  }

  void setupFrame()
  {
    // Timer
    static SDL::Nanoseconds lastTime = SDL::GetTicks();
    SDL::Nanoseconds currTime = SDL::GetTicks();
    delta = currTime - lastTime;
    lastTime = currTime;

    // Figure out proportions
    addressDigits = 0;
    for (Uint64 a = content.size(); a != 0; a >>= 4, addressDigits++);
    if (addressDigits <= 4) {
      addressDigits = 3;
      longAddressBar = false;
    } else {
      longAddressBar = addressDigits > 8;
      addressDigits--;
    }
    float charCount = (longAddressBar ? 16 : 8) + (16 * 5 / 2) + 16;
    charSz.y = charSz.x = windowSz.x / charCount;
    spacingSz = charSz / 2;

    // Update layout
    addressAreaX = 0;
    addressAreaW = charSz.x * (longAddressBar ? 15 : 7) + spacingSz.x;
    hexAreaX = addressAreaX + addressAreaW;
    hexAreaW = 32 * charSz.x + 16 * spacingSz.x;
    asciiAreaX = hexAreaX + hexAreaW;
    asciiAreaW = charSz.x * 16;
    scrollBarX = asciiAreaX + asciiAreaW;
    scrollBarW = spacingSz.x;
    headerH = charSz.y + spacingSz.y;

    // Update cursor and scroll offset
    if (cursor.x < 0) {
      if (cursor.y != 0 || address != 0) {
        cursor.x = 15;
        cursor.y -= 1;
        cursorOnRight = true;
      } else {
        cursorOnRight = false;
        cursor.x = 0;
      }
    } else if (cursor.x >= 16) {
      cursor.x = 0;
      cursor.y += 1;
      cursorOnRight = false;
    }
    rowsInScreen = (windowSz.y - headerH) / charSz.y;
    if (cursor.y < 0) {
      cursor.y = 0;
      if (address >= 16) address -= 16;
    } else if (cursor.y >= rowsInScreen) {
      cursor.y = rowsInScreen - 1;
      address += 16;
    }
    addressRowsCount = (content.size() >> 4) + 1;
    if (address < 0) {
      address = 0;
    } else if (address > content.size()) {
      address = (addressRowsCount - 1) << 4;
    }
    if (addressRow() + rowsInScreen > addressRowsCount) {
      rowsInScreen = addressRowsCount - addressRow();
      if (cursor.y >= rowsInScreen) cursor.y = rowsInScreen - 1;
    }
    if (globalCursor().y == addressRowsCount - 1) {
      Uint64 offset = content.size() % 16;
      if (offset > 0 && cursor.x >= offset) {
        cursor.x = offset;
        cursorOnRight = false;
      }
    }

    // Determine scrollbar size
    barRect.x = scrollBarX;
    barRect.h = SDL::clamp(
      float(windowSz.y * 2) / float(addressRowsCount), 8.f, windowSz.y);
    barRect.w = scrollBarW - 1;
    if (address > 0) {
      barRect.y = (windowSz.y - barRect.h) * float(addressRow()) /
                  float(addressRowsCount - 1);
    } else {
      barRect.y = 0;
    }

    // Make cursor blink
    cursorCounter += delta;
    if (cursorCounter > cursorHalfPeriod) {
      cursorShown = !cursorShown;
      cursorCounter -= cursorHalfPeriod;
    }
  }

  void saveFile(std::string filename)
  {
    try {
      SDL::SaveFile(filename, content);
      lastFilename = std::move(filename);
    } catch (std::exception& e) {
      SDL::Log("Could not save to file: {}", e.what());
    }
  }

  void loadFile(std::string filename)
  {
    auto data = SDL::LoadFile(filename);
    if (data) {
      address = 0;
      cursor = {0, 0};
      content.clear();
      content.reserve(data.size());
      std::ranges::copy(data, std::back_inserter(content));
      lastFilename = std::move(filename);
    }
  }

  void renderLabels()
  {
    // Title backgrounds
    renderer->SetDrawColor(colors::title_background);
    renderer->RenderFillRect(SDL::FRect{0.f, 0.f, scrollBarX, headerH});
    renderer->RenderFillRect(
      SDL::FRect{0.f, headerH, addressAreaW, windowSz.y - headerH});

    // Scrollbar
    renderer->SetDrawColor(colors::placeholder_text);
    renderer->RenderFillRect(
      SDL::FRect{scrollBarX, 0, scrollBarW, float(windowSz.y)});
    renderer->SetDrawColor(colors::title_background);
    renderer->RenderFillRect(barRect);

    // Filters
    setTextColor(colors::placeholder_text);
    putString({addressAreaX, spacingSz.y / 2}, "Go to");
    putString({asciiAreaX, spacingSz.y / 2}, "Search");

    // Address cols
    setTextColor(colors::title_text);
    SDL::FPoint headerPos{hexAreaX + spacingSz.x, spacingSz.y / 2};
    for (int i = 0; i < 16; i++, headerPos.x += charSz.x * 2 + spacingSz.x) {
      putChar(headerPos, hexDigits[i]);
    }

    // Address cols
    SDL::FPoint addrPos{addressAreaX, headerH};
    for (Uint64 i = address; i < content.size() && addrPos.y < windowSz.y;
         i += 16, addrPos.y += charSz.y) {
      putString(addrPos, formatAddress(i >> 4));
    }

    // Mirror cursor
    renderer->SetDrawColor(colors::mirror_cursor);
    renderer->RenderFillRect(SDL::FRect{hexAreaX,
                                        headerH + cursor.y * charSz.y,
                                        hexAreaW - spacingSz.x / 2,
                                        charSz.y});
    renderer->RenderFillRect(
      SDL::FRect{hexAreaX + cursor.x * (charSz.x * 2 + spacingSz.x),
                 headerH,
                 charSz.x * 2 + spacingSz.x / 2,
                 float(windowSz.y)});
    renderer->RenderFillRect(SDL::FRect{asciiAreaX + cursor.x * charSz.x,
                                        headerH + cursor.y * charSz.y,
                                        charSz.x,
                                        charSz.y});

    // Cursor
    if (cursorShown) {
      renderer->SetDrawColor(colors::cursor);
      SDL::FRect cursorRect{
        hexAreaX + cursor.x * (charSz.x * 2 + spacingSz.x),
        headerH + cursor.y * charSz.y + charSz.y - 4,
        charSz.x,
        4,
      };
      if (cursorOnRight) cursorRect.x += charSz.x;
      renderer->RenderFillRect(cursorRect);
    }
  }

  void renderHex()
  {
    setTextColor(colors::hex_text);
    SDL::FPoint p{0, headerH};
    for (Uint64 row = address; row < content.size();
         row += 16, p.y += charSz.y) {
      p.x = hexAreaX;
      Uint64 count = SDL::min(row + 16, content.size());
      for (Uint64 cell = row; cell < count; cell++) {
        putString(p, formatByte(content[cell]));
        p.x += charSz.x * 2 + spacingSz.x;
      }
      if (p.y > windowSz.y) break;
    }
  }

  void renderAscii()
  {
    SDL::FPoint p{0, headerH};
    for (Uint64 row = address; row < content.size();
         row += 16, p.y += charSz.y) {
      p.x = asciiAreaX;
      Uint64 count = SDL::min(row + 16, content.size());
      for (Uint64 cell = row; cell < count; cell++) {
        char ch = content[cell];
        if (ch < ' ' || ch >= 127) {
          setTextColor(colors::placeholder_text);
          putChar(p, placeholder);
        } else {
          setTextColor(colors::text);
          putChar(p, ch);
        }
        p.x += charSz.x;
      }
      if (p.y > windowSz.y) break;
    }
  }

  constexpr Uint64 addressRow() const { return address >> 4; }

  constexpr bool canMoveDown() const
  {
    return globalCursor().y < addressRowsCount - 1;
  }

  constexpr SDL::Point globalCursor() const
  {
    auto p = cursor;
    p.y += addressRow();
    return p;
  }

  std::string formatByte(Uint8 byte)
  {
    return {hexDigits[byte / 16], hexDigits[byte % 16]};
  }

  std::string formatAddress(Uint64 address) const
  {
    char buffer[16];
    int width = longAddressBar ? 15 : 7;
    buffer[width] = 0;
    for (int i = 0; i < width; i++) {
      if (i < addressDigits) {
        int r = address % 16;
        buffer[width - i - 1] = hexDigits[r];
        address /= 16;
      } else {
        buffer[width - i - 1] = ' ';
      }
    }

    return buffer;
  }

  void setTextColor(SDL::Color c) { charTable->SetMod(c); }

  void putString(SDL::FPoint p, std::string_view v)
  {
    for (auto ch : v) {
      putChar(p, ch);
      p.x += charSz.x;
    }
  }

  void putChar(SDL::FPoint p, char ch) { putCharAtRect({p, charSz}, ch); }

  void putCharAtRect(SDL::FRect r, char ch)
  {
    SDL_assert_paranoid(ch >= 32 && ch < 127);
    SDL::Point srcPos = {SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * (ch - 32), 0};
    renderer->RenderTexture(charTable, SDL::FRect{srcPos, sourceCharSz}, r);
  }

  SDL::Texture makeCharTable()
  {
    char asciiChars[127 - 32 + 1];
    for (int i = 32; i < 127; i++) asciiChars[i - 32] = i;
    asciiChars[127 - 32] = 0;

    auto surface = SDL::Surface::Create(
      SDL::Point{SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * (127 - 32),
                 SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE},
      SDL::PIXELFORMAT_RGBA32);
    SDL::Renderer buffer{SDL::Renderer::CreateSoftware(surface)};
    buffer->SetDrawColor(SDL::Color{}); // Transparent
    buffer->RenderClear();
    buffer->SetDrawColor(SDL::Color{255, 255, 255}); // White
    buffer->RenderDebugText({0, 0}, asciiChars);
    buffer->Present();

    SDL::Texture texture = SDL::Texture::CreateFromSurface(renderer, surface);
    texture->SetScaleMode(SDL::SCALEMODE_NEAREST);
    return texture;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main, "Example Template", "1.0", "com.example.template")
