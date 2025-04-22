#include <iostream>
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

int main()
{
  SDL::EnvironmentRef appEnv = SDL::GetEnvironment();
  if (!appEnv) {
    std::cout << "No environment\n";
    return 1;
  }
  for (auto variable : appEnv.GetVariables()) std::cout << variable << '\n';
  return 0;
}
