
#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "common.h"
#include "theme.h"

#define CDONG_UI_IMPLEMENTATION
#include "cdong_ui.h"

#define GAME_PADDLE_WIDTH 125
#define GAME_PADDLE_HEIGHT 15
#define GAME_BALL_RADIUS 10
#define GAME_BALL_VELOCITY (25/0.16)
#define GAME_PADDLE_VELOCITY (70/0.16)

typedef struct {
    Vector2 position;
    Vector2 velocity;
} Ball;

typedef struct {
    Rectangle paddle;
    int score;
} Player;

typedef struct {
    Player player_one;
    Player player_two;
    Ball ball;
} Game;

static bool gameStart = false;

void game_start() {
    gameStart = true;
}

Ball game_init_ball(bool up) {
    int width = GetRenderWidth();
    int height = GetRenderHeight();
    Ball ball = {
        .velocity = {
            .x = 100,
            .y = up ? -GAME_BALL_VELOCITY : GAME_BALL_VELOCITY
        },
        .position = {
            .x = width / 2 - GAME_BALL_RADIUS / 2,
            .y = height / 2 - GAME_BALL_RADIUS / 2
        }
    };
    return ball;
}

Game game_init() {
    Game game = {0};
    int width = GetRenderWidth();
    int height = GetRenderHeight();
    game.ball = game_init_ball(false);

    const float player_x_pos = width / 2 - GAME_PADDLE_WIDTH / 2;
    const int paddle_margin = 10;
    Player player_two = {
        .paddle = {
            .x = player_x_pos,
            .y = paddle_margin,
            .width = GAME_PADDLE_WIDTH,
            .height = GAME_PADDLE_HEIGHT
        },
        .score = 0
    };
    Player player_one = {
        .paddle = {
            .x = player_x_pos,
            .y = height - paddle_margin - GAME_PADDLE_HEIGHT,
            .width = GAME_PADDLE_WIDTH,
            .height = GAME_PADDLE_HEIGHT
        },
        .score = 0
    };
    game.player_one = player_one;
    game.player_two = player_two;
    return game;
}

void game_render_paddle(Rectangle paddle) {
    DrawRectangleRec(paddle, COLOR_FOREGROUND);
}

void game_render_ball(Vector2 position) {
    DrawCircle(position.x, position.y, GAME_BALL_RADIUS, COLOR_FOREGROUND);                              // Draw a color-filled circle
}

void game_render_score(int player_one, int player_two) {
    const int width = GetRenderWidth();
    const int height = GetRenderHeight();
    char buff[16];
    const int y = height/2 - 20;
    const int font_size = 40;
    const int margin = 10;

    sprintf(buff, "%d", player_one);
    MeasureText(buff, font_size);
    DrawText(buff, 0 + margin, y - font_size, font_size, LIGHTGRAY);

    sprintf(buff, "%d", player_two);
    const int w_2 = MeasureText(buff, font_size);
    DrawText(buff, width - w_2 - margin, y + font_size, font_size, LIGHTGRAY);
}

void game_render_playing_field() {
    const int y = GetRenderHeight() / 2;
    const int w = GetRenderWidth();
    const int line_width = 10;
    const int margin = 5;
    for (int i = 1; i <= w; i++) {
        const int start = (i - 1) * line_width;
        const int end = i * line_width;
        DrawLine(start, y, end - margin, y, LIGHTGRAY);
    }
}

void game_render(Game game) {
    BeginDrawing();
    {
        ClearBackground(COLOR_BACKGROUND);
        game_render_paddle(game.player_one.paddle);
        game_render_paddle(game.player_two.paddle);
        game_render_ball(game.ball.position);
        game_render_score(game.player_one.score, game.player_two.score);
        game_render_playing_field();
    }
    EndDrawing();
}

void game_move_paddle_left(Rectangle* rectangle) {
    const float dt = GetFrameTime();
    const int x = rectangle->x - dt * GAME_PADDLE_VELOCITY;
    rectangle->x = MAX(x, 0);
}

void game_move_paddle_right(Rectangle* rectangle) {
    const int width = GetRenderWidth();
    const float dt = GetFrameTime();
    const int x = rectangle->x + dt * GAME_PADDLE_VELOCITY;
    rectangle->x = MIN(x, width - GAME_PADDLE_WIDTH);
}

void game_update_state(Game *game) {
    const float dt = GetFrameTime();
    const int width = GetRenderWidth();
    const int height = GetRenderHeight();
    if (IsKeyDown(KEY_LEFT)) {
        game_move_paddle_left(&game->player_one.paddle);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        game_move_paddle_right(&game->player_one.paddle);
    }
    if (IsKeyDown(KEY_A)) {
        game_move_paddle_left(&game->player_two.paddle);
    }
    if (IsKeyDown(KEY_D)) {
        game_move_paddle_right(&game->player_two.paddle);
    }
    if (
        CheckCollisionCircleRec(game->ball.position, GAME_BALL_RADIUS, game->player_one.paddle) ||
        CheckCollisionCircleRec(game->ball.position, GAME_BALL_RADIUS, game->player_two.paddle)
    ) {
        game->ball.velocity.y *= -1 * 1.25;
    }
    if (game->ball.position.x - GAME_BALL_RADIUS < 0 || game->ball.position.x + GAME_BALL_RADIUS > width) {
        game->ball.velocity.x *= -1;
    }
    if (game->ball.position.y - GAME_BALL_RADIUS < 0) {
        game->player_two.score += 1;
        game->ball = game_init_ball(false);
    } else if (game->ball.position.y + GAME_BALL_RADIUS > height) {
        game->player_one.score += 1;
        game->ball = game_init_ball(true);
    }
    game->ball.position.x += game->ball.velocity.x * dt;
    game->ball.position.y += game->ball.velocity.y * dt;
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(450, 800, GAME_TITLE);
    SetTargetFPS(60);
    
    while (!WindowShouldClose() && !gameStart) {
        if (ui_start_menu() || IsKeyDown(KEY_S)) {
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

// TODO
// - Better controls (faster movement of paddels?
// - New vector when ball collide with paddle
// - Make game size independent
