/*
 * Logic implementation of the Snake game. It is designed to efficiently
 * represent the state of the game in memory.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Based on SDL's demo snake.c
 */

#define SDL3PP_MAIN_USE_CALLBACKS 1
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

constexpr SDL::Nanoseconds stepRate = 125ms;
constexpr int blockInPixels = 24;
constexpr SDL::Point gameSz = {24, 18};
constexpr SDL::Point windowSz = gameSz * blockInPixels;

constexpr SDL::FPoint blockSz{blockInPixels, blockInPixels};
constexpr SDL::Color foodColor{80, 80, 255};
constexpr SDL::Color bodyColor{0, 128, 0};

enum class SnakeCell : Uint8
{
  NOTHING = 0,
  SNAKE_RIGHT = 1,
  SNAKE_UP = 2,
  SNAKE_LEFT = 3,
  SNAKE_DOWN = 4,
  FOOD = 5,
};

enum class SnakeDirection : Uint8
{
  RIGHT,
  UP,
  LEFT,
  DOWN,
};

struct SnakeContext
{
  SnakeCell cells[gameSz.y][gameSz.x] = {};
  SDL::Point head_pos = gameSz / 2;
  SDL::Point tail_pos = head_pos;
  SnakeDirection next_dir = SnakeDirection::RIGHT;
  int inhibit_tail_step = 4;
  unsigned occupied_cells = 3;

  constexpr const SnakeCell& at(SDL::Point p) const { return cells[p.y][p.x]; }
  constexpr SnakeCell& at(SDL::Point p) { return cells[p.y][p.x]; }

  constexpr bool are_cell_full() const
  {
    return occupied_cells == gameSz.x * gameSz.y;
  }

  constexpr void new_food_pos()
  {
    while (true) {
      int x = SDL::rand(gameSz.x);
      int y = SDL::rand(gameSz.y);
      SnakeCell& cell = at({x, y});
      if (cell == SnakeCell::NOTHING) {
        cell = SnakeCell::FOOD;
        return;
      }
    }
  }

  constexpr SnakeContext()
  {
    at(tail_pos) = SnakeCell::SNAKE_RIGHT;
    for (int i = 0; i < 4; i++) {
      new_food_pos();
      ++occupied_cells;
    }
  }

  constexpr void redir(SnakeDirection dir)
  {
    SnakeCell ct = at(head_pos);
    if ((dir == SnakeDirection::RIGHT && ct != SnakeCell::SNAKE_LEFT) ||
        (dir == SnakeDirection::UP && ct != SnakeCell::SNAKE_DOWN) ||
        (dir == SnakeDirection::LEFT && ct != SnakeCell::SNAKE_RIGHT) ||
        (dir == SnakeDirection::DOWN && ct != SnakeCell::SNAKE_UP)) {
      next_dir = dir;
    }
  }

  constexpr static void wrap_around(SDL::Point& val)
  {
    if (val.x < 0) {
      val.x = gameSz.x - 1;
    } else if (val.x >= gameSz.x) {
      val.x = 0;
    }
    if (val.y < 0) {
      val.y = gameSz.y - 1;
    } else if (val.y >= gameSz.y) {
      val.y = 0;
    }
  }

  constexpr void step()
  {
    const SnakeCell dir_as_cell = SnakeCell(Uint8(next_dir) + 1);
    /* Move tail forward */
    if (--inhibit_tail_step == 0) {
      ++inhibit_tail_step;
      SnakeCell ct = at(tail_pos);
      at(tail_pos) = SnakeCell::NOTHING;
      switch (ct) {
      case SnakeCell::SNAKE_RIGHT: tail_pos.x++; break;
      case SnakeCell::SNAKE_UP: tail_pos.y--; break;
      case SnakeCell::SNAKE_LEFT: tail_pos.x--; break;
      case SnakeCell::SNAKE_DOWN: tail_pos.y++; break;
      default: break;
      }
      wrap_around(tail_pos);
    }
    /* Move head forward */
    at(head_pos) = dir_as_cell; // So the tail can follow
    switch (next_dir) {
    case SnakeDirection::RIGHT: ++head_pos.x; break;
    case SnakeDirection::UP: --head_pos.y; break;
    case SnakeDirection::LEFT: --head_pos.x; break;
    case SnakeDirection::DOWN: ++head_pos.y; break;
    }
    wrap_around(head_pos);

    /* Collisions */
    SnakeCell ct = at(head_pos);
    if (ct == SnakeCell::FOOD) {
      if (are_cell_full()) {
        *this = {};
        return;
      }
      new_food_pos();
      ++inhibit_tail_step;
      ++occupied_cells;
    } else if (ct != SnakeCell::NOTHING) {
      *this = {};
      return;
    }

    // So we can check before turning
    at(head_pos) = dir_as_cell;
  }
};

struct Main
{
  static SDL::AppResult Init(Main** m, SDL::AppArgs args)
  {
    SDL::SetAppMetadata("Example Snake game", "1.0", "com.example.Snake");
    SDL::Init(SDL::INIT_VIDEO);
    *m = new Main();
    return SDL::APP_CONTINUE;
  }

  SDL::Window window{"examples/demo/snake", windowSz};
  SDL::Renderer renderer{window};
  SnakeContext snake_ctx;
  SDL::Nanoseconds lastStep = SDL::GetTicks();

  SDL::AppResult Iterate()
  {
    SDL::Nanoseconds now = SDL::GetTicks();

    // run game logic if we're at or past the time to run it.
    // if we're _really_ behind the time to run it, run it
    // several times.
    while ((now - lastStep) > stepRate) {
      snake_ctx.step();
      lastStep += stepRate;
    }

    renderer.SetDrawColor(SDL::Color{0, 0, 0});
    renderer.RenderClear();

    SDL::Point p;
    for (p.x = 0; p.x < gameSz.x; p.x++) {
      for (p.y = 0; p.y < gameSz.y; p.y++) {
        SnakeCell ct = snake_ctx.at(p);
        if (ct == SnakeCell::NOTHING) continue;
        renderer.SetDrawColor(ct == SnakeCell::FOOD ? foodColor : bodyColor);
        renderer.RenderFillRect(SDL::FRect{p * blockInPixels, blockSz});
      }
    }
    renderer.SetDrawColor(SDL::Color{255, 255, 0});
    renderer.RenderFillRect(
      SDL::FRect{snake_ctx.head_pos * blockInPixels, blockSz});
    renderer.Present();
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult handle_key_event(SDL::Scancode key_code)
  {
    switch (key_code) {
      /* Quit. */
    case SDL::SCANCODE_ESCAPE:
    case SDL::SCANCODE_Q: return SDL::APP_SUCCESS;
    /* Restart the game as if the program was launched. */
    case SDL::SCANCODE_R: snake_ctx = SnakeContext{}; break;
    /* Decide new direction of the snake. */
    case SDL::SCANCODE_RIGHT: snake_ctx.redir(SnakeDirection::RIGHT); break;
    case SDL::SCANCODE_UP: snake_ctx.redir(SnakeDirection::UP); break;
    case SDL::SCANCODE_LEFT: snake_ctx.redir(SnakeDirection::LEFT); break;
    case SDL::SCANCODE_DOWN: snake_ctx.redir(SnakeDirection::DOWN); break;
    default: break;
    }
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult Event(const SDL::Event& event)
  {
    switch (event.type) {
    case SDL::EVENT_QUIT: return SDL::APP_SUCCESS;
    case SDL::EVENT_KEY_DOWN: return handle_key_event(event.key.scancode);
    default: break;
    }
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
