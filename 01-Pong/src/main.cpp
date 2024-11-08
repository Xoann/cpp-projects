#include <raylib.h>
#include "ball.h"
#include "paddle.h"
#include "button.h"
#include <iostream>
#include <vector>
#include "chaos_items.h"

// TODO: Create temp ball extention to game ball
// TODO: make cpu paddle follow closest ball
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
std::vector<GameBall*> balls;
GameBall ball(screen_width / 2, screen_height / 2, ball_speed, ball_speed, ball_radius);

Paddle player(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed);
CpuPaddle cpu(screen_width - paddle_width - paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed);

Ball menu_ball(screen_width / 2, screen_height / 2, ball_speed, ball_speed, ball_radius);

// Sounds
Sound hit_sound;
Sound click_sound;
Sound hover_sound;
Sound game_leave_sound;
Sound game_start_sound;
Sound lose_sound;

// Buttons
Button back_button(20, 20, button_width / 2, button_height / 2, "Back", button_font_size / 2, button_roundedness, &click_sound, &hover_sound);

// Menu buttons
Button play_button(screen_width / 2 - 100, screen_height / 2 - 50, button_width, button_height, "Play", button_font_size, button_roundedness, &click_sound, &hover_sound);
Button settings_button(screen_width / 2 - 100, screen_height / 2 + 80, button_width, button_height, "Settings", button_font_size, button_roundedness, &click_sound, &hover_sound);

// Game over buttons
Button replay_button(screen_width / 2 - 100, screen_height / 2 - 50, button_width, button_height, "Replay", button_font_size, button_roundedness, &game_start_sound, &hover_sound);
Button menu_button(screen_width / 2 - 100, screen_height / 2 + 80, button_width, button_height, "Menu", button_font_size, button_roundedness, &click_sound, &hover_sound);

// Pause buttons
Button pause_menu_button(screen_width / 2 - 40, screen_height / 2 + 80, 80, 40, "Menu", 20, button_roundedness, &click_sound, &hover_sound);

// Mode select buttons
Button classic_mode_button(screen_width / 2 - 100, screen_height / 3, button_width, button_height, "Classic", button_font_size, button_roundedness, &game_start_sound, &hover_sound);
Button arcade_mode_button(screen_width / 2 - 100, screen_height / 3 + 125, button_width, button_height, "Arcade", button_font_size, button_roundedness, &game_start_sound, &hover_sound);

bool window_open;

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    SETTINGS,
    MODE_SELECT
};

enum class GameMode {
    CLASSIC,
    ARCADE
};

GameState game_state = GameState::MENU;
GameMode game_mode = GameMode::CLASSIC;

// Chaos items
std::vector<ChaosItem> chaos_items;
float chaos_item_spawn_timer = 0.0f;
float chaos_item_spawn_interval = 3.0f;

void reset_game() {
    balls.clear();
    balls.push_back(&ball);
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

    // Game objects
    for (const ChaosItem& chaos_item : chaos_items) {
        chaos_item.Draw();
    }

    for (GameBall* game_ball : balls) {
        game_ball->Draw();
    }

    player.Draw();
    cpu.Draw();
}

