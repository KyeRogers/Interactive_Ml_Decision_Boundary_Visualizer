#include "simulator.hpp"

Simulator::Simulator() : data_points_(0) {}

std::vector<DataPoint> Simulator::GetDataPoints() const {
  return data_points_;
}

void Simulator::AddDataPoint(DataPoint& new_data_point) {
  data_points_.push_back(new_data_point);
}

