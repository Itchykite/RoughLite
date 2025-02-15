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

    SDL_Log("Handling player event...");
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
            upPressed = false;
            downPressed = false;
			leftPressed = false;
			rightPressed = false;
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

    if (upPressed && leftPressed)
    {
        velocityX = -1;
        velocityY = -1;
    }
    else if (upPressed && rightPressed)
    {
        velocityX = 1;
        velocityY = -1;
    }
    else if (downPressed && leftPressed)
    {
        velocityX = -1;
        velocityY = 1;
    }
    else if (downPressed && rightPressed)
    {
        velocityX = 1;
        velocityY = 1;
    }
    else if (upPressed)
    {
        velocityY = -1;
    }
    else if (downPressed)
    {
        velocityY = 1;
    }
    else if (leftPressed)
    {
        velocityX = -1;
    }
    else if (rightPressed)
    {
        velocityX = 1;
    }
    else
    {
        velocityX = 0;
        velocityY = 0;
    }

    player->SetVelocity(velocityX, velocityY);
}
