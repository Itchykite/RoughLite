#include "PlayerEventHandling.hpp"
#include <SDL3/SDL.h>
#include "Player.hpp"

void PlayerEventHandling(SDL_Event* event, Player* player) // Obs�uga zdarze� gracza
{
	static bool upPressed = false; // Czy wci�ni�to g�r�
	static bool downPressed = false; // Czy wci�ni�to d�
	static bool leftPressed = false; // Czy wci�ni�to lewo
	static bool rightPressed = false; // Czy wci�ni�to prawo

	if (event == nullptr || player == nullptr) // Je�li zdarzenie lub wska�nik gracza jest nieprawid�owy
    {
        SDL_Log("Invalid event or player pointer.");
        return;
    }

    if (event->type == SDL_EVENT_KEY_DOWN) // Sprawdzenie naci�ni�cia przycisku
    {
		switch (event->key.scancode) // Sprawdzenie skanu klawisza
        {
		case SDL_SCANCODE_UP: // Je�li wci�ni�to g�r�
            upPressed = true;
            break;
		case SDL_SCANCODE_DOWN: // Je�li wci�ni�to d�
            downPressed = true;
            break;
		case SDL_SCANCODE_LEFT: // Je�li wci�ni�to lewo
            leftPressed = true;
            break; 
		case SDL_SCANCODE_RIGHT: // Je�li wci�ni�to prawo
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
		case SDL_SCANCODE_UP: // Je�li wci�ni�to g�r�
            upPressed = false;
            break;
		case SDL_SCANCODE_DOWN: // Je�li wci�ni�to d�
            downPressed = false;
            break;
		case SDL_SCANCODE_LEFT: // Je�li wci�ni�to lewo
            leftPressed = false;
            break; 
		case SDL_SCANCODE_RIGHT: // Je�li wci�ni�to prawo
            rightPressed = false;
            break;
        default:
            break;
        }
    }

	float velocityX = 0; // Pr�dko�� x
	float velocityY = 0; // Pr�dko�� y
	const float speed = 500; // Pr�dko��


    //Obs�uga ruchu gracza
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

	player->SetVelocity(velocityX, velocityY); // Ustawienie pr�dko�ci gracza w podanym kierunku
}
