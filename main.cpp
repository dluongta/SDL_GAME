#include "CommonFunction.h"
#include "Board.h"
#include "Board5x5.h"
#include "TextObject.h"
#include "Extension.h"
#include "ResultScreen.h"
#include <ctime>

#undef main

TTF_Font* g_font_text = NULL;
std::string highscore;
bool is_quit = false;

// Text objects
TextObject time_game;
TextObject score_game;
TextObject high_score;

bool Init()
{
    // Đã ép kiểu (unsigned int) để hết cảnh báo (warning)
    srand((unsigned int)time(NULL));

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;

    g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    SDL_WM_SetCaption("SDL Game", NULL);

    if (g_screen == NULL)
        return false;

    if (TTF_Init() == -1)
        return false;

    resetHighScore("high_score.txt");

    g_font_text = TTF_OpenFont("font/Raleway-Medium.ttf", 20);
    if (g_font_text == NULL)
        return false;

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
        return false;

    g_click = Mix_LoadWAV("audio/sound_mouse_click.wav");
    if (g_click == NULL)
        return false;

    return true;
}

// ===================== PLAY MODE 1 =====================
// Đã thêm tham số 'level' để bạn có thể tuỳ chỉnh độ khó của Board sau này
bool PlayMode1(int level)
{
    Board board;
    // TODO: Sau này bạn có thể sửa Board để nhận 'level' vào, ví dụ: Board board(level);

    Uint32 start_time = SDL_GetTicks();

    while (!is_quit)
    {
        SDLCommonFunc::ApplySurface(
            g_bkground, g_screen, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);

        while (SDL_PollEvent(&g_even))
        {
            if (g_even.type == SDL_QUIT)
            {
                is_quit = true;
                return true;
            }

            if (g_even.type == SDL_KEYDOWN &&
                g_even.key.keysym.sym == SDLK_ESCAPE)
            {
                return true;
            }
        }

        board.HandleInputAction(g_even);
        board.convert();
        board.Show_Board(g_screen);

        Uint32 time_val = TIME_PLAYING - (SDL_GetTicks() - start_time) / 1000;
        Uint32 score_val = board.score();

        // TIME
        time_game.SetText("Time: " + std::to_string(time_val));
        time_game.SetRect(WIDTH_BACKGROUND - 120, 10);
        time_game.CreatFontText(g_font_text, g_screen);

        // SCORE
        score_game.SetText("Filled: " + std::to_string(score_val) + "/16");
        score_game.SetRect(WIDTH_BACKGROUND - 120, 30);
        score_game.CreatFontText(g_font_text, g_screen);

        // HIGH SCORE (ONLY DISPLAY)
        high_score.SetText("High Score: " + highscore);
        high_score.SetRect(WIDTH_BACKGROUND - 140, 50);
        high_score.CreatFontText(g_font_text, g_screen);

        SDL_Flip(g_screen);

        // WIN
        if (board.CheckBoard())
        {
            updateHighScore("high_score.txt", score_val, highscore);
            highscore = GetHighScoreFromFile("high_score.txt");

            ResultScreen result;

            ResultAction action =
                result.Show(g_screen, g_font_text, RESULT_WIN, score_val, highscore);

            if (action == ACTION_REPLAY)
                return PlayMode1(level); // Truyền lại level nếu chơi lại

            return false;
        }

        // LOSE
        if (time_val == 0)
        {
            updateHighScore("high_score.txt", score_val, highscore);
            highscore = GetHighScoreFromFile("high_score.txt");

            ResultScreen result;

            ResultAction action =
                result.Show(g_screen, g_font_text, RESULT_LOSE, score_val, highscore);

            if (action == ACTION_REPLAY)
                return PlayMode1(level); // Truyền lại level nếu chơi lại

            return false;
        }

        SDL_Delay(70);
    }

    return false;
}

// ===================== PLAY MODE 2 =====================
// Đã thêm tham số 'level' để bạn có thể tuỳ chỉnh độ khó của Board5x5 sau này
bool PlayMode2(int level)
{
    Board5x5 board;
    // TODO: Sau này bạn có thể sửa Board5x5 để nhận 'level' vào, ví dụ: Board5x5 board(level);

    Uint32 start_time = SDL_GetTicks();

    while (!is_quit)
    {
        SDLCommonFunc::ApplySurface(
            g_bkground, g_screen, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);

        while (SDL_PollEvent(&g_even))
        {
            if (g_even.type == SDL_QUIT)
            {
                is_quit = true;
                return true;
            }

            if (g_even.type == SDL_KEYDOWN &&
                g_even.key.keysym.sym == SDLK_ESCAPE)
            {
                return true;
            }
        }

        board.HandleInputAction(g_even);
        board.convert();
        board.Show_Board(g_screen);

        Uint32 time_val = TIME_PLAYING - (SDL_GetTicks() - start_time) / 1000;
        Uint32 score_val = board.score();

        // TIME
        time_game.SetText("Time: " + std::to_string(time_val));
        time_game.SetRect(WIDTH_BACKGROUND - 120, 10);
        time_game.CreatFontText(g_font_text, g_screen);

        // SCORE
        score_game.SetText("Filled: " + std::to_string(score_val) + "/25");
        score_game.SetRect(WIDTH_BACKGROUND - 120, 30);
        score_game.CreatFontText(g_font_text, g_screen);

        // HIGH SCORE (ONLY DISPLAY)
        high_score.SetText("High Score: " + highscore);
        high_score.SetRect(WIDTH_BACKGROUND - 140, 50);
        high_score.CreatFontText(g_font_text, g_screen);

        SDL_Flip(g_screen);

        // WIN
        if (board.CheckBoard())
        {
            updateHighScore("high_score.txt", score_val, highscore);
            highscore = GetHighScoreFromFile("high_score.txt");

            ResultScreen result;

            ResultAction action =
                result.Show(g_screen, g_font_text, RESULT_WIN, score_val, highscore);

            if (action == ACTION_REPLAY)
                return PlayMode2(level);

            return false;
        }

        // LOSE
        if (time_val == 0)
        {
            updateHighScore("high_score.txt", score_val, highscore);
            highscore = GetHighScoreFromFile("high_score.txt");

            ResultScreen result;

            ResultAction action =
                result.Show(g_screen, g_font_text, RESULT_LOSE, score_val, highscore);

            if (action == ACTION_REPLAY)
                return PlayMode2(level);

            return false;
        }

        SDL_Delay(70);
    }

    return false;
}

// ===================== MAIN =====================
int main(int argc, char* argv[])
{
    if (!Init())
        return 0;

    g_bkground = SDLCommonFunc::LoadImage("image/background.png");
    if (g_bkground == NULL)
        return 0;

    while (true)
    {
        highscore = GetHighScoreFromFile("high_score.txt");

        // Biến lưu chế độ người chơi đã chọn (4x4 hoặc 5x5)
        int selected_mode = 0;

        // Gọi ShowMenu với 3 tham số. Trả về là Level (1, 2, 3) hoặc 0 (Thoát)
        int level = SDLCommonFunc::ShowMenu(g_screen, g_font_text, selected_mode);

        if (level == 0) // Người chơi chọn nút Exit
            break;

        bool is_done = false;

        // Xử lý chạy game theo Mode đã chọn, và có thể dùng 'level' để set độ khó
        if (selected_mode == 4)
            is_done = PlayMode1(level);
        else if (selected_mode == 5)
            is_done = PlayMode2(level);

        if (is_done)
            break;
    }

    SDLCommonFunc::CleanUp();
    SDL_Quit();

    return 0;
}