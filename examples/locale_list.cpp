#include <iostream>
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

int main()
{
  SDL::SDL init{SDL::INIT_VIDEO};
  int count = 0;
  auto locales = SDL::GetPreferredLocales();
  if (!locales) {
    std::cout << "No locales: " << SDL::GetError() << "\n";
    return 1;
  }
  for (auto locale : locales) {
    if (locale->country) {
      std::cout << locale->language << '/' << locale->country << '\n';
    } else {
      std::cout << locale->language << '\n';
    }
  }
  return 0;
}
