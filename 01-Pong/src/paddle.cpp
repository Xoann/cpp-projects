#include "paddle.h"
#include <raylib.h>

Paddle::Paddle(int x, int y, int width, int height, int speed) : x(x), y(y), width(width), height(height), speed(speed) {
    original_paddle_width = width;
    original_paddle_height = height;
}

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
    width = original_paddle_width;
    height = original_paddle_height;
}

CpuPaddle::CpuPaddle(int x, int y, int width, int height, int speed) : Paddle(x, y, width, height, speed) {}

void CpuPaddle::Update(GameBall game_ball, std::vector<TempBall*>& balls) {
    float ball_y = game_ball.y;

    // Get either the closest ball with positive x speed
    // or the farthest ball with negative x speed
    for (TempBall* ball : balls) {
        if (ball->speed_x > 0) {
            if (ball->x > game_ball.x) {
                ball_y = ball->y;
            }
        } else {
            if (ball->x < game_ball.x) {
                ball_y = ball->y;
            }
        }
    }

    if (y + height / 2 < ball_y) {
        y += speed;
    }
    if (y + height / 2 > ball_y) {
        y -= speed;
    }

    LimitMovement();
}