#include <raylib.h>
#include <iostream>

using namespace std;

class Ball {
    public:
    float x,y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        x += speed_x;
        y += speed_y;
    }
};

Ball ball;

int main() 
{
    cout << "Starting the game." << endl;

    const int screen_width = 1280;
    const int screen_height = 800;
    const int paddle_width = 25;
    const int paddle_height = 120;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    while(WindowShouldClose() == false) {
        ball.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(10, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, WHITE);
        ball.Draw();
        DrawRectangle(screen_width - paddle_width - 10, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, WHITE);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}