#include "PlayerEventHandling.hpp"

void PlayerEventHandling(SDL_Event* event, Player* player)
{
	if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP)
	{
		float velocity = (event->type == SDL_EVENT_KEY_DOWN) ? .1f : 0.0f;

		if (event->key.key == SDLK_UP || event->key.key == SDLK_W)
		{

			player->SetVelocity(player->GetVelocityX(), -velocity);
		}

		else if (event->key.key == SDLK_DOWN || event->key.key == SDLK_S)
		{
			player->SetVelocity(player->GetVelocityX(), velocity);
		}

		else if (event->key.key == SDLK_LEFT || event->key.key == SDLK_A)
		{
			player->SetVelocity(-velocity, player->GetVelocityY());
		}

		else if (event->key.key == SDLK_RIGHT || event->key.key == SDLK_D)
		{
			player->SetVelocity(velocity, player->GetVelocityY());
		}
	}
}