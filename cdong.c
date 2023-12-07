#include <stdio.h>
#include "deps/raylib.h"

void render_start_button() {
    int width = GetRenderWidth();
    int height = GetRenderHeight();
    
    int margin = 5;
    int padding = 10;
    const char* msg = "Start";
    int msgWidth = MeasureText(msg, 20);
    Rectangle button = {
        .x = (width - msgWidth - padding) / 2,
        .y = height / 2 + margin,
        .width = msgWidth + padding,
        .height = 20 + padding
    };
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, button)) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            printf("Start game!\n");
        }
        DrawRectangleRec(button, RAYWHITE);
    } else {
        DrawRectangleRec(button, GRAY);
    }
    DrawText(msg, (width - msgWidth) / 2, (height + padding) / 2 + margin, 20, LIGHTGRAY);
}

void render_label(const char* text, int x, int y, int fontSize, Color c) {
    int width = MeasureText(text, fontSize);
    DrawText(text, x - width / 2, y, fontSize, c);
}

void render_game_title() {
    int width = GetRenderWidth();
    int height = GetRenderHeight();
    render_label("C-Dong", width/2, height/2 - 40, 40, LIGHTGRAY);
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(450, 800, "CDong");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);
            render_game_title();
            render_start_button();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
