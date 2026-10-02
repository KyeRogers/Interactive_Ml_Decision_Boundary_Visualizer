/**
 * Create a simulator class to run the programme/simulation
*/

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "data_point.hpp"
#include <vector>

class Simulator {
  public:
    // constructors
    Simulator();

    // getters
    std::vector<DataPoint> GetDataPoints() const;

    // add data point
    void AddDataPoint(DataPoint& new_data_point);


  private:
    std::vector<DataPoint*> data_points_;

};

#endif