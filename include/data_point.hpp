/** 
 * Create the data structure to store points
 */

#ifndef DATAPOINT_HPP
#define DATAPOINT_HPP
 
 class DataPoint {
  public:
    // constructors
    DataPoint(const double x, const double y, const bool flag);

    // getters
    double GetXCoordenate() const; 
    double GetYCoordenate() const;
    bool GetClassFlag() const;
    
    // setters 
    void SetX(const double x);
    void SetY(const double y);
    void SetClassFlag(const bool class_flag);
    void ToggleClassFlag();

    void ScreenToNorm();
    void NormToScreen();

  private:  
    double x_coordenate_;
    double y_coordenate_;
    bool class_flag_; // False(class 0) = red ; True(class 1) = blue
 };

 #endif