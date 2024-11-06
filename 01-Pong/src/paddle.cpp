#include "paddle.h"
#include <raylib.h>

Paddle::Paddle(int x, int y, int width, int height, int speed) : x(x), y(y), width(width), height(height), speed(speed) {}

void Paddle::LimitMovement() {
    if (y <= 0) {
        y = 0;
    } 
    if (y + height >= GetScreenHeight()) {
        y = GetScreenHeight() - height;
    }
}

void Paddle::Update() {
    if (IsKeyDown(KEY_W)) {
        y -= speed;
    }
    if (IsKeyDown(KEY_S)) {
        y += speed;
    }

    LimitMovement();
}

void Paddle::Draw() const {
    DrawRectangleRounded(Rectangle{(float)x, (float)y, (float)width, (float)height}, 0.8, 0, WHITE);
}

void Paddle::ResetPaddle() {
    y = GetScreenHeight() / 2 - height / 2;
}

CpuPaddle::CpuPaddle(int x, int y, int width, int height, int speed) : Paddle(x, y, width, height, speed) {}

void CpuPaddle::Update(int ball_y) {
    if (y + height / 2 < ball_y) {
        y += speed;
    }
    if (y + height / 2 > ball_y) {
        y -= speed;
    }

    LimitMovement();
}