#include <stdio.h>
#include <stdbool.h>
#include "deps/raylib.h"
#include "common.h"

#define GAME_PADDLE_WIDTH 125
#define GAME_PADDLE_HEIGHT 15
#define GAME_BALL_RADIUS 10
#define GAME_TITLE "C-Dong"
#define COLOR_BACKGROUND BLACK
#define COLOR_FOREGROUND RAYWHITE
#define GAME_PADDLE_VELOCITY 2

typedef struct {
    int x;
} Paddle;

typedef struct {
    Paddle player_one_paddle;
    Paddle player_two_paddle;
} Game;

static bool gameStart = false;

void game_start() {
    gameStart = true;
}

void render_start_button() {
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
    if (CheckCollisionPointRec(mousePos, button)) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            game_start();
        }
        DrawRectangleRec(button, COLOR_FOREGROUND);
    } else {
        DrawRectangleRec(button, GRAY);
    }
    DrawText(msg, (width - msgWidth) / 2, (height + padding) / 2 + margin, 20, LIGHTGRAY);
}

void render_label(const char* text, int x, int y, int fontSize, Color c) {
    const int width = MeasureText(text, fontSize);
    DrawText(text, x - width / 2, y, fontSize, c);
}

void render_game_title() {
    const int width = GetRenderWidth();
    const int height = GetRenderHeight();
    render_label(GAME_TITLE, width/2, height/2 - 40, 40, LIGHTGRAY);
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

void game_render_paddle(const int x, const int y) {
    Rectangle paddle_rectangle = {
        .x = x,
        .y = y,
        .width = GAME_PADDLE_WIDTH,
        .height = GAME_PADDLE_HEIGHT
    };
    DrawRectangleRec(paddle_rectangle, COLOR_FOREGROUND);
}

void game_render_ball() {
    DrawCircle(50, 50, GAME_BALL_RADIUS, COLOR_FOREGROUND);                              // Draw a color-filled circle
}

void game_render(Game game) {
    const int height = GetRenderHeight();
    const int paddle_margin = 10;

    BeginDrawing();
    {
        ClearBackground(COLOR_BACKGROUND);
        game_render_paddle(game.player_one_paddle.x, 0 + paddle_margin);
        game_render_paddle(game.player_two_paddle.x, height - GAME_PADDLE_HEIGHT - paddle_margin);
        game_render_ball();
    }
    EndDrawing();
}

void game_move_paddle_left(Paddle* paddle) {
    const int x = paddle->x - GAME_PADDLE_VELOCITY;
    paddle->x = MAX(x, 0);
}

void game_move_paddle_right(Paddle* paddle) {
    const int width = GetRenderWidth();
    const int x = paddle->x + GAME_PADDLE_VELOCITY;
    paddle->x = MIN(x, width - GAME_PADDLE_WIDTH);
}

void game_update_state(Game *game) {
    if (IsKeyDown(KEY_LEFT)) {
        game_move_paddle_left(&game->player_one_paddle);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        game_move_paddle_right(&game->player_one_paddle);
    }
    if (IsKeyDown(KEY_A)) {
        game_move_paddle_left(&game->player_two_paddle);
    }
    if (IsKeyDown(KEY_D)) {
        game_move_paddle_right(&game->player_two_paddle);
    }
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(450, 800, GAME_TITLE);
    SetTargetFPS(60);
    
    while (!WindowShouldClose() && !gameStart) {
        render_menu();
        if (IsKeyDown(KEY_S)) {
            game_start();
        }
    }
    Game game = {0};
    while (!WindowShouldClose()) {
        game_update_state(&game);
        game_render(game);
    }
    CloseWindow();
    return 0;
}
