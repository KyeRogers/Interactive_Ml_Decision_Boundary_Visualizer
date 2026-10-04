/**
 * Create a simulator class to run the programme/simulation
 */

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <string>
#include <vector>

#include "data_point.hpp"
#include "renderer.hpp"

class Simulator {
 public:
  Simulator() = default;

  const std::vector<DataPoint>& GetDataPoints() const;
  void AddDataPoint(const DataPoint& new_data_point);

  void Initialize();

  void LoadFromFile(const std::string& filename);

  // run sim
  void Run();

 private:
  std::vector<DataPoint> data_points_;

  struct LinearModel {
    float weight1 = 0.0f;
    float weight2 = 0.0f;
    float bias = 0.0f;
    float learn_rate = 0.05f;  
    int KEpochs = 50;
  };

  LinearModel model;
  int block_size_ = 10;
  int screen_size_ = 800;
  int dot_radius_ = 6;
  std::string file_status_;
  Renderer renderer_;

  void UpdateModelParameters();
  float Predict(const float x, const float y) const;
  std::vector<float> BuildProbabilityGrid() const;
  void Clear();

};

#endif