#include "raylib.h"

int main() {
  InitWindow(800, 800, "ML Decision Boundary Visualizer");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}