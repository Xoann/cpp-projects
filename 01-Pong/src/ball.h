#pragma once

class Ball {
  public: 
    Ball();
    void Update(int* player_score, int* cpu_score);
    void Draw() const;
    void ResetBall();

    float x, y;
    float speed_x, speed_y;
    int radius;
};;
