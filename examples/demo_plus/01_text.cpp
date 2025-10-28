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
  constexpr void replaceChar(SDL::Point& p, char ch)
  {
    if (ch == '\n') {
      newLine(p);
      return;
    }
    if (p.x < 0 || p.y < 0 || (ch & 0x80) || ch < ' ') return;
    if (p.x >= textMaxSz.x) {
      p.x = 0;
      p.y++;
    }
    if (p.y >= textMaxSz.y) return;
    m_text[p.y][p.x++] = ch;
  }

  constexpr void replaceText(SDL::Point& p, std::string_view v)
  {
    for (auto ch : v) replaceChar(p, ch);
  }

  constexpr void insertText(SDL::Point& p, std::string_view v)
  {
    if (p.y < 0 || p.y >= textMaxSz.y) return;
    std::string remainder = getRowAfter(p);
    replaceText(p, v);
    if (!remainder.empty()) {
      SDL::Point rPoint = p;
      replaceText(rPoint, remainder);
    }
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
    if (p.x <= 0) {
      if (!moveUp(p)) return false;
      moveEnd(p, false);
    } else if (word) {
      if (p.y < 0 || p.y >= textMaxSz.y) return false;
      while (--p.x > 0) {
        if (m_text[p.y][p.x] > ' ' && m_text[p.y][p.x - 1] <= ' ') break;
      }
    } else {
      p.x--;
    }
    return true;
  }

  constexpr bool moveRight(SDL::Point& p, bool word)
  {
    if (p.y < 0 || p.y >= textMaxSz.y) return false;
    int endCol = lastCol(p.y);
    if (endCol < textMaxSz.x && m_text[p.y][endCol] > ' ') endCol++;
    if (p.x >= endCol) {
      if (!moveDown(p)) return false;
      moveHome(p, false);
    } else if (word) {
      while (++p.x < endCol) {
        if (m_text[p.y][p.x - 1] > ' ' && m_text[p.y][p.x] <= ' ') break;
      }
    } else {
      p.x++;
    }
    return true;
  }

  constexpr void moveHome(SDL::Point& p, bool yAxis)
  {
    p.x = 0;
    if (yAxis) p.y = 0;
  }

  constexpr int lastCol(int row) const
  {
    if (row < 0 || row >= textMaxSz.y) return 0;
    for (int col = textMaxSz.x - 1; col > 0; col--) {
      if (m_text[row][col] > ' ') return col;
    }
    return 0;
  }

  constexpr void moveEnd(SDL::Point& p, bool yAxis)
  {
    if (yAxis)
      p.y = textMaxSz.y - 1;
    else if (p.y < 0 || p.y >= textMaxSz.y)
      return;
    p.x = lastCol(p.y);
    if (p.x < (textMaxSz.x - 1) && m_text[p.y][p.x] > ' ') p.x++;
  }

  constexpr bool backspace(SDL::Point& p, bool word = false)
  {
    SDL::Point end = p;
    if (!moveLeft(p, word)) return false;
    doErase(p, end);
    return true;
  }

  constexpr bool deleteNext(SDL::Point p, bool word = false)
  {
    SDL::Point beg = p;
    if (!moveRight(p, word)) return false;
    doErase(beg, p);
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
    } else {
      std::string remainder = getRowAfter(end);
      removeRows(begin.y + 1, end.y - begin.y);
      if (!remainder.empty()) replaceText(begin, remainder);
    }
  }

  constexpr void removeRows(int begin, int size)
  {
    if (size <= 0 || begin < 0 || begin >= textMaxSz.y) return;
    int limit = textMaxSz.y - size;
    for (int i = begin; i < limit; i++) {
      SDL::memcpy(m_text[i], m_text[i + size], textMaxSz.x);
    }
    for (int i = limit; i < textMaxSz.y; i++) {
      SDL::memset(m_text[i], ' ', textMaxSz.x);
    }
  }

  constexpr void insertRow(int begin, int size = 1)
  {
    if (size <= 0 || begin < 0 || begin >= textMaxSz.y) return;
    int limit = textMaxSz.y - size;
    for (int i = limit - 1; i >= begin; i--) {
      SDL::memcpy(m_text[i + size], m_text[i], textMaxSz.x);
    }
    for (int i = begin; i < std::min(begin + size, textMaxSz.y); i++) {
      SDL::memset(m_text[i], ' ', textMaxSz.x);
    }
  }

  constexpr void newLine(SDL::Point& p)
  {
    std::string remainder = getRowAfter(p);
    if (!remainder.empty() && p.x >= 0) {
      SDL::memset(m_text[p.y] + p.x, ' ', textMaxSz.x - p.x);
    }
    if (moveDown(p)) {
      moveHome(p, false);
      insertRow(p.y, 1);
      if (!remainder.empty()) {
        SDL::Point rPoint = p;
        replaceText(rPoint, remainder);
      }
    }
  }

  constexpr std::string getRowAfter(SDL::Point p) const
  {
    if (p.y < 0 || p.y >= textMaxSz.y) return {};
    std::string_view sv{m_text[p.y], textMaxSz.x};
    int limit = lastCol(p.y);
    int col = SDL::clamp(p.x, 0, textMaxSz.x - 1);
    if (limit < textMaxSz.x - 1 && m_text[p.y][limit] > ' ') limit++;
    if (limit <= col) return {};
    return std::string{sv.substr(col, limit - col)};
  }

  constexpr std::string getRow(int row) const { return getRowAfter({0, row}); }
};

