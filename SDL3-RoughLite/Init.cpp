#define GLEW_STATIC
#define SDL_HINT_RENDER_VSYNC "SDL_RENDER_VSYNC"
#include "Init.hpp"
#include "Settings.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "EnemyManager.hpp"

#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL_opengl.h>

SDL_AppResult InitEverything(SDL_Renderer*& renderer, SDL_Window*& window, Player*& player, Map*& map, Camera*& camera, EnemyManager*& enemyManager, TTF_Font*& font, TTF_Font*& bigFont,
    Uint64& startTime, Uint64& lastTime, void** appstate)
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) // Inicjalizacja wideo SDL
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    InitializeSettings();

    window = SDL_CreateWindow("RoughLite", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Tworzenie kontekstu OpenGL
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) 
    {
        SDL_Log("Nie mo¿na utworzyæ kontekstu OpenGL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Ustawiamy kontekst jako aktywny
    SDL_GL_MakeCurrent(window, glContext);

    // W³¹czamy V-Sync (1 = w³¹czony)
    SDL_GL_SetSwapInterval(1);

    SDL_SetWindowFullscreen(window, SDL_GetWindowFullscreenMode);

    //Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;
    //if (vSyncEnabled) // Zmienna okreœlaj¹ca, czy vSync jest w³¹czony
    //{
    //    rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
    //}

    renderer = SDL_CreateRenderer(window, "opengl");
    if (!renderer)
    {
        SDL_Log("Couldn't create renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_OVERSCAN);
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_SetRenderViewport(renderer, NULL);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_GL_MakeCurrent(window, SDL_GL_GetCurrentContext());
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

    LoadAnimationFrames(renderer);

    SDL_Log("Initialization complete");

    lastTime = SDL_GetTicks(); // Pobranie ostatniego czasu
    startTime = SDL_GetTicks(); // Pobranie czasu rozpoczêcia gry

    mapCollisions(map, renderer); // Kolizja za pomoc¹ kwadratów

    font = TTF_OpenFont("Poppins-Bold.ttf", 24);
    bigFont = TTF_OpenFont("Poppins-Bold.ttf", 50);
	
    glewInit();
}