#pragma once

#include "raylib.h"
#include "paddle.h"
#include "ball.h"
#include <vector>

class ChaosItem {
  public:
    ChaosItem(int type, Vector2 position);
    void Draw() const;
    void ApplyEffect(Paddle* player, Paddle* cpu, GameBall* ball, std::vector<TempBall*>& balls);

    int type;
    Vector2 position;
    Color color;
    float radius;
};