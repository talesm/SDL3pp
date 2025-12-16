#include <iostream>
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

int main(int argc, char** argv)
{
  SDL::EnvironmentRaw appEnv = SDL::GetEnvironment();
  if (!appEnv) {
    std::cout << "No environment\n";
    return 1;
  }
  for (auto variable : SDL::GetEnvironmentVariables(appEnv))
    std::cout << variable << '\n';
  return 0;
}
