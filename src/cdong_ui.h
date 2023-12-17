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
    const int width = GetRenderWidth();
    const int height = GetRenderHeight();
    
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
    if (CheckCollisionPointRec(mousePos, button)) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            mousePressed = true;
        }
        DrawRectangleRec(button, COLOR_FOREGROUND);
    } else {
        DrawRectangleRec(button, GRAY);
    }
    DrawText(msg, (width - msgWidth) / 2, (height + padding) / 2 + margin, 20, LIGHTGRAY);
    return mousePressed;
}

void _render_game_title() {
    const int width = GetRenderWidth();
    const int height = GetRenderHeight();
    _render_label(GAME_TITLE, width/2, height/2 - 40, 40, LIGHTGRAY);
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
