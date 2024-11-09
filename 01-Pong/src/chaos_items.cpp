#include "chaos_items.h"
#include "paddle.h"
#include "ball.h"
#include <vector>

ChaosItem::ChaosItem(int type, Vector2 position) 
    : type(type), position(position), radius(20) {
    switch (type) {
        case 0:
            color = YELLOW;
            radius = 100;
            break;
        case 1:
            color = BLUE;
            radius = 100;
            break;
        case 2:
            color = GREEN;
            radius = 100;
            break;
        case 3:
            color = RED;
            radius = 100;
            break;
    }
}

void ChaosItem::Draw() const {
    DrawCircleV(position, radius, color);
}

void ChaosItem::ApplyEffect(Paddle* player, Paddle* cpu, GameBall* ball, std::vector<TempBall*>& balls) {
    switch (type) {
        case 0: {
            // Increase ball speed
            ball->speed_x *= 1.3;
            ball->speed_y *= 1.3;
            break;
        }
        case 1: {
            // Create extra ball
            TempBall* newBall = new TempBall(ball->x, ball->y, ball->speed_x, ball->speed_y, ball->radius);
            newBall->speed_x *= -1; // Reverse the direction of the new ball
            balls.push_back(newBall);
            break;
        }
        case 2: {
            // Increase paddle size
            if (GetRandomValue(0, 1) == 0) {
              player->height += 50;
              if (player->height > 300) {
                player->height = 300;
              }
            } else {
              cpu->height += 50;
              if (cpu->height > 300) {
                cpu->height = 300;
              }
            }
            break;
        }
        case 3: {
            // Decrease paddle size
            if (GetRandomValue(0, 1) == 0) {
              player->height -= 50;
              if (player->height < 50) {
                player->height = 50;
              }
            } else {
              cpu->height -= 50;
              if (cpu->height < 50) {
                cpu->height = 50;
              }
            }
            break;
        }
    }
}