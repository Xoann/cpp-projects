#include "button.h"
#include <raylib.h>
#include <iostream>

Button::Button(float x, float y, float width, float height, const char* text, int font_size, float roundedness, Sound* click_sound, Sound* hover_sound) : x(x), y(y), width(width), height(height), text(text), font_size(font_size), roundedness(roundedness), click_sound(click_sound), hover_sound(hover_sound), is_starting_hover(true) {}

bool Button::Draw(Color color, Color hover_color, Color text_color) {

    Vector2 mouse_pos = GetMousePosition();
    Color current_color = color;

    if (CheckCollisionPointRec(mouse_pos, Rectangle{x, y, width, height})) {
        current_color = hover_color;
        if (is_starting_hover) {
            PlaySound(*hover_sound);
            is_starting_hover = false;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(*click_sound);
            return true;
        }
    } else {
        is_starting_hover = true;
    }

    // Draw button frame
    DrawRectangleRounded(Rectangle{x, y, width, height}, roundedness, 0, current_color);

    int text_width = MeasureText(text, font_size);
    DrawText(text, x + (width - text_width) / 2, y + (height - font_size) /2, font_size, text_color);

    return false;
}