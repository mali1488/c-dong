
#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "common.h"
#include "theme.h"
#include <string.h>

#define CDONG_GAME_IMPLEMENTATION
#include "cdong_game.h"

#define CDONG_UI_IMPLEMENTATION
#include "cdong_ui.h"

static bool game_start = false;

void game_render_paddle(Rectangle paddle) {
    DrawRectangleRec(paddle, COLOR_FOREGROUND);
}

void game_render_ball(Vector2 position) {
    DrawCircle(position.x, position.y, GAME_BALL_RADIUS, COLOR_FOREGROUND);                              // Draw a color-filled circle
}

void game_render_score(int player_one, int player_two) {
    const int width = GetRenderWidth();
    const int height = GetRenderHeight();
    const int y = height/2 - 20;
    const int font_size = 40;
    const int margin = 10;

    char buff[128];
    int_to_char(player_one, buff);
    MeasureText(buff, font_size);
    DrawText(buff, 0 + margin, y - font_size, font_size, LIGHTGRAY);

    int_to_char(player_two, buff);
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

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(450, 800, GAME_TITLE);
    SetTargetFPS(FPS);
    
    while (!WindowShouldClose() && !game_start) {
        if (ui_start_menu() || IsKeyDown(KEY_S)) {
            game_start = true;
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
// - Holding button should make paddles go faster
// - New vector when ball collide with paddle
// - Make game size independent
