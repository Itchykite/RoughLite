#include "Init.hpp"
#include "Settings.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "EnemyManager.hpp"

#include <iostream>

SDL_AppResult InitEverything(SDL_Renderer*& renderer, SDL_Window*& window, Player*& player, Map*& map, Camera*& camera, EnemyManager*& enemyManager, TTF_Font*& font,
    Uint64& startTime, Uint64& lastTime, void** appstate)
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) // Inicjalizacja wideo SDL
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS, &window, &renderer)) // Inicjalizacja okna i jego wielkoœci SDL
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH);

    SDL_SetRenderViewport(renderer, NULL);

    // Ustawienie jakoœci skalowania tekstur
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

    //SDL_Log("Creating rangeRovers...");
    //rangeRover = new EnemyManager(player, map, camera, renderer); // Tworzenie RangeRovera
    //if (!rangeRover)
    //{
    //    SDL_Log("Couldn't create enemy manager");
    //    return SDL_APP_FAILURE;
    //}

    //for (int i = 0; i < RangeRover::numOfRangeRovers; ++i) // Tworzenie konkretenj iloœci RangeRover'ów
    //{
    //    rangeRover->AddRangeRover();
    //    std::cout << "Added RangeRover " << i << std::endl;
    //}

    SDL_Log("Creating enemies...");
    enemyManager = new EnemyManager(player, map, camera, renderer); // Twrozenie wroga
    if (!enemyManager)
    {
        SDL_Log("Couldn't create enemy manager");
        return SDL_APP_FAILURE;
    }

    SDL_Log("Initialization complete");

    lastTime = SDL_GetTicks(); // Pobranie ostatniego czasu
    startTime = SDL_GetTicks(); // Pobranie czasu rozpoczêcia gry

    mapCollisions(map, renderer); // Kolizja za pomoc¹ kwadratów

    font = TTF_OpenFont("Poppins-Bold.ttf", 24);
}