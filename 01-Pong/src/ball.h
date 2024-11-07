#pragma once

class Ball {
  public: 
    Ball(int x, int y, int speed_x, int speed_y, int radius);
    void Update();
    void Draw() const;
    void ResetBall();

    float x, y;
    float speed_x, speed_y;
    int radius;
};;

class GameBall: public Ball {
  public:
    GameBall(int x, int y, int speed_x, int speed_y, int radius);
    void Update(int* player_score, int* cpu_score);
};