#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <vector>

#include "data_point.hpp"
#include "models.hpp"

class Renderer {
 public:
  struct InputEvents {
    bool add_point = false;
    double point_x = 0.0;
    double point_y = 0.0;
    bool point_class = false;
    bool clear_points = false;
    bool load_file = false;
    std::string load_filename;
    float learn_rate_delta = 0.0f;
    int epochs_delta = 0;
    int polynomial_degree = 0;
    int hidden_neurons_delta = 0;
    bool toggle_model = false;
    int block_size_delta = 0;
    int screen_size_delta = 0;
    int dot_radius_delta = 0;
    bool help_open = false;
  };

  void Initialize(int screen_size);
  bool ShouldClose() const;
  InputEvents PollInput(MlModel::Type model_type);
  void Render(const std::vector<DataPoint>& points,
              const std::vector<float>& probabilities, int grid_columns,
              int block_size, int screen_size,
              const std::vector<MlModel::Parameter>& model_parameters,
              MlModel::Type model_type, int polynomial_degree,
              int hidden_neurons, int dot_radius, float learn_rate, int epochs,
              const std::string& file_status);
  void Close();

 private:
  bool show_help_ = false;
  bool file_input_active_ = false;
  std::string filename_entry_;
  int class_0_color_index_ = 0;
  int class_1_color_index_ = 1;
  int current_screen_size_ = 0;

  void DrawHelp(const std::vector<MlModel::Parameter>& model_parameters,
                MlModel::Type model_type, int polynomial_degree,
                int hidden_neurons, float learn_rate, int epochs,
                int block_size, int screen_size, int dot_radius,
                const std::string& file_status) const;
};

#endif