struct Main
{
  SDL::AppResult Init(Main** m, SDL::AppArgs args)
  {
    SDL::SetAppMetadata(
      "Example Demo text edit", "1.0", "com.example.demo.plus.text-edit");
    SDL::Init(SDL::INIT_VIDEO);
    *m = new Main();
    return SDL::APP_CONTINUE;
  }
  SDL::Window window{"examples/demo/textedit", windowSz};
  SDL::Renderer renderer{window};
  Text text;
  SDL::Point cursor{0, 0};
  bool replaceMode = false;

  Main() { window.StartTextInput(); }

  SDL::AppResult Iterate()
  {
    renderer.SetDrawColorFloat(SDL::FColor{.75f, .75f, .75f, 1.f});
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
    case SDL::EVENT_TEXT_INPUT:
      if (replaceMode)
        text.replaceText(cursor, event.text.text);
      else
        text.insertText(cursor, event.text.text);
      break;

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
    case SDL::KEYCODE_LEFT: text.moveLeft(cursor, ctrl); break;
    case SDL::KEYCODE_RIGHT: text.moveRight(cursor, ctrl); break;
    case SDL::KEYCODE_HOME: text.moveHome(cursor, ctrl); break;
    case SDL::KEYCODE_END: text.moveEnd(cursor, ctrl); break;
    case SDL::KEYCODE_RETURN:
    case SDL::KEYCODE_RETURN2:
    case SDL::KEYCODE_KP_ENTER:
      if (ctrl) {
        text.moveHome(cursor, false);
        if (!shift && !text.moveDown(cursor)) break;
        text.insertRow(cursor.y);
      } else {
        text.newLine(cursor);
      }
      break;
    case SDL::KEYCODE_BACKSPACE: text.backspace(cursor, ctrl); break;
    case SDL::KEYCODE_DELETE: text.deleteNext(cursor, ctrl); break;
    case SDL::KEYCODE_INSERT: replaceMode = !replaceMode; break;
    case SDL::KEYCODE_V:
      if (ctrl && SDL::HasClipboardText()) {
        text.insertText(cursor, SDL::GetClipboardText());
      }
      break;
    case SDL::KEYCODE_C:
      if (ctrl) {
        text.moveHome(cursor, false);
        SDL::SetClipboardText(text.getRow(cursor.y) + '\n');
      }
      break;
    case SDL::KEYCODE_X:
      if (ctrl) {
        text.moveHome(cursor, false);
        SDL::SetClipboardText(text.getRow(cursor.y) + '\n');
        text.removeRows(cursor.y, 1);
      }
      break;

    default: break;
    }
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
