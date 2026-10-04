#include "simulator.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

/** Creates a simulator using the default degree-one polynomial model. */
Simulator::Simulator() : Simulator(std::make_unique<PolynomialModel>()) {}

/** Creates a simulator with the supplied model implementation. */
Simulator::Simulator(std::unique_ptr<MlModel> model) : model_(std::move(model)) {
  if (!model_) {
    throw std::invalid_argument("Simulator requires a model.");
  }
  model_->SetLearningRate(learn_rate_);
}

/** Provides read-only access to the simulator's data points. */
const std::vector<DataPoint>& Simulator::GetDataPoints() const {
  return data_points_;
}

/** Adds a point to the training dataset. */
void Simulator::AddDataPoint(const DataPoint& new_data_point) {
  data_points_.push_back(new_data_point);
  model_needs_training_ = true;
}

/** Initializes the renderer using the configured screen size. */
void Simulator::Initialize() { renderer_.Initialize(screen_size_); }

/** Runs input handling, model updates, and rendering until the window closes. */
void Simulator::Run() {
  while (!renderer_.ShouldClose()) {
    const Renderer::InputEvents events = renderer_.PollInput();

    if (events.add_point) {
      DataPoint point(events.point_x, events.point_y, events.point_class);
      point.ScreenToNorm(screen_size_);
      AddDataPoint(point);
    }
    if (events.clear_points) {
      Clear();
    }
    if (events.load_file) {
      if (events.load_filename.empty()) {
        file_status_ = "Error: enter a file path first.";
      } else {
        try {
          LoadFromFile(events.load_filename);
          file_status_ = "Loaded " + std::to_string(data_points_.size()) +
                         " points from file.";
        } catch (const std::exception& error) {
          file_status_ = std::string("Error: ") + error.what();
        }
      }
    }

    const float previous_learn_rate = learn_rate_;
    learn_rate_ =
        std::clamp(learn_rate_ + events.learn_rate_delta, 0.001f, 1.0f);
    model_needs_training_ =
        model_needs_training_ || learn_rate_ != previous_learn_rate;
    model_->SetLearningRate(learn_rate_);
    const int previous_epochs = KEpochs;
    KEpochs = std::clamp(KEpochs + events.epochs_delta, 1, 1000);
    model_needs_training_ =
        model_needs_training_ || KEpochs != previous_epochs;
    if (events.polynomial_degree != 0) {
      if (events.polynomial_degree != model_->GetDegree()) {
        model_->SetDegree(events.polynomial_degree);
        model_needs_training_ = true;
      }
    }
    block_size_ = std::clamp(block_size_ + events.block_size_delta, 2, 100);
    screen_size_ = std::clamp(screen_size_ + events.screen_size_delta, 800, 1600);
    dot_radius_ = std::clamp(dot_radius_ + events.dot_radius_delta, 2, 24);

    if (!events.help_open && model_needs_training_) {
      UpdateModelParameters();
    }

    std::vector<float> probabilities;
    if (!events.help_open) {
      probabilities = BuildProbabilityGrid();
    }
    renderer_.Render(data_points_, probabilities,
                     (screen_size_ + block_size_ - 1) / block_size_,
                     block_size_, screen_size_, model_->GetParameters(),
                     model_->GetDegree(), dot_radius_, learn_rate_, KEpochs,
                     file_status_);
  }
  renderer_.Close();
}

/** Applies the configured number of gradient-descent passes to the dataset. */
void Simulator::UpdateModelParameters() {
  if (data_points_.empty()) {
    model_needs_training_ = false;
    return;
  }
  for (int i{0}; i < KEpochs; i++) {
    for (const DataPoint& point : data_points_) {
      model_->Train(point.GetXCoordenate(), point.GetYCoordenate(),
                    point.GetClassFlag() ? 1.0f : 0.0f);
    }
    model_needs_training_ = false;
  }
}

/** Calculates model probabilities at the center of each rendered grid cell. */
std::vector<float> Simulator::BuildProbabilityGrid() const {
  const int columns = (screen_size_ + block_size_ - 1) / block_size_;
  const int rows = columns;
  std::vector<float> probabilities;
  probabilities.reserve(static_cast<std::size_t>(columns) * rows);

  for (int row = 0; row < rows; ++row) {
    for (int column = 0; column < columns; ++column) {
      const double pixel_x =
          std::min(column * block_size_ + block_size_ / 2.0,
                   static_cast<double>(screen_size_));
      const double pixel_y =
          std::min(row * block_size_ + block_size_ / 2.0,
                   static_cast<double>(screen_size_));
      DataPoint sample(pixel_x, pixel_y, false);
      sample.ScreenToNorm(screen_size_);
      probabilities.push_back(Predict(sample.GetXCoordenate(),
                                      sample.GetYCoordenate()));
    }
  }
  return probabilities;
}

/** Returns the model's sigmoid probability for a normalized coordinate. */
float Simulator::Predict(const float x, const float y) const {
  return model_->Predict(x, y);
}

/**
 * @brief Replaces the dataset with points loaded from a text file.
 * @param filename File with one x y class record per line.
 * @throws std::runtime_error If the file cannot be opened or parsed.
 */
void Simulator::LoadFromFile(const std::string& filename) {
  std::ifstream input_file(filename);
  if (!input_file) {
    throw std::runtime_error("Could not open data file: " + filename);
  }

  std::vector<DataPoint> loaded_points;
  std::string line;
  std::size_t line_number = 0;
  while (std::getline(input_file, line)) {
    ++line_number;
    std::stringstream ss(line);
    double x, y;
    int flag;
    if (!(ss >> x >> y >> flag) || (flag != 0 && flag != 1)) {
      throw std::runtime_error("Invalid data at line " +
                               std::to_string(line_number) + " in " + filename);
    }
    DataPoint point(x, y, flag == 1);
    point.ScreenToNorm(screen_size_);
    loaded_points.push_back(point);
  }
  data_points_.swap(loaded_points);
  learn_rate_ = 0.05f;
  KEpochs = 50;
  model_->SetLearningRate(learn_rate_);
  model_->Reset();
  model_needs_training_ = true;
}

/** Removes all points and resets the model parameters. */
void Simulator::Clear() {
  data_points_.clear();
  learn_rate_ = 0.05f;
  KEpochs = 50;
  model_->SetLearningRate(learn_rate_);
  model_->Reset();
  model_needs_training_ = false;
}
