#include "PlayerEventHandling.hpp"
#include <SDL3/SDL.h>
#include "Player.hpp"
#include "Enemy.hpp"
#include <vector>
#include <memory>

void PlayerEventHandling(SDL_Event* event, Player* player, std::vector<std::unique_ptr<Enemy>>& enemies)
{
    static bool upPressed = false;
    static bool downPressed = false;
    static bool leftPressed = false;
    static bool rightPressed = false;
    static float lastDirX = 1.0f; // Domyœlny kierunek (np. w prawo)
    static float lastDirY = 0.0f;
    static bool isAttacking = false; // Flaga informuj¹ca o ataku

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
        case SDL_SCANCODE_SPACE:
            if (!isAttacking) // Upewnij siê, ¿e nie atakujesz ju¿
            {
                isAttacking = true;
                float dirX = (rightPressed - leftPressed) != 0 ? (rightPressed - leftPressed) : lastDirX;
                float dirY = (downPressed - upPressed) != 0 ? (downPressed - upPressed) : lastDirY;

                player->attack(enemies, dirX, dirY);
            }
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
        case SDL_SCANCODE_SPACE:
            isAttacking = false; // Zakoñcz atak
            break;
        default:
            break;
        }
    }

    // Ustalanie prêdkoœci
    float velocityX = (rightPressed - leftPressed) * 500.0f;
    float velocityY = (downPressed - upPressed) * 500.0f;

    // Aktualizacja kierunku ataku tylko jeœli gracz faktycznie siê porusza
    if (velocityX != 0 || velocityY != 0)
    {
        lastDirX = velocityX / 500.0f;
        lastDirY = velocityY / 500.0f;
    }

    player->SetVelocity(velocityX, velocityY);
}
