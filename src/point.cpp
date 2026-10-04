#include "data_point.hpp"

/** Creates a point from normalized coordinates and its binary class. */
DataPoint::DataPoint(const double x, const double y, const bool flag) :x_coordenate_{x}, y_coordenate_{y}, class_flag_{flag} {}

/** Returns the normalized horizontal coordinate. */
double DataPoint::GetXCoordenate() const {
  return x_coordenate_;
} 

/** Returns the normalized vertical coordinate. */
double DataPoint::GetYCoordenate() const {
  return y_coordenate_;
}

/** Returns whether this point belongs to class 1. */
bool DataPoint::GetClassFlag() const {
  return class_flag_;
}

/** Updates the horizontal coordinate. */
void DataPoint::SetX(const double x) {x_coordenate_ = x; }
/** Updates the vertical coordinate. */
void DataPoint::SetY(const double y) {y_coordenate_ = y; }
/** Assigns the point's binary class. */
void DataPoint::SetClassFlag(const bool class_flag) { class_flag_ = class_flag; }
/** Switches the point to the other binary class. */
void DataPoint::ToggleClassFlag() { class_flag_ = !class_flag_; }

/**
 * @brief Converts screen pixels to normalized model coordinates.
 * @param screen_size Width and height of the square screen in pixels.
 */
void DataPoint::ScreenToNorm(const double screen_size) {
  const double center = screen_size / 2.0;
  x_coordenate_ = (x_coordenate_ - center) / center;
  y_coordenate_ = (center - y_coordenate_) / center;
}

/**
 * @brief Converts normalized model coordinates to screen pixels.
 * @param screen_size Width and height of the square screen in pixels.
 */
void DataPoint::NormToScreen(const double screen_size) {
  const double center = screen_size / 2.0;
  x_coordenate_ = (x_coordenate_ * center) + center;
  y_coordenate_ = (-y_coordenate_ * center) + center;
}
