#pragma once

#include <SDL3/SDL.h>
#include <functional>
#include <iostream>

class Button
{
public:
    Button(float x, float y, float w, float h, SDL_Color color, std::function<void()> onClick)
        : rect{ x, y, w, h }, color(color), onClick(onClick), isPressed(false) {
    }

    void Render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &rect);
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
