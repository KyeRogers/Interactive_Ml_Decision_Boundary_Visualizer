#include "simulator.hpp"

/** Starts the simulator and runs its main loop. */
int main() {
  Simulator sim(std::make_unique<NeuralNetworkModel>(8));
  sim.Initialize();

  sim.Run();
  return 0;
}