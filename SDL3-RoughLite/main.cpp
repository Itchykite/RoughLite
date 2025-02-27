#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "Settings.hpp" // Postawowe ustwienia
#include "Player.hpp" // Gracz
#include "PlayerEventHandling.hpp" // Przechwytywanie przycisk�w
#include "Map.hpp" // Mapa
#include "Camera.hpp" // Kamera
#include "Enemy.hpp" // Przeciwnik
#include "EnemyManager.hpp" // Mened�er przeciwnik�w (Dodawnie konkretnej ilo�ci)
#include "RangeRover.hpp" // Kr�c�cy si� ziut
#include "OtherFunctions.hpp" // Inne funkcje
#include "GameState.hpp"
#include "Init.hpp"

#include <vector>
#include <memory>
#include <iostream>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Event lastEvent;

Map* map = nullptr; // wska�nik mapa
Camera* camera = nullptr; // wska�nik kamera
Player* player = nullptr; // wska�nik gracz
EnemyManager* enemyManager = nullptr; // wska�nik przeciwnik
EnemyManager* rangeRover = nullptr; // wska�nik kr�c�cy si� ziut
TTF_Font* font = nullptr; // wska�nik czcionka

Uint64 lastTime = 0;
Uint64 startTime = 0;

GameStateRunning gameState = GameStateRunning::MENU;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    InitEverything(renderer, window, player, map, camera, enemyManager, font, startTime, lastTime, appstate);

	return SDL_APP_CONTINUE;  /* continue running the program. */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    lastEvent = *event; // Zapisujemy event

    if (event->type == SDL_EVENT_QUIT)  // Zamkni�cie po naci�ni�ciu przycisku 'X'
    {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

	if (event->type == SDL_EVENT_KEY_DOWN) // Zamkni�cie po przyci�ni�ciu na klawiaturze ESCAPE
	{
		if (event->key.key == SDLK_ESCAPE)
		{
			return SDL_APP_SUCCESS;
		}
	}

    if (!player->isGameOver)
    {
        PlayerEventHandling(event, player, enemyManager->enemies, renderer); // Funkcja zawieraj�ca przechwytywanie WASD do poruszania si� gracza
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    if (gameState == GameStateRunning::MENU)
    {
        gameMenu(renderer, lastEvent);
    }

	if (gameState == GameStateRunning::GAME)
	{
		gameRunning(renderer, player, map, camera, enemyManager, startTime, lastTime, appstate);
	}

    if (gameState == GameStateRunning::GAMEOVER)
    {
		GameOver(renderer, font, player);
    }

	if (gameState == GameStateRunning::EXIT)
	{
		return SDL_APP_SUCCESS;
	}

    SDL_RenderPresent(renderer); // Renderowanie ca�o�ci

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    // Zwolnienie wska�nik�w

    SDL_Log("Cleaning up...");
    delete player;
    delete map;
    delete camera;
    delete enemyManager;
    delete rangeRover;
}