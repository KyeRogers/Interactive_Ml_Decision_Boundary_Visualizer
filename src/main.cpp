#include "raylib.h"
#include "simulator.hpp"

int main() {  
  Simulator sim;
  sim.Initialize();

 sim.Run();
  return 0;
}