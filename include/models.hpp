#ifndef MODELS_HPP
#define MODELS_HPP

#include <cmath>
#include <string>
#include <utility>
#include <vector>

class MlModel {
 public:
  struct Parameter {
    std::string name;
    float value;
  };

  virtual ~MlModel() = default;
  virtual float Predict(float x1, float x2) const = 0;
  virtual void Train(float x1, float x2, float label) = 0;
  virtual void Reset() = 0;
  virtual void SetLearningRate(float learning_rate) = 0;
  virtual void SetDegree(int degree) = 0;
  virtual int GetDegree() const = 0;
  virtual std::vector<Parameter> GetParameters() const = 0;
};

class PolynomialModel : public MlModel {
 private:
  int degree;
  std::vector<float> weights;
  float bias = 0.0f;
  float lr = 0.05f;

  std::vector<float> ExpandFeatures(float x1, float x2) const;

 public:
  explicit PolynomialModel(int deg = 1, float learning_rate = 0.05f);
  float Predict(float x1, float x2) const override;
  void Train(float x1, float x2, float label) override;
  void Reset() override;
  void SetLearningRate(float learning_rate) override;
  void SetDegree(int degree) override;
  int GetDegree() const override;
  std::vector<Parameter> GetParameters() const override;
};

#endif