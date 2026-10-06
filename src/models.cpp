#include "models.hpp"

#include <algorithm>
#include <cmath>
#include <random>
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

// Neural Network

void NeuralNetworkModel::Reset() {
  weights_.assign(k_hidden_, std::vector<float>(2, 0.0f));
  biases_.assign(k_hidden_, 0.0f);
  output_weights_.assign(k_hidden_, 0.0f);
  forward_cache_.hidden_.assign(k_hidden_, {0.0, 0.0});
  output_bias_ = 0.0f;

  std::random_device rd;
  std::mt19937 gen(rd());

  std::normal_distribution<float> hidden_dist(0.0f, 1.0f);
  for (std::size_t i = 0; i < weights_.size(); ++i) {
    for (std::size_t j = 0; j < weights_[i].size(); ++j) {
      weights_[i][j] = hidden_dist(gen);
    }
  }

  const float output_limit =
      std::sqrt(6.0f / static_cast<float>(k_hidden_ + 1));
  std::uniform_real_distribution<float> output_dist(-output_limit,
                                                     output_limit);
  for (float& weight : output_weights_) {
    weight = output_dist(gen);
  }
}

NeuralNetworkModel::ForwardCache NeuralNetworkModel::Forward(float x1,
                                                             float x2) const {
  ForwardCache temp;
  temp.x1 = x1;
  temp.x2 = x2;
  temp.hidden_.resize(k_hidden_);
  for (int i = 0; i < k_hidden_; ++i) {
    temp.hidden_[i].z = weights_[i][0] * x1 + weights_[i][1] * x2 + biases_[i];
    temp.hidden_[i].a = std::max(0.0, temp.hidden_[i].z);
    temp.z2 += temp.hidden_[i].a * output_weights_[i];
  }
  temp.z2 += output_bias_;

  temp.a2 = 1.0 / (1.0 + std::exp(-temp.z2));

  return temp;
}

float NeuralNetworkModel::Predict(float x1, float x2) const {
  ForwardCache temp = Forward(x1, x2);
  return temp.a2;
}

void NeuralNetworkModel::Train(float x1, float x2, float label) {
  forward_cache_ = Forward(x1, x2);

  const float output_error = forward_cache_.a2 - label;

  std::vector<float> hidden_errors(k_hidden_);
  for (int i = 0; i < k_hidden_; ++i) {
    const float relu_derivative =
        forward_cache_.hidden_[i].z > 0.0 ? 1.0f : 0.0f;
    hidden_errors[i] =
        output_error * output_weights_[i] * relu_derivative;
  }

  output_bias_ -= lr * output_error;

  for (int i = 0; i < k_hidden_; ++i) {
    output_weights_[i] -= lr * output_error * forward_cache_.hidden_[i].a;
    weights_[i][0] -= lr * hidden_errors[i] * x1;
    weights_[i][1] -= lr * hidden_errors[i] * x2;
    biases_[i] -= lr * hidden_errors[i];
  }
}

NeuralNetworkModel::NeuralNetworkModel(const int k_hidden)
    : k_hidden_{k_hidden} {
  if (k_hidden_ <= 0) {
    throw std::invalid_argument("Neural network needs at least one hidden neuron.");
  }
  Reset();
}

void NeuralNetworkModel::SetLearningRate(float learning_rate) {
  lr = learning_rate;
}

void NeuralNetworkModel::SetDegree(int degree) { (void)degree; }

int NeuralNetworkModel::GetDegree() const { return 0; }

std::vector<MlModel::Parameter> NeuralNetworkModel::GetParameters() const {
  std::vector<MlModel::Parameter> params;
  params.push_back({"learning_rate", lr});
  params.push_back({"k_hidden", static_cast<float>(k_hidden_)});
  return params;
}