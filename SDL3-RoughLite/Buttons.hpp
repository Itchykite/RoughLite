#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <iostream>

#include "GameState.hpp"

class Player;
class Map;
class EnemyManager;

class Button
{
public:
    Button(float x, float y, float w, float h, SDL_Color color, std::function<void()> onClick)
        : rect{ x, y, w, h }, color(color), onClick(onClick), isPressed(false) {
    }

    void Render(SDL_Renderer* renderer, TTF_Font* font, const char* name, Player* player, Map* map, EnemyManager* enemyManager)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &rect);

        SDL_Color textColor = { 255, 255, 255, 255 };

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, name, 0, textColor);
        if (!textSurface)
        {
            SDL_Log("Unable to render text surface: %s", SDL_GetError());
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!texture)
        {
            SDL_Log("Unable to create texture from rendered text: %s", SDL_GetError());
        }

        SDL_FRect dst;
        dst.w = textSurface->w;
        dst.h = textSurface->h;
        dst.x = rect.x + dst.w + dst.w / 2;
        dst.y = rect.y + dst.h - dst.h / 2;
        SDL_RenderTexture(renderer, texture, NULL, &dst);
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(textSurface);
    }

    void handleClick(const SDL_Event& event)
    {
        float mouseX = event.button.x;
        float mouseY = event.button.y;

        bool isInside = mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h;

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && isInside)
        {
            isPressed = true;
        }

        else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && isPressed && isInside)
        {
            isPressed = false;
            if (onClick)
            {
                onClick();
            }
        }
    }

private:
    SDL_FRect rect;
    SDL_Color color;
    std::function<void()> onClick;
    bool isPressed;
};
