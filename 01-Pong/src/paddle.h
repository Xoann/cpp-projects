#pragma once

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
};

class CpuPaddle: public Paddle {
  public:
    CpuPaddle(int x, int y, int width, int height, int speed);
    void Update(int ball_y);
};