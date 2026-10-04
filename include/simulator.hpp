/**
 * Create a simulator class to run the programme/simulation
 */

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <vector>

#include "data_point.hpp"

class Simulator {
 public:
  Simulator() = default;

  const std::vector<DataPoint>& GetDataPoints() const;
  void AddDataPoint(const DataPoint& new_data_point);

  // initialize screen
  void Initialize() const;

  // run sim
  void Run();

 private:
  std::vector<DataPoint> data_points_;

  struct LinearModel {
    float weight1 = 0.0f;
    float weight2 = 0.0f;
    float bias = 0.0f;
    float learn_rate = 0.05f;  
    int KEpochs = 1;
  } model;

  void UpdateModelParameters();

  void Draw() const;

};

#endif