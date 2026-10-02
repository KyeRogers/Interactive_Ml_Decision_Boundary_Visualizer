#include "data_point.hpp"

// constructor
DataPoint::DataPoint(const double x, const double y, const bool flag) :x_coordenate_{x}, y_coordenate_{y}, class_flag_{flag} {}
DataPoint::DataPoint(Vector2 position, const bool flag) :x_coordenate_{static_cast<double>(position.x)}, y_coordenate_{static_cast<double>(position.y)}, class_flag_{flag} {}

// getters
double DataPoint::GetXCoordenate() const {
  return x_coordenate_;
} 

double DataPoint::GetYCoordenate() const {
  return y_coordenate_;
}

bool DataPoint::GetClassFlag() const {
  return class_flag_;
}

Vector2 DataPoint::GetPosition() const {
  return Vector2{static_cast<int>(x_coordenate_), static_cast<int>(y_coordenate_)};
}

// setters 
void DataPoint::SetX(const double x) {x_coordenate_ = x; }
void DataPoint::SetY(const double y) {y_coordenate_ = y; }
void DataPoint::SetClassFlag(const bool class_flag) { class_flag_ = class_flag; }
void DataPoint::ToggleClassFlag() { class_flag_ = !class_flag_; }

/** 
 * @brief Converts raylib coordenate system to a normalised system for efficiency
 * Center of window (400, 400) -> (0.0, 0.0) in ML space.
 * Top-left pixel (0, 0) -> (-1.0, 1.0) in ML space. 
 * Bottom-right pixel (800, 800)-> (1.0, -1.0) in ML space.
*/
void DataPoint::ScreenToNorm() {
  x_coordenate_ = (x_coordenate_ - 400.0) / 400;
  y_coordenate_ = (400.0 - y_coordenate_) / 400;
}

/** 
 * @brief Converts normalised coordenate system to a theraylib coordenate system
 * Center of window (0.0, 0.0) -> (400.0, 400.0) in Raylib space.
 * Top-left pixel (-1.0, 1.0) -> (0.0, 0.0) in Raylib space. 
 * Bottom-right pixel (1.0, -1.0)-> (800.0, 800.0) in Raylib space.
*/
void DataPoint::NormToScreen() {
  x_coordenate_ = (x_coordenate_ * 400.0) + 400.0;
  y_coordenate_ = (-y_coordenate_ * 400.0) + 400.0;
}
