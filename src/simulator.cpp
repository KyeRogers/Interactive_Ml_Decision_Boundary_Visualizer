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
      data_points_.push_back(new_point);
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      temp_click = GetMousePosition();  
      DataPoint new_point(temp_click, 1);
      new_point.ScreenToNorm();
      data_points_.push_back(new_point);
    } else if (IsKeyPressed(KEY_SPACE)) {
      data_points_.erase(data_points_.begin(), data_points_.end());
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (DataPoint& point : data_points_) {
      point.NormToScreen();
      if (point.GetClassFlag()) {
        DrawCircleV(point.GetPosition(), 8.0f, BLUE);
      } else {
        DrawCircleV(point.GetPosition(), 8.0f, RED);
      }
      point.ScreenToNorm();
    }
    EndDrawing();
  }
  CloseWindow();
}