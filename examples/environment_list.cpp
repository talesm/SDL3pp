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
  auto variables = appEnv.GetVariables();
  if (!variables) {
    std::cout << "No variables\n";
    return 1;
  }
  for (auto variable : variables) std::cout << variable << '\n';
  return 0;
}
