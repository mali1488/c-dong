#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "theme.h"

bool ui_start_menu();

#ifdef CDONG_UI_IMPLEMENTATION

int _render_label(const char* text, int x, int y, int font_size, Color c) {
    const int width = MeasureText(text, font_size);
    DrawText(text, x - width / 2, y, font_size, c);
    return width;
}

bool render_start_button() {
    const int width = GetScreenWidth();
    const int height = GetScreenHeight();
    
    const int margin = 5;
    const char* msg = "Start";
    const float font_size = width * 0.05;
    const int msg_width = MeasureText(msg, font_size);
    const float rect_pos_y = height / 2 + margin;
    const float rect_pos_x = (width - msg_width) / 2;
    const padding = msg_width * 0.1;
    
    Rectangle button = {
        .x = rect_pos_x,
        .y = rect_pos_y,
        .width = msg_width + padding,
        .height = font_size + padding
    };
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = false;
    Color background_color = BLUE;
    Color text_color = WHITE;
    if (CheckCollisionPointRec(mousePos, button)) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            mousePressed = true;
        }
        background_color = BLACK;
        text_color = COLOR_FOREGROUND;
    }
    DrawRectangleRec(button, background_color);
    DrawText(msg, rect_pos_x + padding / 2, rect_pos_y + padding / 2, font_size, text_color);
    return mousePressed;
}

void render_game_title() {
    const int width = GetScreenWidth();
    const int height = GetScreenHeight();
    const int font_size = width * 0.1;
    const int w = MeasureText(GAME_TITLE, font_size);
    DrawText(GAME_TITLE, width / 2 - w / 2, height / 2 - font_size, font_size, COLOR_FOREGROUND);
}

bool ui_start_menu() {
    bool game_start = false;
    BeginDrawing();
    {
        ClearBackground(BLACK);
        render_game_title();
        if (render_start_button()) {
            game_start = true;
        }
    }
    EndDrawing();
    return game_start;
}

#endif // CDONG_UI_IMPLEMENTATION
