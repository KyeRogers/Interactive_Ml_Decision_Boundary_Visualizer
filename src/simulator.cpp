#include "simulator.hpp"

#include "raylib.h"

const std::vector<DataPoint>& Simulator::GetDataPoints() const {
  return data_points_;
}

void Simulator::AddDataPoint(const DataPoint& new_data_point) {
  data_points_.push_back(new_data_point);
}

void Simulator::Initialize() const {
  InitWindow(800, 800, "ML Decision Boundary Visualizer");
  SetTargetFPS(60);
}

void Simulator::Run() {
  Vector2 temp_click;
  while (!WindowShouldClose()) {
    // register mouse clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      temp_click = GetMousePosition();
      DataPoint new_point(temp_click, 0);
      new_point.ScreenToNorm();
      AddDataPoint(new_point);
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      temp_click = GetMousePosition();
      DataPoint new_point(temp_click, 1);
      new_point.ScreenToNorm();
      AddDataPoint(new_point);
    } else if (IsKeyPressed(KEY_SPACE)) {
      data_points_.clear();
    }

    UpdateModelParameters();

    Draw();
  }
  CloseWindow();
}

void Simulator::UpdateModelParameters() {
  for (int i{0}; i < model.KEpochs; i++) {
    // loop through dataset points
    for (const DataPoint& point : data_points_) {
      // For each point $(x_1, x_2, y)$, calculate prediction
      // ŷ = learn_rate(w_1 x_1 + w_2 x_2 + b)
      float prediction = (model.weight1 * point.GetXCoordenate() +
                          model.weight2 * point.GetYCoordenate() + model.bias);

      // Calculate error (ŷ - y)
      float error = prediction - point.GetClassFlag();

      // Apply the update equations to shift weight1, weight2 and bias
      model.weight1 -= model.learn_rate * error * point.GetXCoordenate();
      model.weight2 -= model.learn_rate * error * point.GetYCoordenate();
      model.bias -= model.learn_rate * error;
    }
  }
}

void Simulator::Draw() const {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  for (const DataPoint point : data_points_) {
    DataPoint screen_point = point;
    screen_point.NormToScreen();
    if (point.GetClassFlag()) {
      DrawCircleV(screen_point.GetPosition(), 8.0f, BLUE);
    } else {
      DrawCircleV(screen_point.GetPosition(), 8.0f, RED);
    }
  }
  EndDrawing();
}
