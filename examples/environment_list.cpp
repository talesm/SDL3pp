#include <iostream>
#include "SDL3pp/SDL3pp_stdinc.h"

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
  for (char** variable = &variables[0]; *variable != nullptr; variable++) {
    std::cout << *variable << '\n';
  }
  return 0;
}
