#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "theme.h"

#define FPS 60.0
#define GAME_PADDLE_WIDTH 125
#define GAME_PADDLE_HEIGHT 15
#define GAME_BALL_RADIUS 10
#define GAME_BALL_VELOCITY (2.0/(1.0/FPS))
#define GAME_PADDLE_VELOCITY (7.0/(1.0/FPS))


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

Game game_init();
void game_update_state(Game *game);

#ifdef CDONG_GAME_IMPLEMENTATION

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

void game_update_player_state(Game *game) {
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

void game_update_ball_state(Game *game) {
    const float dt = GetFrameTime();
    const int width = GetRenderWidth();
    const int height = GetRenderHeight();
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

void game_update_state(Game *game) {
    game_update_player_state(game);
    game_update_ball_state(game);
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

#endif // CDONG_GAME_IMPLEMENTATION
