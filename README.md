# Interactive_Ml_Decision_Boundary_Visualizer

## THE GOAL
With this project i am looking to build an interactive , real-time machine learning visualizer built from scratch using c++ and Raylib.
I plan to learn and implement binary classification algorithms such as logistic regression, and stochastic gradient descent.

## What it will do
* The user will  left click to place a red (class 0) data point, and right click to place a blue (class 1) data point, on the 2d screen.
* The model will execute continuous gradient descent optimization steps directly within the graphics rendering loop to show weight convergence instantly.
* The decision grid will evaluate model probabilities across discrete  screen coordinates to interpolate bavkground colours and visulaize decision bounaries.
* It will use a pure c++ engine with custom implementation of linear algebra, sigmoid activation, and gradient calculations with zero dependencies on heavy ML frameworks like PyTorch or TensorFlow

