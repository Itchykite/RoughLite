#include "PlayerEventHandling.hpp"
#include <SDL3/SDL.h>
#include "Player.hpp"

void PlayerEventHandling(SDL_Event* event, Player* player)
{
    static bool upPressed = false;
    static bool downPressed = false;
    static bool leftPressed = false;
    static bool rightPressed = false;

    if (event == nullptr || player == nullptr)
    {
        SDL_Log("Invalid event or player pointer.");
        return;
    }

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch (event->key.scancode)
        {
        case SDL_SCANCODE_UP:
            upPressed = true;
            break;
        case SDL_SCANCODE_DOWN:
            downPressed = true;
            break;
        case SDL_SCANCODE_LEFT:
            leftPressed = true;
            break;
        case SDL_SCANCODE_RIGHT:
            rightPressed = true;
            break;
        default:
            break;
        }
    }
    else if (event->type == SDL_EVENT_KEY_UP)
    {
        switch (event->key.scancode)
        {
        case SDL_SCANCODE_UP:
            upPressed = false;
            break;
        case SDL_SCANCODE_DOWN:
            downPressed = false;
            break;
        case SDL_SCANCODE_LEFT:
            leftPressed = false;
            break;
        case SDL_SCANCODE_RIGHT:
            rightPressed = false;
            break;
        default:
            break;
        }
    }

    float velocityX = 0;
    float velocityY = 0;
    const float speed = 500;

    if (upPressed && leftPressed)
    {
        velocityX = -speed;
        velocityY = -speed;
    }
    else if (upPressed && rightPressed)
    {
        velocityX = speed;
        velocityY = -speed;
    }
    else if (downPressed && leftPressed)
    {
        velocityX = -speed;
        velocityY = speed;
    }
    else if (downPressed && rightPressed)
    {
        velocityX = speed;
        velocityY = speed;
    }
    else if (upPressed)
    {
        velocityY = -speed;
    }
    else if (downPressed)
    {
        velocityY = speed;
    }
    else if (leftPressed)
    {
        velocityX = -speed;
    }
    else if (rightPressed)
    {
        velocityX = speed;
    }
    else
    {
        velocityX = 0;
        velocityY = 0;
    }

    player->SetVelocity(velocityX, velocityY);
}
