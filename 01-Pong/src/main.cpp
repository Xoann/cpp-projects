#include <raylib.h>
#include "ball.h"
#include "paddle.h"
#include "button.h"
#include <iostream>

using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

const int screen_width = 1280;
const int screen_height = 800;
const int fps = 144;

const int paddle_width = 25;
const int paddle_height = 120;
const int paddle_offset = 10;
const int paddle_speed = 3;

const int ball_radius = 20;
const int ball_speed = 4;

const int max_score = 5;

// Button variables
const int button_width = 200;
const int button_height = 100;
const int button_font_size = 40;
const float button_roundedness = 0.1;

// Game objects
GameBall ball(screen_width / 2, screen_height / 2, ball_speed, ball_speed, ball_radius);
Paddle player(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed);
CpuPaddle cpu(screen_width - paddle_width - paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed);

Ball menu_ball(screen_width / 2, screen_height / 2, ball_speed, ball_speed, ball_radius);

// Sounds
Sound hit_sound;
Sound click_sound;
Sound hover_sound;

// Buttons
Button play_button(screen_width / 2 - 100, screen_height / 2 - 50, button_width, button_height, "Play", button_font_size, button_roundedness, &hover_sound, &click_sound);
Button settings_button(screen_width / 2 - 100, screen_height / 2 + 80, button_width, button_height, "Settings", button_font_size, button_roundedness, &hover_sound, &click_sound);
Button replay_button(screen_width / 2 - 100, screen_height / 2 - 50, button_width, button_height, "Replay", button_font_size, button_roundedness, &hover_sound, &click_sound);
Button menu_button(screen_width / 2 - 100, screen_height / 2 + 80, button_width, button_height, "Menu", button_font_size, button_roundedness, &hover_sound, &click_sound);
Button back_button(20, 20, button_width / 2, button_height / 2, "Back", button_font_size / 2, button_roundedness, &hover_sound, &click_sound);
Button pause_menu_button(screen_width / 2 - 40, screen_height / 2 + 80, 80, 40, "Menu", 20, button_roundedness, &hover_sound, &click_sound);

bool window_open;

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    SETTINGS
};

GameState game_state = GameState::MENU;

void reset_game() {
    player_score = 0;
    cpu_score = 0;
    ball.ResetBall();
}

void draw_game_field() {
    // Background
    ClearBackground(Dark_Green);
    DrawRectangle(0, 0, screen_width / 2, screen_height, Green);
    DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
    DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

    // Scores
    DrawText(TextFormat("%i", player_score), screen_width / 4 - 20, 20, 80, WHITE);
    DrawText(TextFormat("%i", cpu_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

    // Draw
    player.Draw();
    ball.Draw();
    cpu.Draw();
}

int handle_playing() {
    // Check for state changes
    if (IsKeyPressed(KEY_ESCAPE)) {
        game_state = GameState::PAUSED;
        EndDrawing();
    }

    if (player_score == max_score || cpu_score == max_score) {
        game_state = GameState::GAME_OVER;
    }

    // Update
    ball.Update(&player_score, &cpu_score);
    player.Update();
    cpu.Update(ball.y);

    // Check collision
    // if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{(float)player.x, (float)player.y, (float)player.width, (float)player.height})) {
    //     ball.speed_x *= -1;
    //     PlaySound(hit_sound);
    // }

    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{(float)player.x, (float)player.y, (float)player.width, (float)player.height})) {
    // Reverse horizontal direction
    ball.speed_x *= -1;

    // Bounce vertically depending on where the ball hits the paddle
    if (ball.y < player.y) {
        ball.speed_y = -abs(ball.speed_y);
    } else if (ball.y > player.y + player.height) {
        ball.speed_y = abs(ball.speed_y);
    }

    PlaySound(hit_sound);
}

    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{(float)cpu.x, (float)cpu.y, (float)cpu.width, (float)cpu.height})) {
        // Reverse horizontal direction
    ball.speed_x *= -1;

    // Bounce vertically depending on where the ball hits the paddle
    if (ball.y < cpu.y) {
        ball.speed_y = -abs(ball.speed_y);
    } else if (ball.y > cpu.y + cpu.height) {
        ball.speed_y = abs(ball.speed_y);
    }

    PlaySound(hit_sound);
    }

    draw_game_field();

    return 0;
}

void handle_menu() {
    ClearBackground(Dark_Green);
    if (IsKeyPressed(KEY_ENTER)) {
        game_state = GameState::PLAYING;
    }

    menu_ball.Update();
    menu_ball.Draw();

    DrawText("Pong", screen_width / 2 - MeasureText("Pong", 60) / 2, screen_height / 4 - 30, 60, WHITE);

    if (settings_button.Draw(Green, Light_Green, YELLOW)) {
        game_state = GameState::SETTINGS;
    }

    if (play_button.Draw(Green, Light_Green, YELLOW)) {
        reset_game();
        game_state = GameState::PLAYING;
    }
}

void handle_paused() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        game_state = GameState::PLAYING;
    }

    draw_game_field();

    DrawRectangle(0, 0, screen_width, screen_height, Fade(BLACK, 0.5f));

    DrawText("Game Paused", screen_width / 2 - MeasureText("Game Paused", 60) / 2, screen_height / 2 - 30, 60, WHITE);
    DrawText("Press ESC to resume", screen_width / 2 - MeasureText("Press ESC to resume", 20) / 2, screen_height / 2 + 30, 20, WHITE);
    if(pause_menu_button.Draw(Green, Light_Green, WHITE)) {
        game_state = GameState::MENU;
    }
}

void handle_game_over() {
    draw_game_field();

    const char* game_over_message = player_score > cpu_score ? "You win!" : "You lose!";
    const char* result = TextFormat("%i - %i", player_score, cpu_score);

    DrawRectangle(0, 0, screen_width, screen_height, Fade(BLACK, 0.5f));
    DrawText(game_over_message, screen_width / 2 - MeasureText(game_over_message, 60) / 2, screen_height / 4, 60, WHITE);
    DrawText(result, screen_width / 2 - MeasureText(result, 40) / 2, screen_height / 4 + 60, 40, WHITE);
    
    if (replay_button.Draw(Green, Light_Green, WHITE)) {
        reset_game();
        game_state = GameState::PLAYING;
    }
    
    if (menu_button.Draw(Green, Light_Green, WHITE)) {
        reset_game();
        game_state = GameState::MENU;
    }
}

void handle_settings() {
    ClearBackground(Dark_Green);

    DrawText("Settings", screen_width / 2 - MeasureText("Settings", 60) / 2, screen_height / 4 - 30, 60, WHITE);
    if (back_button.Draw(Green, Light_Green, WHITE)) {
        game_state = GameState::MENU;
    }
}

int main() 
{
    // Setup window
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(fps);
    SetExitKey(0);
    window_open = true;

    // Load sounds
    InitAudioDevice();
    hit_sound = LoadSound("./assets/hit.wav");
    click_sound = LoadSound("./assets/click.ogg");
    hover_sound = LoadSound("./assets/hover.ogg");

    SetSoundVolume(hit_sound, 0.1f);
    SetSoundVolume(click_sound, 0.5f);
    SetSoundVolume(hover_sound, 0.5f);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        switch (game_state) {
            case GameState::MENU:
                handle_menu();
                break;
            case GameState::PLAYING:
                handle_playing();
                break;
            case GameState::PAUSED:
                handle_paused();
                break;
            case GameState::GAME_OVER:
                handle_game_over();
                break;
            case GameState::SETTINGS:
                handle_settings();
                break;
        }

        EndDrawing();
    }

    UnloadSound(hit_sound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}