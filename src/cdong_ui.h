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

bool _render_start_button() {
    const int width = GetScreenWidth();
    const int height = GetScreenHeight();
    
    const int margin = 5;
    const int padding = 10;
    const char* msg = "Start";
    const int msgWidth = MeasureText(msg, 20);
    Rectangle button = {
        .x = (width - msgWidth - padding) / 2,
        .y = height / 2 + margin,
        .width = msgWidth + padding,
        .height = 20 + padding
    };
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = false;
    Color background_color = COLOR_FOREGROUND;
    Color text_color = BLACK;
    if (CheckCollisionPointRec(mousePos, button)) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            mousePressed = true;
        }
        background_color = BLACK;
        text_color = COLOR_FOREGROUND;
    }
    DrawRectangleRec(button, background_color);
    DrawText(msg, (width - msgWidth) / 2, (height + padding) / 2 + margin, 20, text_color);
    return mousePressed;
}

void _render_game_title() {
    const int width = GetScreenWidth();
    const int height = GetScreenHeight();
    const int font_size = 40;
    const int w = MeasureText(GAME_TITLE, font_size);
    DrawText(GAME_TITLE, width / 2 - w / 2, height / 2 - font_size, font_size, COLOR_FOREGROUND);
}

bool ui_start_menu() {
    bool game_start = false;
    BeginDrawing();
    {
        ClearBackground(BLACK);
        _render_game_title();
        if (_render_start_button()) {
            game_start = true;
        }
    }
    EndDrawing();
    return game_start;
}

#endif // CDONG_UI_IMPLEMENTATION
