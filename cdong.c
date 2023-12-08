#include <stdio.h>
#include <stdbool.h>
#include "deps/raylib.h"

#define PADDLE_WIDTH 125
#define PADDLE_HEIGHT 15
#define COLOR_BACKGROUND BLACK
#define COLOR_FOREGROUND RAYWHITE

typedef struct {
    int x;
} Paddle;

typedef struct {
    Paddle player_one_paddle;
    Paddle player_two_paddle;
} Game;

static bool gameStart = false;

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
            gameStart = true;
        }
        DrawRectangleRec(button, COLOR_FOREGROUND);
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

void render_menu() {
    BeginDrawing();
    {
        ClearBackground(BLACK);
        render_game_title();
        render_start_button();
    }
    EndDrawing();
}

void render_game(Game game) {
    int width = GetRenderWidth();
    int height = GetRenderHeight();
    const int paddle_margin = 10;

    BeginDrawing();
    {
        ClearBackground(COLOR_BACKGROUND);
        Rectangle player_one = {
            .x = game.player_one_paddle.x,
            .y = 0 + paddle_margin,
            .width = PADDLE_WIDTH,
            .height = PADDLE_HEIGHT
        };
        DrawRectangleRec(player_one, COLOR_FOREGROUND);
        Rectangle player_two = {
            .x = game.player_two_paddle.x, 
            .y = height - PADDLE_HEIGHT - paddle_margin, 
            .width = PADDLE_WIDTH, 
            .height = PADDLE_HEIGHT
        };
        DrawRectangleRec(player_two, COLOR_FOREGROUND);
    }
    EndDrawing();
}

void update_game_state(Game *game) {
    const int vel = 2;
    if (IsKeyDown(KEY_LEFT)) {
        game->player_one_paddle.x -= vel;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        game->player_one_paddle.x += vel;
    }
    if (IsKeyDown(KEY_A)) {
        game->player_two_paddle.x -= vel;
    }
    if (IsKeyDown(KEY_D)) {
        game->player_two_paddle.x += vel;
    }
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(450, 800, "CDong");
    SetTargetFPS(60);
    
    while (!WindowShouldClose() && !gameStart) {
        render_menu();
    }
    Game game = {0};
    while (!WindowShouldClose()) {
        update_game_state(&game);
        render_game(game);
    }
    CloseWindow();
    return 0;
}
