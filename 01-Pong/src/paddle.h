#pragma once

#include "ball.h"
#include <vector>

class Paddle {
  protected:
  void LimitMovement();

  public:
    Paddle(int x, int y, int width, int height, int speed);
    void Update();
    void Draw() const;
    void ResetPaddle();

    int x, y;
    int width, height;
    int speed;

  private:
    float original_paddle_width;
    float original_paddle_height;
};

class CpuPaddle: public Paddle {
  public:
    CpuPaddle(int x, int y, int width, int height, int speed);
    void Update(GameBall game_ball, std::vector<TempBall*>& balls);
};