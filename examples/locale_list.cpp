#include <iostream>
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

int main()
{
  SDL::SDL init{SDL::INIT_VIDEO};
  for (auto locale : SDL::GetPreferredLocales()) {
    if (locale->country) {
      std::cout << locale->language << '/' << locale->country << '\n';
    } else {
      std::cout << locale->language << '\n';
    }
  }
  return 0;
}
