#include "models.hpp"

#include <algorithm>
#include <stdexcept>

std::vector<float> PolynomialModel::ExpandFeatures(float x1, float x2) const {
  std::vector<float> features;
  for (int k = 1; k <= degree; ++k) {
    for (int i = k; i >= 0; --i) {
      const int j = k - i;
      features.push_back(std::pow(x1, i) * std::pow(x2, j));
    }
  }
  return features;
}

PolynomialModel::PolynomialModel(int deg, float learning_rate)
    : degree(deg), lr(learning_rate) {
  if (degree < 1 || degree > 10) {
    throw std::invalid_argument("Polynomial degree must be between 1 and 10.");
  }
  weights.resize(ExpandFeatures(0.0f, 0.0f).size(), 0.0f);
}

float PolynomialModel::Predict(float x1, float x2) const {
  const std::vector<float> features = ExpandFeatures(x1, x2);
  float z = bias;
  for (std::size_t k = 0; k < weights.size(); ++k) {
    z += weights[k] * features[k];
  }

  return 1.0f / (1.0f + std::exp(-z));
}

void PolynomialModel::Train(float x1, float x2, float label) {
  const std::vector<float> features = ExpandFeatures(x1, x2);
  const float error = Predict(x1, x2) - label;

  for (std::size_t k = 0; k < weights.size(); ++k) {
    weights[k] -= lr * error * features[k];
  }
  bias -= lr * error;
}

void PolynomialModel::Reset() {
  std::fill(weights.begin(), weights.end(), 0.0f);
  bias = 0.0f;
}

void PolynomialModel::SetLearningRate(float learning_rate) {
  lr = learning_rate;
}

void PolynomialModel::SetDegree(int new_degree) {
  if (new_degree < 1 || new_degree > 10) {
    throw std::invalid_argument("Polynomial degree must be between 1 and 10.");
  }
  degree = new_degree;
  weights.resize(ExpandFeatures(0.0f, 0.0f).size(), 0.0f);
}

int PolynomialModel::GetDegree() const { return degree; }

std::vector<MlModel::Parameter> PolynomialModel::GetParameters() const {
  std::vector<Parameter> parameters;
  parameters.reserve(weights.size() + 1);
  for (std::size_t i = 0; i < weights.size(); ++i) {
    parameters.push_back({"Weight " + std::to_string(i + 1), weights[i]});
  }
  parameters.push_back({"Bias", bias});
  return parameters;
}