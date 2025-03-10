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
TTF_Font* font = nullptr; // wskaŸnik czcionka

Uint64 lastTime = 0;
Uint64 startTime = 0;

GameStateRunning gameState = GameStateRunning::MENU;
GameStateRunning previousState = gameState; // Dodanie zmiennej previousState

void resetLastTime() 
{
    lastTime = SDL_GetTicks();
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    InitEverything(renderer, window, player, map, camera, enemyManager, font, startTime, lastTime, appstate);
    loadPlayerStats(player);
    loadGameTime(player);

    for (auto& res : availableResolutions)
    {
		SDL_Log("Resolution: %dx%d", res.width, res.height);
    }

	return SDL_APP_CONTINUE;  /* continue running the program. */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    lastEvent = *event; // Zapisujemy event

    if (event->type == SDL_EVENT_QUIT)  // Zamkniêcie po naciœniêciu przycisku 'X'
    {
        savePlayerStats(player);
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

	if (event->type == SDL_EVENT_KEY_DOWN) // Zamkniêcie po przyciœniêciu na klawiaturze ESCAPE
	{
        if (event->key.key == SDLK_ESCAPE)
        {
            if (gameState != GameStateRunning::MENU)
            {
                gameState = GameStateRunning::MENU;
            }
            else if (gameState != GameStateRunning::GAME)
            {
                gameState = GameStateRunning::GAME;
                SDL_Log("Wznowiono gre (GAME)");
                resetLastTime(); // Resetowanie lastTime przy przejœciu do GAME
            }
        }

        if (event->key.key == SDLK_F11) // Prze³¹czanie trybu pe³noekranowego za pomoc¹ F11
        {
			SDL_Log("F11 pressed");
            isFullscreen = !isFullscreen;
            if (isFullscreen)
            {
                SDL_SetWindowFullscreen(window, SDL_GetWindowFullscreenMode);
            }
            else
            {
                SDL_SetWindowFullscreen(window, 0); // Powrót do trybu okienkowego
            }

            SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH);
        }
	}

    if (gameState == GameStateRunning::GAME && !player->isGameOver)
    {
        PlayerEventHandling(event, player, enemyManager->enemies, renderer); // Funkcja zawieraj¹ca przechwytywanie WASD do poruszania siê gracza
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    Uint32 frameStart = SDL_GetTicks();

    if (gameState != previousState)
    {
        if (gameState == GameStateRunning::GAME)
        {
            resetLastTime(); // Resetowanie lastTime przy przejœciu do GAME
        }
        previousState = gameState;
    }

    switch (gameState)
    {
    case GameStateRunning::GAME:
        gameRunning(renderer, player, map, camera, enemyManager, startTime, lastTime, lastEvent, font, appstate, gameState);
        break;

    case GameStateRunning::MENU:
        gameMenu(renderer, lastEvent, font, player, map, enemyManager);
        break;

    case GameStateRunning::GAMEOVER:
        GameOver(renderer, font, player, lastTime, startTime);
        savePlayerStats(player);
        break;

    case GameStateRunning::STATS:
        gameStats(renderer, font, player);
        break;

    case GameStateRunning::SETTINGS:
        gameSettings(renderer, lastEvent, font, gameState, window, camera, player);
        break;

    case GameStateRunning::LEVELUP:
        levelUp(renderer, font, player, lastEvent, gameState, enemyManager);
        break;

    case GameStateRunning::EXIT:
        saveGameTime(player, startTime);
        savePlayerStats(player);
        return SDL_APP_SUCCESS;
        break;

    default:
        return SDL_APP_FAILURE;
        break;
    }

    SDL_RenderPresent(renderer); // Renderowanie ca³oœci

    /*Uint32 frameTime = SDL_GetTicks() - frameStart;
    Uint32 frameDelayTime = 1000 / fps_t;

    if (frameDelayTime > frameTime)
    {
        SDL_Delay(frameDelayTime - frameTime);
    }*/

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
}