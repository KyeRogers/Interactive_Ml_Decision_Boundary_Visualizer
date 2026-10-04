#include "simulator.hpp"

#include <cmath>

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
      model = LinearModel();
    }

    UpdateModelParameters();

    Draw(10);
  }
  CloseWindow();
}

void Simulator::UpdateModelParameters() {
  for (int i{0}; i < model.KEpochs; i++) {
    // loop through dataset points
    for (const DataPoint& point : data_points_) {
      // For each point $(x_1, x_2, y)$, calculate prediction
      // ŷ = learn_rate(w_1 x_1 + w_2 x_2 + b)
      float prediction =
          Predict(point.GetXCoordenate(), point.GetYCoordenate());

      // Calculate error (ŷ - y)
      float error = prediction - point.GetClassFlag();

      // Apply the update equations to shift weight1, weight2 and bias
      model.weight1 -= model.learn_rate * error * point.GetXCoordenate();
      model.weight2 -= model.learn_rate * error * point.GetYCoordenate();
      model.bias -= model.learn_rate * error;
    }
  }
}

void Simulator::Draw(const int step) const {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  // draw background
  for (int x{0}; x < 800; x += step) {
    for (int y{0}; y < 800; y += step) {
      DataPoint temp(x + step/2, y + step/2, 0);
      temp.ScreenToNorm();
      float prediction = Predict(temp.GetXCoordenate(), temp.GetYCoordenate());

      // interpolate colour using prediction as factor
      Color colour = ColorLerp(RED, BLUE, prediction);
      DrawRectangle(x, y, step, step, colour);
    }
  }
  // draw points
  for (const DataPoint point : data_points_) {
    DataPoint screen_point = point;
    screen_point.NormToScreen();
    if (point.GetClassFlag()) {
      DrawCircleV(screen_point.GetPosition(), 6.0f, BLUE);
    } else {
      DrawCircleV(screen_point.GetPosition(), 6.0f, RED);
    }
  }
  EndDrawing();
}

float Simulator::Predict(const float x, const float y) const {
  float prediction = model.weight1 * x + model.weight2 * y + model.bias;
  return 1.0f / (1.0f + std::exp(-prediction));  // sigmoid function
}