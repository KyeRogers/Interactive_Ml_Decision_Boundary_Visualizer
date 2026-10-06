/**
 * Create a simulator class to run the programme/simulation
 */

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <memory>
#include <string>
#include <vector>

#include "data_point.hpp"
#include "models.hpp"
#include "renderer.hpp"

class Simulator {
 public:
  Simulator();
  explicit Simulator(std::unique_ptr<MlModel> model);

  const std::vector<DataPoint>& GetDataPoints() const;
  void AddDataPoint(const DataPoint& new_data_point);

  void Initialize();

  void LoadFromFile(const std::string& filename);

  // run sim
  void Run();

 private:
  std::vector<DataPoint> data_points_;

  std::unique_ptr<MlModel> model_;
  bool model_needs_training_ = false;
  int polynomial_degree_ = 1;
  int hidden_neurons_ = 8;
  float learn_rate_ = 0.05f;
  int KEpochs = 50;
  int block_size_ = 20;
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