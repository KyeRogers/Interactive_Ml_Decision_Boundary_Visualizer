#include "renderer.hpp"

#include <algorithm>

#include "raylib.h"

namespace {
struct NamedColor {
  const char* name;
  Color color;
};

constexpr NamedColor kPointColors[] = {
    {"Red", RED}, {"Blue", BLUE}, {"Green", GREEN}, {"Orange", ORANGE},
    {"Purple", PURPLE}, {"Gold", GOLD}, {"Pink", PINK}, {"Maroon", MAROON}};
constexpr int kPointColorCount =
    static_cast<int>(sizeof(kPointColors) / sizeof(kPointColors[0]));
constexpr Rectangle kFileInputBounds{75.0f, 232.0f, 475.0f, 36.0f};
constexpr Rectangle kLoadButtonBounds{560.0f, 232.0f, 150.0f, 36.0f};
constexpr Rectangle kLearnRateMinusBounds{350.0f, 476.0f, 38.0f, 32.0f};
constexpr Rectangle kLearnRatePlusBounds{394.0f, 476.0f, 38.0f, 32.0f};
constexpr Rectangle kEpochsMinusBounds{620.0f, 358.0f, 38.0f, 32.0f};
constexpr Rectangle kEpochsPlusBounds{664.0f, 358.0f, 38.0f, 32.0f};
constexpr Rectangle kBlockSizeMinusBounds{620.0f, 408.0f, 38.0f, 32.0f};
constexpr Rectangle kBlockSizePlusBounds{664.0f, 408.0f, 38.0f, 32.0f};
constexpr Rectangle kScreenSizeMinusBounds{620.0f, 458.0f, 38.0f, 32.0f};
constexpr Rectangle kScreenSizePlusBounds{664.0f, 458.0f, 38.0f, 32.0f};
constexpr Rectangle kClass0ColorMinusBounds{350.0f, 564.0f, 38.0f, 32.0f};
constexpr Rectangle kClass0ColorPlusBounds{394.0f, 564.0f, 38.0f, 32.0f};
constexpr Rectangle kClass1ColorMinusBounds{350.0f, 608.0f, 38.0f, 32.0f};
constexpr Rectangle kClass1ColorPlusBounds{394.0f, 608.0f, 38.0f, 32.0f};
constexpr Rectangle kDotRadiusMinusBounds{620.0f, 558.0f, 38.0f, 32.0f};
constexpr Rectangle kDotRadiusPlusBounds{664.0f, 558.0f, 38.0f, 32.0f};

/** Checks whether the left mouse button was pressed inside a UI rectangle. */
bool IsClicked(Rectangle bounds) {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
         CheckCollisionPointRec(GetMousePosition(), bounds);
}

/** Draws a button and highlights it while the pointer hovers over it. */
void DrawButton(Rectangle bounds, const char* label) {
  const bool hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
  DrawRectangleRec(bounds, hovered ? SKYBLUE : LIGHTGRAY);
  DrawRectangleLinesEx(bounds, 1.0f, DARKGRAY);
  constexpr int font_size = 20;
  DrawText(label,
           static_cast<int>(bounds.x +
                            (bounds.width - MeasureText(label, font_size)) /
                                2),
           static_cast<int>(bounds.y +
                            (bounds.height - font_size) / 2),
           font_size, DARKGRAY);
}

/** Advances or reverses an index through the selectable point colors. */
int CycleColor(int index, int direction) {
  return (index + direction + kPointColorCount) % kPointColorCount;
}
}  // namespace

/** Creates the Raylib window and configures its frame rate and exit key. */
void Renderer::Initialize(int screen_size) {
  current_screen_size_ = screen_size;
  InitWindow(screen_size, screen_size, "ML Decision Boundary Visualizer");
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);
}

/** Reports whether the user or operating system requested window closure. */
bool Renderer::ShouldClose() const { return WindowShouldClose(); }

