#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "Settings.hpp"
#include "Player.hpp"
#include "PlayerEventHandling.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "Enemy.hpp"
#include "EnemyManager.hpp"
#include "RangeRover.hpp"

#include <vector>
#include <memory>
#include <iostream>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

Map* map = nullptr;
Camera* camera = nullptr;
Player* player = nullptr;
EnemyManager* enemyManager = nullptr;
EnemyManager* rangeRover = nullptr;

Uint32 lastTime = 0;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) 
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Log("Creating map...");
    map = new Map(mapWidth, mapHeight);
    if (!map->LoadTexture(renderer, "Images/mapa.png"))
    {
        SDL_Log("Couldn't load map texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!map->LoadCollisionSurface("Images/mapa_.png"))
    {
        SDL_Log("Couldn't load collision surface");
        return SDL_APP_FAILURE;
    }

    mapCollisions(map);
    
    SDL_Log("Creating camera...");
    camera = new Camera(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!camera)
    {
        SDL_Log("Couldn't create camera");
        return SDL_APP_FAILURE;
    }
    SDL_Log("Creating player...");

    player = new Player(map, camera, renderer);
    if (!player)
    {
        SDL_Log("Couldn't create player");
        return SDL_APP_FAILURE;
    }

    player->SetPosition(512.0f, 1024.0f);

    SDL_Log("Creating rangeRovers...");
    rangeRover = new EnemyManager(player, map, camera, renderer);
    if (!rangeRover)
    {
        SDL_Log("Couldn't create enemy manager");
        return SDL_APP_FAILURE;
    }

    for (int i = 0; i < RangeRover::numOfRangeRovers; ++i)
    {
        rangeRover->AddRangeRover();
        std::cout << "Added RangeRover " << i << std::endl;
    }

    SDL_Log("Creating enemies...");
    enemyManager = new EnemyManager(player, map, camera, renderer);
    if (!enemyManager)
    {
        SDL_Log("Couldn't create enemy manager");
        return SDL_APP_FAILURE;
    }

    for (int i = 0; i < 0; ++i)
    {
        enemyManager->AddEnemy();
		std::cout << "Added enemy " << i << std::endl;
    }

    SDL_Log("Initialization complete");

    lastTime = SDL_GetTicks();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) 
    {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		if (event->key.key == SDLK_ESCAPE)
		{
			return SDL_APP_SUCCESS;
		}
	}
    
    PlayerEventHandling(event, player);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

    map->Render(renderer, camera->GetX(), camera->GetY());

    player->Update(deltaTime);
    player->Render(renderer);

	rangeRover->UpdateRangeRover(deltaTime);
	rangeRover->RenderRangeRover(renderer);

    enemyManager->Update(deltaTime);
    enemyManager->Render(renderer);

    SDL_FPoint playerPosition = { player->GetX(), player->GetY() };
    int playerX = static_cast<int>(playerPosition.x);
    int playerY = static_cast<int>(playerPosition.y);

    if (!map->IsPixelTransparent(playerX, playerY))
    {
        player->Update(-deltaTime);
    }

	SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    SDL_Log("Cleaning up...");
    delete player;
    delete map;
    delete camera;
    delete enemyManager;
    delete rangeRover;
}