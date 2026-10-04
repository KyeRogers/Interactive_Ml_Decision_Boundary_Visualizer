#include "simulator.hpp"

/** Starts the simulator and runs its main loop. */
int main() {  
  Simulator sim;
  sim.Initialize();

 sim.Run();
  return 0;
}