/** Polls Raylib input and returns actions for the simulator to process. */
Renderer::InputEvents Renderer::PollInput() {
  InputEvents events;
  bool file_input_cancelled = false;
  if (file_input_active_) {
    int pressed_char = GetCharPressed();
    while (pressed_char > 0) {
      if (pressed_char >= 32 && pressed_char <= 126 &&
          filename_entry_.size() < 255) {
        filename_entry_ += static_cast<char>(pressed_char);
      }
      pressed_char = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !filename_entry_.empty()) {
      filename_entry_.pop_back();
    }
    if (IsKeyPressed(KEY_ENTER)) {
      events.load_file = true;
      events.load_filename = filename_entry_;
      file_input_active_ = filename_entry_.empty();
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
      file_input_active_ = false;
      file_input_cancelled = true;
    }
  } else {
    bool help_requested = IsKeyPressed(KEY_H);
    int pressed_char = GetCharPressed();
    while (pressed_char > 0) {
      help_requested = help_requested || pressed_char == '?';
      pressed_char = GetCharPressed();
    }
    if (help_requested) {
      show_help_ = true;
    }
  }

  if (show_help_ && !file_input_active_ && !file_input_cancelled &&
      (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_Q))) {
    show_help_ = false;
  }

  if (IsKeyPressed(KEY_F) && !file_input_active_) {
    show_help_ = true;
    file_input_active_ = true;
    filename_entry_.clear();
  }

  if (show_help_) {
    if (IsClicked(kFileInputBounds)) {
      file_input_active_ = true;
    } else if (IsClicked(kLoadButtonBounds)) {
      events.load_file = true;
      events.load_filename = filename_entry_;
      file_input_active_ = filename_entry_.empty();
    } else if (IsClicked(kLearnRateMinusBounds)) {
      events.learn_rate_delta = -0.01f;
    } else if (IsClicked(kLearnRatePlusBounds)) {
      events.learn_rate_delta = 0.01f;
    } else if (IsClicked(kEpochsMinusBounds)) {
      events.epochs_delta = -10;
    } else if (IsClicked(kEpochsPlusBounds)) {
      events.epochs_delta = 10;
    } else if (IsClicked(kBlockSizeMinusBounds)) {
      events.block_size_delta = -2;
    } else if (IsClicked(kBlockSizePlusBounds)) {
      events.block_size_delta = 2;
    } else if (IsClicked(kScreenSizeMinusBounds)) {
      events.screen_size_delta = -100;
    } else if (IsClicked(kScreenSizePlusBounds)) {
      events.screen_size_delta = 100;
    } else if (IsClicked(kDotRadiusMinusBounds)) {
      events.dot_radius_delta = -1;
    } else if (IsClicked(kDotRadiusPlusBounds)) {
      events.dot_radius_delta = 1;
    } else if (IsClicked(kClass0ColorMinusBounds)) {
      class_0_color_index_ = CycleColor(class_0_color_index_, -1);
    } else if (IsClicked(kClass0ColorPlusBounds)) {
      class_0_color_index_ = CycleColor(class_0_color_index_, 1);
    } else if (IsClicked(kClass1ColorMinusBounds)) {
      class_1_color_index_ = CycleColor(class_1_color_index_, -1);
    } else if (IsClicked(kClass1ColorPlusBounds)) {
      class_1_color_index_ = CycleColor(class_1_color_index_, 1);
    }
  } else {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      const Vector2 mouse = GetMousePosition();
      events.add_point = true;
      events.point_x = mouse.x;
      events.point_y = mouse.y;
      events.point_class = false;
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      const Vector2 mouse = GetMousePosition();
      events.add_point = true;
      events.point_x = mouse.x;
      events.point_y = mouse.y;
      events.point_class = true;
    } else if (IsKeyPressed(KEY_SPACE)) {
      events.clear_points = true;
    }
  }

  events.help_open = show_help_;
  return events;
}

/**
 * @brief Renders the probability grid, points, or help controls.
 * @param points Normalized points to display.
 * @param probabilities Row-major model probabilities for grid cells.
 * @param grid_columns Number of grid cells per row.
 */
void Renderer::Render(const std::vector<DataPoint>& points,
                      const std::vector<float>& probabilities,
                      int grid_columns, int block_size, int screen_size,
                      float weight1, float weight2, int dot_radius, float bias,
                      float learn_rate, int epochs,
                      const std::string& file_status) {
  if (screen_size != current_screen_size_) {
    current_screen_size_ = screen_size;
    SetWindowSize(screen_size, screen_size);
  }

  BeginDrawing();
  ClearBackground(RAYWHITE);
  if (show_help_) {
    DrawHelp(weight1, weight2, bias, learn_rate, epochs, block_size,
             screen_size, dot_radius, file_status);
    EndDrawing();
    return;
  }

  const int grid_rows =
      static_cast<int>((probabilities.size() + grid_columns - 1) / grid_columns);
  for (int row = 0; row < grid_rows; ++row) {
    for (int column = 0; column < grid_columns; ++column) {
      const std::size_t index =
          static_cast<std::size_t>(row) * grid_columns + column;
      if (index >= probabilities.size()) {
        continue;
      }
      const int x = column * block_size;
      const int y = row * block_size;
      const Color color =
          ColorLerp(kPointColors[class_0_color_index_].color,
                    kPointColors[class_1_color_index_].color,
                    probabilities[index]);
      DrawRectangle(x, y, std::min(block_size, screen_size - x),
                    std::min(block_size, screen_size - y), color);
    }
  }

  const float point_radius = static_cast<float>(dot_radius);
  for (const DataPoint& point : points) {
    const Vector2 position{
        static_cast<float>(point.GetXCoordenate() * screen_size / 2.0 +
                           screen_size / 2.0),
        static_cast<float>(-point.GetYCoordenate() * screen_size / 2.0 +
                           screen_size / 2.0)};
    const Color color = point.GetClassFlag()
                            ? kPointColors[class_1_color_index_].color
                            : kPointColors[class_0_color_index_].color;
    DrawCircleV(position, point_radius, color);
  }
  DrawText("Press ? for help (H)", 20, 20, 20, DARKGRAY);
  EndDrawing();
}

