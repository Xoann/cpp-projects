#pragma once

#include "raylib.h"

class Button {
  public: 
    Button(float x, float y, float width, float height, const char* text, int font_size, float roundedness, Sound* click_sound, Sound* hover_sound);
    bool Draw(Color color, Color hover_color, Color text_color);

    float x, y;
    float width, height;
    const char* text;
    int font_size;
    float roundedness;
    Sound* click_sound;
    Sound* hover_sound;

  private:
    mutable bool is_starting_hover;
};;