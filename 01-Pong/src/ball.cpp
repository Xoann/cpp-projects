#include "ball.h"
#include <raylib.h>

Ball::Ball() : x(GetScreenWidth() / 2), y(GetScreenHeight() / 2), speed_x(7), speed_y(7), radius(20) {}

void Ball::Update(int* player_score, int* cpu_score) {
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

void Ball::Draw() const {
    DrawCircle(x, y, radius, YELLOW);
}

void Ball::ResetBall() {
    x = GetScreenWidth() / 2;
    y = GetScreenHeight() / 2;

    int speed_choices[2] = {-1, 1};
    speed_x *= speed_choices[GetRandomValue(0, 1)];
    speed_y *= speed_choices[GetRandomValue(0, 1)];
}

