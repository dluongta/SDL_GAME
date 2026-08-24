#include "CommonFunction.h"
#include "TextObject.h"

SDL_Surface* SDLCommonFunc::LoadImage(std::string file_path)
{
    SDL_Surface* load_image = NULL;
    SDL_Surface* optimize_image = NULL;
    load_image = IMG_Load(file_path.c_str());
    if (load_image != NULL)
    {
        optimize_image = SDL_DisplayFormat(load_image);
        SDL_FreeSurface(load_image);
    }
    return optimize_image;
}

SDL_Rect SDLCommonFunc::ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y, int w, int h)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    offset.w = w;
    offset.h = h;
    SDL_BlitSurface(src, NULL, des, &offset);
    return offset;
}

void SDLCommonFunc::CleanUp()
{
    SDL_FreeSurface(g_screen);
    SDL_FreeSurface(g_bkground);
    SDL_FreeSurface(g_menu);
    Mix_FreeChunk(g_click);
}

bool SDLCommonFunc::CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect)
{
    if (x > rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
    {
        return true;
    }
    return false;
}

int SDLCommonFunc::ShowLevelMenu(SDL_Surface* des, TTF_Font* font, int mode)
{
    const int LevelItemNum = 4;
    TextObject text_level[LevelItemNum];
    TextObject title;

    title.SetColor(TextObject::RED_TEXT);
    if (mode == 0)
        title.SetText("4x4 Mode - Select Level");
    else
        title.SetText("5x5 Mode - Select Level");

    title.SetRect(180, 100);

    text_level[0].SetText("Level 1");
    text_level[1].SetText("Level 2");
    text_level[2].SetText("Level 3");
    text_level[3].SetText("Back Menu");

    for (int i = 0; i < LevelItemNum; i++)
    {
        text_level[i].SetColor(TextObject::RED_TEXT);
        text_level[i].SetRect(250, 180 + i * 50);
    }

    bool selected[LevelItemNum] = { 0, 0, 0, 0 };
    int xm = 0, ym = 0;
    SDL_Event l_event;

    while (true)
    {
        SDL_FillRect(des, NULL, SDL_MapRGB(des->format, 255, 255, 255));

        title.CreatFontText(font, des);
        for (int i = 0; i < LevelItemNum; i++)
        {
            text_level[i].CreatFontText(font, des);
        }

        while (SDL_PollEvent(&l_event))
        {
            switch (l_event.type)
            {
            case SDL_QUIT:
                return -1;
            case SDL_MOUSEMOTION:
            {
                xm = l_event.motion.x;
                ym = l_event.motion.y;
                for (int i = 0; i < LevelItemNum; i++)
                {
                    if (CheckFocusWithRect(xm, ym, text_level[i].GetRect()))
                    {
                        if (selected[i] == 0)
                        {
                            selected[i] = 1;
                            text_level[i].SetColor(TextObject::PURPLE_TEXT);
                        }
                    }
                    else
                    {
                        if (selected[i] == 1)
                        {
                            selected[i] = 0;
                            text_level[i].SetColor(TextObject::RED_TEXT);
                        }
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                xm = l_event.motion.x;
                ym = l_event.motion.y;
                for (int i = 0; i < LevelItemNum; i++)
                {
                    if (CheckFocusWithRect(xm, ym, text_level[i].GetRect()))
                    {
                        return i;
                    }
                }
            }
            break;
            case SDL_KEYDOWN:
                if (l_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return -1;
                }
                break;
            }
        }
        SDL_Flip(des);
        SDL_Delay(16);
    }
}

int SDLCommonFunc::ShowMenu(SDL_Surface* des, TTF_Font* font, int& out_mode)
{
    g_menu = LoadImage("image/background2d.png");
    if (g_menu == NULL)
    {
        return 0;
    }

    const int MenuItemNum = 3;
    SDL_Rect pos_arr[MenuItemNum];
    pos_arr[0].x = 250;
    pos_arr[0].y = 200;
    pos_arr[1].x = 250;
    pos_arr[1].y = 250;
    pos_arr[2].x = 250;
    pos_arr[2].y = 300;

    TextObject text_menu[MenuItemNum];
    text_menu[0].SetText("Play 4x4 Mode");
    text_menu[0].SetColor(TextObject::RED_TEXT);
    text_menu[0].SetRect(pos_arr[0].x, pos_arr[0].y);

    text_menu[1].SetText("Play 5x5 Mode");
    text_menu[1].SetColor(TextObject::RED_TEXT);
    text_menu[1].SetRect(pos_arr[1].x, pos_arr[1].y);

    text_menu[2].SetText("Exit");
    text_menu[2].SetColor(TextObject::RED_TEXT);
    text_menu[2].SetRect(pos_arr[2].x, pos_arr[2].y);

    bool selected[MenuItemNum] = { 0, 0, 0 };
    int xm = 0;
    int ym = 0;
    SDL_Event m_event;

    while (true)
    {
        SDLCommonFunc::ApplySurface(g_menu, des, 0, 0, 800, 600);

        for (int i = 0; i < MenuItemNum; i++)
        {
            text_menu[i].CreatFontText(font, des);
        }

        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 0;
            case SDL_MOUSEMOTION:
            {
                xm = m_event.motion.x;
                ym = m_event.motion.y;
                for (int i = 0; i < MenuItemNum; i++)
                {
                    if (CheckFocusWithRect(xm, ym, text_menu[i].GetRect()))
                    {
                        if (selected[i] == 0)
                        {
                            selected[i] = 1;
                            text_menu[i].SetColor(TextObject::PURPLE_TEXT);
                        }
                    }
                    else
                    {
                        if (selected[i] == 1)
                        {
                            selected[i] = 0;
                            text_menu[i].SetColor(TextObject::RED_TEXT);
                        }
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                xm = m_event.motion.x;
                ym = m_event.motion.y;
                for (int i = 0; i < MenuItemNum; i++)
                {
                    if (CheckFocusWithRect(xm, ym, text_menu[i].GetRect()))
                    {
                        if (i == 0 || i == 1)
                        {
                            int level_choice = ShowLevelMenu(des, font, i);

                            if (level_choice == -1)
                            {
                                exit(0);
                            }
                            else if (level_choice == 3)
                            {
                                break;
                            }
                            else if (level_choice >= 0 && level_choice <= 2)
                            {
                                out_mode = (i == 0) ? 4 : 5;
                                return level_choice + 1;
                            }
                        }
                        else if (i == 2)
                        {
                            return 0;
                        }
                    }
                }
            }
            break;
            case SDL_KEYDOWN:
                if (m_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    exit(0);
                }
                break;
            default:
                break;
            }
        }
        SDL_Flip(des);
        SDL_Delay(16);
    }
    return 0;
}