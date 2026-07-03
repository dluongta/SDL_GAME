#include "ResultScreen.h"

ResultAction ResultScreen::Show(SDL_Surface* screen,
    TTF_Font* font,
    ResultType result,
    int score,
    std::string highScore)
{
    SDL_Event e;

    while (true)
    {
        // Nền trắng
        SDL_FillRect(
            screen,
            NULL,
            SDL_MapRGB(screen->format, 255, 255, 255));

        TextObject title;
        TextObject txtScore;
        TextObject txtHigh;
        TextObject replay;
        TextObject menu;

        // Chữ tĩnh màu đỏ
        title.SetColor(TextObject::RED_TEXT);
        txtScore.SetColor(TextObject::RED_TEXT);
        txtHigh.SetColor(TextObject::RED_TEXT);

        if (result == RESULT_WIN)
            title.SetText("YOU WIN!");
        else
            title.SetText("GAME OVER!");

        txtScore.SetText("Score : " + std::to_string(score));
        txtHigh.SetText("High Score : " + highScore);
        replay.SetText("Play Again");
        menu.SetText("Back Menu");

        title.SetRect(250, 80);
        txtScore.SetRect(250, 170);
        txtHigh.SetRect(250, 210);
        replay.SetRect(220, 320);
        menu.SetRect(420, 320);

        // --- XỬ LÝ ĐỔI MÀU KHI HOVER ---
        int mouse_x = 0;
        int mouse_y = 0;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        // Hover cho nút Play Again (Dải tọa độ 220-360 x 320-350)
        if (mouse_x >= 220 && mouse_x <= 360 && mouse_y >= 320 && mouse_y <= 350)
        {
            replay.SetColor(TextObject::PURPLE_TEXT);
        }
        else
        {
            replay.SetColor(TextObject::RED_TEXT);
        }

        // Hover cho nút Back Menu (Dải tọa độ 420-560 x 320-350)
        if (mouse_x >= 420 && mouse_x <= 560 && mouse_y >= 320 && mouse_y <= 350)
        {
            menu.SetColor(TextObject::PURPLE_TEXT);
        }
        else
        {
            menu.SetColor(TextObject::RED_TEXT);
        }

        title.CreatFontText(font, screen);
        txtScore.CreatFontText(font, screen);
        txtHigh.CreatFontText(font, screen);
        replay.CreatFontText(font, screen);
        menu.CreatFontText(font, screen);

        SDL_Flip(screen);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                exit(0);

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = e.button.x;
                int y = e.button.y;

                // Play Again
                if (x >= 220 && x <= 360 &&
                    y >= 320 && y <= 350)
                {
                    return ACTION_REPLAY;
                }

                // Back Menu
                if (x >= 420 && x <= 560 &&
                    y >= 320 && y <= 350)
                {
                    return ACTION_MENU;
                }
            }
        }

        SDL_Delay(16);
    }
}