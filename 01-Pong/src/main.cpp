#include <raylib.h>
#include <iostream>
#include "ball.h"
#include "paddle.h"

using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

const int screen_width = 1280;
const int screen_height = 800;

const int paddle_width = 25;
const int paddle_height = 120;
const int paddle_offset = 10;
const int paddle_speed = 6;


Ball ball;
Paddle player(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed);
CpuPaddle cpu(screen_width - paddle_width - paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed);

int main() 
{

    
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    player.width = paddle_width;
    player.height = paddle_height;
    player.x = 10;
    player.y = screen_height / 2 - paddle_height / 2;
    player.speed = 6;

    cpu.width = paddle_width;
    cpu.height = paddle_height;
    cpu.x = screen_width - paddle_width - 10;
    cpu.y = screen_height / 2 - paddle_height / 2;
    cpu.speed = 6;

    while(WindowShouldClose() == false) {

        // Update
        ball.Update(&player_score, &cpu_score);
        player.Update();
        cpu.Update(ball.y);

        // Check collision
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{(float)player.x, (float)player.y, (float)player.width, (float)player.height})) {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{(float)cpu.x, (float)cpu.y, (float)cpu.width, (float)cpu.height})) {
            ball.speed_x *= -1;
        }

        BeginDrawing();
        ClearBackground(Dark_Green);

        DrawRectangle(0, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

        // Draw
        player.Draw();
        ball.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", player_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpu_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}