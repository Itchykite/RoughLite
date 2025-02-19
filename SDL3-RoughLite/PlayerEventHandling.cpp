#include "PlayerEventHandling.hpp"
#include <SDL3/SDL.h>
#include "Player.hpp"

void PlayerEventHandling(SDL_Event* event, Player* player) // Obs³uga zdarzeñ gracza
{
	static bool upPressed = false; // Czy wciœniêto górê
	static bool downPressed = false; // Czy wciœniêto dó³
	static bool leftPressed = false; // Czy wciœniêto lewo
	static bool rightPressed = false; // Czy wciœniêto prawo

	if (event == nullptr || player == nullptr) // Jeœli zdarzenie lub wskaŸnik gracza jest nieprawid³owy
    {
        SDL_Log("Invalid event or player pointer.");
        return;
    }

    if (event->type == SDL_EVENT_KEY_DOWN) // Sprawdzenie naciœniêcia przycisku
    {
		switch (event->key.scancode) // Sprawdzenie skanu klawisza
        {
		case SDL_SCANCODE_UP: // Jeœli wciœniêto górê
            upPressed = true;
            break;
		case SDL_SCANCODE_DOWN: // Jeœli wciœniêto dó³
            downPressed = true;
            break;
		case SDL_SCANCODE_LEFT: // Jeœli wciœniêto lewo
            leftPressed = true;
            break; 
		case SDL_SCANCODE_RIGHT: // Jeœli wciœniêto prawo
            rightPressed = true;
            break;
        default:
            break;
        }
    }
	else if (event->type == SDL_EVENT_KEY_UP) // Sprawdzenie puszczenia przycisku
    {
		switch (event->key.scancode) // Sprawdzenie skanu klawisza
        {
		case SDL_SCANCODE_UP: // Jeœli wciœniêto górê
            upPressed = false;
            break;
		case SDL_SCANCODE_DOWN: // Jeœli wciœniêto dó³
            downPressed = false;
            break;
		case SDL_SCANCODE_LEFT: // Jeœli wciœniêto lewo
            leftPressed = false;
            break; 
		case SDL_SCANCODE_RIGHT: // Jeœli wciœniêto prawo
            rightPressed = false;
            break;
        default:
            break;
        }
    }

	float velocityX = 0; // Prêdkoœæ x
	float velocityY = 0; // Prêdkoœæ y
	const float speed = 500; // Prêdkoœæ


    //Obs³uga ruchu gracza
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

	player->SetVelocity(velocityX, velocityY); // Ustawienie prêdkoœci gracza w podanym kierunku
}
