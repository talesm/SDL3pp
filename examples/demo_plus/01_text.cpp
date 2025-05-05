#define SDL3PP_MAIN_USE_CALLBACKS 1
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

constexpr SDL::Point windowSz = {640, 480};
constexpr SDL::Point textMaxSz = windowSz / SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

class Text
{
  char m_text[textMaxSz.y][textMaxSz.x] = {};

public:
  constexpr Text()
  {
    for (auto& row : m_text) {
      for (auto& ch : row) ch = ' ';
    }
  }
  constexpr void insertChar(SDL::Point& p, char ch)
  {
    if (p.x < 0 || p.y < 0 || (ch & 0x80) || ch < ' ') return;
    if (p.x >= textMaxSz.x) {
      p.x = 0;
      p.y++;
    }
    if (p.y >= textMaxSz.y) return;
    m_text[p.y][p.x++] = ch;
  }

  constexpr bool moveUp(SDL::Point& p)
  {
    if (p.y <= 0) return false;
    p.y--;
    return true;
  }

  constexpr bool moveDown(SDL::Point& p)
  {
    if (p.y >= textMaxSz.y - 1) return false;
    p.y++;
    return true;
  }

  constexpr bool moveLeft(SDL::Point& p, bool word)
  {
    if (p.x > 0)
      p.x--;
    else if (moveUp(p))
      moveEnd(p, false);
    else
      return false;
    return true;
  }
  constexpr bool moveRight(SDL::Point& p, bool word)
  {
    if (p.x < textMaxSz.x - 1)
      p.x++;
    else if (moveDown(p))
      moveHome(p, false);
    else
      return false;
    return true;
  }

  constexpr void moveHome(SDL::Point& p, bool yAxis)
  {
    p.x = 0;
    if (yAxis) p.y = 0;
  }

  constexpr void moveEnd(SDL::Point& p, bool yAxis)
  {
    p.x = textMaxSz.x - 1;
    if (yAxis)
      p.y = textMaxSz.y - 1;
    else if (p.y < 0 || p.y >= textMaxSz.y)
      return;
    if (m_text[p.y][p.x] > ' ') return;
    while (p.x > 0) {
      if (m_text[p.y][p.x - 1] > ' ') break;
      p.x--;
    }
  }

  constexpr bool backspace(SDL::Point& p, bool word = false)
  {
    SDL::Point prev = p;
    if (!moveLeft(p, word)) return false;
    doErase(p, prev);
    return true;
  }

  constexpr void doErase(SDL::Point begin, SDL::Point end)
  {
    if (begin.y == end.y) {
      for (; end.x < textMaxSz.x; end.x++, begin.x++) {
        m_text[begin.y][begin.x] = m_text[begin.y][end.x];
      }
      for (; begin.x < textMaxSz.x; begin.x++) {
        m_text[begin.y][begin.x] = ' ';
      }
      // TODO if line becomes empty
    } else {
      // TODO
    }
  }

  constexpr void newLine(SDL::Point& p, bool blank, bool above)
  {
    moveDown(p);
    moveHome(p, false);
  }

  constexpr void insertText(SDL::Point& p, std::string_view v)
  {
    for (auto ch : v) insertChar(p, ch);
  }

  std::string getRow(int row) const
  {
    if (row < 0 || row > textMaxSz.y) return {};
    std::string_view sv{m_text[row], textMaxSz.x};
    int lastCol = textMaxSz.x - 1;
    for (int col = lastCol; col >= 0; col--) {
      if (sv[col] > ' ') return std::string{sv.substr(0, col + 1)};
    }
    return {};
  }
};

struct Main
{

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"examples/demo/textedit", windowSz};
  SDL::Renderer renderer{window};
  Text text;
  SDL::Point cursor{0, 0};

  Main() { window.StartTextInput(); }

  SDL::AppResult Iterate()
  {
    renderer.SetDrawColor(SDL::FColor{.75f, .75f, .75f, 1.f});
    renderer.RenderClear();

    renderer.SetDrawColor(SDL::Color{0, 0, 0});
    renderer.RenderRect(SDL::FRect(cursor * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE,
                                   {SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE,
                                    SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE}));

    for (int i = 0; i < textMaxSz.y; i++) {
      std::string row = text.getRow(i);
      renderer.RenderDebugText(
        {0, float(i * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE)}, row);
    }

    renderer.Present();
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult Event(const SDL::Event& event)
  {
    switch (event.type) {
    case SDL::EVENT_QUIT: return SDL::APP_SUCCESS;
    case SDL::EVENT_KEY_DOWN: return KeyEvent(event.key);
    case SDL::EVENT_MOUSE_BUTTON_DOWN:
      cursor = SDL::Point{int(event.button.x), int(event.button.y)} /
               SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
      break;
    case SDL::EVENT_TEXT_INPUT: text.insertText(cursor, event.text.text); break;

    default: break;
    }
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult KeyEvent(const SDL::KeyboardEvent& key)
  {
    bool ctrl = key.mod & SDL::KMOD_CTRL;
    bool shift = key.mod & SDL::KMOD_SHIFT;
    switch (key.key) {
    case SDL::KEYCODE_UP: text.moveUp(cursor); break;
    case SDL::KEYCODE_DOWN: text.moveDown(cursor); break;
    case SDL::KEYCODE_BACKSPACE: text.backspace(cursor, ctrl); break;
    case SDL::KEYCODE_LEFT: text.moveLeft(cursor, ctrl); break;
    case SDL::KEYCODE_RIGHT: text.moveRight(cursor, ctrl); break;
    case SDL::KEYCODE_HOME: text.moveHome(cursor, ctrl); break;
    case SDL::KEYCODE_END: text.moveEnd(cursor, ctrl); break;
    case SDL::KEYCODE_RETURN:
    case SDL::KEYCODE_RETURN2:
    case SDL::KEYCODE_KP_ENTER: text.newLine(cursor, ctrl, shift); break;

    default: break;
    }
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main,
                        "Example Demo text edit",
                        "1.0",
                        "com.example.demo.plus.text-edit")
