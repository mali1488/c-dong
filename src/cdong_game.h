#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "raylib.h"
#include "theme.h"

#define FPS 60.0
#define GAME_PADDLE_WIDTH_SCALE 0.2
#define GAME_PADDLE_HEIGHT 15
#define GAME_BALL_RADIUS_SCALE 0.015
#define GAME_BALL_VELOCITY (2.0/(1.0/FPS))
#define GAME_PADDLE_VELOCITY (7.0/(1.0/FPS))

typedef struct {
    Vector2 position;
    Vector2 velocity;
} Ball;

typedef struct {
    float x_position;
    int score;
} Player;

typedef struct {
    Player player_one;
    Player player_two;
    Ball ball;
} Game;

Game game_init();
void game_update_state(Game *game);
float game_ball_radius();
Rectangle paddle_rect(float x_pos, bool upper_player);

#ifdef CDONG_GAME_IMPLEMENTATION

void game_move_paddle_left(float* x_position, KeyboardKey speed_key) {
    const float dt = GetFrameTime();
    float vel = GAME_PADDLE_VELOCITY;
    if (IsKeyDown(speed_key)) {
        vel *= 2;
    }
    const int x = *x_position - dt * vel;
    *x_position = MAX(x, 0);
}

void game_move_paddle_right(float* x_position, KeyboardKey speed_key) {
    const int width = GetScreenWidth();
    const float dt = GetFrameTime();
    float vel = GAME_PADDLE_VELOCITY;
    if (IsKeyDown(speed_key)) {
        vel *= 2;
    }
    const int x = *x_position + dt * vel;
    *x_position = MIN(x, width - width * GAME_PADDLE_WIDTH_SCALE);
}

void game_update_player_state(Game *game) {
    if (IsKeyDown(KEY_LEFT)) {
        game_move_paddle_left(&game->player_one.x_position, KEY_RIGHT_SHIFT);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        game_move_paddle_right(&game->player_one.x_position, KEY_RIGHT_SHIFT);
    }
    if (IsKeyDown(KEY_A)) {
        game_move_paddle_left(&game->player_two.x_position, KEY_LEFT_SHIFT);
    }
    if (IsKeyDown(KEY_D)) {
        game_move_paddle_right(&game->player_two.x_position, KEY_LEFT_SHIFT);
    }
}

float game_ball_radius() {
    return GetScreenWidth() * GAME_BALL_RADIUS_SCALE;
}

Ball game_init_ball(bool up) {
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    Ball ball = {
        .velocity = {
            .x = 100,
            .y = up ? -GAME_BALL_VELOCITY : GAME_BALL_VELOCITY
        },
        .position = {
            .x = width / 2 - (game_ball_radius()) / 2,
            .y = height / 2 - (game_ball_radius()) / 2
        }
    };
    return ball;
}

Rectangle paddle_rect(float x_pos, bool upper_player) {
    const float height = GetScreenHeight();
    const float width = GetScreenWidth();
    const float paddle_height =  height * 0.01;
    Rectangle rect = {
        .x = x_pos,
        .y = upper_player ? 0 : height - paddle_height,
        .height = paddle_height,
        .width = width * GAME_PADDLE_WIDTH_SCALE
    };
    return rect;
}

void game_update_ball_state(Game *game) {
    const float dt = GetFrameTime();
    const int width = GetScreenWidth();
    const int height = GetScreenHeight();
    if (
        CheckCollisionCircleRec(game->ball.position, game_ball_radius(), paddle_rect(game->player_one.x_position, true)) ||
        CheckCollisionCircleRec(game->ball.position, game_ball_radius(), paddle_rect(game->player_two.x_position, false))
    ) {
        game->ball.velocity.y *= -1;
        if (fabs(game->ball.velocity.y) < 1000) { 
            game->ball.velocity.y *= 1.25;
        }
    }
    if (game->ball.position.x - game_ball_radius() < 0 || game->ball.position.x + game_ball_radius() > width) {
        game->ball.velocity.x *= -1;
    }
    if (game->ball.position.y - game_ball_radius() < 0) {
        game->player_two.score += 1;
        game->ball = game_init_ball(false);
    } else if (game->ball.position.y + game_ball_radius() > height) {
        game->player_one.score += 1;
        game->ball = game_init_ball(true);
    }
    game->ball.position.x += game->ball.velocity.x * dt;
    game->ball.position.y += game->ball.velocity.y * dt;
}

void game_update_state(Game *game) {
    game_update_player_state(game);
    game_update_ball_state(game);
}

Game game_init() {
    Game game = {0};
    int width = GetScreenWidth();
    game.ball = game_init_ball(false);

    const float player_x_pos = width / 2 - (width * GAME_PADDLE_WIDTH_SCALE) / 2;
    Player player_two = {
        .x_position = player_x_pos,
        .score = 0
    };
    Player player_one = {
        .x_position = player_x_pos,
        .score = 0
    };
    game.player_one = player_one;
    game.player_two = player_two;
    return game;
}

#endif // CDONG_GAME_IMPLEMENTATION
