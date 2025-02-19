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

#include <vector>
#include <memory>
#include <iostream>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

Map* map = nullptr; // wskaŸnik mapa
Camera* camera = nullptr; // wskaŸnik kamera
Player* player = nullptr; // wskaŸnik gracz
EnemyManager* enemyManager = nullptr; // wskaŸnik przeciwnik
EnemyManager* rangeRover = nullptr; // wskaŸnik krêc¹cy siê ziut

Uint32 lastTime = 0;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) // Inicjalizacja wideo SDL
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) // Inicjalizacja okna i jego wielkoœci SDL
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    SDL_Log("Creating map...");
    map = new Map(mapWidth, mapHeight); // Tworzenie mapy
    if (!map->LoadTexture(renderer, "Images/mapa.png"))
    {
        SDL_Log("Couldn't load map texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!map->LoadCollisionSurface("Images/mapa_.png")) // Na³orzenie kolizji za pomoc¹ mapy
    {
        SDL_Log("Couldn't load collision surface");
        return SDL_APP_FAILURE;
    }

    mapCollisions(map); // Kolizja za pomoc¹ kwadratów
    
    SDL_Log("Creating camera...");
    camera = new Camera(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Tworzenie kamery wzglêdem okna
    if (!camera)
    {
        SDL_Log("Couldn't create camera");
        return SDL_APP_FAILURE;
    }
    SDL_Log("Creating player...");

    player = new Player(map, camera, renderer); // Tworzenie Gracza
    if (!player)
    {
        SDL_Log("Couldn't create player");
        return SDL_APP_FAILURE;
    }

    player->SetPosition(512.0f, 1024.0f); // Ustawienie pozycji gracza na mapie

    SDL_Log("Creating rangeRovers...");
    rangeRover = new EnemyManager(player, map, camera, renderer); // Tworzenie RangeRovera
    if (!rangeRover)
    {
        SDL_Log("Couldn't create enemy manager");
        return SDL_APP_FAILURE;
    }

    for (int i = 0; i < RangeRover::numOfRangeRovers; ++i) // Tworzenie konkretenj iloœci RangeRover'ów
    {
        rangeRover->AddRangeRover();
        std::cout << "Added RangeRover " << i << std::endl;
    }

    SDL_Log("Creating enemies...");
    enemyManager = new EnemyManager(player, map, camera, renderer); // Twrozenie wroga
    if (!enemyManager)
    {
        SDL_Log("Couldn't create enemy manager");
        return SDL_APP_FAILURE;
    }
    
    SDL_Log("Initialization complete");

    lastTime = SDL_GetTicks(); // Pobranie ostatniego czasu

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
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
    
    PlayerEventHandling(event, player); // Funkcja zawieraj¹ca przechwytywanie WASD do poruszania siê gracza

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    Uint32 currentTime = SDL_GetTicks(); // Pobiera aktualny czas
    float deltaTime = (currentTime - lastTime) / 1000.0f; // Czas co sekunde
    lastTime = currentTime; // Pobiera ostatnio zapisany czas, z przed sekundy

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Koloruje ekran na czarno
	SDL_RenderClear(renderer); // i go renderuje 

    map->Render(renderer, camera->GetX(), camera->GetY()); // Renderowanie mapy

    player->Update(deltaTime); // Aktulizacja gracza co sekunde
    player->Render(renderer); // Renderowanie gracza

	rangeRover->UpdateRangeRover(deltaTime); // Aktualizacja rangeRovera co sekunde
	rangeRover->RenderRangeRover(renderer); // Renderowanie rangeRovera

    enemyManager->Update(deltaTime); // Aktualizacja przeciwnika co sekunde
    enemyManager->Render(renderer); // Renerowanie przeciwników

    SDL_FPoint playerPosition = { player->GetX(), player->GetY() }; // Pobieranie aktualnej pozycji gracza jako punkt
    int playerX = static_cast<int>(playerPosition.x); // przechowanie aktualnego x dla gracza, int
    int playerY = static_cast<int>(playerPosition.y); // przechowanie aktualnego y dla gracza, int

    if (!map->IsPixelTransparent(playerX, playerY)) // sprawdzenie przezroczystoœci
    {
        player->Update(-deltaTime); // Cofniêcie gracza o delta time, tak aby nie móg³ siê ruszaæ
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