/** Closes the Raylib window. */
void Renderer::Close() { CloseWindow(); }

/** Draws the help panel, file input, model values, and interactive controls. */
void Renderer::DrawHelp(float weight1, float weight2, float bias,
                        float learn_rate, int epochs, int block_size,
                        int screen_size, int dot_radius,
                        const std::string& file_status) const {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
  DrawRectangle(40, 35, 720, 730, RAYWHITE);
  DrawRectangleLines(40, 35, 720, 730, DARKGRAY);
  DrawText("HELP & SIMULATION CONTROLS", 210, 55, 22, MAROON);
  DrawText("Left click: class 0   |   Right click: class 1   |   Space: reset",
           75, 95, 18, DARKGRAY);
  DrawText("F: open file input   |   Type x y class (0 or 1), one point per line",
           75, 125, 18, DARKGRAY);
  DrawText("File path:", 75, 205, 18, BLACK);
  DrawRectangleRec(kFileInputBounds, WHITE);
  DrawRectangleLinesEx(kFileInputBounds, 1.0f,
                       file_input_active_ ? BLUE : DARKGRAY);
  std::string shown_filename =
      filename_entry_.empty() ? "Click here and type a path..." : filename_entry_;
  if (shown_filename.size() > 48) {
    shown_filename = "..." + shown_filename.substr(shown_filename.size() - 45);
  }
  DrawText(shown_filename.c_str(), 83, 240, 18,
           filename_entry_.empty() ? GRAY : BLACK);
  if (file_input_active_ && static_cast<int>(GetTime() * 2.0) % 2 == 0) {
    const int cursor_x = 83 + MeasureText(shown_filename.c_str(), 18);
    DrawLine(cursor_x + 2, 239, cursor_x + 2, 259, BLACK);
  }
  DrawButton(kLoadButtonBounds, "Load");
  DrawText(file_status.c_str(), 75, 275, 16,
           file_status.find("Error:") == 0 ? RED : DARKGRAY);
  DrawLine(70, 310, 730, 310, LIGHTGRAY);
  DrawText("Model parameters", 75, 325, 20, BLACK);
  DrawText(TextFormat("Weight 1: %.4f", weight1), 75, 365, 18, DARKBLUE);
  DrawText(TextFormat("Weight 2: %.4f", weight2), 75, 395, 18, DARKBLUE);
  DrawText(TextFormat("Bias: %.4f", bias), 75, 425, 18, DARKBLUE);
  DrawText(TextFormat("Learn rate: %.4f", learn_rate), 75, 482, 18, DARKBLUE);
  DrawButton(kLearnRateMinusBounds, "-");
  DrawButton(kLearnRatePlusBounds, "+");
  DrawText(TextFormat("Epochs: %d", epochs), 455, 365, 18, DARKBLUE);
  DrawButton(kEpochsMinusBounds, "-");
  DrawButton(kEpochsPlusBounds, "+");
  DrawText(TextFormat("Pixel block: %d px", block_size), 455, 415, 18,
           DARKBLUE);
  DrawButton(kBlockSizeMinusBounds, "-");
  DrawButton(kBlockSizePlusBounds, "+");
  DrawText(TextFormat("Screen: %d x %d", screen_size, screen_size), 455, 465,
           18, DARKBLUE);
  DrawButton(kScreenSizeMinusBounds, "-");
  DrawButton(kScreenSizePlusBounds, "+");
  DrawText("Epochs: +/- 10 (1-1000)", 455, 495, 16, GRAY);
  DrawText("Pixel block: +/- 2 (2-100)", 455, 518, 16, GRAY);
  DrawText("Screen: +/- 100 (800-1600)", 455, 541, 16, GRAY);
  DrawText(TextFormat("Dot radius: %d px", dot_radius), 455, 565, 18,
           DARKBLUE);
  DrawButton(kDotRadiusMinusBounds, "-");
  DrawButton(kDotRadiusPlusBounds, "+");
  DrawText("Dot radius: +/- 1 (2-24 px)", 455, 600, 16, GRAY);
  DrawText("Learn rate: +/- 0.01 (0.001-1.0)", 75, 525, 16, GRAY);
  DrawText(TextFormat("Class 0 color: %s",
                      kPointColors[class_0_color_index_].name),
           75, 565, 18, DARKBLUE);
  DrawRectangle(290, 568, 42, 22, kPointColors[class_0_color_index_].color);
  DrawButton(kClass0ColorMinusBounds, "-");
  DrawButton(kClass0ColorPlusBounds, "+");
  DrawText(TextFormat("Class 1 color: %s",
                      kPointColors[class_1_color_index_].name),
           75, 609, 18, DARKBLUE);
  DrawRectangle(290, 612, 42, 22, kPointColors[class_1_color_index_].color);
  DrawButton(kClass1ColorMinusBounds, "-");
  DrawButton(kClass1ColorPlusBounds, "+");
  DrawText("Use +/- to cycle colors (defaults: red / blue)", 75, 660, 16,
           GRAY);
  DrawText("Press ESC or Q to close", 280, 745, 18, RED);
}
