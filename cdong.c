#include <stdio.h>
#include <stdbool.h>
#include "deps/raylib.h"
#include "common.h"

#define GAME_PADDLE_WIDTH 125
#define GAME_PADDLE_HEIGHT 15
#define GAME_BALL_RADIUS 10
#define GAME_BALL_VELOCITY (20/0.16)
#define GAME_TITLE "C-Dong"
#define GAME_PADDLE_VELOCITY 2

#define COLOR_BACKGROUND BLACK
#define COLOR_FOREGROUND RAYWHITE

typedef struct {
    Rectangle player_one_paddle;
    Rectangle player_two_paddle;
    Vector2 ball_position;
    Vector2 ball_velocity;
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

void game_render_paddle(Rectangle paddle) {
    DrawRectangleRec(paddle, COLOR_FOREGROUND);
}

void game_render_ball(Vector2 position) {
    DrawCircle(position.x, position.y, GAME_BALL_RADIUS, COLOR_FOREGROUND);                              // Draw a color-filled circle
}

void game_render(Game game) {
    BeginDrawing();
    {
        ClearBackground(COLOR_BACKGROUND);
        game_render_paddle(game.player_one_paddle);
        game_render_paddle(game.player_two_paddle);
        game_render_ball(game.ball_position);
    }
    EndDrawing();
}

void game_move_paddle_left(Rectangle* rectangle) {
    const int x = rectangle->x - GAME_PADDLE_VELOCITY;
    rectangle->x = MAX(x, 0);
}

void game_move_paddle_right(Rectangle* rectangle) {
    const int width = GetRenderWidth();
    const int x = rectangle->x + GAME_PADDLE_VELOCITY;
    rectangle->x = MIN(x, width - GAME_PADDLE_WIDTH);
}

void game_update_state(Game *game) {
    const float dt = GetFrameTime();
    const int width = GetRenderWidth();
    const int height = GetRenderHeight();
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
    if (
        CheckCollisionCircleRec(game->ball_position, GAME_BALL_RADIUS, game->player_one_paddle) ||
        CheckCollisionCircleRec(game->ball_position, GAME_BALL_RADIUS, game->player_two_paddle)
    ) {
        game->ball_velocity.y *= -1 * 1.25;
    }
    if (game->ball_position.x - GAME_BALL_RADIUS < 0 || game->ball_position.x + GAME_BALL_RADIUS > width) {
        game->ball_velocity.x *= -1;
    }
    if (game->ball_position.y - GAME_BALL_RADIUS < 0 || game->ball_position.y + GAME_BALL_RADIUS > height) {
        game->ball_velocity.y *= -1;
    }
    game->ball_position.x += game->ball_velocity.x * dt;
    game->ball_position.y += game->ball_velocity.y * dt;
}

Game game_init() {
    Game game = {0};
    int width = GetRenderWidth();
    int height = GetRenderHeight();
    game.ball_velocity.x = 100;
    game.ball_velocity.y = -GAME_BALL_VELOCITY;
    game.ball_position.x = width / 2 - GAME_BALL_RADIUS / 2;
    game.ball_position.y = height / 2 - GAME_BALL_RADIUS / 2;

    const float player_x_pos = width / 2 - GAME_PADDLE_WIDTH / 2;
    const int paddle_margin = 10;
    Rectangle player_one_paddle = {
        .x = player_x_pos,
        .y = paddle_margin,
        .width = GAME_PADDLE_WIDTH,
        .height = GAME_PADDLE_HEIGHT
    };
    game.player_one_paddle = player_one_paddle;
     Rectangle player_two_paddle = {
        .x = player_x_pos,
        .y = height - paddle_margin - GAME_PADDLE_HEIGHT,
        .width = GAME_PADDLE_WIDTH,
        .height = GAME_PADDLE_HEIGHT
    };
    game.player_two_paddle = player_two_paddle;
    return game;
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
    Game game = game_init();
    while (!WindowShouldClose()) {
        game_update_state(&game);
        game_render(game);
    }
    CloseWindow();
    return 0;
}
