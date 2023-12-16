#include "raylib.h"
#include "theme.h"
#include <stdbool.h>

bool ui_start_menu();

#ifdef CDONG_UI_IMPLEMENTATION

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

void _render_label(const char* text, int x, int y, int fontSize, Color c) {
    const int width = MeasureText(text, fontSize);
    DrawText(text, x - width / 2, y, fontSize, c);
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
