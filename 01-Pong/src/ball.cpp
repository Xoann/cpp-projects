#include "ball.h"
#include <raylib.h>

Ball::Ball(int x, int y, int speed_x, int speed_y, int radius) : x(x), y(y), speed_x(speed_x), speed_y(speed_y), radius(radius) {
  original_speed_x = speed_x;
  original_speed_y = speed_y;
}

void Ball::Update() {
  x += speed_x;
  y += speed_y;

  if (y + radius >= GetScreenHeight() || y - radius <= 0) {
    speed_y *= -1;
  }
  if (x + radius >= GetScreenWidth() || x - radius <= 0) { 
    speed_x *= -1;
  }
  
}

void Ball::Draw() const {
    DrawCircle(x, y, radius, YELLOW);
}

void Ball::ResetBall() {
    x = GetScreenWidth() / 2;
    y = GetScreenHeight() / 2;

    int speed_choices[2] = {-1, 1};
    speed_x = original_speed_x;
    speed_y = original_speed_y;
    speed_x *= speed_choices[GetRandomValue(0, 1)];
    speed_y *= speed_choices[GetRandomValue(0, 1)];
}


GameBall::GameBall(int x, int y, int speed_x, int speed_y, int radius) : Ball(x, y, speed_x, speed_y, radius) {}

void GameBall::Update(int* player_score, int* cpu_score) {
  x += speed_x;
  y += speed_y;

  if (y + radius >= GetScreenHeight() || y - radius <= 0) {
    speed_y *= -1;
  }
  if (x + radius >= GetScreenWidth()) { // Player wins
    ResetBall();
    (*player_score)++;
  }
  
  if (x - radius <= 0) { // Cpu wins
    ResetBall();
    (*cpu_score)++;
  }
}