int handle_playing() {
    // Check for state changes
    if (IsKeyPressed(KEY_ESCAPE)) {
        game_state = GameState::PAUSED;
        EndDrawing();
    }

    if (player_score == max_score || cpu_score == max_score) {
        if (cpu_score == max_score) {
            PlaySound(lose_sound);
        } else {}
        game_state = GameState::GAME_OVER;
    }

    // Spawn chaos items
    chaos_item_spawn_timer += GetFrameTime();

    if (chaos_item_spawn_timer >= chaos_item_spawn_interval) {
        chaos_item_spawn_timer = 0.0f;

        Vector2 random_position = Vector2{(float)GetRandomValue(40, screen_width - 40), (float)GetRandomValue(40, screen_height - 40)};

        int random_type = GetRandomValue(0, 2);

        chaos_items.emplace_back(random_type, random_position);
    }

    // Update
    for (std::vector<GameBall*>::size_type i = 0; i < balls.size(); i++) {
        (*balls[i]).Update(&player_score, &cpu_score);
    }

    player.Update();
    cpu.Update(ball.y);

    for (GameBall* game_ball : balls) {
        if (CheckCollisionCircleRec(Vector2{game_ball->x, game_ball->y}, game_ball->radius, Rectangle{(float)player.x, (float)player.y, (float)player.width, (float)player.height})) {
            // Reverse horizontal direction
            game_ball->speed_x *= -1;

            // Bounce vertically depending on where the ball hits the paddle
            if (game_ball->y < player.y) {
                game_ball->speed_y = -abs(game_ball->speed_y);
            } else if (game_ball->y > player.y + player.height) {
                game_ball->speed_y = abs(game_ball->speed_y);
            }

            PlaySound(hit_sound);
        }

        if (CheckCollisionCircleRec(Vector2{game_ball->x, game_ball->y}, game_ball->radius, Rectangle{(float)cpu.x, (float)cpu.y, (float)cpu.width, (float)cpu.height})) {
            // Reverse horizontal direction
            game_ball->speed_x *= -1;

            // Bounce vertically depending on where the ball hits the paddle
            if (game_ball->y < cpu.y) {
                game_ball->speed_y = -abs(game_ball->speed_y);
            } else if (game_ball->y > cpu.y + cpu.height) {
                game_ball->speed_y = abs(game_ball->speed_y);
            }

            PlaySound(hit_sound);
        }
    }
    // // Check for collisions
    // if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{(float)player.x, (float)player.y, (float)player.width, (float)player.height})) {
    //     // Reverse horizontal direction
    //     ball.speed_x *= -1;

    //     // Bounce vertically depending on where the ball hits the paddle
    //     if (ball.y < player.y) {
    //         ball.speed_y = -abs(ball.speed_y);
    //     } else if (ball.y > player.y + player.height) {
    //         ball.speed_y = abs(ball.speed_y);
    //     }

    //     PlaySound(hit_sound);
    // }

    // if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{(float)cpu.x, (float)cpu.y, (float)cpu.width, (float)cpu.height})) {
    //         // Reverse horizontal direction
    //     ball.speed_x *= -1;

    //     // Bounce vertically depending on where the ball hits the paddle
    //     if (ball.y < cpu.y) {
    //         ball.speed_y = -abs(ball.speed_y);
    //     } else if (ball.y > cpu.y + cpu.height) {
    //         ball.speed_y = abs(ball.speed_y);
    //     }

    //     PlaySound(hit_sound);
    // }

    // Check for chaos item collisions
    for (std::vector<ChaosItem>::size_type i = 0; i < chaos_items.size(); i++) {
        if (CheckCollisionCircles(Vector2{ball.x, ball.y}, ball.radius, chaos_items[i].position, chaos_items[i].radius)) {
            chaos_items[i].ApplyEffect(&player, &cpu, &ball, balls);
            chaos_items.erase(chaos_items.begin() + i);
        }
    }

    // Draw
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
        game_state = GameState::MODE_SELECT;
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
        PlaySound(game_leave_sound);
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

void handle_mode_select() {
    ClearBackground(Dark_Green);

    DrawText("Select Mode", screen_width / 2 - MeasureText("Select Mode", 60) / 2, screen_height / 4 - 30, 60, WHITE);
    if (back_button.Draw(Green, Light_Green, WHITE)) {
        game_state = GameState::MENU;
    }

    if (classic_mode_button.Draw(Green, Light_Green, YELLOW)) {
        game_mode = GameMode::CLASSIC;
        game_state = GameState::PLAYING;
    }

    if (arcade_mode_button.Draw(Green, Light_Green, YELLOW)) {
        game_mode = GameMode::ARCADE;
        game_state = GameState::PLAYING;
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
    game_leave_sound = LoadSound("./assets/game_leave.ogg");
    game_start_sound = LoadSound("./assets/game_start.ogg");
    lose_sound = LoadSound("./assets/lose.ogg");

    SetSoundVolume(hit_sound, 0.1f);
    SetSoundVolume(click_sound, 0.5f);
    SetSoundVolume(hover_sound, 0.5f);
    SetSoundVolume(game_leave_sound, 0.2f);
    SetSoundVolume(game_start_sound, 0.2f);
    SetSoundVolume(lose_sound, 0.5f);

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
            case GameState::MODE_SELECT:
                handle_mode_select();
                break;
        }

        EndDrawing();
    }

    UnloadSound(hit_sound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}