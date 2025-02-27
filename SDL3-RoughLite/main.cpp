#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "Settings.hpp" // Postawowe ustwienia
#include "Player.hpp" // Gracz
#include "PlayerEventHandling.hpp" // Przechwytywanie przycisków
#include "Map.hpp" // Mapa
#include "Camera.hpp" // Kamera
#include "Enemy.hpp" // Przeciwnik
#include "EnemyManager.hpp" // Mened¿er przeciwników (Dodawnie konkretnej iloœci)
#include "RangeRover.hpp" // Krêc¹cy siê ziut
#include "OtherFunctions.hpp" // Inne funkcje
#include "GameState.hpp"
#include "Init.hpp"

#include <vector>
#include <memory>
#include <iostream>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Event lastEvent;

Map* map = nullptr; // wskaŸnik mapa
Camera* camera = nullptr; // wskaŸnik kamera
Player* player = nullptr; // wskaŸnik gracz
EnemyManager* enemyManager = nullptr; // wskaŸnik przeciwnik
EnemyManager* rangeRover = nullptr; // wskaŸnik krêc¹cy siê ziut
TTF_Font* font = nullptr; // wskaŸnik czcionka

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

    if (event->type == SDL_EVENT_QUIT)  // Zamkniêcie po naciœniêciu przycisku 'X'
    {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

	if (event->type == SDL_EVENT_KEY_DOWN) // Zamkniêcie po przyciœniêciu na klawiaturze ESCAPE
	{
		if (event->key.key == SDLK_ESCAPE)
		{
			return SDL_APP_SUCCESS;
		}
	}

    if (!player->isGameOver)
    {
        PlayerEventHandling(event, player, enemyManager->enemies, renderer); // Funkcja zawieraj¹ca przechwytywanie WASD do poruszania siê gracza
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

    SDL_RenderPresent(renderer); // Renderowanie ca³oœci

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    // Zwolnienie wskaŸników

    SDL_Log("Cleaning up...");
    delete player;
    delete map;
    delete camera;
    delete enemyManager;
    delete rangeRover;
}