
#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "common.h"
#include "theme.h"

#define CDONG_GAME_IMPLEMENTATION
#include "cdong_game.h"

#define CDONG_UI_IMPLEMENTATION
#include "cdong_ui.h"

static bool game_start = false;

void game_render_paddle(float x_pos, bool upper_player) {
    Rectangle rect = paddle_rect(x_pos, upper_player);
    DrawRectangleRec(rect, COLOR_FOREGROUND);
}

void game_render_ball(Vector2 position) {
    DrawCircle(position.x, position.y, game_ball_radius(), COLOR_FOREGROUND);                              // Draw a color-filled circle
}

void game_render_score(int player_one, int player_two) {
    const float width = GetScreenWidth();
    const float height = GetScreenHeight();
    const float middle_y = height / 2;
    const float font_size = width * 0.1;
    const float padding = width * 0.03;

    char buff[128];
    int_to_char(player_one, buff);
    DrawText(buff, 0 + padding, middle_y - font_size - padding, font_size, LIGHTGRAY);

    int_to_char(player_two, buff);
    const float w2 = MeasureText(buff, font_size);
    DrawText(buff, width - w2 - padding, middle_y + padding, font_size, LIGHTGRAY);
}

void game_render_playing_field() {
    const int y = GetScreenHeight() / 2;
    const int w = GetScreenWidth();
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
        game_render_paddle(game.player_one.x_position, true);
        game_render_paddle(game.player_two.x_position, false);
        game_render_ball(game.ball.position);
        game_render_score(game.player_one.score, game.player_two.score);
        game_render_playing_field();
    }
    EndDrawing();
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(450, 800, GAME_TITLE);
    SetWindowMinSize(200, 400);
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
