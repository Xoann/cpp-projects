#pragma once

#include <vector>

class Ball {
  public: 
    Ball(int x, int y, int speed_x, int speed_y, int radius);
    void Update();
    void Draw() const;
    void ResetBall();

    float x, y;
    float speed_x, speed_y;
    int radius;
  
  private:
    float original_speed_x;
    float original_speed_y;
};;

class GameBall: public Ball {
  public:
    GameBall(int x, int y, int speed_x, int speed_y, int radius);
    bool Update(int* player_score, int* cpu_score);
};

class TempBall: public GameBall {
  public:
    TempBall(int x, int y, int speed_x, int speed_y, int radius);
    bool Update(int* player_score, int* cpu_score, std::vector<TempBall*>& balls);
    void Draw() const;
    void ResetBall(std::vector<TempBall*>& balls);
};