#include <stdio.h>
#include "deps/raylib.h"



int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(450, 800, "CDong");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        int width = GetRenderWidth();
        int height = GetRenderHeight();
        {
            ClearBackground(BLACK);
            const char* msg = "CDong";
            int msgWidth = MeasureText(msg, 20);
            DrawText(msg, width/2 - msgWidth/2, height/2, 20, LIGHTGRAY);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}