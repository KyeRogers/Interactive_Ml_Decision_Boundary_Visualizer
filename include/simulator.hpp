/**
 * Create a simulator class to run the programme/simulation
*/

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "data_point.hpp"
#include <vector>

class Simulator {
  public:
    Simulator() = default;

    const std::vector<DataPoint>& GetDataPoints() const;
    void AddDataPoint(const DataPoint& new_data_point);

    // initialize screen
    void Initialize() const;

    // run sim
    void Run();

  private:
    std::vector<DataPoint> data_points_;
};

#endif