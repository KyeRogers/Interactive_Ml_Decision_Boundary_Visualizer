#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <vector>

#include "data_point.hpp"

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
    int block_size_delta = 0;
    int screen_size_delta = 0;
    int dot_radius_delta = 0;
    bool help_open = false;
  };

  void Initialize(int screen_size);
  bool ShouldClose() const;
  InputEvents PollInput();
  void Render(const std::vector<DataPoint>& points,
              const std::vector<float>& probabilities, int grid_columns,
              int block_size, int screen_size, float weight1, float weight2,
              int dot_radius, float bias, float learn_rate, int epochs,
              const std::string& file_status);
  void Close();

 private:
  bool show_help_ = false;
  bool file_input_active_ = false;
  std::string filename_entry_;
  int class_0_color_index_ = 0;
  int class_1_color_index_ = 1;
  int current_screen_size_ = 0;

  void DrawHelp(float weight1, float weight2, float bias, float learn_rate,
                int epochs, int block_size, int screen_size, int dot_radius,
                const std::string& file_status) const;
};

#endif
