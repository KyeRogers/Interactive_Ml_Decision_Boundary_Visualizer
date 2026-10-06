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
 public:
  explicit PolynomialModel(int deg = 1, float learning_rate = 0.05f);
  float Predict(float x1, float x2) const override;
  void Train(float x1, float x2, float label) override;
  void Reset() override;
  void SetLearningRate(float learning_rate) override;
  void SetDegree(int degree) override;
  int GetDegree() const override;
  std::vector<Parameter> GetParameters() const override;

 private:
  int degree;
  std::vector<float> weights;
  float bias = 0.0f;
  float lr = 0.05f;

  std::vector<float> ExpandFeatures(float x1, float x2) const;
};

class NeuralNetworkModel : public MlModel {
 public:
  explicit NeuralNetworkModel(const int k_hidden);
  void Reset() override;
  float Predict(float x1, float x2) const override;
  void Train(float x1, float x2, float label) override;

  void SetLearningRate(float learning_rate) override;
  void SetDegree(int degree) override;
  int GetDegree() const override;
  std::vector<Parameter> GetParameters() const override;

 private:
  // hyperparameters
  float lr = 0.05f;
  int k_hidden_;  // number of hidden neurons

  // model parameters
  std::vector<std::vector<float>> weights_;  // size hidden neuron * 2(w1, w2)
  std::vector<float> biases_;                // size hidden neurons
  std::vector<float> output_weights_;        // size hidden neurons
  float output_bias_{0.0f};

  struct HiddenNeuronCache {
    double z{0.0};  // Pre-activation
    double a{0.0};  // Post-activation (ReLU result)
  };

  struct ForwardCache {
    // inputs
    double x1{0.0}, x2{0.0};

    // hidden layer activations
    std::vector<HiddenNeuronCache> hidden_;

    // output layer actuvations
    double z2{0.0};  // pre ( weighted sum)
    double a2{0.0};  // post (final prediction)

  } forward_cache_;

  ForwardCache Forward(float x1, float x2) const;
};

#endif