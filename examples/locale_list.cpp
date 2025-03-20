#include <iostream>
#include <SDL3/SDL_main.h>
#include <SDL3pp/SDL3pp.h>

int main()
{
  SDL::SDL init{SDL::INIT_VIDEO};
  int count = 0;
  auto locales = SDL::GetPreferredLocales();
  if (!locales) {
    std::cout << "No locales: " << SDL::GetError() << "\n";
    return 1;
  }
  std::cout << count << " locales:\n";
  for (int i = 0; i < count; ++i) {
    auto locale = locales[i];
    if (locale->country) {
      std::cout << locale->language << '/' << locale->country << '\n';
    } else {
      std::cout << locale->language << '\n';
    }
  }
  return 0;
}
