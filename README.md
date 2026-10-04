# Interactive_Ml_Decision_Boundary_Visualizer

## THE GOAL
With this project i am looking to build an interactive , real-time machine learning visualizer built from scratch using c++ and Raylib.
I plan to learn and implement binary classification algorithms such as logistic regression, and stochastic gradient descent.

## What it will do
* The user will left click to place a class 0 point and right click to place a class 1 point on the 2D screen. They start red and blue, and their colors can be changed in the help menu.
* The model trains when data, polynomial degree, learning rate, or epoch count changes rather than retraining every frame.
* The decision grid will evaluate model probabilities across discrete  screen coordinates to interpolate bavkground colours and visulaize decision bounaries.
* It will use a pure c++ engine with custom implementation of linear algebra, sigmoid activation, and gradient calculations with zero dependencies on heavy ML frameworks like PyTorch or TensorFlow

## Controls
* Press `H` or `?` to open the help and simulation controls panel.
* Click the file path field (or press `F`), enter a path, and press `Enter` or click `Load`.
* Data files contain one point per line in `x y class` format. Coordinates are screen pixels from 0 to the current screen size, and class must be `0` or `1`.
* Use the `+` and `-` buttons in the panel to change the learning rate, epoch count, and pixel block size.
* The help panel also lets you change each class color and the square screen size (800-1600 pixels, starting at 800).
* Adjust the displayed dot radius in the help panel (2-24 pixels, starting at 6).
* Select polynomial degree 1-10 in the help panel by clicking a number, or press a number-row key at any time (`0` selects degree 10).
* The decision grid starts with 20-pixel blocks; larger blocks improve rendering speed at the cost of a coarser boundary.

## Code structure
* `Simulator` owns the data, model, file loading, training, and prediction-grid generation.
* `Renderer` owns Raylib window management, input polling, UI controls, and drawing.
* `DataPoint` stores normalized coordinates and class labels without depending on Raylib.
